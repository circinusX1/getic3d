// PlugInCat.h: interface for the PlugInCat class.
//
//////////////////////////////////////////////////////////////////////
#include <comcat.h>
#include <objsafe.h>
#include <atlbase.h>

#ifndef __PLUGINCAT_H__
#define __PLUGINCAT_H__

#ifdef COM_WAY

extern const GUID CatGuid;

class PlugInCat  
{
public:

    HRESULT DeleteCategory(TCHAR* catDescription="Getic 3D Editor")
    {
        USES_CONVERSION;

        CComPtr<ICatRegister>   catInfo;
        HRESULT hr = catInfo.CoCreateInstance(CLSID_StdComponentCategoriesMgr);
        if(FAILED(hr))return hr;

        CATEGORYINFO    ci[1];

        ci[0].catid = CatGuid;
        ci[0].lcid  = 0x409;
        wcscpy(ci[0].szDescription, T2W(catDescription));
        return catInfo->UnRegisterCategories(1, &ci[0].catid);
    }

    HRESULT    AddCLSID(const GUID& clsid)
    {
        USES_CONVERSION;

        CComPtr<ICatRegister>   catInfo;
        HRESULT hr = catInfo.CoCreateInstance(CLSID_StdComponentCategoriesMgr);
        if(FAILED(hr))return hr;

        // ensure the category registration
        CATEGORYINFO    catinfo;
        catinfo.catid = CatGuid;
        catinfo.lcid  = 0x409;
        CATID        rgcatid[1] ;
        rgcatid[0] = CatGuid;

        ::wcscpy(catinfo.szDescription, T2W ("Getic 3D 3.0"));
        hr = catInfo->RegisterCategories(1, &catinfo);
        hr = catInfo->RegisterClassImplCategories(clsid,1,rgcatid);

        return hr;

    }
    
    HRESULT    RemoveCLSID(const GUID& clsid, CATID catidIn = CatGuid)
    {
        USES_CONVERSION;

        CComPtr<ICatRegister>   catInfo;
        HRESULT hr = catInfo.CoCreateInstance(CLSID_StdComponentCategoriesMgr);
        if(FAILED(hr))return hr;
        
        CATID   catid = catidIn;
        hr = catInfo->UnRegisterClassImplCategories(clsid,1,&catid);
        return hr;
        
     

    }
    

};

#endif //COM_WAY
#endif // __PLUGINCAT_H__
