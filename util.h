#ifndef _UTIL_H_
#define _UTIL_H_

#define SystemModuleInformation 11

extern ULONG RtlRandomEx(
	PULONG Seed
);

typedef struct _SYSTEM_MODULE_ENTRY
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrdexIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR FullPathName[256];
} SYSTEM_MODULE_ENTRY, *PSYSTEM_MODULE_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG Count;
	SYSTEM_MODULE_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;


LPVOID GetDrvBase(
	PCHAR szDriverName,
	DWORD * SizeOfImage
);

BOOL DumpToFile(
	PCHAR szDumpPath
);

PCHAR RandomString(
	INT MaxLength
);
#endif
