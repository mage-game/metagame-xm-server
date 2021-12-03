

#ifndef THREADQUEUEEX_H
#define THREADQUEUEEX_H

#include <vector>
#include "mutex.h"

/*
	EX�̶߳���
		ʹ��˫���н��зֿ���д�����ݴ洢����ʹ��std::vector

		�ŵ㣺Ч�ʼ���
		���������ֻ��һ�������ߣ��Թ��졢���������ĵ���ʱ�������У�һ�����ڴ洢��ͨ�ṹ
					ֻ��Push��TryPop���ֲ���
		��ע���˴���m_buff_readû�м��������ж�������ߣ���m_buff_read�������
				��pop����ԭ������Ȼ��һ�ݱ��ݣ����Զ�����Ҫ�ϸ�ʱ������������������T������
*/

template<class T>
class _ThreadQueueEx
{
public:
	_ThreadQueueEx():m_read_index(0) {}
	~_ThreadQueueEx(){}

	void Push(const T& val)
	{
		m_write_lock.Lock();
		m_buff_write.push_back(val);
		m_write_lock.Unlock();
	}

	bool TryPop(T *val)
	{
		if (m_read_index == (int)m_buff_read.size())
		{
			m_buff_read.clear();
			m_read_index = 0;

			m_write_lock.Lock();
			m_buff_write.swap(m_buff_read);
			if (m_read_index == (int)m_buff_read.size())
			{
				m_write_lock.Unlock();
				return false;
			}
			m_write_lock.Unlock();
		}
		*val = m_buff_read[m_read_index];
		++m_read_index;
		return true;
	}
protected:
	std::vector<T>	m_buff_write;
	Mutex			m_write_lock;

	std::vector<T>	m_buff_read;
	int				m_read_index;

};



#endif



