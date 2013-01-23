#pragma once
#include "elementBase.h"

namespace TxSkin
{
	class elRoot : public skin_element
	{
	public:
		elRoot(skin* skin);
		~elRoot();

		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
	};
}