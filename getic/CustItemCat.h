
#ifndef __CUST_ITEM_CATS__
#define __CUST_ITEM_CATS__

#include "baseutils.h"
#include "bspfilestr.h"

inline VAR FromSignature(const char* sig)
{
    if(!stricmp(sig,"BYTE"))
        return V_BYTE;
    if(!stricmp(sig,"CHAR"))
        return V_CHAR;
    if(!strcmp(sig,"TEXT"))
        return V_TEXT;
    if(!strcmp(sig,"FILENAME"))
        return V_FILENAME;
    if(!stricmp(sig,"INT"))
        return V_INT;
    if(!strcmp(sig,"float"))
        return V_REAL;
    if(!strcmp(sig,"REAL"))
        return V_REAL;
    if(!strcmp(sig,"CLR"))
        return V_CLR;
    if(!strcmp(sig,"V3"))
        return V_V3;
    if(!strcmp(sig,"UV"))
        return V_UV;
    if(!stricmp(sig,"DOUBLE"))
        return V_DOUBLE;
    return V_BUFFER;
};


inline int TypeSize(VAR typ)
{
	switch(typ)
	{
		case V_BYTE:
			return sizeof(unsigned char);
		case V_CHAR:
			return sizeof(char);
		case V_TEXT:
		case V_FILENAME:
			return MAX_SLEN*sizeof(char);
		case V_INT:
			return sizeof(int);
		case V_REAL:
			return sizeof(REAL);
		case V_CLR:
			return sizeof(CLR);
		case V_V3:
			return sizeof(V3);
		case V_UV:
			return sizeof(UV);
		case V_DOUBLE:
			return sizeof(double);
	}
	ASSERT(0);
	return 0;
};

struct  NameTypeValue
{
    NameTypeValue(){
        _name[0]=0;
        _type=(_VAR)-1;
        memset(_value,0,sizeof(_value));
        memset(_valueMax,0,sizeof(_value));
        memset(_valueMin,0,sizeof(_value));
    }

    char    _name[MAX_SLEN];
    VAR     _type;
    BYTE    _value[MAX_SLEN];
    BYTE    _valueMax[MAX_SLEN];
    BYTE    _valueMin[MAX_SLEN];
    
    template <class T> void SetValue(T& t)
    {
        T*  pt = (T*)_value;
        *pt = t;
    }

    template <char> void SetValue(char* t)
    {
        SCopy((char*)_value,t, 32);
    }

    template <class T>void  GetValue(T& t)
    {
        T*  pt = (T*)_value;
        t = *pt ;
    }
    template <char>void GetValue(char* t)
    {
        SCopy(t,(char*)_value, 32);
    }
//
    template <class T> void SetMaxValue(T& t)
    {
        T*  pt = (T*)_value;
        *pt = t;
    }
    template <char> void SetMaxValue(char* t)
    {
        SCopy((char*)_value,t, 32);
    }

    template <class T>void  GetMaxValue(T& t)
    {
        T*  pt = (T*)_value;
        t = *pt ;
    }
    template <char>void GetMaxValue(char* t)
    {
        SCopy(t,(char*)_value, 32);
    }

//
    template <class T> void SetMinValue(T& t)
    {
        T*  pt = (T*)_value;
        *pt = t;
    }
    template <char> void SetMinValue(char* t)
    {
        SCopy((char*)_value,t, 32);
    }

    template <class T>void  GetMinValue(T& t)
    {
        T*  pt = (T*)_value;
        t = *pt ;
    }
    template <char>void GetMinValue(char* t)
    {
        SCopy(t,(char*)_value, 32);
    }



};

//-----------------------------------------------------------------------------
// item category. realisation into specific items
class ItemCat
{
public:
    ItemCat(){
        _catname[0]=0;
        _itemname[0]=0;
    }
    char                     _catname[MAX_SLEN];  // 
    char                     _itemname[MAX_SLEN]; // 
    vvector<NameTypeValue>   _classprops;         // class config dialog
    vvector<NameTypeValue>   _objprops;           // per object properties


};

#endif // __CUST_ITEM_CATS__
