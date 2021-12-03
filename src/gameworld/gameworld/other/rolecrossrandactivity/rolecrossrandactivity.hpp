#ifndef __ROLE_CROSS_RAND_ACTIVITY_HPP__
#define __ROLE_CROSS_RAND_ACTIVITY_HPP__

#include "servercommon/activitydef.hpp"
#include "servercommon/rolecrossrandactivitydef.hpp"

class Role;
class ActivityShadow;

namespace Protocol {
	class CSCrossRandActivityRequest;
}

class RoleCrossRandActivity
{
public:
	RoleCrossRandActivity();
	~RoleCrossRandActivity();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const RoleCrossRandActivityParam &p);
	void GetInitParam(RoleCrossRandActivityParam *p);

	void OnResetData();
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnWeekChange();

	void OnRoleLogin();
	void Update(unsigned int now_second);

	void OnActivityOpen(int activity_type);
	void OnActivityClose(int activity_type);

	void OnClientRequest(Protocol::CSCrossRandActivityRequest * msg);		// 跨服随机活动请求
	void CheckAllActRewardStatus();											// 检查所有活动的数据

	void OnAddChongZhi(long long chongzhi);									// 充值触发，不保证每一笔单独触发此函数，可能几次充值一次触发
	void OnConsumeGold(long long consume);

	// =============================================
	//	跨服随机-充值排行
	// =============================================
	void OnChongzhiRankCheckReward();										// 充值排行，检查奖励
	void SendChongzhiInfo();


	// =============================================
	//	跨服随机-一元夺宝
	// =============================================
	void OnCloudPurchaseOpera(int opera_type, int param1, int param2);
	bool OnCloudPurchaseRMBBuyTicket(int gold_num);
	void OnAddCloudPurchaseScore(int score, const char* reason);
	bool ConsumeCloudPurchaseTicket(const int ticket_num, const char *reason);
	void OnCheckResetCloudPurchaseData();
	void SendCluodPurchaseConvertInfo();
	void OnRoleBuyCluodPurchaseSucc(ItemID item_id, int buy_count);
	void SendCloudPurchaseBuyInfo();
	void SendCloudPureChaseUserInfo();

	// =============================================
	//	跨服随机-消费排行
	// =============================================
	void OnConsumeRankCheckReward();
	void SendConsumeInfo();
	
private:
	bool HasRewardNotGive(int activity_type, unsigned int last_join_act_time);

	void OnChongzhiRankAddChongzhi(long long chongzhi);
	void OnChongzhiRankReq(int opera_type, int param_1, int param_2, int param_3);

	void OnCloudScoreConvertItem(int seq, int item_count);
	void OnCloudPurchaseConvertAddRecord(ItemID item, int count);
	int GetCloudConvertRecordCount(ItemID item_id);

	void OnConsumeRankAddConsume(long long consume);
	void OnConsumeRankReq(int opera_type, int param_1, int param_2, int param_3);

	Role *m_role;
	ActivityShadow *m_act_shadow;

	RoleCrossRandActivityParam m_activity_info;					
};

#endif // __ROLE_CROSS_RAND_ACTIVITY_HPP__


