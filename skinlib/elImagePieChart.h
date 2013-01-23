#pragma once
#include "elImageChartBase.h"

namespace TxSkin
{
	class elImagePieChart : public elImageChartBase
	{
		int m_startAngle;
	public:
		elImagePieChart(	skin* skin, 
							LPCWSTR id			= NULL, 
							LPCWSTR modParam	= NULL, 
							int startAngle		= 0);
		virtual ~elImagePieChart(void);

		virtual void	draw(cairo_t* cr, int left, int top, int sliceID);
		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
	};
}
