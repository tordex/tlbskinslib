#pragma once
#include "elementBase.h"
#include "elColorChartBase.h"

namespace TxSkin
{
	class elVRowChart : public elColorChartBase
	{
	public:
		elVRowChart(	skin* skin, 
						LPCWSTR modParam	= NULL, 
						LPCWSTR id			= NULL);
		~elVRowChart();
		virtual void draw(cairo_t* cr, int left, int top, int sliceID);
	};
}
