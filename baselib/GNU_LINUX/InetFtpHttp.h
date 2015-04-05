// InetFtpHttp.h: interface for the InetFtpHttp class.
//
#ifndef __INDET_FTPHTTP_H__
#define __INDET_FTPHTTP_H__

#include "../basecont.h"
#include "../sock.h"


typedef DWORD (*PDAV)(BYTE* pdata, DWORD dwlength, void* pUser);

//---------------------------------------------------------------------------------------
class InetFtpHttp
{
public:
    InetFtpHttp(PDAV pFncallback, void* pUser):_pFncallback(pFncallback){
        _pUser       = pUser;   // 2 pass back  with callback
        b_httpMode   = -1;
        b_break      = 0;
        _poxyport    = 0;    

    };
    virtual ~InetFtpHttp(){
        Disconnect();
    };

public:
    BOOL    Connect(const string& user, const string& password, const string& address, const int port);
    void    Break(){b_break = 1;};
    BOOL    IsBroke(){return b_break;}
    BOOL    GetFile(const string& fullname);
    BOOL    PutFile(const string& filename, const string& strpath);
    void    Callback(const TCHAR* strStatus){};
    void    Disconnect();
    void    SetProxy(const string& addr, int port){_proxyaddr=addr; _poxyport=port;};
private:
    PDAV         _pFncallback;
    tcp_cli_sock s_socket;
    BOOL         b_httpMode;
    BOOL         b_break;
    void*        _pUser;
    string       _httpaddress;
    string       _wwwweb;
    int          _port;
    string       _proxyaddr;
    int          _poxyport;   
};

//---------------------------------------------------------------------------------------
struct Ip
{
    Ip(){n_port=0;}
    Ip(const Ip &ip){
        s_url    = ip.s_url;
        s_user   = ip.s_user;
        s_passwd = ip.s_passwd;
        s_ip     = ip.s_ip;
        n_port   = ip.n_port;
        s_target = ip.s_target;
    }
    const Ip& operator=(const Ip &ip)
    {
        if(this != &ip)
        {
            s_url    = ip.s_url;
            s_user   = ip.s_user;
            s_passwd = ip.s_passwd;
            s_ip     = ip.s_ip;
            n_port   = ip.n_port;
            s_target = ip.s_target;
        }
        return *this;
    }
    bool ParseUrl(const tstring& ip)
    {
        s_user.erase();
        s_passwd.erase();
        s_ip.erase();
        n_port = 0;
        s_url    = ip;
        s_target.erase();
        size_t ln = ip.length();
        size_t p1 = ip.find("://",0),p2,p3;
        if(-1 == p1)
            return false;
        s_ip = ip.substr(0,p1 + 3);
        p1 += 3;
        if((p3 = ip.find("@",p1))!=-1)// has user passwd
        {
            p2 = ip.find(":", p1);
            if(p2 !=-1)
            {
                s_user    = ip.substr(p1,p2-p1);
                s_passwd  = ip.substr(p2+1,p3-p2-1);
            }
            p1 = p3+1;
        }
        if((p2 = ip.find("/",p1))!=-1)// has target
        {
             s_target =  ip.substr(p2, ln-p2);
        }
        if((p3 = ip.find(":",p1))!=-1)// has port
        {
            n_port = ::atoi(ip.substr(p3+1, ln-p3-1).c_str());
            p2 = p3;
        }
        s_ip += ip.substr(p1, p2-p1);
        return true;
    }
    tstring  s_url;
    tstring  s_user;
    tstring  s_passwd;
    tstring  s_ip;
    tstring  s_target;
    int      n_port;

};


#endif // !__INDET_FTPHTTP_H__
