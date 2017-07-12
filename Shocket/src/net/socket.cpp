#include <net/socket.h>
#include <stdio.h>

namespace SH {

struct SH_DATA_PASSTHROUGH {
	Socket* socket;
	SOCKET sock;
	SH_RECEIVE_CALLBACK callback;
};

DWORD WINAPI recv_thread(LPVOID param) {

	SH_DATA_PASSTHROUGH* data = (SH_DATA_PASSTHROUGH*)param;

	char tmpData[SH_DEFAULT_BUFFER_SIZE] = { 0 };

	sockaddr_in from;

	while (true) {
		int len = sizeof(sockaddr_in);

		unsigned int received = recvfrom(data->sock, tmpData, SH_DEFAULT_BUFFER_SIZE, 0, (sockaddr*)&from, &len);
		data->callback(inet_ntoa(from.sin_addr), ntohs(from.sin_port), tmpData, received);
	}

	return 0;
}

void Socket::SetupReceive() {

	tHandle = CreateThread(0, 0, recv_thread, new SH_DATA_PASSTHROUGH{this, sock, callback}, THREAD_TERMINATE, 0);
}

Socket::Socket(SOCKET sock) {
	this->sock = sock;

	int len = sizeof(sockaddr_in);

	getsockname(sock, (sockaddr*)&addr, &len);
}

Socket::Socket(unsigned short port, SH_SOCKET_TYPE type) {

	sock = socket(AF_INET, (int)type, type == SH_SOCKET_UDP ? IPPROTO_UDP : IPPROTO_TCP);

	if (sock == INVALID_SOCKET) {
		//TODO: legit error messages
		return;
	}

	char name[256];

	gethostname(name, 256);

	hostent* local = gethostbyname(name);

	memset(&addr, 0, sizeof(sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	
	memcpy(&addr.sin_addr.S_un.S_addr, local->h_addr_list[0], sizeof(in_addr));

	if (bind(sock, (const sockaddr*)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		//TODO: error handling
		printf("%u\n", WSAGetLastError());
		closesocket(sock);
		sock = 0;
	}
}

Socket::~Socket() {
	if (tHandle != INVALID_HANDLE_VALUE) TerminateThread(tHandle, 0);
	closesocket(sock);
}

bool Socket::Connect(const char* ip, unsigned short port) {
	sockaddr_in tmp = { 0 };
	
	tmp.sin_family = AF_INET;
	tmp.sin_port = htons(port);
	tmp.sin_addr.S_un.S_addr = inet_addr(ip);

	if (connect(sock, (const sockaddr*)&tmp, sizeof(sockaddr_in)) != 0) {
		//TODO: error stuff
		return false;
	}

	memcpy(&remote, &tmp, sizeof(sockaddr_in));

	SetupReceive();

	return true;
}

void Socket::Send(const void* data, unsigned int size) {
	unsigned int split = size / SH_DEFAULT_BUFFER_SIZE;
	unsigned int rest = size % SH_DEFAULT_BUFFER_SIZE;
	unsigned int totalSent = 0;

	while (split--) {
		unsigned int sent = send(sock, (const char*)data + totalSent, SH_DEFAULT_BUFFER_SIZE, 0);
		if (sent != SH_DEFAULT_BUFFER_SIZE) {
			//TODO: handle stuff
		}

		totalSent += sent;
	}

	if (rest) {
		totalSent += send(sock, (const char*)data + totalSent, size - totalSent, 0);
	}
}

void Socket::SetReceiveCallback(SH_RECEIVE_CALLBACK callback) {
	this->callback = callback;
	if (tHandle != INVALID_HANDLE_VALUE) {
		TerminateThread(tHandle, 0);
	}

	SetupReceive();
}

void Socket::Listen() {
	listen(sock, 0);
	isListening = true;
}

Socket* Socket::Accept() const {
	if (!isListening) return nullptr;
	sockaddr_in tmp = { 0 };

	int len = sizeof(sockaddr_in);

	SOCKET s = accept(sock, (sockaddr*)&tmp, &len);

	Socket* connectedSocket = new Socket(s);

	memcpy(&connectedSocket->remote, &tmp, sizeof(sockaddr_in));

	return connectedSocket;
}

}