#include "TxSkinEngine.h"
#include "elText.h"
#include "skinBase.h"
#include "skinParams.h"

using namespace TxSkin;

TxSkin::elText::elText( skin* skin, LPCWSTR paramID, LPCWSTR fontName, BOOL bold, BOOL italic, UINT flags, TxSkin::color color, DWORD quality, int glowSize /*= 0*/, TxSkin::color glowColor /*= TxSkin::color(0, 0, 0, 0)*/, LPCWSTR id /*= NULL*/, LPCWSTR modParam ) : skin_element(skin, id)
{
	m_size_type		= font_size_type_cell;
	m_show_units	= show_units_none;
	m_paramID		= NULL;
	m_fontName		= NULL;
	m_bold			= bold;
	m_italic		= italic;
	m_flags			= flags;
	m_hFont			= NULL;
	m_glowSize		= glowSize;
	m_quality		= quality;
	m_fontSize		= 0;
	m_modParam		= NULL;

	makeStr(m_paramID,	paramID);
	makeStr(m_fontName, fontName);
	makeStr(m_modParam,	modParam);

	if(m_paramID)
	{
		addState(elStateNormal, color, glowColor);
	}
}

TxSkin::elText::~elText()
{
	if(m_fontName)	delete m_fontName;
	if(m_paramID)	delete m_paramID;
	if(m_hFont)		DeleteObject(m_hFont);
}

void TxSkin::elText::draw( cairo_t* cr, int left, int top, int sliceID )
{
	elTextData* data = (elTextData*) getModState(sliceID);
	if(data)
	{
		std::wstring str;
		if(getString(str, sliceID))
		{
			HDC hdc = cairo_win32_surface_get_dc(cairo_get_target(cr));

			RECT rcText;
			getRect(&rcText);
			OffsetRect(&rcText, left, top);

			DWORD flags = TxSkin::fdtColor;
			if(m_glowSize)
			{
				flags |= TxSkin::fdtDrawGlow | TxSkin::fdtGlowColor | TxSkin::fdtGlowSize;
			} else
			{
				flags |= TxSkin::fdtNoGlow | TxSkin::fdtGlowSize;
			}
			m_skin->drawText(hdc, str.c_str(), -1, &rcText, m_flags, flags, m_hFont, data->m_color.ToCOLORREF(), FALSE, data->m_glowColor.ToCOLORREF(), m_glowSize);
			
			GdiFlush();
		}
	}
}

void TxSkin::elText::init()
{
	if(m_hFont)	DeleteObject(m_hFont);
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	if(!m_fontSize)
	{
		lf.lfHeight	= (height() - m_glowSize * 2);
	} else
	{
		lf.lfHeight	= m_fontSize;
	}
	if(m_size_type == font_size_type_char && lf.lfHeight > 0)
	{
		lf.lfHeight = -lf.lfHeight;
	}

	lf.lfWeight			= m_bold ? FW_BOLD : FW_NORMAL;
	lf.lfItalic			= m_italic;
	lf.lfCharSet		= ANSI_CHARSET;
	lf.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	lf.lfQuality		= (BYTE) m_quality;
	lf.lfPitchAndFamily = FF_DONTCARE;
	StringCchCopy(lf.lfFaceName, LF_FACESIZE, m_fontName);

	m_hFont	= CreateFontIndirect(&lf);
}

void TxSkin::elText::addState( elementState uState, TxSkin::color color, TxSkin::color glowColor, TxSkin::modParamValue& modVal )
{
	addModState(modVal, uState, new elTextData(color, glowColor));
}

int TxSkin::elText::calcWidth()
{
	if(m_autoCalcWidth)
	{
		int wid = width();
		if(m_autoCalcWidth)
		{
			parameter* param = m_skin->findParam(m_paramID);
			if(param)
			{
				HDC hdc = GetDC(NULL);
				wid = 0;

				for(int i=0; i < param->getSlicesCount(); i++)
				{
					std::wstring str;
					if(getString(str, param->getSliceID(i)))
					{
						RECT rcText = {0, 0, 1, 1};

						DWORD flags = TxSkin::fdtColor;
						if(m_glowSize)
						{
							flags |= TxSkin::fdtDrawGlow | TxSkin::fdtGlowColor | TxSkin::fdtGlowSize;
						} else
						{
							flags |= TxSkin::fdtNoGlow | TxSkin::fdtGlowSize;
						}
						m_skin->drawText(hdc, str.c_str(), -1, &rcText, m_flags | DT_CALCRECT, flags, m_hFont, 0, FALSE, 0, m_glowSize);

						if(wid < rcText.right - rcText.left)
						{
							wid = rcText.right - rcText.left;
						}
					}
				}
				ReleaseDC(NULL, hdc);
			}
		}
		return wid + 1;
	}
	return width();
}

BOOL TxSkin::elText::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	freeClearStr(m_fontName);
	freeClearStr(m_paramID);

	BOOL ret = skin_element::loadXML(node, baseurl);

	m_paramID	= xmlGetAttributeSTR(node, L"src");
	if(!m_paramID)
	{
		makeStr(m_paramID, L"");
	}

	m_fontName	= xmlGetAttributeSTR(node, L"font");
	if(!m_fontName)
	{
		makeStr(m_fontName, L"Arial");
	}

	BOOL antialias = TRUE;

	m_bold			= xmlGetAttributeValueBOOL(node, L"bold", FALSE);
	m_italic		= xmlGetAttributeValueBOOL(node, L"italic", FALSE);
	antialias		= xmlGetAttributeValueBOOL(node, L"antialias", TRUE);
	m_quality		= xmlGetAttributeValueSTRArray(node, L"quality", antialias ? CLEARTYPE_QUALITY : NONANTIALIASED_QUALITY, L"default\0draft\0proof\0nonantialiased\0antialiased\0cleartype\0cleartype natural\0");
	m_glowSize		= xmlGetAttributeIntAlt(node, L"glow-size;glowSize", 0);
	m_fontSize		= xmlGetAttributeIntAlt(node, L"font-size;fontSize", 0);
	m_show_units	= (show_units) xmlGetAttributeValueSTRArrayAlt(node, L"units", 0, L"none\0left\0right\0");

	m_size_type		= (font_size_type) xmlGetAttributeValueSTRArrayAlt(node, L"size-type;sizeType", font_size_type_cell, L"cell\0char\0");

	int align		= xmlGetAttributeValueSTRArray(node, L"align", 0, L"left\0right\0center\0");
	BOOL vCenter	= xmlGetAttributeBOOLAlt(node, L"v-center;vCenter", TRUE);
	BOOL singleLine	= xmlGetAttributeBOOLAlt(node, L"single-line;singleLine", TRUE);
	m_flags = DT_NOPREFIX;
	switch(align)
	{
	case 0:
		m_flags |= DT_LEFT;
		break;
	case 1:
		m_flags |= DT_RIGHT;
		break;
	case 2:
		m_flags |= DT_CENTER;
		break;
	}
	if(vCenter)		m_flags |= DT_VCENTER;
	if(singleLine)
	{
		m_flags |= DT_SINGLELINE;
	} else
	{
		m_flags |= DT_WORDBREAK;
	}

	loadXMLStates(node, baseurl);

	return ret;
}

elementData* TxSkin::elText::getStateData( IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam )
{
	LPWSTR strColor		= xmlGetAttributeSTR(node, L"color");
	if(!strColor)
	{
		return NULL;
	}

	LPWSTR strGlowColor	= xmlGetAttributeSTRAlt(node, L"glow-color;glowColor");
	TxSkin::color	color		= color::from_string(strColor);
	TxSkin::color	glowColor;
	if(strGlowColor)
	{
		glowColor = color::from_string(strGlowColor);
	}
	freeClearStr(strColor);
	freeClearStr(strGlowColor);
	return new elTextData(color, glowColor);
}

BOOL TxSkin::elText::getString( std::wstring& outStr, int sliceID )
{
	TxSkin::parameter* param = m_skin->findParam(m_paramID);
	if(param)
	{
		sliceParameter* slice = param->getSlice(sliceID);
		if(slice)
		{
			switch(m_show_units)
			{
			case show_units_right:
				{
					outStr = slice->stringVal() ? slice->stringVal() : L"";
					if(slice->units_str())
					{
						outStr += slice->units_str();
					}
				}
				break;
			case show_units_left:
				{
					outStr = slice->units_str() ? slice->units_str() : L"";
					if(slice->stringVal())
					{
						outStr += slice->stringVal();
					}
				}
				break;
			default:
				outStr = slice->stringVal() ? slice->stringVal() : L"";
				break;
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL TxSkin::elText::isUsedParam( LPCWSTR paramID )
{
	if(skin_element::isUsedParam(paramID))
	{
		return TRUE;
	}
	if(m_paramID && !lstrcmpi(m_paramID, paramID))
	{
		return TRUE;
	}
	return FALSE;
}

void TxSkin::elText::apply_scale_factor( double scale )
{
	if(scale)
	{
		skin_element::apply_scale_factor(scale);
		m_fontSize = (int) ((double) m_fontSize * scale);
	}
}
