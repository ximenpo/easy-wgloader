#include "StdAfx.h"
#include "IECustomizer.h"

#include "easy-wgloader.h"

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
        /* [in] */ IDispatch __RPC_FAR *pdispReserved){
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
    /* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo) {
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
        /* [in] */ DWORD dw) {
	*pchKey	= CComBSTR(g_param.cs_REG_IE_OPTION.c_str()).Detach();
	return	S_OK;
}