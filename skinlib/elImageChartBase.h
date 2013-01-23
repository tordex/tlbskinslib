#pragma once
#include "elementBase.h"

namespace TxSkin
{
	class elImageChartData : public elementData
	{
	public:
		CTxDIB*				m_bmp;
		TxSkin::margins		m_margins;
	public:
		elImageChartData(CTxDIB* bmp, const TxSkin::margins* margins)
		{
			m_bmp		= bmp;
			if(margins)
			{
				m_margins	= *margins;
			}
		}
		virtual ~elImageChartData();
	};


	class elImageChartBase : public skin_element
	{
	protected:
		txArray<LPWSTR>		m_values;
	public:
		elImageChartBase(	skin* skin, 
							LPCWSTR id			= NULL, 
							LPCWSTR modParam	= NULL);
		virtual ~elImageChartBase(void);

		void addValue(LPCWSTR paramID, TxSkin::modParamValue& modVal, TxSkin::elementState state, CTxDIB* bmp, const TxSkin::margins* margins);

		virtual BOOL			loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual elementData*	getStateData(IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam);
		virtual BOOL			isUsedParam(LPCWSTR paramID);
	};
}
