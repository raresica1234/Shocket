#include "shocket.h"

#include <Winsock2.h>

namespace SH {

bool ShocketInitWinsock() {
	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		//TODO: handle error
		return false;
	}

	return true;
}

void ShocketCleanupWinsock() {
	WSACleanup();
}

}