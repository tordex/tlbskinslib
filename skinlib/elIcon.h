#pragma once
#include "elementBase.h"

namespace TxSkin
{
	class elIcon : public skin_element
	{
		LPWSTR	m_srcIcon;
		LPWSTR	m_defIcon;
		LPWSTR	m_baseurl;
	public:
		elIcon(	skin* skin, 
				LPCWSTR src = NULL, 
				LPCWSTR id	= NULL);
		virtual ~elIcon(void);

		virtual void	draw(cairo_t* cr, int left, int top, int sliceID);
		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
	};

}
