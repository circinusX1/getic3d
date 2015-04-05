// Scripts.cpp: implementation of the CScripts class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "z-edmap.h"
#include "Scripts.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "sceitem.h"

static char Strfmt[8912]={0};


struct ScrTmpl
{
	char*	psz;
};

ScrTmpl	tmpls[] = 
{
		{"0-%d"},
		{"1-%d"},
		{"2-%d"},
		{"3-%d"},
		{"4-%d"},
		{"5-%d"},
		{"6-%d"},
		{"7-%d"},
		{"8-%d"},
		{"9-%d"},
		{"0-%d"},
		{"1-%d"},
		{"12-%d"},
		{"13-%d"},
		{"14-%d"},
		{"15-%d"},
		{"16-%d"},
		{"17-%d"},
		{"long	OnTrigger_%s(ITrigger* pTrigger, IObject* pObject)\r\n"
		"{\r\n"
		"	return 1;\r\n"
		"}\r\n"},

};


const char*	CScripts::GetTemplate(SceItem* pItem)
{
	char	name[32];
	_tcscpy(name, pItem->_name);
	StripSpaces(name);

	_stprintf(Strfmt, tmpls[(int)pItem->_item].psz, name)	;

	return Strfmt;
}
