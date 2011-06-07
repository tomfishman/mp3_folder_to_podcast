// ZunePodcastMaker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void set_file_time(TCHAR *file_name, FILETIME *file_time)
{
	HANDLE handle= CreateFile(file_name,  GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFileTime(handle, file_time, file_time, file_time);
	CloseHandle(handle);
}

void walk_time(FILETIME *file_time)
{
	ULARGE_INTEGER time;
	time.LowPart= file_time->dwLowDateTime;
	time.HighPart= file_time->dwHighDateTime;

	ULONGLONG one_hundred_nano_secs_a_day= 24L*60L*60L*(ULONGLONG)10000000;
	time.QuadPart+= one_hundred_nano_secs_a_day;

	file_time->dwLowDateTime= time.LowPart;
	file_time->dwHighDateTime= time.HighPart;
}

int _tmain(int argc, _TCHAR* argv[])
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	FILETIME file_time;
	GetSystemTimeAsFileTime(&file_time);

	if( argc == 2 )
	{
		_tprintf (TEXT("Target file is %s\n"), argv[1]);

		hFind = FindFirstFile(argv[1], &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE) 
		{
			do 
			{
				_tprintf (TEXT("processing %s\n"), FindFileData.cFileName);
				set_file_time(FindFileData.cFileName, &file_time);
				walk_time(&file_time);
			} while (FindNextFile(hFind, &FindFileData));

			FindClose(hFind);
		}
		else 
		{
			printf ("FindFirstFile failed (%d)\n", GetLastError());
		} 
	}
	else
	{
		_tprintf(TEXT("Usage: %s [target_file]\n"), argv[0]);
	}

	return 0;
}

