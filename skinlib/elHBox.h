#pragma once
#include "elementBase.h"

namespace TxSkin
{
	class elHBOX : public skin_element
	{
	public:
		elHBOX(skin* skin);
		~elHBOX();

		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual void	init();
	};
}
