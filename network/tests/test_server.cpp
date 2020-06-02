//
// Created by keith on 22.05.2020.
//

#include "network_server.h"

int main(int argc, char** argv)
{
	ServerNetwork S("127.0.0.1", 34356);
	std::shared_ptr<std::map<std::string, std::string>> map;

	while (true)
	{
		auto C = S.StandConnection();

		map = C.RecvMsg();
		std::cout << C.GetClientIP() << std::endl;
		if (map == nullptr)
		{
			std::cout << "null" << std::endl;
			continue;
		}
		for (auto& it : *map)///вывод на экран
		{
			std::cout << it.first << " : " << it.second << std::endl;
		}
		std::cout << "end" << std::endl;
		int s = C.SendMsg(map);
		std::cout << s << std::endl;

	}
	return 0;
}