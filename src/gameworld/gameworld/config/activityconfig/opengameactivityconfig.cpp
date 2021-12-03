#include "opengameactivityconfig.hpp"

#include "servercommon/configcommon.h"
#include "servercommon/vipdef.hpp"
#include "monster/monsterpool.h"

OpenGameActivityConfig::OpenGameActivityConfig()
{

}

OpenGameActivityConfig::~OpenGameActivityConfig()
{

}


bool OpenGameActivityConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "OpenGameActivityConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其它
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 装备收集
		PugiXmlNode root_element = RootElement.child("puton_equipment");
		if (root_element.empty())
		{
			*err = configname + ": no [puton_equipment].";
			return false;
		}

		iRet = this->InitPutonEquipmentRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPutonEquipmentRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 战力冲刺
		PugiXmlNode root_element = RootElement.child("capability");
		if (root_element.empty())
		{
			*err = configname + ": no [capability].";
			return false;
		}

		iRet = this->InitCapabilityRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCapabilityRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}


	{
		// 等级冲刺
		PugiXmlNode root_element = RootElement.child("rolelevel");
		if (root_element.empty())
		{
			*err = configname + ": no [rolelevel].";
			return false;
		}

		iRet = this->InitRoleLevelRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRoleLevelRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 装备礼包
		PugiXmlNode root_element = RootElement.child("equipment_gift");
		if (root_element.empty())
		{
			*err = configname + ": no [equipment_gift].";
			return false;
		}

		iRet = this->InitEquipmentGiftRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitEquipmentGiftRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 击杀boss_id
		PugiXmlNode root_element = RootElement.child("kill_boss");
		if (root_element.empty())
		{
			*err = configname + ": no [kill_boss].";
			return false;
		}

		iRet = this->InitKillBossIdConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitKillBossIdConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 击杀boss奖励
		PugiXmlNode root_element = RootElement.child("kill_boss_reward");
		if (root_element.empty())
		{
			*err = configname + ": no [kill_boss_reward].";
			return false;
		}

		iRet = this->InitKillBossRewardConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitKillBossRewardConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 礼包限购
		PugiXmlNode root_element = RootElement.child("gift_shop");
		if (root_element.empty())
		{
			*err = configname + ": no [gift_shop].";
			return false;
		}

		iRet = this->InitGiftShopConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGiftShopConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 礼包限购2
		PugiXmlNode root_element = RootElement.child("limit_buy_gift");
		if (root_element.empty())
		{
			*err = configname + ": no [limit_buy_gift].";
			return false;
		}

		iRet = this->InitGiftShop2Config(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGiftShop2Config failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int OpenGameActivityConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "baibeifanli_price", m_other_cfg.baibeifanli_price) || m_other_cfg.baibeifanli_price <= 0)
		{
			return -1;
		}

		PugiXmlNode item_element = data_element.child("baibeifanli_item");
		if (item_element.empty() || !m_other_cfg.baibeifanli_item.ReadConfig(item_element))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "baibeifanli_level_limit", m_other_cfg.baibeifanli_level_limit) || m_other_cfg.baibeifanli_level_limit < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "baibeifanli_end_time", m_other_cfg.baibeifanli_end_time) || m_other_cfg.baibeifanli_end_time < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "baibeifanli_price_2", m_other_cfg.baibeifanli2_price) || m_other_cfg.baibeifanli2_price < 0)
		{
			return -5;
		}

		PugiXmlNode baibeifanli_item_2_item_element = data_element.child("baibeifanli_item_2");
		if (baibeifanli_item_2_item_element.empty() || !m_other_cfg.baibeifanli2_item.ReadConfig(baibeifanli_item_2_item_element))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "baibeifanli_level_limit_2", m_other_cfg.baibeifanli2_level_limit) || m_other_cfg.baibeifanli2_level_limit < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "baibeifanli_price_2_openday", m_other_cfg.baibeifanli2_open_time) || m_other_cfg.baibeifanli2_open_time < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "baibeifanli_end_time_2", m_other_cfg.baibeifanli2_end_time) || m_other_cfg.baibeifanli2_end_time < 0)
		{
			return -9;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int OpenGameActivityConfig::InitPutonEquipmentRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	m_puton_equipment_reward_cfg.cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_puton_equipment_reward_cfg.cfg_count >= OGAPutonEquipmentRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		OGAPutonEquipmentRewardConfig::ConfigItem &cfg_item = m_puton_equipment_reward_cfg.cfg_list[m_puton_equipment_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_puton_equipment_reward_cfg.cfg_count || cfg_item.seq >= 32)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_num", cfg_item.need_num) || cfg_item.need_num <= 0 || cfg_item.need_num >= 10)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "need_jie", cfg_item.need_jie) || cfg_item.need_jie < 0 || cfg_item.need_jie >= ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "need_color", cfg_item.need_color) || cfg_item.need_color <= ItemBase::I_COLOR_INVALID || cfg_item.need_color >= ItemBase::I_COLOR_MAX)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "global_fetch_times", cfg_item.global_fetch_times) || cfg_item.global_fetch_times <= 0)
		{
			return -5;
		}

		{
			// 读物品奖励
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

			cfg_item.item_count = 0;

			while (!item_element.empty())
			{
				if (cfg_item.item_count >= OGAPutonEquipmentRewardConfig::REWARD_ITEM_MAX_COUNT)
				{
					return -1003;
				}

				if (!cfg_item.item_list[cfg_item.item_count].ReadConfig(item_element))
				{
					return -1004;
				}

				++ cfg_item.item_count;
				item_element = item_element.next_sibling();
			}
		}

		++ m_puton_equipment_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int OpenGameActivityConfig::InitCapabilityRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	m_capability_reward_cfg.cfg_count = 0;

	int last_need_capability = 0;

	while (!data_element.empty())
	{
		if (m_capability_reward_cfg.cfg_count >= OGACapabilityRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		OGACapabilityRewardConfig::ConfigItem &cfg_item = m_capability_reward_cfg.cfg_list[m_capability_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_capability_reward_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "capability", cfg_item.capability) || cfg_item.capability <= last_need_capability)
		{
			return -2;
		}
		last_need_capability = cfg_item.capability;

		if (!PugiGetSubNodeValue(data_element, "global_fetch_times", cfg_item.global_fetch_times) || cfg_item.global_fetch_times <= 0)
		{
			return -3;
		}

		{
			// 读物品奖励
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

			cfg_item.item_count = 0;

			while (!item_element.empty())
			{
				if (cfg_item.item_count >= OGACapabilityRewardConfig::REWARD_ITEM_MAX_COUNT)
				{
					return -1003;
				}

				if (!cfg_item.item_list[cfg_item.item_count].ReadConfig(item_element))
				{
					return -1004;
				}

				++ cfg_item.item_count;
				item_element = item_element.next_sibling();
			}
		}

		++ m_capability_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int OpenGameActivityConfig::InitRoleLevelRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	m_rolelevel_reward_cfg.cfg_count = 0;

	int last_rolelevel = 0;

	while (!data_element.empty())
	{
		if (m_rolelevel_reward_cfg.cfg_count >= OGARoleLevelRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		OGARoleLevelRewardConfig::ConfigItem &cfg_item = m_rolelevel_reward_cfg.cfg_list[m_rolelevel_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_rolelevel_reward_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "level", cfg_item.level) || cfg_item.level <= last_rolelevel)
		{
			return -2;
		}
		last_rolelevel = cfg_item.level;

		if (!PugiGetSubNodeValue(data_element, "global_fetch_times", cfg_item.global_fetch_times) || cfg_item.global_fetch_times <= 0)
		{
			return -3;
		}

		{
			// 读物品奖励
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

			cfg_item.item_count = 0;

			while (!item_element.empty())
			{
				if (cfg_item.item_count >= OGACapabilityRewardConfig::REWARD_ITEM_MAX_COUNT)
				{
					return -1003;
				}

				if (!cfg_item.item_list[cfg_item.item_count].ReadConfig(item_element))
				{
					return -1004;
				}

				++ cfg_item.item_count;
				item_element = item_element.next_sibling();
			}
		}

		++ m_rolelevel_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int OpenGameActivityConfig::InitEquipmentGiftRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_seq = 0;
	int last_prof = 0;

	while (!data_element.empty())
	{
 		OGAEquipmentGiftConfig::ConfigItem cfg_item;
 		
 		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq < 0 || cfg_item.seq >= OGAEquipmentGiftConfig::MAX_SEQ)
 		{
 			return -1;
 		}
 
 		if (cfg_item.seq != last_seq)
 		{
 			if (last_prof != 4)
 			{
 				return -10;
 			}
 
 			last_seq = cfg_item.seq;
 			last_prof = 0;
 		}
 
 		if (!PugiGetSubNodeValue(data_element, "prof", cfg_item.prof) || cfg_item.prof <= PROF_TYPE_INVALID ||  cfg_item.prof >= PROF_TYPE_PROF_NO_LIMIT || 1 != cfg_item.prof - last_prof)
 		{
 			return -2;
 		}
 		last_prof = cfg_item.prof;
 
 		if (!PugiGetSubNodeValue(data_element, "price", cfg_item.price) || cfg_item.price <= 0)
 		{
 			return -3;
 		}
 
 		PugiXmlNode Itemelement = data_element.child("reward_item");
 		if (!cfg_item.item.ReadConfig(Itemelement))
 		{
 			return -20;
 		}
 
 		m_equipmentgift_cfg.equipmentgift_vec.push_back(cfg_item);

		data_element = data_element.next_sibling();
	}
	return 0;
}

int OpenGameActivityConfig::InitKillBossIdConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int boss_seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", boss_seq) || boss_seq <= 0 || boss_seq >= OGA_BOSS_MAX_SEQ)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id", m_kill_boss_cfg.kill_boss_id_list[boss_seq]) || !MONSTERPOOL->IsMonsterExist(m_kill_boss_cfg.kill_boss_id_list[boss_seq]))
		{
			return -2;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int OpenGameActivityConfig::InitKillBossRewardConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int reward_seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", reward_seq) || reward_seq < 0 || reward_seq >= OGA_BOSS_REWARD_MAX_SEQ)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_seq_1", m_kill_boss_cfg.kill_boss_reward_list[reward_seq].boss_seq_1) || m_kill_boss_cfg.kill_boss_reward_list[reward_seq].boss_seq_1 < 0 || m_kill_boss_cfg.kill_boss_reward_list[reward_seq].boss_seq_1 >= OGA_BOSS_MAX_SEQ)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_seq_2", m_kill_boss_cfg.kill_boss_reward_list[reward_seq].boss_seq_2) || m_kill_boss_cfg.kill_boss_reward_list[reward_seq].boss_seq_2 < 0 || m_kill_boss_cfg.kill_boss_reward_list[reward_seq].boss_seq_2 >= OGA_BOSS_MAX_SEQ)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_seq_3", m_kill_boss_cfg.kill_boss_reward_list[reward_seq].boss_seq_3) || m_kill_boss_cfg.kill_boss_reward_list[reward_seq].boss_seq_3 < 0 || m_kill_boss_cfg.kill_boss_reward_list[reward_seq].boss_seq_3 >= OGA_BOSS_MAX_SEQ)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_seq_4", m_kill_boss_cfg.kill_boss_reward_list[reward_seq].boss_seq_4) || m_kill_boss_cfg.kill_boss_reward_list[reward_seq].boss_seq_4 < 0 || m_kill_boss_cfg.kill_boss_reward_list[reward_seq].boss_seq_4 >= OGA_BOSS_MAX_SEQ)
		{
			return -5;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -6;
		}

		if (!m_kill_boss_cfg.kill_boss_reward_list[reward_seq].reward_item.ReadConfig(item_element))
		{
			return -7;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int OpenGameActivityConfig::InitGiftShopConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		OGAGiftShopConfig cfg_item;

		if (!PugiGetSubNodeValue(data_element, "level", cfg_item.level) || cfg_item.level <= 0 || cfg_item.level > MAX_ROLE_LEVEL)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq < 0 || cfg_item.seq >= OGA_GIFT_MAX_SEQ)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "price", cfg_item.price) || cfg_item.price <= 0)
		{
			return -3;
		}

		char name_str[32] = {0};
		for (int i = 0; i < OGA_GIFT_MAX_TYPE; i++)
		{
			sprintf(name_str, "gift_item_%d", i);

			PugiXmlNode item_element = data_element.child(name_str);
			if (item_element.empty())
			{
				return 1000 -i * 10 - 1;
			}

			int item_id = 0;
			if (PugiGetSubNodeValue(item_element, "item_id", item_id) && item_id > 0)
			{
				if (!cfg_item.gift_item_list[i].ReadConfig(item_element))
				{
					return 1000 -i * 10 - 2;
				}
			}
			else
			{
				break;
			}
		}

		m_gift_shop_cfg_vec.push_back(cfg_item);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int OpenGameActivityConfig::InitGiftShop2Config(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		OGAGiftShopConfig cfg_item;

		if (!PugiGetSubNodeValue(data_element, "level", cfg_item.level) || cfg_item.level <= 0 || cfg_item.level > MAX_ROLE_LEVEL)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq < 0 || cfg_item.seq >= OGA_GIFT_MAX_SEQ)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "price", cfg_item.price) || cfg_item.price <= 0)
		{
			return -3;
		}

		char name_str[32] = { 0 };
		for (int i = 0; i < OGA_GIFT_MAX_TYPE; i++)
		{
			sprintf(name_str, "gift_item_%d", i);

			PugiXmlNode item_element = data_element.child(name_str);
			if (item_element.empty())
			{
				return 1000 - i * 10 - 1;
			}

			int item_id = 0;
			if (PugiGetSubNodeValue(item_element, "item_id", item_id) && item_id > 0)
			{
				if (!cfg_item.gift_item_list[i].ReadConfig(item_element))
				{
					return 1000 - i * 10 - 2;
				}
			}
			else
			{
				break;
			}
		}

		m_gift_shop_2_cfg_vec.push_back(cfg_item);
		data_element = data_element.next_sibling();
	}

	return 0;
}

const OGAOtherConfig & OpenGameActivityConfig::GetOtherConfig()
{
	return m_other_cfg;
}

const OGAPutonEquipmentRewardConfig::ConfigItem * OpenGameActivityConfig::GetPutonEquipmentReward(int seq)
{
	if (seq >= 0 && seq < OGAPutonEquipmentRewardConfig::CONFIG_ITEM_MAX_COUNT && seq < m_puton_equipment_reward_cfg.cfg_count)
	{
		return &m_puton_equipment_reward_cfg.cfg_list[seq];
	}

	return NULL;
}

const OGACapabilityRewardConfig::ConfigItem * OpenGameActivityConfig::GetCapabiblityReward(int seq)
{
	if (seq >= 0 && seq < OGACapabilityRewardConfig::CONFIG_ITEM_MAX_COUNT && seq < m_capability_reward_cfg.cfg_count)
	{
		return &m_capability_reward_cfg.cfg_list[seq];
	}

	return NULL;
}

const OGARoleLevelRewardConfig::ConfigItem * OpenGameActivityConfig::GetRolelevelReward(int seq)
{
	if (seq >= 0 && seq < OGARoleLevelRewardConfig::CONFIG_ITEM_MAX_COUNT && seq < m_rolelevel_reward_cfg.cfg_count)
	{
		return &m_rolelevel_reward_cfg.cfg_list[seq];
	}

	return NULL;
}

const OGAEquipmentGiftConfig::ConfigItem * OpenGameActivityConfig::GetEquipmentGiftItemCfg(int seq, int prof)
{
 	for (OGAEquipmentGiftConfig::EquipmentGiftVec::const_iterator iter = m_equipmentgift_cfg.equipmentgift_vec.begin(); iter != m_equipmentgift_cfg.equipmentgift_vec.end(); ++ iter)
 	{
 		if (seq == iter->seq && prof == iter->prof)
		{
			return &(*iter);
		}
 	}

	return NULL;
}

const OGAGiftShopConfig * OpenGameActivityConfig::GetGiftShopConfig(int level, int seq)
{
	if (level <= 0 || level > MAX_ROLE_LEVEL || seq < 0 || seq >= OGA_GIFT_MAX_SEQ)
	{
		return nullptr;
	}

	for (auto it = m_gift_shop_cfg_vec.rbegin(); it != m_gift_shop_cfg_vec.rend(); it++)
	{
		if (level >= it->level && seq == it->seq)
		{
			return &(*it);
		}
	}

	return nullptr;
}

const OGAGiftShopConfig * OpenGameActivityConfig::GetGiftShop2Config(int level, int seq)
{
	if (level <= 0 || level > MAX_ROLE_LEVEL || seq < 0 || seq >= OGA_GIFT_MAX_SEQ)
	{
		return nullptr;
	}

	for (auto it = m_gift_shop_2_cfg_vec.rbegin(); it != m_gift_shop_2_cfg_vec.rend(); it++)
	{
		if (level >= it->level && seq == it->seq)
		{
			return &(*it);
		}
	}

	return nullptr;
}

int OpenGameActivityConfig::GetPutonEquipmentActFlag(ItemGridData equipment_list[Equipment::E_INDEX_MAX])
{
	int act_flag = 0;

	for (int i = 0; i < m_puton_equipment_reward_cfg.cfg_count && i < OGAPutonEquipmentRewardConfig::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		OGAPutonEquipmentRewardConfig::ConfigItem &cfg_item = m_puton_equipment_reward_cfg.cfg_list[i];
		int act_num = 0;

		for (int k = 0; k < Equipment::E_INDEX_MAX; ++ k)
		{
			const ItemBase *item_base = ITEMPOOL->GetItem(equipment_list[k].item_id);
			if (NULL == item_base || ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType())
			{
				continue;
			}

			EquipmentParam *param = (EquipmentParam *)equipment_list[k].param;
			if (NULL == param)
			{
				continue;
			}

			if (1 == item_base->GetLimitLevel())		// 新手不算
			{
				continue;
			}

			const Equipment *equipment = (Equipment *)item_base;

			if (equipment->GetOrder() >= cfg_item.need_jie && equipment->GetColor() >= cfg_item.need_color)
			{
				++ act_num;
			}
		}

		if (act_num >= cfg_item.need_num)
		{
			act_flag = act_flag | (1 << i);
		}
	}

	return act_flag;
}

int OpenGameActivityConfig::GetKillBossSeq(int boss_id)
{
	for (int i = 0; i < OGA_BOSS_MAX_SEQ; i++)
	{
		if (m_kill_boss_cfg.kill_boss_id_list[i] == boss_id)
		{
			return i;
		}
	}

	return 0;
}

const OGAKillBossReward * OpenGameActivityConfig::GetKillBossReward(int seq)
{
	if (seq < 0 || seq >= OGA_BOSS_REWARD_MAX_SEQ)
	{
		return NULL;
	}

	return &m_kill_boss_cfg.kill_boss_reward_list[seq];
}