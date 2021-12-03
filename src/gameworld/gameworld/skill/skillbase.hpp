#ifndef __SKILLBASE_HPP__
#define __SKILLBASE_HPP__

#include <string>

#include "gamedef.h"
#include "servercommon/servercommon.h"

#include "skilldef.hpp"

#include "obj/character/attribute.hpp"

class Character;
class Scene;
class TiXmlElement;

#define SKILL_ATTR_PER 0.0001f //技能相关的加成万分比

class SkillBase
{
public:
	enum SKILL_TYPE
	{
		SKILL_TYPE_INVALID = 0,

		SKILL_TYPE_ACTIVE ,					// 主动技能
		SKILL_TYPE_ACTIVE_CHONGFENG,		// 主动技能 冲锋类 用于定身检查 
	
		SKILL_TYPE_PASSIVE,					// 被动技能
		SKILL_TYPE_NUQI,					// 怒气技能

		SKILL_TYPE_MAX,
	};

	enum FIGHT_STATUS_POLICY
	{
		FIGHT_STATUS_POLICY_NONE = 0,		// 不进入战斗状态
		FIGHT_STATUS_POLICY_PERFORM = 1,	// 释放进入战斗状态
		FIGHT_STATUS_POLICY_INJURE = 2,		// 造成伤害进入战斗状态
	};

	enum AOE_REASON
	{
		AOE_REASON_SKILL = 0,
		AOE_REASON_FAZHEN,
	};

	enum AOE_RANGE_TYPE
	{
		AOE_RANGE_TYPE_NONE = 0,					// 单体
		AOE_RANGE_TYPE_SELF_CENTERED_QUADRATE,		// 方形，以自己为中心
		AOE_RANGE_TYPE_TARGET_CENTERED_QUADRATE,	// 方形，以目标为中心
		AOE_RANGE_TYPE_SELF_CENTERED_CIRCLE,		// 圆形，以自己为中心
		AOE_RANGE_TYPE_TARGET_CENTERED_CIRCLE,		// 圆形，以目标为中心
		AOE_RANGE_TYPE_SELF_BEGINNING_RECT,			// 矩形，以自己为起点
		AOE_RANGE_TYPE_SELF_BEGINNING_SECTOR,		// 扇形，以自己为起点
		AOE_RANGE_TYPE_MAX,
	};

	enum TIPS_WAY
	{
		TIPS_WAY_NONE = 0,					// 不提示
		TIPS_WAY_READING,					// 读条
		TIPS_WAY_PERFORM,					// 释放

		TIPS_WAY_MAX,
	};

	enum SKILL_DUAN_STEP
	{
		SKILL_DUAN_STEP_1 = 1,
		SKILL_DUAN_STEP_2 = 2,
		SKILL_DUAN_STEP_3 = 3,
	};

	SkillBase(SkillType skill_type, bool is_attack_character, bool is_target_enemy);
	virtual ~SkillBase();

	SkillType GetSkillType() const { return m_skill_type; }

	UInt16 GetSkillId() const { return m_skill_id; }
	void SetSkillID(UInt16 skill_id) { m_skill_id = skill_id; }
	int GetMaxLevel() const { return m_max_level; }

	int GetAttackDis() const { return m_attack_dis + 2; } // 容错 比客户端距离大2
	int GetCostMp() const { return m_cost_mp; }
	int GetProfLimit() const { return m_prof_limit; }

	inline int GetLearnLevelLimit(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_learn_level_limit[skill_level];
	}
	
	inline int GetProb(int skill_level) const 
	{ 
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_prob[skill_level];
	}

	inline time_t GetCoolDown(int skill_level) const 
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_cooldown_time_list[skill_level];
	}

	inline bool GetIsBeEffectSkillPercent(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_is_be_effect_list[skill_level] == 1;
	}

	inline int GetHurtPercent(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_hurt_percent_list[skill_level];
	}

	inline int GetFixHurt(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_fix_hurt_list[skill_level];
	}

	inline int GetAttackRange(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_attack_range_list[skill_level];
	}

	inline int GetAttackRange2(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_attack_range2_list[skill_level];
	}

	inline int GetAttackRangeEnemyNum(int skill_level) const 
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_attack_range_enemy_num_list[skill_level];
	}

	inline int GetParamA(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_param_a[skill_level];
	}

	inline int GetParamB(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_param_b[skill_level];
	}

	inline int GetParamC(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_param_c[skill_level];
	}

	inline int GetParamD(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_param_d[skill_level];
	}

	inline int GetParamE(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_param_e[skill_level];
	}

	inline int GetParamF(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_param_f[skill_level];
	}

	inline int GetParamG(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_param_g[skill_level];
	}

	inline int GetCapablity(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_capability_list[skill_level];
	}

	inline int GetSkillCapPer(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_skill_cap_per_list[skill_level];
	}

	inline int GetLearnMojing(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_mojing_cost_list[skill_level];
	}

	inline int GetLearnCoin(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_coin_cost_list[skill_level];
	}

	inline ItemID GetLearnItemCostId(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_item_cost_id_list[skill_level];
	}

	inline int GetLearnItemCost(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_item_cost_list[skill_level];
	}

	inline int GetHurtByHpPer(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_calc_hurt_by_hp_per[skill_level];
	}

	inline int GetBaodiHurtByHpPer(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return 0;
		return m_calc_baodi_hurt_by_hp_per[skill_level];
	}

	inline bool IsEffectToOtherTarget(int skill_level) const
	{
		if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return false;
		return m_effect_to_other_target[skill_level] > 0;
	}

	inline int GetReadingS() const { return m_reading_s; }
	inline int GetContinuePerformS() const { return m_continue_perform_s; }

	virtual bool ValidInSaveArea() const { return 0 != m_valid_in_save_area; }
	virtual int GetFightStatusPolicy() const { return m_fight_status_policy; }
	virtual int GetTipsWay() const { return m_tips_way; }

	virtual bool IsAttackCharacter() const { return m_is_attack_character; }
	virtual bool IsTargetEnemy() const { return m_attack_enemy; }

	virtual bool CanTriggerQTE() const { return 0 != m_trigger_qte; }
	virtual bool IsJumpSkill() const { return false; }
	virtual bool StopWhenPerform() const { return true; }

	virtual void Perform(Character *chara, int skill_level, Character *target, const Posi &pos, int special_param = 0) const = 0;
	virtual void ReCalcAttr(Character *chara, int skill_level, CharIntAttrs &base_add) const {};
	
	virtual float GetIncHurtPercent(Character *chara, int skill_level) const { return 0; }
	virtual float GetDecHurtPercent(Character *chara, int skill_level) const { return 0; }

	virtual bool LoadConfig(const char *dir, std::string *err) = 0;
	virtual bool LoadFromDataNode(TiXmlElement *dataElement, std::string *err);

	virtual const char * GetSkillName() = 0;
	virtual bool CheckTarget(Character *chara, Character *target) const;

	static int GetSceneObjByArea(Scene *scene, const Posi &point, Axis range, ObjID *target_list, int max_num);

	static int GetTargetList(Scene *scene, Character *chara, const Posi &point, Axis range, Axis range2, ObjID *target_list, int max_num, int range_type, int effect_target_type);
	static int GetTargetByArea(Character *chara, Character *target, Axis range, ObjID *target_list, int max_num, bool check_enemy = true);
	static int GetTargetByArea(Character *chara, const Posi &point, Axis range, ObjID *target_list, int max_num, bool check_enemy = true);
	static int GetFriendByArea(Character *chara, const Posi &point, Axis range, ObjID *friend_list, int max_num, bool check_alive = true);

	static void SendAOEAttackMsgHelper(Character *chara, ObjID target_id, UInt16 skill_id, const Posi &pos, short aoe_reason = AOE_REASON_SKILL);
	static void SendAOEAttackMsgHelper(Scene *scene, ObjID deliver_id, ObjID target_id, UInt32 obser_handle, UInt16 skill_id, const Posi &pos, short aoe_reason = AOE_REASON_SKILL);

	void SendSkillTip(Character *cha) const;

protected:
	const SkillType	m_skill_type;												// 技能类型
	UInt16			m_skill_id;													// 技能ID
	int				m_max_level;												// 技能最大等级

	int				m_attack_dis;												// 攻击距离
	int				m_skill_range_type;											// 范围类型
	int				m_attack_range_list[MAX_SKILL_LEVEL + 1];					// AOE技能攻击范围
	int				m_attack_range2_list[MAX_SKILL_LEVEL + 1];					// AOE技能攻击范围
	int				m_attack_range_enemy_num_list[MAX_SKILL_LEVEL + 1];			// AOE技能攻击范围 挑选目标个数列表
	unsigned int	m_cooldown_time_list[MAX_SKILL_LEVEL + 1];					// cd时间

	bool			m_is_attack_character;										// 是否对cha释放
	bool			m_attack_enemy;												// 是否对敌方
	int				m_cost_mp;													// 消耗法力值
	int				m_prof_limit;												// 职业限制
	int				m_learn_level_limit[MAX_SKILL_LEVEL + 1];					// 学习等级限制
	int				m_prob[MAX_SKILL_LEVEL + 1];								// 释放概率

	int				m_is_be_effect_list[MAX_SKILL_LEVEL + 1];					// 是否受技能增、减伤影响
	int				m_hurt_percent_list[MAX_SKILL_LEVEL + 1];					// 基本伤害万分比
	int				m_fix_hurt_list[MAX_SKILL_LEVEL + 1];						// 基本固定伤害

	bool			m_is_repel;													// 是否击退目标

	int				m_param_a[MAX_SKILL_LEVEL + 1];								// 参数A
	int				m_param_b[MAX_SKILL_LEVEL + 1];								// 参数B
	int				m_param_c[MAX_SKILL_LEVEL + 1];								// 参数C
	int				m_param_d[MAX_SKILL_LEVEL + 1];								// 参数D
	int				m_param_e[MAX_SKILL_LEVEL + 1];								// 参数E
	int				m_param_f[MAX_SKILL_LEVEL + 1];								// 参数F
	int				m_param_g[MAX_SKILL_LEVEL + 1];								// 参数G

	int				m_mojing_cost_list[MAX_SKILL_LEVEL + 1];					// 升级熟练度
	ItemID			m_item_cost_id_list[MAX_SKILL_LEVEL + 1];					// 学习消耗物品id
	int				m_item_cost_list[MAX_SKILL_LEVEL + 1];						// 学习消耗物品
	int				m_coin_cost_list[MAX_SKILL_LEVEL + 1];						// 学习消耗铜币
	int				m_capability_list[MAX_SKILL_LEVEL + 1];						// 战斗力
	int				m_skill_cap_per_list[MAX_SKILL_LEVEL + 1];					// 战斗力万分比
	int				m_calc_hurt_by_hp_per[MAX_SKILL_LEVEL + 1];					// 敌方血量上限伤害万分比
	int				m_calc_baodi_hurt_by_hp_per[MAX_SKILL_LEVEL + 1];			// 敌方最大血量保底伤害万分比
	int				m_effect_to_other_target[MAX_SKILL_LEVEL + 1];				// 是否对主目标以外生效
	
	int				m_reading_s;												// 读条时间秒
	int				m_continue_perform_s;										// 持续施法时间秒
	int				m_trigger_qte;												// 可否触发QTE技能

	int				m_valid_in_save_area;										// 安全区是否可放
	int				m_fight_status_policy;										// 进入战斗状态策略

	int				m_tips_way;													// 技能提示方式
	int				m_check_shanbi;												// 是否检查闪避

	void SendAttackMsg(Character *cha, Character *target, bool is_force_broadcast = true) const;
	void SendAOEAttackMsg(Character *chara, Character *target, const Posi &pos, short aoe_reason = AOE_REASON_SKILL) const;
	void SendSkillPhase(Character *cha, UInt16 skill_id, int skill_phase) const;

	bool IsBaoji(Character *chara, Character *target, int &fight_type) const;						// 是否暴击
	bool IsShanbi(Character *chara, Character *target, int target_add_shanbi_percent = 0) const;	// 是否闪避
	bool CheckShanbi() const;

	long long CalcInjureAndType(Character *cha, Character *target, int level, int &fight_type, bool is_be_effect_skill_per = false, double add_hurt_percent = 0.0) const;
	int GetTotalHurtPercent(Character *cha, int level) const;

	virtual int HurtTrigger(Character *cha, Character *target, long long &injure, int fight_type, int target_index = 0) const;
	virtual int GetAOESkillTarget(Character *chara, int level, Character *target, const Posi &pos, ObjID *target_list, int max_num) const;
	virtual long long PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, double add_hurt_percent) const;

	void PerformSingleSkill(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	void PerformAOESkill(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;

	static int GetTargetByQuadrate(Scene *scene, const Posi &point, Axis range, ObjID *target_list, int max_num);
	static int GetTargetByRect(Scene *scene, const Posi &pos_self, const Posi &pos_target, Axis width, Axis height, ObjID *target_list, int max_num);
	static int GetTargetBySector(Scene *scene, const Posi &pos_self, const Posi &pos_target, Axis radius, int angle, ObjID *target_list, int max_num);
	static int GetTargetByCircle(Scene *scene, const Posi &point, Axis radius, ObjID *target_list, int max_num);

private:
	double GetGongji(Character *cha, Character *target) const;
	bool GetIsPojia(Character *cha) const;												// 是否破甲一击
	bool GetIsHuixinyiji(Character *cha, bool is_baoji, int &fight_type) const;			// 是否会心一击
	bool GetIsGedang(Character *cha, Character *target, int &fight_type) const;			// 是否格挡
	bool GetIsZhufuyiji(Character *cha) const;											// 是否祝福一击

	double GetHurtIncPercent(Character *cha, Character *target, const double add_hurt_percent, const double pofang_per) const;
	double GetHurtDecPercent(Character *cha, Character *target, const double mianshang_per) const;

	// 伤害加成-----------------------------------------------------------------------------
	double GetObjTypeAddHurtPer(Character *cha, Character *target) const;				
	double GetRMAddHurtPer(Character *cha, Character *target) const;					// 玩家打怪
	double GetMRAddHurtPer(Character *cha, Character *target) const;					// 怪打玩家
	double GetRRAddHurtPer(Character *cha, Character *target) const;					// 玩家打玩家

	long long GetBaojiExtraHurt(Character *cha, Character *target) const;				// 暴击额外加成伤害

	// 伤害减免----------------------------------------------------------------------------
	double GetObjTypeDecHurtPer(Character *cha, Character *target) const;
	double GetMRDecHurtPer(Character *target) const;									// 怪打玩家
	double GetRMDecHurtPer(Character *cha, Character *target) const;					// 玩家打怪
	double GetRRDecHurtPer(Character *cha, Character *target) const;					// 玩家打玩家

	long long GetOtherSysInjure(Character *cha, Character *target, long long base_injure) const; // 其他系统伤害

	// -------------------------------------------------------------------------------------
	void CalcFixInjure(Character *cha, Character *target, long long &injure) const;
	void CalcFixHuretInjure(Character *cha, Character *target, long long &injure) const;

	// -------------------------------------------------------------------------------------
	double GetAddBaojiHurtPer(Character *cha) const;										// 暴击伤害万分比增加
	double GetAddKangbaoPer(Character *cha) const;											// 抗暴率增加
	double GetAddKangbaoHurtPer(Character *cha) const;										// 暴击抵抗伤害率增加
	
	double GetAddHuixinyijiHurtPer(Character *cha) const;									// 会心一击伤害率增加
	Attribute GetAddHuixinyiji(Character *cha) const;										// 会心一击率增加

};

#endif