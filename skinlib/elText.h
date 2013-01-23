#pragma  once
#include "elementBase.h"

namespace TxSkin
{

	class elTextData : public elementData
	{
	public:
		TxSkin::color	m_color;
		TxSkin::color	m_glowColor;
	public:
		elTextData(TxSkin::color color, TxSkin::color glowColor)
		{
			m_color		= color;
			m_glowColor	= glowColor;
		}
	};

	enum show_units
	{
		show_units_none,
		show_units_left,
		show_units_right
	};

	class elText : public skin_element
	{
		LPWSTR					m_paramID;
		show_units				m_show_units;
		LPWSTR					m_fontName;
		UINT					m_flags;
		BOOL					m_bold;
		BOOL					m_italic;
		HFONT					m_hFont;
		int						m_glowSize;
		BOOL					m_antialias;
		int						m_fontSize;
		font_size_type			m_size_type;
	public:
		elText(	skin* skin, 
				LPCWSTR paramID				= NULL, 
				LPCWSTR fontName			= L"Arial", 
				BOOL bold					= FALSE, 
				BOOL italic					= FALSE, 
				UINT flags					= 0, 
				TxSkin::color color			= TxSkin::color(0, 0, 0, 0), 
				BOOL antialias				= TRUE, 
				int glowSize				= 0, 
				TxSkin::color glowColor		= TxSkin::color(0, 0, 0, 0), 
				LPCWSTR id					= NULL, 
				LPCWSTR modParam			= NULL);
		virtual ~elText();

		virtual void	draw(cairo_t* cr, int left, int top, int sliceID);
		virtual void	init();
		virtual	int		calcWidth();
		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual elementData* getStateData(IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam);
		virtual BOOL	getString(std::wstring& outStr, int sliceID);
		virtual BOOL	isUsedParam(LPCWSTR paramID);
		virtual void	apply_scale_factor(double scale);

		int		fontSize()						{ return m_fontSize;	}
		void	fontSize(int val)				{ m_fontSize = val;		}
		void	set_show_units(show_units val)	{ m_show_units = val;	}
		void	addState(elementState uState, TxSkin::color color, TxSkin::color glowColor = TxSkin::color(0, 0, 0, 0), TxSkin::modParamValue& modVal = TxSkin::modParamValue((int) 0));
	private:
		TxSkin::color	getColor(int sliceID);
	};
}
