#pragma once

#include "WinsocketManager.h"
#include "ThreadManager.h"

class ImageRecJob : public ThreadManager::Job
{
public:
	ImageRecJob();
	~ImageRecJob();

	void ExecuteJob() override;
	void Convert() override {};
};

