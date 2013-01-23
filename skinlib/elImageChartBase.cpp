#include "TxSkinEngine.h"
#include "elImageChartBase.h"
#include "skinBase.h"

TxSkin::elImageChartBase::elImageChartBase( skin* skin, LPCWSTR id /*= NULL*/, LPCWSTR modParam /*= NULL*/ ) : skin_element(skin, id, modParam)
{

}
TxSkin::elImageChartBase::~elImageChartBase(void)
{
	for(int i=0; i < m_values.GetCount(); i++)
	{
		freeClearStr(m_values[i]);
	}
	m_values.Clear();
}

BOOL TxSkin::elImageChartBase::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	BOOL ret = skin_element::loadXML(node, baseurl);

	IXMLDOMNode* child = NULL;
	node->get_firstChild(&child);
	while(child)
	{
		BSTR name = NULL;
		child->get_nodeName(&name);
		if(name)
		{
			if(!lstrcmp(name, L"value"))
			{
				LPWSTR str = xmlGetAttributeSTR(child, L"src");
				if(str)
				{
					m_values.Add(&str);
					loadXMLStates(child, baseurl, TxSkin::elStateNormal, TxSkin::modParamValue((int) 0), (ULONG_PTR) (m_values.GetCount() - 1));
				}
			}
			SysFreeString(name);
		}

		IXMLDOMNode* newChild = NULL;
		child->get_nextSibling(&newChild);
		child->Release();
		child = newChild;
	}

	return ret;
}

TxSkin::elementData* TxSkin::elImageChartBase::getStateData( IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam )
{
	LPWSTR file = xmlGetAttributeSTR(node, L"file");
	CTxDIB* bmp = NULL;
	TxSkin::margins margins;
	if(file)
	{
		WCHAR fileName[MAX_PATH];
		StringCchCopy(fileName, MAX_PATH, baseurl);
		StringCchCat(fileName, MAX_PATH, file);
		bmp = new CTxDIB(fileName);
		delete file;
		margins.left	= xmlGetAttributeIntAlt(node, L"sizing-margin-left;sizing_margin_left",		0);
		margins.right	= xmlGetAttributeIntAlt(node, L"sizing-margin-right;sizing_margin_right",		0);
		margins.top		= xmlGetAttributeIntAlt(node, L"sizing-margin-top;sizing_margin_top",		0);
		margins.bottom	= xmlGetAttributeIntAlt(node, L"sizing-margin-bottom;sizing_margin_bottom",	0);
	}
	return new elImageChartData(bmp, &margins);
}

void TxSkin::elImageChartBase::addValue( LPCWSTR paramID, TxSkin::modParamValue& modVal, TxSkin::elementState state, CTxDIB* bmp, const TxSkin::margins* margins )
{
	if(paramID)
	{
		LPWSTR p = NULL;
		makeStr(p, paramID);
		m_values.Add(&p);
		addModState(modVal, state, new elImageChartData(bmp, margins), (ULONG_PTR) (m_values.GetCount() - 1));
	}
}

BOOL TxSkin::elImageChartBase::isUsedParam( LPCWSTR paramID )
{
	if(skin_element::isUsedParam(paramID))
	{
		return TRUE;
	}
	for(int i=0; i < m_values.GetCount(); i++)
	{
		if(m_values[i] && !lstrcmpi(m_values[i], paramID))
		{
			return TRUE;
		}
	}
	return FALSE;
}

TxSkin::elImageChartData::~elImageChartData()
{
	if(m_bmp)	delete m_bmp;
}
