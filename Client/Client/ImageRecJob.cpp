#include <iostream>

#include "ImageRecJob.h"

static const int DEFAULT_BUFLEN = 350000;


ImageRecJob::ImageRecJob()
{
}


ImageRecJob::~ImageRecJob()
{
}

void ImageRecJob::ExecuteJob()
{
	SOCKET socket;
	// Create socket
	addrinfo* result = WinsocketManager::GetInstance().CreateSocket("1337", WinsocketManager::Protocol::TCP, socket);
	bool done = false;

	if (result != nullptr)
	{
		// Bind

		int iResult = WinsocketManager::GetInstance().BindSocket(socket, result);

		if (iResult == 0)
		{
			while (done == false)
			{
				printf("Listening for receiving images");

				iResult = WinsocketManager::GetInstance().Listen(socket);

				if (iResult == 0)
				{

					// Wait for a client to be connected
					SOCKET clientSocket = WinsocketManager::GetInstance().Accept(socket);

					printf("Receving image...\n");

					char recvbuf[DEFAULT_BUFLEN];
					int iResult, iSendResult;
					int recvbuflen = DEFAULT_BUFLEN;

					// Receive until the peer shuts down the connection
					do {

						iResult = recv(clientSocket, recvbuf, recvbuflen, 0);

						if (iResult > 0) {

							printf("Bytes received: %d\n", iResult);
							printf("Output: \n", iResult);

							// Echo the buffer back to the sender
							const char* sendBuffer = "1";

							iSendResult = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
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
