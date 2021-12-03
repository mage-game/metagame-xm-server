#ifndef __MOUNT_MANAGER_HPP__
#define __MOUNT_MANAGER_HPP__

#include "servercommon/struct/mountparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;

class MountManager
{
public:
	MountManager();
	~MountManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const MountParam &mount_param);
	void GetOtherInitParam(MountParam *param);
	void GetRoleAppearance(RoleAppearance *appearance);

	void OnLogin();
	void OnRoleLevelChange(int level);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void Update(time_t now_second);

	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	//void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag);

	bool GoonMount(bool goon_mount, bool notify = true, bool is_force = false);
	bool IsOnMount();

	int GetMountAppeID();	

	bool ActiveImage(int imageid, bool recalc = true);
	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);
	bool IsActiveSpecialImage(int imageid);

	bool UseImage(int imageid);
	bool UnUseImage(int imageid);
	
	bool MountCanFly();

	void CheckActive();

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	MountParam * GetMountParam();

	void SendMountInfo();

	void PrintMount(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	bool OnUseShuxingdan(int slot_idx, short &use_count);

	bool UseTemporaryImage(int imageid);

	int RefreshMountMinEquipLevel();

	int GetMountEquipSkillLevel(){return m_mount_param.equip_skill_level;}

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	int GetSpecialImageLevelById(int imageid);

	int GetGradeBlessVal();													// 获取祝福值

	void CalcBaseAttrAddFromSkill(CharIntAttrs &base_add);					// 技能提供的基础属性加成

	int GetMedalSkillLevel();												// 获取勋章技能等级

	int GetGrade();
private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	const static int NEXTDAY_CLEAR_BLESS_VAL_TIME = 5;

	Role *m_role;
	
	MountParam m_mount_param;
	
	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_other_capability;

};

#endif // __MOUNT_MANAGER_HPP__


