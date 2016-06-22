// stdafx.cpp : 只包括标准包含文件的源文件
// easy-wgloader.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

#include	<GdiPlus.h>
#pragma		comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

//////////////////////////////////////////////////////////////////
class	GdiplusInitializer{
public:
	GdiplusInitializer(){
		GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiplusToken_, &gdiplusStartupInput, NULL);
	}
	~GdiplusInitializer(){
		GdiplusShutdown(gdiplusToken_);
	}

private:
	ULONG_PTR		gdiplusToken_;
};
static	GdiplusInitializer	s_gdiplus_initializer;

//////////////////////////////////////////////////////////////////
//	ATL Module
CComModule		g_module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()
