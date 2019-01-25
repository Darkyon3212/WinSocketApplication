#ifndef __MEMORYMANAGER_H__
#define __MEMORYMANAGER_H__

#include <map>

#include "Singleton.hpp"

class MemoryManager : public Singleton<MemoryManager>
{
public:

	MemoryManager();
	~MemoryManager();

	struct Header
	{
		void* m_pNext;
		bool isUse;
		size_t sizeOfBlock;
	};

	void AlocateBaseMemory(const size_t totalSizeOfProgram);
	void* Aloc(const size_t numberOfBytes);
	void Free(void** pointerToMemory);
	void CheckFragmets();
	void* CheckAndAloc(size_t numberOfBytes);

private:
	char* m_pMemoryBaseBuffer;
	unsigned int m_current;
	unsigned int m_end;
	std::map<void*, Header> m_memoryMap;
};


#endif // __MEMORYMANAGER_H__
