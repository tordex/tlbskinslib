#pragma once
#include "elementBase.h"

namespace TxSkin
{
	class elSliderData : public elementData
	{
	public:
		CTxDIB*		m_imgThumb;
		CTxDIB*		m_bgStart;
		CTxDIB*		m_bgEnd;
		CTxDIB*		m_background;
		int			m_startMargin;
		int			m_endMargin;
	public:
		elSliderData()
		{
			m_bgStart		= NULL;
			m_bgEnd			= NULL;
			m_background	= NULL;
			m_imgThumb		= NULL;
			m_startMargin	= 0;
			m_endMargin		= 0;
		}
		virtual ~elSliderData();
	};


	class elSliderBase : public skin_element
	{
	protected:
		LPWSTR	m_valParam;
		LPWSTR	m_maxValParam;
	public:
		elSliderBase(	skin* skin,
						LPCWSTR param,
						LPCWSTR maxParam,
						LPCWSTR id		= NULL,
						LPCWSTR modParam = NULL);

		virtual ~elSliderBase(void);

		virtual BOOL			loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual elementData*	getStateData(IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam);
		virtual BOOL			isUsedParam(LPCWSTR paramID);
		virtual void			onCapture(BOOL capture, int sliceID);
	private:
		CTxDIB*					load_bmp(LPCWSTR flName, LPCWSTR baseurl);
	};
}
