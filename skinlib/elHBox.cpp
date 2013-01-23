#include "TxSkinEngine.h"
#include "elHBox.h"
#include "skinBase.h"

using namespace TxSkin;

elHBOX::elHBOX( skin* skin ) : skin_element(skin)
{
}

elHBOX::~elHBOX()
{

}

BOOL TxSkin::elHBOX::loadXML( IXMLDOMNode* node, LPCWSTR baseurl )
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

void TxSkin::elHBOX::init()
{
	int h					= 0;
	int w					= 0;

	int flex_count			= 0;
	int w_noflex			= 0;
	int processed_flexes	= 0;

	for(int i=0; i < m_elements.GetCount(); i++)
	{
		if(!m_elements[i]->float_el())
		{
			m_elements[i]->X(w + m_elements[i]->margin_left());
			m_elements[i]->Y(m_elements[i]->margin_top());

			if(!m_elements[i]->height())
			{
				m_elements[i]->height( (height() ? height() : m_parent->height()) - ( m_elements[i]->margin_top() + m_elements[i]->margin_bottom() ) );
			}

			w += m_elements[i]->width() + m_elements[i]->margin_left() + m_elements[i]->margin_right();
			if(!m_elements[i]->flex())
			{
				w_noflex += m_elements[i]->width() + m_elements[i]->margin_left() + m_elements[i]->margin_right();
			}
			h = max(h, m_elements[i]->height() + m_elements[i]->margin_top() + m_elements[i]->margin_bottom());
			flex_count += m_elements[i]->flex();
			if(m_elements[i]->flex())
			{
				processed_flexes++;
			}
		}
	}

	int flex_width = (width() ? width() : m_parent->width()) - w_noflex;

	if(flex_count)
	{
		h = 0;
		w = 0;

		int processed_flex_width = 0;

		for(int i=0; i < m_elements.GetCount(); i++)
		{
			if(!m_elements[i]->float_el())
			{
				m_elements[i]->X(w + m_elements[i]->margin_left());
				m_elements[i]->Y(m_elements[i]->margin_top());
				if(m_elements[i]->flex())
				{
					processed_flexes--;
					int fh = (int) (flex_width * (double) m_elements[i]->flex() / (double) flex_count) - (m_elements[i]->margin_left() + m_elements[i]->margin_right());
					if(processed_flexes <= 0 && processed_flex_width + fh != flex_width)
					{
						fh = flex_width - processed_flex_width - (m_elements[i]->margin_left() + m_elements[i]->margin_right());
					}
					m_elements[i]->width(fh);
					processed_flex_width += fh + (m_elements[i]->margin_left() + m_elements[i]->margin_right());
				}
				w += m_elements[i]->width() + m_elements[i]->margin_left() + m_elements[i]->margin_right();
				h = max(h, m_elements[i]->height() + m_elements[i]->margin_top() + m_elements[i]->margin_bottom());
			}
		}
	}

	height(h);
	width(w);

	skin_element::init();
}
