#include <memory.h>

#include "gamecommon.h"
#include "character.h"
#include "scene/scene.h"
#include "scene/map.h"
#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgfight.h"
#include "protocal/msgscene.h"
#include "obj/character/role.h"
#include "obj/character/roleshadow.hpp"

#include "servercommon/errornum.h"
#include "servercommon/commondata.hpp"
#include "globalconfig/globalconfig.h"
#include "engineadapter.h"

#include "world.h"

#include "effect/effectbase.hpp"

#include "protocal/msgrole.h"

#include "servercommon/performancecatch.hpp"

#include "effect/skilleffect/effectonce.hpp"

#include "scene/scenemanager.h"
#include "scene/marryshadow/marryshadow.h"
#include "scene/speciallogic/worldspecial/specialhunyan.hpp"
#include "serverlogic.h"
#include "gameworld/gameworld/other/worldboss/worldbossshadow.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/upgradeskill/upgradeskillconfig.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/footprint/footprint.hpp"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "other/shenshou/shenshou.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/rolezhuanzhi/rolezhuanzhi.hpp"
#include "other/mojie/mojieskill.hpp"
#include "effect/skilleffect/effectbuff.hpp"

Character::Character(ObjType type)
	: Obj(type), 
	m_last_check_valid_time(0), m_last_check_pos(0, 0),
	m_action_status(ACTION_IDLE),
	m_logic_result(LOGIC_NONE),
	m_cooldown_end_time(0), 
	m_dead_time(0),
	m_old_dir(Dir_S), m_old_cos(0), m_old_sin(0),
	m_attack_seq(0), m_is_mianyi_buff(false)
{
}

Character::Character(ObjType type, const GameName _name)
	: Obj(type, INVALID_OBJ_ID, _name), 
	m_last_check_valid_time(0), m_last_check_pos(0, 0),
	m_action_status(ACTION_IDLE),
	m_logic_result(LOGIC_NONE),
	m_cooldown_end_time(0), 
	m_dead_time(0),
	m_old_dir(Dir_S), m_old_cos(0), m_old_sin(0),
	m_attack_seq(0), m_is_mianyi_buff(false)
{
}

Character::~Character()
{
	
}

void Character::Init(const CharIntAttrs &int_attr, const SkillListParam &skill, const SkillOtherParam &skill_other, int effect_buff_len, const char *effect_buff)
{
	m_int_attrs = int_attr;
	m_effect_list.Init(this, effect_buff_len, effect_buff); 
	m_skill_manager.Init(this, skill, skill_other);
}

bool Character::CanCharacterAction(bool is_notify_client, bool is_move)
{
	if (!this->IsAlive())
	{
		if (is_notify_client && Obj::OBJ_TYPE_ROLE == m_obj_type)
		{
			Role *roleThis = (Role*)this;

			Protocol::SCActionReject ar;
			ar.action_reject_reason = Protocol::SCActionReject::ACTION_REJECT_REASON_DIE;
			EngineAdapter::Instance().NetSend(roleThis->GetNetId(), (const char *)&ar, sizeof(ar));
		}

		return false;
	}

	if (this->IsXuanYun())
	{
		return false;
	}

	if (!is_move && m_effect_list.IsBianshen())
	{
		return false;
	}

	return true;
}

bool Character::Move(Axis _x, Axis _y, Dir _dir, Scalar distance, short height, bool check_valid, short press_onward)
{
	if (NULL == m_scene) return false;

	// 合法性验证
	if (m_obj_type == Obj::OBJ_TYPE_ROLE && (Role::MOVE_MODE_FLY == ((Role*)this)->GetMoveMode())) // 角色飞行模式下可以不用检查地图可走点
	{
		if (_dir != Dir_S && fabs(_dir) > Dir_Max /* _dir >= Dir_Count*/)
		{
			return false;
		}
	}
	else
	{
		if ((this->CheckMapValidate() && !m_scene->GetMap()->Validate(m_obj_type, _x, _y)) || (_dir != Dir_S && fabs(_dir) > Dir_Max) /* _dir >= Dir_Count*/)
		{
			return false;
		}
	}

	// 如果是客户端发来的消息，则需要验证与当前位置的距离是否合法
	bool is_check_valid = GLOBALCONFIG->GetOtherConfig_IsCheckValid();
	if (check_valid && is_check_valid)
	{
		const static unsigned int max_move_speed = GLOBALCONFIG->GetOtherConfig_MaxMoveSpeed();
		const static Axis move_check_length = (Axis)GLOBALCONFIG->GetOtherConfig_MoveCheck_Length();

		Posi to_pos(_x, _y);

		{
			// 检查当前位置偏差，允许在一定范围内
			Posi delta_pos = m_posi - to_pos;
			if (delta_pos.x > move_check_length || delta_pos.x < -move_check_length
				|| delta_pos.y > move_check_length || delta_pos.y < -move_check_length)
			{
				// 又是个挂
				//printf("cur:%d,%d  to:%d,%d       ****1\n", m_posi.x, m_posi.y, to_pos.x, to_pos.y);
				if (m_obj_type == Obj::OBJ_TYPE_ROLE)
				{
					((Role*)this)->ResetPos(m_posi.x, m_posi.y);
				}
				return false;
			}
		}

		{
			// 如果过了当前位置偏差的检查，则防止短时间内发送大量在允许偏差范围内的包
			// 此时则需要检查速度
			unsigned int now = EngineAdapter::Instance().GetGameTime();
			unsigned int time_dis = now - m_last_check_valid_time;

			// 直接用当前速度判断可能会有问题，因为在一次矢量运动过程中速度可能改变
			// 所以需要由策划配置一个最大速度来判断
			Axis dis = Axis(time_dis * float(max_move_speed) / 100000.0f);
			dis > 1000 ? (dis = 1000) : 0;

			//float check_dis = (m_last_check_pos).Dist(to_pos);
			//printf("%d+%d=%d %f\n", dis, move_check_length, dis + move_check_length, check_dis);

			if ((m_last_check_pos - to_pos).IsLongerThan(dis + move_check_length))
			{
				// 这小子用挂，拦截掉
				//printf("last:%d,%d  to:%d,%d dis:      ****2\n", m_last_check_pos.x, m_last_check_pos.y, to_pos.x, to_pos.y, dis);
				if (m_obj_type == Obj::OBJ_TYPE_ROLE)
				{
					((Role*)this)->ResetPos(m_posi.x, m_posi.y);
				}
				return false;
			}

			if (m_last_check_valid_time != now || !this->CheckMoveFrequency())
			{
				// 同一帧内收到多条移动协议是有可能的，这时只记录第一次
				m_last_check_valid_time = now;
				m_last_check_pos = to_pos;
			}
		}
	}

	if (check_valid && !CanCharacterAction(true, true))
	{
		return false;
	}

	if (check_valid && this->IsDingShen())
	{
		return false;
	}
	
	StopAttack();

	m_posf.Init((float)_x, (float)_y);
	m_posi.Init(_x, _y);
	m_dir = _dir;
	m_dir_distance = distance;
	m_height = height;
	m_scene->GetZoneMatrix()->MoveObser(m_obser_handle);
	m_action_status = (m_dir == Dir_S ? ACTION_IDLE : ACTION_RUN);

	SendPosiInfo(press_onward);

	// printf("Move [%d, %d] dir:%d distance:%f.\n", _x, _y, (int)m_dir, distance);

	return true;
}

bool Character::MoveBySkill(Axis _x, Axis _y, Dir _dir, Scalar distance, bool check_special_state)
{
	// 合法性验证
	if (!m_scene->GetMap()->IsValidatePos(m_obj_type, _x, _y) || (_dir != Dir_S && fabs(_dir) > Dir_Max) /* _dir >= Dir_Count*/)
	{
		return false;
	}

	if (check_special_state)
	{
		if (!CanCharacterAction())
		{
			return false;
		}

		if (this->IsDingShen())
		{
			return false;
		}
	}

	m_posf.Init((float)_x, (float)_y);
	m_posi.Init(_x, _y);
	m_dir = _dir;
	m_dir_distance = distance;
	m_scene->GetZoneMatrix()->MoveObser(m_obser_handle);	

	// SendPosiInfo();

	return true;
}

bool Character::SetDir(Dir _dir, Scalar distance)
{
	// 检查状态
	if (!CanCharacterAction(false, true))
	{
		return false;
	}

	if (this->IsDingShen())
	{
		return false;
	}

	if (fabs(_dir) < Dir_Max)
	{
		StopAttack();

		m_dir = _dir;
		m_dir_distance = distance;
		m_action_status = (m_dir == Dir_S ? ACTION_IDLE : ACTION_RUN);
		SendPosiInfo();

		return true;
	}

	return false;
}

void Character::ActionStop()
{
	StopAttack();

	bool not_static = false;
	if (!IsStatic())
	{
		not_static = true;
	}
	
	// m_dir = Dir_S;

	m_dir_distance = 0;
	m_action_status = ACTION_IDLE;

	if (not_static)
	{
		SendPosiInfo();
	}
}

void Character::MoveStop()
{
	if (m_action_status == ACTION_RUN && !IsStatic())
	{
		m_dir_distance = 0;
		m_action_status = ACTION_IDLE;
		SendPosiInfo();
	}
}

void Character::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	m_logic_result = LOGIC_NONE;

	if (IsAlive())
	{
		m_dead_time = 0;

		switch (m_action_status)
		{
		case ACTION_IDLE:
			break;

		case ACTION_RUN:
			m_logic_result = MoveLogic(interval);
			break;

		case ACTION_ATTACK:
			/*if (Obj::OBJ_TYPE_ROLE != m_obj_type && Obj::OBJ_TYPE_MONSTER != m_obj_type)
			{
				m_logic_result = AttackLogic(interval);
			}*/
			break;

		default:
			break;
		}

		bool die = false;
		m_effect_list.Effect(interval, this, &die);
	}
	else 
	{
		m_dead_time += interval;
	}
}

int Character::MoveLogic(unsigned long interval)
{
	if (NULL == m_scene) return MOVE_STATIC; 

	if (m_dir == Dir_S)
	{
		m_action_status = ACTION_IDLE;
		return MOVE_STATIC;
	}

	//变更客户端格子大小从0.5 变更至1, 整体减移动速度50%
	Scalar distance = (Scalar)GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) / 100.0f * interval / 1000.0f * this->GetMoveRate();
	distance = (distance < m_dir_distance ? distance : m_dir_distance);
		
	if (m_old_dir != m_dir)
	{
		m_old_dir = m_dir;
		m_old_cos = cos(m_dir); m_old_sin = sin(m_dir); 
	}

	// Posf pos = m_posf + gamecommon::g_move_dir_vec[int(m_dir)] * distance;
	// Posf diff_pos(distance * m_old_cos, distance * m_old_sin);
	Posf pos = m_posf + Posf(m_old_cos, m_old_sin) * distance;

	bool check_map_validate = this->CheckMapValidate();
	if (!check_map_validate || (check_map_validate && m_scene->GetMap()->Validate(m_obj_type, (Axis)pos.x, (Axis)pos.y)))
	{
		m_posf = pos;
		m_posi.Init((Axis)m_posf.x, (Axis)m_posf.y);
		m_dir_distance -= distance;

		//printf("x %d y %d dis %f m_dis %f\n", m_posi.x, m_posi.y, distance, m_dir_distance);

		m_scene->GetZoneMatrix()->MoveObser(m_obser_handle);
		if (m_dir_distance <= 0)
		{
			m_action_status = ACTION_IDLE;

			if (Obj::OBJ_TYPE_MARRY == m_obj_type)
			{
				// 婚宴-巡游
				SpecialHunyan *special_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialHunyan(COMMON_SCENE_KEY, false);
				if (NULL != special_logic && Scene::SCENE_TYPE_HUN_YAN_FB == special_logic->GetSceneType())
				{
					special_logic->OnCharacterMoveStop(this);
				}
			}
		}
	}
	else
	{
		MoveStop();
		return MOVE_STATIC;
	}

	return MOVE_RUN;
}

void Character::SendFlyHeightChangeInfo()
{
	Protocol::SCObjFlyHeightChange msg;

	msg.obj_id = m_obj_id;
	msg.fly_height = m_fly_height;

	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&msg, sizeof(Protocol::SCObjFlyHeightChange));
}

void Character::SendPosiInfo(short press_onward)
{
	Protocol::SCObjMove scum;

	scum.dir = m_dir;
	scum.press_onward = press_onward;
	scum.obj_id = m_obj_id;
	scum.pos_x = (short)m_posi.x;
	scum.pos_y = (short)m_posi.y;
	scum.distance = m_dir_distance;
	scum.move_mode = 0;
	if (Obj::OBJ_TYPE_ROLE == m_obj_type)
	{
		Role *this_role = (Role *)this;
		scum.move_mode = this_role->GetMoveMode();
	}
	scum.height = m_height;

	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&scum, sizeof(Protocol::SCObjMove));
}

bool Character::IsAlive()
{
	return m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP) > 0;
}

bool Character::StartAttack(ObjID obj_id)
{
	if (!CanCharacterAction(true)) return false; 

	Obj *target = m_scene->GetObj(obj_id);
	if (NULL == target) return false;
	
	if (!Obj::IsCharactor(target->GetObjType())) return false;
	
	if (OBJ_TYPE_ROLE == m_obj_type)
	{
		Map *map = m_scene->GetMap();
		Posi target_pos = target->GetPos();

		if (map->SaveArea(m_posi.x, m_posi.y))
		{
			Role *pthis = (Role*)this;
			pthis->NoticeNum(errornum::EN_SAVEAREA_FIGHT);
			return false;
		}
		if (map->SaveArea(target_pos.x, target_pos.y))
		{
			Role *pthis = (Role*)this;
			pthis->NoticeNum(errornum::EN_SAVEAREA_FIGHT_TARGET);
			return false;
		}

		Role *target_role = m_scene->GetPrincipal(obj_id);
		if (NULL != target_role && target_role != this)
		{
			const int XINSHOU_PROTECTED_LEVEL = GLOBALCONFIG->GetRoleXinShouProtectedLevel();
			if (target_role->GetLevel() < XINSHOU_PROTECTED_LEVEL)
			{
				Role *pthis = (Role*)this;
				pthis->NoticeNum(errornum::EN_ROLE_XINSHOU_PROTECT);
				return false;
			}

			if (m_scene->ForbidPK())
			{
				Role *pthis = (Role*)this;
				pthis->NoticeNum(errornum::EN_SCENE_FORBID_PK);
				return false;
			}
		}
	}

	if (!IsEnemy(target)) return false;

	if (m_action_status == ACTION_RUN && m_dir != Dir_S)
	{
		ActionStop();
	}

	m_action_status = ACTION_ATTACK;

	return true;
}

void Character::StopAttack()
{
	if (ACTION_ATTACK == m_action_status)
	{
		m_action_status = ACTION_IDLE;

		if (NULL != m_scene) m_scene->GetSpecialLogic()->OnStopAttack(this);
	}
}

// 执行技能
bool Character::PerformSkill(int skill_index, const Posi &pos, ObjID target_id, int check_flag, int special_param, int skill_data)
{
	if (NULL == m_scene) 
		return false;

	Character *target = NULL;
	Posi target_pos;

	bool is_skill_atk_chara = false;

	if (m_skill_manager.CheckSkillAttackCharacter(skill_index))				// 打角色
	{
		is_skill_atk_chara = true;

		Obj *target_obj = m_scene->GetObj(target_id);
		if (NULL == target_obj || !Obj::IsCharactor(target_obj->GetObjType()))
		{
			return false;
		}

		target = (Character *)target_obj;
		target_pos = target->GetPos();

		if (0 != (check_flag & SKILL_CHECK_FLAG_TARGET) && m_skill_manager.CheckSkillTargetEnemy(skill_index))				// 对敌人
		{
			// 1. 检查敌我关系
			if (!target->IsAlive()) 
				return false;

			if (!IsEnemy(target))
			{
				if (OBJ_TYPE_ROLE == m_obj_type)
				{
					this->NoticeNum(errornum::EN_ROLE_FIGHT_TARGET_NOT_ENEMY);
				}
				return false;
			}

			// 2. 检查新手保护
			if (OBJ_TYPE_ROLE == m_obj_type)								// 如果是角色发起攻击
			{
				Role *target_role = m_scene->GetPrincipal(target_id);
				if (NULL != target_role && this != target_role)
				{
					const int XINSHOU_PROTECTED_LEVEL = GLOBALCONFIG->GetRoleXinShouProtectedLevel();
					if (target_role->GetLevel() < XINSHOU_PROTECTED_LEVEL && m_scene->InCommonScene() && !Scene::IsBossScene(m_scene->GetSceneID()))  // 2019-04-02普通非活动场景才有新手保护
					{
						this->NoticeNum(errornum::EN_ROLE_XINSHOU_PROTECT);
						return false;
					}

					if (m_scene->ForbidPK())
					{
						this->NoticeNum(errornum::EN_SCENE_FORBID_PK);
						return false;
					}

					if (target_role->GetMoveMode() != Role::MOVE_MODE_NORMAL)
					{
						this->NoticeNum(errornum::EN_ROLE_FLY_PROTECT);
						return false;
					}
				}
			}
		}
	}
	else
	{
		target_pos = pos;
	}

	if (!m_skill_manager.SetNextSkill(skill_index, pos, target_id))
	{
		return false;
	}

	Map *map = m_scene->GetMap();
	if (!m_skill_manager.NextValidInSaveArea() && map->SaveArea(m_posi.x, m_posi.y))
	{
		if (OBJ_TYPE_ROLE == m_obj_type)
		{
			this->NoticeNum(errornum::EN_SAVEAREA_FIGHT);
		}
		return false;
	}
	if (!m_skill_manager.NextValidInSaveArea() && map->SaveArea(target_pos.x, target_pos.y))
	{
		if (OBJ_TYPE_ROLE == m_obj_type)
		{
			this->NoticeNum(errornum::EN_SAVEAREA_FIGHT_TARGET);
		}
		return false;
	}

	if (this->IsFool())
	{
		if (Obj::OBJ_TYPE_ROLE == GetObjType())
		{
			this->NoticeNum(errornum::EN_FIGHT_BIANXING_FOOL);
		}

		return false;
	}

	if (!m_skill_manager.NextIsDefaultSkill() && this->IsChenMo())
	{
		if (Obj::OBJ_TYPE_ROLE == GetObjType())
		{
			this->NoticeNum(errornum::EN_ROLE_FIGHT_CHENMO_STATUS);
		}

		return false;
	}

	if (m_skill_manager.NextIsChongfengSkill() && this->IsDingShen())
	{
		if (Obj::OBJ_TYPE_ROLE == GetObjType())
		{
			//this->NoticeNum(errornum::EN_ROLE_FIGHT_DINGSHEN_STATUS);
		}

		return false;
	}

// 	if (m_skill_manager.NextIsNuqiSkill())
// 	{
// 		if (Obj::OBJ_TYPE_ROLE != GetObjType()) return false;
// 
// 		Role *this_role = (Role *)this;
// 		if (this_role->GetNuqi() < GLOBALCONFIG->GetMaxNuqi())
// 		{
// 			this->NoticeNum(errornum::EN_ROLE_NUQI_NOT_ENOUGH);
// 			return false;
// 		}
// 	}

	// 检查攻击距离够不够
	if (0 != (check_flag & SKILL_CHECK_FLAG_DISTANCE) && !m_skill_manager.CheckNextSkillDistance(target))
	{
		if (Obj::OBJ_TYPE_ROLE == GetObjType()) 
		{
			this->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);

			if (is_skill_atk_chara && NULL != target)
			{
				Role *this_role = (Role *)this;

				Protocol::SCSkillTargetPos stp;
				stp.target_obj_id = target->GetId();
				stp.pos_x = target->GetPos().x;
				stp.pos_y = target->GetPos().y;

				EngineAdapter::Instance().NetSend(this_role->GetNetId(), (const char *)&stp, sizeof(stp));
			}
		}

		return false;
	}

	if (0 != (check_flag & SKILL_CHECK_FLAG_CD) && !m_skill_manager.CheckNextSkillCoolDown())
	{
		if (Obj::OBJ_TYPE_ROLE == GetObjType()) 
		{
			this->NoticeNum(errornum::EN_ROLE_FIGHT_CD_NOT_ENOUGH);
		}

		return false;
	}

	if (m_skill_manager.NextStopActionwhenPerform())	// 有些技能不需要停止移动
	{
		if (m_action_status == ACTION_RUN && m_dir != Dir_S)
		{
			ActionStop();
		}

		m_action_status = ACTION_ATTACK;
	}

	m_skill_manager.SetNextSkillData(skill_data);
	m_skill_manager.PerformNextSkill(target, special_param);

	Attribute cooldown_time = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GLOBAL_COOLDOWN);
	m_effect_list.ModifyGlobalCD(&cooldown_time);
	m_cooldown_end_time = ServerLogic::GetInstServerLogic()->GetPiTime() + (unsigned long)cooldown_time;

	return true;
}

// 受击，伤害重算
void Character::OnAttackCalcHurt(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill, int fight_type)
{
	m_scene->GetSpecialLogic()->OnAttackCalcHurt(this, attacker, delta_hp);
}

// 这个函数不能修改delta_hp，需要修改delta_hp继承OnAttackCalcHurt修改
void Character::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill)
{
	m_scene->GetSpecialLogic()->OnAttack(this, attacker, delta_hp);
	if (from_skill)
	{
		if (Obj::OBJ_TYPE_ROLE == m_obj_type)		// 被打加怒气
		{
			Role *this_role = (Role *)this;
			this_role->AddNuqi(GLOBALCONFIG->GetNuqiAtked());
		}

		{
			Obj *atk_obj = m_scene->GetObj(attacker);
			if (NULL != atk_obj)
			{
				if (Obj::IsCharactor(atk_obj->GetObjType()))
				{
					m_skill_manager.OnAttack((Character *)atk_obj, skill_id, delta_hp, from_skill);
				}

				if (Obj::OBJ_TYPE_ROLE == atk_obj->GetObjType())
				{
					Role *atk_role = (Role *)atk_obj;
					atk_role->AddNuqi(GLOBALCONFIG->GetNuqiAtkOther());
				}
			}
		}
	}
}

int Character::OnHurtOther(Character *target, int skill_id, long long &injure, int fight_type, int target_index)
{
	// 计算被动技能效果
	m_skill_manager.OnHurtOther(target, skill_id, injure, fight_type);

	// 攻城战继承此接口 SpecialGongChengZhan::OnHurtOther
	m_scene->GetSpecialLogic()->OnHurtOther(this, target);
	return 0;
}

int Character::CalcUpGradeSkillEffect(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, Attribute &holy_delta_hp, bool from_skill, bool is_fix_hurt)
{
	int effect_type = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_NON;

	if (delta_hp >= 0) return effect_type;

	Obj* attacker_obj = m_scene->GetObj(attacker);
	if (NULL == attacker_obj || Obj::OBJ_TYPE_ROLE != attacker_obj->GetObjType()) return effect_type;

	Role *attacker_role = (Role *)attacker_obj;
	CommonDataParam *common_data = attacker_role->GetCommonDataParam();
	if (NULL == common_data) return effect_type;

	int active_skill_count = 0;
	for (int skill_index = UpGradeSkillConfig::SKILL_TYPE_MOUNT; skill_index < UpGradeSkillConfig::SKILL_TYPE_MAX; skill_index++)
	{
		if (0 != (common_data->upgrade_skill_skill_active_flag & (1 << skill_index)))
		{
			active_skill_count++;
		}
	}

	if (active_skill_count <= 0) return effect_type;

	int need_gauge = LOGIC_CONFIG->GetUpGradeSkillCfg().GetGaugeBySkillCount(active_skill_count);
	if (need_gauge <= 0) return effect_type;

	if (common_data->upgrade_skill_next_skill_index < UpGradeSkillConfig::SKILL_TYPE_MOUNT || common_data->upgrade_skill_next_skill_index >= UpGradeSkillConfig::SKILL_TYPE_MAX)
	{
		common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_MOUNT;
	}

	if (0 <= common_data->upgrade_skill_attack_count && common_data->upgrade_skill_attack_count < need_gauge)
	{
		return effect_type;
	}
	else
	{
		common_data->upgrade_skill_attack_count = 0;
	}

	bool is_perform_skill = false;
	for(int i = UpGradeSkillConfig::SKILL_TYPE_MOUNT; i < UpGradeSkillConfig::SKILL_TYPE_MAX && !is_perform_skill; i++)
	{
		if (0 == (common_data->upgrade_skill_skill_active_flag & (1 << common_data->upgrade_skill_next_skill_index)))
		{
			common_data->upgrade_skill_next_skill_index++;

			if (common_data->upgrade_skill_next_skill_index < UpGradeSkillConfig::SKILL_TYPE_MOUNT || common_data->upgrade_skill_next_skill_index >= UpGradeSkillConfig::SKILL_TYPE_MAX)
			{
				common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_MOUNT;
			}

			continue;
		}

		if (0 != holy_delta_hp)
		{
			delta_hp += holy_delta_hp;
		}
		else
		{
			if (is_fix_hurt)
			{
				delta_hp += delta_hp;
				holy_delta_hp = delta_hp;
			}
			else
			{
				int injure_per = this->GetAttackerGradeSkillInjurePer(attacker_role, common_data->upgrade_skill_next_skill_index);
				if (injure_per > 0)
				{
					Attribute holy_hp = static_cast<Attribute>(attacker_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (injure_per * ROLE_ATTR_PER));
					if (holy_hp > 0)
					{
						delta_hp -= holy_hp;
						holy_delta_hp = -holy_hp;
					}
				}
			}
		}

		switch(common_data->upgrade_skill_next_skill_index)
		{
		case UpGradeSkillConfig::SKILL_TYPE_MOUNT:
			{
				common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_WING;
				effect_type = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_MOUNT;
				is_perform_skill = true;
			}
			break;

		case UpGradeSkillConfig::SKILL_TYPE_WING:
			{
				common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_HALO;
				effect_type = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_WING;
				is_perform_skill = true;
			}
			break;

		case UpGradeSkillConfig::SKILL_TYPE_HALO:
			{
				common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_SHENGONG;
				effect_type = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_HALO;
				is_perform_skill = true;
			}
			break;

		case UpGradeSkillConfig::SKILL_TYPE_SHENGONG:
			{
				common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_SHENYI;
				effect_type = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_SHENGONG;
				is_perform_skill = true;
			}
			break;

		case UpGradeSkillConfig::SKILL_TYPE_SHENYI:
			{
				common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_FIGHT_MOUNT;
				effect_type = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_WATER_SHENYI;
				is_perform_skill = true;
			}
			break;

		case UpGradeSkillConfig::SKILL_TYPE_FIGHT_MOUNT:
			{
				common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_FOOTPRINT;
				effect_type = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_FIGHT_MOUNT;
				is_perform_skill = true;
			}
			break;

		case UpGradeSkillConfig::SKILL_TYPE_FOOTPRINT:
			{
				common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_FABAO;
				effect_type = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_FOOTPRINT;
				is_perform_skill = true;
			}
			break;

		case UpGradeSkillConfig::SKILL_TYPE_FABAO:
			{
				common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_MOUNT;
				effect_type = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_FABAO;
				is_perform_skill = true;
			}
			break;

		default:
			{
				common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_MOUNT;
			}
			break;
		}
	}

	// 计算下一个技能index
	for(int j = UpGradeSkillConfig::SKILL_TYPE_MOUNT; j < UpGradeSkillConfig::SKILL_TYPE_MAX; j++)
	{
		if (0 == (common_data->upgrade_skill_skill_active_flag & (1 << common_data->upgrade_skill_next_skill_index)))
		{
			common_data->upgrade_skill_next_skill_index++;

			if (common_data->upgrade_skill_next_skill_index < UpGradeSkillConfig::SKILL_TYPE_MOUNT || common_data->upgrade_skill_next_skill_index >= UpGradeSkillConfig::SKILL_TYPE_MAX)
			{
				common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_MOUNT;
			}

			continue;
		}
		else
		{
			break;
		}
	}

	this->SendGradeSkillInfo(attacker_role);

	return effect_type;
}

int Character::GetAttackerGradeSkillInjurePer(Role *attacker_role, int skill_type)
{
	if (NULL == attacker_role || skill_type < UpGradeSkillConfig::SKILL_TYPE_MOUNT || skill_type >= UpGradeSkillConfig::SKILL_TYPE_MAX) return 0;

	int skill_level = 0;
	switch(skill_type)
	{
	case UpGradeSkillConfig::SKILL_TYPE_MOUNT:
		{
			skill_level = attacker_role->GetMountManager()->GetMountEquipSkillLevel();
			UpGradeSkillLevelCfg* skill_cfg = LOGIC_CONFIG->GetUpGradeSkillCfg().GetSkillLevelCfg(UpGradeSkillConfig::SKILL_TYPE_MOUNT, skill_level);
			if (NULL != skill_cfg) return skill_cfg->injure_per;
		}
		break;

	case UpGradeSkillConfig::SKILL_TYPE_WING:
		{
			skill_level = attacker_role->GetWing()->GetWingEquipSkillLevel();
			UpGradeSkillLevelCfg* skill_cfg = LOGIC_CONFIG->GetUpGradeSkillCfg().GetSkillLevelCfg(UpGradeSkillConfig::SKILL_TYPE_WING, skill_level);
			if (NULL != skill_cfg) return skill_cfg->injure_per;
		}
		break;

	case UpGradeSkillConfig::SKILL_TYPE_HALO:
		{
			skill_level = attacker_role->GetHalo()->GetHaloEquipSkillLevel();
			UpGradeSkillLevelCfg* skill_cfg = LOGIC_CONFIG->GetUpGradeSkillCfg().GetSkillLevelCfg(UpGradeSkillConfig::SKILL_TYPE_HALO, skill_level);
			if (NULL != skill_cfg) return skill_cfg->injure_per;
		}
		break;

	case UpGradeSkillConfig::SKILL_TYPE_SHENGONG:
		{
			skill_level = attacker_role->GetShengong()->GetShengongEquipSkillLevel();
			UpGradeSkillLevelCfg* skill_cfg = LOGIC_CONFIG->GetUpGradeSkillCfg().GetSkillLevelCfg(UpGradeSkillConfig::SKILL_TYPE_SHENGONG, skill_level);
			if (NULL != skill_cfg) return skill_cfg->injure_per;
		}
		break;

	case UpGradeSkillConfig::SKILL_TYPE_SHENYI:
		{
			skill_level = attacker_role->GetShenyi()->GetShenyiEquipSkillLevel();
			UpGradeSkillLevelCfg* skill_cfg = LOGIC_CONFIG->GetUpGradeSkillCfg().GetSkillLevelCfg(UpGradeSkillConfig::SKILL_TYPE_SHENYI, skill_level);
			if (NULL != skill_cfg) return skill_cfg->injure_per;
		}
		break;

	case UpGradeSkillConfig::SKILL_TYPE_FIGHT_MOUNT:
		{
			skill_level = attacker_role->GetFightMountManager()->GetFightMountEquipSkillLevel();
			UpGradeSkillLevelCfg* skill_cfg = LOGIC_CONFIG->GetUpGradeSkillCfg().GetSkillLevelCfg(UpGradeSkillConfig::SKILL_TYPE_FIGHT_MOUNT, skill_level);
			if (NULL != skill_cfg) return skill_cfg->injure_per;
		}
		break;

	case UpGradeSkillConfig::SKILL_TYPE_FOOTPRINT:
		{
			skill_level = attacker_role->GetFootPrint()->GetFootprintEquipSkillLevel();
			UpGradeSkillLevelCfg* skill_cfg = LOGIC_CONFIG->GetUpGradeSkillCfg().GetSkillLevelCfg(UpGradeSkillConfig::SKILL_TYPE_FOOTPRINT, skill_level);
			if (NULL != skill_cfg) return skill_cfg->injure_per;
		}
		break;

	case UpGradeSkillConfig::SKILL_TYPE_FABAO:
		{
			skill_level = attacker_role->GetFabaoManager()->GetFabaoEquipSkillLevel();
			UpGradeSkillLevelCfg* skill_cfg = LOGIC_CONFIG->GetUpGradeSkillCfg().GetSkillLevelCfg(UpGradeSkillConfig::SKILL_TYPE_FABAO, skill_level);
			if (NULL != skill_cfg) return skill_cfg->injure_per;
		}
		break;
	}

	return 0;
}

void Character::SendGradeSkillInfo(Role *role)
{
	if (NULL == role) return;

	static Protocol::SCUpGradeSkillInfo skill_info;
	skill_info.upgrade_cur_calc_num = role->GetCommonDataParam()->upgrade_skill_attack_count;
	skill_info.upgrade_next_skill = role->GetCommonDataParam()->upgrade_skill_next_skill_index;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&skill_info, sizeof(skill_info));
}

void Character::OnDie(ObjID killer)
{
	ActionStop();

	m_dead_time = 0;

	m_scene->GetSpecialLogic()->OnCharacterDie(this, killer);
	unsigned int continue_kill_max_interval = GLOBALCONFIG->GetRoleCfg().GetTriggerContinueKillMaxInterval();
	if (this->GetId() != killer && continue_kill_max_interval > 0)
	{
		Obj *killer_obj = m_scene->GetObj(killer);
		if (NULL != killer_obj && OBJ_TYPE_ROLE == killer_obj->GetObjType())
		{
			Role *killer_role = (Role *)killer_obj;
			unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			CommonDataParam *common_data = killer_role->GetCommonDataParam();

			if (now_sec > common_data->trigger_continue_kill_invalid_timestamp || common_data->continue_kill_count < 0)
			{
				common_data->continue_kill_count = 1;
			}
			else
			{
				++common_data->continue_kill_count;

				if (now_sec - common_data->last_add_continue_kill_buff_timestamp > 1)
				{
					globalconfig::RoleContinueKillCfg *kill_cfg = GLOBALCONFIG->GetRoleCfg().GetContinueKillCfg(common_data->continue_kill_count);
					if (NULL != kill_cfg && kill_cfg->buff_time > 0 && kill_cfg->gongji_add_per > 0)
					{
						int gongji = (int)(killer_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (kill_cfg->gongji_add_per * ROLE_ATTR_PER));
						EffectBuff *effect = new EffectBuff(killer_role->GetId(), EffectBase::PRODUCT_ID_CONTINUE_KILL, kill_cfg->buff_time * 1000, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
						effect->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, gongji, kill_cfg->gongji_add_per);
						effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
						effect->SetClientEffectType(EffectBase::ECT_SKILL_CONTINUE_KILL);
						killer_role->AddEffect(effect);
						common_data->last_add_continue_kill_buff_timestamp = now_sec;
					}
				}
			}

			common_data->trigger_continue_kill_invalid_timestamp = now_sec + continue_kill_max_interval;

			if (now_sec > common_data->last_send_continue_kill_timestamp)
			{
				common_data->last_send_continue_kill_timestamp = now_sec + 5;
				killer_role->SendContinueKillInfo();
			}
		}
	}
}

void Character::AddEffect(EffectBase *effect, bool *is_die)
{
	if (!this->IsAlive() || this->IsWudi() || this->IsTerritoryWarTowerWudi() || m_effect_list.IsInvisible())
	{
		delete effect;
		return;
	}
	else if (EffectBase::EFFECT_TYPE_ONCE == effect->GetEffectType())
	{
		EffectOnce *effect_once = (EffectOnce *)effect;

		bool die = false;
		effect_once->Effect(this, &die);

		if (die) this->OnDie(effect_once->GetDeliverer());

		if (NULL != is_die) *is_die = die;

		delete effect;
		return;
	}
	else if (effect->IsChemoEffect() && this->IsMianYiChenMo())
	{
		delete effect;
		return;
	}
	else if (effect->IsDingshenEffect() && this->IsMianYiDingShen())
	{
		delete effect;
		return;
	}
	else if (effect->IsXuanyunEffect() && this->IsMianYiXuanYun())
	{
		delete effect;
		return;
	}
	else if (effect->IsChihuanEffect() && this->IsMianYiChiHuan())
	{
		delete effect;
		return;
	}
	else if (effect->IsXuanyunEffect())
	{
		if (Obj::OBJ_TYPE_ROLE == this->GetObjType())
		{
			Role *role = (Role *)this;
			if (role->GetRoleZhuanZhi()->GetRoleZhuanZhiSkill().CheckMainyiXuanyun())
			{
				delete effect;
				return;
			}
		}
	}
	else if (this->IsMianyiBuff())
	{
		delete effect;
		return;
	}

	if (OBJ_TYPE_ROLE == this->GetObjType())
	{
		Role *role = static_cast<Role *>(this);
		role->GetMojieSkill()->CheckDecControlBuffTime(effect);
	}

	m_effect_list.Add(effect);

	if (OBJ_TYPE_ROLE == this->GetObjType())
	{
		Role *role = static_cast<Role *>(this);
		role->GetMojieSkill()->CheckAddDecControlBuffTimeBuff(effect);
	}
}

bool Character::IsMianYiXuanYun()
{
	if (Obj::OBJ_TYPE_ROLE == m_obj_type)
	{
		Role *role = (Role *)this;
		if (role->GetShenshou()->IsMianYiXuanYun())
		{
			return true;
		}
	}

	return m_effect_list.HaveSpecialBuff(EBT_MIANYI_XUANYUN) || m_effect_list.HaveSpecialBuff(EBT_SUPER_MIANYI); 
}

bool Character::IsMianYiChenMo()
{
	if (Obj::OBJ_TYPE_ROLE == m_obj_type)
	{
		Role *role = (Role *)this;
		if (role->GetShenshou()->IsMianYiChenMo())
		{
			return true;
		}
	}

	return m_effect_list.HaveSpecialBuff(EBT_MIANYI_CHENMO) || m_effect_list.HaveSpecialBuff(EBT_SUPER_MIANYI);
}

void Character::ClearFightBuffEffect(int clear_reason, bool clear_skill_buff, bool clear_scene_buff)
{
	m_effect_list.ClearFightEffect(clear_reason, clear_skill_buff, clear_scene_buff);
}

void Character::ClearAllDeBuffEffect()
{
	m_effect_list.ClearAllDeBuffEffect();
}

void Character::SkillResetPos(const Posi &pos, int reset_pos_type, int skill_id, bool check_special_state , ObjID deliver_obj_id)
{
	if (reset_pos_type <= SKILL_RESET_POS_TYPE_INVALID || reset_pos_type >= SKILL_RESET_POS_TYPE_MAX) return;

	Posi deltaP = pos - this->GetPos();
	Scalar distance = 0;
	Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
	
	if (SKILL_RESET_POS_TYPE_CHONGFENG == reset_pos_type || SKILL_RESET_POS_TYPE_CAPTURE == reset_pos_type)
	{
		distance <= 4 ? distance = 0 : (distance -= 4);	// 冲锋到足够近 不要重叠
	}

	Posf diff_pos(distance * cos(dir), distance * sin(dir));
	Posf dest_pos = m_posf + diff_pos;

	Dir reset_dir = dir;
	if (SKILL_RESET_POS_TYPE_JITUI == reset_pos_type || SKILL_RESET_POS_TYPE_TOWER_DEFEND_ZHENFA == reset_pos_type)
	{
		reset_dir = this->GetDir();
	}
	
	if (MoveBySkill((Axis)dest_pos.x, (Axis)dest_pos.y, reset_dir, 0, check_special_state))
	{
		m_last_check_valid_time = 0;
	
		Protocol::SCSkillResetPos skill_rp;
		skill_rp.reset_pos_type = reset_pos_type;
		skill_rp.dir = reset_dir;
		skill_rp.obj_id = m_obj_id;
		skill_rp.pos_x = m_posi.x;
		skill_rp.pos_y = m_posi.y;
		skill_rp.skill_id = skill_id;
		skill_rp.deliver_obj_id = deliver_obj_id;
		skill_rp.reserve = 0;
		if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&skill_rp, sizeof(skill_rp));

		if (Obj::OBJ_TYPE_ROLE == m_obj_type)
		{
			Role *pThis = (Role*)this;
			m_scene->GetZoneMatrix()->MoveAOI(pThis->GetAoiHandler());
			pThis->ResetRoleStatus();
		}

		return;
	}
	
}

int Character::SkillGetProf()
{
	if (Obj::OBJ_TYPE_ROLE == m_obj_type || Obj::OBJ_TYPE_ROLE_SHADOW == m_obj_type)
	{
		Role *pThis = (Role*)this;
		return pThis->GetProf() > PROF_TYPE_PROF_ZHUANZHI_BASE ? (pThis->GetProf() % PROF_TYPE_PROF_ZHUANZHI_BASE) : pThis->GetProf();
	}

	return PROF_TYPE_INVALID;
}


void Character::SetCooldownEndTime(unsigned long end_time)
{
	m_cooldown_end_time = end_time;
}

bool Character::CheckGlobalCD()
{
	return m_cooldown_end_time < ServerLogic::GetInstServerLogic()->GetPiTime();
}

void Character::NotifyFightSpecialEffect(short skill_special_effect, ObjID deliver_obj_id, short float_type, Attribute float_value)
{
	static Protocol::SCFightSpecialFloat holy_injure;
	holy_injure.obj_id = this->GetId();
	holy_injure.float_type = float_type; // 类型见 SCFightSpecialFloat::FLOAT_VALUE_TYPE;
	holy_injure.float_value = float_value;
	holy_injure.skill_special_effect = skill_special_effect; // 类型见：SCFightSpecialFloat::SPECIAL_EFFECT_NON
	holy_injure.deliver_obj_id = deliver_obj_id;
	this->GetScene()->GetZoneMatrix()->NotifyAreaMsg(this->GetObserHandle(), &holy_injure, sizeof(Protocol::SCFightSpecialFloat));
}

bool Character::FlyHeightChange(short fly_height)
{
	if (NULL == m_scene) return false;

	if (!CanCharacterAction(true, true))
	{
		return false;
	}

	if (this->IsDingShen())
	{
		return false;
	}

	StopAttack();

	m_fly_height = fly_height;

	SendFlyHeightChangeInfo();

	return true;
}
