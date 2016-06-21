#pragma once

#include "resource.h"

struct	Params{
	unsigned int	delay;		//	第一次延迟显示时间（秒）
	std::wstring	game_url;	//	进入游戏网址
	std::wstring	game_title;	//	游戏标题

	void	clear(){
		delay	= 0;
		game_url.clear();
		game_title.clear();
	};
};

extern	CComModule		g_module; 
extern	stringify_data	g_config;
extern	Params			g_param;
