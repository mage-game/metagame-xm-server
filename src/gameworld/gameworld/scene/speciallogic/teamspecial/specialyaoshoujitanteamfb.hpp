#ifndef __SPECIAL_YAOSHOUJITAN_FB_HPP__
#define __SPECIAL_YAOSHOUJITAN_FB_HPP__

#include "servercommon/fbdef.hpp"
#include "scene/speciallogic/speciallogic.hpp"

class SpecialYaoShouJiTanTeamFb : public SpecialLogic
{
	//属性有相克，如：金克木，火克金，水克火，木克水	
	enum ATTR_TYPE
	{
		ATTR_MIN = 0,
		ATTR_MU = ATTR_MIN,		// 木
		ATT_JIN,				// 金
		ATTR_HUO,				// 火
		ATTR_SHUI,				// 水

		ATTR_MAX,
	};
	UNSTD_STATIC_CHECK(YAOSHOUJITAN_TEAMFB_MAX_ATTR_NUM == ATTR_MAX);

	enum KE_TYPE
	{
		KE_NONE = 0,			// 不克
		KE_1K2 = 1,				// 属性1 克 属性2
		KE_2K1 = 2,				// 属性2 克 属性1
	};

public:
	SpecialYaoShouJiTanTeamFb(Scene *scene);
	~SpecialYaoShouJiTanTeamFb();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool SpecialCanGuWu() { return true; }

	void CreateMonster(int mode);
	virtual void OnRoleEnterScene(Role *role);
	virtual bool OnTimeout();
	virtual void OnAllMonsterDead();
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual long long SpecialRecalcInjure(Character *attacker, Character *injurer, long long injure);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual void OnLeaveFB(Role *role);

private:
	bool IsInTeam(int uid);
	void CalcRoleAttr(Role *role);
	void CalcBossAttr();
	int GetRoleAttrType(Role *role);
	int CalcAttrKe(Character *be_attack, Character *attacker);
	int CalcAttrKe(int attr1, int attr2);
	void OnFinish(bool is_pass);
	void PutPassReward();
	void SyncSceneLogicInfo();

	bool m_is_init;
	bool m_is_finish;
	bool m_is_pass;

	int m_mode;
	int m_team_index;
	int m_boss_attr_type;
	unsigned int m_next_calc_boss_attr_time;

	YaoShouJiTanTeamFbRoleAttrInfo m_role_attrs[YAOSHOUJITAN_TEAMFB_MAX_ATTR_NUM];
};

#endif

