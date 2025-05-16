#pragma once
#include "stb_image.h"

class ImageLoaderSTB
{
public:
	void Load(const char* file, int* width, int* height, int* nrComponents);
	unsigned char* GetData();
	void Free();
private:
	unsigned char* data;
};

