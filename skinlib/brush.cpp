#include "TxSkinEngine.h"
#include "brush.h"


TxSkin::brush::~brush()
{

}

void TxSkin::brush::add_color_stop( double offset, color clr )
{
	color_stop cs;
	cs.m_offset	= offset;
	cs.m_color	= clr;
	m_colors.push_back(cs);
}


void TxSkin::solid_brush::apply( cairo_t* cr, double x, double y, double width, double height )
{
	cairo_set_source_rgba(cr, m_color.red_f(), m_color.green_f(), m_color.blue_f(), m_color.alpha_f());
}

void TxSkin::h_gradient_brush::apply( cairo_t* cr, double x, double y, double width, double height )
{
	if(m_pattern)
	{
		cairo_pattern_destroy(m_pattern);
		m_pattern = NULL;
	}

	m_pattern = cairo_pattern_create_linear(x, y, x + width, y);

	cairo_pattern_add_color_stop_rgba( m_pattern, 0, m_color1.red_f(), m_color1.green_f(), m_color1.blue_f(), m_color1.alpha_f() );

	for(color_stop::vector::iterator cs = m_colors.begin(); cs != m_colors.end(); cs++)
	{
		cairo_pattern_add_color_stop_rgba( m_pattern, cs->m_offset, cs->m_color.red_f(), cs->m_color.green_f(), cs->m_color.blue_f(), cs->m_color.alpha_f() );
	}

	cairo_pattern_add_color_stop_rgba( m_pattern, 1, m_color2.red_f(), m_color2.green_f(), m_color2.blue_f(), m_color2.alpha_f() );

	cairo_set_source(cr, m_pattern);
}

TxSkin::h_gradient_brush::~h_gradient_brush()
{
	if(m_pattern)
	{
		cairo_pattern_destroy(m_pattern);
	}
}

TxSkin::v_gradient_brush::~v_gradient_brush()
{
	if(m_pattern)
	{
		cairo_pattern_destroy(m_pattern);
	}
}

void TxSkin::v_gradient_brush::apply( cairo_t* cr, double x, double y, double width, double height )
{
	if(m_pattern)
	{
		cairo_pattern_destroy(m_pattern);
		m_pattern = NULL;
	}

	m_pattern = cairo_pattern_create_linear(x, y, x, y + height);

	cairo_pattern_add_color_stop_rgba( m_pattern, 0, m_color1.red_f(), m_color1.green_f(), m_color1.blue_f(), m_color1.alpha_f() );

	for(color_stop::vector::iterator cs = m_colors.begin(); cs != m_colors.end(); cs++)
	{
		cairo_pattern_add_color_stop_rgba( m_pattern, cs->m_offset, cs->m_color.red_f(), cs->m_color.green_f(), cs->m_color.blue_f(), cs->m_color.alpha_f() );
	}

	cairo_pattern_add_color_stop_rgba( m_pattern, 1, m_color2.red_f(), m_color2.green_f(), m_color2.blue_f(), m_color2.alpha_f() );

	cairo_set_source(cr, m_pattern);
}

TxSkin::radial_gradient_brush::~radial_gradient_brush()
{
	if(m_pattern)
	{
		cairo_pattern_destroy(m_pattern);
	}
}

void TxSkin::radial_gradient_brush::apply( cairo_t* cr, double x, double y, double width, double height )
{
	if(m_pattern)
	{
		cairo_pattern_destroy(m_pattern);
		m_pattern = NULL;
	}

	double r = max(width, height) / 2.0;

	m_pattern = cairo_pattern_create_radial(x + width / 2.0, y + height / 2.0, 0, x + width / 2.0, y + height / 2.0, r);

	cairo_pattern_add_color_stop_rgba( m_pattern, 0, m_color1.red_f(), m_color1.green_f(), m_color1.blue_f(), m_color1.alpha_f() );

	for(color_stop::vector::iterator cs = m_colors.begin(); cs != m_colors.end(); cs++)
	{
		cairo_pattern_add_color_stop_rgba( m_pattern, cs->m_offset, cs->m_color.red_f(), cs->m_color.green_f(), cs->m_color.blue_f(), cs->m_color.alpha_f() );
	}

	cairo_pattern_add_color_stop_rgba( m_pattern, 1, m_color2.red_f(), m_color2.green_f(), m_color2.blue_f(), m_color2.alpha_f() );

	cairo_set_source(cr, m_pattern);
}
