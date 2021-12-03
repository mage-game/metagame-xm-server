#include <memory.h>
#include <stdlib.h>

#include "mazeconfig.hpp"
#include "servercommon/servercommon.h"

#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "obj/character/attribute.hpp"
#include "gameworld/checkresourcecenter.hpp"

#include <string>


MazeConfig::MazeConfig() : m_box_item_id(0), m_find_item_count(0), m_box_total_weight(0), m_box_reward_count(0)
{
	memset(m_buy_list, 0, sizeof(m_buy_list));
}

MazeConfig::~MazeConfig()
{

}

bool MazeConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "MazeConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 
		PugiXmlNode box_element = RootElement.child("box");
		if (box_element.empty())
		{
			*err = configname + ": no [box].";
			return false;
		}

		iRet = InitBoxCfg(box_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBoxCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 
		PugiXmlNode buy_element = RootElement.child("buy_times");
		if (buy_element.empty())
		{
			*err = configname + ": no [buy_times].";
			return false;
		}

		iRet = InitBuyCfg(buy_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBuyCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 其他
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = InitOther(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOther failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 积分奖励
		PugiXmlNode other_element = RootElement.child("jifen_reward");
		if (other_element.empty())
		{
			*err = configname + ": no [jifen_reward].";
			return false;
		}

		iRet = InitJifenRewardCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitJifenRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int MazeConfig::InitBoxCfg(PugiXmlNode RootElement)
{
	m_box_total_weight = 0;
	m_box_reward_count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (m_box_reward_count >= MAX_BOX_REWARD_COUNT)
		{
			return -10;
		}

		MazeBox cfg;

		int weight = 0;
		if (!PugiGetSubNodeValue(dataElement, "weight", weight) || weight < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "item_count", cfg.item_count) || cfg.item_count < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "coin", cfg.coin) || cfg.coin < 0)
		{
			return -2;
		}

		m_box_total_weight += weight;
		cfg.weight = m_box_total_weight;

		m_box_list[m_box_reward_count ++] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int MazeConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "box_item_id", m_box_item_id) || 
			NULL == ITEMPOOL->GetItem(m_box_item_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "find_item_count", m_find_item_count) || m_find_item_count <= 0)
		{
			return -2;
		}
	}

	return 0;
}

int MazeConfig::InitBuyCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int buy_times = 0;
		int gold = 0;

		if (!PugiGetSubNodeValue(dataElement, "buy_times", buy_times) || buy_times <= 0 || buy_times > MAX_BUY_TIMES)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold", gold) || gold <= 0)
		{
			return -2;
		}

		m_buy_list[buy_times] = gold;

		dataElement = dataElement.next_sibling();
	}

	for (int i = 1; i <= MAX_BUY_TIMES; ++i)
	{
		if (m_buy_list[i] <= 0)
		{
			return -100;
		}
	}

	return 0;
}

int MazeConfig::InitJifenRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -20000;
	}

	m_jifen_reward_cfg.cfg_count = 0;

	int last_need_jifen = 0;

	while (!dataElement.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq >= MazeJifenRewardConfig::CONFIG_ITEM_MAX_COUNT || seq != m_jifen_reward_cfg.cfg_count)
		{
			return -1;
		}

		MazeJifenRewardConfig::ConfigItem &cfg_item = m_jifen_reward_cfg.cfg_list[m_jifen_reward_cfg.cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(dataElement, "need_jifen", cfg_item.need_jifen) || cfg_item.need_jifen <= last_need_jifen)
		{
			return -2;
		}
		last_need_jifen = cfg_item.need_jifen;

		PugiXmlNode rewardItemElement = dataElement.child("reward_item");
		if (rewardItemElement.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(rewardItemElement))
		{
			return -4;
		}

		if (cfg_item.reward_item.item_id != m_box_item_id)
		{
			return -5;
		}

		++ m_jifen_reward_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

bool MazeConfig::RandReward(int &item_count, int &coin)
{
	item_count = 0;
	coin = 0;

	if (m_box_total_weight <= 0)
	{
		return false;
	}

	int rand_val = RandomNum(m_box_total_weight);
	for (int i = 0; i < m_box_reward_count; ++i)
	{
		if (rand_val < m_box_list[i].weight)
		{
			item_count = m_box_list[i].item_count;
			coin = m_box_list[i].coin;

			return true;
		}
	}

	return false;
}

int MazeConfig::GetBuyCostGold(int buy_times)
{
	if (buy_times <= 0 || buy_times > MAX_BUY_TIMES)
	{
		return 0;
	}

	return m_buy_list[buy_times];
}

const MazeJifenRewardConfig::ConfigItem * MazeConfig::GetJifenReward(int seq)
{
	if (seq >= 0 && seq < m_jifen_reward_cfg.cfg_count && seq < MazeJifenRewardConfig::CONFIG_ITEM_MAX_COUNT)
	{
		return &m_jifen_reward_cfg.cfg_list[seq];
	}

	return NULL;
}
