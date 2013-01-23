#include "..\tlbskinslib.h"
#include "skinBase.h"

using namespace TxSkin;

skin_element* TxSkin::skin::createElement( LPCWSTR name, skin* skin )
{
	skin_element* el = NULL;
	if(!lstrcmp(name, L"image"))
	{
		el = new elImage(skin);
	} else if(!lstrcmp(name, L"text"))
	{
		el = new elText(skin);
	} else if(!lstrcmp(name, L"colorHistogram") || !lstrcmp(name, L"histogram-color"))
	{
		el = new elColorHistogram(skin);
	} else if(!lstrcmp(name, L"rect"))
	{
		el = new elColorRect(skin);
	} else if(!lstrcmp(name, L"formatedText") || !lstrcmp(name, L"text-formated"))
	{
		el = new elFormatedText(skin);
	} else if(!lstrcmp(name, L"hBox") || !lstrcmp(name, L"h-box"))
	{
		el = new elHBOX(skin);
	} else if(!lstrcmp(name, L"hRowChart") || !lstrcmp(name, L"chart-h-row"))
	{
		el = new elHRowChart(skin);
	} else if(!lstrcmp(name, L"hSlider") || !lstrcmp(name, L"h-slider"))
	{
		el = new elHSlider(skin);
	} else if(!lstrcmp(name, L"vSlider") || !lstrcmpi(name, L"v-slider"))
	{
		el = new elVSlider(skin);
	} else if(!lstrcmp(name, L"imageHRow") || !lstrcmp(name, L"chart-image-h-row"))
	{
		el = new elImageHRow(skin);
	} else if(!lstrcmp(name, L"imagePieChart") || !lstrcmp(name, L"chart-image-pie"))
	{
		el = new elImagePieChart(skin);
	} else if(!lstrcmp(name, L"imageSetChart") || !lstrcmp(name, L"chart-image-set"))
	{
		el = new elImageSetChart(skin);
	} else if(!lstrcmp(name, L"imageText") || !lstrcmp(name, L"text-image"))
	{
		el = new elImageText(skin);
	} else if(!lstrcmp(name, L"imageVRow") || !lstrcmp(name, L"chart-image-v-row"))
	{
		el = new elImageVRow(skin);
	} else if(!lstrcmp(name, L"label"))
	{
		el = new elLabel(skin);
	} else if(!lstrcmp(name, L"pieChart") || !lstrcmp(name, L"chart-pie"))
	{
		el = new elPieChart(skin);
	} else if(!lstrcmp(name, L"rotateImage") || !lstrcmp(name, L"image-rotate"))
	{
		el = new elRotateImage(skin);
	} else if(!lstrcmp(name, L"stretchImageHistogram") || !lstrcmp(name, L"histogram-image-stretch"))
	{
		el = new elStretchImageHistogram(skin);
	} else if(!lstrcmp(name, L"tileImageHistogram") || !lstrcmp(name, L"histogram-image-tile"))
	{
		el = new elTileImageHistogram(skin);
	} else if(!lstrcmp(name, L"vBox") || !lstrcmp(name, L"v-box"))
	{
		el = new elVBOX(skin);
	} else if(!lstrcmp(name, L"vRowChart") || !lstrcmp(name, L"chart-v-row"))
	{
		el = new elVRowChart(skin);
	} else if(!lstrcmp(name, L"icon"))
	{
		el = new elIcon(skin);
	}

	return el;
}
