#include <iostream>

#include "MessageJob.h"


static const int DEFAULT_BUFLEN = 512;

MessageJob::MessageJob()
{

}

MessageJob::~MessageJob()
{

}

void MessageJob::ExecuteJob()
{
	SOCKET socket;
	addrinfo* result = WinsocketManager::GetInstance().CreateSocket("27015", WinsocketManager::Protocol::TCP, socket);
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

					printf("Client is now connected!\n");

					char recvbuf[DEFAULT_BUFLEN];
					int iResult, iSendResult;
					int recvbuflen = DEFAULT_BUFLEN;

					// Receive until the peer shuts down the connection
					do {

						iResult = recv(clientSocket, recvbuf, recvbuflen, 0);

						if (iResult > 0) {

							printf("Bytes received: %d\n", iResult);
							printf("Output: \n", iResult);

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
								WSACleanup();
							}
							printf("Bytes sent: %d\n", iSendResult);
						}
						else if (iResult == 0)
							printf("Connection closing...\n");
						else {
							printf("recv failed: %d\n", WSAGetLastError());
							closesocket(clientSocket);
							WSACleanup();
						}

					} while (iResult > 0);

					iResult = shutdown(clientSocket, SD_SEND);
					if (iResult == SOCKET_ERROR) {
						printf("shutdown failed: %d\n", WSAGetLastError());

						closesocket(clientSocket);
						WSACleanup();

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

void MessageJob::Convert()
{

}
