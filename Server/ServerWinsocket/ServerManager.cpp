#include "ServerManager.h"



ServerManager::ServerManager()
	: m_avaibleIPs(std::vector<std::string>())
{
}


ServerManager::~ServerManager()
{
}

bool ServerManager::DoIpAlreadyExits(std::string ip)
{
	
	for (int i = 0; i < m_avaibleIPs.size(); i++)
	{
		if (m_avaibleIPs.at(i) == ip)
			return true;
	}

	return false;
}

void ServerManager::RemoveIP(std::string ip)
{
	for (std::vector<std::string>::iterator it = m_avaibleIPs.begin(); it != m_avaibleIPs.end(); it++)
	{
		if ((*it) == ip)
		{
			m_avaibleIPs.erase(it);
			break;
		}
	}
}
