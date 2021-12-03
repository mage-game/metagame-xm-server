
#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include <memory.h>

template<class T>
class Heap		// “¿¿µ<‘ÀÀ„
{
public:

	Heap():m_data(0), m_size(0), m_max_size(0)
	{
		Resize(128);
	}

	~Heap()
	{
		if (m_data != 0)
			free(m_data);
	}

	void Push(const T &val)
	{
		if (m_size == m_max_size) Resize(m_max_size * 2);

		int s = m_size;
		++m_size;
		while(s > 0)
		{
			int f = (s - 1) / 2;
			if (!(val < m_data[f])) break;

			m_data[s] = m_data[f];
			s = f;
		}
		m_data[s] = val;
	}

	void PopFront()
	{
		if (m_size == 0) return ;

		--m_size;

		if (m_size == 0) return ;

		T tmp = m_data[m_size];
		int f = 0;
		int half_size = m_size / 2;
		while(f < half_size)
		{
			int s = f * 2 + 1;
			if (s + 1 < m_size && m_data[s + 1] < m_data[s]) ++s;

			if (!(m_data[s] < tmp)) break;

			m_data[f] = m_data[s];
			f = s;
		}
		m_data[f] = tmp;
	}

	T & Front() { return m_data[0];	}
	bool Front(T *v)
	{
		if (m_size != 0)
		{
			*v = m_data[0];
			return true;
		}
		return false;
	}
	int Size() { return m_size; }
	void Clear() { m_size = 0; }

protected:
	T *m_data;
	int m_size;
	int m_max_size;

	void Resize(int size)
	{
		if (size <= m_max_size) return;
		T *new_data = (T*)malloc(size * sizeof(T));
		if (m_data != 0)
		{
			memcpy(new_data, m_data, m_size * sizeof(T));
			free(m_data);
		}
		m_data = new_data;
		m_max_size = size;
	}
};

#endif

