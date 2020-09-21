#include"TCP_Server.h"

int RUNNING = 0;
int SOCKET_INIT_ERR = 1;
int SOCKET_BIND_ERR = 2;
int SOCKET_LISTENING_ERR = 3;
int CLOSED = 4;

std::mutex _mx;

TCP_Server::TCP_Server(std::uint16_t port, std::function<void(const Client&, std::mutex&)> handler):
	_port(port), _handler(handler) {}

TCP_Server::~TCP_Server() {
#ifdef _WIN32
	WSACleanup();
#endif
};

int TCP_Server::start() {
#ifdef _WIN32 //Windows
	if (WSAStartup(MAKEWORD(2, 2), &w_data) == 0) {
		printf("WSA Startup succes\n");
	}
	SOCKADDR_IN addres;
	addres.sin_addr.S_un.S_addr = INADDR_ANY;
	addres.sin_port = htons(_port);
	addres.sin_family = AF_INET;
	_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_socket == SOCKET_ERROR) {
		return SOCKET_INIT_ERR;
	}
	if (bind(_server_socket, (struct sockaddr*) & addres, sizeof(addres)) == SOCKET_ERROR) {
		return SOCKET_BIND_ERR;
	}
	if (listen(_server_socket, SOMAXCONN) == SOCKET_ERROR) {
		return SOCKET_LISTENING_ERR;
	}

#else //Linux
	_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_server_socket == -1) {
		return SOCKET_INIT_ERR;
	}
	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(_port);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(_server_socket, (struct sockaddr*) (&sa), sizeof(sa)) == -1) {
		return SOCKET_BIND_ERR;
	}
	if (listen(_server_socket, SOMAXCONN) == -1) {
		return SOCKET_LISTENING_ERR;
	}

#endif

	return RUNNING;
}


void TCP_Server::prosess() {
#ifdef _WIN32
	while (true) {
		SOCKET client_socket;
		SOCKADDR_IN client_addres;
		int addrlen = sizeof(client_addres);
		client_socket = accept(_server_socket, (struct sockaddr*)(&client_addres), &addrlen);

#else
	while (true) {
		int client_socket;
		struct sockaddr_in client_addres;
		int addrlen = sizeof(struct sockaddr_in);
		client_socket = accept(_server_socket, (struct sockaddr*)(&client_addres), (socklen_t*)(&addrlen));
#endif
		if (client_socket != 0) {
			std::cout << "Connected to client."<< std::endl;
			_client_threads.push_back(std::thread(_handler, Client(client_socket, client_addres), std::ref(_mx)));
		}
	}
}


int TCP_Server::stop() {
	shutdown(_server_socket, 0);
#ifdef _WIN32
	closesocket(_server_socket);
#else
	close(_server_socket);
#endif
	for (auto& th : _client_threads) {
		th.join();
	}
	return CLOSED;
};

int Client::recv_data(char* buff, int offset, int size) const {
	if (offset > size) { return 0; }
	int rec = recv(_client_socket, buff + offset, size, 0);
	if (rec == 0) {
		_mx.lock();
		std::cout << "recv return 0. Connection closed" << std::endl;
		_mx.unlock();
	}
	if (rec < 0) {
		_mx.lock();
		std::cout << "recv failed with error" << std::endl;
		_mx.unlock();
	}

	return rec;
};
