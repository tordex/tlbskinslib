#pragma once
#include "elementBase.h"

namespace TxSkin
{
	class elColorHistogram : public skin_element
	{
		LPWSTR						m_param;
		LPWSTR						m_maxValParam;
		TxSkin::brush*				m_brush;
		TxSkin::pen*				m_pen;
		int							m_step;
		TxSkin::histogramStart		m_startFrom;
		TxSkin::histogramDir		m_dir;
	public:
		elColorHistogram(	skin* skin, 
							LPCWSTR param						= NULL, 
							LPCWSTR maxValParam					= NULL, 
							TxSkin::histogramStart startFrom	= TxSkin::startFromTop, 
							TxSkin::histogramDir dir			= TxSkin::dirPositive, 
							TxSkin::brush* brush				= NULL, 
							TxSkin::pen* pen					= NULL, 
							int step							= 1, 
							LPCWSTR id							= NULL);
		~elColorHistogram();

		virtual void draw(cairo_t* cr, int left, int top, int sliceID);
		virtual BOOL loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
		virtual BOOL isUsedParam(LPCWSTR paramID);

	private:
		void draw_h_histogram(cairo_t* cr, int left, int top, int* data, int count, int max_val);
		void draw_v_histogram(cairo_t* cr, int left, int top, int* data, int count, int max_val);
	};
}
