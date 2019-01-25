#ifndef __TGAJOB_H__
#define __TGAJOB_H__

#include "ThreadManager.h"

extern "C"
{
	#include "tga_reader.h"
}

class TGAJob : public ThreadManager::Job
{
public:
	TGAJob(unsigned char* pImageBuffer, unsigned int chunkID, unsigned int xOffset, unsigned int yOffset, unsigned int width, unsigned int height);
	~TGAJob();

	void ExecuteJob() override;
	void Convert() override;

	unsigned char* GetChunkPixels(unsigned char* entirePixelArray, const int entireWidth, const int entireHeight);
	unsigned char* ResizeChunk(unsigned char* chunkPixelArray, const int newWidth, const int newheight);

private:
	unsigned char* m_pImageBuffer;
	unsigned int m_chunkID;
	unsigned char* savedPixelArray;

	unsigned int m_xOffset;
	unsigned int m_yOffset;

	unsigned int m_width;
	unsigned int m_height;
};

#endif // !__TGAJOB_H__

