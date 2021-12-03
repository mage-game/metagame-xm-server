#include "bosscardconfig.hpp"
#include "monster/monsterpool.h"

BossCardConfig::BossCardConfig() : now_max_bosscard_type(0)
{

}

bool BossCardConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = { 0 };
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "BossCardConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	// ÆäËûÅäÖÃ
	{
		PugiXmlNode bosscard_element = RootElement.child("other");
		if (bosscard_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(bosscard_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: BossCardConfig::InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	// BOSSÍ¼¼øÅäÖÃ
	{
		PugiXmlNode bosscard_element = RootElement.child("bosscard_cfg");
		if (bosscard_element.empty())
		{
			*err = configname + ": no [bosscard_cfg].";
			return false;
		}

		iRet = this->InitBossCardInfoCfg(bosscard_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: BossCardConfig::InitBossCardInfoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// BOSSÍ¼¼ø½±ÀøÅäÖÃ
	{
		PugiXmlNode bosscard_element = RootElement.child("bosscard_reward_cfg");
		if (bosscard_element.empty())
		{
			*err = configname + ": no [bosscard_reward_cfg].";
			return false;
		}

		iRet = this->InitBossCardRewardCfg(bosscard_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: BossCardConfig::InitBossCardRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int BossCardConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "open_level", m_other_cfg.open_level) || m_other_cfg.open_level < 0)
	{
		return -1;
	}

	return 0;
}

int BossCardConfig::InitBossCardInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		BossCardActiveConfig info_cfg;
		if (!PugiGetSubNodeValue(dataElement, "monster_id", info_cfg.monster_id) || info_cfg.monster_id <= 0 )//|| !MONSTERPOOL->IsMonsterExist(info_cfg.monster_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_seq", info_cfg.monster_seq) || info_cfg.monster_seq < 0 || info_cfg.monster_seq >= BOSS_CARD_MAX_COUNT)
		{
			return -2;
		}

		if (PugiGetSubNodeValue(dataElement, "gongji", info_cfg.gongji) && info_cfg.gongji < 0)
		{
			return -10;
		}

		if (PugiGetSubNodeValue(dataElement, "maxhp", info_cfg.maxhp) && info_cfg.maxhp < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", info_cfg.fangyu) || info_cfg.fangyu < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "per_pojia", info_cfg.per_pojia) || info_cfg.per_pojia < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "card_type", info_cfg.card_type) || info_cfg.card_type < 0 || info_cfg.card_type > BOSS_CARD_MAX_TYPE)
		{
			return -14;
		}
		if (info_cfg.card_type > now_max_bosscard_type)
		{
			now_max_bosscard_type = info_cfg.card_type;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", info_cfg.scene_id) || info_cfg.scene_id < 0)
		{
			return -15;
		}

		if (m_boss_card_cfg_list.find(info_cfg.monster_id) != m_boss_card_cfg_list.end())
		{
			return -100;
		}

		m_boss_card_cfg_list[info_cfg.monster_id] = info_cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossCardConfig::InitBossCardRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int last_card_type = -1;
	while (!dataElement.empty())
	{
		BossCardRewardConfig info_cfg;
		if (!PugiGetSubNodeValue(dataElement, "card_type", info_cfg.card_type) || info_cfg.card_type < 0 || info_cfg.card_type > BOSS_CARD_MAX_TYPE)
		{
			return -1;
		}

		if (info_cfg.card_type != last_card_type + 1)
		{
			return -2;
		}
		last_card_type = info_cfg.card_type;

		PugiXmlNode Itemelement = dataElement.child("reward_item");
		int item_id = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", item_id) && item_id > 0)
		{
			if (!info_cfg.reward_item.ReadConfig(Itemelement))
			{
				return -3;
			}
		}

		m_bosscard_reward_cfg[last_card_type] = info_cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int BossCardConfig::GetBossCardCfgSeq(int monster_id)
{
	std::map<int, BossCardActiveConfig>::iterator it = m_boss_card_cfg_list.find(monster_id);
	if (it != m_boss_card_cfg_list.end())
	{
		return it->second.monster_seq;
	}

	return -1;
}

const BossCardActiveConfig * BossCardConfig::GetBossCardCfg(int monster_seq)
{
	std::map<int, BossCardActiveConfig>::iterator it = m_boss_card_cfg_list.begin();
	for (; it != m_boss_card_cfg_list.end(); it ++)
	{
		if (it->second.monster_seq == monster_seq)
		{
			return &it->second;
		}
	}

	return NULL;
}

const BossCardRewardConfig *BossCardConfig::GetBossCardRewardCfg(int card_type)
{
	if (card_type < 0 || card_type > BOSS_CARD_MAX_TYPE)
	{
		return NULL;
	}
	return &m_bosscard_reward_cfg[card_type];
}