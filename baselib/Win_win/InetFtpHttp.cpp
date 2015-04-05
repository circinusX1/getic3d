//============================================================================
// Author: Octavian Marius Chincisan 2006 March - 2006 Sept
//============================================================================

#include "InetFtpHttp.h"

//-----------------------------------------------------------------------------
static InetFtpHttp* PThis;

//-----------------------------------------------------------------------------
static BOOL    ConnectionTimeOut(void* pVoid, DWORD time)
{
    if(PThis->IsBroke())
        return 0;
    if(time < 10000) //timeout 10 seconds
        return 1;
    return 0;
}

//-----------------------------------------------------------------------------
BOOL    InetFtpHttp::Connect(const string& user, const string& password, const string& address, const int port)
{
    PThis = this;
    if(address.find("ftp://")!=-1)
    {
        return 0;
    }
    else if(address.find("http://")!=-1)
    {
        _httpaddress = address;
        _wwwweb = address.substr(7,address.length()-7);
        _port = port;
        b_httpMode = 1;
        return 0;
    }
    return 0;
}

//-----------------------------------------------------------------------------
void    InetFtpHttp::Disconnect()
{
    s_socket.destroy();
}

//-----------------------------------------------------------------------------
BOOL    InetFtpHttp::GetFile(const string& fullname, HWND dipMsg)
{
    BOOL rv = 0;
    
    if(b_httpMode==1)
    {
        try{
            if(!s_socket.isopen())
            {
                if(_poxyport)
                {
                    if(s_socket.connect(_proxyaddr.c_str(), _poxyport, ConnectionTimeOut, 0) < 0)
                    {
                        throw 1;
                    }
                }
                else if(s_socket.connect(_wwwweb.c_str(), _port, ConnectionTimeOut, 0) < 0)
                {
                    throw 1;
                } 
            }
            UINT i=0,j=0;
            char request[8912];
            char szfullname[256] = {0};

            for(i=0; i<fullname.length();i++)
            {
                if(fullname[i]== '\\')
                    szfullname[j++]= '/';
                else if(fullname[i]== ' ')
                {
                        szfullname[j++]='%';
                        szfullname[j++]='2';
                        szfullname[j++]='0';
                }
                else
                    szfullname[j++]=fullname[i];
            }

			sprintf(request,    "GET %s HTTP/1.0\r\n"
                                "Host: %s\r\n"
                                "User-Agent: Genet 1.0\r\n\r\n",
                                 szfullname,
                                 _wwwweb.c_str());

            if(s_socket.send((BYTE*)request, strlen(request)+1) > 0)
            {
                BOOL    hdr = 0;
                BYTE    recBuff[4095];
                BYTE    *pBegin = recBuff;
                int     shot;

                ge_sleep(32);

                while(1)
                {
                    shot  = s_socket.receive( recBuff, sizeof(recBuff)-1);
                    ge_sleep(1);
                    if(shot <= 0)
                    {
						UINT err = GetLastError();
                        break;
                    }
                    recBuff[shot] = 0;
                    if(hdr==0)
                    {
                        if(strstr((const char*)recBuff,"404 Not Found"))
                        {
                            throw 404;
                        }

                        char* pcrlfcrlf = (char*)strstr((const char*)recBuff,"\r\n\r\n");
                        if(pcrlfcrlf)
                        {
                            pcrlfcrlf += strlen("\r\n\r\n");
                            shot      -= (pcrlfcrlf - (char*)pBegin);
                            pBegin    = (BYTE*)pcrlfcrlf;
                            hdr       = 1;
                            if(_pFncallback)
                            {
                                if(-1==(_pFncallback)(recBuff, -((BYTE*)pcrlfcrlf-(BYTE*)recBuff), _pUser))
                                    break;
                            }
                        }
                    }

                    if(_pFncallback)
                    {
                        if (-1 == (_pFncallback)(pBegin, shot, _pUser))
                        {
                            rv = 0;
                            break;
                        }
                        rv = 1;
                    }
                    pBegin = recBuff;
                }     
            }
            else throw 1;
        }catch(...)
        {
            rv=0;
        }
        Disconnect();
    }
    if(rv==0)throw 1;
    return rv;
}

//-----------------------------------------------------------------------------
BOOL    InetFtpHttp::PutFile(const string& filename, const string& strpath)
{
    return 0;
}
