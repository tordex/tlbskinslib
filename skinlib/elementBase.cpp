#include "TxSkinEngine.h"
#include "elementBase.h"
#include "skinBase.h"
#include "skinParams.h"

using namespace TxSkin;

TxSkin::skin_element::skin_element( skin* skin, LPCWSTR id, LPCWSTR modParam )
{
	m_mod_by_units	= false;
	m_shrink		= shrink_none;
	m_minWidth		= 0;
	m_minHeight		= 0;
	m_vAlign		= vAlignTop;
	m_hAlign		= hAlignLeft;
	m_modParam		= NULL;
	m_autoCalcWidth	= FALSE;
	m_skin			= skin;
	m_parent		= NULL;
	m_x				= 0;
	m_y				= 0;
	m_cx			= 0;
	m_cy			= 0;
	m_x_init		= 0;
	m_y_init		= 0;
	m_cx_init		= 0;
	m_cy_init		= 0;
	m_id			= NULL;
	m_flex			= 0;
	m_float			= FALSE;

	if(id && id[0])
	{
		makeStr(m_id,	id);
	}
	makeStr(m_modParam, modParam);
	addSlice(0);
}

TxSkin::skin_element::~skin_element()
{
	clear();
	freeClearStr(m_id);
	freeClearStr(m_modParam);
}

void TxSkin::skin_element::clear()
{
	for(int i=0; i < m_elements.GetCount(); i++)
	{
		delete m_elements[i];
	}
	m_elements.Clear();

	for(int i=0; i < m_states.GetCount(); i++)
	{
		delete m_states[i];
	}
	m_states.Clear();

	for(int i=0; i < m_slices.GetCount(); i++)
	{
		delete m_slices[i];
	}
	m_slices.Clear();

	m_cx = 0;
	m_cy = 0;
}

void TxSkin::skin_element::draw( cairo_t* cr, int left, int top, int sliceID )
{
	for(int i=0; i < m_elements.GetCount(); i++)
	{
		m_elements[i]->draw(cr, m_x + left, m_y + top, sliceID);
	}
}

void TxSkin::skin_element::addElement( skin_element* el )
{
	el->setParent(this);
	m_elements.Add(&el);
	for(int i=0; i < m_slices.GetCount(); i++)
	{
		el->addSlice(m_slices[i]->m_sliceID);
	}
}

void TxSkin::skin_element::init()
{
	int w = m_minWidth;
	int h = m_minHeight;

	for(int i=0; i < m_elements.GetCount(); i++)
	{
		if(!m_elements[i]->float_el())
		{
			m_elements[i]->init();
			w = max(w, m_elements[i]->X() + m_elements[i]->width());
			h = max(h, m_elements[i]->Y() + m_elements[i]->height());
		}
	}
	if(m_shrink == shrink_width || m_shrink == shrink_both || !width())		width(w);
	if(m_shrink == shrink_height || m_shrink == shrink_both || !height())	height(h);

	// process floating elements
	for(int i=0; i < m_elements.GetCount(); i++)
	{
		if(m_elements[i]->float_el())
		{
			switch(m_elements[i]->hAlign())
			{
			case hAlignRight:
				m_elements[i]->X(width() - m_elements[i]->margin_right() - m_elements[i]->width());
				break;
			case hAlignCenter:
				m_elements[i]->X(width() / 2 - m_elements[i]->width() / 2 + m_elements[i]->margin_left());
				break;
			case hAlignStretch:
				m_elements[i]->X(m_elements[i]->margin_left());
				m_elements[i]->width(width() - m_elements[i]->margin_left() - m_elements[i]->margin_right());
				break;
			default:
				m_elements[i]->X(m_elements[i]->margin_left());
				break;
			}
			switch(m_elements[i]->vAlign())
			{
			case vAlignBottom:
				m_elements[i]->Y(height() - m_elements[i]->margin_bottom() - m_elements[i]->height());
				break;
			case vAlignCenter:
				m_elements[i]->Y(height() / 2 - m_elements[i]->height() / 2 + m_elements[i]->margin_top());
				break;
			case vAlignStretch:
				m_elements[i]->Y(m_elements[i]->margin_top());
				m_elements[i]->height(height() - m_elements[i]->margin_top() - m_elements[i]->margin_bottom());
				break;
			default:
				m_elements[i]->Y(m_elements[i]->margin_top());
				break;
			}
			m_elements[i]->init();
		}
	}
}

BOOL TxSkin::skin_element::setState( elementState st, int sliceID )
{
	sliceElement* slice = getSlice(sliceID);
	if(slice)
	{
		if(slice->m_state != st)
		{
			slice->m_state = st;
			return TRUE;
		}
	}
	return FALSE;
}

skin_element* TxSkin::skin_element::findElement( int x, int y, int sliceID )
{
	POINT pt;
	pt.x = x; pt.y = y;
	skin_element* found = NULL;
	for(int i = m_elements.GetCount() - 1; i >= 0 && !found; i-- )
	{
		if(m_elements[i]->id())
		{
			if(m_elements[i]->getState(sliceID) != elStateDisabled)
			{
				RECT rcItem;
				m_elements[i]->getRect(&rcItem);
				rcItem.left		+= X();
				rcItem.right	+= X();
				rcItem.top		+= Y();
				rcItem.bottom	+= Y();
				if(PtInRect(&rcItem, pt))
				{
					found = m_elements[i];
				}
			}
		} else if(m_elements[i]->m_elements.GetCount())
		{
			if(m_elements[i]->getState(sliceID) != elStateDisabled)
			{
				found = m_elements[i]->findElement(x - X(), y - Y(), sliceID);
			}
		}
	}
	return found;
}

BOOL TxSkin::skin_element::mouseOver( int x, int y, int sliceID )
{
	RECT rcItem;
	getRect(&rcItem);
	POINT pt = {x, y};

	BOOL ret = FALSE;

	if(isCapture(sliceID))
	{
		if(PtInRect(&rcItem, pt))
		{
			ret = setState(elStatePushed, sliceID);
		} else
		{
			ret = setState(elStateNormal, sliceID);
		}
	} else
	{
		if(PtInRect(&rcItem, pt))
		{
			ret = setState(elStateHover, sliceID);
		} else
		{
			ret = setState(elStateNormal, sliceID);
		}
	}
	for(int i=0; i < m_elements.GetCount(); i++)
	{
		if(m_elements[i]->mouseOver(x - X(), y - Y(), sliceID))
		{
			ret = TRUE;
		}
	}
	return ret;
}

BOOL TxSkin::skin_element::mouseLeave(int sliceID)
{
	setCapture(FALSE, sliceID);
	BOOL ret = setState(elStateNormal, sliceID);
	for(int i = 0; i < m_elements.GetCount(); i++)
	{
		if(m_elements[i]->mouseLeave(sliceID))
		{
			ret = TRUE;
		}
	}
	return ret;
}


void TxSkin::skin_element::setCapture( BOOL isCapture, int sliceID )
{
	sliceElement* slice = getSlice(sliceID);
	if(slice)
	{
		if(slice->m_isCaptured != isCapture)
		{
			slice->m_isCaptured = isCapture;
			if(slice->m_isCaptured)
			{
				m_skin->setCapture(this, sliceID);
				onCapture(TRUE, sliceID);
			} else
			{
				m_skin->setCapture(NULL, sliceID);
				onCapture(FALSE, sliceID);
			}
		}
	}
}

BOOL TxSkin::skin_element::isCapture(int sliceID)
{
	sliceElement* slice = getSlice(sliceID);
	if(slice)
	{
		return slice->m_isCaptured;
	}
	return FALSE;
}

BOOL TxSkin::skin_element::lButtonDown( int x, int y, int sliceID )
{
	setCapture(TRUE, sliceID);
	return setState(elStatePushed, sliceID);
}

BOOL TxSkin::skin_element::lButtonUp( int x, int y, int sliceID )
{
	if(getState(sliceID) == elStatePushed)
	{
		if(m_skin->m_callback)
		{
			m_skin->m_callback->OnElementClick(m_id, sliceID);
		}
	}
	setCapture(FALSE, sliceID);
	return setState(elStateNormal, sliceID);
}

BOOL TxSkin::skin_element::mouseEnter(int sliceID)
{
	return setState(elStateHover, sliceID);
}

BOOL TxSkin::skin_element::disableItem( LPCWSTR id, BOOL disable, int sliceID )
{
	if(m_id)
	{
		if(!lstrcmp(id, m_id))
		{
			if(disable)
			{
				return setState(elStateDisabled, sliceID);
			} else
			{
				return setState(elStateNormal, sliceID);
			}
		}
	}
	for(int i=0; i < m_elements.GetCount(); i++)
	{
		if(m_elements[i]->disableItem(id, disable, sliceID))
		{
			return TRUE;
		}
	}
	return FALSE;
}

int TxSkin::skin_element::calcWidth()
{
	return m_cx;
}

void TxSkin::skin_element::recalcWidth()
{
	if(m_parent)	return;
	int maxWidth = 0;
	for(int i=0; i < m_elements.GetCount(); i++)
	{
		if(m_elements[i]->isBackground()) continue;

		int newWidth = m_elements[i]->calcWidth();
		if(newWidth != m_elements[i]->width())
		{
			for(int j=0; j < m_elements.GetCount(); j++)
			{
				if(i == j) continue;
				if(m_elements[j]->X() >= m_elements[i]->X() + m_elements[i]->width())
				{
					m_elements[j]->X(m_elements[j]->X() + newWidth - m_elements[i]->width());
				}
			}
			m_elements[i]->width(newWidth);
		}
		m_elements[i]->recalcWidth();
		if(maxWidth < m_elements[i]->X() + m_elements[i]->width())
		{
			maxWidth = m_elements[i]->X() + m_elements[i]->width();
		}
	}
	width(maxWidth);
}

sliceElement* TxSkin::skin_element::getSlice( int sliceID )
{
	for(int i=0; i < m_slices.GetCount(); i++)
	{
		if(m_slices[i]->m_sliceID == sliceID)
		{
			return m_slices[i];
		}
	}
	return NULL;
}

void TxSkin::skin_element::addSlice( int sliceID )
{
	if(!getSlice(sliceID))
	{
		sliceElement* slice = new sliceElement(sliceID);
		m_slices.Add(&slice);
		for(int i=0; i < m_elements.GetCount(); i++)
		{
			m_elements[i]->addSlice(sliceID);
		}
	}
}

elementState TxSkin::skin_element::getState( int sliceID )
{
	sliceElement* slice = getSlice(sliceID);
	if(slice)
	{
		return slice->m_state;
	}
	return elStateNormal;
}

void TxSkin::skin_element::deleteSlice( int sliceID )
{
	for(int i=0; i < m_slices.GetCount(); i++)
	{
		if(m_slices[i]->m_sliceID == sliceID)
		{
			delete m_slices[i];
			m_slices.Delete(i);
			break;
		}
	}
	for(int i=0; i < m_elements.GetCount(); i++)
	{
		m_elements[i]->deleteSlice(sliceID);
	}
}

BOOL TxSkin::skin_element::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	m_id			= xmlGetAttributeSTR(node, TEXT("command"));
	m_modParam		= xmlGetAttributeSTR(node, TEXT("mod"));
	m_x				= xmlGetAttributeValueNonSTR<int>(node,	TEXT("x"),				0);
	m_y				= xmlGetAttributeValueNonSTR<int>(node,	TEXT("y"),				0);
	m_cx			= xmlGetAttributeValueNonSTR<int>(node,	TEXT("width"),			0);
	m_cy			= xmlGetAttributeValueNonSTR<int>(node,	TEXT("height"),			0);
	
	m_flex			= xmlGetAttributeValueNonSTR<int>(node,	TEXT("flex"),			0);

	m_margin.left	= xmlGetAttributeIntAlt(node,	L"margin-left;marginLeft",		0);
	m_margin.right	= xmlGetAttributeIntAlt(node,	L"margin-right;marginRight",	0);
	m_margin.top	= xmlGetAttributeIntAlt(node,	L"margin-top;marginTop",		0);
	m_margin.bottom	= xmlGetAttributeIntAlt(node,	L"margin-bottom;marginBottom",	0);

	m_minWidth		= xmlGetAttributeIntAlt(node,	L"min-width;minWidth",		0);
	m_minHeight		= xmlGetAttributeIntAlt(node,	L"min-height;minHeight",		0);

	m_float			= xmlGetAttributeValueBOOL(node,	TEXT("float"),	FALSE);
	if(m_float)
	{
		m_vAlign = (vertical_align)		xmlGetAttributeValueSTRArray(node, L"valign", 0, L"top\0bottom\0center\0stretch\0");
		m_hAlign = (horizontal_align)	xmlGetAttributeValueSTRArray(node, L"halign", 0, L"left\0right\0center\0stretch\0");
	}

	m_shrink = (size_shink) xmlGetAttributeValueSTRArray(node, L"shrink", shrink_none, L"none\0width\0height\0both\0");

	return TRUE;
}

void TxSkin::skin_element::processXMLState( IXMLDOMNode* node, TxSkin::elementState state, TxSkin::modParamValue& modVal, LPCWSTR baseurl, ULONG_PTR lParam )
{

}

void TxSkin::skin_element::loadXMLStates( IXMLDOMNode* node, LPCWSTR baseurl, TxSkin::elementState state, TxSkin::modParamValue& modVal, ULONG_PTR lParam )
{
	IXMLDOMNode* child = NULL;

	node->get_firstChild(&child);
	if(child)
	{
		BSTR name = NULL;
		child->get_nodeName(&name);
		if(name)
		{
			if(lstrcmp(name, L"mod") && lstrcmp(name, L"state"))
			{
				child->Release();
				child = NULL;
			}
			SysFreeString(name);
		} else
		{
			child->Release();
			child = NULL;
		}
	}

	if(!child)
	{
		elementData* data = getStateData(node, baseurl, lParam);
		if(data)
		{
			addModState(modVal, state, data, lParam);
		} else
		{
			processXMLState(node, state, modVal, baseurl, lParam);
		}
	} else
	{
		while(child)
		{
			BSTR name = NULL;
			child->get_nodeName(&name);
			if(name)
			{
				if(!lstrcmp(name, L"mod"))
				{
					LPCWSTR strVal = xmlGetAttributeSTRAlt(child, L"string;strValue");
					if(strVal)
					{
						modParamValue val(strVal);
						delete strVal;
						loadXMLStates(child, baseurl, state, val, lParam);
					} else
					{
						int intVal = xmlGetAttributeValueNonSTR<int>(child, L"value", 0);
						modParamValue val(intVal);
						loadXMLStates(child, baseurl, state, val, lParam);
					}
				} else if(!lstrcmp(name, L"state"))
				{
					TxSkin::elementState val = (TxSkin::elementState) xmlGetAttributeValueSTRArray(child, L"value", 0, L"normal\0over\0down\0disabled\0");
					loadXMLStates(child, baseurl, val, modVal, lParam);
				}
				SysFreeString(name);
			}

			IXMLDOMNode* newChild = NULL;
			child->get_nextSibling(&newChild);
			child->Release();
			child = newChild;
		}
	}
}

TxSkin::brush* TxSkin::skin_element::loadBrush( IXMLDOMNode* node )
{
	TxSkin::brush* brush = NULL;
	IXMLDOMNode* nodeBrush = xmlOpenNode(node, L"brush");
	if(nodeBrush)
	{
		LPWSTR type = xmlGetAttributeSTR(nodeBrush, L"type");
		if(type)
		{
			bool load_colors = true;
			if(!lstrcmp(L"xGradient", type))
			{
				LPWSTR clr1 = xmlGetAttributeSTR(nodeBrush, L"color1");
				LPWSTR clr2 = xmlGetAttributeSTR(nodeBrush, L"color2");
				if(clr1 && clr2)
				{
					brush = new TxSkin::h_gradient_brush(color::from_string(clr1), color::from_string(clr2));
				}
				freeClearStr(clr1);
				freeClearStr(clr2);
			} else if(!lstrcmp(L"yGradient", type))
			{
				LPWSTR clr1 = xmlGetAttributeSTR(nodeBrush, L"color1");
				LPWSTR clr2 = xmlGetAttributeSTR(nodeBrush, L"color2");
				if(clr1 && clr2)
				{
					brush = new TxSkin::v_gradient_brush(color::from_string(clr1), color::from_string(clr2));
				}
				freeClearStr(clr1);
				freeClearStr(clr2);
			} else if(!lstrcmp(L"solid", type))
			{
				LPWSTR clr = xmlGetAttributeSTR(nodeBrush, L"color");
				if(clr)
				{
					load_colors = false;
					brush = new TxSkin::solid_brush(color::from_string(clr));
				}
				freeClearStr(clr);
			} else if(!lstrcmp(L"radial", type))
			{
				LPWSTR clr1 = xmlGetAttributeSTR(nodeBrush, L"color1");
				LPWSTR clr2 = xmlGetAttributeSTR(nodeBrush, L"color2");
				if(clr1 && clr2)
				{
					brush = new TxSkin::radial_gradient_brush(color::from_string(clr1), color::from_string(clr2));
				}
				freeClearStr(clr1);
				freeClearStr(clr2);
			}
			delete type;
			if(load_colors && brush)
			{
				IXMLDOMNode* child = NULL;
				nodeBrush->get_firstChild(&child);
				while(child)
				{
					BSTR name = NULL;
					child->get_nodeName(&name);
					if(name)
					{
						if(!lstrcmp(name, L"stop"))
						{
							int offset = xmlGetAttributeValueNonSTR<int>(child, L"offset", 0);
							LPWSTR clrStr = xmlGetAttributeSTR(child, L"color");
							if(clrStr)
							{
								color clr = color::from_string(clrStr);
								brush->add_color_stop((double) offset / 100.0, clr);
								freeClearStr(clrStr);
							}
						}
						SysFreeString(name);
					}

					IXMLDOMNode* newChild = NULL;
					child->get_nextSibling(&newChild);
					child->Release();
					child = newChild;
				}
			}
		}
		nodeBrush->Release();
	}
	return brush;
}

TxSkin::pen* TxSkin::skin_element::loadPen( IXMLDOMNode* node )
{
	TxSkin::pen* pen = NULL;
	IXMLDOMNode* nodePen = xmlOpenNode(node, L"pen");
	if(nodePen)
	{
		LPWSTR type = xmlGetAttributeSTR(nodePen, L"type");
		if(type)
		{
			if(!lstrcmp(L"solid", type))
			{
				LPWSTR clr = xmlGetAttributeSTR(nodePen, L"color");
				if(clr)
				{
					int width = xmlGetAttributeValueNonSTR<int>(nodePen, L"width", 1);
					pen = new TxSkin::solid_pen(color::from_string(clr), width);
				}
				freeClearStr(clr);
			}
			delete type;
		}
		nodePen->Release();
	}
	return pen;
}

void TxSkin::skin_element::addModState( TxSkin::modParamValue& modVal, TxSkin::elementState state, elementData* data, ULONG_PTR lParam )
{
	elModStates* st = new elModStates;
	st->data		= data;
	st->modVal		= modVal;
	st->state		= state;
	st->lParam		= lParam;
	m_states.Add(&st);
}

elementData* TxSkin::skin_element::getModState( int sliceID,  ULONG_PTR lParam )
{
	elementData* ret = NULL;

	int		intVal	= 0;
	LPCWSTR strVal	= NULL;
	int		state	= getState(sliceID);

	if(m_modParam)
	{
		TxSkin::parameter* param = m_skin->findParam(m_modParam);
		if(param)
		{
			sliceParameter* slice = param->getSlice(sliceID);
			if(slice)
			{
				if(!m_mod_by_units)
				{
					intVal = slice->intVal();
					strVal = slice->stringVal();
				} else
				{
					intVal = slice->units_int();
					strVal = slice->units_str();
				}
			}
		}
	}

	for(int i = 0; i < m_states.GetCount(); i++)
	{
		if(m_states[i]->state == state && m_states[i]->lParam == lParam)
		{
			if(m_states[i]->modVal.isStr())
			{
				if(m_states[i]->modVal == strVal)
				{
					ret = m_states[i]->data;
					break;
				}
			} else if(m_states[i]->modVal == intVal)
			{
				ret = m_states[i]->data;
				break;
			}
		}

		if(!ret)
		{
			if(m_states[i]->state == state || m_states[i]->lParam == lParam)
			{
				if(m_states[i]->modVal.isStr())
				{
					if(m_states[i]->modVal == strVal)
					{
						ret = m_states[i]->data;
					}
				} else if(m_states[i]->modVal == intVal)
				{
					ret = m_states[i]->data;
				}
			}
		}
	}
	return ret;
}

elementData* TxSkin::skin_element::getStateData( IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam )
{
	return NULL;
}

void TxSkin::skin_element::clearSlices()
{
	for(int i=0; i < m_slices.GetCount(); i++)
	{
		delete m_slices[i];
	}
	m_slices.Clear();
}

BOOL TxSkin::skin_element::isUsedParam( LPCWSTR paramID )
{
	if(m_modParam && !lstrcmp(m_modParam, paramID))
	{
		return TRUE;
	}
	for(int i = 0; i < m_elements.GetCount(); i++)
	{
		if(m_elements[i]->isUsedParam(paramID))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void TxSkin::skin_element::onCapture( BOOL capture, int sliceID )
{

}

void TxSkin::skin_element::set_root_size( int cx, int cy )
{
	if(!width())
	{
		width(cx);
	}
	if(!height())
	{
		height(cy);
	}
}

void TxSkin::skin_element::finish_loading()
{
	m_x_init		= m_x;
	m_y_init		= m_y;
	m_cx_init		= m_cx;
	m_cy_init		= m_cy;

	for(int i=0; i < m_elements.GetCount(); i++)
	{
		m_elements[i]->finish_loading();
	}
}

void TxSkin::skin_element::reset_positions()
{
	m_x		= m_x_init;
	m_y		= m_y_init;
	m_cx	= m_cx_init;
	m_cy	= m_cy_init;

	for(int i=0; i < m_elements.GetCount(); i++)
	{
		m_elements[i]->reset_positions();
	}
}

void TxSkin::skin_element::width( int wVal )
{
	m_cx = max(wVal, m_minWidth);
}

int TxSkin::skin_element::width()
{
	return m_cx;
}

void TxSkin::skin_element::height( int hVal )
{
	m_cy = max(hVal, m_minHeight);
}

int TxSkin::skin_element::height()
{
	return m_cy;
}

void TxSkin::skin_element::apply_scale_factor( double scale )
{
	if(scale)
	{
		m_x		= (int) ((double) m_x * scale);
		m_y		= (int) ((double) m_y * scale);
		m_cx	= (int) ((double) m_cx * scale);
		m_cy	= (int) ((double) m_cy * scale);

		for(int i=0; i < m_elements.GetCount(); i++)
		{
			m_elements[i]->apply_scale_factor(scale);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

TxSkin::sliceElement::sliceElement(int sliceID)
{
	m_sliceID		= sliceID;
	m_isCaptured	= FALSE;
	m_state			= elStateNormal;
}


TxSkin::elementData::elementData()
{

}

TxSkin::elementData::~elementData()
{

}