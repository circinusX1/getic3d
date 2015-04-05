//============================================================================
// Author: Octavian Marius Chincisan 2006 March - 2006 Sept
//============================================================================

#include "baseutils.h"
#include <io.h>
#include "BzipCompressor.h"

//---------------------------------------------------------------------------------------
CBzipCompressor::CBzipCompressor(LPCTSTR szFileName, LPCTSTR destLoc, CB_SN pCb)
{
    _pcBCall = pCb;
    ::memset(&_ze,0,sizeof(_ze));
    if(destLoc == 0)
        _hz = CreateZip((void *)szFileName, 0, ZIP_FILENAME);
    else
    {
        _hz = OpenZip((void*)szFileName, 0, ZIP_FILENAME);
        ::_tcscpy(_destLoc, destLoc);
        if(_destLoc[_tcslen(_destLoc)-1]!=_T('\\'))
            _tcscat(_destLoc,_T("\\"));
    }
    ::_tcscpy(_srcFile, szFileName);
    _dstFile[0]=0;
}

//---------------------------------------------------------------------------------------
CBzipCompressor::~CBzipCompressor()
{
    if(_hz)
        CloseZip(_hz);
}

//---------------------------------------------------------------------------------------
BOOL    CBzipCompressor::AddFile(LPCTSTR pszFile)
{
    if(_hz)
    {
        PathHandler ph(pszFile);
        ZRESULT zr = ZipAdd(_hz, ph.File(), (void *)pszFile, 0, ZIP_FILENAME);
        return (zr == ZR_OK);
    }
    return FALSE;
}

//---------------------------------------------------------------------------------------
BOOL    CBzipCompressor::Process(BOOL compress, BOOL overwrite)
{
    ZRESULT zr = 0;
    BOOL    rv = FALSE;

    if(_hz == 0) return FALSE;
    if(compress)
    {
        CloseZip(_hz);
        _hz = 0;
        return TRUE;
    }
    else
    {
        ZRESULT zr = GetZipItem(_hz, -1, &_ze);
        if(zr == ZR_OK)
        {
            ZIP_ENTRY     ze;
            memset(&ze,0,sizeof(ze));
            char        targetName[_MAX_PATH];

            for(int i=0; i < _ze.index; i++)
            {
                zr = GetZipItem(_hz, i, &ze);
                if(zr == ZR_OK)
                {
                    ::_stprintf(targetName,_T("%s%s"),_destLoc,ze.name);

                    if(!overwrite)
                    {
                        struct _finddata_t c_file;
                        long    hFile    = _findfirst( targetName, &c_file );
                        if(hFile!=-1)
                        {
                            _findclose( hFile );
                            rv = 1;
                            if(_dstFile[0]==0)
                                ::_tcscpy(_dstFile, targetName);
                            if(_pcBCall)
                                (_pcBCall)(targetName);
                        }
                        else
                        {
                            overwrite=TRUE;
                        }
                    }
                    if(overwrite)
                    {
                        if(_dstFile[0]==0)
                            ::_tcscpy(_dstFile, targetName);
                        if(_pcBCall)
                            (_pcBCall)(targetName);
                        try{
                            zr = UnzipItem(_hz, i, targetName, 0, ZIP_FILENAME);
                        }
                        catch(...)
                        {
                            zr=-1;
                        }
                    }
                }
            }
        }
        if(_hz)
            CloseZip(_hz);
        _hz = 0;
    }
    return  zr == ZR_OK;
}







