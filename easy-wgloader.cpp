// easy-wgloader.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "easy-wgloader.h"
#include "DlgLogin.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EASYWGLOADER, szWindowClass, MAX_LOADSTRING);

	_Module.Init(NULL, hInstance);

	do
	{
		//	login
		{
			LoginDialog	dlg;
			if(dlg.DoModal(NULL) == IDCANCEL){
				break;
			}
		}

		//	game
		{
		}
	}while(true);

	_Module.Term();

	return (int) EXIT_SUCCESS;
}
