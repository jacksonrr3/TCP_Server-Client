#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <vector>

//#include "Constants.h"

#ifdef _WIN32  // Windows
//#include <windows.h>

//#pragma warning(disable: 4996)
#include <WinSock2.h>
//#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#else //Linux

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#endif

#define BUFF_SIZE 1024



class Client {

#ifdef _WIN32 
	SOCKET _client_socket = INVALID_SOCKET;
	SOCKADDR_IN _client_addres;
#else
	int _client_socket;
	struct sockaddr_in _client_addres;
#endif
public:
	Client(SOCKET socket, SOCKADDR_IN addr) :_client_socket(socket), _client_addres(addr) {}
	
	int recv_data(char* buff, int offset = 0, int size = BUFF_SIZE) const;
	//const char* get_data();

	friend class TCP_Server;
};

class TCP_Server {
	std::function<void(const Client&, std::mutex&)> _handler;
	std::vector<std::thread> _client_threads;
//	std::map<int, Client> _clients;
	

	std::uint16_t _port;
#ifdef _WIN32 
	SOCKET _server_socket = INVALID_SOCKET;
	WSAData w_data;
#else
	int _server_socket;
#endif

public:
	TCP_Server(std::uint16_t port, std::function<void(const Client&, std::mutex&)> handler);

	~TCP_Server();

	int start();

	int stop();

	void prosess();


};
