#pragma once

#include "WinsocketManager.h"
#include "ThreadManager.h"

class MessageJob : public ThreadManager::Job
{
public:
	MessageJob();
	~MessageJob();

	void ExecuteJob() override;
};

