#pragma once

static const int DEFAULT_BUFLEN = 512;

#include "ThreadManager.h"
#include "WinsocketManager.h"

class MessageSendJob : public ThreadManager::Job
{
public:
	MessageSendJob();
	~MessageSendJob();

	void ExecuteJob() override;
	void Convert() override;
};

