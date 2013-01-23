#pragma once
#include "elementBase.h"
#include "elColorChartBase.h"

namespace TxSkin
{
	class elHRowChart : public elColorChartBase
	{
	public:
		elHRowChart(	skin* skin, 
						LPCWSTR modParam	= NULL, 
						LPCWSTR id			= NULL);
		~elHRowChart();
		virtual void draw(cairo_t* cr, int left, int top, int sliceID);
	};
}
