
#include "baseutils.h"
#include "..\\_include\\geticplug.h"
#include "geticplugbsp.h"


//---------------------------------------------------------------------------------------
class FlStream
{
public:
    FlStream():_plg("bspsave.dll"){};

    virtual ~FlStream(){
        _plg.Free();
    }

    BOOL    Open(const char* file, const char* how)
    {
        if(_plg.IsValid())
        {
            _ok = _plg->aopen(file, how);
        }
        else
        {
            _pf = ::_tfopen(file, how);
            _ok = _pf != 0;
        }
        _store = (how[0]==_T('w'));
        return _ok;
    }
    void Close(){
        if(_plg.IsValid())
        {
            if(_ok)
                _plg->close();
        }
        else
        {
            if(_pf)
                ::fclose(_pf);
            _pf=0;
        }
    }
    
    void Pos(unsigned long pos, DWORD cseek=SEEK_SET){
        if(_plg.IsValid())
        {
            _plg->seek(pos,(SEEK_POS)cseek);
        }
        else
        {
            ::fseek(_pf,pos,cseek);
        }
    }
    
    void ReStart(){
        if(_plg.IsValid())
        {
            _plg->seek(0,FSEEK_SET);
        }
        else
        {
            ::fseek(_pf,0,SEEK_SET);
        }
    }
    
    DWORD GetPos(){
        if(_plg.IsValid())
        {
            return _plg->tell();
        }
        return ::ftell(_pf);
    }
    
    DWORD Getlength(){
        if(_plg.IsValid())
        {
            _plg->seek(0,FSEEK_END);
            DWORD flength = ftell(_pf);
            _plg->seek(0,FSEEK_SET);
            return flength;
        }
        ::fseek(_pf,0,SEEK_END);
        DWORD flength = ftell(_pf);
        ::fseek(_pf,0,SEEK_SET);
        return flength;
    }
    
    BOOL ReadLine(char* line, int len){
        if(_plg.IsValid())
        {
            return _plg->gets(line,len)!=0;
        }
        return ::fgets(line,len,_pf)!=0;
    }
    
    template <class T> BOOL Serialize(T& t){
        if(_store) 
            return Write(t);
        return Read(t);
    }

    int    IsEof(){
        if(_plg.IsValid())
        {
            return _plg->eof();
        }
        return feof(_pf);
    }

    template <class T> BOOL Write(T& t, const char* sinfo=0){
        if(_plg.IsValid())
        {
            if(sinfo)
                _plg->start_section(sinfo);
            int lenr = _plg->write(&t,1,sizeof(T));
            return (lenr == sizeof(T));
        }
        int lenr = ::fwrite(&t,1,sizeof(T),_pf);
        return (lenr == sizeof(T));
    }
    
    template <class T> BOOL Read(T& t) {
        if(_plg.IsValid())
        {
            int lenr = _plg->(&t,1,sizeof(T));
            return (lenr == sizeof(T));
        }
        int lenr = ::fread(&t,1,sizeof(T),_pf);
        return (lenr == sizeof(T));
    }
    
    BOOL Read(void* pv, size_t len) {
        if(_plg.IsValid())
        {
            int lenr = _plg->read(pv,1,len);
            return (lenr == lenr);
        }
        int lenr = ::fread(pv,1,len,_pf);
        return (lenr == lenr);
    }
    
    BOOL Write(void* pb, size_t len) {
        if(_plg.IsValid())
        {
            return (len == _plg->write(pb,1,len));
        }
        return (len == ::fwrite(pb,1,len,_pf));
    } 
    
    BOOL Serialize(void* pv, int len){
        if(_store) return Write(pv,len);
        return Read(pv,len);
    }
    
    int WriteTextAsIs(const char* t){
        if(_plg.IsValid())
        {
            short l = _tcslen(t);
            return _plg->write(t, 1, _tcslen(t));
        }
        short l = _tcslen(t);
        return fwrite(t, 1, _tcslen(t),  _pf);
    }
    
    int WriteStr(const char* t){
        if(_plg.IsValid())
        {
            short l = _tcslen(t);
            _plg->write(&l, 1, sizeof(short));
            return _plg->write(t, 1, _tcslen(t));
        }
        short l = _tcslen(t);
        ::fwrite(&l, 1, sizeof(short), _pf);
        return fwrite(t, 1, _tcslen(t),  _pf);
    }
    
    int ReadStr(const char* t){
        if(_plg.IsValid())
        {
            short len;
            _plg->read(&len, 1, sizeof(short));
            int rd = _plg->read((void*)t, 1, len);
            return rd=len;
        }
        short len;
        ::fread(&len, 1, sizeof(short), _pf);
        int rd = ::fread((void*)t, 1, len,  _pf);
        return rd=len;
    }
    
    void start_section(const char* sectionName)
    {
        if(_plg.IsValid())
            _plg->start_section(sectionName);
    }

private:
    PlugInDll<IBSPFileHook>     _plg;
    FILE*                       _pf;
    long                        _ok;
    bool                        _store;
};

