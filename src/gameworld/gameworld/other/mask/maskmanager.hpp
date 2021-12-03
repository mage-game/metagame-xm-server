#ifndef __MASK_MANAGER_HPP__
#define __MASK_MANAGER_HPP__

#include "servercommon/struct/maskparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;

class MaskManager
{
public:
	MaskManager();
	~MaskManager();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const MaskParam &mask_param);
	void GetOtherInitParam(MaskParam *param);

	void OnRoleLogin();
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void Update(time_t now_second);

	void OnRoleLevelChange(int level);

	void OnHurtOther(Character *target, int skill_id, long long &injure, int target_index, int &passive_flag);
	void CheckOnAttack();

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	void GetRoleAppearance(RoleAppearance *appearance);
	int GetMaskAppeID();
	int GetMaskEquipSkillLevel() { return m_mask_param.equip_skill_level; }

	int RefreshMaskMinEquipLevel();
	void CheckActive();

	bool ActiveImage(int imageid, bool recalc = true);
	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);

	bool OnUseShuxingdan(int slot_idx, short &use_count);

	void SendMaskInfo();
	void PrintMask(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	int GetSpecialImageLevelById(int special_image_id);
	int GetGrade() { return m_mask_param.grade; }								// 获取当前阶数

	MaskParam * GetMaskParam();

private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;
	
	MaskParam m_mask_param;
	
	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_other_capability;

	time_t m_last_check_hp_time;										// 上次检查剩余血量时间

};

#endif // __MASK_MANAGER_HPP__


