#include "monsterdropsum.hpp"

#include "gamelog.h"
#include "engineadapter.h"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "monster/monsterinitparam.h"
#include "monster/monsterpool.h"
#include "monster/drop/droppool.hpp"
#include "item/itempool.h"
#include "item/itembase.h"

MonsterDropSum::MonsterDropSum()
	: m_last_log_time(0), m_last_drop_time(0)
{

}

MonsterDropSum::~MonsterDropSum()
{

}

MonsterDropSum & MonsterDropSum::Instance()
{
	static MonsterDropSum mds;
	return mds;
}

void MonsterDropSum::Update(unsigned long interval)
{
	//unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//if (now_second >= m_last_log_time + LOG_TIME_INTERVAL)
	//{
	//	for (MonsterDropMapIt it = m_monster_drop_map.begin(); it != m_monster_drop_map.end(); ++ it)
	//	{
	//		const MonsterInitParam *monster_init_param = MONSTERPOOL->GetMonsterParam(it->first);
	//		if (NULL != monster_init_param)
	//		{
	//			for (DropItemMapIt item_it = it->second.drop_item_map.begin(); item_it != it->second.drop_item_map.end(); ++ item_it)
	//			{
	//				const ItemBase *item_base = ITEMPOOL->GetItem(item_it->first);
	//				if (NULL != item_base)
	//				{
	//					gamelog::g_log_monster_drop_sum.printf(LL_INFO, ", [%d]%s, %d, [%d]%s * %d", (int)monster_init_param->monster_id, monster_init_param->monster_name, it->second.monster_die_count, 
	//						(int)item_base->GetItemId(), item_base->GetItemName(), item_it->second);
	//				}
	//			}
	//		}
	//	}

	//	m_last_log_time = now_second;
	//}

	//if (now_second >= m_last_drop_time + 1)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		for (int i = 0; i < MonsterPool::MAX_MONSTER_ID; i++)
	//		{
	//			const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(i);
	//			if (NULL != monster_param)
	//			{
	//				for (std::vector<UInt16>::const_iterator it = monster_param->dropid_list.begin(); it != monster_param->dropid_list.end(); ++it)
	//				{
	//					DROPPOOL->DropTest(*it, NULL, Posi(0, 0), INVALID_USER_ID, i, false);
	//				}

	//				for (std::vector<UInt16>::const_iterator it = monster_param->firsthit_dropid_list.begin(); it != monster_param->firsthit_dropid_list.end(); ++it)
	//				{
	//					DROPPOOL->DropTest(*it, NULL, Posi(0, 0), INVALID_USER_ID, i, false);
	//				}

	//				for (std::vector<UInt16>::const_iterator it = monster_param->dps_dropid_list.begin(); it != monster_param->dps_dropid_list.end(); ++it)
	//				{
	//					DROPPOOL->DropTest(*it, NULL, Posi(0, 0), INVALID_USER_ID, i, false);
	//				}

	//				for (std::vector<UInt16>::const_iterator it = monster_param->rand_dropid_list.begin(); it != monster_param->rand_dropid_list.end(); ++it)
	//				{
	//					DROPPOOL->DropTest(*it, NULL, Posi(0, 0), INVALID_USER_ID, i, false);
	//				}

	//				this->OnKillMonster(i);
	//			}
	//		}
	//	}
	//	
	//	m_last_drop_time = now_second;
	//}
}

void MonsterDropSum::OnKillMonster(int monster_id)
{
	//MonsterDropMapIt it = m_monster_drop_map.find(monster_id);
	//if (it != m_monster_drop_map.end())
	//{
	//	++ (it->second.monster_die_count);
	//}
	//else
	//{
	//	MonsterDropInfo monster_drop_info;
	//	monster_drop_info.monster_die_count = 1;

	//	m_monster_drop_map[monster_id] = monster_drop_info;
	//}
}

void MonsterDropSum::OnMonsterDropItem(int monster_id, ItemID item_id, int num)
{
	//MonsterDropMapIt it = m_monster_drop_map.find(monster_id);
	//if (it != m_monster_drop_map.end())
	//{
	//	DropItemMapIt item_it = it->second.drop_item_map.find(item_id);
	//	if (item_it != it->second.drop_item_map.end())
	//	{
	//		item_it->second += num;
	//	}
	//	else
	//	{
	//		it->second.drop_item_map[item_id] = num;
	//	}
	//}
}

