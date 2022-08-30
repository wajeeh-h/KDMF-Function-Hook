#pragma once
#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include <windef.h>
#include <ntstrsafe.h>
#include <wdm.h>

namespace Definitions 
{
	constexpr auto CODE_CLIENT_REQUEST = 0x1;;
	constexpr auto CODE_READ_MEMORY = 0x2;
	constexpr auto CODE_WRITE_MEMORY = 0x3;

	typedef enum _SYSTEM_INFORMATION_CLASS
	{
		SystemBasicInformation,
		SystemProcessorInformation,
		SystemPerformanceInformation,
		SystemTimeOfDayInformation,
		SystemPathInformation,
		SystemProcessInformation,
		SystemCallCountInformation,
		SystemDeviceInformation,
		SystemProcessorPerformanceInformation,
		SystemFlagsInformation,
		SystemCallTimeInformation,
		SystemModuleInformation,
		SystemLocksInformation,
		SystemStackTraceInformation,
		SystemPagedPoolInformation,
	} SYSTEM_INFORMATION_CLASS, * PSYSTEM_INFORMATION_CLASS;

	typedef struct _PEB_LDR_DATA32
	{
		ULONG Length;
		UCHAR Initialized;
		ULONG SsHandle;
		LIST_ENTRY32 InLoadOrderModuleList;
		LIST_ENTRY32 InMemoryOrderModuleList;
		LIST_ENTRY32 InInitializationOrderModuleList;
	} PEB_LDR_DATA32, * PPEB_LDR_DATA32;

	typedef struct _LDR_DATA_TABLE_ENTRY32
	{
		LIST_ENTRY32 InLoadOrderLinks;
		LIST_ENTRY32 InMemoryOrderLinks;
		LIST_ENTRY32 InInitializationOrderLinks;
		ULONG DllBase;
		ULONG EntryPoint;
		ULONG SizeOfImage;
		UNICODE_STRING32 FullDllName;
		UNICODE_STRING32 BaseDllName;
		ULONG Flags;
		USHORT LoadCount;
		USHORT TlsIndex;
		LIST_ENTRY32 HashLinks;
		ULONG TimeDateStamp;
	} LDR_DATA_TABLE_ENTRY32, * PLDR_DATA_TABLE_ENTRY32;

	typedef struct _PEB32 {
		UCHAR InheritedAddressSpace;
		UCHAR ReadImageFileExecOptions;
		UCHAR BeingDebugged;
		UCHAR Spare;
		ULONG Mutant;
		ULONG ImageBaseAddress;
		ULONG/*PPEB_LDR_DATA32*/ Ldr;
	} PEB32, * PPEB32;

	typedef struct _RTL_PROCESS_MODULE_INFORMATION
	{
		HANDLE Section;
		PVOID MappedBase;
		PVOID ImageBase;
		ULONG ImageSize;
		ULONG Flags;
		USHORT LoadOrderIndex;
		USHORT InitOrderIndex;
		USHORT LoadCount;
		USHORT OffsetToFileName;
		UCHAR FullPathName[256];
	} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

	typedef struct _RTL_PROCESS_MODULES
	{
		ULONG NumberOfModules;
		RTL_PROCESS_MODULE_INFORMATION Modules[1];
	} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

	typedef struct _INFO_STRUCT
	{
		ULONG code;
		ULONG process_id;
		ULONG client_base;
		ULONG address;
		ULONG buffer;
		ULONG size;
	}INFO_STRUCT, * PINFO_STRUCT;

	extern "C" __declspec(dllimport)
		NTSTATUS NTAPI ZwProtectVirtualMemory(
			HANDLE ProcessHandle,
			PVOID * BaseAddress,
			PULONG NumberOfBytesToProtect,
			ULONG NewAccessProtection,
			PULONG OldAccessProtection
		);

	extern "C" NTKERNELAPI
		PVOID NTAPI RtlFindExportedRoutineByName(
			_In_ PVOID ImageBase,
			_In_ PCCH RoutineName
		);

	extern "C" NTSTATUS ZwQuerySystemInformation(
		ULONG InfoClass,
		PVOID Buffer,
		ULONG Length,
		PULONG ReturnLength
	);

	extern "C" NTKERNELAPI
		PPEB PsGetProcessPeb(
			_In_ PEPROCESS Process
		);

	extern "C" NTKERNELAPI
		PVOID PsGetProcessWow64Process(
			__in PEPROCESS Process
		);

	extern "C" NTSTATUS NTAPI MmCopyVirtualMemory(
		PEPROCESS FromProcess,
		PVOID FromAddress,
		PEPROCESS ToProcess,
		PVOID ToAddress,
		SIZE_T BufferSize,
		KPROCESSOR_MODE PreviousMode,
		_Out_ PSIZE_T NumberOfBytesCopied
	);
}
