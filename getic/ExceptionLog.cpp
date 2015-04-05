// ExceptionLog.cpp: implementation of the ExceptionLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "z_edmap.h"
#include "ExceptionLog.h"

FILE* sgLogFile;

// from Dr Dobbs may  2006 Stefan Wörthmüller...
void   LogStackFrames(PVOID FaultAddress, char *eNextBP)
{      char *p, *pBP;                                     
       unsigned i, x, BpPassed;
       static int  CurrentlyInTheStackDump = 0;
   
       BpPassed = (eNextBP != NULL);
       if(! eNextBP)
       {
          _asm mov     eNextBP, eBp   
       }
       else 
           fprintf(sgLogFile, "\n  Fault Occurred At $ADDRESS:%08LX\n", 
                                   (int)FaultAddress);
                             // prevent infinite loops
       for(i = 0; eNextBP && i < 100; i++)
       {      
           pBP = eNextBP;           // keep current BasePointer
           eNextBP = *(char **)pBP; // dereference next BP 
           p = pBP + 8; 
           // Write 20 Bytes of potential arguments
           fprintf(sgLogFile, "         with ");        
           for(x = 0; p < eNextBP && x < 20; p++, x++)
               fprintf(sgLogFile, "%02X ", *(unsigned char *)p);
           fprintf(sgLogFile, "\n\n");          
                  if(i == 1 && ! BpPassed) 
               fprintf(sgLogFile, "*************************************\n"
                                 "         Fault Occurred Here:\n");
        // Write the backjump address
        fprintf(sgLogFile, "*** %2d called from $ADDRESS:%08LX\n", 
                                  i, *(char **)(pBP + 4));
        if(*(char **)(pBP + 4) == NULL)
            break; 
       }

}


LONG Win32FaultHandler(struct _EXCEPTION_POINTERS* ExInfo)
{
    char  *FaultTx = "";
    switch(ExInfo->ExceptionRecord->ExceptionCode)
    {
      case EXCEPTION_ACCESS_VIOLATION      : 
                                      FaultTx = "ACCESS VIOLATION"         ; break;
      case EXCEPTION_DATATYPE_MISALIGNMENT : 
                                       FaultTx = "DATATYPE MISALIGNMENT"    ; break;
      case EXCEPTION_FLT_DIVIDE_BY_ZERO    : 
                                      FaultTx = "FLT DIVIDE BY ZERO"       ; break;
        default: 
            FaultTx = "(unknown)";           break;
    }   
    //sgLogFile = fopen("exceptions.log", "w");
    int    wsFault    = ExInfo->ExceptionRecord->ExceptionCode;
    PVOID  CodeAdress = ExInfo->ExceptionRecord->ExceptionAddress;
    sgLogFile = fopen("getic.log", "w");
    if(sgLogFile != NULL)
    {
        
        fprintf(sgLogFile, "****************************************************\n");
        fprintf(sgLogFile, "*** A Program Fault occurred:\n  Build: %s %s",__DATE__, __TIME__);
        fprintf(sgLogFile, "*** Error code %08X: %s\n", wsFault, FaultTx);
        fprintf(sgLogFile, "****************************************************\n");
        fprintf(sgLogFile, "***   Address: %08X\n", (int)CodeAdress);
        fprintf(sgLogFile, "***     Flags: %08X\n", 
                                     ExInfo->ExceptionRecord->ExceptionFlags);
       LogStackFrames(CodeAdress, (char *)ExInfo->ContextRecord->Ebp);
       fclose(sgLogFile);
    }
    /*if(want to continue)
    {
       ExInfo->ContextRecord->Eip++;
       return EXCEPTION_CONTINUE_EXECUTION;
    }
    */ 
    ::MessageBox(0, "Unhandled Exception \n 'getic.log' was generated. \n Application will be closed ! \n\n\n"
                 "To help us in debuging process you can post  getic.log content to out 'forum'","Getic", MB_OK|MB_ICONHAND);
    return EXCEPTION_EXECUTE_HANDLER;
}

