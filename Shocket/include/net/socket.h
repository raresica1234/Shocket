#pragma once


#include <sh.h>

#include <winsock.h>


#define SH_DEFAULT_BUFFER_SIZE 2048

namespace SH {

enum SH_SOCKET_TYPE {
	SH_SOCKET_UDP = SOCK_DGRAM,
	SH_SOCKET_TCP = SOCK_STREAM
};

typedef void(*SH_RECEIVE_CALLBACK)(const char* ip, unsigned short port, void* data, unsigned int size);


class SHAPI Socket {
private:
	SH_SOCKET_TYPE type;
protected:
	sockaddr_in addr;
	sockaddr_in remote;

	bool isListening = false;

	SOCKET sock;

	SH_RECEIVE_CALLBACK callback;

	HANDLE tHandle = INVALID_HANDLE_VALUE;

	virtual void SetupReceive();

	Socket(SOCKET sock);
	Socket(unsigned short port, SH_SOCKET_TYPE type);
public:
	virtual ~Socket();

	virtual void Listen() = 0;
	virtual Socket* Accept() const = 0;

	virtual bool Connect(const char* ip, unsigned short port) = 0;
	virtual void Send(const void* data, unsigned int size);

	void SetReceiveCallback(SH_RECEIVE_CALLBACK callback);

	inline unsigned short GetPort() const { return ntohs(addr.sin_port); }
	inline const char* GetIP() const { return inet_ntoa(addr.sin_addr); }

	inline unsigned short GetRemotePort() const { return ntohs(remote.sin_port); }
	inline const char* GetRemoteIP() const { return inet_ntoa(remote.sin_addr); }


	inline SH_SOCKET_TYPE GetType() const { return type; }
};

class SHAPI UDPSocket : public Socket {
public:
	UDPSocket(unsigned short port);

	void Listen() override;
	Socket* Accept() const override;;

	bool Connect(const char* ip, unsigned short port) override;
};

class SHAPI TCPSocket : public Socket {
public:
	TCPSocket(unsigned short port);

	void Listen() override;
	Socket* Accept() const override;;

	bool Connect(const char* ip, unsigned short port) override;
};

}

