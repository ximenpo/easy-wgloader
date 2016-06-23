#include "StdAfx.h"
#include "DlgLogin.h"

#include	<ShellAPI.h>

#include	"DlgImage.h"

#include	"simple/string.h"
#include	"simple-win32/res.h"
#include	"simple-win32/win.h"
#include	"simple-win32/string.h"
#include	"simple-win32/atl/ImageButton.h"

static	const	UINT	TIMER_NAVIGATE		= 101;
static	const	UINT	TIMER_SHOWWINDOW	= 102;
static	const	UINT	TIMER_CLOSEWINDOW	= 103;

LoginDialog::LoginDialog(void)
	:	m_pWeb(NULL)
	,	m_hBrush(NULL)
	,	m_pImageDlg(NULL)
{
}


LoginDialog::~LoginDialog(void)
{
	if(NULL != m_pImageDlg) {
		m_pImageDlg->DestroyWindow();
		delete	m_pImageDlg;
		m_pImageDlg	= NULL;
	}
}


void	LoginDialog::do_CloseWindow() {
	SetTimer(TIMER_CLOSEWINDOW, 0, NULL);
}


void	LoginDialog::do_IsGameLoader(_variant_t& ret) {
	ret	= VARIANT_TRUE;
}


void	LoginDialog::do_LoadGame(_variant_t url, _variant_t title, _variant_t& ret) {
	g_param.game_url	= url.bstrVal;
	g_param.game_title	= title.bstrVal;

	this->do_CloseWindow();

	ret	= VARIANT_TRUE;
}

LRESULT LoginDialog::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	this->do_CloseWindow();
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
			m_imgCache.clear();
		}
	}

	//	window style.
	{
		std::string	rgn	= g_config.get_value("login/rgn", "");
		std::string	img_trans	= g_config.get_value("login/img_trans", "");

		{
			DWORD	wnd_ex_style	= this->GetExStyle() & ~WS_EX_LAYERED;
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

	//	Buttons
	{
		int	btn_id	= IDC_BTN_FIRST;
		std::deque<std::string>	btn_names;
		string_split(g_config.get_value("login/buttons", ""), "|", std::back_inserter(btn_names), true);
		std::deque<std::string>::const_iterator
			it		= btn_names.begin(),
			it_end	= btn_names.end();
		for(; it != it_end; ++it) {
			stringify::node_id	cfg_id;
			if(!g_config.fetch(*it, &cfg_id)){
				continue;
			}

			RECT	rc;
			if(!String_ToWHRect(g_config.get_value(*it, "rect", ""), rc)) {
				continue;
			}

			ImageButton*	btn	= new ImageButton();
			::CreateWindowA("BUTTON",
				it->c_str(),
				WS_VISIBLE|WS_CHILD|BS_OWNERDRAW,
				rc.left, rc.top, rc.right, rc.bottom,
				m_hWnd,
				HMENU(++btn_id),
				g_instace,
				NULL
				);
			btn->SubclassWindow(GetDlgItem(btn_id));
			btn->SetWindowLongW(GWL_USERDATA, cfg_id);

			m_imgCache.load(btn->NormalImage,	g_config.get_value(*it, "img_u", ""));
			m_imgCache.load(btn->SelectedImage, g_config.get_value(*it, "img_d", ""));
			m_imgCache.load(btn->HoverImage,	g_config.get_value(*it, "img_a", ""));
			m_imgCache.load(btn->DisabledImage, g_config.get_value(*it, "img_g", ""));

			m_imgButtons.insert(std::make_pair(btn_id, btn));
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

		{
			m_IECustomizer.SetExternalDispatch((DispatchImpl*)this);
		}

		//	debug
		{
			m_pWeb->put_Silent(g_param.debug ? VARIANT_FALSE : VARIANT_TRUE);
		}
	}

	SetTimer(TIMER_NAVIGATE, 0, NULL);
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

	//	Buttons
	{
		ImageButtonList::const_iterator
			it		= m_imgButtons.begin(),
			it_end	= m_imgButtons.end()
			;

		for(; it != it_end; ++it){
			it->second->DestroyWindow();
			delete	it->second;
		}
		m_imgButtons.clear();
	}

	//	Brush
	if(NULL != m_hBrush) {
		DeleteObject(m_hBrush);
		m_hBrush	= NULL;
	}

	m_memDC.uninitialize();

	bHandled	= FALSE;
	return 0;
}


LRESULT LoginDialog::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	switch(wParam){
	case TIMER_NAVIGATE:
		{
			KillTimer(TIMER_NAVIGATE);
			CComVariant	sURL(string_ansi_to_wchar(g_config.get_value("login/url", "about:blank"), NULL));
			m_pWeb->Navigate2(&sURL,0,0,0,0);

			SetTimer(TIMER_SHOWWINDOW, g_param.delay, NULL);
		}break;
	case TIMER_SHOWWINDOW:
		{
			KillTimer(TIMER_SHOWWINDOW);
			if(NULL != m_pImageDlg) {
				m_pImageDlg->ShowWindow(SW_SHOW);
			}
			ShowWindow(SW_SHOW);
		}break;
	case TIMER_CLOSEWINDOW:
		{
			KillTimer(TIMER_CLOSEWINDOW);
			DestroyWindow();
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


LRESULT LoginDialog::OnImageButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled) {
	do{
		ImageButtonList::const_iterator	it	= m_imgButtons.find(wID);
		if(it == m_imgButtons.end()){
			break;
		}

		std::string		scheme, *url;
		stringify::node_id	cfg_id	= it->second->GetWindowLongW(GWL_USERDATA);
		if(		!g_config.fetch(cfg_id, "url", &url)
			||	!string_parse_url(url->c_str(), &scheme)
			){
				break;
		}

		//	web links
		if(		0 == _stricmp(scheme.c_str(), "http")
			||	0 == _stricmp(scheme.c_str(), "https")
			){
				::ShellExecuteA(m_hWnd, "open", url->c_str(), NULL, NULL, SW_SHOW);
				break;
		}

		if(0 != _stricmp(scheme.c_str(), "app")) {
			break;
		}

		if(0 == _stricmp(url->c_str(), "app://close")) {
			this->do_CloseWindow();
			break;
		}

		if(0 == _stricmp(url->c_str(), "app://minimize")) {
			if(NULL != m_pImageDlg) {
				m_pImageDlg->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			}else{
				this->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			}
			break;
		}

	}while(false);

	return	0;
}


void __stdcall LoginDialog::DocumentCompleteWeb(LPDISPATCH pDisp, VARIANT* URL)
{
	IDispatch*	pDoc;
	if(SUCCEEDED(m_pWeb->get_Document(&pDoc))) {
		m_IECustomizer.AttachTo(pDoc);
		pDoc->Release();
	}
}
