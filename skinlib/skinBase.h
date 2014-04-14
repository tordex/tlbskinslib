#pragma once

namespace TxSkin
{
	class parameter;

	enum sliceMode
	{
		sliceByRow,
		sliceByCol
	};

	class skin
	{
		friend class skin_element;

		txArray<parameter*>	m_params;
		txArray<LPWSTR>			m_iconPaths;
		skin_element*				m_base;
		ISkinCallBack*			m_callback;
		txArray<int>			m_slises;
		sliceMode				m_sliceMode;
		int						m_sliceRowCol;

		int						m_elSliceID;
		skin_element*				m_elCapture;

		// cache
		cairo_surface_t*		m_surface;
		cairo_t*				m_cr;
		int						m_cache_width;
		int						m_cache_height;

		skin_fit				m_fit;
		int						m_fit_to_width;
		int						m_fit_to_height;
	public:
		skin(void);
		virtual ~skin(void);

		void clear();
		void clearParams();
		void addParam(LPCWSTR id, int maxValsCount = 0);
		int  width();
		int  height();
		LPCWSTR getOverID(int x, int y);

		skin_element* getBase()	{ return m_base; }

		void	init(int cx = 0, int cy = 0);

		void	draw(HDC hdc, int x, int y);
		int		getParamInt(LPCWSTR paramID, int sliceID);

		parameter* findParam( LPCWSTR paramID );
		BOOL	setParamInt(LPCWSTR paramID, int val, LPCWSTR format = NULL, int sliceID = 0);
		BOOL	addParamValue(LPCWSTR paramID, int val, int sliceID = 0);
		int*	getParamValues(LPCWSTR paramID, int* count, int sliceID = 0);
		LPCWSTR	getParamString(LPCWSTR paramID, int sliceID = 0);
		BOOL	setParamString(LPCWSTR paramID, LPCWSTR val, int sliceID = 0);
		LPCWSTR	getUnitsString(LPCWSTR paramID, int sliceID = 0);
		BOOL	setUnitsString(LPCWSTR paramID, LPCWSTR val, int sliceID = 0);
		int		getUnitsInt(LPCWSTR paramID, int sliceID = 0);
		BOOL	setUnitsInt(LPCWSTR paramID, int val, int sliceID = 0);
		BOOL	mouseOver(int x, int y);
		BOOL	mouseLeave();
		BOOL	lButtonDown(int x, int y);
		BOOL	lButtonUp(int x, int y);
		void	setCallback(ISkinCallBack* callback)	{ m_callback = callback; }
		BOOL	disableItem(LPCWSTR id, BOOL disable, int sliceID = 0);
		void	addSlice(int sliceID);
		void	deleteSlice(int sliceID);
		void	clearSlices();
		void	setSliceIndex(int sliceID, int idx);
		void	setSliceMode(sliceMode mode, int sliceRowCol);
		void	getSliceRect(int sliceIDX, LPRECT rcSlice);
		void	getSliceRectID(int sliceID, LPRECT rcSlice);
		int		getSlicesCount()	{ return m_slises.GetCount(); }
		void	getRowCols(int& cols, int& rows);
		void	addIconPath(LPCWSTR path);
		void	clearIconPath();
		BOOL	findIconPath(LPCWSTR iconFile, LPWSTR path);
		int		findSliceXY(int x, int y, LPRECT rcSlice);
		BOOL	isInCapture()	{ return m_elCapture ? TRUE : FALSE; }
		BOOL	isUsedParam(LPCWSTR paramID);
		BOOL	setParamAutoFit(LPCWSTR paramID, BOOL autoFit, int sliceID = 0);
		BOOL	getParamAutoFit(LPCWSTR paramID, int sliceID = 0);

		ISkinCallBack* getCallback()	{ return m_callback; }

		// skin helper functions

		CTxDIB*			loadIniImage(LPCWSTR section, LPCWSTR key, LPCWSTR iniFile);

		BOOL	load(LPCWSTR xmlFile);
		BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		BOOL	loadText(LPCWSTR text, LPCWSTR baseurl);

		virtual void drawText(HDC hdc, LPCWSTR text, int c, LPRECT rcText, UINT dtParams, UINT flags = 0, HFONT hFont = NULL, COLORREF color = 0, INT vertical = FALSE, COLORREF glowColor = 0, INT glowSize = 0);
		virtual skin_element* createElement(LPCWSTR name, skin* skin);

	protected:
		void	setCapture(skin_element* elCapture, int sliceID);
	private:
		void	init_cache(int w, int h);
		void	clear_cache();
		double	get_scale_factor();
	};
}
