#include "client_app.h"
using namespace std;

int main(int argc, char** argv)
{
	ClientApp clientApp;
	clientApp.ParseCmdArguments(argc, argv);
	int error = clientApp.ExecuteRequest();
	return error;
}