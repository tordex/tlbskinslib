#include "TxSkinEngine.h"
#include "elRotateImage.h"
#include "skinBase.h"
#define _USE_MATH_DEFINES 
#include <math.h>


using namespace TxSkin;

elRotateImage::elRotateImage( skin* skin, int startAngle, int endAngle, LPCWSTR valParam, LPCWSTR maxValParam, LPCWSTR id ) : skin_element(skin, id)
{
	m_startAngle	= startAngle;
	m_endAngle		= endAngle;
	m_valParam		= NULL;
	m_maxValParam	= NULL;
	makeStr(m_valParam,		valParam);
	makeStr(m_maxValParam,	maxValParam);
}

elRotateImage::~elRotateImage()
{
	freeClearStr(m_maxValParam);
	freeClearStr(m_valParam);
}

void elRotateImage::addState( CTxDIB* img, TxSkin::modParamValue& modVal, elementState state )
{
	addModState(modVal, state, new elRotateImageData(img));
}

void elRotateImage::addState( LPCWSTR img, TxSkin::modParamValue& modVal, elementState state )
{
	addState(new CTxDIB(img), modVal, state);
}

void elRotateImage::draw( cairo_t* cr, int left, int top, int sliceID )
{
	cairo_save(cr);

	elRotateImageData* data = (elRotateImageData*) getModState(sliceID);
	if(data)
	{
		int val		= m_skin->getParamInt(m_valParam, sliceID);
		int maxVal	= _wtoi(m_maxValParam);
		if(!maxVal)
		{
			maxVal = m_skin->getParamInt(m_maxValParam, sliceID);
		}

		double angle = 0;
		if(m_startAngle < m_endAngle)
		{
			angle = m_startAngle + (double) val * (double) (m_endAngle - m_startAngle) / (double) maxVal;
		} else
		{
			angle = m_startAngle - (double) val * (double) (m_startAngle - m_endAngle) / (double) maxVal;
		}

		cairo_surface_t* img = cairo_image_surface_create_for_data((unsigned char*) data->m_image->getBits(), CAIRO_FORMAT_ARGB32, data->m_image->getWidth(), data->m_image->getHeight(), data->m_image->getWidth() * 4);

		cairo_matrix_t flib_m;
		cairo_matrix_init(&flib_m, 1, 0, 0, -1, 0, 0);

		cairo_translate (cr, X() + left, Y() + top);
		cairo_rotate (cr, angle * M_PI/180);
		cairo_translate (cr, -m_rotateX, -m_rotateY);

		cairo_transform(cr, &flib_m);
		cairo_translate (cr, 0, -data->m_image->getHeight());

		cairo_set_source_surface(cr, img, 0, 0);
		cairo_paint(cr);

		cairo_surface_destroy(img);
	}

	cairo_restore(cr);
}

TxSkin::elRotateImageData::~elRotateImageData()
{
	if(m_image)
	{
		delete m_image;
	}
}

BOOL TxSkin::elRotateImage::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	BOOL ret = TxSkin::skin_element::loadXML(node, baseurl);

	freeClearStr(m_maxValParam);
	freeClearStr(m_valParam);

	m_valParam		= xmlGetAttributeSTR(node, L"src");
	m_maxValParam	= xmlGetAttributeSTR(node, L"total");
	m_startAngle	= xmlGetAttributeIntAlt(node, L"start-angle;startAngle",		360);
	m_endAngle		= xmlGetAttributeIntAlt(node, L"end-angle;endAngle",			0);
	m_rotateX		= xmlGetAttributeIntAlt(node, L"rotate-x;rotateX",				0);
	m_rotateY		= xmlGetAttributeIntAlt(node, L"rotate-y;rotateY",				0);

	loadXMLStates(node, baseurl);

	return ret;
}

void TxSkin::elRotateImage::processXMLState( IXMLDOMNode* node, TxSkin::elementState state, TxSkin::modParamValue& modVal, LPCWSTR baseurl, ULONG_PTR lParam )
{
	LPWSTR file = xmlGetAttributeSTR(node, L"file");
	if(file)
	{
		int states = xmlGetAttributeValueNonSTR<int>(node, L"states", 0);
		WCHAR fileName[MAX_PATH];
		StringCchCopy(fileName, MAX_PATH, baseurl);
		StringCchCat(fileName, MAX_PATH, file);
		if(!states)
		{
			addState(fileName, modVal, state);
		} else
		{
			BOOL vertical = xmlGetAttributeValueBOOL(node, L"vertical", TRUE);
			addState(fileName, modVal, vertical, states);
		}
		delete file;
	}
}

void TxSkin::elRotateImage::addState( CTxDIB* img, TxSkin::modParamValue& modVal, BOOL isVertical, int statesCount )
{
	int rows = 1;
	int cols = 1;
	if(isVertical)
	{
		rows = statesCount;
	} else
	{
		cols = statesCount;
	}
	CTxDibSet imgSet(img, rows, cols);
	if(isVertical)
	{
		for(int i=0; i < statesCount; i++)
		{
			addState(new CTxDIB(*imgSet.get(0, i)), modVal, (elementState) i);
		}
	} else
	{
		for(int i=0; i < statesCount; i++)
		{
			addState(new CTxDIB(*imgSet.get(i, 0)), modVal, (elementState) i);
		}
	}
}

void TxSkin::elRotateImage::addState( LPCWSTR img, TxSkin::modParamValue& modVal, BOOL isVertical, int statesCount )
{
	CTxDIB dib(img);
	if(dib.isValid())
	{
		addState(&dib, modVal, isVertical, statesCount);
	}
}

BOOL TxSkin::elRotateImage::isUsedParam( LPCWSTR paramID )
{
	if(skin_element::isUsedParam(paramID))
	{
		return TRUE;
	}
	if(m_valParam && !lstrcmpi(m_valParam, paramID))
	{
		return TRUE;
	}
	if(m_maxValParam && !lstrcmpi(m_maxValParam, paramID))
	{
		return TRUE;
	}
	return FALSE;
}
