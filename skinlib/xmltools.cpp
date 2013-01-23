#include "TxSkinEngine.h"
#include "xmltools.h"
#include "tokenizer.h"
#include <vector>


int TxSkin::xmlGetAttributeIntAlt( IXMLDOMNode* item, LPCTSTR attrName, int defValue )
{
	std::vector<std::wstring> tokens;
	tokenize(attrName, tokens, L"; ");


	CComVariant var;
	CComQIPtr<IXMLDOMElement> spXMLChildElement;
	spXMLChildElement = item;
	for(size_t i = 0; i < tokens.size(); i++)
	{
		HRESULT hr = spXMLChildElement->getAttribute(CComBSTR(tokens[i].c_str()), &var);
		if(hr == S_OK)
		{
			if(var.ChangeType(CVarTypeInfo<int>::VT) == S_OK)
			{
				return XML_GETVARIANTVALUE(var, int);
			}
		}
	}
	return defValue;
}

BOOL TxSkin::xmlGetAttributeBOOLAlt(IXMLDOMNode* item, LPCTSTR attrName, BOOL defValue)
{
	std::vector<std::wstring> tokens;
	tokenize(attrName, tokens, L"; ");

	for(size_t i = 0; i < tokens.size(); i++)
	{
		LPWSTR val = xmlGetAttributeSTR(item, tokens[i].c_str());
		if(val)
		{
			if(!StrCmpI(val, L"true"))
			{
				delete val;
				return TRUE;
			} else if(!StrCmpI(val, L"false"))
			{
				delete val;
				return FALSE;
			}
			delete val;
		}
	}
	return defValue;
}

int TxSkin::xmlGetAttributeValueSTRArrayAlt(IXMLDOMNode* item, LPCTSTR attrName, int defValue, LPWSTR array)
{
	std::vector<std::wstring> tokens;
	tokenize(attrName, tokens, L"; ");

	for(size_t i = 0; i < tokens.size(); i++)
	{
		LPWSTR val = xmlGetAttributeSTR(item, tokens[i].c_str());
		if(val)
		{
			LPWSTR ar = array;
			for(int i=0; ar[0]; i++)
			{
				if(!StrCmpI(val, ar))
				{
					delete val;
					return i;
				}
				ar += lstrlen(ar) + 1;
			}
			delete val;
			return defValue;
		}
	}
	return defValue;
}

LPWSTR TxSkin::xmlGetAttributeSTRAlt(IXMLDOMNode* item, LPCTSTR attrName)
{
	CComVariant var;
	CComQIPtr<IXMLDOMElement> spXMLChildElement;
	spXMLChildElement = item;
	if(spXMLChildElement.p)
	{
		std::vector<std::wstring> tokens;
		tokenize(attrName, tokens, L"; ");

		for(size_t i = 0; i < tokens.size(); i++)
		{
			HRESULT hr = spXMLChildElement->getAttribute(CComBSTR(tokens[i].c_str()), &var);
			if(hr == S_OK)
			{
				if(var.ChangeType(VT_BSTR) == S_OK)
				{
					LPWSTR ret = new WCHAR[lstrlen(var.bstrVal) + 1];
					lstrcpy(ret, var.bstrVal);
					return ret;
				}
			}
		}
	}
	return NULL;
}
