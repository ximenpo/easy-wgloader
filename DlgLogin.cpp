#include "StdAfx.h"
#include "DlgLogin.h"

#include	"simple/string.h"
#include	"simple-win32/res.h"

static	const	UINT	TIMER_SHOWWINDOW	= 100;

LoginDialog::LoginDialog(void)
	:	m_pWeb(NULL)
{
}


LoginDialog::~LoginDialog(void)
{
}

LRESULT LoginDialog::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DestroyWindow();
	return 0;
}

LRESULT LoginDialog::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled	= FALSE;
	return 0;
}

LRESULT LoginDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(!superClass::OnInitDialog(uMsg, wParam, lParam, bHandled)){
		return	FALSE;
	}
	
	ShowWindow(SW_HIDE);
	{
		std::string	rgn	= g_config.get_value("login/rgn", "");
		std::string	img_trans	= g_config.get_value("login/img_trans", "");

		{
			LONG	wnd_ex_style	= this->GetWindowLongW(GWL_EXSTYLE) & ~WS_EX_LAYERED;
			if(rgn.empty()){
				wnd_ex_style	|=	WS_EX_LAYERED;
			}
			SetWindowLong(GWL_EXSTYLE, wnd_ex_style);
		}

		if(!rgn.empty()){
			//	rgn
			size_t	size	= 0;
			if(!Res_LoadFile(rgn.c_str(), NULL, size)){
				return	true;
			}
			std::auto_ptr<char>	data(new char[size]);
			if(!Res_LoadFile(rgn.c_str(), data.get(), size)){
				return	true;
			}

			XFORM xform;
			xform.eM11 = (FLOAT) 1.0;
			xform.eM22 = (FLOAT) 1.0;
			xform.eM12 = (FLOAT) 0.0;
			xform.eM21 = (FLOAT) 0.0;
			xform.eDx  = (FLOAT) 0.0;
			xform.eDy  = (FLOAT) 0.0;

			HRGN rgn = ExtCreateRegion(&xform, DWORD(size),(RGNDATA*)data.get());
			SetWindowRgn(rgn, FALSE);
		}else if(!img_trans.empty()){
			//	LayeredWindow with trans key
			unsigned int	r(255),g(0),b(255);
			if(string_tonumbers(img_trans, r, g, b)){
				SetLayeredWindowAttributes(m_hWnd, RGB(r,g,b), 0, LWA_COLORKEY);
			}
		}else{
			//	LayeredWindow with png, default.
		}
	}

	//	IE Control
	{
		m_ctrlWeb	= GetDlgItem(IDC_WEB);
		m_ctrlWeb.QueryControl(__uuidof(IWebBrowser2), (void**)&m_pWeb);

		{
			CComVariant	sURL(_T("http://www.baidu.com"));
			m_pWeb->Navigate2(&sURL,0,0,0,0);
		}
	}

	CenterWindow();

	SetTimer(TIMER_SHOWWINDOW, g_param.delay, NULL);
	return TRUE;
}


LRESULT LoginDialog::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	//	IE Control
	{
		if(NULL != m_pWeb){
			m_pWeb->Stop();
			m_pWeb->Release(); 
		}
	}

	bHandled	= FALSE;
	return 0;
}


LRESULT LoginDialog::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	switch(wParam){
	case TIMER_SHOWWINDOW:{
		KillTimer(TIMER_SHOWWINDOW);
		ShowWindow(SW_SHOW);
		UpdateWindow();
		m_ctrlWeb.UpdateWindow();
						  }break;
	}
	return 0;
}
