#pragma once

namespace TxSkin
{
	class pen
	{
	protected:
		int m_width;
	public:
		pen(int width)
		{
			m_width	= width;
		}
		virtual ~pen();

		int		width()		{ return m_width;			}
		double	width_f()	{ return (double) m_width;	}

		virtual void apply(cairo_t* cr) = 0;
	};

	class solid_pen : public pen
	{
		TxSkin::color	m_color;
	public:
		solid_pen(TxSkin::color clr, int width) : pen(width), m_color(clr)
		{
		}

		virtual void apply(cairo_t* cr);
	};
}
