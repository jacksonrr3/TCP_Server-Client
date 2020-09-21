#include <iostream>
#include <bitset>
#include <string>
#include "TCP_Client.h"


std::string code_func(const std::string& str) {
	std::string res;
	std::size_t str_size = str.size();
	if (str_size <= 127) {
		unsigned char l = str_size;
		res.resize(1);
		res[0] = (char)l;
	}
	else {
		res.resize(1 + sizeof(std::size_t));
		unsigned char * uc = (unsigned char*)(&str_size);
		for (int i = 0; i < sizeof(std::size_t); i++) {
			res[i + 1] = (char)(uc[i]);
		}
	}
	res += str;

	return res;
}


int main(int argc, char** argv) {

	try {
		if (argc < 3) {
			std::cerr << "Usage: client <port> <server>\n";
			return -1;
		}

		TCP_Client client(std::atoi(argv[1]));
		client.connect_to_server(argv[2]);
		std::string msg;
		do	
		{
			std::cout << "Enter message:" << std::endl;
			std::getline(std::cin, msg);
			if (msg == "stop") { 
				std::cout << "Stop sending" << std::endl;
				break; 
			}
			int n = client.send_data(code_func(msg));
			//int n = client.send_data(msg);
			std::cout << "send " << n << " byte" << std::endl;
		} while (true);
		
		client.disconnect();


	}
	catch (std::exception& e) {
		std::cerr << "Exeption: " << e.what() << std::endl;
	}

	return 0;
}
