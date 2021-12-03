//-------------------------------------------------------------------------------------------

// Role其他系统管理器

//-------------------------------------------------------------------------------------------

#ifndef __ROLE_MODULE_MGR_H__
#define __ROLE_MODULE_MGR_H__

#include "gamedef.h"
#include "roleothermgr_def.h"
#include "servercommon/servercommon.h"


// 各种前置声明
class Role;
class RoleOtherInitParam;
class CharIntAttrs;
class Character;
class RoleAllModule;
class IRoleModule;

MODULE_FORWARD_DECLARATION();

enum RoleModuleType
{
	//数据存储依赖，顺序关系
	ROLE_MODULE_TYPE_MIN = 0,
	ROLE_MODULE_TYPE_TIANSHENHUTI = ROLE_MODULE_TYPE_MIN,  // 周末装备

	ROLE_MODULE_TYPE_COUNT
};

class RoleOtherMgr
{
public:
	RoleOtherMgr();
	~RoleOtherMgr();

	void* operator new(size_t size);
	void operator delete(void* p);

	// Init funcs
	void SetRole(Role *role);
	void InitParam(const RoleOtherInitParam &init_param);
	void GetInitParam(RoleOtherInitParam *init_param);
	void InitParamPage10(DataPageParam &data_page);
	void InitParamPage10ByCahce(DataPageParam& data_page);
	void GetInitParamPage10(DataPageParam &data_page);

	void ReCalcAttr(CharIntAttrs &base_add, int recalc_reason, bool recalc_all);

	// Events funcs
	void Update(time_t now_second, unsigned long interval);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnWeekChange();
	void OnMonthChange();
	void OnAttackCalcHurt(ObjID attacker, Attribute &delta_hp);
	void OnHurtOther(Character *target, int skill_id, long long &injure, int target_index, int &passive_flag);
	void OnKillOther(Character *dead);
	void OnDie(ObjID killer_objid);
	void OnEnterScene();
	void OnLeaveScene();
	void OnRoleLogin();
	void OnRoleLogout();

	// Common Req funcs
	void OnAllInfoReq();

	IRoleModule * GetRoleModule(int type) const;

	// Get funcs
	MODULE_GET_FUNCS_DECLARATION();

private:
	Role *m_role;

	IRoleModule *m_all_modules[ROLE_MODULE_TYPE_COUNT]{};
};

#endif // __ROLE_MODULE_MGR_H__
