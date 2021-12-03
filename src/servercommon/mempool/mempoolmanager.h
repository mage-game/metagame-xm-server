#pragma once

#include "common/memory/mempool.h"
#include <climits>
#include <map>
#include <iostream>

namespace gamemempool
{
	// 内存池管理器，简化内存池使用，继而简化代码设计
	// 注意：跟MemPool一样，不是线程安全的
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

		MemPool *m_mempool_list[USHRT_MAX];				// 内存池个数，以100字节为最小粒度，最大支持单个6553500字节的内存池
		std::map<void *, int> m_ptr_to_pool_index_map;	// 分配的内存指针映射到内存池下标
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

