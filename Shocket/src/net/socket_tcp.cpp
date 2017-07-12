#include <net/socket.h>

namespace SH {

TCPSocket::TCPSocket(unsigned short port) : Socket(port, SH_SOCKET_TCP) {

}



}