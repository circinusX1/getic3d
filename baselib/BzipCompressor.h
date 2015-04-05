#ifndef __BZIP_CONP_H__
#define __BZIP_CONP_H__

#include "XZip.h"
#include "XUnzip.h"

#ifdef _UNICODE
	typedef ZIPENTRYW   ZIP_ENTRY;
#else
	typedef ZIPENTRY    ZIP_ENTRY;
#endif

typedef void (*CB_SN)(const char* pszDestname);
class CBzipCompressor
{
public:
	CBzipCompressor(LPCTSTR sourceFileName, LPCTSTR destLocation=0, CB_SN pCb=0);
	virtual ~CBzipCompressor();
    BOOL    Process(BOOL compress,BOOL overwrite=1);
    BOOL    AddFile(LPCTSTR pszFile);
    const   char* GetDestFileName(){return _dstFile;}
private:

    ZIP_ENTRY   _ze;
    HZIP        _hz;
    char      _destLoc[_MAX_PATH];
    char      _srcFile[_MAX_PATH];
    char      _dstFile[_MAX_PATH];
    CB_SN      _pcBCall; 
};

#endif // !defined(AFX_BZIPCOMPRESSOR_H__38FAFA84_F329_4AC4_983B_58F4CEA8E3AC__INCLUDED_)
