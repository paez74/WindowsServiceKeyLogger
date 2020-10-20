#include <windows.h>
#include <stdio.h>
 
#define TEXTFILE "C:\\Users\\paez7\\Documents\\Tec\\ProgramacionAvanzada\\WService\\log.txt"
 
SERVICE_STATUS ServiceStatus; 
SERVICE_STATUS_HANDLE hStatus; 
  
void  ServiceMain(int argc, char** argv); 
void  ControlHandler(DWORD request); 
int InitService();
 
int WriteToLog(char* str) // funcion que escribe en el archivo 
{
    FILE* log;
    log = fopen(TEXTFILE, "a+");
    if (log == NULL)
        return -1;
    fprintf(log, "%s\n", str);
    fclose(log);
    return 0;
}
 
void main() 
{ 
    /* typedef struct _SERVICE_TABLE_ENTRYA {
        LPSTR                    lpServiceName;
        LPSERVICE_MAIN_FUNCTIONA lpServiceProc;
        } SERVICE_TABLE_ENTRYA, *LPSERVICE_TABLE_ENTRYA;*/
    SERVICE_TABLE_ENTRY ServiceTable[1];
    ServiceTable[0].lpServiceName = "logwritingservice";
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
    // Start the control dispatcher thread for our service
    StartServiceCtrlDispatcher(ServiceTable);   // tiene que ser un apuntador(arreglo de tabla de servicio)
}
 
void ServiceMain(int argc, char** argv) 
{ 
    int error; 
  
    ServiceStatus.dwServiceType        = SERVICE_WIN32; 
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode      = 0; 
    ServiceStatus.dwServiceSpecificExitCode = 0; 
    ServiceStatus.dwCheckPoint         = 0; 
    ServiceStatus.dwWaitHint           = 0; 
  
    hStatus = RegisterServiceCtrlHandler(
        "logwritingservice", 
        (LPHANDLER_FUNCTION)ControlHandler); 
    if (hStatus == (SERVICE_STATUS_HANDLE)0) 
    { 
        // Registering Control Handler failed
        return; 
    }  
    
    error = InitService(); 
    if (error) // si hubo algun error se cambia el estado  
    {
        ServiceStatus.dwCurrentState       = SERVICE_STOPPED; 
        ServiceStatus.dwWin32ExitCode      = -1; 
        SetServiceStatus(hStatus, &ServiceStatus); 
        return; 
    } 
    // Hay que settear el estado actual del servicio al manager.
    ServiceStatus.dwCurrentState = SERVICE_RUNNING; 
    SetServiceStatus (hStatus, &ServiceStatus);
  
    while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
    {
     
        //Solo loopea el servicio cada cierto tiempo para que no salga
        while(1)
        {
            _sleep(500);         
        }
         
    }
    return; 
}
  
// Service initialization
int InitService() 
{ 
    int result;
    result = WriteToLog("Comienza el servicio.");
    return(result); 
} 
 
 
// Control handler function
void ControlHandler(DWORD request) 
{ 
    switch(request) 
    { 
        case SERVICE_CONTROL_STOP: 
             WriteToLog("Se termino el servicio.");
 
            ServiceStatus.dwWin32ExitCode = 0; 
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            SetServiceStatus (hStatus, &ServiceStatus);
            return; 
  
        case SERVICE_CONTROL_SHUTDOWN: 
            WriteToLog("Se termino el servicio.");
 
            ServiceStatus.dwWin32ExitCode = 0; 
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            SetServiceStatus (hStatus, &ServiceStatus);
            return; 
         
        default:
            break;
    }  
     
    // Report current status
    SetServiceStatus (hStatus,  &ServiceStatus);
  
    return; 
}   