#ifndef BMP_FILE_LOADER_H
#define BMP_FILE_LOADER_H
/* ========================================================================
$File: BMPFileLoader.h $
$Date: 07-28-16 $
$Revision: 08-6-16 $
$Creator: George Wulfers $
$Notice: (C) Copyright 2016 by WulfersGames, Inc. All Rights Reserved. $
======================================================================== */

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