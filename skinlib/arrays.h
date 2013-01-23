#pragma once

template <class T> class txArray
{
	int m_count;
	T* m_items;
public:
	txArray();
	~txArray();
	T& operator[](int i) { return m_items[i]; }
	int GetCount() { return m_count; }
	void Add(T* vItem);
	void Clear();
	void Delete(int index);
	txArray<T>& operator=(txArray<T>& v);
	T* GetItems() { return m_items; }
	int GetSizeOfItems() { return m_count * sizeof(T); }
	void SetSize(int newSize);
};

template <class T> 
txArray<T>::txArray()
{
	m_count = 0;
	m_items = 0;
}

template <class T> 
txArray<T>::~txArray()
{
	Clear();
}

template <class T> 
void txArray<T>::Add(T* vItem)
{
	if(!m_items)
	{
		m_items = (T*) malloc(sizeof(T));
		m_items[0] = *vItem;
		m_count = 1;
	} else
	{
		m_items = (T*) realloc(m_items, sizeof(T) * (m_count + 1));
		m_items[m_count] = *vItem;
		m_count++;
	}
}

template <class T> 
void txArray<T>::Clear()
{
	if(m_items)
	{
		free(m_items);
	}
	m_items = 0;
	m_count = 0;
}

template <class T> 
void txArray<T>::Delete(int index)
{
	if(index >= 0 && index < m_count)
	{
		for(int i=index; i < m_count-1; i++)
		{
			m_items[i] = m_items[i + 1];
		}
		if(m_count > 1)
		{
			m_items = (T*) realloc(m_items, sizeof(T) * (m_count - 1));
			m_count--;
		} else
		{
			Clear();
		}
	}
}


template <class T> 
txArray<T>& txArray<T>::operator=(txArray<T>& v)
{
	Clear();
	if(v.m_count)
	{
		m_items = (T*) malloc(sizeof(T) * v.m_count);
		memcpy(m_items, v.m_items, sizeof(T) * v.m_count);
		m_count = v.m_count;
	}
	return *this;
}

template <class T> 
void txArray<T>::SetSize(int newSize)
{
	if(!m_items)
	{
		m_items = (T*) malloc(sizeof(T) * newSize);
		m_count = newSize;
		ZeroMemory(m_items, sizeof(T) * newSize);
	} else
	{
		m_items = (T*) realloc(m_items, sizeof(T) * newSize);
		m_count = newSize;
		ZeroMemory(m_items, sizeof(T) * newSize);
	}
}
