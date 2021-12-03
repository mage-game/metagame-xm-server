#include "mempoolmanager.h"
#include <cstring>
#include <cassert>

namespace gamemempool
{
	MempoolManager MempoolManager::m_instance;
	MempoolManager & MempoolManager::GetInstance()
	{
		return m_instance;
	}

	MempoolManager::MempoolManager()
	{
		memset(m_mempool_list, 0, sizeof(m_mempool_list));
	}

	MempoolManager::~MempoolManager()
	{
		for (int i = 0; i < USHRT_MAX; ++i)
		{
			if (NULL != m_mempool_list[i])
			{
				delete m_mempool_list[i];
			}
		}
	}
	
	void *MempoolManager::Alloc(unsigned int alloc_size)
	{
		unsigned int hash_index = alloc_size / 100;
		unsigned int real_size = (hash_index + 1) * 100; // 实际内存向上取整，比如180字节是存储在200字节的内存池里

#ifdef _DEBUG
		assert(hash_index >= 0 && hash_index < USHRT_MAX);
#endif

		if (NULL == m_mempool_list[hash_index])
		{
			m_mempool_list[hash_index] = new MemPool(real_size, 16, "MempoolManager::Alloc");
		}

//		printf("====== Alloc(%d, %d, %d)\n", alloc_size, hash_index, real_size);
		void *mem = m_mempool_list[hash_index]->Alloc();
		m_ptr_to_pool_index_map[mem] = hash_index;
		return mem;
	}

	void MempoolManager::Free(void* mem)
	{
		std::map<void *, int>::iterator it = m_ptr_to_pool_index_map.find(mem);
		if (it == m_ptr_to_pool_index_map.end())
		{
			return;
		}

#ifdef _DEBUG
		assert(it->second >= 0 && it->second < USHRT_MAX);
#endif

//		printf("_______Free(%d)\n", it->second);
		m_mempool_list[it->second]->Free(mem);
		m_ptr_to_pool_index_map.erase(it);
	}

}
