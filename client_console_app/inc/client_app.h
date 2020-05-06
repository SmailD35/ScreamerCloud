//
// Created by Aleksandr Dergachev on 10.04.2020.
//

#ifndef CLIENT_CONSOLE_APP__CLIENT_APP_H_
#define CLIENT_CONSOLE_APP__CLIENT_APP_H_

#include <map>
#include <boost/program_options.hpp>
#include "file.h"
#include "client_network_mock.h"

namespace po = boost::program_options;

enum CmdCodeClient { UPLOAD_CLI, DOWNLOAD_CLI, DELETE_CLI, LIST_CLI, REGISTER_CLI, LOGIN_CLI };

struct User
{
	std::string login;
	std::string password;
};

struct ClientRequest
{
	int cmdCode = 0;
	std::map <std::string,std::string> requestData;
};

class ClientApp
{
 public:
	ClientApp();
	ClientApp(ClientNetwork* clientNetwork) : _clientNetwork(clientNetwork) {};
	~ClientApp();
	void ParseCmdArguments(int argc, char** argv);
	int ExecuteRequest();
 private:
	int UploadFile(const std::string &file_name);
	int DownloadFile(const std::string &file_name);
	int DownloadFile(const std::string &file_name, const std::string &download_path);
	int DeleteFile(const std::string &file_name);
	int ListAll();
	int ListDirectory(const std::string &directory_path);
	int RegisterUser();
	int LoginUser();
	int Help();
	int Help(const std::string &help_topic);
 private:
	std::string _currentDirectory;
	User _user;
	File _file;
	ClientRequest _clientRequest;
	ClientNetwork* _clientNetwork;
};

#endif //CLIENT_CONSOLE_APP__CLIENT_APP_H_