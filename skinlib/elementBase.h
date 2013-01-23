#pragma once

namespace TxSkin
{
	enum horizontal_align
	{
		hAlignLeft,
		hAlignRight,
		hAlignCenter,
		hAlignStretch
	};

	enum vertical_align
	{
		vAlignTop,
		vAlignBottom,
		vAlignCenter,
		vAlignStretch
	};

	class skin;

	class sliceElement
	{
	public:
		int				m_sliceID;
		BOOL			m_isCaptured;
		elementState	m_state;

		sliceElement(int sliceID);
	};

	class modParamValue
	{
		int		m_intVal;
		LPWSTR	m_strVal;
		BOOL	m_isStr;
	public:
		modParamValue()
		{
			m_strVal	= NULL;
			m_intVal	= 0;
			m_isStr		= FALSE;
		}
		modParamValue(const modParamValue& val)
		{
			m_strVal	= NULL;
			makeStr(m_strVal, val.m_strVal);
			m_intVal	= val.m_intVal;
			m_isStr		= val.m_isStr;
		}
		modParamValue(int val)
		{
			m_intVal	= val;
			m_strVal	= NULL;
			m_isStr		= FALSE;
		}
		modParamValue(LPCWSTR val)
		{
			m_strVal	= NULL;
			m_intVal	= 0;
			m_isStr		= TRUE;
			makeStr(m_strVal, val);
		}
		~modParamValue()
		{
			freeClearStr(m_strVal);
		}
		BOOL isStr() { return m_isStr; }
		BOOL operator==(int val)
		{
			if(m_isStr) return FALSE;
			if(m_intVal == val) return TRUE;
			return FALSE;
		}
		BOOL operator==(LPCWSTR val)
		{
			if(!m_isStr) return FALSE;
			if(!val && !m_strVal) return TRUE;
			if(val && m_strVal)
			{
				if(!StrCmpI(val, m_strVal)) return TRUE;
			}
			return FALSE;
		}
		void operator=(const modParamValue& val)
		{
			makeStr(m_strVal, val.m_strVal);
			m_intVal	= val.m_intVal;
			m_isStr		= val.m_isStr;
		}
	};

	class elementData
	{
	public:
		elementData();
		virtual ~elementData();
	};

	class skin_element
	{
	protected:

		struct elModStates
		{
			elementData*			data;
			TxSkin::elementState	state;
			TxSkin::modParamValue	modVal;
			ULONG_PTR				lParam;
			~elModStates()
			{
				if(data) delete data;
			}
		};

		skin*						m_skin;
		skin_element*				m_parent;
		txArray<skin_element*>		m_elements;
		txArray<elModStates*>		m_states;
		LPWSTR						m_id;
		LPWSTR						m_modParam;
		txArray<sliceElement*>		m_slices;

		// position and size
		BOOL				m_autoCalcWidth;
		int					m_x;
		int					m_y;
		int					m_cx;
		int					m_cy;
		int					m_x_init;
		int					m_y_init;
		int					m_cx_init;
		int					m_cy_init;
		int					m_flex;
		TxSkin::margins		m_margin;
		BOOL				m_float;
		vertical_align		m_vAlign;
		horizontal_align	m_hAlign;
		int					m_minWidth;
		int					m_minHeight;
		size_shink			m_shrink;
		bool				m_mod_by_units;

	public:
		skin_element(skin* skin, LPCWSTR id = NULL, LPCWSTR modParam = NULL);
		virtual ~skin_element();

		virtual void	X(int xVal)			{ m_x = xVal;		}
		virtual int		X()					{ return m_x;		}
		virtual void	Y(int yVal)			{ m_y = yVal;		}
		virtual int		Y()					{ return m_y;		}

		void	min_width(int val)		{ m_minWidth = val;			}
		int		min_width()				{ return m_minWidth;		}

		void	min_height(int val)		{ m_minHeight = val;		}
		int		min_height()			{ return m_minHeight;		}

		void	margin_left(int val)	{ m_margin.left = val;		}
		int		margin_left()			{ return m_margin.left;		}

		void	margin_right(int val)	{ m_margin.right = val;		}
		int		margin_right()			{ return m_margin.right;	}

		void	margin_top(int val)		{ m_margin.top = val;		}
		int		margin_top()			{ return m_margin.top;		}

		void	margin_bottom(int val)	{ m_margin.bottom = val;	}
		int		margin_bottom()			{ return m_margin.bottom;	}

		size_shink	shrink()				{ return m_shrink;			}
		void		shrink(size_shink val)	{ m_shrink = val;			}

		void	flex(int val)			{ m_flex = val;				}
		int		flex()					{ return m_flex;			}

		void	float_el(int val)		{ m_float = val;			}
		int		float_el()				{ return m_float;			}

		void	set_root_size(int cx, int cy);
		void	finish_loading();
		void	reset_positions();

		BOOL	empty()					{ return m_elements.GetCount() ? FALSE : TRUE; }

		void				vAlign(vertical_align val)		{ m_vAlign = val;	}
		vertical_align		vAlign()						{ return m_vAlign;	}

		void				hAlign(horizontal_align val)	{ m_hAlign = val;	}
		horizontal_align	hAlign()						{ return m_hAlign;	}

		virtual void	width(int wVal);
		virtual int		width();

		virtual void	height(int hVal);
		virtual int		height();

		virtual BOOL	isBackground()		{ return FALSE;		}

		LPCWSTR	id()				{ return m_id;		}

		void	getRect(LPRECT rc)	{ rc->left = X(); rc->top = Y(); rc->right = X() + width(); rc->bottom = Y() + height(); }

		BOOL			setState(elementState st, int sliceID);
		elementState	getState(int sliceID);

		void	autoCalcWidth(BOOL calc)			{ m_autoCalcWidth = calc; }
		void	setParent(skin_element* parent)	{ m_parent = parent; }
		void	addElement(skin_element* el);
		void	setCapture(BOOL isCapture, int sliceID);
		BOOL	isCapture(int sliceID);
		BOOL	disableItem(LPCWSTR id, BOOL disable, int sliceID);
		void	addSlice(int sliceID);
		void	deleteSlice(int sliceID);
		void	clearSlices();
		void	addModState(TxSkin::modParamValue& modVal, TxSkin::elementState state, elementData* data, ULONG_PTR lParam = NULL);
		elementData* getModState(int sliceID, ULONG_PTR lParam = NULL);

		skin_element* findElement(int x, int y, int sliceID);

		virtual void	clear();
		virtual void	draw(cairo_t* cr, int left, int top, int sliceID);
		virtual void	init();
		virtual void	apply_scale_factor(double scale);

		virtual BOOL	mouseOver(int x, int y, int sliceID);
		virtual BOOL	mouseLeave(int sliceID);
		virtual BOOL	mouseEnter(int sliceID);
		virtual BOOL	lButtonDown(int x, int y, int sliceID);
		virtual BOOL	lButtonUp(int x, int y, int sliceID);
		virtual	int		calcWidth();
		virtual void	recalcWidth();
		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual void	processXMLState(IXMLDOMNode* node, TxSkin::elementState state, TxSkin::modParamValue& modVal, LPCWSTR baseurl, ULONG_PTR lParam);
		virtual elementData* getStateData(IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam);
		virtual BOOL	isUsedParam(LPCWSTR paramID);
		virtual void	onCapture(BOOL capture, int sliceID);

	protected:
		void					loadXMLStates(IXMLDOMNode* node, LPCWSTR baseurl, TxSkin::elementState state = TxSkin::elStateNormal, TxSkin::modParamValue& modVal = TxSkin::modParamValue((int) 0), ULONG_PTR lParam = NULL);
		TxSkin::brush*			loadBrush(IXMLDOMNode* node);
		TxSkin::pen*			loadPen(IXMLDOMNode* node);

	private:
		sliceElement*	getSlice(int sliceID);
	};
}
