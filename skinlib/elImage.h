#pragma once
#include "elementBase.h"

namespace TxSkin
{

	class elImageData : public elementData
	{
	public:
		CTxDIB*				m_img;
		TxSkin::margins		m_margins;
	public:
		elImageData(CTxDIB* img, const TxSkin::margins* mg)
		{
			m_img = img;
			if(mg)
			{
				m_margins = *mg;
			} else
			{
				m_margins.left		= 0;
				m_margins.right		= 0;
				m_margins.top		= 0;
				m_margins.bottom	= 0;
			}
		}
		virtual ~elImageData();
	};

	class elImage : public skin_element
	{
	public:
		elImage(	skin* skin, 
					LPCWSTR paramID = NULL, 
					LPCWSTR id		= NULL);
		virtual ~elImage();

		void addImage(CTxDIB* img, TxSkin::modParamValue& modVal, elementState state, const TxSkin::margins* mg);
		void addImage(LPCWSTR  img, TxSkin::modParamValue& modVal, elementState state, const TxSkin::margins* mg);

		void addImageStates(CTxDIB* img, BOOL isVertical, int count, TxSkin::modParamValue& modVal, const TxSkin::margins* mg);
		void addImageStates(LPCWSTR  img, BOOL isVertical, int count, TxSkin::modParamValue& modVal, const TxSkin::margins* mg);

		virtual void	draw(cairo_t* cr, int left, int top, int sliceID);
		virtual void	init();
		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual void	processXMLState(IXMLDOMNode* node, TxSkin::elementState state, TxSkin::modParamValue& modVal, LPCWSTR baseurl, ULONG_PTR lParam);

	private:
		CTxDIB* getImage(int sliceID, TxSkin::margins* mg);
	};
}
