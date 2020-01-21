#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "service.h"

PCHAR ExtractService(
	PCHAR szDriverPath
)
{
	PCHAR szDriverSvc = NULL;
	PCHAR szDriverStr = NULL;
	PCHAR szDriverExt = NULL;
	DWORD dwDriverLen = 0;

	if ( ((szDriverStr = strrchr(szDriverPath, '\\')) != NULL) ||
	     ((szDriverStr = strrchr(szDriverPath, '/'))  != NULL) &&
	     ((szDriverExt = strstr(szDriverPath, ".sys")) != NULL)
	   )
	{
		szDriverStr++; 
		
		dwDriverLen = (DWORD)(szDriverExt - szDriverStr);
		szDriverSvc = malloc(dwDriverLen+1);

		strncpy(szDriverSvc, szDriverStr, dwDriverLen);

		return szDriverSvc;
	};
	return NULL;
};

BOOL LoadDriver(
	PCHAR szDriverPath
)
{
  PCHAR     szDriverSvc  = NULL;
  SC_HANDLE ServiceMan   = NULL;
  SC_HANDLE ServicePtr   = NULL;
  BOOL      boolRetVal   = FALSE;

  szDriverSvc = ExtractService(szDriverPath);
  if ( szDriverSvc == NULL ) {
	  printf("[ ] LoadDriver() : Please provide a full path\n");
	  return FALSE;
  };

  ServiceMan = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
  if ( ServiceMan != NULL ) {
    ServicePtr = CreateServiceA(ServiceMan, szDriverSvc, szDriverSvc,
		    SERVICE_START | DELETE | SERVICE_STOP,
		    SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START,
		    SERVICE_ERROR_IGNORE, szDriverPath, NULL,
		    NULL, NULL, NULL, NULL);
    if ( ServicePtr != NULL ) {
	if ( StartService(ServicePtr, 0, NULL) == TRUE )
		boolRetVal = TRUE;

	CloseServiceHandle(ServicePtr);
    };
    CloseServiceHandle(ServiceMan);
  };
  return boolRetVal;
};

BOOL UnloadDriver(
	PCHAR szDriverPath
)
{
  PCHAR          szDriverSvc  = NULL;
  SC_HANDLE      ServiceMan   = NULL;
  SC_HANDLE      ServicePtr   = NULL;
  SERVICE_STATUS ServiceStat  =  { 0 };
  BOOL           boolRetVal   = FALSE;

  szDriverSvc = ExtractService(szDriverPath);
  if ( szDriverSvc == NULL ) {
	  printf("[ ] UnloadDriver() : Please provide a full path\n");
	  return FALSE;
  };

  ServiceMan = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
  if ( ServiceMan != NULL ) {
    ServicePtr = OpenServiceA(ServiceMan, 
		    szDriverSvc, SERVICE_STOP | DELETE);
    if ( ServicePtr != NULL ) {
      ControlService
      (
        ServicePtr, 
	SERVICE_CONTROL_STOP, 
	&ServiceStat
      );
      DeleteService(ServicePtr);
      CloseServiceHandle(ServicePtr);
      boolRetVal = TRUE;
    };
    CloseServiceHandle(ServiceMan);
  };
  return boolRetVal;
};
