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

#define SKILL_ATTR_PER 0.0001f //������صļӳ���ֱ�

class SkillBase
{
public:
	enum SKILL_TYPE
	{
		SKILL_TYPE_INVALID = 0,

		SKILL_TYPE_ACTIVE ,					// ��������
		SKILL_TYPE_ACTIVE_CHONGFENG,		// �������� ����� ���ڶ����� 
	
		SKILL_TYPE_PASSIVE,					// ��������
		SKILL_TYPE_NUQI,					// ŭ������

		SKILL_TYPE_MAX,
	};

	enum FIGHT_STATUS_POLICY
	{
		FIGHT_STATUS_POLICY_NONE = 0,		// ������ս��״̬
		FIGHT_STATUS_POLICY_PERFORM = 1,	// �ͷŽ���ս��״̬
		FIGHT_STATUS_POLICY_INJURE = 2,		// ����˺�����ս��״̬
	};

	enum AOE_REASON
	{
		AOE_REASON_SKILL = 0,
		AOE_REASON_FAZHEN,
	};

	enum AOE_RANGE_TYPE
	{
		AOE_RANGE_TYPE_NONE = 0,					// ����
		AOE_RANGE_TYPE_SELF_CENTERED_QUADRATE,		// ���Σ����Լ�Ϊ����
		AOE_RANGE_TYPE_TARGET_CENTERED_QUADRATE,	// ���Σ���Ŀ��Ϊ����
		AOE_RANGE_TYPE_SELF_CENTERED_CIRCLE,		// Բ�Σ����Լ�Ϊ����
		AOE_RANGE_TYPE_TARGET_CENTERED_CIRCLE,		// Բ�Σ���Ŀ��Ϊ����
		AOE_RANGE_TYPE_SELF_BEGINNING_RECT,			// ���Σ����Լ�Ϊ���
		AOE_RANGE_TYPE_SELF_BEGINNING_SECTOR,		// ���Σ����Լ�Ϊ���
		AOE_RANGE_TYPE_MAX,
	};

	enum TIPS_WAY
	{
		TIPS_WAY_NONE = 0,					// ����ʾ
		TIPS_WAY_READING,					// ����
		TIPS_WAY_PERFORM,					// �ͷ�

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

	int GetAttackDis() const { return m_attack_dis + 2; } // �ݴ� �ȿͻ��˾����2
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
	const SkillType	m_skill_type;												// ��������
	UInt16			m_skill_id;													// ����ID
	int				m_max_level;												// �������ȼ�

	int				m_attack_dis;												// ��������
	int				m_skill_range_type;											// ��Χ����
	int				m_attack_range_list[MAX_SKILL_LEVEL + 1];					// AOE���ܹ�����Χ
	int				m_attack_range2_list[MAX_SKILL_LEVEL + 1];					// AOE���ܹ�����Χ
	int				m_attack_range_enemy_num_list[MAX_SKILL_LEVEL + 1];			// AOE���ܹ�����Χ ��ѡĿ������б�
	unsigned int	m_cooldown_time_list[MAX_SKILL_LEVEL + 1];					// cdʱ��

	bool			m_is_attack_character;										// �Ƿ��cha�ͷ�
	bool			m_attack_enemy;												// �Ƿ�Եз�
	int				m_cost_mp;													// ���ķ���ֵ
	int				m_prof_limit;												// ְҵ����
	int				m_learn_level_limit[MAX_SKILL_LEVEL + 1];					// ѧϰ�ȼ�����
	int				m_prob[MAX_SKILL_LEVEL + 1];								// �ͷŸ���

	int				m_is_be_effect_list[MAX_SKILL_LEVEL + 1];					// �Ƿ��ܼ�����������Ӱ��
	int				m_hurt_percent_list[MAX_SKILL_LEVEL + 1];					// �����˺���ֱ�
	int				m_fix_hurt_list[MAX_SKILL_LEVEL + 1];						// �����̶��˺�

	bool			m_is_repel;													// �Ƿ����Ŀ��

	int				m_param_a[MAX_SKILL_LEVEL + 1];								// ����A
	int				m_param_b[MAX_SKILL_LEVEL + 1];								// ����B
	int				m_param_c[MAX_SKILL_LEVEL + 1];								// ����C
	int				m_param_d[MAX_SKILL_LEVEL + 1];								// ����D
	int				m_param_e[MAX_SKILL_LEVEL + 1];								// ����E
	int				m_param_f[MAX_SKILL_LEVEL + 1];								// ����F
	int				m_param_g[MAX_SKILL_LEVEL + 1];								// ����G

	int				m_mojing_cost_list[MAX_SKILL_LEVEL + 1];					// ����������
	ItemID			m_item_cost_id_list[MAX_SKILL_LEVEL + 1];					// ѧϰ������Ʒid
	int				m_item_cost_list[MAX_SKILL_LEVEL + 1];						// ѧϰ������Ʒ
	int				m_coin_cost_list[MAX_SKILL_LEVEL + 1];						// ѧϰ����ͭ��
	int				m_capability_list[MAX_SKILL_LEVEL + 1];						// ս����
	int				m_skill_cap_per_list[MAX_SKILL_LEVEL + 1];					// ս������ֱ�
	int				m_calc_hurt_by_hp_per[MAX_SKILL_LEVEL + 1];					// �з�Ѫ�������˺���ֱ�
	int				m_calc_baodi_hurt_by_hp_per[MAX_SKILL_LEVEL + 1];			// �з����Ѫ�������˺���ֱ�
	int				m_effect_to_other_target[MAX_SKILL_LEVEL + 1];				// �Ƿ����Ŀ��������Ч
	
	int				m_reading_s;												// ����ʱ����
	int				m_continue_perform_s;										// ����ʩ��ʱ����
	int				m_trigger_qte;												// �ɷ񴥷�QTE����

	int				m_valid_in_save_area;										// ��ȫ���Ƿ�ɷ�
	int				m_fight_status_policy;										// ����ս��״̬����

	int				m_tips_way;													// ������ʾ��ʽ
	int				m_check_shanbi;												// �Ƿ�������

	void SendAttackMsg(Character *cha, Character *target, bool is_force_broadcast = true) const;
	void SendAOEAttackMsg(Character *chara, Character *target, const Posi &pos, short aoe_reason = AOE_REASON_SKILL) const;
	void SendSkillPhase(Character *cha, UInt16 skill_id, int skill_phase) const;

	bool IsBaoji(Character *chara, Character *target, int &fight_type) const;						// �Ƿ񱩻�
	bool IsShanbi(Character *chara, Character *target, int target_add_shanbi_percent = 0) const;	// �Ƿ�����
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
	bool GetIsPojia(Character *cha) const;												// �Ƿ��Ƽ�һ��
	bool GetIsHuixinyiji(Character *cha, bool is_baoji, int &fight_type) const;			// �Ƿ����һ��
	bool GetIsGedang(Character *cha, Character *target, int &fight_type) const;			// �Ƿ��
	bool GetIsZhufuyiji(Character *cha) const;											// �Ƿ�ף��һ��

	double GetHurtIncPercent(Character *cha, Character *target, const double add_hurt_percent, const double pofang_per) const;
	double GetHurtDecPercent(Character *cha, Character *target, const double mianshang_per) const;

	// �˺��ӳ�-----------------------------------------------------------------------------
	double GetObjTypeAddHurtPer(Character *cha, Character *target) const;				
	double GetRMAddHurtPer(Character *cha, Character *target) const;					// ��Ҵ��
	double GetMRAddHurtPer(Character *cha, Character *target) const;					// �ִ����
	double GetRRAddHurtPer(Character *cha, Character *target) const;					// ��Ҵ����

	long long GetBaojiExtraHurt(Character *cha, Character *target) const;				// ��������ӳ��˺�

	// �˺�����----------------------------------------------------------------------------
	double GetObjTypeDecHurtPer(Character *cha, Character *target) const;
	double GetMRDecHurtPer(Character *target) const;									// �ִ����
	double GetRMDecHurtPer(Character *cha, Character *target) const;					// ��Ҵ��
	double GetRRDecHurtPer(Character *cha, Character *target) const;					// ��Ҵ����

	long long GetOtherSysInjure(Character *cha, Character *target, long long base_injure) const; // ����ϵͳ�˺�

	// -------------------------------------------------------------------------------------
	void CalcFixInjure(Character *cha, Character *target, long long &injure) const;
	void CalcFixHuretInjure(Character *cha, Character *target, long long &injure) const;

	// -------------------------------------------------------------------------------------
	double GetAddBaojiHurtPer(Character *cha) const;										// �����˺���ֱ�����
	double GetAddKangbaoPer(Character *cha) const;											// ����������
	double GetAddKangbaoHurtPer(Character *cha) const;										// �����ֿ��˺�������
	
	double GetAddHuixinyijiHurtPer(Character *cha) const;									// ����һ���˺�������
	Attribute GetAddHuixinyiji(Character *cha) const;										// ����һ��������

};

#endif