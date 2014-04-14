#include "TxSkinEngine.h"
#include "elFormatedText.h"
#include "skinBase.h"
#include "skinParams.h"

using namespace TxSkin;

TxSkin::elFormatedText::elFormatedText( skin* skin, 
										LPCWSTR text				/*= NULL*/, 
										LPCWSTR fontName			/*= L"Arial"*/, 
										BOOL bold					/*= FALSE*/, 
										BOOL italic					/*= FALSE*/, 
										UINT flags					/*= 0*/, 
										TxSkin::color color			/*= TxSkin::color(0, 0, 0, 0)*/, 
										DWORD quality				/*= CLEARTYPE_QUALITY*/, 
										int glowSize				/*= 0*/, 
										TxSkin::color glowColor		/*= TxSkin::color(0, 0, 0, 0)*/, 
										LPCWSTR id					/*= NULL*/, 
										LPCWSTR modParam			/*= NULL*/) : elText(skin, NULL, fontName, bold, italic, flags, color, quality, glowSize, glowColor, id, modParam)
{
	m_text = NULL;
	makeStr(m_text,	text);

	if(m_text)
	{
		addState(elStateNormal, color, glowColor);
	}
}

TxSkin::elFormatedText::~elFormatedText()
{
	freeClearStr(m_text);
}

BOOL TxSkin::elFormatedText::getString( std::wstring& outStr, int sliceID )
{
	if(!m_text)
	{
		return FALSE;
	}
	outStr = L"";
	LPWSTR txt = m_text;
	while(txt[0])
	{
		if(txt[0] == L'{')
		{
			std::wstring paramID;
			txt++;
			while(txt[0] && txt[0] != L'}')
			{
				paramID += txt[0];
				txt++;
			}
			LPCWSTR val = m_skin->getParamString(paramID.c_str(), sliceID);
			if(val)
			{
				outStr += val;
			}
			if(txt[0])
			{
				txt++;
			}
		} else
		{
			outStr += txt[0];
			txt++;
		}
	}
	return TRUE;
}

BOOL TxSkin::elFormatedText::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	m_text = xmlGetAttributeSTR(node, L"text");

	return elText::loadXML(node, baseurl);
}
