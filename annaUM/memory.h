#pragma once
#include "definitions.h"
#include <TlHelp32.h>

namespace Memory
{
	ULONG get_process_id(const char* process_name);
	wchar_t* charToWChar(const char* text);
}

