#include "TxSkinEngine.h"
#include "elRoot.h"
#include "skinBase.h"


TxSkin::elRoot::elRoot( skin* skin ) : skin_element(skin)
{

}

TxSkin::elRoot::~elRoot()
{

}

BOOL TxSkin::elRoot::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
{
	BOOL ret = skin_element::loadXML(node, baseurl);

	IXMLDOMNode* child = NULL;
	node->get_firstChild(&child);
	while(child)
	{
		BSTR name = NULL;
		child->get_nodeName(&name);
		if(name)
		{
			skin_element* el = m_skin->createElement(name, m_skin);
			if(el)
			{
				if(el->loadXML(child, baseurl))
				{
					addElement(el);
				} else
				{
					delete el;
				}
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
