#pragma once
#include "memory.h"

namespace Hook
{
	bool call_kernel(void* kernel_address);
	NTSTATUS hook_fn(PVOID param);
}
