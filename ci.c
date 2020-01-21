#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#define PTR(x) ((ULONG_PTR)x)

LPVOID GetCipInit(LPVOID ImageBase)
{
	HANDLE  hCiLib;
	LPVOID  fCiInit;
	INT     CallCnt;
	DWORD   Distance;
	LPVOID  fCipInit;

	if ( (hCiLib = LoadLibraryExA(
			  "C:\\Windows\\System32\\ci.dll",
			  NULL,
			  DONT_RESOLVE_DLL_REFERENCES
			)
	      ) != NULL 
	   ) 
	{	
		CallCnt = 0; 
		fCiInit = GetProcAddress(hCiLib, "CiInitialize");
		for ( int i = 0 ; i < 500 ; i++ )
		{
			if ( ((PBYTE)fCiInit)[i] == 0xE8 ) {
				if ( CallCnt != 1 ) {
					CallCnt++;
					continue;
				};
				Distance  = (*(DWORD *)( PTR(fCiInit) + i + 1 ) + 5);
				fCiInit   = (LPVOID)( PTR(ImageBase) + ( PTR(fCiInit) - PTR(hCiLib) ));
				fCipInit  = (LPVOID)(fCiInit + i + Distance);
				goto EndLoop;
			};
		};
EndLoop:
		FreeLibrary(hCiLib);
	};
	return fCipInit;
};
