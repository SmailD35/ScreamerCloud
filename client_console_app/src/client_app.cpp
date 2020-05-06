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
		("upload", po::value<std::string>(), "upload file")
		("download", po::value<std::string>(), "download file")
		("delete", po::value<std::string>(), "delete file from cloud storage")
		("list,l", po::value<std::string>()->default_value(string("/")), "list files in 'arg' directory")
		("register", "register new user")
		("login", "login user");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout << desc << "\n";
		return;
	}

	if (vm.count("upload")) {
		_clientRequest.cmdCode = UPLOAD_CLI;
		_clientRequest.requestData["file_name"] = vm["upload"].as<std::string>();
		return;
	}

	if (vm.count("download")) {
		_clientRequest.cmdCode = DOWNLOAD_CLI;
		_clientRequest.requestData["file_name"] = vm["download"].as<std::string>();
		return;
	}

	if (vm.count("delete")) {
		_clientRequest.cmdCode = DELETE_CLI;
		_clientRequest.requestData["file_name"] = vm["delete"].as<std::string>();
		return;
	}

	if (vm.count("list")) {
		_clientRequest.cmdCode = LIST_CLI;
		_clientRequest.requestData["path"] = vm["list"].as<std::string>();
		return;
	}

	if (vm.count("register")) {
		_clientRequest.cmdCode = REGISTER_CLI;
		return;
	}

	if (vm.count("login")) {
		_clientRequest.cmdCode = LOGIN_CLI;
		return;
	}
}

int ClientApp::ExecuteRequest()
{
	if (!_clientRequest.cmdCode) return 0;
	switch (_clientRequest.cmdCode)
	{
	case UPLOAD_CLI:
		return UploadFile(_clientRequest.requestData["file_name"]);
	case DOWNLOAD_CLI:
		if(_clientRequest.requestData.count("download_path"))
			return DownloadFile(_clientRequest.requestData["file_name"],
				_clientRequest.requestData["download_path"]);
		else
			return DownloadFile(_clientRequest.requestData["file_name"]);
	case DELETE_CLI:
		return DeleteFile(_clientRequest.requestData["file_name"]);
	case LIST_CLI:
		return ListDirectory(_clientRequest.requestData["path"]);
	case REGISTER_CLI:
		return RegisterUser();
	case LOGIN_CLI:
		return LoginUser();
	default:
		return -2;
	}
}

int ClientApp::UploadFile(const std::string &file_name)
{
	_clientNetwork->SendMsg(_clientRequest.requestData);
	map<string,string> receivedMsg;
	receivedMsg = _clientNetwork->RecvMsg();
	if (!receivedMsg.count("cmd_code"))
		return 1;
	if (receivedMsg["cmd_code"] != UPLOAD_SRV)
		return 2;
	if (!receivedMsg.count("error_code"))
		return 3;
	if (receivedMsg["error_code"] == "0")
		_clientNetwork->SendFile(_file);
	else
		return 4;
	return 0;
}

int ClientApp::DownloadFile(const std::string &file_name)
{
	_clientNetwork->SendMsg(_clientRequest.requestData);
	map<string,string> receivedMsg;
	receivedMsg = _clientNetwork->RecvMsg();
	if (!receivedMsg.count("cmd_code"))
		return 1;
	if (receivedMsg["cmd_code"] != DOWNLOAD_SRV)
		return 2;
	if (!receivedMsg.count("error_code"))
		return 3;
	if (receivedMsg["error_code"] == "0")
		_clientNetwork->RecvFile(&_file);
	else
		return 4;
	return 0;
}

int ClientApp::DownloadFile(const std::string &file_name, const std::string &download_path)
{
	cout << "Download file: " << file_name << " in " << download_path << " directory\n";
	return 0;
}

int ClientApp::DeleteFile(const std::string &file_name)
{
	_clientNetwork->SendMsg(_clientRequest.requestData);
	map<string,string> receivedMsg;
	receivedMsg = _clientNetwork->RecvMsg();
	if (!receivedMsg.count("cmd_code"))
		return 1;
	if (receivedMsg["cmd_code"] != DELETE_SRV)
		return 2;
	if (!receivedMsg.count("error_code"))
		return 3;
	if (receivedMsg["error_code"] == "0")
		return 0;
	else
		return 4;
}

int ClientApp::ListAll()
{
	_clientNetwork->SendMsg(_clientRequest.requestData);
	map<string,string> receivedMsg;
	receivedMsg = _clientNetwork->RecvMsg();
	if (!receivedMsg.count("cmd_code"))
		return 1;
	if (receivedMsg["cmd_code"] != LIST_SRV)
		return 2;
	if (!receivedMsg.count("error_code"))
		return 3;
	if (receivedMsg["error_code"] == "0")
		return 0;
	else
		return 4;
}

int ClientApp::ListDirectory(const std::string &directory_path)
{
	cout << "List directory: " << directory_path << "\n";
	return 0;
}

int ClientApp::RegisterUser()
{
	cout << "Registration process\n";
	return 0;
}

int ClientApp::LoginUser()
{
	cout << "Login process\n";
	return 0;
}

int ClientApp::Help()
{
	cout << "help screamer\n";
	return 0;
}

int ClientApp::Help(const std::string &help_topic)
{
	cout << "help " << help_topic << "\n";
	return 0;
}