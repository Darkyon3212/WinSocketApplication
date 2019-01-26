#include "MessageSendJob.h"

MessageSendJob::MessageSendJob()
{

}

MessageSendJob::~MessageSendJob()
{

}

void MessageSendJob::ExecuteJob()
{
	SOCKET socket;
	addrinfo* result = WinsocketManager::GetInstance().CreateSocketClient("127.0.0.1", "27015", WinsocketManager::Protocol::TCP, socket);

	if (result != nullptr)
	{
		int IResult = WinsocketManager::GetInstance().Connect(socket, result);

		if (IResult == 0)
		{
			// Send message

			int recvbuflen = DEFAULT_BUFLEN;

			const char *sendbuf = "this is a test";
			char recvbuf[DEFAULT_BUFLEN];

			int iResult = send(socket, sendbuf, (int)strlen(sendbuf), 0);
			if (iResult == SOCKET_ERROR)
			{
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(socket);
				WSACleanup();
			}

			printf("Bytes Sent: %ld\n", iResult);

			iResult = shutdown(socket, SD_SEND);
			if (iResult == SOCKET_ERROR) 
			{
				printf("shutdown failed: %d\n", WSAGetLastError());
				closesocket(socket);
				WSACleanup();
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

void MessageSendJob::Convert()
{

}
