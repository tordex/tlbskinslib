#include "TxSkinEngine.h"
#include "elLabel.h"

using namespace TxSkin;

TxSkin::elLabel::elLabel( skin* skin, 
									   LPCWSTR text				/*= NULL*/, 
									   LPCWSTR fontName			/*= L"Arial"*/, 
									   BOOL bold				/*= FALSE*/, 
									   BOOL italic				/*= FALSE*/, 
									   UINT flags				/*= 0*/, 
									   TxSkin::color color		/*= TxSkin::color(0, 0, 0, 0)*/, 
									   DWORD quality			/*= CLEARTYPE_QUALITY*/, 
									   int glowSize				/*= 0*/, 
									   TxSkin::color glowColor	/*= TxSkin::color(0, 0, 0, 0)*/, 
									   LPCWSTR id				/*= NULL*/, 
									   LPCWSTR modParam			/*= NULL*/) : elText(skin, NULL, fontName, bold, italic, flags, color, quality, glowSize, glowColor, id, modParam)
{
	m_text = NULL;
	makeStr(m_text,	text);

	if(m_text)
	{
		addState(elStateNormal, color, glowColor);
	}
}

TxSkin::elLabel::~elLabel()
{
	freeClearStr(m_text);
}


BOOL TxSkin::elLabel::getString( std::wstring& outStr, int sliceID )
{
	if(m_text)
	{
		outStr = m_text;
		return TRUE;
	}
	return FALSE;
}

BOOL TxSkin::elLabel::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	m_text = xmlGetAttributeSTR(node, L"text");

	return elText::loadXML(node, baseurl);
}
