#include "NetworkJob.h"

NetworkJob::NetworkJob(std::string ip, NetworkType type)
	: m_ip(ip)
	, m_type(type)
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
	addrinfo* result = WinsocketManager::GetInstance().CreateSocketClient(m_ip.c_str(), "1338", WinsocketManager::Protocol::TCP, socket);

	if (result != nullptr)
	{
		// Send message

		int IResult = WinsocketManager::GetInstance().Connect(socket, result);

		if (IResult == 0)
		{
			// Send message

			int sizeOfSendBuffer = 10;
			const char* sendBuffer = "connect";

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

void NetworkJob::Kick()
{
	SOCKET socket;
	addrinfo* result = WinsocketManager::GetInstance().CreateSocket("1339", WinsocketManager::Protocol::TCP, socket);
	bool done = false;

	if (result != nullptr)
	{
		int IResult = WinsocketManager::GetInstance().BindSocket(socket, result);

		if (IResult == 0)
		{
			while (done == false)
			{
				// Listen

				IResult = WinsocketManager::GetInstance().Listen(socket);

				if (IResult == 0)
				{
					char* recvbuf = new char[124];
					int recvbuflen = 124;

					sockaddr_in recvAddr;
					int inAddrLen = sizeof(recvAddr);

					SOCKET recvSocket = WinsocketManager::GetInstance().Accept(socket);

					do
					{
						IResult = recv(recvSocket, recvbuf, recvbuflen, 0);

						if (IResult > 0)
						{
							char* resultString = new char[IResult];

							memcpy(resultString, recvbuf, recvbuflen);

							printf(resultString);
						}

					} while (IResult > 0);

					IResult = shutdown(recvSocket, SD_SEND);
					if (IResult == SOCKET_ERROR)
					{
						printf("shutdown failed: %d\n", WSAGetLastError());
						closesocket(recvSocket);
					}
				}
			}
		}
	}
}
