#pragma once

#include "atlwin.h"

#include "easy-wgloader.h"

class LoginDialog :
	public CAxDialogImpl<LoginDialog, CAxWindow>
{
public:
	LoginDialog(void);
	~LoginDialog(void);

protected:
	typedef	LoginDialog								thisClass;
	typedef	CAxDialogImpl<LoginDialog, CAxWindow>	superClass;

private: 
	CAxWindow		m_ctrlWeb;
	IWebBrowser2*	m_pWeb;

public:
	enum	{IDD = IDD_LOGIN};
	BEGIN_MSG_MAP(thisClass)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(superClass)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	END_MSG_MAP()

private:
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

