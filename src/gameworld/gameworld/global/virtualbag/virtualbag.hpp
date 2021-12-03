/*************************************************

Copyright:

Author:Luyaoyuan

Date:2018-03-19

LastModifyDate:2018-03-19

Description: 提供功能类的虚拟背包逻辑

使用方法：建立一个类继承虚拟背包类，并实现纯虚函数；
	以下为例子
	class PastureSpiritVirtualBag : public VirtualBag<PastureSpiritGridParam>
	{
	public:
		PastureSpiritVirtualBag(PastureSpiritGridParam *bag_list, int size)
		: VirtualBag<PastureSpiritGridParam>(bag_list, size)
		{
		}

		void SendBagInfoByIndex(int index) const;
		void SendBagAllInfo(VIRTUAL_BAG_INDEX_LIST *index_list = nullptr) const;

	private:
		void OnBagChange(VIRTUAL_BAG_INDEX_LIST *index_list = nullptr) const override;
		void PrintLogPut(int virtual_item_id, int num, bool is_bind, const char *reason) const override;
		void PrintLogConsume(int virtual_item_id, int num, const char *reason) const override;
		int GetStackLimitByVid(short virtual_item_id) const override;
	};

**************************************************/

#ifndef __VIRTUAL_BAG_TEMPLATE_H__
#define __VIRTUAL_BAG_TEMPLATE_H__
//gcc 4.4.7-23 build error
#ifdef max
#undef max
#undef min
#endif
#include <algorithm>
#include "servercommon/virtualbaggridparamdef.hpp"
#include <vector>
#include "engineadapter.h"

class Role;

template <typename T>
class VirtualBag
{
#define VIRTUAL_BAG_LOG_PUT(LOG) \
{\
	gamelog::LOG.buff_printf("%s user[%d %s], item_id[%d], num[%d], is_bind[%d], reason[%s]",\
		__FUNCTION__, this->GetRole()->GetUID(), this->GetRole()->GetName(), virtual_item_id, num, is_bind, reason);\
}
#define VIRTUAL_BAG_LOG_CONSUME(LOG) \
{\
	gamelog::LOG.buff_printf("%s user[%d %s], item_id[%d], num[%d], reason[%s]",\
		__FUNCTION__, this->GetRole()->GetUID(), this->GetRole()->GetName(), virtual_item_id, num, reason);\
}
public:
	typedef std::vector<int> INDEX_LIST;

	VirtualBag(T *bag_list, int size);
	virtual ~VirtualBag();

	void SetRole(Role *role) { m_role = role; };
	Role * GetRole() const { return m_role; };

	bool HasEnoughEmptyGrid(int count = 1) const;
	bool HasEnoughSpace(short virtual_item_id, short num, bool is_bind, short stack_limit);
	bool PutVirtualItem(short virtual_item_id, short num, bool is_bind, const char *reason, short stack_num, bool is_notify = true);
	bool PutVirtualItem(const T &virtual_item, const char *reason, short stack_num, bool is_notify = true);
	bool ConsumeVirtualItemByIndex(int grid_index, int num, const char *reason);
	bool ConsumeVirtualItemByVirtualItemId(short virtual_item_id, int num, const char *reason);

	// 获取格子的指针
	T * GetGridPrt(int index) const;

	// 重置格子
	void ResetGrid(T &grid);

	// 整理背包
	void SortBag(bool is_ignore_bind = false);
	int GetBagSize() const { return m_bag_size; }

private:
	// 当背包有变动会调用这个函数，通知客户，参数是变动的索引列表；
	virtual void OnBagChange(INDEX_LIST *index_list = NULL) const = 0;

	// 打印日志 放入物品
	virtual void PrintLogPut(int virtual_item_id, int num, bool is_bind, const char *reason) const = 0;

	// 打印日志 消耗物品
	virtual void PrintLogConsume(int virtual_item_id, int num, const char *reason) const = 0;

	// 获取物品的堆叠数
	virtual int GetStackLimitByVid(short virtual_item_id) const = 0;

	Role *m_role;

	T *m_bag_grid_list;
	int m_bag_size;
	int m_empty_grid_count;

	time_t m_last_sort_timestemp;
	INDEX_LIST m_dirty_index_list;
};

template <typename T>
::VirtualBag<T>::VirtualBag(T *bag_list, int size): m_role(NULL), m_bag_size(size), m_empty_grid_count(0), m_last_sort_timestemp(0)
{
#ifdef _DEBUG
	//检查，必须要有继承关系，保证有这个模板类用到的成员变量；
	VirtualBagGridParam *tmp = bag_list;
	tmp->IsEmpty();
#endif

	m_bag_grid_list = bag_list;
	for (int i = 0; i < size; ++i)
	{
		if (m_bag_grid_list[i].IsEmpty())
		{
			++m_empty_grid_count;
		}
	}
}

template <typename T>
::VirtualBag<T>::~VirtualBag()
{
}

template <typename T>
bool VirtualBag<T>::HasEnoughEmptyGrid(int count) const
{
	return m_empty_grid_count >= count;
}

template <typename T>
bool VirtualBag<T>::HasEnoughSpace(short virtual_item_id, short num, bool is_bind, short stack_limit)
{
	for (int i = 0; i < m_bag_size; ++i)
	{
		T &grid = m_bag_grid_list[i];
		short can_put_num = 0;
		if (grid.v_item_id == virtual_item_id && grid.is_bind == (is_bind ? 1 : 0))
		{
			can_put_num = stack_limit - grid.num;
		}
		else if (grid.IsEmpty())
		{
			can_put_num = stack_limit;
		}

		num -= can_put_num;
		if (num <= 0) return true;
	}

	return false;
}

template <typename T>
bool VirtualBag<T>::PutVirtualItem(short virtual_item_id, short num, bool is_bind, const char *reason, short stack_num, bool is_notify)
{
	if (virtual_item_id <= VirtualBagGridParam::INVALID_V_ITEM_ID || num <= 0 || stack_num <= 0)
	{
		return false;
	}

	if (!this->HasEnoughSpace(virtual_item_id, num, is_bind, stack_num))
	{
		return false;
	}

	int tmp_num = num;
	INDEX_LIST empty_index_list;
	m_dirty_index_list.clear();

	for (int i = 0; i < m_bag_size; ++i)
	{
		T &grid = m_bag_grid_list[i];
		if (grid.IsEmpty())
		{
			empty_index_list.push_back(i);
		}

		if (grid.v_item_id == virtual_item_id && grid.is_bind == (is_bind ? 1 : 0))
		{
			int add_num = stack_num - grid.num;
			add_num = add_num > num ? num : add_num;
			grid.num += add_num;
			tmp_num -= add_num;

			m_dirty_index_list.push_back(i);
			if (tmp_num <= 0) break;
		}
	}

// 	for (auto it: empty_index_list)
// 	{
	for (unsigned int j = 0; j < empty_index_list.size(); ++j)
	{
		if (tmp_num <= 0) break;

		T &grid = m_bag_grid_list[j];
		grid.v_item_id = virtual_item_id;
		grid.is_bind = is_bind;

		int add_num = stack_num > tmp_num ? tmp_num : stack_num;
		tmp_num -= add_num;
		grid.num += add_num;
		--m_empty_grid_count;
		m_dirty_index_list.push_back(j);
	}

	if (is_notify)
	{
		this->OnBagChange(&m_dirty_index_list);
	}
	m_dirty_index_list.clear();
	this->PrintLogPut(virtual_item_id, num, is_bind, reason);

	return true;
}

template <typename T>
bool VirtualBag<T>::PutVirtualItem(const T &virtual_item, const char *reason, short stack_num, bool is_notify)
{
	if (virtual_item.IsEmpty() || virtual_item.num <= 0 || stack_num <= 0)
	{
		return false;
	}

	if (!this->HasEnoughSpace(virtual_item.v_item_id, virtual_item.num, virtual_item.is_bind != 0, stack_num))
	{
		return false;
	}

	int item_num = virtual_item.num;

	for (int i = 0; i < m_bag_size; ++i)
	{
		T &grid = m_bag_grid_list[i];
		short real_put_num = 0;

		if (grid.IsEmpty())
		{
			real_put_num = item_num > stack_num ? stack_num : item_num;
			grid = virtual_item;
			grid.num = real_put_num;
			item_num -= real_put_num;

			--m_empty_grid_count;
			m_dirty_index_list.push_back(i);
		}
		else if (grid.v_item_id == virtual_item.v_item_id && grid.is_bind == virtual_item.is_bind)
		{
			short can_put_num = stack_num - grid.num;
			if (can_put_num <= 0) continue;

			real_put_num = item_num > can_put_num ? can_put_num : item_num;
			grid.num += real_put_num;
			item_num -= real_put_num;

			m_dirty_index_list.push_back(i);
		}

		if (0 == item_num) break;
	}

	if (is_notify)
	{
		this->OnBagChange(&m_dirty_index_list);
	}
	m_dirty_index_list.clear();

	this->PrintLogPut(virtual_item.v_item_id, virtual_item.num, virtual_item.is_bind != 0 ? true : false, reason);
	return true;
}

template <typename T>
bool VirtualBag<T>::ConsumeVirtualItemByIndex(int grid_index, int num, const char *reason)
{
	if (grid_index < 0 || grid_index >= m_bag_size)
	{
		return false;
	}

	T &grid = m_bag_grid_list[grid_index];
	if (grid.num < num)
	{
		return false;
	}

	grid.num -= num;
	if (grid.num < 0)
	{
		this->ResetGrid(grid);
	}

	m_dirty_index_list.clear();
	m_dirty_index_list.push_back(grid_index);
	this->OnBagChange(&m_dirty_index_list);
	m_dirty_index_list.clear();

	this->PrintLogConsume(grid.v_item_id, num, reason);

	return true;
}

template <typename T>
bool VirtualBag<T>::ConsumeVirtualItemByVirtualItemId(short virtual_item_id, int num, const char *reason)
{
	if (virtual_item_id <= VirtualBagGridParam::INVALID_V_ITEM_ID)
	{
		return false;
	}

	short tmp_num = num;
	m_dirty_index_list.clear();

	// 总消耗物品列表
	INDEX_LIST total_consume_index_set;

	// 非绑定可消耗物品列表
	INDEX_LIST not_bind_consume_index_set;

	// 先检查绑定是否足够消耗
	for (int i = 0; i < m_bag_size; ++i)
	{
		T &grid = m_bag_grid_list[i];
		if (grid.v_item_id == virtual_item_id)
		{
			if (grid.is_bind != 0)
			{
				total_consume_index_set.push_back(i);
				tmp_num -= grid.num;

				if (tmp_num <= 0) break;
			}
			else if (grid.is_bind == 0)
			{
				// 把要消耗的非绑定格子索引 放到 总消耗列表
				tmp_num -= grid.num;
				not_bind_consume_index_set.push_back(i);
				if (tmp_num <= 0) break;
			}
		}
	}

	//	需要消耗物品数量不足
	if (tmp_num > 0)
	{
		return false;
	}

	// 实际消耗
	int consume_num = num;
// 	for (auto v : total_consume_index_set)
// 	{
	short real_consume_num = 0;
	for (int k = 0; k < m_bag_size; ++k)
	{
		T &grid = m_bag_grid_list[k];

		if (grid.v_item_id == virtual_item_id)
		{
			if (grid.is_bind != 0)
			{
				real_consume_num = grid.num > consume_num ? consume_num : grid.num;
				grid.num -= real_consume_num;
				consume_num -= real_consume_num;

				if (grid.num == 0)
				{
					this->ResetGrid(grid);
				}

				if (tmp_num <= 0) break;
			}
			else if (grid.is_bind == 0)
			{
				real_consume_num = grid.num > consume_num ? consume_num : grid.num;
				grid.num -= real_consume_num;
				consume_num -= real_consume_num;

				if (grid.num == 0)
				{
					this->ResetGrid(grid);
				}

				if (tmp_num <= 0) break;
			}

			m_dirty_index_list.push_back(k);

			if (0 == consume_num) break;
		}
	}

	this->OnBagChange(&m_dirty_index_list);
	m_dirty_index_list.clear();

	this->PrintLogConsume(virtual_item_id, num, reason);
	return true;
}

template <typename T>
T * VirtualBag<T>::GetGridPrt(int index) const
{
	if (index < 0 || index > m_bag_size)
	{
		return NULL;
	}

	return &m_bag_grid_list[index];
}

template <typename T>
void VirtualBag<T>::ResetGrid(T &grid)
{
	grid.Reset();
	++m_empty_grid_count;
}

bool static Compare(const VirtualBagGridParam &l, const VirtualBagGridParam &r)
{
	return l < r;
}

template <typename T>
void VirtualBag<T>::SortBag(bool is_ignore_bind)
{
	if (EngineAdapter::Instance().Time() - m_last_sort_timestemp < 1)
	{
		return;
	}
	m_last_sort_timestemp = EngineAdapter::Instance().Time();

	m_empty_grid_count = 0;
	for (int i = 0; i < m_bag_size; ++i)
	{
		T &cur_grid = m_bag_grid_list[i];
		if (cur_grid.IsEmpty())
		{
			++m_empty_grid_count;
			continue;
		}

		int stack_limit = this->GetStackLimitByVid(cur_grid.v_item_id);
		if (stack_limit <= 0)
		{
			continue;
		}

		if (cur_grid.num == stack_limit) continue;

		for (int j = i + 1; j < m_bag_size; ++j)
		{
			T &merge_grid = m_bag_grid_list[j];
			if (merge_grid.IsEmpty())
			{
				continue;
			}

			if (cur_grid.v_item_id == merge_grid.v_item_id &&
				(is_ignore_bind || cur_grid.is_bind == merge_grid.is_bind))
			{
				short can_put_num = stack_limit - cur_grid.num;
				short real_put_num = merge_grid.num > can_put_num ? can_put_num : merge_grid.num;

				cur_grid.num += real_put_num;
				merge_grid.num -= real_put_num;
				if (is_ignore_bind && (1 == cur_grid.is_bind || 1 == merge_grid.is_bind))
				{
					cur_grid.is_bind = 1;
					merge_grid.is_bind = 1;
				}

				if (0 == merge_grid.num)
				{
					merge_grid.Reset();
				}

				if (stack_limit == cur_grid.num)
				{
					break;
				}
			}
		}
	}

	std::sort(&m_bag_grid_list[0], &m_bag_grid_list[m_bag_size], &Compare);
	this->OnBagChange();
}



#endif // __SERVERCOMMON_H__
