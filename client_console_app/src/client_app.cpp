//
// Created by Aleksandr Dergachev on 10.04.2020.
//

#include "client_app.h"

using namespace std;

ClientApp::ClientApp()
{

}

ClientApp::~ClientApp()
{

}

void ClientApp::ParseCmdArguments(int argc, char** argv)
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("upload,u", po::value<vector<std::string>>()->multitoken(), "upload file")
		("download,d", po::value<vector<std::string>>()->multitoken(), "download file")
		("delete", po::value<std::string>(), "delete file from cloud storage")
		("list,l", po::value<std::string>()->default_value(string("/")), "list files in 'arg' directory")
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
		_clientRequest["cmd_code"] = to_string(UPLOAD_CLI);
		_clientRequest["file_name"] = vm["upload"].as<vector<std::string>>()[0];
		_clientRequest["upload_directory"] = vm["upload"].as<vector<std::string>>()[1];
		return;
	}

	if (vm.count("download")) {
		_clientRequest["cmd_code"] = to_string(DOWNLOAD_CLI);
		_clientRequest["file_name"] = vm["download"].as<vector<std::string>>()[0];
		_clientRequest["download_directory"] = vm["download"].as<vector<std::string>>()[1];
		return;
	}

	if (vm.count("delete")) {
		_clientRequest["cmd_code"] = to_string(DELETE_CLI);
		_clientRequest["file_name"] = vm["delete"].as<std::string>();
		return;
	}

	if (vm.count("list")) {
		_clientRequest["cmd_code"] = to_string(LIST_CLI);
		_clientRequest["path"] = vm["list"].as<std::string>();
		return;
	}

	if (vm.count("register")) {
		_clientRequest["cmd_code"] = to_string(REGISTER_CLI);
		return;
	}

	if (vm.count("login")) {
		_clientRequest["cmd_code"] = to_string(LOGIN_CLI);
		return;
	}
}

int ClientApp::ExecuteRequest()
{
	if (!_clientRequest.count("cmd_code")) return -3;

	switch (stoi(_clientRequest["cmd_code"]))
	{
	case UPLOAD_CLI:
		return UploadFile();
	case DOWNLOAD_CLI:
		return DownloadFile();
	case DELETE_CLI:
		return DeleteFile();
	case LIST_CLI:
		return List();
	case REGISTER_CLI:
		return RegisterUser();
	case LOGIN_CLI:
		return LoginUser();
	default:
		return -2;
	}
}

int ClientApp::UploadFile()
{
	Request();
	if (ValidateResponse())
	{
		cout << "Uploading file...\n";
		thread progressBar(&ClientApp::PrintProgress, this, consoleWidth);
		_clientNetwork->SendFile(_file);
		progressBar.join();
		return 0;
	}
		return -1;
}

int ClientApp::DownloadFile()
{
	Request();
	if (ValidateResponse())
	{
		cout << "Downloading file...\n";
		thread progressBar(&ClientApp::PrintProgress, this, consoleWidth);
		_clientNetwork->RecvFile(&_file);
		progressBar.join();
		return 0;
	}
	return -1;
}

int ClientApp::DeleteFile()
{
	Request();
	if (!ValidateResponse())
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

	Request();
	if (!ValidateResponse())
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

	Request();
	if (!ValidateResponse())
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
	_serverResponse = _clientNetwork->RecvMsg();
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
	while (_file.GetProgress() < 100)
	{
		progress = (float(_file.GetProgress()) / 100) * outputWidth;
		cout << string(outputWidth + 5, '\b');
		cout << string(progress, '#') << string(outputWidth - progress, '_') << ' ' << _file.GetProgress() << '%';
		cout.flush();
		this_thread::sleep_for(chrono::milliseconds(500));
	}
	cout << endl;
}