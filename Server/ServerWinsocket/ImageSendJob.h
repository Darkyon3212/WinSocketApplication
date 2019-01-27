#pragma once

#include "ThreadManager.h"
#include "WinsocketManager.h"

#include <string>

class ImageSendJob : public ThreadManager::Job
{
public:
	ImageSendJob(const char* filepath, std::string clientIP);
	~ImageSendJob();

	void ExecuteJob() override;
	void Convert() override {}

	bool Load(char**);
	void Send(char* pixelData);

private:
	std::string m_filepath;
	std::string m_clientIP;
	int m_width;
	int m_height;
};

