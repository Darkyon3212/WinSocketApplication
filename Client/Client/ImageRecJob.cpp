#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <cmath>

#include "ImageRecJob.h"
#include "TGAWriter.h"

static const int DEFAULT_BUFLEN_FILE = 80000000;


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
				printf("Listening for receiving images\n");

				iResult = WinsocketManager::GetInstance().Listen(socket);

				if (iResult == 0)
				{

					// Wait for a client to be connected
					SOCKET clientSocket = WinsocketManager::GetInstance().Accept(socket);

					printf("Receving image...\n");

					char* recvbuf = new char[DEFAULT_BUFLEN_FILE];
					int iResult, iSendResult;
					int recvbuflen = DEFAULT_BUFLEN_FILE;

					// Receive until the peer shuts down the connection
					do 
					{

						iResult = recv(clientSocket, recvbuf, recvbuflen, MSG_WAITALL);

						if (iResult > 0) 
						{

							printf("Bytes received: %d\n", iResult);

							unsigned char* pixelData = new unsigned char[iResult - 2];

							//int size = sizeof(recvbuf) / sizeof(*recvbuf);

							int widthSize = (unsigned char)recvbuf[iResult - 2];
							int heightSize = (unsigned char)recvbuf[iResult - 1];

							// Get width
							int width = 0;
							int height = 0;

							for (int i = 0; i < widthSize; i++)
							{
								int digit = (int)recvbuf[iResult - i - (heightSize + 1) - 2];

								width += digit * pow(10, i);
							}

							// Get height
							for (int i = 0; i < heightSize; i++)
							{
								int digit = (int)recvbuf[iResult - i - 3];

								height += digit * pow(10, i);
							}

							for (int i = 0; i < iResult - widthSize - heightSize - 2; i++)
							{
								pixelData[i] = recvbuf[i];
							}

							//memcpy(pixelData, recvbuf, iResult);

							TGAWriter::GetInstance().save("output.tga", pixelData, width, height);

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
