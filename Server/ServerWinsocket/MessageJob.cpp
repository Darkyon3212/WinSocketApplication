#include <iostream>

#include "MessageJob.h"
#include "ServerManager.h"


static const int DEFAULT_BUFLEN = 512;

MessageJob::MessageJob(MessageType type, std::string clientIP, std::string message)
	: m_type(type)
	, m_clientIP(clientIP)
	, m_message(message)
{

}

MessageJob::~MessageJob()
{

}

void MessageJob::ExecuteJob()
{
	if (m_type == MessageType::EPrivate)
	{
		Private();
	}
	else if (m_type == MessageType::EBroadcast)
	{
		Broadcast();
	}
}

void MessageJob::Convert()
{

}

void MessageJob::Private()
{
	SOCKET socket;
	addrinfo* result = WinsocketManager::GetInstance().CreateSocketSend(m_clientIP.c_str(), "1340", WinsocketManager::Protocol::TCP, socket);
	bool done = false;

	if (result != nullptr)
	{
		int IResult = WinsocketManager::GetInstance().Connect(socket, result);

		if (IResult == 0)
		{
			// Send message

			int sizeOfSendBuffer = m_message.size();
			const char* sendBuffer = m_message.c_str();

			int recvbuflen = 512;
			char recvbuf[512];

			int iResult = send(socket, sendBuffer, sizeOfSendBuffer, 0);
			if (iResult == SOCKET_ERROR)
			{
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(socket);
			}

			printf("Bytes Sent: %ld\n", iResult);

			iResult = shutdown(socket, SD_SEND);
			if (iResult == SOCKET_ERROR)
			{
				printf("shutdown failed: %d\n", WSAGetLastError());
				closesocket(socket);
			}

			do
			{
				iResult = recv(socket, recvbuf, recvbuflen, 0);
				if (iResult > 0)
					printf("Bytes received: %d\n", iResult);
				else if (iResult == 0)
					printf("Connection closed\n");
				else
					printf("recv failed: %d\n", WSAGetLastError());
			} while (iResult > 0);
		}
	}
}

void MessageJob::Broadcast()
{
	for (int i = 0; i < ServerManager::GetInstance().GetAvaibleIPs().size(); i++)
	{
		std::string ip = ServerManager::GetInstance().GetAvaibleIPs().at(i);

		SOCKET socket;
		addrinfo* result = WinsocketManager::GetInstance().CreateSocketSend(ip.c_str(), "1340", WinsocketManager::Protocol::TCP, socket);

		if (result != nullptr)
		{
			int IResult = WinsocketManager::GetInstance().Connect(socket, result);

			if (IResult == 0)
			{
				// Send message

				int sizeOfSendBuffer = m_message.size();
				const char* sendBuffer = m_message.c_str();

				int recvbuflen = 512;
				char recvbuf[512];

				int iResult = send(socket, sendBuffer, sizeOfSendBuffer, 0);
				if (iResult == SOCKET_ERROR)
				{
					printf("send failed: %d\n", WSAGetLastError());
					closesocket(socket);
				}

				printf("Bytes Sent: %ld\n", iResult);

				iResult = shutdown(socket, SD_SEND);
				if (iResult == SOCKET_ERROR)
				{
					printf("shutdown failed: %d\n", WSAGetLastError());
					closesocket(socket);
				}

				do
				{
					iResult = recv(socket, recvbuf, recvbuflen, 0);
					if (iResult > 0)
						printf("Bytes received: %d\n", iResult);
					else if (iResult == 0)
						printf("Connection closed\n");
					else
						printf("recv failed: %d\n", WSAGetLastError());
				} while (iResult > 0);
			}
		}
	}
}
