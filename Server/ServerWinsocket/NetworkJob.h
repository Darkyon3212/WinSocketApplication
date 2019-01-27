#pragma once

#include "WinsocketManager.h"
#include "ThreadManager.h"

class NetworkJob : public ThreadManager::Job
{
public:

	enum NetworkType
	{
		EConnect,
		EKick
	};

	NetworkJob(NetworkType type, std::string clientIP = "");
	~NetworkJob();

	void ExecuteJob() override;

private:
	void Connect();
	void Kick();

	NetworkType m_type;
	std::string m_clientIP;
};

