
//   #pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Thu Sep 16 20:57:52 2004
 */
/* Compiler settings for Z_edmap.idl:
    Os, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __Z_edmap_h_h__
#define __Z_edmap_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
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

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_Z_edmap_0000 */
/* [local] */ 

typedef struct _Plg_Vertex
    {
    float xyz[ 3 ];
    float nrm[ 3 ];
    float uv[ 2 ];
    float lm[ 2 ];
    } 	Plg_Vertex;

typedef struct _Plg_Poly
    {
    size_t flags;
    BYTE color[ 4 ];
    Plg_Vertex *vXes;
    int nCnt;
    } 	Plg_Poly;

typedef struct _Plg_Brush
    {
    size_t flags;
    Plg_Poly *pPolys;
    int nCnt;
    short name[ 32 ];
    } 	Plg_Brush;

typedef struct _Plg_Plane
    {
    float normal[ 3 ];
    float ddist;
    } 	Plg_Plane;

typedef 
enum _EPLUGTYPE
    {	PLUG_IMPORTER	= 1,
	PLUG_EXPORTER	= PLUG_IMPORTER + 1
    } 	EPLUGTYPE;



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
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IIGetic * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IIGetic * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IIGetic * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IIGetic * This,
            /* [out] */ size_t *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IIGetic * This,
            /* [in] */ size_t iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IIGetic * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ size_t cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IIGetic * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ size_t *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Test )( 
            IIGetic * This);
        
        END_INTERFACE
    } IIGeticVtbl;

    interface IIGetic
    {
        CONST_VTBL struct IIGeticVtbl *lpVtbl;
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
    IIGetic * This);


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
            Plg_Brush **ppBrush) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExportFile( 
            BSTR bsFileName,
            Plg_Brush *pBrush) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMenuStringAndType( 
            BSTR *bsFileName,
            EPLUGTYPE *type) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGeticPlugVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGeticPlug * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGeticPlug * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGeticPlug * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGeticPlug * This,
            /* [out] */ size_t *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGeticPlug * This,
            /* [in] */ size_t iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGeticPlug * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ size_t cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGeticPlug * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ size_t *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ImportFile )( 
            IGeticPlug * This,
            BSTR bsFileName,
            Plg_Brush **ppBrush);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExportFile )( 
            IGeticPlug * This,
            BSTR bsFileName,
            Plg_Brush *pBrush);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMenuStringAndType )( 
            IGeticPlug * This,
            BSTR *bsFileName,
            EPLUGTYPE *type);
        
        END_INTERFACE
    } IGeticPlugVtbl;

    interface IGeticPlug
    {
        CONST_VTBL struct IGeticPlugVtbl *lpVtbl;
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
    IGeticPlug * This,
    BSTR bsFileName,
    Plg_Brush **ppBrush);


void __RPC_STUB IGeticPlug_ImportFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    size_t *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGeticPlug_ExportFile_Proxy( 
    IGeticPlug * This,
    BSTR bsFileName,
    Plg_Brush *pBrush);


void __RPC_STUB IGeticPlug_ExportFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    size_t *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IGeticPlug_GetMenuStringAndType_Proxy( 
    IGeticPlug * This,
    BSTR *bsFileName,
    EPLUGTYPE *type);


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


