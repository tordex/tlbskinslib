#pragma once
#include "color.h"
#include "brush.h"
#include "pen.h"

namespace TxSkin
{
	interface IEnumSkinsCallBack
	{
		virtual void onSkinFileFound(LPCWSTR skinFile, LPWSTR shortPath) = 0;
	};

	void			enumSkins(IEnumSkinsCallBack* callback, LPCWSTR path, LPCWSTR iniFile);
	TxSkin::color	parseColor(LPWSTR str);
	TxSkin::color	parseColor(LPCWSTR section, LPCWSTR key, TxSkin::color defColor, LPCWSTR iniFile);
	TxSkin::pen*	parsePen(LPWSTR str);
	TxSkin::pen*	parsePen(LPWSTR section, LPWSTR key, LPWSTR iniFile);
	TxSkin::brush*	parseBrush(LPWSTR str);
	TxSkin::brush*	parseBrush(LPWSTR section, LPWSTR key, LPWSTR iniFile);
	RECT			parseRect(LPWSTR str);
	RECT			parseRect(LPWSTR section, LPWSTR key, LPWSTR iniFile);

}