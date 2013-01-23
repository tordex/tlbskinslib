#include "TxSkinEngine.h"
#include "elIcon.h"
#include "skinBase.h"

using namespace TxSkin;

TxSkin::elIcon::elIcon( skin* skin, LPCWSTR src /*= NULL*/, LPCWSTR id /*= NULL*/ ) : skin_element(skin, id, NULL)
{
	m_defIcon	= NULL;
	m_srcIcon	= NULL;
	m_baseurl	= NULL;
	makeStr(m_srcIcon, src);
}

TxSkin::elIcon::~elIcon( void )
{
	freeClearStr(m_defIcon);
	freeClearStr(m_srcIcon);
	freeClearStr(m_baseurl);
}

void TxSkin::elIcon::draw( cairo_t* cr, int left, int top, int sliceID )
{
	HDC hdc = cairo_win32_surface_get_dc(cairo_get_target(cr));
	if(m_baseurl && m_srcIcon)
	{
		LPCWSTR icon = m_skin->getParamString(m_srcIcon);
		if(icon)
		{
			WCHAR fileName[MAX_PATH];
			StringCchCopy(fileName, MAX_PATH, m_baseurl);
			PathAddBackslash(fileName);
			StringCchCat(fileName, MAX_PATH, icon);
			if(PathFileExists(fileName))
			{
				CTxDIB bmp(fileName);
				bmp.draw(hdc, X() + left, Y() + top);
			} else
			{
				if(m_skin->findIconPath(icon, fileName))
				{
					CTxDIB bmp(fileName);
					bmp.draw(hdc, X() + left, Y() + top);
				} else 
				{
					StringCchCopy(fileName, MAX_PATH, m_baseurl);
					PathAddBackslash(fileName);
					StringCchCat(fileName, MAX_PATH, m_defIcon);
					if(PathFileExists(fileName))
					{
						CTxDIB bmp(fileName);
						bmp.draw(hdc, X() + left, Y() + top);
					} else if(m_skin->findIconPath(m_defIcon, fileName))
					{
						CTxDIB bmp(fileName);
						bmp.draw(hdc, X() + left, Y() + top);
					}
				}
			}
		}
	}
}

BOOL TxSkin::elIcon::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	BOOL ret = TxSkin::skin_element::loadXML(node, baseurl);

	m_srcIcon	= xmlGetAttributeSTR(node, TEXT("src"));
	m_defIcon	= xmlGetAttributeSTR(node, TEXT("deficon"));
	makeStr(m_baseurl, baseurl);

	return ret;
}
