// easy-wgloader.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "easy-wgloader.h"

#include "DlgLogin.h"
#include "DlgGame.h"

#include <fstream>

#include "simple/app.h"
#include "simple/string.h"
#include "simple-win32/mac.h"
#include "simple-win32/ie_download.h"

//	config data
stringify_data	g_config;
Params			g_param;
HINSTANCE		g_instace;

//
//	Initialize
//
static	bool	Initialize(HINSTANCE hInstance)
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
			std::wstring	str(__wargv[2]);
			s_config_ex.assign(string_wchar_to_ansi(str));
		}
		if(__argc >= 4) {
			std::wstring	str(__wargv[3]);
			s_config_temp.assign(string_wchar_to_ansi(str));
		}
	}

	::SetCurrentDirectoryA(app_dir().c_str());
	std::string	cfg_dir(".\\");
	if(__argc >= 2){
		std::wstring	dir(__wargv[1]);
		if(::PathIsDirectoryW(dir.c_str())) {
			cfg_dir.assign(string_wchar_to_ansi(dir));
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
			return	false;
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

		std::string	data	= g_config.get_value("config/api_IsGameLoader","");
		if(!data.empty()){
			g_param.cs_IsGameLoader.assign(string_ansi_to_wchar(data));
		}
		data	= g_config.get_value("config/api_LoadGame","");
		if(!data.empty()){
			g_param.cs_LoadGame.assign(string_ansi_to_wchar(data));
		}
	}

	::SetCurrentDirectoryA(cfg_dir.c_str());

	return	true;
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if(!Initialize(hInstance)){
		return	EXIT_FAILURE;
	}

	//
	//	Main Procedure.
	//
	CoInitialize(NULL);
	g_module.Init(NULL, hInstance);

	{
		g_param.clear_temporary_vars();
		g_param.delay		= atoi(g_config.get_value("config/delay", "0").c_str());
		g_param.wnd_title	= g_config.get_value("config/title", "");
		g_param.wnd_icon	= (HICON)::LoadImageA(NULL, 
			g_config.get_value("config/icon", "").c_str(),
			IMAGE_ICON,
			0, 0,
			LR_SHARED|LR_DEFAULTSIZE|LR_LOADFROMFILE
			);
	}

	//	report
	do
	{
		std::string	report	= g_config.get_value("config/report", "");
		if(report.empty()) {
			break;
		}

		if(report.find_first_of("{}") != std::string::npos){
			string_replace(report, "{MAC}", Mac_GetPhysicalAdapterMac());
		}

		class URLReportCallbacker	: public IE_DownloadCallbacker
		{
		public:
			virtual /* [local] */ HRESULT STDMETHODCALLTYPE OnDataAvailable(
				/* [in] */ DWORD grfBSCF,
				/* [in] */ DWORD dwSize,
				/* [in] */ FORMATETC *pformatetc,
				/* [in] */ STGMEDIUM *pstgmed)
			{
				return	E_ABORT;
			}
		};
		URLReportCallbacker	callback;
		if(SUCCEEDED(URLOpenStreamA(NULL, report.c_str(), 0, &callback))){
		}
	}while(false);

	// UIs.
	do
	{
		//	login
		{
			LoginDialog	dlg;
			dlg.Create(NULL);

			MSG msg;
			while (dlg.IsWindow() && GetMessage(&msg, NULL, 0, 0))
			{
				//	hack for webbrowser's TAB/RETURN/DELETE
				if(		msg.message >= WM_KEYFIRST
					&&	msg.message <= WM_KEYLAST
					&&	dlg.PreProcessKeyboardMessage(&msg)
					){
						continue;
				}
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
	CoUninitialize();

	return (int) EXIT_SUCCESS;
}
