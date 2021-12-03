#include "specialchallengefield.hpp"

#include "obj/character/role.h"
#include "obj/character/roleshadow.hpp"

#include "engineadapter.h"
#include "hightimermgr.h"

#include "protocal/msgactivity.hpp"

#include "internalcomm.h"

#include "item/itembase.h"
#include "item/itempool.h"
#include "other/event/eventhandler.hpp"
#include "effect/effectbase.hpp"
#include "global/challengefield/challengefield.hpp"
#include "global/challengefield/challengeuser.hpp"
#include "scene/scene.h"
#include "globalconfig/globalconfig.h"
#include "config/logicconfigmanager.hpp"
#include "global/challengefield/challengefieldconfig.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "other/capability/capability.hpp"
#include "other/cross/rolecross.h"

SpecialChallengeField::SpecialChallengeField(Scene *scene)
	: SpecialLogic(scene), m_status(CHALLENGE_FIELD_STATE_WAITING), m_next_status_time(0), m_delay_kick_time(0), m_role_waiting_timeout(0), m_role_old_nuqi(0)
{

}

SpecialChallengeField::~SpecialChallengeField()
{

}

void SpecialChallengeField::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	// 等待超时，应该是客户端没有发准备协议，则自动准备
	if (this->IsWaiting() && m_role_waiting_timeout > 0 && now_second > m_role_waiting_timeout)
	{
		ForceToNextStatus();

		m_role_waiting_timeout = 0;
	}

	if (!this->IsFinish())
	{
		if (0 != m_next_status_time && now_second >= m_next_status_time)
		{
			this->OnChangeToNextStatus();
		}
	}
	else
	{
		if (0 != m_delay_kick_time && now_second >= m_delay_kick_time)
		{
			m_delay_kick_time = 0;

			this->DelayKickOutAllRole();
		}
	}
}

void SpecialChallengeField::OnRoleEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	m_role_old_nuqi = role->GetNuqi();
	UniqueUserID unique_user_id = role->GetRoleCross()->GetCrossOriginalUniqueUserID();

	if (!this->IsFinish())
	{
		bool is_first_enter = (INVALID_UNIQUE_USER_ID == m_side_list[CHALLENGE_FIELD_SIDE_SPONSOR].user_id);

		// 设置发起方信息
		m_side_list[CHALLENGE_FIELD_SIDE_SPONSOR].obj_id = role->GetId();
		m_side_list[CHALLENGE_FIELD_SIDE_SPONSOR].user_id = unique_user_id;

		const ChallengeFieldOtherConfig &other_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg();

		ChallengeUser *sponsor_user = ChallengeField::Instance().GetChallengeUser(unique_user_id);
		if (NULL != sponsor_user)
		{
			Attribute maxhp = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP);
			Attribute gongji = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI);
			Attribute add_maxhp = (Attribute)(maxhp * (sponsor_user->GetBuyBuffTimes() * other_cfg.buff_add_maxhp_per * ROLE_ATTR_PER));
			Attribute add_gongji = (Attribute)(gongji * (sponsor_user->GetBuyBuffTimes() * other_cfg.buff_add_gongji_per * ROLE_ATTR_PER));
			EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, 10001, 99999000, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
			effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, add_maxhp, 0);
			effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, add_gongji, 0);
			role->AddEffect(effect_buff);
		}

		if (is_first_enter)
		{
			// 设置角色信息
			{
				role->GetSkillManager()->ClearAllCD();
				role->AddNuqi(-GLOBALCONFIG->GetMaxNuqi());
				//role->GetEffectList()->ClearAllDeBuffEffect();
				role->GetEffectList()->ClearFightEffect(EffectBase::CLEAR_REASON_ENTER_SCENE);
				role->Recover();
			}

			// 创建RoleShadow
			{
				ChallengeUser *sponsor_user = ChallengeField::Instance().GetChallengeUser(unique_user_id);
				if (NULL != sponsor_user)
				{
					ChallengeUser *opponent_user = ChallengeField::Instance().GetChallengeUser(sponsor_user->GetCurrOpponentUserID()); 
					if (NULL != opponent_user)
					{
						RoleShadow *role_shadow = new RoleShadow(ROLE_SHADOW_TYPE_CHALLENGE_FIELD);
						ChallengeParam param = opponent_user->GetChallengeParam();
						RoleAppearance appearance = opponent_user->GetChallengeAppearance();
						if (opponent_user->IsRobot())
						{
							double attr_rate = param.capability * 1.0 / role->GetCapability()->GetTotalCapability();
							param.max_hp = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * attr_rate);
							param.gongji = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * attr_rate);
							param.fangyu = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU) * attr_rate);
							param.mingzhong = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG) * attr_rate);
							param.shanbi = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI) * attr_rate);
							param.baoji = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI) * attr_rate);
							param.jianren = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN) * attr_rate);
							param.fujiashanghai = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI) * attr_rate);
							param.dikangshanghai = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_DIKANG_SHANGHAI) * attr_rate);
							param.jingzhun_per = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN) * attr_rate);
							param.baoji_per = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT) * attr_rate);
							param.kangbao_per = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO) * attr_rate);
							param.pofang_per = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG) * attr_rate);
							param.mianshang_per = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG) * attr_rate);
							param.constant_zengshang = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG) * attr_rate);
							param.constant_mianshang = (Attribute)(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_MIANSHANG) * attr_rate);
						}

						param.max_hp += (Attribute)(param.max_hp * (opponent_user->GetBuyBuffTimes() * other_cfg.buff_add_maxhp_per * ROLE_ATTR_PER));
						param.gongji += (Attribute)(param.gongji * (opponent_user->GetBuyBuffTimes() * other_cfg.buff_add_gongji_per * ROLE_ATTR_PER));

						role_shadow->Init(param, appearance, 0, 0);
						role_shadow->SetPos(LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().opponent_pos);
						m_scene->AddObj(role_shadow);

						// 设置对手RoleShadow信息
						m_side_list[CHALLENGE_FIELD_SIDE_OPPONENT].user_id = role_shadow->GetUniqueUserID();
						m_side_list[CHALLENGE_FIELD_SIDE_OPPONENT].obj_id = role_shadow->GetId();
					}
				}
			}
		}

		if (INVALID_UNIQUE_USER_ID == m_side_list[CHALLENGE_FIELD_SIDE_OPPONENT].user_id)
		{
			this->OnChallengeFinish(CHALLENGE_FIELD_SIDE_OPPONENT);
		}
		else
		{
			if (this->IsFighting())
			{
				Obj *obj = m_scene->GetObj(m_side_list[CHALLENGE_FIELD_SIDE_OPPONENT].obj_id);
				if (NULL != obj && Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
				{
					RoleShadow *role_shadow = (RoleShadow *)obj;
					role_shadow->ForceSetEnemy(role->GetId());
				}
			}

			if (this->IsWaiting())
			{
				//this->ForceToNextStatus(); // 客户端需要播放特效，这里不能强制设置状态
				this->BroadcastChallengeStatus(); // 下发等待状态
			}
		}
	}

	m_role_waiting_timeout = EngineAdapter::Instance().Time() + CHALLENGE_FIELD_WAITING_TIME;

	EventHandler::Instance().OnEnterChallengeField(role);
}

void SpecialChallengeField::OnRoleLeaveScene(Role *role, bool is_logout)
{
	int role_new_nuqi = role->GetNuqi();
	int delta_nuqi = m_role_old_nuqi - role_new_nuqi;

	role->AddNuqi(delta_nuqi);

	if (!this->IsFinish())
	{
		if (m_side_list[CHALLENGE_FIELD_SIDE_SPONSOR].obj_id == role->GetId()) // 发起方退出 
		{
			this->OnChallengeFinish(CHALLENGE_FIELD_SIDE_OPPONENT);
		}
	}

	if (!role->IsAlive())
	{
		role->ReAlive(100, 100, role->GetPos(), true);
	}
	else
	{
		role->Recover();
	}
}

void SpecialChallengeField::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (!this->IsFinish())
	{
		if (m_side_list[CHALLENGE_FIELD_SIDE_SPONSOR].obj_id == dead->GetId())
		{
			this->OnChallengeFinish(CHALLENGE_FIELD_SIDE_OPPONENT);
		}
		else if (m_side_list[CHALLENGE_FIELD_SIDE_OPPONENT].obj_id == dead->GetId())
		{
			this->OnChallengeFinish(CHALLENGE_FIELD_SIDE_SPONSOR);
		}
	}
}

bool SpecialChallengeField::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (!this->IsFighting() || role->GetId() == obj->GetId()) return false;

	if (Obj::IsCharactor(obj->GetObjType()))
	{
		Character *chara = (Character*)obj;
		return chara->IsAlive();
	}

	return false;
}

bool SpecialChallengeField::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	if (!this->IsFighting()) return false;

	return (role->GetId() == obj->GetId());
}

bool SpecialChallengeField::SpecialCanSupplyEffect(Role *role, int supply_type)
{
	if (SUPPLY_TYPE_ROLE_HP == supply_type)
	{
		return false;
	}

	return true;
}

void SpecialChallengeField::OnChangeToNextStatus()
{
	switch (m_status + 1)
	{
	case CHALLENGE_FIELD_STATE_READY:
		{
			m_next_status_time += CHALLENGE_FIELD_READY_TIME;
		}
		break;

	case CHALLENGE_FIELD_STATE_FIGHTING:
		{
			m_next_status_time += CHALLENGE_FIELD_FIGHT_TIME;

			Obj *obj = m_scene->GetObj(m_side_list[CHALLENGE_FIELD_SIDE_OPPONENT].obj_id);
			if (NULL != obj && Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
			{
				RoleShadow *rs = (RoleShadow*)obj;
				rs->ForceSetEnemy(m_side_list[CHALLENGE_FIELD_SIDE_SPONSOR].obj_id);
			}
		}
		break;

	case CHALLENGE_FIELD_STATE_FINISH:
		{
			this->OnChallengeFinish(CHALLENGE_FIELD_SIDE_OPPONENT);
		}
		break;
	}

	if (CHALLENGE_FIELD_STATE_FINISH > m_status) ++ m_status;

	this->BroadcastChallengeStatus();
}

void SpecialChallengeField::BroadcastChallengeStatus()
{
	static Protocol::SCChallengeFieldStatus csi;
	csi.status = m_status;
	csi.next_status_timestamp = static_cast<unsigned int>(m_next_status_time);

	for (int side = CHALLENGE_FIELD_SIDE_SPONSOR; side < CHALLENGE_FIELD_SIDE_MAX; ++ side)
	{
		memset(&csi.side_info_list[side], 0, sizeof(csi.side_info_list[0]));

		csi.side_info_list[side].role_id = UniqueUserIDToLongLong(m_side_list[side].user_id);
		csi.side_info_list[side].obj_id = m_side_list[side].obj_id;

		Obj *obj = m_scene->GetObj(m_side_list[side].obj_id);
		if (NULL != obj)
		{
			if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
			{
				Role *role = (Role *)obj;

				csi.side_info_list[side].level = static_cast<short>(role->GetLevel());
				role->GetName(csi.side_info_list[side].name);
				csi.side_info_list[side].camp = role->GetCamp();
				csi.side_info_list[side].prof = role->GetProf();
				csi.side_info_list[side].avatar = role->GetAvatar();
				csi.side_info_list[side].sex = role->GetSex();
				csi.side_info_list[side].hp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				csi.side_info_list[side].max_hp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				csi.side_info_list[side].mp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP);
				csi.side_info_list[side].max_mp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP);
				csi.side_info_list[side].speed = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
				csi.side_info_list[side].pos_x = static_cast<short>(role->GetPos().x);
				csi.side_info_list[side].pos_y = static_cast<short>(role->GetPos().y);
				csi.side_info_list[side].dir = role->GetDir();
				csi.side_info_list[side].distance = role->GetDirDistance();
				csi.side_info_list[side].capability = role->GetCapability()->GetTotalCapability();
				csi.side_info_list[side].guild_id = role->GetGuildID();
				csi.side_info_list[side].guild_post = role->GetGuildPost();
				role->GetGuildName(csi.side_info_list[side].guild_name);
			}
			else if (Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
			{
				RoleShadow *role_shadow = (RoleShadow *)obj;

				csi.side_info_list[side].level = static_cast<short>(role_shadow->GetLevel());
				role_shadow->GetName(csi.side_info_list[side].name);
				csi.side_info_list[side].camp = role_shadow->GetCamp();
				csi.side_info_list[side].prof = role_shadow->GetProf();
				csi.side_info_list[side].avatar = role_shadow->GetAvatar();
				csi.side_info_list[side].sex = role_shadow->GetSex();
				csi.side_info_list[side].hp = role_shadow->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				csi.side_info_list[side].max_hp = role_shadow->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				csi.side_info_list[side].mp = role_shadow->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP);
				csi.side_info_list[side].max_mp = role_shadow->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP);
				csi.side_info_list[side].speed = role_shadow->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
				csi.side_info_list[side].pos_x = static_cast<short>(role_shadow->GetPos().x);
				csi.side_info_list[side].pos_y = static_cast<short>(role_shadow->GetPos().y);
				csi.side_info_list[side].dir = role_shadow->GetDir();
				csi.side_info_list[side].distance = role_shadow->GetDirDistance();
				csi.side_info_list[side].capability = role_shadow->GetCapability();
				csi.side_info_list[side].guild_id = role_shadow->GetGuildID();
				csi.side_info_list[side].guild_post = role_shadow->GetGuildPost();
				role_shadow->GetGuildName(csi.side_info_list[side].guild_name);
			}
		}
	}

	m_scene->SendToRole((const char *)&csi, sizeof(csi));
}

void SpecialChallengeField::ForceToNextStatus()
{
	m_next_status_time = EngineAdapter::Instance().Time();
}

void SpecialChallengeField::OnChallengeFinish(int win_side)
{
	if (this->IsFinish()) return;

	m_status = CHALLENGE_FIELD_STATE_FINISH;
	m_next_status_time = 0;

	{
		Obj *obj = m_scene->GetObj(m_side_list[CHALLENGE_FIELD_SIDE_OPPONENT].obj_id);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
		{
			RoleShadow *rs = (RoleShadow*)obj;
			rs->ForceSetEnemy(INVALID_OBJ_ID);
		}
	}

	ChallengeField::Instance().OnChallengeResult(m_side_list[CHALLENGE_FIELD_SIDE_SPONSOR].user_id, 
		m_side_list[CHALLENGE_FIELD_SIDE_OPPONENT].user_id, CHALLENGE_FIELD_SIDE_SPONSOR == win_side);

	m_delay_kick_time = EngineAdapter::Instance().Time() + CHALLENGE_FIELD_DELAY_KICK_TIME;

	this->BroadcastChallengeStatus();
}

void SpecialChallengeField::StartReadyFightCountdownReq(Role *role)
{
	//if (NULL == role)
	//{
	//	return;
	//}

	time_t now = EngineAdapter::Instance().Time();

	if (IsWaiting() && m_role_waiting_timeout > 0 && now <= m_role_waiting_timeout)
	{
		ForceToNextStatus();

		OnChangeToNextStatus();

		m_role_waiting_timeout = 0;
	}
}

