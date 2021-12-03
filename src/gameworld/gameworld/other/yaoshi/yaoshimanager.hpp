#ifndef __YAOSHI_MANAGER_HPP__
#define __YAOSHI_MANAGER_HPP__

#include "servercommon/struct/yaoshiparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;

class YaoShiManager
{
public:
	YaoShiManager();
	~YaoShiManager();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const YaoShiParam &yaoshi_param);
	void GetOtherInitParam(YaoShiParam *param);

	void OnRoleLogin();
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void Update(time_t now_second);

	void OnRoleLevelChange(int level);

	void OnHurtOther(Character *target, int skill_id, long long &injure, int target_index, int &passive_flag);
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	void GetRoleAppearance(RoleAppearance *appearance);
	int GetYaoShiAppeID();
	int GetYaoShiEquipSkillLevel() { return m_yaoshi_param.equip_skill_level; }

	int RefreshYaoShiMinEquipLevel();
	void CheckActive();

	bool ActiveImage(int imageid, bool recalc = true);
	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);

	bool OnUseShuxingdan(int slot_idx, short &use_count);

	void SendYaoShiInfo();
	void PrintYaoShi(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	YaoShiParam * GetYaoShiParam();

	int GetSpecialImageLevelById(int imageid);
	int GetGrade() { return m_yaoshi_param.grade; }								// 获取当前阶数

private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;
	
	YaoShiParam m_yaoshi_param;
	
	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_other_capability;

};

#endif // __YAOSHI_MANAGER_HPP__


