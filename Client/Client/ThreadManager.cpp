#include <thread>
#include <iostream>

#include "ThreadManager.h"



ThreadManager::ThreadManager()
	: m_currentJobs(std::vector<Job*>())
	, m_threads(std::vector<std::thread>())
{
}

ThreadManager::~ThreadManager()
{
	
}

bool ThreadManager::ThreadRun()
{
	std::cout << "New thread created and running" << std::endl;

	while (true)
	{
		Job* job(nullptr);

		GetInstance().mutex.lock();

		std::vector<Job*>& currentJobs = GetInstance().GetCurrentJobs();

		if (currentJobs.size() > 0)
		{
			job = currentJobs.at(0);
			std::vector<Job*>::iterator it = currentJobs.begin();
			currentJobs.erase(it);
			std::cout << "An thread got a new job to do" << std::endl;
		}

		GetInstance().mutex.unlock();

		if (job != nullptr)
		{
			job->ExecuteJob();
			job->Convert();

			delete job;
			job = nullptr;
		} 
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(240));
		}
	}
}
