#include "magicalpreciouscfg.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "checkresourcecenter.hpp"
#include <string>
#include<algorithm>

MagicalPreciousCfg::MagicalPreciousCfg():m_max_chapter_count(0)
{
	memset(m_chapter_task_count, 0, sizeof(m_chapter_task_count));
}

MagicalPreciousCfg::~MagicalPreciousCfg()
{
}

bool MagicalPreciousCfg::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "MagicalPreciousCfg", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode lvl_element = RootElement.child("other");
		if (lvl_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(lvl_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode lvl_element = RootElement.child("chapter_cfg");
		if (lvl_element.empty())
		{
			*err = configname + ": no [chapter_cfg].";
			return false;
		}

		iRet = this->InitChapterTaskCfg(lvl_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitChapterTaskCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode lvl_element = RootElement.child("finish_chapter_reward_cfg");
		if (lvl_element.empty())
		{
			*err = configname + ": no [finish_chapter_reward_cfg].";
			return false;
		}

		iRet = this->InitFinishChapterRewardCfg(lvl_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitFinishChapterRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const int MagicalPreciousCfg::GetChapterMaxRewardIndex(int chapter)
{
	if (chapter < 0 || chapter >= m_max_chapter_count || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER)
	{
		return 0;
	}

	return m_chapter_task_count[chapter];
}

const MagicalPreciousChapterCfg * MagicalPreciousCfg::GetChapterTaskCfg(short chapter, short reward_index)
{
	if (chapter < 0 || chapter >= m_max_chapter_count || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER)
	{
		return NULL;
	}

	if (reward_index < 0 || reward_index >= m_chapter_task_count[chapter] || reward_index >= MAGICAL_PRECIOUS_MAX_REWARD_INDEX)
	{
		return NULL;
	}

	if (m_chapter_cfg[chapter][reward_index].condition_type <= CONDITION_TYPE_INVALID || m_chapter_cfg[chapter][reward_index].condition_type >= MAX_CONDITION_TYPE)
	{
		return NULL;
	}

	return &m_chapter_cfg[chapter][reward_index];
}

const MagicalPreciousFinishChapterRewardCfg * MagicalPreciousCfg::GetFinishChapterRewardCfg(int chapter, int seq)
{
	if (chapter < 0 || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER || seq < 0)
	{
		return NULL;
	}

	auto it = std::find_if(m_chapter_finish_cfg_vec.begin(), m_chapter_finish_cfg_vec.end(), [chapter, seq](const MagicalPreciousFinishChapterRewardCfg &cmp_item) {
		if (chapter == cmp_item.chapter_id && seq == cmp_item.seq)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_chapter_finish_cfg_vec.end())
	{
		return &(*it);
	}

	return NULL;
}

std::vector<const MagicalPreciousChapterCfg *> MagicalPreciousCfg::GetChapterCfgByCondition(int condition_type)
{
	std::vector<const MagicalPreciousChapterCfg *> cfg_vec;
	for (int i = 0; i < MAGICAL_PRECIOUS_MAX_CHAPTER && i < m_max_chapter_count; ++i)
	{
		for (int j = 0; j < MAGICAL_PRECIOUS_MAX_REWARD_INDEX && j < m_chapter_task_count[i]; ++j)
		{
			const MagicalPreciousChapterCfg *cfg = nullptr;
			if (condition_type == m_chapter_cfg[i][j].condition_type)
			{
				cfg = &m_chapter_cfg[i][j];
				cfg_vec.push_back(cfg);
			}
		}
	}

	return cfg_vec;
}

int MagicalPreciousCfg::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "open_level", m_other_cfg.open_level) || m_other_cfg.open_level < 0 || m_other_cfg.open_level > MAX_ROLE_LEVEL)
	{
		return -1;
	}

	return 0;
}

int MagicalPreciousCfg::InitChapterTaskCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_chapter = -1;
	while (!dataElement.empty())
	{
		int chapter = -1;
		if (!PugiGetSubNodeValue(dataElement, "chapter_id", chapter) || chapter < 0 || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER)
		{
			return -1;
		}
		
		int reward_index = -1;
		if (!PugiGetSubNodeValue(dataElement, "reward_index", reward_index) || reward_index < 0 || reward_index >= MAGICAL_PRECIOUS_MAX_REWARD_INDEX)
		{
			return -2;
		}

		MagicalPreciousChapterCfg &cfg = m_chapter_cfg[chapter][reward_index];

		cfg.chapter_id = chapter;
		cfg.reward_index = reward_index;

		if (last_chapter != chapter)
		{
			m_max_chapter_count++;
		}
		last_chapter = chapter;

		if (!PugiGetSubNodeValue(dataElement, "condition_type", cfg.condition_type) || cfg.condition_type <= CONDITION_TYPE_INVALID || cfg.condition_type >= MAX_CONDITION_TYPE)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "shenwang_reward", cfg.shenwang_reward) || cfg.shenwang_reward < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "bind_gold_reward", cfg.bind_gold_reward) || cfg.bind_gold_reward < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_a", cfg.param_a) || cfg.param_a < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_b", cfg.param_b) || cfg.param_b < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_c", cfg.param_c) || cfg.param_c < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "get_score", cfg.get_score) || cfg.get_score <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "mojing_reward", cfg.mojing_reward) || cfg.mojing_reward < 0)
		{
			return -10;
		}

		{
			PugiXmlNode element = dataElement.child("reward_item_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - i;
				}

				if (!cfg.reward_item[i].ReadConfig(item_element))
				{
					if (cfg.reward_item[i].item_id == 0)
					{
						break;
					}

					return - 151 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		++m_chapter_task_count[chapter];

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int MagicalPreciousCfg::InitFinishChapterRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int chapter = -1;
		if (!PugiGetSubNodeValue(dataElement, "chapter_id", chapter) || chapter < 0 || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER)
		{
			return -1;
		}

		MagicalPreciousFinishChapterRewardCfg cfg;
		cfg.chapter_id = chapter;

		if (!PugiGetSubNodeValue(dataElement, "seq", cfg.seq) || cfg.seq < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_score", cfg.need_score) || cfg.need_score <= 0)
		{
			return -3;
		}

		{
			cfg.reward_item_count = ItemConfigData::ReadConfigList(dataElement, std::string("reward_item"),
				cfg.reward_item, MagicalPreciousFinishChapterRewardCfg::MAX_REWARD_ITEM_NUM);
			if (cfg.reward_item_count < 0)
			{
				return -100 + cfg.reward_item_count;
			}
		}

		m_chapter_finish_cfg_vec.emplace_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}
