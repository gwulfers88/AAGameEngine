#ifndef BMP_FILE_LOADER_H
#define BMP_FILE_LOADER_H

#include "constants.h"
#include "SysFileHandling.h"

struct BitmapImage
{
	i32 width;
	i32 height;
	u32* pixels;
};

BitmapImage LoadBMP(char* filename);

#endif