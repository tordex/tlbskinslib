#pragma once

namespace TxSkin
{
	struct color_stop
	{
		typedef std::vector<color_stop>		vector;
		double	m_offset;
		color	m_color;
	};

	class brush
	{
	protected:
		color_stop::vector	m_colors;
	public:
		brush()
		{

		}
		virtual ~brush();
		
		void add_color_stop(double offset, color clr);

		virtual void apply(cairo_t* cr, double x, double y, double width, double height) = 0;
	};

	class solid_brush : public brush
	{
		color	m_color;
	public:
		solid_brush(color clr)
		{
			m_color	= clr;
		}
		virtual void apply(cairo_t* cr, double x, double y, double width, double height);
	};

	class h_gradient_brush : public brush
	{
		color				m_color1;
		color				m_color2;
		cairo_pattern_t*	m_pattern;
	public:
		h_gradient_brush(color clr1, color clr2)
		{
			m_color1	= clr1;
			m_color2	= clr2;
			m_pattern	= NULL;
		}
		virtual ~h_gradient_brush();

		virtual void apply(cairo_t* cr, double x, double y, double width, double height);
	};

	class v_gradient_brush : public brush
	{
		color				m_color1;
		color				m_color2;
		cairo_pattern_t*	m_pattern;
	public:
		v_gradient_brush(color clr1, color clr2)
		{
			m_color1	= clr1;
			m_color2	= clr2;
			m_pattern	= NULL;
		}
		virtual ~v_gradient_brush();

		virtual void apply(cairo_t* cr, double x, double y, double width, double height);
	};

	class radial_gradient_brush : public brush
	{
		color				m_color1;
		color				m_color2;
		cairo_pattern_t*	m_pattern;
	public:
		radial_gradient_brush(color clr1, color clr2)
		{
			m_color1	= clr1;
			m_color2	= clr2;
			m_pattern	= NULL;
		}
		virtual ~radial_gradient_brush();

		virtual void apply(cairo_t* cr, double x, double y, double width, double height);
	};
}
