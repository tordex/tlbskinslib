#pragma once
#include "elementBase.h"
#include "elText.h"

namespace TxSkin
{
	class elLabel : public elText
	{
		LPWSTR				m_text;
	public:
		elLabel(	skin* skin, 
					LPCWSTR text				= NULL, 
					LPCWSTR fontName			= L"Arial", 
					BOOL bold					= FALSE, 
					BOOL italic					= FALSE, 
					UINT flags					= 0, 
					TxSkin::color color			= TxSkin::color(0, 0, 0, 0), 
					BOOL antialias				= TRUE, 
					int glowSize				= 0, 
					TxSkin::color glowColor		= TxSkin::color(0, 0, 0, 0), 
					LPCWSTR id					= NULL, 
					LPCWSTR modParam			= NULL);
		virtual ~elLabel();
		virtual BOOL getString(std::wstring& outStr, int sliceID);
	};
}
