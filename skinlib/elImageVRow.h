#pragma once
#include "elImageChartBase.h"

namespace TxSkin
{
	enum imgVRAlign
	{
		imgVRAlignBottom,
		imgVRAlignTop
	};

	class elImageVRow : public elImageChartBase
	{
		imgVRAlign					m_align;
		imageRowChartType			m_drawType;
	public:
		elImageVRow(	skin* skin, 
						LPCWSTR id					= NULL,
						LPCWSTR modParam			= NULL, 
						imgVRAlign align			= imgVRAlignBottom, 
						imageRowChartType drawType	= imageChartTile);
		virtual ~elImageVRow(void);

		virtual void	draw(cairo_t* cr, int left, int top, int sliceID);
		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
	};
}
