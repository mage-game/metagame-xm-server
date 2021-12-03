

#ifndef THREADQUEUE_H
#define THREADQUEUE_H

#include "mutex.h"
#include "semaphore.h"
#include <stdlib.h>
#include <memory>

/*
	��׼���������Ѷ��У�ʹ���ź�������PV�������ֿ�����д��
	T ������Ҫӵ�� public ���ʵ� ���ƹ��캯������ֵ��������Լ� ����������

	���ã��������κ�����
	Ч�ʣ�����ʹ���ź�������PV��������������ͨ����(cpu 2.X GHz)�ϴ�Լÿ��70W~100W��֮��
*/

template<class T>
class _ThreadQueue
{
public:
	_ThreadQueue(unsigned long buffersize=1)
		:m_read_ptr(0),m_write_ptr(0),m_readable(0), m_writeable(buffersize), m_buffer(0), m_size(0)
	{
		
		unsigned long size = buffersize + 1;	//bufferҪ�ȶ��г��ȴ�1������read_ptr��write_ptr�ڶ�����ʱ���ص�
		m_buffer = (T*)malloc( size   *sizeof(T) );			//�����ù��캯��
		if ( m_buffer == 0 )
		{
			throw std::bad_alloc();								//���䲻�ɹ�ǿ�����쳣�����Ƿ�Ӧ����������
		}
		m_size = size;
	}
	~_ThreadQueue()
	{
 		free(m_buffer);
	}

	void Push(const T& val)
	{
		m_writeable.Down();
		LockAndPush(val);
		m_readable.Up();
	}
	void Pop(T *val)
	{
		m_readable.Down();
		LockAndPop(val);
		m_writeable.Up();
	}
	void Top(T *val)
	{
		m_readable.Down();
		m_reader.Lock();
		*val = m_buffer[m_read_ptr];
		m_reader.Unlock();
		m_readable.Up();
	}

	bool TryPush(const T& val, unsigned long timeout_millsecond = 0)
	{
		if ( Semaphore::Succeed != m_writeable.TryDown(timeout_millsecond) )
		{
			return false;
		}
		LockAndPush(val);
		m_readable.Up();
		return true;
	}

	bool TryPop(T *val, unsigned long timeout_millsecond = 0)
	{
		if ( Semaphore::Succeed != m_readable.TryDown(timeout_millsecond) )
		{
			return false;
		}
		LockAndPop(val);
		m_writeable.Up();
		return true;
	}

	unsigned long MaxSize()const
	{
		return m_size - 1;
	}

	unsigned long Size()const
	{
		return (unsigned long)((m_size + ((int)m_write_ptr - (int)m_read_ptr)) % m_size);
	}

private:
	void LockAndPush(const T& val)
	{
		m_writer.Lock();
		new (&m_buffer[m_write_ptr])T(val);
		++m_write_ptr;
		if ( m_write_ptr >= m_size )
		{
			m_write_ptr -= m_size;
		}
		m_writer.Unlock();
	}

	void LockAndPop(T *val)
	{	
		m_reader.Lock();
		*val = m_buffer[m_read_ptr];
		m_buffer[m_read_ptr].~T();
		++m_read_ptr;
		if ( m_read_ptr >= m_size )
		{
			m_read_ptr -= m_size;
		}
		m_reader.Unlock();
	}

	_ThreadQueue(const _ThreadQueue&);
	_ThreadQueue& operator=(const _ThreadQueue&);

	unsigned long m_read_ptr;
	unsigned long m_write_ptr;

	Mutex m_reader;
	Mutex m_writer;

	Semaphore m_readable;
	Semaphore m_writeable;

	T *m_buffer;
	unsigned long m_size;
};

#endif
