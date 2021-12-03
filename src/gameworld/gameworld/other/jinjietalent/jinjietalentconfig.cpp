#include "jinjietalentconfig.hpp"
#include "servercommon/struct/itemlistparam.h"

JinJieTalentConfig::JinJieTalentConfig()
{
}

JinJieTalentConfig::~JinJieTalentConfig()
{
}

bool JinJieTalentConfig::Init(const std::string & configname, std::string * err)
{
	char errinfo[1024] = { 0 };
	int iRet = 0;

	TiXmlDocument document;

	if (configname == "" || !document.LoadFile(configname))
	{
		sprintf(errinfo, "%s: Load LiemingConfig Error,\n %s.", configname.c_str(), document.ErrorDesc());
		*err = errinfo;
		return false;
	}
	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		*err = configname + ": xml root node error.";
		return false;
	}
	{
		TiXmlElement *root_element = RootElement->FirstChildElement("talent_list");
		if (NULL == root_element)
		{
			*err = configname + ": no [talent_list].";
			return false;
		}
		iRet = this->InitTalentList(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitTalentList failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *root_element = RootElement->FirstChildElement("grid_list");
		if (NULL == root_element)
		{
			*err = configname + ": no [grid_list].";
			return false;
		}
		iRet = this->InitGridList(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGridList failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}


	{
		TiXmlElement *root_element = RootElement->FirstChildElement("talent_skill");
		if (NULL == root_element)
		{
			*err = configname + ": no [talent_skill].";
			return false;
		}
		iRet = this->InitTalentSkill(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitTalentSkill failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *root_element = RootElement->FirstChildElement("choujiang_stage");
		if (NULL == root_element)
		{
			*err = configname + ": no [choujiang_stage].";
			return false;
		}
		iRet = this->InitChoujiangStage(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitChoujiangStage failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *root_element = RootElement->FirstChildElement("skill_weight");
		if (NULL == root_element)
		{
			*err = configname + ": no [skill_weight].";
			return false;
		}
		iRet = this->InitSkillWeight(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSkillWeight failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *root_element = RootElement->FirstChildElement("flush_cost");
		if (NULL == root_element)
		{
			*err = configname + ": no [flush_cost].";
			return false;
		}
		iRet = this->InitFlushCost(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitFlushCost failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *root_element = RootElement->FirstChildElement("other");
		if (NULL == root_element)
		{
			*err = configname + ": no [other].";
			return false;
		}
		iRet = this->InitOther(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOther failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int JinJieTalentConfig::InitTalentList(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	while (NULL != data_element)
	{
		int skill_type = 0;
		int grid_id = 0;
		int tl_type = 0;
		if (!GetSubNodeValue(data_element, "talent_type", tl_type))
		{
			return -1;
		}
		if (!GetSubNodeValue(data_element, "skill_type", skill_type))
		{
			return -2;
		}
		if (!GetSubNodeValue(data_element, "grid_id", grid_id))
		{
			return -3;
		}
		m_other_cfg.tl_type_skill_final_skill[tl_type] = std::pair<int,int>(skill_type,grid_id);
		m_other_cfg.skill_tl_type_final_skill[skill_type] = tl_type;
		data_element = data_element->NextSiblingElement();
	}
	return 0;
}

int JinJieTalentConfig::InitGridList(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	while (NULL != data_element)
	{
		int grid_id = 0;
		JinJieActivateTalentGridCondCfg grid_cond;
		if (!GetSubNodeValue(data_element, "grid_id", grid_id))
		{
			return -4;
		}
		if (!GetSubNodeValue(data_element, "need_grade", grid_cond.need_grade))
		{
			return -5;
		}
		if (!GetSubNodeValue(data_element, "pre_grid_id1", grid_cond.pre_index1))
		{
			return -6;
		}
		if (!GetSubNodeValue(data_element, "pre_grid_id2", grid_cond.pre_index2))
		{
			return -7;
		}
		if (!GetSubNodeValue(data_element, "pre_grid_id3", grid_cond.pre_index3))
		{
			return -8;
		}
		if (!GetSubNodeValue(data_element, "pre_quality", grid_cond.per_gird_quality))
		{
			return -9;
		}
		m_other_cfg.activate_cond[grid_id] = grid_cond;
		data_element = data_element->NextSiblingElement();
	}
	return 0;
}

int JinJieTalentConfig::InitTalentSkill(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	while (NULL != data_element)
	{
		JinJieTalenSkillParamCfg skill_cfg;

		if (!GetSubNodeValue(data_element, "skill_id", skill_cfg.skill_id))
		{
			return -10;
		}
		if (!GetSubNodeValue(data_element, "book_id", skill_cfg.book_id))
		{
			return -11;
		}
		if (!GetSubNodeValue(data_element, "skill_type", skill_cfg.skill_type))
		{
			return -12;
		}
		if (!GetSubNodeValue(data_element, "skill_quality", skill_cfg.skill_quality))
		{
			return -13;
		}
		if (!GetSubNodeValue(data_element, "skill_star", skill_cfg.skill_star))
		{
			return -14;
		}
		if (!GetSubNodeValue(data_element, "need_item_id", skill_cfg.need_item_id))
		{
			return -15;
		}
		if (!GetSubNodeValue(data_element, "need_item_count", skill_cfg.need_item_count))
		{
			return -16;
		}
		if (!GetSubNodeValue(data_element, "forget_gold", skill_cfg.forget_gold))
		{
			return -17;
		}
		if (!GetSubNodeValue(data_element, "value", skill_cfg.value))
		{
			return -18;
		}

		if (!GetSubNodeValue(data_element, "per", skill_cfg.per))
		{
			return -19;
		}

		if (!GetSubNodeValue(data_element, "param_1", skill_cfg.param1))
		{
			return -19;
		}

		if (!GetSubNodeValue(data_element, "param_2", skill_cfg.param2))
		{
			return -20;
		}

		if (!GetSubNodeValue(data_element, "param_3", skill_cfg.param3))
		{
			return -21;
		}

		if (!GetSubNodeValue(data_element, "param_4", skill_cfg.param4))
		{
			return -22;
		}
		if (!GetSubNodeValue(data_element, "capability", skill_cfg.capability))
		{
			return -23;
		}
		if (!GetSubNodeValue(data_element, "cd", skill_cfg.cd))
		{
			return -24;
		}

		std::map<int, TalentSkillStarMp>::iterator it = m_other_cfg.skill_info.find(skill_cfg.skill_id);
		if (it != m_other_cfg.skill_info.end())
		{
			TalentSkillStarMp skill_star_cfg;
			it->second[skill_cfg.skill_star] = skill_cfg;
		}
		else
		{
			TalentSkillStarMp skill_star_cfg;
			skill_star_cfg[skill_cfg.skill_star] = skill_cfg;
			m_other_cfg.skill_info[skill_cfg.skill_id] = skill_star_cfg;
		}
		m_other_cfg.bookid_skillid[skill_cfg.book_id] = skill_cfg.skill_id;
		data_element = data_element->NextSiblingElement();
	}
	return 0;
}

int JinJieTalentConfig::InitChoujiangStage(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}

	while (NULL != data_element)
	{
		int stage = 0;
		int min_count = 0;
		int max_count = 0;
		if (!GetSubNodeValue(data_element, "stage", stage))
		{
			return -30;
		}
		if (!GetSubNodeValue(data_element, "min_count", min_count))
		{
			return -31;
		}
		if (!GetSubNodeValue(data_element, "max_count", max_count))
		{
			return -32;
		}
		m_other_cfg.stage_array[stage] = std::pair<int, int>(min_count, max_count);
		data_element = data_element->NextSiblingElement();
	}
	return 0;
}

int JinJieTalentConfig::InitSkillWeight(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}
	std::map<int, std::map<int, int> > stage_range;
	while (NULL != data_element)
	{
		int stage = 0;
		int skill_id = 0;
		int weight = 0;
		if (!GetSubNodeValue(data_element, "stage", stage))
		{
			return -40;
		}
		if (!GetSubNodeValue(data_element, "skill_id", skill_id))
		{
			return -41;
		}
		if (!GetSubNodeValue(data_element, "weight", weight))
		{
			return -42;
		}
		std::map<int, std::map<int, int> >::iterator it = stage_range.find(stage);
		if (it != stage_range.end())
		{
			it->second[skill_id] = weight;
		}
		else
		{
			std::map<int, int> stage_cfg;
			stage_cfg.insert(std::pair<int, int>(skill_id, weight));
			stage_range[stage] = stage_cfg;
		}
		data_element = data_element->NextSiblingElement();
	}
	//算出每个阶段每个技能的权重范围
	std::map<int, std::map<int, int> >::iterator stage_range_it = stage_range.begin();
	for (; stage_range_it != stage_range.end(); stage_range_it++)
	{
		std::map<int, std::pair<int, int> > skill_weight;
		int min_weight = 0;
		int max_weight = 0;
		std::map<int, int>::iterator skill_it = stage_range_it->second.begin();
		for (; skill_it != stage_range_it->second.end(); skill_it++)
		{
			max_weight = min_weight + skill_it->second;
			skill_weight[skill_it->first] = std::pair<int, int>(min_weight, max_weight);
			min_weight = max_weight + 1;
		}
		m_other_cfg.stage_skill_weight[stage_range_it->first] = skill_weight;
	}
	return 0;
}

int JinJieTalentConfig::InitFlushCost(TiXmlElement * RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}
	while (NULL != data_element)
	{
		int count = 0;
		int gold = 0;
		if (!GetSubNodeValue(data_element, "count", count))
		{
			return -50;
		}
		if (!GetSubNodeValue(data_element, "gold", gold))
		{
			return -51;
		}
		m_other_cfg.flush_cost_gold[count] = gold;
		data_element = data_element->NextSiblingElement();
	}
	return 0;
}

int JinJieTalentConfig::InitOther(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -10000;
	}
	while (NULL != data_element)
	{
		int free_count = 0;
		if (!GetSubNodeValue(data_element, "free_count", free_count) || free_count < 0)
		{
			return -1;
		}
		m_other_cfg.other_cfg.free_count = free_count;
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

const JinJieTalentOtherConfig& JinJieTalentConfig::GetTalentOtherCfg()
{
	return m_other_cfg;
}

const JinJieActivateTalentGridCondCfg * JinJieTalentConfig::GetActivateGridCondCfg(int index)
{
	std::map<int, JinJieActivateTalentGridCondCfg>::iterator it = m_other_cfg.activate_cond.find(index);
	if (it != m_other_cfg.activate_cond.end())
	{
		return &it->second;
	}
	return NULL;
}

const JinJieTalenSkillParamCfg * JinJieTalentConfig::GetTalenSkillInfoCfg(int skill_id, int star)
{
	std::map<int, TalentSkillStarMp>::iterator skill_it = m_other_cfg.skill_info.find(skill_id);
	if (skill_it != m_other_cfg.skill_info.end())
	{
		TalentSkillStarMp::iterator star_it = skill_it->second.find(star);
		if (star_it != skill_it->second.end())
		{
			return &star_it->second;
		}
	}
	return NULL;
}

int JinJieTalentConfig::GetTalenSkillId(int skill_type, int skill_quality, int star)
{
	std::map<int, TalentSkillStarMp>::iterator skill_it = m_other_cfg.skill_info.begin();
	for (; skill_it != m_other_cfg.skill_info.end(); skill_it++)
	{
		TalentSkillStarMp::iterator star_it = skill_it->second.find(star);
		if (star_it != skill_it->second.end())
		{
			if (star_it->second.skill_quality == skill_quality && star_it->second.skill_type == skill_type)
			{
				return star_it->second.skill_id;
			}
		}
	}
	return 0;
}

short JinJieTalentConfig::GetSkillByBookId(int book_id)
{
	std::map<int, int>::iterator it = m_other_cfg.bookid_skillid.find(book_id);
	if (it != m_other_cfg.bookid_skillid.end())
	{
		return it->second;
	}
	return 0;
}

std::vector<int> JinJieTalentConfig::RandomSkill(int stage,int count)
{
	std::vector<int> vec_skill;
	std::map<int, std::map<int, std::pair<int,int> > >::iterator it = m_other_cfg.stage_skill_weight.find(stage);
	if (it != m_other_cfg.stage_skill_weight.end())
	{
		int max_random = 0;
		for (std::map<int, std::pair<int, int> >::iterator itt = it->second.begin(); itt != it->second.end(); itt++)
		{
			if (itt->second.second > max_random)
			{
				max_random = itt->second.second + 1;
			}
		}

		if (max_random <= 0)
		{
			return vec_skill;
		}

		for (int i = 0; i < count; i++)
		{
			int ret_random = RandomNum(max_random);
			for (std::map<int, std::pair<int, int> >::iterator itt = it->second.begin(); itt != it->second.end(); itt++)
			{
				if (itt->second.first <= ret_random && itt->second.second >= ret_random)
				{
					vec_skill.push_back(itt->first);
					continue;
				}
			}
		}
	}
	return vec_skill;
}

bool JinJieTalentConfig::IsFinalSkill(int skill_type)
{
	std::map<int, int>::iterator it = m_other_cfg.skill_tl_type_final_skill.find(skill_type);
	if (it != m_other_cfg.skill_tl_type_final_skill.end())
	{
		return true;
	}
	return false;
}

std::pair<int, int> JinJieTalentConfig::GetTalentFinalSkill(int tl_type)
{
	std::map<int, std::pair<int, int> >::iterator it = m_other_cfg.tl_type_skill_final_skill.find(tl_type);
	if (it != m_other_cfg.tl_type_skill_final_skill.end())
	{
		return it->second;
	}
	return std::pair<int, int>(0, 0);
}

short JinJieTalentConfig::GetStage(int flush_count)
{
	int max_stage = 0;
	std::map<int, std::pair<int, int> >::iterator it = m_other_cfg.stage_array.begin();
	for (; it != m_other_cfg.stage_array.end(); it++)
	{
		max_stage = it->first;
		if (it->second.first <= flush_count && it->second.second >= flush_count)
		{
			return it->first;
		}
	}
	return max_stage;
}

short JinJieTalentConfig::GetFlushGold(int flush_number)
{
	std::map<int, int>::iterator it = m_other_cfg.flush_cost_gold.find(flush_number);
	if (it != m_other_cfg.flush_cost_gold.end())
	{
		return it->second;
	}
	return 0;
}

int JinJieTalentConfig::GetSkillStarAllStuff(int skill_id, int skill_star, ItemConfigData* data, size_t max_count)
{
	std::map<int, TalentSkillStarMp>::iterator skill_it = m_other_cfg.skill_info.find(skill_id);
	if (skill_it == m_other_cfg.skill_info.end())
	{
		return 0;
	}

	if (skill_it->second.empty())
	{
		return 0;
	}

	auto now_skill_it = skill_it->second.find(skill_star);
	if (now_skill_it == skill_it->second.end())
	{
		return 0;
	}


	std::map<ItemID, int> item_count;

	for (auto& skill_level_vec : m_other_cfg.skill_info)
	{
		if (skill_level_vec.second.empty())
		{
			continue;
		}

		if (skill_level_vec.second.begin()->second.skill_type != now_skill_it->second.skill_type)
		{//类型不同
			continue;
		}

		for (auto& skill_info : skill_level_vec.second)
		{
			if (skill_info.second.skill_quality > now_skill_it->second.skill_quality)
			{//品质高于
				continue;
			}
			else if (skill_info.second.skill_quality == now_skill_it->second.skill_quality && skill_info.second.skill_star >= now_skill_it->second.skill_star)
			{
				continue;
			}

			item_count[skill_info.second.need_item_id] += skill_info.second.need_item_count;
			if ( skill_info.second.skill_quality == 0 && skill_info.second.skill_star == 0)
			{
				item_count[skill_info.second.book_id] += 1;
			}
		}
	}

	int index = 0;
	for (auto& value : item_count)
	{
		if (index >= static_cast<int>(max_count))
			return index;

		data[index].is_bind = true;
		data[index].item_id = value.first;
		data[index].num = value.second;
		index++;
	}
	return index;
}

const JinJieTalentOtherCfg* JinJieTalentConfig::GetOtherCfg()
{
	return &m_other_cfg.other_cfg;
}
