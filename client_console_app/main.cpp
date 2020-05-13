#include "client_app.h"
using namespace std;

int main(int argc, char** argv)
{
	ClientApp clientApp;
	map <std::string,std::string> clientRequest = clientApp.ParseCmdArguments(argc, argv);
	int error = clientApp.ExecuteRequest(clientRequest);
	return error;
}