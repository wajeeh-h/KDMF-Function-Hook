#pragma warning(disable:4996)
#include "memory.h"

wchar_t* Memory::charToWChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs(wa, text, size);
	return wa;
}

ULONG Memory::get_process_id(const char* process_name)
{
	ULONG process_id = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (snapshot)
	{
		PROCESSENTRY32 process_entry;
		process_entry.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(snapshot, &process_entry))
		{
			do
			{
				if (!_wcsicmp(process_entry.szExeFile, charToWChar(process_name)))
				{
					process_id = process_entry.th32ProcessID;
					break;
				}
			} while (Process32Next(snapshot, &process_entry));
		}
		CloseHandle(snapshot);
	}
	return process_id;
}
