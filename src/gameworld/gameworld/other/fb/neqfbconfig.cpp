#include "neqfbconfig.hpp"
#include <memory.h>
#include <stdlib.h>

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "task/taskpool.h"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"

#include <string>
#include <set>


NeqFBConfig::NeqFBConfig()
{
	memset(m_vip_buy_count_list, 0, sizeof(m_vip_buy_count_list));
	memset(m_nonvip_buy_count_list, 0, sizeof(m_nonvip_buy_count_list));
	memset(m_roll_gold_list, 0, sizeof(m_roll_gold_list));
}

NeqFBConfig::~NeqFBConfig()
{

}

bool NeqFBConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "NeqFBConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// ¹Ø¿¨ÅäÖÃ
		PugiXmlNode lvl_element = RootElement.child("levelcfg");
		if (lvl_element.empty())
		{
			*err = configname + ": no [levelcfg].";
			return false;
		}

		iRet = this->InitLevelCfg(lvl_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¹ºÂòÅäÖÃ
		PugiXmlNode buy_element = RootElement.child("buyablecfg");
		if (buy_element.empty())
		{
			*err = configname + ": no [buyablecfg].";
			return false;
		}

		iRet = this->InitBuyCfg(buy_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBuyCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ÐÇÐÇ½±ÀøÅäÖÃ
		PugiXmlNode star_element = RootElement.child("starcfg");
		if (star_element.empty())
		{
			*err = configname + ": no [starcfg].";
			return false;
		}

		iRet = this->InitStarRewardCfg(star_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitStarRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 
		PugiXmlNode roll_element = RootElement.child("rolllist");
		if (roll_element.empty())
		{
			*err = configname + ": no [rolllist].";
			return false;
		}

		iRet = this->InitRollCfg(roll_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRollCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 
		PugiXmlNode roll_cost_element = RootElement.child("roll_cost");
		if (roll_cost_element.empty())
		{
			*err = configname + ": no [roll_cost].";
			return false;
		}

		iRet = this->InitGoldRollCost(roll_cost_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGoldRollCost failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ÆäËûÅäÖÃ
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const NeqFBLevelCfg * NeqFBConfig::GetLevelCfg(int chapter, int level) const
{
	if (chapter < 0 || chapter >=  NEQFB_MAX_CHAPTER || level < 0 || level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
	{
		return NULL;
	}

	return &m_level_list[chapter][level];
}

int NeqFBConfig::GetVipBuyTimes(int vip_level) const
{
	if (vip_level <= 0 || vip_level > MAX_VIP_LEVEL)
	{
		return 0;
	}

	return m_vip_buy_count_list[vip_level];
}

int NeqFBConfig::GetNonVipBuyTimes(int vip_level) const
{
	if (vip_level <= 0 || vip_level > MAX_VIP_LEVEL)
	{
		return 0;
	}

	return m_nonvip_buy_count_list[vip_level];
}

const NeqFBStarRewardList * NeqFBConfig::GetStarRewardList(int chapter, int seq) const
{
	int key = this->MakeStarRewardKey(chapter, seq);

	std::map<int, NeqFBStarRewardList>::const_iterator it = m_star_reward_map.find(key);
	if (it != m_star_reward_map.end())
	{
		return &it->second;
	}

	return NULL;
}

const NeqFBLevelCfg * NeqFBConfig::GetLevelByScene(int scene_id) const
{
	std::map<int, NeqFBLevelCfg>::const_iterator iter = m_scene_level_map.find(scene_id);
	if (m_scene_level_map.end() == iter) return NULL;

	return &iter->second;
}

int NeqFBConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	//int count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		NeqFBLevelCfg  cfg ;

		if (!PugiGetSubNodeValue(dataElement, "chapter", cfg.chapter) || cfg.chapter < 0 || cfg.chapter >= NEQFB_MAX_CHAPTER)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "level", cfg.level) || cfg.level < 0 || cfg.level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x", cfg.pos_x) || cfg.pos_x <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", cfg.pos_y) || cfg.pos_y <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_id", cfg.boss_id) || !MONSTERPOOL->IsMonsterExist(cfg.boss_id))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_x", cfg.boss_x) || cfg.boss_x <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_y", cfg.boss_y) || cfg.boss_y <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "sec_3_star", cfg.sec_3_star) || cfg.sec_3_star <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "sec_2_star", cfg.sec_2_star) || cfg.sec_2_star <= cfg.sec_3_star)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "times", cfg.limit_times) || cfg.limit_times <= 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "role_level", cfg.role_level) || cfg.role_level <= 0 || cfg.role_level > MAX_ROLE_LEVEL)
		{
			return -14;
		}

		{
			if (!PugiGetSubNodeValue(dataElement, "monster_id", cfg.monster_id) || !MONSTERPOOL->IsMonsterExist(cfg.monster_id))
			{
				return -15;
			}

			char tag_name[32] = {0};

			for (int i = 1; i <= NeqFBLevelCfg::MAX_MONSTER_TO_FLUSH; ++i)
			{
				SNPRINTF(tag_name, sizeof(tag_name), "monster_x%d", i);

				if (!PugiGetSubNodeValue(dataElement, tag_name, cfg.monster_pos_list[i - 1].x))
				{
					return -600;
				}

				SNPRINTF(tag_name, sizeof(tag_name), "monster_y%d", i);

				if (!PugiGetSubNodeValue(dataElement, tag_name, cfg.monster_pos_list[i - 1].y))
				{
					return -601;
				}
			}
		}

		int count = ItemConfigData::ReadConfigList(dataElement, "find_reward", cfg.find_reward, MAX_ATTACHMENT_ITEM_NUM);
		if (count < 0)
		{
			return -100+count;
		}

		if (m_scene_level_map.end() != m_scene_level_map.find(cfg.scene_id))
		{
			return -20;
		}
		cfg.invalid = 1;

		m_level_list[cfg.chapter][cfg.level] = cfg;

		m_scene_level_map[cfg.scene_id] = cfg;

		++ count;

		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int NeqFBConfig::InitBuyCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int vip_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "vip_level", vip_level) || vip_level < 0 || vip_level > MAX_VIP_LEVEL)
		{
			return -1;
		}

		int vip_buyable_times = 0;
		if (!PugiGetSubNodeValue(dataElement, "vip_buyable_times", vip_buyable_times) || vip_buyable_times < 0)
		{
			return -2;
		}

		int nonvip_buyable_times = 0;
		if (!PugiGetSubNodeValue(dataElement, "nonvip_buyable_times", nonvip_buyable_times) || nonvip_buyable_times < 0)
		{
			return -3;
		}

		m_vip_buy_count_list[vip_level] = vip_buyable_times;
		m_nonvip_buy_count_list[vip_level] = nonvip_buyable_times;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int NeqFBConfig::InitStarRewardCfg(PugiXmlNode RootElement)
{
	int chapter_count_list[NEQFB_MAX_CHAPTER];
	memset(chapter_count_list, 0, sizeof(chapter_count_list));

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int chapter = 0;
		if (!PugiGetSubNodeValue(dataElement, "chapter", chapter) || chapter < 0 || chapter >= NEQFB_MAX_CHAPTER)
		{
			return -1;
		}

		if (chapter_count_list[chapter] >= 3)
		{
			return -50;
		}

		int need_star = 0;
		if (!PugiGetSubNodeValue(dataElement, "start", need_star) || need_star <= 0)
		{
			return -2;
		}
		int add_challenge_count = 0;
		if (!PugiGetSubNodeValue(dataElement, "add_challenge_count", add_challenge_count) || add_challenge_count < 0)
		{
			return -3;
		}

		NeqFBStarRewardList cfglist;
		cfglist.need_star = need_star;
		cfglist.reward_count = 0;
		cfglist.add_challenge_count = add_challenge_count;

		PugiXmlNode item_list_element = dataElement.child("reward_item_list");
		if (item_list_element.empty())
		{
			return -100;
		}

		cfglist.reward_count = 0;

		PugiXmlNode item_element = item_list_element.child("reward_item");
		while (!item_element.empty())
		{
			if (cfglist.reward_count >= (int)(sizeof(cfglist.item_list) / sizeof(cfglist.item_list[0])))
			{
				return -10;
			}

			if (!cfglist.item_list[cfglist.reward_count].ReadConfig(item_element))
			{
				return -11;
			}

			++cfglist.reward_count;

			item_element = item_element.next_sibling();
		}

		if (cfglist.reward_count <= 0)
		{
			return -20;
		}

		int key = this->MakeStarRewardKey(chapter, chapter_count_list[chapter]);
		m_star_reward_map[key] = cfglist;

		++ chapter_count_list[chapter];

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

//int NeqFBConfig::InitRollCfg(PugiXmlNode RootElement)
//{
//	int max_chapter = 0; int max_level = 0;
//
//	PugiXmlNode data_element = RootElement.child("data");
//	while (!data_element.empty())
//	{
//		NeqRollItem cfg;
//
//		PugiXmlNode Itemelement = data_element.child("item");
//		if (NULL != Itemelement)
//		{
//			if (!cfg.item_cfg.ReadConfig(Itemelement))
//			{
//				return -1;
//			}
//		}
//
//		if (!PugiGetSubNodeValue(data_element, "min_chapter", cfg.min_chapter) || cfg.min_chapter < 0 || cfg.min_chapter >= NEQFB_MAX_CHAPTER)
//		{
//			return -20;
//		}
//
//		if (!PugiGetSubNodeValue(data_element, "min_level", cfg.min_level) || cfg.min_level < 0 || cfg.min_level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
//		{
//			return -21;
//		}
//
//		if (!PugiGetSubNodeValue(data_element, "max_chapter", cfg.max_chapter) || cfg.max_chapter < 0 || cfg.max_chapter >= NEQFB_MAX_CHAPTER)
//		{
//			return -22;
//		}
//		max_chapter = cfg.max_chapter;
//
//		if (!PugiGetSubNodeValue(data_element, "max_level", cfg.max_level) || cfg.max_level < 0 || cfg.max_level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
//		{
//			return -23;
//		}
//		max_level = cfg.max_level;
//
//		if (cfg.max_chapter < cfg.min_chapter)
//		{
//			return -25;
//		}
//		if (cfg.max_chapter == cfg.min_chapter)
//		{
//			if (cfg.max_level < cfg.min_level)
//			{
//				return -26;
//			}
//		}
//
//		if (!PugiGetSubNodeValue(data_element, "i_type", cfg.i_type) || cfg.i_type < 0 || cfg.i_type >= MAX_ROLL_ITEM_TYPE)
//		{
//			return -30;
//		}
//
//		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight <= 0)
//		{
//			return -30;
//		}
//
//		m_roll_list[cfg.i_type].push_back(cfg);
//
//		data_element = data_element.next_sibling();
//	}
//
//	{
//		// ¼ì²é
//		for (int chapter = 0; chapter < max_chapter; ++ chapter)
//		{
//			int cur_chapter_max_level = NEQFB_MAX_LEVEL_PER_CHAPTER;
//			if (chapter == max_chapter)
//			{
//				cur_chapter_max_level = max_level;
//			}
//
//			for (int level = 0; level < cur_chapter_max_level; ++ level)
//			{
//				if (!this->CheckRollCfgHelper(chapter, level))
//				{
//					return -50000;
//				}
//			}
//		}
//	}
//
//	return 0;
//}

int NeqFBConfig::InitRollCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	while (!data_element.empty())
	{
		NeqRollItem cfg;

		PugiXmlNode Itemelement = data_element.child("item");
		if (NULL != Itemelement)
		{
			if (!cfg.item_cfg.ReadConfig(Itemelement))
			{
				return -1;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "min_chapter", cfg.min_chapter) || cfg.min_chapter < 0 || cfg.min_chapter >= NEQFB_MAX_CHAPTER)
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(data_element, "min_level", cfg.min_level) || cfg.min_level < 0 || cfg.min_level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
		{
			return -21;
		}

		if (!PugiGetSubNodeValue(data_element, "max_chapter", cfg.max_chapter) || cfg.max_chapter < 0 || cfg.max_chapter >= NEQFB_MAX_CHAPTER)
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(data_element, "max_level", cfg.max_level) || cfg.max_level < 0 || cfg.max_level >= NEQFB_MAX_LEVEL_PER_CHAPTER)
		{
			return -23;
		}

		if (cfg.max_chapter < cfg.min_chapter)
		{
			return -25;
		}
		if (cfg.max_chapter == cfg.min_chapter)
		{
			if (cfg.max_level < cfg.min_level)
			{
				return -26;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "i_type", cfg.i_type) || cfg.i_type < 0 || cfg.i_type >= MAX_ROLL_ITEM_TYPE)
		{
			return -30;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight <= 0)
		{
			return -30;
		}

		for (int chapter = cfg.min_chapter; chapter <= cfg.max_chapter; ++chapter)
		{
			for (int level = cfg.min_level; level <= cfg.max_level; ++level)
			{
				int key = this->MakeChapterLevelKey(chapter, level);
				if (m_roll_item_map.find(key) != m_roll_item_map.end())
				{
					m_roll_item_map[key].roll_item_vec.push_back(cfg);
					m_roll_item_map[key].total_weight += cfg.weight;
				}
				else
				{
					RollItemAndWeight riaw;
					riaw.roll_item_vec.push_back(cfg);
					riaw.total_weight = cfg.weight;
					m_roll_item_map[key] = riaw;
				}
			}
		}

		data_element = data_element.next_sibling();
	}
	return 0;
}

int NeqFBConfig::InitGoldRollCost(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	while (!data_element.empty())
	{
		int gold_roll_times = 0;
		int need_gold = 0;

		if (!PugiGetSubNodeValue(data_element, "gold_roll_times", gold_roll_times) || 
			gold_roll_times <= 0 || gold_roll_times > NEQFB_MAX_GOLD_ROLL_TIMES)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "gold", need_gold) || need_gold <= 0)
		{
			return -2;
		}

		m_roll_gold_list[gold_roll_times] = need_gold;

		data_element = data_element.next_sibling();
	}

	for (int i = 1; i <= NEQFB_MAX_GOLD_ROLL_TIMES; ++i)
	{
		if (m_roll_gold_list[i] <= 0)
		{
			return -10;
		}
	}

	return 0;
}

int NeqFBConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "auto_grid", m_neq_other.auto_grid) || m_neq_other.auto_grid <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "day_total_count", m_neq_other.day_total_count) || m_neq_other.day_total_count <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "buy_times_gold", m_neq_other.buy_times_gold) || m_neq_other.buy_times_gold <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "roll_times_1", m_neq_other.roll_times_1) || m_neq_other.roll_times_1 <= 0 || m_neq_other.roll_times_1 > NEQFB_MAX_FREE_ROLL_TIMES)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "roll_times_2", m_neq_other.roll_times_2) || m_neq_other.roll_times_2 <= 0 || m_neq_other.roll_times_2 > NEQFB_MAX_FREE_ROLL_TIMES)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "roll_times_3", m_neq_other.roll_times_3) || m_neq_other.roll_times_3 <= 0 || m_neq_other.roll_times_3 > NEQFB_MAX_FREE_ROLL_TIMES)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "auto_item_count", m_neq_other.auto_item_count) || m_neq_other.auto_item_count <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "auto_item_id", m_neq_other.auto_item_id) || NULL == ITEMPOOL->GetItem(m_neq_other.auto_item_id))
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "auto_limit", m_neq_other.auto_limit) || m_neq_other.auto_limit <=0)
		{
			return -10;
		}
	}

	return 0;
}

int NeqFBConfig::MakeStarRewardKey(int chapter, int seq) const
{
	return 100 * chapter + seq;
}

int NeqFBConfig::MakeChapterLevelKey(int chapter, int level) const
{
	return 100 * chapter + level;
}

bool NeqFBConfig::CheckRollItemChapterLevel(const NeqRollItem &tmp_roll_item, int chapter, int level)
{
	// Âú×ãÕÂ½ÚºÍµÈ¼¶Ìõ¼þ
	if (chapter >= tmp_roll_item.min_chapter && chapter <= tmp_roll_item.max_chapter && 
		level >= tmp_roll_item.min_level && level <= tmp_roll_item.max_level)
	{
		return true;
	}

	return false;
}

bool NeqFBConfig::CheckRollCfgHelper(int chapter, int level)
{
	std::vector<NeqRollItem> temp_vec0;
	std::vector<NeqRollItem> temp_vec1;
	std::vector<NeqRollItem> temp_vec2;

	std::vector<NeqRollItem> temp_item_list;

	for (std::vector<NeqRollItem>::iterator it = m_roll_list[0].begin(); it != m_roll_list[0].end(); ++ it)
	{
		if (this->CheckRollItemChapterLevel(*it, chapter, level))
		{
			temp_vec0.push_back(*it);
		}
	}

	if (static_cast<int>(temp_vec0.size()) < NEQFB_ROLL_ITYPE_0_ITEM_COUNT)
	{
		return false;
	}

	for (std::vector<NeqRollItem>::iterator it = m_roll_list[1].begin(); it != m_roll_list[1].end(); ++ it)
	{
		if (this->CheckRollItemChapterLevel(*it, chapter, level))
		{
			temp_vec1.push_back(*it);
		}
	}

	if (static_cast<int>(temp_vec1.size()) < NEQFB_ROLL_ITYPE_1_ITEM_COUNT)
	{
		return false;
	}

	for (std::vector<NeqRollItem>::iterator it = m_roll_list[2].begin(); it != m_roll_list[2].end(); ++it)
	{
		if (this->CheckRollItemChapterLevel(*it, chapter, level))
		{
			temp_vec2.push_back(*it);
		}
	}

	if (static_cast<int>(temp_vec2.size()) < NEQFB_ROLL_ITYPE_2_ITEM_COUNT) 
	{
		return false;
	}

	return true;
}

//bool NeqFBConfig::GetRollPool(NeqRollItemToRole roll_list[NEQFB_ROLLPOOL_TOTAL_COUNT], int chapter, int level, bool is_random_order)
//{
//	std::vector<NeqRollItem> temp_vec0;
//	std::vector<NeqRollItem> temp_vec1;
//	std::vector<NeqRollItem> temp_vec2;
//
//	std::vector<NeqRollItem> temp_roll_list;
//
//	for (std::vector<NeqRollItem>::iterator it = m_roll_list[0].begin(); it != m_roll_list[0].end(); ++it)
//	{
//		if (this->CheckRollItemChapterLevel(*it, chapter, level))
//		{
//			temp_vec0.push_back(*it);
//		}
//	}
//
//	if (static_cast<int>(temp_vec0.size()) < NEQFB_ROLL_ITYPE_0_ITEM_COUNT)
//	{
//		return false;
//	}
//
//	for (int i = 0; i < NEQFB_ROLL_ITYPE_0_ITEM_COUNT; ++i)
//	{
//		int rand_index = RandomNum(static_cast<int>(temp_vec0.size()));
//		std::vector<NeqRollItem>::iterator it = temp_vec0.begin();
//		advance(it, rand_index);
//		if (temp_vec0.end() == it)
//		{
//			return false;
//		}
//
//		temp_roll_list.push_back(*it);
//		temp_vec0.erase(it);
//	}
//
//	for (std::vector<NeqRollItem>::iterator it = m_roll_list[1].begin(); it != m_roll_list[1].end(); ++it)
//	{
//		if (this->CheckRollItemChapterLevel(*it, chapter, level))
//		{
//			temp_vec1.push_back(*it);
//		}
//	}
//
//	if (static_cast<int>(temp_vec1.size()) < NEQFB_ROLL_ITYPE_1_ITEM_COUNT)
//	{
//		return false;
//	}
//
//	for (int i = 0; i < NEQFB_ROLL_ITYPE_1_ITEM_COUNT; ++i)
//	{
//		int rand_index = RandomNum(static_cast<int>(temp_vec1.size()));
//		std::vector<NeqRollItem>::iterator it = temp_vec1.begin();
//		advance(it, rand_index);
//		if (temp_vec1.end() == it)
//		{
//			return false;
//		}
//
//		temp_roll_list.push_back(*it);
//		temp_vec1.erase(it);
//	}
//
//	for (std::vector<NeqRollItem>::iterator it = m_roll_list[2].begin(); it != m_roll_list[2].end(); ++it)
//	{
//		if (this->CheckRollItemChapterLevel(*it, chapter, level))
//		{
//			temp_vec2.push_back(*it);
//		}
//	}
//
//	if (static_cast<int>(temp_vec2.size()) < NEQFB_ROLL_ITYPE_2_ITEM_COUNT)
//	{
//		return false;
//	}
//
//	for (int i = 0; i < NEQFB_ROLL_ITYPE_2_ITEM_COUNT; ++i)
//	{
//		int rand_index = RandomNum(static_cast<int>(temp_vec2.size()));
//		std::vector<NeqRollItem>::iterator it = temp_vec2.begin();
//		advance(it, rand_index);
//		if (temp_vec2.end() == it)
//		{
//			return false;
//		}
//
//		temp_roll_list.push_back(*it);
//		temp_vec2.erase(it);
//	}
//
//	int ret_count = 0;
//	for (std::vector<NeqRollItem>::iterator it = temp_roll_list.begin(); it != temp_roll_list.end() && ret_count < NEQFB_ROLLPOOL_TOTAL_COUNT; ++it)
//	{
//		roll_list[ret_count].i_type = it->i_type;
//		roll_list[ret_count].item_cfg = it->item_cfg;
//		roll_list[ret_count].weight = it->weight;
//		++ret_count;
//	}
//
//	if (is_random_order)
//	{
//		for (int i = 0; i < NEQFB_ROLLPOOL_TOTAL_COUNT; ++i)
//		{
//			int rand_val = RandomNum(NEQFB_ROLLPOOL_TOTAL_COUNT);
//
//			NeqRollItemToRole tmp_item = roll_list[i];
//			roll_list[i] = roll_list[rand_val];
//			roll_list[rand_val] = tmp_item;
//		}
//	}
//
//	return true;
//}

bool NeqFBConfig::GetRollPool(NeqRollItemToRole roll_list[NEQFB_ROLLPOOL_TOTAL_COUNT], int chapter, int level, bool is_random_order)
{
	int key = this->MakeChapterLevelKey(chapter, level);
	if (m_roll_item_map.find(key) == m_roll_item_map.end())
	{
		return false;
	}
	std::vector<NeqRollItem> &roll_item_vec = m_roll_item_map[key].roll_item_vec;
	int total_weight = m_roll_item_map[key].total_weight;
	for (int i = 0; i < NEQFB_ROLLPOOL_TOTAL_COUNT; ++i)
	{
		int rand_value = RandomNum(total_weight);
		int now_weight = 0;
		for (size_t j = 0; j < roll_item_vec.size(); ++j)
		{
			now_weight += roll_item_vec[j].weight;
			if (rand_value < now_weight)
			{
				roll_list[i].i_type = roll_item_vec[j].i_type;
				roll_list[i].item_cfg = roll_item_vec[j].item_cfg;
				roll_list[i].weight = roll_item_vec[j].weight;
				break;
			}
		}
	}

	if (is_random_order)
	{
		for (int i = 0; i < NEQFB_ROLLPOOL_TOTAL_COUNT; ++i)
		{
			int rand_val = RandomNum(NEQFB_ROLLPOOL_TOTAL_COUNT);

			NeqRollItemToRole tmp_item = roll_list[i];
			roll_list[i] = roll_list[rand_val];
			roll_list[rand_val] = tmp_item;
		}
	}

	return true;
}

int NeqFBConfig::GetRollTimes(int star)
{
	if (star >= 3)
	{
		return m_neq_other.roll_times_3;
	}
	else if (star == 2)
	{
		return m_neq_other.roll_times_2;
	}
	else
	{
		return m_neq_other.roll_times_1;
	}
}

int NeqFBConfig::GetGoldRollCost(int roll_times)
{
	if (roll_times <= 0 || roll_times > NEQFB_MAX_GOLD_ROLL_TIMES)
	{
		return 0;
	}

	return m_roll_gold_list[roll_times];
}



