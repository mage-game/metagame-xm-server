#ifndef __MYSTRIOUS_SHOP_IN_MALL_HPP__
#define __MYSTRIOUS_SHOP_IN_MALL_HPP__

#include "gamedef.h"
#include "protocal/msgshop.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/maildef.hpp"
#include "servercommon/mysteriousshopinmalldef.hpp"
#include "other/shop/mysteriousshopinmallconfig.hpp"

#include <vector>

class Role;

class MysteriousShopInMall
{
public:
	MysteriousShopInMall();
	~MysteriousShopInMall();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const MysteriousShopInMallParam &param, const DisCountShopParam &param_1);
	void GetInitParam(MysteriousShopInMallParam *param, DisCountShopParam *param_1);

	void Update(unsigned int now_second);
	void OnRoleLogin();

	void MysteriousshopinmallOp(int type, int param1);		// ����

	//ÿ���ػ�
	void RefreshDiscountShopInfo();
	void SendDiscountShopInfo();
	void OnDiscountShopBuy(int seq, int num);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

private:
	void OnBuyMoneyShopItemReq(int seq);					// ����
	void OnRefreshMoneyShopItemReq(bool is_fresh_all);		// ˢ��

	void CheckRefreshTime();								// ����Ƿ�ʱˢ��
	int GetIndexBySeq(int seq);								// ������
	void RefreshMoneyShopItem(bool is_all);					// ˢ�������̵���Ʒ
	void SendMoneyShopItemSeq();							// �����̵���Ϣ

	MysteriousShopInMallParam m_param;

	DisCountShopParam m_discountshop_param;

	Role *m_role;
};

#endif // __MYSTRIOUS_SHOP_IN_MALL_HPP__

