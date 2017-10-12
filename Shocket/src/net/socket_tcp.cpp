#include <net/socket.h>
#include <stdio.h>

namespace SH {

TCPSocket::TCPSocket(unsigned short port) : Socket(port, SH_SOCKET_TCP) {

}

void TCPSocket::Listen() {
	listen(sock, 0);
	isListening = true;
}


Socket* TCPSocket::Accept() const {
	if (!isListening) return nullptr;
	sockaddr_in tmp = { 0 };

	int len = sizeof(sockaddr_in);

	SOCKET s = accept(sock, (sockaddr*)&tmp, &len);

	TCPSocket* connectedSocket = new TCPSocket(s);

	memcpy(&connectedSocket->remote, &tmp, sizeof(sockaddr_in));

	return connectedSocket;
}

bool TCPSocket::Connect(const char* ip, unsigned short port) {
	sockaddr_in tmp = { 0 };

	tmp.sin_family = AF_INET;
	tmp.sin_port = htons(port);
	tmp.sin_addr.S_un.S_addr = inet_addr(ip);

	if (connect(sock, (const sockaddr*)&tmp, sizeof(sockaddr_in)) != 0) {
		//TODO: error stuff
		printf("Error connecting to %s:%u %u\n", ip, port, WSAGetLastError());
		return false;
	}

	memcpy(&remote, &tmp, sizeof(sockaddr_in));

	SetupReceive();

	return true;
}

}