// sock.cpp: implementation of the sock class.
//============================================================================
// Author: Octavian Marius Chincisan 2006 March - 2006 Sept
//============================================================================

#include <stdio.h>
#include "sock.h"

//-----------------------------------------------------------------------------
DWORD       sock::_tout = 15000;
class IP2String
{
public:
   IP2String(DWORD dw){
       ::sprintf( _s,"%u.%u.%u.%u",
                      (int)(dw&0xFF),
                      (int)((dw&0xFF00)>>8),
                      (int)((dw&0xFF0000)>>16),
                      (int)((dw&0xFF000000)>>24));
   }
   operator const char*(){return _s;}
private:
   char _s[64];
};
#define IP2STR(dwip_)   (const char*)IP2String(dwip_)

//-----------------------------------------------------------------------------
void sock::Init()
{
#ifdef WIN32
    WSADATA     WSAData;
    WSAStartup (MAKEWORD(1,1), &WSAData);
#endif
}

//-----------------------------------------------------------------------------

void sock::Uninit()
{
#ifdef WIN32
    WSACleanup();
#endif
}

//-----------------------------------------------------------------------------
BOOL sock::CTime(void* , DWORD time)
{
     return time < sock::_tout;
}


//-----------------------------------------------------------------------------
sock::sock()
{
    _sock        = INVALID_SOCKET;
    _error       = 0;
    _blocking    = 0;
};

//-----------------------------------------------------------------------------
sock::~sock()
{
    destroy();
};

//-----------------------------------------------------------------------------
int sock::setblocking(DWORD block)
{
    if(isopen())
    {
        unsigned long dw = !block;
    #ifdef WIN32
        int           rv = ::ioctlsocket(_sock, FIONBIO ,&dw);
    #else
        int           rv = ::ioctl(_sock, FIONBIO ,&dw);
    #endif //
        return rv;
    }
    return -1;
}

//-----------------------------------------------------------------------------
int sock::create(int )
{
   return -1;
}

//-----------------------------------------------------------------------------
int  sock::GetLocalIP(DWORD* dw)
{
    sockaddr_in locSin    ;
    hostent*    pHe       = 0;
    char        szBuf[80] = {0};
    DWORD       dwSize    = 0;

    locSin.sin_family = AF_INET;
    dwSize = sizeof(szBuf);
    ::gethostname((char*)szBuf,(int)dwSize);
    pHe = ::gethostbyname(szBuf);
    if (pHe == NULL){
        return ge_error();
    }
    ::memcpy((char*)&(locSin.sin_addr), pHe->h_addr,pHe->h_length);
    *dw = locSin.sin_addr.s_addr;
    return 0;
}

//-----------------------------------------------------------------------------
int sock::GetLocalIP(char* pRetLocalAddr)
{
    sockaddr_in locSin;
    hostent*    pHe;
    char        szBuf[80];
    DWORD       dwSize;

    locSin.sin_family = AF_INET;
    dwSize = sizeof(szBuf);
    gethostname((char*)szBuf,(int)dwSize);
    pHe = gethostbyname(szBuf);
    if (pHe == NULL)
    {
        return ge_error();
    }
    ::memcpy((char*)&(locSin.sin_addr), pHe->h_addr,pHe->h_length);

    ::sprintf(pRetLocalAddr,"%u.%u.%u.%u",
                (int)((locSin.sin_addr.s_addr    ) & 0xFF),
                (int)((locSin.sin_addr.s_addr>>8 ) & 0xFF),
                (int)((locSin.sin_addr.s_addr>>16) & 0xFF),
                (int)((locSin.sin_addr.s_addr>>24) & 0xFF));
    return 0;
}

//-----------------------------------------------------------------------------
int sock::getoption(int option)
{
    if(isopen())
    {
        int optionVal;
        int optLen = sizeof(optionVal);
        if(0 == getsockopt(_sock, SOL_SOCKET, option, (char*)&optionVal, &optLen))
            return optionVal;
    }
    return -1;
}


//-----------------------------------------------------------------------------
//err = ::setsockopt(_sock, SOL_SOCKET, SO_SNDBUF, (char *) &bfSz, sizeof(bfSz));
int sock::setoption(int option, int optionsize)
{
    if(isopen())
        return ::setsockopt(_sock, SOL_SOCKET, option, (char *) &optionsize, sizeof(optionsize));
    return -1;
}


//-----------------------------------------------------------------------------
int  sock::select_receive(BYTE* buff, int length, struct sockaddr_in& rsin, 
                                  int toutms)
{
    timeval  tv   = {toutms/1000, toutms%1000};
    fd_set   rdSet;

    _error = 0;
    FD_ZERO(&rdSet);
    FD_SET(_sock, &rdSet);
    int nfds = (int)_sock+1; 
    if(::select(nfds, &rdSet, 0, 0, &tv) > 0)
    {
        return receive(buff, length);
    }
    _error = ge_error();
    return 0;
}


//-----------------------------------------------------------------------------
char* tcp_sock::getsocketaddr(char* pAddr)
{
    ::strcpy(pAddr, inet_ntoa((struct in_addr)_remote_sin.sin_addr));
    return pAddr;
}

//-----------------------------------------------------------------------------
int tcp_sock::send(const BYTE* buff, const int length, int , const char* )
{
    _error = 0;
    int i = ::send(_sock,(char *)buff, length, 0);
    if(-1==i)
        _error = ge_error();
    return i;
}

//-----------------------------------------------------------------------------
int tcp_sock::receive(BYTE* buff, int length, int , const char* )
{
    _error = 0;
    int rb = ::recv(_sock,(char *)buff, length, 0);
    if(rb==-1)
        _error = ge_error();
    return rb;
}

//-----------------------------------------------------------------------------
void sock::destroy()
{
    if((int)_sock > 0)
    {
        ::shutdown (_sock, 0x02);
        ge_sleep(10);
#ifdef WIN32
        ::closesocket(_sock);
#else
        ::close(_sock);
#endif
    }
    _sock=INVALID_SOCKET;
}

/*
//-----------------------------------------------------------------------------
const char*  tcp_cli_sock::tcp_cli_sock()
{
    ::sprintf(_sip,"%d.%d.%d.%d",
          (int)(_remote_sin.sin_addr.s_addr&0xFF),
          (int)(_remote_sin.sin_addr.s_addr&0xFF00)>>8,
          (int)(_remote_sin.sin_addr.s_addr&0xFF0000)>>16,
          (int)(_remote_sin.sin_addr.s_addr&0xFF000000)>>24);
    return _sip;
}
*/
//-----------------------------------------------------------------------------
tcp_sock::tcp_sock(){}

//-----------------------------------------------------------------------------
int tcp_sock::create(int opt)
{
    _error = 0;
    ::memset(&_local_sin,0,sizeof(_local_sin));
    ::memset(&_remote_sin,0,sizeof(_remote_sin));
    if(_sock != -1)
        return -1;
    _sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_sock < 0)
		_error = ge_error();
    return _sock;
}

//-----------------------------------------------------------------------------
int tcp_sock::listen(int maxpending)
{
    _error = 0;
    int rv = ::listen(_sock, maxpending);
    if((int)INVALID_SOCKET == rv)
    {
        _error = ge_error();
    }
    return rv;
}

//-----------------------------------------------------------------------------
tcp_srv_sock::tcp_srv_sock(){}

//-----------------------------------------------------------------------------
int tcp_srv_sock::create(int port)
{
    
    _error = 0;
    ::memset(&_local_sin,0,sizeof(_local_sin));
    ::memset(&_remote_sin,0,sizeof(_remote_sin));
    _sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_sock <= 0)
    {
		_error = ge_error();
        return _sock;
    }


    _local_sin.sin_family		= AF_INET;
    _local_sin.sin_addr.s_addr	= ::htonl(INADDR_ANY);
	_local_sin.sin_port		    = ::htons(_port = port);
	if(::bind(_sock,(struct sockaddr*)&_local_sin,sizeof(_local_sin)) < 0)
	{
		_error =  ge_error();
		destroy();
        return -1;
	}
    return 0;
}


//-----------------------------------------------------------------------------
int tcp_srv_sock::accept(tcp_cli_sock& cliSock)
{
    _error = 0;
    int clilen = sizeof(cliSock._remote_sin);
    cliSock._sock = ::accept(_sock,
                             (struct sockaddr*)&cliSock._remote_sin,
                             &clilen);
    if(cliSock._sock < 0)
    {
		_error = ge_error();
	}
    return cliSock._sock;
}

//-----------------------------------------------------------------------------
int tcp_cli_sock::create(int opt)
{
    return 0;
}

//-----------------------------------------------------------------------------
int tcp_cli_sock::pconnect(DWORD ip, int port)
{
    _error = 0;
    if(_sock != -1)
        return -1;
    _sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_sock < 0)
	{
        _error = ge_error();
        return _sock;
	}

    _remote_sin.sin_family		= AF_INET;
    _remote_sin.sin_addr.s_addr	= ::htonl(ip);
    _remote_sin.sin_port		= ::htons(port);

    _error = ::connect(_sock,(const struct sockaddr*)&_remote_sin,sizeof(_remote_sin));
    return _sock;
}

//-----------------------------------------------------------------------------
int tcp_cli_sock::pconnect(const char* sip, int port, CancelCB cbCall, void* pUser)
{
    int             err;
    fd_set          fdWr;
    timeval         tv = {1, 0};

    if(_sock != (int)INVALID_SOCKET){
        destroy();
    }
    _error = 0;
    _sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if(_sock < 0)
	{
		_error = ge_error();
        return _sock;
	}
	_remote_sin.sin_family		= AF_INET;
    _remote_sin.sin_addr.s_addr	= ::inet_addr(sip);
    _remote_sin.sin_port		= ::htons(port);

    // non blocking node couse we can cancel it by Cancel Call
    setblocking(0);
    DWORD         ti = ge_gettick();
    err = ::connect(_sock, (const struct sockaddr*)&_remote_sin, sizeof(_remote_sin));

    if(0==cbCall)
        cbCall = sock::DefCBCall;

    while(cbCall(pUser, ge_gettick()-ti))
    {
        FD_ZERO(&fdWr);
        FD_SET((UINT)_sock, &fdWr);
        err = ::select(0,0,&fdWr,0,&tv);
        if(err > 0)
        {
            if(FD_ISSET(_sock, &fdWr))
            {
                setblocking(1);
                return 0;   // no error
            }
        }
#ifdef _DEBUG
        ge_sleep(1);
#endif //
    }
    destroy();
    return INVALID_SOCKET;
}


//-----------------------------------------------------------------------------
int tcp_cli_sock::connect(const char* sip, int port, CancelCB cbCall, void* pUser)
{
    _error = 0;
    sockaddr_in    locSin;
    hostent* pHe = ::gethostbyname(sip);
    if(pHe==0)
        pHe = gethostbyaddr(  sip, ::strlen(sip),AF_INET );
    _error = 0;
    if(pHe)
    {
        ::memcpy((char*)&(locSin.sin_addr), pHe->h_addr,pHe->h_length);
#ifndef WINDOWS
        return pconnect(IP2STR(locSin.sin_addr.s_addr), port, cbCall, pUser);
#else
        return pconnect(IP2STR(locSin.sin_addr.S_un.S_addr), port, cbCall, pUser);
#endif

    }
    _error = ge_error();
    return -1;
}


//-----------------------------------------------------------------------------
BOOL sock::DefCBCall(void*, DWORD dw)
{
    return dw < 10000;
}

//-----------------------------------------------------------------------------
int udp_sock::create(int opt)
{
    _error = 0;
    _sock = ::socket (AF_INET, SOCK_DGRAM, 0);
    if ((int)_sock < 0)
    {
        _error = ge_error();
        destroy();
    }
    return _sock;
}


//-----------------------------------------------------------------------------
int  udp_sock::send(const BYTE* buff, const int length, int port, const char* ip)
{
    int snd ;
    _error = 0;
    if(_connected)
    {
        snd = ::send(_sock,(char *)buff, length, 0);
    }
    else
    {
        if(ip)
        {
            _remote_sin.sin_addr.s_addr = inet_addr(ip);
            _remote_sin.sin_port        = htons (port);
            _remote_sin.sin_family      = AF_INET;
        }
        snd = ::sendto(_sock, (char*)buff, length, 0,
                       (struct sockaddr  *) &_remote_sin,
                       sizeof (_remote_sin)) ;
    }
    if(-1 == snd){
        _error = ge_error();
    }
    return snd;
}

//-----------------------------------------------------------------------------
int  udp_sock::send(const BYTE* buff, const int length, const struct sockaddr_in& rsin)
{
    int snd ;
    _error = 0;
    if(_connected)
    {
        snd = ::send(_sock,(char *)buff, length, 0);
    }
    else
    {
        snd = ::sendto(_sock, (char*)buff, length, 0,
                      (struct sockaddr  *) &rsin,
                      sizeof (rsin)) ;
    }
    if(-1 == snd){
        _error = ge_error();
    }
    return snd;
}

//-----------------------------------------------------------------------------
int  udp_sock::receive(BYTE* buff, int length, struct sockaddr_in& rsin)
{
    int rcv;  
    if(_connected)
    {
        rcv = ::recv(_sock,(char *)buff, length, 0);
    }
    else
    {
        int iRecvLen  = sizeof(rsin);
        rcv =  (int)::recvfrom (_sock,
                           (char*)buff, length,
                           0,
                           (struct sockaddr  *) &rsin,
                           &iRecvLen);
    }
    if(rcv==-1){
        _error = ge_error();
    }
    return rcv;
}


//-----------------------------------------------------------------------------
int  udp_sock::receive(BYTE* buff, int length, int port, const char* ip)
{
    _error  = 0;
    int rcv ;
    if(_connected)
    {
        rcv = ::recv(_sock,(char *)buff, length, 0);
    }
    else
    {
        if(ip)
        {
            _remote_sin.sin_addr.s_addr = inet_addr(ip);
            _remote_sin.sin_port        = htons (port);
            _remote_sin.sin_family      = AF_INET;
            int iRecvLen                = sizeof(_remote_sin);
            rcv =  (int)recvfrom (_sock,
                               (char*)buff, length,
                               0,
                               (struct sockaddr  *) &_remote_sin,
                               &iRecvLen);

        }
        else
        {
            memset(&_remote_sin,0,sizeof(_remote_sin));
            int iRecvLen                = sizeof(_remote_sin);
            rcv =  (int)recvfrom (_sock, (char*)buff, length,
                                 0,
                                 (struct sockaddr  *) &_remote_sin,
                                 &iRecvLen);
 
        }
    }

    if(rcv==-1)
    {
        _error = ge_error();
    }
    return rcv;
}

//-----------------------------------------------------------------------------
int  udp_server_sock::create(int port)
{
    int       err;
    int       bfSz = 4096;
    long      abc  = 1;

    _error    = 0;
    _sock = ::socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == _sock) goto tERR;

    ::memset(&_local_sin, 0, sizeof(_local_sin));
    _local_sin.sin_family        = AF_INET;
    _local_sin.sin_addr.s_addr   = ::htonl(INADDR_ANY);
    _local_sin.sin_port          = ::htons(port);
    err = ::bind(_sock,(struct sockaddr *)&_local_sin, sizeof(_local_sin));

    return 0;
tERR:
    _error = ge_error();
    destroy();
    return -1;
}

//-----------------------------------------------------------------------------
int  udp_client_sock::create(int opt)
{
    _error    = 0;
    _sock = ::socket(AF_INET, SOCK_DGRAM, 0);
    if(_sock>0)
        return 0;
    _error = ge_error();
    destroy();
    return -1;
}


int  udp_sock::connect(const char* sip, int port, CancelCB cbCall, void* pUser)
{
    int             err;
    fd_set          fdWr;
    timeval         tv = {1, 0};

    _connected = 0;
    if(_sock != (int)INVALID_SOCKET){
        destroy();
    }
    _error = 0;
    _sock = ::socket(AF_INET, SOCK_DGRAM, 0);
    if(_sock < 0)
	{
		_error = ge_error();
        return _sock;
	}
	_remote_sin.sin_family		= AF_INET;
    _remote_sin.sin_addr.s_addr	= ::inet_addr(sip);
    _remote_sin.sin_port		= ::htons(port);
    memset(&_remote_sin.sin_zero, 0, 8);

    // non blocking node couse we can cancel it by Cancel Call
    setblocking(0);
    DWORD         ti = ge_gettick();
    err = ::connect(_sock, (const struct sockaddr*)&_remote_sin, sizeof(_remote_sin));
    if(0==cbCall)
        cbCall = sock::DefCBCall;

    while(cbCall(pUser, ge_gettick()-ti))
    {
        FD_ZERO(&fdWr);
        FD_SET((UINT)_sock, &fdWr);
        err = ::select(0,0,&fdWr,0,&tv);
        if(err > 0)
        {
            if(FD_ISSET(_sock, &fdWr))
            {
                setblocking(1);
                _connected = 1;
                return 0;   // no error
            }
        }
#ifdef _DEBUG
        ge_sleep(1);
#endif//
    }
    destroy();
    return INVALID_SOCKET;
}



//-----------------------------------------------------------------------------
/*
// ip range 224.0.0.0 to 238.255.255.255 as
int udp_group_sock::create(int opt)
{
    _error = 0;
    ::memset(&_mcastGrp,0,sizeof(_mcastGrp));
	_groupmember = FALSE;

    // from server
    _local_sin.sin_family      = AF_INET;
    _local_sin.sin_port        = htons (0);
    _local_sin.sin_addr.s_addr = htonl (INADDR_ANY);

    if (::bind (_sock, (struct sockaddr*) &_local_sin,
              sizeof (_local_sin)) == SOCKET_ERROR)
    {
        _error =  ge_error();
		destroy();
		return -1;
    }
    int iOptVal = opt;  // time to live (in the net)
    if (setsockopt (_sock, IPPROTO_IP, IP_MULTICAST_TTL,
                    (char  *)&iOptVal, sizeof (int)) == SOCKET_ERROR)
    {
        _error =  ge_error();
		destroy();
		return -1;
    }
    return 0;
}

//-----------------------------------------------------------------------------
int udp_group_sock::send(const BYTE* buff, int length, int port, const char* ipGrp)
{
	int snd;

    _error = 0;
    _remote_sin.sin_family = AF_INET;
    _remote_sin.sin_port = htons (port);
    _remote_sin.sin_addr.s_addr = inet_addr(ipGrp);
    snd = ::sendto (_sock, (char*)buff, length, 0,
                    (struct sockaddr  *) &_remote_sin, sizeof (_remote_sin)) ;
    if(snd < 0)
    {
		_error = ge_error();
    }
    return snd;
}

//-----------------------------------------------------------------------------
int udp_group_sock::receive(BYTE* pbuff, int length, int port, const char* ipGrp)
{
    _error = 0;
    int iRecvLen = sizeof (_remote_sin);
    int i =  (int)recvfrom (_sock, (char*)pbuff, length, 0,
                            (struct sockaddr  *) &_remote_sin, &iRecvLen);
    if(i==-1)
    {
        _error = ge_error();
    }
    return i;
}

//-----------------------------------------------------------------------------
void    udp_group_sock::destroy()
{
    if(_groupmember)
    {
        _error = ::setsockopt (_sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                              (char  *)&_mcastGrp, sizeof (_mcastGrp)) ;
    }
    udp_sock::destroy();
}

//-----------------------------------------------------------------------------
int udp_group_sock::join(const char* ipGrp, int port)
{
    if(_sock != -1)
        return -1;

    _error = 0;
    _local_sin.sin_family = AF_INET;
    _local_sin.sin_port = htons (port);
    _local_sin.sin_addr.s_addr = htonl (INADDR_ANY);
    int one = 1;
    ::setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one));
    int bnd = ::bind (_sock, (struct sockaddr  *) &_local_sin, sizeof (_local_sin));
    if (bnd == -1)
    {
		_error =  ge_error();
		destroy();
		return bnd;
    }

    //join the multicast group
    _mcastGrp.imr_multiaddr.s_addr = inet_addr (ipGrp);
    _mcastGrp.imr_interface.s_addr = INADDR_ANY;
    bnd = ::setsockopt (_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char  *)&_mcastGrp, sizeof (_mcastGrp)) ;
    if (bnd == -1)
    {
		_error =  ge_error();
		destroy();
		return bnd;
    }
    _groupmember = TRUE;
    return 0;
}
*/
