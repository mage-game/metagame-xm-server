#ifndef __ZERO_GIFT_HPP__
#define __ZERO_GIFT_HPP__

#include "servercommon/struct/zerogiftparam.hpp"

class Role;

class ZeroGift
{
public:
	ZeroGift();
	~ZeroGift();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const ZeroGiftParam &param);
	void GetInitParam(ZeroGiftParam *param);

	void Update(time_t now_second);
	void OnDayChange();

	void ZeroGiftOperate(int operate_type, int param_1, int param_2);

	void SendZeroGiftInfo();

	void SendZeroGiftGodCostumeInfo();					//发送四阶神装协议

	void OnRoleUpLevel(int old_level, int level);

	void OnBuy(int seq);
	void OnBuyGodCostume(int seq);						//购买四阶神装
	void OnFetchRewardGold(int seq);

private:

	void CheckZeroGiftOpen();
	void OnFetchRewardItem(int seq, int day_count);
	void OnFetchGodCostume(int seq, int level_phase);	//领取四阶神装奖励

	Role *m_role;
	ZeroGiftParam m_param;
};

#endif // __ZERO_GIFT_HPP__

