#pragma once
#include "elementBase.h"

namespace TxSkin
{
	class elVBOX : public skin_element
	{
	public:
		elVBOX(skin* skin);
		~elVBOX();

		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual void	init();
	};

}
