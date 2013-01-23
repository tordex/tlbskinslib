#include "TxSkinEngine.h"
#include "skinBase.h"
#include "elSliderBase.h"

TxSkin::elSliderBase::elSliderBase( skin* skin, LPCWSTR param, LPCWSTR maxParam, LPCWSTR id /*= NULL*/, LPCWSTR modParam /*= NULL*/ ) : TxSkin::skin_element(skin, id, modParam)
{
	m_valParam		= NULL;
	m_maxValParam	= NULL;

	makeStr(m_valParam,		param);
	makeStr(m_maxValParam,	maxParam);
}

TxSkin::elSliderBase::~elSliderBase( void )
{
	freeClearStr(m_valParam);
	freeClearStr(m_maxValParam);
}

BOOL TxSkin::elSliderBase::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	BOOL ret = skin_element::loadXML(node, baseurl);

	freeClearStr(m_valParam);
	freeClearStr(m_maxValParam);

	m_valParam		= xmlGetAttributeSTR(node, L"value");
	m_maxValParam	= xmlGetAttributeSTRAlt(node, L"max-value;max_value");

	loadXMLStates(node, baseurl);

	return ret;
}

TxSkin::elementData* TxSkin::elSliderBase::getStateData( IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam )
{
	elSliderData* data = new elSliderData();

	LPWSTR background	= xmlGetAttributeSTR(node,		L"background");
	LPWSTR bg_start		= xmlGetAttributeSTRAlt(node,	L"background-start;background_start");
	LPWSTR bg_end		= xmlGetAttributeSTRAlt(node,	L"background-end;background_end");
	LPWSTR thumb		= xmlGetAttributeSTR(node,		L"ball");

	data->m_startMargin = xmlGetAttributeIntAlt(node,	L"margin-start;margin_start",	0);
	data->m_endMargin	= xmlGetAttributeIntAlt(node,	L"margin-end;margin_end",		0);
	
	data->m_background	= load_bmp(background, baseurl);
	data->m_bgStart		= load_bmp(bg_start, baseurl);
	data->m_bgEnd		= load_bmp(bg_end, baseurl);
	data->m_imgThumb	= load_bmp(thumb, baseurl);

	freeClearStr(background);
	freeClearStr(bg_start);
	freeClearStr(bg_end);
	freeClearStr(thumb);

	return data;
}

CTxDIB* TxSkin::elSliderBase::load_bmp( LPCWSTR flName, LPCWSTR baseurl )
{
	CTxDIB* bmp = NULL;
	if(flName && baseurl)
	{
		WCHAR path[MAX_PATH];
		StringCchCopy(path, MAX_PATH, baseurl);
		PathAddBackslash(path);
		StringCchCat(path, MAX_PATH, flName);
		bmp = new CTxDIB(path);
	}
	return bmp;
}

BOOL TxSkin::elSliderBase::isUsedParam( LPCWSTR paramID )
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

void TxSkin::elSliderBase::onCapture( BOOL capture, int sliceID )
{
	if(m_skin->getCallback())
	{
		if(capture)
		{
			m_skin->getCallback()->OnSliderBegin(m_valParam, sliceID);
		} else
		{
			m_skin->getCallback()->OnSliderEnd(m_valParam, sliceID);
		}
	}
}

TxSkin::elSliderData::~elSliderData()
{
	if(m_bgStart)		delete m_bgStart;
	if(m_bgEnd)			delete m_bgEnd;
	if(m_imgThumb)		delete m_imgThumb;
	if(m_background)	delete m_background;
}
