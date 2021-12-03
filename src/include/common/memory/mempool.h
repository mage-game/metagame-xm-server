
#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <vector>

/*
	�����ڴ�أ�ʵ�ּ򵥵Ķ����ڴ滺�����
	�ڴ����Ϊ��ʱ��������ڴ������룬��Ϊ��������Ԥ���ռ䣬�ڴ��ͷ�ʱ���������ڴ��ͷţ����������

	ע�⣺���̰߳�ȫ
*/

class MemPool
{
public:
	MemPool(unsigned int alloc_size, unsigned int _increase, const char *class_name);
	~MemPool();

	void * Alloc();
	void Free(void *mem);

protected:
	void Increase();

private:
	MemPool(const MemPool &_m);
	MemPool & operator=(const MemPool &_r);

private:
	std::vector<void *> m_block_pool;
	std::vector<void *> m_malloc_record;

	unsigned int m_size;
	unsigned int m_size_for_increase;

	int m_record_index;
	const char *m_name;
};





#define REGISTER_MEMPOOL(PoolNameSpace, ClassName, INCREASE_NUM, ClassNameStr) \
namespace PoolNameSpace\
{\
	MemPool g_##ClassName##_mem_pool(sizeof(ClassName), INCREASE_NUM, ClassNameStr);\
}\
void *ClassName::operator new(size_t c)\
{\
	void *mem = PoolNameSpace::g_##ClassName##_mem_pool.Alloc();\
	return mem;\
}\
void ClassName::operator delete(void *m)\
{\
	PoolNameSpace::g_##ClassName##_mem_pool.Free(m);\
}

#define DEFINE_MEMPOOL\
void *	operator new(size_t c);\
void	operator delete(void *m);\

#endif

