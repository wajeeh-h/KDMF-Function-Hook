#pragma once
#include "memory.h"
#include "hook.h"

namespace Driver 
{
	ULONG get_client_base(ULONG process_id);

	template<typename t>
	t read_memory(ULONG process_id, ULONG address);

	template<typename t>
	void write_memory(ULONG process_id, ULONG address, t buffer);

	template<typename t>
	t read_memory(ULONG process_id, ULONG address)
	{
		Definitions::INFO_STRUCT info = { 0 };
		info.code = Definitions::CODE_READ_MEMORY;
		info.process_id = process_id;
		info.address = address;
		info.size = sizeof(t);

		Hook::run_hook(&info);

		return *(t*)&info.buffer;
	}

	template<typename t>
	void write_memory(ULONG process_id, ULONG address, t buffer)
	{
		Definitions::INFO_STRUCT info = { 0 };
		info.code = Definitions::CODE_WRITE_MEMORY;
		info.process_id = process_id;
		info.address = address;
		info.buffer = buffer;
		info.size = sizeof(t);

		Hook::run_hook(&info);
	}
}