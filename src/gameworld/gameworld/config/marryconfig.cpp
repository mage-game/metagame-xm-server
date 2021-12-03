#include "marryconfig.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"

#include "item/itempool.h"
#include "item/itemgriddata.h"
#include "item/itembase.h"
#include "equipment/equipment.h"

#include "checkresourcecenter.hpp"

MarryConfig::MarryConfig() 
	: m_marry_xunyou_hongbao_item_id(0), m_marry_xunyou_flower_item_id(0), m_marry_reserve_item_id(0), m_point_count(0), m_marry_scene_id(0), m_bai_tiandi_pos(0, 0), m_marry_need_intimacy(0), 
	m_marry_xunyou_hongbao_drop_num(0), m_marry_xunyou_hongbao_drop_dis(0), m_marry_xunyou_add_exp_interval(0), m_marry_xunyou_marryobj_speed(0), m_marry_xunyou_marryobj_count(0)
{
	memset(m_marry_xunyou_opera_count_list, 0, sizeof(m_marry_xunyou_opera_count_list));
}

MarryConfig::~MarryConfig()
{

}

bool MarryConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "MarryConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (!RootElement)
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode MarryLevelCfgListElement = RootElement.child("marry_level_cfg_list");
		if (MarryLevelCfgListElement.empty())
		{
			*err = path + ": xml node error in [marry_level_cfg_list]";
			return false;
		}

		PugiXmlNode MarryLevelElement = MarryLevelCfgListElement.child("marry_level_cfg");
		if (MarryLevelElement.empty())
		{
			*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg]";
			return false;
		}

		while (!MarryLevelElement.empty())
		{
			int marry_level = 0;
			if (!PugiGetSubNodeValue(MarryLevelElement, "marry_level", marry_level) || marry_level < MARRY_LEVEL_TYPE_INVALID || marry_level > MARRY_LEVEL_TYPE_MAX)
			{
				*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->marry_level]";
				return false;
			}

			if (!m_marry_level_cfg_list[marry_level].Invalid())
			{
				*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->marry_level] repeat";
				return false;
			}

			if (!PugiGetSubNodeValue(MarryLevelElement, "need_intimacy", m_marry_level_cfg_list[marry_level].need_intimacy) || m_marry_level_cfg_list[marry_level].need_intimacy <= 0)
			{
				*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->need_intimacy]";
				return false;
			}

			if (!PugiGetSubNodeValue(MarryLevelElement, "need_coin", m_marry_level_cfg_list[marry_level].need_coin) || m_marry_level_cfg_list[marry_level].need_coin < 0)
			{
				*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->need_coin]";
				return false;
			}

			if (!PugiGetSubNodeValue(MarryLevelElement, "need_gold", m_marry_level_cfg_list[marry_level].need_gold) || m_marry_level_cfg_list[marry_level].need_gold < 0)
			{
				*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->need_gold]";
				return false;
			}

			PugiXmlNode RewardShizhuangListElement = MarryLevelElement.child("reward_shizhuang_list");
			if (RewardShizhuangListElement.empty())
			{
				*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_shizhuang_list]";
				return false;
			}

			PugiXmlNode RewardShizhuangElement = RewardShizhuangListElement.child("reward_shizhuang");
			if (RewardShizhuangElement.empty())
			{
				*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_shizhuang_list->reward_shizhuang]";
				return false;
			}

			int shizhuang_count = 0;
			while (!RewardShizhuangElement.empty())
			{
				if (shizhuang_count >= MARRY_LEVEL_REWARD_SHIZHUANG_NUM)
				{
					*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_shizhuang_list->reward_shizhuang] more than limit";
					return false;
				}

				MarryLevelShizhuangCfg *shizhuang_cfg = &m_marry_level_cfg_list[marry_level].reward_shizhuang_list[shizhuang_count];

				if (!PugiGetSubNodeValue(RewardShizhuangElement, "prof", shizhuang_cfg->prof) || shizhuang_cfg->prof < PROF_TYPE_INVALID || shizhuang_cfg->prof >= PROF_TYPE_MAX)
				{
					*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_shizhuang_list->reward_shizhuang->prof]";
					return false;
				}

				if (!PugiGetSubNodeValue(RewardShizhuangElement, "sex", shizhuang_cfg->sex) || shizhuang_cfg->sex < FEMALE || shizhuang_cfg->sex > MALE)
				{
					*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_shizhuang_list->reward_shizhuang->sex]";
					return false;
				}

				if (!PugiGetSubNodeValue(RewardShizhuangElement, "item_id", shizhuang_cfg->shizhuang_item.item_id) || NULL == ITEMPOOL->GetItem(shizhuang_cfg->shizhuang_item.item_id))
				{
					*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_shizhuang_list->reward_shizhuang->item_id]";
					return false;
				}

				if (!PugiGetSubNodeValue(RewardShizhuangElement, "item_num", shizhuang_cfg->shizhuang_item.num) || shizhuang_cfg->shizhuang_item.num <= 0)
				{
					*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_shizhuang_list->reward_shizhuang->num]";
					return false;
				}
				if (!ItemBase::CanMerge(shizhuang_cfg->shizhuang_item.item_id) && shizhuang_cfg->shizhuang_item.num > 1)
				{
					*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_shizhuang_list->reward_shizhuang->num]";
					return false;
				}

				if (!PugiGetSubNodeValue(RewardShizhuangElement, "is_bind", shizhuang_cfg->shizhuang_item.is_bind))
				{
					*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_shizhuang_list->reward_shizhuang->is_bind]";
					return false;
				}

				++ shizhuang_count;
				RewardShizhuangElement = RewardShizhuangElement.next_sibling();
			}
			m_marry_level_cfg_list[marry_level].reward_shizhuang_count = shizhuang_count;

			PugiXmlNode RewardItemElement = MarryLevelElement.child("reward_item");
			if (RewardItemElement.empty())
			{
				*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_item]";
				return false;
			}

			int reward_item_count = 0;
			while(!RewardItemElement.empty())
			{
				if (reward_item_count >= MARRY_REWARD_ITEM_MAX_NUM)
				{
					*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_item]";
					return false;
				}

				ItemConfigData *item = &m_marry_level_cfg_list[marry_level].reward_item_list[reward_item_count];
				if (!PugiGetSubNodeValue(RewardItemElement, "item_id", item->item_id) || NULL == ITEMPOOL->GetItem(item->item_id))
				{
					*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_item->item_id]";
					return false;
				}

				if (!PugiGetSubNodeValue(RewardItemElement, "item_num", item->num) || item->num <= 0)
				{
					*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_item->item_num]";
					return false;
				}
				if (!ItemBase::CanMerge(item->item_id) && item->num > 1)
				{
					*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_item->item_num]";
					return false;
				}

				if (!PugiGetSubNodeValue(RewardItemElement, "is_bind", item->is_bind))
				{
					*err = path + ": xml node error in [marry_level_cfg_list->marry_level_cfg->reward_item->is_bind]";
					return false;
				}

				++ reward_item_count;
				RewardItemElement = RewardItemElement.next_sibling();
			}
			m_marry_level_cfg_list[marry_level].reward_count = reward_item_count;

			MarryLevelElement = MarryLevelElement.next_sibling();
		}
	}

	{
		PugiXmlNode MarryXunYouElement = RootElement.child("marry_xunyou");
		if (MarryXunYouElement.empty())
		{
			*err = path + ": xml node error in [marry_xunyou]";
			return false;
		}

		if (!PugiGetSubNodeValue(MarryXunYouElement, "sa_hongbao_count", m_marry_xunyou_opera_count_list[MARRY_XUNYOU_OPERA_TYPE_SA_HONGBAO]) || m_marry_xunyou_opera_count_list[MARRY_XUNYOU_OPERA_TYPE_SA_HONGBAO] <= 0)
		{
			*err = path + ": xml node error in [marry_xunyou->sa_hongbao_count]";
			return false;
		}

		if (!PugiGetSubNodeValue(MarryXunYouElement, "ranfang_yanhua_count", m_marry_xunyou_opera_count_list[MARRY_XUNYOU_OPERA_TYPE_RANFANG_YANHUA]) || m_marry_xunyou_opera_count_list[MARRY_XUNYOU_OPERA_TYPE_RANFANG_YANHUA] <= 0)
		{
			*err = path + ": xml node error in [marry_xunyou->ranfang_yanhua_count]";
			return false;
		}

		if (!PugiGetSubNodeValue(MarryXunYouElement, "meigui_biaobai_count", m_marry_xunyou_opera_count_list[MARRY_XUNYOU_OPERA_TYPE_MEIGUI_BIAOBAI]) || m_marry_xunyou_opera_count_list[MARRY_XUNYOU_OPERA_TYPE_MEIGUI_BIAOBAI] <= 0)
		{
			*err = path + ": xml node error in [marry_xunyou->ranfang_yanhua_count]";
			return false;
		}

		if (!PugiGetSubNodeValue(MarryXunYouElement, "hongbao_item_id", m_marry_xunyou_hongbao_item_id) || NULL == ITEMPOOL->GetItem(m_marry_xunyou_hongbao_item_id))
		{
			*err = path + ": xml node error in [marry_xunyou->hongbao_item_id]";
			return false;
		}

		if (!PugiGetSubNodeValue(MarryXunYouElement, "flower_item_id", m_marry_xunyou_flower_item_id) || NULL == ITEMPOOL->GetItem(m_marry_xunyou_flower_item_id))
		{
			*err = path + ": xml node error in [marry_xunyou->flower_item_id]";
			return false;
		}

		PugiXmlNode XunYouPointListElement = MarryXunYouElement.child("xunyou_point_list");
		if (XunYouPointListElement.empty())
		{
			*err = path + ": xml node error in [marry_xunyou->xunyou_point_list]";
			return false;
		}

		PugiXmlNode DesPointElement = XunYouPointListElement.child("des_point");
		if (DesPointElement.empty())
		{
			*err = path + ": xml node error in [marry_xunyou->xunyou_point_list->des_point]";
			return false;
		}

		int point_count = 0;
		while (!DesPointElement.empty())
		{
			if (point_count >= MARRY_XUNYOU_POINT_MAX_NUM)
			{
				*err = path + ": xml node error in [marry_xunyou->xunyou_point_list->des_point] more than limit";
				return false;
			}

			MarryXunYouPoint *point = &m_marry_xunyou_point_list[point_count];

			if (!PugiGetSubNodeValue(DesPointElement, "pos_x", point->pos.x) || point->pos.x <= 0)
			{
				*err = path + ": xml node error in [marry_xunyou->xunyou_point_list->des_point->pos_x]";
				return false;
			}

			if (!PugiGetSubNodeValue(DesPointElement, "pos_y", point->pos.y) || point->pos.y <= 0)
			{
				*err = path + ": xml node error in [marry_xunyou->xunyou_point_list->des_point->pos_y]";
				return false;
			}

			if (!PugiGetSubNodeValue(DesPointElement, "stop_time", point->stop_time) || point->stop_time < 0)
			{
				*err = path + ": xml node error in [marry_xunyou->xunyou_point_list->des_point->stop_time]";
				return false;
			}

			++ point_count;
			DesPointElement = DesPointElement.next_sibling();
		}
		m_point_count = point_count;
	}

	{
		PugiXmlNode MarryHunYanlement = RootElement.child("marry_hunyan");
		if (MarryHunYanlement.empty())
		{
			*err = path + ": xml node error in [marry_hunyan]";
			return false;
		}

		if (!PugiGetSubNodeValue(MarryHunYanlement, "hunyan_scene_id", m_marry_hunyan_cfg.scene_id) || m_marry_hunyan_cfg.scene_id <= 0)
		{
			*err = path + ": xml node error in [marry_hunyan->hunyan_scene_id]";
			return false;
		}
		//CheckResourceCenter::Instance().GetSceneCheck()->Add(m_marry_hunyan_cfg.scene_id);

		if (!PugiGetSubNodeValue(MarryHunYanlement, "enter_pos_x", m_marry_hunyan_cfg.enter_pos.x) || m_marry_hunyan_cfg.enter_pos.x <= 0)
		{
			*err = path + ": xml node error in [marry_hunyan->enter_pos_x]";
			return false;
		}

		if (!PugiGetSubNodeValue(MarryHunYanlement, "enter_pos_y", m_marry_hunyan_cfg.enter_pos.y) || m_marry_hunyan_cfg.enter_pos.y <= 0)
		{
			*err = path + ": xml node error in [marry_hunyan->enter_pos_y]";
			return false;
		}

		if (!PugiGetSubNodeValue(MarryHunYanlement, "add_exp", m_marry_hunyan_cfg.add_exp) || m_marry_hunyan_cfg.add_exp <= 0)
		{
			*err = path + ": xml node error in [marry_hunyan->add_exp]";
			return false;
		}

		if (!PugiGetSubNodeValue(MarryHunYanlement, "invite_rune_item_id", m_marry_hunyan_cfg.invite_rune_item_id) || NULL == ITEMPOOL->GetItem(m_marry_hunyan_cfg.invite_rune_item_id))
		{
			*err = path + ": xml node error in [marry_hunyan->invite_rune_item_id]";
			return false;
		}

		if (!PugiGetSubNodeValue(MarryHunYanlement, "firework_item_id", m_marry_hunyan_cfg.firework_item_id) || NULL == ITEMPOOL->GetItem(m_marry_hunyan_cfg.firework_item_id))
		{
			*err = path + ": xml node error in [marry_hunyan->firework_item_id]";
			return false;
		}

		if (!PugiGetSubNodeValue(MarryHunYanlement, "firework_add_exp", m_marry_hunyan_cfg.firework_add_exp) || m_marry_hunyan_cfg.firework_add_exp <= 0)
		{
			*err = path + ": xml node error in [marry_hunyan->firework_add_exp]";
			return false;
		}

		PugiXmlNode HunYanBlessGiftListlement = MarryHunYanlement.child("bless_gift_list");
		if (HunYanBlessGiftListlement.empty())
		{
			*err = path + ": xml node error in [marry_hunyan->bless_gift_list]";
			return false;
		}

		PugiXmlNode HunYanBlessGiftlement = HunYanBlessGiftListlement.child("bless_gift");
		if (HunYanBlessGiftlement.empty())
		{
			*err = path + ": xml node error in [marry_hunyan->bless_gift_list->bless_gift]";
			return false;
		}

		int bless_gift_count = 0;
		while (!HunYanBlessGiftlement.empty())
		{
			if (bless_gift_count >= MARRY_HUNYAN_BLESS_GIFT_MAX_NUM)
			{
				*err = path + ": xml node error in [marry_hunyan->bless_gift_list->bless_gift] more than limit";
				return false;
			}

			MarryHunyanBlessGift *gift = &m_marry_hunyan_cfg.bless_gift_list[bless_gift_count];

			if (!PugiGetSubNodeValue(HunYanBlessGiftlement, "gift_coin", gift->gift_coin) || gift->gift_coin < 0)
			{
				*err = path + ": xml node error in [marry_hunyan->bless_gift_list->bless_gift->gift_coin]";
				return false;
			}

			if (!PugiGetSubNodeValue(HunYanBlessGiftlement, "item_cost_gold", gift->item_cost_gold) || gift->item_cost_gold < 0)
			{
				*err = path + ": xml node error in [marry_hunyan->bless_gift_list->bless_gift->item_cost_gold]";
				return false;
			}

			if (!PugiGetSubNodeValue(HunYanBlessGiftlement, "item_id", gift->gift_item.item_id) || (gift->gift_item.item_id > 0 && NULL == ITEMPOOL->GetItem(gift->gift_item.item_id)))
			{
				*err = path + ": xml node error in [marry_hunyan->bless_gift_list->bless_gift->item_id]";
				return false;
			}

			if (!PugiGetSubNodeValue(HunYanBlessGiftlement, "item_num", gift->gift_item.num) || gift->gift_item.num < 0)
			{
				*err = path + ": xml node error in [marry_hunyan->bless_gift_list->bless_gift->item_num]";
				return false;
			}
			if (!ItemBase::CanMerge(gift->gift_item.item_id) && gift->gift_item.num > 1)
			{
				*err = path + ": xml node error in [marry_hunyan->bless_gift_list->bless_gift->item_num]";
				return false;
			}

			if (!PugiGetSubNodeValue(HunYanBlessGiftlement, "item_is_bind", gift->gift_item.is_bind))
			{
				*err = path + ": xml node error in [marry_hunyan->bless_gift_list->bless_gift->item_is_bind]";
				return false;
			}

			++ bless_gift_count;
			HunYanBlessGiftlement = HunYanBlessGiftlement.next_sibling();
		}
		m_marry_hunyan_cfg.bless_gift_count = bless_gift_count;
	}

	{
		if (!PugiGetSubNodeValue(RootElement, "marry_scene_id", m_marry_scene_id) || m_marry_scene_id <= 0)
		{
			*err = path + ": xml node error in [marry_scene_id]";
			return false;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(m_marry_scene_id);

		if (!PugiGetSubNodeValue(RootElement, "marry_bai_tiandi_pos_x", m_bai_tiandi_pos.x) || m_bai_tiandi_pos.x <= 0)
		{
			*err = path + ": xml node error in [marry_bai_tiandi_pos_x]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "marry_bai_tiandi_pos_y", m_bai_tiandi_pos.y) || m_bai_tiandi_pos.y <= 0)
		{
			*err = path + ": xml node error in [marry_bai_tiandi_pos_y]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "marry_need_intimacy", m_marry_need_intimacy) || m_marry_need_intimacy <= 0)
		{
			*err = path + ": xml node error in [marry_need_intimacy]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "marry_xunyou_hongbao_drop_num", m_marry_xunyou_hongbao_drop_num) || m_marry_xunyou_hongbao_drop_num <= 0)
		{
			*err = path + ": xml node error in [marry_xunyou_hongbao_drop_num]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "marry_xunyou_hongbao_drop_dis", m_marry_xunyou_hongbao_drop_dis) || m_marry_xunyou_hongbao_drop_dis <= 0)
		{
			*err = path + ": xml node error in [marry_xunyou_hongbao_drop_dis]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "marry_xunyou_add_exp_interval", m_marry_xunyou_add_exp_interval) || m_marry_xunyou_add_exp_interval <= 0)
		{
			*err = path + ": xml node error in [marry_xunyou_add_exp_interval]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "marry_xunyou_marryobj_speed", m_marry_xunyou_marryobj_speed) || m_marry_xunyou_marryobj_speed <= 0)
		{
			*err = path + ": xml node error in [marry_xunyou_marryobj_speed]";
			return false;
		}

		if (!PugiGetSubNodeValue(RootElement, "marry_reserve_item_id", m_marry_reserve_item_id) || NULL == ITEMPOOL->GetItem(m_marry_reserve_item_id))
		{
			*err = path + ": xml node error in [marry_reserve_item_id]";
			return false;
		}

		PugiXmlNode MarryMarryobjDisListlement = RootElement.child("marry_xunyou_marryobj_dis_list");
		if (MarryMarryobjDisListlement.empty())
		{
			*err = path + ": xml node error in [marry_xunyou_marryobj_dis_list]";
			return false;
		}

		PugiXmlNode MarryMarryobjDislement = MarryMarryobjDisListlement.child("dis");
		if (MarryMarryobjDislement.empty())
		{
			*err = path + ": xml node error in [marry_xunyou_marryobj_dis_list->dis]";
			return false;
		}

		int marryobj_count = 0;
		while (!MarryMarryobjDislement.empty())
		{
			if (marryobj_count >= MARRY_MARRYOBJ_MAX_NUM)
			{
				*err = path + ": xml node error in [marry_xunyou_marryobj_dis_list->dis] more than limit";
				return false;
			}

			if (!PugiGetNodeValue(MarryMarryobjDislement, m_marry_xunyou_marryobj_dis_list[marryobj_count]) || m_marry_xunyou_marryobj_dis_list[marryobj_count] < 0)
			{
				*err = path + ": xml node error in [marry_xunyou_marryobj_dis_list->dis]";
				return false;
			}

			++ marryobj_count;
			MarryMarryobjDislement = MarryMarryobjDislement.next_sibling();
		}
		m_marry_xunyou_marryobj_count = marryobj_count;
	}

	{
		// π∫¬Ú—≤”Œ≤Ÿ◊˜≈‰÷√
		PugiXmlNode buyXunyouOperaElement = RootElement.child("buy_xunyou_opera_cfg");
		if (buyXunyouOperaElement.empty())
		{
			*err = path + ": xml node error in [buy_xunyou_opera_cfg]";
			return false;
		}

		int need_gold = 0;
		int buy_times = 0;

		if (!PugiGetSubNodeValue(buyXunyouOperaElement, "hongbao_need_gold", need_gold) || need_gold <= 0)
		{
			*err = path + ": xml node error in [buy_xunyou_opera_cfg->hongbao_need_gold]";
			return false;
		}

		if (!PugiGetSubNodeValue(buyXunyouOperaElement, "hongbao_buy_times", buy_times) || buy_times <= 0)
		{
			*err = path + ": xml node error in [buy_xunyou_opera_cfg->buy_times]";
			return false;
		}

		m_buy_xunyou_cfg_list[MARRY_XUNYOU_OPERA_TYPE_SA_HONGBAO].need_gold = need_gold;
		m_buy_xunyou_cfg_list[MARRY_XUNYOU_OPERA_TYPE_SA_HONGBAO].buy_times = buy_times;

		if (!PugiGetSubNodeValue(buyXunyouOperaElement, "yanhua_need_gold", need_gold) || need_gold <= 0)
		{
			*err = path + ": xml node error in [buy_xunyou_opera_cfg->yanhua_need_gold]";
			return false;
		}

		if (!PugiGetSubNodeValue(buyXunyouOperaElement, "yanhua_buy_times", buy_times) || buy_times <= 0)
		{
			*err = path + ": xml node error in [buy_xunyou_opera_cfg->yanhua_buy_times]";
			return false;
		}

		m_buy_xunyou_cfg_list[MARRY_XUNYOU_OPERA_TYPE_RANFANG_YANHUA].need_gold = need_gold;
		m_buy_xunyou_cfg_list[MARRY_XUNYOU_OPERA_TYPE_RANFANG_YANHUA].buy_times = buy_times;

		if (!PugiGetSubNodeValue(buyXunyouOperaElement, "meigui_need_gold", need_gold) || need_gold <= 0)
		{
			*err = path + ": xml node error in [buy_xunyou_opera_cfg->meigui_need_gold]";
			return false;
		}

		if (!PugiGetSubNodeValue(buyXunyouOperaElement, "meigui_buy_times", buy_times) || buy_times <= 0)
		{
			*err = path + ": xml node error in [buy_xunyou_opera_cfg->meigui_buy_times]";
			return false;
		}

		m_buy_xunyou_cfg_list[MARRY_XUNYOU_OPERA_TYPE_MEIGUI_BIAOBAI].need_gold = need_gold;
		m_buy_xunyou_cfg_list[MARRY_XUNYOU_OPERA_TYPE_MEIGUI_BIAOBAI].buy_times = buy_times;
	}

	return true;
}

bool MarryConfig::GetMarryLevelCost(int marry_level, int *need_intimacy, int *need_coin, int *need_gold)
{
	if (marry_level <= MARRY_LEVEL_TYPE_INVALID || marry_level >= MARRY_LEVEL_TYPE_MAX) return false;

	if (NULL != need_intimacy) *need_intimacy = m_marry_level_cfg_list[marry_level].need_intimacy;
	if (NULL != need_coin) *need_coin = m_marry_level_cfg_list[marry_level].need_coin;
	if (NULL != need_gold) *need_gold = m_marry_level_cfg_list[marry_level].need_gold;

	return true;
}

int MarryConfig::GetMarryXunyouOperaCount(int opera_type)
{
	if (opera_type <= MARRY_XUNYOU_OPERA_TYPE_INVALID || opera_type >= MARRY_XUNYOU_OPERA_TYPE_MAX) return 0;

	return m_marry_xunyou_opera_count_list[opera_type];
}

int MarryConfig::GetMarryXunyouPointStopTimeS(int point_index)
{
	if (point_index < 0 || point_index >= m_point_count) return 0;

	return m_marry_xunyou_point_list[point_index].stop_time;
}

const Posi MarryConfig::GetMarryXunyouPointPos(int point_index)
{
	if (point_index < 0 || point_index >= m_point_count) return Posi(0, 0);

	return m_marry_xunyou_point_list[point_index].pos;
}

bool MarryConfig::GetMarryHunyanEnterPos(int *scene_id, Posi *pos)
{
	if (NULL == scene_id || NULL == pos) return false;

	*scene_id = m_marry_hunyan_cfg.scene_id;
	*pos = m_marry_hunyan_cfg.enter_pos;

	return true;
}

MarryHunyanBlessGift * MarryConfig::GetMarryHunyanBlessGift(int index)
{
	if (index < 0 || index >= m_marry_hunyan_cfg.bless_gift_count) return NULL;

	return &m_marry_hunyan_cfg.bless_gift_list[index];
}

int MarryConfig::GetMarryLevelReward(int marry_level, char prof, char sex, ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM])
{
	UNSTD_STATIC_CHECK(MARRY_REWARD_ITEM_MAX_NUM <= MAX_ATTACHMENT_ITEM_NUM);

	if (marry_level <= MARRY_LEVEL_TYPE_INVALID || marry_level >= MARRY_LEVEL_TYPE_MAX 
		|| prof <= PROF_TYPE_INVALID || prof >= PROF_TYPE_MAX || sex < FEMALE || sex > MALE) return 0;

	if (prof > PROF_TYPE_PROF_ZHUANZHI_BASE) prof -= PROF_TYPE_PROF_ZHUANZHI_BASE;

	int reward_count = m_marry_level_cfg_list[marry_level].reward_count;

	for (int i = 0; i < reward_count; i++)
	{
		reward_item_list[i] = m_marry_level_cfg_list[marry_level].reward_item_list[i];
	}

	if (reward_count < MAX_ATTACHMENT_ITEM_NUM)
	{
		for (int i = 0; i < m_marry_level_cfg_list[marry_level].reward_shizhuang_count; i++)
		{
			MarryLevelShizhuangCfg *shizhuang = &m_marry_level_cfg_list[marry_level].reward_shizhuang_list[i];
			if (shizhuang->prof == prof && shizhuang->sex == sex)
			{
				reward_item_list[reward_count] = shizhuang->shizhuang_item;
				++ reward_count;
				break;
			}
		}
	}

	return reward_count;
}

ItemID MarryConfig::GetMarryLevelHunJieItemID(int marry_level)
{
	if (marry_level <= MARRY_LEVEL_TYPE_INVALID || marry_level >= MARRY_LEVEL_TYPE_MAX) return 0;

	for (int i = 0; i < m_marry_level_cfg_list[marry_level].reward_count; i++)
	{
		ItemConfigData &item_data = m_marry_level_cfg_list[marry_level].reward_item_list[i];

		const ItemBase *item_base = ITEMPOOL->GetItem(item_data.item_id);
		if (NULL == item_base) continue;
		
		if (ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType())
		{
			const Equipment *equipment = (const Equipment*)item_base;

			{
				return equipment->GetItemId();
			}
		}
	}

	return 0;
}

int MarryConfig::GetMarryXunyouMarryobjDis(int marryobj_seq)
{
	if (marryobj_seq < 0 || marryobj_seq >= m_marry_xunyou_marryobj_count) return 0;

	return m_marry_xunyou_marryobj_dis_list[marryobj_seq];
}

bool MarryConfig::GetBuyXunyouOperaCfg(int op_type, int *need_gold, int *buy_times)
{
	if (op_type <= MARRY_XUNYOU_OPERA_TYPE_INVALID || op_type >= MARRY_XUNYOU_OPERA_TYPE_MAX || 
		NULL == need_gold || NULL == buy_times)
	{
		return false;
	}

	*need_gold = m_buy_xunyou_cfg_list[op_type].need_gold;
	*buy_times = m_buy_xunyou_cfg_list[op_type].buy_times;

	return true;
}
