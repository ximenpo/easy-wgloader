#pragma once

#include "atlwin.h"

#include "easy-wgloader.h"

class ImageDialog :
	public CDialogImpl<ImageDialog>
{
public:
	ImageDialog(void);
	~ImageDialog(void);
	
public:
	enum	{IDD = IDD_DUMMY};
	void	UpdateImage(HWND hWnd, HDC hdc);

private:
	HWND	m_ctrlWnd;

private:
	BEGIN_MSG_MAP(ImageDialog)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNCHitTest)
		MESSAGE_HANDLER(WM_MOVE, OnMove)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()

private:
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNCHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
public:
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

