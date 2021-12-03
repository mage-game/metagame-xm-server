#include "specialyaoshoujitanteamfb.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/fb/yaoshoujitanteamfbconfig.hpp"
#include "monster/monsterpool.h"
#include "engineadapter.h"
#include "global/team/team.hpp"
#include "obj/character/role.h"
#include "protocal/msgfb.h"
#include "item/itempool.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "scene/worldshadow/worldshadow.hpp"
#include "obj/character/monster.h"
#include "other/event/eventhandler.hpp"
#include "world.h"
#include "global/team/teammanager.hpp"
#include "global/team/team.hpp"
#include "item/knapsack.h"
#include "friend/teaminfo.hpp"
#include "other/daycounter/daycounter.hpp"

SpecialYaoShouJiTanTeamFb::SpecialYaoShouJiTanTeamFb(Scene *scene) 
	: SpecialLogic(scene), m_is_init(false), m_is_finish(false), m_is_pass(false), m_mode(0), m_team_index(0), m_boss_attr_type(0), m_next_calc_boss_attr_time(0)
{
	for (int i = 0; i < YAOSHOUJITAN_TEAMFB_MAX_ATTR_NUM; ++ i)
	{
		m_role_attrs[i].Reset();
	}
}

SpecialYaoShouJiTanTeamFb::~SpecialYaoShouJiTanTeamFb()
{
	
}

void SpecialYaoShouJiTanTeamFb::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (!m_is_init)
	{
		m_is_init = true;

		m_mode = LOGIC_CONFIG->GetYaoShouJiTanTeamFbCfg().GetModeBySceneId(m_scene->GetSceneID());
		this->CreateMonster(m_mode);
	}

	if (m_is_finish)
	{
		return;
	}

	// do other
	if (m_next_calc_boss_attr_time >= 0 && now_second >= m_next_calc_boss_attr_time)
	{
		const YaoShouJiTanTeamFBModeConfig *mode_cfg = LOGIC_CONFIG->GetYaoShouJiTanTeamFbCfg().GetFbModeCfg(m_mode);
		if (NULL != mode_cfg)
		{
			this->CalcBossAttr();
			m_next_calc_boss_attr_time = static_cast<unsigned int>(now_second) + mode_cfg->change_time_gap;
		}
	}

	this->CheckSceneMonsterStatus(interval, now_second);
}

void SpecialYaoShouJiTanTeamFb::CreateMonster(int mode)
{
	const YaoShouJiTanTeamFBModeConfig * mode_cfg = LOGIC_CONFIG->GetYaoShouJiTanTeamFbCfg().GetFbModeCfg(mode);
	if (NULL == mode_cfg) return;

	MONSTERPOOL->CreateMonster(mode_cfg->boss_id, m_scene, mode_cfg->boss_pos);
}

void SpecialYaoShouJiTanTeamFb::OnRoleEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}
	this->CalcRoleAttr(role);
	m_team_index = role->GetTeamInfo()->GetTeamIndex();
	this->SyncSceneLogicInfo();
}

bool SpecialYaoShouJiTanTeamFb::OnTimeout()
{
	this->OnFinish(false);
	this->DelayKickOutAllRole();
	return false;
}

void SpecialYaoShouJiTanTeamFb::OnAllMonsterDead()
{
	this->OnFinish(true);
}

void SpecialYaoShouJiTanTeamFb::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (NULL == cha) return;

	Obj *attacker = m_scene->GetObj(enemy_id);
	if (NULL == attacker || !attacker->IsCharactor(attacker->GetObjType())) return;

	int ke_type = this->CalcAttrKe(cha, (Character *)attacker);
	if (KE_1K2 == ke_type && Obj::OBJ_TYPE_MONSTER == cha->GetObjType())  //仅boss被打相克时才加血
	{	
		const YaoShouJiTanTeamFBModeConfig *mode_cfg = LOGIC_CONFIG->GetYaoShouJiTanTeamFbCfg().GetFbModeCfg(m_mode);
		if (NULL != mode_cfg && cha->IsAlive())
		{
			Attribute hp_add_value = Attribute(cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * mode_cfg->boss_js_per / 10000);
			cha->ChangeHp(&hp_add_value, true);
		}
	}
}

long long SpecialYaoShouJiTanTeamFb::SpecialRecalcInjure(Character *attacker, Character *injurer, long long injure)
{
	int ke_type = this->CalcAttrKe(injurer, attacker);
	if (KE_2K1 == ke_type)  //攻击者克被攻击者
	{
		return injure * 2;
	}
	else if (KE_1K2 == ke_type)
	{
		return 0;
	}
	
	return injure;
}

bool SpecialYaoShouJiTanTeamFb::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		return false;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

void SpecialYaoShouJiTanTeamFb::OnLeaveFB(Role *role)
{
	if (NULL == role) return;

	if (m_team_index == role->GetTeamInfo()->GetTeamIndex())
	{
		Team *team = TeamManager::Instance().GetTeam(m_team_index);
		if (NULL != team)
		{
			TeamManager::Instance().RemoveTeamMember(role, m_team_index, role->GetUserId());
		}
	}
}

bool SpecialYaoShouJiTanTeamFb::IsInTeam(int uid)
{
	Team *team = TeamManager::Instance().GetTeam(m_team_index);
	if (NULL == team)
	{
		return false;
	}

	bool is_in_team = NULL != team->GetTeamMember(IntToUid(uid), NULL);
	return is_in_team;
}

void SpecialYaoShouJiTanTeamFb::CalcRoleAttr(Role *role)
{
	if (NULL == role) return;
	if (this->GetRoleAttrType(role) >= ATTR_MIN) return;

	int empty_index = -1;
	for (int i = 0; i < YAOSHOUJITAN_TEAMFB_MAX_ATTR_NUM; ++ i)
	{
		if (m_role_attrs[i].IsEmpty())
		{
			empty_index = i;
			break;
		}
	}

	if (empty_index >= 0 && empty_index < YAOSHOUJITAN_TEAMFB_MAX_ATTR_NUM)
	{
		m_role_attrs[empty_index].uid = role->GetUID();
		int attr = empty_index;  //一定对应
		m_role_attrs[empty_index].attr = attr;
	}
}

void SpecialYaoShouJiTanTeamFb::CalcBossAttr()
{
	++ m_boss_attr_type;
	if (m_boss_attr_type >= ATTR_MAX)
	{
		m_boss_attr_type = ATTR_MIN;
	}

	this->SyncSceneLogicInfo();
}

int SpecialYaoShouJiTanTeamFb::GetRoleAttrType(Role *role)
{
	if (NULL == role) return -1;

	for (int i = 0; i < YAOSHOUJITAN_TEAMFB_MAX_ATTR_NUM; ++ i)
	{
		if (role->GetUID() == m_role_attrs[i].uid)
		{
			return m_role_attrs[i].attr;
		}
	}
	
	return -1;
}

int SpecialYaoShouJiTanTeamFb::CalcAttrKe(Character *be_attack, Character *attacker)
{
	if (NULL == be_attack || NULL == attacker) return KE_NONE;

	if (be_attack->GetObjType() == attacker->GetObjType()) return KE_NONE;

	int be_attack_attr = 0, attacker_attr = 0;
	if (Obj::OBJ_TYPE_ROLE == be_attack->GetObjType())
	{
		Role *role = (Role *)be_attack;
		be_attack_attr = this->GetRoleAttrType(role);	
	}

	if (Obj::OBJ_TYPE_MONSTER == be_attack->GetObjType())
	{
		be_attack_attr = m_boss_attr_type;
	}

	if (Obj::OBJ_TYPE_ROLE == attacker->GetObjType())
	{
		Role *role = (Role *)attacker;
		attacker_attr = this->GetRoleAttrType(role);	
	}

	if (Obj::OBJ_TYPE_MONSTER == attacker->GetObjType())
	{
		attacker_attr = m_boss_attr_type;
	}

	return this->CalcAttrKe(be_attack_attr, attacker_attr);
}

//属性有相克，如：金克木，火克金，水克火，木克水		
int SpecialYaoShouJiTanTeamFb::CalcAttrKe(int attr1, int attr2)
{
	if (attr1 < ATTR_MIN || attr1 >= ATTR_MAX || attr2 < ATTR_MIN || attr2 >= ATTR_MAX)
	{
		return KE_NONE;
	}

	if (1 == attr1 - attr2 || (ATTR_MIN == attr1 && ATTR_MAX - 1 == attr2))
	{
		return KE_1K2;
	}

	else if (1 == attr2 - attr1 || (ATTR_MIN == attr2 && ATTR_MAX - 1 == attr1))
	{
		return KE_2K1;
	}

	else
	{
		return KE_NONE;
	}
}

void SpecialYaoShouJiTanTeamFb::OnFinish(bool is_pass)
{
	if (m_is_finish)
	{
		return;
	}

	m_is_finish = true;
	m_is_pass = is_pass;

	if (m_is_finish && m_is_pass)
	{
		this->PutPassReward();
	}

	this->SyncSceneLogicInfo();
}

void SpecialYaoShouJiTanTeamFb::PutPassReward()
{
	if (!m_is_finish || !m_is_pass)
	{
		return;
	}

	const YaoShouJiTanTeamFBModeConfig *mode_cfg = LOGIC_CONFIG->GetYaoShouJiTanTeamFbCfg().GetFbModeCfg(m_mode);
	if (NULL == mode_cfg)
	{
		return;
	}

	// 不在本场景里的队员不拿奖励也不扣次数
	ItemConfigData item_configdata_list[MAX_ATTACHMENT_ITEM_NUM] = {mode_cfg->reward_item1, mode_cfg->reward_item2, mode_cfg->reward_item3};
	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);

		if (NULL == temp_role || !this->IsInTeam(temp_role->GetUID()))
		{
			continue;
		}

		if (temp_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_YAOSHOUJITAN_JOIN_TIMES) < mode_cfg->free_join_times)
		{
			temp_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_YAOSHOUJITAN_JOIN_TIMES);

			temp_role->GetKnapsack()->PutListOrMail(item_configdata_list, PUT_REASON_YAOSHOUJITAN_PASS_REWARD);

			EventHandler::Instance().OnPassYsjtTeamFb(temp_role);
		}
	}
}

void SpecialYaoShouJiTanTeamFb::SyncSceneLogicInfo()
{
	static Protocol::SCYsjtTeamFbSceneLogicInfo cmd;

	cmd.time_out_stamp = m_scene->GetSceneTimeOutStamp();
	cmd.scene_type = this->GetSceneType();
	cmd.is_finish = m_is_finish ? 1 : 0;
	cmd.is_pass = m_is_pass ? 1 : 0;
	cmd.kill_boss_num = m_is_pass ? 1 : 0;
	cmd.pass_time_s = this->GetPassTime();
	cmd.boss_attr_type = m_boss_attr_type;
	cmd.mode = m_mode;
	memcpy(cmd.role_attrs, m_role_attrs, sizeof(cmd.role_attrs));

	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		if (NULL != temp_role && this->IsInTeam(temp_role->GetUID()))
		{
			EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
		}
	}
}
