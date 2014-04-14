#include "TxSkinEngine.h"
#include "skinBase.h"
#include "skinParams.h"
#include "elementBase.h"
#include <shlwapi.h>
#include <ctype.h>
#include <tchar.h>
#include "xmltools.h"
#include "elRoot.h"

using namespace TxSkin;

TxSkin::skin::skin(void)
{
	m_fit			= fit_to_none;
	m_surface		= NULL;
	m_cr			= NULL;
	m_cache_width	= 0;
	m_cache_height	= 0;

	m_base			= new elRoot(this);
	m_callback		= NULL;
	m_sliceMode		= sliceByCol;
	m_sliceRowCol	= 1;
	m_elSliceID		= -1;
	m_elCapture		= NULL;
	addSlice(0);
}

TxSkin::skin::~skin(void)
{
	clear();
	clearParams();
	delete m_base;

	for(int i=0; i < m_iconPaths.GetCount(); i++)
	{
		if(m_iconPaths[i])	delete m_iconPaths[i];
	}
	m_iconPaths.Clear();
	clear_cache();
}

void TxSkin::skin::addSlice( int sliceID )
{
	for(int i=0; i < m_slises.GetCount(); i++)
	{
		if(m_slises[i] == sliceID)
		{
			return;
		}
	}
	m_slises.Add(&sliceID);
	m_base->addSlice(sliceID);
	for(int i=0; i < m_params.GetCount(); i++)
	{
		m_params[i]->addSlice(sliceID);
	}
}

void skin::clearParams()
{
	for(int i=0; i < m_params.GetCount(); i++)
	{
		delete m_params[i];
	}
	m_params.Clear();
}

void skin::init(int cx, int cy)
{
	m_fit_to_width	= cx;
	m_fit_to_height	= cy;
	m_base->set_root_size(cx, cy);
	m_base->apply_scale_factor(get_scale_factor());
	m_base->init();
}

void skin::draw( HDC hdc, int x, int y )
{
	init_cache(width(), height());

	for(int i=0; i < m_slises.GetCount(); i++)
	{
		RECT rcSlice;
		getSliceRect(i, &rcSlice);
		m_base->draw(m_cr, rcSlice.left, rcSlice.top, m_slises[i]);
	}

	HDC src = cairo_win32_surface_get_dc(m_surface);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;

	AlphaBlend(hdc, x, y,
		width(), height(), src,
		0, 0,
		width(), height(),
		bf);
}

int skin::getParamInt( LPCWSTR paramID, int sliceID )
{
	TxSkin::parameter* param = findParam(paramID);
	if(param)
	{
		return param->getIntVal(sliceID);
	} else if(paramID)
	{
		return _wtoi(paramID);
	}
	return 0;
}

LPCWSTR skin::getParamString( LPCWSTR paramID, int sliceID )
{
	TxSkin::parameter* param = findParam(paramID);
	if(param)
	{
		return param->getStringVal(sliceID);
	}
	return paramID;
}

BOOL skin::setParamInt( LPCWSTR paramID, int val, LPCWSTR format, int sliceID )
{
	TxSkin::parameter* param = findParam(paramID);
	if(param)
	{
		if(param->getIntVal(sliceID) != val || format && !param->getStringVal(sliceID))
		{
			param->setFormatedVal(val, format, sliceID);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL skin::setParamString( LPCWSTR paramID, LPCWSTR val, int sliceID )
{
	TxSkin::parameter* param = findParam(paramID);
	if(param)
	{
		LPCWSTR oldVal = param->getStringVal(sliceID);
		if(val && !oldVal || !val && oldVal ||
		   val && oldVal && lstrcmp(val, oldVal))
		{
			param->setStringVal(val, sliceID);
			return TRUE;
		}
	}
	return FALSE;
}

void skin::addParam( LPCWSTR id, int maxValsCount )
{
	TxSkin::parameter* p = new TxSkin::parameter(id, maxValsCount);
	m_params.Add(&p);
}

BOOL skin::mouseOver( int x, int y )
{
	RECT rcSlice;
	int sliceID = findSliceXY(x, y, &rcSlice);
	BOOL ret = FALSE;
	if(!m_elCapture)
	{
		if(m_elSliceID != sliceID)
		{
			m_base->mouseLeave(m_elSliceID);
			m_elSliceID = sliceID;
		}
		ret = m_base->mouseOver(x - rcSlice.left, y - rcSlice.top, sliceID);
	} else
	{
		getSliceRectID(m_elSliceID, &rcSlice);
		ret = m_elCapture->mouseOver(x - rcSlice.left, y - rcSlice.top, m_elSliceID);
	}

	return ret;
}

BOOL skin::lButtonDown( int x, int y )
{
	RECT rcSlice;
	int sliceID = findSliceXY(x, y, &rcSlice);

	skin_element* elOver = m_base->findElement(x - rcSlice.left, y - rcSlice.top, sliceID);
	if(elOver)
	{
		return elOver->lButtonDown(x - rcSlice.left, y - rcSlice.top, sliceID);
	}
	return FALSE;
}

BOOL skin::lButtonUp( int x, int y )
{
	BOOL ret = FALSE;
	RECT rcSlice;
	if(m_elCapture)
	{
		getSliceRectID(m_elSliceID, &rcSlice);
		ret = m_elCapture->lButtonUp(x, y, m_elSliceID);
	} else
	{
		int sliceID = findSliceXY(x - rcSlice.left, y - rcSlice.top, &rcSlice);
		ret = m_base->lButtonUp(x - rcSlice.left, y - rcSlice.top, sliceID);
	}
	return ret;
}

BOOL skin::mouseLeave()
{
	return m_base->mouseLeave(m_elSliceID);
}

void skin::setCapture( skin_element* elCapture, int sliceID )
{
	m_elCapture = elCapture;
	m_elSliceID	= sliceID;
}

BOOL skin::disableItem( LPCWSTR id, BOOL disable, int sliceID )
{
	return m_base->disableItem(id, disable, sliceID);
}

BOOL skin::addParamValue( LPCWSTR paramID, int val, int sliceID )
{
	TxSkin::parameter* param = findParam(paramID);
	if(param)
	{
		param->addValue(val, sliceID);
		return TRUE;
	}
	return FALSE;
}

int* skin::getParamValues( LPCWSTR paramID, int* count, int sliceID )
{
	*count = 0;
	TxSkin::parameter* param = findParam(paramID);
	if(param)
	{
		return param->getValues(count, sliceID);
	}
	return NULL;
}

TxSkin::parameter* skin::findParam( LPCWSTR paramID )
{
	for(int i=0; i < m_params.GetCount(); i++)
	{
		if(!lstrcmp(paramID, m_params[i]->getID()))
		{
			return m_params[i];
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

void TxSkin::skin::setSliceMode( sliceMode mode, int sliceRowCol )
{
	m_sliceMode		= mode;
	m_sliceRowCol	= sliceRowCol;
	if(m_sliceRowCol <= 0) 
	{
		m_sliceRowCol = 1;
	}
}

int TxSkin::skin::width()
{
	int rows = 1;
	int cols = 1;
	getRowCols(cols, rows);

	return m_base->width() * cols;
}

int TxSkin::skin::height()
{
	int rows = 1;
	int cols = 1;
	getRowCols(cols, rows);

	return m_base->height() * rows;
}

void TxSkin::skin::getRowCols( int& cols, int& rows )
{
	if(m_sliceMode == sliceByRow)
	{
		rows = m_sliceRowCol;
		cols = m_slises.GetCount() / m_sliceRowCol;
		if(m_slises.GetCount() % m_sliceRowCol)
		{
			cols++;
		}
		if(rows > m_slises.GetCount())
		{
			rows = m_slises.GetCount();
		}
	} else
	{
		cols = m_sliceRowCol;
		rows = m_slises.GetCount() / m_sliceRowCol;
		if(m_slises.GetCount() % m_sliceRowCol)
		{
			rows++;
		}
		if(cols > m_slises.GetCount())
		{
			cols = m_slises.GetCount();
		}
	}
}

int TxSkin::skin::findSliceXY( int x, int y, LPRECT rcSlice )
{
	int sliceIDX = -1;

	if(m_base->width() && m_base->height())
	{
		int cols = 0;
		int rows = 0;
		getRowCols(cols, rows);
		int col = x / m_base->width();
		int row = y / m_base->height();

		if(m_sliceMode == sliceByRow)
		{
			sliceIDX = col * rows + row;
		} else
		{
			sliceIDX = row * cols + col;
		}
		if(sliceIDX >= 0 && sliceIDX < m_slises.GetCount())
		{
			getSliceRect(sliceIDX, rcSlice);
			return m_slises[sliceIDX];
		}
	}
	return -1;
}

void TxSkin::skin::getSliceRect( int sliceIDX, LPRECT rcSlice )
{
	if(!rcSlice) return;

	int cols = 0;
	int rows = 0;
	getRowCols(cols, rows);
	int col = -1;
	int row = -1;
	if(m_sliceMode == sliceByRow)
	{
		col = sliceIDX / rows;
		row = sliceIDX - col * rows;
	} else
	{
		row = sliceIDX / cols;
		col = sliceIDX - row * cols;
	}
	rcSlice->left	= col * m_base->width();
	rcSlice->top	= row * m_base->height();
	rcSlice->right	= rcSlice->left + m_base->width();
	rcSlice->bottom	= rcSlice->top  + m_base->height();
}

void TxSkin::skin::getSliceRectID( int sliceID, LPRECT rcSlice )
{
	for(int i=0; i < m_slises.GetCount(); i++)
	{
		if(m_slises[i] == sliceID)
		{
			getSliceRect(i, rcSlice);
			break;
		}
	}
}

void TxSkin::skin::deleteSlice( int sliceID )
{
	if(m_elSliceID == sliceID)
	{
		setCapture(NULL, sliceID);
	}
	for(int i=0; i < m_slises.GetCount(); i++)
	{
		if(m_slises[i] == sliceID)
		{
			m_slises.Delete(i);
			break;
		}
	}
	m_base->deleteSlice(sliceID);
	for(int i=0; i < m_params.GetCount(); i++)
	{
		m_params[i]->deleteSlice(sliceID);
	}
}

void TxSkin::skin::clear()
{
	m_fit			= fit_to_none;
	m_fit_to_width	= 0;
	m_fit_to_height	= 0;
	m_elCapture		= NULL;
	m_elSliceID		= -1;
	delete m_base;
	m_base = new elRoot(this);
}

CTxDIB* TxSkin::skin::loadIniImage( LPCWSTR section, LPCWSTR key, LPCWSTR iniFile )
{
	TCHAR skinPath[MAX_PATH];
	lstrcpy(skinPath, iniFile);
	PathRemoveFileSpec(skinPath);
	PathAddBackslash(skinPath);

	TCHAR str[MAX_PATH];
	TCHAR flName[MAX_PATH];
	GetPrivateProfileString(section, key, TEXT(""), str, MAX_PATH, iniFile);
	if(str[0])
	{
		lstrcpy(flName, skinPath);
		lstrcat(flName, str);
		return new CTxDIB(flName);
	}
	return NULL;
}

void TxSkin::skin::setSliceIndex( int sliceID, int idx )
{
	for(int i=0; i < m_slises.GetCount(); i++)
	{
		if(m_slises[i] == sliceID)
		{
			if(i != idx)
			{
				int v = m_slises[idx];
				m_slises[idx]	= m_slises[i];
				m_slises[i]		= v;
			}
			break;
		}
	}
}

LPCWSTR TxSkin::skin::getOverID(int x, int y)
{
	RECT rcSlice;
	int sliceID = findSliceXY(x, y, &rcSlice);
	BOOL ret = FALSE;

	skin_element* overItem = m_base->findElement(x - rcSlice.left, y - rcSlice.top, sliceID);
	if(overItem)
	{
		return overItem->id();
	}

	return NULL;
}

BOOL TxSkin::skin::load( LPCWSTR xmlFile )
{
	if(!xmlFile) return FALSE;

	WCHAR baseURL[MAX_PATH];
	StringCchCopy(baseURL, MAX_PATH, xmlFile);
	PathRemoveFileSpec(baseURL);
	PathAddBackslash(baseURL);


	BOOL ret = FALSE;
	CComPtr<IXMLDOMDocument> doc;
	doc.CoCreateInstance(CLSID_DOMFreeThreadedDocument, NULL, CLSCTX_INPROC_SERVER);
	doc->put_async(VARIANT_FALSE);
	doc->put_validateOnParse(VARIANT_FALSE);
	doc->put_resolveExternals(VARIANT_FALSE);
	VARIANT_BOOL bSuccess = VARIANT_FALSE;

	if(doc->load(CComVariant(xmlFile), &bSuccess) == S_OK && bSuccess == VARIANT_TRUE)
	{
		IXMLDOMNode* node = xmlOpenNode(doc, L"skin");
		if(node)
		{
			ret = loadXML(node, baseURL);
			node->Release();
		}
	}
	return ret;
}

BOOL TxSkin::skin::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	clear();

	m_fit = (skin_fit) xmlGetAttributeValueSTRArray(node, L"fit", fit_to_none, L"none\0width\0height\0min\0max\0");

	return m_base->loadXML(node, baseurl);
}

void TxSkin::skin::drawText( HDC hdc, LPCWSTR text, int c, LPRECT rcText, UINT dtParams, UINT flags /*= 0*/, HFONT hFont /*= NULL*/, COLORREF color /*= 0*/, INT vertical /*= FALSE*/, COLORREF glowColor /*= 0*/, INT glowSize /*= 0*/ )
{

}

void TxSkin::skin::addIconPath( LPCWSTR path )
{
	LPWSTR str = NULL;
	makeStr(str, path);
	m_iconPaths.Add(&str);
}

BOOL TxSkin::skin::findIconPath( LPCWSTR iconFile, LPWSTR path )
{
	BOOL ret = FALSE;
	for(int i = 0; i < m_iconPaths.GetCount(); i++)
	{
		WCHAR flName[MAX_PATH];
		StringCchCopy(flName, MAX_PATH, m_iconPaths[i]);
		PathAddBackslash(flName);
		StringCchCat(flName, MAX_PATH, iconFile);
		if(PathFileExists(flName))
		{
			ret = TRUE;
			StringCchCopy(path, MAX_PATH, flName);
			break;
		}
	}
	return ret;
}

void TxSkin::skin::clearIconPath()
{
	for(int i=0; i < m_iconPaths.GetCount(); i++)
	{
		if(m_iconPaths[i])	delete m_iconPaths[i];
	}
	m_iconPaths.Clear();
}

void TxSkin::skin::clearSlices()
{
	setCapture(NULL, 0);
	m_slises.Clear();
	m_base->clearSlices();
	for(int i=0; i < m_params.GetCount(); i++)
	{
		m_params[i]->clearSlices();
	}
}

BOOL TxSkin::skin::isUsedParam( LPCWSTR paramID )
{
	return m_base->isUsedParam(paramID);
}

BOOL TxSkin::skin::loadText( LPCWSTR text, LPCWSTR baseurl )
{
	if(!text) return FALSE;

	BOOL ret = FALSE;
	CComPtr<IXMLDOMDocument> doc;
	doc.CoCreateInstance(CLSID_DOMFreeThreadedDocument, NULL, CLSCTX_INPROC_SERVER);
	doc->put_async(VARIANT_FALSE);
	doc->put_validateOnParse(VARIANT_FALSE);
	doc->put_resolveExternals(VARIANT_FALSE);
	VARIANT_BOOL bSuccess = VARIANT_FALSE;

	if(doc->loadXML(CComBSTR(text), &bSuccess) == S_OK && bSuccess == VARIANT_TRUE)
	{
		IXMLDOMNode* node = xmlOpenNode(doc, L"skin");
		if(node)
		{
			ret = loadXML(node, baseurl);
			node->Release();
		}
	}
	return ret;
}

void TxSkin::skin::clear_cache()
{
	if(m_cr)		cairo_destroy(m_cr);
	if(m_surface)	cairo_surface_destroy(m_surface);
	m_cr			= NULL;
	m_surface		= NULL;
	m_cache_width	= 0;
	m_cache_height	= 0;
}

void TxSkin::skin::init_cache( int w, int h )
{
	if(m_cr && m_surface)
	{
		if(m_cache_width == w && m_cache_height == h)
		{
			cairo_save (m_cr);
			cairo_set_source_rgba (m_cr, 0, 0, 0, 0);
			cairo_set_operator (m_cr, CAIRO_OPERATOR_SOURCE);
			cairo_paint (m_cr);
			cairo_restore (m_cr);

			HDC hdc = cairo_win32_surface_get_dc(m_surface);
			RECT rcDraw;
			rcDraw.left		= 0;
			rcDraw.top		= 0;
			rcDraw.bottom	= h;
			rcDraw.right	= w;
			FillRect(hdc, &rcDraw, (HBRUSH) GetStockObject(BLACK_BRUSH));
			return;
		}
	}
	clear_cache();

	m_surface = cairo_win32_surface_create_with_dib(CAIRO_FORMAT_ARGB32, width(), height());
	m_cr = cairo_create (m_surface);

	m_cache_width	= w;
	m_cache_height	= h;
}

double TxSkin::skin::get_scale_factor()
{
	double scale = 0;
	switch(m_fit)
	{
	case fit_to_width:
		if(width() != m_fit_to_width && width())
		{
			scale = (double) m_fit_to_width / (double) width();
		}
		break;
	case fit_to_height:
		if(height() != m_fit_to_height && height())
		{
			scale = (double) m_fit_to_height / (double) height();
		}
		break;
	}
	return scale;
}

LPCWSTR TxSkin::skin::getUnitsString( LPCWSTR paramID, int sliceID /*= 0*/ )
{
	TxSkin::parameter* param = findParam(paramID);
	if(param)
	{
		return param->getStringUnits(sliceID);
	}
	return NULL;
}

BOOL TxSkin::skin::setUnitsString( LPCWSTR paramID, LPCWSTR val, int sliceID /*= 0*/ )
{
	TxSkin::parameter* param = findParam(paramID);
	if(param)
	{
		LPCWSTR oldVal = param->getStringUnits(sliceID);
		if(val && !oldVal || !val && oldVal ||
			val && oldVal && lstrcmp(val, oldVal))
		{
			param->setStringUnits(val, sliceID);
			return TRUE;
		}
	}
	return FALSE;
}

int TxSkin::skin::getUnitsInt( LPCWSTR paramID, int sliceID /*= 0*/ )
{
	TxSkin::parameter* param = findParam(paramID);
	if(param)
	{
		return param->getIntUnits(sliceID);
	}
	return 0;
}

BOOL TxSkin::skin::setUnitsInt( LPCWSTR paramID, int val, int sliceID /*= 0*/ )
{
	TxSkin::parameter* param = findParam(paramID);
	if(param)
	{
		int oldVal = param->getIntUnits(sliceID);
		if(val != oldVal)
		{
			param->setIntUnits(val, sliceID);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL TxSkin::skin::setParamAutoFit( LPCWSTR paramID, BOOL autoFit, int sliceID )
{
	TxSkin::parameter* param = findParam(paramID);
	if(param)
	{
		return param->setAutoFit(autoFit, sliceID);
	}
	return FALSE;
}

BOOL TxSkin::skin::getParamAutoFit( LPCWSTR paramID, int sliceID /*= 0*/ )
{
	TxSkin::parameter* param = findParam(paramID);
	if(param)
	{
		return param->getAutoFit(sliceID);
	}
	return FALSE;
}

