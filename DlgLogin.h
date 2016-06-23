#pragma once

#include <map>

#include "atlwin.h"

#include "easy-wgloader.h"
#include "simple-win32/bitmap.h"
#include "simple-win32/dispatch_impl.h"
#include "simple-win32/gdiplus/subimage.h"

#include "IECustomizer.h"

class	ImageDialog;
class	ImageButton;

class LoginDialog :
	public CAxDialogImpl<LoginDialog, CAxWindow>,
	public IDispEventImpl<IDC_WEB,LoginDialog>,
	public DispatchImpl
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
	IECustomizer	m_IECustomizer;

	HBRUSH			m_hBrush;
	ImageDialog*	m_pImageDlg;

	typedef	std::map<int,ImageButton*>	ImageButtonList;
	ImageButtonList	m_imgButtons;

	Bitmap_HDC			m_memDC;
	SubImage_ImageCache	m_imgCache;

	void	do_CloseWindow();

	void	do_IsGameLoader(_variant_t& ret);
	void	do_LoadGame(_variant_t url, _variant_t title, _variant_t& ret);

private:
	DISPATCH_ITEMS_BEGIN(LoginDialog)
		DISPATCH_READONLYPROP(101, L"IsGameLoader", do_IsGameLoader)
		DISPATCH_FUNCTION(102, L"LoadGame", do_LoadGame)
		DISPATCH_ITEMS_END()

public:
	enum	{IDD = IDD_LOGIN};
	enum	{IDC_BTN_FIRST	= 10000, IDC_BTN_LAST=10100};
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNCHitTest)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		COMMAND_RANGE_HANDLER(IDC_BTN_FIRST, IDC_BTN_LAST, OnImageButtonClick)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
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
public:
	BEGIN_SINK_MAP(LoginDialog)
		SINK_ENTRY(IDC_WEB, 259, DocumentCompleteWeb)
	END_SINK_MAP()
	void __stdcall DocumentCompleteWeb(LPDISPATCH pDisp, VARIANT* URL);
};

