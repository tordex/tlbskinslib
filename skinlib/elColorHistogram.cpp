#include "TxSkinEngine.h"
#include "elColorHistogram.h"
#include "skinBase.h"

using namespace TxSkin;

elColorHistogram::elColorHistogram(	skin* skin, 
									LPCWSTR param, 
									LPCWSTR maxValParam, 
									TxSkin::histogramStart startFrom, 
									TxSkin::histogramDir dir, 
									TxSkin::brush* brush, 
									TxSkin::pen* pen, 
									int step, LPCWSTR id ) : skin_element(skin, id)
{
	m_param			= NULL;
	m_maxValParam	= NULL;
	m_param			= NULL;
	m_brush			= brush;
	m_pen			= pen;
	m_startFrom		= startFrom;
	m_dir			= dir;
	m_step			= step;

	makeStr(m_maxValParam,	maxValParam);
	makeStr(m_param,		param);
}

elColorHistogram::~elColorHistogram()
{
	if(m_param)			delete m_param;
	if(m_maxValParam)	delete m_maxValParam;
	if(m_brush)			delete m_brush;
	if(m_pen)			delete m_pen;
}

void elColorHistogram::draw( cairo_t* cr, int left, int top, int sliceID )
{
	int count = 0;
	int* vals = m_skin->getParamValues(m_param, &count, sliceID);
	if(vals)
	{
		int max_val = m_skin->getParamInt(m_maxValParam, sliceID);
		BOOL autoFit = m_skin->getParamAutoFit(m_param, sliceID);

		switch(m_startFrom)
		{
		case startFromTop:
		case startFromBottom:
			draw_h_histogram(cr, left, top, vals, count, max_val, autoFit);
			break;
		case startFromLeft:
		case startFromRight:
			draw_v_histogram(cr, left, top, vals, count, max_val, autoFit);
			break;
		}

	}
}

BOOL TxSkin::elColorHistogram::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	skin_element::loadXML(node, baseurl);

	freeClearStr(m_param);
	freeClearStr(m_maxValParam);

	m_param			= xmlGetAttributeSTR(node, L"src");
	m_maxValParam	= xmlGetAttributeSTR(node, L"max");

	m_startFrom		= (TxSkin::histogramStart) xmlGetAttributeValueSTRArrayAlt(node, L"start-from;startFrom", TxSkin::startFromBottom, L"top\0bottom\0left\0right\0");
	m_dir			= (TxSkin::histogramDir) (xmlGetAttributeValueSTRArray(node, L"dir", 1, L"reverse\0straight\0") == 0 ? TxSkin::dirNegative : TxSkin::dirPositive);

	m_step = xmlGetAttributeValueNonSTR<int>(node,	L"step",	1);
	m_brush	= loadBrush(node);
	m_pen	= loadPen(node);

	return TRUE;
}

BOOL TxSkin::elColorHistogram::isUsedParam( LPCWSTR paramID )
{
	if(skin_element::isUsedParam(paramID))
	{
		return TRUE;
	}
	if(m_param && !lstrcmpi(m_param, paramID))
	{
		return TRUE;
	}
	if(m_maxValParam && !lstrcmpi(m_maxValParam, paramID))
	{
		return TRUE;
	}
	return FALSE;
}

void TxSkin::elColorHistogram::draw_h_histogram(cairo_t* cr, int left, int top, int* data, int count, int max_val, BOOL autoFit)
{
	if(max_val || autoFit)
	{
		int x		= left + X();
		int y		= top + Y();
		int width	= skin_element::width();
		int height	= skin_element::height();
		int step	= m_step * m_dir;
		if(!step)
		{
			step = 1;
		}

		if(autoFit)
		{
			switch(m_startFrom)
			{
			case startFromBottom:
			case startFromTop:
				{
					max_val = 0;
					int idx = 0;
					for(int x = left + X(); x <= left + X() + skin_element::width() - abs(step) && idx < count; x += abs(step), idx++)
					{
						max_val = max(max_val, data[idx]);
					}
				}
				break;
			case startFromLeft:
			case startFromRight:
				{
					max_val = 0;
					int idx = 0;
					for(int y = top + Y(); y <= top + Y() + skin_element::height() - abs(step) && idx < count; y += abs(step), idx++)
					{
						max_val = max(max_val, data[idx]);
					}
				}
				break;
			}
			if(!max_val) return;
		}


		cairo_save(cr);

		cairo_rectangle(cr, x, y, width, height);
		cairo_clip(cr);
		bool is_first = true;

		int startX = step > 0 ? 0 : width;
		int startY = y;

		int val = 0;

		for(int idx = 0, xx = startX; ; xx += step, idx++)
		{
			if(idx >= count)
			{
				break;
			}

			val = min(data[idx], max_val);

			double yy = 0;
			if(m_startFrom == startFromBottom)
			{
				yy = y + height - (double) val * (double) height / (double) max_val;
			} else
			{
				yy = y + (double) val * (double) height / (double) max_val;
			}

			if(is_first)
			{
				is_first = false;
				cairo_move_to(cr, x + xx, yy);
			} else
			{
				cairo_line_to(cr, x + xx, yy);
			}

			if(step > 0 && xx > width || step < 0 && xx < 0)
			{
				break;
			}
		}

		cairo_path_t* hPath = cairo_copy_path(cr);

		if(m_brush)
		{
			double lastX;
			double lastY;
			cairo_get_current_point(cr, &lastX, &lastY);

			if(m_startFrom == startFromBottom)
			{
				cairo_line_to(cr, lastX, y + height);
				cairo_line_to(cr, x + startX, y + height);
			} else
			{
				cairo_line_to(cr, lastX, y);
				cairo_line_to(cr, x + startX, y);
			}
			cairo_close_path(cr);

			m_brush->apply(cr, x, y, width, height);
			cairo_fill(cr);
		}

		if(m_pen)
		{
			cairo_new_path(cr);
			cairo_append_path(cr, hPath);
			m_pen->apply(cr);
			cairo_stroke(cr);
		}

		cairo_restore(cr);

		cairo_path_destroy(hPath);
	}
};

void TxSkin::elColorHistogram::draw_v_histogram( cairo_t* cr, int left, int top, int* data, int count, int max_val, BOOL autoFit )
{
	if(max_val || autoFit)
	{
		int x		= left + X();
		int y		= top + Y();
		int width	= skin_element::width();
		int height	= skin_element::height();
		int step	= m_step * m_dir;
		if(!step)
		{
			step = 1;
		}

		if(autoFit)
		{
			switch(m_startFrom)
			{
			case startFromBottom:
			case startFromTop:
				{
					max_val = 0;
					int idx = 0;
					for(int x = left + X(); x <= left + X() + skin_element::width() - abs(step) && idx < count; x += abs(step), idx++)
					{
						max_val = max(max_val, data[idx]);
					}
				}
				break;
			case startFromLeft:
			case startFromRight:
				{
					max_val = 0;
					int idx = 0;
					for(int y = top + Y(); y <= top + Y() + skin_element::height() - abs(step) && idx < count; y += abs(step), idx++)
					{
						max_val = max(max_val, data[idx]);
					}
				}
				break;
			}
			if(!max_val) return;
		}

		cairo_save(cr);

		cairo_rectangle(cr, x, y, width, height);
		cairo_clip(cr);
		bool is_first = true;

		int startY = step > 0 ? 0 : height;
		int startX = x;
		int val = 0;

		for(int idx = 0, yy = startY; ; yy += step, idx++)
		{
			if(idx >= count)
			{
				break;
			}

			val = min(data[idx], max_val);

			double xx = 0;
			if(m_startFrom == startFromRight)
			{
				xx = x + width - (double) val * (double) width / (double) max_val;
			} else
			{
				xx = x + (double) val * (double) width / (double) max_val;
			}

			if(is_first)
			{
				is_first = false;
				cairo_move_to(cr, xx, y + yy);
			} else
			{
				cairo_line_to(cr, xx, y + yy);
			}

			if(step > 0 && yy > height || step < 0 && yy < 0)
			{
				break;
			}
		}

		cairo_path_t* hPath = cairo_copy_path(cr);

		if(m_brush)
		{
			double lastX;
			double lastY;
			cairo_get_current_point(cr, &lastX, &lastY);

			if(m_startFrom == startFromRight)
			{
				cairo_line_to(cr, x + width, lastY);
				cairo_line_to(cr, x + width, y + startY);
			} else
			{
				cairo_line_to(cr, x, lastY);
				cairo_line_to(cr, x, y + startY);
			}
			cairo_close_path(cr);

			m_brush->apply(cr, x, y, width, height);
			cairo_fill(cr);
		}

		if(m_pen)
		{
			cairo_new_path(cr);
			cairo_append_path(cr, hPath);
			m_pen->apply(cr);
			cairo_stroke(cr);
		}

		cairo_restore(cr);

		cairo_path_destroy(hPath);
	}
}
