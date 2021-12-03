
#ifndef CLMEMPOOL_H
#define CLMEMPOOL_H


/*
	Custom length memory pool
	可定制长度的内存池，使用定长内存池MemPool来组合成可定制长度的内存池
	
	使用：该类使用Init来初始化定制长度，考虑到内存池使用时可能作为全局数据存在，无法调用Init函数，在此
	情况下，请继承该类，在子类的构造函数中调用CLMemPool::Init函数
	
	PS：Init过后使用Alloc和Free，线程安全
*/

class MemPool;
class Mutex;

class CLMemPool
{
public:
	CLMemPool(const char *name);
	virtual ~CLMemPool();

	/*
		放入一个长度列表，初始化内存池
		@length_list：每个内存池对应可申请的定长内存为length_list中的对应元素。注意：length_list元素内容必须升序排列
		@n: length_list数组长度
		@increase: 定长内存池耗尽时的预留内存块个数
	*/
	void Init(unsigned int *length_list, unsigned int n, unsigned int increase=32);

	void * Alloc(unsigned int size);
	void Free(void *mem);


private:
	CLMemPool(const CLMemPool& _p);
	CLMemPool &operator=(const CLMemPool& _p);

private:
	unsigned int *m_length_list;
	MemPool **m_pool_list;
	unsigned int m_pool_num;

	Mutex *m_pool_lock;

	int m_record_index;
	const char *m_name;
};

#endif


