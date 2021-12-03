#ifndef __JINJIE_TALENT_HPP__
#define __JINJIE_TALENT_HPP__

#include <map>
#include <vector>

#include "servercommon/jinjietalentdef.hpp"
#include "servercommon/configcommon.h"
#include "obj/character/attribute.hpp"

class Role;
class Character;
/*
 天赋
*/
class JinJieTalent
{
private:
	struct TelentAttrConfig : AttrConfig
	{
		int constant_zengshang = 0;
		int constant_mianshang = 0;
	};
	struct TelentExtendAttr
	{
		TelentExtendAttr(int skill_type, Attribute value, int per)
			: skill_type(skill_type)
			, value(value)
			, per(per) 
		{
		}
		int skill_type;
		Attribute value;
		int per;
	};

	struct SkillParam
	{
		SkillParam(int last_time, int star) 
			: last_time(last_time)
			, star(star)
		{
		}
		SkillParam()
		{
		}
		int last_time;
		int star;
	};

	typedef std::map<int, std::vector<TelentExtendAttr> > TelentExtendAttrMp;
public:
	JinJieTalent();
	~JinJieTalent();


	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const JinJieTalentParam &talent_param);
	void GetInitParam(JinJieTalentParam *talent_param);

	void OnLogin();
	void OndayChange(int olddayid, int nowdayid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	//受击 PVP
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill);
	//攻击别人 PVP
	void OnHurtOther(Character *target, long long injure);
	//检测并开启天赋格子
	void OnCheckOpenTalentGrid(int tl_type, int grade);
	//镶嵌技能
	void OnPutOn(int tl_type,int grid_index,int knapsack_index);
	//卸下技能
	void OnPutOff(int tl_type, int grid_index);
	//刷新抽奖 0:刷新一个  1:刷新所有
	void OnChoujiangFlush(int count);
	//苏醒技能
	void OnAwakeSkill(int grid_index);
	//升级技能
	void OnUpgrageSkillLevel(int tl_type, int grid_index, bool auto_buy);
	//GM开启所有格子
	void OnTestOpenAllGrid();

	void SendTalentAllInfo();
	void UpdateTalentSingleGrid(int tl_type, int number);
	void SendTalentChoujiangPage();

	//关注技能
	void OnSaveAttentionSkill(short skill_id);
	void OnCancelAttentionSkill(short skill_id);
	void SendAttentionSkillList();
	bool IsValidAttentionSkillIdx(int idx);

private:

	int GetTalentGrade(int tl_type);

	Role *m_role;
	JinJieTalentParam m_param;
	CharIntAttrs m_attrs_add;

	int m_capability;

	std::map<int, SkillParam> m_skill_list;
};

#endif //__TALENT_HPP__