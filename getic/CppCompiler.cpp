//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.  
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------

#include "stdafx.h"
#include "CppCompiler.h"
#include "resource.h"
#include "z-edmap.h"

//--|  FilterOut|------------------------------------------------------------------------
char*  FilterOut(char*    outBuff)
{
    if(outBuff[0]==0)
      return outBuff;
    
    char* pDest = outBuff;
    char* pSrc   = outBuff;
    
    while(*pSrc)
    {
        if(*pSrc < ' ' && *pSrc!='\r' && *pSrc!='\n')
        {
            ++pSrc;
            continue;
        }
        *pDest++ = *pSrc++;
    }
    *pDest++ = 0;
    
    return outBuff;
}

CppCompiler::CppCompiler():_dc(""),_binary(0),_length(0)
{
}

//--|CppCompiler::~CppCompiler|-----------------------------------------------------------
CppCompiler::~CppCompiler()
{
    if(_binary)
      delete[]_binary;
    _binary = 0;
}

//--|  CppCompiler::Compile|--------------------------------------------------------------
DWORD  CppCompiler::Compile(char* buffer)
{
    char szTmpFile[_MAX_PATH];
    
    _tcscpy(szTmpFile, "temp\\temp.cpp");
    FILE* pf = _tfopen(szTmpFile,"rb");
    if(!pf)
      return FALSE;
    fwrite(buffer,_tcslen(buffer),1,pf);
    fclose(pf);
    return CompileFile(szTmpFile);
}


//--|  CppCompiler::TestDLL|--------------------------------------------------------------
BOOL  CppCompiler::TestDLL()
{
    BOOL bretval = FALSE;
    _results="";
/*    
    MyEngine me;
    // virtual _stdcall char* GetName()PURE;
    HINSTANCE hdll = ::LoadLibrary("temp\\temp.dll");
    
    _TRY{        
    if(hdll < (HINSTANCE)32)
        {
            _results += MKSTR("Cannot load glued code: %d / %d",GetLastError(), (DWORD)hdll);
            return FALSE;
        }
        
        EVT_CB pIs = (EVT_CB)::GetProcAddress(hdll,"Event");
        DWORD rv = 0;
        if(pIs)
        {
            rv = (pIs)(GM_CREATE, &me,0);
            
            if(rv)
              _results += MKSTR("Library Called Successfully. ");
        }
        
    }
    _CATCHX()
    {
        bretval = 0;
    }
    ::FreeLibrary(hdll);
    */
    return bretval;
}


//--| BYTE* CppCompiler::GetResultBinary|-------------------------------------------------
const BYTE* CppCompiler::GetResultBinary(long& len)
{
    
    if(_binary)
      delete[]_binary;
    
    //FILE pf = _tfopen("temp.dll","");
    
    
    len = _length;
    return _binary;
}

//--| CppCompiler::CompileFile|-----------------------------------------------------------
DWORD CppCompiler::CompileFile(const char* pFileName)
{
    _results = "";
    
    //
    // generate the temp header file g_script
    HRSRC     hrsrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_GSCRIPT_HDR1), "GSCRIPT_HDR");
    if(hrsrc)
    {
        DWORD dwsz = SizeofResource(AfxGetResourceHandle(), hrsrc);
        
        HGLOBAL hResLoad = LoadResource(AfxGetResourceHandle(), hrsrc);
        if(hResLoad)
        {
            void* pData = LockResource(hResLoad);
            
            if(pData)
            {
                FILE* pf = _tfopen("temp\\script.h","wb");
                if(pf){
                    fwrite(pData,1,dwsz,pf);
                    fclose(pf);
                }
                
            }//GetCurrentUser SID
        }
    }
    //
    
    if(_binary)
      delete[]_binary;
    _binary = 0;
    
    char appExe[_MAX_PATH];
    char cmdLine[2048] = {0};
    
    _stprintf(appExe,"%s\\%s",theApp.CompilerDir(),"g++.exe");
    
    _tcscpy(cmdLine, "-c temp\\temp.cpp -o temp\\temp.o -I\"temp/\" -I\"script/headers/\" -g3"); //-vtable-thunks
    return InvokeExternalApp(appExe,cmdLine);
    
}

//--|  CppCompiler::CleanTmpFiles|--------------------------------------------------------
void  CppCompiler::CleanTmpFiles()
{
    _unlink("temp\\temp.o");
    _unlink("temp\\temp.cpp");
    _unlink("temp\\temp.dll");
}

//--|  CppCompiler::MakeDLL|--------------------------------------------------------------
DWORD  CppCompiler::MakeDLL()
{
    char appExe[_MAX_PATH];
    char cmdLine[2048] = {0};
    _results = "";
    
    _stprintf(appExe,"%s\\%s",theApp.CompilerDir(),"dllwrap.exe");
    
    _tcscpy(cmdLine, "--output-def temp\\temp.def --driver-name c++ --implib temp\\temp.a temp\\temp.o  --no-export-all-symbols --add-stdcall-alias -o temp\\temp.dll");
    return InvokeExternalApp(appExe,cmdLine);
}

//--| CppCompiler::InvokeExternalApp|-----------------------------------------------------
DWORD CppCompiler::InvokeExternalApp(const char* pCmd,char* cmdLine)
{
    STARTUPINFO          tsi = {0};
    PROCESS_INFORMATION  tpi = {0};
    DWORD                nRead;
    SECURITY_ATTRIBUTES  sa  = {0};
    char                 outBuff[256];
    DWORD                rtVal=9999;
    
    _results = "";
    
    HANDLE  hOutputReadTmp, hOutputRead, hOutputWrite, hInputWriteTmp, hInputRead,
        hInputWrite, hErrorWrite;
    
    sa.nLength              = sizeof(sa);
    sa.lpSecurityDescriptor = 0;
    sa.bInheritHandle       = 1;
    
    ::CreatePipe(&hOutputReadTmp, &hOutputWrite, &sa, 0);
    ::DuplicateHandle(GetCurrentProcess(), hOutputWrite, GetCurrentProcess(),
                     &hErrorWrite, 0, true, DUPLICATE_SAME_ACCESS);
    ::CreatePipe(&hInputRead, &hInputWriteTmp, &sa, 0);
    ::DuplicateHandle(GetCurrentProcess(), hOutputReadTmp,  GetCurrentProcess(),
                     &hOutputRead,  0, false, DUPLICATE_SAME_ACCESS);
    ::DuplicateHandle(GetCurrentProcess(), hInputWriteTmp, GetCurrentProcess(),
                     &hInputWrite, 0, false, DUPLICATE_SAME_ACCESS);
    
    CloseHandle(hOutputReadTmp);
    CloseHandle(hInputWriteTmp);
    
    tsi.cb         = sizeof(tsi);
    tsi.dwFlags    = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    tsi.hStdInput  = hInputRead;
    tsi.hStdOutput = hOutputWrite;
    tsi.hStdError  = hErrorWrite;
    
    // pass new env variables to this process
    //
    char    cdALL[_MAX_PATH];
    char    cdBIN[_MAX_PATH];
    char    cdCD[_MAX_PATH];
    
    _getcwd(cdCD, _MAX_PATH);
    
    _tcscpy(cdALL,"path=");
    //    _tcscat(cdALL, cdBIN);
    //    _tcscat(cdALL, ";");
    _tcscat(cdALL, theApp.CompilerDir());
    
    
    _tcscpy(cdBIN, pCmd);
    _tcscat(cdBIN, " ");
    _tcscat(cdBIN, cmdLine);
    BOOL b = ::CreateProcess(0, cdBIN, &sa, &sa, 1, 0, (void*)cdALL, cdCD, &tsi, &tpi);
    if(!b)
    {
        nRead = GetLastError();
        _results += MKSTR("Error: %d spawning '%s'. Please Select Settings.\r\n", nRead, pCmd);
    }
    
    ::CloseHandle(hOutputWrite);
    ::CloseHandle(hInputRead );
    ::CloseHandle(hErrorWrite);
    BOOL terminated =FALSE;
    if(b)
    {
        DWORD to ;
        START_WAIT("");
        for(int i=0;i<30;i++)
        {
            to = ::WaitForSingleObject(tpi.hProcess,30000);
            if(to==WAIT_TIMEOUT){
                START_WAIT(MKSTR("Compiling: %d sec",30-i));
            }
        }
        END_WAIT("");
        
        if(to == WAIT_TIMEOUT)
        {
            _results += "Internal Compiler Error: Compiler terminated\r\n";
            TerminateProcess(tpi.hProcess, -1);
            terminated = TRUE;
        }
        ::CloseHandle(tpi.hThread);
        GetExitCodeProcess(tpi.hProcess, &rtVal);
    }
    
    if(FALSE ==terminated)
    {
        do{
            nRead = 0;
            if(!ReadFile(hOutputRead, outBuff, 16, &nRead, 0))
              break;
            if(nRead)
            {
                outBuff[nRead]=0;
                FilterOut(outBuff);
                _results += outBuff;
            }
        }while(nRead);
    }
    
    ::CloseHandle(hOutputRead);
    
    
    return rtVal;
}
