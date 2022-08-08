#include "memory.h"

PVOID get_module_base(const char* module_name)
{
	ULONG size = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, NULL, size, &size);
	if (!size)
	{
		DbgPrintEx(0, 0, "[annaKMDF] ERROR - Unexpected Result From ZwQueryStstemInformation\n");
		return NULL;
	}

	PRTL_PROCESS_MODULES processes = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, size, 0x4e425151);
	status = ZwQuerySystemInformation(SystemModuleInformation, processes, size, &size);
	PRTL_PROCESS_MODULE_INFORMATION process = processes->Modules;

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

PVOID get_module_export(const char* module_name, LPCSTR routine_name)
{
	PVOID lp_module = get_module_base(module_name);
	if (!lp_module)
	{
		DbgPrintEx(0, 0, "[annaKMDF] ERROR - Invalid Module Base Address\n");
		return NULL;
	}
	return RtlFindExportedRoutineByName(lp_module, routine_name);
}

bool write(void* address, void* buffer, size_t size)
{
	if (!RtlCopyMemory(address, buffer, size))
	{
		DbgPrintEx(0, 0, "[annaKMDF] ERROR - Could Not Write Memory\n");
		return false;
	}
	return true;
}

bool write_safe(void* address, void* buffer, size_t size)
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