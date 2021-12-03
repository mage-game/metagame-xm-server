#ifndef __TALENT_HPP__
#define __TALENT_HPP__

#include "obj/character/attribute.hpp"
#include "obj/character/character.h"
#include "servercommon/talentdef.hpp"
#include "protocal/msgother.h"

class Role;
class TalentLevelConifg;

struct TalentVirSkillInfo
{
	TalentVirSkillInfo() { this->Reset(); }

	void Reset()
	{
		skill_stack_count = 0;
		skill_effect_time_ms = 0;
		last_perform_timestamp = 0;
	}

	int skill_stack_count;					// ����
	int skill_effect_time_ms;				// ���ܳ���ʱ��
	unsigned int last_perform_timestamp;	// ��һ���ͷ�ʱ��
};

class Talent
{
public:
	Talent();
	~Talent();

	enum TALENT_VIR_SKILL_INDEX
	{
		TALENT_VIR_SKILL_INDEX_ID_107 = 0,
		TALENT_VIR_SKILL_INDEX_ID_108,
		TALENT_VIR_SKILL_INDEX_ID_109,
		TALENT_VIR_SKILL_INDEX_ID_110,
		TALENT_VIR_SKILL_INDEX_ID_208,
		TALENT_VIR_SKILL_INDEX_ID_209,
		TALENT_VIR_SKILL_INDEX_ID_210,
		TALENT_VIR_SKILL_INDEX_ID_414,
		TALENT_VIR_SKILL_INDEX_ID_306,
		TALENT_VIR_SKILL_INDEX_ID_412,
		TALENT_VIR_SKILL_INDEX_ID_311,
		TALENT_VIR_SKILL_INDEX_ID_312,
		TALENT_VIR_SKILL_INDEX_ID_417,
		TALENT_VIR_SKILL_INDEX_ID_310,

		TALENT_VIR_SKILL_TYPE_MAX,
	};

	void SetRole(Role *role) { m_role = role; }

	void InitParam(Role *role, const TalentParam &param);
	void GetInitParam(TalentParam *param) const;

	static bool GetAttrTypeFromAttrString(std::string &attr_str, short *attr_type);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	/*
	����ģ������Լӳɵ��ñ�������ȡ ���ݴ����base_attr ���ӳ�%�޸� target_attr
	*/
	void OtherModuleReCalcAttr(int typ,const CharIntAttrs &base_attr, CharIntAttrs &target_attr);

	void OnUpLevel(int old_level, int new_level);											// ��ɫ���������츳����
	void OnZhuanzhi();																		// ��ɫתְ�����츳����
	bool OnUseTalenBook(int talent_point);													// ʹ�ü����������츳����

	void Update(unsigned long interval, unsigned int now_second);
	void OnHurtOther(Character *target, int skill_id, long long injure, int fight_type);
	void OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);
	void OnKillOtherRole(Role * target_role);

	//long long GetTalentExtraValue(int talent_attr_type, long long base_value);
	long long GetTalentFixInjure(Character *target, long long injure, const int skill_id);
	long long DecHurtOnRoleBeenAttack(long long injure);

	int GetTalentLevel(int talent_id);
	void OnTalentOperate(Protocol::CSRoleTelentOperate *rto);

	const TalentVirSkillInfo *GetTalentVirSkillInfo(int vir_skill_index);

	void GMAddTianFuUpLevel(int talent_id);

	const long long GetTalentAttrValue(int talent_attr_type);

	long long ReCalExtraCap();										// �������̶�ս��
	long long ReCalSkillCapPer();									// ���㼼��ս����ֱ�
private:	
	void OnTalentUpLevel(int talent_id);
	void OnResetTalentPoint(bool is_auto_buy);
	void SendTalentInfo();

	CharIntAttrs::RECALC_REASON GetOtherModuleReason(TALENT_ATTR_TYPE typ);

	void ReCalcOtherModule(const std::set<CharIntAttrs::RECALC_REASON>& other_md);

	//
	const TalentLevelConifg* GetTalentLevelCfg(int talent_id);

	//���������츳
	void OnHurtOtherTalent107(Character * target, int skill_id, long long injure, int fight_type);
	void OnHurtOtherTalent108(Character * target, int skill_id, long long injure, int fight_type);
	void OnHurtOtherTalent110(Character * target, int skill_id, long long injure, int fight_type);
	void OnHurtOtherTalent414(Character * target, int skill_id, long long injure, int fight_type);
	void OnHurtOtherTalent412(Character * target, int skill_id, long long injure, int fight_type);
	void OnHurtOtherTalent115(Character * target, int skill_id, long long injure, int fight_type);
	void OnHurtOtherTalent310(Character * target, int skill_id, long long injure, int fight_type);
	void OnHurtOtherTalent306();

	//�ܻ������츳
	void OnRoleBeenAttackTalent206(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);
	void OnRoleBeenAttackTalent207(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);
	void OnRoleBeenAttackTalent208(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);
	void OnRoleBeenAttackTalent209(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);
	void OnRoleBeenAttackTalent210(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);
	void OnRoleBeenAttackTalent311(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);
	void OnRoleBeenAttackTalent312(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);
	void OnRoleBeenAttackTalent417(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);

	//�Զ������츳
	bool OnPerformSkill(Character *target, long long injure, int max_target_num);

	Role *m_role;
	CharIntAttrs m_attrs_add;
	TalentParam m_param;

	TalentVirSkillInfo m_vir_skill_info[TALENT_VIR_SKILL_TYPE_MAX];
};

#endif