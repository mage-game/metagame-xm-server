#include "rollmoneyconfig.h"

RollMoneyConfig::RollMoneyConfig()
{

}

RollMoneyConfig::~RollMoneyConfig()
{

}

bool RollMoneyConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "RollMoneyConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	// ¼ÓÔØÍ­±ÒÅäÖÃ
	PugiXmlNode coin_element = RootElement.child("roll_coin_cfg");
	if (coin_element.empty())
	{
		*err = path + ": no [roll_coin_cfg].";
		return false;
	}

	iRet = this->InitRollCoinCfg(coin_element);
	if (iRet)
	{
		sprintf(errinfo,"%s: InitRollCoinCfg failed %d", path.c_str(), iRet);
		*err = errinfo;
		return false;
	}

	// ¼ÓÔØÔª±¦ÅäÖÃ
	PugiXmlNode gold_element = RootElement.child("roll_gold_cfg");
	if (gold_element.empty())
	{
		*err = path + ": no [roll_gold_cfg].";
		return false;
	}

	iRet = this->InitRollGoldCfg(gold_element);
	if (iRet)
	{
		sprintf(errinfo,"%s: InitRollGoldCfg failed %d", path.c_str(), iRet);
		*err = errinfo;
		return false;
	}

	// ¼ÓÔØÆäËüÅäÖÃ
	PugiXmlNode other_element = RootElement.child("other_cfg");
	if (gold_element.empty())
	{
		*err = path + ": no [other_cfg].";
		return false;
	}

	iRet = this->InitOtherCfg(other_element);
	if (iRet)
	{
		sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
		*err = errinfo;
		return false;
	}

	return true;
}

int RollMoneyConfig::InitRollCoinCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_num = -1;

	while (!dataElement.empty())
	{
		if (last_num >= ROLL_MONEY_NUM_MAX_COUNT)
		{
			return -10000;
		}

		int coin_num = 0;
		if (!PugiGetSubNodeValue(dataElement, "coin_num", coin_num) || coin_num != last_num + 1)
		{
			return -1;
		}
		last_num = coin_num;

		for (int i = 0; i < ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT; ++ i)
		{
			char tmp_element_name[32]; memset(tmp_element_name, 0, sizeof(tmp_element_name));
			SNPRINTF(tmp_element_name, sizeof(tmp_element_name), "coin_%d", i);

			int num_weight = 0;
			if (!PugiGetSubNodeValue(dataElement, tmp_element_name, num_weight) || num_weight < 0)
			{
				return -2;
			}

			m_roll_coin_cfg.cfg_item_list[i].num_weight[coin_num] = num_weight;
		}

		dataElement = dataElement.next_sibling();
	}

	if (ROLL_MONEY_NUM_MAX_COUNT != last_num + 1)
	{
		return -30000;
	}

	if (0 != m_roll_coin_cfg.cfg_item_list[0].num_weight[0])
	{
		return -40000;
	}

	if (0 != m_roll_coin_cfg.cfg_item_list[ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT - 1].num_weight[0])
	{
		return -50000;
	}

	return 0;
}

int RollMoneyConfig::InitRollGoldCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_num = -1;

	while (!dataElement.empty())
	{
		if (last_num >= ROLL_MONEY_NUM_MAX_COUNT)
		{
			return -10000;
		}

		int coin_num = 0;
		if (!PugiGetSubNodeValue(dataElement, "gold_num", coin_num) || coin_num != last_num + 1)
		{
			return -1;
		}
		last_num = coin_num;

		for (int i = 0; i < ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT; ++ i)
		{
			char tmp_element_name[32]; memset(tmp_element_name, 0, sizeof(tmp_element_name));
			SNPRINTF(tmp_element_name, sizeof(tmp_element_name), "gold_%d", i);

			int num_weight = 0;
			if (!PugiGetSubNodeValue(dataElement, tmp_element_name, num_weight) || num_weight < 0)
			{
				return -2;
			}

			m_roll_bind_gold_cfg.cfg_item_list[i].num_weight[coin_num] = num_weight;
		}

		dataElement = dataElement.next_sibling();
	}

	if (ROLL_MONEY_NUM_MAX_COUNT != last_num + 1)
	{
		return -30000;
	}

	if (0 != m_roll_bind_gold_cfg.cfg_item_list[0].num_weight[0])
	{
		return -40000;
	}

	if (0 != m_roll_bind_gold_cfg.cfg_item_list[ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT - 1].num_weight[0])
	{
		return -50000;
	}

	return 0;
}

int RollMoneyConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "active_degree_need", m_other_cfg.active_degree_need) || m_other_cfg.active_degree_need <= 0)
	{
		return -1;
	}

	return 0;
}

