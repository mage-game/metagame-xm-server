#include "dropconfig.hpp"

#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"

#include "item/itempool.h"
#include "item/itemgriddata.h"
#include "item/itembase.h"
#include "equipment/equipment.h"

DropConfig::DropConfig()
	: m_drop_id(0), m_drop_item_total_count(0), m_drop_item_prob_count(0)
{

}

DropConfig::~DropConfig()
{

}

bool DropConfig::LoadConfig(const char *path, std::string *err)
{
	char errinfo[1024] = {0};

	TiXmlDocument document;

	if (NULL == path || !document.LoadFile(path))
	{
		sprintf(errinfo,"%s: Load Drop Config Error, %s.", path, document.ErrorDesc());
		*err = errinfo;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		sprintf(errinfo, "%s: xml node error in root.", path);
		*err = errinfo;
		return false;
	}

	if (!GetSubNodeValue(RootElement, "drop_id", m_drop_id) || m_drop_id <= 0)
	{
		sprintf(errinfo,"%s: xml node error in node [droper->drop_id(%d)]", path, m_drop_id);
		*err = errinfo;
		return false;
	}

	{
		TiXmlElement *drop_item_prob_list_Element = RootElement->FirstChildElement("drop_item_prob_list");
		if (!drop_item_prob_list_Element)
		{
			sprintf(errinfo,"%s: xml node error in node [drop_item_prob_list].", path);
			*err = errinfo;
			return false;
		}

		int total_prob = 0;

		TiXmlElement *drop_item_prob_Element = drop_item_prob_list_Element->FirstChildElement("drop_item_prob");
		while (NULL != drop_item_prob_Element)
		{
			if (m_drop_item_total_count >= DROP_ITEM_PROB_MAX)
			{
				sprintf(errinfo,"%s: xml node error in node [droper->drop_item_prob_list->drop_item_prob_count(%d)]", path, m_drop_item_prob_count);
				*err = errinfo;
				return false;
			}

			DropItemCfg *drop_item_prob = &m_drop_item_prob_list[m_drop_item_total_count];
			if (!GetSubNodeValue(drop_item_prob_Element, "item_id", drop_item_prob->item_config_data.item_id))
			{
				sprintf(errinfo,"%s: xml node error in node [droper->drop_item_prob_list->drop_item_prob->item_id(%d)]", path, drop_item_prob->item_config_data.item_id);
				*err = errinfo;
				return false;
			}

			if (!GetSubNodeValue(drop_item_prob_Element, "num", drop_item_prob->item_config_data.num) || drop_item_prob->item_config_data.num <= 0)
			{
				sprintf(errinfo,"%s: xml node error in node [droper->drop_item_prob_list->drop_item_prob->num(%d),id(%d)]", path, (int)drop_item_prob->item_config_data.num, drop_item_prob->item_config_data.item_id);
				*err = errinfo;
				return false;
			}

			if (COIN_ITEM_ID != drop_item_prob->item_config_data.item_id)
			{
				const ItemBase *itembase = ITEMPOOL->GetItem(drop_item_prob->item_config_data.item_id);
				if (NULL == itembase)
				{
					sprintf(errinfo,"%s: xml node error in node [droper->drop_item_prob_list->drop_item_prob->item_id(%d) not exist]", path, (int)drop_item_prob->item_config_data.item_id);
					*err = errinfo;
					return false;
				}

				if (drop_item_prob->item_config_data.num > itembase->GetPileLimit())
				{
					sprintf(errinfo,"%s: xml node error in node [droper->drop_item_prob_list->drop_item_prob->num(%d),id(%d)]", path, (int)drop_item_prob->item_config_data.num, drop_item_prob->item_config_data.item_id);
					*err = errinfo;
					return false;
				}

				if (!ItemBase::CanMerge(drop_item_prob->item_config_data.item_id) && drop_item_prob->item_config_data.num > 1)
				{
					sprintf(errinfo,"%s: xml node error in node [droper->drop_item_prob_list->drop_item_prob->num(%d)]", path, (int)drop_item_prob->item_config_data.num);
					*err = errinfo;
					return false;
				}
			}
			
			if (!GetSubNodeValue(drop_item_prob_Element, "is_bind", drop_item_prob->item_config_data.is_bind))
			{
				sprintf(errinfo,"%s: xml node error in node [droper->drop_item_prob_list->drop_item_prob->is_bind(%d)]", path, (int)drop_item_prob->item_config_data.is_bind);
				*err = errinfo;
				return false;
			}

			if (!GetSubNodeValue(drop_item_prob_Element, "prob", drop_item_prob->prob))
			{
				sprintf(errinfo,"%s: xml node error in node [droper->drop_item_prob_list->drop_item_prob->prob(%d)]", path, drop_item_prob->prob);
				*err = errinfo;
				return false;
			}

			//int is_broadcast = 0;
			if (!GetSubNodeValue(drop_item_prob_Element, "broadcast", drop_item_prob->broadcast))
			{
				sprintf(errinfo,"%s: xml node error in node [droper->drop_item_prob_list->drop_item_prob->broadcast(%d)]", path, drop_item_prob->broadcast);
				*err = errinfo;
				return false;
			}

			// 必掉物品的概率 填-1代表必掉落
			if (DROP_CERTAIN_PROB != drop_item_prob->prob)
			{
				drop_item_prob->prob += total_prob;
				total_prob = drop_item_prob->prob;

				++ m_drop_item_prob_count;
			}

			++ m_drop_item_total_count;
			if (m_drop_item_total_count != m_drop_item_prob_count && DROP_CERTAIN_PROB != drop_item_prob->prob)
			{
				sprintf(errinfo,"%s: xml node error in node [droper->drop_item_prob_list->drop_item_prob-> (m_drop_item_total_count != m_drop_item_prob_count && DROP_CERTAIN_PROB != drop_item_prob->prob)]", path);
				*err = errinfo;
				return false;
			}

			drop_item_prob_Element = drop_item_prob_Element->NextSiblingElement();
		}

		if (total_prob > PROBABILITY_TEN_MILLION)
		{
			sprintf(errinfo,"%s: xml node error in node [droper->drop_item_prob_list->total_prob(%d) more than %d]", path, total_prob, PROBABILITY_TEN_MILLION);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

// 随机掉落物品
int DropConfig::RandDropItem(char prof, DropItemCfg item_list[DROP_ITEM_PROB_MAX])
{
	int count = 0;

	{
		
		int item_rand_i = 0, item_rand_prob = RandomNum(PROBABILITY_TEN_MILLION);
		for (item_rand_i = 0; item_rand_i < m_drop_item_prob_count; ++ item_rand_i)
		{
			if (m_drop_item_prob_list[item_rand_i].prob > item_rand_prob)
			{
				break;
			}
		}
		if (item_rand_i < 0 || item_rand_i >= m_drop_item_prob_count) return 0;

// 		if(0 != prof)
// 		{
// 			//装备判断
// 			//不是本职业的装备换成对应职业的装备
// 			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_drop_item_prob_list[item_rand_i].item_config_data.item_id);
// 			if (NULL != equip && ItemBase::I_TYPE_EQUIPMENT == equip->GetItemType())
// 			{
// 				//装备限制职业
// 				if(PROF_TYPE_PROF_NO_LIMIT > equip->GetLimitProf() &&
// 					PROF_TYPE_INVALID < equip->GetLimitProf() &&
// 					prof != equip->GetLimitProf())
// 				{
// 					//四分之一概率
// 					if((RandomNum(10000)) >= 7500)
// 					{
// 						for (int i = 0; i < m_drop_item_prob_count; ++i)
// 						{
// 							const Equipment *new_equip = (const Equipment *)ITEMPOOL->GetItem(m_drop_item_prob_list[i].item_config_data.item_id);
// 							if(NULL != new_equip && ItemBase::I_TYPE_EQUIPMENT == new_equip->GetItemType())
// 							{
// 								if(new_equip->GetLimitProf() == prof && 
// 									equip->GetEquipType() == new_equip->GetEquipType() &&
// 									equip->GetLimitLevel() == new_equip->GetLimitLevel() &&
// 									equip->GetColor() == new_equip->GetColor())
// 								{
// 									item_rand_i = i;
// 									break;		
// 								}
// 							}
// 						}	
// 					}
// 				}
// 			}
// 		}
		// 如果不是自己职业的，有一定概率转换成自己职业
		// 1/3 + 2/3 * 1/4 = 0.5 (三职业)
		// 1/4 + 3/4 * 1/3 = 0.5 (四职业)(也就是有一半概率掉落的是自己的职业装备) --- 刘琪 
		if (0 != prof)
		{
			prof %= PROF_TYPE_PROF_ZHUANZHI_BASE;
			//装备判断
			//不是本职业的装备换成对应职业的装备
			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_drop_item_prob_list[item_rand_i].item_config_data.item_id);
			if (NULL != equip && ItemBase::I_TYPE_EQUIPMENT == equip->GetItemType())
			{
				//装备限制职业
				if (PROF_TYPE_PROF_NO_LIMIT > equip->GetLimitProf() &&
					PROF_TYPE_INVALID < equip->GetLimitProf() &&
					prof != equip->GetLimitProf())
				{
					if ((rand() % 10000) >= 6667)
					{
						for (int i = 0; i < m_drop_item_prob_count; ++i)
						{
							const Equipment *new_equip = (const Equipment *)ITEMPOOL->GetItem(m_drop_item_prob_list[i].item_config_data.item_id);
							if (NULL != new_equip && ItemBase::I_TYPE_EQUIPMENT == new_equip->GetItemType())
							{
								if (new_equip->GetLimitProf() == prof &&
									equip->GetEquipType() == new_equip->GetEquipType() &&
									equip->GetLimitLevel() == new_equip->GetLimitLevel() &&
									equip->GetColor() == new_equip->GetColor())
								{
									item_rand_i = i;
									break;
								}
							}
						}
					}
				}
			}
		}

		item_list[count ++] = m_drop_item_prob_list[item_rand_i];
	}

	return count;
}

// 固定掉落物品
int DropConfig::GetFixDropItem(int rand_count, DropItemCfg item_list[DROP_ITEM_PROB_MAX])
{
	if (NULL == item_list || rand_count < 0) return 0;
	if (rand_count >= DROP_ITEM_PROB_MAX) return rand_count;

	int count = rand_count;

	for (int i = m_drop_item_prob_count; i < m_drop_item_total_count; i++)
	{
		item_list[count++] = m_drop_item_prob_list[i];
		if (count >= DROP_ITEM_PROB_MAX)
		{
			break;
		}
	}

	return count;
}

int DropConfig::RandDropItemN(int n, DropItemCfg item_list[DROP_ITEM_PROB_MAX])
{
	if (n < 1)
		return 0;
	int count = 0;

	for (int item_rand_i = 0; item_rand_i < m_drop_item_prob_count; ++item_rand_i)
	{
		auto& drop_item = m_drop_item_prob_list[item_rand_i];

		// 计算单个的概率,为读配置的时候累加了概率
		int64_t tem_prob = (int64_t)drop_item.prob;

		if (item_rand_i > 0 && item_rand_i <= DROP_ITEM_PROB_MAX)
		{
			auto& last_drop_item = m_drop_item_prob_list[item_rand_i - 1];
			tem_prob = (int64_t)drop_item.prob - (int64_t)last_drop_item.prob;
		}

		if (tem_prob <= (int64_t)0)
		{
			continue;
		}
	
		int64_t n_prob = tem_prob * (int64_t)n;
		int item_count = 0;
		item_count += static_cast<int>(n_prob / PROBABILITY_TEN_MILLION);
		n_prob = n_prob % PROBABILITY_TEN_MILLION;
		auto rand_prob = RandomNum(PROBABILITY_TEN_MILLION);
		if (rand_prob < n_prob)
		{
			++item_count;
		}

		if (item_count > 0)
		{
			item_list[count] = drop_item;
			item_list[count].item_config_data.num *= item_count;
			++count;
		}
	}

	return count;
}