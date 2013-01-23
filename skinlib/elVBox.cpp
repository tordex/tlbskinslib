#include "TxSkinEngine.h"
#include "elVBox.h"
#include "skinBase.h"

using namespace TxSkin;

TxSkin::elVBOX::elVBOX( skin* skin) : skin_element(skin)
{
}

TxSkin::elVBOX::~elVBOX()
{

}

BOOL TxSkin::elVBOX::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
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

void TxSkin::elVBOX::init()
{
	int h					= 0;
	int w					= 0;

	int flex_count			= 0;
	int h_noflex			= 0;
	int processed_flexes	= 0;

	for(int i=0; i < m_elements.GetCount(); i++)
	{
		if(!m_elements[i]->float_el())
		{
			m_elements[i]->Y(h + m_elements[i]->margin_top());
			m_elements[i]->X(m_elements[i]->margin_left());
			if(!m_elements[i]->width())
			{
				m_elements[i]->width( ( width() ? width() : m_parent->width() ) - ( m_elements[i]->margin_left() + m_elements[i]->margin_right() ) );
			}
			h += m_elements[i]->height() + m_elements[i]->margin_top() + m_elements[i]->margin_bottom();
			if(!m_elements[i]->flex())
			{
				h_noflex += m_elements[i]->height() + m_elements[i]->margin_top() + m_elements[i]->margin_bottom();
			}
			w = max(w, m_elements[i]->width() + m_elements[i]->margin_left() + m_elements[i]->margin_right());
			flex_count += m_elements[i]->flex();
			if(m_elements[i]->flex())
			{
				processed_flexes++;
			}
		}
	}

	int flex_height = (height() ? height() :  m_parent->height()) - h_noflex;

	if(flex_count)
	{
		h = 0;
		w = 0;

		int processed_flex_height = 0;

		for(int i=0; i < m_elements.GetCount(); i++)
		{
			if(!m_elements[i]->float_el())
			{
				m_elements[i]->Y(h + m_elements[i]->margin_top());
				m_elements[i]->X(m_elements[i]->margin_left());
				if(m_elements[i]->flex())
				{
					processed_flexes--;
					int fh = (int) (flex_height * (double) m_elements[i]->flex() / (double) flex_count) - (m_elements[i]->margin_top() + m_elements[i]->margin_bottom());
					if(processed_flexes <= 0 && processed_flex_height + fh != flex_height)
					{
						fh = flex_height - processed_flex_height - (m_elements[i]->margin_top() + m_elements[i]->margin_bottom());
					}
					m_elements[i]->height(fh);
					processed_flex_height += fh + (m_elements[i]->margin_top() + m_elements[i]->margin_bottom());
				}
				h += m_elements[i]->height() + m_elements[i]->margin_top() + m_elements[i]->margin_bottom();
				w = max(w, m_elements[i]->width() + m_elements[i]->margin_left() + m_elements[i]->margin_right());
			}
		}
	}

	height(h);
	width(w);

	skin_element::init();
}
