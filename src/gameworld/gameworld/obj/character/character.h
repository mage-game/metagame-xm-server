#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "gamedef.h"
#include "obj/obj.h"

#include "servercommon/struct/skilllistparam.h"

#include "skill/skillmanager.hpp"
#include "effect/buffmanager.hpp"
#include "effect/effectlist.hpp"

#include "attribute.hpp"

class Character : public Obj
{
public:
	enum _ActionStatus
	{
		ACTION_IDLE,
		ACTION_RUN,
		ACTION_ATTACK,
		ACTION_COUNT,
	};

	enum LogicResult
	{
		LOGIC_NONE,
		MOVE_STATIC,
		MOVE_RUN,
		ATTACK_NONE,
		ATTACK_DONE,
		ATTACK_OUTOFRANG,
		ATTACK_COOLDOWN,
		ATTACK_SPECIAL_STATE,
		ACTION_PLAYING,
	};

	enum SKILL_RESET_POS_TYPE
	{
		SKILL_RESET_POS_TYPE_INVALID = 0,
		SKILL_RESET_POS_TYPE_CHONGFENG,				// 冲锋
		SKILL_RESET_POS_TYPE_JUMP,					// 跳跃
		SKILL_RESET_POS_TYPE_FOUNTAIN,				// 喷泉
		SKILL_RESET_POS_TYPE_CAPTURE,				// 捕抓
		SKILL_RESET_POS_TYPE_JITUI,					// 击退
		SKILL_RESET_POS_TYPE_TOWER_DEFEND_ZHENFA,	// 塔防阵法
		SKILL_RESET_POS_TYPE_SHUNYI,				// 瞬移
		SKILL_RESET_POS_TYPE_MAX
	};

	Character(ObjType type);
	Character(ObjType type, const GameName _name);
	virtual ~Character();

	void Init(const CharIntAttrs &int_attr, const SkillListParam &skill, const SkillOtherParam &skill_other = INVALID_SKILL_OTHER_PARAM, int effect_buff_len = 0, const char *effect_buff = NULL);	 
	
	int GetActionStatus() { return m_action_status; }
	int GetLogicResult() { return m_logic_result; }

	virtual bool CanCharacterAction(bool is_notify_client = false, bool is_move = false);
	virtual bool Move(Axis _x, Axis _y, Dir _dir, Scalar distance, short height, bool check_valid=false, short press_onward = 0);
	virtual bool MoveBySkill(Axis _x, Axis _y, Dir _dir, Scalar distance, bool check_special_state = true);
	virtual bool SetDir(Dir _dir, Scalar distance);
	void ActionStop();
	void MoveStop();
	bool IsStatic() { return (m_dir == Dir_S) || (m_dir_distance < 0.0001); }

	virtual void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	virtual int GetLevel() { return 0; }
	virtual void NoticeNum(int notice_num) {}
	virtual bool StartAttack(ObjID obj_id);
	virtual void StopAttack();
	virtual bool PerformSkill(int skill_index, const Posi &pos, ObjID target_id, int check_flag = -1, int special_param = 0, int skill_data = 0);

	virtual bool IsFriend(Obj *obj, bool check_alive = true) { return false; }
	virtual bool IsEnemy(Obj *obj) { return false; }
	virtual bool IsAlive();	
	
	virtual void OnAttackCalcHurt(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill, int fight_type);
	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill);
	virtual int OnHurtOther(Character *target, int skill_id, long long &injure, int fight_type, int target_index);

	int CalcUpGradeSkillEffect(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, Attribute &holy_delta_hp, bool from_skill, bool is_fix_hurt);
	int GetAttackerGradeSkillInjurePer(Role *attacker_role, int skill_type);
	void SendGradeSkillInfo(Role *role);

	virtual void OnDie(ObjID killer);

	SkillManager * GetSkillManager() { return &m_skill_manager; }
	EffectList * GetEffectList() { return &m_effect_list; }

	void AddEffect(EffectBase *effect, bool *is_die = NULL);

	bool IsXuanYun() 
	{ 
		return m_effect_list.HaveSpecialBuff(EBT_XUANYUN) || m_effect_list.HaveSpecialBuff(EBT_FROZEN) ; 
	}

	bool IsDingShen() { return m_effect_list.HaveSpecialBuff(EBT_DINGSHEN); }
	bool IsChenMo() { return m_effect_list.HaveSpecialBuff(EBT_CHENMO); }
	bool IsChiHuan() { return m_effect_list.HaveSpecialBuff(EBT_CHIHUAN); }
	bool IsWudi() { return m_effect_list.HaveSpecialBuff(EBT_WUDI); }
	bool IsTerritoryWarTowerWudi() { return m_effect_list.HaveSpecialBuff(EBT_TERRITORYWAR_TOWER_WUDI); }
	bool IsFool() { return m_effect_list.HaveSpecialBuff(EBT_BIANXING_FOOL); }

	virtual bool IsMianyiDeadlyAttack() { return false; }
	bool IsMianYiXuanYun();
	bool IsMianYiDingShen() { return m_effect_list.HaveSpecialBuff(EBT_MIANYI_DINGSHEN) || m_effect_list.HaveSpecialBuff(EBT_SUPER_MIANYI); }
	bool IsMianYiChenMo();
	bool IsMianYiChiHuan() { return m_effect_list.HaveSpecialBuff(EBT_MIANYI_CHIHUAN) || m_effect_list.HaveSpecialBuff(EBT_SUPER_MIANYI); }
	bool IsMianyiCapture() { return m_effect_list.HaveSpecialBuff(EBT_SUPER_MIANYI); }
	bool IsMianyiBuff() { return m_is_mianyi_buff || m_effect_list.IsInWudiProtect(); }
	void SetMianyiBuff(bool is_mianyi_buff) { m_is_mianyi_buff = is_mianyi_buff; }
	bool IsInForbidRcover() { return m_effect_list.IsForbidRecover(); }

	virtual bool CanBeCaptureBySkill() { return true; }
	virtual bool IsFixHurtOnAttack() { return false; }
	virtual int GetFixHurtOnAttack() { return 1000; }

	bool IsFullHp()	{ return this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP) >= this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP); } // 满血
	bool IsFullMp() { return this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP) >= this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP); } // 满蓝

	void ClearFightBuffEffect(int clear_reason, bool clear_skill_buff = true, bool clear_scene_buff = true);
	void ClearAllDeBuffEffect();

	bool CheckNextSkillDistance(Character *target, Posi *pos){ return m_skill_manager.CheckNextSkillDistance(target, pos); }
	Attribute GetCharaIntAttr(int index) { return m_int_attrs.GetAttr(index); }
	void SetCharaIntAttr(int index, Attribute value) { return m_int_attrs.SetAttr(index, value); }
	void GetCharaIntAttrPercents(CharIntAttrs &attrs);

	void ChangeCharAttrHelper(int attr_type, Attribute attr_value, unsigned char attr_notify_reason = CharIntAttrs::ATTR_NOTIFY_REASON_NORMAL);
	void ChangeCharAttrHelper(CharIntAttrs::AttrPair *attr_list, int count, unsigned char attr_notify_reason = CharIntAttrs::ATTR_NOTIFY_REASON_NORMAL);
	
	void ChangeHp(Attribute *value, bool notfiy = false, unsigned char attr_notify_reason = CharIntAttrs::ATTR_NOTIFY_REASON_NORMAL, bool is_ignore_forbidrecover = false);
	void ChangeMp(Attribute *value, bool notfiy = false, unsigned char attr_notify_reason = CharIntAttrs::ATTR_NOTIFY_REASON_NORMAL);

	void Recover(bool recover_hp = true, bool recover_mp = true, bool notify = true, unsigned char attr_notify_reason = CharIntAttrs::ATTR_NOTIFY_REASON_NORMAL);

	void SkillResetPos(const Posi &pos, int reset_pos_type, int skill_id = 0, bool check_special_state = true, ObjID deliver_obj_id = INVALID_OBJ_ID);
	int SkillGetProf();

	void SetCooldownEndTime(unsigned long end_time);
	bool CheckGlobalCD();

	virtual void OnQTEEffectDone() {}
	virtual void CheckQTEEffect(Character *target, bool is_gongji = true) {}

	virtual unsigned short GetAttackSeq() { return m_attack_seq; }
	virtual void IncAttackSeq() { ++m_attack_seq; }

	virtual int GetHurtIncPercent(Character *target) { return 0; }
	virtual int GetHurtDecPercent(Character *performer) { return 0; }

	void NotifyFightSpecialEffect(short skill_special_effect, ObjID deliver_obj_id = INVALID_OBJ_ID, short float_type = 0, Attribute float_value = 0);

	virtual bool FlyHeightChange(short fly_height);			// 飞行高度改变

protected:
	void ChangeHpMpHelper(Attribute *which_cur, Attribute *which_max, Attribute *value, int attr_type, bool notity = false, unsigned char attr_notify_reason = CharIntAttrs::ATTR_NOTIFY_REASON_NORMAL);

	void SendPosiInfo(short press_onward = 0);

	virtual bool CheckMapValidate() { return true; }
	virtual bool CheckMoveFrequency() { return true; }
	virtual float GetMoveRate() { return 1.0f; }

	int MoveLogic(unsigned long interval);
	//int AttackLogic(Character *target, unsigned long interval, bool check_global_cd = true);

	void SendFlyHeightChangeInfo();				// 发送给下高度改变信息
	
	unsigned int m_last_check_valid_time;					// 上次检测移动非法的时间
	Posi m_last_check_pos;									// 上次检测移动非法的坐标

	int m_action_status;									// 动作状态
	int	m_logic_result;										// 逻辑结果

	unsigned long m_cooldown_end_time;						// 全局cooldown

	int m_dead_time;										// 死亡时间

	SkillManager m_skill_manager;							// 技能管理器
	EffectList m_effect_list;								// 效果列表
	CharIntAttrs m_int_attrs;								// 属性列表

	float m_old_dir;										// 避免频繁调用cos sin 记录最后一次在方向
	float m_old_cos;										// 上个方向在cos值
	float m_old_sin;										// 上个方向在sin值

	unsigned short m_attack_seq;							// 攻击序号
	bool m_is_mianyi_buff;
};

#endif // __CHARACTER_H__
