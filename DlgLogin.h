#pragma once

#include <map>

#include "atlwin.h"

#include "easy-wgloader.h"
#include "simple-win32/bitmap.h"
#include "simple-win32/gdiplus/subimage.h"

class	ImageDialog;
class	ImageButton;

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

	HBRUSH			m_hBrush;
	ImageDialog*	m_pImageDlg;

	typedef	std::map<int,ImageButton*>	ImageButtonList;
	ImageButtonList	m_imgButtons;

	Bitmap_HDC			m_memDC;
	SubImage_ImageCache	m_imgCache;

public:
	enum	{IDD = IDD_LOGIN};
	enum	{IDC_BTN_FIRST	= 10000, IDC_BTN_LAST=10100};
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNCHitTest)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
		COMMAND_RANGE_HANDLER(IDC_BTN_FIRST, IDC_BTN_LAST, OnImageButtonClick)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

private:
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNCHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnImageButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

