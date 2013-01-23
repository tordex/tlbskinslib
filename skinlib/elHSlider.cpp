#include "TxSkinEngine.h"
#include "elHSlider.h"
#include "skinBase.h"


TxSkin::elHSlider::elHSlider( skin* skin, LPCWSTR param, LPCWSTR maxParam, LPCWSTR id /*= NULL*/, LPCWSTR modParam /*= NULL*/ ) :
			elSliderBase(skin, param, maxParam, id, modParam)
{
}

TxSkin::elHSlider::~elHSlider()
{
}

void TxSkin::elHSlider::draw( cairo_t* cr, int left, int top, int sliceID )
{
	cairo_save(cr);

	HDC hdc = cairo_win32_surface_get_dc(cairo_get_target(cr));

	elSliderData* data = (elSliderData*) getModState(sliceID);
	if(!data) return;

	int thumbWidth = 0;
	if(data->m_imgThumb)
	{
		thumbWidth = data->m_imgThumb->getWidth();
	}

	int x = getThumbX(sliceID, thumbWidth) + left;

	RECT rcBg;
	getRect(&rcBg);
	rcBg.left	+= left;
	rcBg.top	+= top;
	rcBg.right	+= left;
	rcBg.bottom	+= top;

	MARGINS mg;
	mg.cxLeftWidth	= data->m_startMargin;
	mg.cxRightWidth	= data->m_endMargin;
	mg.cyBottomHeight = mg.cyTopHeight = 0;

	if(data->m_background)
	{
		rcBg.bottom = rcBg.top + data->m_background->getHeight();
		CTxSkinDIB skin;
		skin.load(data->m_background, &mg, FALSE, FALSE);
		
		skin.draw(hdc, &rcBg, NULL);
	} else
	{
		if(data->m_bgStart)
		{
			HRGN clip = CreateRectRgn(rcBg.left, rcBg.top, x + thumbWidth / 2, rcBg.bottom);
			SelectClipRgn(hdc, clip);

			CTxSkinDIB skin;
			skin.load(data->m_bgStart, &mg, FALSE, FALSE);
			skin.draw(hdc, &rcBg, NULL);

			SelectClipRgn(hdc, NULL);
			DeleteObject(clip);
		}
		if(data->m_bgEnd)
		{
			HRGN clip = CreateRectRgn(x + thumbWidth / 2, rcBg.top, rcBg.right, rcBg.bottom);
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
		data->m_imgThumb->draw(hdc, x, Y() + top);
	}

	GdiFlush();

	cairo_restore(cr);
}

int TxSkin::elHSlider::getThumbX(int sliceID, int thumbWidth)
{
	int w	= width() - thumbWidth;
	int val = m_skin->getParamInt(m_valParam, sliceID);
	int max_val = m_skin->getParamInt(m_maxValParam, sliceID);
	if(max_val)
	{
		return (X() + w * val / max_val);
	}
	return X();
}

BOOL TxSkin::elHSlider::mouseOver( int x, int y, int sliceID )
{
	if(!isCapture(sliceID))
	{
		return TxSkin::skin_element::mouseOver(x, y, sliceID);
	}
	if(m_skin->getCallback())
	{
		m_skin->getCallback()->OnSliderValue(m_valParam, getNewVal(x, sliceID), sliceID);
	}
	return TRUE;
}

BOOL TxSkin::elHSlider::lButtonDown( int x, int y, int sliceID )
{
	if(m_skin->getCallback())
	{
		m_skin->getCallback()->OnSliderValue(m_valParam, getNewVal(x, sliceID), sliceID);
	}
	return TxSkin::skin_element::lButtonDown(x, y, sliceID);
}

int TxSkin::elHSlider::getNewVal( int x, int sliceID )
{
	elSliderData* data = (elSliderData*) getModState(sliceID);
	int thumbWidth = 0;
	if(data && data->m_imgThumb)
	{
		thumbWidth = data->m_imgThumb->getWidth();
	}

	int w = width() - thumbWidth;
	int maxVal = m_skin->getParamInt(m_maxValParam, sliceID);
	int val = maxVal * (x - thumbWidth / 2 - X()) / w;

	if(val < 0)			val = 0;
	if(val > maxVal)	val = maxVal;
	return val;
}
