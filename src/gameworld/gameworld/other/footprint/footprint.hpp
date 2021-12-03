#ifndef __FOOTPRINT_MANAGER_HPP__
#define __FOOTPRINT_MANAGER_HPP__

#include "servercommon/footprintdef.hpp"
#include "obj/character/attribute.hpp"
#include "footprintconfig.hpp"
#include "protocal/msgfootprint.hpp"


class Character;
class Role;

class FootPrint
{
public:
	FootPrint();
	~FootPrint();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const FootprintParam &footprint_param);
	void GetOtherInitParam(FootprintParam *param);
	void GetRoleAppearance(RoleAppearance *appearance);

	void OnLogin();
	void OnRoleLevelChange(int level);

	void Update(time_t now_second);

	int GetFootprintAppeID();

	int GetHurtDecPercent();
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill);

	void OperationReq(const Protocol::CSFootprintOperate *req);

	bool ActiveImage(int imageid, bool recalc = true);
	bool ActiveSpecialImage(int imageid, bool is_notify = true);
	bool UnActiveSpecialImage(int imageid);

	bool UseImage(int imageid);
	bool UnUseImage(int imageid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	FootprintParam * GetFootprintParam();

	void SendFootprintInfo();

	void PrintFootprint(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	void OnCompleteTask(TaskID task_id);
	void OnRoleUpLevel(int level);

	bool OnUseShuxingdan(int slot_idx, short &use_count);

	bool OnUseChengzhangdan(int use_count);

	int RefreshFootprintMinEquipLevel();

	int GetFootprintEquipSkillLevel() { return m_footprint_param.equip_skill_level; }

	bool Upgrade(bool is_auto_buy, int repeat_times = 1);
	bool UplevelEquip(int equip_idx);
	bool UpStarLevel(short stuff_index, short is_auto_buy, int loop_times);
	bool UplevelSkill(int skill_idx, bool is_auto_buy);
	bool FootprintSpecialImgUpgrade(short special_img_id);
	bool UpOneGrade(int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num);
	bool ActiveFootprint(bool is_recal = true);
	bool GMResetGrade();

	// 升星助力
	static int UplevelAndSaveBless(Role *role, int add_star);

	// 检查是否能够升级
	static bool CheckCanUpGrade(Role* role);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void CheckActive();

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	int GetGrade() { return m_footprint_param.grade; }					// 获取阶数

	int GetSpecialImageLevelById(int special_image_id);

private:
	const static int SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID

	Role *m_role;

	FootprintParam m_footprint_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_other_capability;
	int m_last_add_fangyu;
};

#endif // __FOOTPRINT_MANAGER_HPP__






