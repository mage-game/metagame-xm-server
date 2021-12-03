#include "jinglingadvantage.hpp"
#include "jinglingadvantageconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "gameworld/engineadapter.h"
#include "obj/otherobj/gatherobj.h"
#include "obj/character/role.h"
#include "gameworld/world.h"
#include "gameworld/scene/scene.h"
#include "gameworld/item/knapsack.h"
#include "protocal/msgjingling.h"
#include "servercommon/commondata.hpp"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/errornum.h"
#include "equipment/equipmentmanager.h"
#include "config/monsterreplaceconfig.hpp"
#include "monster/monsterpool.h"
#include "obj/character/role.h"
#include "other/vip/vip.hpp"
#include "other/event/eventhandler.hpp"
#include "item/gift/randgift.hpp"

JinglingAdvantage::JinglingAdvantage()
{

}

JinglingAdvantage::~JinglingAdvantage()
{

}

JinglingAdvantage & JinglingAdvantage::Instance()
{
	static JinglingAdvantage obj;
	return obj;
}

void JinglingAdvantage::OnServerStart()
{
	const std::vector<JinglingAdvantageGatherPosCfg> &pos_cfg_vec = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageGatherRandPosCfg();
	for (std::vector<JinglingAdvantageGatherPosCfg>::const_iterator it = pos_cfg_vec.begin(); it != pos_cfg_vec.end(); it++)
	{
		GatherScene &info = m_gather_scene_map[it->scene_id];
		info.scene_id = it->scene_id;
		GatherScene::EggPos egg_pos_info;
		egg_pos_info.exist = 0;
		egg_pos_info.pos = it->pos;
		info.egg_pos_vec.push_back(egg_pos_info);
	}
}

void JinglingAdvantage::OnAdvantageReq(Role *role)
{
	if (NULL != role)
	{
		this->SyncGatherSceneInfo(role);
		this->SynJinglingCountToRole(role);
	}
}

bool JinglingAdvantage::CanGather(Role *role, int gather_id)
{
	if (role == nullptr) return false;

	const JinglingAdvantageGatherCfg *gather = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageGatherCfg(gather_id);
	if (NULL == gather)
	{
		return true;
	}

	if (!role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	const JinglingAdvantageOtherCfg *other_cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageOtherCfg();
	if (NULL != other_cfg && role->GetLevel() < other_cfg->gather_level_limit)
	{
		role->NoticeNum(errornum::EN_JINGLING_ADVANTAGE_LEVEL_LIMIT);
		return false;
	}

	if (JinglingAdvantageGatherCfg::JINGLING_ADVANTAGE_GATHER_TYPE_BLUE == gather->gather_type)
	{
		if (NULL != other_cfg && role->GetCommonDataParam()->jingling_advantage_gather_blue >= other_cfg->times)
		{
			role->NoticeNum(errornum::EN_JINGLING_ADVANTAGE_TIMES_LIMIT);
			return false;
		}
	}

	return true;
}

void JinglingAdvantage::Update(unsigned int now_second)
{
	bool is_change = false;

	const JinglingAdvantageOtherCfg *other_cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageOtherCfg();
	for (std::map<int, GatherScene>::iterator it = m_gather_scene_map.begin(); it != m_gather_scene_map.end(); it++)
	{
		GatherScene &info = it->second;

		int gather_count = info.blue_count + info.purple_count;
		if (gather_count >= other_cfg->min_num) continue;

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(info.scene_id, 0);
		if (NULL == scene) continue;

		for (int i = 0; i < other_cfg->max_num - gather_count && i < 1000; i++)
		{
			if (!this->RefreshOneGather(scene, info, JinglingAdvantageGatherCfg::JINGLING_ADVANTAGE_GATHER_TYPE_BLUE))
			{
				break;
			}

			is_change = true;
		}
	}

	if (is_change)
	{
		this->SyncGatherSceneInfo();
	}
}

void JinglingAdvantage::OnGather(Role *role, int gather_id, const Posi &gather_pos)
{
	if (nullptr == role)
	{
		return;
	}

	const JinglingAdvantageGatherCfg *gather_cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageGatherCfg(gather_id);
	if (NULL == gather_cfg)
	{
		return;
	}

	if (!role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	std::map<int, GatherScene>::iterator it = m_gather_scene_map.find(role->GetScene()->GetSceneID());
	if (it == m_gather_scene_map.end())
	{
		return;
	}
	GatherScene &info = it->second;
	for (int i = 0; i < (int)info.egg_pos_vec.size(); ++i)
	{
		if (gather_pos == info.egg_pos_vec[i].pos)
		{
			info.egg_pos_vec[i].exist = 0;
			break;
		}
	}

	static ItemGridData item_grid_data; item_grid_data.Reset();
	int grid_index = -1;
	role->GetKnapsack()->Put(gather_cfg->reward_item, PUT_REASON_JINGLING_ADVANTAGE, &grid_index);
	if (grid_index >= 0)
	{
		if (role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data) &&
			EquipmentManager::GetFixSpecialAttr(&item_grid_data, 0, gather_cfg->reward_item.item_id))
		{
			role->GetKnapsack()->SetItemDirty(grid_index);
		}
	}

	if (JinglingAdvantageGatherCfg::JINGLING_ADVANTAGE_GATHER_TYPE_BLUE == gather_cfg->gather_type)
	{
		info.blue_count--;
		role->GetCommonDataParam()->jingling_advantage_gather_blue++;

		const JinglingAdvantageOtherCfg *other_cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageOtherCfg();
		if (RandomNum(10000) < other_cfg->rate)
		{
			if (this->RefreshOneGather(role->GetScene(), info, JinglingAdvantageGatherCfg::JINGLING_ADVANTAGE_GATHER_TYPE_PUEPLE))
			{
				int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_jingling_refresh_purple,
					role->GetUID(), role->GetName(), (int)role->GetCamp());
				if (sendlen > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}
	else
	{
		info.purple_count--;
	}

	this->CheckRefreshBoss(role, gather_pos);

	this->SynJinglingCountToRole(role);
	this->SyncGatherSceneInfo();
	EventHandler::Instance().OnJingLingGather(role, 1);
}

void JinglingAdvantage::OnSkipGather(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	const JinglingAdvantageOtherCfg *other_cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageOtherCfg();
	if (NULL == other_cfg || role == nullptr)
	{
		return;
	}

	if (role->GetLevel() < other_cfg->gather_level_limit || role->GetLevel() < other_cfg->skip_level_limit)
	{
		return;
	}

	int times = other_cfg->times - role->GetCommonDataParam()->jingling_advantage_gather_blue;
	if (times <= 0)
	{
		return;
	}

	if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(times))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg->skip_gather_consume * times, "Skip jingling_advantage blue gather"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	std::map<ItemID, ItemConfigData> item_grid_map;
	for (int i = 0; i < times; ++i)
	{
		const JinglingAdvantageGatherCfg *gather_cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageRandGatherCfg(JinglingAdvantageGatherCfg::JINGLING_ADVANTAGE_GATHER_TYPE_BLUE);
		if (NULL == gather_cfg)
		{
			continue;
		}
		ItemID item_id = gather_cfg->reward_item.item_id;
		const RandGift *rand_gift = dynamic_cast<const RandGift*>(ITEMPOOL->GetItem(item_id));
		//不是随机礼包就直接放背包里
		if (NULL == rand_gift)
		{
			static ItemGridData item_grid_data; item_grid_data.Reset();
			int grid_index = -1;
			role->GetKnapsack()->Put(gather_cfg->reward_item, PUT_REASON_JINGLING_ADVANTAGE, &grid_index);
			if (grid_index >= 0)
			{
				if (role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data) &&
					EquipmentManager::GetFixSpecialAttr(&item_grid_data, 0, gather_cfg->reward_item.item_id))
				{
					role->GetKnapsack()->SetItemDirty(grid_index);
				}
			}
			continue;
		}
		if (item_grid_map.find(item_id) == item_grid_map.end())
		{
			ItemConfigData local_item_config_data(item_id, gather_cfg->reward_item.is_bind, gather_cfg->reward_item.num);
			item_grid_map[item_id] = local_item_config_data;
		}
		else
		{
			item_grid_map[item_id].num += gather_cfg->reward_item.num;
		}
	}

	for (auto &temp_it : item_grid_map)
	{
		const RandGift *rand_gift = dynamic_cast<const RandGift*>(ITEMPOOL->GetItem(temp_it.first));
		if (NULL == rand_gift)
		{
			continue;
		}

		ItemGridData item_grid_data;
		item_grid_data.item_id = temp_it.second.item_id;
		item_grid_data.num = temp_it.second.num;
		item_grid_data.is_bind = temp_it.second.is_bind;
		rand_gift->Use(role, item_grid_data, item_grid_data.num);
	}

	role->GetCommonDataParam()->jingling_advantage_gather_blue += times;
	this->SynJinglingCountToRole(role);
	EventHandler::Instance().OnJingLingGather(role, times);
}

void JinglingAdvantage::OnSkipGather(Role * role, int skip_times)
{
	if (nullptr == role || skip_times <= 0)
	{
		return;
	}

	const JinglingAdvantageOtherCfg *other_cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageOtherCfg();
	if (NULL == other_cfg || role == nullptr)
	{
		return;
	}

	if (role->GetLevel() < other_cfg->gather_level_limit || role->GetLevel() < other_cfg->skip_level_limit)
	{
		return;
	}

	int times = other_cfg->times - role->GetCommonDataParam()->jingling_advantage_gather_blue;
	if (skip_times > times)
	{
		return;
	}
	times = skip_times;
	if (times <= 0)
	{
		return;
	}

	if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(times))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg->skip_gather_consume * times, "Skip jingling_advantage blue gather"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	std::map<ItemID, ItemConfigData> item_grid_map;
	for (int i = 0; i < times; ++i)
	{
		const JinglingAdvantageGatherCfg *gather_cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageRandGatherCfg(JinglingAdvantageGatherCfg::JINGLING_ADVANTAGE_GATHER_TYPE_BLUE);
		if (NULL == gather_cfg)
		{
			continue;
		}
		ItemID item_id = gather_cfg->reward_item.item_id;
		const RandGift *rand_gift = dynamic_cast<const RandGift*>(ITEMPOOL->GetItem(item_id));
		//不是随机礼包就直接放背包里
		if (NULL == rand_gift)
		{
			static ItemGridData item_grid_data; item_grid_data.Reset();
			int grid_index = -1;
			role->GetKnapsack()->Put(gather_cfg->reward_item, PUT_REASON_JINGLING_ADVANTAGE, &grid_index);
			if (grid_index >= 0)
			{
				if (role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data) &&
					EquipmentManager::GetFixSpecialAttr(&item_grid_data, 0, gather_cfg->reward_item.item_id))
				{
					role->GetKnapsack()->SetItemDirty(grid_index);
				}
			}
			continue;
		}
		if (item_grid_map.find(item_id) == item_grid_map.end())
		{
			ItemConfigData local_item_config_data(item_id, gather_cfg->reward_item.is_bind, gather_cfg->reward_item.num);
			item_grid_map[item_id] = local_item_config_data;
		}
		else
		{
			item_grid_map[item_id].num += gather_cfg->reward_item.num;
		}
	}

	for (auto &temp_it : item_grid_map)
	{
		const RandGift *rand_gift = dynamic_cast<const RandGift*>(ITEMPOOL->GetItem(temp_it.first));
		if (NULL == rand_gift)
		{
			continue;
		}

		ItemGridData item_grid_data;
		item_grid_data.item_id = temp_it.second.item_id;
		item_grid_data.num = temp_it.second.num;
		item_grid_data.is_bind = temp_it.second.is_bind;
		rand_gift->Use(role, item_grid_data, item_grid_data.num);
	}

	role->GetCommonDataParam()->jingling_advantage_gather_blue += times;
	this->SynJinglingCountToRole(role);
	EventHandler::Instance().OnJingLingGather(role, times);
}

bool JinglingAdvantage::RefreshOneGather(Scene *scene, GatherScene &info, int gather_type)
{
	const JinglingAdvantageGatherCfg *gather_cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageRandGatherCfg(gather_type);
	if (NULL == gather_cfg)
	{
		return false;
	}

	std::vector<int> pos_index_vec; pos_index_vec.clear();
	for (int i = 0; i < (int)info.egg_pos_vec.size(); ++i)
	{
		if (info.egg_pos_vec[i].exist == 0)
		{
			pos_index_vec.push_back(i);
		}
	}
	int pos_size = pos_index_vec.size();
	if (pos_size <= 0)
	{
		return false;
	}

	size_t pos_index = RandomNum(pos_size);
	if (pos_index < 0 || pos_index >= pos_index_vec.size())
	{
		return false;
	}

	int egg_pos_index = pos_index_vec[pos_index];

	Posi pos = info.egg_pos_vec[egg_pos_index].pos;
	info.egg_pos_vec[egg_pos_index].exist = 1;

	const JinglingAdvantageOtherCfg *other_cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageOtherCfg();

	GatherObj *gather_obj = new GatherObj();
	gather_obj->Init(NULL, gather_cfg->gather_id, other_cfg->gather_time, 0, true);
	gather_obj->SetPos(pos);
	scene->AddObj(gather_obj);

	if (JinglingAdvantageGatherCfg::JINGLING_ADVANTAGE_GATHER_TYPE_BLUE == gather_type)
	{
		info.blue_count++;
	}
	else
	{
		info.purple_count++;
	}

	return true;
}

void JinglingAdvantage::SyncGatherSceneInfo(Role *role)
{
	Protocol::SCJingLingAdvantageInfo jingling_advantage_info;

	int index = 0;
	for (std::map<int, GatherScene>::iterator iter = m_gather_scene_map.begin(); iter != m_gather_scene_map.end() && index < Protocol::SCJingLingAdvantageInfo::SCENE_COUNT; ++iter, ++index)
	{
		jingling_advantage_info.pos_list[index].scene_id = iter->second.scene_id;
		jingling_advantage_info.pos_list[index].purple_count = iter->second.purple_count;
		jingling_advantage_info.pos_list[index].blue_count = iter->second.blue_count;
	}

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&jingling_advantage_info, sizeof(jingling_advantage_info));
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&jingling_advantage_info, sizeof(jingling_advantage_info));
	}
}

void JinglingAdvantage::SynJinglingCountToRole(Role *role)
{
	if (NULL != role)
	{
		static Protocol::SCJingLingAdvantageCount pro;
		pro.today_gather_blue_jingling_count = role->GetCommonDataParam()->jingling_advantage_gather_blue;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
	}
}

void JinglingAdvantage::OnEnterBossScene(Role *role, int monster_id)
{
	if (nullptr != role)
	{
		for (auto iter = m_boss_scene_deque.rbegin(); iter != m_boss_scene_deque.rend(); ++iter)
		{
			if (iter->monster_id == monster_id)
			{
				World::GetInstWorld()->GetSceneManager()->GoTo(role, iter->scene_id, iter->scene_key, iter->pos);
				return;
			}
		}
	}
}

void JinglingAdvantage::CheckRefreshBoss(Role *role, const Posi &gather_pos)
{
	if (nullptr != role)
	{
		const JinglingAdvantageBossCfg *boss_cfg = LOGIC_CONFIG->GetJinglingAdvantageCfg().GetJinglingAdvantageBossCfg(role->GetVip()->GetVipLevel());
		if (nullptr != boss_cfg)
		{
			if (RandomNum(10000) < boss_cfg->rate)
			{
				int real_monster_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByWorldLevel(boss_cfg->monster_id);

				if (MONSTERPOOL->CreateMonster(real_monster_id, role->GetScene(), gather_pos))
				{
					static Protocol::SCJingLingAdvantageBossInfo pro;
					pro.monster_id = real_monster_id;
					pro.reserve_sh = 0;

					BossSceneCfg scene_cfg;
					scene_cfg.monster_id = real_monster_id;
					scene_cfg.pos.x = gather_pos.x;
					scene_cfg.pos.y = gather_pos.y;
					scene_cfg.scene_id = role->GetScene()->GetSceneID();
					scene_cfg.scene_key = role->GetScene()->GetSceneKey();
					scene_cfg.invalid_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + BOSS_SCENE_CFG_VALID_TIME_S;

					m_boss_scene_deque.push_back(scene_cfg);

					memcpy(pro.role_name, role->GetName(), sizeof(pro.role_name));
					pro.scene_id = scene_cfg.scene_id;
					pro.reserve_1 = 0;

					World::GetInstWorld()->GetSceneManager()->SendToVipPhaseUser((char *)&pro, sizeof(pro), boss_cfg->vip_level_min, boss_cfg->vip_level_max);
				}
			}
		}
	}
}

void JinglingAdvantage::OnGetEggPosBySceneId(Role *role, int scene_id)
{
	if (role == nullptr)
	{
		return;
	}

	Protocol::SCJingLingEggPosInfo pro;
	pro.scene_id = scene_id;
	pro.pos_x = 0;
	pro.pos_y = 0;
	std::map<int, GatherScene>::iterator it = m_gather_scene_map.find(scene_id);
	if (it != m_gather_scene_map.end())
	{
		// 随机一个坐标给客户端
		std::vector<int> index_vec; index_vec.clear();   // 保存存在蛋得下标
		for (int i = 0; i < (int)it->second.egg_pos_vec.size(); i++)
		{
			if (it->second.egg_pos_vec[i].exist > 0)
			{
				index_vec.push_back(i);
			}
		}

		if ((int)index_vec.size() > 0)
		{
			int rand_index = RandomNum((int)index_vec.size());
			if (rand_index >= 0 && rand_index < (int)index_vec.size())
			{
				int pos_index = index_vec[rand_index];
				if (pos_index >= 0 && pos_index < (int)it->second.egg_pos_vec.size())
				{
					pro.pos_x = it->second.egg_pos_vec[pos_index].pos.x - 1;
					pro.pos_y = it->second.egg_pos_vec[pos_index].pos.y - 1;
				}
			}
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
}