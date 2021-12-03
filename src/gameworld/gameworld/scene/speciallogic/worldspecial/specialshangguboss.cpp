#include "specialshangguboss.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "scene/scenemanager.h"
#include "protocal/msgfight.h"
#include "servercommon/commondata.hpp"
#include "internalcomm.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"
#include "global/worldboss/worldbossmanager.hpp"
#include "obj/otherobj/fallingitem.h"
#include "config/logicconfigmanager.hpp"
#include "global/team/teammanager.hpp"
#include "other/event/eventhandler.hpp"
#include "servercommon/errornum.h"
#include "global/bossshanggu/shanggubossconfig.hpp"
#include "monster/monsterpool.h"
#include "global/bossshanggu/bossshanggumanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "other/vip/vip.hpp"
#include "item/knapsack.h"

SpecialShangGuBoss::SpecialShangGuBoss(Scene *scene)
	: SpecialLogic(scene)
{
}

SpecialShangGuBoss::~SpecialShangGuBoss()
{

}

void SpecialShangGuBoss::Update(unsigned long interval, time_t now_second)
{
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		return;
	}

	SpecialLogic::Update(interval, now_second);

	this->CheckSceneMonsterStatus(interval, now_second);

	for (std::map<int, RoleFbInfo>::iterator it = m_role_info_map.begin(); it != m_role_info_map.end(); ++it)
	{
		if (it->second.next_add_nuqi_time > 0 && now_second > it->second.next_add_nuqi_time)
		{
			it->second.next_add_nuqi_time = now_second + LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossOtherCfg().shanggu_angry_add_interval_s;

			this->AddAngryValueToRole(it->first, 1);
		}
		if (it->second.kick_out_time > 0 && now_second >= it->second.kick_out_time)
		{
			Role *role = m_scene->GetRoleByUID(it->first);
			if (NULL != role) // 玩家在副本里，则踢出去
			{
				this->DelayKickOutRole(role);
			}
		}
	}
}

void SpecialShangGuBoss::OnRoleEnterScene(Role *role)
{
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		this->DelayKickOutRole(role);
		return;
	}

	std::map<int, RoleFbInfo>::iterator it = m_role_info_map.find(role->GetUID());
	if (it != m_role_info_map.end())
	{
		if (it->second.angry_value >= KICK_OUT_NEED_MAX_NUQI && it->second.kick_out_time > 0 && EngineAdapter::Instance().Time() >= it->second.kick_out_time)
		{
			m_role_info_map.erase(role->GetUID());
			this->DelayKickOutRole(role);
			return;
		}
	}
	else
	{
		RoleFbInfo &info = m_role_info_map[role->GetUID()];
		info.angry_value = 0;
		info.kick_out_time = 0;
		info.next_add_nuqi_time = EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossOtherCfg().shanggu_angry_add_interval_s;
	}

	this->SendInfoToRole(role);
}

void SpecialShangGuBoss::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (!is_logout && NULL != role)// 手动退出场景，可以清空所有数据了
	{
		m_role_info_map.erase(role->GetUID());

		if (!role->IsAlive())
		{
			role->ReAlive(100, 100, role->GetPos(), true);
		}
		else
		{
			role->Recover();
		}
	}
}

void SpecialShangGuBoss::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		return;
	}

	if (NULL == dead)
	{
		return;
	}

	Role *killer_role = m_scene->GetPrincipal(killer_id);
	
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster *)dead;
		if (!monster->IsBossMonster())
		{
			if (NULL != killer_role)
			{
				// 黄金怪
				const ShangGuBossInfoConfig::ItemConfig *boss_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossInfoWithSceneID(m_scene->GetSceneID(), monster->GetMonsterId());
				if (NULL != boss_cfg)
				{
					this->AddAngryValueToRole(killer_role->GetUID(), boss_cfg->kill_add_angry);
				}
				else
				{
					// 击杀小怪增加怒气
					this->AddAngryValueToRole(killer_role->GetUID(), LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossOtherCfg().shanggu_angry_kill_monster_add);
				}
			}
		}

		{
			const ShangGuBossLayerConfig *layer_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCfgBySceneId(m_scene->GetSceneID());
			if (NULL != layer_cfg)
			{
				BossShangGuManager::Instance().OnShangGuBossEraseMap(layer_cfg->layer, monster->GetId());
			}
		}
	}
	else if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		Role *dead_role = (Role*)dead;
		this->AddAngryValueToRole(dead_role->GetUID(), LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossOtherCfg().shanggu_angry_die_add); // 玩家死亡增加怒气
	}
}

void SpecialShangGuBoss::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		return;
	}

	if (NULL == role)
	{
		return;
	}

	const ShangGuBossOtherCfg &other_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossOtherCfg();

	if (gather_id == other_cfg.shanggu_min_box_id)
	{
		this->AddAngryValueToRole(role->GetUID(), other_cfg.shanggu_angry_min_box_add);

		role->GetKnapsack()->PutOrMail(other_cfg.reward_item_min, PUT_REASON_SHANGGU_BOSS_GATHER_REWARD);
	}
	else if (gather_id == other_cfg.shanggu_max_box_id)
	{
		this->AddAngryValueToRole(role->GetUID(), other_cfg.shanggu_angry_max_box_add);
		role->GetKnapsack()->PutOrMail(other_cfg.reward_item_max, PUT_REASON_SHANGGU_BOSS_GATHER_REWARD);
	}


	const ShangGuBossLayerConfig *layer_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCfgBySceneId(m_scene->GetSceneID());
	if (NULL != layer_cfg)
	{
		BossShangGuManager::Instance().OnShangGuBossEraseMap(layer_cfg->layer, obj_id);
	}
}

void SpecialShangGuBoss::OnPickFallingItem(Role *role, FallingItem *falling_item)
{
}

bool SpecialShangGuBoss::SpecialCanReAlive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		return true;
	}

	return true;
}

void SpecialShangGuBoss::AddAngryValueToDropOwner(Role *role, unsigned short monster_id)
{
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		return;
	}

	const ShangGuBossInfoConfig::ItemConfig *boss_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossInfoWithSceneID(m_scene->GetSceneID(), monster_id);
	const ShangGuBossHideBossConfig::LevelConfig *hide_boss_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossHideBossCfgByMonsterId(monster_id);
	int kill_add_angry = 0;
	bool is_from_boss = false;
	if (NULL != boss_cfg)
	{
		is_from_boss = true;
		kill_add_angry = boss_cfg->kill_add_angry;
	}
	else if (NULL != hide_boss_cfg)
	{
		kill_add_angry = hide_boss_cfg->kill_add_angry;
	}

	if (0 != kill_add_angry)
	{
		if (NULL != role)
		{
			this->AddAngryValueToRole(role->GetUID(), kill_add_angry);
		}
	}

	// 击杀者，隐藏boss不需调用
	if (is_from_boss)
	{
		BossShangGuManager::Instance().OnShangGuBossDie(m_scene->GetSceneID(), monster_id, role);
	}
}

void SpecialShangGuBoss::AddAngryValueToRole(int uid, int add_nuqi)
{
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		return;
	}

	RoleFbInfo &info = m_role_info_map[uid];
	if (info.angry_value >= KICK_OUT_NEED_MAX_NUQI) // 怒气满了还在一直加怒气？先不理
	{
		return;
	}

	info.angry_value += add_nuqi;
	if (info.angry_value >= KICK_OUT_NEED_MAX_NUQI)
	{
		info.kick_out_time = EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossOtherCfg().shanggu_angry_kick_need_tims_s;
	}

	this->SendInfoToRole(m_scene->GetRoleByUID(uid));
}

void SpecialShangGuBoss::SendInfoToRole(Role *role)
{
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		return;
	}

	if (NULL == role) return;
	RoleFbInfo &role_info = m_role_info_map[role->GetUID()];

	static Protocol::SCShangGuBossSceneInfo dbsi;
	dbsi.angry_value = role_info.angry_value;
	dbsi.kick_out_time = (unsigned int)role_info.kick_out_time;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&dbsi, sizeof(dbsi));
}