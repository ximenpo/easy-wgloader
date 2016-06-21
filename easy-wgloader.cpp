// easy-wgloader.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "easy-wgloader.h"

#include "DlgLogin.h"
#include "DlgGame.h"

#include <fstream>

#include "simple/app.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//
	//	Initialize
	//
	::SetCurrentDirectoryA(app_dir().c_str());
	{
		std::string	cfg_file;
		if(__argc >= 2){
			if(::PathIsDirectoryA(__argv[1])){
				cfg_file.assign(__argv[1]);
				cfg_file.append("\\");
			}
		}
		cfg_file.append("config.ini");
		std::ifstream	ifs(cfg_file);
		if(!ifs || !stringify_from_ini_stream(g_config, ifs)){
			return	EXIT_FAILURE;
		}
	}

	//
	//	Main Procedure.
	//

	g_module.Init(NULL, hInstance);

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
			GameDialog	dlg;
			dlg.DoModal(NULL);
		}
	}while(true);

	g_module.Term();

	return (int) EXIT_SUCCESS;
}
