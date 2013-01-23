#include "TxSkinEngine.h"
#include "elImageHRow.h"
#include "skinBase.h"

using namespace TxSkin;


TxSkin::elImageHRow::elImageHRow( skin* skin, LPCWSTR id /*= NULL*/, LPCWSTR modParam /*= NULL*/, imgHRAlign align /*= imgHRAlignLeft*/, TxSkin::imageRowChartType drawType /*= TxSkin::imageChartStretch*/ ) : elImageChartBase(skin, id, modParam)
{
	m_align		= align;
	m_drawType	= drawType;
}
TxSkin::elImageHRow::~elImageHRow( void )
{
}

void TxSkin::elImageHRow::draw( cairo_t* cr, int left, int top, int sliceID )
{
	HDC hdc = cairo_win32_surface_get_dc(cairo_get_target(cr));

	txArray<int> paramValues;
	int maxVal = 0;
	for(int i=0; i < m_values.GetCount(); i++)
	{
		int val = m_skin->getParamInt(m_values[i], sliceID);
		paramValues.Add(&val);
		maxVal	+= val;
	}

	int processedWidth = 0;
	for(int i=0; i < m_values.GetCount(); i++)
	{
		if(paramValues[i])
		{
			int drawWidth = (int) ((double) paramValues[i] / (double) maxVal * (double) width());
			if(processedWidth + drawWidth > width())
			{
				drawWidth = width() - processedWidth;
			}
			if(i == m_values.GetCount() - 1)
			{
				if(processedWidth + drawWidth < width())
				{
					drawWidth = width() - processedWidth;
				}
			}
			if(drawWidth)
			{
				int startX	= 0;
				int endX	= 0;

				if(m_align == imgHRAlignLeft)
				{
					startX = X() + processedWidth;
					endX = startX + drawWidth;
				} else
				{
					startX = X() + width() - processedWidth - drawWidth;
					endX = startX + drawWidth;
				}

				elImageChartData* data = (elImageChartData*) getModState(sliceID, (ULONG_PTR) i);
				if(data && data->m_bmp)
				{
					switch(m_drawType)
					{
					case imageChartTile:
						{
							if(m_align == imgHRAlignLeft)
							{
								if(data->m_bmp->getWidth())
								{
									for(int x = startX; x <= endX - (int) data->m_bmp->getWidth(); x += data->m_bmp->getWidth())
									{
										data->m_bmp->draw(hdc, x + left, Y() + top, data->m_bmp->getWidth(), height());
									}
								}
							} else
							{
								if(data->m_bmp->getWidth())
								{
									for(int x = endX - data->m_bmp->getWidth(); x >= startX; x -= data->m_bmp->getWidth())
									{
										data->m_bmp->draw(hdc, x + left, Y() + top, data->m_bmp->getWidth(), height());
									}
								}
							}
						}
						break;
					case imageChartStretch:
						{
							data->m_bmp->draw(hdc, startX + left, Y() + top, drawWidth, height());
						}
						break;
					case imageChartCrop:
						{
							RECT rcDraw;
							rcDraw.left		= X() + left;
							rcDraw.top		= Y() + top;
							rcDraw.right	= rcDraw.left + width();
							rcDraw.bottom	= rcDraw.top + height();

							HRGN clip = CreateRectRgn(startX + left, Y() + top, startX + left + drawWidth, Y() + top + height());
							SelectClipRgn(hdc, clip);
							CTxSkinDIB skin;
							skin.load(data->m_bmp, &((MARGINS) data->m_margins), FALSE, FALSE);
							skin.draw(hdc, &rcDraw, NULL);
							SelectClipRgn(hdc, NULL);
							DeleteObject(clip);
						}
						break;
					}
				}
			}

			processedWidth += drawWidth;
		}
	}

	GdiFlush();
}

BOOL TxSkin::elImageHRow::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	m_drawType	= (TxSkin::imageRowChartType)	xmlGetAttributeValueSTRArray(node, L"draw", 1, L"tile\0stretch\0crop\0");
	m_align		= (TxSkin::imgHRAlign)			xmlGetAttributeValueSTRArray(node, L"align", 0, L"left\0right\0");
	return elImageChartBase::loadXML(node, baseurl);
}
