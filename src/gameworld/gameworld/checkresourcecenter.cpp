#include "checkresourcecenter.hpp"

#include "equipment/equipment.h"
#include "globalconfig/globalconfig.h"

#include "monster/monsterpool.h"

#include "task/taskpool.h"

#include "item/itempool.h"
#include "item/other/otheritem.hpp"
#include "item/expense/norexitem.hpp"

#include "gameworld/other/shop/shop.hpp"
#include "gameworld/scene/sceneregister/sceneregister.hpp"

#include "config/logicconfigmanager.hpp"
#include "equipment/suitbase.hpp"

#include "skill/skillpool.hpp"
#include "skill/bossskillconditionpool.hpp"
#include "other/shop/shop.hpp"

CheckResourceCenter::CheckResourceCenter()
{

}

CheckResourceCenter::~CheckResourceCenter()
{

}

CheckResourceCenter & CheckResourceCenter::Instance()
{
	static CheckResourceCenter check_res;
	return check_res;
}

bool CheckResourceCenter::Check(std::string *err)
{
	bool ret = this->CheckHelper(err);
	this->Clear();

	return ret;
}

bool CheckResourceCenter::CheckHelper(std::string *err)
{
	if (!m_suit_check.Check(err)) return false;
	if (!m_pet_egg_check.Check(err)) return false;
	if (!m_task_check.Check(err)) return false;
	if (!m_item_valid_check.Check(err)) return false;
	if (!m_item_task_check.Check(err)) return false;
	if (!m_npc_task_check.Check(err)) return false;
	if (!m_skill_call_monster_check.Check(err)) return false;
	if (!m_item_sell_check.Check(err)) return false;
	if (!m_other_sex_item_check.Check(err)) return false;
	if (!m_monster_call_monster_check.Check(err)) return false;
	if (!m_scene_check.Check(err)) return false;
	if (!m_pet_zizhi_check.Check(err)) return false;
	if (!m_norex_check.Check(err)) return false;
	if (!m_worldevent_check.Check(err)) return false;
	if (!m_condskill_check.Check(err)) return false;

	return true;
}

void CheckResourceCenter::Clear()
{
	m_suit_check.equip_suit_vec.clear();
	m_pet_egg_check.pet_vec.clear();
	m_task_check.task_vec.clear();
	m_item_valid_check.item_vec.clear();
	m_item_task_check.item_task_vec.clear();
	m_npc_task_check.npc_task_vec.clear();
	m_skill_call_monster_check.skil_monster_vec.clear();
	m_item_sell_check.item_vec.clear();
	m_other_sex_item_check.item_vec.clear();
	m_monster_call_monster_check.monster_call_monster_vec.clear();
	m_scene_check.scene_vec.clear();
	m_pet_zizhi_check.pet_vec.clear();
	m_norex_check.item_vec.clear();
	m_worldevent_check.worldevent_vec.clear();
	m_condskill_check.cond_vec.clear();
}

//////////////////////////////////////////////////////////////////////////

void CheckResourceCenter::SuitCheckStruct::Add(ItemID equip_id, UInt16 suit_id)
{
	equip_suit_vec.push_back(std::make_pair(equip_id, suit_id));
}

bool CheckResourceCenter::SuitCheckStruct::Check(std::string *err)
{
	char errinfo[1024] = {0};

	for (size_t i = 0 ; i < equip_suit_vec.size() ; i ++)
	{
		ItemID equip_id = equip_suit_vec[i].first;
		if (0 == equip_id) break;

		UInt16 suit_id = equip_suit_vec[i].second;
		if (NULL == LOGIC_CONFIG->GetEquipOtherConfig().GetSuit(suit_id))
		{
			sprintf(errinfo, "[SuitCheck] equip_id = %d, suit_id = %d is invalid !", equip_id, suit_id);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

void CheckResourceCenter::PetEggCheckStruct::Add(ItemID item_id, UInt16 pet_config_id)
{
	pet_vec.push_back(std::make_pair(item_id, pet_config_id));
}

bool CheckResourceCenter::PetEggCheckStruct::Check(std::string *err)
{
	//char errinfo[1024] = {0};

	//const globalconfig::PetConfig::CreateConfig & create_config = GLOBALCONFIG->GetPetConfig().create_table;

	//for (size_t i = 0 ; i < pet_vec.size() ; i ++)
	//{
	//	ItemID item_id = pet_vec[i].first;
	//	if (0 == item_id) break;

	//	UInt16 pet_config_id = pet_vec[i].second;
	//	if (NULL == create_config.GetConfigData(pet_config_id))
	//	{
	//		sprintf(errinfo, "[PetEgg] item_id = %d, pet_config_id = %d is invalid !", item_id, pet_config_id);
	//		*err = errinfo;
	//		return false;
	//	}
	//}

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CheckResourceCenter::TaskCheckStruct::Add(TaskID task_id, bool is_must_common_task)
{
	task_vec.push_back(CheckItem(task_id, is_must_common_task));
}

bool CheckResourceCenter::TaskCheckStruct::Check(std::string *err)
{
	char errinfo[1024] = {0};
	
	for (size_t i = 0; i < task_vec.size(); i++)
	{
		TaskID task_id = task_vec[i].task_id;

		const TaskCfg *task = TASKPOOL->GetCfgByTaskID(task_id);
		if (NULL == task)
		{
			sprintf(errinfo, "[Task] task_id = %d is invalid !", (int)task_id);
			*err = errinfo;
			//return false;
		}

		/*if (task_vec[i].is_must_common_task && 
			!(TASK_TYPE_TRUNK == task->task_type || TASK_TYPE_BRANCH == task->task_type))
		{
			sprintf(errinfo, "[Task] task_id = %d, task type is not TT_TRUNK or TT_BRANCH or TT_DAY !", (int)task_id);
			*err = errinfo;
			return false;
		}*/
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CheckResourceCenter::ItemValidCheckStruct::Add(ItemID item_owner, ItemID item_id, int num)
{
	item_vec.push_back(CheckItem(item_owner, item_id, num));
}

bool CheckResourceCenter::ItemValidCheckStruct::Check(std::string *err)
{
	char errinfo[1024] = {0};

	for (std::vector<CheckItem>::iterator it = item_vec.begin(); it != item_vec.end(); ++it)
	{
		ItemID item_id = (*it).item_id;
		if (NULL == ITEMPOOL->GetItem(item_id))
		{
			sprintf(errinfo, "[ItemIdInvalid] item_id = %d num = %d item_owner = %d !", item_id, (*it).num, (*it).item_owner);
			*err = errinfo;
			return false;
		}

		if (!ItemBase::CanMerge(item_id) && (*it).num > 1)
		{
			sprintf(errinfo, "[ItemNumError] item_id = %d  num = %d item_owner = %d !", item_id, (*it).num, (*it).item_owner);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CheckResourceCenter::CheckPackage(bool is_hp_package, int value, int interval)
{
	PackageCheckStruct *package_check = is_hp_package ? &m_hp_package_check : &m_mp_package_check; 

	if (!package_check->is_init)
	{
		package_check->is_init = true;
		package_check->value = value;
		package_check->interval = interval;

		return true;
	}

	if (package_check->value != value || package_check->interval != interval) return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CheckResourceCenter::ItemTaskCheckStruct::Add(ItemID item_id, TaskID task_id)
{
	item_task_vec.push_back(std::make_pair(item_id, task_id));
}

bool CheckResourceCenter::ItemTaskCheckStruct::Check(std::string *err)
{
	char errinfo[1024] = {0};

	for (std::vector<std::pair<ItemID, TaskID> >::iterator it = item_task_vec.begin(); it != item_task_vec.end(); ++it)
	{
		TaskID task_id = (*it).second;
		if (NULL == TASKPOOL->GetCfgByTaskID(task_id))
		{
			sprintf(errinfo, "[TaskIdInvalid] item_id = %d task_id = %d !", (int)it->first, (int)task_id);
			*err = errinfo;
			//return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CheckResourceCenter::NPCTaskCheckStruct::Add(UInt16 npc_id, TaskID task_id)
{
	npc_task_vec.push_back(std::make_pair(npc_id, task_id));
}

bool CheckResourceCenter::NPCTaskCheckStruct::Check(std::string *err)
{
	char errinfo[1024] = {0};

	for (std::vector<std::pair<UInt16, TaskID> >::iterator it = npc_task_vec.begin(); it != npc_task_vec.end(); ++it)
	{
		TaskID task_id = (*it).second;
		if (NULL == TASKPOOL->GetCfgByTaskID(task_id))
		{
			sprintf(errinfo, "[TaskIdInvalid] npc_id = %d task_id = %d !", (int)it->first, (int)task_id);
			*err = errinfo;
			//return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CheckResourceCenter::SkillCallMonsterCheckStruct::Add(UInt16 skill_id, int monster_id)
{
	skil_monster_vec.push_back(std::pair<UInt16, int>(skill_id, monster_id));
}

bool CheckResourceCenter::SkillCallMonsterCheckStruct::Check(std::string *err)
{
	char errinfo[1024] = {0};

	for (std::vector<std::pair<UInt16, int> >::iterator it = skil_monster_vec.begin(); it != skil_monster_vec.end(); ++it)
	{
		UInt16 monster_id = (*it).second;
		if (NULL == MONSTERPOOL->GetMonsterParam(monster_id))
		{
			sprintf(errinfo, "[SkillMonsterIdInvalid] skill_id = %d monster_id = %d!", (*it).first, monster_id);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CheckResourceCenter::ItemSellCheckStruct::Add(ItemID item_id)
{
	item_vec.push_back(item_id);
}

bool CheckResourceCenter::ItemSellCheckStruct::Check(std::string *err)
{
	for (std::vector<ItemID>::iterator it = item_vec.begin(); it != item_vec.end(); ++it)
	{
		ItemID item_id = *it;

		int price = 0;
		int bind_price = 0;
		if (!LOGIC_CONFIG->GetShop().GetShopItemGoldPrice(item_id, &price, &bind_price))
		{
			char errinfo[1024] = {0};

			sprintf(errinfo, "[ItemSellCheckStruct::Check] item_id = %d not exist in shop !", item_id);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

void CheckResourceCenter::OtherSexItemCheckStruct::Add(ItemID item_id, ItemID other_sex_itemid)
{
	item_vec.push_back(std::pair<ItemID, ItemID>(item_id, other_sex_itemid));
}

bool CheckResourceCenter::OtherSexItemCheckStruct::Check(std::string *err)
{
	for (std::vector<std::pair<ItemID, ItemID> >::iterator it = item_vec.begin(); it != item_vec.end(); ++it)
	{
		const ItemBase *src_item = ITEMPOOL->GetItem(it->first);
		const ItemBase *tar_item = ITEMPOOL->GetItem(it->second);

		if (NULL == src_item || NULL == tar_item || src_item->GetItemId() != tar_item->GetOtherSexItemId() || src_item->GetItemType() != tar_item->GetItemType())
		{
			char errinfo[1024] = {0};

			sprintf(errinfo, "[OtherSexItemCheckStruct::Check] item_id = %d , other_sex_itemid = %d", (int)it->first, (int)it->second);
			*err = errinfo;
			return false;
		}

		if (tar_item->GetItemType() == ItemBase::I_TYPE_EQUIPMENT && !src_item->CheckIsSameItemExceptSex(tar_item))
		{
			char errinfo[1024] = {0};

			sprintf(errinfo, "[OtherSexItemCheckStruct::Check] item_id = %d , other_sex_itemid = %d", (int)it->first, (int)it->second);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CheckResourceCenter::MonsterCallMonsterCheckStruct::Add(UInt16 monster_id, UInt16 call_monster_id)
{
	monster_call_monster_vec.push_back(std::pair<UInt16, UInt16>(monster_id, call_monster_id));
}

bool CheckResourceCenter::MonsterCallMonsterCheckStruct::Check(std::string *err)
{
	for (std::vector<std::pair<UInt16, UInt16> >::iterator it = monster_call_monster_vec.begin(); it != monster_call_monster_vec.end(); ++it)
	{
		UInt16 call_monster_id = (*it).second;
		if (!MONSTERPOOL->IsMonsterExist(call_monster_id))
		{
			char errinfo[1024] = {0};

			sprintf(errinfo, "[MonsterCallMonsterCheckStruct::Check] monster_id = %d call_monster_id = %d not exist !", (*it).first, call_monster_id);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CheckResourceCenter::SceneCheckStruct::Add(int scene_id)
{
	scene_vec.push_back(scene_id);
}

bool CheckResourceCenter::SceneCheckStruct::Check(std::string *err)
{
	for (std::vector<int>::iterator it = scene_vec.begin(); it != scene_vec.end(); ++it)
	{
		int scene_id = *it;
		if (!SceneRegister::Instance().CheckSceneIsExist(scene_id))
		{
			char errinfo[1024] = {0};

			sprintf(errinfo, "[SceneCheckStruct::Check] scene_id = %d not exist !", scene_id);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

void CheckResourceCenter::PetZizhiCheckStruct::Add(short pet_id)
{
	pet_vec.push_back(pet_id);
}

bool CheckResourceCenter::PetZizhiCheckStruct::Check(std::string *err)
{
	
	return true;
}

void CheckResourceCenter::NorexItemCheckStruct::Add(ItemID item_id)
{
	item_vec.push_back(item_id);
}

bool CheckResourceCenter::NorexItemCheckStruct::Check(std::string *err)
{
	char errinfo[1024] = {0};

	for (std::vector<ItemID>::iterator it = item_vec.begin(); it != item_vec.end(); ++it)
	{
		ItemID item_id = *it;

		const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
		if (NULL == itembase || ItemBase::I_TYPE_EXPENSE != itembase->GetItemType())
		{
			sprintf(errinfo, "[NorexItemCheckStruct::Check] item_id = %d not exist !", item_id);
			*err = errinfo;
			return false;
		}

		const NorexItem *norex_item = (const NorexItem *)itembase;

		short use_type = norex_item->GetUseType();
		//int param1 = norex_item->GetParam1();
		/*int param2 = norex_item->GetParam2();*/

		switch (use_type)
		{
		/*case NorexItem::I_NOREX_ITEM_MOUNT_CARD:
			{
				if (NULL == LOGIC_CONFIG->GetMountOpConfig().GetMountImage(param1))
				{
					sprintf(errinfo, "I_NOREX_ITEM_MOUNT_CARD item %d mountid = %d not exist !", item_id, param1);
					*err = errinfo;
					return false;
				}
			}
			break;*/

		/*case NorexItem::I_NOREX_ITEM_SKILLBOOK:
			{
				if (NULL == SKILLPOOL->GetSkill(param1))
				{
					sprintf(errinfo, "I_NOREX_ITEM_SKILLBOOK item %d skill %d not exist !", item_id, param1);
					*err = errinfo;
					return false;
				}
			}
			break;*/

		case NorexItem::I_NOREX_ITEM_TITLE_CARD:
			{
				/*if (NULL == LOGIC_CONFIG->GetTitlePool().GetTitleCfg(param1))
				{
					sprintf(errinfo, "I_NOREX_ITEM_TITLE_CARD item %d title %d not exist !", item_id, param1);
					*err = errinfo;
					return false;
				}*/
			}
			break;

		default:
			break;
		}
	}

	return true;
}

void CheckResourceCenter::WorldEventCheckStruct::Add(unsigned short event_id)
{
	worldevent_vec.push_back(event_id);
}

bool CheckResourceCenter::WorldEventCheckStruct::Check(std::string *err)
{
	

	return true;
}

void CheckResourceCenter::BossCondSkillCheckStruct::Add(UInt16 monster_id, unsigned short cond_id)
{
	cond_vec.push_back(std::pair<UInt16, UInt16>(monster_id, cond_id));
}

bool CheckResourceCenter::BossCondSkillCheckStruct::Check(std::string *err)
{
	char errinfo[1024] = {0};

	for (std::vector<std::pair<UInt16, unsigned short> >::iterator it = cond_vec.begin(); it != cond_vec.end(); ++it)
	{
		unsigned short cond_id = (*it).second;

		if (NULL == LOGIC_CONFIG->GetBossSkillConditionPool().GetBossSkillConditionConfig(cond_id))
		{
			sprintf(errinfo, "[BossCondSkillCheckStruct::Check] monster:%d cond_id = %d not exist !", (*it).first, cond_id);
			*err = errinfo;
			return false;
		}
	}

	return true;
}
