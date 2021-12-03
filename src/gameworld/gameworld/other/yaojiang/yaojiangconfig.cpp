#include "yaojiangconfig.hpp"

#include "obj/character/role.h"
#include "item/itempool.h"
#include "servercommon/errornum.h"

YaojiangConfig::YaojiangConfig()
{

}

YaojiangConfig::~YaojiangConfig()
{

}

bool YaojiangConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "YaojiangConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (!RootElement)
	{
		*err = path + ": xml root node error.";
		return false;
	}

	/*{
		PugiXmlNode CostItemElement = RootElement.child("cost_item");
		if (CostItemElement.empty())
		{
			*err = path + ": xml error in node [rand_num].";
			return false;
		}

		PugiXmlNode DataElement = CostItemElement.child("data");
		while (!DataElement.empty())
		{
			int type = 0;
			if (!PugiGetSubNodeValue(DataElement, "type", type) || type < 0 || m_yaojiang_item_pool_map.find(type) != m_yaojiang_item_pool_map.end())
			{
				*err = path + ": xml error in node [rand_num->data->type].";
				return false;
			}

			int cost_item_id = 0;
			if (!PugiGetSubNodeValue(DataElement, "cost_item_id", cost_item_id) || NULL == ITEMPOOL->GetItem(cost_item_id))
			{
				*err = path + ": xml error in node [rand_num->data->num].";
				return false;
			}

			m_yaojiang_item_pool_map[type].cost_item_id = cost_item_id;

			DataElement = DataElement.next_sibling();
		}
	}

	{
		PugiXmlNode YaojiangItemElement = RootElement.child("yaojiang_item");
		if (YaojiangItemElement.empty())
		{
			*err = path + ": xml error in node [yaojiang_item].";
			return false;
		}

		PugiXmlNode DataElement = YaojiangItemElement.child("data");
		while (!DataElement.empty())
		{
			int type = 0;
			if (!PugiGetSubNodeValue(DataElement, "type", type) || m_yaojiang_item_pool_map.find(type) == m_yaojiang_item_pool_map.end())
			{
				*err = path + ": xml error in node [yaojiang_item->data->type].";
				return false;
			}

			if (m_yaojiang_item_pool_map[type].item_count < 0 || m_yaojiang_item_pool_map[type].item_count >= YAOJIANG_ITEM_NUM)
			{
				*err = path + ": xml error in node [yaojiang_item->data->type] too much.";
				return false;
			}

			YaojiangItemCfg &cfg = m_yaojiang_item_pool_map[type].item_list[m_yaojiang_item_pool_map[type].item_count];

			if (!PugiGetSubNodeValue(DataElement, "weight", cfg.weight) || cfg.weight <= 0)
			{
				*err = path + ": xml error in node [yaojiang_item->data->weight].";
				return false;
			}
			m_yaojiang_item_pool_map[type].total_weight += cfg.weight;

			if (!PugiGetSubNodeValue(DataElement, "is_notice", cfg.is_notice))
			{
				*err = path + ": xml error in node [yaojiang_item->data->is_notice].";
				return false;
			}

			PugiXmlNode ItemElement = DataElement.child("item");
			if (ItemElement || !cfg.item.ReadConfig(ItemElement.empty()))
			{
				*err = path + ": xml error in node [yaojiang_item->data->item].";
				return false;
			}

			++m_yaojiang_item_pool_map[type].item_count;

			DataElement = DataElement.next_sibling();
		}

		for (YaojiangItemPoolMap::iterator it = m_yaojiang_item_pool_map.begin(); it != m_yaojiang_item_pool_map.end(); ++it)
		{
			if (YAOJIANG_ITEM_NUM != it->second.item_count || it->second.total_weight <= 0)
			{
				*err = path + ": xml error item_data_count != 8 || total_weight <= 0.";
				return false;
			}
		}
	}*/

	return true;
}

ItemID YaojiangConfig::GetCostItemId(int yaojiang_type)
{
	YaojiangItemPoolMap::iterator it = m_yaojiang_item_pool_map.find(yaojiang_type);
	if (it == m_yaojiang_item_pool_map.end())
	{
		return 0;
	}

	return it->second.cost_item_id;
}

const YaojiangItemCfg * YaojiangConfig::RandYaojiangItemCfg(int yaojiang_type, int *index)
{
	if (NULL == index)
	{
		return NULL;
	}

	YaojiangItemPoolMap::iterator it = m_yaojiang_item_pool_map.find(yaojiang_type);
	if (it == m_yaojiang_item_pool_map.end())
	{
		return NULL;
	}

	YaojiangItemPool &pool = it->second;
	if (pool.total_weight <= 0)
	{
		return NULL;
	}

	int rand_num = RandomNum(pool.total_weight);

	for (int i = 0; i < YAOJIANG_ITEM_NUM; ++i)
	{
		if (rand_num < pool.item_list[i].weight)
		{
			*index = i;
			return &pool.item_list[i];
		}

		rand_num -= pool.item_list[i].weight;
	}

	return NULL;
}

