#include "TxSkinEngine.h"
#include "elVRowChart.h"
#include "skinBase.h"

using namespace TxSkin;

elVRowChart::elVRowChart( skin* skin, LPCWSTR modParam, LPCWSTR id ) : elColorChartBase(skin, modParam, id)
{
}

elVRowChart::~elVRowChart()
{
}

void TxSkin::elVRowChart::draw( cairo_t* cr, int left, int top, int sliceID )
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

	double lastValue = 0;
	double last_line_width = 0;

	for(int i=0; i < (int) values.size(); i++)
	{
		double val = 0;
		if(max_val)
		{
			val = (double) values[i].val * (double) height() / (double) max_val;
		}

		int x = left	+ X();
		int y = (int) (top + Y() + lastValue);
		int h = (int) val;

		if(i == (int) values.size() - 1 && y + h != top + Y() + height())
		{
			h = top + Y() + height() - y;
		}
		if(y + h > top + Y() + height())	h = top + Y() + height() - y;

		if(h > 0)
		{
			double line_width = (values[i].pen ? values[i].pen->width_f() : 0);
			double rx	= x + line_width / 2.0;
			double ry	= y + line_width / 2.0 - last_line_width;
			double rw	= width() - line_width;
			double rh	= h - line_width + last_line_width;

			cairo_rectangle(cr, rx, ry, rw, rh);

			if(values[i].brush)
			{
				values[i].brush->apply(cr, rx, ry, rw, rh);
				cairo_fill_preserve(cr);
			}
			if(values[i].pen)
			{
				values[i].pen->apply(cr);
				cairo_stroke(cr);
			}
			cairo_new_path(cr);
			last_line_width = line_width;
		}

		lastValue += val;
	}

	cairo_restore(cr);
}
