#include "convertshop.hpp"

#include "servercommon/configcommon.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "gameworld/engineadapter.h"
#include "item/itempool.h"
#include "obj/character/role.h"
#include "gamelog.h"
#include "globalconfig/globalconfig.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "gstr.h"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"

#include "config/logicconfigmanager.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "global/activity/activitymanager.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "equipment/equipmentmanager.h"
#include "other/shop/roleshop.hpp"
#include "other/sublock/sublock.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "other/convertshop/roleconvertshop.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"

ConvertShop::ConvertShop()
{

}

ConvertShop::~ConvertShop()
{

}

bool ConvertShop::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ConvertShop", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load ConvertShop config [%s] Error.cannot find root node.", configname.c_str());
		*err = errinfo;
		return false;
	}

	int iRet = 0;

	{
		PugiXmlNode root_element = RootElement.child("multiple_cost_cfg");
		if (root_element.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->convert_shop_list]", configname.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitMultipleCost(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMultipleCost failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("convert_shop");
		if (root_element.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->convert_shop_list]", configname.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitConverShop(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitConverShop failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	return true;
}

int ConvertShop::InitMultipleCost(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_multiple_cost_id = 1;
	int last_max_times = 0;
	int last_price_multile = 0;
	int index = 0;

	while(!dataElement.empty())
	{
		int multiple_cost_id = 0;
		int times_min = 0;
		int times_max = 0;
		int price_multile = 0;

		if (!PugiGetSubNodeValue(dataElement, "multiple_cost_id", multiple_cost_id) ||  (multiple_cost_id <= 0))
		{
			return -1;
		}

		if (multiple_cost_id != last_multiple_cost_id)
		{
			last_max_times = 0;
			last_price_multile = 0;
			index = 0;
		}
		last_multiple_cost_id = multiple_cost_id;

		if (!PugiGetSubNodeValue(dataElement, "times_min", times_min) ||  (last_max_times != 0 && times_min - last_max_times != 1))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "times_max", times_max) ||  (times_max < times_min))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "price_multile", price_multile) ||  (price_multile <= last_price_multile))
		{
			return -4;
		}

		last_max_times = times_max;

		if (index >= MAX_CONVERT_MULTIPLE_NUM)
		{
			return -21;
		}

		MultipleCostItemList &multi_cost = m_multiplecost_map[multiple_cost_id];
		MultipleCostItem *item = &multi_cost.item_list[index];
		item->multiple_cost_id = multiple_cost_id;
		item->times_min = times_min;
		item->times_max = times_max;

		if (!PugiGetSubNodeValue(dataElement, "price_multile", item->price_multile) || item->price_multile < last_price_multile)
		{
			return -22;
		}
		last_price_multile = item->price_multile;

		++ index;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ConvertShop::InitConverShop(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int limit_convert_shop_item_cfg_count = 0;
	int lifetime_convert_shop_item_num = 0;

	int last_seq_list[SCORE_TO_ITEM_TYPE_MAX]; 
	for (int i = 0; i < SCORE_TO_ITEM_TYPE_MAX; ++ i)
	{
		last_seq_list[i] = -1;
	}

	while (!dataElement.empty())
	{
		int type = 0;
		if (!PugiGetSubNodeValue(dataElement, "conver_type", type) || type <= SCORE_TO_ITEM_TYPE_INVALID || type >= SCORE_TO_ITEM_TYPE_MAX)
		{
			return -1;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq != last_seq_list[type] + 1)
		{
			return -2;
		}
		last_seq_list[type] = seq;

		if (m_convert_shop_item_list[type].count >= Protocol::MAX_CONVERT_SCORE_TO_ITEM_NUM)
		{
			return -3;
		}

		ConvertShopItem *shop_item = &m_convert_shop_item_list[type].item_list[m_convert_shop_item_list[type].count];

		if (!PugiGetSubNodeValue(dataElement, "item_id", shop_item->item_id) || shop_item->item_id <= 0)
		{
			return -4;
		}

		if (NULL == ITEMPOOL->GetItem(shop_item->item_id))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_bind", shop_item->is_bind) || (0 != shop_item->is_bind && 1 != shop_item->is_bind))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_id", shop_item->need_stuff_id) || (shop_item->need_stuff_id != 0 && NULL == ITEMPOOL->GetItem(shop_item->need_stuff_id)))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "price_type", shop_item->price_type) || shop_item->price_type <= 0 || shop_item->price_type >= SCORE_TO_ITEM_PRICE_TYPE_MAX)
		{
			return -80;
		}

		if (!PugiGetSubNodeValue(dataElement, "price", shop_item->price) || shop_item->price <= 0)
		{
			return -8;
		}

		const ItemBase *itemimp = ITEMPOOL->GetItem(shop_item->item_id);
		if (NULL == itemimp)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_count", shop_item->need_stuff_count) || (shop_item->need_stuff_id != 0 && shop_item->need_stuff_count <= 0))
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "require_type", shop_item->require_type) || shop_item->require_type < 0 || shop_item->require_type >= CONVERT_REQUIRE_TYPE_MAX)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "require_value", shop_item->require_value_min) || shop_item->require_value_min < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "require_value_max", shop_item->require_value_max) || shop_item->require_value_max < 0)
		{
			return -131;
		}

		if (!PugiGetSubNodeValue(dataElement, "limit_convert_count", shop_item->limit_convert_count) || shop_item->limit_convert_count < 0 || shop_item->limit_convert_count > 100)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "multiple_cost_id", shop_item->multiple_cost_id) || (shop_item->multiple_cost_id != 0 && m_multiplecost_map.end() == m_multiplecost_map.find(shop_item->multiple_cost_id)))
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "lifetime_convert_count", shop_item->lifetime_convert_count) || shop_item->lifetime_convert_count < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_world_notice", shop_item->is_world_notice))
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_guild_notice", shop_item->is_guild_notice))
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(dataElement, "from_open_game_day", shop_item->from_open_game_day) || shop_item->from_open_game_day <= 0)
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(dataElement, "to_open_game_day", shop_item->to_open_game_day) || shop_item->to_open_game_day <= 0)
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(dataElement, "limit_conver_time", shop_item->limit_conver_time) || shop_item->limit_conver_time < 0)
		{
			return -21;
		}

		if (shop_item->from_open_game_day > shop_item->to_open_game_day)
		{
			return -201;
		}

		if (shop_item->lifetime_convert_count != 0)
		{
			lifetime_convert_shop_item_num++;

			if (lifetime_convert_shop_item_num > MAX_CONVERT_RECORD_ITEM_COUNT)
			{
				return -37;
			}
		}

		if (shop_item->limit_convert_count)
		{ 
			++ limit_convert_shop_item_cfg_count;
		}

		++ m_convert_shop_item_list[type].count;
		dataElement = dataElement.next_sibling();
	}

	if (limit_convert_shop_item_cfg_count > MAX_CONVERT_RECORD_ITEM_COUNT)
	{
		return -160;
	}

	return 0;
}

// 兑换
void ConvertShop::ConvertScoreToItem(Role *role, Protocol::CSScoreToItemConvert &stic)
{
	if (NULL == role || role->GetSublock()->CheckHasLock()) 
	{
		return;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (stic.scoretoitem_type <= SCORE_TO_ITEM_TYPE_INVALID || stic.scoretoitem_type >= SCORE_TO_ITEM_TYPE_MAX) 
	{
		return;
	}

	if (!this->CheckConvertReqValid(role, stic)) 
	{
		return;
	}

	if (stic.scoretoitem_type == SCORE_TO_ITEM_TYPE_HAPPYTREE && !ActivityManager::Instance().IsActivityOpen(ACTIVITY_TYPE_HAPPYTREE_GROW_EXCHANGE))
	{
		role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
		return;
	}

	if (stic.scoretoitem_type == SCORE_TO_ITEM_TYPE_TREASURE && !RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TREASURE_CONVERT))
	{
		role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
		return;
	}

	// 计算消耗
	int price_type;
	int total_price = 0;
	int total_need_stuff_count = 0;

	{
		if (stic.index < 0 || stic.index >= m_convert_shop_item_list[stic.scoretoitem_type].count)
		{
			return;
		}

		if (!this->CheckConvertRequire(role, stic.scoretoitem_type, stic.index, stic.num))
		{
			return;
		}
		
		ConvertShopItem *item = &m_convert_shop_item_list[stic.scoretoitem_type].item_list[stic.index];
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
		if (opengame_day < item->from_open_game_day || opengame_day > item->to_open_game_day)
		{
			return;		//不在开服天数区间内不能兑换
		}
		const ItemBase *itembase = ITEMPOOL->GetItem(item->item_id);

		if (NULL == itembase) 
		{
			return;
		}
		if (stic.num <= 0 || stic.num > itembase->GetPileLimit())
		{
			return;
		}
		if (!ItemBase::CanMerge(item->item_id) && 1 != stic.num) 
		{
			return;
		}

		price_type = item->price_type;
		int convert_record_count = role->GetConvertRecord()->GetRecordCount(stic.scoretoitem_type, stic.index);
		int mulitplecost = this->GetPriceMultipleCost(item->multiple_cost_id, convert_record_count + stic.num);
		total_price += item->price * mulitplecost * stic.num;
		
		if (item->need_stuff_id > 0 && item->need_stuff_count > 0)
		{
			total_need_stuff_count += item->need_stuff_count * stic.num;
		}
	}

	if (total_price <= 0 || price_type <= 0 || price_type >= SCORE_TO_ITEM_PRICE_TYPE_MAX)
	{
		return;
	}

	ConvertShopItem *item = &m_convert_shop_item_list[stic.scoretoitem_type].item_list[stic.index];

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();
	
	//检查材料
	{
		if (SCORE_TO_ITEM_PRICE_TYPE_MOJING == price_type && role->GetRoleShop()->GetChestShopMojing() < total_price)
		{
			role->NoticeNum(errornum::EN_MOJING_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_SHENGWANG == price_type && role->GetRoleShop()->GetChestShopShengwang() < total_price)
		{
			role->NoticeNum(errornum::EN_SHENGWANG_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_GONGXUN == price_type && role->GetRoleShop()->GetChestShopGongXun() < total_price)
		{
			role->NoticeNum(errornum::EN_GONGXUN_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_WEIWANG == price_type && role->GetRoleShop()->GetChestShopWeiWang() < total_price)
		{
			role->NoticeNum(errornum::EN_WEIWANG_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT == price_type && role->GetRoleShop()->GetChestShopTreasureCredit() < total_price)
		{
			role->NoticeNum(errornum::EN_TREASURE_HUNT_CREDIT_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT1 == price_type && role->GetRoleShop()->GetChestShopTreasureCredit1() < total_price)
		{
			role->NoticeNum(errornum::EN_TREASURE_HUNT_CREDIT_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT2 == price_type && role->GetRoleShop()->GetChestShopTreasureCredit2() < total_price)
		{
			role->NoticeNum(errornum::EN_TREASURE_HUNT_CREDIT_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT3 == price_type && role->GetRoleShop()->GetChestShopTreasureCredit3() < total_price)
		{
			role->NoticeNum(errornum::EN_TREASURE_HUNT_CREDIT_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_JINGLING_CREDIT == price_type && role->GetRoleShop()->GetChestShopJingLingCredit() < total_price)
		{
			role->NoticeNum(errornum::EN_JINGLING_CREDIT_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_HAPPY_TREE == price_type && role->GetRoleShop()->GetChestShopHappyTreeGrow() < total_price)
		{
			role->NoticeNum(errornum::EN_HAPPYTREE_GROW_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_CROSS_HONOR == price_type && role->GetCrossHonor() < total_price)
		{
			role->NoticeNum(errornum::EN_CROSS_HONOR_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_GUANGHUI == price_type && role->GetRoleShop()->GetChestShopGuanghui() < total_price)
		{
			role->NoticeNum(errornum::EN_GUANGHUI_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_MYSTERIOUS_SHOP == price_type && role->GetRoleShop()->GetChestShopMysteriousShopCredit() < total_price)
		{
			role->NoticeNum(errornum::EN_MYSTERIOUS_SHOP_CREDIT_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_BLUE_LINGZHI == price_type && role->GetRoleShop()->GetChestShopBuleLingzhi() < total_price)
		{
			role->NoticeNum(errornum::EN_CONVERTSHOP_LINGZHI_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_PURPLE_LINGZHI == price_type && role->GetRoleShop()->GetChestShopPurpleLingzhi() < total_price)
		{
			role->NoticeNum(errornum::EN_CONVERTSHOP_LINGZHI_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_ORANGE_LINGZHI == price_type && role->GetRoleShop()->GetChestShopOrangeLingzhi() < total_price)
		{
			role->NoticeNum(errornum::EN_CONVERTSHOP_LINGZHI_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_PRECIOUS_BOSS_SCORE == price_type && role->GetRoleShop()->GetChestShopPreciousBossScore() < total_price)
		{
			role->NoticeNum(errornum::EN_PRECIOUS_BOSS_SCORE_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_SHENZHOUWEAPON_SCORE == price_type && role->GetRoleShop()->GetChestShopShenZhouWeaponCredit() < total_price)
		{
			role->NoticeNum(errornum::EN_SHENZHOUWENPON_SCORE_NOT_ENOUGH);
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_ZHUANZHI_STONE_SCORE == price_type && role->GetZhuanzhiEquip()->GetStoneScore() < total_price)
		{
			gstr::SendError(role, "zhuanzhi_stone_score_not_enough");
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_HUNJING == price_type && role->GetRoleShop()->GetChestShopHunjing() < total_price)
		{
			gstr::SendError(role, "en_score_not_enough");
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_GONGXIAN == price_type && role->GetGuildGongxian() < total_price)
		{
			gstr::SendError(role, "guild_gongxian_not_enough");
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_JUST_USE_ITEM == price_type && !(role->GetKnapsack()->GetItemNum(item->need_stuff_id) >= total_need_stuff_count))
		{
			role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}

		if (item->need_stuff_id > 0 && total_need_stuff_count > 0)
		{
			short stuff_count = 0;
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

			stuff_list[stuff_count].item_id = item->need_stuff_id;
			stuff_list[stuff_count].num = total_need_stuff_count;
			stuff_list[stuff_count].buyable = false;
			++ stuff_count;

			if (!ItemExtern::GetConsumeListByOrder(role, false, stuff_count, stuff_list, &consume_list))
			{
				role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
				return;
			}
		}
	}

	// 消耗
	{
		if (SCORE_TO_ITEM_PRICE_TYPE_MOJING == price_type && !role->GetRoleShop()->AddChestShopMojing((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_SHENGWANG == price_type && !role->GetRoleShop()->AddChestShopShengwang((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_GONGXUN == price_type && !role->GetRoleShop()->AddChestShopGongXun((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_WEIWANG == price_type && !role->GetRoleShop()->AddChestShopWeiWang((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT == price_type && !role->GetRoleShop()->AddChestShopTreasureCredit((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT1 == price_type && !role->GetRoleShop()->AddChestShopTreasureCredit1((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT2 == price_type && !role->GetRoleShop()->AddChestShopTreasureCredit2((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT3 == price_type && !role->GetRoleShop()->AddChestShopTreasureCredit3((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_JINGLING_CREDIT == price_type && !role->GetRoleShop()->AddChestShopJingLingCredit((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_HAPPY_TREE == price_type && !role->GetRoleShop()->AddChestShopHappyTreeGrow((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_CROSS_HONOR == price_type && !role->AddCrossHonor((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_GUANGHUI == price_type && !role->GetRoleShop()->AddChestShopGuanghui((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_MYSTERIOUS_SHOP == price_type && !role->GetRoleShop()->AddChestShopMysteriousShopCredit((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_BLUE_LINGZHI == price_type && !role->GetRoleShop()->AddChestShopHunyinLingzhi((-1 * total_price), 0, 0, "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_PURPLE_LINGZHI == price_type && !role->GetRoleShop()->AddChestShopHunyinLingzhi(0, (-1 * total_price), 0, "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_ORANGE_LINGZHI == price_type && !role->GetRoleShop()->AddChestShopHunyinLingzhi(0, 0, (-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_PRECIOUS_BOSS_SCORE == price_type && !role->GetRoleShop()->AddChestShopPreciousBossScore((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_SHENZHOUWEAPON_SCORE == price_type && !role->GetRoleShop()->AddChestShopShenZhouWeaponCredit((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_ZHUANZHI_STONE_SCORE == price_type && !role->GetZhuanzhiEquip()->OnComsumeStoneScore(total_price))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_HUNJING == price_type && !role->GetRoleShop()->AddChestShopHunjing((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (SCORE_TO_ITEM_PRICE_TYPE_GONGXIAN == price_type && !role->AddGuildGongxian((-1 * total_price), "ConvertItem"))
		{
			return;
		}

		if (consume_list.count > 0 && !knapsack->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "ConvertShop")) 
		{
			return;
		}
	}

	if (item->limit_convert_count > 0 || item->multiple_cost_id > 0 || item->lifetime_convert_count > 0)
	{
		role->GetConvertRecord()->AddRecordCount(stic.scoretoitem_type, stic.index, stic.num, 0 != item->lifetime_convert_count);
	}

	// 给予兑换物
	{
		if (stic.index < 0 || stic.index >= m_convert_shop_item_list[stic.scoretoitem_type].count)
		{
			return;
		}

		ItemConfigData item_config_data;
		item_config_data.item_id = item->item_id;
		item_config_data.num = stic.num;
		item_config_data.is_bind = (0 != item->is_bind ? true : false);

		int grid_index = -1;
		if (!knapsack->Put(item_config_data, PUT_REASON_CONVERT_SHOP, &grid_index))
		{
			gamelog::g_log_convertshop.printf(LL_ERROR, "[ConvertShop::Convert Fail][user[%d %s] conver_type[%d], index[%d], convert_item[id:%d, num:%d], total_cost[%d]",
				UidToInt(role->GetUserId()), role->GetName(), stic.scoretoitem_type, stic.index, item_config_data.item_id, item_config_data.num, total_price);
		}

		//为装备获得额外属性
		{
			bool is_addxianpin = false, is_addlucky = false;
			if (SCORE_TO_ITEM_TYPE_TREASURE_HUNT == stic.scoretoitem_type || SCORE_TO_ITEM_TYPE_EQUIP == stic.scoretoitem_type || SCORE_TO_ITEM_TYPE_CS_EQUIP1 == stic.scoretoitem_type )  // 随机获得属性
			{	
				is_addxianpin = true;
			}

			if (SCORE_TO_ITEM_TYPE_CS_EQUIP1 == stic.scoretoitem_type)
			{
				is_addlucky = true;
			}

			if(grid_index >= 0)
			{
				ItemGridData item_grid_data;
				role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data);
				bool is_success = false;

				if (is_addxianpin && EquipmentManager::GetFixSpecialAttr(&item_grid_data, Equipment::EQUIPMENT_FIX_GETWAY_CHEST_SHOP, 0))
				{
					is_success = true;
				}

				if (is_addlucky && GLOBALCONFIG->GetEquipConfig().GetLucky(&item_grid_data))
				{
					is_success = true;
				}

				if (is_success)
				{
					role->GetKnapsack()->SetItemDirty(grid_index);
				}
			}
		}

		role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_CONVERT_SHOP, stic.num, price_type);
		role->GetRoleGoal()->CheckCond(COND_TYPE_CONVERT_SHOP, stic.num, price_type);
		
		{
			int log_money_type = 0;
			if (SCORE_TO_ITEM_PRICE_TYPE_MOJING == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_MOJING;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_SHENGWANG == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_SHENGWANG;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_GONGXUN == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_GONGXUN;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_WEIWANG == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_WEIWANG;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_TREASURE_HUNT_CREDIT;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_JINGLING_CREDIT == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_JINGLING_CREDIT;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_HAPPY_TREE == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_HAPPYTREE_GROW;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_MYSTERIOUS_SHOP == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_MYSTERIOUS_SHOP_CREDIT;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_GUANGHUI == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_GUANGHUI;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_BLUE_LINGZHI == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_BLUE_LINGZHI;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_PURPLE_LINGZHI == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_PURPLE_LINGZHI;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_ORANGE_LINGZHI == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_ORANGE_LINGZHI;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_PRECIOUS_BOSS_SCORE == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_PRECIOUS_BOSS;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_ZHUANZHI_STONE_SCORE == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_ZHUANZHI_STONE;
			}

			else if (SCORE_TO_ITEM_PRICE_TYPE_HUNJING == price_type)
			{
				log_money_type = LOG_MONEY_TYPE_HUNJING;
			}

			ROLE_LOG_QUICK10(LOG_TYPE_GET_ITEM, role, item_config_data.item_id, LOG_GET_ITEM_WAY_DUIHUAN, NULL, NULL, log_money_type, total_price, role->GetLevel(), role->GetRoleActivity()->GetHistoryChongZhi());
		}

		gamelog::g_log_convertshop.printf(LL_INFO, "[ConvertShop::Convert Succ][user[%d %s] conver_type[%d], index[%d], convert_item[id:%d, num:%d], total_cost[%d]",
			UidToInt(role->GetUserId()), role->GetName(), stic.scoretoitem_type, stic.index, item_config_data.item_id, item_config_data.num, total_price);

		role->NoticeNum(noticenum::NT_CONVERTSHOP_CONVERT_SUCC);

		if (0 != item->is_world_notice)
		{
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_convershop_world_notice,
				role->GetUID(), role->GetName(), (int)role->GetCamp(), item->item_id, item->price_type);

			if (len > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
			}
		}

		if (0 != item->is_guild_notice)
		{ 
			unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			CommonDataParam *common_data = role->GetCommonDataParam();
			if (NULL != common_data && now_sec > common_data->covertshop_guild_last_notice_time + 60)
			{
				common_data->covertshop_guild_last_notice_time = now_sec;

				Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
				if (NULL != guild)
				{
					int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_convershop_guild_notice,
						role->GetUID(), role->GetName(), (int)role->GetCamp(), item->item_id, item->price_type);

					if (len > 0)
					{
						//guild->GuildSystemMsg(len, gamestring::GAMESTRING_BUF);
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
					}
				}
			}
		}
	}
}

bool ConvertShop::CheckConvertRequire(Role *role, int convert_type, int conver_index, int convert_num)
{
	if (NULL == role || convert_type <= SCORE_TO_ITEM_TYPE_INVALID || convert_type >= SCORE_TO_ITEM_TYPE_MAX) 
	{
		return false;
	}

	if (conver_index < 0 || conver_index >= m_convert_shop_item_list[convert_type].count || conver_index >= Protocol::MAX_CONVERT_SCORE_TO_ITEM_NUM) 
	{
		return false;
	}

	const ConvertShopItem &convershop_item = m_convert_shop_item_list[convert_type].item_list[conver_index];

	// 限时
	if (convershop_item.limit_conver_time > 0)
	{
		unsigned int begin_timestamp = role->GetConvertRecord()->GetRecordLimitBeginTimeStamp(convert_type, conver_index);
		unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		
		if (begin_timestamp > 0 && 
			now_sec >= begin_timestamp + convershop_item.limit_conver_time)
		{
			role->NoticeNum(errornum::EN_CONVERTSHOP_LIMIT_TIME_INVALID);
			return false;
		}
	}

	switch(convershop_item.require_type)
	{
	case CONVERT_REQUIRE_TYPE_SUIT_LEVEL:
		{
			if (role->GetLevel() < convershop_item.require_value_min || role->GetLevel() > convershop_item.require_value_max)
			{
				role->NoticeNum(errornum::NT_CONVERTSHOP_LEVEL_LIMIT);
				return false;
			}
		}
		break;
	}

	if (convershop_item.limit_convert_count > 0)
	{
		int convert_record_count = role->GetConvertRecord()->GetRecordCount(convert_type, conver_index);
		if (convert_record_count + convert_num > convershop_item.limit_convert_count)
		{
			role->NoticeNum(errornum::NT_CONVERTSHOP_CONVERT_COUNT_LIMIT);
			return false;
		}
	}

	if (convershop_item.lifetime_convert_count > 0)
	{
		int lifetime_convert_record_count = role->GetConvertRecord()->GetLifetimeRecordCount(convert_type, conver_index);
		if (lifetime_convert_record_count + convert_num > convershop_item.lifetime_convert_count)
		{
			role->NoticeNum(errornum::NT_CONVERTSHOP_LIFETIME_CONVERT_COUNT_LIMIT);
			return false;
		}
	}

	return true;
}

bool ConvertShop::CheckConvertReqValid(Role *role, Protocol::CSScoreToItemConvert &stic)
{
	return true;
}

// 获取翻倍数量
int ConvertShop::GetPriceMultipleCost(int multiplecost_id, int times)
{
	std::map<int, MultipleCostItemList>::iterator iter = m_multiplecost_map.find(multiplecost_id);
	if (m_multiplecost_map.end() == iter) 
	{
		return 1;
	}
	
	for (int i = 0; i < MAX_CONVERT_MULTIPLE_NUM; ++ i)
	{
		MultipleCostItem &item = iter->second.item_list[i];
		if (times >= item.times_min && times <= item.times_max)
		{
			return item.price_multile;
		}
	}

	return 1;
}

void ConvertShop::CheckLimitTimeConvertItem(Role *role, int level)
{
	if (nullptr == role) return;

	bool send_to_client = false;

	// 遍历检查是否有开始计时的限时兑换道具
	for (int type = SCORE_TO_ITEM_TYPE_INVALID; type < SCORE_TO_ITEM_TYPE_MAX; ++type)
	{
		const ConvertShopItemList &shop_item_list = m_convert_shop_item_list[type];

		for (int i = 0; i < shop_item_list.count && i < Protocol::MAX_CONVERT_SCORE_TO_ITEM_NUM; i++)
		{
			const ConvertShopItem &item_list = shop_item_list.item_list[i];
			if (item_list.limit_conver_time > 0 && CONVERT_REQUIRE_TYPE_SUIT_LEVEL == item_list.require_type && level >= item_list.require_value_min)
			{
				if (role->GetConvertRecord()->AddRecordLimitBeginTimeStamp(type, i))
				{
					send_to_client = true;
				}
			}
		}
	}

	if (send_to_client)
	{
		role->GetConvertRecord()->SendInfo();
	}
}

