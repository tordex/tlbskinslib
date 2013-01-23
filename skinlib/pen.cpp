#include "TxSkinEngine.h"
#include "pen.h"


TxSkin::pen::~pen()
{

}


void TxSkin::solid_pen::apply( cairo_t* cr )
{
	cairo_set_line_width(cr, m_width);
	cairo_set_source_rgba(cr, m_color.red_f(), m_color.green_f(), m_color.blue_f(), m_color.alpha_f());
}
