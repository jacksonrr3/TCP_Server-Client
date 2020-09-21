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
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#else //Linux

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#endif

#define BUFF_SIZE 1024


class TCP_Client {
	std::uint16_t _port;

#ifdef _WIN32
	SOCKET _socket = INVALID_SOCKET;
	WSAData w_data;
#else
	int _socket;
#endif
public:
	TCP_Client(std::uint16_t port);
	~TCP_Client();

	int connect_to_server(const std::string& host);
	int disconnect();

	bool send_data(const std::string& buffer) const;
};
