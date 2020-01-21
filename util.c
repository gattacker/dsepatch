#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include "util.h"
#include "intel.h"

LPVOID KeGetBase(
	PCHAR szDriverName,
	DWORD *SizeOfImage
)
{
	LPVOID ImageBasePtr;
	PCHAR szModuleName;
	ULONG StructSize;
	PSYSTEM_MODULE_INFORMATION SystemModInfo;
	PSYSTEM_MODULE_ENTRY       SystemModEntry;
	
	if ( *SizeOfImage != 0 )
		*SizeOfImage = 0;

	NtQuerySystemInformation(SystemModuleInformation, &StructSize, 0, &StructSize);

	if ( (SystemModInfo = HeapAlloc(
			GetProcessHeap(), 
			HEAP_ZERO_MEMORY, 
			StructSize)
	     ) != NULL 
	   ) { 
		NtQuerySystemInformation(
			SystemModuleInformation, 
			SystemModInfo, 
			StructSize, 
			0
		);
		SystemModEntry = SystemModInfo->Module;
		for ( int i = 0 ; i < SystemModInfo->Count ; i++ )
		{
			szModuleName = SystemModEntry[i].FullPathName
				+ SystemModEntry[i].OffsetToFileName;

			if ( strcmp(szModuleName, szDriverName) == 0 )
			{
				*SizeOfImage = SystemModEntry[i].ImageSize;
				ImageBasePtr = SystemModEntry[i].ImageBase;
			};
		};
		szModuleName = NULL;
	};

	if ( SystemModInfo != NULL )
		HeapFree(GetProcessHeap(), 0, SystemModInfo);

	return ImageBasePtr;
};

LPVOID KeGetExport(
	LPVOID ImageBase
)
{
	HANDLE                  hDriver;
	IMAGE_DOS_HEADER        DosHdr;
	IMAGE_NT_HEADERS        NtHdr;
	PIMAGE_EXPORT_DIRECTORY Export;
	DWORD                   ExpOff;
	DWORD                   ExpLen;
	ULONG32                *FunPtr;
	ULONG32                *StrPtr;
	USHORT                 *OrdPtr;

	RtlSecureZeroMemory(&DosHdr, sizeof(IMAGE_DOS_HEADER));
	RtlSecureZeroMemory(&NtHdr, sizeof(IMAGE_NT_HEADERS));
	RtlSecureZeroMemory(&Export, sizeof(IMAGE_EXPORT_DIRECTORY));

	hDriver = GetHandle();
	if ( hDriver != INVALID_HANDLE_VALUE )
	{
		if ( MemCpy(
			hDriver,
			(ULONG_PTR)&DosHdr,
			(ULONG_PTR)ImageBase,
			sizeof(IMAGE_DOS_HEADER)
		     ) != TRUE 
		   ) 
		{
			printf("[ ] KeDumpImage: 0x%x\n", GetLastError());
			goto Cleanup;
		};

		if ( MemCpy(
			hDriver, 
			(ULONG_PTR)&NtHdr,
			(ULONG_PTR)(ImageBase + DosHdr.e_lfanew),
			sizeof(IMAGE_NT_HEADERS)
		     ) != TRUE
		   )
		{
			printf("[ ] KeDumpImage : 0x%x\n", GetLastError());
			goto Cleanup;
		};

		ExpOff = NtHdr.OptionalHeader.DataDirectory[0].VirtualAddress;
		ExpLen = NtHdr.OptionalHeader.DataDirectory[0].Size;

		Export = HeapAlloc
	 	( 
			GetProcessHeap(),
			HEAP_ZERO_MEMORY,
			ExpLen
		);

		if ( MemCpy(
			hDriver,
			(ULONG_PTR)Export,
			(ULONG_PTR)(ImageBase + ExpOff),
			ExpLen
		     ) != TRUE
		   ) 
		{
			printf("[ ] KeDumpImage : 0x%x\n", GetLastError());
			goto Cleanup;
		};
	} else {
		printf("[ ] KeDumpImage : 0x%x\n", GetLastError());
	};
Cleanup:
	if ( hDriver != INVALID_HANDLE_VALUE )
		CloseHandle(hDriver);

	if ( Export != NULL )
		HeapFree(GetProcessHeap(), 0, Export);

	return NULL;
};
