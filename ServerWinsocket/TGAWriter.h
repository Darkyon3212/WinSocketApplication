#pragma once

#include <stdio.h>

#include "Singleton.hpp"

#define BYTE_RANGE 256;

class TGAWriter : public Singleton<TGAWriter>
{
public:
	struct Header {
		char idLength;
		char mapType;
		char imageType;

		int mapFirst;
		int mapLength;
		char mapEntry;

		int x;
		int y;

		int width;
		int height;

		char bitsPerPixel;

		char misc;
	};

	void save(const char* filepath, unsigned char* pixelData, int wdith, int height);

private:
	int littleEndianify(int number);
	int bigEndianify(int number);
	void writeHeader(Header header, FILE* tgaFile);
};

