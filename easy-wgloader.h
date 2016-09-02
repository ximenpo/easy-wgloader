#pragma once

#include "resource.h"

struct	Params{
	bool			debug;		//	是否调试状态
	
	std::string		wnd_title;
	HICON			wnd_icon;

	std::string		auth_code;

	std::wstring	cs_REG_IE_OPTION;
	std::wstring	cs_IsGameLoader;
	std::wstring	cs_LoadGame;

	std::string		cs_AUTH_ENV_NAME;
	std::string		cs_AUTH_COOKIE_NAME;
	std::string		cs_AUTH_COOKIE_DOMAIN;
	
	//	temporary vars
	unsigned int	delay;		//	第一次延迟显示时间（秒）
	HWND			main_wnd;	//	主窗口句柄
	std::wstring	game_url;	//	进入游戏网址
	std::wstring	game_title;	//	游戏标题
	
	void	clear_temporary_vars(){
		main_wnd	= 0;
		delay		= 0;
		game_url.clear();
		game_title.clear();
	};
};

extern	HINSTANCE		g_instace;

extern	CComModule		g_module; 
extern	stringify_data	g_config;
extern	Params			g_param;
