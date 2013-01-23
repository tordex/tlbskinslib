#include "TxSkinEngine.h"
#include "elColorChartBase.h"
#include "skinBase.h"

using namespace TxSkin;

TxSkin::elChartData::~elChartData()
{
	if(m_pen)		delete m_pen;
	if(m_brush)		delete m_brush;
}


TxSkin::elColorChartBase::elColorChartBase(skin* skin, LPCWSTR modParam, LPCWSTR id) : skin_element(skin, id)
{
	makeStr(m_modParam, modParam);
}

TxSkin::elColorChartBase::~elColorChartBase(void)
{
	for(int i=0; i < m_values.GetCount(); i++)
	{
		freeClearStr(m_values[i]);
	}
	m_values.Clear();
}

void TxSkin::elColorChartBase::addValue( LPCWSTR paramID, TxSkin::modParamValue& modVal, TxSkin::elementState state, TxSkin::brush* brush, TxSkin::pen* pen )
{
	if(paramID)
	{
		LPWSTR p = NULL;
		makeStr(p, paramID);
		m_values.Add(&p);
		addModState(modVal, state, new elChartData(brush, pen), (ULONG_PTR) (m_values.GetCount() - 1));
	}
}

BOOL TxSkin::elColorChartBase::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
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

elementData* TxSkin::elColorChartBase::getStateData( IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam )
{
	return new elChartData(loadBrush(node), loadPen(node));
}

BOOL TxSkin::elColorChartBase::isUsedParam( LPCWSTR paramID )
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
