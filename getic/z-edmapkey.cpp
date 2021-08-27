// z_ed2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <io.h>
#include <afxdisp.h>
#include <FCNTL.H>
#include "z-edmap.h"
#include "MainFrm.h"
#include "z_ed2Doc.h"
#include "z_ed2View.h"
#include "texsys.h"
#ifdef COM_WAY
    #include <initguid.h>
    #include "IGetic.h"
    #include "Z_edmap_i.c"
#else
    #include "RC_GETICPLUG.H"
#endif
#include "Staticgl.h"
#include "ToolFrame.h"
#include "Crc.h"

#ifndef APSTUDIO_INVOKED
    #define APSTUDIO_INVOKED
#endif //
#ifdef APSTUDIO_READONLY_SYMBOLS
    #undef APSTUDIO_READONLY_SYMBOLS
#endif // 
#include "geticgui.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int GRuns;


#define SSEQ        "@:$"

struct Sec
{
    char            sig[4];
    int             payed;
    unsigned long   crc;
    SYSTEMTIME      st;
    char            sige[16];
};
//static Sec sec = {"@:$",0,0,0,"@@@@@@@@@@@@@@@"};

static size_t __gKey = 0x5A;
static SYSTEMTIME PAYED_TIME = {2000,1,0,31,11,59,59,89};

static void Encode(BYTE* pd,  int ln)
{
	for(int i=0;i<ln;i++)
		*(pd+i)=*(pd+i)^__gKey;
}

static void Decode(BYTE* pd,  int ln)
{
	for(int i=0;i<ln;i++)
		*(pd+i)=*(pd+i)^__gKey;
}

static size_t GetOffset(BYTE* pb, long len)
{
    for(long i=0;i<len; i++,pb++)
    {
        if(!::memcmp(pb,SSEQ,3))
        {
             return i;
        }
    }
    return 0;
}


class TimeBomb
{
public:
    TimeBomb():_file(0),_offset(0),_expired(FALSE){
        
        int hf = _open( "geticgui.dll", _O_RDONLY );
        if(hf)
        {
            memset(&_ft,0,sizeof(_ft));
            FILETIME ct={0}, lst={0};
            GetFileTime((HANDLE)hf, &ct, &lst, &_ft);
            _close(hf);
        }
        _crcFromReg = 0;
        _crcFromReg = theApp.GetProfileInt("KEY0", "HEX", 0);

    }

    ~TimeBomb()
    {
        delete[] _file;
        OFSTRUCT    os = {0}; 
        int hf = _open( "geticgui.dll", _O_WRONLY );
        SetFileTime((HANDLE)hf, (LPFILETIME) 0, (LPFILETIME) 0, &_ft);
        _close(hf);
       
    }

    BOOL    ReadDllSec()
    {
		theApp._runs = 0;
        if(_expired)
		{
            return 0;
		}

        FileWrap    fw;
        BOOL        rv = FALSE;
		SYSTEMTIME  firsttime = {0};
        
        if(!fw.Open("geticgui.dll","r+"))//fopen
		{
			PostQuitMessage(0);
            return FALSE;
		}
        long    len    = fw.Getlength();
        _file = new BYTE[len];
        if(!_file) 
		{
			return FALSE;
		}
       
        fw.Read(_file, len);
        _offset = GetOffset(_file, len);

        if(!_offset)
		{
			_expired=1;
            return 0;
		}

        // get sec security stream from file
        _pSec = (Sec*)(_file + _offset);       // get security buffer

        Decode((BYTE*)_pSec,sizeof(Sec));      

		::memcpy(&firsttime, &_pSec->st, sizeof(firsttime));
        size_t       crcFromDll = _pSec->crc;                          
        Encode((BYTE*)_pSec,sizeof(Sec));

        // calc crc with crc == 0 couse the crc changes
        _pSec->crc = 0;     // make crc 0
        size_t dwCrcCalc = _crc.update_crc(0, _file, len);   

        Decode((BYTE*)_pSec,sizeof(Sec));
        // check id crc calculated is differen than the one stored
        if(dwCrcCalc != crcFromDll)
        {
            AfxMessageBox("1 One of setting data or timebomb key was corrupted. Trial Expired.", MB_OK|MB_ICONINFORMATION);
            _expired = TRUE;
            return FALSE;
        }

        // see if in registry have same crc
        if(0 != _crcFromReg)
        {
            if(dwCrcCalc != _crcFromReg)
            {
                AfxMessageBox("2 One of setting data or timebomb key was corrupted. Trial Expired.", MB_OK|MB_ICONINFORMATION);
                _expired = TRUE;
                return FALSE;
            }
        }
        // write the time in the dll
        if(_pSec->st.wYear == 0)
        {
            AfxMessageBox("Getic 3D 1.2. First Run. This is a 30 days trial version",MB_OK|MB_ICONINFORMATION);
            GetLocalTime(&_pSec->st);
            Encode((BYTE*)_pSec, sizeof(Sec));
            _pSec->crc = 0;
            dwCrcCalc = _crc.update_crc(0, _file, len);   // we have the inital crc
            Decode((BYTE*)_pSec, sizeof(Sec));

            _pSec->crc = dwCrcCalc;
            _crcFromReg = dwCrcCalc;
			theApp.WriteProfileInt("KEY0", "HEX", _crcFromReg);
            Encode((BYTE*)_pSec, sizeof(Sec));
            fw.Pos(_offset);
            fw.Write(_pSec, sizeof(Sec));
            return 1;
        }
        else
        {
            if(!memcmp(&firsttime, &PAYED_TIME, sizeof(PAYED_TIME)))
            {
				_expired=0;
                return 1;
            }
            SYSTEMTIME  ct;
            GetLocalTime(&ct);

			COleDateTime timeStart  = firsttime;
			COleDateTime timeEnd	= ct;
			
			COleDateTimeSpan spanElapsed = timeEnd - timeStart;
            
			int secs = spanElapsed.GetSeconds();
			int days = spanElapsed.GetDays();
			//AfxMessageBox(MKSTR("%d",days));
			if(days >= 0 && days < 30)
			{
				theApp._runs = 30-days; 
                AfxMessageBox(MKSTR("Getic 3D 1.2. This is a 30 days trial version. You have %d days left",30-spanElapsed.GetDays()),MB_OK|MB_ICONINFORMATION);
			}
            else
            {
                AfxMessageBox("Getic 3D 1.2. This is a 30 days trial version. Trial Expired",MB_OK|MB_ICONINFORMATION);
                _expired=1;
				theApp._runs = 0;
            }
        }
        return 0;
    }
    BOOL    _expired;

private:
  
    BYTE*    _file;
    Sec*    _pSec;
    size_t   _offset;
    CCrc    _crc;
    size_t   _crcFromReg;
    FILETIME _ft;
};



void CZ_ed2App::TestKey()
{
    TimeBomb    tb;

    tb.ReadDllSec();
    _returnVal=!tb._expired;
}

