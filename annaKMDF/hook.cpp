#include "hook.h"

bool Hook::call_kernel(void* kernel_address)
{
	if (!kernel_address)
		return false;

	void* function = reinterpret_cast<PVOID*>(Memory::get_module_export("\\SystemRoot\\System32\\drivers\\dxgkrnl.sys", "NtQueryCompositionSurfaceStatistics"));

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
	Memory::write_safe(function, &origin_fn, sizeof(origin_fn));
	return true;
}

NTSTATUS Hook::hook_fn(PVOID param)
{
	Definitions::PINFO_STRUCT process_info = (Definitions::PINFO_STRUCT)param;

	if (process_info->code == Definitions::CODE_CLIENT_REQUEST)
	{
		PEPROCESS target_process = NULL;
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)process_info->process_id, &target_process)))
		{
			KAPC_STATE apc;
			KeStackAttachProcess(target_process, &apc);
			ULONG _export = Memory::export_module_base(target_process, L"client.dll");
			KeUnstackDetachProcess(&apc);
			if (_export) 
			{ 
				process_info->client_base = _export; 
			}
		}
	}
	else if (process_info->code == Definitions::CODE_READ_MEMORY)
	{
		PEPROCESS target_process = NULL;
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)process_info->process_id, &target_process)))
		{
			Memory::read_process_memory(target_process, (void*)process_info->address, &process_info->buffer, process_info->size);
		}
	}
	else if (process_info->code == Definitions::CODE_WRITE_MEMORY)
	{
		PEPROCESS target_process = NULL;
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)process_info->process_id, &target_process)))
		{
			Memory::write_process_memory(target_process, &process_info->buffer, (void*)process_info->address, process_info->size);
		}
	}
	return STATUS_SUCCESS;
}
