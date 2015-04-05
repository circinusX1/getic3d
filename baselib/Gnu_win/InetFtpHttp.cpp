#include "../stdafx.h"
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
        return 1;
    }
    else if(address.find("http://")!=-1)
    {
        _httpaddress = address;
        _wwwweb = address.substr(7,address.length()-7);
        _port = port;
        b_httpMode = 1;
         return 1;
    }
    return 0;
}

//-----------------------------------------------------------------------------
void    InetFtpHttp::Disconnect()
{
    s_socket.destroy();
}

//-----------------------------------------------------------------------------
BOOL    InetFtpHttp::GetFile(const string& fullname)
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
                        return 0;
                    }
                }
                else if(s_socket.connect(_wwwweb.c_str(), _port, ConnectionTimeOut, 0) < 0)
                {
                    return 0;
                } 
            }
            char request[1024];
            sprintf(request,    "GET %s HTTP/1.0\r\n"
                                "Host: %s\r\n"
                                "User-Agent: Genet/1.0\r\n"
                                "Keep-Alive: 300\r\n"
                                "Connection: keep-alive\r\n\r\n",
                                 fullname.c_str(),
                                 _wwwweb.c_str());

            if(s_socket.send((BYTE*)request, strlen(request)+1) > 0)
            {
                BYTE    recBuff[2048];
                int     shot;
                ge_sleep(64);

                while(1)
                {
                    shot  = s_socket.receive( recBuff, sizeof(recBuff)-1);
                    if(shot <= 0)
                    {
                        break;
                    }
                    recBuff[shot] = 0;
                    if(_pFncallback)
                    {
                        if(-1==(_pFncallback)(recBuff, shot, _pUser))
                            break;
                        rv = 1;
                    }
                }     
            }
        }catch(...)
        {
            rv=0;
        }
        Disconnect();
    }
    return rv;
}

//-----------------------------------------------------------------------------
BOOL    InetFtpHttp::PutFile(const string& filename, const string& strpath)
{
    return 0;
}
