//
// Created by keith on 22.05.2020.
//

#include "network_server.h"

int main(int argc, char** argv)
{
	ServerNetwork S("127.0.0.1", 34356);
	auto C = S.StandConnection();
	std::map<std::string, std::string> * map;
	//while (true)
	//{
		map = C.RecvMsg();
		for (auto& it : *map)///вывод на экран
		{
			std::cout << it.first << " : " << it.second << std::endl;
		}
		std::cout << "end" << std::endl;
		C.SendMsg(*map);
		auto * F = new OutFile(2349, "/home/keith/recv", "smth.log");
		C.RecvFile(F);
		std::cout << "end" << std::endl;

	//}
	return 0;
}