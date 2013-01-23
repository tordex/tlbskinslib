#pragma once
#include "elementBase.h"

namespace TxSkin
{
	class elRotateImageData : public elementData
	{
	public:
		CTxDIB*	m_image;
	public:
		elRotateImageData(CTxDIB* image)
		{
			m_image	= image;
		}
		virtual ~elRotateImageData();
	};

	class elRotateImage : public skin_element
	{
		int					m_startAngle;
		int					m_endAngle;
		LPWSTR				m_valParam;
		LPWSTR				m_maxValParam;
		int					m_rotateX;
		int					m_rotateY;
	public:
		elRotateImage(	skin* skin, 
						int startAngle			= 360, 
						int endAngle			= 0, 
						LPCWSTR valParam		= NULL, 
						LPCWSTR maxValParam		= NULL, 
						LPCWSTR id				= NULL);
		virtual ~elRotateImage();

		virtual void draw(cairo_t* cr, int left, int top, int sliceID);

		void addState(CTxDIB* img, TxSkin::modParamValue& modVal, elementState state);
		void addState(LPCWSTR  img, TxSkin::modParamValue& modVal, elementState state);

		void addState(CTxDIB* img, TxSkin::modParamValue& modVal, BOOL isVertical, int statesCount);
		void addState(LPCWSTR  img, TxSkin::modParamValue& modVal, BOOL isVertical, int statesCount);

		void set_rotate_point(int x, int y)		{ m_rotateX = x; m_rotateY = y; }

		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual void	processXMLState(IXMLDOMNode* node, TxSkin::elementState state, TxSkin::modParamValue& modVal, LPCWSTR baseurl, ULONG_PTR lParam);
		virtual BOOL	isUsedParam(LPCWSTR paramID);
	};
}
