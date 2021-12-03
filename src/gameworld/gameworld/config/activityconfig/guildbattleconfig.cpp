// #include "guildbattleconfig.hpp"
// 
// #include "monster/monsterpool.h"
// #include "monster/monsterinitparam.h"
// #include "item/itempool.h"
// 
// #include "servercommon/configcommon.h"
// 
// #include "checkresourcecenter.hpp"
// #include <string>
// 
// GuildBattleConfig::GuildBattleConfig()
// {
// }
// 
// GuildBattleConfig::~GuildBattleConfig()
// {
// }
// 
// bool GuildBattleConfig::Init(const std::string &configname, std::string *err)
// {
// 	char err_info[1024] = {0};
// 	int iRet = 0;
// 
// 	TiXmlDocument document;
// 
// 	if ("" == configname || !document.LoadFile(configname))
// 	{
// 		sprintf(err_info,"%s load GuildBattleConfig fail !\n %s",configname.c_str(),document.ErrorDesc());
// 		*err = err_info;
// 		return false;
// 	}
// 
// 	TiXmlElement *RootElement = document.RootElement();
// 	if (NULL == RootElement)
// 	{
// 		*err = configname + " xml root node error";
// 		return false;
// 	}
// 
// 	{	// 其他配置
// 		TiXmlElement *root_elment = RootElement->FirstChildElement("other");
// 		if (NULL == root_elment)
// 		{
// 			*err = configname + "xml not other node";
// 			return false;
// 		}
// 		iRet = this->InitGuildBattleOtherCfg(root_elment);
// 		if (0 != iRet)
// 		{
// 			sprintf(err_info, "%s InitGuildBattleOtherCfg fail %d ", configname.c_str(), iRet);
// 			*err = err_info;
// 			return false;
// 		}
// 
// 
// 		
// 	}
// 	
// 	{	// 怪物配置
// 		TiXmlElement *root_element = RootElement->FirstChildElement("monster");
// 		if (NULL == root_element)
// 		{
// 			*err = configname + "xml not scence_monster node";
// 			return false;
// 		}
// 
// 		iRet = this->InitGuildBattleMonsterCfg(root_element);
// 		if (0 != iRet)
// 		{
// 			sprintf(err_info, "%s InitGuildBattleMonsterCfg fail %d ", configname.c_str(), iRet);
// 			*err = err_info;
// 			return false;
// 		}
// 	}
// 
// 	{	// 护送物品积分配置
// 		TiXmlElement *root_element = RootElement->FirstChildElement("goods_credit");
// 		if (NULL == root_element)
// 		{
// 			*err = configname + "xml not goods_credit node";
// 			*err = err_info;
// 			return false;
// 		}
// 
// 		iRet = this->InitGuildBattleGoodsCreditCfg(root_element);
// 		if (0 != iRet)
// 		{
// 			sprintf(err_info,"%s InitGuildBattleGoodsCreditCfg fail %d",configname.c_str(),iRet);
// 			*err = err_info;
// 			return false;
// 		}
// 	}
// 
// 	{	// 击杀玩家积分配置
// 		TiXmlElement *root_element = RootElement->FirstChildElement("kill_other_credit");
// 		if (NULL == root_element)
// 		{
// 			*err = configname + "xml not kill_other_credit node";
// 			return false;
// 		}
// 
// 		iRet = this->InitGuildBattleKillOtherCreditCfg(root_element);
// 		if (0 != iRet)
// 		{
// 			sprintf(err_info,"%s InitGuildBattleKillOtherCreditCfg fail %d",configname.c_str(),iRet);
// 			*err = err_info;
// 			return false;
// 		}
// 	}
// 
// 	{	// 个人积分奖励配置
// 		TiXmlElement *root_element = RootElement->FirstChildElement("personal_credit_reward");
// 		if (NULL == root_element)
// 		{
// 			*err = configname + "xml not personal_credit_reward node";
// 			return false;
// 		}
// 
// 		iRet = this->InitGuildBattlePersonalRewardCfg(root_element);
// 		if (0 != iRet)
// 		{
// 			sprintf(err_info,"%s InitGuildBattlePersonalRewardCfg fail %d",configname.c_str(),iRet);
// 			*err = err_info;
// 			return false;
// 		}
// 	}
// 
// 	{	// 积分排名奖励配置
// 		TiXmlElement *root_element = RootElement->FirstChildElement("rank_reward");
// 		if (NULL == root_element)
// 		{
// 			*err = configname + "xml not rank_reward node";
// 			return false;
// 		}
// 
// 		iRet =  this->InitGuildBattleRankRewardCfg(root_element);
// 		if (0 != iRet)
// 		{
// 			sprintf(err_info,"%s InitGuildBattleRankRewardCfg fail %d",configname.c_str(),iRet);
// 			*err = err_info;
// 			return false;
// 		}
// 	}
// 
// 	{	// 稀有怪随机坐标配置
// 		TiXmlElement *root_element = RootElement->FirstChildElement("rare_monster_refresh_pos");
// 		if (NULL == root_element)
// 		{
// 			*err = configname + "xml not rare_monster_refresh_pos node";
// 			return false;
// 		}
// 
// 		iRet =  this->InitGuildBattleRareMonsterPos(root_element);
// 		if (0 != iRet)
// 		{
// 			sprintf(err_info,"%s InitGuildBattleRareMonsterPos fail %d",configname.c_str(),iRet);
// 			*err = err_info;
// 			return false;
// 		}
// 	}
// 
// 	{	// 银箱子随机坐标配置
// 		TiXmlElement *root_element = RootElement->FirstChildElement("sliver_refresh_pos");
// 		if (NULL == root_element)
// 		{
// 			*err = configname + "xml not sliver_refresh_pos node";
// 			return false;
// 		}
// 
// 		iRet =  this->InitGuildBattleSliverBoxPos(root_element);
// 		if (0 != iRet)
// 		{
// 			sprintf(err_info,"%s InitGuildBattleSliverBoxPos fail %d",configname.c_str(),iRet);
// 			*err = err_info;
// 			return false;
// 		}
// 	}
// 
// 	return true;
// }
// 
// int GuildBattleConfig::GetGuildBattleEnterInfo(Posi *pos)
// {
// 	if (NULL == pos) return 0;
// 	*pos = m_other_cfg.realive_pos;
// 	return m_other_cfg.scene_id;
// }
// 
// const GuildBattleMonsterCfg * GuildBattleConfig::GuildBattleGetMonsterCfg(int monster_id) const
// {
// 	if (monster_id <= 0) return NULL;
// 
// 	const std::map<int, GuildBattleMonsterCfg>::const_iterator iter = m_monster_cfg_list.find(monster_id);
// 
// 	if (m_monster_cfg_list.end() == iter) 
// 		return NULL;
// 	else	
// 	{
// 		if (iter->second.monster_id <= 0)
// 			return NULL;
// 	}
// 
// 	return &iter->second;
// 	
// }
// 
// const GuildBattleMonsterCfg * GuildBattleConfig::GetBossMonsterByWorldLevel(int world_level)
// {
// 	if (world_level < 0 || world_level > MAX_ROLE_LEVEL)
// 	{
// 		return NULL;
// 	}
// 
// 	std::map<int, GuildBattleMonsterCfg>::iterator iter = m_monster_cfg_list.begin();
// 	for (;iter != m_monster_cfg_list.end(); iter++)
// 	{
// 		if (iter->second.is_boss != 1)
// 		{
// 			continue;
// 		}
// 		
// 		if (world_level < iter->second.world_level_min || world_level > iter->second.world_level_max)
// 		{
// 			continue;
// 		}
// 		
// 		return &iter->second;
// 	}
// 
// 	return NULL;
// }
// 
// const GuildBattleMonsterCfg * GuildBattleConfig::GetRareMonsterByWorldLevel(int world_level)
// {
// 	if (world_level < 0 || world_level > MAX_ROLE_LEVEL)
// 	{
// 		return NULL;
// 	}
// 
// 	std::map<int, GuildBattleMonsterCfg>::iterator iter = m_monster_cfg_list.begin();
// 	for (;iter != m_monster_cfg_list.end(); iter++)
// 	{
// 		if (iter->second.is_rare_monster != 1)
// 		{
// 			continue;
// 		}
// 
// 		if (world_level < iter->second.world_level_min || world_level > iter->second.world_level_max)
// 		{
// 			continue;
// 		}
// 
// 		return &iter->second;
// 	}
// 
// 	return NULL;
// }
// 
// const GuildBattleGoodsCreditCfg * GuildBattleConfig::GetGoodsCreditCfg(int gather_id) const
// {
// 	if (gather_id < 0) return NULL;
// 	
// 	std::map<int,GuildBattleGoodsCreditCfg>::const_iterator iter = m_goods_credit_list.find(gather_id);
// 
// 	if (m_goods_credit_list.end() == iter)
// 		return NULL;
// 	else
// 	{
// 		if (iter->second.gather_id<= 0 || iter->second.gather_time_s < 0 || iter->second.reward_credit_personal <= 0 || iter->second.reward_credit_guild <= 0 || iter->second.speed_slow_down > 10000)
// 			return NULL;
// 	}
// 
// 	return &iter->second;
// }
// 
// const GuildBattleKillOtherCreditCfg * GuildBattleConfig::GetKillOtherCreditCfg(int kill_count) const
// {
// 	if (kill_count <= 0) return NULL;
// 
// 	std::map<int,GuildBattleKillOtherCreditCfg>::const_iterator iter = m_kill_other_credit_list.begin();
// 
// 	for (;iter != m_kill_other_credit_list.end();iter++)
// 	{
// 		if (iter->second.kill_count_min <= kill_count && kill_count <= iter->second.kill_count_max)
// 		{
// 			return &iter->second;
// 		}
// 	}
// 
// 	return NULL;
// }
// 
// const GuildBattlePersonalRewardCfg * GuildBattleConfig::GetPersonalRewardCfg(int require_credit) const
// {
// 	if (require_credit <= 0) return NULL;
// 
// 	std::map<int,GuildBattlePersonalRewardCfg>::const_iterator iter = m_personal_reward_list.begin();
// 
// 	for (;iter != m_personal_reward_list.end();iter++)
// 	{
// 		if (iter->second.require_credit_min <= require_credit && require_credit <= iter->second.require_credit_max)
// 		{
// 			return &iter->second;
// 		}
// 	}
//  
//  	return NULL;
// }
// 
// const GuildBattlePersonalRewardCfg * GuildBattleConfig::GetPersonalRewardCfgByIndex(int index) const
// {
// 	if (index <= 0) return NULL;
// 
// 	std::map<int,GuildBattlePersonalRewardCfg>::const_iterator iter = m_personal_reward_list.begin();
// 
// 	for (;iter != m_personal_reward_list.end();iter++)
// 	{
// 		if (iter->second.reward_index == index)
// 		{
// 			return &iter->second;
// 		}
// 	}
// 
// 	return NULL;
// }
// 
// const GuildBattleRankRewardCfg *GuildBattleConfig::GetRankRewardCfg(int rank)const
// {
// 	if (rank < 0) return NULL;
// 
// 	const std::map<int, GuildBattleRankRewardCfg>::const_iterator iter = m_rank_reward_list.find(rank);
// 	if (m_rank_reward_list.end() == iter)
// 		return NULL;
// 	
// 	return &iter->second;
// }
// 
// const GuildBattleRareMonsterPos* GuildBattleConfig::GetRareMonsterRandPost() const
// {
// 	int count = static_cast<int>(m_rare_monster_pos.size());
// 
// 	if (count > 0)
// 	{
// 		int rand_pos = RandomNum(count);
// 		return &m_rare_monster_pos[rand_pos];
// 	}
// 	return NULL;
// }
// 
// const Posi * GuildBattleConfig::GetSilverBoxRandPost() const
// {
// 	int count = static_cast<int>(m_silver_box_pos.size());
// 
// 	if (count > 0)
// 	{
// 		int rand_pos = RandomNum(count);
// 		return &m_silver_box_pos[rand_pos];
// 	}
// 	return NULL;
// }
// 
// 
// int GuildBattleConfig::InitGuildBattleOtherCfg(TiXmlElement *RootElement)
// {
// 	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
// 	if (NULL == dataElement)
// 	{
// 		return -10000;
// 	}
// 
// 	if(!GetSubNodeValue(dataElement,"scene_id",m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
// 	{
// 		return -1;
// 	}
// 	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.scene_id);
// 
// 	if (!GetSubNodeValue(dataElement,"realive_pos_x",m_other_cfg.realive_pos.x) || m_other_cfg.realive_pos.x <= 0)
// 	{
// 		return -2;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"realive_pos_y",m_other_cfg.realive_pos.y) || m_other_cfg.realive_pos.y <= 0)
// 	{
// 		return -3;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"boss_x",m_other_cfg.boss_refresh_pos.x) || m_other_cfg.boss_refresh_pos.x <= 0)
// 	{
// 		return -4;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"boss_y",m_other_cfg.boss_refresh_pos.y) || m_other_cfg.boss_refresh_pos.y <= 0)
// 	{
// 		return -5;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"recalc_time",m_other_cfg.recalc_time) || m_other_cfg.recalc_time <= 0)
// 	{
// 		return -6;
// 	}
// 	
// 	if (!GetSubNodeValue(dataElement,"shield_times_1",m_other_cfg.shield_times_1) || m_other_cfg.shield_times_1 <= 0)
// 	{
// 		return -8;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"shield_times_2",m_other_cfg.shield_times_2) || m_other_cfg.shield_times_2 <= 0)
// 	{
// 		return -9;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"silver_box_count",m_other_cfg.silver_box_count) || m_other_cfg.silver_box_count < 0)
// 	{
// 		return -10;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"rand_x",m_other_cfg.rand_x) || m_other_cfg.rand_x <= 0)
// 	{
// 		return -11;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"rand_y",m_other_cfg.rand_y) || m_other_cfg.rand_y <= 0)
// 	{
// 		return -12;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"glod_box_count",m_other_cfg.glod_box_count) || m_other_cfg.glod_box_count < 0)
// 	{
// 		return -13;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"gold_box_id",m_other_cfg.gold_box_id) || m_other_cfg.gold_box_id <= 0)
// 	{
// 		return -14;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"sliver_box_id",m_other_cfg.sliver_box_id) || m_other_cfg.sliver_box_id <= 0)
// 	{
// 		return -15;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"common_box_id",m_other_cfg.common_box_id) || m_other_cfg.common_box_id <= 0)
// 	{
// 		return -16;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"token_id",m_other_cfg.token_id) || m_other_cfg.token_id <= 0)
// 	{
// 		return -17;
// 	}
// 
// 	if (!GetSubNodeValue(dataElement,"rare_monster_count",m_other_cfg.refresh_rare_monster_count) || m_other_cfg.refresh_rare_monster_count < 0)
// 	{
// 		return -18;
// 	}
// 
// 	{
// 		TiXmlElement *element = dataElement->FirstChildElement("huizhang_extra_reward_list");
// 		if (NULL == element)
// 		{
// 			return -100;
// 		}
// 
// 		int i = 0;
// 		TiXmlElement *item_element = element->FirstChildElement("huizhang_extra_reward");
// 		while (NULL != item_element)
// 		{
// 			if (i >= MAX_ATTACHMENT_ITEM_NUM)
// 			{
// 				return -101 - i;
// 			}
// 
// 			if (!m_other_cfg.huizhang_extra_reward[i].ReadConfig(item_element))
// 			{
// 				return - 151 - i;
// 			}
// 
// 			i++;
// 			m_other_cfg.huizhang_extra_reward_item_count++;
// 			item_element = item_element->NextSiblingElement();
// 		}
// 	}
// 
// 	if (!GetSubNodeValue(dataElement, "lucky_interval", m_other_cfg.lucky_interval) || m_other_cfg.lucky_interval <= 0)
// 	{
// 		return -19;
// 	}
// 
// 	{
// 		TiXmlElement *Itemelement = dataElement->FirstChildElement("lucky_item");
// 		ItemID itemid = 0;
// 		if (NULL != Itemelement && GetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid) )
// 		{
// 			if (!m_other_cfg.lucky_item.ReadConfig(Itemelement))
// 			{
// 				return -20;
// 			}
// 
// 		}else
// 		{
// 			return -21;
// 		}
// 	}
// 
// 	if (!GetSubNodeValue(dataElement, "luck_people", m_other_cfg.luck_people_num) || m_other_cfg.luck_people_num <= 0)
// 	{
// 		return -22;
// 	}
// 
// 	return 0;
// }
// 
// int GuildBattleConfig::InitGuildBattleMonsterCfg(TiXmlElement *RootElement)
// {
// 	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
// 	if (NULL == dataElement)
// 	{
// 		return -10000;
// 	}
// 
// 	int last_monster_id = 0;
// 
// 	while(NULL != dataElement)
// 	{
// 		int monster_id = 0;
// 		if (!GetSubNodeValue(dataElement, "monster_id", monster_id) || monster_id <= 0 || !MONSTERPOOL->IsMonsterExist(monster_id))
// 		{ 
// 			return -1;
// 		}
// 		GuildBattleMonsterCfg &cfg = m_monster_cfg_list[monster_id];
// 		cfg.monster_id = monster_id;
// 		last_monster_id = monster_id;
// 		
// 		if (!GetSubNodeValue(dataElement,"refresh_time_s",cfg.first_refresh_time_s) || cfg.first_refresh_time_s < 0)
// 		{
// 			return -2;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"refresh_interval_s",cfg.refresh_interval_s) || cfg.refresh_interval_s <= 0)
// 		{
// 			return -3;
// 		}
// 		
// 		if (!GetSubNodeValue(dataElement,"kill_personal_credit",cfg.kill_personal_credit) || cfg.kill_guild_credit < 0)
// 		{
// 			return -4;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"kill_guild_credit",cfg.kill_guild_credit) || cfg.kill_guild_credit < 0)
// 		{
// 			return -5;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"world_level_min",cfg.world_level_min) || cfg.world_level_min < 0)
// 		{
// 			return -6;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"world_level_max",cfg.world_level_max) || cfg.world_level_max < 0 || cfg.world_level_max < cfg.world_level_min)
// 		{
// 			return -7;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"is_boss",cfg.is_boss) || (cfg.is_boss != 0 && cfg.is_boss != 1))
// 		{
// 			return -8;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"is_rare_monster",cfg.is_rare_monster) || (cfg.is_rare_monster != 0 && cfg.is_rare_monster != 1))
// 		{
// 			return -9;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"prepare_notice_time_s",cfg.prepare_notice_time_s) || cfg.prepare_notice_time_s < 0 || cfg.prepare_notice_time_s >= cfg.refresh_interval_s)
// 		{
// 			return -10;
// 		}
// 
// 		dataElement = dataElement->NextSiblingElement();
// 	}
// 
// 	return 0;
// }
// 
// int GuildBattleConfig::InitGuildBattleGoodsCreditCfg(TiXmlElement *RootElement)
// {
// 	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
// 	if (NULL ==dataElement)
// 	{
// 		return -10000;
// 	}
// 	
// 	while (NULL != dataElement)
// 	{
// 		int gather_id = 0;
// 		if (!GetSubNodeValue(dataElement, "gather_id", gather_id) || gather_id < 0)
// 		{
// 			return -1;
// 		}
// 
// 		if (m_goods_credit_list.end() != m_goods_credit_list.find(gather_id))
// 		{
// 			return -100;
// 		}
// 
// 		GuildBattleGoodsCreditCfg &cfg = m_goods_credit_list[gather_id];
// 		cfg.gather_id = gather_id;
// 
// 		if (!GetSubNodeValue(dataElement, "gather_time_s",cfg.gather_time_s) || cfg.gather_time_s < 0)
// 		{
// 			return -3;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement, "reward_credit_personal",cfg.reward_credit_personal) || cfg.reward_credit_personal < 0)
// 		{
// 			return -4;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement, "reward_credit_guild",cfg.reward_credit_guild) || cfg.reward_credit_guild < 0)
// 		{
// 			return -5;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement, "speed_ratio",cfg.speed_slow_down) || cfg.speed_slow_down < 0 || cfg.speed_slow_down > 10000)
// 		{
// 			return -6;
// 		}
// 
// 		dataElement = dataElement->NextSiblingElement();
// 	}
// 	
// 	return 0;
// }
// 
// int GuildBattleConfig::InitGuildBattleKillOtherCreditCfg(TiXmlElement *RootElemen)
// {
// 	TiXmlElement *dataElement = RootElemen->FirstChildElement("data");
// 	if (NULL == dataElement)
// 	{
// 		return -10000;
// 	}
// 
// 	int last_kill_count_min = 0;
// 
// 	while(NULL != dataElement)
// 	{
// 
// 		int kill_count_min = 0;
// 		if (!GetSubNodeValue(dataElement, "kill_count_min" ,kill_count_min) || kill_count_min <= 0 || kill_count_min <= last_kill_count_min )
// 		{
// 			return -1;
// 		}
// 		
// 		if (m_kill_other_credit_list.end() != m_kill_other_credit_list.find(kill_count_min))
// 		{
// 			return -100;
// 		}
// 
// 		GuildBattleKillOtherCreditCfg &cfg = m_kill_other_credit_list[kill_count_min];
// 		cfg.kill_count_min = kill_count_min;
// 		last_kill_count_min = kill_count_min;
// 
// 		if (!GetSubNodeValue(dataElement,"kill_count_max",cfg.kill_count_max) || cfg.kill_count_max <= 0 || cfg.kill_count_min > cfg.kill_count_max )
// 		{
// 			return -2;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"reward_person_credit",cfg.reward_preson_credit) || cfg.reward_preson_credit <= 0)
// 		{
// 			return -3;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"reward_guild_credit",cfg.reward_guild_credit) || cfg.reward_guild_credit <= 0)
// 		{
// 			return -4;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"kill_count_index",cfg.kill_count_index) || cfg.kill_count_index <= 0 || cfg.kill_count_index > 31)
// 		{
// 			return -5;
// 		}
// 
// 		dataElement = dataElement->NextSiblingElement();
// 	}
// 
// 	return 0;
// }
// 
// int GuildBattleConfig::InitGuildBattlePersonalRewardCfg(TiXmlElement *RootElemen)
// {
// 	TiXmlElement *dataElement = RootElemen->FirstChildElement("data");
// 	if (NULL == dataElement)
// 	{
// 		return -10000;
// 	}
// 
// 	int last_require_credit_min = 0;
// 	int last_index = 0;
// 	
// 	while(NULL != dataElement)
// 	{
// 		int require_credit_min = 0;
// 		if (!GetSubNodeValue(dataElement, "reward_credit_min" ,require_credit_min) || require_credit_min < 0 || require_credit_min <= last_require_credit_min)
// 		{
// 			return -1;
// 		}
// 
// 		if (m_personal_reward_list.end() != m_personal_reward_list.find(require_credit_min))
// 		{
// 			return -100;
// 		}
// 
// 		GuildBattlePersonalRewardCfg &cfg = m_personal_reward_list[require_credit_min];
// 		cfg.require_credit_min = require_credit_min;
// 		last_require_credit_min = require_credit_min;
// 
// 		if (!GetSubNodeValue(dataElement,"require_credit_max",cfg.require_credit_max) || cfg.require_credit_max <= 0 || cfg.require_credit_min > cfg.require_credit_max)
// 		{
// 			return -2;
// 		}
// 
// 		{
// 			TiXmlElement *element = dataElement->FirstChildElement("reward_item_list");
// 			if (NULL == element)
// 			{
// 				return -100;
// 			}
// 
// 			int i = 0;
// 			TiXmlElement *item_element = element->FirstChildElement("reward_item");
// 			while (NULL != item_element)
// 			{
// 				if (i >= MAX_ATTACHMENT_ITEM_NUM)
// 				{
// 					return -101 - i;
// 				}
// 
// 				if (!cfg.reward_item[i].ReadConfig(item_element))
// 				{
// 					return - 151 - i;
// 				}
// 
// 				i++;
// 				item_element = item_element->NextSiblingElement();
// 			}
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"banggong",cfg.banggong) || cfg.banggong < 0)
// 		{
// 			return -3;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"reward_index",cfg.reward_index) || cfg.reward_index <= 0 || cfg.reward_index > 31 || cfg.reward_index < last_index)
// 		{
// 			return -4;
// 		}
// 		last_index = cfg.reward_index;
// 		
// 		if (!GetSubNodeValue(dataElement,"shengwang",cfg.shengwang) || cfg.shengwang < 0)
// 		{
// 			return -5;
// 		}
// 
// 		dataElement = dataElement->NextSiblingElement();
// 	}
// 
// 	return 0;
// }
// 
// int GuildBattleConfig::InitGuildBattleRankRewardCfg(TiXmlElement *RootElemen)
// {
// 	TiXmlElement *dataElement = RootElemen->FirstChildElement("data");
// 	if (NULL == dataElement)
// 	{
// 		return -10000;
// 	}
// 
// 	int last_rank = 0;
// 	while(NULL != dataElement)
// 	{
// 		int rank;
// 		if (!GetSubNodeValue(dataElement,"rank",rank) || rank < 0 || rank < last_rank)
// 		{
// 			return -1;
// 		}
// 
// 		if (m_rank_reward_list.end() != m_rank_reward_list.find(rank))
// 		{
// 			return -100;
// 		}
// 
// 		GuildBattleRankRewardCfg &cfg = m_rank_reward_list[rank];
// 		cfg.rank = rank;
// 		last_rank = rank;
// 
// 		if (!GetSubNodeValue(dataElement,"base_role_exp",cfg.base_role_exp) || cfg.base_role_exp < 0)
// 		{
// 			return -4;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"banggong",cfg.banggong) || cfg.banggong < 0)
// 		{
// 			return -5;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement,"shengwang",cfg.shengwang) || cfg.shengwang < 0)
// 		{
// 			return -6;
// 		}
// 
// 		{
// 			TiXmlElement *element = dataElement->FirstChildElement("reward_item_list");
// 			if (NULL == element)
// 			{
// 				return -100;
// 			}
// 
// 			int i = 0;
// 			TiXmlElement *item_element = element->FirstChildElement("reward_item");
// 			while (NULL != item_element)
// 			{
// 				if (i >= MAX_ATTACHMENT_ITEM_NUM)
// 				{
// 					return -101 - i;
// 				}
// 
// 				if (!cfg.reward_item[i].ReadConfig(item_element))
// 				{
// 					return - 151 - i;
// 				}
// 
// 				i++;
// 				cfg.reward_item_count++;
// 				item_element = item_element->NextSiblingElement();
// 			}
// 		}
// 
// 		dataElement = dataElement->NextSiblingElement();
// 	}
// 	
// 	return 0;
// }
// 
// int GuildBattleConfig::InitGuildBattleRareMonsterPos(TiXmlElement *RootElemen)
// {
// 	TiXmlElement *dataElement = RootElemen->FirstChildElement("data");
// 	if (NULL == dataElement)
// 	{
// 		return -10000;
// 	}
// 
// 	while (NULL != dataElement)
// 	{
// 		GuildBattleRareMonsterPos positem;
// 
// 		if (!GetSubNodeValue(dataElement, "scene_id", positem.scene_id))
// 		{
// 			return -1;
// 		}
// 		CheckResourceCenter::Instance().GetSceneCheck()->Add(positem.scene_id);
// 
// 		if (!GetSubNodeValue(dataElement, "pos_x", positem.rare_monster_pos.x) || positem.rare_monster_pos.x <= 0)
// 		{
// 			return -2;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement, "pos_y", positem.rare_monster_pos.y) || positem.rare_monster_pos.y <= 0)
// 		{
// 			return -3;
// 		}
// 
// 		m_rare_monster_pos.push_back(positem);
// 
// 		dataElement = dataElement->NextSiblingElement();
// 	}
// 
// 	return 0;
// 
// }
// 
// int GuildBattleConfig::InitGuildBattleSliverBoxPos(TiXmlElement *RootElemen)
// {
// 	TiXmlElement *dataElement = RootElemen->FirstChildElement("data");
// 	if (NULL == dataElement)
// 	{
// 		return -10000;
// 	}
// 
// 	while (NULL != dataElement)
// 	{
// 		Posi pos;
// 
// 		if (!GetSubNodeValue(dataElement, "pos_x", pos.x) || pos.x <= 0)
// 		{
// 			return -1;
// 		}
// 
// 		if (!GetSubNodeValue(dataElement, "pos_y", pos.y) || pos.y <= 0)
// 		{
// 			return -2;
// 		}
// 
// 		m_silver_box_pos.push_back(pos);
// 
// 		dataElement = dataElement->NextSiblingElement();
// 	}
// 
// 	return 0;
// 
// }
