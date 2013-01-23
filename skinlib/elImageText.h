#pragma once
#include "elementBase.h"

namespace TxSkin
{
	class elImageTextUnitsData : public elementData
	{
	public:
		CTxDIB*	m_bmp;
	public:
		elImageTextUnitsData(CTxDIB* bmp)
		{
			m_bmp = bmp;
		}

		~elImageTextUnitsData()
		{
			if(m_bmp)
			{
				delete m_bmp;
			}
		}
	};

	class elImageTextUnits : public skin_element
	{
		BOOL	m_atRight;
		LPWSTR	m_param;
	public:
		elImageTextUnits(	skin* skin, LPCWSTR param );
		virtual ~elImageTextUnits();

		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual void	processXMLState(IXMLDOMNode* node, TxSkin::elementState state, TxSkin::modParamValue& modVal, LPCWSTR baseurl, ULONG_PTR lParam);

		BOOL	position()	{ return m_atRight; }
	};

	class elImageTextData : public elementData
	{
	public:
		CTxDibSet*	img;
		BOOL		isVertical;
	public:
		elImageTextData(CTxDibSet* vImg, BOOL vertical)
		{
			img			= vImg;
			isVertical	= vertical;
		}
		virtual ~elImageTextData();
	};

	class elImageText : public skin_element
	{
		elImageTextUnits*	m_units;
		LPWSTR				m_paramID;
		txArray<LPWSTR>		m_alphabets;
		text_halign			m_align;
	public:
		elImageText(	skin* skin, 
						LPCWSTR paramID		= NULL, 
						LPCWSTR id			= NULL);
		virtual ~elImageText(void);

		void addAlphabetImage(LPCWSTR alphabet, CTxDIB* img, BOOL isVertical, elementState state, TxSkin::modParamValue& modVal);

		virtual void			draw(cairo_t* cr, int left, int top, int sliceID);
		virtual BOOL			loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual elementData*	getStateData(IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam);
		virtual BOOL			isUsedParam(LPCWSTR paramID);

	private:
		int get_text_width(LPCWSTR val, int sliceID);
	};
}
