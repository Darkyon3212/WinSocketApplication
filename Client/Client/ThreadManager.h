#pragma once

#include <vector>
#include <thread>
#include <mutex>

#include "Singleton.hpp"

class ThreadManager : public Singleton<ThreadManager>
{
public:
	ThreadManager();
	~ThreadManager();

	struct Job
	{
		virtual void ExecuteJob() = 0;
		virtual void Convert() {}
	};

	static bool ThreadRun();

	std::vector<Job*>& GetCurrentJobs() { return m_currentJobs; }
	void AddJob(Job* const job) { m_currentJobs.push_back(job); }

	//std::vector<std::thread> GetThreads() { return m_threads; } 
	//void AddThread(const std::thread thread) { m_threads.push_back(thread); }

	std::mutex mutex;
	std::vector<std::thread> m_threads;

private:
	std::vector<Job*> m_currentJobs;
};

