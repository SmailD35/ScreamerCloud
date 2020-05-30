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
	std::string login;
	std::string password;
};

class ClientApp
{
 public:
	explicit ClientApp(std::string IP = "127.0.0.1", int port = 23545);
	~ClientApp() = default;
	std::shared_ptr<std::map<std::string, std::string>> ParseCmdArguments(int argc, char** argv);
	int ExecuteRequest(const std::shared_ptr<std::map<std::string, std::string>> &request);
 private:
	int UploadFile(const std::shared_ptr<std::map<std::string, std::string>> &request);
	int DownloadFile(const std::shared_ptr<std::map<std::string, std::string>> &request);
	int DeleteFile(const std::shared_ptr<std::map<std::string, std::string>> &request);
	int DeleteUser(const std::shared_ptr<std::map<std::string, std::string>> &request);
	int List(const std::shared_ptr<std::map<std::string, std::string>> &request);
	int RegisterUser(const std::shared_ptr<std::map<std::string, std::string>> &request);
	int LoginUser(const std::shared_ptr<std::map<std::string, std::string>> &request);
	std::shared_ptr<std::map<std::string, std::string>> Request(const std::shared_ptr<std::map<std::string, std::string>> &request);
	bool ValidateResponse(const std::shared_ptr<std::map<std::string, std::string>> &response, int cmd_code);
	void PrintProgress(const std::shared_ptr<File> &file, int outputWidth = 50);
	void PrintFileList(const std::shared_ptr<std::map<std::string, std::string>> &list);
	void ReadUserConfig(std::string configPath = "../client_conf.json");
	void WriteUserConfig(std::string configPath = "../client_conf.json");

 private:
	User _user;
	std::shared_ptr<ClientNetwork> _clientNetwork;
};

#endif //CLIENT_CONSOLE_APP__CLIENT_APP_H_