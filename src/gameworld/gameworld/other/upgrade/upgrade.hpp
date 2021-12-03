#ifndef __UPGRADE_SYS_HPP__
#define __UPGRADE_SYS_HPP__

#include "servercommon/struct/upgradeparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;
class UpgradeManager;
struct SkillConifg;

class Upgrade
{
public:
	Upgrade(UpgradeManager *upgrade_manager, int upgrade_type);
	virtual ~Upgrade();

	virtual void Init(Role *role, const UpgradeBaseData &upgrade_base_data);
	virtual void GetInitParam(UpgradeBaseData *upgrade_base_data);

	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	virtual void Update(time_t now_second);

	virtual void OnLogin();

	virtual void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	bool OnFightOut(bool is_fight_out, bool notify = true, bool is_force = false);
	bool IsFightOut();

	int GetUpgradeAppeID();													// 获取形象ID

	UpgradeBaseData * GetBaseData();										// 获取系统数据
	int GetUpgradeType() { return m_upgrade_type; }							// 获取系统类型
	int GetUpgradeCritType();												// 获取系统暴击类型

	bool CheckCanOperate();													// 检查能否操作

	bool OnConsumeUpgradeStuff(ItemID stuff_id, ItemID stuff2_id, int total_consume_count, bool is_auto_buy, std::string reason);   // 进阶系统材料消耗接口

	//进阶相关---------------------------------------------------------------
	 bool OnUpgrade(bool is_auto_buy, int repeat_times = 1);										// 进阶

	 bool CheckCanUpOneGrade(int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num);	// 检查能否直升一阶
	 bool OnUpOneGrade();													// 直升一阶

	//形象相关---------------------------------------------------------------
	bool ActiveImage(int imageid, bool recalc = true);						// 激活形象
	bool UnActiveImage(int imageid);										// 还原形象
	bool IsImageActive(int imageid);										// 判断是否激活

	bool UseImage(int imageid);												// 使用形象
	bool UnUseImage(int imageid);											// 不使用形象

	bool UseTemporaryImage(int imageid);									// 使用临时形象

	bool CheckCanImgUpgrade(int img_id);									// 检查能否形象进阶
	bool OnImgUpgrade(int img_id);											// 形象进阶

	void OnUpgradeAppeChange();												// 外观改变

	//装备相关---------------------------------------------------------------
	bool CheckCanUplevelEquip(int equip_idx);								// 能否升级装备
	bool OnUplevelEquip(int equip_idx);										// 升级装备

	//技能相关---------------------------------------------------------------
	bool CheckCanUpLevelSkill(int skill_idx, bool is_auto_buy);				// 能否升级技能
	bool OnUplevelSkill(int skill_idx);   									// 升级技能
	void OnAddSkillAttr(CharIntAttrs &attrs, const SkillConifg *skill_cfg);	// 获取类型技能属性

	//触发相关---------------------------------------------------------------
	void OnCompleteTask(TaskID task_id);									// 完成任务
	void OnRoleUpLevel(int level);											// 角色升级

	void CheckActiveSys();													// 检查能否激活
	bool ActiveSys(bool is_recal = true);									// 激活系统

	bool OnUseShuxingdan(int slot_idx, short &use_count);					// 使用属性丹

	void SendUpgradeInfo();													// 发送信息

	void PrintUpgradeLog(const char *reason, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0);

	//外部调用相关---------------------------------------------------------------
	void GetBaseAttr(CharIntAttrs &tmp_attr);								// 获取系统基础属性

	int GetSkill0Level(){return m_base_data.skill_level_list[0];}			// 获取技能0等级
	int GetGrade() { return m_base_data.grade; }							// 获取阶数

	bool IsSpecialImg(int img_id);											// 是否是特殊形象

	void SuperSpecialCapAdd(int image_id, int grade, CharIntAttrs &m_attrs_add);	//超强战力加成

	bool GMResetGrade(Role *role);		// GM重置阶数

	int GetImageLevel(int imageid);

	void CalcBaseAttrAddFromSkill(CharIntAttrs &base_add);					// 技能提供的基础属性加成

	int GetMedalSkillLevel();												// 获取勋章技能等级

private:
	Role *m_role = nullptr;
	UpgradeBaseData m_base_data;
	CharIntAttrs m_attrs_add;								// 各属性加成数值

	UpgradeManager *m_upgrade_manager = nullptr;			// 进阶系统管理器

	int m_upgrade_type = UPGRADE_TYPE_INVALID;				// 进阶类型

	int m_other_capability = 0;								// 其他额外战力

};

#endif // 


