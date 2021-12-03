#include "chestshop.hpp"

#include "servercommon/configcommon.h"

#include "item/itemgriddata.h"
#include "item/itempool.h"
#include "item/itemextern.hpp"

#include "engineadapter.h"
#include "internalcomm.h"

#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "globalconfig/globalconfig.h"
#include "servercommon/string/gameworldstr.h"
#include "scene/activityshadow/activityshadow.hpp"

#include "world.h"
#include "gamelog.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/activitydef.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "config/joinlimitconfig.hpp"
#include "other/jingling/jinglingconfig.hpp"
#include "config/zhuanzhiconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "equipment/equipmentmanager.h"
#include "global/worldstatus/worldstatus.hpp"
#include "other/chestshop/rolechestshop.hpp"
#include "other/shop/roleshop.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/monitor/monitor.hpp"
#include "other/sublock/sublock.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/achieve/title/titleconfig.hpp"
#include "other/lotterycount/lotterycount.hpp"
#include "global/cross/crossmanager.h"
#include "other/cross/rolecross.h"

#include<algorithm>

ChestShop::ChestShop() :m_rare_item_id(0), m_free_cd_s_1(0), m_free_cd1_s_1(0), m_free_cd2_s_1(0), m_free_cd_s_30(0), m_free_1_times_per_day(0), m_add_score(0), m_add_score1(0), m_add_score2(0) , m_add_score3(0), m_free_jingling_cd_s_1(0),m_free_1_times_jingling_per_day(0),
						m_add_jingling_score(0),m_first_buy_hit_equipid_1(0), m_first_buy_hit_equipid_2(0), m_first_buy_hit_equipid_3(0), m_first_buy_hit_equipid_4(0), m_first_buy_2_hit_equipid(0), m_first_buy_3_hit_equipid(0), m_first_buy_4_hit_equipid(0),
						m_first_buy_hit_jinglingid_1(0), m_first_buy_hit_jinglingid_2(0), m_first_buy_hit_jinglingid_3(0), m_first_buy_hit_jinglingid_4(0),
						m_equip_use_itemid(0), m_equip_10_use_itemid(0), m_equip_30_use_itemid(0), m_equip1_use_itemid(0), m_equip1_10_use_itemid(0), m_equip1_30_use_itemid(0), m_equip2_use_itemid(0), m_equip2_10_use_itemid(0), m_equip2_30_use_itemid(0), m_equip3_use_itemid(0), m_equip3_10_use_itemid(0), m_equip3_30_use_itemid(0), m_jingling_use_itemid(0), m_jingling_10_use_itemid(0), m_one_rare_shop_consume_gold(0),
						m_ten__rare_shop_consume_gold(0), m_rare_shop_total_weight(0)
{
	memset(m_replace_item_id, 0, sizeof(m_replace_item_id));
	memset(m_gold_list, 0, sizeof(m_gold_list));
}

ChestShop::~ChestShop()
{

}

bool ChestShop::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ChestShop::Init", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode ChujiItemListElement = RootElement.child("item_list1");
		if (ChujiItemListElement.empty())
		{
			*err = configname + ": no [item_list1].";
			return false;
		}

		iRet = this->InitChestShopItemList(ChujiItemListElement, CHEST_SHOP_DAY_0);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitChestShopItemList CHEST_SHOP_DAY_0 failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode ChujiItemListElement = RootElement.child("item_list2");
		if (ChujiItemListElement.empty())
		{
			*err = configname + ": no [item_list2].";
			return false;
		}

		iRet = this->InitChestShopItemList(ChujiItemListElement, CHEST_SHOP_DAY_8);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitChestShopItemList CHEST_SHOP_DAY_8 failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode ChujiItemListElement = RootElement.child("item_list3");
		if (ChujiItemListElement.empty())
		{
			*err = configname + ": no [item_list3].";
			return false;
		}

		iRet = this->InitChestShopItemList(ChujiItemListElement, CHEST_SHOP_DAY_15);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitChestShopItemList CHEST_SHOP_DAY_15 failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode ChujiItemListElement = RootElement.child("item_list4");
		if (ChujiItemListElement.empty())
		{
			*err = configname + ": no [item_list4].";
			return false;
		}

		iRet = this->InitChestShopItemList(ChujiItemListElement, CHEST_SHOP_DAY_30);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitChestShopItemList CHEST_SHOP_DAY_30 failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 其他配置
	{
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOther(other_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitOther failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode rare_chestshop_element = RootElement.child("rare_chestshop");
		if (rare_chestshop_element.empty())
		{
			*err = configname + ": no [rare_chestshop].";
			return false;
		}

		iRet = this->InitRareChestShop(rare_chestshop_element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s: InitRareChestShop failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int ChestShop::GetModeCount(int mode)
{
	if (mode <= CHEST_SHOP_MODE_INVALID || mode >= CHEST_SHOP_MODE_MAX) return 0;

	static struct
	{
		int mode;
		int mode_count;
	}mode_mapper[] = 
	{
		{CHEST_SHOP_MODE_INVALID, 0},
		{CHEST_SHOP_MODE_1, MODE_1_COUNT},
		{CHEST_SHOP_MODE_10, MODE_10_COUNT},
		{CHEST_SHOP_MODE_30, MODE_30_COUNT},
		{ CHEST_SHOP_MODE1_1, MODE_1_COUNT },
		{ CHEST_SHOP_MODE1_10, MODE_10_COUNT },
		{ CHEST_SHOP_MODE1_30, MODE_30_COUNT },
		{ CHEST_SHOP_MODE2_1, MODE_1_COUNT },
		{ CHEST_SHOP_MODE2_10, MODE_10_COUNT },
		{ CHEST_SHOP_MODE2_30, MODE_30_COUNT },
		{CHEST_SHOP_JL_MODE_1, MODE_1_COUNT},
		{CHEST_SHOP_JL_MODE_10, MODE_10_COUNT},
		{CHEST_SHOP_JL_MODE_50, MODE_50_COUNT},
		{ CHEST_SHOP_MODE3_1, MODE_1_COUNT },
		{ CHEST_SHOP_MODE3_10, MODE_10_COUNT },
		{ CHEST_SHOP_MODE3_30, MODE_30_COUNT },
		{CHEST_SHOP_MODE_MAX, 0},
	};

	UNSTD_STATIC_CHECK(sizeof(mode_mapper) / sizeof(mode_mapper[0]) == CHEST_SHOP_MODE_MAX + 1);

	return mode_mapper[mode].mode_count;
}

ChestShopItemMap *ChestShop::GetChestShopItemMap(Role *role)
{
	if (nullptr == role)
	{
		return nullptr;
	}
	int open_day = role->GetOpenServerDay();
	int day_index = 0;
	if (open_day >= 30)
	{
		day_index = CHEST_SHOP_DAY_30;
	}
	else if(open_day >= 15)
	{
		day_index = CHEST_SHOP_DAY_15;
	}
	else if(open_day >= 8)
	{
		day_index = CHEST_SHOP_DAY_8;
	}
	else if(open_day >= 0)
	{
		day_index = CHEST_SHOP_DAY_0;
	}
	
	if (day_index >= 0 && day_index < CHEST_SHOP_DAY_MAX)
	{
		return &m_chest_item_map[day_index];
	}

	return NULL;
}

void ChestShop::Buy(Role *role, int shop_type, int mode)
{
	if (NULL == role || role->GetSublock()->CheckHasLock()) return;

	if (mode <= CHEST_SHOP_MODE_INVALID || mode >= CHEST_SHOP_MODE_MAX)
	{
		return;
	}

	if (shop_type <= 0 || shop_type >= CHEST_SHOP_TYPE_MAX)
	{
		return;
	}

	// 参与限制
	{
		JoinLimitUnit limit_unit;
		LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_CHESTSHOP, &limit_unit);

		if (role->GetLevel() < limit_unit.level)
		{
			role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
			return;
		}
	}
	
	UNSTD_STATIC_CHECK(MODE_1_COUNT > 0);

	int mode_count = this->GetModeCount(mode);
	if (mode_count < MODE_1_COUNT || mode_count > MODE_MAX_COUNT) return;

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!role->GetRoleChestShop()->CheckHaveSpace(shop_type, mode_count))
	{
		role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	bool send_record = false;
	int replace_num = 1;

	unsigned int now_sec = (unsigned int)(EngineAdapter::Instance().Time());
	int gold_cost = m_gold_list[mode];
	
	unsigned int next_free = role->GetRoleShop()->GetChestShopNextFreeTime(mode);
	if (MODE_1_COUNT == mode_count && now_sec >= next_free)
	{
		gold_cost = 0;
	}

	//尝试使用物品来寻宝
	bool use_item_to_buy = false;
	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	if (gold_cost > 0)
	{
		ItemID use_item_id = this->GetChestShopUseItem(shop_type, mode);

		if (use_item_id > 0)
		{
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
			stuff_list[0].item_id = use_item_id;
			stuff_list[0].num = 1;
			stuff_list[0].buyable = false;

			if (ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, false))
			{
				use_item_to_buy = true;
				gold_cost = 0;
			}
		}
	}

	//bool is_result_bind = true;

	if (gold_cost > 0)
	{
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHESTSHOP))
		{
			int discount = LOGIC_CONFIG->GetRandActivityCfg().GetChestshopDiscount();
			
			gold_cost = static_cast<int>(ceil(gold_cost * 1.0 * discount / 100));
			if (gold_cost <= 0)
			{
				return;
			}
		}

		if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(gold_cost))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		//is_result_bind = false;
	}

	UNSTD_STATIC_CHECK(MAX_ROLE_CHEST_GRID_HALF  >= MODE_MAX_COUNT);
	UNSTD_STATIC_CHECK(CHEST_SHOP_MAX_ITEM_NUM >= MODE_MAX_COUNT);

	int list_num = 0;
	static ChestItemInfo item_list[MODE_MAX_COUNT];
	// 确保每次清理
	for (int i = 0; i < MODE_MAX_COUNT; ++ i)
	{
		item_list[i].Reset();
	}

	bool broadcast_mark[MODE_MAX_COUNT] = {0};

	int rate_count = 0, total_rate = 0;
	int rate_value_list[CHEST_SHOP_MAX_ITEM_NUM] = {0};
	int item_index_list[CHEST_SHOP_MAX_ITEM_NUM] = {0};

	int item_index = 0;
	
	ChestShopItemMap *chestshop_item_map = this->GetChestShopItemMap(role);
	if (NULL == chestshop_item_map)
	{
		return;
	}
	
	std::map<UInt16, ChestShopItem> &item_map = chestshop_item_map->item_map;

	for (ChestShopItemMapIt it = item_map.begin(), end = item_map.end(); it != end && rate_count < CHEST_SHOP_MAX_ITEM_NUM; ++it)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(it->second.item_id);
		if (NULL != item_base && PROF_TYPE_PROF_ZHUANZHI_BASE > 0)
		{

			if (PROF_TYPE_PROF_NO_LIMIT == item_base->GetLimitProf() || 
				item_base->GetLimitProf() == role->GetProf() % PROF_TYPE_PROF_ZHUANZHI_BASE)
			{
				if (use_item_to_buy || gold_cost <= 0)
				{
					rate_value_list[rate_count] = it->second.useitem_rate_list[mode];
				}
				else
				{
					rate_value_list[rate_count] = it->second.rate_list[mode];
				}

				// 跳过配0权重的物品
				if (rate_value_list[rate_count] > 0)
				{
					item_index_list[rate_count] = item_index; // 记录下物品的索引

					total_rate += rate_value_list[rate_count];
					++rate_count;
				}
			}
		}

		++ item_index;
	}
	if (total_rate <= 0) return;

	bool got_first_hit_equip = false;
	bool got_first_hit_jingling = false;
	//bool got_rare_item = false;
	int mode_curr_count = 0, round_count = 0;
	
	//抽物品逻辑
	{
		//第一次抽装备特殊处理
		if ((CHEST_SHOP_TYPE_EQUIP == shop_type) || (CHEST_SHOP_TYPE_EQUIP1 == shop_type) || (CHEST_SHOP_TYPE_EQUIP2 == shop_type) || CHEST_SHOP_TYPE_EQUIP3 == shop_type)  //固定送
		{
			if (!role->GetRoleShop()->IsFirstEquipBeHited(shop_type))
			{
				ItemID first_fix_itemid = this->GetFirstBuyHitEquipId(role->GetBaseProf(), shop_type);
				if (first_fix_itemid > 0)
				{
					item_list[0].item_id = first_fix_itemid;
					item_list[0].num = 1;
					item_list[0].is_bind = true;
					list_num = 1;
					mode_curr_count = 1;
					got_first_hit_equip = true;

					// 赠送6条仙品属性
					const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_list[0].item_id);
					if (NULL != equip_item && ItemBase::I_TYPE_EQUIPMENT == equip_item->GetItemType())
					{
						static ItemGridData item_grid_data; item_grid_data.Reset();
						item_grid_data.item_id = first_fix_itemid;
						item_grid_data.param = new EquipmentParam();

						if (GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(&item_grid_data, ItemNamespace::MAX_XIANPIN_NUM))
						{
							EquipmentParam *equip_param = (EquipmentParam *)item_grid_data.param;
							for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; ++i)
							{
								item_list[0].xianpin_type_list[i] = char(equip_param->xianpin_type_list[i]);
							}
						}

						// 释放内存
						if (item_grid_data.param)
						{
							delete item_grid_data.param;
							item_grid_data.param = nullptr;
						}
					}
				}
			}
		}

		//第一次抽精灵特殊处理
		if (CHEST_SHOP_TYPE_JINGLING == shop_type && CHEST_SHOP_JL_MODE_1 == mode && !role->GetRoleShop()->IsFirstJingLingBeHited())
		{
			ItemID first_fix_itemid = this->GetFirstBuyHitJinglingId();
			if (first_fix_itemid > 0)
			{
				item_list[0].item_id = first_fix_itemid;
				item_list[0].num = 1;
				item_list[0].is_bind = true;
				list_num = 1;
				mode_curr_count = 1;
				got_first_hit_jingling = true;

				static ItemGridData item_grid_data; item_grid_data.Reset();
				item_grid_data.item_id = first_fix_itemid;
				item_grid_data.param = new EquipmentParam();

				if (LOGIC_CONFIG->GetJingLingCfg().GetRandomTalent(&item_grid_data) && LOGIC_CONFIG->GetJingLingCfg().SetJingLingInitLevel(&item_grid_data))
				{
					EquipmentParam *equip_param = (EquipmentParam *)item_grid_data.param;
					for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; ++ i)
					{
						item_list[list_num].xianpin_type_list[i] = char(equip_param->xianpin_type_list[i]);
					}
					item_list[list_num].strength = equip_param->strengthen_level;
				}

				// 释放内存
				if (item_grid_data.param)
				{
					delete item_grid_data.param;
					item_grid_data.param = nullptr;
				}

				EventHandler::Instance().OnChestShop(role, shop_type, first_fix_itemid);
			}
		}

		while (round_count ++ < 256 && mode_curr_count < mode_count && list_num < MODE_MAX_COUNT)
		{
			int rand_index = 0, rand_value = RandomNum(total_rate);
			for (rand_index = 0; rand_index < rate_count; rand_index++)
			{
				if (rate_value_list[rand_index] != 0 && rand_value <= rate_value_list[rand_index]) break;

				rand_value -= rate_value_list[rand_index];
			}

			if (rand_index >= 0 && rand_index < rate_count)
			{
				int item_index = item_index_list[rand_index];
				if (item_index >= 0 && item_index < (int)item_map.size())
				{
					ChestShopItemMapIt it = item_map.begin();
					std::advance(it, item_index);

					UInt16 item_id = it->first, num = 1;
					bool is_bind = it->second.is_bind;
					bool is_repeat = false;
					int is_replace_garbage = 0;
					role->GetLotteryCount()->ReplaceLotterySequence(shop_type, item_id, &is_replace_garbage);	//根据世界和个人寻宝次数替换物品id
					if (item_map.find(item_id) == item_map.end())
					{
						item_id = it->first;
					}
					if (item_id != it->first)
					{
						is_bind = item_map[item_id].is_bind;
					}
					int j = 0;
					for (j = 0; j < list_num; j++) // 重复检查
					{
						if (item_list[j].item_id == item_id && !it->second.can_repeat)
						{
							role->GetLotteryCount()->ReplaceLotterySequenceBySeqType(shop_type, LOTTERY_SEQUENCE_TYPE_GARBAGE, item_id);
							num = replace_num;
							is_repeat = true;
							is_replace_garbage = 1;
							j = list_num;
							break;
						}
					}

//					const ItemBase *base_cfg = ITEMPOOL->GetItem(item_id);
					//for (j = 0; j < list_num; j++)
					//{
					//	if (item_list[j].item_id == item_id && item_list[j].num < base_cfg->GetPileLimit())
					//	{
					//		if (it->second.can_repeat || is_repeat) item_list[j].num += num;
					//		break;
					//	}
					//}

					if (j >= list_num)
					{
						item_list[list_num].item_id = item_id;
						item_list[list_num].num = num;
						item_list[list_num].is_bind = is_bind;	//现在是否绑定由配置决定
					
						const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_id);
						if (NULL != equip_item && ItemBase::I_TYPE_EQUIPMENT == equip_item->GetItemType())
						{
							static ItemGridData item_grid_data;
							item_grid_data.Reset();

							item_grid_data.item_id = item_id;
							item_grid_data.param = new EquipmentParam();
							
							if ((CHEST_SHOP_TYPE_EQUIP == shop_type) || (CHEST_SHOP_TYPE_EQUIP1 == shop_type) || (CHEST_SHOP_TYPE_EQUIP2 == shop_type) || CHEST_SHOP_TYPE_EQUIP3 == shop_type)  //装备幸运属性 , 仙品属性
							{
								if (GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(&item_grid_data))
								{
									EquipmentParam *equip_param = (EquipmentParam *)item_grid_data.param;
									for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; ++ i)
									{
										item_list[list_num].xianpin_type_list[i] = char(equip_param->xianpin_type_list[i]);
									}
								}
								
								int lucky_rate = it->second.lucky_rate;
								if (lucky_rate > 0 && lucky_rate <= 100 && RandomNum(100) <= lucky_rate)
								{
									if (GLOBALCONFIG->GetEquipConfig().GetLucky(&item_grid_data))
									{
										EquipmentParam *equip_param = (EquipmentParam *)item_grid_data.param;
										item_list[list_num].has_lucky = equip_param->has_lucky;
									}
								}
								if (equip_item->GetSubType() == Equipment::E_TYPE_SPECIAL_MINGHUN)
								{
									item_list[list_num].strength = 1;	//寻宝 设置仙宠灵魂的初始等级
								}
							}
							else if (CHEST_SHOP_TYPE_JINGLING == shop_type)
							{
								if (LOGIC_CONFIG->GetJingLingCfg().GetRandomTalent(&item_grid_data) && LOGIC_CONFIG->GetJingLingCfg().SetJingLingInitLevel(&item_grid_data))
								{
									EquipmentParam *equip_param = (EquipmentParam *)item_grid_data.param;
									for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; ++ i)
									{
										item_list[list_num].xianpin_type_list[i] = char(equip_param->xianpin_type_list[i]);
									}
									item_list[list_num].strength = equip_param->strengthen_level;
								}
							}

							// 释放内存
							if (item_grid_data.param)
							{
								delete item_grid_data.param;
								item_grid_data.param = nullptr;
							}

							EventHandler::Instance().OnChestShop(role, shop_type, item_id);
						}

						broadcast_mark[list_num] = it->second.is_broadcast && 0 == is_replace_garbage;
						++ list_num;
					}

					++mode_curr_count;

					/*if (item_id == m_rare_item_id)		// 如果是珍稀
					{
						got_rare_item = true;
					}*/

					if (it->second.is_record && !is_repeat)
					{
						WorldStatus::Instance().AddChestShopRecord(role, item_id, shop_type);
						send_record = true;
					}
				}
			}
		}
	}
	
	// 屏蔽珍贵物品保底功能，需要再开回来
// 	{
// 		// 保证一个珍贵物品
// 		if ((CHEST_SHOP_MODE_10 == mode && !got_rare_item))
// 		{
// 			if (item_list[0].num == 1)
// 			{
// 				item_list[0].item_id = (unsigned short)m_rare_item_id;
// 				broadcast_mark[0] = true;
// 			}
// 			else if (item_list[0].num > 1)
// 			{
// 				if (0 < list_num && list_num < MODE_10_COUNT)
// 				{
// 					-- item_list[0].num;
// 
// 					item_list[list_num].item_id = (unsigned short)m_rare_item_id;
// 					item_list[list_num].num = 1;
// 					item_list[list_num].is_bind = is_result_bind;
// 					broadcast_mark[list_num] = true;
// 
// 					++ list_num;
// 				}
// 			}
// 		}
// 		else if((CHEST_SHOP_MODE_30 == mode && !got_rare_item))
// 		{
// 			if (item_list[0].num == 1)
// 			{
// 				item_list[0].item_id = (unsigned short)m_rare_item_id;
// 				broadcast_mark[0] = true;
// 			}
// 			else if (item_list[0].num > 1)
// 			{
// 				if (0 < list_num && list_num < MODE_30_COUNT)
// 				{
// 					-- item_list[0].num;
// 
// 					item_list[list_num].item_id = (unsigned short)m_rare_item_id;
// 					item_list[list_num].num = 1;
// 					item_list[list_num].is_bind = is_result_bind;
// 					broadcast_mark[list_num] = true;
// 
// 					++ list_num;
// 				}
// 			}
// 		}
// 	}

	//寻宝寻到的护符一定是本职业的,另外给所有装备加上传奇属性
	for (int i = 0; i < list_num && i < MODE_MAX_COUNT; ++i)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(item_list[i].item_id);
		if (NULL != item_base)
		{
			if (ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType())
			{
				const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_list[i].item_id);
				if (ItemBase::I_COLOR_ORANGE <= equip->GetColor())	//新增装备限制：橙色以上的装备必须是本职业的(具体根据配置来定)
				{
					ItemID change_itemid = 0;
					if (LOGIC_CONFIG->GetZhuanzhiCfg().GetChangeItemID(equip->GetItemId(), role->GetProf() % PROF_TYPE_PROF_ZHUANZHI_BASE, &change_itemid))
					{
						const ItemBase *change_item = ITEMPOOL->GetItem(change_itemid);
						if (NULL != change_item) 
						{
							item_list[i].item_id = change_item->GetItemId();
						}
					}
				}
				//传奇属性
				static ItemGridData item_grid_data;
				item_grid_data.Reset();

				item_grid_data.item_id = item_list[i].item_id;
				item_grid_data.param = new EquipmentParam();

				if (EquipmentManager::GetFixSpecialAttr(&item_grid_data, Equipment::EQUIPMENT_FIX_GETWAY_CHEST_SHOP, 0))
				{
					EquipmentParam *equip_param = (EquipmentParam *)item_grid_data.param;
					for (int num = 0; num < ItemNamespace::MAX_XIANPIN_NUM; ++ num)
					{
						item_list[i].xianpin_type_list[num] = char(equip_param->xianpin_type_list[num]);
					}
				}

				// 释放内存
				if (item_grid_data.param)
				{
					delete item_grid_data.param;
					item_grid_data.param = nullptr;
				}
			}
		}
	}

	// 要在外面打包 OnBuyItemList里面会更改item_list信息
	Protocol::SCChestShopItemListPerBuy per_buy_item_list;
	per_buy_item_list.count = list_num;
	per_buy_item_list.shop_type = shop_type;
	per_buy_item_list.mode = mode;
	for (int j = 0; j < list_num && j < MAX_ROLE_CHEST_GRID; j ++)
	{
		per_buy_item_list.item_list[j].item_id = item_list[j].item_id;
		per_buy_item_list.item_list[j].num = static_cast<short>(item_list[j].num);
		per_buy_item_list.item_list[j].is_bind = item_list[j].is_bind ? 1 : 0;
		per_buy_item_list.item_list[j].shop_type = shop_type;
		per_buy_item_list.item_list[j].index = j;
		per_buy_item_list.item_list[j].has_lucky = item_list[j].has_lucky;
		per_buy_item_list.item_list[j].strength = item_list[j].strength;
		memcpy(per_buy_item_list.item_list[j].xianpin_type_list, item_list[j].xianpin_type_list, sizeof(per_buy_item_list.item_list[j].xianpin_type_list));
	}

	role->GetMonitor()->AddMonitorNum(MONITOR_TYPE_CHEST_SHOP_BUY_NUM, mode_count);

	gamelog::g_log_chestshop.buff_printf("ChestShop::Buy [user[%d %s] mode:%d gold_cost:%d nextfree:%u items[", 
		role->GetUID(), (const char*)role->GetName(), (int)mode, (int)gold_cost,
		role->GetRoleShop()->GetChestShopNextFreeTime(mode));

	{
		int list_str_len = 0; static char list_str[20480] = {0};

		for (int i = 0; i < list_num && i < MODE_MAX_COUNT; ++i)
		{
			if (0 != item_list[i].item_id)
			{
				gamelog::g_log_chestshop.buff_printf(" %d:%d", (int)item_list[i].item_id, (int)item_list[i].num);
			
				bool is_append_item_str = false;

				if (broadcast_mark[i])			    // 如果没有获得仙女卡，就按配置的广播规则  
				{
					is_append_item_str = true;
				}

				if (is_append_item_str)
				{
					char item_str[128] = {0};

					const ItemBase *item_base = ITEMPOOL->GetItem(item_list[i].item_id);
					if (NULL != item_base)
					{
						int itemlen = 0;

						if (ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType())
						{
							EquipmentParam *temp_equipment_param = new EquipmentParam();
							temp_equipment_param->has_lucky = item_list[i].has_lucky;
							temp_equipment_param->strengthen_level = item_list[i].strength;
							for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++ k)
							{
								temp_equipment_param->xianpin_type_list[k] = static_cast<int>(item_list[i].xianpin_type_list[k]);
							}
						
							const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(role->GetUserId()), (int)item_list[i].item_id, false, 0, temp_equipment_param, NULL);
							if (NULL != equip_param_string)
							{
								itemlen = SNPRINTF(item_str, sizeof(item_str), "{eq;%d;%s}x%d  ", (int)item_list[i].item_id, equip_param_string, (int)item_list[i].num);
							}

							delete temp_equipment_param;
						}
						else
						{
							itemlen = SNPRINTF(item_str, sizeof(item_str), "{i;%d}x%d  ", (int)item_list[i].item_id, (int)item_list[i].num);
						}

						if (itemlen > 0)
						{
							STRNCPY(list_str + list_str_len, item_str, sizeof(list_str) - list_str_len);
							list_str_len += itemlen;
						}
					}
					
				}
			}
		}

		list_str[sizeof(list_str) - 1] = 0;

		if (list_str_len > 0)
		{
			int sendlen = 0;
			if (CHEST_SHOP_TYPE_EQUIP == shop_type || CHEST_SHOP_TYPE_EQUIP1 == shop_type || CHEST_SHOP_TYPE_EQUIP2 == shop_type || CHEST_SHOP_TYPE_EQUIP3 == shop_type)
			{
				sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chuji_equip_chestshop_item_broadcast, 
				role->GetUID(), (const char*)role->GetName(), (int)role->GetCamp(), list_str);
			}
			else if (CHEST_SHOP_TYPE_JINGLING == shop_type)
			{
				sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chuji_jingling_chestshop_item_broadcast, 
				role->GetUID(), (const char*)role->GetName(), (int)role->GetCamp(), list_str);
			}

			if (sendlen > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	gamelog::g_log_chestshop.buff_printf(" ]]");
	gamelog::g_log_chestshop.commit_buff(LL_INFO);

	if (mode_curr_count == mode_count && role->GetRoleChestShop()->OnBuyItemList(shop_type, list_num, item_list, "ChestShopBuy")) 
	{
		//使用物品来抽
		if (consume_list.count > 0 && 0 == gold_cost && use_item_to_buy)   
		{
			role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "ChestShopBuy");
		}
		else
		{
			// 消耗元宝
			if (gold_cost > 0)
			{
				if (CHEST_SHOP_TYPE_JINGLING == shop_type)
				{
					role->GetKnapsack()->GetMoney()->UseGold(gold_cost, "ChestShopBuyJingLing");
				}
				else
				{
					role->GetKnapsack()->GetMoney()->UseGold(gold_cost, "ChestShopBuy");
				}
			}

			// 免费的则设置下次免费时间
			else
			{
				if (CHEST_SHOP_MODE_1 == mode || CHEST_SHOP_MODE1_1 == mode || CHEST_SHOP_MODE2_1 == mode)
				{
					// 策划需求更改为只按时间重置免费机会，去除每日免费次数限制，所以注释了以下代码，不要删，以防又改回原来的玩法
// 					int free_buy_1_count = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_FREE_CHEST_BUY_1);
// 					int free_buy_1_limit = LOGIC_CONFIG->GetChestShop().GetFreeTimePerDay() - 1;
// 					if (free_buy_1_count < free_buy_1_limit)						
// 					{
// 						role->GetRoleShop()->SetChestShopNextFreeTime(mode, now_sec + m_free_cd_s_1);
// 						role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_FREE_CHEST_BUY_1);
// 					}
// 					// 最后一次抽取后将下次免费抽取时间增加一天
// 					else																
// 					{
// 						int m_free_cd = EngineAdapter::Instance().NextDayInterval(0, 0, 0);
// 						role->GetRoleShop()->SetChestShopNextFreeTime(mode, now_sec + m_free_cd);							
// 						role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_FREE_CHEST_BUY_1);
// 					}
		
					int cd_s = this->GetFreeCD1(mode);
					role->GetRoleShop()->SetChestShopNextFreeTime(mode, now_sec + cd_s);

				}
				else if (CHEST_SHOP_JL_MODE_1 == mode)
				{
					// 策划需求更改为只按时间重置免费机会，去除每日免费次数限制，所以注释了以下代码，不要删，以防又改回原来的玩法
// 					int free_buy_1_count = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_FREE_CHEST_JINGLING_BUY_1);
// 					int free_buy_1_limit = LOGIC_CONFIG->GetChestShop().GetFreeTimeJingLingPerDay() - 1;
// 					if (free_buy_1_count < free_buy_1_limit)						
// 					{
// 						role->GetRoleShop()->SetChestShopNextFreeTime(mode, now_sec + m_free_jingling_cd_s_1);
// 						role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_FREE_CHEST_JINGLING_BUY_1);
// 					}
// 					// 最后一次抽取后将下次免费抽取时间增加一天
// 					else																
// 					{
// 						int m_free_cd = EngineAdapter::Instance().NextDayInterval(0, 0, 0);
// 						role->GetRoleShop()->SetChestShopNextFreeTime(mode, now_sec + m_free_cd);							
// 						role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_FREE_CHEST_JINGLING_BUY_1);
// 					}

					int cd_s = this->GetFreeCD1(mode);
					role->GetRoleShop()->SetChestShopNextFreeTime(mode, now_sec + cd_s);
 				}

				role->GetRoleChestShop()->SendFreeInfo();
			}
		}
	
		// 第一次抽装备记录
		if ((CHEST_SHOP_TYPE_EQUIP == shop_type || CHEST_SHOP_TYPE_EQUIP1 == shop_type || CHEST_SHOP_TYPE_EQUIP2 == shop_type || CHEST_SHOP_TYPE_EQUIP3 == shop_type)
			&& got_first_hit_equip)
		{
			role->GetRoleShop()->SetFirstEquipBeHited(shop_type);
		}

		// 第一次抽装备记录
		if (CHEST_SHOP_TYPE_JINGLING == shop_type && got_first_hit_jingling)
		{
			role->GetRoleShop()->SetFirstJingLingBeHited();
		}

		//花元宝的送积分
		if (gold_cost > 0)
		{
			int add_score = mode_count * GetAddScore(shop_type);

			//检查寻宝\猎取精灵双倍积分活动是否开启
			if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DOUBLE_SCORE))
			{
				add_score *= 2;
			}
			
			if (add_score > 0)
			{
				switch (shop_type)
				{
				case CHEST_SHOP_TYPE_EQUIP:
				{
					role->GetRoleShop()->AddChestShopTreasureCredit(add_score, "Chestshop");
				}
				break;
				case CHEST_SHOP_TYPE_EQUIP1:
				{
					role->GetRoleShop()->AddChestShopTreasureCredit1(add_score, "Chestshop");
				}
				break;
				case CHEST_SHOP_TYPE_EQUIP2:
				{
					role->GetRoleShop()->AddChestShopTreasureCredit2(add_score, "Chestshop");
				}
				break;
				case CHEST_SHOP_TYPE_EQUIP3:
				{
					role->GetRoleShop()->AddChestShopTreasureCredit3(add_score, "Chestshop");
				}
				break;
				case CHEST_SHOP_TYPE_JINGLING:
				{
					role->GetRoleShop()->AddChestShopJingLingCredit(add_score, "ChestshopJingling");
				}
				break;
				default : break;
				}
			}

			{
				//增加称号寻宝次数
				if (CHEST_SHOP_TYPE_EQUIP == shop_type || CHEST_SHOP_TYPE_EQUIP1 == shop_type || CHEST_SHOP_TYPE_EQUIP2 == shop_type || CHEST_SHOP_TYPE_EQUIP3 == shop_type)
				{
					role->AddEquipXunBaoTimes(mode_count);
					role->GetTitleManager()->CheckAllTitleCondition(TITLE_TYPE_XUNBAO);
				}
				else
				{
					role->AddJLXunbaoTimes(mode_count);
					role->GetTitleManager()->CheckAllTitleCondition(TITLE_TYPE_LIEQU);
				}
			}
		}

		int sendlen = sizeof(per_buy_item_list) - (MAX_ROLE_CHEST_GRID - per_buy_item_list.count) * sizeof(ChestItemInfo);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&per_buy_item_list, sendlen);

		EventHandler::Instance().OnChestShopXunBao(role, shop_type, mode_count, (0 == gold_cost));
	}

	// 发送
	if (send_record)
	{
		WorldStatus::Instance().SendChestShopRecord(role, shop_type);
	}

	// 日志
	{
		gamelog::g_log_function_stats.printf(LL_INFO, "[Join][user[%s %d] type:ChestShop times:%d]",
			role->GetName(), role->GetUID(), mode_count);

		ROLE_LOG_QUICK10(LOG_TYPE_FUNCTION_STATUS, role, LOG_TYPE_FUNCTION_STATUS_TYPE_XUNBAO, mode_count, NULL, NULL,
			role->GetLevel(), role->GetRoleActivity()->GetHistoryChongZhi(), 0, 0);
	}
}

ItemID ChestShop::GetChestShopUseItem(int shop_type, int mode)
{
	ItemID use_item_id = 0;

	struct ChestshopKeyItem
	{
		int shop_type;
		int mode;
		ItemID item_id;
		short reserve_sh;
	};
	static const std::vector<ChestshopKeyItem> chestshop_items = {
		{ CHEST_SHOP_TYPE_EQUIP, CHEST_SHOP_MODE_1, m_equip_use_itemid },
		{ CHEST_SHOP_TYPE_EQUIP, CHEST_SHOP_MODE_10, m_equip_10_use_itemid },
		{ CHEST_SHOP_TYPE_EQUIP, CHEST_SHOP_MODE_30, m_equip_30_use_itemid },
		{ CHEST_SHOP_TYPE_EQUIP1, CHEST_SHOP_MODE1_1, m_equip1_use_itemid },
		{ CHEST_SHOP_TYPE_EQUIP1, CHEST_SHOP_MODE1_10, m_equip1_10_use_itemid },
		{ CHEST_SHOP_TYPE_EQUIP1, CHEST_SHOP_MODE1_30, m_equip1_30_use_itemid },
		{ CHEST_SHOP_TYPE_EQUIP2, CHEST_SHOP_MODE2_1, m_equip2_use_itemid },
		{ CHEST_SHOP_TYPE_EQUIP2, CHEST_SHOP_MODE2_10, m_equip2_10_use_itemid },
		{ CHEST_SHOP_TYPE_EQUIP2, CHEST_SHOP_MODE2_30, m_equip2_30_use_itemid },
		{ CHEST_SHOP_TYPE_JINGLING, CHEST_SHOP_JL_MODE_1, m_jingling_use_itemid },
		{ CHEST_SHOP_TYPE_JINGLING, CHEST_SHOP_JL_MODE_10, m_jingling_10_use_itemid },
		{ CHEST_SHOP_TYPE_JINGLING, CHEST_SHOP_JL_MODE_50, m_jingling_50_use_itemid },
		{ CHEST_SHOP_TYPE_EQUIP3, CHEST_SHOP_MODE3_1, m_equip3_use_itemid },
		{ CHEST_SHOP_TYPE_EQUIP3, CHEST_SHOP_MODE3_10, m_equip3_10_use_itemid },
		{ CHEST_SHOP_TYPE_EQUIP3, CHEST_SHOP_MODE3_30, m_equip3_30_use_itemid },
	};

	auto it = std::find_if(chestshop_items.cbegin(), chestshop_items.cend(), [shop_type, mode](const ChestshopKeyItem &cmp_item) {
		if (shop_type == cmp_item.shop_type && mode == cmp_item.mode)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != chestshop_items.end())
	{
		use_item_id = it->item_id;
	}

	return use_item_id;
}

int ChestShop::GetFreeCD1(int mode)
{
	int cd_s = 0;
	if (CHEST_SHOP_MODE_1 == mode)
	{
		cd_s = m_free_cd_s_1;
	}

	else if (CHEST_SHOP_MODE1_1 == mode)
	{
		cd_s = m_free_cd1_s_1;
	}

	else if (CHEST_SHOP_MODE2_1 == mode)
	{
		cd_s = m_free_cd2_s_1;
	}

	else if (CHEST_SHOP_JL_MODE_1 == mode)
	{
		cd_s = m_free_jingling_cd_s_1;
	}

	return cd_s;
}

int ChestShop::GetAddScore(int short_type)
{
	int score = 0;
	if (CHEST_SHOP_TYPE_EQUIP == short_type)
	{
		score = m_add_score;
	}

	else if (CHEST_SHOP_TYPE_EQUIP1 == short_type)
	{
		score = m_add_score1;
	}

	else if (CHEST_SHOP_TYPE_EQUIP2 == short_type)
	{
		score = m_add_score2;
	}

	else if (CHEST_SHOP_TYPE_EQUIP3 == short_type)
	{
		score = m_add_score3;
	}

	else if (CHEST_SHOP_TYPE_JINGLING == short_type)
	{
		score = m_add_jingling_score;
	}

	return score;
}

int ChestShop::InitChestShopItemList(PugiXmlNode ItemListElement, int day_index)
{
	if (day_index < 0 || day_index >= CHEST_SHOP_DAY_MAX)
	{
		return -10000;
	}
	ChestShopItemMap &shop_item_map = m_chest_item_map[day_index];

	PugiXmlNode ItemElement = ItemListElement.child("data");
	while (!ItemElement.empty())
	{
		ChestShopItem shop_item;
		if (!PugiGetSubNodeValue(ItemElement, "item_id", shop_item.item_id) || shop_item.item_id <= 0)
		{
			return -3;
		}
		const ItemBase *itembase = ITEMPOOL->GetItem(shop_item.item_id);
		if (NULL == itembase)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(ItemElement, "rate_1", shop_item.rate_list[CHEST_SHOP_MODE_1]) || shop_item.rate_list[CHEST_SHOP_MODE_1] < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(ItemElement, "use_item_rate_1", shop_item.useitem_rate_list[CHEST_SHOP_MODE_1]) || shop_item.useitem_rate_list[CHEST_SHOP_MODE_1] < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(ItemElement, "rate_10", shop_item.rate_list[CHEST_SHOP_MODE_10]) || shop_item.rate_list[CHEST_SHOP_MODE_10] < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(ItemElement, "rate_30", shop_item.rate_list[CHEST_SHOP_MODE_30]) || shop_item.rate_list[CHEST_SHOP_MODE_30] < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(ItemElement, "rate1_1", shop_item.rate_list[CHEST_SHOP_MODE1_1]) || shop_item.rate_list[CHEST_SHOP_MODE1_1] < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(ItemElement, "use_item_rate1_1", shop_item.useitem_rate_list[CHEST_SHOP_MODE1_1]) || shop_item.useitem_rate_list[CHEST_SHOP_MODE1_1] < 0)
		{
			return -107;
		}

		if (!PugiGetSubNodeValue(ItemElement, "rate1_10", shop_item.rate_list[CHEST_SHOP_MODE1_10]) || shop_item.rate_list[CHEST_SHOP_MODE1_10] < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(ItemElement, "rate1_30", shop_item.rate_list[CHEST_SHOP_MODE1_30]) || shop_item.rate_list[CHEST_SHOP_MODE1_30] < 0)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(ItemElement, "rate2_1", shop_item.rate_list[CHEST_SHOP_MODE2_1]) || shop_item.rate_list[CHEST_SHOP_MODE2_1] < 0)
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(ItemElement, "use_item_rate2_1", shop_item.useitem_rate_list[CHEST_SHOP_MODE2_1]) || shop_item.useitem_rate_list[CHEST_SHOP_MODE2_1] < 0)
		{
			return -108;
		}

		if (!PugiGetSubNodeValue(ItemElement, "rate2_10", shop_item.rate_list[CHEST_SHOP_MODE2_10]) || shop_item.rate_list[CHEST_SHOP_MODE2_10] < 0)
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(ItemElement, "rate2_30", shop_item.rate_list[CHEST_SHOP_MODE2_30]) || shop_item.rate_list[CHEST_SHOP_MODE2_30] < 0)
		{
			return -20;
		}

		//策划要求：至尊寻宝卡概率与消耗元宝10连抽概率一样
		shop_item.useitem_rate_list[CHEST_SHOP_MODE_10] = shop_item.rate_list[CHEST_SHOP_MODE_10];
		shop_item.useitem_rate_list[CHEST_SHOP_MODE1_10] = shop_item.rate_list[CHEST_SHOP_MODE1_10];
		shop_item.useitem_rate_list[CHEST_SHOP_MODE2_10] = shop_item.rate_list[CHEST_SHOP_MODE2_10];
		
		// 消耗元宝30连抽与使用物品30连抽概率一样
		shop_item.useitem_rate_list[CHEST_SHOP_MODE_30] = shop_item.rate_list[CHEST_SHOP_MODE_30];
		shop_item.useitem_rate_list[CHEST_SHOP_MODE1_30] = shop_item.rate_list[CHEST_SHOP_MODE1_30];
		shop_item.useitem_rate_list[CHEST_SHOP_MODE2_30] = shop_item.rate_list[CHEST_SHOP_MODE2_30];

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(ItemElement, "is_broadcast", is_broadcast) || (0 != is_broadcast && 1 != is_broadcast) )
		{
			return -9;
		}
		shop_item.is_broadcast = (0 != is_broadcast);

		int can_repeat = 0;
		if (!PugiGetSubNodeValue(ItemElement, "can_repeat", can_repeat) || (0 != can_repeat && 1 != can_repeat))
		{
			return -10;
		}
		shop_item.can_repeat = (0 != can_repeat);

		if (shop_item_map.item_map.find(shop_item.item_id) != shop_item_map.item_map.end())
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(ItemElement, "jingling_rate_1", shop_item.rate_list[CHEST_SHOP_JL_MODE_1]) || shop_item.rate_list[CHEST_SHOP_JL_MODE_1] < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(ItemElement, "use_item_jingling_rate_1", shop_item.useitem_rate_list[CHEST_SHOP_JL_MODE_1]) || shop_item.useitem_rate_list[CHEST_SHOP_JL_MODE_1] < 0)
		{
			return -204;
		}

		if (!PugiGetSubNodeValue(ItemElement, "jingling_rate_10", shop_item.rate_list[CHEST_SHOP_JL_MODE_10]) || shop_item.rate_list[CHEST_SHOP_JL_MODE_10] < 0)
		{
			return -15;
		}

		//策划要求：至尊寻宝卡概率与消耗元宝10连抽概率一样
		shop_item.useitem_rate_list[CHEST_SHOP_JL_MODE_10] = shop_item.rate_list[CHEST_SHOP_JL_MODE_10];

		if (!PugiGetSubNodeValue(ItemElement, "jingling_rate_50", shop_item.rate_list[CHEST_SHOP_JL_MODE_50]) || shop_item.rate_list[CHEST_SHOP_JL_MODE_50] < 0)
		{
			return -16;
		}
		
		// 消耗元宝50连抽与使用物品50连抽概率一样
		shop_item.useitem_rate_list[CHEST_SHOP_JL_MODE_10] = shop_item.rate_list[CHEST_SHOP_JL_MODE_10];

		if (!PugiGetSubNodeValue(ItemElement, "lucky_rate", shop_item.lucky_rate) || shop_item.lucky_rate < 0 || shop_item.lucky_rate > 100)
		{
			return -17;
		}

		int is_record = 0;
		if (!PugiGetSubNodeValue(ItemElement, "is_record", is_record) || (0 != is_record && 1 != is_record))
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(ItemElement, "is_bind", shop_item.is_bind))
		{
			return -100;
		}
		
		shop_item.is_record = (0 != is_record);

		shop_item_map.item_map[shop_item.item_id] = shop_item;
		
		ItemElement = ItemElement.next_sibling();
	}

	if (shop_item_map.item_map.size() > (size_t)CHEST_SHOP_MAX_ITEM_NUM)
	{
		return -14;
	}
	
	return 0;
}

int ChestShop::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "replace_item_id", m_replace_item_id[CHEST_SHOP_TYPE_EQUIP]) ||
			NULL == ITEMPOOL->GetItem(m_replace_item_id[CHEST_SHOP_TYPE_EQUIP]))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "replace_garbage_item_id_1", m_replace_item_id[CHEST_SHOP_TYPE_EQUIP1]) ||
			NULL == ITEMPOOL->GetItem(m_replace_item_id[CHEST_SHOP_TYPE_EQUIP1]))
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(dataElement, "replace_garbage_item_id_2", m_replace_item_id[CHEST_SHOP_TYPE_EQUIP2]) ||
			NULL == ITEMPOOL->GetItem(m_replace_item_id[CHEST_SHOP_TYPE_EQUIP2]))
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(dataElement, "replace_garbage_item_id_3", m_replace_item_id[CHEST_SHOP_TYPE_EQUIP3]) ||
			NULL == ITEMPOOL->GetItem(m_replace_item_id[CHEST_SHOP_TYPE_EQUIP3]))
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(dataElement, "replace_garbage_item_id_jingling", m_replace_item_id[CHEST_SHOP_TYPE_JINGLING]) ||
			NULL == ITEMPOOL->GetItem(m_replace_item_id[CHEST_SHOP_TYPE_JINGLING]))
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(dataElement, "rare_item_id", m_rare_item_id) ||
			NULL == ITEMPOOL->GetItem(m_rare_item_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold_1", m_gold_list[CHEST_SHOP_MODE_1]) || m_gold_list[CHEST_SHOP_MODE_1] <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold_10", m_gold_list[CHEST_SHOP_MODE_10]) || m_gold_list[CHEST_SHOP_MODE_10] <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold_30", m_gold_list[CHEST_SHOP_MODE_30]) || m_gold_list[CHEST_SHOP_MODE_30] <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold1_1", m_gold_list[CHEST_SHOP_MODE1_1]) || m_gold_list[CHEST_SHOP_MODE1_1] <= 0)
		{
			return -33;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold1_10", m_gold_list[CHEST_SHOP_MODE1_10]) || m_gold_list[CHEST_SHOP_MODE1_10] <= 0)
		{
			return -34;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold1_30", m_gold_list[CHEST_SHOP_MODE1_30]) || m_gold_list[CHEST_SHOP_MODE1_30] <= 0)
		{
			return -35;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold2_1", m_gold_list[CHEST_SHOP_MODE2_1]) || m_gold_list[CHEST_SHOP_MODE2_1] <= 0)
		{
			return -36;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold2_10", m_gold_list[CHEST_SHOP_MODE2_10]) || m_gold_list[CHEST_SHOP_MODE2_10] <= 0)
		{
			return -37;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold2_30", m_gold_list[CHEST_SHOP_MODE2_30]) || m_gold_list[CHEST_SHOP_MODE2_30] <= 0)
		{
			return -38;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold3_1", m_gold_list[CHEST_SHOP_MODE3_1]) || m_gold_list[CHEST_SHOP_MODE3_1] <= 0)
		{
			return -136;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold3_10", m_gold_list[CHEST_SHOP_MODE3_10]) || m_gold_list[CHEST_SHOP_MODE3_10] <= 0)
		{
			return -137;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold3_30", m_gold_list[CHEST_SHOP_MODE3_30]) || m_gold_list[CHEST_SHOP_MODE3_30] <= 0)
		{
			return -138;
		}

		if (!PugiGetSubNodeValue(dataElement, "free_cd_s_1", m_free_cd_s_1) || m_free_cd_s_1 <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "free_cd1_s_1", m_free_cd1_s_1) || m_free_cd1_s_1 <= 0)
		{
			return -39;
		}

		if (!PugiGetSubNodeValue(dataElement, "free_cd2_s_1", m_free_cd2_s_1) || m_free_cd2_s_1 <= 0)
		{
			return -40;
		}

		if (!PugiGetSubNodeValue(dataElement, "free_cd_s_30", m_free_cd_s_30) || m_free_cd_s_30 <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "free_1_times_per_day", m_free_1_times_per_day) || m_free_1_times_per_day < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "jingling_gold_1", m_gold_list[CHEST_SHOP_JL_MODE_1]) || m_gold_list[CHEST_SHOP_JL_MODE_1] <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "jingling_gold_10", m_gold_list[CHEST_SHOP_JL_MODE_10]) || m_gold_list[CHEST_SHOP_JL_MODE_10] <= 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "jingling_gold_50", m_gold_list[CHEST_SHOP_JL_MODE_50]) || m_gold_list[CHEST_SHOP_JL_MODE_50] <= 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_score", m_add_score) || m_add_score < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_score1", m_add_score1) || m_add_score1 < 0)
		{
			return -36;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_score2", m_add_score2) || m_add_score2 < 0)
		{
			return -37;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_score3", m_add_score3) || m_add_score3 < 0)
		{
			return -38;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_buy_hit_equipid_1", m_first_buy_hit_equipid_1) || NULL == ITEMPOOL->GetItem(m_first_buy_hit_equipid_1))
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_buy_hit_equipid_2", m_first_buy_hit_equipid_2) || NULL == ITEMPOOL->GetItem(m_first_buy_hit_equipid_2))
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_buy_hit_equipid_3", m_first_buy_hit_equipid_3) || NULL == ITEMPOOL->GetItem(m_first_buy_hit_equipid_3))
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_buy_hit_equipid_4", m_first_buy_hit_equipid_4) || NULL == ITEMPOOL->GetItem(m_first_buy_hit_equipid_4))
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_buy_2_hit_equipid", m_first_buy_2_hit_equipid) || NULL == ITEMPOOL->GetItem(m_first_buy_2_hit_equipid))
		{
			return -32;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_buy_3_hit_equipid", m_first_buy_3_hit_equipid) || NULL == ITEMPOOL->GetItem(m_first_buy_3_hit_equipid))
		{
			return -33;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_buy_4_hit_equipid", m_first_buy_4_hit_equipid) || NULL == ITEMPOOL->GetItem(m_first_buy_4_hit_equipid))
		{
			return -34;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_buy_hit_jinglingid_1", m_first_buy_hit_jinglingid_1) || NULL == ITEMPOOL->GetItem(m_first_buy_hit_jinglingid_1))
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_buy_hit_jinglingid_2", m_first_buy_hit_jinglingid_2) || NULL == ITEMPOOL->GetItem(m_first_buy_hit_jinglingid_2))
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_buy_hit_jinglingid_3", m_first_buy_hit_jinglingid_3) || NULL == ITEMPOOL->GetItem(m_first_buy_hit_jinglingid_3))
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(dataElement, "first_buy_hit_jinglingid_4", m_first_buy_hit_jinglingid_4) || NULL == ITEMPOOL->GetItem(m_first_buy_hit_jinglingid_4))
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip_use_itemid", m_equip_use_itemid) || (0 != m_equip_use_itemid && NULL == ITEMPOOL->GetItem(m_equip_use_itemid)))
		{
			return -21;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip_10_use_itemid", m_equip_10_use_itemid) || (0 != m_equip_10_use_itemid && NULL == ITEMPOOL->GetItem(m_equip_10_use_itemid)))
		{
			return -23;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip_30_use_itemid", m_equip_30_use_itemid) || (0 != m_equip_30_use_itemid && NULL == ITEMPOOL->GetItem(m_equip_30_use_itemid)))
		{
			return -25;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip1_use_itemid", m_equip1_use_itemid) || (0 != m_equip1_use_itemid && NULL == ITEMPOOL->GetItem(m_equip1_use_itemid)))
		{
			return -21;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip1_10_use_itemid", m_equip1_10_use_itemid) || (0 != m_equip1_10_use_itemid && NULL == ITEMPOOL->GetItem(m_equip1_10_use_itemid)))
		{
			return -23;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip1_30_use_itemid", m_equip1_30_use_itemid) || (0 != m_equip1_30_use_itemid && NULL == ITEMPOOL->GetItem(m_equip1_30_use_itemid)))
		{
			return -25;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip2_use_itemid", m_equip2_use_itemid) || (0 != m_equip2_use_itemid && NULL == ITEMPOOL->GetItem(m_equip2_use_itemid)))
		{
			return -33;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip2_10_use_itemid", m_equip2_10_use_itemid) || (0 != m_equip2_10_use_itemid && NULL == ITEMPOOL->GetItem(m_equip2_10_use_itemid)))
		{
			return -34;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip2_30_use_itemid", m_equip2_30_use_itemid) || (0 != m_equip2_30_use_itemid && NULL == ITEMPOOL->GetItem(m_equip2_30_use_itemid)))
		{
			return -35;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip3_use_itemid", m_equip3_use_itemid) || (0 != m_equip3_use_itemid && NULL == ITEMPOOL->GetItem(m_equip3_use_itemid)))
		{
			return -43;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip3_10_use_itemid", m_equip3_10_use_itemid) || (0 != m_equip3_10_use_itemid && NULL == ITEMPOOL->GetItem(m_equip3_10_use_itemid)))
		{
			return -44;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip3_30_use_itemid", m_equip3_30_use_itemid) || (0 != m_equip3_30_use_itemid && NULL == ITEMPOOL->GetItem(m_equip3_30_use_itemid)))
		{
			return -45;
		}

		if (!PugiGetSubNodeValue(dataElement, "jingling_use_itemid", m_jingling_use_itemid) || (0 != m_jingling_use_itemid && NULL == ITEMPOOL->GetItem(m_jingling_use_itemid)))
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(dataElement, "jingling_10_use_itemid", m_jingling_10_use_itemid) || (0 != m_jingling_10_use_itemid && NULL == ITEMPOOL->GetItem(m_jingling_10_use_itemid)))
		{
			return -24;
		}

		if (!PugiGetSubNodeValue(dataElement, "jingling_50_use_itemid", m_jingling_50_use_itemid) || (0 != m_jingling_50_use_itemid && NULL == ITEMPOOL->GetItem(m_jingling_50_use_itemid)))
		{
			return -26;
		}

		if (!PugiGetSubNodeValue(dataElement, "one_consume_gold", m_one_rare_shop_consume_gold) || m_one_rare_shop_consume_gold <= 0)
		{
			return -27;
		}

		if (!PugiGetSubNodeValue(dataElement, "ten_consume_gold", m_ten__rare_shop_consume_gold) || m_ten__rare_shop_consume_gold <= 0)
		{
			return -28;
		}


		if (!PugiGetSubNodeValue(dataElement, "jingling_free_cd_s_1", m_free_jingling_cd_s_1) || m_free_jingling_cd_s_1 <= 0)
		{
			return -29;
		}

		if (!PugiGetSubNodeValue(dataElement, "jingling_free_1_times_per_day", m_free_1_times_jingling_per_day) || m_free_1_times_jingling_per_day < 0)
		{
			return -30;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_jingling_score", m_add_jingling_score) || m_add_jingling_score < 0)
		{
			return -31;
		}
		
	
	}

	return 0;
}

int ChestShop::InitRareChestShop(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	int last_seq = 0;
	int min_buy_count = 0;
	m_rare_shop_total_weight = 0;

	while (!dataElement.empty())
	{	
		RareChestShopCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "seq", cfg.seq) || cfg.seq != last_seq + 1)
		{
			return -1;
		}
		last_seq = cfg.seq;

		PugiXmlNode item_element = dataElement.child("reward_item");
		if (item_element.empty())
		{
			return -101;
		}

		if (!cfg.item.ReadConfig(item_element))
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(dataElement, "weight", cfg.weight) || cfg.weight <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast) || (cfg.is_broadcast != 0 && cfg.is_broadcast != 1))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "min_required", cfg.min_buy_count))
		{
			return -4;
		}

		if (cfg.min_buy_count > 0 && min_buy_count == 0)
		{
			min_buy_count = cfg.min_buy_count;
		}
		else if (cfg.min_buy_count > 0 && min_buy_count > 0 && cfg.min_buy_count != min_buy_count)	//确保保底次数一致
		{
			return -40;
		}

		if (!PugiGetSubNodeValue(dataElement, "lucky_rate", cfg.lucky_rate) || cfg.lucky_rate < 0 || cfg.lucky_rate > 100)
		{
			return -5;
		}

		m_rare_shop_total_weight += cfg.weight;

		m_rare_chest_reward_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	if (m_rare_shop_total_weight <= 0)
	{
		return -6;
	}

	return 0;
}

void ChestShop::GMBuyTest(Role *role, int chest_shop_type, int mode, std::map<unsigned short, int> &itemmap)
{
	
}

ItemID ChestShop::GetFirstBuyHitEquipId(int prof, int shop_type)
{
	ItemID first_item_id = 0;

	struct ChestshopFirstEquipItem
	{
		int prof;
		int shop_type;
		ItemID item_id;
		short reserve_sh;
	};
	static const std::vector<ChestshopFirstEquipItem> first_equip_items = {
		{ PROF_TYPE_PROF_1, CHEST_SHOP_TYPE_EQUIP, m_first_buy_hit_equipid_1 },
		{ PROF_TYPE_PROF_2, CHEST_SHOP_TYPE_EQUIP, m_first_buy_hit_equipid_2 },
		{ PROF_TYPE_PROF_3, CHEST_SHOP_TYPE_EQUIP, m_first_buy_hit_equipid_3 },
		{ PROF_TYPE_PROF_4, CHEST_SHOP_TYPE_EQUIP, m_first_buy_hit_equipid_4 },
		{ PROF_TYPE_PROF_1, CHEST_SHOP_TYPE_EQUIP1, m_first_buy_2_hit_equipid },
		{ PROF_TYPE_PROF_2, CHEST_SHOP_TYPE_EQUIP1, m_first_buy_2_hit_equipid },
		{ PROF_TYPE_PROF_3, CHEST_SHOP_TYPE_EQUIP1, m_first_buy_2_hit_equipid },
		{ PROF_TYPE_PROF_4, CHEST_SHOP_TYPE_EQUIP1, m_first_buy_2_hit_equipid },
		{ PROF_TYPE_PROF_1, CHEST_SHOP_TYPE_EQUIP2, m_first_buy_3_hit_equipid },
		{ PROF_TYPE_PROF_2, CHEST_SHOP_TYPE_EQUIP2, m_first_buy_3_hit_equipid },
		{ PROF_TYPE_PROF_3, CHEST_SHOP_TYPE_EQUIP2, m_first_buy_3_hit_equipid },
		{ PROF_TYPE_PROF_4, CHEST_SHOP_TYPE_EQUIP2, m_first_buy_3_hit_equipid },
		{ PROF_TYPE_PROF_1, CHEST_SHOP_TYPE_EQUIP3, m_first_buy_4_hit_equipid },
		{ PROF_TYPE_PROF_2, CHEST_SHOP_TYPE_EQUIP3, m_first_buy_4_hit_equipid },
		{ PROF_TYPE_PROF_3, CHEST_SHOP_TYPE_EQUIP3, m_first_buy_4_hit_equipid },
		{ PROF_TYPE_PROF_4, CHEST_SHOP_TYPE_EQUIP3, m_first_buy_4_hit_equipid },
	};

	auto it = std::find_if(first_equip_items.cbegin(), first_equip_items.cend(), [prof, shop_type](const ChestshopFirstEquipItem &cmp_item) {
		if (prof == cmp_item.prof && shop_type == cmp_item.shop_type)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != first_equip_items.end())
	{
		first_item_id = it->item_id;
	}

	return first_item_id;
}

ItemID ChestShop::GetFirstBuyHitJinglingId()
{
	int rand_val = RandomNum(100);
	if (rand_val <= 25)
	{
		return m_first_buy_hit_jinglingid_1;
	}
	else if (rand_val <= 50)
	{
		return m_first_buy_hit_jinglingid_2;
	}
	else if (rand_val <= 75)
	{
		return m_first_buy_hit_jinglingid_3;
	}
	else if (rand_val <= 100)
	{
		return m_first_buy_hit_jinglingid_4;
	}

	return 0;
}

const RareChestShopCfg * ChestShop::GetRareChestShopReward(const int need_buy_times, bool *is_get_rare)
{
	if (NULL == is_get_rare)
	{
		return NULL;
	}

	std::vector<int> seq_vec;
	int reward_seq = -1;

	// 有大于保底次数的直接给其中一个
	for (std::vector<RareChestShopCfg>::const_iterator iter = m_rare_chest_reward_vec.begin(); iter != m_rare_chest_reward_vec.end(); ++ iter)
	{
		if (iter->min_buy_count > 0 && need_buy_times >= iter->min_buy_count)
		{
			seq_vec.push_back(iter->seq);
		}
	}

	if (seq_vec.size() > 0)
	{
		int rand_val = RandomNum((int)seq_vec.size());
		reward_seq = seq_vec.at(rand_val);
	}

	for (std::vector<RareChestShopCfg>::const_iterator iter = m_rare_chest_reward_vec.begin(); 
		iter != m_rare_chest_reward_vec.end() && reward_seq > 0; ++ iter)
	{
		if (reward_seq == iter->seq)
		{
			*is_get_rare = true;
			return &(*iter);
		}
	}

	if (m_rare_shop_total_weight <= 0)
	{
		return NULL;
	}

	// 以下是随机
	int rand_val = RandomNum(m_rare_shop_total_weight);

	for (std::vector<RareChestShopCfg>::const_iterator iter = m_rare_chest_reward_vec.begin(); iter != m_rare_chest_reward_vec.end(); ++ iter)
	{
		if (rand_val < iter->weight)
		{
			return & (*iter);
		}
		rand_val -= iter->weight;
	}
	
	return NULL;
}

void ChestShop::RareBuy(Role *role, int times)
{
	if (NULL == role || times <= 0)
	{
		return;
	}

	if (!role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, times))
	{
		role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	int consume_gold = 0;
	if (1 == times)
	{
		consume_gold = m_one_rare_shop_consume_gold;
	}
	else if (10 == times)
	{
		consume_gold = m_ten__rare_shop_consume_gold;
	}
	else
	{
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(consume_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	ChestItemInfo item_list[MODE_10_COUNT];
	for (int i = 0; i < MODE_10_COUNT; ++ i)
	{
		item_list[i].Reset();
	}

	short broadcast_index_list[MODE_10_COUNT];
	int broadcast_count = 0;
	for (int i = 0; i < MODE_10_COUNT; ++ i)
	{
		broadcast_index_list[i] = -1;
	}

	int list_num = 0;
	int cur_cycle = 0;
	bool is_get_rare = false;

	gamelog::g_log_chestshop.buff_printf("ChestShop::Buy [user[%d %s] times:%d gold_cost:%d items[", 
		role->GetUID(), (const char*)role->GetName(), (int)times, (int)consume_gold);

	for (list_num = 0; list_num < times && cur_cycle < times && cur_cycle < MODE_MAX_COUNT;)
	{
		++ cur_cycle;
			
		is_get_rare = false;
		const RareChestShopCfg *reward_cfg = this->GetRareChestShopReward(role->GetRareChestShopTimes(), &is_get_rare);
		if (NULL == reward_cfg)
		{
			continue;
		}

		if (1 == reward_cfg->is_broadcast && broadcast_count >= 0 && broadcast_count < MODE_10_COUNT)
		{
			broadcast_index_list[broadcast_count] = list_num;
			++ broadcast_count;
		}

		static ItemGridData item_grid_data; item_grid_data.Reset();
		item_grid_data.item_id = reward_cfg->item.item_id;
		item_grid_data.param = new EquipmentParam();

		item_list[list_num].item_id = reward_cfg->item.item_id;
		item_list[list_num].num = reward_cfg->item.num;
		item_list[list_num].is_bind = reward_cfg->item.is_bind;

		const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_list[list_num].item_id);
		if (NULL != equip_item && Equipment::E_TYPE_MIN <= equip_item->GetEquipType() && equip_item->GetEquipType() < Equipment::E_TYPE_MAX)
		{
			if (GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(&item_grid_data))
			{
				EquipmentParam *equip_param = (EquipmentParam *)item_grid_data.param;
				for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; ++ i)
				{
					item_list[list_num].xianpin_type_list[i] = char(equip_param->xianpin_type_list[i]);
				}
			}

			if (RandomNum(100) < reward_cfg->lucky_rate && reward_cfg->lucky_rate <= 100)
			{
				if (GLOBALCONFIG->GetEquipConfig().GetLucky(&item_grid_data))
				{
					EquipmentParam *equip_param = (EquipmentParam *)item_grid_data.param;
					item_list[list_num].has_lucky = equip_param->has_lucky;
				}
			}
		}
		else if (NULL != equip_item && Equipment::E_TYPE_SPECIAL_JINGLING == equip_item->GetEquipType())
		{
			if (LOGIC_CONFIG->GetJingLingCfg().GetRandomTalent(&item_grid_data) && LOGIC_CONFIG->GetJingLingCfg().SetJingLingInitLevel(&item_grid_data))
			{
				EquipmentParam *equip_param = (EquipmentParam *)item_grid_data.param;
				for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; ++ i)
				{
					item_list[list_num].xianpin_type_list[i] = char(equip_param->xianpin_type_list[i]);
				}
				item_list[list_num].strength = equip_param->strengthen_level;
			}
		}

		++ list_num;

		role->AddRareChestShopTimes(1);

		if (true == is_get_rare)
		{
			role->ResetRareChestShopTimes();
		}

		gamelog::g_log_chestshop.buff_printf("(%s), ", ItemConfigDataLog(&reward_cfg->item, NULL));
	}

	if (consume_gold > 0 && !role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "ChestShop::RareBuy"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		gamelog::g_log_chestshop.clear_buff();
		return;
	}

	gamelog::g_log_chestshop.buff_printf(" ]");
	gamelog::g_log_chestshop.commit_buff(LL_INFO);

	Protocol::SCChestShopItemListPerBuy per_buy_item_list;
	per_buy_item_list.count = list_num;
	per_buy_item_list.shop_type = 3;
	//per_buy_item_list.mode = mode;
	for (int j = 0; j < list_num && j < MAX_ROLE_CHEST_GRID; j ++)
	{
		per_buy_item_list.item_list[j].item_id = item_list[j].item_id;
		per_buy_item_list.item_list[j].num = static_cast<short>(item_list[j].num);
		per_buy_item_list.item_list[j].is_bind = item_list[j].is_bind ? 1 : 0;
		per_buy_item_list.item_list[j].shop_type = 3;
		per_buy_item_list.item_list[j].index = j;
		per_buy_item_list.item_list[j].has_lucky = item_list[j].has_lucky;
		per_buy_item_list.item_list[j].strength = item_list[j].strength;
		memcpy(per_buy_item_list.item_list[j].xianpin_type_list, item_list[j].xianpin_type_list, sizeof(per_buy_item_list.item_list[j].xianpin_type_list));
	}

	int send_length = sizeof(per_buy_item_list) - (MAX_ROLE_CHEST_GRID - per_buy_item_list.count) * sizeof(ChestItemInfo);
	if (send_length > 0)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&per_buy_item_list, send_length);
	}

	// 传闻
	int list_str_len = 0; 
	static char list_str[20480] = {0};

	for (int i = 0; i < broadcast_count; ++ i)
	{
		char item_str[128] = {0};
		memset(list_str, 0, sizeof(list_str));

		int index = broadcast_index_list[i];
		if (index < 0 || index > MODE_10_COUNT)
		{
			continue;
		}

		const ItemBase *item_base = ITEMPOOL->GetItem(item_list[index].item_id);
		if (NULL != item_base)
		{
			int itemlen = 0;

			if (ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType())
			{
				EquipmentParam *temp_equipment_param = new EquipmentParam();
				temp_equipment_param->has_lucky = item_list[index].has_lucky;
				temp_equipment_param->strengthen_level = item_list[index].strength;
				for (int k = 0; k < ItemNamespace::MAX_XIANPIN_NUM; ++ k)
				{
					temp_equipment_param->xianpin_type_list[k] = static_cast<int>(item_list[index].xianpin_type_list[k]);
				}

				const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(role->GetUserId()), (int)item_list[index].item_id, false, 0, temp_equipment_param, NULL);
				if (NULL != equip_param_string)
				{
					itemlen = SNPRINTF(item_str, sizeof(item_str), "{eq;%d;%s}x%d  ", (int)item_list[index].item_id, equip_param_string, (int)item_list[index].num);
				}

				delete temp_equipment_param;
			}
			else
			{
				itemlen = SNPRINTF(item_str, sizeof(item_str), "{i;%d}x%d  ", (int)item_list[index].item_id, (int)item_list[index].num);
			}

			if (itemlen > 0)
			{
				STRNCPY(list_str + list_str_len, item_str, sizeof(list_str) - list_str_len);
				list_str_len += itemlen;
			}


			list_str[sizeof(list_str) - 1] = 0;

			if (list_str_len > 0)
			{
				int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_gaoji_chestshop_item_broadcast, 
					role->GetUID(), (const char*)role->GetName(), (int)role->GetCamp(), list_str);

				if (sendlen > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}

	// 最后给予由于OnBuyItemList后itemlist数量为0，但上面传闻依赖itemlist
	role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, list_num, item_list, "ChestShop::RareBuy");
}
