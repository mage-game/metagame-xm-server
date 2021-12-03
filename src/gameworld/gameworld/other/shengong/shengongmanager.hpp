#ifndef __SHENGONG_MANAGER_HPP__
#define __SHENGONG_MANAGER_HPP__

#include "servercommon/shengongdef.hpp"
#include "obj/character/attribute.hpp"
#include "shengongconfig.hpp"

class Character;
class Role;

class ShengongManager
{
public:
	ShengongManager();
	~ShengongManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const ShengongParam &shengong_param);
	void GetOtherInitParam(ShengongParam *param);
	void GetRoleAppearance(RoleAppearance *appearance);

	void OnLogin();
	void OnRoleLevelChange(int level);

	void Update(time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag);

	int GetShengongAppeID();	

	bool ActiveImage(int imageid, bool recalc = true);
	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);

	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	ShengongParam * GetShengongParam();

	void SendShengongInfo();

	void PrintShengong(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	bool OnUseShuxingdan(int slot_idx, short &use_count);

	bool OnUseChengzhangdan(int use_count);

	int RefreshShengongMinEquipLevel();

	int GetShengongEquipSkillLevel(){return m_shengong_param.equip_skill_level;}

	void CheckActive();

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	int GetSpecialImageLevelById(int imageid);

	int GetGrade();

private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;

	ShengongParam m_shengong_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_other_capability;
};

#endif // __SHENGONG_MANAGER_HPP__




