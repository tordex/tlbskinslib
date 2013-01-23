#include "TxSkinEngine.h"
#include "elVSlider.h"
#include "skinBase.h"


TxSkin::elVSlider::elVSlider( skin* skin, LPCWSTR param, LPCWSTR maxParam, LPCWSTR id /*= NULL*/, LPCWSTR modParam /*= NULL*/ ) :
			elSliderBase(skin, param, maxParam, id, modParam)
{
}

TxSkin::elVSlider::~elVSlider()
{
}

void TxSkin::elVSlider::draw( cairo_t* cr, int left, int top, int sliceID )
{
	HDC hdc = cairo_win32_surface_get_dc(cairo_get_target(cr));
	
	elSliderData* data = (elSliderData*) getModState(sliceID);
	if(!data) return;

	int thumbHeight = 0;
	if(data->m_imgThumb)
	{
		thumbHeight = data->m_imgThumb->getHeight();
	}

	RECT rcBg;
	getRect(&rcBg);

	rcBg.left	+= left;
	rcBg.right	+= left;
	rcBg.top	+= top;
	rcBg.bottom	+= top;

	int y = getThumbY(sliceID, thumbHeight) + top;

	MARGINS mg;
	mg.cxLeftWidth	= 0;
	mg.cxRightWidth	= 0;
	mg.cyTopHeight	= data->m_startMargin;
	mg.cyBottomHeight	= data->m_endMargin;

	if(data->m_background)
	{
		CTxSkinDIB skin;
		skin.load(data->m_background, &mg, FALSE, FALSE);
		skin.draw(hdc, &rcBg, NULL);
	} else
	{
		TxSkin::margins rcMargins(0, 0, data->m_startMargin, data->m_endMargin);

		if(data->m_bgStart)
		{
			HRGN clip = CreateRectRgn(rcBg.left, y + thumbHeight / 2, rcBg.right, rcBg.bottom);
			SelectClipRgn(hdc, clip);

			CTxSkinDIB skin;
			skin.load(data->m_bgStart, &mg, FALSE, FALSE);
			skin.draw(hdc, &rcBg, NULL);

			SelectClipRgn(hdc, NULL);
			DeleteObject(clip);
		}
		if(data->m_bgEnd)
		{
			HRGN clip = CreateRectRgn(rcBg.left, rcBg.top, rcBg.right, (y + thumbHeight / 2));
			SelectClipRgn(hdc, clip);

			CTxSkinDIB skin;
			skin.load(data->m_bgEnd, &mg, FALSE, FALSE);
			skin.draw(hdc, &rcBg, NULL);

			SelectClipRgn(hdc, NULL);
			DeleteObject(clip);
		}
	}


	if(data->m_imgThumb)
	{
		data->m_imgThumb->draw(hdc, X() + left, y);
	}
}

int TxSkin::elVSlider::getThumbY(int sliceID, int thumbHeight)
{
	int h	= height() - thumbHeight;
	int val = m_skin->getParamInt(m_valParam, sliceID);
	return Y() + height() - thumbHeight - h * val / m_skin->getParamInt(m_maxValParam, sliceID);
}

BOOL TxSkin::elVSlider::mouseOver( int x, int y, int sliceID )
{
	if(!isCapture(sliceID))
	{
		return TxSkin::skin_element::mouseOver(x, y, sliceID);
	}
	if(m_skin->getCallback())
	{
		m_skin->getCallback()->OnSliderValue(m_valParam, getNewVal(y, sliceID), sliceID);
	}
	return TRUE;
}

BOOL TxSkin::elVSlider::lButtonDown( int x, int y, int sliceID )
{
	if(m_skin->getCallback())
	{
		m_skin->getCallback()->OnSliderValue(m_valParam, getNewVal(y, sliceID), sliceID);
	}
	return TxSkin::skin_element::lButtonDown(x, y, sliceID);
}

int TxSkin::elVSlider::getNewVal( int y, int sliceID )
{
	elSliderData* data = (elSliderData*) getModState(sliceID);
	int thumbHeight = 0;
	if(data && data->m_imgThumb)
	{
		thumbHeight = data->m_imgThumb->getHeight();
	}

	int h = height() - thumbHeight;
	int maxVal = m_skin->getParamInt(m_maxValParam, sliceID);
	int val = (int) ((double) maxVal * (double) (Y() + height() - thumbHeight - y + thumbHeight / 2.0) / (double) h);

	if(val < 0)			val = 0;
	if(val > maxVal)	val = maxVal;
	return val;
}
