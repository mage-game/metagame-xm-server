#pragma once

class CharIntAttrs;
class Character;
class Role;
class AutoParamBase;

class IRoleModule
{
public:
	IRoleModule() : m_role(nullptr) {}
	virtual ~IRoleModule() {}

	virtual void SetRole(Role *role) { m_role = role; }

	virtual AutoParamBase & GetParam() = 0;
	//初始化数据之后马上调用
	virtual void OnInit() {}
	//保存数据之前调用
	virtual void OnSave() {}

	virtual void ReCalcAttr(CharIntAttrs &base_add, int recalc_reason, bool recalc_all) {}

	virtual void OnRoleLogin() {}
	virtual void OnRoleLogout() {}

	virtual void Update(time_t now_second, unsigned long interval) {}
	virtual void OnDayChange(const ClockReturnDayItem &old_day, const ClockReturnDayItem &new_day) {}
	virtual void OnWeekChange() {}
	virtual void OnMonthChange() {}

	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp) {}
	virtual void OnHurtOther(Character *target, int skill_id, long long &injure, int target_index, int &passive_flag) {}

	virtual void OnAllInfoReq() {}

protected:
	Role * m_role;
};
