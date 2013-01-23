#pragma once
#include "elSliderBase.h"

namespace TxSkin
{
	class elVSlider : public elSliderBase
	{
	public:
		elVSlider(		skin* skin,
			LPCWSTR param		= NULL,
			LPCWSTR maxParam	= NULL,
			LPCWSTR id			= NULL,
			LPCWSTR modParam	= NULL);
		virtual ~elVSlider();

		virtual void		draw(cairo_t* cr, int left, int top, int sliceID);
		virtual BOOL		mouseOver(int x, int y, int sliceID);
		virtual BOOL		lButtonDown(int x, int y, int sliceID);
	private:
		int					getThumbY(int sliceID, int thumbHeight);
		int					getNewVal(int y, int sliceID);
	};
}
