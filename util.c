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

LPVOID KeDumpImage(
	LPVOID ImageBase,
	DWORD ImageSize
)
{
	LPVOID           lpMemoryImage = NULL;
	HANDLE           hDriver       = NULL;

	IMAGE_DOS_HEADER DosHdr;
	IMAGE_NT_HEADERS NtHdr;

	RtlSecureZeroMemory(&DosHdr, sizeof(IMAGE_DOS_HEADER));
	RtlSecureZeroMemory(&NtHdr, sizeof(IMAGE_NT_HEADERS));

	hDriver = GetHandle();
	if ( hDriver != INVALID_HANDLE_VALUE )
	{
		LPVOID pMemoryPtr;
		LPVOID pImageEnd;
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

		printf("[+] %p image size @ %i\n", ImageBase, ImageSize);

		lpMemoryImage = HeapAlloc(GetProcessHeap(),
				HEAP_ZERO_MEMORY, ImageSize);

		if ( lpMemoryImage != NULL ) {
		  if ( GetPhys(
		         hDriver,
			 (ULONG_PTR)lpMemoryImage,
			 (PULONG_PTR)&pMemoryPtr
		       ) != TRUE 
		     )
		  {
			  printf("[ ] KeDumpImage: GetPhys() = 0x%x\n",
					  GetLastError());
			  goto Cleanup;
		  };

		  pImageEnd = (LPVOID)(ImageBase + ImageSize);

		  printf("[+] Image End %p\n", pImageEnd);

		  printf("[+] Virtual : %p, Physical: %p\n",
				  lpMemoryImage, pMemoryPtr);

		  if ( MemCpy(
		         hDriver,
			 (ULONG_PTR)lpMemoryImage,
			 (ULONG_PTR)ImageBase,
			 ImageSize
		       ) != TRUE
		     )
		  {
			  printf("[ ] KeDumpImage: MemCpy() = 0x%x\n",
					  GetLastError());
			  goto Cleanup;
		  };

		  printf("[+] Copied %i bytes to %p\n", lpMemoryImage);
	        };
	} else {
		printf("[ ] KeDumpImage : 0x%x\n", GetLastError());
	};
Cleanup:
	if ( hDriver != INVALID_HANDLE_VALUE )
		CloseHandle(hDriver);

	return lpMemoryImage;
};
