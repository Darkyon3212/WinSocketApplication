#pragma once

#include <string>
#include <vector>

#include "Singleton.hpp"

class ServerManager : public Singleton<ServerManager>
{
public:
	ServerManager();
	~ServerManager();

	std::vector<std::string> GetAvaibleIPs() { return m_avaibleIPs; }
	void AddIP(std::string ip) { m_avaibleIPs.push_back(ip); }
	bool DoIpAlreadyExits(std::string ip);
	void RemoveIP(std::string ip);

private:
	std::vector<std::string> m_avaibleIPs;
};

