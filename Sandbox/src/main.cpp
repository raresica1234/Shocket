#include <stdio.h>
#include <iostream>

#include <sh.h>
#include <shocket.h>
#include <net/socket.h>

using namespace SH;

int main(int, char**) {

	if (!ShocketInitWinsock()) {
		printf("Initialization failed");
		exit(-1);
	}
	printf("Initialization iz gud");

	Socket socket = Socket(IPv4, UDP);
	socket.connect("192.168.10.102", "27015");

	ShocketCleanupWinsock();

	std::cin.get();

	return 0;
}