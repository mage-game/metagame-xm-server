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

	void SendZeroGiftGodCostumeInfo();					//�����Ľ���װЭ��

	void OnRoleUpLevel(int old_level, int level);

	void OnBuy(int seq);
	void OnBuyGodCostume(int seq);						//�����Ľ���װ
	void OnFetchRewardGold(int seq);

private:

	void CheckZeroGiftOpen();
	void OnFetchRewardItem(int seq, int day_count);
	void OnFetchGodCostume(int seq, int level_phase);	//��ȡ�Ľ���װ����

	Role *m_role;
	ZeroGiftParam m_param;
};

#endif // __ZERO_GIFT_HPP__

