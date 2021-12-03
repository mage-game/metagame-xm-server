#ifndef __GREATE_SOLDIER_HPP__
#define __GREATE_SOLDIER_HPP__

#include "servercommon/greatesoldierdef.hpp"
#include "obj/character/attribute.hpp"
#include "greatesoldier_skill.hpp"
#include "engineadapter.h"
#include "greatesoldier_goal.hpp"

#define GREATE_SOLDIER(role) role->GetGreateSoldier()

class Role;

class GreateSoldier
{
	friend class GreateSoldierSkill;
public:
	GreateSoldier() : m_role(nullptr), m_other_capability(0), m_combine_flag(0), m_bianshen_cd_reduce_s(0), m_fabao_skill_id(0) {}
	~GreateSoldier() {}

	void SetRole(Role *role) { m_role = role; }
	void InitParam(const GreateSoldierParam &param);
	void GetInitParam(GreateSoldierParam *param) { *param = m_param; }
	void OnRoleLogin(unsigned int last_logout_time);
	void OnRoleLogout();

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalcAttrBigSmallGoalPer(CharIntAttrs &attr_add);			//大小目标万分比加成

	void Update(time_t now_second, unsigned long interval);
	void OnDayChange(UInt32 old_dayid, UInt32 now_dayid);
	void OnRoleLevelChange(int level);
	void OnCompleteTask(TaskID task_id);


	GreateSoldierParam & GetParam() { return m_param; }
	GreateSoldierGoal & GetGoal() { return m_goal; }

	char GetHuanhuaID() const { return m_param.other_param.use_huanhua_id; }
	int	 OnUseHuanhuaCard(int id, bool is_notify = true);	// 当使用了幻化卡,激活幻化形象标记
	void OnUseHuanhuaID(int id);							// 使用某个幻化形象。0表示取消

	bool OnLevelUp(int seq);								// 升级请求
	void OnBianshen();										// 变身请求
	void OnBianshenTrial(int seq);							// 变身体验请求
	void OnPuton(int item_seq, int slot_seq);				// 装备将位
	void OnPutoff(int slot_seq);							// 卸下将位
	bool OnSlotLevelUp(int slot_seq);						// 升级槽位
	void OnDraw(int draw_type, bool is_auto_buy);			// 抽奖
	void OnUpgradePotential(int seq);						// 潜能升级

	int PutOnEquip(ItemDataWrapper &item_wrapper, int seq);		// 穿上装备
	bool TakeOffEquip(int seq, int equip_slot_index, bool is_puton);	// 卸下装备
	void UpgradeEquip(int seq, int equip_slot_index, short destroy_num, short destroy_backpack_index_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM]);  // 升级装备/吞噬

	void SendAllInfo();

	bool IsBianshenStatus() { return m_param.other_param.cur_used_seq >= 0; }
	UInt32 GetBianshenRemainingTime();				// 获取变身剩余时间
	void AddBianshenCDReduce(int reduce_s, UInt16 skill_id);	// 缩减变身CD
	void ClearBianshenCDReduce(UInt16 skill_id);	// 取消缩减变身CD
	void ForceClearBianshenStatus();				// 强制取消变身状态

	const int GetSlotLevel(const int slot_seq);			// 获取槽位等级
	const int GetPotentialLevelBySeq(const int seq);	// 获取名将的潜能等级

	bool OnTiyanActive(int seq, int tiyan_time_sec);
	bool IsActive(int seq);							// 是否激活名将
	void UnActive(int seq);

	void SetBianshenTrail(int is_on);				// 设置是否处于体验变身

	void GmClearBianshenCD();						// 清除变身cd
	void GmClearAllInfo();

	int GetAchieveXinghunlevelNum(int level);		// 获取星魂达到某个等级的数量
	int GetXinghunSumLevel();						// 获取星魂总等级
	int GetActiveCount(int color);					// 获取激活名将个数
	int GetLevelCount(int level);					// 获取满足等级的名将个数

	void SyncFabaoSkillID(UInt16 skill_id) { m_fabao_skill_id = skill_id; }
	void OnFetchLimitDrawReward(int seq);			// 领取奖励
	void OnExchangeSeq(int seq);

	void OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);
	void SetGreateSoldierBaoji(bool is_baoji) { m_skill.SetBaoji(is_baoji); }	// 设置名将技能暴击属性
	bool IsGreateSoldierBaoji() { return m_skill.IsBaoji(); }					// 是否名将暴击属性技能

private:
	void SendItemInfo(int seq = -1);				// 发送名将信息，seq为-1则全发
	void SendOtherInfo(bool is_bianshen_cd_end);	// 发送其他信息，is_bianshen_cd_end为是否变身cd结束
	void SendSlotInfo();							// 发送将位信息

	int GetCombineMinLevel(int seq);				// 获取组合最低等级
	void CheckCombineFlag();						// 检查组合标记
	void CheckFetchLimitDrawReward();				// 切天补发奖励
	void SendFetchRewardInfo();						// 发送领奖信息
	void CheckFuncOpen();

	
private:
	Role *m_role;
	GreateSoldierParam m_param;
	CharIntAttrs m_attrs_add;
	int m_other_capability;

	UInt32 m_combine_flag;							// 激活的组合标记
	GreateSoldierSkill m_skill;						// 名将技能
	GreateSoldierGoal m_goal;						// 名将目标

	int m_bianshen_cd_reduce_s;						// 变身CD缩短时间
	std::map<UInt16, int> m_bianshen_cd_reduce_skillid;	// 已使变身CD缩短的Skill ID -> reduce time

	UInt16 m_fabao_skill_id;						// 同步 fabao 技能ID，用于清除变身CD缩短
};

//-------------------------------------------------------------------------------------------

inline UInt32 GreateSoldier::GetBianshenRemainingTime()
{
	if (m_param.other_param.bianshen_end_timestamp < (UInt32)(EngineAdapter::Instance().Time()))
	{
		return 0;
	}
	else
	{
		auto remaining_time = m_param.other_param.bianshen_end_timestamp - (UInt32)(EngineAdapter::Instance().Time());
		return remaining_time;
	}
}

//-------------------------------------------------------------------------------------------

#endif // __GREATE_SOLDIER_HPP__