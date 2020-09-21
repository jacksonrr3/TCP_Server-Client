#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <algorithm>
#include "TCP_Client.h"


std::string code_func(const std::string& str) {
	std::string res;
	std::vector<unsigned char> v_res;
	std::size_t str_size = str.size();
	if (str_size < 127) {
		unsigned char l = str_size;
		res.resize(1);
		res[0] = (char)l;
	}
	else {

		while (str_size){
			unsigned char un_ch = (unsigned char)(str_size%256);
			str_size /= 256;
			v_res.push_back(un_ch);
		}
		std::reverse(v_res.begin(), v_res.end());
		res.resize(1 + v_res.size());
		res[0] = ((char)(v_res.size()) | 0b1000000);

		for (int i = 0; i < v_res.size(); i++) {
			res[i + 1] = v_res[i];
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
			int n = client.send_data(code_func(std::string(300, 'c')));
			//int n = client.send_data(code_func(msg));
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
