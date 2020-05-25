//
// Created by Aleksandr Dergachev on 10.04.2020.
//

#ifndef CLIENT_CONSOLE_APP__CLIENT_APP_H_
#define CLIENT_CONSOLE_APP__CLIENT_APP_H_

#include <map>
#include <thread>
#include <chrono>
#include <boost/program_options.hpp>
#include "file.h"
#include "network_client.h"

namespace po = boost::program_options;

const int consoleWidth = 50;

struct User
{
	std::string login;
	std::string password;
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
	int UploadFile();
	int DownloadFile();
	int DeleteFile();
	int List();
	int RegisterUser();
	int LoginUser();
	void Request();
	bool ValidateResponse();
	void PrintProgress(int outputWidth);

 private:
	std::string _currentDirectory;
	User _user;
	File* _file;
	std::map <std::string,std::string> _clientRequest;
	std::map <std::string,std::string> _serverResponse;
	ClientNetwork* _clientNetwork;
};

#endif //CLIENT_CONSOLE_APP__CLIENT_APP_H_