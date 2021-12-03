#ifndef __FIGHT_MOUNT_MANAGER_HPP__
#define __FIGHT_MOUNT_MANAGER_HPP__

#include "servercommon/struct/fightmountparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;

class FightMountManager
{
public:
	FightMountManager();
	~FightMountManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const FightMountParam &mount_param);
	void GetOtherInitParam(FightMountParam *param);
	void GetRoleAppearance(RoleAppearance *appearance);

	void OnLogin();
	void OnRoleLevelChange(int level);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void Update(time_t now_second);

	//void OnHurtOther(Character *target, int skill_id, long long &injure, int target_index, int &passive_flag);
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);

	bool GoonFightMount(bool goon_mount, bool notify = true, bool is_force = false);
	bool IsOnFightMount();

	int GetFightMountAppeID();	

	bool ActiveImage(int imageid, bool recalc = true);
	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);

	bool UseImage(int imageid);
	bool UnUseImage(int imageid);
	
	bool FightMountCanFly();

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	FightMountParam * GetFightMountParam();

	void SendFightMountInfo();

	void PrintFightMount(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	bool OnUseShuxingdan(int slot_idx, short &use_count);

	bool OnUseChengzhangdan(int use_count);

	int RefreshFightMountMinEquipLevel();

	int GetFightMountEquipSkillLevel(){return m_mount_param.equip_skill_level;}

	void CheckActive();

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	int GetSpecialImageLevelById(int imageid);

	void CalcBaseAttrAddFromSkill(CharIntAttrs &base_add);					// 技能提供的基础属性加成

	int GetMedalSkillLevel();												// 获取勋章技能等级

	int GetGrade();															// 获取阶数
private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;
	
	FightMountParam m_mount_param;
	
	CharIntAttrs m_attrs_add;												// 各属性加成数值

	unsigned long m_last_skill_time;
	int m_other_capability;

};

#endif // __MOUNT_MANAGER_HPP__


