#ifndef __GLOBAL_ROLE_HPP__
#define __GLOBAL_ROLE_HPP__

#include "servercommon/struct/global/globalroledataparam.hpp"
#include "obj/character/attribute.hpp"
#include "servercommon/crossroleglobaldef.hpp"

class GlobalRoleManager;
class GlobalRole;
class Role;
struct UserCacheNode;
class LittlePet;
class LoveTree;
class LoveContract;
class JingLingHome;
class RoleFighting;
class QingyuanGlobal;
class CommonDataGlobal;
class SpouseHome;

class IGlobalRoleItem
{
public:
	friend class GlobalRole;

	IGlobalRoleItem(GlobalRole *g_role);
	virtual ~IGlobalRoleItem() {}

	virtual void Update(unsigned long interval, time_t now_second) {}
	virtual void ResetData(unsigned int old_dayid, unsigned int now_dayid) {}

protected:
	void OnDataChange();
	Role *GetRole() const;
	UserCacheNode *GetUserCache() const;

	int m_role_id;

private:
	GlobalRole *m_global_role;
};

class GlobalRole
{
public:
	friend class IGlobalRoleItem;

	GlobalRole(GlobalRoleManager *mgr, int uid);
	~GlobalRole();

	int GetOwnerUid() { return m_role_id; }
	
	void Init(const GlobalRoleDatalListParam::RoleDataAttr &global_role_attr);
	void GetAttr(GlobalRoleDatalListParam::RoleDataAttr &global_role_attr);
	void ReCalcAttr(CharIntAttrs &base_add, int recalc_reason, bool recalc_all, Role *role);

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void GetCrossData(RoleCrossGlobalData &cross_role_global_data, Role *role);
	void OnRoleLogin(Role *role);
	void OnRoleLogout(Role *role);
	void UpdateToRank(Role *role) const;
	
	RoleAppearance &GetRoleAppearance();
	CommonDataGlobal * GetCommonDataGlobal() { return m_common_data_global; }

	LittlePet * GetLittlePet() { return m_little_pet; }
	LoveTree * GetLoveTree() { return m_love_tree;}
	LoveContract * GetLoveContract() { return m_love_contract;}
	JingLingHome * GetJingLingHome() { return m_jing_ling_home; }
	RoleFighting * GetRoleFighting() { return m_role_fighting; }
	QingyuanGlobal * GetQingyuanGlobal() { return m_qingyuan_global; }
	SpouseHome * GetSpouseHomeGlobal() { return m_spouse_home_global; }

protected:
	void OnDataChange();
	void CheckFightingChallengeRankReward() const;			// 决斗场 - 挑衅，排行奖励发放

private:
	GlobalRoleManager *m_global_role_mgr;

	int m_role_id;

	RoleAppearance m_role_appearance;						// 角色外观数据
	CommonDataGlobal *m_common_data_global;

	LittlePet *m_little_pet;
	LoveTree * m_love_tree;
	LoveContract *m_love_contract;
	JingLingHome *m_jing_ling_home;
	RoleFighting *m_role_fighting;
	QingyuanGlobal *m_qingyuan_global;
	SpouseHome *m_spouse_home_global;
};

#endif
