#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include "util.h"
#include "driver.h"

LPVOID GetDrvBase(
	PCHAR szDriverName
)
{
	LPVOID ImageBasePtr;
	PCHAR szModuleName;
	ULONG StructSize;
	PSYSTEM_MODULE_INFORMATION SystemModInfo;
	PSYSTEM_MODULE_ENTRY       SystemModEntry;
	
	NtQuerySystemInformation(SystemModuleInformation, &StructSize, 0, &StructSize);

	ImageBasePtr = NULL;
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

			if ( strcmp(szModuleName, szDriverName) == 0 ) {
				ImageBasePtr = SystemModEntry[i].ImageBase;
			};
		};
		szModuleName = NULL;
	};

	if ( SystemModInfo != NULL )
		HeapFree(GetProcessHeap(), 0, SystemModInfo);

	return ImageBasePtr;
};

BOOL DumpToFile(
	PCHAR szDumpPath
)
{
	HANDLE hDriverFile = NULL;
	DWORD  NumBytes    = 0;

	hDriverFile = CreateFileA(
		  szDumpPath,
		  GENERIC_READ | GENERIC_WRITE,
		  0,
		  NULL,
		  CREATE_ALWAYS,
		  FILE_ATTRIBUTE_NORMAL,
		  NULL
	);

	if ( hDriverFile != INVALID_HANDLE_VALUE ) {
		WriteFile(
			hDriverFile,
		  	&drvbuf,
		  	34568,
		  	&NumBytes,
			NULL
		);
		CloseHandle(hDriverFile);
		return TRUE;
	};
	return FALSE;
};

PCHAR RandomString(
	INT MaxLength
)
{
	CHAR  szStringName[MaxLength + 1];
	ULONG ulSeedCount;
	PCHAR szStringPtr;

	ulSeedCount = GetTickCount();
	RtlSecureZeroMemory(&szStringName, MaxLength + 1);
	for ( int i = 0 ; i < MaxLength ; i++ )
		szStringName[i] = RtlRandomEx(&ulSeedCount) % 26 + 97;

	szStringPtr = malloc(MaxLength + 1);
	memset(szStringPtr, '\0', MaxLength + 1);
	memcpy(szStringPtr, szStringName, MaxLength);

	return szStringPtr;
};
