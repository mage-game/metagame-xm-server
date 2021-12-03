#include "cardzuconfig.hpp"
#include "obj/character/role.h"
#include "item/itemextern.hpp"
#include "item/itempool.h"
#include "world.h"
#include <servercommon/configcommon.h>

CardzuConfig::CardzuConfig() : m_max_card_id(0), m_max_zuhe_id(0) 
{

}

CardzuConfig::~CardzuConfig()
{

}

bool CardzuConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "CardzuConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 卡牌信息
		PugiXmlNode root_element = RootElement.child("card_info");
		if (root_element.empty())
		{
			*err = configname + ": no [card_info].";
			return false;
		}

		iRet = this->InitCardInfoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCardInfoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 卡牌组合信息
		PugiXmlNode root_element = RootElement.child("zuhe_info");
		if (root_element.empty())
		{
			*err = configname + ": no [zuhe_info].";
			return false;
		}

		iRet = this->InitZuheInfoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitZuheInfoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 卡牌组合升级
		PugiXmlNode root_element = RootElement.child("cardzu_upgrade");
		if (root_element.empty())
		{
			*err = configname + ": no [cardzu_upgrade].";
			return false;
		}

		iRet = this->InitZuheUpgradeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitZuheUpgradeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 卡组信息
		PugiXmlNode root_element = RootElement.child("cardzu_info");
		if (root_element.empty())
		{
			*err = configname + ": no [cardzu_info].";
			return false;
		}

		iRet = this->InitCardzuInfoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCardzuInfoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const int CardzuConfig::GetCardzuIDWithCardID(int card_idx) const
{
	if (card_idx >= 0 && card_idx <= m_max_card_id)
	{
		return m_card_cfglist[card_idx].cardzu_id;
	}

	return -1;
}

const int CardzuConfig::GetCardzuIDWithZuheID(int zuhe_idx) const
{
	if (zuhe_idx >= 0 && zuhe_idx <= m_max_zuhe_id)
	{
		return m_zuhe_cfglist[zuhe_idx].cardzu_id;
	}

	return -1;
}

int CardzuConfig::GetCardzuNeedActiveCount(int cardzu_id)
{
	if (cardzu_id < 0 || cardzu_id >= CARDZU_TYPE_MAX_COUNT)
	{
		return 999999999;
	}

	CardzuActiveConfig &cfg = m_cardzu_active_cfglist[cardzu_id];
	return cfg.need_active_count;
}

const CardzuCardConfig *CardzuConfig::GetCardCfg(int card_idx)
{
	if (card_idx < 0 || card_idx > m_max_card_id)
	{
		return NULL;
	}

	return &m_card_cfglist[card_idx];
}

const CardzuZuheConfig *CardzuConfig::GetZuheCfg(int zuhe_idx)
{
	if (zuhe_idx < 0 || zuhe_idx > m_max_zuhe_id)
	{
		return NULL;
	}

	return &m_zuhe_cfglist[zuhe_idx];
}

struct RollCardItem
{
	RollCardItem() : card_id(-1), weight(0) {}

	int card_id;
	int weight;
};

const CardzuCardConfig *CardzuConfig::GetRandomCard(int choutcard_type, int cardzu_id, int card_star_num_min, int card_star_num_max)
{
	if (cardzu_id < 0 || cardzu_id >= CARDZU_TYPE_MAX_COUNT)
	{
		return NULL;
	}

	if (card_star_num_min <= 0 || card_star_num_max > CARDZU_CARD_STAR_MAX_COUNT || card_star_num_max < card_star_num_min)
	{
		return NULL;
	}
	
	int all_weight = 0;
	std::vector<RollCardItem> card_list;

	for (CardzuCardSetIt it = m_cardzu_cardset_list[cardzu_id].begin(); it != m_cardzu_cardset_list[cardzu_id].end(); ++ it)
	{
		CardzuCardConfig *card_cfg = &m_card_cfglist[*it];
		if (NULL != card_cfg)
		{
			if (card_cfg->star_count >= card_star_num_min && card_cfg->star_count <= card_star_num_max)
			{
				RollCardItem roll_item;
				roll_item.card_id = card_cfg->card_id;

				switch(choutcard_type)
				{
				case CARDZU_CHOUCARD_TYPE_COIN:
					{
						roll_item.weight = card_cfg->coin_weight;
					}
					break;

				case CARDZU_CHOUCARD_TYPE_GOLD:
					{
						roll_item.weight = card_cfg->gold_weight;
					}
					break;

				case CARDZU_CHOUCARD_TYPE_GOLD_BATCH:
					{
						roll_item.weight = card_cfg->gold_batch_weight;
					}
					break;

				case CARDZU_CHOUCARD_TYPE_GOLD_BIND:
					{
						roll_item.weight = card_cfg->gold_bind_weight;
					}
					break;
				}

				all_weight += roll_item.weight;
				card_list.push_back(roll_item);
			}
		}
	}

	if (all_weight <= 0)
	{
		return NULL;
	}

	int rand_weight = RandomNum(all_weight);

	for (std::vector<RollCardItem>::const_iterator it = card_list.begin(); it != card_list.end(); ++ it)
	{
		int cur_weight = it->weight;
		if (rand_weight < cur_weight)
		{
			return this->GetCardCfg(it->card_id);
		}

		rand_weight -= cur_weight;
	}

	return NULL;
}

const CardzuZuheConfig::AttributeConfig *CardzuConfig::GetZuheAttributeCfg(int zuhe_idx, int zuhe_level)
{
	if (zuhe_idx >= 0 && zuhe_idx <= m_max_zuhe_id)
	{
		CardzuZuheConfig &cfg = m_zuhe_cfglist[zuhe_idx];

		if (zuhe_level > 0 && zuhe_level <= cfg.zuhe_max_level)
		{
			return &cfg.attribute_cfglist[zuhe_level];
		}
	}

	return NULL;
}

CardzuConfig::CardzuZuheSet *CardzuConfig::GetZuheSet(int cardzu_id)
{
	if (cardzu_id < 0 || cardzu_id >= CARDZU_TYPE_MAX_COUNT)
	{
		return NULL;
	}

	return &m_cardzu_zuheset_list[cardzu_id];
}

int CardzuConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "choucard_need_coin", m_other_cfg.choucard_need_coin) || m_other_cfg.choucard_need_coin <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "choucard_need_gold", m_other_cfg.choucard_need_gold) || m_other_cfg.choucard_need_gold <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "choucard_batch_need_gold", m_other_cfg.choucard_batch_need_gold) || m_other_cfg.choucard_batch_need_gold <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "choucard_need_gold_bind", m_other_cfg.choucard_need_gold_bind) || m_other_cfg.choucard_need_gold_bind <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "coin_choucard_lingli", m_other_cfg.coin_choucard_lingli) || m_other_cfg.coin_choucard_lingli <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "gold_choucard_lingli", m_other_cfg.gold_choucard_lingli) || m_other_cfg.gold_choucard_lingli <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "batch_gold_choucard_lingli", m_other_cfg.batch_gold_choucard_lingli) || m_other_cfg.batch_gold_choucard_lingli <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "gold_bind_choucard_lingli", m_other_cfg.gold_bind_choucard_lingli) || m_other_cfg.gold_bind_choucard_lingli <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "coin_chouka_max_time", m_other_cfg.coin_chouka_max_time) || m_other_cfg.coin_chouka_max_time <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "gold_bind_chouka_max_time", m_other_cfg.gold_bind_chouka_max_time) || m_other_cfg.gold_bind_chouka_max_time <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "choucard_batch_need_item_id", m_other_cfg.choucard_batch_need_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.choucard_batch_need_item_id))
	{
		return -11;
	}

	return 0;
}

int CardzuConfig::InitCardzuInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int cardzu_active_cfgcount = 0;

	while (!data_element.empty())
	{
		if (cardzu_active_cfgcount >= CARDZU_TYPE_MAX_COUNT)
		{
			return -1;
		}

		CardzuActiveConfig &cfg = m_cardzu_active_cfglist[cardzu_active_cfgcount];

		if (!PugiGetSubNodeValue(data_element, "cardzu_id", cfg.cardzu_id) || cfg.cardzu_id != cardzu_active_cfgcount)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "need_active_count", cfg.need_active_count) || cfg.need_active_count < 0)
		{
			return -3;
		}

		++ cardzu_active_cfgcount;
		data_element = data_element.next_sibling();
	}

	if (CARDZU_TYPE_MAX_COUNT != cardzu_active_cfgcount)
	{
		return -20000;
	}

	if (m_cardzu_active_cfglist[0].need_active_count > 0)
	{
		return -20001;
	}

	for (int i = 1; i < CARDZU_TYPE_MAX_COUNT; ++ i)
	{
		if (m_cardzu_active_cfglist[i].need_active_count > (int)m_cardzu_cardset_list[i - 1].size())
		{
			return -20002;
		}
	}

	return 0;
}

int CardzuConfig::InitCardInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_max_card_id = -1;

	while (!data_element.empty())
	{
		if (m_max_card_id >= CARDZU_MAX_CARD_ID)
		{
			return -1;
		}

		int card_id = 0;
		if (!PugiGetSubNodeValue(data_element, "card_idx", card_id) || card_id != m_max_card_id + 1)
		{
			return -2;
		}
		m_max_card_id = card_id;

		CardzuCardConfig &cfg = m_card_cfglist[card_id];
		cfg.card_id = card_id;

		if (!PugiGetSubNodeValue(data_element, "cardzu_id", cfg.cardzu_id) || cfg.cardzu_id < 0 || cfg.cardzu_id >= CARDZU_TYPE_MAX_COUNT)
		{
			return -3;
		}

		m_cardzu_cardset_list[cfg.cardzu_id].insert(cfg.card_id);

		if (!PugiGetSubNodeValue(data_element, "star_count", cfg.star_count) || cfg.star_count <= 0 || cfg.star_count > CARDZU_CARD_STAR_MAX_COUNT)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "got_lingli", cfg.sell_lingli) || cfg.sell_lingli <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "need_lingli", cfg.need_lingli) || cfg.need_lingli <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "coin_weight", cfg.coin_weight) || cfg.coin_weight < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "gold_weight", cfg.gold_weight) || cfg.gold_weight < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "gold_batch_weight", cfg.gold_batch_weight) || cfg.gold_batch_weight < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "gold_bind_weight", cfg.gold_bind_weight) || cfg.gold_bind_weight < 0)
		{
			return -11;
		}

		int broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "broadcast", broadcast) || broadcast < 0)
		{
			return -12;
		}
		cfg.broadcast = (0 != broadcast);

		data_element = data_element.next_sibling();
	}

	for (int cardzu_index = 0; cardzu_index < CARDZU_TYPE_MAX_COUNT; ++ cardzu_index)
	{
		if (m_cardzu_cardset_list[cardzu_index].size() <= 0)
		{
			return -20000;
		}
	}

	return 0;
}

int CardzuConfig::InitZuheInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_max_zuhe_id = -1;

	while (!data_element.empty())
	{
		if (m_max_zuhe_id >= CARDZU_MAX_ZUHE_ID)
		{
			return -1;
		}

		int zuhe_id = 0;
		if (!PugiGetSubNodeValue(data_element, "zuhe_idx", zuhe_id) || zuhe_id != m_max_zuhe_id + 1)
		{
			return -2;
		}
		m_max_zuhe_id = zuhe_id;

		CardzuZuheConfig &cfg = m_zuhe_cfglist[zuhe_id];
		cfg.zuhe_id = zuhe_id;

		if (!PugiGetSubNodeValue(data_element, "cardzu_id", cfg.cardzu_id) || cfg.cardzu_id < 0 || cfg.cardzu_id >= CARDZU_TYPE_MAX_COUNT)
		{
			return -3;
		}

		m_cardzu_zuheset_list[cfg.cardzu_id].insert(cfg.zuhe_id);

		{
			// 第一个卡牌必须要有，其他可选
			if (!PugiGetSubNodeValue(data_element, "need_card1_id", cfg.need_card1_id) || this->GetCardzuIDWithCardID(cfg.need_card1_id) != cfg.cardzu_id)
			{
				return -5;
			}

			if (!PugiGetSubNodeValue(data_element, "need_card1_num", cfg.need_card1_num) || cfg.need_card1_num <= 0)
			{
				return -6;
			}
		}
		
		{
			// 第二个卡牌
			PugiGetSubNodeValue(data_element, "need_card2_id", cfg.need_card2_id);
			PugiGetSubNodeValue(data_element, "need_card2_num", cfg.need_card2_num);

			if (cfg.need_card2_num > 0)
			{
				if (GetCardzuIDWithCardID(cfg.need_card2_id) != cfg.cardzu_id)
				{
					return -7;
				}
			}
			else
			{
				cfg.need_card2_id = 0;
				cfg.need_card2_num = 0;
			}
		}

		{
			PugiGetSubNodeValue(data_element, "need_card3_id", cfg.need_card3_id);
			PugiGetSubNodeValue(data_element, "need_card3_num", cfg.need_card3_num);

			if (cfg.need_card3_num > 0)
			{
				if (GetCardzuIDWithCardID(cfg.need_card3_id) != cfg.cardzu_id)
				{
					return -9;
				}
			}
			else
			{
				cfg.need_card3_id = 0;
				cfg.need_card3_num = 0;
			}
		}

		data_element = data_element.next_sibling();
	}

	for (int cardzu_index = 0; cardzu_index < CARDZU_TYPE_MAX_COUNT; ++ cardzu_index)
	{
		if (m_cardzu_zuheset_list[cardzu_index].size() <= 0) // 每个卡组都要有卡牌组合
		{
			return -20000;
		}
	}

	return 0;
}

int CardzuConfig::InitZuheUpgradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int zuhe_last_need_lingli[CARDZU_MAX_ZUHE_ID + 1]; memset(zuhe_last_need_lingli, 0, sizeof(zuhe_last_need_lingli));

	while (!data_element.empty())
	{
		int zuhe_id = 0;

		if (!PugiGetSubNodeValue(data_element, "zuhe_idx", zuhe_id) || zuhe_id < 0 || zuhe_id > m_max_zuhe_id)
		{
			return -1;
		}

		CardzuZuheConfig &cfg = m_zuhe_cfglist[zuhe_id];

		if (cfg.zuhe_max_level >= CARDZU_ZUHE_MAX_LEVEL)
		{
			return -2;
		}

		int zuhe_level = 0;
		if (!PugiGetSubNodeValue(data_element, "zuhe_level", zuhe_level) || zuhe_level != cfg.zuhe_max_level + 1)
		{
			return -3;
		}
		cfg.zuhe_max_level = zuhe_level;

		CardzuZuheConfig::AttributeConfig &sub_cfg = cfg.attribute_cfglist[zuhe_level];
		sub_cfg.zuhe_id = zuhe_id;

		if (!PugiGetSubNodeValue(data_element, "upgrade_need_lingli", sub_cfg.upgrade_need_lingli) || sub_cfg.upgrade_need_lingli <= zuhe_last_need_lingli[zuhe_id])
		{
			return -4;
		}
		zuhe_last_need_lingli[zuhe_id] = sub_cfg.upgrade_need_lingli;

		if (!PugiGetSubNodeValue(data_element, "maxhp", sub_cfg.maxhp) || sub_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", sub_cfg.gongji) || sub_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", sub_cfg.fangyu) || sub_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", sub_cfg.mingzhong) || sub_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", sub_cfg.shanbi) || sub_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", sub_cfg.baoji) || sub_cfg.baoji < 0)
		{
			return -105;
		}
		
		if (!PugiGetSubNodeValue(data_element, "jianren", sub_cfg.jianren) || sub_cfg.jianren < 0)
		{
			return -106;
		}

		data_element = data_element.next_sibling();
	}

	for (int zuhe_index = 0; zuhe_index <= m_max_zuhe_id; ++ zuhe_index)
	{
		if (m_zuhe_cfglist[zuhe_index].zuhe_max_level <= 0) // 所有卡牌组合都必须有对应属性配置
		{
			return -20000;
		}
	}

	return 0;
}