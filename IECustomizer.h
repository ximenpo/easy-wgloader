#pragma once

#include	"simple-win32/dispatch_impl.h"
#include	"simple-win32/atl/IEHostWindow.h"

#include	"easy-wgloader.h"

class IECustomizer :
	public IEHostWindow,
	private DispatchImpl
{
public:
	IECustomizer(void);
	~IECustomizer(void);

public:
	DECLARE_POLY_AGGREGATABLE(IECustomizer);

public:
	DISPATCH_ITEMS_BEGIN(IECustomizer)
		DISPATCH_READONLYPROP(101, g_param.cs_IsGameLoader.c_str(), do_IsGameLoader)
		DISPATCH_FUNCTION(102, g_param.cs_LoadGame.c_str(), do_LoadGame)
		DISPATCH_ITEMS_END()

private:
	void	do_IsGameLoader(_variant_t& ret);
	void	do_LoadGame(_variant_t url, _variant_t title, _variant_t& ret);

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

	virtual HRESULT STDMETHODCALLTYPE		GetExternal(
		/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
};

