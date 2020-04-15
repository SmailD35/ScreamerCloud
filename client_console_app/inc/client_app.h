//
// Created by Aleksandr Dergachev on 10.04.2020.
//

#ifndef CLIENT_CONSOLE_APP__CLIENT_APP_H_
#define CLIENT_CONSOLE_APP__CLIENT_APP_H_

#include <iostream>
#include <map>
#include "file.h"
#include "client_network_mock.h"

//cmd codes
#define HELP_CLI 		1
#define UPLOAD_CLI 		2
#define DOWNLOAD_CLI 	3
#define DELETE_CLI 		4
#define LIST_CLI 		5
#define REGISTER_CLI 	6
#define LOGIN_CLI 		7

using namespace std;

struct User
{
	string login;
	string password;
};

struct ClientRequest
{
	int cmdCode = 0;
	map <string,string> requestData;
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
	string _currentDirectory;
	User _user;
	File _file;
	ClientRequest _clientRequest;
	ClientNetwork* _clientNetwork;
	int UploadFile(string file_name);
	int DownloadFile(string file_name);
	int DownloadFile(string file_name, string download_path);
	int DeleteFile(string file_name);
	int ListAll();
	int ListDirectory(string directory_path);
	int RegisterUser();
	int LoginUser();
	int Help();
	int Help(string help_topic);
};

#endif //CLIENT_CONSOLE_APP__CLIENT_APP_H_