#include "touzijihuaconfig.hpp"

#include "servercommon/configcommon.h"

TouzijihuaConfig::TouzijihuaConfig()
{
	memset(m_plan_price, 0, sizeof(m_plan_price));
}

TouzijihuaConfig::~TouzijihuaConfig()
{

}

bool TouzijihuaConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "TouzijihuaConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 投资计划配置
		PugiXmlNode root_element = RootElement.child("plan");
		if (root_element.empty())
		{
			*err = path + " xml not plan node ";
			return false;
		}

		iRet = this->InitRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 其他配置
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 新投资计划配置
		PugiXmlNode root_element = RootElement.child("new_plan");
		if (root_element.empty())
		{
			*err = path + " xml not new_plan node ";
			return false;
		}

		iRet = this->InitNewRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitNewRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 成长基金配置
		PugiXmlNode root_element = RootElement.child("foundation");
		if (root_element.empty())
		{
			*err = path + " xml not foundation node ";
			return false;
		}

		iRet = this->InitFoundationCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitFoundationCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("fb_plan");
		if (root_element.empty())
		{
			*err = path + "xml not fb_plan node";
			return false;
		}

		iRet = this->InitFbPlanCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitFbPlanCfg fail %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("boss_plan");
		if (root_element.empty())
		{
			*err = path + "xml not boss_plan node";
			return false;
		}
		iRet = this->InitBossPlanCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitBossPlanCfg fail %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("shenyu_plan");
		if (root_element.empty())
		{
			*err = path + "xml not shenyu_plan node";
			return false;
		}
		iRet = this->InitShenyuBossPlanCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitShenyuBossPlanCfg fail %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

const TouzijihuaRewardConfig::ConfigItem * TouzijihuaConfig::GetRewardCfg(int type, int seq)
{
	if (type >= 0 && type < TOUZI_JIHUA_PLAN_MAX_TYPE)
	{
		if (seq >= 0 && seq < TouzijihuaRewardConfig::CONFIG_ITEM_MAX_COUNT && seq < m_reward_cfglist[type].cfg_count)
		{
			return &m_reward_cfglist[type].cfg_list[seq];
		}
	}

	return NULL;
}

int TouzijihuaConfig::GetPlanPrice(int plan_type)
{
	if (plan_type >= TOUZI_JIHUA_PLAN_TYPE_0 && plan_type < TOUZI_JIHUA_PLAN_MAX_TYPE)
	{
		return m_plan_price[plan_type];
	}

	return 0;
}

int TouzijihuaConfig::GetNewPlanPrice()
{
	return m_new_plan_price;
}

int TouzijihuaConfig::GetNewPlanFirstReward()
{
	return m_new_plan_first_reward;
}

const NewTouzijihuaRewardConfig * TouzijihuaConfig::GetNewRewardCfg(int day_index)
{
	if (day_index < 0 || day_index >= MAX_NEW_TOUZIJIHUA_DAY)
	{
		return NULL;
	}

	return &m_new_reward_cfg_list[day_index];
}

const FoundationConfig * TouzijihuaConfig::GetFoundationCfg(int chongzhi_num)
{
	for (const auto &cfg : m_foundation_cfg_vec)
	{
		if (cfg.chongzhi_gold == chongzhi_num)
		{
			return &cfg;
		}
	}

	return nullptr;
}

int TouzijihuaConfig::GetFoundationRewardGold(int reward_index, int reward_times, int role_level)
{
	if (reward_index < 0 || reward_index >= static_cast<int>(m_foundation_cfg_vec.size()))
	{
		return 0;
	}

	const FoundationConfig &cfg = m_foundation_cfg_vec[reward_index];

	if (reward_times < 0 || reward_times >= static_cast<int>(cfg.reward_cfg_vec.size()))
	{
		return 0;
	}

	if (role_level < cfg.reward_cfg_vec[reward_times].reward_level)
	{
		return 0;
	}

	return cfg.reward_cfg_vec[reward_times].reward_gold;
}

int TouzijihuaConfig::GetFoundationRewardLimitMaxLevel(int reward_index)
{
	if (reward_index < 0 || reward_index >= static_cast<int>(m_foundation_cfg_vec.size()))
	{
		return 0;
	}

	return m_foundation_cfg_vec[reward_index].active_level_max;
}

int TouzijihuaConfig::GetFbPlanPrice() const
{
	return m_fb_plan_price;
}
int TouzijihuaConfig::GetBossPlanPrice() const
{
	return m_boss_plan_price;
}

const FbPlanConfig::FbPlanItem * TouzijihuaConfig::GetFbPlanCfgItem(int index) const
{
	for (unsigned int i = 0; i < m_fb_plan_cfg.fb_plan_vec.size(); i++)
	{
		if (m_fb_plan_cfg.fb_plan_vec[i].index == index)
		{
			return &m_fb_plan_cfg.fb_plan_vec[i];
		}
	}
	return NULL;
}

const BossPlanConfig::BossPlanItem * TouzijihuaConfig::GetBossPlanCfgItem(int index) const
{
	for (unsigned int i = 0; i < m_boss_plan_cfg.boss_plan_vec.size(); i++)
	{
		if (m_boss_plan_cfg.boss_plan_vec[i].index == index)
		{
			return &m_boss_plan_cfg.boss_plan_vec[i];
		}
	}

	return NULL;
}

int TouzijihuaConfig::GetFbPlanIndex(int pass_level)
{
	for (int i = (int)m_fb_plan_cfg.fb_plan_vec.size() - 1; i >= 0; --i)
	{
		if (pass_level >= m_fb_plan_cfg.fb_plan_vec[i].pass_level)
		{
			return m_fb_plan_cfg.fb_plan_vec[i].index;
		}
	}
	return -1;
}

int TouzijihuaConfig::GetBossPlanIndex(int kill_num)
{
	for (int i = (int)m_boss_plan_cfg.boss_plan_vec.size() - 1; i >= 0; --i)
	{
		if (kill_num >= m_boss_plan_cfg.boss_plan_vec[i].kill_num)
		{
			return m_boss_plan_cfg.boss_plan_vec[i].index;
		}
	}

	return -1;
}

int TouzijihuaConfig::GetShenyuBossPlanPrice() const
{
	return m_shenyu_boss_plan_price;
}

const ShenyuBossPlanConfig::BossPlanItem * TouzijihuaConfig::GetShenyuBossPlanCfgItem(int index) const
{
	for (unsigned int i = 0; i < m_shenyu_boss_plan_cfg.boss_plan_vec.size(); i++)
	{
		if (m_shenyu_boss_plan_cfg.boss_plan_vec[i].index == index)
		{
			return &m_shenyu_boss_plan_cfg.boss_plan_vec[i];
		}
	}

	return NULL;
}

int TouzijihuaConfig::GetShenyuBossPlanIndex(int kill_num)
{
	for (int i = (int)m_shenyu_boss_plan_cfg.boss_plan_vec.size() - 1; i >= 0; --i)
	{
		if (kill_num >= m_shenyu_boss_plan_cfg.boss_plan_vec[i].kill_num)
		{
			return m_shenyu_boss_plan_cfg.boss_plan_vec[i].index;
		}
	}

	return -1;
}

int TouzijihuaConfig::InitRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_level_list[TOUZI_JIHUA_PLAN_MAX_TYPE]; memset(last_level_list, 0, sizeof(last_level_list));

	while (!dataElement.empty())
	{
		int type = 0;
		if (!PugiGetSubNodeValue(dataElement, "type", type) || type < 0 || type >= TOUZI_JIHUA_PLAN_MAX_TYPE)
		{
			return -1;
		}

		TouzijihuaRewardConfig &reward_cfg = m_reward_cfglist[type];
		
		int seq = 0;
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq != reward_cfg.cfg_count || seq >= TouzijihuaRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -2;
		}

		TouzijihuaRewardConfig::ConfigItem &cfg_item = reward_cfg.cfg_list[seq];
		cfg_item.plan_type = type;
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(dataElement, "need_level", cfg_item.need_level) || cfg_item.need_level <= last_level_list[type]) 
		{
			return -3;
		}
		last_level_list[type] = cfg_item.need_level;

		if (!PugiGetSubNodeValue(dataElement, "reward_gold", cfg_item.reward_gold) || cfg_item.reward_gold < 0) 
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_gold_bind", cfg_item.reward_gold_bind) || cfg_item.reward_gold_bind < 0) 
		{
			return -5;
		}

		if ((0 == cfg_item.reward_gold && 0 == cfg_item.reward_gold_bind) ||
			(0 != cfg_item.reward_gold && 0 != cfg_item.reward_gold_bind))
		{
			return -6;
		}

		++ reward_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TouzijihuaConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	if (!PugiGetSubNodeValue(dataElement, "plan_0_price", m_plan_price[TOUZI_JIHUA_PLAN_TYPE_0]) || m_plan_price[TOUZI_JIHUA_PLAN_TYPE_0] <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "plan_1_price", m_plan_price[TOUZI_JIHUA_PLAN_TYPE_1]) || m_plan_price[TOUZI_JIHUA_PLAN_TYPE_1] <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "plan_2_price", m_plan_price[TOUZI_JIHUA_PLAN_TYPE_2]) || m_plan_price[TOUZI_JIHUA_PLAN_TYPE_2] <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "plan_3_price", m_plan_price[TOUZI_JIHUA_PLAN_TYPE_3]) || m_plan_price[TOUZI_JIHUA_PLAN_TYPE_3] <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "new_plan_price", m_new_plan_price) || m_new_plan_price <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "new_plan_reward", m_new_plan_first_reward) || m_new_plan_first_reward <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "fb_plan_price", m_fb_plan_price) || m_fb_plan_price <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "boss_plan_price", m_boss_plan_price) || m_boss_plan_price <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "shenyu_plan_price", m_shenyu_boss_plan_price) || m_shenyu_boss_plan_price <= 0)
	{
		return -9;
	}

	return 0;
}

int TouzijihuaConfig::InitNewRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100000;
	}

	for (int i = 0; i < MAX_NEW_TOUZIJIHUA_DAY; i++)
	{
		NewTouzijihuaRewardConfig &cfg = m_new_reward_cfg_list[i];
		if (!PugiGetSubNodeValue(data_element, "day_index", cfg.day_index) || cfg.day_index != i)
		{
			return -1;
		}

		{
			PugiXmlNode item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty())
			{
				return -1001;
			}

			PugiXmlNode item_element = item_list_element.child("reward_item");
			if (item_element.empty())
			{
				return -1002;
			}

			cfg.reward_item_count = 0;
			while (!item_element.empty())
			{
				if (cfg.reward_item_count >= MAX_NEW_TOUZIJIHUA_REWARD_COUNT)
				{
					return -1003;
				}

				if (!cfg.reward_item_list[cfg.reward_item_count].ReadConfig(item_element))
				{
					return -1004;
				}

				cfg.reward_item_count++;
				item_element = item_element.next_sibling();
			}
		}

		{
			PugiXmlNode item_list_element = data_element.child("vip_reward_item_list");
			if (item_list_element.empty())
			{
				return -2001;
			}

			PugiXmlNode item_element = item_list_element.child("vip_reward_item");
			if (item_element.empty())
			{
				return -2002;
			}

			cfg.vip_reward_item_count = 0;
			while (!item_element.empty())
			{
				if (cfg.vip_reward_item_count >= MAX_NEW_TOUZIJIHUA_REWARD_COUNT)
				{
					return -2003;
				}

				if (!cfg.vip_reward_item_list[cfg.vip_reward_item_count].ReadConfig(item_element))
				{
					return -2004;
				}

				cfg.vip_reward_item_count++;
				item_element = item_element.next_sibling();
			}
		}

		if (!PugiGetSubNodeValue(data_element, "reward_gold_bind", cfg.reward_gold_bind) || cfg.reward_gold_bind <= 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int TouzijihuaConfig::InitFoundationCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100000;
	}

	while (!data_element.empty())
	{
		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0)
		{
			return -1;
		}

		int chongzhi_gold = 0;
		if (!PugiGetSubNodeValue(data_element, "gold", chongzhi_gold) || chongzhi_gold <= 0)
		{
			return -2;
		}

		int active_level_min = 0;
		if (!PugiGetSubNodeValue(data_element, "active_level_min", active_level_min) || active_level_min <= 0)
		{
			return -3;
		}

		int active_level_max = 0;
		if (!PugiGetSubNodeValue(data_element, "active_level_max", active_level_max) || active_level_max < active_level_min)
		{
			return -4;
		}

		FoundationConfig *cfg = nullptr;
		for (auto iter = m_foundation_cfg_vec.begin(); iter != m_foundation_cfg_vec.end(); ++iter)
		{
			if (iter->chongzhi_gold == chongzhi_gold)
			{
				cfg = &(*iter);
				break;
			}
		}

		if (nullptr == cfg)
		{
			FoundationConfig new_cfg;
			new_cfg.seq = seq;
			new_cfg.chongzhi_gold = chongzhi_gold;
			new_cfg.active_level_min = active_level_min;
			new_cfg.active_level_max = active_level_max;
			m_foundation_cfg_vec.push_back(new_cfg);
			if (static_cast<int>(m_foundation_cfg_vec.size()) > MAX_FOUNDATION_TYPE)
			{
				return -101;
			}

			cfg = &(*m_foundation_cfg_vec.rbegin());
		}

		if (nullptr == cfg)
		{
			return -100;
		}

		FoundationConfig::FoundationRewardCfg reward_cfg;
		if (!PugiGetSubNodeValue(data_element, "reward_level", reward_cfg.reward_level) || reward_cfg.reward_level <= 0)
		{
			return -5;
		}

		//auto it = cfg->reward_cfg_vec.rbegin();
		//if (it != cfg->reward_cfg_vec.rend() && it->reward_level >= reward_cfg.reward_level)
		//{
		//	return -6;
		//}

		if (!PugiGetSubNodeValue(data_element, "reward_gold", reward_cfg.reward_gold) || reward_cfg.reward_gold < 0)
		{
			return -7;
		}

		cfg->reward_cfg_vec.push_back(reward_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int TouzijihuaConfig::InitFbPlanCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}
	int last_index = 0;
	int last_pass_level = 0;
	while (!dataElement.empty())
	{
		FbPlanConfig::FbPlanItem fb_plan_item;
		if (!PugiGetSubNodeValue(dataElement, "index", fb_plan_item.index) || fb_plan_item.index < 0 )
		{
			return -1;
		}
		if(fb_plan_item.index != last_index + 1 && fb_plan_item.index != 0)
		{
			return -2;
		}

		last_index = fb_plan_item.index;

		if (!PugiGetSubNodeValue(dataElement, "pass_level", fb_plan_item.pass_level) || fb_plan_item.pass_level < 0 || fb_plan_item.pass_level < last_pass_level)
		{
			return -3;
		}
		last_pass_level = fb_plan_item.pass_level;

		if (!PugiGetSubNodeValue(dataElement, "reward_gold_bind", fb_plan_item.reward_gold_bind) || fb_plan_item.reward_gold_bind < 0)
		{
			return -4;
		}

		m_fb_plan_cfg.fb_plan_vec.push_back(fb_plan_item);
		dataElement = dataElement.next_sibling();
	}
	return 0;
}
int TouzijihuaConfig::InitBossPlanCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}
	int last_index = 0;
	int last_kill_num = 0;
	while(!dataElement.empty())
	{
		BossPlanConfig::BossPlanItem boss_plan_item;
		if (!PugiGetSubNodeValue(dataElement, "index", boss_plan_item.index) || boss_plan_item.index < 0)
		{
			return -1;
		}
		if (boss_plan_item.index != last_index +1 && boss_plan_item.index != 0)
		{
			return -2;
		}
		last_index = boss_plan_item.index;

		if (!PugiGetSubNodeValue(dataElement, "kill_num", boss_plan_item.kill_num) || boss_plan_item.kill_num < 0 || boss_plan_item.kill_num < last_kill_num)
		{
			return-3;
		}
		last_kill_num = boss_plan_item.kill_num;

		if (!PugiGetSubNodeValue(dataElement, "reward_gold_bind", boss_plan_item.reward_gold_bind) || boss_plan_item.reward_gold_bind < 0)
		{
			return -4;
		}
		m_boss_plan_cfg.boss_plan_vec.push_back(boss_plan_item);
		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int TouzijihuaConfig::InitShenyuBossPlanCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}
	int last_index = 0;
	int last_kill_num = 0;
	while (!dataElement.empty())
	{
		ShenyuBossPlanConfig::BossPlanItem boss_plan_item;
		if (!PugiGetSubNodeValue(dataElement, "index", boss_plan_item.index) || boss_plan_item.index < 0)
		{
			return -1;
		}
		if (boss_plan_item.index != last_index + 1 && boss_plan_item.index != 0)
		{
			return -2;
		}
		last_index = boss_plan_item.index;

		if (!PugiGetSubNodeValue(dataElement, "kill_num", boss_plan_item.kill_num) || boss_plan_item.kill_num < 0 || boss_plan_item.kill_num < last_kill_num)
		{
			return-3;
		}
		last_kill_num = boss_plan_item.kill_num;

		if (!PugiGetSubNodeValue(dataElement, "reward_gold_bind", boss_plan_item.reward_gold_bind) || boss_plan_item.reward_gold_bind < 0)
		{
			return -4;
		}
		m_shenyu_boss_plan_cfg.boss_plan_vec.push_back(boss_plan_item);
		dataElement = dataElement.next_sibling();
	}
	return 0;
}
