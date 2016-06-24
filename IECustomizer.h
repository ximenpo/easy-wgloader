#pragma once

#include	"simple-win32/ie.h"

class IECustomizer :
	public IE_DocHostUIHandler
{
public:
	IECustomizer(void);
	~IECustomizer(void);

public:
    //·µ»ØS_OK£¬ÆÁ±ÎµôÓÒ¼ü²Ëµ¥
    virtual HRESULT STDMETHODCALLTYPE		ShowContextMenu(
        /* [in] */ DWORD dwID,
        /* [in] */ POINT __RPC_FAR *ppt,
        /* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
        /* [in] */ IDispatch __RPC_FAR *pdispReserved);
    virtual HRESULT STDMETHODCALLTYPE		GetHostInfo(
        /* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
    virtual HRESULT STDMETHODCALLTYPE		GetOptionKeyPath(
        /* [out] */ LPOLESTR __RPC_FAR *pchKey,
        /* [in] */ DWORD dw);
};

