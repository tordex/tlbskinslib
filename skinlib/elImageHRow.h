#pragma once
#include "elImageChartBase.h"

namespace TxSkin
{
	enum imgHRAlign
	{
		imgHRAlignLeft,
		imgHRAlignRight
	};

	class elImageHRow : public elImageChartBase
	{
		imgHRAlign			m_align;
		imageRowChartType	m_drawType;
	public:
		elImageHRow(	skin* skin, 
						LPCWSTR id					= NULL,
						LPCWSTR modParam			= NULL, 
						imgHRAlign align			= imgHRAlignLeft, 
						TxSkin::imageRowChartType drawType = TxSkin::imageChartStretch);
		virtual ~elImageHRow(void);

		virtual void	draw(cairo_t* cr, int left, int top, int sliceID);
		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
	};
}
