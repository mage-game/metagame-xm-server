#include "guildactivityconfig.hpp"

#include "item/itempool.h"
#include "monster/monsterpool.h"

#include "checkresourcecenter.hpp"

GuildActivityConfig::GuildActivityConfig() :m_boss_max_count(0), m_boss_gather_cfg_count(0), m_signin_cfg_count(0),
					m_tianci_tongbi_last_time(0), m_tianci_tongbi_standy_time(0), m_tianci_tongbi_max_npc_touch_distance(0), m_tianci_tongbi_flush_bigcoin_need_maturity(0), 
	m_tianci_tongbi_flush_big_coin_num(0), m_tianci_tongbi_gold_bind_num(0), m_tianci_tongbi_exp_count_limit(0), m_tianci_tongbi_bigcoin_gather_id(0), m_tianci_tongbi_npc_pos(0, 0), m_tianci_tongbi_acquire_gold_bind_limit(0),
	m_tianci_tongbi_gather_refresh_pos_count(0), m_tianci_tongbi_bigcoin_flush_pos_count(0), m_tianci_tongbi_bigcoin_flush_pos_total_rate(0)
{
}

GuildActivityConfig::~GuildActivityConfig()
{

}

bool GuildActivityConfig::Init(const std::string &configname, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "GuildActivityConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load Guild config [%s] Error.cannot find root node.", configname.c_str());
		*err = errinfo;
		return false;
	}

	{
		PugiXmlNode bossElement = RootElement.child("other");
		if (bossElement.empty())
		{
			*err = configname + ": no xml node [config->other]";
			return false;
		}

		iRet = this->InitOtherCfg(bossElement);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode bossElement = RootElement.child("boss_cfg");
		if (bossElement.empty())
		{
			*err = configname + ": no xml node [config->boss_cfg]";
			return false;
		}

		iRet = this->InitBossCfg(bossElement);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitBossCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode bossElement = RootElement.child("boss_gather_cfg");
		if (bossElement.empty())
		{
			*err = configname + ": no xml node [config->boss_gather_cfg]";
			return false;
		}

		iRet = this->InitBossGatherCfg(bossElement);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitBossGatherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 排行奖励
	{
		PugiXmlNode bossElement = RootElement.child("hurt_boss_rank");
		if (bossElement.empty())
		{
			*err = configname + ": no xml node [config->hurt_boss_rank]";
			return false;
		}

		iRet = this->InitBossRankRewardCfg(bossElement);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitBossRankRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 星级配置
	{
		PugiXmlNode bossElement = RootElement.child("star_level_cfg");
		if (bossElement.empty())
		{
			*err = configname + ": no xml node [config->star_level_cfg]";
			return false;
		}

		iRet = this->InitBossStarLevelCfg(bossElement);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitBossStarLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode SignElement = RootElement.child("signin_cfg");
		if (SignElement.empty())
		{
			*err = configname + ": no xml node [config->signin_cfg]";
			return false;
		}

		iRet = this->InitSigninCfg(SignElement);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitSigninCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 天赐铜币
	{
		PugiXmlNode TianCiTongBiElement = RootElement.child("guild_tianci_tongbi_gather");
		if (TianCiTongBiElement.empty())
		{
			*err = configname + ": no xml node [config->guild_tianci_tongbi_gather]";
			return false;
		}

		iRet = this->InitTianCiTongBiGather(TianCiTongBiElement);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitTianCiTongBiGather failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode TianCiTongBiCoinPosListElement = RootElement.child("guild_tianci_tongbi_coin_pos_list");
		if (TianCiTongBiCoinPosListElement.empty())
		{
			*err = configname + ": no xml node [config->guild_tianci_tongbi_coin_pos_list]";
			return false;
		}

		iRet = this->InitTianCiTongBiBigCoinFlushPos(TianCiTongBiCoinPosListElement);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitTianCiTongBiBigCoinFlushPos failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode TianCiTongBiGatherRefreshPosElement = RootElement.child("guild_tianci_tongbi_gather_refresh_pos");
		if (TianCiTongBiGatherRefreshPosElement.empty())
		{
			*err = configname + ": no xml node [config->guild_tianci_tongbi_gather_refresh_pos]";
			return false;
		}

		iRet = this->InitTianCiTongBiGatherRefreshPos(TianCiTongBiGatherRefreshPosElement);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitTianCiTongBiGatherRefreshPos failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 经验球采集物配置
	{
		PugiXmlNode TianCiTongBiGatherRefreshPosElement = RootElement.child("collection_exp_ball");
		if (TianCiTongBiGatherRefreshPosElement.empty())
		{
			*err = configname + ": no xml node [config->collection_exp_ball]";
			return false;
		}

		iRet = this->InitTianCiTongBiBigExpBallCfg(TianCiTongBiGatherRefreshPosElement);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitTianCiTongBiBigExpBallCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int GuildActivityConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	if (!PugiGetSubNodeValue(data_element, "boss_super_call_gold", m_other_cfg.boss_super_call_gold) || m_other_cfg.boss_super_call_gold <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "boss_normal_call_max_count", m_other_cfg.boss_normal_call_max_count) || m_other_cfg.boss_normal_call_max_count < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "boss_super_call_max_count", m_other_cfg.boss_super_call_max_count) || m_other_cfg.boss_super_call_max_count < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "boss_uplevel_item_id", m_other_cfg.boss_uplevel_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.boss_uplevel_item_id))
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "boss_uplevel_item_exp", m_other_cfg.boss_uplevel_item_exp) || m_other_cfg.boss_uplevel_item_exp <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "rank_update_interval_s", m_other_cfg.rank_update_interval_s) || m_other_cfg.rank_update_interval_s <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "boss_notice_timelimit_min", m_other_cfg.boss_notice_timelimit_min) || m_other_cfg.boss_notice_timelimit_min <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "signin_gongxian", m_other_cfg.signin_gongxian) || m_other_cfg.signin_gongxian < 0)
	{
		return -40;
	}

	if (!PugiGetSubNodeValue(data_element, "signin_gold_bind", m_other_cfg.signin_gold_bind) || m_other_cfg.signin_gold_bind < 0)
	{
		return -41;
	}

	if (!PugiGetSubNodeValue(data_element, "signin_title_need_num", m_other_cfg.signin_title_need_num) || m_other_cfg.signin_title_need_num < 0)
	{
		return -42;
	}

	int is_open = 0;
	if (!PugiGetSubNodeValue(data_element, "signin_is_open", is_open) || is_open < 0)
	{
		return -42;
	}
	m_other_cfg.signin_is_open = (0 != is_open);

	if (!PugiGetSubNodeValue(data_element, "signin_need_level", m_other_cfg.signin_need_level) || m_other_cfg.signin_need_level < 0)
	{
		return -43;
	}

	if (!PugiGetSubNodeValue(data_element, "guild_tianci_tongbi_last_time", m_tianci_tongbi_last_time) || m_tianci_tongbi_last_time <= 0)
	{
		return -44;
	}

	if (!PugiGetSubNodeValue(data_element, "guild_tianci_tongbi_standy_s", m_tianci_tongbi_standy_time) || m_tianci_tongbi_standy_time < 0)
	{
		return -45;
	}

	if (!PugiGetSubNodeValue(data_element, "guild_tianci_tongbi_refresh_coin_need_maturity", m_tianci_tongbi_flush_bigcoin_need_maturity) || m_tianci_tongbi_flush_bigcoin_need_maturity <= 0)
	{
		return -47;
	}

	if (!PugiGetSubNodeValue(data_element, "guild_tianci_tongbi_refresh_coin_num", m_tianci_tongbi_flush_big_coin_num) || m_tianci_tongbi_flush_big_coin_num <= 0)
	{
		return -48;
	}

	if (!PugiGetSubNodeValue(data_element, "tianci_tongbi_bigcoin_gather_id", m_tianci_tongbi_bigcoin_gather_id) || m_tianci_tongbi_bigcoin_gather_id < 0)
	{
		return -49;
	}

	if (!PugiGetSubNodeValue(data_element, "bigcoin_gather_disppear_time_s", m_bigcoin_gather_disppear_time_s) || m_bigcoin_gather_disppear_time_s < 0)
	{
		return -50;
	}

// 	if (!PugiGetSubNodeValue(data_element, "guild_tianci_tongbi_reward_num_bigcoin", m_tianci_tongbi_gold_bind_num) || m_tianci_tongbi_gold_bind_num <= 0)
// 	{
// 		return -50;
// 	}

	if (!PugiGetSubNodeValue(data_element, "guild_tianci_tongbi_commit_gather_npc_pos_x", m_tianci_tongbi_npc_pos.x) || m_tianci_tongbi_npc_pos.x <= 0)
	{
		return -52;
	}

	if (!PugiGetSubNodeValue(data_element, "guild_tianci_tongbi_commit_gather_npc_pos_y", m_tianci_tongbi_npc_pos.y) || m_tianci_tongbi_npc_pos.y <= 0)
	{
		return -53;
	}

	if (!PugiGetSubNodeValue(data_element, "tianci_tongbi_max_npc_touch_distance", m_tianci_tongbi_max_npc_touch_distance) || m_tianci_tongbi_max_npc_touch_distance < 0)
	{
		return -54;
	}

// 	if (!PugiGetSubNodeValue(data_element, "tianci_tongbi_gold_bind_limit", m_tianci_tongbi_acquire_gold_bind_limit) || m_tianci_tongbi_acquire_gold_bind_limit < 0)
// 	{
// 		return -55;
// 	}

	if (!PugiGetSubNodeValue(data_element, "tianci_tongbi_exp_count_limit", m_tianci_tongbi_exp_count_limit) || m_tianci_tongbi_exp_count_limit <= 0)
	{
		return -56;
	}

	{
		PugiXmlNode element = data_element.child("signin_item_list");
		if (element.empty())
		{
			return -100;
		}

		int i = 0;
		PugiXmlNode item_element = element.child("signin_item");
		while (!item_element.empty())
		{
			if (i >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -101 - i;
			}

			if (!m_other_cfg.signin_item[i].ReadConfig(item_element))
			{
				if (0 == m_other_cfg.signin_item[i].item_id)
				{
					break;
				}

				return -151 - i;
			}

			i++;
			item_element = item_element.next_sibling();
		}
	}

	if (!PugiGetSubNodeValue(data_element, "exp_factor_collection", m_other_cfg.exp_factor) || m_other_cfg.exp_factor < 0)
	{
		return -57;
	}

	return 0;
}

int GuildActivityConfig::InitBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}
	
	int boss_level = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "boss_level", boss_level) || boss_level < 0 || boss_level >= GUILD_BOSS_MAX_LEVEL)
		{
			return -1;
		}
		GuildBossConfig &cfg = m_boss_cfg_list[boss_level];
		cfg.level = boss_level;

		if (!PugiGetSubNodeValue(data_element, "uplevel_exp", cfg.uplevel_exp) || cfg.uplevel_exp < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id", cfg.boss_id) || !MONSTERPOOL->IsMonsterExist(cfg.boss_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_x", cfg.pos.x) || cfg.pos.x <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_y", cfg.pos.y) || cfg.pos.y <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "totem_exp_reward", cfg.totem_exp_reward) || cfg.totem_exp_reward < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_gather_bind_gold", cfg.boss_gather_bind_gold) || cfg.boss_gather_bind_gold <= 0)
		{
			return -7;
		}

		{
			PugiXmlNode item_element = data_element.child("normal_item_reward");
			if (item_element.empty() || !cfg.normal_item_reward.ReadConfig(item_element))
			{
				return -101;
			}
		}

		{
			PugiXmlNode item_element = data_element.child("super_item_reward");
			if (item_element.empty() || !cfg.super_item_reward.ReadConfig(item_element))
			{
				return -102;
			}
		}

		{
			PugiXmlNode item_element = data_element.child("super_call_item_reward");
			if (item_element.empty() || !cfg.super_call_item_reward.ReadConfig(item_element))
			{
				return -103;
			}
		}

		m_boss_max_count++;
		data_element = data_element.next_sibling();
	}

	if (m_boss_max_count < 2)
	{
		return -7;
	}

	return 0;
}

int GuildActivityConfig::InitBossGatherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_boss_gather_cfg_count = 0;
	while (!data_element.empty())
	{
		if (m_boss_gather_cfg_count >= MAX_GUILD_BOSS_GATHER_COUNT)
		{
			return -100;
		}

		GuildBossGatherConfig &cfg = m_boss_gather_cfg_list[m_boss_gather_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "gather_id", cfg.gather_id) || cfg.gather_id <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_x", cfg.pos.x) || cfg.pos.x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_y", cfg.pos.y) || cfg.pos.y <= 0)
		{
			return -3;
		}

		m_boss_gather_cfg_count++;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int GuildActivityConfig::InitBossRankRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	while (!data_element.empty())
	{
		GuildBossRankRewardCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "min_rank", cfg.min_rank) || cfg.min_rank < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "max_rank", cfg.max_rank) || cfg.max_rank < 0 || cfg.min_rank > cfg.max_rank)
		{
			return -2;
		}

		{
			cfg.reward_count = ItemConfigData::ReadConfigList(data_element, std::string("reward_item"),
				cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (cfg.reward_count < 0)
			{
				return -200 + cfg.reward_count;
			}
		}

		m_rank_reward_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GuildActivityConfig::InitBossStarLevelCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	int star_level_count = 0;
	int last_cost_time_s = 0;
	while (!dataElement.empty())
	{
		if (star_level_count < 0 || star_level_count >= GUILD_BOSS_STAR_LEVEL_COUNT)
		{
			return -1;
		}

		GuildBossStarLevelCfg &cfg = m_star_level_cfg_list[star_level_count];

		if (!PugiGetSubNodeValue(dataElement, "cost_time_s", cfg.cost_time_s) || cfg.cost_time_s < 0)
		{
			return -2;
		}

		if (cfg.cost_time_s > 0 && last_cost_time_s > 0 && cfg.cost_time_s > last_cost_time_s)
		{
			return -3;
		}

		last_cost_time_s = cfg.cost_time_s;

		if (!PugiGetSubNodeValue(dataElement, "star_level", cfg.star_level) || cfg.star_level <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_id", cfg.gather_id) || cfg.gather_id <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "limit_num", cfg.limit_num) || cfg.limit_num <= 0)
		{
			return -6;
		}

		{
			cfg.reward_count = ItemConfigData::ReadConfigList(dataElement, std::string("reward_item"),
				cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (cfg.reward_count < 0)
			{
				return -200 + cfg.reward_count;
			}
		}

		++star_level_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildActivityConfig::InitSigninCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	m_signin_cfg_count = 0;
	int last_need_count = 0;
	while (!dataElement.empty())
	{
		if (m_signin_cfg_count >= GUILD_MAX_SIGN_IN_CFG_COUNT)
		{
			return -1001;
		}

		int index = 0;
		if (!PugiGetSubNodeValue(dataElement, "index", index) || index < 0 || index != m_signin_cfg_count)
		{
			return -1002;
		}

		GuildSigninCfg &cfg = m_signin_cfg[index];
		cfg.index = index;

		if (!PugiGetSubNodeValue(dataElement, "need_count", cfg.need_count) || cfg.need_count <= last_need_count)
		{
			return -1003;
		}
		last_need_count = cfg.need_count;

		{
			PugiXmlNode item_element = dataElement.child("reward_item");
			if (item_element.empty() || !cfg.reward_item.ReadConfig(item_element))
			{
				return -101;
			}
		}

		++m_signin_cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildActivityConfig::InitTianCiTongBiGather(PugiXmlNode RootElement)
{
	int gather_type = GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID + 1;
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		if (gather_type >= GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_id", m_tianci_tongbi_gather_list[gather_type].gather_id) || m_tianci_tongbi_gather_list[gather_type].gather_id <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "rate", m_tianci_tongbi_gather_list[gather_type].rate) || m_tianci_tongbi_gather_list[gather_type].rate <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_item_num", m_tianci_tongbi_gather_list[gather_type].reward_mojing_num) || m_tianci_tongbi_gather_list[gather_type].reward_mojing_num <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_maturity", m_tianci_tongbi_gather_list[gather_type].reward_maturity) || m_tianci_tongbi_gather_list[gather_type].reward_maturity <= 0)
		{
			return -6;
		}

		++gather_type;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildActivityConfig::InitTianCiTongBiGatherRefreshPos(PugiXmlNode RootElement)
{
	m_tianci_tongbi_gather_refresh_pos_count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		if (m_tianci_tongbi_gather_refresh_pos_count >= (int)static_array_size(m_tianci_tongbi_gather_refresh_pos_list))
		{
			return -1;
		}

		TianCiTongBiGatherRefreshPos &flush_pos = m_tianci_tongbi_gather_refresh_pos_list[m_tianci_tongbi_gather_refresh_pos_count];

		if (!PugiGetSubNodeValue(dataElement, "guild_tianci_tongbi_gather_pos_x", flush_pos.pos_x) || flush_pos.pos_x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "guild_tianci_tongbi_gather_pos_y", flush_pos.pos_y) || flush_pos.pos_y <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_id", flush_pos.gather_id) || flush_pos.gather_id <= 0)
		{
			return -4;
		}

		++m_tianci_tongbi_gather_refresh_pos_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildActivityConfig::InitTianCiTongBiBigCoinFlushPos(PugiXmlNode RootElement)
{
	m_tianci_tongbi_bigcoin_flush_pos_count = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		if (m_tianci_tongbi_bigcoin_flush_pos_count >= GUILD_TIANCI_TONGBI_BIGCOIN_FLUSH_POS_MAX)
		{
			return -1;
		}

		TianCiTongBiBigCoinFlushPos &flush_pos = m_tianci_tongbi_bigcoin_flush_pos_list[m_tianci_tongbi_bigcoin_flush_pos_count];

		if (!PugiGetSubNodeValue(dataElement, "x", flush_pos.pos_x) || flush_pos.pos_x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "y", flush_pos.pos_y) || flush_pos.pos_y <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "rand_dis", flush_pos.range) || flush_pos.range <= 0)
		{
			return -4;
		}

		flush_pos.rate = flush_pos.range * flush_pos.range;
		m_tianci_tongbi_bigcoin_flush_pos_total_rate += flush_pos.rate;

		++m_tianci_tongbi_bigcoin_flush_pos_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildActivityConfig::InitTianCiTongBiBigExpBallCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		int role_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "role_level", role_level) || role_level <= 0)
		{
			return -1;
		}

		int collection_exp = 0;
		if (!PugiGetSubNodeValue(dataElement, "collection_exp", collection_exp) || collection_exp <= 0)
		{
			return -2;
		}

		if (m_exp_ball_map.end() != m_exp_ball_map.find(role_level))
		{
			return -100;
		}

		m_exp_ball_map[role_level] = collection_exp;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const GuildBossConfig * GuildActivityConfig::GetBossCfg(int boss_level)
{
	if (boss_level < 0 || boss_level >= GUILD_BOSS_MAX_LEVEL || m_boss_cfg_list[boss_level].level != boss_level)
	{
		return NULL;
	}

	return &m_boss_cfg_list[boss_level];
}

const GuildBossGatherConfig * GuildActivityConfig::GetBossGatherCfg(int index)
{
	if (index < 0 || index >= MAX_GUILD_BOSS_GATHER_COUNT)
	{
		return NULL;
	}

	return &m_boss_gather_cfg_list[index];
}

const GuildBossRankRewardCfg * GuildActivityConfig::GetRankRewardCfg(const int rank)
{
	for (std::vector<GuildBossRankRewardCfg>::iterator iter = m_rank_reward_vec.begin(); iter != m_rank_reward_vec.end(); iter++)
	{
		if ((*iter).min_rank <= rank && rank <= (*iter).max_rank)
		{
			return &(*iter);
		}
	}
	return NULL;
}

const GuildBossStarLevelCfg * GuildActivityConfig::GetStarLevelCfg(int star_level)
{
	for (int i = 0; i < GUILD_BOSS_STAR_LEVEL_COUNT; i++)
	{
		if (star_level == m_star_level_cfg_list[i].star_level)
		{
			return &m_star_level_cfg_list[i];
		}
	}

	return NULL;
}

const GuildSigninCfg * GuildActivityConfig::GetSigninCfg(int index) const
{
	if (index < 0 || index >= m_signin_cfg_count || m_signin_cfg[index].index != index)
	{
		return NULL;
	}

	return  &m_signin_cfg[index];
}

int GuildActivityConfig::GetTianCiTongBiCoinRewardExp(int role_level)
{
	auto it = m_exp_ball_map.find(role_level);
	if(it != m_exp_ball_map.end())
	{
		return it->second;
	}

	return 0;
}

int GuildActivityConfig::RandTianCiTongBiGatherId()
{
	int total_rate = 0;
	for (int i = GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID + 1; i < GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX; i++)
	{
		total_rate += m_tianci_tongbi_gather_list[i].rate;
	}

	if (total_rate <= 0) return 0;

	int rand_rate = RandomNum(total_rate);
	for (int i = GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID + 1; i < GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX; i++)
	{
		if (rand_rate <= m_tianci_tongbi_gather_list[i].rate)
		{
			return m_tianci_tongbi_gather_list[i].gather_id;
		}

		rand_rate -= m_tianci_tongbi_gather_list[i].rate;
	}

	return 0;
}

int GuildActivityConfig::GetTianCiTongBiGatherType(int gather_id)
{
	for (int i = GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID + 1; i < GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX; i++)
	{
		if (gather_id == m_tianci_tongbi_gather_list[i].gather_id)
		{
			return i;
		}
	}

	return GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID;
}

TianCiTongBiGather * GuildActivityConfig::GetTianCiTongBiGatherReward(int gather_type)
{
	//if (NULL == reward_mojing || NULL == reward_maturity) return nullptr;

	if (gather_type < GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID || gather_type >= GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX) return nullptr;

	//*reward_mojing = m_tianci_tongbi_gather_list[gather_type].reward_item_num;
	//*reward_maturity = m_tianci_tongbi_gather_list[gather_type].reward_maturiy;

	return &m_tianci_tongbi_gather_list[gather_type];
}

TianCiTongBiGatherRefreshPos *GuildActivityConfig::GetTianCiTongBiGatherPos(int &gather_pos_num)
{
	if (m_tianci_tongbi_gather_refresh_pos_count > 0 && m_tianci_tongbi_gather_refresh_pos_count <= GUILD_TIANCI_TONGBI_GATHER_REFRESH_POS_MAX)
	{
		//int index = RandomNum(m_tianci_tongbi_gather_refresh_pos_count);
		//
		//pos_x = m_tianci_tongbi_gather_refresh_pos_list[index].pos_x;
		//pos_y = m_tianci_tongbi_gather_refresh_pos_list[index].pos_y;
		//range = m_tianci_tongbi_gather_refresh_pos_list[index].gather_id;
		gather_pos_num = m_tianci_tongbi_gather_refresh_pos_count;

		return m_tianci_tongbi_gather_refresh_pos_list;
	}

	return nullptr;
}

bool GuildActivityConfig::GetTianCiTongBiBigCoinPos(int & pos_x, int & pos_y, int & range)
{
	if (m_tianci_tongbi_bigcoin_flush_pos_count <= 0) return false;

	if (m_tianci_tongbi_bigcoin_flush_pos_total_rate <= 0) return false;
	int rand_rate = RandomNum(m_tianci_tongbi_bigcoin_flush_pos_total_rate);
	int rand_index = 0;

	for (int i = 0; i < m_tianci_tongbi_bigcoin_flush_pos_count && i < GUILD_TIANCI_TONGBI_BIGCOIN_FLUSH_POS_MAX; i++)
	{
		if (rand_rate <= m_tianci_tongbi_bigcoin_flush_pos_list[i].rate)
		{
			rand_index = i;
			break;
		}
		else
		{
			rand_rate -= m_tianci_tongbi_bigcoin_flush_pos_list[i].rate;
		}
	}

	if (0 <= rand_index && rand_index < GUILD_TIANCI_TONGBI_BIGCOIN_FLUSH_POS_MAX)
	{
		pos_x = m_tianci_tongbi_bigcoin_flush_pos_list[rand_index].pos_x;
		pos_y = m_tianci_tongbi_bigcoin_flush_pos_list[rand_index].pos_y;
		range = m_tianci_tongbi_bigcoin_flush_pos_list[rand_index].range;

		return true;
	}

	return false;
}

int GuildActivityConfig::GetTianCiTreeNpcInfo(Posi &pos)
{
	pos.x = m_tianci_tongbi_npc_pos.x;
	pos.y = m_tianci_tongbi_npc_pos.y;

	return 0;
}
