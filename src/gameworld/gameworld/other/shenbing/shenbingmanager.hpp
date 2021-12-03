#ifndef __SHENBING_MANAGER_HPP__
#define __SHENBING_MANAGER_HPP__

#include "servercommon/struct/shenbingparam.hpp"
#include "obj/character/attribute.hpp"
#include "shenbingconfig.hpp"

class Character;
class Role;

class ShenBingManager
{
public:
	ShenBingManager();
	~ShenBingManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const ShenBingParam &shenbing_param);
	void GetInitParam(ShenBingParam *shenbing_param);

	ShenBingParam * GetShenBingParam(){ return &m_shenbing_param;}
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	int OnHurtBoss();

	void OnHurtRole(Character *target);

	void Update(time_t now_second);

	void OnShenBingSkillEffect(time_t now_sec);

	void SendAllShenBingInfo();

	bool OnUseShuxingdan(int slot_idx, short &use_count);

	bool UpLevel(Role *role, short stuff_index);

	void PrintShenBing(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	int GetShenBingLevel() { return m_shenbing_param.level; }

private:
	void RefreshSkillLevel();

	Role *m_role;

	ShenBingParam m_shenbing_param;
	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_skill_level_list[ShenBingConfig::MAX_SHENBING_SKILL_COUNT];
	long long m_last_add_gongji;
};

#endif // __SHENBING_MANAGER_HPP__
