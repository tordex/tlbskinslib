#include "TxSkinEngine.h"
#include "elImage.h"
#include "skinBase.h"

using namespace TxSkin;

elImage::elImage( skin* skin, LPCWSTR paramID, LPCWSTR id ) : skin_element(skin, id, paramID)
{
}

elImage::~elImage()
{
}

void elImage::addImage( CTxDIB* img, TxSkin::modParamValue& modVal, elementState state, const TxSkin::margins* mg )
{
	elImageData* data = new elImageData(img, mg);
	addModState(modVal, state, data);
	if(img)
	{
		if(width()  < (int) img->getWidth())	width(img->getWidth());
		if(height() < (int) img->getHeight())	height(img->getHeight());
	}
}

void elImage::addImage( LPCWSTR img, TxSkin::modParamValue& modVal, elementState state, const TxSkin::margins* mg )
{
	addImage(new CTxDIB(img), modVal, state, mg);
}

void elImage::addImageStates( CTxDIB* img, BOOL isVertical, int count, TxSkin::modParamValue& modVal, const TxSkin::margins* mg )
{
	int rows = 1;
	int cols = 1;
	if(isVertical)
	{
		rows = count;
	} else
	{
		cols = count;
	}
	CTxDibSet imgSet(img, rows, cols);
	if(isVertical)
	{
		for(int i=0; i < count; i++)
		{
			addImage(new CTxDIB(*imgSet.get(0, i)), modVal, (elementState) i, mg);
		}
	} else
	{
		for(int i=0; i < count; i++)
		{
			addImage(new CTxDIB(*imgSet.get(i, 0)), modVal, (elementState) i, mg);
		}
	}
}

void elImage::addImageStates( LPCWSTR img, BOOL isVertical, int count, TxSkin::modParamValue& modVal, const TxSkin::margins* mg )
{
	CTxDIB* pImg = new CTxDIB(img);
	if(pImg->isValid())
	{
		addImageStates(pImg, isVertical, count, modVal, mg);
	}
	delete pImg;
}

void elImage::draw( cairo_t* cr, int left, int top, int sliceID )
{
	TxSkin::margins mg;
	CTxDIB* img = getImage(sliceID, &mg);
	if(img)
	{
		MARGINS dmg = mg;

		CTxSkinDIB skin;
		skin.load(img, &dmg, FALSE, FALSE);


		RECT rcDraw;
		rcDraw.left		= X() + left;
		rcDraw.top		= Y() + top;
		rcDraw.right	= rcDraw.left + width();
		rcDraw.bottom	= rcDraw.top + height();
		
		HDC hdc = cairo_win32_surface_get_dc(cairo_get_target(cr));
		skin.draw(hdc, &rcDraw, NULL);
		GdiFlush();
	}
}

CTxDIB* elImage::getImage(int sliceID, TxSkin::margins* mg)
{
	elImageData* data = (elImageData*) getModState(sliceID);
	if(data)
	{
		*mg = data->m_margins;
		return data->m_img;
	}
	return NULL;
}

void elImage::init()
{
	if(!width() || !height())
	{
		int h = 0;
		int w = 0;
		for(int i=0; i < m_states.GetCount(); i++)
		{
			elImageData* data = (elImageData*) m_states[i]->data;
			if(data && data->m_img)
			{
				if(w < (int) data->m_img->getWidth())
				{
					w = data->m_img->getWidth();
				}
				if(h < (int) data->m_img->getHeight())
				{
					h = data->m_img->getHeight();
				}
			}
		}
		if(!width())	width(w);
		if(!height())	height(h);
	}

	skin_element::init();
}

BOOL TxSkin::elImage::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	BOOL ret = TxSkin::skin_element::loadXML(node, baseurl);

	loadXMLStates(node, baseurl);

	return ret;
}

void TxSkin::elImage::processXMLState( IXMLDOMNode* node, TxSkin::elementState state, TxSkin::modParamValue& modVal, LPCWSTR baseurl, ULONG_PTR lParam )
{
	LPWSTR file = xmlGetAttributeSTR(node, L"file");
	if(file)
	{
		TxSkin::margins margins;
		margins.left	= xmlGetAttributeIntAlt(node,	L"sizing-margin-left;sizing_margin_left",		0);
		margins.right	= xmlGetAttributeIntAlt(node,	L"sizing-margin-right;sizing_margin_right",		0);
		margins.top		= xmlGetAttributeIntAlt(node,	L"sizing-margin-top;sizing_margin_top",			0);
		margins.bottom	= xmlGetAttributeIntAlt(node,	L"sizing-margin-bottom;sizing_margin_bottom",	0);

		int states = xmlGetAttributeValueNonSTR<int>(node, L"states", 0);
		WCHAR fileName[MAX_PATH];
		StringCchCopy(fileName, MAX_PATH, baseurl);
		StringCchCat(fileName, MAX_PATH, file);
		if(!states)
		{
			addImage(fileName, modVal, state, &margins);
		} else
		{
			BOOL vertical = xmlGetAttributeValueBOOL(node, L"vertical", TRUE);
			addImageStates(fileName, vertical, states, modVal, &margins);
		}
		delete file;
	}
}

TxSkin::elImageData::~elImageData()
{
	if(m_img)
	{
		delete m_img;
	}
}
