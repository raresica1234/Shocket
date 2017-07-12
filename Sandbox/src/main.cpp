#include <shocket.h>
#include <net/socket.h>
#include <stdio.h>
#include <Windows.h>

using namespace SH;

DWORD WINAPI recvFunc(LPVOID param) {
	TCPSocket* recvSocket = new TCPSocket(8192);

	recvSocket->Listen();

	Socket* sock = recvSocket->Accept();

	sock->SetReceiveCallback([](const char* ip, unsigned short port, void* data, unsigned int size) -> void {
		printf("Recv From: %s:%u Data: %s\n", ip, port, data);
	});

	return 0;
}

int main() {

	ShocketInitWinsock();
	
	TCPSocket* sendSocket = new TCPSocket(8191);


	CreateThread(0, 0, recvFunc, 0, 0, 0);

	Sleep(1000);

	sendSocket->Connect("localhost", 8192);

	const char data[] = "Test message";

	for (int i = 0; i < 100; i++) {
		sendSocket->Send(data, sizeof(data));
		Sleep(1);
	}

	system("PAUSE");

	ShocketCleanupWinsock();

}