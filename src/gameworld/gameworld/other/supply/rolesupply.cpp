#include "rolesupply.hpp"

#include "supplyconfig.hpp"

#include "obj/character/role.h"
#include "protocal/msgother.h"
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "globalconfig/globalconfig.h"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "item/knapsack.h"
#include "other/daycounter/daycounter.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include <limits.h>

RoleSupply::RoleSupply()
	: m_role(NULL)
{

}

RoleSupply::~RoleSupply()
{

}

void RoleSupply::Init(Role *role, const RoleSupplyParam &param)
{
	m_role = role;
	memcpy(m_supply_list, param.supply_list, sizeof(m_supply_list));
}

void RoleSupply::GetInitParam(RoleSupplyParam *param)
{
	memcpy(param->supply_list, m_supply_list, sizeof(m_supply_list));
}

void RoleSupply::Update(time_t now_second)
{
	bool need_send = false;

	for (int i = 0; i < SUPPLY_TYPE_MAX; i++)
	{
		if (!m_role->GetRoleStatusManager()->CanSupplyEffect(i))
		{
			continue;
		}

		int recover_interval_s = LOGIC_CONFIG->GetSupplyConfig().GetSupplyRecoverInterval(i);
		if (now_second >= m_supply_list[i].last_supply_time + recover_interval_s && m_supply_list[i].supply_left_value > 0)
		{
			switch (i)
			{
			case SUPPLY_TYPE_ROLE_HP:
				{
					Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
					Attribute hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
					Attribute hp_per = 0;
					if (max_hp > 0) hp_per = static_cast<Attribute>(hp * 100.0 / max_hp);

					if (hp_per < m_supply_list[i].supply_range_per && m_role->IsAlive())
					{
						// 策划说不要以前按等级回固定血的模式，改成按最大血量万分比不区分战斗非战斗状态
// 						int extra_recover_hp = 0;
// 						if (m_role->GetRoleStatusManager()->IsHpSupplyRewardExtra())
// 						{
// 							extra_recover_hp = static_cast<int>(max_hp * 0.1);
// 						}
// 
// 						int recover_hp = LOGIC_CONFIG->GetSupplyConfig().GetSupplyRecoverValue(i, m_role->GetLevel()) + extra_recover_hp;
// 						if (!m_role->GetRoleStatusManager()->IsInFightStatus())
// 						{
// 							recover_hp *= LOGIC_CONFIG->GetSupplyConfig().GetNotFightRate();
// 						}
						
						// 星座秘籍回血加成
						double other_add_per = m_role->GetChineseZodiac()->GetHuiXuePercent() * ROLE_ATTR_PER;

						double recover_hp_per = LOGIC_CONFIG->GetSupplyConfig().GetRecoverPer() + LOGIC_CONFIG->GetSupplyConfig().GetRecoverPer() * other_add_per;

						Attribute recover_hp = static_cast<Attribute>(max_hp * (recover_hp_per * ROLE_ATTR_PER));

						if (recover_hp > max_hp - hp)
						{
							recover_hp = max_hp - hp;
						}

						if (recover_hp > m_supply_list[i].supply_left_value) 
						{
							recover_hp = m_supply_list[i].supply_left_value;
						}

						m_role->ChangeHp(&recover_hp, true);

						m_supply_list[i].supply_left_value -= recover_hp;
						m_supply_list[i].last_supply_time = static_cast<unsigned int>(now_second);
						
						need_send = true;
					}
				}
				break;

			default:
				{
					m_supply_list[i].last_supply_time = static_cast<unsigned int>(now_second);
				}
				break;
			}
		}
	}

	if (need_send)
	{
		this->SendSupplyInfo();
	}
}

void RoleSupply::OnBuySupplyItem(int supply_type, int index, bool is_use_no_bind_gold)
{
	if (supply_type < 0 || supply_type >= SUPPLY_TYPE_MAX || 
		index < 0 || index >= MAX_SUPPLY_ITEM_COUNT)
	{
		return;
	}

	const SupplyItem *supply_item = LOGIC_CONFIG->GetSupplyConfig().GetSupplyItem(supply_type, index);
	if (NULL == supply_item || supply_item->price <= 0 || supply_item->value <= 0)
	{
		return;
	}

	int need_gold = 0; int need_coin = 0;

	if (supply_item->is_use_gold)
	{
		if (is_use_no_bind_gold)
		{
			if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(supply_item->price))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
		}
		else
		{
			if (!m_role->GetKnapsack()->GetMoney()->GoldBindMoreThan(supply_item->price))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
		}

		need_gold = supply_item->price;
	}
	else
	{
		if (!m_role->GetKnapsack()->GetMoney()->AllCoinMoreThan(supply_item->price))
		{
			m_role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
			return;
		}

		need_coin = supply_item->price;
	}

	switch(supply_type)
	{
	case SUPPLY_TYPE_ROLE_TILI:
		{
			if (m_role->GetEnergy() >= GLOBALCONFIG->GetMaxEnergy())
			{
				m_role->NoticeNum(errornum::EN_ENERGY_IS_FULL);
				return;
			}

			if (need_coin > 0)
			{
				if (!m_role->GetKnapsack()->GetMoney()->UseAllCoin(need_coin, "BuySupplyItemCost"))
					return;
			}

			if (need_gold > 0)
			{
				if (is_use_no_bind_gold)
				{
					if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "BuySupplyTiliCost"))
						return;
				}
				else
				{
					if (!m_role->GetKnapsack()->GetMoney()->UseGoldBind(need_gold, "BuySupplyTiliCost"))
						return;
				}
			}

			m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_BUY_ENERGY_TIMES);

			m_role->AddEnergy(supply_item->value, "SupplyBuyEnergy");

			m_role->NoticeNum(noticenum::NT_BUY_ENERGY_SUCC);
		}
		break;

	default:
		{
			if (supply_item->value > INT_MAX - m_supply_list[supply_type].supply_left_value)
			{
				m_role->NoticeNum(errornum::EN_SUPPLY_MAX_LIMIT);
				return;
			}

			if (need_coin > 0)
			{
				if (!m_role->GetKnapsack()->GetMoney()->UseAllCoin(need_coin, "BuySupplyHpCost"))
					return;
			}

			if (need_gold > 0)
			{
				if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "BuySupplyItemCost"))
					return;
			}

			m_supply_list[supply_type].supply_left_value += supply_item->value;

			m_role->NoticeNum(noticenum::NT_BUY_ROLE_HP_SUCC);
		}
		break;
	}

	this->SendSupplyInfo();
}

void RoleSupply::OnSetSupplyRecoverRangePer(int supply_type, int supply_range_per)
{
	if (supply_type < 0 || supply_type >= SUPPLY_TYPE_MAX || 
		supply_range_per < 0 || supply_range_per > 100)
	{
		return;
	}

	m_supply_list[supply_type].supply_range_per = static_cast<short>(supply_range_per);

	this->SendSupplyInfo();
}

void RoleSupply::SendSupplyInfo()
{
	static Protocol::SCSupplyInfo si;

	for (int i = 0; i < SUPPLY_TYPE_MAX; i++)
	{
		si.supply_info_list[i].supply_left_value = m_supply_list[i].supply_left_value;
		si.supply_info_list[i].supply_range_per = m_supply_list[i].supply_range_per;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&si, sizeof(si));
}

bool RoleSupply::OnUseXuebao(int supply_type, int recover_value)
{
	if (supply_type < 0 || supply_type >= SUPPLY_TYPE_MAX)
	{
		return false;
	}

	if (recover_value > INT_MAX - m_supply_list[supply_type].supply_left_value)
	{
		m_role->NoticeNum(errornum::EN_SUPPLY_MAX_LIMIT);
		return false;
	}

	m_supply_list[supply_type].supply_left_value += recover_value;

	this->SendSupplyInfo();

	return true;
}