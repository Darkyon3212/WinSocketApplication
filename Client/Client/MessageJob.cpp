#include <iostream>

#include "MessageJob.h"


MessageJob::MessageJob()
{
}


MessageJob::~MessageJob()
{
}

void MessageJob::ExecuteJob()
{
	SOCKET socket;
	addrinfo* result = WinsocketManager::GetInstance().CreateSocket("1340", WinsocketManager::Protocol::TCP, socket);
	bool done = false;

	if (result != nullptr)
	{
		int iResult = WinsocketManager::GetInstance().BindSocket(socket, result);

		if (iResult == 0)
		{
			while (done == false)
			{
				iResult = WinsocketManager::GetInstance().Listen(socket);

				if (iResult == 0)
				{
					// Wait for a client to be connected
					SOCKET clientSocket = WinsocketManager::GetInstance().Accept(socket);

					char recvbuf[512];
					int iResult, iSendResult;
					int recvbuflen = 512;

					// Receive until the peer shuts down the connection
					do {

						iResult = recv(clientSocket, recvbuf, recvbuflen, 0);

						if (iResult > 0) {

							printf("Bytes received: %d\n", iResult);

							for (int i = 0; i < iResult; i++)
							{
								if (!recvbuf[i] == NULL)
								{
									std::cout << recvbuf[i];
								}
								else
								{
									break;
								}
							}

							std::cout << std::endl;

							// Echo the buffer back to the sender
							iSendResult = send(clientSocket, recvbuf, iResult, 0);
							if (iSendResult == SOCKET_ERROR) {
								printf("send failed: %d\n", WSAGetLastError());
								closesocket(clientSocket);
							}
							printf("Bytes sent: %d\n", iSendResult);
						}
						else if (iResult == 0)
							printf("Connection closing...\n");
						else {
							printf("recv failed: %d\n", WSAGetLastError());
							closesocket(clientSocket);
						}

					} while (iResult > 0);

					iResult = shutdown(clientSocket, SD_SEND);
					if (iResult == SOCKET_ERROR) {
						printf("shutdown failed: %d\n", WSAGetLastError());

						closesocket(clientSocket);

						done = true;
					}
					else
					{
						closesocket(clientSocket);
					}
				}
			}
		}
	}
}
