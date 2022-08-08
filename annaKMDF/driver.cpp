#include "hook.h"

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	call_kernel(&hook_fn);
	
	DbgPrintEx(0, 0, "[annaKMDF] Driver Loaded...\n");
	return STATUS_SUCCESS;
}
