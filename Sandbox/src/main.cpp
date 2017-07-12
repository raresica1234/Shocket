#include <shocket.h>
#include <net/socket.h>
#include <stdio.h>
#include <Windows.h>

using namespace SH;



DWORD WINAPI recvFunc(LPVOID param) {
	TCPSocket* recvSocket = new TCPSocket(8192);

	recvSocket->Listen();
	printf("Listening\n");
	Socket* sock = recvSocket->Accept();

	sock->SetReceiveCallback([](const char* ip, unsigned short port, void* data, unsigned int size) -> void {
		printf("Recv From: %s:%u Data: %s\n", ip, port, data);
	});

	return 0;
}

int main() {

	ShocketInitWinsock();

	// set up for receiving stuff
	TCPSocket* recvSocket = new TCPSocket(8192);

	recvSocket->Listen();
	printf("Listening\n");
	Socket* sock = recvSocket->Accept();

	sock->SetReceiveCallback([](const char* ip, unsigned short port, void* data, unsigned int size) -> void {
		printf("Recv From: %s:%u Data: %s\n", ip, port, data);
	});

	return 0;

	//connecting and sending

	
	TCPSocket* sendSocket = new TCPSocket(8191);

	printf("Connecting\n");
	sendSocket->Connect("192.168.0.35", 8192);

	const char data[] = "Test message";

	for (int i = 0; i < 2; i++) {
		sendSocket->Send(data, sizeof(data));
		Sleep(1);
	}

	system("PAUSE");

	ShocketCleanupWinsock();

}