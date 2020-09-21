#include "TCP_Client.h"

int CONNECTED = 0;
int SOCKET_INIT_ERR = 1;
int SOCKET_CONNECT_ERR = 2;
int DISCONNECT = 3;

TCP_Client::TCP_Client(std::uint16_t port) :_port(port) {}
TCP_Client::~TCP_Client() {
}

int TCP_Client::connect_to_server(const std::string& host) {
#ifdef _WIN32
	if (WSAStartup(MAKEWORD(2, 2), &w_data) == 0) {
		printf("WSA Startup succes\n");
	}
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (_socket == INVALID_SOCKET) {
		return SOCKET_INIT_ERR;
	}
	sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.S_un.S_addr = inet_addr(host.c_str()); 
	serv_addr.sin_addr.S_un.S_addr = InetPton(AF_INET, (PCWSTR)(host.c_str()), &serv_addr.sin_addr);
	serv_addr.sin_port = htons(_port);

#else
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1) {
		return SOCKET_INIT_ERR;
	}
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(_port);
	serv_addr.sin_addr.s_addr = inet_addr(host.c_str());
#endif
	if (connect(_socket, (sockaddr*)(&serv_addr), sizeof(serv_addr)) !< 0) {
		return SOCKET_CONNECT_ERR;
	}

	return CONNECTED;
}


int TCP_Client::disconnect() {
#ifdef _WIN32
	closesocket(_socket);
	WSACleanup();
#else
	shutdown(_socket, 0);
	close(_socket);
#endif
	return DISCONNECT;
}

bool TCP_Client::send_data(const std::string& buffer) const {
	int res = send(_socket, buffer.c_str(), buffer.size(), 0);
	return (res > 0);
}
