#pragma once

#include "ThreadManager.h"
#include "WinsocketManager.h"

class MessageJob : public ThreadManager::Job
{
public:
	MessageJob();
	~MessageJob();

	void ExecuteJob() override;
	void Convert() override;

private:
	SOCKET messageSocket;
};

