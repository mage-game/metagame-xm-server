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

	void MysteriousshopinmallOp(int type, int param1);		// 操作

	//每日特惠
	void RefreshDiscountShopInfo();
	void SendDiscountShopInfo();
	void OnDiscountShopBuy(int seq, int num);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

private:
	void OnBuyMoneyShopItemReq(int seq);					// 购买
	void OnRefreshMoneyShopItemReq(bool is_fresh_all);		// 刷新

	void CheckRefreshTime();								// 检查是否到时刷新
	int GetIndexBySeq(int seq);								// 暂无用
	void RefreshMoneyShopItem(bool is_all);					// 刷新神秘商店物品
	void SendMoneyShopItemSeq();							// 发送商店信息

	MysteriousShopInMallParam m_param;

	DisCountShopParam m_discountshop_param;

	Role *m_role;
};

#endif // __MYSTRIOUS_SHOP_IN_MALL_HPP__

