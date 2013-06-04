#include "TxSkinEngine.h"
#include "elImageText.h"
#include "skinBase.h"

using namespace TxSkin;

elImageText::elImageText(skin* skin, LPCWSTR paramID, LPCWSTR id) : skin_element(skin, id)
{
	m_units		= NULL;
	m_paramID	= NULL;
	makeStr(m_paramID,	paramID);
}

elImageText::~elImageText(void)
{
	freeClearStr(m_paramID);
	for(int i=0; i < m_alphabets.GetCount(); i++)
	{
		delete m_alphabets[i];
	}
	m_alphabets.Clear();
	if(m_units)
	{
		delete m_units;
	}
}

void TxSkin::elImageText::addAlphabetImage( LPCWSTR alphabet, CTxDIB* img, BOOL isVertical, elementState state, TxSkin::modParamValue& modVal )
{
	if(alphabet)
	{
		int abIDX = -1;
		for(int i = 0; i < m_alphabets.GetCount(); i++)
		{
			if(!lstrcmp(m_alphabets[i], alphabet))
			{
				abIDX = i;
				break;
			}
		}
		if(abIDX < 0)
		{
			LPWSTR newAlphabet = NULL;
			makeStr(newAlphabet, alphabet);
			m_alphabets.Add(&newAlphabet);
			abIDX = m_alphabets.GetCount() - 1;
		}

		int rows = 1;
		int cols = 1;
		if(isVertical)
		{
			rows = lstrlen(alphabet);
		} else
		{
			cols = lstrlen(alphabet);
		}

		addModState(modVal, state, new elImageTextData(new CTxDibSet(img, rows, cols), isVertical), (ULONG_PTR) abIDX);
	}
}

void TxSkin::elImageText::draw( cairo_t* cr, int left, int top, int sliceID )
{
	HDC hdc = cairo_win32_surface_get_dc(cairo_get_target(cr));

	LPCWSTR val = m_skin->getParamString(m_paramID, sliceID);
	if(val && val[0])
	{
		elImageTextUnitsData* units = NULL;
		if(m_units)
		{
			units = (elImageTextUnitsData*) m_units->getModState(sliceID);
		}

		int x = X();

		switch(m_align)
		{
		case text_halign_right:
			{
				int txt_width = get_text_width(val, sliceID);
				if(units && units->m_bmp)
				{
					txt_width += units->m_bmp->getWidth();
				}
				x = X() + width() - txt_width;
			}
			break;
		case text_halign_center:
			{
				int txt_width = get_text_width(val, sliceID);
				if(units && units->m_bmp)
				{
					txt_width += units->m_bmp->getWidth();
				}
				x = X() + width() / 2 - txt_width / 2;
			}
			break;
		}

		if(units)
		{
			if(!m_units->position())
			{
				units->m_bmp->draw(hdc, x + left, Y() + top);
				x += units->m_bmp->getWidth();
			}
		}
		for(int i = 0; val[i]; i++)
		{
			int abIDX = -1;
			int chrIDX = -1;
			for(int j = 0; j < m_alphabets.GetCount(); j++)
			{
				LPWSTR str = wcschr(m_alphabets[j], val[i]);
				if(str)
				{
					abIDX	= j;
					chrIDX	= (int) (str - m_alphabets[j]);
					break;
				}
			}
			if(abIDX >= 0 && chrIDX >= 0)
			{
				elImageTextData* data = (elImageTextData*) getModState(sliceID, (ULONG_PTR) abIDX);
				if(data)
				{
					int row = 0;
					int col = 0;
					if(data->isVertical)
					{
						row = chrIDX;
					} else
					{
						col = chrIDX;
					}
					if(data->img->get(col, row))
					{
						data->img->get(col, row)->draw(hdc, x + left, Y() + top);
					}
					x += data->img->width();
				}
			}
		}
		
		if(units)
		{
			if(m_units->position())
			{
				units->m_bmp->draw(hdc, x + left, Y() + top);
			}
		}

	}

	GdiFlush();
}

BOOL TxSkin::elImageText::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	BOOL ret = TxSkin::skin_element::loadXML(node, baseurl);

	freeClearStr(m_paramID);

	m_paramID		= xmlGetAttributeSTR(node, L"src");
	m_align			= (text_halign) xmlGetAttributeValueSTRArray(node, L"align", text_halign_left, L"left\0right\0center\0");

	IXMLDOMNode* child = NULL;
	node->get_firstChild(&child);
	while(child)
	{
		BSTR name = NULL;
		child->get_nodeName(&name);
		if(name)
		{
			if(!lstrcmp(name, L"alphabet"))
			{
				LPWSTR str = xmlGetAttributeSTR(child, L"text");
				if(str)
				{
					m_alphabets.Add(&str);
					loadXMLStates(child, baseurl, TxSkin::elStateNormal, TxSkin::modParamValue((int) 0), (ULONG_PTR) (m_alphabets.GetCount() - 1));
				}
			} else if(!lstrcmp(name, L"units"))
			{
				if(m_units)
				{
					delete m_units;
				}
				m_units = new elImageTextUnits(m_skin, m_paramID);
				m_units->loadXML(child, baseurl);
			}
			SysFreeString(name);
		}

		IXMLDOMNode* newChild = NULL;
		child->get_nextSibling(&newChild);
		child->Release();
		child = newChild;
	}

	return ret;
}

elementData* TxSkin::elImageText::getStateData( IXMLDOMNode* node, LPCWSTR baseurl, ULONG_PTR lParam )
{
	if((int) lParam >= 0 && (int) lParam < m_alphabets.GetCount())
	{
		LPWSTR file = xmlGetAttributeSTR(node, L"file");
		if(file)
		{
			WCHAR fileName[MAX_PATH];
			StringCchCopy(fileName, MAX_PATH, baseurl);
			StringCchCat(fileName, MAX_PATH, file);
			BOOL vertical = xmlGetAttributeValueBOOL(node, L"vertical", TRUE);
			delete file;

			int rows = 1;
			int cols = 1;
			if(vertical)
			{
				rows = lstrlen(m_alphabets[(int) lParam]);
			} else
			{
				cols = lstrlen(m_alphabets[(int) lParam]);
			}
			
			CTxDIB dib(fileName);
			if(dib.isValid())
			{
				return new elImageTextData(new CTxDibSet(&dib, rows, cols), vertical);
			}
		}
	}
	return NULL;
}

BOOL TxSkin::elImageText::isUsedParam( LPCWSTR paramID )
{
	if(skin_element::isUsedParam(paramID))
	{
		return TRUE;
	}
	if(m_paramID && !lstrcmpi(m_paramID, paramID))
	{
		return TRUE;
	}
	return FALSE;
}

int TxSkin::elImageText::get_text_width( LPCWSTR val, int sliceID )
{
	int txt_width = 0;
	for(int i = 0; val[i]; i++)
	{
		int abIDX = -1;
		int chrIDX = -1;
		for(int j = 0; j < m_alphabets.GetCount(); j++)
		{
			LPWSTR str = wcschr(m_alphabets[j], val[i]);
			if(str)
			{
				abIDX	= j;
				chrIDX	= (int) (str - m_alphabets[j]);
				break;
			}
		}
		if(abIDX >= 0 && chrIDX >= 0)
		{
			elImageTextData* data = (elImageTextData*) getModState(sliceID, (ULONG_PTR) abIDX);
			if(data)
			{
				txt_width +=data->img->width();
			}
		}
	}
	return txt_width;
}

TxSkin::elImageTextData::~elImageTextData()
{
	if(img)
	{
		delete img;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

TxSkin::elImageTextUnits::elImageTextUnits( skin* skin, LPCWSTR param ) : TxSkin::skin_element(skin, NULL, param)
{
	m_atRight = FALSE;
	m_param = NULL;
	makeStr(m_param, param);
}

TxSkin::elImageTextUnits::~elImageTextUnits()
{

}

BOOL TxSkin::elImageTextUnits::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	BOOL ret = TxSkin::skin_element::loadXML(node, baseurl);
	m_atRight = xmlGetAttributeValueSTRArray(node, L"position", 1, L"left\0right\0");
	loadXMLStates(node, baseurl);
	m_mod_by_units = xmlGetAttributeValueBOOL(node, L"mod-by-units", FALSE) == TRUE;
	if(m_mod_by_units)
	{
		makeStr(m_modParam, m_param);
	}
	return ret;
}

void TxSkin::elImageTextUnits::processXMLState( IXMLDOMNode* node, TxSkin::elementState state, TxSkin::modParamValue& modVal, LPCWSTR baseurl, ULONG_PTR lParam )
{
	LPWSTR file = xmlGetAttributeSTR(node, L"file");
	if(file)
	{
		int states = xmlGetAttributeValueNonSTR<int>(node, L"states", 0);
		WCHAR fileName[MAX_PATH];
		StringCchCopy(fileName, MAX_PATH, baseurl);
		StringCchCat(fileName, MAX_PATH, file);
		if(!states)
		{
			elImageTextUnitsData* data = new elImageTextUnitsData(new CTxDIB(fileName));
			addModState(modVal, state, data);
		} else
		{
			BOOL vertical = xmlGetAttributeValueBOOL(node, L"vertical", TRUE);

			int rows = 1;
			int cols = 1;
			if(vertical)
			{
				rows = states;
			} else
			{
				cols = states;
			}
			CTxDIB dib(fileName);
			if(dib.isValid())
			{
				CTxDibSet imgSet(&dib, rows, cols);
				if(vertical)
				{
					for(int i=0; i < states; i++)
					{
						elImageTextUnitsData* data = new elImageTextUnitsData(new CTxDIB(*imgSet.get(0, i)));
						addModState(modVal, (elementState) i, data);
					}
				} else
				{
					for(int i=0; i < states; i++)
					{
						elImageTextUnitsData* data = new elImageTextUnitsData(new CTxDIB(*imgSet.get(i, 0)) );
						addModState(modVal, (elementState) i, data);
					}
				}
			}
		}
		delete file;
	}
}

