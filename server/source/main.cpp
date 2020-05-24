#include <iostream>
#include "server.h"
#include <thread>

using namespace std;

int main()
{
	auto server = new Server("127.0.0.1", 23545);

	thread connectionsLoop(&Server::ConnectionsLoop, server);
	thread queriesLoop(&Server::QueriesLoop, server);
	thread workerLoop(&Server::WorkerLoop, server);

	connectionsLoop.join();
	queriesLoop.join();
	workerLoop.join();

	return 0;
}