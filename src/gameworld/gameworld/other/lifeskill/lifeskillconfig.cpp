#include "lifeskillconfig.hpp"

#include "servercommon/configcommon.h"

#include "checkresourcecenter.hpp"
#include "monster/monsterpool.h"
#include "item/itempool.h"
#include "scene/scene.h"
#include "item/itemgriddata.h"
#include "item/itembase.h"

LifeSkillConfig::LifeSkillConfig()
{

}

LifeSkillConfig::~LifeSkillConfig()
{

}

bool LifeSkillConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "LifeSkillConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	int iRet = 0;

	{
		PugiXmlNode elem = RootElement.child("skill");
		if (NULL == elem)
		{
			sprintf(errinfo,"%s:xml node error in node [config->skill]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitSkill(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitSkill fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode elem = RootElement.child("shuliandu");
		if (NULL == elem)
		{
			sprintf(errinfo,"%s:xml node error in node [config->shuliandu]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitShuliandu(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitShuliandu fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode elem = RootElement.child("make");
		if (NULL == elem)
		{
			sprintf(errinfo,"%s:xml node error in node [config->make]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitMake(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitMake fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode elem = RootElement.child("career_replace");
		if (NULL == elem)
		{
			sprintf(errinfo,"%s:xml node error in node [config->career_replace]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitProfReplace(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitProfReplace fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int LifeSkillConfig::InitSkill(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_level = 0;
	int last_type = 0;

	while (!data_element.empty())
	{
		int level = 0;
		int type = 0;
		int huoli = 0;

		if (!PugiGetSubNodeValue(data_element, "type", type) || type < 0 || type >= LIFE_SKIL_TYPE_MAX)
		{
			return -1;
		}

		if (last_type != type)
		{
			last_type = type;
			last_level = 0;
		}

		if (!PugiGetSubNodeValue(data_element, "level", level) || (1 != level - last_level))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "huoli", huoli) || huoli <= 0)
		{
			return -3;
		}

		if (level > LIFE_SKILL_LV_MAX)
		{
			return -4;
		}

		LifeSkillItemCfg &item_cfg = m_skill_map[type];
		item_cfg.type = type;
		item_cfg.need_huoli_list[level] = huoli;

		last_level = level;

		data_element = data_element.next_sibling();
	}

	if (last_level != LIFE_SKILL_LV_MAX)
	{
		return -5;
	}

	return 0;
}

int LifeSkillConfig::InitShuliandu(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_level = 0;
	int last_type = 0;

	while (!data_element.empty())
	{
		int level = 0;
		int type = 0;
		int shuliandu = 0;

		if (!PugiGetSubNodeValue(data_element, "type", type) || type < 0 || type >= LIFE_SKIL_TYPE_MAX)
		{
			return -1;
		}

		if (last_type != type)
		{
			last_type = type;
			last_level = 0;
		}

		if (!PugiGetSubNodeValue(data_element, "level", level) || (1 != level - last_level))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "shuliandu", shuliandu) || shuliandu <= 0)
		{
			return -3;
		}

		if (level > LIFE_SKILL_SHULIANDU_LV_MAX)
		{
			return -4;
		}

		LifeSkillShulianduItemCfg &item_cfg = m_shuliandu_map[type];
		item_cfg.type = type;
		item_cfg.need_shuliandu[level] = shuliandu;

		last_level = level;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int LifeSkillConfig::InitMake(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_skill_index = -1;
	while (!data_element.empty())
	{
		int index = 0;
		if (!PugiGetSubNodeValue(data_element, "index", index) || (1 != index - last_skill_index))
		{
			return -1;
		}
		last_skill_index = index;

		LifeSkillMakeItemCfg &item_cfg = m_make_map[index];
		item_cfg.index = index;

		if (!PugiGetSubNodeValue(data_element, "type", item_cfg.type) || item_cfg.type < 0 || item_cfg.type >= LIFE_SKIL_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff1", item_cfg.stuff1) || (item_cfg.stuff1 != 0 && NULL == ITEMPOOL->GetItem(item_cfg.stuff1)))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "count1", item_cfg.count1) || (item_cfg.stuff1 != 0 && item_cfg.count1 <= 0))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff2", item_cfg.stuff2) || (item_cfg.stuff2 != 0 && NULL == ITEMPOOL->GetItem(item_cfg.stuff2)))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "count2", item_cfg.count2) || (item_cfg.stuff2 != 0 && item_cfg.count2 <= 0))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "need_skill_level", item_cfg.need_skill_level) || item_cfg.need_skill_level > LIFE_SKILL_LV_MAX)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "need_huoli", item_cfg.need_huoli) || item_cfg.need_huoli <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "add_shuliandu", item_cfg.add_shuliandu) || item_cfg.add_shuliandu <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "shuliandu_xisu", item_cfg.shuliandu_xisu) || item_cfg.shuliandu_xisu <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "green_weight", item_cfg.green_weight) || item_cfg.green_weight < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "green_item_id", item_cfg.green_item_id) || NULL == ITEMPOOL->GetItem(item_cfg.green_item_id))
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "blue_weight", item_cfg.blue_weight) || item_cfg.blue_weight < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "blue_item_id", item_cfg.blue_item_id) || NULL == ITEMPOOL->GetItem(item_cfg.blue_item_id))
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "purple_weight", item_cfg.purple_weight) || item_cfg.purple_weight < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "purple_item_id", item_cfg.purple_item_id) || NULL == ITEMPOOL->GetItem(item_cfg.purple_item_id))
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "orange_weight", item_cfg.orange_weight) || item_cfg.orange_weight < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(data_element, "orange_item_id", item_cfg.orange_item_id) || NULL == ITEMPOOL->GetItem(item_cfg.orange_item_id))
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(data_element, "lucky_rate", item_cfg.lucky_rate) || item_cfg.lucky_rate >= 100)
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(data_element, "self_career_rate", item_cfg.self_prof_rate) || item_cfg.self_prof_rate >= 100)
		{
			return -19;
		}

		int total_weight = item_cfg.green_weight + item_cfg.blue_weight + item_cfg.purple_weight + item_cfg.orange_weight;
		if (total_weight <= 0 || total_weight > 100)
		{
			return -100;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int LifeSkillConfig::InitProfReplace(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ItemID make_item_id = 0;
		if (!PugiGetSubNodeValue(data_element, "make_item_id", make_item_id) || NULL == ITEMPOOL->GetItem(make_item_id))
		{
			return -1;
		}

		LifeSkillProfReplaceCfg &item_cfg = m_prof_replace_map[make_item_id];
		item_cfg.make_item_id = make_item_id;
		
		ItemID re_item_id1 = 0, re_item_id2 = 0, re_item_id3 = 0, re_item_id4 = 0;
		
		if (!PugiGetSubNodeValue(data_element, "career_replace1", re_item_id1) || NULL == ITEMPOOL->GetItem(re_item_id1))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "career_replace2", re_item_id2) || NULL == ITEMPOOL->GetItem(re_item_id2))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "career_replace3", re_item_id3) || NULL == ITEMPOOL->GetItem(re_item_id3))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "career_replace4", re_item_id4) || NULL == ITEMPOOL->GetItem(re_item_id4))
		{
			return -5;
		}
		
		UNSTD_STATIC_CHECK(LifeSkillProfReplaceCfg::PROF_NUM == 4);
		item_cfg.prof_replace_list[0] = re_item_id1;
		item_cfg.prof_replace_list[1] = re_item_id2;
		item_cfg.prof_replace_list[2] = re_item_id3;
		item_cfg.prof_replace_list[3] = re_item_id4;

		data_element = data_element.next_sibling();
	}

	return 0;
}


const LifeSkillItemCfg *LifeSkillConfig::GetSkillItemCfg(int type)
{
	LifeSkillMap::const_iterator iter = m_skill_map.find(type);
	if (m_skill_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const LifeSkillShulianduItemCfg *LifeSkillConfig::GetShulianduCfg(int type)
{
	ShulianduMap::const_iterator iter = m_shuliandu_map.find(type);
	if (m_shuliandu_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const LifeSkillMakeItemCfg *LifeSkillConfig::GetMakeCfg(int index)
{
	MakeMap::const_iterator iter = m_make_map.find(index);
	if (m_make_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const LifeSkillProfReplaceCfg *LifeSkillConfig::GetProfReplaceCfg(ItemID item_id)
{
	ProfReplaceMap::const_iterator iter = m_prof_replace_map.find(item_id);
	if (m_prof_replace_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

int LifeSkillConfig::GetTotalShuliandu(int type, int level)
{
	const LifeSkillShulianduItemCfg *item_cfg = this->GetShulianduCfg(type);
	if (NULL == item_cfg) return 0;

	int total_sld = 0;
	for (int i = 0; i <= LIFE_SKILL_SHULIANDU_LV_MAX && i <= level; ++ i)
	{
		total_sld += item_cfg->need_shuliandu[i];
	}

	return total_sld;
}


void LifeSkillConfig::GetMakeItemId(const LifeSkillMakeItemCfg* make_item_cfg, ItemID *item_id, int self_prof, int cur_total_sld, int total_sld, int *is_make_precious)
{
	if (NULL == make_item_cfg || NULL == item_id || cur_total_sld <= 0 || total_sld <= 0) return;

	int green_weight = make_item_cfg->green_weight;
	int blue_weight = make_item_cfg->blue_weight;
	int purple_weight = make_item_cfg->purple_weight;
	int orange_weight = make_item_cfg->orange_weight;

	// 紫概率 = 对应物品的紫上限 * (0.5 + 0.5 * 当前总熟练度 / 最大总熟练度)

	// 橙概率 = 对应物品的橙上限 * (0.1 + 0.9 * 当前总熟练度 / 最大总熟练度)

	if (LIFE_SKIL_TYPE_ZHIYAO == make_item_cfg->type || LIFE_SKIL_TYPE_FUMO == make_item_cfg->type) // 只重算紫
	{
		purple_weight = static_cast<int>(make_item_cfg->purple_weight * (0.5f + 0.5f * cur_total_sld / total_sld));
	}

	// 重算橙
	{
		orange_weight = static_cast<int>(make_item_cfg->orange_weight * (0.1f + 0.9f * cur_total_sld / total_sld));
	}
	
	int rand_value = RandomNum(100);

	{
		if (rand_value <= green_weight)
		{
			*item_id = this->GetProfReplaceItemId(make_item_cfg, make_item_cfg->green_item_id, self_prof);
			return;
		}
		rand_value -= green_weight;
	}

	{
		if (rand_value <= blue_weight)
		{
			*item_id = this->GetProfReplaceItemId(make_item_cfg, make_item_cfg->blue_item_id, self_prof);
			return;
		}
		rand_value -= blue_weight;
	}

	{
		if (rand_value <= purple_weight)
		{
			*item_id = this->GetProfReplaceItemId(make_item_cfg, make_item_cfg->purple_item_id, self_prof);
			return;
		}
		rand_value -= purple_weight;
	}

	{
		if (rand_value <= orange_weight)
		{
			if (NULL != is_make_precious)
			{
				*is_make_precious = 1;
			}

			*item_id = this->GetProfReplaceItemId(make_item_cfg, make_item_cfg->orange_item_id, self_prof);
			return;
		}
		rand_value -= orange_weight;
	}

	//失败认为出绿色
	*item_id = this->GetProfReplaceItemId(make_item_cfg, make_item_cfg->green_item_id, self_prof);
}

// 允许制作出的物品分职业
ItemID LifeSkillConfig::GetProfReplaceItemId(const LifeSkillMakeItemCfg* make_item_cfg, ItemID item_id, int self_prof)
{
	if (NULL == make_item_cfg)
	{
		return item_id;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(item_id);
	if (NULL == item_base)
	{
		return item_id;
	}

	const LifeSkillProfReplaceCfg *replace_cfg = this->GetProfReplaceCfg(item_id);
	if (NULL == replace_cfg)
	{
		return item_id;
	}


	// 分配权重
	int weight_list[LifeSkillProfReplaceCfg::PROF_NUM];
	int total_weight = 0;

	for (int i = 0; i < LifeSkillProfReplaceCfg::PROF_NUM; ++ i)
	{
		if (i == self_prof % 10 - 1)
		{
			weight_list[i] = make_item_cfg->self_prof_rate;
		}
		else
		{
			weight_list[i] = (100 - make_item_cfg->self_prof_rate) / 3;
		}

		total_weight += weight_list[i];
	}

	if (total_weight <= 0)
	{
		return item_id;
	}

	// 替换物品
	int rand_value = RandomNum(total_weight);

	for (int i = 0; i < LifeSkillProfReplaceCfg::PROF_NUM; i ++)
	{
		if (rand_value <= weight_list[i])
		{
			return replace_cfg->prof_replace_list[i];
		}
		rand_value -= weight_list[i];
	}

	return item_id;
}