#include "teamequipfbmanager.hpp"
#include "gameworld/world.h"
#include "obj/character/role.h"
#include "scene/scene.h"
#include "engineadapter.h"
#include "internalcomm.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "other/fb/rolefbequip.hpp"
#include "gameworld/config/logicconfigmanager.hpp"
#include "other/fb/fbequipconfig.hpp"
#include "protocal/msgfb.h"
#include "other/event/eventhandler.hpp"
#include "global/team/teammanager.hpp"
#include "global/team/team.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "item/itempool.h"
#include "obj/otherobj/fallingitem.h"
#include "scene/speciallogic/teamspecial/specialequipteam.hpp"

TeamEquipFBManager & TeamEquipFBManager::Instance()
{
	static TeamEquipFBManager tefbm;
	return tefbm;
}

TeamEquipFBManager::TeamEquipFBManager() : m_clear_fb_interval(0)
{

}

TeamEquipFBManager::~TeamEquipFBManager()
{

}

void TeamEquipFBManager::Update(unsigned long interval)
{
	m_clear_fb_interval += interval;

	if (m_clear_fb_interval >= 2000)
	{
		m_clear_fb_interval = 0;

		// 2秒一次，这里只是把场景中的role踢掉，这时场景删除不掉，因为role是延迟删除，删场景留到下一帧

		unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();
		for (EquipFBTeamMapIt it = m_equipfb_team_map.begin(); it != m_equipfb_team_map.end();)
		{
			EquipFBTeam &teamfb_info = it->second;
			if ((teamfb_info.is_allover && now_second >= teamfb_info.kick_out_timestamp) || teamfb_info.active_number <= 0)
			{
				Team *team = TeamManager::Instance().GetTeam(it->first);
				if (nullptr != team)
				{
					Role *leader_role = team->GetLeader();
					if (nullptr != leader_role)
					{
						EventHandler::Instance().OnPassTeamFb(leader_role);
					}
				}

				World::GetInstWorld()->GetSceneManager()->DestroyTeamEquipFB(it->first, false);

				m_scenekey_to_delete_set.insert(it->first);

				m_equipfb_team_map.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}
	else
	{
		// 这帧真正删除副本场景
		for (std::set<int>::iterator iter = m_scenekey_to_delete_set.begin(); iter != m_scenekey_to_delete_set.end(); ++iter)
		{
			World::GetInstWorld()->GetSceneManager()->DestroyTeamEquipFB(*iter, true);
		}

		m_scenekey_to_delete_set.clear();
	}
}

void TeamEquipFBManager::OnEnterScene(Role *role, int scene_key, int scene_id)
{
	long long total_pass_exp = 0;

	EquipFBTeamMapIt iter = m_equipfb_team_map.find(scene_key);
	if (iter == m_equipfb_team_map.end())
	{
		EquipFBTeam fb_team_info;
		fb_team_info.active_number = 1;
		fb_team_info.user_list[0].user_id = role->GetUserId();
		fb_team_info.user_list[0].user_camp = role->GetCamp();
		role->GetName(fb_team_info.user_list[0].user_name);
		fb_team_info.fb_start_time = static_cast<int>(EngineAdapter::Instance().Time());

		m_equipfb_team_map[scene_key] = fb_team_info;

		role->GetFBEquip()->OnTeamFBMemberEnter(scene_id, fb_team_info.fb_start_time);
	}
	else
	{
		EquipFBTeam *fb_team = &iter->second;

		++fb_team->active_number;

		for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
		{
			if (role->GetUserId() == fb_team->user_list[i].user_id)
			{
				total_pass_exp = fb_team->total_pass_exp_list[i];
				break;
			}

			if (INVALID_USER_ID == fb_team->user_list[i].user_id)
			{
				fb_team->user_list[i].user_id = role->GetUserId();
				fb_team->user_list[i].user_camp = role->GetCamp();
				role->GetName(fb_team->user_list[i].user_name);
				break;
			}
		}

		role->GetFBEquip()->OnTeamFBMemberEnter(scene_id,fb_team->fb_start_time);

		for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
		{
			if (role->GetUserId() != fb_team->user_list[i].user_id)
			{
				Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(fb_team->user_list[i].user_id);
				if (NULL != tmp_role)
				{
					tmp_role->GetFBEquip()->OnTeamFBMemberEnter(scene_id, fb_team->fb_start_time);
				}
			}
		}
	}

	this->SendTotalPassExp(role, total_pass_exp);
}

void TeamEquipFBManager::OnLeaveScene(Role *role, int scene_key)
{
	EquipFBTeamMapIt iter = m_equipfb_team_map.find(scene_key);
	if (iter != m_equipfb_team_map.end())
	{
		--iter->second.active_number;
	}
}

void TeamEquipFBManager::OnLayerFinish(int scene_key, int scene_id, bool is_pass, bool can_jump, int finish_time)
{
	EquipFBTeamMapIt iter = m_equipfb_team_map.find(scene_key);
	if (iter != m_equipfb_team_map.end())
	{
		EquipFBTeam *fb_team = &iter->second;

		bool is_pass_all = false;
		int pass_layer = 0;

		const FBEquipLayerCfg *cur_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgBySceneID(scene_id);
		if (nullptr != cur_layer_cfg)
		{
			pass_layer = cur_layer_cfg->layer;
			const int max_layer = LOGIC_CONFIG->GetEquipFBConfig().GetTeamMaxLayer();

			if (cur_layer_cfg->layer >= max_layer)
			{
				is_pass_all = true;
			}
		}

		if (!iter->second.is_allover)
		{
			iter->second.is_allover = !is_pass || is_pass_all;
			if (is_pass_all)
			{
				const FBEquipOtherCfg &other_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg();
				iter->second.kick_out_timestamp = (unsigned int)EngineAdapter::Instance().Time() + other_cfg.kick_time_s;
			}
		}

		for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(fb_team->user_list[i].user_id);
			if (NULL != role && NULL != role->GetScene())
			{
				if (role->GetScene()->GetSceneID() == scene_id)
				{
					if (is_pass)
					{
						role->GetFBEquip()->OnTeamPass(pass_layer);
					}

					if (iter->second.is_allover)	//通关或失败都会发放进入副本时的历史通关奖励
					{
						short min_layer = role->GetFBEquip()->GetMemberMinLayer();
						role->GetFBEquip()->GiveEquipTeamFbLayerReward(min_layer);
					}

					role->GetFBEquip()->OnFBFinish(true, is_pass, can_jump, iter->second.is_allover);
				}
				else if (iter->second.is_allover && role->GetScene()->GetSceneType() == Scene::SCENE_TYPE_TEAM_EQUIPFB)
				{
					SpecialEquipTeam *equipteam = (SpecialEquipTeam *)role->GetScene()->GetSpecialLogic();
					equipteam->SendSceneLogicInfo(role);          // 发倒计时给在其它层的队友
				}
			}
		}
	}
}

void TeamEquipFBManager::OnAddPassExp(Role *role, int scene_key, long long addexp)
{
	EquipFBTeamMapIt iter = m_equipfb_team_map.find(scene_key);
	if (iter != m_equipfb_team_map.end())
	{
		EquipFBTeam *fb_team = &iter->second;

		for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
		{
			if (role->GetUserId() == fb_team->user_list[i].user_id)
			{
				fb_team->total_pass_exp_list[i] += addexp;

				this->SendTotalPassExp(role, fb_team->total_pass_exp_list[i]);

				break;
			}
		}
	}
}

void TeamEquipFBManager::SendTotalPassExp(Role *role, long long total_pass_exp)
{
	Protocol::SCEquipFBTotalPassExp tefi;
	tefi.total_pass_exp = total_pass_exp;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&tefi, sizeof(tefi));
}

void TeamEquipFBManager::SyncResultToTeamRank(EquipFBTeam *fbteam, int scene_id, int finish_time)
{
	//if (NULL == fbteam) return;
	//
	//const FBEquipLayerCfg *layer_cfg =  LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgBySceneID(scene_id);
	//if (NULL == layer_cfg || layer_cfg->is_mystery) return;							// 神秘层不参与排名

	//static gglobalprotocal::SyncTeamRankInfo rank_info;

	//for (int i = 0; i < MAX_TEAM_MEMBER_NUM; i++)
	//{
	//	rank_info.member_list[i].Reset();
	//}

	//int count = 0;
	//for (int i = 0; i < MAX_TEAM_MEMBER_NUM && count < MAX_TEAM_MEMBER_NUM; ++i)
	//{
	//	if (INVALID_USER_ID == fbteam->user_list[i].user_id)
	//	{
	//		continue;
	//	}

	//	rank_info.member_list[count].uid = UidToInt(fbteam->user_list[i].user_id);
	//	F_STRNCPY(rank_info.member_list[count].user_name, fbteam->user_list[i].user_name, sizeof(rank_info.member_list[count].user_name));
	//	rank_info.member_list[count].camp = fbteam->user_list[i].user_camp;

	//	++count;
	//}

	//rank_info.rank_type = TEAM_RANK_TYPE_EQUIP_FB;
	//rank_info.rank_value = this->GetEquipFBRankValueHelper(layer_cfg->show_layer, finish_time);

	//InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, (const char*)&rank_info, sizeof(rank_info));
}

long long TeamEquipFBManager::GetEquipFBRankValueHelper(int layer, int finish_time)
{
	union EquipFBTeamRankValue
	{
		EquipFBTeamRankValue() : val(0) {}

		long long val;

		struct
		{
			int finish_time;			// 所花时间
			int layer;					// 层数
		};
	};

	EquipFBTeamRankValue ll;
	ll.finish_time = 30000 - finish_time;
	ll.layer = layer;

	return ll.val;
}

void TeamEquipFBManager::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper, int scene_key)
{
	if (role == nullptr || falling_item == nullptr || item_wrapper == nullptr) return;

	EquipFBTeamMapIt iter = m_equipfb_team_map.find(scene_key);
	if (iter == m_equipfb_team_map.end()) return;

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; i++)
	{
		if (iter->second.user_list[i].user_id != role->GetUserId()) continue;
		const ItemBase *item = ITEMPOOL->GetItem(item_wrapper->item_id);
		if (item == nullptr) return;

		int min_color = ItemBase::I_COLOR_MAX;
		int min_color_index = -1;
		for (int j = 0; j < NEQFB_PICK_FALL_ITEM_RECORD_COUNT; ++j)
		{
			if (iter->second.user_list[i].total_fall_item_list[j].item_id <= 0 || iter->second.user_list[i].total_fall_item_list[j].item_id == item->GetItemId())               // 还没满8个直接存入并返回
			{
				iter->second.user_list[i].total_fall_item_list[j].item_id = item->GetItemId();
				iter->second.user_list[i].total_fall_item_list[j].item_num += falling_item->GetItemNum();
				iter->second.user_list[i].total_fall_item_list[j].item_color = item->GetColor();
				return;
			}
			else if (iter->second.user_list[i].total_fall_item_list[j].item_color < min_color)     // 找出品质最低的
			{
				min_color_index = j;
			}
		}

		if (min_color_index < 0 || min_color_index >= NEQFB_PICK_FALL_ITEM_RECORD_COUNT)   // 找不到品质最低的，最后一个换成最新的
		{
			min_color_index = NEQFB_PICK_FALL_ITEM_RECORD_COUNT - 1;
		} 

		{
			iter->second.user_list[i].total_fall_item_list[min_color_index].item_id = item->GetItemId();
			iter->second.user_list[i].total_fall_item_list[min_color_index].item_num = falling_item->GetItemNum();
			iter->second.user_list[i].total_fall_item_list[min_color_index].item_color = item->GetColor();
			return;
		}
	}
}

int TeamEquipFBManager::GetTotalFallItem(Role *role, int scene_key, Protocol::SCEquipFBResult::FallItemInfo *total_fall_item_list, const int max_list_count)
{
	int item_count = 0;
	if (role == nullptr || total_fall_item_list == nullptr) return item_count;
	EquipFBTeamMapIt iter = m_equipfb_team_map.find(scene_key);
	if (iter == m_equipfb_team_map.end()) return item_count;


	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; i++)
	{
		if (iter->second.user_list[i].user_id != role->GetUserId()) continue;
		for (int j = 0; j < max_list_count && item_count < NEQFB_PICK_FALL_ITEM_RECORD_COUNT; ++j)
		{
			if (iter->second.user_list[i].total_fall_item_list[j].item_id > 0)
			{
				total_fall_item_list[item_count].item_id = iter->second.user_list[i].total_fall_item_list[j].item_id;
				total_fall_item_list[item_count].item_num = iter->second.user_list[i].total_fall_item_list[j].item_num;
				total_fall_item_list[item_count].item_color = iter->second.user_list[i].total_fall_item_list[j].item_color;
				++item_count;
			}
		}
	}
	return item_count;
}

bool TeamEquipFBManager::GetIsTeamAllOver(int scene_key)
{
	EquipFBTeamMapIt iter = m_equipfb_team_map.find(scene_key);
	if (iter == m_equipfb_team_map.end()) return false;

	return iter->second.is_allover;
}

unsigned int TeamEquipFBManager::GetTeamKickOutTimestamp(int scene_key)
{
	EquipFBTeamMapIt iter = m_equipfb_team_map.find(scene_key);
	if (iter == m_equipfb_team_map.end()) return 0;

	return iter->second.kick_out_timestamp;
}