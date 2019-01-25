#include "TGAWriter.h"

int TGAWriter::littleEndianify(int number)
{
	return number % BYTE_RANGE
}

int TGAWriter::bigEndianify(int number)
{
	return number / BYTE_RANGE;
}

void TGAWriter::writeHeader(Header header, FILE* tgaFile)
{
	fputc(header.idLength, tgaFile);
	fputc(header.mapType, tgaFile);
	fputc(header.imageType, tgaFile);

	fputc(littleEndianify(header.mapFirst), tgaFile);
	fputc(bigEndianify(header.mapFirst), tgaFile);

	fputc(littleEndianify(header.mapLength), tgaFile);
	fputc(bigEndianify(header.mapLength), tgaFile);

	fputc(header.mapEntry, tgaFile);

	fputc(littleEndianify(header.x), tgaFile);
	fputc(bigEndianify(header.x), tgaFile);
	fputc(littleEndianify(header.y), tgaFile);
	fputc(bigEndianify(header.y), tgaFile);

	fputc(littleEndianify(header.width), tgaFile);
	fputc(bigEndianify(header.width), tgaFile);
	fputc(littleEndianify(header.height), tgaFile);
	fputc(bigEndianify(header.height), tgaFile);

	fputc(header.bitsPerPixel, tgaFile);
	fputc(header.misc, tgaFile);
}

void TGAWriter::save(const char* filepath, unsigned char* pixelData, int width, int height)
{
	FILE *tga;                 // pointer to file that we will write
	Header header;       // variable of targa_header type

	header.idLength = 0;
	header.mapType = 0;
	header.imageType = 2;     // uncompressed RGB image

	header.mapFirst = 0;
	header.mapLength = 0;
	header.mapEntry = 0;

	header.x = 0;
	header.y = 0;
	header.width = width;
	header.height = height;

	header.bitsPerPixel = 32;
	header.misc = 0x20;      
							 
	fopen_s(&tga, filepath, "wb");

	writeHeader(header, tga);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			// B G R A order 

			fputc(pixelData[(width * y + x) * 4 + 2], tga);
			fputc(pixelData[(width * y + x) * 4 + 1], tga);
			fputc(pixelData[(width * y + x) * 4 + 0], tga);
			fputc(pixelData[(width * y + x) * 4 + 3], tga);
		}
	}

	fclose(tga);
}
