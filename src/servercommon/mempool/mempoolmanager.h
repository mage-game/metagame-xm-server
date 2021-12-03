#pragma once

#include "common/memory/mempool.h"
#include <climits>
#include <map>
#include <iostream>

namespace gamemempool
{
	// �ڴ�ع����������ڴ��ʹ�ã��̶��򻯴������
	// ע�⣺��MemPoolһ���������̰߳�ȫ��
	class MempoolManager
	{
	public:
		static MempoolManager & GetInstance();
		~MempoolManager();

		void* Alloc(unsigned int alloc_size);
		void Free(void* mem);

	private:
		static MempoolManager m_instance;
		MempoolManager();

		MemPool *m_mempool_list[USHRT_MAX];				// �ڴ�ظ�������100�ֽ�Ϊ��С���ȣ����֧�ֵ���6553500�ֽڵ��ڴ��
		std::map<void *, int> m_ptr_to_pool_index_map;	// ������ڴ�ָ��ӳ�䵽�ڴ���±�
	};
}

#define REGISTER_MEMPOOLMGR(PoolNameSpace, ClassName, INCREASE_NUM, ClassNameStr) \
void *ClassName::operator new(size_t c)\
{\
	void *mem = gamemempool::MempoolManager::GetInstance().Alloc(sizeof(ClassName));\
	return mem;\
}\
void ClassName::operator delete(void *m)\
{\
	gamemempool::MempoolManager::GetInstance().Free(m);\
}

