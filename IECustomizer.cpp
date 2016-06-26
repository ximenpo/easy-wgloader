#include "StdAfx.h"
#include "IECustomizer.h"

void	IEExternal::do_IsGameLoader(_variant_t& ret) {
	ret	= VARIANT_TRUE;
}

void	IEExternal::do_LoadGame(_variant_t url, _variant_t title, _variant_t& ret) {
	g_param.game_url	= url.bstrVal;
	g_param.game_title	= title.bstrVal;

	::SendMessageA(g_param.main_wnd, WM_CLOSE, 0, 0);

	ret	= VARIANT_TRUE;
}


IECustomizer::IECustomizer(void)
{
}

IECustomizer::~IECustomizer(void)
{
}

//·µ»ØS_OK£¬ÆÁ±ÎµôÓÒ¼ü²Ëµ¥
HRESULT STDMETHODCALLTYPE IECustomizer::ShowContextMenu(
	/* [in] */ DWORD dwID,
	/* [in] */ POINT __RPC_FAR *ppt,
	/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
	/* [in] */ IDispatch __RPC_FAR *pdispReserved)
{
	if(g_param.debug){
		return	S_FALSE;
	}

	switch( dwID )
	{
	case CONTEXT_MENU_CONTROL:
	case CONTEXT_MENU_TEXTSELECT:
	case CONTEXT_MENU_UNKNOWN:
	case CONTEXT_MENU_VSCROLL:
	case CONTEXT_MENU_HSCROLL:
		return S_FALSE;
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE IECustomizer::GetHostInfo(
	/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo)
{
	if(g_param.debug){
		return	S_FALSE;
	}

	pInfo->dwFlags	|= DOCHOSTUIFLAG_DIALOG
		|	DOCHOSTUIFLAG_ACTIVATE_CLIENTHIT_ONLY
		|	DOCHOSTUIFLAG_NO3DBORDER
		|	DOCHOSTUIFLAG_NO3DOUTERBORDER
		|	DOCHOSTUIFLAG_ENABLE_REDIRECT_NOTIFICATION
		;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE IECustomizer::GetOptionKeyPath(
	/* [out] */ LPOLESTR __RPC_FAR *pchKey,
	/* [in] */ DWORD dw)
{
	*pchKey	= AtlAllocTaskOleString(g_param.cs_REG_IE_OPTION.c_str());
	return	S_OK;
}

HRESULT STDMETHODCALLTYPE IECustomizer::GetExternal(
	/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
{
	if(ppDispatch == NULL) {
		return E_POINTER;
	}

	external_.AddRef();
	*ppDispatch	= &external_;

	return S_OK;
}


HRESULT STDMETHODCALLTYPE	IECustomizer::ShowMessage(
    /* [in] */ HWND hwnd,
    /* [in] */ LPOLESTR lpstrText,
    /* [in] */ LPOLESTR lpstrCaption,
    /* [in] */ DWORD dwType,
    /* [in] */ LPOLESTR lpstrHelpFile,
    /* [in] */ DWORD dwHelpContext,
    /* [out] */ LRESULT *plResult)
{
	if(g_param.debug){
		return	S_FALSE;
	}

    return	S_OK;
}