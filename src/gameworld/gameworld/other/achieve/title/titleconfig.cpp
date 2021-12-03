#include <string>
#include "titleconfig.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "servercommon/titledef.hpp"
#include "item/itempool.h"

#include <string>
#include <set>

TitlePool::TitlePool() : m_pata_title_count(0), m_jingling_title_count(0), condition_count(0), m_famous_man_title_count(0), m_challenge_field_title_count(0)
{

}

TitlePool::~TitlePool()
{

}

bool TitlePool::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "TitlePool", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// ³ÆºÅ
		PugiXmlNode title_element = RootElement.child("title_list");
		if (title_element.empty())
		{
			*err = configname + ": no [title_list].";
			return false;
		}

		iRet = this->InitTitle(title_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTitle failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ÅÅÐÐ°ñ³ÆºÅ
		PugiXmlNode toprank_element = RootElement.child("top_rank_tilte_list");
		if (toprank_element.empty())
		{
			*err = configname + ": no [top_rank_tilte_list].";
			return false;
		}

		iRet = this->InitRankTitle(toprank_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRankTitle failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¿ç·þÅÅÐÐ°ñ³ÆºÅ
		PugiXmlNode toprank_element = RootElement.child("cross_top_rank_tilte_list");
		if (toprank_element.empty())
		{
			*err = configname + ": no [cross_top_rank_tilte_list].";
			return false;
		}

		iRet = this->InitCrossRankTitle(toprank_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCrossRankTitle failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// »î¶¯³ÆºÅ
		PugiXmlNode activity_tilte_element = RootElement.child("activity_tilte_list");
		if (activity_tilte_element.empty())
		{
			*err = configname + ": no [activity_tilte_list].";
			return false;
		}

		iRet = this->InitActivityTitle(activity_tilte_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitActivity failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ÆäËû³ÆºÅ
		PugiXmlNode other_tilte_element = RootElement.child("other_title_list");
		if (other_tilte_element.empty())
		{
			*err = configname + ": no [other_title_list].";
			return false;
		}

		iRet = this->InitOtherTitle(other_tilte_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherTitle failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ÅÀËþ³ÆºÅ
		/*PugiXmlNode patafb_tilte_element = RootElement.child("patafb_title");
		if (patafb_tilte_element.empty())
		{
			*err = configname + ": no [patafb_title].";
			return false;
		}

		iRet = this->InitPataFbTitle(patafb_tilte_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPataFbTitle failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}*/
	}

	{
		// ¾«Áé¿¨ÅÆ³ÆºÅ
		PugiXmlNode jingling_card_tilte_element = RootElement.child("jingling_card_title");
		if (jingling_card_tilte_element.empty())
		{
			*err = configname + ": no [jingling_card_title].";
			return false;
		}

		iRet = this->InitJingLingCardTitle(jingling_card_tilte_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitJingLingCardTitle failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ÃûÈËÌÃ³ÆºÅ
		PugiXmlNode famous_man_tilte_element = RootElement.child("famous_man");
		if (famous_man_tilte_element.empty())
		{
			*err = configname + ": no [famous_man].";
			return false;
		}

		iRet = this->InitFamousManTitle(famous_man_tilte_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitFamousManTitle failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ³ÆºÅ½ø½×
		PugiXmlNode title_upgrade_element = RootElement.child("upgrade");
		if (title_upgrade_element.empty())
		{
			*err = configname + ": no [title_upgrade].";
			return false;
		}

		iRet = this->InitTitleUpgrade(title_upgrade_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTitleUpgrade failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// Í³Ò»¼ì²é³ÆºÅ
		for (std::map<unsigned short, TitleCfg>::const_iterator iter = m_title_map.begin(); 
			m_title_map.end() != iter; ++iter)
		{
			unsigned short title_id = iter->first;
			const TitleCfg &title_cfg = iter->second;

			if (TITLESOURCETYPE_RANK == title_cfg.source_type)
			{
				if (!this->IsTitleInRankList(title_id))
				{
					sprintf(errinfo,"%s: title %d source rank but not in rank title list", configname.c_str(), title_id);
					*err = errinfo;
					return false;
				}
			}
			else if (TITLESOURCETYPE_ACTIVITY == title_cfg.source_type)
			{
				if (!this->IsTitleInActivity(title_id))
				{
					sprintf(errinfo,"%s: title %d source mingrentang but not in mingrentang title list", configname.c_str(), title_id);
					*err = errinfo;
					return false;
				}
			}
			/*else if (TITLESOURCETYPE_PATAFB == title_cfg.source_type)
			{
				if (!this->IsTitleInPataFb(title_id))
				{
					sprintf(errinfo,"%s: title %d source patafb but not in patafb title list", configname.c_str(), title_id);
					*err = errinfo;
					return false;
				}
			}*/
			else if (TITLESOURCETYPE_JINGLINGCARD == title_cfg.source_type)
			{
				if (!this->IsTitleInJinglingCard(title_id))
				{
					sprintf(errinfo,"%s: title %d source jinglingcard but not in jinglingcard title list", configname.c_str(), title_id);
					*err = errinfo;
					return false;
				}
			}
			else if (TITLESOURCETYPE_OTHER == title_cfg.source_type)
			{
				if (!this->IsTitleInOther(title_id))
				{
					sprintf(errinfo,"%s: title %d source other but not in other title list", configname.c_str(), title_id);
					*err = errinfo;
					return false;
				}
			}
			else if (TITLESOURCETYPE_FAMOUS_MAN == title_cfg.source_type)
			{
				if (!this->IsTitleInFamousMan(title_id))
				{
					sprintf(errinfo,"%s: title %d source famousman but not in famousman title list", configname.c_str(), title_id);
					*err = errinfo;
					return false;
				}
			}
		}
	}

	{
		// »ñµÃ³ÆºÅÌõ¼þ
		PugiXmlNode condition_element = RootElement.child("check_condition");
		if (condition_element.empty())
		{
			*err = configname + ": no [check_condition].";
			return false;
		}

		iRet = this->InitCondition(condition_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCondition failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

// 	{
// 		// ¾º¼¼³¡³ÆºÅ
// 		PugiXmlNode challenge_field_title_element = RootElement.child("challenge_field");
// 		if (challenge_field_title_element.empty())
// 		{
// 			*err = configname + ": no [challenge_field].";
// 			return false;
// 		}
// 
// 		iRet = this->InitChallengeFieldTitle(challenge_field_title_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo, "%s: InitChallengeFieldTitle failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
// 	}

	return true;
}

const TitleCfg* TitlePool::GetTitleCfg(unsigned short title_id) const
{
	std::map<unsigned short, TitleCfg>::const_iterator iter = m_title_map.find(title_id);
	if (m_title_map.end() == iter) return NULL;

	return &iter->second;	
}

unsigned short TitlePool::GetConditionTitle(int type, int condition) const
{
	if (type < TITLE_TYPE_BEGIN || type >= TITLE_TYPE_MAX || condition <= 0)
	{
		return 0;
	}

	std::map<int, TitleConditonCfg>::const_iterator iter = m_condition_map.find(type);

	if (iter != m_condition_map.end() && condition >= iter->second.condition)
	{
		return iter->second.title_id;
	}

	return 0;
}

int TitlePool::InitTitle(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (m_title_map.size() >= MAX_TITLE_COUNT_TO_SAVE)  // ³ÆºÅÅäÖÃ×ÜÊý²»³¬¹ý´æ´¢µÄ¸öÊý
		{
			return -99999;
		}

		int title_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "title_id", title_id) || title_id <= 0 || title_id > 65000)
		{
			return -1;
		}

		int source_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "source_type", source_type) || source_type <= TITLESOURCETYPE_INVALID || source_type >= TITLESOURCETYPE_MAX)
		{
			return -2;
		}

		TitleCfg cfg;
		cfg.title_id = title_id;
		cfg.source_type = source_type;

		if (!PugiGetSubNodeValue(dataElement, "valid_secs", cfg.valid_secs) || cfg.valid_secs < 0) return -10;

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -20;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) return -40;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -50;

		if (!PugiGetSubNodeValue(dataElement, "maxhp_add_per", cfg.max_hp_add_per) || cfg.max_hp_add_per < 0) return -60;

		if (!PugiGetSubNodeValue(dataElement, "gongji_add_per", cfg.gongji_add_per) || cfg.gongji_add_per < 0) return -70;

		if (!PugiGetSubNodeValue(dataElement, "fangyu_add_per", cfg.fangyu_add_per) || cfg.fangyu_add_per < 0) return -80;

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", is_broadcast) || is_broadcast < 0) return -70;
		cfg.is_broadcast = (0 != is_broadcast);

		std::string title_name;
		if (!PugiGetSubNodeValue(dataElement, "name", title_name) || title_name.length() >= sizeof(cfg.title_name)) return -70;

		memcpy(cfg.title_name, title_name.c_str(), (int)title_name.length());
		cfg.title_name[title_name.length()] = 0;

		if (m_title_map.end() != m_title_map.find(cfg.title_id))
		{
			return -1000;
		}

		m_title_map[cfg.title_id] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

unsigned short TitlePool::GetTopRankTitle(int rank_top_type, int rank, int level) const
{
	if (rank_top_type < 0 || rank_top_type >= RANK_TOP_TYPE_MAX) return 0;
	
	if (rank < 1 || rank > TOP_TEN) return 0;

	if (level > 0 && level < m_rank_title_list[rank_top_type].level_limit) return 0;

	return m_rank_title_list[rank_top_type].title_id_list[rank];
}

int TitlePool::InitRankTitle(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int top_rank_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "top_rank_type", top_rank_type) || top_rank_type < 0 || top_rank_type >= RANK_TOP_TYPE_MAX)
		{
			return -1;
		}

		int top_1_title_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "top_1_title_id", top_1_title_id) || NULL == this->GetTitleCfg(top_1_title_id))
		{
			return -2;
		}

		int top_2_10_title_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "top_2_10_title_id", top_2_10_title_id) || 
			(0 != top_2_10_title_id && NULL == this->GetTitleCfg(top_2_10_title_id)))
		{
			return -3;
		}

		int level_limit = 0;
		if (!PugiGetSubNodeValue(dataElement, "level_limit", level_limit) || level_limit < 0)
		{
			return -4;
		}

		RankTitleCfg cfg;
		cfg.is_valid = true;
		cfg.level_limit = level_limit;
		cfg.title_id_list[1] = top_1_title_id;
		for (int i = 2; i <= TOP_TEN; ++i)
		{
			cfg.title_id_list[i] = top_2_10_title_id;
		}

		if (m_rank_title_list[top_rank_type].is_valid)
		{
			return -10;
		}

		m_rank_title_list[top_rank_type] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

unsigned short TitlePool::GetActivityTitle(int activity_type) const
{
	std::map<int, unsigned short>::const_iterator it = m_activity_title_map.find(activity_type);
	if (m_activity_title_map.end() == it) return 0;

	return it->second;	
}

unsigned short TitlePool::GetOtherTitle(int other_title_type) const
{
	std::map<int, unsigned short>::const_iterator it = m_other_title_map.find(other_title_type);
	if (m_other_title_map.end() == it) return 0;

	return it->second;	
}

unsigned short TitlePool::GetCrossTopRankTitle(int rank_top_type, int rank) const
{
	if (rank_top_type < CROSS_RANK_TOP_TYPE_MIN || rank_top_type >= CROSS_RANK_TOP_TYPE_MAX) return 0;

	if (rank < 1 || rank > CrossRankTitleCfg::CROSS_RANK_TITLE_MAX_RANK) return 0;

	return m_cross_rank_title_list[rank_top_type].title_id_list[rank];
}

unsigned short TitlePool::GetPataFbTitle(int max_pass_level) const
{
	for (int i = m_pata_title_count - 1; i >= 0; -- i)
	{
		const PataFbTitleCfg &title_cfg = m_patafb_title_list[i];
		if (max_pass_level >= title_cfg.pass_level)
		{
			return title_cfg.title_id;
		}
	}

	return 0;
}

unsigned short TitlePool::GetJinglingCardTitle(int card_level) const
{
	if (m_jingling_title_count > JINGLINGCARD_TITLE_MAX_COUNT)
	{
		return 0;
	}

	for (int i = m_jingling_title_count - 1; i >= 0; -- i)
	{
		const JingLingCardTitleCfg &title_cfg = m_jinglingcard_title_list[i];
		if (card_level >= title_cfg.level && title_cfg.level != 0)
		{
			return title_cfg.title_id;
		}
	}

	return 0;
}

unsigned short TitlePool::GetFamousManTitle(int famous_type) const
{
	if (m_famous_man_title_count > FAMOUS_MAN_TITLE_MAX_COUNT)
	{
		return 0;
	}

	for (int i = 0; i < m_famous_man_title_count && i < FAMOUS_MAN_TITLE_MAX_COUNT; ++ i)
	{
		if (famous_type == m_famous_man_title_list[i].famous_type)
		{
			return m_famous_man_title_list[i].title_id;
		}
	}

	return 0;
}

unsigned short TitlePool::GetChallengeFieldRankTitle(int rank_pos) const
{
	if (rank_pos < 0)
	{
		return 0;
	}

	for (int i = 0; i < m_challenge_field_title_count && i < CHALLENGE_FIELD_TITLE_MAX_COUNT; ++i)
	{
		if (rank_pos >= m_challenge_field_title_list[i].min_rank_pos &&
			rank_pos <= m_challenge_field_title_list[i].max_rank_pos)
		{
			return m_challenge_field_title_list[i].title_id;
		}
	}

	return 0;
}

const TitleUpgradeConfig * TitlePool::GetUpgradeCfg(int title_id, int grade)
{
	TitleGradeVec::const_iterator iter = m_upgrade_cfg_vec.begin();
	while(iter != m_upgrade_cfg_vec.end())
	{
		if (iter->title_id == title_id && iter->grade == grade)
		{
			return &(*iter);
		}

		++iter;
	}

	return NULL;
}

int TitlePool::InitActivityTitle(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int activity_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "activity_type", activity_type))
		{
			return -1;
		}

		if (m_activity_title_map.find(activity_type) != m_activity_title_map.end())
		{
			return -2;
		}

		unsigned short title_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "title_id", title_id) || NULL == this->GetTitleCfg(title_id))
		{
			return -3;
		}

		m_activity_title_map[activity_type] = title_id;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}


int TitlePool::InitOtherTitle(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int other_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "other_type", other_type))
		{
			return -1;
		}

		if (m_other_title_map.find(other_type) != m_other_title_map.end())
		{
			return -2;
		}

		unsigned short title_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "title_id", title_id) || NULL == this->GetTitleCfg(title_id))
		{
			return -3;
		}

		m_other_title_map[other_type] = title_id;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}


int TitlePool::InitCrossRankTitle(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int top_rank_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "top_rank_type", top_rank_type) || top_rank_type < CROSS_RANK_TOP_TYPE_MIN || top_rank_type >= CROSS_RANK_TOP_TYPE_MAX)
		{
			return -1;
		}

		int top_1_title_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "top_1_title_id", top_1_title_id) || NULL == this->GetTitleCfg(top_1_title_id))
		{
			return -2;
		}

		int top_2_title_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "top_2_title_id", top_2_title_id) || 
			(0 != top_2_title_id && NULL == this->GetTitleCfg(top_2_title_id)))
		{
			return -3;
		}

		int top_3_title_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "top_3_title_id", top_3_title_id) || 
			(0 != top_3_title_id && NULL == this->GetTitleCfg(top_3_title_id)))
		{
			return -4;
		}

		int top_4_10_title_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "top_4_10_title_id", top_4_10_title_id) || 
			(0 != top_4_10_title_id && NULL == this->GetTitleCfg(top_4_10_title_id)))
		{
			return -5;
		}

		CrossRankTitleCfg cfg;
		cfg.is_valid = true;
		cfg.title_id_list[1] = top_1_title_id;
		cfg.title_id_list[2] = top_2_title_id;
		cfg.title_id_list[3] = top_3_title_id;
		for (int i = 4; i <= CrossRankTitleCfg::CROSS_RANK_TITLE_MAX_RANK; ++i)
		{
			cfg.title_id_list[i] = top_4_10_title_id;
		}

		if (m_cross_rank_title_list[top_rank_type].is_valid)
		{
			return -10;
		}

		m_cross_rank_title_list[top_rank_type] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TitlePool::InitPataFbTitle(PugiXmlNode RootElement)
{
	int last_level = 0;

	std::set<unsigned short> repeat_checker;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (m_pata_title_count >= PATA_FB_TITLE_MAX_COUNT)
		{
			return - 10000;
		}

		PataFbTitleCfg &title_cfg = m_patafb_title_list[m_pata_title_count];

		if (!PugiGetSubNodeValue(dataElement, "level", title_cfg.pass_level) || title_cfg.pass_level <= 0)
		{
			return -1;
		}

		if (title_cfg.pass_level <= last_level)
		{
			return -2;
		}
		last_level = title_cfg.pass_level;

		if (!PugiGetSubNodeValue(dataElement, "title_id", title_cfg.title_id) || NULL == this->GetTitleCfg(title_cfg.title_id))
		{
			return -3;
		}

		if (repeat_checker.end() != repeat_checker.find(title_cfg.title_id))
		{
			return -4;
		}
		repeat_checker.insert(title_cfg.title_id);

		++ m_pata_title_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TitlePool::InitJingLingCardTitle(PugiXmlNode RootElement)
{
	int last_level = 0;

	std::set<unsigned short> repeat_checker;
	m_jingling_title_count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (m_jingling_title_count >= JINGLINGCARD_TITLE_MAX_COUNT)
		{
			return - 10000;
		}

		JingLingCardTitleCfg &title_cfg = m_jinglingcard_title_list[m_jingling_title_count];

		if (!PugiGetSubNodeValue(dataElement, "level", title_cfg.level) || title_cfg.level <= 0 || title_cfg.level <= last_level)
		{
			return -1;
		}

		last_level = title_cfg.level;

		if (!PugiGetSubNodeValue(dataElement, "title_id", title_cfg.title_id) || NULL == this->GetTitleCfg(title_cfg.title_id))
		{
			return -3;
		}

		if (repeat_checker.end() != repeat_checker.find(title_cfg.title_id))
		{
			return -4;
		}
		repeat_checker.insert(title_cfg.title_id);

		++ m_jingling_title_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TitlePool::InitFamousManTitle(PugiXmlNode RootElement)
{
	int last_famous_type = 0;

	std::set<unsigned short> repeat_checker;
	m_famous_man_title_count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (m_famous_man_title_count >= FAMOUS_MAN_TITLE_MAX_COUNT)
		{
			return - 10000;
		}

		FamousManTitleCfg &title_cfg = m_famous_man_title_list[m_famous_man_title_count];

		if (!PugiGetSubNodeValue(dataElement, "famous_type", title_cfg.famous_type) || title_cfg.famous_type <= 0 || title_cfg.famous_type <= last_famous_type)
		{
			return -1;
		}

		last_famous_type = title_cfg.famous_type;

		if (!PugiGetSubNodeValue(dataElement, "title_id", title_cfg.title_id) || NULL == this->GetTitleCfg(title_cfg.title_id))
		{
			return -3;
		}

		if (repeat_checker.end() != repeat_checker.find(title_cfg.title_id))
		{
			return -4;
		}
		repeat_checker.insert(title_cfg.title_id);

		++ m_famous_man_title_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TitlePool::InitChallengeFieldTitle(PugiXmlNode RootElement)
{
	int last_max_rank_pos = -1;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (m_challenge_field_title_count >= CHALLENGE_FIELD_TITLE_MAX_COUNT || m_challenge_field_title_count<0)
		{
			return -10000;
		}

		ChallengeFieldTitleCfg &title_cfg = m_challenge_field_title_list[m_challenge_field_title_count];

		if (!PugiGetSubNodeValue(dataElement, "min_rank_pos", title_cfg.min_rank_pos) || title_cfg.min_rank_pos < 0 || last_max_rank_pos >= title_cfg.min_rank_pos)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_rank_pos", title_cfg.max_rank_pos) || title_cfg.max_rank_pos < 0 || title_cfg.max_rank_pos < title_cfg.min_rank_pos)
		{
			return -2;
		}
		last_max_rank_pos = title_cfg.max_rank_pos;

		if (!PugiGetSubNodeValue(dataElement, "title_id", title_cfg.title_id) || NULL == this->GetTitleCfg(title_cfg.title_id))
		{
			return -3;
		}
		++m_challenge_field_title_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TitlePool::InitCondition(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	int last_type = -1;

	while (!dataElement.empty())
	{
		int type = -1;
		if (!PugiGetSubNodeValue(dataElement, "type", type) || type < TITLE_TYPE_BEGIN || type >= TITLE_TYPE_MAX || type != last_type + 1)
		{
			return -1;
		}
		last_type = type;
		TitleConditonCfg &cfg = m_condition_map[type];

		if (!PugiGetSubNodeValue(dataElement, "condition", cfg.condition) || cfg.condition < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "title_id", cfg.title_id) || NULL == this->GetTitleCfg(cfg.title_id))
		{
			return -3;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TitlePool::InitTitleUpgrade(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	short last_id = 0;
	//int last_grade = 0;

	while (!dataElement.empty())
	{
		static TitleUpgradeConfig cfg;
		cfg.Reset();

		if (!PugiGetSubNodeValue(dataElement, "title_id", cfg.title_id) || NULL == this->GetTitleCfg(cfg.title_id))
		{
			return -1;
		}

		if (last_id != cfg.title_id)
		{
			last_id = 0;
		}

		if (!PugiGetSubNodeValue(dataElement, "level", cfg.grade) || 0 >= cfg.grade || MAX_TITLE_GRADE < cfg.grade)
		{
			return -2;
		}

		//last_grade = cfg.grade;
		last_id = cfg.title_id;

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", cfg.stuff_id) || NULL == ITEMPOOL->GetItem(cfg.stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_num", cfg.stuff_num) || 0 >= cfg.stuff_num)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.max_hp) || cfg.max_hp < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji_add_per", cfg.gongji_add_per) || cfg.gongji_add_per < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu_add_per", cfg.fangyu_add_per) || cfg.fangyu_add_per < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp_add_per", cfg.max_hp_add_per) || cfg.max_hp_add_per < 0)
		{
			return -10;
		}

		m_upgrade_cfg_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

bool TitlePool::IsTitleInRankList(unsigned short title_id)
{
	for (int i = 0; i < RANK_TOP_TYPE_MAX; ++i)
	{
		RankTitleCfg &rank_cfg = m_rank_title_list[i];
		if (rank_cfg.is_valid)
		{
			for (int j = 1; j <= TOP_TEN; ++j)
			{
				if (title_id == rank_cfg.title_id_list[j])
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool TitlePool::IsTitleInActivity(unsigned short title_id)
{
	for (std::map<int, unsigned short>::const_iterator it = m_activity_title_map.begin(); m_activity_title_map.end() != it; ++it)
	{
		if (it->second == title_id)
		{
			return true;
		}
	}

	return false;
}

bool TitlePool::IsTitleInOther(unsigned short title_id)
{
	for (std::map<int, unsigned short>::const_iterator it = m_other_title_map.begin(); m_other_title_map.end() != it; ++it)
	{
		if (it->second == title_id)
		{
			return true;
		}
	}

	return false;
}

bool TitlePool::IsTitleInPataFb(unsigned short title_id)
{
	for (int i = 0; i < m_pata_title_count && i < PATA_FB_TITLE_MAX_COUNT; ++ i)
	{
		if (title_id == m_patafb_title_list[i].title_id)
		{
			return true;
		}
	}

	return false;
}

bool TitlePool::IsTitleInJinglingCard(unsigned short title_id)
{
	for (int i = 0; i < m_jingling_title_count && i < JINGLINGCARD_TITLE_MAX_COUNT; ++ i)
	{
		if (title_id == m_jinglingcard_title_list[i].title_id)
		{
			return true;
		}
	}

	return false;
}

bool TitlePool::IsTitleInFamousMan(unsigned short title_id)
{
	for (int i = 0; i < m_famous_man_title_count && i < FAMOUS_MAN_TITLE_MAX_COUNT; ++ i)
	{
		if (title_id == m_famous_man_title_list[i].title_id)
		{
			return true;
		}
	}

	return false;
}
