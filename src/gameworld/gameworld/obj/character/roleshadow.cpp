#include "roleshadow.hpp"
#include <algorithm>
#include <map>

#include "ai/roleai/roleaifool.hpp"
#include "ai/roleai/roleaichallengefile.hpp"
#include "ai/roleai/roleaiworldevent.hpp"
#include "ai/roleai/roleairoleboss.hpp"
#include "ai/roleai/roleaicampdefend.hpp"
#include "ai/roleai/roleaielementfield.hpp"
#include "ai/roleai/roleairoleclone.hpp"

#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"
#include "gamecommon.h"
#include "other/event/eventhandler.hpp"
#include "global/usercache/usercache.hpp"
#include "engineadapter.h"
#include "obj/character/monster.h"
#include "hightimermgr.h"
#include "globalconfig/globalconfig.h"

RoleShadow::RoleShadow(int role_shadow_type)
	: Character(OBJ_TYPE_ROLE_SHADOW), m_ai(NULL), m_high_time_reg_id(0), m_role_shadow_type(role_shadow_type), m_plat_type(0), m_user_id(INVALID_USER_ID), m_level(0), m_avatar(0), m_sex(0), m_camp(0), m_prof(0), m_capability(0),
	m_avatar_timestamp(0), m_vip_level(0), m_guild_post(0), m_guild_id(0),
	m_hurt_monster_inc_percent(0), m_hurt_role_inc_percent(0), m_hurt_building_inc_percent(0), 
	m_monster_hurt_dec_percent(0), m_role_hurt_dec_percent(0), m_absorb_percent(0), m_absorb_regular(0), m_once_hurt_inc_percent(0),
	m_protect_gather_maxhp_percent(0), m_gather_speed_up_percent(0), m_birth_pos(0, 0), m_is_addtimer(false), m_special_param(0)
{
	memset(m_guild_name, 0, sizeof(GuildName));
	memset(m_soul_skill_last_perform_time_list, 0, sizeof(m_soul_skill_last_perform_time_list));
}

RoleShadow::~RoleShadow()
{
	if (m_is_addtimer)
	{
		m_is_addtimer = false;
		HighTimerMgr::Instance().UnRegister(this);
	}

	if (NULL != m_ai)
	{
		delete m_ai; m_ai = NULL;
	}
}

void RoleShadow::Init(ChallengeParam &param, RoleAppearance &appearance, Attribute hp, Attribute mp)
{
	m_plat_type = param.plat_type;
	m_user_id = IntToUid(param.role_id);
	m_level = param.level;
	m_avatar = param.avatar;
	m_sex = param.sex;
	m_camp = param.camp;
	m_prof = param.prof;
	m_capability = param.capability;
	this->SetName(param.role_name);
	m_vip_level = param.vip_level;
	m_guild_post = param.guild_post;

	m_guild_id = param.guild_id;
	F_STRNCPY(m_guild_name, param.guild_name, sizeof(GuildName));

	m_appearance = appearance;

	m_avatar_timestamp = param.avatar_timestamp;

	static const Attribute ROLE_GLOBAL_CD = GLOBALCONFIG->GetRoleGlobalCooldown();

	CharIntAttrs int_attribute;
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GLOBAL_COOLDOWN, ROLE_GLOBAL_CD);

	Attribute curr_hp = (0 == hp ? param.max_hp : hp);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP, curr_hp);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, param.max_hp);

	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, param.gongji);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, param.fangyu);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG, param.mingzhong);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI, param.shanbi);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI, param.baoji);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN, param.jianren);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED, param.move_speed);

	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI, param.fujiashanghai);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_DIKANG_SHANGHAI, param.dikangshanghai);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN, param.jingzhun_per);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT, param.baoji_per);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO, param.kangbao_per);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG, param.pofang_per);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG, param.mianshang_per);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG, param.constant_zengshang);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_MIANSHANG, param.constant_mianshang);

	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, param.max_hp);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, param.gongji);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, param.fangyu);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, param.mingzhong);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, param.shanbi);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, param.baoji);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, param.jianren);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED, param.move_speed);

	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI, param.fujiashanghai);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI, param.dikangshanghai);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN, param.jingzhun_per);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT, param.baoji_per);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO, param.kangbao_per);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, param.pofang_per);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, param.mianshang_per);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG, param.constant_zengshang);
	int_attribute.SetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG, param.constant_mianshang);

	SkillListParam skill_param; skill_param.count = 0;
	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++ i)
	{
		ChallengeParam::SkillInfo *skill_info = &param.skill_list[i];
		if (NULL != skill_info && 0 != skill_info->skill_id)
		{
			skill_param.skill_list[skill_param.count].index = skill_param.count;
			skill_param.skill_list[skill_param.count].skill_id = skill_info->skill_id;
			skill_param.skill_list[skill_param.count].level = skill_info->level;
			skill_param.skill_list[skill_param.count].last_perform_time = 0;
			skill_param.skill_list[skill_param.count].exp = 0;

			++ skill_param.count;
		}
	}

	SkillOtherParam skill_other_parma;
	Character::Init(int_attribute, skill_param, skill_other_parma, 0, NULL);

	// 	this->CheckSoulSkill();

	switch (m_role_shadow_type)
	{
	case ROLE_SHADOW_TYPE_CHALLENGE_FIELD:
	{
		m_ai = new RoleAIChallengefile(this);
	}
	break;

	case ROLE_SHADOW_THPE_WORLD_EVENT:
	{
		m_ai = new RoleAIWorldEvent(this);
	}
	break;

	case ROLE_SHADOW_TYPE_ROLE_BOSS:
	{
		m_ai = new RoleAIRoleBoss(this);
	}
	break;

	case ROLE_SHADOW_TYPE_CAMPDEFEND:
	{
		m_ai = new RoleAICampDefend(this);
	}
	break;

	case ROLE_SHADOW_TYPE_ELEMENT_FILED:
	{
		m_ai = new RoleAIElementField(this);
	}
	break;

	case ROLE_SHADOW_TYPE_CLONE_ROLE:
	{
		m_ai = new RoleAIRoleClone(this);
	}
	break;

	default:
	{
		m_ai = new RoleAIFool(this);
	}
	break;
	}

	m_ai->Init(param);
}

int RoleShadow::GetRoleShadowType()
{ 
	if (NULL != m_ai) return m_ai->GetAIType();
	return ROLE_SHADOW_TYPE_INVALID;
}

void RoleShadow::GetGuildName(GuildName guild_name)
{
	if (NULL == guild_name)
	{
		return;
	}

	F_STRNCPY(guild_name, m_guild_name, sizeof(GuildName));
}

void RoleShadow::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	Character::Update(interval, now_second, now_dayid);

	if (!this->IsAlive())   // 如果死了 就删掉自己
	{
		const static int dead_delay = (int)GLOBALCONFIG->GetOtherConfig_Monster_Dead_Delay();
		if (m_dead_time > dead_delay)
		{
			m_scene->DeleteObj(m_obj_id);
		}
	}
	else
	{
		if (NULL != m_ai) m_ai->Update(interval, static_cast<unsigned int>(now_second));
	}
}

void RoleShadow::CalcAI(unsigned long interval)
{
	if (!CheckGlobalCD()) return;
	
	if (this->IsAlive())
	{
		if (NULL != m_ai)
		{
			m_ai->AI(interval);
		}
	}
}

void RoleShadow::SetBirthPos(const Posi &birth_pos)
{
	if (NULL != m_ai) m_ai->SetBirthPos(birth_pos);
}

void RoleShadow::OnEnterScene()
{
	Character::OnEnterScene();
	if (NULL != m_ai) m_ai->OnEnterScene();
	if (!m_is_addtimer)
	{
		m_is_addtimer = true;
		HighTimerMgr::Instance().Register(this, &RoleShadow::CalcAI);
	}
}

void RoleShadow::OnLeaveScene()
{
	if (m_is_addtimer)
	{
		m_is_addtimer = false;
		HighTimerMgr::Instance().UnRegister(this);
	}

	if (NULL != m_ai) m_ai->OnLeaveScene();
	Character::OnLeaveScene();
}

void RoleShadow::OnAOIEnter(ObjID obj_id)
{
	if (NULL != m_ai) m_ai->OnAOIEnter(obj_id);
}

void RoleShadow::OnAOILeave(ObjID obj_id)
{
	if (NULL != m_ai) m_ai->OnAOILeave(obj_id);
}

bool RoleShadow::IsEnemy(Obj *obj)
{
	if (obj->GetId() == m_obj_id) return false;

	if (Obj::IsCharactor(obj->GetObjType()))
	{
		Character *cha = (Character*)obj;

		if (cha->IsAlive())
		{
			if (ROLE_SHADOW_TYPE_CAMPDEFEND == m_role_shadow_type)
			{
				return false;
			} else if (ROLE_SHADOW_TYPE_CLONE_ROLE == m_role_shadow_type)
			{
				Role* role = m_scene->GetRoleByUID(UidToInt(m_user_id));
				if (role == obj)
				{
					return false;
				}
				if (role)
				{
					return role->IsEnemy(obj);
				}
			}


			if (NULL != m_ai) return m_ai->IsEnemy(obj);
		}
	}

	return false;
}

bool RoleShadow::IsFriend(Obj *obj, bool check_alive)
{
	if (NULL != m_ai) return m_ai->IsFriend(obj, check_alive);
	return false;
}

void RoleShadow::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill)
{
	Character::OnAttack(attacker, skill_id, delta_hp, from_skill);

	if (NULL != m_ai && INVALID_OBJ_ID == m_ai->GetEnemy())
	{
		this->ForceSetEnemy(attacker);
	}

	if (NULL != m_scene)
	{
		Obj *obj = m_scene->GetObj(attacker);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			Role *role = (Role *)obj;

			HurInfotMapIt it = m_hurt_info_map.find(role->GetUID());
			if (m_hurt_info_map.end() != it)
			{
				it->second.hurt += -delta_hp;
				it->second.last_hurt_timestamp = EngineAdapter::Instance().Time();
			}
			else
			{
				HurtInfo hurt_info;
				hurt_info.hurt = -delta_hp;
				hurt_info.last_hurt_timestamp = EngineAdapter::Instance().Time();
				m_hurt_info_map[role->GetUID()] = hurt_info;
			}
		}
	}

	if (NULL != m_ai) m_ai->OnAttack(attacker, skill_id, delta_hp);
}

void RoleShadow::OnDie(ObjID killer)
{
	Character::OnDie(killer);

	if (NULL != m_scene)
	{
		Obj *obj = m_scene->GetObj(killer);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			Role *role = (Role *)obj;

			EventHandler::Instance().OnKillRoleShadow(role, this);
		}
	}
}

bool RoleShadow::MoveBySkill(Axis _x, Axis _y, Dir _dir, Scalar distance, bool check_special_state)
{
	if (ROLE_SHADOW_TYPE_CAMPDEFEND == this->GetRoleShadowType())
	{
		return false;
	}

	return Character::MoveBySkill(_x, _y, _dir, distance, check_special_state);
}

void RoleShadow::ForceSetEnemy(ObjID enemy_objid)
{
	if (enemy_objid != this->GetId() && NULL != m_ai)
	{
		m_ai->ForceSetEnemy(enemy_objid);
		
		// 竞技场的机器人设置敌人后不用立即触发攻击
		if (ROLE_SHADOW_TYPE_CHALLENGE_FIELD != m_role_shadow_type || Scene::SCENE_TYPE_CHALLENGE_FIELD != m_scene->GetSceneType())
		{
			m_ai->ForceStartAttack();
		}
	}
}

int RoleShadow::GetHurtIncPercent(Character *target)
{
	int inc_percent = 0;

	inc_percent += this->GetSoulSkillHurtIncPercent(target);

	return inc_percent;
}

int RoleShadow::GetHurtDecPercent(Character *performer)
{
	int dec_percent = 0;

	dec_percent += this->GetSoulSkillHurtDecPercent(performer);

	return dec_percent;
}

bool RoleShadow::GetTopHurtUid(int *uid, const std::set<int> &filter_uid)
{
	if (NULL == uid)
	{
		return false;
	}

	int top_uid = 0; long long top_hurt = 0;

	for (HurInfotMapIt iter = m_hurt_info_map.begin(); m_hurt_info_map.end() != iter; ++iter)
	{
		if (filter_uid.end() == filter_uid.find(iter->first))
		{
			if (UserCacheManager::Instance().IsUserOnline(iter->first))
			{
				if (iter->second.hurt > top_hurt)
				{
					top_hurt = iter->second.hurt;
					top_uid = iter->first;
				}
			}
		}
	}
	if (0 == top_uid)
	{
		return false;
	}

	*uid = top_uid;
	return true;
}

int RoleShadow::RandGetHurtUid(int uid_count, int *uid_list, const std::set<int> &filter_uid)
{
	return 0;
	
	if (uid_count <= 0 || NULL == uid_list) 
	{
		return false;
	}

	std::vector<int> rand_uid_set;

	for (HurInfotMapIt iter = m_hurt_info_map.begin(); m_hurt_info_map.end() != iter; ++iter)
	{
		if (filter_uid.end() == filter_uid.find(iter->first))
		{
			if (UserCacheManager::Instance().IsUserOnline(iter->first))
			{
				rand_uid_set.push_back(iter->first);
			}
		}
	}

	std::random_shuffle(rand_uid_set.begin(), rand_uid_set.end());
	
	int rand_uid_count = 0;
	
	for (std::vector<int>::iterator it = rand_uid_set.begin(); rand_uid_set.end() != it && rand_uid_count < uid_count; ++ it)
	{
		uid_list[rand_uid_count] = *it;
		++ rand_uid_count;
	}
	

	return rand_uid_count;
}

long long RoleShadow::GetHurtByUid(int uid)
{
	HurInfotMapIt iter = m_hurt_info_map.find(uid);
	if (m_hurt_info_map.end() == iter)
	{
		return 0;
	}

	return iter->second.hurt;
}

void RoleShadow::GetHurtInfo(std::vector<HurtRoleShadowInfo> &hurt_vector)
{
	hurt_vector.clear();

	HurtRoleShadowInfo hurt_info;

	for (HurInfotMapIt iter = m_hurt_info_map.begin(); m_hurt_info_map.end() != iter; ++iter)
	{
		hurt_info.hurt = iter->second.hurt;
		hurt_info.uid = iter->first;
		hurt_vector.push_back(hurt_info);
	}
}

bool RoleShadow::CheckSkillRangeUseDefaultSkill()
{
	return (!m_skill_manager.CheckNextSkillAttackCharacter());
}

void RoleShadow::CheckSoulSkill()
{
// 	m_hurt_monster_inc_percent = 0;
// 	m_hurt_role_inc_percent = 0;
// 	m_hurt_building_inc_percent = 0;
// 	m_monster_hurt_dec_percent = 0;
// 	m_role_hurt_dec_percent = 0;
// 	m_absorb_percent = 0;
// 	m_absorb_regular = 0;
// 	m_protect_gather_maxhp_percent = 0;
// 	m_gather_speed_up_percent = 0;
// 
// 	int active_slot_count = LOGIC_CONFIG->GetXianjianConfig().GetActiveSkillSlotCount(m_xianjian_info.active_xianjian_count);
// 
// 	for (int i = 0; i < active_slot_count; ++ i)
// 	{
// 		int soul_id = m_xianjian_info.soul_skill_list[i].skill_id;
// 		if (soul_id < 0 || soul_id >= MAX_XIANJIAN_SOUL_COUNT)
// 		{
// 			soul_id = -1;
// 			continue;
// 		}
// 
// 		const XianjianSoulSkillConfig *skill_cfg = LOGIC_CONFIG->GetXianjianConfig().GetSoulSkillCfg(soul_id, m_xianjian_info.soul_skill_list[i].skill_level);
// 		if (NULL == skill_cfg)
// 		{
// 			continue;
// 		}
// 
// 		switch (soul_id)
// 		{
// 		case SOUL_SKILL_PERCENT_XIXUE:
// 			{
// 				m_absorb_percent += skill_cfg->param_a;
// 			}
// 			break;
// 
// 		case SOUL_SKILL_REGULAR_XIXUE:
// 			{
// 				m_absorb_regular += skill_cfg->param_a;
// 			}
// 			break;
// 
// 		case SOUL_SKILL_HURT_ROLE_INC_PERCENT:
// 			{
// 				m_hurt_role_inc_percent += skill_cfg->param_a;
// 			}
// 			break;
// 
// 		case SOUL_SKILL_HURT_MONSTER_INC_PERCENT:
// 			{
// 				m_hurt_monster_inc_percent += skill_cfg->param_a;
// 			}
// 			break;
// 
// 		case SOUL_SKILL_HURT_BUILDING_INC_PERCENT:
// 			{
// 				m_hurt_building_inc_percent += skill_cfg->param_a;
// 			}
// 			break;
// 
// 		case SOUL_SKILL_ROLE_HURT_DEC_PERCENT:
// 			{
// 				m_role_hurt_dec_percent += skill_cfg->param_a;
// 			}
// 			break;
// 
// 		case SOUL_SKILL_MONSTER_HURT_DEC_PERCENT:
// 			{
// 				m_monster_hurt_dec_percent += skill_cfg->param_a;
// 			}
// 			break;
// 
// 		case SOUL_SKILL_GATHER_SPEED_UP:
// 			{
// 				m_gather_speed_up_percent += skill_cfg->param_a;
// 			}
// 			break;
// 
// 		case SOUL_SKILL_GATHER_NO_INTERRUPT:
// 			{
// 				m_protect_gather_maxhp_percent += skill_cfg->param_a;
// 			}
// 
// 		default:
// 			break;
// 		}
// 	}
}


int RoleShadow::GetSoulSkillHurtIncPercent(Character *target)
{
	int inc_percent = 0;

	if (NULL != target)
	{
		inc_percent += m_once_hurt_inc_percent;

		if (Obj::OBJ_TYPE_MONSTER == target->GetObjType())
		{
			Monster *monster = (Monster *)target;

			const static unsigned short BUILDING_MONSTER_ID = 1110;

			if (BUILDING_MONSTER_ID == monster->GetMonsterId()) // 对建筑的伤害加成
			{
				inc_percent += m_hurt_building_inc_percent;
			}
			else
			{
				inc_percent += m_hurt_monster_inc_percent;
			}
		}
		else
		{
			inc_percent += m_hurt_role_inc_percent;
		}
	}

	return inc_percent;
}

int RoleShadow::GetSoulSkillHurtDecPercent(Character *performer)
{
	int dec_percent = 0;

	if (NULL != performer)
	{
		if (Obj::OBJ_TYPE_MONSTER == performer->GetObjType())
		{
			dec_percent += m_monster_hurt_dec_percent;
		}
		else
		{
			dec_percent += m_role_hurt_dec_percent;
		}
	}

	return dec_percent;
}
// 
// void RoleShadow::OnHurtOtherSoulSkill(Character *target, int skill_id, long long injure)
// {
// 	if (NULL == target || injure <= 0)
// 	{
// 		return;
// 	}
// 
// 	int absorb_hp = m_absorb_regular + static_cast<int>(injure * m_absorb_percent / 100);
// 	if (absorb_hp > 0 && this->IsAlive())
// 	{
// 		EffectOnce *addhp_effect = new EffectOnce(this->GetId(), 0, absorb_hp, EffectBase::FIGHT_TYPE_NORMAL);
// 		this->AddEffect(addhp_effect);
// 	}
// 
// 	m_once_hurt_inc_percent = 0;
// 
// 	time_t now_sec = EngineAdapter::Instance().Time();
// 	int active_slot_count = LOGIC_CONFIG->GetXianjianConfig().GetActiveSkillSlotCount(m_xianjian_info.active_xianjian_count);
// 
// 	for (int i = 0; i < active_slot_count; ++ i)
// 	{
// 		int rand_value = RandomNum(100);
// 		int soul_id = m_xianjian_info.soul_skill_list[i].skill_id;
// 
// 		if (soul_id < 0 || soul_id >= MAX_XIANJIAN_SOUL_COUNT)
// 		{
// 			continue;
// 		}
// 
// 		const XianjianSoulSkillConfig *skill_cfg = LOGIC_CONFIG->GetXianjianConfig().GetSoulSkillCfg(soul_id, m_xianjian_info.soul_skill_list[i].skill_level);
// 		if (NULL == skill_cfg)
// 		{
// 			continue;
// 		}
// 
// 		time_t &skill_last_perform_timestamp = m_soul_skill_last_perform_time_list[i];
// 
// 		if (now_sec < skill_last_perform_timestamp + skill_cfg->cd_s)
// 		{
// 			continue;
// 		}
// 
// 		bool is_trigger = false;
// 
// 		switch(soul_id)
// 		{
// 		case SOUL_SKILL_HURT_INC_PERCENT_ONE_TIMES:
// 			{
// 				if (rand_value < skill_cfg->rate)
// 				{
// 					is_trigger = true;
// 
// 					m_once_hurt_inc_percent = skill_cfg->param_a;
// 					skill_last_perform_timestamp = now_sec;
// 				}
// 			}
// 			break;
// 
// 		case SOUL_SKILL_XUANXUN:
// 			{
// 				// 眩晕
// 				if (rand_value < skill_cfg->rate)
// 				{
// 					if (skill_cfg->param_b > 0)
// 					{
// 						EffectSpecialState *effect_yun = new EffectSpecialState(this->GetId(), skill_cfg->skill_id, skill_cfg->param_b * 1000, EBT_XUANYUN, skill_cfg->soul_level);
// 						effect_yun->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
// 						target->AddEffect(effect_yun);
// 
// 						skill_last_perform_timestamp = now_sec;
// 						is_trigger = true;
// 					}
// 				}
// 			}
// 			break;
// 
// 		case SOUL_SKILL_JIANSHU:
// 			{
// 				// 减速
// 				if (rand_value < skill_cfg->rate)
// 				{
// 					int dur_ms = skill_cfg->param_b * 1000;
// 					int dec_speed_percent = skill_cfg->param_a;
// 					int dec_movespeed = static_cast<int>(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (dec_speed_percent / 100.0f));
// 
// 					if (dec_movespeed > 0 && dur_ms > 0)												
// 					{
// 						EffectMoveSpeed *effect_speed = new EffectMoveSpeed(this->GetId(), skill_cfg->skill_id, dur_ms, EBT_CHIHUAN, skill_cfg->soul_level);
// 						effect_speed->SetMoveSpeed(-dec_movespeed, -dec_speed_percent);
// 						effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
// 						target->AddEffect(effect_speed);
// 
// 						skill_last_perform_timestamp = now_sec;
// 						is_trigger = true;
// 					}
// 				}
// 			}
// 			break;
// 
// 		case SOUL_SKILL_JIANFANG:
// 			{
// 				// 减防
// 				if (rand_value < skill_cfg->rate)
// 				{
// 					int dur_ms = skill_cfg->param_b * 1000;
// 					int dec_fangyu = (int)(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * skill_cfg->param_a / 100.0f);
// 
// 					if (dur_ms > 0 && dec_fangyu > 0)
// 					{
// 						EffectBuff *effect_buff = new EffectBuff(this->GetId(), skill_cfg->skill_id, dur_ms, skill_cfg->soul_level);
// 						effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
// 						effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, -dec_fangyu, skill_cfg->param_a);
// 						target->AddEffect(effect_buff);
// 
// 						skill_last_perform_timestamp = now_sec;
// 						is_trigger = true;
// 					}
// 				}
// 			}
// 			break;
// 
// 		default:
// 			break;
// 		}
// 	}
// 
// }

// void RoleShadow::OnAttackSoulSkill(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill)
// {
// 	time_t now_sec = EngineAdapter::Instance().Time();
// 	int active_slot_count = LOGIC_CONFIG->GetXianjianConfig().GetActiveSkillSlotCount(m_xianjian_info.active_xianjian_count);
// 
// 	for (int i = 0; i < active_slot_count; ++ i)
// 	{
// 		int rand_value = RandomNum(100);
// 		int soul_id = m_xianjian_info.soul_skill_list[i].skill_id;
// 
// 		if (soul_id < 0 || soul_id >= MAX_XIANJIAN_SOUL_COUNT)
// 		{
// 			continue;
// 		}
// 
// 		const XianjianSoulSkillConfig *skill_cfg = LOGIC_CONFIG->GetXianjianConfig().GetSoulSkillCfg(soul_id, m_xianjian_info.soul_skill_list[i].skill_level);
// 		if (NULL == skill_cfg)
// 		{
// 			continue;
// 		}
// 
// 		time_t &skill_last_perform_timestamp = m_soul_skill_last_perform_time_list[i];
// 
// 		if (now_sec < skill_last_perform_timestamp + skill_cfg->cd_s)
// 		{
// 			continue;
// 		}
// 
// 		bool is_trigger = false;
// 
// 		switch(soul_id)
// 		{
// 		case SOUL_SKILL_WUDI:
// 			{
// 				// 无敌
// 				if (rand_value < skill_cfg->rate)
// 				{
// 					int dur_ms = skill_cfg->param_b * 1000;
// 					if (dur_ms > 0)
// 					{
// 						EffectSpecialState *special_effect = new EffectSpecialState(this->GetId(), skill_cfg->skill_id, dur_ms, EBT_WUDI, skill_cfg->soul_level);
// 						this->AddEffect(special_effect);
// 
// 						skill_last_perform_timestamp = now_sec;
// 						is_trigger = true;
// 					}
// 				}
// 			}
// 			break;
// 
// 		default:
// 			break;
// 		}
// 	}
// }


