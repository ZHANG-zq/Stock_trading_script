

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Dec 17 22:46:59 2012
 */
/* Compiler settings for mk.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __mk_h_h__
#define __mk_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __Imk_FWD_DEFINED__
#define __Imk_FWD_DEFINED__
typedef interface Imk Imk;
#endif 	/* __Imk_FWD_DEFINED__ */


#ifndef __mk_FWD_DEFINED__
#define __mk_FWD_DEFINED__

#ifdef __cplusplus
typedef class mk mk;
#else
typedef struct mk mk;
#endif /* __cplusplus */

#endif 	/* __mk_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __mk_LIBRARY_DEFINED__
#define __mk_LIBRARY_DEFINED__

/* library mk */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_mk;

#ifndef __Imk_DISPINTERFACE_DEFINED__
#define __Imk_DISPINTERFACE_DEFINED__

/* dispinterface Imk */
/* [uuid] */ 


EXTERN_C const IID DIID_Imk;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("71A97981-2CD6-4F8D-BEE2-8599A58725DA")
    Imk : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ImkVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Imk * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Imk * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Imk * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            Imk * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            Imk * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            Imk * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            Imk * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ImkVtbl;

    interface Imk
    {
        CONST_VTBL struct ImkVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Imk_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define Imk_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define Imk_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define Imk_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define Imk_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define Imk_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define Imk_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __Imk_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_mk;

#ifdef __cplusplus

class DECLSPEC_UUID("52E15AFB-9124-4592-8253-BB7D67DB6CC6")
mk;
#endif
#endif /* __mk_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


