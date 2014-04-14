#pragma once

namespace TxSkin
{
	class sliceParameter
	{
		LPWSTR				m_units_str;
		int					m_units_int;
		LPWSTR				m_stringVal;
		int					m_intVal;
		txArray<int>		m_vals;
		int					m_maxValsCount;
		int					m_sliceID;
		BOOL				m_autoFit;
	public:
		sliceParameter(int sliceID, int maxValsCount);
		~sliceParameter();

		void	stringVal(LPCWSTR val)		{ makeStr(m_stringVal, val);	}
		LPCWSTR	stringVal()					{ return m_stringVal;			}

		void	autoFit(BOOL val)			{ m_autoFit = val;				}
		BOOL	autoFit()					{ return m_autoFit;				}

		void	units_str(LPCWSTR val)		{ makeStr(m_units_str, val);	}
		LPCWSTR	units_str()					{ return m_units_str;			}

		void	units_int(int val)			{ m_units_int = val;			}
		int		units_int()					{ return m_units_int;			}

		void	intVal(int val)				{ m_intVal = val;				}
		int		intVal()					{ return m_intVal;				}
		int		getSliceID()				{ return m_sliceID;				}
		void	addValue(int val);
		int*	getValues(int* count);
		void	setFormatedVal(int val, LPCWSTR format);
		int		getMaxValsCount()			{ return m_maxValsCount;		}
	};

	class parameter
	{
	protected:
		LPWSTR						m_ID;
		txArray<sliceParameter*>	m_slices;
	public:
		parameter(LPCWSTR id, int maxValsCount = 0);
		virtual ~parameter();

		void	addSlice(int sliceID, int maxValsCount = 0);
		void	deleteSlice(int sliceID);
		void	clearSlices();

		LPCWSTR getID()	{ return m_ID;	}

		void	setStringVal(LPCWSTR val, int sliceID);
		LPCWSTR	getStringVal(int sliceID);

		void	setIntVal(int val, int sliceID);
		int		getIntVal(int sliceID);

		BOOL	setAutoFit(int val, int sliceID);
		BOOL	getAutoFit(int sliceID);

		void	setIntUnits(int val, int sliceID);
		int		getIntUnits(int sliceID);

		void	setStringUnits(LPCWSTR val, int sliceID);
		LPCWSTR	getStringUnits(int sliceID);

		void	addValue(int val, int sliceID);
		int*	getValues(int* count, int sliceID);
		void	setFormatedVal(int val, LPCWSTR format, int sliceID);

		int		getSlicesCount()		{ return m_slices.GetCount();			}
		int		getSliceID(int idx)		{ return m_slices[idx]->getSliceID();	}

		sliceParameter*	getSlice(int sliceID);
	};

}
