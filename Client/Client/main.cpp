#include <iostream>
#include <string>
#include <sstream>

#include "WinsocketManager.h"
#include "ThreadManager.h"
#include "TGAWriter.h"

#include "ImageRecJob.h"
#include "NetworkJob.h"
#include "MessageJob.h"

class Split
{
public:
	static void split(const std::string &s, char delim, std::vector<std::string> &elems);
};

int main()
{
	WinsocketManager::Initialize();
	ThreadManager::Initialize();
	TGAWriter::Initialize();

	ThreadManager::GetInstance().m_threads.push_back(std::thread(ThreadManager::ThreadRun));
	ThreadManager::GetInstance().m_threads.push_back(std::thread(ThreadManager::ThreadRun));
	ThreadManager::GetInstance().m_threads.push_back(std::thread(ThreadManager::ThreadRun));
	ThreadManager::GetInstance().m_threads.push_back(std::thread(ThreadManager::ThreadRun));
	//ThreadManager::GetInstance().AddJob(new MessageSendJob());
	ThreadManager::GetInstance().AddJob(new ImageRecJob());
	ThreadManager::GetInstance().AddJob(new NetworkJob("", NetworkJob::EKick));
	ThreadManager::GetInstance().AddJob(new MessageJob());

	//ThreadManager::ThreadRun();

	while (true) 
	{
		std::string line;
		std::getline(std::cin, line);
		std::vector<std::string> words = std::vector<std::string>();

		Split::split(line, ' ', words);

		if (words[0] == "connect")
		{
			if (words.size() > 1)
			{
				std::string ip = words[1];

				ThreadManager::GetInstance().AddJob(new NetworkJob(ip, NetworkJob::EConnect));
			}
			else
			{
				std::cout << "Missing IP argument" << std::endl;
			}
		}
	}

	system("pause");


    return 0;
}

void Split::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}