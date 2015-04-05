// LbEditable.h : header file
//
//  This File is Downloaded from toronto.planeteer.com/~zalmoxe 
//
//
//	
//
/////////////////////////////////////////////////////////////////////////////
// CLbEditable window
/////////////////////////////////////////////////////////////////////////////
#ifndef LBEDITABLE_H
#define LBEDITABLE_H
#pragma warning(disable: 4786)

#include "poly.h"
#include <string>
#include <map>
#include <vector>
using namespace std;

class CLbEditable;
class CCbFromLb : public CComboBox
{
// Construction
public:
	CCbFromLb():_index(-1),_pParent(0){};

// Attributes
public:
	int	_index;
    CLbEditable* _pParent;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCbFromLb)
	//}}AFX_VIRTUAL
    
// Implementation
public: 
	virtual ~CCbFromLb(){};

	// Generated message map functions
protected:
	//{{AFX_MSG(CCbFromLb)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    void OnCloseup() ;

	DECLARE_MESSAGE_MAP()
    
};


class CEditFromLb : public CEdit
{
// Construction
public:
	CEditFromLb():_index(-1),_pParent(0){};

// Attributes
public:
	int	_index;
    CLbEditable* _pParent;
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditFromLb)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditFromLb(){};

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditFromLb)
	//}}AFX_MSG
    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
	//////////////////////
    
private:
    
};


class CLbItem
{
public:
	typedef enum {
		NONE=0,
		CTEXT,
		CBOX,
		CBOXDIR,
	}C_TYPE_;

    typedef enum
    {
        V_INT,
        V_REAL,
        V_CLR,
        V_UV,
        V_V3,
        V_STR,
    }V_TYPE_;
public:

	virtual ~CLbItem(){}

    CLbItem(C_TYPE_ t, const char* prop,  CLR& val)
    {
        char s[32];
        _stprintf(s,"%d,%d,%d",val.r,val.g,val.b);
        _vtype = V_CLR;
        SetItem(t, prop, s);
    }

    CLbItem(C_TYPE_ t, const char* prop,  REAL val)
    {
        char s[32];
        _vtype = V_REAL;
        _stprintf(s,"%.4f",val);
        SetItem(t, prop, s);
    }

    CLbItem(C_TYPE_ t, const char* prop,  const char* p, int seldef=0)
    {
        _vtype = V_STR;

        SetItem(t, prop, p, seldef);
    }

	void SetItem(C_TYPE_ t, const char* prop,  const char* p, int seldef=0)
	{
		_prop = prop;
		_t = t;
		if(t==CTEXT)
		{
			_curval=p;
		}
		else if(t==CBOX)
		{
            char local[1024];
            _tcscpy(local, p);
		
            char tk[] = ",";
			char* token = _tcstok( local, tk);
			while( token != NULL )
			{
				_cbStrings.push_back(token);
				token = _tcstok( NULL, tk );
			}
            
			if(_curval.length()==0)
				_curval = _cbStrings[seldef];

		}
		else if(t==CBOXDIR)
		{
			// p holds the directory wild sequence
			HANDLE			hfFind;
			WIN32_FIND_DATA	fDATA = {0};	
			if((hfFind=FindFirstFile(p, &fDATA)) != INVALID_HANDLE_VALUE)
			{
				if(_curval.length()==0)
					_curval=fDATA.cFileName;
				_cbStrings.push_back(fDATA.cFileName );
				while(FindNextFile(hfFind, &fDATA)!=0)
				{
					_cbStrings.push_back(fDATA.cFileName );
				}
				FindClose(hfFind);  
			}
		}
	}
	
	tstring			    _curval;
	tstring			    _prop;
	vvector<tstring>	_cbStrings;
	C_TYPE_			    _t;
    V_TYPE_             _vtype;         //variable type
};



class CLbEditable : public CListBox
{
// Construction
public:
	CLbEditable();

// Attributes
public:

	int  InsertItem(CLbItem* pItem)
	{
		int i = AddString(pItem->_prop.c_str());
		SetItemData(i, (DWORD)pItem);
        return i;
	}
    
    void          LBSetProp(int idx, char* sel);
    void          LBSetProp(const char* prop, char* sel);
    void          LBSetProp(const char* prop, int sel);
    void          LBSetProp(const char* prop, CLR& sel);
    
    const char*   GetProp(int idx);
    const char*   GetProp(char* idx);

// Operations
public:

	void DeleteContent()
	{
		while(GetCount())
		{
			CLbItem* p = (CLbItem*)GetItemData(0);
			delete p;
			DeleteString(0);
		}
	}
    void KillFOcus(BOOL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLbEditable)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
// Implementation
public:
	virtual ~CLbEditable();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLbEditable)
	afx_msg void OnSelchange();
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
    

	DECLARE_MESSAGE_MAP()
//////////////////////////////////////
public:	
    BOOL            _withKeys;
    CWnd*           _pParent;
	CEditFromLb		_edit;
    CEditFromLb		_editkey;
	CCbFromLb		_combo;
    BOOL            _force;    
};



/////////////////////////////////////////////////////////////////////////////
#endif //LBEDITABLE_H
