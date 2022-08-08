#pragma once
#include "definitions.h"

PVOID get_module_base(const char* module_name);
PVOID get_module_export(const char* module_name, LPCSTR export_name);
bool write(void* address, void* buffer, size_t size);
bool write_safe(void* address, void* buffer, size_t size);

