#pragma once
#include "elementBase.h"

namespace TxSkin
{
	class elTileImageHistogram : public skin_element
	{
		LPWSTR						m_param;
		LPWSTR						m_maxValParam;
		CTxDIB*						m_img;
		TxSkin::histogramStart		m_startFrom;
		TxSkin::histogramDir		m_dir;
	public:
		elTileImageHistogram(	skin* skin, 
								LPCWSTR param						= NULL, 
								LPCWSTR maxValParam					= NULL, 
								TxSkin::histogramStart startFrom	= TxSkin::startFromTop, 
								TxSkin::histogramDir dir			= TxSkin::dirPositive, 
								CTxDIB* img							= NULL, 
								LPCWSTR id							= NULL);
		~elTileImageHistogram();

		virtual void	draw(cairo_t* cr, int left, int top, int sliceID);
		virtual BOOL	isUsedParam(LPCWSTR paramID);
		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
	};

	class elStretchImageHistogram : public skin_element
	{
		LPWSTR						m_param;
		LPWSTR						m_maxValParam;
		CTxDIB*						m_img;
		TxSkin::histogramStart		m_startFrom;
		TxSkin::histogramDir		m_dir;
		MARGINS						m_margins;
	public:
		elStretchImageHistogram(	skin* skin, 
									LPCWSTR param							= NULL, 
									LPCWSTR maxValParam						= NULL, 
									TxSkin::histogramStart startFrom		= TxSkin::startFromTop, 
									TxSkin::histogramDir dir				= TxSkin::dirPositive,
									CTxDIB* img								= NULL, 
									const TxSkin::margins* imageMargins		= NULL, 
									LPCWSTR id								= NULL);
		~elStretchImageHistogram();

		virtual void	draw(cairo_t* cr, int left, int top, int sliceID);
		virtual BOOL	isUsedParam(LPCWSTR paramID);
		virtual BOOL	loadXML(IXMLDOMNode* node, LPCWSTR baseurl);
	};

	inline RECT make_rect(int x, int y, int w, int h)
	{
		RECT rc;
		rc.left		= x;
		rc.top		= y;
		rc.right	= x + w;
		rc.bottom	= y + h;
		return rc;
	}
}
