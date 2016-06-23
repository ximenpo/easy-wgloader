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
		//	config files' name
		std::string	s_config, s_config_ex, s_config_temp;
		{
			char	buf[MAX_PATH];
			LoadStringA(hInstance, IDS_CONFIG_TEMP, buf, MAX_PATH);
			s_config_temp.assign(buf);
			LoadStringA(hInstance, IDS_CONFIG_EX, buf, MAX_PATH);
			s_config_ex.assign(buf);
			LoadStringA(hInstance, IDS_CONFIG, buf, MAX_PATH);
			s_config.assign(buf);

			if(__argc >= 3) {
				s_config_ex.assign(__argv[2]);
			}
			if(__argc >= 4) {
				s_config_temp.assign(__argv[3]);
			}
		}

		//	System Strings
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

		//	debug options
		{
#ifndef	NDEBUG
			cfg_dir	= "..\\bin\\";
#endif
			g_param.debug	= false;
			if(::PathFileExistsA((cfg_dir + "debug.ini").c_str())) {
				g_param.debug	= true;
			}
		}

		//	first config(overide extend and normal)
		if(::PathFileExistsA((cfg_dir + s_config_temp).c_str())){
			std::ifstream	ifs(cfg_dir + s_config_temp);
			if(!ifs || !stringify_from_ini_stream(g_config, ifs)){
				//	ignore errors ...
			}

			if(!g_param.debug) {
				DeleteFileA((cfg_dir + s_config_temp).c_str());
			}
		}

		//	exttend	config(overide normal)
		if(::PathFileExistsA((cfg_dir + s_config_ex).c_str())){
			std::ifstream	ifs(cfg_dir + s_config_ex);
			if(!ifs || !stringify_from_ini_stream(g_config, ifs)){
				//	ignore errors ...
			}
		}

		//	global config
		{
			std::ifstream	ifs(cfg_dir + s_config);
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
