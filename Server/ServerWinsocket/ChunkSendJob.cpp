#define _CRT_SECURE_NO_WARNINGS

#include <cstring>

#include "ChunkSendJob.h"

extern "C"
{
#include "tga_reader.h"
}

ChunkSendJob::ChunkSendJob(std::string filepath, std::string clientIP, int xOffset, int yOffset, int width, int height)
	: m_filepath(filepath)
	, m_clientIP(clientIP)
	, m_xOffset(xOffset)
	, m_yOffset(yOffset)
	, m_width(width)
	, m_height(height)
{
}

ChunkSendJob::~ChunkSendJob()
{
}

void ChunkSendJob::ExecuteJob()
{
	char* pixelData = nullptr;
	bool success = Load(&pixelData);

	if (success)
	{
		pixelData = GetChunk(pixelData, m_entireWidth);
		Send(pixelData);
	}
}

bool ChunkSendJob::Load(char** pixelData)
{
	std::FILE* file = fopen(m_filepath.c_str(), "rb");

	if (file)
	{
		int size;
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fseek(file, 0, SEEK_SET);

		unsigned char *buffer = (unsigned char *)tgaMalloc(size);
		fread(buffer, 1, size, file);
		fclose(file);

		m_entireWidth = tgaGetWidth(buffer);

		*pixelData = (char*)tgaRead(buffer, TGA_READER_ABGR);

		tgaFree(buffer);

		return true;
	}
	else
	{
		printf("Could not find given file!");
	}

	return false;
}

char* ChunkSendJob::GetChunk(char* pixelData, int entireWidth)
{
	const int size = m_width * m_height * 4;
	char* pixels = new char[size];

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int ReadStride = (entireWidth * (y + m_yOffset) + (x + m_xOffset)) * 4;
			int WriteStride = (m_width * y + x) * 4;
			pixels[WriteStride + 0] = pixelData[ReadStride + 0];
			pixels[WriteStride + 1] = pixelData[ReadStride + 1];
			pixels[WriteStride + 2] = pixelData[ReadStride + 2];
			pixels[WriteStride + 3] = pixelData[ReadStride + 3];
		}
	}

	return pixels;
}

void ChunkSendJob::Send(char* pixelData)
{
	SOCKET socket;

	addrinfo* result = WinsocketManager::GetInstance().CreateSocketSend(m_clientIP.c_str(), "1337", WinsocketManager::Protocol::TCP, socket);
	bool done = false;

	if (result != nullptr)
	{
		// Connect

		int IResult = WinsocketManager::GetInstance().Connect(socket, result);

		if (IResult == 0)
		{
			// Send message

			int recvbuflen = 512;
			char recvbuf[512];

			//memcpy(sendBuffer, pixelData, m_width * m_height * COLORCHANNELS);

			// Put width and height in buffer

			std::vector<short int> widthDigits = std::vector<short int>();
			std::vector<short int> heightDigits = std::vector<short int>();
			int width(m_width);
			int height(m_height);

			// Get width digit 
			while (width > 0)
			{
				int digit = width % 10;
				width /= 10;
				widthDigits.push_back(digit);
			}

			// Get height digits
			while (height > 0)
			{
				int digit = height % 10;
				height /= 10;
				heightDigits.push_back(digit);
			}

			int sizeOfSendBuffer = m_width * m_height * 4 + widthDigits.size() + heightDigits.size() + 2;
			int sizeOfPixelBuffer = m_width * m_height * 4;
			char* sendBuffer = new char[sizeOfSendBuffer];

			for (int i = widthDigits.size() - 1; i > -1; i--)
			{
				sendBuffer[sizeOfSendBuffer - i - heightDigits.size() - 2 - 1] = (char)widthDigits.at(i);
			}

			for (int i = heightDigits.size() - 1; i > -1; i--)
			{
				sendBuffer[sizeOfSendBuffer - i - 2 - 1] = heightDigits.at(i);
			}

			sendBuffer[sizeOfSendBuffer - 1] = widthDigits.size();
			sendBuffer[sizeOfSendBuffer - 2] = heightDigits.size();

			for (int i = 0; i < sizeOfPixelBuffer; i++)
			{
				sendBuffer[i] = pixelData[i];
			}

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

			tgaFree(pixelData);
			delete[] sendBuffer;
		}
	}
}
