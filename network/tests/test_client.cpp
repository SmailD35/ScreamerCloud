#include <sstream>
#include <iostream>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "network_client.h"

int main(int argc, char **argv)
{
	/*std::map<int, int> map = {{1,2}, {2,1}};
	std::stringstream ss;
	boost::archive::text_oarchive oarch(ss);
	oarch << map;
	std::map<int, int> new_map;
	boost::archive::text_iarchive iarch(ss);
	iarch >> new_map;
	std::cout << (map == new_map) << std::endl;*/

	std::map<std::string, std::string> map = {{"login", "egor"}, {"password", "12345"}};
	std::map<std::string, std::string> * map_new;

	ClientNetwork Client("127.0.0.1", 34356);
	Client.SendMsg(map);
	map_new = Client.RecvMsg();
	for (auto& it : *map_new)///вывод на экран
	{
		std::cout << it.first << " : " << it.second << std::endl;
	}
	std::cout << "end" << std::endl;
	return 0;
}