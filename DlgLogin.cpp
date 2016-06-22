#include "StdAfx.h"
#include "DlgLogin.h"

#include	"simple/string.h"
#include	"simple-win32/res.h"
#include	"simple-win32/win.h"
#include	"simple-win32/string.h"

#include	"DlgImage.h"

static	const	UINT	TIMER_SHOWWINDOW	= 100;

LoginDialog::LoginDialog(void)
	:	m_pWeb(NULL)
	,	m_hBrush(NULL)
	,	m_pImageDlg(NULL)
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
	RECT	rc_client;
	GetClientRect(&rc_client);

	PAINTSTRUCT ps;
	HDC hdc	= BeginPaint(&ps);

	if(m_memDC.get_HDC() != NULL){
		BitBlt(hdc, 
			rc_client.left, rc_client.top, 
			rc_client.right, rc_client.bottom, 
			m_memDC, 
			0, 0, 
			SRCCOPY
			);
	}

	EndPaint(&ps);
	bHandled	= FALSE;
	return 0;
}

LRESULT LoginDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(!superClass::OnInitDialog(uMsg, wParam, lParam, bHandled)){
		return	FALSE;
	}

	ShowWindow(SW_HIDE);

	//	window size.
	{
		int	w, h;
		if(string_tonumbers(g_config.get_value("login/size",""), w, h)){
			MoveWindow(0, 0, w, h, FALSE);
		}

		::SetWindowTextA(m_hWnd, g_config.get_value("login/title", "").c_str());
		CenterWindow();
	}

	//	bg imgs
	{
		RECT	rc;
		GetClientRect(&rc);
		HDC	hdc	= GetDC();
		m_memDC.initialize(rc.right - rc.left, rc.bottom - rc.top, hdc, RGB(0,0,0));
		ReleaseDC(hdc);

		SubImage	img;
		if(m_imgCache.load(img, g_config.get_value("login/img", ""))) {
			Gdiplus::Graphics graphics(m_memDC);
			graphics.DrawImage(img.image,
				0, 0,
				img.rect.X, img.rect.Y, img.rect.Width, img.rect.Height,
				Gdiplus::UnitPixel);
		}
	}

	//	window style.
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
			Win_SetRgnFromFile(m_hWnd, rgn.c_str());
		}else{
			if(!img_trans.empty()){
				//	LayeredWindow with trans key
				unsigned int	r(255),g(0),b(255);
				if(string_tonumbers(img_trans, r, g, b)){
					SetLayeredWindowAttributes(m_hWnd, RGB(r,g,b), 0, LWA_COLORKEY);
				}
			}else{
				//	LayeredWindow with png, default.
				m_hBrush	= ::CreateSolidBrush(RGB(255,0,255));
				SetLayeredWindowAttributes(m_hWnd, RGB(255,0,255), 0, LWA_COLORKEY);

				//	background image
				m_pImageDlg	= new ImageDialog();
				m_pImageDlg->Create(NULL);
				m_pImageDlg->UpdateImage(m_hWnd, m_memDC, g_config.get_value("login/title", "").c_str());

				// no need now.
				m_memDC.uninitialize();
			}
		}
	}

	//	IE Control
	{
		RECT	rc;
		String_ToRect(g_config.get_value("login/web_rect", ""), rc);
		m_ctrlWeb	= GetDlgItem(IDC_WEB);
		m_ctrlWeb.MoveWindow(&rc, FALSE);
		m_ctrlWeb.QueryControl(__uuidof(IWebBrowser2), (void**)&m_pWeb);

		{
			CComVariant	sURL(string_ansi_to_wchar(g_config.get_value("login/url", "about:blank"), NULL));
			m_pWeb->Navigate2(&sURL,0,0,0,0);
		}
	}

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
	//	Brush
	if(NULL != m_hBrush) {
		DeleteBrush(m_hBrush);
		m_hBrush	= NULL;
	}

	m_memDC.uninitialize();

	bHandled	= FALSE;
	return 0;
}


LRESULT LoginDialog::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	switch(wParam){
	case TIMER_SHOWWINDOW:{
		KillTimer(TIMER_SHOWWINDOW);
		if(NULL != m_pImageDlg) {
			m_pImageDlg->ShowWindow(SW_SHOW);
		}
		ShowWindow(SW_SHOW);

		//	TODO:
		//UpdateWindow();
		//m_ctrlWeb.UpdateWindow();
						  }break;
	}
	return 0;
}

LRESULT LoginDialog::OnNCHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	return HTCAPTION;
}


LRESULT LoginDialog::OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	if(NULL != m_hBrush) {
		return	LRESULT(m_hBrush);
	}

	bHandled	= FALSE;
	return 0;
}
