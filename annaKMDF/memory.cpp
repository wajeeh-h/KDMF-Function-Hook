#include "memory.h"

PVOID Memory::get_module_base(const char* module_name)
{
	ULONG size = 0;
	NTSTATUS status = Definitions::ZwQuerySystemInformation(Definitions::SystemModuleInformation, NULL, size, &size);
	if (!size)
	{
		DbgPrintEx(0, 0, "[annaKMDF] ERROR - Unexpected Result From ZwQueryStstemInformation\n");
		return NULL;
	}

	Definitions::PRTL_PROCESS_MODULES processes = (Definitions::PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, size, 0x4e425151);
	status = Definitions::ZwQuerySystemInformation(Definitions::SystemModuleInformation, processes, size, &size);
	Definitions::PRTL_PROCESS_MODULE_INFORMATION process = processes->Modules;

	if (!NT_SUCCESS(status))
	{
		DbgPrintEx(0, 0, " [annaKMDF] ERROR - Unexpected Result From ZwQueryStstemInformation\n");
		return NULL;
	}
	
	PVOID module_base = 0;

	for (ULONG i = 0; i < processes->NumberOfModules; i++)
	{
		if (strcmp((char*)process[i].FullPathName, module_name) == 0)
		{
			module_base = process[i].ImageBase;
			break;
		}
	}
	if (processes)
		ExFreePoolWithTag(processes, 0x4e425151);

	if (module_base <= NULL)
	{
		DbgPrintEx(0, 0, "[annaKMDF] ERROR - Invalid Module Base Address\n");
		return NULL;
	}

	return module_base;
}

PVOID Memory::get_module_export(const char* module_name, LPCSTR routine_name)
{
	PVOID lp_module = get_module_base(module_name);
	if (!lp_module)
	{
		DbgPrintEx(0, 0, "[annaKMDF] ERROR - Invalid Module Base Address\n");
		return NULL;
	}
	return Definitions::RtlFindExportedRoutineByName(lp_module, routine_name);
}

bool Memory::write(void* address, void* buffer, size_t size)
{
	if (!RtlCopyMemory(address, buffer, size))
	{
		DbgPrintEx(0, 0, "[annaKMDF] ERROR - Could Not Write Memory\n");
		return false;
	}
	return true;
}

bool Memory::write_safe(void* address, void* buffer, size_t size)
{
	PMDL Mdl = IoAllocateMdl(address, size, FALSE, FALSE, NULL);
	if (!Mdl)
	{
		DbgPrintEx(0, 0, "[annaKMDF] ERROR - Could Not Write (Safe) Memrory \n");
		return false;
	}

	MmProbeAndLockPages(Mdl, KernelMode, IoReadAccess);
	PVOID Mapping = MmMapLockedPagesSpecifyCache(Mdl, KernelMode, MmNonCached, NULL, FALSE, NormalPagePriority);
	MmProtectMdlSystemAddress(Mdl, PAGE_READWRITE);
	memcpy(Mapping, buffer, size);

	MmUnmapLockedPages(Mapping, Mdl);
	MmUnlockPages(Mdl);
	IoFreeMdl(Mdl);
	return true;
}

bool Memory::read_process_memory(PEPROCESS target_process, void* source, void* target, size_t size) 
{	
	if (!target_process) {
		return false;
	}
	size_t bytes = 0;
	bool status = Definitions::MmCopyVirtualMemory(target_process, source, IoGetCurrentProcess(), target, size, KernelMode, &bytes);
	if (!status || !bytes) {
		return false;
	}
	return true;
}

bool Memory::write_process_memory(PEPROCESS target_process, void* source, void* target, size_t size)
{
	if (!target_process) 
	{ 
		return false; 
	}
	size_t bytes = 0;
	NTSTATUS status = Definitions::MmCopyVirtualMemory(IoGetCurrentProcess(), source, target_process, target, size, KernelMode, &bytes);
	if (!NT_SUCCESS(status) || !bytes)
	{
		return false;
	}
	return true;
}

ULONG Memory::export_module_base(PEPROCESS process, LPCWSTR module_name)
{
	if (!process) { return 0; }

	__try
	{
		Definitions::PPEB32 peb32 = (Definitions::PPEB32)Definitions::PsGetProcessWow64Process(process);
		if (!peb32 || !peb32->Ldr) { return 0; }
		
		for (PLIST_ENTRY32 plist_entry = (PLIST_ENTRY32)((Definitions::PPEB_LDR_DATA32)peb32->Ldr)->InLoadOrderModuleList.Flink;
			plist_entry != &((Definitions::PPEB_LDR_DATA32)peb32->Ldr)->InLoadOrderModuleList;
			plist_entry = (PLIST_ENTRY32)plist_entry->Flink)
		{
			Definitions::PLDR_DATA_TABLE_ENTRY32 pentry = CONTAINING_RECORD(plist_entry, Definitions::LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);

			if (wcscmp((PWCH)pentry->BaseDllName.Buffer, module_name) == 0)
			{
				return pentry->DllBase;
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrintEx(0, 0, "[annaKMDF] ERROR - Could Not Find Process \n");
	}
	return 0;
}