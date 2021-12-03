#ifndef __FABAOMANAGER_H__
#define __FABAOMANAGER_H__

class Role;
class Character;
struct RoleAppearance;

#include "obj/character/attribute.hpp"
#include "servercommon/struct/fabaoparam.hpp"

class FabaoManager
{
public:
	FabaoManager();
	~FabaoManager();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const FabaoParam &param);
	void GetInitParam(FabaoParam *param);
	FabaoParam* GetParam() { return  &m_param; }
	void OnLogin();                                                      // 先放着，以后可能用得到
	void Update(unsigned int now_second);                                   // 刷新  
	void OnRoleUpLevel(int level);                                          // 角色升级时调用，用于激活法宝
	bool ActiveFabao(bool is_recal = true);                                 // 激活法宝

	void SendInfo();                                                        // 推送法宝信息
	void GetRoleAppearance(RoleAppearance *appearance);                     // 获取外观
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);                // 计算加成

	bool UpOneGrade(int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num);				// 使用进阶丹直接进阶
	bool OnUpGrade(bool is_auto_buy, int repeat_times = 1);                                       // 进阶
	void UseImg(short img_id);                                              // 使用形象

	void UseSpecialImg(short special_img_id);                               // 使用特殊形象
	void UnUseSpecialImg(short special_img_id);                             // 不使用特殊形象

	bool ActiveSpecialImg(short special_img_id, bool is_notify = true);		// 激活特殊形象
	bool UnActiveSpecialImg(short special_img_id);                          // 取消激活特殊形象
	bool FabaoSpecialImgUpgrade(short special_img_id);                      // 法宝特殊形象进阶
	int	GetHasActiveFabao();                                                // 获取已经激活的特殊形象数目
	bool OnUseShuxingdan(int slot_idx, short &use_count);                      // 使用属性丹(资质丹)
	bool UplevelSkill(int skill_idx, bool is_auto_buy);                     // 升级技能
	int GetHurtDecPercent();                                                // 其它玩家打自己时减少的伤害百分比
	int GetFabaoEquipSkillLevel() { return m_param.equip_skill_level; }     // 获取法宝装备技能等级
	bool UplevelEquip(int equip_idx);                                       // 装备升级，参数为装备索引
	int RefreshFabaoMinEquipLevel();                                        // 找到装备最低等级

	//void OnRoleBeenAttack(ObjID attacker);
	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure);

	void OnCompleteTask(TaskID task_id);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void CheckActive();

	int GetGrade() { return m_param.grade; }								// 获取当前阶数

	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

	int GetSpecialImageLevelById(int imageid);

	void CalcBaseAttrAddFromSkill(CharIntAttrs &base_add);					// 技能提供的基础属性加成

	int GetMedalSkillLevel();												// 获取勋章技能等级

private:                           
	const static int NEXTDAY_CLEAR_BLESS_VAL_TIME = 5;                      //第二天五点祝福值清零
	const static int CLEAR_BLESS_VAL_GRADE_LIMIT = 5;                       //大于等于4阶第二天五点祝福值清零
	Role *m_role;                                                           // 角色
	FabaoParam m_param;                                                     // 法宝数据

	CharIntAttrs m_attrs_add;												// 各属性加成数值

	UInt64 m_skill0_last_colddown_time;										// 上一次技能冷却结束时间

	int m_other_capability;
};

#endif
