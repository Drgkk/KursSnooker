#include "ImageLoaderSTB.h"

void ImageLoaderSTB::Load(const char* file, int* width, int* height, int* nrComponents)
{
	data = stbi_load(file, width, height, nrComponents, 0);
}

unsigned char* ImageLoaderSTB::GetData()
{
	return this->data;
}

void ImageLoaderSTB::Free()
{
	stbi_image_free(data);
}
