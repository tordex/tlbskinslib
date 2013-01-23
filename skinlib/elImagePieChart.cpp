#include "TxSkinEngine.h"
#include "elImagePieChart.h"
#include "skinBase.h"
#define _USE_MATH_DEFINES 
#include <math.h>


TxSkin::elImagePieChart::elImagePieChart( skin* skin, LPCWSTR id /*= NULL*/, LPCWSTR modParam /*= NULL*/, int startAngle /*= 0*/ ) : elImageChartBase(skin, id, modParam)
{
	m_startAngle = startAngle;
}
TxSkin::elImagePieChart::~elImagePieChart( void )
{
}

void TxSkin::elImagePieChart::draw( cairo_t* cr, int left, int top, int sliceID )
{
	cairo_save(cr);

	struct data
	{
		CTxDIB*	img;
		int		val;
	};
	std::vector<data> values;

	int max_val = 0;
	for(int i=0; i < m_values.GetCount(); i++)
	{
		elImageChartData* dt = (elImageChartData*) getModState(sliceID, (ULONG_PTR) i);
		if(dt)
		{
			data d;
			d.img	= dt->m_bmp;
			d.val	= m_skin->getParamInt(m_values[i], sliceID);
			values.push_back(d);
			max_val += d.val;
		}
	}

	double center_x = left + X() + width() / 2.0;
	double center_y = top + Y() + height() / 2.0;

	double xStartAngle = m_startAngle; 

	cairo_matrix_t flib_m;
	cairo_matrix_init(&flib_m, 1, 0, 0, -1, 0, 0);

	for(int i=0; i < (int) values.size(); i++)
	{
		if(!values[i].val) continue;
		double angle = 360;
		if(max_val)
		{
			angle = (double) (360 * (double) values[i].val / (double) max_val);
		}

		cairo_arc(cr, center_x, center_y, width() / 2, xStartAngle * M_PI / 180.0, (xStartAngle + angle) * M_PI / 180.0);
		cairo_line_to(cr, center_x, center_y);
		cairo_close_path(cr);
		cairo_clip(cr);
		
		if(values[i].img)
		{
			cairo_save(cr);

			cairo_surface_t* img = cairo_image_surface_create_for_data((unsigned char*) values[i].img->getBits(), CAIRO_FORMAT_ARGB32, values[i].img->getWidth(), values[i].img->getHeight(), values[i].img->getWidth() * 4);

			cairo_set_matrix(cr, &flib_m);
			cairo_translate (cr, X() + left, -(Y() + top));
			cairo_translate (cr, 0, -values[i].img->getHeight());
			
			cairo_set_source_surface(cr, img, 0, 0);
			cairo_paint(cr);

			cairo_surface_destroy(img);
			cairo_restore(cr);

		}
		cairo_reset_clip(cr);

		xStartAngle += angle;
	}

	cairo_restore(cr);
}

BOOL TxSkin::elImagePieChart::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	m_startAngle = xmlGetAttributeIntAlt(node, L"start-angle;startAngle", -90);
	return elImageChartBase::loadXML(node, baseurl);
}

