#pragma once

#include "Singleton.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

class WinsocketManager : public Singleton<WinsocketManager>
{
public:
	WinsocketManager();
	~WinsocketManager();

	enum Protocol
	{
		TCP,
		UDP
	};

	int InitWinsocket();
	addrinfo* CreateSocket(const char* port, Protocol protocol, SOCKET& rSocket);
	addrinfo* CreateSocketClient(const char* ip, const char* port, Protocol protocol, SOCKET& rSocket);
	int BindSocket(SOCKET& rSocket, addrinfo* result);
	int Listen(SOCKET& rSocket);
	SOCKET Accept(SOCKET& rSocket);
	int Connect(SOCKET& rSocket, addrinfo* result);

private:
	WSADATA m_wsaData;
};

