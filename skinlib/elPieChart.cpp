#include "TxSkinEngine.h"
#include "elPieChart.h"
#include "skinBase.h"
#define _USE_MATH_DEFINES 
#include <math.h>

using namespace TxSkin;

elPieChart::elPieChart( skin* skin, LPCWSTR modParam, LPCWSTR id, int startAngle ) : elColorChartBase(skin, modParam, id)
{
	m_startAngle = startAngle;
}

elPieChart::~elPieChart()
{
}

BOOL TxSkin::elPieChart::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	BOOL ret = elColorChartBase::loadXML(node, baseurl);
	m_startAngle = xmlGetAttributeIntAlt(node, L"start-angle;startAngle", 0);
	return ret;
}


void TxSkin::elPieChart::draw( cairo_t* cr, int left, int top, int sliceID )
{
	cairo_save(cr);

	struct data
	{
		TxSkin::brush*	brush;
		TxSkin::pen*	pen;
		int				val;
	};
	std::vector<data> values;

	int max_val = 0;
	for(int i=0; i < m_values.GetCount(); i++)
	{
		elChartData* dt = (elChartData*) getModState(sliceID, (ULONG_PTR) i);
		if(dt)
		{
			data d;
			d.brush	= dt->m_brush;
			d.pen	= dt->m_pen;
			d.val	= m_skin->getParamInt(m_values[i], sliceID);
			values.push_back(d);
			max_val += d.val;
		}
	}

	double center_x = left + X() + width() / 2.0;
	double center_y = top + Y() + height() / 2.0;
	double radius = min(width(), height()) / 2.0;

	double xStartAngle = m_startAngle; 

	for(int i=0; i < (int) values.size(); i++)
	{
		if(!values[i].val) continue;
		double angle = 360;
		if(max_val)
		{
			angle = (double) (360 * (double) values[i].val / (double) max_val);
		}

		cairo_arc(cr, center_x, center_y, radius - (values[i].pen ? values[i].pen->width_f() / 2 : 0), xStartAngle * (M_PI / 180.0), (xStartAngle + angle) * (M_PI / 180.0));
		cairo_line_to(cr, center_x, center_y);
		cairo_close_path(cr);

		if(values[i].brush)
		{
			values[i].brush->apply(cr, left + X(), top + Y(), width(), height());
			cairo_fill_preserve(cr);
		}

		if(values[i].pen)
		{
			values[i].pen->apply(cr);
			cairo_stroke(cr);
		}

		cairo_new_path(cr);

		xStartAngle += angle;
	}

	cairo_restore(cr);
}

