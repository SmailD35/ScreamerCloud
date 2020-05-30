#include "client_app.h"
using namespace std;

int main(int argc, char** argv)
{
	ClientApp clientApp;
	shared_ptr<map<string, string>> request = clientApp.ParseCmdArguments(argc, argv);
	int error = clientApp.ExecuteRequest(request);
	return error;
}