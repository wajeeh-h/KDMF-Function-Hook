#include "hook.h"

bool call_kernel(void* kernel_address)
{
	if (!kernel_address)
		return false;

	void* function = reinterpret_cast<PVOID*>(get_module_export("\\SystemRoot\\System32\\drivers\\dxgkrnl.sys", "NtQueryCompositionSurfaceStatistics"));

	if (!function) 
	{
		DbgPrintEx(0, 0, "[annaKMDF] ERROR - Could Not Find Function\n");
		return false;
	}

	// 12 Bytes
	BYTE origin_fn[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	// Shell Code
	BYTE mov_rax[] = { 0x48, 0xB8 };
	BYTE jmp_rax[] = { 0xFF, 0xE0 };

	// Copy the instructions and function address to the shell code arr (origin_fn)
	RtlSecureZeroMemory(&origin_fn, sizeof(origin_fn));
	memcpy((PVOID)(ULONG_PTR)origin_fn, &mov_rax, sizeof(mov_rax));

	uintptr_t hook_address = reinterpret_cast<uintptr_t>(kernel_address);
	memcpy((PVOID)((ULONG_PTR)origin_fn + sizeof(mov_rax)), &hook_address, sizeof(void*));
	memcpy((PVOID)((ULONG_PTR)origin_fn + sizeof(mov_rax) + sizeof(void*)), &jmp_rax, sizeof(jmp_rax));
	
	// Inject the shell code and hook the function
	write_safe(function, &origin_fn, sizeof(origin_fn));
	return true;
}

NTSTATUS hook_fn(PVOID param)
{
	return STATUS_SUCCESS;
}
