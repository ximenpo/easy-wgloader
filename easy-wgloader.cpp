// easy-wgloader.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "easy-wgloader.h"

#include "DlgLogin.h"
#include "DlgGame.h"

#include <fstream>

#include "simple/app.h"

//	config data
stringify_data	g_config;
Params			g_param;
HINSTANCE		g_instace;


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
	{
		//	Strings
		{
			wchar_t	buf[MAX_PATH];
			
			LoadStringW(hInstance, IDS_REG_IE_OPTION, buf, MAX_PATH);
			g_param.cs_REG_IE_OPTION.assign(buf);

			LoadStringW(hInstance, IDS_IsGameLoader, buf, MAX_PATH);
			g_param.cs_IsGameLoader.assign(buf);
			
			LoadStringW(hInstance, IDS_LoadGame, buf, MAX_PATH);
			g_param.cs_LoadGame.assign(buf);
		}

		::SetCurrentDirectoryA(app_dir().c_str());
		std::string	cfg_dir(".\\");
		if(__argc >= 2){
			if(::PathIsDirectoryA(__argv[1])){
				cfg_dir.assign(__argv[1]);
				cfg_dir.append("\\");
			}
		}
		
#ifndef	NDEBUG
		cfg_dir	= "..\\bin\\";
#endif

		//	first	config(overide global)
		if(::PathFileExistsA((cfg_dir+"first.ini").c_str())){
			std::ifstream	ifs(cfg_dir+"first.ini");
			if(!ifs || !stringify_from_ini_stream(g_config, ifs)){
				//	ignore errors ...
			}
#ifdef	NDEBUG
			DeleteFileA((cfg_dir+"first.ini").c_str());
#endif
		}

		//	global config
		{
			std::ifstream	ifs(cfg_dir+"config.ini");
			if(!ifs || !stringify_from_ini_stream(g_config, ifs)){
				return	EXIT_FAILURE;
			}
		}
		::SetCurrentDirectoryA(cfg_dir.c_str());
	}

	//
	//	Main Procedure.
	//

	g_module.Init(NULL, hInstance);

	{
		g_param.clear_temporary_vars();
		g_param.delay	= atoi(g_config.get_value("config/delay", "0").c_str());
	}

	do
	{
		g_param.debug	= g_config.get_value("config/debug","") == "1";

		//	login
		{
			LoginDialog	dlg;
			dlg.Create(NULL);

			MSG msg;
			while (dlg.IsWindow() && GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if(g_param.game_url.empty()){
			break;
		}

		//	game
		{
			GameDialog	dlg;
			dlg.DoModal(NULL);
		}

		g_param.clear_temporary_vars();
	}while(true);

	g_module.Term();

	return (int) EXIT_SUCCESS;
}
