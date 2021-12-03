#ifndef __SHENGQI_H__
#define __SHENGQI_H__

#include "protocal/msgshengqi.hpp"
#include "obj/character/attribute.hpp"

class Role;

class Shengqi
{
public:
	Shengqi();
	~Shengqi();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const ShengqiParam &shengqi_param);
	void GetInitParam(ShengqiParam *shengqi_param);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalcAttrBigSmallGoalAddPer(CharIntAttrs & attrs_add);			//大小目标万分比加成
	void ShengqiOpera(int opera_type, int param1, int param2);
	void SendShengqiInfo();
	void OnActivateShengqi(ItemID item_id, bool open_system);
	void OnOpenSpiritAttribute(ItemID item_id, int slot_idx);
	void ShengqiSpirit(ItemID item_id, ItemID stuff_id);
	void ShengqiStrength(ItemID item_id, ItemID stuff_id);
	void Decompose(ItemID stuff_id , int num);

	int GetTotalStrengthLevel();
	int GetStrengthLevelCount(int level);               // 获取达到强化等级的圣器个数

	void OnDayChange(UInt32 old_dayid, UInt32 now_dayid);
	void OnRoleLevelChange(int level);
	void OnCompleteTask(TaskID task_id);
	void CheckFuncOpen();


	void CheckActivateShengqiSystem();
	void OnRoleLogin();

private:
	Role *m_role;

	ShengqiParam m_param;
	CharIntAttrs m_attrs_add;												// 各属性加成数值
};

#endif
