#ifndef __QILINBI_MANAGER_HPP__
#define __QILINBI_MANAGER_HPP__

#include "servercommon/struct/qilinbiparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;

class QiLinBiManager
{
public:
	QiLinBiManager();
	~QiLinBiManager();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const QiLinBiParam &qilinbi_param);
	void GetOtherInitParam(QiLinBiParam *param);

	void OnRoleLogin();
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void Update(time_t now_second);

	void OnRoleLevelChange(int level);

	void OnHurtOther(Character *target, int skill_id, long long &injure, int target_index, int &passive_flag);
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	void GetRoleAppearance(RoleAppearance *appearance);
	int GetQiLinBiAppeID();
	int GetQiLinBiEquipSkillLevel() { return m_qilinbi_param.equip_skill_level; }

	int RefreshQiLinBiMinEquipLevel();
	void CheckActive();

	bool ActiveImage(int imageid, bool recalc = true);
	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);

	bool OnUseShuxingdan(int slot_idx, short &use_count);

	void SendQiLinBiInfo();
	void PrintQiLinBi(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	QiLinBiParam * GetQiLinBiParam();

	int GetSpecialImageLevelById(int imageid);
	int GetGrade() { return m_qilinbi_param.grade; }								// 获取当前阶数

private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;
	
	QiLinBiParam m_qilinbi_param;
	
	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_other_capability;

};

#endif // __QILINBI_MANAGER_HPP__


