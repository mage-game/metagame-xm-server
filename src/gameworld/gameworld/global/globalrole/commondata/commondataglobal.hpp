#ifndef __COMMONDATA_GLOBAL_HPP__
#define __COMMONDATA_GLOBAL_HPP__

#include "global/globalrole/globalrole.hpp"

#include <deque>

class Role;

class CommonDataGlobal : public IGlobalRoleItem
{
public:
	CommonDataGlobal(GlobalRole *g_role);
	~CommonDataGlobal();

	virtual void Init(const CommonDataGlobalParam &attr_data);
	virtual void GetAttr(CommonDataGlobalParam &attr_data);

	void OnRoleLogin(Role *role);
	void OnRoleLogout(Role *role);

	//跨服元宝消耗--------------------------------------------------------
	void AddUseGoldOnCross(int comsume_gold);

	//合服BOSS抢夺--------------------------------------------------------
	void AddCombineKillBoss(int count);
	void CheckAddCombineKillBossCount();

	// 攻城战-------------------------------------------------------------
	void SetChengzhu();				// 设置城主标记
	void ClearChengzhu();			// 清除城主标记
	void SyncChengzhu();			// 同步攻城战城主技能
	void SyncChengzhuToCloss();		// 同步攻城战城主技能到跨服

	// 物品赠送-------------------------------------------------------------
	void InitGiveItemRecord();
	void SaveGiveItemRecord();
	void AddGiveItemRecord(const ItemGiftInfo &info, bool is_give);		// 增加赠送/收到记录
	void SendGiveItemRecord(Role *role, bool is_give);

	// 跨服竞技场-------------------------------------------------------------
	void CheckReMoveInvalidId();							// 检查移除奖励id
	void CalcNextClearTimes();						
	void AddCrossChallengefieldRankImageId(short rank, short mount_id, short title_id);

	short GetCrossChallengefieldRankMountId();
	void CheckSyncCrossChallengefieldRankMountId(bool is_add);

	short GetCrossChallengefieldRankTitleId();
	void CheckSyncCrossChallengefieldRankTitleId(bool is_add);

private:

	CommonDataGlobalParam m_param;

	std::deque<ItemGiftInfo> m_give_item_list;		// 赠送记录
	std::deque<ItemGiftInfo> m_recieve_item_list;	// 收到记录
};

#endif // __COMMONDATA_GLOBAL_HPP__
