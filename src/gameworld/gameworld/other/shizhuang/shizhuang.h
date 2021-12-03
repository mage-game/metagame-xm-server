#ifndef __SHIZHUANG_H__
#define __SHIZHUANG_H__

class Role;
struct RoleAppearance;

#include "obj/character/attribute.hpp"
#include "obj/character/character.h"
#include "servercommon/struct/shizhuangparam.hpp"

enum SHIZHUANG_CHECK_ACTIVE_TYPE
{
	SHIZHUANG_CHECK_ACTIVE_TYPE_ALL,
	SHIZHUANG_CHECK_ACTIVE_TYPE_EQUIP,
	SHIZHUANG_CHECK_ACTIVE_TYPE_GCZ,
	SHIZHUANG_CHECK_ACTIVE_TYPE_1V1,
	SHIZHUANG_CHECK_ACTIVE_TYPE_CROSS_ADD_CAP,
	SHIZHUANG_CHECK_ACTIVE_TYPE_CROSS_ADD_CHARM, 
};

class Shizhuang
{
public:
	Shizhuang();
	~Shizhuang();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const ShizhuangParam &param);
	void GetInitParam(ShizhuangParam *param);
	ShizhuangParam* GetParam() { return  &m_param; }
	void OnWeekChange();
	void OnLogin();
	void Update(unsigned int now_second);
	void OnRoleUpLevel(int level);                                          // 角色升级时调用，用于激活时装
	void OnCompleteTask(TaskID task_id);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	bool ActiveShiZhuang(int shizhuang_type, bool is_recal = true);         // 激活时装

	void CheckShizhuangSkill();
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	void OnHurtOther(Character *target, long long base_injure, long long &injure);

	//使用进阶丹
	bool UpOneGradeShiZhuang(int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num);
	bool UpOneGradeShenBing(int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num);

	void SendInfo();
	void GetRoleAppearance(RoleAppearance *appearance);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	bool OnUpGrade(int shizhuang_type, bool is_auto_buy, int repeat_times = 1);
	bool UpgradeSpecialImg(int shizhuang_type, int special_img_id);
	void UseImg(int shizhuang_type, int img_id);
	void UseMaxGradeImg(int shizhuang_type);

	void UseSpecialImg(int shizhuang_type, int special_img_id);
	void UnUseSpecialImg(int shizhuang_type, int special_img_id);

	bool ActiveSpecialImg(int part, int index, bool is_notify = true);		// 激活特殊形象
	bool UnActiveSpecialImg(int part, int index, bool is_need_mail_notice = false);
	void CheckActiveByCondition(int check_active_type = SHIZHUANG_CHECK_ACTIVE_TYPE_ALL);
	bool ActiveBodySpecialImage(int index, bool is_notify = true);
	bool ActiveWuqiSpecialImage(int index, bool is_notify = true);

	int	GetHasActiveShizhuang(int part);
	int RefreshMinEquipLevel(short shizhuang_type);
	bool OnUseShuxingdan(int slot_idx, short &use_count);                      // 使用属性丹(资质丹)
	bool UpSkillLevel(Role *role, short shizhuang_type, int skill_idx, bool is_auto_buy);
	ShizhuangParam *GetShiZhuangParam() { return &m_param; }
	bool UplevelEquip(Role *role, short shizhuang_type, int equip_idx);
	void CheckActive();
	bool IsActive(int part, int index);
	bool IsActiveImage(int part, int index);
	void ShiZhuangHuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);
	void ShiZhuangWuQiHuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	int GetShiZhuangGrade(int shizhuang_type);

	int GetSpecialImageLevelById(int shizhuang_type, int imageid);
private:
	bool UpOneGrade(int typ, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num);
	Role *m_role;
	ShizhuangParam m_param;

	CharIntAttrs m_attrs_add_array[SHIZHUANG_TYPE_MAX];												// 各属性加成数值
	const static short NEXTDAY_CLEAR_BLESS_VAL_TIME = 5;                   //下一天凌晨5点；

	UInt64 m_body_skill0_last_colddown_time;				// 上一次时装技能冷却结束时间
	UInt64 m_wuqi_skill0_last_colddown_time;				// 上一次神兵技能冷却结束时间

	int m_other_capability[SHIZHUANG_TYPE_MAX];
};

#endif

