#ifndef __QINGYUAN_GLOBAL_HPP__
#define __QINGYUAN_GLOBAL_HPP__

#include "obj/character/attribute.hpp"
#include "global/globalrole/globalrole.hpp"
#include "protocal/msgother.h"

class Role;

class QingyuanGlobal : public IGlobalRoleItem
{
public:
	QingyuanGlobal(GlobalRole *g_role);
	~QingyuanGlobal();

	virtual void Init(const QingyuanGlobalParam &attr_data);
	virtual void GetAttr(QingyuanGlobalParam &attr_data);

	void Update(unsigned int now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnRoleLogin(Role *role);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc, Role *role);
	void GetTotalAttr(CharIntAttrs &tmp_attr, bool is_self);

	void SetLoverId(int lover_id, bool is_calc = true);
	int GetLoverId();

	void QingyuanEquipOperate(Role *role ,int operate_type, int param_1, int param_2, int param_3); // 操作请求

	void OnUpQingyuanEquip2Level(Role *role, int equip_index);										// 升级情比金坚装备
	void OnBuyGift(Role *role, int seq);															// 购买材料礼包

	bool AddMarryLevelExp(Role *role, int exp);
	int GetMarryLevel() { return m_param.marry_level; }

	void SetIsAllTaskComplete(short is_all_task_complete);
	short GetIsAllTaskComplete() { return m_param.is_all_task_complete; }

	int PutOnQingyuanEquip(Role *role, const ItemDataWrapper &item_wrapper);						// 穿情缘装备

	int GetSatisfyMagicalPreciousEquipCount(int order, int color);									// 获取符合条件的装备个数
	int GetActiveQingyuanEquip2Count();																// 获取已激活情比金坚装备个数

	// 夫妻光环
	void SetFightOutCouplehalo(char halo_type) { m_param.fight_out_couple_halo_type = halo_type; }
	int GetFightOutCouplehalo() { return m_param.fight_out_couple_halo_type; }

	// 表白
	UInt16 GetProfessLevel() { return m_param.profess_level; }
	void SetProfessLevel(short level) { m_param.profess_level = level; }
	void ProfessLevelUp() { ++m_param.profess_level; }
	UInt32 GetProfessExp() { return m_param.profess_exp; }
	void SetProfessExp(int exp) { m_param.profess_exp = exp; }

	void CheckSetAddProfessSharePer(int shizhuang_id);
	void SetAddProfessSharePer() { m_param.is_add_profess_share_per = 1; }
	bool IsAddProfessSharePer() { return m_param.is_add_profess_share_per == 1; }					// 是否有表白共享属性额外加成

	// 特殊宝宝
	bool OnActiveSpecialBaby(Role *role, int special_baby_type, int quality);						// 激活特殊宝宝
	bool GetRandActiveSpecialBaby(Role *role, int &special_baby_type);								// 随机获取一个激活的特殊宝宝

	void SendQingyuanGlobalInfo(Role *role, bool is_self);											// 发送信息

private:

	int TakeOffQingyuanEquip(Role *role, int equip_index);											// 脱情缘装备
	int GetEquipIndexByType(int equip_type);														// 获取装备部位
	void ActiveQingyuanSuitSlot(Role *role, int type, int slot, int knapack_index);					// 激活装备套装
	bool CheckSuitCondition(Role *role, int type, int slot);										// 检查套装

	CharIntAttrs m_attrs_add;
	QingyuanGlobalParam m_param;
};


#endif // __QINGYUAN_GLOBAL_HPP__
