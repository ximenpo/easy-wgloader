#include "StdAfx.h"
#include "DlgGame.h"

#include	"simple/string.h"

GameDialog::GameDialog(void)
	:	m_pWeb(NULL)
{
}


GameDialog::~GameDialog(void)
{
}

void	GameDialog::do_CloseWindow(){
	char	title[MAX_PATH]	= {};
	::GetWindowTextA(m_hWnd, title, sizeof(title) - 1);
	
	std::string	tip	= g_config.get_value("game/close_tip","");
	string_trim(tip);

	if(!tip.empty() && IDYES != ::MessageBoxA(m_hWnd, tip.c_str(), title, MB_YESNO|MB_DEFBUTTON2)){
		return;
	}

	::EndDialog(m_hWnd, 0);
}

LRESULT GameDialog::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	this->do_CloseWindow();
	return 0;
}


LRESULT GameDialog::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	return	1;
}


LRESULT GameDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	g_param.main_wnd	= m_hWnd;
	if(!superClass::OnInitDialog(uMsg, wParam, lParam, bHandled)){
		return	FALSE;
	}
	

	RECT rc;
	GetClientRect(&rc);

	//	Title
	{
		if(!g_param.game_title.empty()){
			this->SetWindowTextW(g_param.game_title.c_str());
		}else{
			::SetWindowTextA(m_hWnd, g_param.wnd_title.c_str());
		}
		
		if(NULL != g_param.wnd_icon) {
			this->SetIcon(g_param.wnd_icon, TRUE);
			this->SetIcon(g_param.wnd_icon, FALSE);
		}
	}

	//	IE Control
	{
		m_ctrlWeb	= GetDlgItem(IDC_WEB);
		m_ctrlWeb.QueryControl(__uuidof(IWebBrowser2), (void**)&m_pWeb);
		
		{
			m_pWeb->put_Resizable(VARIANT_FALSE);
		}

		{
			CComVariant	sURL(g_param.game_url.c_str());
			m_pWeb->Navigate2(&sURL,0,0,0,0);
		}
	}

	ShowWindow(SW_MAXIMIZE);

	return TRUE;
}


LRESULT GameDialog::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
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


LRESULT GameDialog::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	RECT rc;
	GetClientRect(&rc);

	if(m_ctrlWeb.m_hWnd){
		m_ctrlWeb.MoveWindow(rc.left, rc.top, rc.right, rc.bottom, FALSE);
	}

	return 0;
}


LRESULT GameDialog::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	this->do_CloseWindow();
	return 0;
}
