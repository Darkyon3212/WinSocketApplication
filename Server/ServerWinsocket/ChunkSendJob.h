#pragma once

#include "ThreadManager.h"
#include "WinsocketManager.h"

class ChunkSendJob : public ThreadManager::Job
{
public:
	ChunkSendJob(std::string filepath, std::string clientIP, int xOffset, int yOffset, int width, int height);
	~ChunkSendJob();

	void ExecuteJob() override;

private:
	std::string m_filepath;
	std::string m_clientIP;
	int m_xOffset;
	int m_yOffset;
	int m_width;
	int m_height;
	int m_entireWidth;

	bool Load(char** pixelData);
	char* GetChunk(char* pixelData, int entireWidth);
	void Send(char* pixelData);
};

