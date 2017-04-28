#pragma once

#include <stdio.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include <sh.h>

namespace SH {

enum SHAPI SOCKET_FAMILY {
	IPv4 = AF_INET,
	IPv6 = AF_INET6
};

enum SHAPI SOCKET_TYPE {
	TCP = SOCK_STREAM,
	UDP = SOCK_DGRAM
};

class SHAPI Socket {
protected:
	SOCKET ConnectionSocket = INVALID_SOCKET;
	addrinfo *result = NULL, hints;

public:
	Socket(SOCKET_FAMILY family, SOCKET_TYPE type);
	~Socket();

	void connect(const char* ip, const char* port);

};

}

