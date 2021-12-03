/************************************************************************/
/* 
	SmartPtr<T*> ����ָ����

	���ԣ�	ʹ��ԭ�Ӳ���ȷ���̰߳�ȫ���� g++ �Լ� VC ��������ͨ����
			���ƹ����Լ���ֵ����������Ĭ�ϵ�ָ��ת�������������µ����壺

			SmartPtr<const int*> k = SmartPtr<int*>();
			SmartPtr<Base*> a = SmartPtr<Derived*>();

			�ڴ˹����У�ת������Ϊ��ԭ��ָ�뱣��һ�¡�

			�ṩ dynamic_assign ʵ�������ڵ�����ָ������ת�����ӻ��ൽ���ࣩ��
*/
/************************************************************************/

#ifndef SMARTPTR_H
#define SMARTPTR_H

#ifndef NULL
#define NULL 0
#endif

template<class T>class _SmartPtr;

inline void atom_incl(volatile int *val)
{
#ifdef __GNUC__
	asm volatile("lock incl (%0)\n\t"
		:
	:"r"(val)
		:);
#endif
#ifdef _MSC_VER
	__asm
	{
		mov eax, val;
		lock inc dword ptr[eax];
	}
#endif
}

inline void atom_decl(volatile int *val)
{
#ifdef __GNUC__
	asm volatile("lock decl (%0)\n\t"
		:
	:"r"(val)
		:);
#endif
#ifdef _MSC_VER
	__asm
	{
		mov eax, val;
		lock dec dword ptr[eax];
	}
#endif
}

inline bool atom_decl_and_test(volatile int *val)
{
	unsigned char c;
#ifdef __GNUC__
	asm volatile("lock decl (%0); sete %1\n\t"
				 :"=r"(val),"=qm"(c) 
				 :"r"(val)
				 :"memory");
#endif
#ifdef _MSC_VER
	__asm
	{
		mov eax, val;
			
		lock dec dword ptr[eax]; 
		sete c;
	}
#endif
	return c != 0;
}


template<class T>
class _SmartPtr<T*>
{
	template<class U> friend class _SmartPtr;
public:
	_SmartPtr()
		:m_Data(NULL),m_RefCount(NULL)
	{
	}
	_SmartPtr(T *val)
		:m_Data(NULL),m_RefCount(NULL)
	{
		if (val != NULL)
		{
			m_Data = val;
			m_RefCount = new int;
			*m_RefCount = 1;
		}
	}

	_SmartPtr(const _SmartPtr& val)
		:m_Data(val.m_Data),m_RefCount(val.m_RefCount)
	{
		if (m_RefCount!=NULL) 
		{
			atom_incl(m_RefCount);
		}
	}

	template<class U>
	_SmartPtr(const _SmartPtr<U*>& val)
		:m_Data(val.m_Data),m_RefCount(val.m_RefCount)
	{
		if (m_RefCount!=NULL) 
		{
			atom_incl(m_RefCount);
		}
	}

	virtual ~_SmartPtr()
	{
		Release();
	}

	_SmartPtr& operator=(const _SmartPtr& val)
	{
		T *data = val.m_Data;
		volatile int *refcount = val.m_RefCount;
		return assign(data, refcount);
	}

	template<class U>
	_SmartPtr& operator=(const _SmartPtr<U*>& val)
	{
		T *data = val.m_Data;
		volatile int *refcount = val.m_RefCount;
		return assign(data, refcount);
	}

	template<class U>
	bool DynamicAssign(const _SmartPtr<U*>& val)
	{
		T *data = dynamic_cast<T*>(val.m_Data);
		if ( data == NULL ) return false;
		volatile int *refcount = val.m_RefCount;
		assign(data, refcount);
		return true;	
	}

	template<class U>
	bool ConstAssign(const _SmartPtr<const U*>& val)
	{
		T *data = const_cast<T*>(val.m_Data);
		if ( data == NULL ) return false;
		volatile int *refcount = val.m_RefCount;
		assign(data, refcount);
		return true;	
	}

	template<class U>
	bool operator==(const _SmartPtr<U*>& val)const
	{
		return m_Data == val.m_Data;
	}
	bool operator==(const _SmartPtr& val)const
	{
		return m_Data == val.m_Data;
	}
	bool operator==(const T *val)const
	{
		return m_Data == val;
	}
	template<class U>
	bool operator!=(const _SmartPtr<U*>& val)const
	{
		return m_Data != val.m_Data;
	}
	bool operator!=(const _SmartPtr& val)const
	{
		return m_Data != val.m_Data;
	}
	bool operator!=(const T *val)const
	{
		return m_Data != val;
	}

	T *operator->()
	{
		return m_Data;
	}
	const T *operator->()const
	{
		return m_Data;
	}
	T& operator*()
	{
		return *m_Data;
	}
	const T& operator*()const
	{
		return *m_Data;
	}
	bool operator<(const _SmartPtr& val)const
	{
		return m_Data < val.m_Data;
	}

	T *Ptr()
	{
		return m_Data;
	}

	const T *Ptr()const
	{
		return m_Data;
	}

	int RefCount()const
	{
		if ( m_RefCount == NULL ) return 0;
		else return *m_RefCount;
	}

	void Release()
	{
		if (m_RefCount==NULL) return;
		if ( atom_decl_and_test(m_RefCount) )
		{
			delete m_Data;
			delete m_RefCount;
		}
		m_Data = NULL;
		m_RefCount = NULL;
	}

	

protected:

	_SmartPtr& assign(T *data, volatile int *refcount)
	{
		//T *data = val.m_Data;
		//volatile int *refcount = val.m_RefCount;
		if (refcount!=NULL)
		{
			atom_incl(refcount);
		}
		Release();
		m_Data = data;
		m_RefCount = refcount;

		return *this;
	}

	T *m_Data;
	volatile int *m_RefCount;

};











#endif
