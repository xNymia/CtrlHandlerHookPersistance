#include "stdio.h"
#include "windows.h"
#include "stdlib.h"


int setValue(){
	    // Open RunOnce Key using RegCreateKeyEx 
		// - Opens Pre-existing Key or Creates
    
    HKEY hKey;
    DWORD result;

    DWORD res2 = RegCreateKeyEx(
        HKEY_CURRENT_USER,     // hKey - Hive Specification
        "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce",  // Subkey
        0,                      // Reserved
        NULL,                   // Registry Class (eg REG_SZ) - Null if existing
        0,                      // dwOptions - NULL = REG_OPTION_NON_VOLATILE
        KEY_WRITE,              // samDesired - Security Access
        NULL,                   // Security Attribs Structure (ptr)
        &hKey,                  // Buffer for opened handle
        &result                 // Disposition Buffer (Used for Success checks)
    );

		// Error Handler	
    if (res2 != ERROR_SUCCESS){
        
        LPTSTR errorText;

        FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            res2,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&errorText,
            0,
            NULL
        );

        printf("Return Message is: %s\n", errorText);
        LocalFree(errorText);
        return 1;
    }
   
    // Success Detection
   
    if (result == REG_CREATED_NEW_KEY){
        printf("New Key Created\n");
    } else if (result == REG_OPENED_EXISTING_KEY){
        printf("Existing Key Opened\n");
    } else {
        printf("Function Failed\n");
        return 0;
    }

    // Create new RunOnce Value using RegSetValueEx

    LPTSTR keyValue = "testkey";
    const byte *keyData = "C:\\test\\a.exe\0";
    int dataLen = strlen(keyData)+1; 

    DWORD valueRes = RegSetValueEx(
        hKey,                          // Handle passed in from RegCreateKeyEx
        keyValue,                      // Key Identifying Value
        0,                              // reserved must be 0
        REG_SZ,                         // Key Type
        keyData,                       // Pointer to key data
        dataLen                        // Length of KeyData+1 (NULL Term)
    );

	// Error Handler
	
    if (valueRes != ERROR_SUCCESS){
        
        LPTSTR errorText;

        FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            valueRes,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&errorText,
            0,
            NULL
        );

        printf("Return Message is: %s\n", errorText);
        LocalFree(errorText);
        return 1;
    } else {
        printf("New Key Added\n");
    }

    LocalFree(keyValue);

    return 0;
}

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType){
	switch (dwCtrlType)
	{
		case CTRL_SHUTDOWN_EVENT:
			setValue();
			return TRUE;
		case CTRL_LOGOFF_EVENT:
			setValue();
			return TRUE;
		default:
			return FALSE;
	}
	
}

int main(){
	char x = '*';
	DWORD SleepTime = 5000;
	// Install Hander
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	// Do $Stuff
	while(TRUE){
		printf("%c",x);
		Sleep(SleepTime);
	}
	return 0;
}

