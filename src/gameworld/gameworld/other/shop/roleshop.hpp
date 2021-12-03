#ifndef __ROLE_SHOP_HPP__
#define __ROLE_SHOP_HPP__

class Role;

#include "other/shop/shop.hpp"
#include "servercommon/roleshopdef.hpp"

class RoleShop
{
public:
	RoleShop();
	~RoleShop();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const RoleShopParam &param);
	void GetInitParam(RoleShopParam *param);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnRoleLevelUp(int old_level, int role_level);

	unsigned int GetChestShopNextFreeTime(int mode);
	void SetChestShopNextFreeTime(int mode ,unsigned int next_time);

	bool AddChestShopMojing(int score, const char *type);
	int GetChestShopMojing() { return m_param.chest_shop_mojing;}

	bool AddChestShopShengwang(int score, const char *type);
	int GetChestShopShengwang() { return m_param.chest_shop_shengwang;}

	bool AddChestShopGongXun(int score, const char *type);
	int GetChestShopGongXun() { return m_param.chest_shop_gongxun;}

	bool AddChestShopWeiWang(int score, const char *type);
	int GetChestShopWeiWang() { return m_param.chest_shop_weiwang;}

	bool AddChestShopHunjing(int score, const char *type);
	int GetChestShopHunjing() { return m_param.chest_shop_hunjing; }

	bool AddChestShopTreasureCredit(int score, const char *type);
	bool AddChestShopTreasureCredit1(int score, const char * type);
	bool AddChestShopTreasureCredit2(int score, const char * type);
	bool AddChestShopTreasureCredit3(int score, const char * type);
	int GetChestShopTreasureCredit() { return m_param.chest_shop_treasure_credit;}
	int GetChestShopTreasureCredit1() { return m_param.chest_shop_treasure_credit1;}
	int GetChestShopTreasureCredit2() { return m_param.chest_shop_treasure_credit2;}
	int GetChestShopTreasureCredit3() { return m_param.chest_shop_treasure_credit3;}

	bool AddChestShopJingLingCredit(int score, const char *type);
	int GetChestShopJingLingCredit() { return m_param.chest_shop_jingling_credit;}

	bool AddChestShopHappyTreeGrow(int score, const char *type);
	int GetChestShopHappyTreeGrow() { return m_param.chest_shop_happytree_grow;}

	bool AddChestShopMysteriousShopCredit(int score, const char *type);
	int GetChestShopMysteriousShopCredit() { return m_param.chest_shop_mysterious_credit;}

	bool AddChestShopHunyinLingzhi(int blue_lingzhi, int purple_lingzhi, int orange_lingzhi, const char *type);
	int GetChestShopBuleLingzhi() { return m_param.chest_shop_bule_lingzhi; }
	int GetChestShopPurpleLingzhi() { return m_param.chest_shop_purple_linzhi; }
	int GetChestShopOrangeLingzhi() { return m_param.chest_shop_orange_linzhi; }

	bool AddChestShopGuanghui(int score, const char *type);
	int GetChestShopGuanghui() { return m_param.chest_shop_guanghui; }

	bool AddChestShopPreciousBossScore(int score, const char *type);
	int GetChestShopPreciousBossScore() { return m_param.chest_shop_precious_score; }

	bool AddChestShopShenZhouWeaponCredit(int score, const char *type);
	int GetChestShopShenZhouWeaponCredit() { return m_param.chest_shop_shenzhouweapon_score; }

	bool IsFirstEquipBeHited(int shop_type);
	void SetFirstEquipBeHited(int shop_type);

	bool IsFirstJingLingBeHited() { return m_param.first_jingling_is_behited != 0; }
	void SetFirstJingLingBeHited() { m_param.first_jingling_is_behited = 1; }

	bool AddCrossEquipChuanshiScore(int score, const char *type);					// 暂无用，需要时可解除屏蔽
	int GetCrossEquipChuanshiScore() { return m_param.cross_equip_chuanshi_score; }

private:
	Role *m_role;

	RoleShopParam m_param;
};

#endif // __ROLE_SHOP_HPP__

