#include "TxSkinEngine.h"
#include "elImageHistogram.h"
#include "skinBase.h"

using namespace TxSkin;

elTileImageHistogram::elTileImageHistogram( skin* skin, 
											LPCWSTR param, 
											LPCWSTR maxValParam, 
											TxSkin::histogramStart startFrom, 
											TxSkin::histogramDir dir,
											CTxDIB* img, LPCWSTR id ) : skin_element(skin, id)
{
	m_param			= NULL;
	m_maxValParam	= NULL;
	m_img			= img;
	m_startFrom		= startFrom;
	m_dir			= dir;
	makeStr(m_maxValParam,	maxValParam);
	makeStr(m_param,		param);
}

elTileImageHistogram::~elTileImageHistogram()
{
	freeClearStr(m_maxValParam);
	freeClearStr(m_param);
	if(m_img)
	{
		delete m_img;
	}
}

void elTileImageHistogram::draw( cairo_t* cr, int left, int top, int sliceID )
{
	HDC hdc = cairo_win32_surface_get_dc(cairo_get_target(cr));

	if(!m_img || m_img && !m_img->isValid())
	{
		return;
	}

	int count = 0;
	int* vals = m_skin->getParamValues(m_param, &count, sliceID);
	BOOL autoFit = m_skin->getParamAutoFit(m_param, sliceID);

	MARGINS mg = {0};

	int max_val		= m_skin->getParamInt(m_maxValParam, sliceID);

	if(max_val || autoFit)
	{
		int val;

		if(autoFit)
		{
			switch(m_startFrom)
			{
			case startFromBottom:
			case startFromTop:
				{
					max_val = 0;
					int idx = 0;
					for(int x = left + X(); x <= left + X() + width() - m_img->getWidth() && idx < count; x += m_img->getWidth(), idx++)
					{
						max_val = max(max_val, vals[idx]);
					}
				}
				break;
			case startFromLeft:
			case startFromRight:
				{
					max_val = 0;
					int idx = 0;
					for(int y = top + Y(); y <= top + Y() + height() - m_img->getHeight() && idx < count; y += m_img->getHeight(), idx++)
					{
						max_val = max(max_val, vals[idx]);
					}
				}
				break;
			}
			if(!max_val) return;
		}

		switch(m_startFrom)
		{
		case startFromBottom:
			{
				int idx = 0;
				if(m_dir == dirPositive)
				{
					for(int x = left + X(); x <= left + X() + width() - m_img->getWidth() && idx < count; x += m_img->getWidth(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						double yy = top + Y() + height() - (double) val * (double) height() / (double) max_val;
						for(int y = top + Y() + height() - m_img->getHeight(); y >= (int) yy; y -= m_img->getHeight())
						{
							m_img->draw(hdc, x, y);
						}
					}
				} else
				{
					for(int x = left + X() + width() - m_img->getWidth(); x >= left + X() && idx < count; x -= m_img->getWidth(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						double yy = top + Y() + height() - (double) val * (double) height() / (double) max_val;
						for(int y = top + Y() + height() - m_img->getHeight(); y >= (int) yy; y -= m_img->getHeight())
						{
							m_img->draw(hdc, x, y);
						}
					}
				}
			}
			break;
		case startFromTop:
			{
				int idx = 0;
				if(m_dir == dirPositive)
				{
					for(int x = left + X(); x <= left + X() + width() - m_img->getWidth() && idx < count; x += m_img->getWidth(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						double yy = top + Y() + (double) val * (double) height() / (double) max_val;
						for(int y = top + Y(); y <= (int) yy - m_img->getHeight(); y += m_img->getHeight())
						{
							m_img->draw(hdc, x, y);
						}
					}
				} else
				{
					for(int x = left + X() + width() - m_img->getWidth(); x >= left + X() && idx < count; x -= m_img->getWidth(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						double yy = top + Y() + (double) val * (double) height() / (double) max_val;
						for(int y = top + Y(); y <= (int) yy - m_img->getHeight(); y += m_img->getHeight())
						{
							m_img->draw(hdc, x, y);
						}
					}
				}
			}
			break;
		case startFromLeft:
			{
				int idx = 0;
				if(m_dir == dirPositive)
				{
					for(int y = top + Y(); y <= top + Y() + height() - m_img->getHeight() && idx < count; y += m_img->getHeight(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						double xx = left + X() + (double) val * (double) width() / (double) max_val;
						for(int x = left + X(); x <= (int) xx - m_img->getWidth(); x += m_img->getWidth())
						{
							m_img->draw(hdc, x, y);
						}
					}
				} else
				{
					for(int y = top + Y() + height() - m_img->getHeight(); y >= top + Y() && idx < count; y -= m_img->getHeight(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						double xx = left + X() + (double) val * (double) width() / (double) max_val;
						for(int x = left + X(); x <= (int) xx - m_img->getWidth(); x += m_img->getWidth())
						{
							m_img->draw(hdc, x, y);
						}
					}
				}
			}
			break;
		case startFromRight:
			{
				int idx = 0;
				if(m_dir == dirPositive)
				{
					for(int y = top + Y(); y <= top + Y() + height() - m_img->getHeight() && idx < count; y += m_img->getHeight(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						double xx = left + X() + width() - (double) val * (double) width() / (double) max_val;
						for(int x = left + X() + width() - m_img->getWidth(); x >= (int) xx; x -= m_img->getWidth())
						{
							m_img->draw(hdc, x, y);
						}
					}
				} else
				{
					for(int y = top + Y() + height() - m_img->getHeight(); y >= top + Y() && idx < count; y -= m_img->getHeight(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						double xx = left + X() + width() - (double) val * (double) width() / (double) max_val;
						for(int x = left + X() + width() - m_img->getWidth(); x >= (int) xx; x -= m_img->getWidth())
						{
							m_img->draw(hdc, x, y);
						}
					}
				}
			}
			break;
		}

		GdiFlush();
	}
}

BOOL TxSkin::elTileImageHistogram::isUsedParam( LPCWSTR paramID )
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

BOOL TxSkin::elTileImageHistogram::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	skin_element::loadXML(node, baseurl);

	freeClearStr(m_param);
	freeClearStr(m_maxValParam);

	m_param			= xmlGetAttributeSTR(node, L"src");
	m_maxValParam	= xmlGetAttributeSTR(node, L"max");

	m_startFrom		= (TxSkin::histogramStart) xmlGetAttributeValueSTRArrayAlt(node, L"start-from;startFrom", TxSkin::startFromBottom, L"top\0bottom\0left\0right\0");
	m_dir			= (TxSkin::histogramDir) (xmlGetAttributeValueSTRArray(node, L"dir", 1, L"reverse\0straight\0") == 0 ? TxSkin::dirNegative : TxSkin::dirPositive);

	LPWSTR file = xmlGetAttributeSTR(node, L"file");
	if(file)
	{
		WCHAR fileName[MAX_PATH];
		StringCchCopy(fileName, MAX_PATH, baseurl);
		StringCchCat(fileName, MAX_PATH, file);
		m_img = new CTxDIB(fileName);
		delete file;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

TxSkin::elStretchImageHistogram::elStretchImageHistogram(	skin* skin, 
															LPCWSTR param, 
															LPCWSTR maxValParam, 
															TxSkin::histogramStart startFrom, 
															TxSkin::histogramDir dir, 
															CTxDIB* img, 
															const TxSkin::margins* imageMargins, LPCWSTR id ) : skin_element(skin, id)
{
	m_param			= NULL;
	m_maxValParam	= NULL;
	m_img			= img;
	m_startFrom		= startFrom;
	m_dir			= dir;
	makeStr(m_maxValParam,	maxValParam);
	makeStr(m_param,		param);
	ZeroMemory(&m_margins, sizeof(m_margins));
}

TxSkin::elStretchImageHistogram::~elStretchImageHistogram()
{
	freeClearStr(m_maxValParam);
	freeClearStr(m_param);
	if(m_img)
	{
		delete m_img;
	}
}

void TxSkin::elStretchImageHistogram::draw( cairo_t* cr, int left, int top, int sliceID )
{
	HDC hdc = cairo_win32_surface_get_dc(cairo_get_target(cr));

	if(!m_img || m_img && !m_img->isValid())
	{
		return;
	}

	int count = 0;
	int* vals = m_skin->getParamValues(m_param, &count, sliceID);
	BOOL autoFit = m_skin->getParamAutoFit(m_param, sliceID);

	MARGINS mg = {0};

	int max_val		= m_skin->getParamInt(m_maxValParam, sliceID);

	if(max_val || autoFit)
	{
		int val;

		if(autoFit)
		{
			switch(m_startFrom)
			{
			case startFromBottom:
			case startFromTop:
				{
					max_val = 0;
					int idx = 0;
					for(int x = left + X(); x <= left + X() + width() - m_img->getWidth() && idx < count; x += m_img->getWidth(), idx++)
					{
						max_val = max(max_val, vals[idx]);
					}
				}
				break;
			case startFromLeft:
			case startFromRight:
				{
					max_val = 0;
					int idx = 0;
					for(int y = top + Y(); y <= top + Y() + height() - m_img->getHeight() && idx < count; y += m_img->getHeight(), idx++)
					{
						max_val = max(max_val, vals[idx]);
					}
				}
				break;
			}
			if(!max_val) return;
		}


		CTxSkinDIB skin;
		skin.load(m_img, &m_margins, FALSE, FALSE);

		switch(m_startFrom)
		{
		case startFromBottom:
			{
				int idx = 0;
				if(m_dir == dirPositive)
				{
					for(int x = left + X(); x <= left + X() + width() - m_img->getWidth() && idx < count; x += m_img->getWidth(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						int h = (int) ((double) val * (double) height() / (double) max_val);
						skin.draw(hdc, &make_rect(x, top + Y() + height() - h, m_img->getWidth(), h), NULL);
					}
				} else
				{
					for(int x = left + X() + width() - m_img->getWidth(); x >= left + X() && idx < count; x -= m_img->getWidth(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						int h = (int) ((double) val * (double) height() / (double) max_val);
						skin.draw(hdc, &make_rect(x, top + Y() + height() - h, m_img->getWidth(), h), NULL);
					}
				}
			}
			break;
		case startFromTop:
			{
				int idx = 0;
				if(m_dir == dirPositive)
				{
					for(int x = left + X(); x <= left + X() + width() - m_img->getWidth() && idx < count; x += m_img->getWidth(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						int h = (int) ((double) val * (double) height() / (double) max_val);
						skin.draw(hdc, &make_rect(x, top + Y(), m_img->getWidth(), h), NULL);
					}
				} else
				{
					for(int x = left + X() + width() - m_img->getWidth(); x >= left + X() && idx < count; x -= m_img->getWidth(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						int h = (int) ((double) val * (double) height() / (double) max_val);
						skin.draw(hdc, &make_rect(x, top + Y(), m_img->getWidth(), h), NULL);
					}
				}
			}
			break;
		case startFromLeft:
			{
				int idx = 0;
				if(m_dir == dirPositive)
				{
					for(int y = top + Y(); y <= top + Y() + height() - m_img->getHeight() && idx < count; y += m_img->getHeight(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						int w = (int) ((double) val * (double) width() / (double) max_val);
						skin.draw(hdc, &make_rect(left + X(), y, w, m_img->getHeight()), NULL);
					}
				} else
				{
					for(int y = top + Y() + height() - m_img->getHeight(); y >= top + Y() && idx < count; y -= m_img->getHeight(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						int w = (int) ((double) val * (double) width() / (double) max_val);
						skin.draw(hdc, &make_rect(left + X(), y, w, m_img->getHeight()), NULL);
					}
				}
			}
			break;
		case startFromRight:
			{
				int idx = 0;
				if(m_dir == dirPositive)
				{
					for(int y = top + Y(); y <= top + Y() + height() - m_img->getHeight() && idx < count; y += m_img->getHeight(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						int w = (int) ((double) val * (double) width() / (double) max_val);
						skin.draw(hdc, &make_rect(left + X() + width() - w, y, w, m_img->getHeight()), NULL);
					}
				} else
				{
					for(int y = top + Y() + height() - m_img->getHeight(); y >= top + Y() && idx < count; y -= m_img->getHeight(), idx++)
					{
						val  = (vals[idx] > max_val) ? max_val : vals[idx];
						int w = (int) ((double) val * (double) width() / (double) max_val);
						skin.draw(hdc, &make_rect(left + X() + width() - w, y, w, m_img->getHeight()), NULL);
					}
				}
			}
			break;
		}

		GdiFlush();
	}
}

BOOL TxSkin::elStretchImageHistogram::isUsedParam( LPCWSTR paramID )
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

BOOL TxSkin::elStretchImageHistogram::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	skin_element::loadXML(node, baseurl);

	freeClearStr(m_param);
	freeClearStr(m_maxValParam);

	m_param			= xmlGetAttributeSTR(node, L"src");
	m_maxValParam	= xmlGetAttributeSTR(node, L"max");

	m_startFrom		= (TxSkin::histogramStart) xmlGetAttributeValueSTRArrayAlt(node, L"start-from;startFrom", TxSkin::startFromBottom, L"top\0bottom\0left\0right\0");
	m_dir			= (TxSkin::histogramDir) (xmlGetAttributeValueSTRArray(node, L"dir", 1, L"reverse\0straight\0") == 0 ? TxSkin::dirNegative : TxSkin::dirPositive);

	LPWSTR file = xmlGetAttributeSTR(node, L"file");
	if(file)
	{
		WCHAR fileName[MAX_PATH];
		StringCchCopy(fileName, MAX_PATH, baseurl);
		StringCchCat(fileName, MAX_PATH, file);
		m_img = new CTxDIB(fileName);

		m_margins.cxLeftWidth		= xmlGetAttributeIntAlt(node, L"sizing-margin-left;sizing_margin_left",		0);
		m_margins.cxRightWidth		= xmlGetAttributeIntAlt(node, L"sizing-margin-right;sizing_margin_right",	0);
		m_margins.cyTopHeight		= xmlGetAttributeIntAlt(node, L"sizing-margin-top;sizing_margin_top",		0);
		m_margins.cyBottomHeight	= xmlGetAttributeIntAlt(node, L"sizing-margin-bottom;sizing_margin_bottom",	0);

		delete file;
	}

	return TRUE;
}
