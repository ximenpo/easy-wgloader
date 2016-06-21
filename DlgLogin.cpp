#include "StdAfx.h"
#include "DlgLogin.h"

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

	RECT rc;
	GetClientRect(&rc);

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
