#pragma once

#include "atlwin.h"

#include "easy-wgloader.h"

class GameDialog :
	public CAxDialogImpl<GameDialog, CAxWindow>,
	public IDispEventImpl<IDC_WEB,GameDialog>,
	public IDispEventImpl<IDC_WEB_DUMMY,GameDialog>
{
public:
	GameDialog(void);
	~GameDialog(void);

protected:
	typedef	GameDialog								thisClass;
	typedef	CAxDialogImpl<GameDialog, CAxWindow>	superClass;

private:
	void			do_CloseWindow();

private: 
	CAxWindow		m_ctrlWeb;
	IWebBrowser2*	m_pWeb;
	
	CAxWindow		m_ctrlWebDummy;
	IWebBrowser2*	m_pWebDummy;

public:
	enum	{IDD = IDD_GAME};
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

private:
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	BEGIN_SINK_MAP(GameDialog)
		SINK_ENTRY(IDC_WEB, 251, NewWindow2Web)
		SINK_ENTRY(IDC_WEB_DUMMY, 250, BeforeNavigate2WebDummy)
	END_SINK_MAP()
	void __stdcall NewWindow2Web(LPDISPATCH* ppDisp, BOOL* Cancel);
	void __stdcall BeforeNavigate2WebDummy(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
};

