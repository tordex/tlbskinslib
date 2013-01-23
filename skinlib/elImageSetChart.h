#pragma once
#include "elementBase.h"

namespace TxSkin
{
	class elImageSetChartData : public elementData
	{
	public:
		CTxDibSet*	img;
		int			count;
		BOOL		vertical;
	public:
		elImageSetChartData(CTxDIB* bmp, int framesCount, int isVertical)
		{
			if(isVertical)
			{
				img = new CTxDibSet(bmp, framesCount, 1);
			} else
			{
				img = new CTxDibSet(bmp, 1, framesCount);
			}
		}
		virtual ~elImageSetChartData();
	};

	class elImageSetChart : public skin_element
	{
		struct chart_data
		{
			CTxDIB*		bmp;
			int			modVal;
		};
		LPWSTR			m_paramID;
		LPWSTR			m_maxValID;
	public:
		elImageSetChart(	skin* skin, 
							LPWSTR paramID		= NULL, 
							LPWSTR maxValID		= NULL, 
							LPWSTR modParamID	= NULL, 
							LPCWSTR id			= NULL);
		virtual ~elImageSetChart();

		void addValue(CTxDIB* bmp, int count, BOOL isVertical, TxSkin::elementState state, TxSkin::modParamValue& modVal);

		virtual void			draw(cairo_t* cr, int left, int top, int sliceID);
		virtual BOOL			loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual elementData*	getStateData(IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam);
		virtual BOOL			isUsedParam(LPCWSTR paramID);
	};
}
