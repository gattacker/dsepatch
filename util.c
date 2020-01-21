#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winternl.h>
#include "util.h"

LPVOID KeGetBase(
	PCHAR szDriverName,
	DWORD *SizeOfImage
)
{
	PVOID ImageBasePtr;
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
			szModuleName = (PCHAR)(SystemModEntry->FullPathName 
				+ SystemModEntry->OffsetToFileName);

			if ( strcmp(szModuleName, szDriverName) )
			{
				*SizeOfImage = SystemModEntry->ImageSize;
				ImageBasePtr = SystemModEntry->ImageBase;
			};
		};
		szModuleName = NULL;
	};

	if ( SystemModInfo != NULL )
		HeapFree(GetProcessHeap(), 0, SystemModInfo);

	return ImageBasePtr;
};
