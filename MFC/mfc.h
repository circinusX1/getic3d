#ifndef MFC_H
#define MFC_H


#include <vector>
#include <string>
#include <QObject>
#include <QWindow>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include "os.h"

#define afx_msg
#define AFX_DATA

typedef std::string tstring;
typedef int     BOOL;
typedef size_t* LRESULT;
typedef const char* LPCSTR;
typedef size_t* HWND;
typedef size_t  WPARAM;
typedef size_t LPARAM;
typedef void*  HINSTANCE;
typedef void*  HDC;
typedef void*  HFONT;
typedef void*  HGLRC;
typedef void*  HANDLE;

#define CALLBACK

//typedef uint16_t WORD;
//typedef uint32_t DWORD; //MS Q

typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;


struct POINT
{
    int x;
    int y;
};

typedef struct tagMSG {
  size_t   hwnd;
  size_t   message;
  size_t wParam;
  size_t lParam;
  time_t  time;
  POINT  pt;
  size_t  lPrivate;
} MSG, *PMSG, *NPMSG, *LPMSG;



#define _T(x_)   x_

typedef size_t (*WNDPROC)(HWND, WPARAM, LPARAM, MSG*);



struct CCmdTarget
{

};


typedef void( CCmdTarget::* AFX_PMSG )( void );

typedef struct AFX_MSGMAP_ENTRY
{
    size_t      nMessage;
    size_t      nCode;
    size_t      nID;
    size_t      nLastID;
    size_t      nSig;
    AFX_PMSG 	pfn;
}AFX_MSGMAP_ENTRY;


typedef struct AFX_MSGMAP
{
    const AFX_MSGMAP* pBaseMessageMap;
    const AFX_MSGMAP_ENTRY* lpEntries;
}AFX_MSGMAP;


#define DECLARE_MESSAGE_MAP() \
private: \
    static const AFX_MSGMAP_ENTRY _messageEntries[]; \
protected: \
    static AFX_DATA const AFX_MSGMAP messageMap; \
    virtual const AFX_MSGMAP* GetMessageMap() const;

#define BEGIN_MESSAGE_MAP(theClass, baseClass) \
    const AFX_MSGMAP* theClass::GetMessageMap() const \
    { return &theClass::messageMap; } \
        AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap = \
    { &baseClass::messageMap, &theClass::_messageEntries[0] }; \
        AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
    {

#define END_MESSAGE_MAP() \
    {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 } \
    };


#define DECLARE_DYNCREATE (class_name)  \
    static CreateObject(){};

class CWnd : public QWindow
{
public:
    CWnd(){}
    virtual ~CWnd(){}
};


typedef struct tagCCmdUI
{
    CWnd* pwnd;
    BOOL  state;
}CCmdUI;



class CEdit : public QLineEdit
{
public:
    CEdit(){}
    virtual ~CEdit(){}
};


class CDialog : public QDialog
{
public:
    CDialog(){}
    virtual ~CDialog(){}
};


class CWinApp
{
public:
    CWinApp(){};
}

// threads
               // pthread_cond_broadcast

#endif // MFC_H
