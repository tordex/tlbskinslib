#include "TxSkinEngine.h"
#include "elImageVRow.h"
#include "skinBase.h"

using namespace TxSkin;


TxSkin::elImageVRow::elImageVRow( skin* skin, LPCWSTR id /*= NULL*/, LPCWSTR modParam /*= NULL*/, imgVRAlign align /*= imgVRAlignBottom*/, imageRowChartType drawType /*= imageChartTile*/ ) : elImageChartBase(skin, id, modParam)
{
	m_align		= align;
	m_drawType	= drawType;
}

TxSkin::elImageVRow::~elImageVRow( void )
{
}

void TxSkin::elImageVRow::draw( cairo_t* cr, int left, int top, int sliceID )
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

	int processedHeight = 0;
	for(int i=0; i < m_values.GetCount(); i++)
	{
		if(paramValues[i])
		{
			int drawHeight = (int) ((double) paramValues[i] / (double) maxVal * (double) height());
			if(processedHeight + drawHeight > height())
			{
				drawHeight = height() - processedHeight;
			}
			if(i == m_values.GetCount() - 1)
			{
				if(processedHeight + drawHeight < height())
				{
					drawHeight = height() - processedHeight;
				}
			}
			if(drawHeight)
			{
				int startY	= 0;
				int endY	= 0;

				if(m_align == imgVRAlignTop)
				{
					startY = Y() + processedHeight;
					endY = startY + drawHeight;
				} else
				{
					startY = Y() + height() - processedHeight - drawHeight;
					endY = startY + drawHeight;
				}

				elImageChartData* data = (elImageChartData*) getModState(sliceID, (ULONG_PTR) i);
				if(data && data->m_bmp)
				{
					switch(m_drawType)
					{
					case imageChartTile:
						{
							if(m_align == imgVRAlignTop)
							{
								if(data->m_bmp->getHeight())
								{
									for(int y = startY; y <= endY - (int) data->m_bmp->getHeight(); y += data->m_bmp->getHeight())
									{
										data->m_bmp->draw(hdc, X() + left, y + top, width(), data->m_bmp->getHeight());
									}
								}
							} else
							{
								if(data->m_bmp->getHeight())
								{
									for(int y = endY - data->m_bmp->getHeight(); y >= startY; y -= data->m_bmp->getHeight())
									{
										data->m_bmp->draw(hdc, X() + left, y + top, width(), data->m_bmp->getHeight());
									}
								}
							}
						}
						break;
					case imageChartStretch:
						{
							data->m_bmp->draw(hdc, X() + left, startY + top, width(), drawHeight);
						}
						break;
					case imageChartCrop:
						{
							RECT rcDraw;
							rcDraw.left		= X() + left;
							rcDraw.top		= Y() + top;
							rcDraw.right	= rcDraw.left + width();
							rcDraw.bottom	= rcDraw.top + height();

							HRGN clip = CreateRectRgn(X() + left, startY + top, X() + left + width(), startY + top + drawHeight);
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

			processedHeight += drawHeight;
		}
	}

	GdiFlush();
}

BOOL TxSkin::elImageVRow::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	m_drawType	= (TxSkin::imageRowChartType)	xmlGetAttributeValueSTRArray(node, L"draw", 1, L"tile\0stretch\0crop\0");
	m_align		= (TxSkin::imgVRAlign)			xmlGetAttributeValueSTRArray(node, L"align", 0, L"bottom\0top\0");
	return elImageChartBase::loadXML(node, baseurl);
}
