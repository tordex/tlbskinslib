#pragma once
#include "elementBase.h"

namespace TxSkin
{

	class elColorRectData : public elementData
	{
	public:
		TxSkin::brush*	m_brush;
		TxSkin::pen*	m_pen;
	public:
		elColorRectData(TxSkin::brush* brush, TxSkin::pen* pen)
		{
			m_brush	= brush;
			m_pen	= pen;
		}
		virtual ~elColorRectData();
	};

	class elColorRect : public skin_element
	{
		BOOL				m_useParent;
	public:
		elColorRect(skin* skin, LPCWSTR modParam = NULL, BOOL useParent = TRUE, LPCWSTR id = NULL);
		virtual ~elColorRect();

		void addColor(TxSkin::modParamValue& modVal, TxSkin::brush* brush, TxSkin::pen* pen, elementState state = elStateNormal);
		virtual void draw(cairo_t* cr, int left, int top, int sliceID);

		virtual int		width();
		virtual int		height();
		virtual void	width(int wVal)		{ m_cx = wVal;		}
		virtual void	height(int hVal)	{ m_cy = hVal;		}

		virtual void	X(int xVal)			{ m_x = xVal;		}
		virtual int		X();
		virtual void	Y(int yVal)			{ m_y = yVal;		}
		virtual int		Y();
		virtual BOOL	isBackground()		{ return TRUE;		}
		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual elementData* getStateData(IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam);

	private:
		void getColors(TxSkin::brush** brush, TxSkin::pen** pen, int sliceID);
	};
}
