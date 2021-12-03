
#include <assert.h>
#include <memory.h>
#include "singlemanager.h"
#include "obj.h"

#define DEFAULT_SINGLEMANAGER_NUM	32

SingleManager::SingleManager():m_obj(0), m_size(0), m_max_size(0), m_scene(0)
{
}

SingleManager::~SingleManager()
{
	Release();
}

void SingleManager::Init()
{
	assert(m_scene != 0);

	if (m_obj == 0)
	{
		m_size = 0;
		m_max_size = DEFAULT_SINGLEMANAGER_NUM;

		m_obj = new Obj*[m_max_size];
		memset(m_obj, 0, sizeof(Obj*) * m_max_size);
	}
}

void SingleManager::Release()
{
	if (m_obj != 0)
	{
		delete []m_obj;
		m_obj = 0;
		m_size = 0;
		m_max_size = 0;
	}
}

bool SingleManager::Add(Obj *obj)
{
	if (m_size >= m_max_size)
	{
		Resize(m_max_size * 2);
	}

	m_obj[m_size] = obj;
	obj->SetSingleMgrIndex(m_size);
	++m_size;
	return true;
}

bool SingleManager::Remove(Obj *obj)
{
	UInt32 index = obj->GetSingleMgrIndex();
	if (index >= m_size)
	{
		return false;
	}

	--m_size;
	m_obj[m_size]->SetSingleMgrIndex(index);
	m_obj[index] = m_obj[m_size];
	m_obj[m_size] = 0;

	return true;
}


void SingleManager::Resize(UInt32 size)
{
	if (size <= m_max_size)
	{
		return;
	}
	Obj **obj_tmp = m_obj;
	m_obj = new Obj*[size];
	memset(m_obj, 0, sizeof(Obj*) * size);
	if (obj_tmp != 0)
	{
		memcpy(m_obj, obj_tmp, sizeof(Obj*) * m_size);
		delete [] obj_tmp;
	}
	m_max_size = size;
}




