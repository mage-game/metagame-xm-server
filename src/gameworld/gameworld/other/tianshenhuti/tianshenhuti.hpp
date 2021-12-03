#ifndef _TIANSHENHUTI_HPP_
#define _TIANSHENHUTI_HPP_

#include "servercommon/tianshenhutidef.hpp"
#include "obj/character/attribute.hpp"
#include "role_module/i_role_module.hpp"

class Role;

/*
 * 周末装备
 */
class TianshenHuti : public IRoleModule
{
public:
	TianshenHuti();
	~TianshenHuti() override;

	virtual AutoParamBase & GetParam() override { return m_param; }
	virtual void OnInit() override;

	virtual void ReCalcAttr(CharIntAttrs &base_add, int recalc_reason, bool recalc_all) override;
	virtual void OnRoleLogin() override;
	virtual void OnAllInfoReq() override { this->SendAllInfoToRole(); }
	virtual void OnDayChange(const ClockReturnDayItem &old_day, const ClockReturnDayItem &new_day) override;
	virtual void OnWeekChange() override;

	virtual void OnHurtOther(Character *target, int skill_id, long long &injure, int target_index, int &passive_flag) override;

	void PutOnEquip(int backpack_slot_index);								// 把指定背包格子的装备穿在身上
	void TakeOffEquip(int slot_index);										// 把指定部位装备脱下
	void TransformEquip(int slot_index1, int slot_index2, int new_equip_slot);// 转化两个背包里的装备
	void CombineEquip(int slot_index1, int slot_index2, int slot_index3, int client_param);
	bool CombioneOneEquip(int slot_index1, int slot_index2, int slot_index3, int *new_equip_id);
	// 合成三个个背包里的装备
	void CombineEquipOneKey(int level);										// 一键合成

	int GetBackpackEmptyGridNum();											// 获取剩余背包格子
	int PutEuipToBackpack(TianshenEquipID equip_id);						// 把指定装备放进背包
	TianshenEquipID GetBackpackEquip(int slot_index);						// 获取指定背包格子里的装备
	TianshenhutiParam* GetTianshenhutiParam() { return &m_param.Param(); }

	void RollReq(int roll_type);
	void SendAllInfoToRole();												// 发送所有信息给玩家

	void AddRollScore(int add_score, const char *reason);					// 增加抽奖积分
	bool OnAddEquipByItem(int equip_id, const char *reason);
	bool OnAutoAddEquipByItem(int equip_id, const char *reason);			// 自动增加装备到周末装备的背包
	void GetTaozhuangInjureAdd(int skill_index, double *rate_injure, int *static_injure);

private:
	bool RemoveEquipFromBackpack(int slot_index);							// 删除指定位置装备，返回装备id（拿最后一个装备填充空缺，慎用）

	void UpdateTaozhuangNum(bool is_init = false);

	void AddAccumulateTime(int add_times);

	AutoTianshenhutiParam m_param;
	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_backpack_num;														// 当前背包物品个数

	std::map<int, int> m_taozhuang_num_map;
};

#endif