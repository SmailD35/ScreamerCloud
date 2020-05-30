//
// Created by Aleksandr Dergachev on 10.04.2020.
//

#include "client_app.h"
#include "cmd_codes.h"

using namespace std;

ClientApp::ClientApp(string IP, int port)
{
	_clientNetwork = make_shared<ClientNetwork>(IP, port);//new ClientNetwork(IP, port);
	_clientNetwork->Connection();
}

shared_ptr<map<string, string>> ClientApp::ParseCmdArguments(int argc, char** argv)
{
	map<string, string> clientRequest;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("upload,u", po::value<vector<string>>()->multitoken(), "upload file")
		("download,d", po::value<vector<string>>()->multitoken(), "download file")
		("delete", po::value<string>(), "delete file from cloud storage")
		("delete-user", "delete user and all his files from cloud storage")
		("list,l", po::value<string>(), "list files in 'arg' directory")
		("register", "register new user")
		("login", "authorize");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout << desc << "\n";
		return make_shared<map<string, string>>(clientRequest);
	}

	if (vm.count("upload")) {
		fs::path serverPath(vm["upload"].as<vector<string>>()[1]);
		clientRequest["cmd_code"] = to_string(UPLOAD);
		clientRequest["file_name"] = serverPath.filename().string();
		clientRequest["file_directory"] = serverPath.parent_path().string();
		clientRequest["client_file_path"] = vm["upload"].as<vector<string>>()[0];
		return make_shared<map<string, string>>(clientRequest);
	}

	if (vm.count("download")) {
		fs::path serverPath(vm["download"].as<vector<string>>()[0]);
		clientRequest["cmd_code"] = to_string(DOWNLOAD);
		clientRequest["file_name"] = serverPath.filename().string();
		clientRequest["file_directory"] = serverPath.parent_path().string();
		clientRequest["client_file_path"] = vm["download"].as<vector<string>>()[1];
		return make_shared<map<string, string>>(clientRequest);
	}

	if (vm.count("delete")) {
		fs::path serverPath(vm["delete"].as<string>());
		clientRequest["cmd_code"] = to_string(DELETE_FILE);
		clientRequest["file_name"] = serverPath.filename().string();
		clientRequest["file_directory"] = serverPath.parent_path().string();
		return make_shared<map<string, string>>(clientRequest);
	}

	if (vm.count("delete-user")) {
		clientRequest["cmd_code"] = to_string(DELETE_USER);
		return make_shared<map<string, string>>(clientRequest);
	}

	if (vm.count("list")) {
		clientRequest["cmd_code"] = to_string(LIST);
		clientRequest["directory"] = vm["list"].as<std::string>();
		return make_shared<map<string, string>>(clientRequest);
	}

	if (vm.count("register")) {
		clientRequest["cmd_code"] = to_string(REGISTER);
		return make_shared<map<string, string>>(clientRequest);
	}

	if (vm.count("login")) {
		clientRequest["cmd_code"] = to_string(LOGIN);
		return make_shared<map<string, string>>(clientRequest);
	}

	return nullptr;
}

int ClientApp::ExecuteRequest(const shared_ptr<map<string, string>> &request)
{
	if (!request->count("cmd_code")) return -3;

	switch (stoi(request->at("cmd_code")))
	{
	case UPLOAD:
		return UploadFile(request);
	case DOWNLOAD:
		return DownloadFile(request);
	case DELETE_FILE:
		return DeleteFile(request);
	case DELETE_USER:
		return DeleteUser(request);
	case LIST:
		return List(request);
	case REGISTER:
		return RegisterUser(request);
	case LOGIN:
		return LoginUser(request);
	default:
		return -2;
	}
}

int ClientApp::UploadFile(const shared_ptr<map<string, string>> &request)
{
	// TODO: добавить проверку на существование файла
	if (!_user.IsLoggedIn())
	{
		cout << "You are not logged in" << endl;
		return -1;
	}

	fs::path path(request->at("client_file_path"));
	request->erase("client_file_path");
	auto inFile = make_shared<InFile>(path.string());//InFile(_filePath.string());

	request->at("username") = _user.login;
	request->at("password") = _user.password;
	request->at("file_size") = to_string(inFile->GetSize());
	request->at("error_code") = "0";

	shared_ptr<map<string, string>> response = Request(request);
	if (ValidateResponse(response, UPLOAD))
	{
		cout << "Uploading file...\n";
		thread progressBar(&ClientApp::PrintProgress, this, inFile, consoleWidth);
		_clientNetwork->SendFile(inFile);
		progressBar.join();
		cout << "Upload success\n";
		return 0;
	}
	return -1;
}

int ClientApp::DownloadFile(const shared_ptr<map<string, string>> &request)
{
	if (!_user.IsLoggedIn())
	{
		cout << "You are not logged in" << endl;
		return -1;
	}
	request->at("username") = _user.login;
	request->at("password") = _user.password;
	request->at("error_code") = "0";
	fs::path path(request->at("client_file_path"));
	request->erase("client_file_path");

	shared_ptr<map<string, string>> response = Request(request);
	if (ValidateResponse(response, DOWNLOAD))
	{
		stringstream sstream(response->at("file_size"));
		size_t size = 0;
		sstream >> size;
		auto outFile = make_shared<OutFile>(size, path.parent_path().string(), path.filename().string()); //OutFile(size, _filePath.parent_path().string(), _filePath.filename().string());
		cout << "Downloading file...\n";
		thread progressBar(&ClientApp::PrintProgress, this, outFile, consoleWidth);
		_clientNetwork->RecvFile(outFile);
		progressBar.join();
		cout << "Download success\n";
		return 0;
	}
	return -1;
}

int ClientApp::DeleteFile(const shared_ptr<map<string, string>> &request)
{
	if (!_user.IsLoggedIn())
	{
		cout << "You are not logged in" << endl;
		return -1;
	}
	request->at("username") = _user.login;
	request->at("password") = _user.password;
	request->at("error_code") = "0";

	shared_ptr<map<string, string>> response = Request(request);
	if (ValidateResponse(response, DELETE_FILE))
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

int ClientApp::DeleteUser(const shared_ptr<map<string, string>> &request)
{
	if (!_user.IsLoggedIn())
	{
		cout << "You are not logged in" << endl;
		return -1;
	}
	request->at("username") = _user.login;
	request->at("password") = _user.password;
	request->at("error_code") = "0";

	shared_ptr<map<string, string>> response = Request(request);
	if (ValidateResponse(response, DELETE_USER))
	{
		cout << "User deleted successfully" << endl;
		return 0;
	}
	else
	{
		cout << "Delete user failed" << endl;
		return -1;
	}
}

int ClientApp::List(const shared_ptr<map<string, string>> &request)
{
	if (!_user.IsLoggedIn())
	{
		cout << "You are not logged in" << endl;
		return -1;
	}
	request->at("username") = _user.login;
	request->at("password") = _user.password;
	request->at("error_code") = "0";

	shared_ptr<map<string, string>> response = Request(request);
	if (ValidateResponse(response, LIST))
	{
		shared_ptr<map<string, string>> list = _clientNetwork->RecvMsg();
		PrintFileList(list);
	}

	return 0;
}

int ClientApp::RegisterUser(const shared_ptr<map<string, string>> &request)
{
	string username;
	string password;
	cout << "Enter username: ";
	cin >> username;
	cout << endl << "Enter password: ";
	cin >> password;
	cout << endl;

	_user.login = username;
	_user.password = password;

	request->at("username") = username;
	request->at("password") = password;
	request->at("error_code") = "0";

	shared_ptr<map<string, string>> response = Request(request);
	if (ValidateResponse(response, REGISTER))
	{
		cout << "Registration success\n";
		WriteUserConfig();
		return 0;
	}
	else
	{
		cout << "Registration failed\n";
		return -1;
	}
}

int ClientApp::LoginUser(const shared_ptr<map<string, string>> &request)
{
	string username;
	string password;
	cout << "Enter username: ";
	cin >> username;
	cout << endl << "Enter password: ";
	cin >> password;
	cout << endl;

	_user.login = username;
	_user.password = password;

	request->at("username") = username;
	request->at("password") = password;
	request->at("cmd_code") = to_string(LOGIN);
	request->at("error_code") = "0";

	shared_ptr<map<string, string>> response = Request(request);
	if (ValidateResponse(response, LOGIN))
	{
		cout << "Login success\n";
		WriteUserConfig();
		return 0;
	}
	else
	{
		cout << "Login failed\n";
		return -1;
	}
}

shared_ptr<map<string, string>> ClientApp::Request(const shared_ptr<map<string, string>> &request)
{
	_clientNetwork->SendMsg(request);
	return _clientNetwork->RecvMsg();
}

bool ClientApp::ValidateResponse(const shared_ptr<map<string, string>> &response, int cmd_code)
{
	if (!response->count("cmd_code"))
		return false;
	if (response->at("cmd_code") != to_string(cmd_code))
		return false;
	if (!response->count("error_code"))
		return false;
	if (response->at("error_code") == "0")
		return true;

	return false;
}

void ClientApp::PrintProgress(const shared_ptr<File> &file, int outputWidth)
{
	while (file->GetProgress() < 100)
	{
		int progress = (float(file->GetProgress()) / 100) * outputWidth;
		cout << string(outputWidth + 5, '\b');
		string downloadedString(progress, '#');
		string leftString(outputWidth - progress, '_');
		cout << downloadedString << leftString << ' ' << file->GetProgress() << '%';
		cout.flush();
		this_thread::sleep_for(chrono::milliseconds(500));
	}
	cout << string(outputWidth + 5, '\b');
	cout << string(outputWidth, '#')  << " 100%";
	cout << endl;
}

void ClientApp::PrintFileList(const std::shared_ptr<std::map<std::string, std::string>> &list)
{
	for (auto const& x : *list)
	{
		if (x.second == "dir")
			cout << x.first << '/' <<endl;
		else
			cout << x.first << endl;
	}
}

void ClientApp::ReadUserConfig(string configPath)
{
	pt::ptree root;
	try
	{
		pt::read_json(configPath, root);
		string info;
		for (pt::ptree::value_type& values : root.get_child("user"))
		{
			if (values.first == "username")
				_user.login = values.second.data();
			if (values.first == "password")
				_user.password = values.second.data();
		}
	}
	catch (std::exception &exc)
	{
		map<string, string> clientRequest;
		clientRequest["cmd_code"] = to_string(LOGIN);
		LoginUser(make_shared<map<string, string>>(clientRequest));
	}
}

void ClientApp::WriteUserConfig(string configPath)
{
	pt::ptree root;

	string info;
	root.put("user.username", _user.login);
	root.put("user.password", _user.password);

	pt::write_json(configPath, root);
	exit(0);
}

bool User::IsLoggedIn()
{
	return !login.empty() && !password.empty();
}