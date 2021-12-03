#include "challengefbmanager.hpp"
#include "gameworld/world.h"
#include "obj/character/role.h"
#include "scene/scene.h"
#include "engineadapter.h"
#include "internalcomm.h"
#include "world.h"
#include "protocal/msgfb.h"
#include "npc/transmit/fbmanager.hpp"
#include "item/itempool.h"
#include "obj/otherobj/fallingitem.h"

ChallengeFBManager & ChallengeFBManager::Instance()
{
	static ChallengeFBManager tefbm;
	return tefbm;
}

ChallengeFBManager::ChallengeFBManager()
{

}

ChallengeFBManager::~ChallengeFBManager()
{

}

void ChallengeFBManager::Update(unsigned long interval)
{
	for (ChallengeFBUserMapIt it = m_challengefb_user_map.begin(); it != m_challengefb_user_map.end();)
	{
		if (it->second.is_all_finish)
		{
			m_challengefb_user_map.erase(it++);
		}
		else
		{
			++it;
		}
	}

}

void ChallengeFBManager::OnEnter(Role *role)
{
	Challenge_data data;
	data.is_all_finish = false;
	memset(data.drop_list, 0, sizeof(data.drop_list));
	if (m_challengefb_user_map.find(role->GetUserId()) == m_challengefb_user_map.end())
	{
		m_challengefb_user_map[role->GetUserId()] = data;
	}
}

void ChallengeFBManager::OnFbFinish(Role *role)
{
	ChallengeFBUserMapIt it = m_challengefb_user_map.find(role->GetUserId());
	if ( it != m_challengefb_user_map.end())
	{
		it->second.is_all_finish = true;
	}
}

void ChallengeFBManager::OnPickFallingItem(Role *role, FallingItem *falling_item)
{
	ChallengeFBManager::ChallengeFBUserMapIt it = ChallengeFBManager::Instance().m_challengefb_user_map.find(role->GetUserId());
	if (it == ChallengeFBManager::Instance().m_challengefb_user_map.end())
	{
		return;
	}

	if (NULL != role && NULL != falling_item)
	{
		const ItemBase *item = ITEMPOOL->GetItem(falling_item->GetItemId());
		if (NULL != item)
		{
			bool is_find = false;

			for (int i = 0; i < it->second.drop_item_count && i < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++i)
			{
				if (it->second.drop_list[i].item_id == falling_item->GetItemId())
				{
					it->second.drop_list[i].num += falling_item->GetItemNum();
					is_find = true;

					break;
				}
			}

			if (!is_find && it->second.drop_item_count < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM)
			{
				it->second.drop_list[it->second.drop_item_count].item_id = falling_item->GetItemId();
				it->second.drop_list[it->second.drop_item_count].num = falling_item->GetItemNum();

				++(it->second.drop_item_count);
			}
		}
	}
}

int ChallengeFBManager::GetTotalFallItem(Role *role, ItemDataWrapper *total_fall_item_list)
{
	if (role == nullptr || total_fall_item_list == nullptr) return 0;
	ChallengeFBManager::ChallengeFBUserMapIt it = ChallengeFBManager::Instance().m_challengefb_user_map.find(role->GetUserId());
	if (it == m_challengefb_user_map.end()) return 0;

	int item_count = 0;
	for (int i = 0; i < it->second.drop_item_count && i < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++i)
	{
		if (it->second.drop_list[i].item_id > 0)
		{
			total_fall_item_list[item_count].item_id = it->second.drop_list[i].item_id;
			total_fall_item_list[item_count].num = it->second.drop_list[i].num;
			++item_count;
		}
	}
	return item_count;
}