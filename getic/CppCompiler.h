// Compiler.h: interface for the CppCompiler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPILER_H__79D167B9_9342_4637_B43C_12E0155764E5__INCLUDED_)
#define AFX_COMPILER_H__79D167B9_9342_4637_B43C_12E0155764E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tchar.h>
#include "basecont.h"



class CppCompiler  
{
public:
	CppCompiler();
	virtual ~CppCompiler();

    DWORD           Compile(char* buffer);
    DWORD           InvokeExternalApp(const char* p, char* );
    const char*    GetResultString(BOOL cr = FALSE){
		if(cr)
			_results+="\r\n";
        return _results.c_str();
    };  
    const BYTE*     GetResultBinary(long& len);
    void            CleanTmpFiles();
    DWORD           CompileFile(const char* pFileName);
    DWORD           MakeDLL();
    BOOL            TestDLL();
private:
    
    BOOL            InvokeCompiler(const char* pCmd,char* cmdLine);

private:
    CDirChange  _dc;
    tstring     _results;
    BYTE*       _binary;
    long        _length;

};

#endif // !defined(AFX_COMPILER_H__79D167B9_9342_4637_B43C_12E0155764E5__INCLUDED_)
