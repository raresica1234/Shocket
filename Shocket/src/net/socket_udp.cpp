#include <net/socket.h>


namespace SH {


UDPSocket::UDPSocket(unsigned short port) : Socket(port, SH_SOCKET_UDP) {

}

void UDPSocket::Listen() {
	isListening = true;
}

Socket* UDPSocket::Accept() const {
	if (!isListening) return nullptr;


}

bool UDPSocket::Connect(const char* ip, unsigned short port) {

}

}