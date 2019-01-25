
#include <cstdlib>
#include <cstring>

#include "MemoryManager.h"

#define NOTHING 0x0

MemoryManager::MemoryManager()
	: m_pMemoryBaseBuffer(NULL)
	, m_memoryMap(std::map<void*, Header>())
	, m_current(NOTHING)
	, m_end(NOTHING)
{

}

MemoryManager::~MemoryManager()
{
	if (m_pMemoryBaseBuffer != NULL)
		delete[] m_pMemoryBaseBuffer;
}

void MemoryManager::AlocateBaseMemory(const size_t totalSizeOfProgram)
{
	if (m_pMemoryBaseBuffer == NULL)
	{
		m_pMemoryBaseBuffer = new char[totalSizeOfProgram];
		m_end = totalSizeOfProgram;
	}
}

void* MemoryManager::Aloc(const size_t numberOfBytes)
{
	std::map<void*, Header>::iterator it = m_memoryMap.begin();
	if (m_memoryMap.size() != 0)
	{
		std::pair<void*, Header> pair;
		bool found = false;
		while ((*it).second.m_pNext == NULL && !found)
		{
			if ((*it).second.isUse == false)
			{

				if (numberOfBytes <= (*it).second.sizeOfBlock)
				{
					pair = *it;
					found = true;
				}
				else
				{
					it++;
				}
			}
			else
			{
				it++;
			}

			if (it == m_memoryMap.end())
			{
				break;
			}
		}


		if (found)
		{
			// Use a already alocated memory

			void* pointer = pair.first;
			Header& header = pair.second;
			header.isUse = true;

			return pointer;
		}
		else
		{
			// Alocate new memory

			return CheckAndAloc(numberOfBytes);
		}
	}
	else
	{
		return CheckAndAloc(numberOfBytes);
	}
}

void* MemoryManager::CheckAndAloc(size_t numberOfBytes)
{
	if (m_current + numberOfBytes <= m_end)
	{
		Header header = Header();
		header.m_pNext = NULL;
		header.isUse = true;
		header.sizeOfBlock = numberOfBytes;
		void* pointer = &m_pMemoryBaseBuffer[m_current];
		m_memoryMap.insert(std::make_pair(pointer, header));
		m_current += numberOfBytes;

		std::map<void*, Header>::iterator it = m_memoryMap.find(pointer);

		if (m_memoryMap.size() > 1)
		{
			it--;
			(*it).second.m_pNext = pointer;
		}

		return pointer;
	}
	else
	{
		return nullptr;
	}
}

void MemoryManager::Free(void** pointerToMemory)
{
	Header& header = m_memoryMap.at(*pointerToMemory);
	header.isUse = false;
	*pointerToMemory = nullptr;
	CheckFragmets();
}

void MemoryManager::CheckFragmets()
{
	if (m_memoryMap.size() > 1)
	{
		std::map<void*, Header>::iterator it = m_memoryMap.begin();
		while ((*it).second.m_pNext != NULL)
		{

			if (!(*it).second.isUse)
			{

				void* pointer2 = (*it).second.m_pNext;
				Header& header2 = m_memoryMap.at(pointer2);

				std::map<void*, Header>::iterator itNext = m_memoryMap.find(pointer2);

				if (itNext != m_memoryMap.end() && !(*itNext).second.isUse)
				{

					// Merge

					Header& header1 = (*it).second;
					void* pointer1 = (*it).first;

					header1.sizeOfBlock += header2.sizeOfBlock;

					header1.m_pNext = header2.m_pNext;

					m_memoryMap.erase(itNext);
					pointer2 = nullptr;
				}
			}

			it++;

			if (it == m_memoryMap.end())
			{
				break;
			}
		}
	}
}
