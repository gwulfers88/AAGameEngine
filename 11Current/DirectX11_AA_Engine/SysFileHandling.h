#ifndef SYS_FILE_HANDLING_H
#define SYS_FILE_HANDLING_H
/* ========================================================================
$File: SysFileHandling.h $
$Date: 07-28-16 $
$Revision: 08-6-16 $
$Creator: George Wulfers $
$Notice: (C) Copyright 2016 by WulfersGames, Inc. All Rights Reserved. $
======================================================================== */

#include "constants.h"

// Structure used to read in and write out data.
struct FileResult
{
	void* data;
	u32 fileSize;
};

//These should get their own header file
inline void FreeMemory(void* data)
{
	if (data)
	{
		VirtualFree(data, 0, MEM_RELEASE);
		data = 0;
	}
}

inline void* AllocMemory(u32 size)
{
	return VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

inline FileResult ReadEntireFile(i8* filename)
{
	FileResult result = {};

	HANDLE filehandle = CreateFile(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

	if (filehandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER filesize;

		if (GetFileSizeEx(filehandle, &filesize))
		{
			u32 size = filesize.QuadPart;
			result.data = AllocMemory(size);

			if (result.data)
			{
				DWORD bytesRead = 0;

				if (ReadFile(filehandle, result.data, size, &bytesRead, 0) && size == bytesRead)
				{
					result.fileSize = bytesRead;
				}
			}
		}

		CloseHandle(filehandle);
	}

	return result;
}

inline b32 WriteEntireFile(i8* filename, FileResult fileResult)
{
	HANDLE filehandle = CreateFile(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);

	if (filehandle != INVALID_HANDLE_VALUE)
	{
		DWORD bytesWritten = 0;
		WriteFile(filehandle, fileResult.data, fileResult.fileSize, &bytesWritten, 0);
		CloseHandle(filehandle);
	}
	else
	{
		// LOGGING
		return false;
	}

	return true;
}

#endif