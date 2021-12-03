#include "droppool.hpp"

#include "servercommon/configcommon.h"

#include "globalconfig/globalconfig.h"

#include "obj/otherobj/fallingitem.h"
#include "obj/character/role.h"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/monsterdropsum/monsterdropsum.hpp"

#include "scene/map.h"
#include "scene/scene.h"

#include "scene/speciallogic/speciallogic.hpp"

#include "gameworld/gamelog.h"

#include "gamecommon.h"

#include "engineadapter.h"
#include "world.h"
#include "servercommon/commondata.hpp"
#include "gameworld/gameworld/config/logicconfigmanager.hpp"
#include "gameworld/gameworld/config/dropextendconfig.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
DropPool *gs_drop_pool_config = NULL;

DropPool * DropPool::Instance()
{
	if (NULL == gs_drop_pool_config)
	{
		gs_drop_pool_config = new DropPool();
	}

	return gs_drop_pool_config;
}

DropPool::DropPool()
{
	memset(m_drop_list, 0, sizeof(m_drop_list));
}

DropPool::~DropPool()
{
	this->Release();
}

void DropPool::Release()
{
	for (int i = 0 ; i < MAX_DROP_ID; ++i)
	{
		if (NULL != m_drop_list[i])
		{
			delete m_drop_list[i];
		}
	}
	memset(m_drop_list, 0, sizeof(m_drop_list));
}

bool DropPool::Init(const std::string &dir, const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};

	TiXmlDocument document;

	if (configname == "" || !document.LoadFile(dir+configname))
	{
		sprintf(errinfo,"%s: Load DropPool Config Error, %s.", (dir+configname).c_str(), document.ErrorDesc());
		*err = errinfo;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	TiXmlElement *currentElement = NULL;

	if (NULL == RootElement)
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	currentElement = RootElement->FirstChildElement("path");
	while (NULL != currentElement)
	{
		std::string path;

		if (!GetNodeValue(currentElement, path))
		{
			this->Release();
			return false;
		}

		DropConfig *dropconfig = new DropConfig();
		if (!dropconfig->LoadConfig((dir+path).c_str(),err))
		{
			delete dropconfig;
			this->Release();
			return false;
		}

		if (0 != m_drop_list[dropconfig->GetDropID()])
		{
			sprintf(errinfo,"%s: Load Drop Pool Config Error, drop id [%d] is repeat.", (dir+configname).c_str(), dropconfig->GetDropID());
			*err = errinfo;

			delete dropconfig;
			this->Release();
			return false;
		}

		m_drop_list[dropconfig->GetDropID()] = dropconfig;

		currentElement = currentElement->NextSiblingElement();
	}

	return true;
}

bool DropPool::Reload(const std::string &config_dir, const std::string &configname, std::string *err)
{
	DropPool *temp = new DropPool();

	if (!temp->Init(config_dir, configname, err))
	{
		delete temp; temp = NULL;
		return false;
	}

	if (NULL != gs_drop_pool_config) delete gs_drop_pool_config;

	gs_drop_pool_config = temp;

	return true;
}

DropConfig * DropPool::GetDropConfig(UInt16 drop_id)
{
	if (drop_id <= 0 || drop_id >= MAX_DROP_ID) return NULL;
	return m_drop_list[drop_id];
}

void DropPool::DropHelper(UInt16 dropid, Scene *scene, const Posi &pos, const UserID &user_id, char prof, UInt16 monster_id, bool is_log, bool is_from_body, int *dorp_index)
{
	this->Drop(dropid, scene, pos, user_id, prof, monster_id, is_log, is_from_body, dorp_index);
}

void DropPool::DropToHelper(Scene *scene, UInt16 item_id, int item_num, bool item_is_bind, int coin_or_gold, const Posi &pos, const UserID &owner_user_id, 
							UInt16 monster_id, bool is_log, short is_broadcast, bool is_from_body, int *dorp_index)
{
	if (NULL == scene) return;
	if (COIN_ITEM_ID != item_id && BIND_GOLD_ITEM_ID != item_id && NULL == ITEMPOOL->GetItem(item_id)) return;

	Posi drop_pos = pos;

	if (NULL != dorp_index)
	{
		// 位置偏移
		static Posi POS_OFFSET_LIST[] = {
			Posi(0, 0),   Posi(-1, 0),  Posi(-1, -1), Posi(0, -1),  Posi(1, -1),  Posi(1, 0),   Posi(1, 1),   Posi(0, 1),   Posi(-1, 1), 
			Posi(-1, -2), Posi(0, -2),  Posi(1, -2),  Posi(2, -2),  Posi(2, -1),  Posi(2, 0),   Posi(2, 1),   Posi(2, 2),   Posi(1, 2), 
			Posi(0, 2),   Posi(-1, 2),  Posi(-2, 2),  Posi(-2, 1),  Posi(-2, 0),  Posi(-2, -1), Posi(-2, -2), Posi(-2, -3), Posi(-1, -3), 
			Posi(0, -3),  Posi(1, -3),  Posi(2, -3),  Posi(3, -3),  Posi(3, -2),  Posi(3, -1),  Posi(3, 0),   Posi(3, 1),   Posi(3, 2), 
			Posi(3, 3),   Posi(2, 3),   Posi(1, 3),   Posi(0, 3),   Posi(-1, 3),  Posi(-2, 3),  Posi(-3, 3),  Posi(-3, 2),  Posi(-3, 1), 
			Posi(-3, 0),  Posi(-3, -1), Posi(-3, -2), Posi(-3, -3), Posi(-3, -4), Posi(-2, -4), Posi(-1, -4), Posi(0, -4),  Posi(1, -4),
			Posi(2, -4),  Posi(3, -4),  Posi(4, -4),  Posi(4, -3),  Posi(4, -2),  Posi(4, -1),  Posi(4, 0),   Posi(4, 1),   Posi(4, 2),
			Posi(4, 3),   Posi(4, 4),   Posi(3, 4),   Posi(2, 4),   Posi(1, 4),   Posi(0, 4),   Posi(-1, 4),  Posi(-2, 4),  Posi(-3, 4),
			Posi(-4, 4),  Posi(-4, 3),  Posi(-4, 2),  Posi(-4, 1),  Posi(-4, 0),  Posi(-4, -1), Posi(-4, -2), Posi(-4, 3),  Posi(-4, -4)
		};

		static int MAX_POS_COUNT = sizeof(POS_OFFSET_LIST) / sizeof(POS_OFFSET_LIST[0]);
		static bool is_init = false;
		if (!is_init)
		{
			is_init = true;
			unsigned int extradrop_distance = LOGIC_CONFIG->GetDropExtendCfg().GetOtherCfg()->extradrop_distance;
			for (int i = 0; i < MAX_POS_COUNT; i++)
			{
				POS_OFFSET_LIST[i].x *= extradrop_distance;
				POS_OFFSET_LIST[i].y *= extradrop_distance;
			}
		}

		// 最多20次，找到一个有效位置
		for (int i = 0; i < 20; i++)
		{
			*dorp_index = (*dorp_index) % MAX_POS_COUNT;
			drop_pos.x = pos.x + POS_OFFSET_LIST[*dorp_index].x;
			drop_pos.y = pos.y + POS_OFFSET_LIST[*dorp_index].y;
			(*dorp_index)++;

			if (scene->GetMap()->IsValidatePos(Obj::OBJ_TYPE_FALLINGITEM, drop_pos.x, drop_pos.y))
			{
				break;
			}
		}
	}
	else
	{
		drop_pos = gamecommon::GetDisRandPos(pos, 2);
		if (!scene->GetMap()->IsValidatePos(Obj::OBJ_TYPE_FALLINGITEM, drop_pos.x, drop_pos.y))
		{
			drop_pos = pos;
		}
	}

	unsigned int disappear_time = GLOBALCONFIG->GetOtherConfig_ItemDrop_Disappear_Time();
	if (!scene->InStaticScene())
	{
		disappear_time = GLOBALCONFIG->GetOtherConfig_ItemDrop_Disappear_Time_Non_Static_Scene();
	}
	
	FallingItem *fallingitem = new FallingItem();
	fallingitem->Init(item_id, item_num, item_is_bind, coin_or_gold, owner_user_id, scene->GetSceneID(), disappear_time, monster_id, is_broadcast, is_from_body);
	fallingitem->SetPos(drop_pos);
	fallingitem->SetObjPosi(pos);
	scene->AddObj(fallingitem);

	if (27800 <= item_id && 27803 >= item_id)
	{
		Role *drop_role = World::GetInstWorld()->GetSceneManager()->GetRole(owner_user_id);
		if (NULL != drop_role)
		{
			CommonDataParam *common_data = drop_role->GetCommonDataParam();
			++common_data->collect_drop_num;
		}
	}

	if (ItemBase::INVALID_ITEM_ID != item_id && BIND_GOLD_ITEM_ID != item_id && COIN_ITEM_ID != item_id)
	{
		if (is_log)
		{
			gamelog::g_log_drop.printf(LL_INFO, "monster[%d] drop [item_id:%d is_bind:%d] at [scene[%d] pos[%d, %d] time[%u]] to user[%d]",
				(int)monster_id, (int)item_id, (item_is_bind ? 1 : 0), scene->GetSceneID(), drop_pos.x, drop_pos.y, 
				static_cast<unsigned int>(EngineAdapter::Instance().Time()), UidToInt(owner_user_id));
		}	
	}
}

void DropPool::DropTest(UInt16 dropid, Scene *scene, const Posi &pos, const UserID &user_id, UInt16 monster_id, bool is_log)
{
	DropConfig *drop_config = DROPPOOL->GetDropConfig(dropid);
	if (NULL == drop_config) return;

	DropItemCfg item_list[DropConfig::DROP_ITEM_PROB_MAX];

	int item_count = drop_config->RandDropItem(0, item_list);
	item_count = drop_config->GetFixDropItem(item_count, item_list);

	if (item_count > 0 && item_count <= DropConfig::DROP_ITEM_PROB_MAX)
	{
		for (int i = 0; i < item_count; i++)
		{
			if (COIN_ITEM_ID == item_list[i].item_config_data.item_id)
			{
				MonsterDropSum::Instance().OnMonsterDropItem(monster_id, COIN_ITEM_ID, 1);
			}
			else
			{
				const ItemBase *item_base = ITEMPOOL->GetItem(item_list[i].item_config_data.item_id);
				if (NULL != item_base)
				{
					MonsterDropSum::Instance().OnMonsterDropItem(monster_id, item_list[i].item_config_data.item_id, item_list[i].item_config_data.num);
				}
			}
		}
	}
}

// 获取掉落物品列表
int DropPool::GetDropItemList(UInt16 dropid, ItemConfigData item_list[DropConfig::DROP_ITEM_PROB_MAX])
{
	if (NULL == item_list) return 0;

	DropConfig *drop_config = DROPPOOL->GetDropConfig(dropid);
	if (NULL == drop_config) return 0;

	DropItemCfg drop_list[DropConfig::DROP_ITEM_PROB_MAX];

	int item_count = drop_config->RandDropItem(0, drop_list);
	item_count = drop_config->GetFixDropItem(item_count, drop_list);

	if (item_count > DropConfig::DROP_ITEM_PROB_MAX)
	{
		item_count = DropConfig::DROP_ITEM_PROB_MAX;
	}

	for (int i = 0; i < item_count; ++i)
	{
		item_list[i] = drop_list[i].item_config_data;
	}

	return item_count;
}

// 执行掉落
bool DropPool::Drop(UInt16 dropid, Scene *scene, const Posi &pos, const UserID &user_id, char prof, UInt16 monster_id, bool is_log, bool is_from_body, int *dorp_index)
{
	DropConfig *drop_config = DROPPOOL->GetDropConfig(dropid);
	if (NULL == drop_config) return false;

	DropItemCfg item_list[DropConfig::DROP_ITEM_PROB_MAX];

	int item_count = drop_config->RandDropItem(prof, item_list);
	item_count = drop_config->GetFixDropItem(item_count, item_list);

	if (item_count > 0 && item_count <= DropConfig::DROP_ITEM_PROB_MAX)
	{
		for (int i = 0; i < item_count; i++)
		{
			if (COIN_ITEM_ID == item_list[i].item_config_data.item_id)
			{
				this->DropToHelper(scene, COIN_ITEM_ID, 1, item_list[i].item_config_data.is_bind, item_list[i].item_config_data.num, pos, user_id, 
						monster_id, is_log, FALLING_ITEM_BROADCAST_TYPE_NO_BROADCAST, is_from_body, dorp_index);
			}
			else
			{
				const ItemBase *item_base = ITEMPOOL->GetItem(item_list[i].item_config_data.item_id);
				if (NULL != item_base)
				{
					this->DropToHelper(scene, item_list[i].item_config_data.item_id, item_list[i].item_config_data.num, item_list[i].item_config_data.is_bind, 0,
						pos, user_id, monster_id, is_log, item_list[i].broadcast, is_from_body, dorp_index);
				}
			}
		}
	}

	return true;
}

