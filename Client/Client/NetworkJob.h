#pragma once

#include <string>

#include "ThreadManager.h"
#include "WinsocketManager.h"

class NetworkJob : public ThreadManager::Job
{
public:

	enum NetworkType
	{
		EConnect,
		EKick
	};

	NetworkJob(std::string ip, NetworkType type);
	~NetworkJob();

	void ExecuteJob() override;
	void Convert() override {}

	

private:
	void Connect();
	void Kick();

	NetworkType m_type;
	std::string m_ip;
};

