#pragma once

#include	"simple-win32/ie.h"

class IECustomizer :
	public IE_DocHostUIHandler
{
public:
	IECustomizer(void);
	~IECustomizer(void);

public:
    virtual HRESULT STDMETHODCALLTYPE		GetHostInfo(
        /* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
};

