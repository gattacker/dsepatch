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


int main(int argc, char **argv)
{
  CHAR  szTemporaryFolder[MAX_PATH + 1];
  RtlSecureZeroMemory(&szTemporaryFolder, MAX_PATH + 1);
  if ( GetTempPathA(MAX_PATH, (PCHAR)&szTemporaryFolder) != 0 )
  {
	  PCHAR szDriverPath = NULL;

	  asprintf(&szDriverPath, "%s%s.sys", (PCHAR)&szTemporaryFolder,
			  RandomString(10));

	  printf("[+] Driver Temporary Path %s\n", szDriverPath);
  };
};
