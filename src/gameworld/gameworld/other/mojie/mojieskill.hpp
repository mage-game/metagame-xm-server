#ifndef __MOJIE_SKILL_HPP__
#define __MOJIE_SKILL_HPP__

#include "mojieskillconfig.hpp"

class EffectBase;
class Character;
class Role;
class CharIntAttrs;

// 这个系统和类MojieManager没有任何关系
// 和MojieSkillImpl也没有任何关系
class MojieSkill
{
public:
	MojieSkill();
	~MojieSkill();

	void SetRole(Role *role) { m_role = role; }

	void CheckActiveSkill(int pata_layer);

	void Update(unsigned int now_sec, unsigned int interval);

	void OnHurtOther(Character *target, long long &injure, int fight_type, int target_index);

	void OnHurtBoss(Character *target, long long &injure);
	void OnAttackCalcHurt(ObjID attacker, long long &delta_hp, int fight_type);

	void CheckAddDecControlBuffTimeBuff(EffectBase *effect);
	void CheckDecControlBuffTime(EffectBase *effect);

	void InitSkillTypeMaxSkillId();
	void OnInitFinish();

	void GmResetActiveFlag(int skill_id);

	void AddExpBuff(int skill_type, int skill_id);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	int GetExpBuffPercent() { return m_add_exp_buff_percent; }

	//------------------------------------------------
	// 获取技能属性值
	//------------------------------------------------
	Attribute GetBaojiHurtPercent() { return m_baoji_hurt_per; }
	Attribute GetKangbaoPercent() { return m_kangbao_per; }
	Attribute GetKangbaoHurtPercent() { return m_kangbao_hurt_per; }
	Attribute GetHuiXinYiJi() { return m_huixinyiji; }
	Attribute GetHuiXinYiJiHurtPercent() { return m_huixinyiji_hurt_per; }
	Attribute GetMianshangPercent() { return m_mianshang_per; }
	Attribute GetPVEZengshangPercent() { return m_pve_zengshang_per; }
	Attribute GetPVPJianshangPercent() { return m_pvp_jianshang_per; }

private:
	bool CanPerfromSkill(unsigned int now_sec, int skill_type,int &skill_id);        // 传入技能类型，返回该类型已激活的属性最高的技能id，并返回能否释放
	int GetMaxActiveSkillIdByType(int skill_type);                                   // 该类型技能的最大属性技能id
	int CaclMaxSkillIdBySkillType(int skill_type);                                   // 重算一下该类型技能的最大属性技能id
	bool IsActiveSkill(int skill_id);

	Role *m_role;

	int m_skill_type_max_skill_id[SKILL_TYPE_MAX];
	unsigned int m_next_perfrom_skill_time[MojieSkillConfig::MOJIE_SKILL_COUNT];
	unsigned int m_next_update_time;
	int m_other_capability;
	int m_add_exp_buff_percent;				// 技能增加的经验获取万分比加成
	
	CharIntAttrs m_attrs_add;

	//------------------------------------------------
	// 获取技能属性值
	//------------------------------------------------
	Attribute m_baoji_hurt_per;
	Attribute m_kangbao_per;
	Attribute m_kangbao_hurt_per;
	
	Attribute m_huixinyiji;
	Attribute m_huixinyiji_hurt_per;
	
	Attribute m_mianshang_per;
	Attribute m_pve_zengshang_per;
	Attribute m_pvp_jianshang_per;

};

#endif // __MOJIE_SKILL_HPP__
