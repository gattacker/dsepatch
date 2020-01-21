#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#define PTR(x) ((ULONG_PTR)x)

LPVOID GetCiOptions1(LPVOID ImageBase)
{
	HANDLE  hCiLib;
	LPVOID  fCiInit;
	INT     CallCnt;
	DWORD   Distance;
	LPVOID  fCipInit;
	LPVOID  gCiOpts;

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
				fCipInit  = (LPVOID)(fCiInit + i + Distance);
				goto FoundCipInit;
			};
		};
FoundCipInit:
		for ( int i = 0 ; i < 500 ; i++ )
		{
			if ( (((PBYTE)fCipInit)[i] == 0x89) && 
			     (((PBYTE)fCipInit)[i + 1] == 0x0d) 
			   )
			{
				Distance  = (*(DWORD *)( PTR(fCipInit) + i + 2 ) + 6);
				fCipInit  = (LPVOID)( PTR(ImageBase) + ( PTR(fCipInit) - PTR(hCiLib) ));
				gCiOpts   = (LPVOID)(fCipInit + i + Distance);
				goto FoundCiOptions;
			};
		};
	};
FoundCiOptions:
	if ( hCiLib != NULL || 
	     hCiLib != INVALID_HANDLE_VALUE 
	   ) FreeLibrary(hCiLib); 

	return gCiOpts;
};
