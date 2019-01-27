#include "WinsocketManager.h"


WinsocketManager::WinsocketManager()
{
	InitWinsocket();
}


WinsocketManager::~WinsocketManager()
{
}

int WinsocketManager::InitWinsocket()
{
	int result;

	result = WSAStartup(MAKEWORD(2, 2), &m_wsaData);

	return result;
}

addrinfo* WinsocketManager::CreateSocketRecive(const char* port, Protocol protocol, SOCKET& rSocket)
{
	struct addrinfo *result = nullptr, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;

	if (protocol == Protocol::TCP)
	{
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_socktype = SOCK_STREAM;
	}
	else if (protocol == Protocol::UDP)
	{
		hints.ai_protocol = IPPROTO_UDP;
		hints.ai_socktype = SOCK_DGRAM;
	}

	int IResult = 0;
	IResult = getaddrinfo(NULL, port, &hints, &result);

	if (IResult != 0)
	{
		printf("getaddrinfo failed: %d\n", IResult);

		return result;
	}

	SOCKET ListenSocket = INVALID_SOCKET;

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);

		return result;
	}

	rSocket = ListenSocket;

	return result;
}

addrinfo* WinsocketManager::CreateSocketSend(const char* ip, const char* port, Protocol protocol, SOCKET& rSocket)
{
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int IResult;

	IResult = getaddrinfo(ip, port, &hints, &result);

	if (IResult != 0) {
		printf("getaddrinfo failed: %d\n", IResult);
		return result;
	}

	rSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (rSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return result;
	}

	return result;
}

int WinsocketManager::BindSocket(SOCKET& rSocket, addrinfo* result)
{
	int success = 0;

	success = bind(rSocket, result->ai_addr, (int)result->ai_addrlen);
	if (success == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(rSocket);
		return 1;
	}

	freeaddrinfo(result);

	return 0;
}

int WinsocketManager::Listen(SOCKET& rSocket)
{
	int result = listen(rSocket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		closesocket(rSocket);
	}

	return 0;
}

SOCKET WinsocketManager::Accept(SOCKET& rSocket)
{
	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;

	ClientSocket = accept(rSocket, NULL, NULL);

	if (ClientSocket == INVALID_SOCKET)
	{
		closesocket(rSocket);
	}

	return ClientSocket;
}

int WinsocketManager::Connect(SOCKET& rSocket, addrinfo* result)
{
	int iResult = connect(rSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(rSocket);
		rSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (rSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");


		return 1;
	}

	return 0;
}