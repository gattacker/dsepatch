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
