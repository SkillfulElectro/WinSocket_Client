#include "WinSockSimplified.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <cstring>
#pragma comment(lib, "ws2_32.lib")

/*
	with ip method you will define you gonna use IPv4 or IPv6

	with IPv4_IPv6_webaddress you will define Ip of website or ... 
	// you dont need to specify all of them just one is enough

	with port you will define port 
	// for example http has port 80 , https has port 443
*/
WinSockSimplified::WinSockSimplified(ipMethod ip_family, const char* IPv4_IPv6_webaddress, const char* port) :result{ nullptr } {
	int Testing_WSA = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (Testing_WSA != 0) {
		std::cout << "intialization of WSA Data failed";
		std::exit(-1);
	}

	ZeroMemory(&socket_intialize_helper, sizeof(socket_intialize_helper));
	if (ip_family == IPv4) {
		socket_intialize_helper.ai_family = AF_INET;
		socket_intialize_helper.ai_socktype = SOCK_STREAM;
	}
	else {
		socket_intialize_helper.ai_family = AF_INET6;
		socket_intialize_helper.ai_socktype = SOCK_STREAM;
	}

	if (getaddrinfo(IPv4_IPv6_webaddress, port , &socket_intialize_helper, &result) != 0) {
		std::cerr << "Error getting address info." << std::endl;
		WSACleanup();
		std::exit(-1);
	}

	clientSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (clientSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		std::exit(-1);
	}

	if (connect(clientSocket, result->ai_addr, static_cast<int>(result->ai_addrlen)) == SOCKET_ERROR) {
		std::cerr << "Connection error." << std::endl;
		closesocket(clientSocket);
		freeaddrinfo(result);
		WSACleanup();
		std::exit(-1);
	}
}

void WinSockSimplified::SetRequest_optional(const char* request) {
	if (send(clientSocket, request, strlen(request), 0) == SOCKET_ERROR) {
		std::cerr << "Error sending request." << std::endl;
		closesocket(clientSocket);
		freeaddrinfo(result);
		WSACleanup();
		std::exit(-1);
	}
}

void WinSockSimplified::default_Request_set(const char* IPv4_IPv6_webaddress) {
	const char* request_method{ "GET / HTTP/1.1\r\nHost: " };
	const char* request_connectionType{ "\r\nConnection: close\r\n\r\n" };

	int lenCount_IP{ 1 };
	for (; IPv4_IPv6_webaddress[lenCount_IP]; ++lenCount_IP);

	int len_Method{ 1 };
	for (; request_method[len_Method]; ++len_Method);

	int len_connection{ 1 };
	for (; request_connectionType[len_connection]; ++len_connection);


	int total_len{
		lenCount_IP + len_connection + len_Method + 1
	};

	auto request{
		new char[total_len]
	};

	int real_len{ 0 };
	for (int i{ 0 }; i < len_Method; ++i, ++real_len) {
		request[i] = request_method[i];
	}

	for (int i{ 0 }; i < lenCount_IP; ++i, ++real_len) {
		request[real_len] = IPv4_IPv6_webaddress[i];
	}

	for (int i{ 0 }; i < len_connection; ++i, ++real_len) {
		request[real_len] = request_connectionType[i];
	}
	request[total_len - 1] = '\0';

	if (send(clientSocket, request, strlen(request), 0) == SOCKET_ERROR) {
		std::cerr << "Error sending request." << std::endl;
		closesocket(clientSocket);
		freeaddrinfo(result);
		WSACleanup();
		std::exit(-1);
	}

	delete[] request;
}

void WinSockSimplified::get_Response(std::string response) {
	char buffer[4096];

	while (true) {
		int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesReceived <= 0) {
			break;
		}
		response.append(buffer, bytesReceived);
	}
}

WinSockSimplified::~WinSockSimplified() {
	freeaddrinfo(result);
	closesocket(clientSocket);
	WSACleanup();
}