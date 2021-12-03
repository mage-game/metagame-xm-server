#ifndef __SHENYI_MANAGER_HPP__
#define __SHENYI_MANAGER_HPP__

#include "servercommon/shenyidef.hpp"
#include "obj/character/attribute.hpp"
#include "shenyiconfig.hpp"

class Character;
class Role;

class ShenyiManager
{
public:
	ShenyiManager();
	~ShenyiManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const ShenyiParam &shenyi_param);
	void GetOtherInitParam(ShenyiParam *param);
	void GetRoleAppearance(RoleAppearance *appearance);

	void OnLogin();
	void OnRoleLevelChange(int level);

	void Update(time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag);
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);

	int GetShenyiAppeID();	

	bool ActiveImage(int imageid, bool recalc = true);
	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);

	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	ShenyiParam * GetShenyiParam();

	void SendShenyiInfo();

	void PrintShenyi(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	bool OnUseShuxingdan(int slot_idx, short &use_count);

	bool OnUseChengzhangdan(int use_count);

	int RefreshShenyiMinEquipLevel();

	int GetShenyiEquipSkillLevel(){return m_shenyi_param.equip_skill_level;}

	void CheckActive();

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	int GetSpecialImageLevelById(int special_image_id);

	int GetGrade();
private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;

	ShenyiParam m_shenyi_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_other_capability;
};

#endif // __SHENYI_MANAGER_HPP__






