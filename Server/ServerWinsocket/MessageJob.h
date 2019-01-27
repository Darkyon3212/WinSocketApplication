#pragma once

#include "ThreadManager.h"
#include "WinsocketManager.h"

class MessageJob : public ThreadManager::Job
{
public:
	enum MessageType
	{
		EPrivate,
		EBroadcast
	};

	MessageJob(MessageType type, std::string clientIP, std::string message);
	~MessageJob();

	void ExecuteJob() override;
	void Convert() override;

private:
	void Private();
	void Broadcast();

	MessageType m_type;
	std::string m_clientIP;
	std::string m_message;
};

