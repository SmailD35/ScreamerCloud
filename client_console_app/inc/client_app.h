//
// Created by Aleksandr Dergachev on 10.04.2020.
//

#ifndef CLIENT_CONSOLE_APP__CLIENT_APP_H_
#define CLIENT_CONSOLE_APP__CLIENT_APP_H_

#include <map>
#include <thread>
#include <chrono>
#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include "file.h"
#include "network_client.h"

namespace po = boost::program_options;
namespace pt = boost::property_tree;
namespace fs = boost::filesystem;

const int consoleWidth = 50;

struct User
{
	bool IsLoggedIn();
	std::string login = "";
	std::string password = "";
};

class ClientApp
{
 public:
	explicit ClientApp(std::string IP = "127.0.0.1", int port = 23545);
	~ClientApp();
	void ParseCmdArguments(int argc, char** argv);
	int ExecuteRequest();
 private:
	int UploadFile();
	int DownloadFile();
	int DeleteFile();
	int DeleteUser();
	int List();
	int RegisterUser();
	int LoginUser();
	void Request();
	bool ValidateResponse();
	void PrintProgress(int outputWidth = 50);
	void PrintFileList(std::map<std::string, std::string> list);
	void ReadUserConfig(std::string configPath = "../client_conf.json");
	void WriteUserConfig(std::string configPath = "../client_conf.json");

 private:
	std::string _currentDirectory;
	User _user;
	File* _file;
	fs::path _filePath;
	std::map <std::string,std::string> _clientRequest;
	std::map <std::string,std::string> _serverResponse;
	ClientNetwork* _clientNetwork;
};

#endif //CLIENT_CONSOLE_APP__CLIENT_APP_H_