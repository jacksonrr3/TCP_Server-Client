#include <iostream>
#include <cmath>
#include <string>
#include "TCP_Server.h"


/*
void client_handler(const Client& client, std::mutex& _mx) {
	std::size_t data_length = 1;
	std::string msg;
	int res = 0;
	char* buffer = new char [BUFF_SIZE];
	res = client.recv_data(buffer, 0, 1);
	if (res > 0) {
		unsigned char l = (unsigned char)(*buffer);
		if (l & 0b10000000) {
			int size_length = l & 0b01111111;
			res = client.recv_data(buffer, 0, size_length); //добавить проверку 
			for (int i = 0; i < size_length; i++) {
				data_length += (unsigned char)(buffer[i]) * std::pow(256, (size_length - 1 - i));
			}
		}
		else {
			data_length = l & 0b01111111;
		}
	}
	res = 0;
	while (res < data_length) {
		if (int ret = client.recv_data(buffer, 0, BUFF_SIZE) > 0) {
			res += ret;
			msg += std::string(buffer, ret);
		}

		else {
			break;
		}
	}

	_mx.lock();
	std::cout << msg << std::endl;
	_mx.unlock();
}

*/

void client_handler(const Client& client, std::mutex& _mx) {
	char* buffer = new char[BUFF_SIZE];
	int res = client.recv_data(buffer, 0, BUFF_SIZE);
	std::string msg(buffer, res);
	_mx.lock();
	std::cout << msg << std::endl;
	_mx.unlock();
}



int main(int argc, char* argv[])
{
	try {
		if (argc < 2)
		{
			std::cerr << "Usage: server <port>\n";
			return 1;
		}
std::cout << "test _version!" << "1"<< std::endl;
		TCP_Server server(std::atoi(argv[1]), client_handler);
		server.start();
		std::cout << "Start: " << std::endl;
		server.prosess();

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
