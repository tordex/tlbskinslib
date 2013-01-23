#include "TxSkinEngine.h"
#include "elImageSetChart.h"
#include "skinBase.h"

using namespace TxSkin;


TxSkin::elImageSetChart::elImageSetChart( skin* skin, LPWSTR paramID, LPWSTR maxValID, LPWSTR modParamID, LPCWSTR id ) : skin_element(skin, id)
{
	m_paramID		= NULL;
	m_maxValID		= NULL;

	makeStr(m_paramID,		paramID);
	makeStr(m_maxValID,		maxValID);
	makeStr(m_modParam,		modParamID);
}

TxSkin::elImageSetChart::~elImageSetChart()
{
	freeClearStr(m_maxValID);
	freeClearStr(m_paramID);
}

void TxSkin::elImageSetChart::addValue( CTxDIB* bmp, int count, BOOL isVertical, TxSkin::elementState state, TxSkin::modParamValue& modVal )
{
	if(bmp)
	{
		addModState(modVal, state, new elImageSetChartData(bmp, count, isVertical));
	}
}

void TxSkin::elImageSetChart::draw( cairo_t* cr, int left, int top, int sliceID )
{
	HDC hdc = cairo_win32_surface_get_dc(cairo_get_target(cr));

	elImageSetChartData* data = (elImageSetChartData*) getModState(sliceID);

	if(data)
	{
		int maxVal		= m_skin->getParamInt(m_maxValID,	sliceID);
		int val			= m_skin->getParamInt(m_paramID,	sliceID);

		int imgIDX = 0;
		if(maxVal)
		{
			imgIDX = (int) ((double) val / (double) maxVal * (double) data->count);
		}
		if(imgIDX >= data->count) imgIDX = data->count - 1;
		if(data->vertical)
		{
			if(data->img->get(0, imgIDX))
			{
				data->img->get(0, imgIDX)->draw(hdc, X() + left, Y() + top);
			}
		} else
		{
			if(data->img->get(0, imgIDX))
			{
				data->img->get(imgIDX, 0)->draw(hdc, X() + left, Y() + top);
			}
		}
	}

	GdiFlush();
}

BOOL TxSkin::elImageSetChart::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	BOOL ret = TxSkin::skin_element::loadXML(node, baseurl);

	freeClearStr(m_maxValID);
	freeClearStr(m_paramID);

	m_paramID		= xmlGetAttributeSTR(node, L"src");
	m_maxValID		= xmlGetAttributeSTR(node, L"total");

	loadXMLStates(node, baseurl);

	return ret;
}

elementData* TxSkin::elImageSetChart::getStateData( IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam )
{
	elementData* ret = NULL;
	LPWSTR file = xmlGetAttributeSTR(node, L"file");
	if(file)
	{
		int count = xmlGetAttributeValueNonSTR<int>(node, L"count", 0);
		BOOL vertical = xmlGetAttributeValueBOOL(node, L"vertical", TRUE);
		WCHAR fileName[MAX_PATH];
		StringCchCopy(fileName, MAX_PATH, baseurl);
		StringCchCat(fileName, MAX_PATH, file);
		if(count)
		{
			ret = new elImageSetChartData(new CTxDIB(fileName), count, vertical);
		}
		delete file;
	}
	return ret;
}

BOOL TxSkin::elImageSetChart::isUsedParam( LPCWSTR paramID )
{
	if(skin_element::isUsedParam(paramID))
	{
		return TRUE;
	}
	if(m_paramID && !lstrcmpi(m_paramID, paramID))
	{
		return TRUE;
	}
	if(m_maxValID && !lstrcmpi(m_maxValID, paramID))
	{
		return TRUE;
	}
	return FALSE;
}

TxSkin::elImageSetChartData::~elImageSetChartData()
{
	if(img)	delete img;
}
