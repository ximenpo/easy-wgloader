#include "StdAfx.h"
#include "DlgImage.h"


ImageDialog::ImageDialog(void)
	:	m_ctrlWnd(NULL)
{
}


ImageDialog::~ImageDialog(void)
{
}

void	ImageDialog::UpdateImage(HWND hWnd, HDC hdc, const char* title) {
	m_ctrlWnd	= hWnd;

	const BYTE	SourceConstantAlpha	= 255;
	HDC	dcScreen	= ::GetDC(NULL);

	RECT	rc;
	::GetWindowRect(m_ctrlWnd, &rc);
	POINT	ptWndPos	= {rc.left, rc.top};
	SIZE	szWndSize	= {rc.right - rc.left, rc.bottom - rc.top};

	// setup the blend function
	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, SourceConstantAlpha, AC_SRC_ALPHA };
	POINT ptSrc	= {0,0}; // start point of the copy from dcMemory to dcScreen

	// perform the alpha blend
	BOOL bRet= ::UpdateLayeredWindow(m_hWnd, dcScreen, &ptWndPos, &szWndSize, hdc,
		&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);
	::ReleaseDC(NULL, dcScreen);

	::SetWindowLongPtrW(m_ctrlWnd, GWL_HWNDPARENT, LONG(m_hWnd));

	::SetWindowTextA(m_hWnd, title);
}

LRESULT ImageDialog::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if(NULL != m_ctrlWnd) {
		::PostMessageA(m_ctrlWnd, WM_CLOSE, 0, 0);
	}
	return 0;
}

LRESULT ImageDialog::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(NULL != m_ctrlWnd) {
		::SetWindowPos(m_ctrlWnd, NULL, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), SWP_NOZORDER | SWP_NOMOVE);
	}
	return 0;
}

LRESULT ImageDialog::OnMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(NULL != m_ctrlWnd) {
		::SetWindowPos(m_ctrlWnd, NULL, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
	return 0;
}

LRESULT ImageDialog::OnNCHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	return HTCAPTION;
}
