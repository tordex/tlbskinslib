#pragma once
#include "elementBase.h"

namespace TxSkin
{
	class elChartData : public elementData
	{
	public:
		TxSkin::brush*	m_brush;
		TxSkin::pen*	m_pen;
	public:
		elChartData(TxSkin::brush* brush, TxSkin::pen* pen)
		{
			m_brush	= brush;
			m_pen	= pen;
		}
		virtual ~elChartData();
	};

	class elColorChartBase : public skin_element
	{
	protected:
		txArray<LPWSTR>	m_values;
	public:
		elColorChartBase(	skin* skin, 
							LPCWSTR modParam	= NULL, 
							LPCWSTR id			= NULL);
		virtual ~elColorChartBase(void);
		
		virtual BOOL				loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual elementData*		getStateData(IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam);
		virtual BOOL				isUsedParam(LPCWSTR paramID);

		void addValue(LPCWSTR paramID, TxSkin::modParamValue& modVal, TxSkin::elementState state, TxSkin::brush* brush, TxSkin::pen* pen);
	};
}
