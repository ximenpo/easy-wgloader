#include "StdAfx.h"
#include "IECustomizer.h"

#include "easy-wgloader.h"

IECustomizer::IECustomizer(void)
{
}


IECustomizer::~IECustomizer(void)
{
}

HRESULT STDMETHODCALLTYPE IECustomizer::GetHostInfo(
    /* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo) {
	if(g_param.debug){
		return	S_FALSE;
	}

    pInfo->dwFlags	|= DOCHOSTUIFLAG_DIALOG;
    return S_OK;
}
