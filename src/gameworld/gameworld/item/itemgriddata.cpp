#include "itemgriddata.h"

#include "item/itembase.h"
#include "item/itempool.h"
#include "item/itemextern.hpp"
#include "equipment/equipment.h"
#include "servercommon/configcommon.h"
#include "config/logicconfigmanager.hpp"
#include "other/zhuanshengequip/zhuanshengequipconfig.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ItemConfigData::ReadConfig(const PugiXmlNode &element)
{
	if (PugiGetSubNodeValue(element, "item_id", item_id) && 0 != item_id)
	{
		if (NULL == ITEMPOOL->GetItem(item_id))
		{
			return false;
		}

		if (!PugiGetSubNodeValue(element, "is_bind", is_bind))
		{
			return false;
		}

		if (!PugiGetSubNodeValue(element, "num", num) || num <= 0)
		{
			return false;
		}

		if (!ItemBase::CanMerge(item_id) && num > 1)
		{
			return false;
		}
	}

	return true;
}

int ItemConfigData::ReadConfigList(const PugiXmlNode & element, const std::string & element_name, ItemConfigData * item_list, int max_count)
{
	if (element.empty() || NULL == item_list)
	{
		return -1;
	}

	int item_count = 0;

	PugiXmlNode item_list_element = element.child((element_name + "_list").c_str());
	if (item_list_element.empty())
	{
		return -2;
	}

	if (!item_list_element.empty())
	{
		PugiXmlNode item_element = item_list_element.child(element_name.c_str());
		while (!item_element.empty())
		{
			if (item_count >= max_count)
			{
				return -3;
			}

			ItemConfigData &cfg = item_list[item_count];

			if (PugiGetSubNodeValue(item_element, "item_id", cfg.item_id) && 0 != cfg.item_id)
			{
				if (NULL == ITEMPOOL->GetItem(cfg.item_id))
				{
					return -4;
				}

				if (!PugiGetSubNodeValue(item_element, "is_bind", cfg.is_bind))
				{
					return -5;
				}

				if (!PugiGetSubNodeValue(item_element, "num", cfg.num) || cfg.num <= 0)
				{
					return -6;
				}

				if (!ItemBase::CanMerge(cfg.item_id) && cfg.num > 1)
				{
					return -7;
				}

				item_count++;
			}

			item_element = item_element.next_sibling();
		}
	}

	return item_count;
}

int ItemConfigData::ReadConfigByName(const PugiXmlNode & data_element, const char *element_name)
{
	PugiXmlNode item_element = data_element.child(element_name);
	if (NULL == item_element)
	{
		return -10000000;
	}

	if (!this->ReadConfig(item_element))
	{
		return -10000000 - this->item_id;
	}

	return 0;
}

int ItemConfigData::ReadConfigByName(const PugiXmlNode & data_element, const std::string &element_name)
{
	return this->ReadConfigByName(data_element, element_name.c_str());
}

bool ItemConfigData::ReadConfig(TiXmlElement *element)
{
	if (GetSubNodeValue(element, "item_id", item_id) && 0 != item_id)
	{
		if (NULL == ITEMPOOL->GetItem(item_id))
		{
			return false;
		}

		if (!GetSubNodeValue(element, "is_bind", is_bind))
		{
			return false;
		}

		if (!GetSubNodeValue(element, "num", num) || num <= 0)
		{
			return false;
		}

		if (!ItemBase::CanMerge(item_id) && num > 1)
		{
			return false;
		}
	}

	return true;
}

int ItemConfigData::ReadConfigList(TiXmlElement *element, const std::string &element_name, ItemConfigData *item_list, int max_count)
{
	if (NULL == element || NULL == item_list)
	{
		return -1;
	}

	int item_count = 0;

	TiXmlElement *item_list_element = element->FirstChildElement(element_name + "_list");
	if (NULL == item_list_element)
	{
		return -2;
	}

	if (NULL != item_list_element)
	{
		TiXmlElement *item_element = item_list_element->FirstChildElement(element_name);
		while (NULL != item_element)
		{
			if (item_count >= max_count)
			{
				return -3;
			}

			ItemConfigData &cfg = item_list[item_count];

			if (GetSubNodeValue(item_element, "item_id", cfg.item_id) && 0 != cfg.item_id)
			{
				if (NULL == ITEMPOOL->GetItem(cfg.item_id))
				{
					return -4;
				}

				if (!GetSubNodeValue(item_element, "is_bind", cfg.is_bind))
				{	
					return -5;
				}

				if (!GetSubNodeValue(item_element, "num", cfg.num) || cfg.num <= 0)
				{	
					return -6;
				}

				if (!ItemBase::CanMerge(cfg.item_id) && cfg.num > 1)
				{
					return -7;
				}

				item_count++;
			}

			item_element = item_element->NextSiblingElement(element_name);
		}
	}

	return item_count;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ItemGridData::Clear()
{
	if (NULL != param) delete param;
	this->Reset();
}

bool ItemGridData::Set(const ItemDataWrapper &item_data_wrapper, int rand_attr_put_type)
{
	if (item_data_wrapper.num <= 0) return false;
	if (NULL != param) return false;		// 绝对不允许

	const ItemBase *item = ITEMPOOL->GetItem(item_data_wrapper.item_id);
	if (NULL == item) return false;

	item_id = item_data_wrapper.item_id;
	is_bind = (0 != item_data_wrapper.is_bind);
	invalid_time = item_data_wrapper.invalid_time;
	gold_price = item_data_wrapper.gold_price;
	
	if (!is_bind && item->IsBind()) is_bind = true;

	if (ItemBase::I_TYPE_EQUIPMENT != item->GetItemType())
	{
		num = (item_data_wrapper.num >= item->GetPileLimit()) ? item->GetPileLimit() : item_data_wrapper.num;  // 不能超过物品叠加上限 
	}
	else
	{
		num = 1; // 装备不允许叠加 

		if (!item_data_wrapper.has_param)
		{
			param = item->CreateParam();

			EquipmentParam *equipment_param = (EquipmentParam *)param;

			// 进入背包前加上幸运属性
			const ItemBase *item_base = ITEMPOOL->GetItem(item_data_wrapper.item_id);
			if (NULL != item_base && ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType())
			{
				Equipment *equip = (Equipment *)item_base;
				if (Equipment::E_TYPE_ZHUANSHENG_MIN <= equip->GetEquipType() && equip->GetEquipType() < Equipment::E_TYPE_ZHUANSHENG_MAX)
				{
					int rand_attr_count = LOGIC_CONFIG->GetZhuanShengCfg().GetZhuanshengRandAttrCount(rand_attr_put_type);
					char rand_attr_type_list[ItemParamDataStruct::RAND_ATTR_NUM] = {0};
					LOGIC_CONFIG->GetZhuanShengCfg().GetZhuanshengRandAttrType(rand_attr_count, rand_attr_type_list);

					for (int i = 0; i < rand_attr_count && i < ItemParamDataStruct::RAND_ATTR_NUM; i++)
					{
						if (0 == i)
						{
							equipment_param->rand_attr_type_1 = rand_attr_type_list[i];
							equipment_param->rand_attr_val_1 = LOGIC_CONFIG->GetZhuanShengCfg().GetZhuanshengRandAttrValue(equip->GetLimitLevel(), equip->GetColor(), equipment_param->rand_attr_type_1);
						}
						else if (1 == i)
						{
							equipment_param->rand_attr_type_2 = rand_attr_type_list[i];
							equipment_param->rand_attr_val_2 = LOGIC_CONFIG->GetZhuanShengCfg().GetZhuanshengRandAttrValue(equip->GetLimitLevel(), equip->GetColor(), equipment_param->rand_attr_type_2);
						}
						else if (2 == i)
						{
							equipment_param->rand_attr_type_3 = rand_attr_type_list[i];
							equipment_param->rand_attr_val_3 = LOGIC_CONFIG->GetZhuanShengCfg().GetZhuanshengRandAttrValue(equip->GetLimitLevel(), equip->GetColor(), equipment_param->rand_attr_type_3);
						}
					}
				}
			}
		}
		else
		{
			param = item->CreateParamFromStruct(&item_data_wrapper.param_data);
		}
	}

	return true;
}

bool ItemGridData::WrapTo(ItemDataWrapper *item_data_wrapper) const
{
	if (NULL == item_data_wrapper) return false;

	if (NULL != param)
	{
		EquipmentParam *equip_param = (EquipmentParam *)param;
		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_id);
		if (NULL == equip_param || NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT) return false;

		if (!equip_param->SetInStructData(&item_data_wrapper->param_data)) return false;
	}
	else
	{
		item_data_wrapper->param_data.Reset();
	}

	item_data_wrapper->item_id = item_id;
	item_data_wrapper->num = num;
	item_data_wrapper->is_bind = (short)is_bind;
	item_data_wrapper->invalid_time = invalid_time;
	item_data_wrapper->has_param = (NULL != param) ? 1 : 0;
	item_data_wrapper->gold_price = gold_price;

	return true;
}

bool ItemGridData::CanBeMerge(unsigned int merge_invalid_time) const
{
	const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	if (NULL == itembase) return false;

	if (ItemBase::INVALID_ITEM_ID == item_id || num <= 0 || num >= itembase->GetPileLimit() || !ItemBase::CanMerge(item_id) || NULL != param || invalid_time != merge_invalid_time)
	{
		return false;
	}

	return true;
}

int ItemGridData::Merge(const ItemDataWrapper &item_data_wrapper, int *rest_num, int *merged_gold_price, bool ignore_bind)
{
	const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	if (NULL == itembase) return false;

	if (item_id != item_data_wrapper.item_id || !this->CanBeMerge(item_data_wrapper.invalid_time) || (!ignore_bind && (is_bind != (0 != item_data_wrapper.is_bind))) ) return ITEM_MERGE_CANNOT;

	int ret = ITEM_MERGE_NOT_COMPLETE;
	int tmp_rest_num = 0;

	num += item_data_wrapper.num;
	if (num <= itembase->GetPileLimit()) 
	{
		tmp_rest_num = 0;
		ret = ITEM_MERGE_COMPLETE;
	}
	else
	{
		tmp_rest_num = num - itembase->GetPileLimit();
		num = itembase->GetPileLimit();
	}

	if (NULL != rest_num) *rest_num = tmp_rest_num;

	if (item_data_wrapper.gold_price > 0)
	{
		int tmp_merge_gold_price = ItemExtern::GetGoldPriceByNum(item_data_wrapper.item_id, item_data_wrapper.num, item_data_wrapper.gold_price, item_data_wrapper.num - tmp_rest_num);
		gold_price += tmp_merge_gold_price;

		if (NULL != merged_gold_price) *merged_gold_price = tmp_merge_gold_price;
	}

	return ret;
}

int ItemGridData::Merge(const ItemGridData &itemdata, int *rest_num, int *merged_gold_price, bool ignore_bind)
{
	if (NULL != param || NULL != itemdata.param || invalid_time > 0 || itemdata.invalid_time > 0) return ITEM_MERGE_CANNOT;

	static ItemDataWrapper item_wrapper;
	item_wrapper.item_id = itemdata.item_id;
	item_wrapper.num = itemdata.num;
	item_wrapper.is_bind = itemdata.is_bind;
	item_wrapper.gold_price = itemdata.gold_price;

	return Merge(item_wrapper, rest_num, merged_gold_price, ignore_bind);
}


