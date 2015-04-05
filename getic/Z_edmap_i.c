/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Sep 16 21:19:55 2004
 */
/* Compiler settings for F:\getae\getic\Z_edmap.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID LIBID_ZedmapLib = {0xC929FD10,0x0669,0x4C8B,{0xA0,0x44,0xA4,0x83,0x75,0xFB,0x48,0xD4}};


const IID IID_IIGetic = {0x7ABCC0EA,0x9A33,0x4713,{0xBC,0xDA,0x85,0xC8,0xDE,0x42,0x4B,0x6C}};


const IID IID_IGeticPlug = {0x2CADEA3E,0xA57C,0x4336,{0xAD,0x13,0x91,0xBE,0x80,0x4E,0xE9,0xF4}};


const CLSID CLSID_IGetic = {0xA621848E,0xFFAB,0x441B,{0x8D,0xD0,0xE3,0x3B,0x24,0x9E,0xC2,0x9C}};


#ifdef __cplusplus
}
#endif

