//
// Created by Aleksandr Dergachev on 10.04.2020.
//

#include "client_app.h"
#include "cmd_codes.h"

using namespace std;

ClientApp::ClientApp(string IP, int port)
{
	_clientNetwork = new ClientNetwork(IP, port);
	_file = nullptr;
	ReadConfig();
}

ClientApp::~ClientApp()
{
	delete _clientNetwork;
	delete _file;
}

void ClientApp::ParseCmdArguments(int argc, char** argv)
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("upload,u", po::value<vector<std::string>>()->multitoken(), "upload file")
		("download,d", po::value<vector<std::string>>()->multitoken(), "download file")
		("delete", po::value<fs::path>(), "delete file from cloud storage")
		//("list,l", po::value<std::string>()->default_value(string("/")), "list files in 'arg' directory")
		("register", "register new user")
		("login", "authorize");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout << desc << "\n";
		return;
	}

	if (vm.count("upload")) {
		_filePath = fs::path(vm["upload"].as<vector<string>>()[0]);//vm["upload"].as<vector<fs::path>>()[0];
		fs::path serverPath(vm["upload"].as<vector<string>>()[1]);
		_clientRequest["cmd_code"] = to_string(UPLOAD);
		_clientRequest["file_name"] = serverPath.filename().string();
		_clientRequest["file_directory"] = serverPath.parent_path().string();
		return;
	}

	if (vm.count("download")) {
		fs::path serverPath(vm["download"].as<vector<string>>()[0]);
		_filePath = fs::path(vm["download"].as<vector<string>>()[1]);
		_clientRequest["cmd_code"] = to_string(DOWNLOAD);
		_clientRequest["file_name"] = serverPath.filename().string();
		_clientRequest["file_directory"] = serverPath.parent_path().string();
		return;
	}

	if (vm.count("delete")) {
		fs::path serverPath = vm["delete"].as<fs::path>();
		_clientRequest["cmd_code"] = to_string(DELETE);
		_clientRequest["file_name"] = serverPath.filename().string();
		_clientRequest["file_directory"] = serverPath.parent_path().string();
		return;
	}

	if (vm.count("list")) {
		_clientRequest["cmd_code"] = to_string(LIST);
		_clientRequest["path"] = vm["list"].as<std::string>();
		return;
	}

	if (vm.count("register")) {
		_clientRequest["cmd_code"] = to_string(REGISTER);
		return;
	}

	if (vm.count("login")) {
		_clientRequest["cmd_code"] = to_string(LOGIN);
		return;
	}
}

int ClientApp::ExecuteRequest()
{
	if (!_clientRequest.count("cmd_code")) return -3;

	switch (stoi(_clientRequest["cmd_code"]))
	{
	case UPLOAD:
		return UploadFile();
	case DOWNLOAD:
		return DownloadFile();
	case DELETE:
		return DeleteFile();
	case LIST:
		return List();
	case REGISTER:
		return RegisterUser();
	case LOGIN:
		return LoginUser();
	default:
		return -2;
	}
}

int ClientApp::UploadFile()
{
	// TODO: добавить проверку на существование файла
	if (!_user.IsLoggedIn())
	{
		cout << "You are not logged in" << endl;
		return -1;
	}
	_file = new InFile(_filePath.string());
	_clientRequest["username"] = _user.login;
	_clientRequest["password"] = _user.password;
	_clientRequest["file_size"] = to_string(_file->GetSize());
	_clientRequest["error_code"] = "0";

	Request();
	if (ValidateResponse())
	{
		cout << "Uploading file...\n";
		thread progressBar(&ClientApp::PrintProgress, this, consoleWidth);
		_clientNetwork->SendFile(dynamic_cast<InFile*>(_file));
		progressBar.join();
		cout << "Uploading success!\n";
		return 0;
	}
	return -1;
}

int ClientApp::DownloadFile()
{
	if (!_user.IsLoggedIn())
	{
		cout << "You are not logged in" << endl;
		return -1;
	}
	_clientRequest["username"] = _user.login;
	_clientRequest["password"] = _user.password;
	_clientRequest["error_code"] = "0";

	Request();
	if (ValidateResponse())
	{
		stringstream sstream(_serverResponse["file_size"]);
		size_t size = 0;
		sstream >> size;
		_file = new OutFile(size, _filePath.parent_path().string(), _filePath.filename().string());
		cout << "Downloading file...\n";
		thread progressBar(&ClientApp::PrintProgress, this, consoleWidth);
		_clientNetwork->RecvFile(dynamic_cast<OutFile*>(_file));
		progressBar.join();
		cout << "Download success!\n";
		return 0;
	}
	return -1;
}

int ClientApp::DeleteFile()
{
	if (!_user.IsLoggedIn())
	{
		cout << "You are not logged in" << endl;
		return -1;
	}
	_clientRequest["username"] = _user.login;
	_clientRequest["password"] = _user.password;
	_clientRequest["error_code"] = "0";

	Request();
	if (ValidateResponse())
	{
		cout << "File deleted successfully" << endl;
		return 0;
	}
	else
	{
		cout << "Delete file failed" << endl;
		return -1;
	}
}

int ClientApp::List()
{
	Request();
	ValidateResponse();
	// TODO:: вывод списка файлов
}

int ClientApp::RegisterUser()
{
	string username;
	string password;
	cout << "Enter username: ";
	cin >> username;
	cout << endl << "Enter password: ";
	cin >> password;
	cout << endl;

	_clientRequest["username"] = username;
	_clientRequest["password"] = password;
	_clientRequest["error_code"] = "0";

	Request();
	if (ValidateResponse())
	{
		cout << "Registration success\n";
		return 0;
	}
	else
	{
		cout << "Registration failed\n";
		return -1;
	}
}

int ClientApp::LoginUser()
{
	string username;
	string password;
	cout << "Enter username: ";
	cin >> username;
	cout << endl << "Enter password: ";
	cin >> password;
	cout << endl;

	_clientRequest["username"] = username;
	_clientRequest["password"] = password;
	_clientRequest["error_code"] = "0";

	Request();
	if (ValidateResponse())
	{
		cout << "Login success\n";
		return 0;
	}
	else
	{
		cout << "Login failed\n";
		return -1;
	}
}

void ClientApp::Request()
{
	_clientNetwork->SendMsg(_clientRequest);
	_serverResponse = std::move(*_clientNetwork->RecvMsg());
}

bool ClientApp::ValidateResponse()
{
	if (!_serverResponse.count("cmd_code"))
		return false;
	if (_serverResponse["cmd_code"] != _clientRequest["cmd_code"])
		return false;
	if (!_serverResponse.count("error_code"))
		return false;
	if (_serverResponse["error_code"] == "0")
		return true;

	return false;
}

void ClientApp::PrintProgress(int outputWidth)
{
	int progress = 0;
	while (_file->GetProgress() < 100)
	{
		progress = (float(_file->GetProgress()) / 100) * outputWidth;
		cout << string(outputWidth + 5, '\b');
		cout << string(progress, '#') << string(outputWidth - progress, '_') << ' ' << _file->GetProgress() << '%';
		cout.flush();
		this_thread::sleep_for(chrono::milliseconds(500));
	}
	cout << endl;
}

void ClientApp::ReadConfig(string configPath)
{
	pt::ptree root;
	pt::read_json(configPath, root);

	string  info;

	for (pt::ptree::value_type &values : root.get_child("user"))
	{
		if (values.first == "username")
			_user.login = values.second.data();
		if (values.first == "password")
			_user.password = values.second.data();
	}
}

bool User::IsLoggedIn()
{
	if (login != "" && password != "")
		return true;
	else
		return false;
}