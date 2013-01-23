#pragma once

namespace TxSkin
{

	struct def_color
	{
		const wchar_t*	name;
		const wchar_t*	rgb;
	};

	extern def_color g_def_colors[];

	class color
	{
	public:
		RGBQUAD	clr;

		color()
		{
			ZeroMemory(&clr, sizeof(clr));
		}

		color(BYTE red, BYTE green, BYTE blue, BYTE alpha = 0xFF)
		{
			clr.rgbRed		= red;
			clr.rgbGreen	= green;
			clr.rgbBlue		= blue;
			clr.rgbReserved	= alpha;
		}

		color(COLORREF val)
		{
			clr.rgbRed		= GetRValue(val);
			clr.rgbGreen	= GetGValue(val);
			clr.rgbBlue		= GetBValue(val);
			clr.rgbReserved	= 255;
		}

		color(const color& val)
		{
			clr = val.clr;
		}

		void operator=(const color& val)
		{
			clr = val.clr;
		}

		COLORREF ToCOLORREF()
		{
			return RGB(red(), green(), blue());
		}

		BYTE red() const			{ return clr.rgbRed;		}
		BYTE green() const			{ return clr.rgbGreen;		}
		BYTE blue() const			{ return clr.rgbBlue;		}
		BYTE alpha() const			{ return clr.rgbReserved;	}

		double red_f() const		{ return clr.rgbRed / 255.0;		}
		double green_f() const		{ return clr.rgbGreen / 255.0;		}
		double blue_f() const		{ return clr.rgbBlue / 255.0;		}
		double alpha_f() const		{ return clr.rgbReserved / 255.0;	}

		static color			from_string(const wchar_t* str);

	private:
		static const wchar_t*	resolve_name(const wchar_t* name);
		static color			parseColor(LPCWSTR clr);
	};

	inline TxSkin::color lightColor(TxSkin::color clr, long level)
	{
		BYTE red   = clr.red();
		BYTE green = clr.green();
		BYTE blue  = clr.blue();
		red		= (BYTE) max(0, min(255, (int)((red   - 128) + 128 + level)));
		green	= (BYTE) max(0, min(255, (int)((green - 128) + 128 + level)));
		blue	= (BYTE) max(0, min(255, (int)((blue  - 128) + 128 + level)));
		return TxSkin::color(red, green, blue, clr.alpha());
	}
}