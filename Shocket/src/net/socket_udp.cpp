#include <net/socket.h>


namespace SH {


UDPSocket::UDPSocket(unsigned short port) : Socket(port, SH_SOCKET_UDP) {

}



}