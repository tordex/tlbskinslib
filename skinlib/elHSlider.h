#pragma once
#include "elSliderBase.h"

namespace TxSkin
{
	class elHSlider : public elSliderBase
	{
	public:
		elHSlider(		skin* skin,
						LPCWSTR param		= NULL,
						LPCWSTR maxParam	= NULL,
						LPCWSTR id			= NULL,
						LPCWSTR modParam	= NULL);
		virtual ~elHSlider();

		virtual void		draw(cairo_t* cr, int left, int top, int sliceID);
		virtual BOOL		mouseOver(int x, int y, int sliceID);
		virtual BOOL		lButtonDown(int x, int y, int sliceID);
	private:
		int					getThumbX(int sliceID, int thumbWidth);
		int					getNewVal(int x, int sliceID);
	};
}
