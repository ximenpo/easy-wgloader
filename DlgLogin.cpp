#include "StdAfx.h"
#include "DlgLogin.h"


LoginDialog::LoginDialog(void)
	:	m_pWeb(NULL)
{
}


LoginDialog::~LoginDialog(void)
{
}

LRESULT LoginDialog::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	::EndDialog(m_hWnd, wID);
	return 0;
}

LRESULT LoginDialog::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}



LRESULT LoginDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(!superClass::OnInitDialog(uMsg, wParam, lParam, bHandled)){
		return	FALSE;
	}

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
		m_ctrlWeb.DestroyWindow();
	}

	bHandled	= FALSE;
	return 0;
}
