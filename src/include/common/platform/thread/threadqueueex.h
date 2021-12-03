

#ifndef THREADQUEUEEX_H
#define THREADQUEUEEX_H

#include <vector>
#include "mutex.h"

/*
	EX线程队列
		使用双队列进行分开读写，数据存储容器使用std::vector

		优点：效率极高
		适用情况：只有一个消费者，对构造、析构函数的调用时机不敏感，一般用于存储普通结构
					只有Push和TryPop两种操作
		备注：此处对m_buff_read没有加锁，如有多个消费者，则m_buff_read必须加锁
				在pop出后，原队列仍然有一份备份，所以对于需要严格按时调用析构函数的类型T则不适用
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



