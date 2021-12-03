#ifndef __TOUSHI_MANAGER_HPP__
#define __TOUSHI_MANAGER_HPP__

#include "servercommon/struct/toushiparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;

class TouShiManager
{
public:
	TouShiManager();
	~TouShiManager();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const TouShiParam &toushi_param);
	void GetOtherInitParam(TouShiParam *param);

	void OnRoleLogin();
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void Update(time_t now_second);

	void OnRoleLevelChange(int level);
	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag);
	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	void CalcShanbiExtraEffect();

	void GetRoleAppearance(RoleAppearance *appearance);
	int GetTouShiAppeID();
	int GetTouShiEquipSkillLevel() { return m_toushi_param.equip_skill_level; }

	int RefreshTouShiMinEquipLevel();
	void CheckActive();

	bool ActiveImage(int imageid, bool recalc = true);
	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);

	bool OnUseShuxingdan(int slot_idx, short &use_count);

	void SendTouShiInfo();
	void PrintTouShi(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	TouShiParam * GetTouShiParam();

	int GetSpecialImageLevelById(int imageid);
	int GetGrade() { return m_toushi_param.grade; }								// 获取当前阶数

private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;
	
	TouShiParam m_toushi_param;
	
	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_other_capability;

};

#endif // __TOUSHI_MANAGER_HPP__


