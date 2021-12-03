#ifndef __HALO_MANAGER_HPP__
#define __HALO_MANAGER_HPP__

#include "servercommon/halodef.hpp"
#include "obj/character/attribute.hpp"
#include "haloconfig.hpp"

class Character;
class Role;

class HaloManager
{
public:
	HaloManager();
	~HaloManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const HaloParam &halo_param);
	void GetOtherInitParam(HaloParam *param);
	void GetRoleAppearance(RoleAppearance *appearance);

	void OnLogin();
	void OnRoleLevelChange(int level);

	void Update(time_t now_second);
	
	void OnHaloSkillEffect(time_t now_sec);

	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure);

	int GetHaloAppeID();	

	bool ActiveImage(int imageid, bool recalc = true);
	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);

	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	HaloParam * GetHaloParam();

	void SendHaloInfo();

	void PrintHalo(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void CheckActive();

	bool OnUseShuxingdan(int slot_idx, short &use_count);

	bool OnUseChengzhangdan(int use_count);

	int RefreshHaloMinEquipLevel();

	int GetHaloEquipSkillLevel(){return m_halo_param.equip_skill_level;}

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	int GetSpecialImageLevelById(int imageid);

	void CalcBaseAttrAddFromSkill(CharIntAttrs &base_add);					// 技能提供的基础属性加成

	int GetMedalSkillLevel();												// 获取勋章技能等级

	int GetGrade();
private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;

	HaloParam m_halo_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_other_capability;
	long long m_last_add_fangyu;

	UInt64 m_skill0_last_colddown_time;				// 上一次技能冷却结束时间
};

#endif // __HALO_MANAGER_HPP__


