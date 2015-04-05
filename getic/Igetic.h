// IGetic.h : Declaration of the CIGetic

#ifndef __IGETIC_H_
#define __IGETIC_H_

#ifdef COM_WAY

#include "resource.h"       // main symbols

extern const GUID          CatGuid;
extern _ATL_CATMAP_ENTRY   CatEntry[];

/////////////////////////////////////////////////////////////////////////////
// CIGetic
class ATL_NO_VTABLE CIGetic : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CIGetic, &CLSID_IGetic>,
	public ISupportErrorInfo,
	public IDispatchImpl<IIGetic, &IID_IIGetic, &LIBID_ZedmapLib>
{
public:
	CIGetic()
	{
	}

    /*
    static const struct _ATL_CATMAP_ENTRY* GetCategoryMap() {
        return CatEntry;
    };
    */

DECLARE_REGISTRY_RESOURCEID(IDR_IGETIC)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CIGetic)
	COM_INTERFACE_ENTRY(IIGetic)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IIGetic
public:
	STDMETHOD(Test)();
};

#endif //COM_WAY
#endif //__IGETIC_H_
