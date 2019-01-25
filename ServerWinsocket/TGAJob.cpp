#define _CRT_SECURE_NO_DEPRECATE

static const int colorChannels = 4;
static const int sourceWidth = 256;
static const int sourceHeight = 256;

#include <string>
#include <sstream>

#include "TGAJob.h"
#include "TGAWriter.h"

TGAJob::TGAJob(unsigned char* pImageBuffer, unsigned int chunkID, unsigned int xOffset, unsigned int yOffset, unsigned int width, unsigned int height)
	: m_pImageBuffer(pImageBuffer)
	, m_chunkID(chunkID)
	, m_xOffset(xOffset)
	, m_yOffset(yOffset)
	, m_width(width)
	, m_height(height)
{

}

TGAJob::~TGAJob()
{
	//delete[] m_pImageBuffer;
}

void TGAJob::ExecuteJob()
{
	// Gets pixels of file with ARGB format
	// pixels[0] = first pixel alpha component
	// pixels[1] = first pixel red component
	// pixels[2] = first pixel green component
	// pixels[3] = first pixel blue component

	// A R G B
	unsigned char* entirePixels = (unsigned char*)tgaRead(m_pImageBuffer, TGA_READER_ABGR);

	unsigned char* chunkPixels = GetChunkPixels(entirePixels, sourceWidth, sourceHeight);
	savedPixelArray = ResizeChunk(chunkPixels, sourceWidth, sourceHeight);
	delete[] chunkPixels;
}

void TGAJob::Convert()
{
	ThreadManager::GetInstance().mutex.lock();

	std::ostringstream stream;
	stream << "images/image";
	stream << m_chunkID;
	stream << ".tga";
	std::string path = stream.str();

	TGAWriter::GetInstance().save(path.c_str(), savedPixelArray, 256, 256);

	delete[] savedPixelArray;

	ThreadManager::GetInstance().mutex.unlock();
}

unsigned char* TGAJob::GetChunkPixels(unsigned char* entirePixelArray, const int entireWidth, const int entireHeight)
{
	const int size = m_width * m_height * colorChannels;
	unsigned char* pixels = new unsigned char[size];

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int ReadStride = (entireWidth * (y + m_yOffset) + (x + m_xOffset)) * colorChannels;
			int WriteStride = (m_width * y + x) * colorChannels;
			pixels[WriteStride + 0] = entirePixelArray[ReadStride + 0];
			pixels[WriteStride + 1] = entirePixelArray[ReadStride + 1];
			pixels[WriteStride + 2] = entirePixelArray[ReadStride + 2];
			pixels[WriteStride + 3] = entirePixelArray[ReadStride + 3];
		}
	}

	return pixels;
}

unsigned char* TGAJob::ResizeChunk(unsigned char* chunkPixelArray, const int newWidth, const int newHeight)
{
	const int size = m_width * (newWidth / m_width) * m_height * (newHeight / m_height) * colorChannels;
	unsigned char* destPixels = new unsigned char[size];

	for (int oldY = 0; oldY < m_width; oldY++)
	{
		for (int oldX = 0; oldX < m_width; oldX++)
		{
			for (int newY = 0; newY < m_width; newY++)
			{
				for (int newX = 0; newX < m_width; newX++)
				{
					// A B G R
					int DestStride = (newWidth * (newY + oldY * 16) + (newX + oldX * 16)) * colorChannels;
					int SourceStride = (m_width * oldY + oldX) * colorChannels;

					destPixels[DestStride + 0] = chunkPixelArray[SourceStride + 0];
					destPixels[DestStride + 1] = chunkPixelArray[SourceStride + 1];
					destPixels[DestStride + 2] = chunkPixelArray[SourceStride + 2];
					destPixels[DestStride + 3] = chunkPixelArray[SourceStride + 3];
				}
			}
		}
	}

	return destPixels;
}

