#pragma once
#include <Windows.h>

namespace Definitions 
{
	constexpr auto CODE_CLIENT_REQUEST = 0x1;;
	constexpr auto CODE_READ_MEMORY = 0x2;
	constexpr auto CODE_WRITE_MEMORY = 0x3;

	typedef struct _INFO_STRUCT
	{
		ULONG code;
		ULONG process_id;
		ULONG client_base;
		ULONG address;
		ULONG buffer;
		ULONG size;
	}INFO_STRUCT, * PINFO_STRUCT;
}