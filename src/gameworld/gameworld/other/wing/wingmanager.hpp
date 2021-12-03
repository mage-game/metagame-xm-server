#ifndef __WING_MANAGER_HPP__
#define __WING_MANAGER_HPP__

#include "servercommon/wingdef.hpp"
#include "obj/character/attribute.hpp"
#include "wingconfig.hpp"

class Character;
class Role;

class WingManager
{
public:
	WingManager();
	~WingManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const WingParam &wing_param);
	void GetOtherInitParam(WingParam *param);
	void GetRoleAppearance(RoleAppearance *appearance);

	void OnLogin();
	void OnRoleLevelChange(int level);

	void Update(time_t now_second);

	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure);

	int GetWingAppeID();	

	bool ActiveImage(int imageid, bool recalc = true);
	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid, bool is_activity_img = false);

	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	WingParam * GetWingParam();

	void SendWingInfo();

	void PrintWing(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	bool OnUseShuxingdan(int slot_idx, short &use_count);

	bool UseTemporaryImage(int imageid);

	int RefreshWingMinEquipLevel();

	int GetWingEquipSkillLevel(){return m_wing_param.equip_skill_level;}

	void OnChallengeFieldRankInfo(int rank_pos);		// // 通过邮件发放，已无用
	void CheckShowChallengeFieldRank();

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void CheckActive();

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	void CheckActiveSpecialImg(bool is_mail = true);

	int GetSpecialImageLevelById(int imageid);

	void CalcBaseAttrAddFromSkill(CharIntAttrs &base_add);					// 技能提供的基础属性加成

	int GetMedalSkillLevel();												// 获取勋章技能等级

	int GetGrade();
private:
	void SendUnActiveSpecialImgMail(int img_id);

	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;

	WingParam m_wing_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_other_capability;

	unsigned long long m_skill_shouhu_loss_effect_time;						// 守护技能有效时间

	int m_skill_shouhu_count;												// 羽翼守护技能次数
};

#endif // __WING_MANAGER_HPP__




