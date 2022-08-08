#pragma once
#include "memory.h"

bool call_kernel(void* kernel_address);
NTSTATUS hook_fn(PVOID param);
