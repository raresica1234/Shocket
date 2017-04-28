#include "net/socket.h"

namespace SH {

	Socket::Socket(SOCKET_FAMILY family, SOCKET_TYPE type) {
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = (int)family;
		hints.ai_socktype = (int)type;
		hints.ai_protocol = type == SOCK_STREAM ? IPPROTO_TCP : IPPROTO_UDP;
	}

	Socket::~Socket() {
		closesocket(ConnectionSocket);
	}

	void Socket::connect(const char* ip, const char* port) {
		if (getaddrinfo(ip, port, &hints, &result) != 0) {
			//TODO: handle try-again?
		}
		ConnectionSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

		if (ConnectionSocket == INVALID_SOCKET) {
			//TODO: handle try-again?
			printf("[Shocket] Error connecting: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
		}
	}

}