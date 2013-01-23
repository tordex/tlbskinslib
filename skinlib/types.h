#pragma once

#include <Uxtheme.h>

namespace TxSkin
{

	typedef void*	HTX_SKIN;

	class margins
	{
	public:
		int	left;
		int right;
		int top;
		int bottom;

		margins(int l, int r, int t, int b)
		{
			left	= l;
			right	= r;
			top		= t;
			bottom	= b;
		}

		margins()
		{
			left	= 0;
			right	= 0;
			top		= 0;
			bottom	= 0;
		}

		operator MARGINS()
		{
			MARGINS dst;
			dst.cxLeftWidth		= left;
			dst.cxRightWidth	= right;
			dst.cyTopHeight		= top;
			dst.cyBottomHeight	= bottom;
			return dst;
		}
	};

	enum histogramStart
	{
		startFromTop,
		startFromBottom,
		startFromLeft,
		startFromRight
	};

	enum histogramDir
	{
		dirPositive		= 1,
		dirNegative		= -1
	};

	enum size_shink
	{
		shrink_none,
		shrink_width,
		shrink_height,
		shrink_both
	};

	enum text_halign
	{
		text_halign_left,
		text_halign_right,
		text_halign_center
	};

	enum font_size_type
	{
		font_size_type_cell,
		font_size_type_char
	};

	enum skin_fit
	{
		fit_to_none,
		fit_to_width,
		fit_to_height,
		fit_to_min,
		fit_to_max
	};
};

