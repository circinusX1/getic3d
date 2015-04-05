// sock.h: interface for the sock class.
//============================================================================
// Author: Octavian Marius Chincisan 2006 March - 2006 Sept
//============================================================================

#ifndef __SOCK_H__
#define __SOCK_H__

#include "baselib.h"


#ifndef _CFG_WIN_WIN
/*
struct ip_mreq {
        struct in_addr  imr_multiaddr;
        struct in_addr  imr_interface;
};
*/
#define IP_DEFAULT_MULTICAST_TTL   1
#define IP_DEFAULT_MULTICAST_LOOP  1
#define IP_MAX_MEMBERSHIPS         20
#define IP_MULTICAST_TTL    3
#define IP_MULTICAST_LOOP   4
#define IP_ADD_MEMBERSHIP   5
#define IP_DROP_MEMBERSHIP  6

#endif //
//---------------------------------------------------------------------------------------
typedef BOOL (*CancelCB)(void* pVoid, DWORD time);

#ifndef WSABASEERR
    #define WSABASEERR              10000
    #define WSAESHUTDOWN            (WSABASEERR+58)
    #define WSAENETDOWN             (WSABASEERR+50)
    #define WSAECONNABORTED         (WSABASEERR+53)
    #define WSAEWOULDBLOCK          (WSABASEERR+35)
    #define WSAEINPROGRESS          (WSABASEERR+36)
    #define WSAECONNRESET           (WSABASEERR+54)
#endif//WSABASEERR



//---------------------------------------------------------------------------------------
class sock
{
public:
    static       BOOL DefCBCall(void*,DWORD);
    static void Init();
    static void Uninit();
    static int  GetLocalIP(char* pRetLocalAddr);
    static int  GetLocalIP(DWORD* dw);
    static BOOL CTime(void* pT, DWORD time);

    sock();
    virtual ~sock();
    virtual int     create(int opt=0);
    virtual void    destroy();
    virtual int     send(const BYTE* buff, const int length, int port=0, const char* ip=0  )=0;
    virtual int     send(const BYTE* buff, const int length, const struct sockaddr_in& rsin)=0;
    virtual int     receive(BYTE* buff, int length, int port=0, const char* ip=0  )=0;
    virtual int     receive(BYTE* buff, int length, struct sockaddr_in& rsin)=0;
    virtual int     select_receive(BYTE* buff, int length, struct sockaddr_in& rsin, int toutms);
    int             setblocking(DWORD block);
    int             setoption(int option, int optionsize);
    int             getoption(int option);
    const int&      socket()const {return _sock;}
    const BOOL      isopen()const{return _sock!=-1;}
    const int       error()const{return _error;}
protected:
    int             _sock;
    int             _error;
    BOOL            _bClose;
    static  DWORD   _tout;
    int             _blocking;
};

//---------------------------------------------------------------------------------------
class tcp_srv_sock;
class tcp_sock : public sock
{
public:
    friend class tcp_srv_sock;
    tcp_sock();
    virtual int     create(int opt=0);
    virtual int     send(const BYTE* buff, const int length, int port=0, const char* ip=0  );
    virtual int     send(const BYTE* buff, const int length, const struct sockaddr_in& rsin){return send(buff, length);}
    virtual int     receive(BYTE* buff, int length, int port=0, const char* ip=0  );
    virtual int     receive(BYTE* buff, int length, struct sockaddr_in& rsin){return receive(buff,length);}
    char*           getsocketaddr(char* pAddr);
    int             listen(int maxpending=16);
    struct          sockaddr_in& Rsin(){return _remote_sin;}
    struct          sockaddr_in& Lsin(){return _local_sin;}
protected:
    struct sockaddr_in	_local_sin;	   // source
	struct sockaddr_in	_remote_sin;          // dest
};


//---------------------------------------------------------------------------------------
class tcp_cli_sock;
class tcp_srv_sock : public tcp_sock
{
    int _port;
public:
    tcp_srv_sock();
    int     accept(tcp_cli_sock& cliSock);
    virtual int    create(int opt=0);
};

//---------------------------------------------------------------------------------------
class tcp_cli_sock : public tcp_sock
{
    int          pconnect(DWORD ip, int port);
    int          pconnect(const char* sip, int port, CancelCB cbCall=sock::DefCBCall, void* pUser=0);
public:
    explicit tcp_cli_sock(){}
    explicit tcp_cli_sock(const tcp_cli_sock& s):tcp_sock(){
        _sock       = s._sock;
        _error      = s._error;
        _bClose     = s._bClose;
        _blocking   = s._blocking;
        ::memcpy(&_local_sin, &s._local_sin, sizeof(_local_sin));
        ::memcpy(&_remote_sin, &s._remote_sin, sizeof(_remote_sin));
    }
    tcp_cli_sock& operator =   (const tcp_cli_sock& s){
        if(this != &s){
            _sock       = s._sock;
            _error      = s._error;
            _bClose     = s._bClose;
            _blocking   = s._blocking;
            ::memcpy(&_local_sin, &s._local_sin, sizeof(_local_sin));
            ::memcpy(&_remote_sin, &s._remote_sin, sizeof(_remote_sin));
        }
        return *this;
    }
    virtual int  create(int opt=0);
    int          connect(const char* sip, int port, CancelCB cbCall=sock::DefCBCall, void* pUser=0);
    DWORD        getremoteip(){return (DWORD)_remote_sin.sin_addr.s_addr;}

};

//---------------------------------------------------------------------------------------
class udp_sock : public sock
{
public:
    udp_sock():_connected(0){}
    virtual ~udp_sock(){destroy();}
    virtual void    destroy(){sock::destroy();_connected=0;};
    virtual int     create(int opt=0);
    virtual int     send(const BYTE* buff, const int length, int port=0, const char* ip=0  );
    virtual int     send(const BYTE* buff, const int length, const struct sockaddr_in& rsin);
    virtual int     receive(BYTE* buff, int length, int port=0, const char* ip=0  );
    virtual int     receive(BYTE* buff, int length, struct sockaddr_in& rsin);

    struct sockaddr_in& Rsin(){return _remote_sin;}
    struct sockaddr_in& Lsin(){return _local_sin;}
    void            SetRsin(const struct sockaddr_in& in){::memcpy(&_remote_sin, &in, sizeof(struct sockaddr_in));}
    int             connect(const char* sip, int port, CancelCB cbCall=sock::DefCBCall, void* pUser=0);
protected:
    BOOL                _connected;
    struct sockaddr_in	_local_sin;
    struct sockaddr_in	_remote_sin;          // dest
};


//---------------------------------------------------------------------------------------
class udp_server_sock : public udp_sock
{
public:
    udp_server_sock(){}
    virtual int     create(int opt=0);
    sockaddr_in&    remote(){return _remote_sin;}
    void            remote(sockaddr_in& s){memcpy(&_remote_sin,&s,sizeof(s));}
};

//---------------------------------------------------------------------------------------
class udp_client_sock : public udp_sock
{
public:
    udp_client_sock(){}
    virtual int     create(int opt=0);
    
};


//---------------------------------------------------------------------------------------
class udp_group_sock : public udp_sock
{
/*
    struct ip_mreq    _mcastGrp;
    BOOL               _groupmember;

public:
    udp_group_sock(int opt=0){};
    virtual ~udp_group_sock(){destroy();};
    virtual int  create(int opt=0);
    int     join(const char* ipGrp, int port);

    virtual int     send(BYTE* buff, int length, int port=0, const char* ip=0  );
    virtual int     receive(BYTE* buff, int length, int port=0, const char* ip=0  );
    virtual void    destroy();
    */
};

#define IS_SOCKK_ERR(err_)  err_ == WSAECONNRESET   ||\
                            err_ == WSAECONNABORTED ||\
                            err_ == WSAESHUTDOWN    ||\
                            err_ == WSAETIMEDOUT    ||\
                            err_ == WSAECONNREFUSED ||\
                            err_ == WSAEOPNOTSUPP   ||\
                            err_ == WSAENETDOWN




#endif // !

