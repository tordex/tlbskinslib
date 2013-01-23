#pragma once

#pragma warning ( disable : 4995 34 )


#include <Windows.h>
#include <tchar.h>
#include <ObjBase.h>
#include "xmltools.h"
#include <strsafe.h>
#include <string>
#include "..\..\cairo\src\cairo.h"
#include "..\..\cairo\src\cairo-win32.h"
#include "..\..\txdib\TxDIB.h"
#include "types.h"
#include "color.h"
#include "brush.h"
#include "pen.h"
#include "arrays.h"

interface ISkinCallBack
{
	virtual void OnElementClick(LPCWSTR id, int sliceID)			= 0;
	virtual void OnSliderValue(LPCWSTR id, int val, int sliceID)	= 0;
	virtual void OnSliderBegin(LPCWSTR id, int sliceID)	= 0;
	virtual void OnSliderEnd(LPCWSTR id, int sliceID)	= 0;
};

interface ISkinTextPainter
{
	virtual void	drawText(HDC hdc, LPCWSTR text, int c, LPRECT rcText, UINT dtParams, UINT flags = 0, HFONT hFont = NULL, COLORREF color = 0, INT vertical = FALSE, COLORREF glowColor = 0, INT glowSize = 0) = 0;
};

namespace TxSkin
{
	const	DWORD	fdtDrawGlow		= 0x01;	// always draw glow
	const	DWORD	fdtNoGlow		= 0x02;	// don't draw glow
	const	DWORD	fdtGlowColor	= 0x04;	// glowColor is valid
	const	DWORD	fdtColor		= 0x08;	// color is valid
	const	DWORD	fdtGlowSize		= 0x10;	// glow size is valid

	enum elementState
	{
		elStateNormal,
		elStateHover,
		elStatePushed,
		elStateDisabled
	};

	inline void freeClearStr(LPWSTR& str)
	{
		if(str) delete str; 
		str = NULL;
	}
	inline void makeStr(LPWSTR& str, LPCWSTR cpstr)
	{
		freeClearStr(str); 
		if(cpstr) 
		{ 
			int len = lstrlen(cpstr) + 1;
			str = new WCHAR[len]; 
			StringCchCopy(str, len, cpstr); 
		}
	}

	enum imageRowChartType
	{
		imageChartTile,
		imageChartStretch,
		imageChartCrop
	};
};
