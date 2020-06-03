#include "server.h"
#include <thread>

using namespace std;

int main()
{
	auto server = make_shared<Server>();

	thread connectionsLoop(&Server::ConnectionsLoop, server);
	thread queriesLoop(&Server::QueriesLoop, server);
	thread workerLoop(&Server::WorkerLoop, server);

	connectionsLoop.join();
	queriesLoop.join();
	workerLoop.join();

	return 0;
}