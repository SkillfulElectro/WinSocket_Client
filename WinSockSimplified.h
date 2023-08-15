#pragma once
/*
	including needed headers for windows socket
*/
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

class WinSockSimplified
{
private:
	WSADATA wsaData;
	addrinfo socket_intialize_helper;
	SOCKET clientSocket;
	addrinfo* result;
public:
	enum ipMethod {
		IPv4,
		IPv6
	};

	/*
		getting IP which you want to use , Ip address of server and port

		// instead of Ip address you can you website address
	*/
	WinSockSimplified(ipMethod , const char* , const char*);

	void SetRequest_optional(const char*);
	void default_Request_set(const char* IPv4_IPv6_webaddress);

	/*
	returning pointer to protocol , you can choose protocol from ws2def.h
	*/
	auto optinal_ptr_SetProtocol() {
		return &socket_intialize_helper.ai_protocol;
	}

	/*
		give std::string basket to me , to i fill it with answer from server
	*/
	void get_Response(std::string&);

	// cleaning :D
	~WinSockSimplified();
};

