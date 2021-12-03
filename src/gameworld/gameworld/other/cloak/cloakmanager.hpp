#ifndef __CLOAK_MANAGER_HPP__
#define __CLOAK_MANAGER_HPP__

#include "servercommon/cloakdef.hpp"
#include "obj/character/attribute.hpp"
#include "cloakconfig.hpp"
#include "protocal/msgcloak.hpp"

class Character;
class Role;

class CloakManager
{
public:
	CloakManager();
	~CloakManager();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const CloakParam &cloak_param);
	void GetOtherInitParam(CloakParam *param);
	void GetRoleAppearance(RoleAppearance *appearance);

	void OnLogin();
	void OnRoleLevelChange(int level);

	void Update(time_t now_second);

	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag);
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);

	long long CalcBaojiExtraHurt(Character *target);
	void CalcShanbiExtraEffect();

	int GetFootprintAppeID();

	void OperationReq(const Protocol::CSCloakOperate *req);

	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);

	bool IsImageActive(int attr_id);
	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	CloakParam * GetCloakParam();

	void SendCloakInfo();

	void Cloakprint(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	int RefreshCloakMinEquipLevel();

	int GetCloakEquipSkillLevel() { return m_cloak_param.equip_skill_level; }

	bool OnUseShuxingdan(int slot_idx, short &use_count);
	bool ActiveCloak(bool is_recal = true);
	bool UpLevel(short stuff_index, short is_auto_buy);
	bool CloakSpecialImgUpgrade(short special_img_id);

	bool UplevelEquip(int equip_idx);
	bool UplevelSkill(int skill_idx, bool is_auto_buy);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void CheckActive();

	int GetCloakLevel() { return m_cloak_param.level; }
	
private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;

	CloakParam m_cloak_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_other_capability;
	int m_last_add_fangyu;
};

#endif // __CLOAK_MANAGER_HPP__






