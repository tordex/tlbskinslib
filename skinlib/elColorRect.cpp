#include "TxSkinEngine.h"
#include "elColorRect.h"
#include "skinBase.h"

using namespace TxSkin;

elColorRect::elColorRect( skin* skin, LPCWSTR modParam, BOOL useParent, LPCWSTR id ) : skin_element(skin, id)
{
	m_useParent	= useParent;
	m_modParam	= NULL;
	makeStr(m_modParam, modParam);
}

void elColorRect::draw( cairo_t* cr, int left, int top, int sliceID )
{
	cairo_save(cr);
	
	TxSkin::brush*	brush	= NULL;
	TxSkin::pen*	pen		= NULL;
	getColors(&brush, &pen, sliceID);

	double pen_width = (pen ? pen->width_f() : 0);

	double x = pen_width / 2 + X() + left;
	double y = pen_width / 2 + Y() + top;
	double w = width() - pen_width;
	double h = height() - pen_width;

	cairo_rectangle(cr, x, y, w, h);

	if(brush)
	{
		brush->apply(cr, x, y, w, h);
		cairo_fill_preserve(cr);
	}
	if(pen)
	{
		pen->apply(cr);
		cairo_stroke(cr);
	}
	cairo_new_path(cr);
	cairo_restore(cr);
}

elColorRect::~elColorRect()
{
}

void elColorRect::getColors( TxSkin::brush** brush, TxSkin::pen** pen, int sliceID )
{
	elColorRectData* data = (elColorRectData*) getModState(sliceID);
	if(data)
	{
		brush[0]	= data->m_brush;
		pen[0]		= data->m_pen;
	}
}

void elColorRect::addColor( TxSkin::modParamValue& modVal, TxSkin::brush* brush, TxSkin::pen* pen, elementState state )
{
	elColorRectData* data = new elColorRectData(brush, pen);
	addModState(modVal, state, data);
}

int TxSkin::elColorRect::width()
{
	if(m_useParent)
	{
		return m_parent->width();
	}
	return skin_element::width();
}

int TxSkin::elColorRect::height()
{
	if(m_useParent)
	{
		return m_parent->height();
	}
	return skin_element::height();
}

int TxSkin::elColorRect::X()
{
	if(m_useParent)
	{
		return m_parent->X();
	}
	return skin_element::X();
}

int TxSkin::elColorRect::Y()
{
	if(m_useParent)
	{
		return m_parent->Y();
	}
	return skin_element::Y();
}

BOOL TxSkin::elColorRect::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	BOOL ret = skin_element::loadXML(node, baseurl);

	m_useParent	= xmlGetAttributeBOOLAlt(node, L"use-parent;useParent",	FALSE);
	loadXMLStates(node, baseurl);

	return ret;
}

elementData* TxSkin::elColorRect::getStateData( IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam )
{
	TxSkin::brush*	brush	= loadBrush(node);
	TxSkin::pen*	pen		= loadPen(node);

	if(brush || pen)
	{
		return new elColorRectData(brush, pen);
	}
	return NULL;
}

TxSkin::elColorRectData::~elColorRectData()
{
	if(m_brush)
	{
		delete m_brush;
	}
	if(m_pen)
	{
		delete m_pen;
	}
}
