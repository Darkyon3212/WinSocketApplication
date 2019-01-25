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
	addrinfo* result = WinsocketManager::GetInstance().CreateSocket("27015", WinsocketManager::Protocol::TCP, socket);

	if (result != nullptr)
	{
		int iResult = WinsocketManager::GetInstance().BindSocket(socket, result);

		if (iResult == 0)
		{
			iResult = WinsocketManager::GetInstance().Listen(socket);

			if (iResult == 0)
			{
				// Wait for a client to be connected
				SOCKET clientSocket = WinsocketManager::GetInstance().Accept(socket);
			}
		}
	}
}

void MessageJob::Convert()
{

}
