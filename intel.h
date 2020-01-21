#ifndef _INTEL_H_
#define _INTEL_H_

#define IOCTL_TRIGGER_FUNCTION   0x80862007
#define CASE_NUMBER_MEMCPY       0x33
#define CASE_NUMBER_GET_PHYSICAL 0x25

typedef struct _MEMCPY_INFO
{
  ULONG64 CaseNum;
  ULONG64 Reserved;
  ULONG64 Src;
  ULONG64 Dst;
  ULONG64 Len;
} MEMCPY_INFO, *LPMEMCPY_INFO;

typedef struct _GET_PHYS_INFO
{
  ULONG64 CaseNum;
  ULONG64 Reserved;
  ULONG64 PhysAddr;
  ULONG64 VirtAddr;
} GET_PHYS_INFO, *LPGET_PHYS_INFO;

BOOL MemCpy(
	HANDLE hDriver,
	ULONG64 Destination,
	ULONG64 Source,
	ULONG64 Length
);

BOOL GetPhys(
	HANDLE hDriver,
	ULONG64 VirtualAddress,
	ULONG64 *lpPhysicalAddress
);

HANDLE GetHandle();

#endif
