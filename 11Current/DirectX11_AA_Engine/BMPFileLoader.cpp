/* ========================================================================
$File: BMPFileLoader.cpp $
$Date: 07-28-16 $
$Revision: 08-6-16 $
$Creator: George Wulfers $
$Notice: (C) Copyright 2016 by WulfersGames, Inc. All Rights Reserved. $
======================================================================== */

#include "BMPFileLoader.h"

// Follow link for more info: http://www.fileformat.info/format/bmp/egff.htm
// IMPORTANT: This struct must be 1 byte aligned !!!
// For more info see : https://msdn.microsoft.com/en-us/library/2e70t5y1.aspx
#pragma pack(push, 1)
struct BitmapHeader
{
	u16 FileType;			/*	File type always 4D42h ("BM")	*/
	u32 FileSize;			/*	Size of the file in bytes	*/
	u16 Reserved1;			/*	Always 0	*/
	u16 Reserved2;			/*	Always 0	*/
	u32 BitmapOffset;		/*	Starting position of image data in bytes	*/

	u32 Size;				/*	Size of this header in bytes	*/
	i32 Width;				/*	Image width in pixels	*/
	i32 Height;				/*	Image Height in pixels	*/
	u16 Planes;				/*	Number of color planes	*/
	u16 BitsPerPixel;		/*	Number of bits per pixel	*/

	/* Fields added for Windows 3.x follow this line */
	u32 Compression;		/*	Compression methods used	*/
	u32 SizeOfBitmap;		/*	Size of bitmap in bytes	*/
	i32 HorzResolution;		/*	Horizontal resolution in pixels per meter	*/
	i32 VertResolution;		/*	Vertical resolution in pixels per meter	*/
	u32 ColorsUsed;			/*	Number of colors in the image	*/
	u32 ColorsImportant;	/*	Minimum number of important colors	*/

	u32 RedMask;			/*	Mask identifying bits of red component	*/
	u32 GreenMask;			/*	Mask identifying bits of green component	*/
	u32 BlueMask;			/*	Mask identifying bits of blue component	*/
};
#pragma pack(pop)

//NOTE: Shift the RR and AA for our format.
/* If you are using this generically for some reason,
Please remember that BMP files CAN GO IN EITHER DIRECTION and
the height will be negative for top-down.
(Also, there can be compression, etc, etc.. Dont think this
is a complete BMP loading code because it isnt!!)
*/
BitmapImage LoadBMP(char* filename)
{
	BitmapImage result = {};

	FileResult readResult = ReadEntireFile(filename);

	if (readResult.fileSize != 0)
	{
		BitmapHeader* bmpHeader = (BitmapHeader*)readResult.data;

		u32* pixels = (u32*)((u8*)readResult.data + bmpHeader->BitmapOffset);
		result.pixels = pixels;
		result.width = bmpHeader->Width;
		result.height = bmpHeader->Height;
	}

	return result;
}