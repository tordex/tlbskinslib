#include "TxSkinEngine.h"
#include "skinParams.h"

using namespace TxSkin;

TxSkin::parameter::parameter( LPCWSTR id, int maxValsCount )
{
	m_ID	= NULL;
	if(id)
	{
		makeStr(m_ID, id);
	} else
	{
		makeStr(m_ID, L"");
	}
	addSlice(0, maxValsCount);
}

TxSkin::parameter::~parameter()
{
	freeClearStr(m_ID);
	for(int i=0; i < m_slices.GetCount(); i++)
	{
		delete m_slices[i];
	}
}

void TxSkin::parameter::setStringVal( LPCWSTR val, int sliceID )
{
	sliceParameter* slice = getSlice(sliceID);
	if(slice)
	{
		slice->stringVal(val);
	}
}

void TxSkin::parameter::setFormatedVal( int val, LPCWSTR format, int sliceID )
{
	sliceParameter* slice = getSlice(sliceID);
	if(slice)
	{
		slice->setFormatedVal(val, format);
	}
}

void TxSkin::parameter::addValue( int val, int sliceID )
{
	sliceParameter* slice = getSlice(sliceID);
	if(slice)
	{
		slice->addValue(val);
	}
}

int* TxSkin::parameter::getValues( int* count, int sliceID )
{
	*count = 0;
	sliceParameter* slice = getSlice(sliceID);
	if(slice)
	{
		return slice->getValues(count);
	}
	return NULL;
}

sliceParameter* TxSkin::parameter::getSlice( int sliceID )
{
	for(int i=0; i < m_slices.GetCount(); i++)
	{
		if(m_slices[i]->getSliceID() == sliceID)
		{
			return m_slices[i];
		}
	}
	return NULL;
}

LPCWSTR TxSkin::parameter::getStringVal(int sliceID)
{
	sliceParameter* slice = getSlice(sliceID);
	if(slice)
	{
		return slice->stringVal();
	}
	return NULL;
}

void TxSkin::parameter::setIntVal( int val, int sliceID )
{
	sliceParameter* slice = getSlice(sliceID);
	if(slice)
	{
		slice->intVal(val);
	}
}

int TxSkin::parameter::getIntVal( int sliceID )
{
	sliceParameter* slice = getSlice(sliceID);
	if(slice)
	{
		return slice->intVal();
	}
	return 0;
}

void TxSkin::parameter::addSlice( int sliceID, int maxValsCount /*= 0*/ )
{
	int maxCount = maxValsCount;
	if(m_slices.GetCount())
	{
		maxCount = m_slices[0]->getMaxValsCount();
	}
	sliceParameter* slice = new sliceParameter(sliceID, maxCount);
	m_slices.Add(&slice);
}

void TxSkin::parameter::deleteSlice( int sliceID )
{
	int idx = -1;
	for(int i=0; i < m_slices.GetCount(); i++)
	{
		if(m_slices[i]->getSliceID() == sliceID)
		{
			idx = i;
			break;
		}
	}
	if(!(idx == 0 && m_slices.GetCount() == 1))
	{
		delete m_slices[idx];
		m_slices.Delete(idx);
	}
}

void TxSkin::parameter::clearSlices()
{
	for(int i=0; i < m_slices.GetCount(); i++)
	{
		delete m_slices[i];
	}
	m_slices.Clear();
}

void TxSkin::parameter::setIntUnits( int val, int sliceID )
{
	sliceParameter* slice = getSlice(sliceID);
	if(slice)
	{
		slice->units_int(val);
	}
}

int TxSkin::parameter::getIntUnits( int sliceID )
{
	sliceParameter* slice = getSlice(sliceID);
	if(slice)
	{
		return slice->units_int();
	}
	return 0;
}

void TxSkin::parameter::setStringUnits( LPCWSTR val, int sliceID )
{
	sliceParameter* slice = getSlice(sliceID);
	if(slice)
	{
		slice->units_str(val);
	}
}

LPCWSTR TxSkin::parameter::getStringUnits( int sliceID )
{
	sliceParameter* slice = getSlice(sliceID);
	if(slice)
	{
		return slice->units_str();
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
sliceParameter::sliceParameter( int sliceID, int maxValsCount )
{
	m_sliceID		= sliceID;
	m_maxValsCount	= maxValsCount;
	m_stringVal		= NULL;
	m_intVal		= 0;
	m_units_str		= NULL;
	m_units_int		= 0;
	if(m_maxValsCount)
	{
		m_vals.SetSize(m_maxValsCount);
		for(int i=0; i < m_maxValsCount; i++)
		{
			m_vals[i] = 0;
		}
	}
}

TxSkin::sliceParameter::~sliceParameter()
{
	freeClearStr(m_stringVal);
	freeClearStr(m_units_str);
}

void TxSkin::sliceParameter::addValue( int val )
{
	for(int i = m_vals.GetCount() - 1; i >0; i-- )
	{
		m_vals[i] = m_vals[i - 1];
	}
	if(m_vals.GetCount())
	{
		m_vals[0] = val;
	}
}

void TxSkin::sliceParameter::setFormatedVal( int val, LPCWSTR format )
{
	m_intVal	= val;
	if(format)
	{
		freeClearStr(m_stringVal);
		m_stringVal = new WCHAR[lstrlen(format) + 50];
		wsprintf(m_stringVal, format, val);
	}
}

int* TxSkin::sliceParameter::getValues( int* count )
{
	*count = m_vals.GetCount();
	return m_vals.GetItems();
}
