#pragma once
#include "definitions.h"

namespace Memory 
{
	PVOID get_module_base(const char* module_name);
	PVOID get_module_export(const char* module_name, LPCSTR export_name);
	ULONG export_module_base(PEPROCESS process, LPCWSTR module_name);
	bool write(void* address, void* buffer, size_t size);
	bool write_safe(void* address, void* buffer, size_t size);
	bool read_process_memory(PEPROCESS target_process, void* source, void* target, size_t size);
	bool write_process_memory(PEPROCESS target_process, void* source, void* target, size_t size);
}

