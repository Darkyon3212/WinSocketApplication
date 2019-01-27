#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "NetworkJob.h"
#include "ServerManager.h"

NetworkJob::NetworkJob(NetworkType type, std::string clientIP)
	: m_type(type)
	, m_clientIP(clientIP)
{
}

NetworkJob::~NetworkJob()
{
}

void NetworkJob::ExecuteJob()
{
	if (m_type == NetworkType::EConnect)
	{
		Connect();
	}
	else if (m_type = NetworkType::EKick)
	{
		Kick();
	}
}

void NetworkJob::Connect()
{
	SOCKET socket;
	addrinfo* result = WinsocketManager::GetInstance().CreateSocketRecive("1338", WinsocketManager::Protocol::TCP, socket);

	sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);

	char RecvBuf[16];
	int BufLen = 16;
	bool done = false;

	if (result != nullptr)
	{
		int IResult = WinsocketManager::GetInstance().BindSocket(socket, result);

		while (done == false)
		{
			// listen
			printf("Listening for connections...\n");

			IResult = WinsocketManager::GetInstance().Listen(socket);

			if (IResult == 0)
			{

				char* recvbuf = new char[10];
				int iResult, iSendResult;
				int recvbuflen = 10;

				sockaddr_in recvAddr;
				int inAddrLen = sizeof(recvAddr);

				SOCKET recvSocket = WinsocketManager::GetInstance().Accept(socket);

				do 
				{

					iResult = recv(recvSocket, recvbuf, recvbuflen, MSG_WAITALL);

					if (iResult > 0) 
					{

						printf("Bytes received: %d\n", iResult);

						IResult = getpeername(recvSocket, (SOCKADDR*)&recvAddr, &inAddrLen);

						char* IP = inet_ntoa(recvAddr.sin_addr);

						// Echo the buffer back to the sender
						const char* sendBuffer;

						if (!ServerManager::GetInstance().DoIpAlreadyExits(IP))
						{
							sendBuffer = "added";
							ServerManager::GetInstance().AddIP(IP);
						}
						else
						{
							sendBuffer = "exits";
						}

						iSendResult = send(recvSocket, sendBuffer, sizeof(sendBuffer), 0);
						if (iSendResult == SOCKET_ERROR) {
							printf("send failed: %d\n", WSAGetLastError());
							closesocket(recvSocket);
						}
						printf("Bytes sent: %d\n", iSendResult);
					}
					else if (iResult == 0)
						printf("Connection closing...\n");
					else {
						printf("recv failed: %d\n", WSAGetLastError());
						closesocket(recvSocket);
					}

				} while (iResult > 0);

				iResult = shutdown(recvSocket, SD_SEND);
				if (iResult == SOCKET_ERROR) {
					printf("shutdown failed: %d\n", WSAGetLastError());

					closesocket(recvSocket);

					done = true;
				}
				else
				{
					closesocket(recvSocket);
				}
				
			}
		}
	}
}

void NetworkJob::Kick()
{
	SOCKET socket;
	addrinfo* result = WinsocketManager::GetInstance().CreateSocketSend(m_clientIP.c_str(), "1339", WinsocketManager::Protocol::TCP, socket);

	if (result != nullptr)
	{
		int IResult = WinsocketManager::GetInstance().Connect(socket, result);

		if (IResult == 0)
		{
			ServerManager::GetInstance().RemoveIP(m_clientIP);

			int recvbuflen = 512;

			const char* sendbuf = "You have been kicked";
			int sendLength = sizeof("You have been kicked");
			char recvbuf[512];

			int iResult = send(socket, sendbuf, sendLength, 0);
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
