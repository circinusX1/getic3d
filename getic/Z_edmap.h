#if 0
/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Sep 16 21:19:55 2004
 */
/* Compiler settings for F:\getae\getic\Z_edmap.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __Z_edmap_h__
#define __Z_edmap_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IIGetic_FWD_DEFINED__
#define __IIGetic_FWD_DEFINED__
typedef interface IIGetic IIGetic;
#endif 	/* __IIGetic_FWD_DEFINED__ */


#ifndef __IGeticPlug_FWD_DEFINED__
#define __IGeticPlug_FWD_DEFINED__
typedef interface IGeticPlug IGeticPlug;
#endif 	/* __IGeticPlug_FWD_DEFINED__ */


#ifndef __IGetic_FWD_DEFINED__
#define __IGetic_FWD_DEFINED__

#ifdef __cplusplus
typedef class IGetic IGetic;
#else
typedef struct IGetic IGetic;
#endif /* __cplusplus */

#endif 	/* __IGetic_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_Z_edmap_0000 */
/* [local] */ 

typedef struct  _Plg_Vertex
    {
    float xyz[ 3 ];
    float nrm[ 3 ];
    float uv[ 2 ];
    float lm[ 2 ];
    }	Plg_Vertex;

typedef struct  _Plg_Poly
    {
    size_t flags;
    BYTE color[ 4 ];
    Plg_Vertex __RPC_FAR *vXes;
    int nCnt;
    }	Plg_Poly;

typedef struct  _Plg_Brush
    {
    size_t flags;
    Plg_Poly __RPC_FAR *pPolys;
    int nCnt;
    short name[ 32 ];
    }	Plg_Brush;

typedef struct  _Plg_Plane
    {
    float normal[ 3 ];
    float ddist;
    }	Plg_Plane;

typedef 
enum _EPLUGTYPE
    {	PLUG_IMPORTER	= 1,
	PLUG_EXPORTER	= PLUG_IMPORTER + 1
    }	EPLUGTYPE;



extern RPC_IF_HANDLE __MIDL_itf_Z_edmap_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Z_edmap_0000_v0_0_s_ifspec;


#ifndef __ZedmapLib_LIBRARY_DEFINED__
#define __ZedmapLib_LIBRARY_DEFINED__

/* library ZedmapLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ZedmapLib;

#ifndef __IIGetic_INTERFACE_DEFINED__
#define __IIGetic_INTERFACE_DEFINED__

/* interface IIGetic */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IIGetic;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7ABCC0EA-9A33-4713-BCDA-85C8DE424B6C")
    IIGetic : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Test( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IIGeticVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IIGetic __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IIGetic __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IIGetic __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IIGetic __RPC_FAR * This,
            /* [out] */ size_t __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IIGetic __RPC_FAR * This,
            /* [in] */ size_t iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IIGetic __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ size_t cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IIGetic __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ size_t __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Test )( 
            IIGetic __RPC_FAR * This);
        
        END_INTERFACE
    } IIGeticVtbl;

    interface IIGetic
    {
        CONST_VTBL struct IIGeticVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIGetic_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IIGetic_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IIGetic_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IIGetic_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IIGetic_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IIGetic_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IIGetic_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IIGetic_Test(This)	\
    (This)->lpVtbl -> Test(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IIGetic_Test_Proxy( 
    IIGetic __RPC_FAR * This);


void __RPC_STUB IIGetic_Test_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    size_t *_pdwStubPhase);



#endif 	/* __IIGetic_INTERFACE_DEFINED__ */


#ifndef __IGeticPlug_INTERFACE_DEFINED__
#define __IGeticPlug_INTERFACE_DEFINED__

/* interface IGeticPlug */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IGeticPlug;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2CADEA3E-A57C-4336-AD13-91BE804EE9F4")
    IGeticPlug : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ImportFile( 
            BSTR bsFileName,
            Plg_Brush __RPC_FAR *__RPC_FAR *ppBrush) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExportFile( 
            BSTR bsFileName,
            Plg_Brush __RPC_FAR *pBrush) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMenuStringAndType( 
            BSTR __RPC_FAR *bsFileName,
            EPLUGTYPE __RPC_FAR *type) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGeticPlugVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IGeticPlug __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IGeticPlug __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IGeticPlug __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IGeticPlug __RPC_FAR * This,
            /* [out] */ size_t __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IGeticPlug __RPC_FAR * This,
            /* [in] */ size_t iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IGeticPlug __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ size_t cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IGeticPlug __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ size_t __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ImportFile )( 
            IGeticPlug __RPC_FAR * This,
            BSTR bsFileName,
            Plg_Brush __RPC_FAR *__RPC_FAR *ppBrush);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ExportFile )( 
            IGeticPlug __RPC_FAR * This,
            BSTR bsFileName,
            Plg_Brush __RPC_FAR *pBrush);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetMenuStringAndType )( 
            IGeticPlug __RPC_FAR * This,
            BSTR __RPC_FAR *bsFileName,
            EPLUGTYPE __RPC_FAR *type);
        
        END_INTERFACE
    } IGeticPlugVtbl;

    interface IGeticPlug
    {
        CONST_VTBL struct IGeticPlugVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGeticPlug_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGeticPlug_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGeticPlug_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGeticPlug_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IGeticPlug_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IGeticPlug_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IGeticPlug_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IGeticPlug_ImportFile(This,bsFileName,ppBrush)	\
    (This)->lpVtbl -> ImportFile(This,bsFileName,ppBrush)

#define IGeticPlug_ExportFile(This,bsFileName,pBrush)	\
    (This)->lpVtbl -> ExportFile(This,bsFileName,pBrush)

#define IGeticPlug_GetMenuStringAndType(This,bsFileName,type)	\
    (This)->lpVtbl -> GetMenuStringAndType(This,bsFileName,type)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGeticPlug_ImportFile_Proxy( 
    IGeticPlug __RPC_FAR * This,
    BSTR bsFileName,
    Plg_Brush __RPC_FAR *__RPC_FAR *ppBrush);


void __RPC_STUB IGeticPlug_ImportFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    size_t *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGeticPlug_ExportFile_Proxy( 
    IGeticPlug __RPC_FAR * This,
    BSTR bsFileName,
    Plg_Brush __RPC_FAR *pBrush);


void __RPC_STUB IGeticPlug_ExportFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    size_t *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGeticPlug_GetMenuStringAndType_Proxy( 
    IGeticPlug __RPC_FAR * This,
    BSTR __RPC_FAR *bsFileName,
    EPLUGTYPE __RPC_FAR *type);


void __RPC_STUB IGeticPlug_GetMenuStringAndType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    size_t *_pdwStubPhase);



#endif 	/* __IGeticPlug_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_IGetic;

#ifdef __cplusplus

class DECLSPEC_UUID("A621848E-FFAB-441B-8DD0-E33B249EC29C")
IGetic;
#endif
#endif /* __ZedmapLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
#endif //0