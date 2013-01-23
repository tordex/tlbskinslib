#pragma once
#include "elementBase.h"
#include "elColorChartBase.h"

namespace TxSkin
{
	class elPieChart : public elColorChartBase
	{
		int		m_startAngle;
	public:
		elPieChart(	skin* skin, 
					LPCWSTR modParam	= NULL, 
					LPCWSTR id			= NULL, 
					int startAngle		= 0);
		~elPieChart();

		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual void	draw(cairo_t* cr, int left, int top, int sliceID);
	};
}
