#include "combineserveractivityconfig.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "config/logicconfigmanager.hpp"
#include "global/combineserveractivity/combineserveractivityconfig.hpp"
#include "item/itempool.h"
#include "monster/monsterpool.h"

CombineServerActivityConfig::CombineServerActivityConfig() 
{
	memset(m_sub_activity_rank_type_map, 0, sizeof(m_sub_activity_rank_type_map));
	memset(m_rank_type_sub_activity_map, 0, sizeof(m_rank_type_sub_activity_map));

	m_sub_activity_rank_type_map[CSA_SUB_TYPE_RANK_QIANGGOU] = CSA_RANK_TYPE_QIANGGOU;
	m_sub_activity_rank_type_map[CSA_SUB_TYPE_CHONGZHI_RANK] = CSA_RANK_TYPE_CHONGZHI;
	m_sub_activity_rank_type_map[CSA_SUB_TYPE_CONSUME_RANK] = CSA_RANK_TYPE_CONSUME;

	for (int sub_type = CSA_SUB_TYPE_INVALID + 1; sub_type < CSA_SUB_TYPE_MAX; ++ sub_type)
	{
		int rank_type = m_sub_activity_rank_type_map[sub_type];
		if (rank_type > CSA_RANK_TYPE_INVALID && rank_type < CSA_RANK_TYPE_MAX)
		{
			m_rank_type_sub_activity_map[rank_type] = sub_type;
		}
	}
}

CombineServerActivityConfig::~CombineServerActivityConfig()
{

}

bool CombineServerActivityConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CombineServerActivityConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 子活动时间配置
		PugiXmlNode root_element = RootElement.child("activity_time");
		if (root_element.empty())
		{
			*err = path + " xml not activity_time node ";
			return false;
		}

		iRet = this->InitSubActivityTimeCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitSubActivityTimeCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 排行奖励
		PugiXmlNode root_element = RootElement.child("rank_reward");
		if (root_element.empty())
		{
			*err = path + " xml not rank_reward node ";
			return false;
		}

		iRet = this->InitRankRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRankRewardCfg fail %d ", path.c_str(), iRet);
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
		// 抢购配置
		PugiXmlNode root_element = RootElement.child("qianggou");
		if (root_element.empty())
		{
			*err = path + " xml not qianggou node ";
			return false;
		}

		iRet = this->InitRankQianggouCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRankQianggouCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 摇奖配置
		PugiXmlNode root_element = RootElement.child("roll_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not roll_cfg node ";
			return false;
		}

		iRet = this->InitRollCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRollCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 单笔充值配置
		PugiXmlNode root_element = RootElement.child("single_charge");
		if (root_element.empty())
		{
			*err = path + " xml not single_charge node ";
			return false;
		}

		iRet = this->InitSingleChargeCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitSingleChargeCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 登录奖励配置
		PugiXmlNode root_element = RootElement.child("login_gift");
		if (root_element.empty())
		{
			*err = path + " xml not login_gift node ";
			return false;
		}

		iRet = this->InitLoginGiftRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitLoginGiftRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 全民疯抢配置
		PugiXmlNode root_element = RootElement.child("server_panic_buy");
		if (root_element.empty())
		{
			*err = path + ": xml no server_panic_buy node";
			return false;
		}

		iRet = this->InitServerPanicBuyCfg(root_element);
		if (iRet)
		{
			sprintf(err_info,"%s: InitServerPanicBuyCfg failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 个人疯抢配置
		PugiXmlNode root_element = RootElement.child("personal_panic_buy");
		if (root_element.empty())
		{
			*err = path + ": xml no personal_panic_buy node";
			return false;
		}

		iRet = this->InitPersonalPanicBuyCfg(root_element);
		if (iRet)
		{
			sprintf(err_info,"%s: InitPersonalPanicBuyCfg failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 投资计划
		PugiXmlNode root_element = RootElement.child("touzi_jihua");
		if (root_element.empty())
		{
			*err = path + ": xml no touzi_jihua node";
			return false;
		}

		iRet = this->InitTouzijihuaCfg(root_element);
		if (iRet)
		{
			sprintf(err_info, "%s: InitTouzijihuaCfg failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 成长基金
		PugiXmlNode root_element = RootElement.child("foundation");
		if (root_element.empty())
		{
			*err = path + ": xml no foundation node";
			return false;
		}

		iRet = this->InitFoundationCfg(root_element);
		if (iRet)
		{
			sprintf(err_info, "%s: InitFoundationCfg failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 经验炼制
		PugiXmlNode root_element = RootElement.child("exp_refine");
		if (root_element.empty())
		{
			*err = path + ": xml no exp_refine node";
			return false;
		}

		iRet = this->InitExpRefineCfg(root_element);
		if (iRet)
		{
			sprintf(err_info, "%s: InitExpRefineCfg failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// BOSS抢夺
		PugiXmlNode root_element = RootElement.child("kill_boss");
		if (root_element.empty())
		{
			*err = path + ": xml no kill_boss node";
			return false;
		}

		iRet = this->InitKillBossCfg(root_element);
		if (iRet)
		{
			sprintf(err_info, "%s: InitKillBossCfg failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

CSATimeConfig * CombineServerActivityConfig::GetCSATimeConfig(int activity_sub_type)
{
	if (activity_sub_type < 0 || activity_sub_type >= CSA_SUB_TYPE_MAX || activity_sub_type <= CSA_SUB_TYPE_INVALID) return NULL;

	return &m_time_config_list[activity_sub_type];
}

bool CombineServerActivityConfig::IsRankSubActivityType(int activity_sub_type)
{
	if (activity_sub_type > CSA_SUB_TYPE_INVALID && activity_sub_type < CSA_SUB_TYPE_MAX)
	{
		int rank_type = LOGIC_CONFIG->GetCombineServerActivityConfig().GetRelatedRankType(activity_sub_type);

		return rank_type > CSA_RANK_TYPE_INVALID && rank_type < CSA_RANK_TYPE_MAX;
	}

	return false;
}

int CombineServerActivityConfig::GetRelatedRankType(int sub_type)
{
	if (sub_type > CSA_SUB_TYPE_INVALID && sub_type < CSA_SUB_TYPE_MAX)
	{
		return m_sub_activity_rank_type_map[sub_type];
	}

	return CSA_RANK_TYPE_INVALID;
}

int CombineServerActivityConfig::GetRelatedSubType(int rank_type)
{
	if (rank_type > CSA_RANK_TYPE_INVALID && rank_type < CSA_RANK_TYPE_MAX)
	{
		return m_rank_type_sub_activity_map[rank_type];
	}

	return CSA_SUB_TYPE_INVALID;
}

const CSARankRewardConfig * CombineServerActivityConfig::GetCSARankRewardCfgByRankType(int rank_type)
{
	int sub_type = this->GetRelatedSubType(rank_type);

	return this->GetCSARankRewardCfgBySubType(sub_type);
}

const CSARankRewardConfig * CombineServerActivityConfig::GetCSARankRewardCfgBySubType(int sub_type)
{
	if (sub_type > CSA_SUB_TYPE_INVALID && sub_type < CSA_SUB_TYPE_MAX)
	{
		if (m_rank_reward_list[sub_type].IsValid())
		{
			return &m_rank_reward_list[sub_type];
		}
	}

	return NULL;
}

const CSAQianggouItemConfig * CombineServerActivityConfig::GetRankQianggouCfg(int item_idx)
{
	if (item_idx >= 0 && item_idx < COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE)
	{
		return &m_rank_qianggou_itemlist[item_idx];
	}

	return NULL;
}


const CSASingleChargeCfg::ConfigItem * CombineServerActivityConfig::GetSingleChargeCfg(long long charge_value)
{
	for (int index = m_single_charge_cfg.cfg_count - 1; index >= 0; index --)
	{
		CSASingleChargeCfg::ConfigItem *cfg_item = &m_single_charge_cfg.cfg_list[index];
		if (charge_value >= cfg_item->charge_value)
		{
			return cfg_item;
		}
	}

	return NULL;
}

const CSALoginGiftCfg::ConfigItem * CombineServerActivityConfig::GetLoginGiftRewardCfg(int seq)
{
	if (seq >= 0 && seq < m_login_gift_cfg.cfg_count)
	{
		return &m_login_gift_cfg.cfg_list[seq];
	}

	return NULL;
}


const CSAServerPanicBuyCfg::ConfigItem * CombineServerActivityConfig::GetServerPanicBuyItemCfg(int item_idx)
{
	if (item_idx >= 0 && item_idx < m_server_panic_buy_cfg.cfg_count)
	{
		return &m_server_panic_buy_cfg.cfg_list[item_idx];
	}

	return NULL;
}

const CSAPersonalPanicBuyCfg::ConfigItem * CombineServerActivityConfig::GetPersonalPanicBuyItemCfg(int item_idx)
{
	if (item_idx >= 0 && item_idx <= m_personal_panic_buy_cfg.cfg_count)
	{
		return &m_personal_panic_buy_cfg.cfg_list[item_idx];
	}

	return NULL;
}

const CSATouzijihuaCfg * CombineServerActivityConfig::GetTouzijihuaCfg(int phase)
{
	if (phase < 0 || phase >= static_cast<int>(m_touzijihua_reward_cfg_vec.size()))
	{
		return nullptr;
	}

	return &m_touzijihua_reward_cfg_vec[phase];
}

const CSAFoundationConfig * CombineServerActivityConfig::GetFoundationCfg(int chongzhi_num)
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

int CombineServerActivityConfig::GetFoundationRewardGold(int reward_index, int reward_times, int delta_level)
{
	if (reward_index < 0 || reward_index >= static_cast<int>(m_foundation_cfg_vec.size()))
	{
		return 0;
	}

	const CSAFoundationConfig &cfg = m_foundation_cfg_vec[reward_index];

	if (reward_times < 0 || reward_times >= static_cast<int>(cfg.reward_cfg_vec.size()))
	{
		return 0;
	}

	if (delta_level < cfg.reward_cfg_vec[reward_times].need_up_level)
	{
		return 0;
	}

	return cfg.reward_cfg_vec[reward_times].reward_gold;
}

int CombineServerActivityConfig::GetFoundationRewardTimesCount(int reward_index)
{
	if (reward_index < 0 || reward_index >= static_cast<int>(m_foundation_cfg_vec.size()))
	{
		return 0;
	}

	const CSAFoundationConfig &cfg = m_foundation_cfg_vec[reward_index];

	return static_cast<int>(cfg.reward_cfg_vec.size());
}

int CombineServerActivityConfig::GetExpRefineCost(int role_level, int delta_level)
{
	if (delta_level <= 0)
	{
		return 0;
	}

	auto iter = m_exp_refine_cfg_vec.cbegin();
	for (; iter != m_exp_refine_cfg_vec.cend(); ++iter)
	{
		if (role_level < iter->level)
		{
			break;
		}
	}

	int total_cost = 0;
	const int to_level = role_level + delta_level;

	for (; iter != m_exp_refine_cfg_vec.cend(); ++iter)
	{
		// 意味着没有跨档，计算后跳出
		if (to_level <= iter->level)
		{
			total_cost += delta_level * iter->cost;
			break;
		}
		else
		{
			// 跨档了，先计算出当前档位应该消耗的金额
			total_cost += (iter->level - role_level) * iter->cost;

			// 再修正等级
			role_level = iter->level;
			delta_level = to_level - role_level;
		}
	}

	return total_cost;
}

const CSAKillBossCfg * CombineServerActivityConfig::GetKillBossCfg(int boss_id)
{
	auto it = m_kill_boss_cfg.find(boss_id);
	if (it != m_kill_boss_cfg.end())
	{
		return &it->second;
	}

	return nullptr;
}

int CombineServerActivityConfig::InitSubActivityTimeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int sub_type = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "sub_type", sub_type) || sub_type <= CSA_SUB_TYPE_INVALID || sub_type >= CSA_SUB_TYPE_MAX)
		{
			return -1;
		}

		CSATimeConfig &config = m_time_config_list[sub_type];
		config.sub_type = sub_type;

		int is_open = 0;
		if (!PugiGetSubNodeValue(data_element, "is_open", is_open))
		{
			return -3;
		}
		config.is_open = (0 != is_open);

		if (!PugiGetSubNodeValue(data_element, "begin_day", config.begin_day) || (config.is_open && config.begin_day <= 0))
		{
			return -4;
		}
		if (!PugiGetSubNodeValue(data_element, "end_day", config.end_day) || (config.is_open && config.end_day < config.begin_day))
		{
			return -5;
		}
		if (!PugiGetSubNodeValue(data_element, "end_time", config.end_time) || (config.is_open && (config.end_time < 0 || config.end_time > 2400)))
		{
			return -6;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}


int CombineServerActivityConfig::InitRankRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int sub_type = 0;
		if (!PugiGetSubNodeValue(data_element, "sub_type", sub_type) || 
			(!CombineServerActivityConfig::IsRankSubActivityType(sub_type) && CSA_SUB_TYPE_BOSS != sub_type))	//策划需求，合服boss排行榜需要显示10个玩家信息
		{
			return -1;
		}

		if (m_rank_reward_list[sub_type].IsValid())
		{
			return -2;
		}

		CSARankRewardConfig &cfg_item = m_rank_reward_list[sub_type];
		cfg_item.sub_type = sub_type;

		if (!PugiGetSubNodeValue(data_element, "rank_limit", cfg_item.rank_limit))
		{
			return -3;
		}

		UNSTD_STATIC_CHECK(3 == COMBINE_SERVER_ACTIVITY_RANK_REWARD_ROLE_NUM);

		{
			PugiXmlNode reward_item = data_element.child("reward_item_1");
			if (NULL == reward_item)
			{
				return -100;
			}

			if (!cfg_item.rank_reward_items[0].ReadConfig(reward_item))
			{
				return -101;
			}
		}

		{
			PugiXmlNode reward_item = data_element.child("reward_item_2");
			if (NULL == reward_item)
			{
				return -110;
			}

			if (!cfg_item.rank_reward_items[1].ReadConfig(reward_item))
			{
				return -111;
			}
		}

		{
			PugiXmlNode reward_item = data_element.child("reward_item_3");
			if (NULL == reward_item)
			{
				return -120;
			}

			if (!cfg_item.rank_reward_items[2].ReadConfig(reward_item))
			{
				return -121;
			}
		}

		data_element = data_element.next_sibling();
	}

	for (int i = CSA_SUB_TYPE_INVALID + 1; i < CSA_SUB_TYPE_MAX; ++ i)
	{
		if (CombineServerActivityConfig::IsRankSubActivityType(i) && NULL == this->GetCSARankRewardCfgBySubType(i))
		{
			return -20000;
		}
	}

	return 0;
}

int CombineServerActivityConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "roll_cost", m_roll_cfg.roll_cost) || m_roll_cfg.roll_cost <= 0)
	{
		return -1;
	}

	{
		PugiXmlNode item_list_element = data_element.child("gcz_chengzhu_reward_list");
		if (item_list_element.empty())
		{
			return -101;
		}

		PugiXmlNode gcz_winner_item_element = item_list_element.child("gcz_chengzhu_reward");
		if (gcz_winner_item_element.empty())
		{
			return -102;
		}

		m_other_cfg.gcz_winner_item_count = 0;
		while (!gcz_winner_item_element.empty())
		{
			if (m_other_cfg.gcz_winner_item_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				break;
			}
			if (!m_other_cfg.gcz_winner_item_list[m_other_cfg.gcz_winner_item_count].ReadConfig(gcz_winner_item_element))
			{
				return -103;
			}
			++m_other_cfg.gcz_winner_item_count;
			gcz_winner_item_element = gcz_winner_item_element.next_sibling();
		}
	}

	{

		PugiXmlNode item_list_element = data_element.child("gcz_camp_reward_list");
		if (item_list_element.empty())
		{
			return -111;
		}

		PugiXmlNode gcz_camp_item_element = item_list_element.child("gcz_camp_reward");
		if (gcz_camp_item_element.empty())
		{
			return -112;
		}

		m_other_cfg.gcz_camp_item_count = 0;
		while (!gcz_camp_item_element.empty())
		{
			if (m_other_cfg.gcz_camp_item_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				break;
			}
			if (!m_other_cfg.gcz_camp_item_list[m_other_cfg.gcz_camp_item_count].ReadConfig(gcz_camp_item_element))
			{
				return -113;
			}
			++m_other_cfg.gcz_camp_item_count;
			gcz_camp_item_element = gcz_camp_item_element.next_sibling();
		}
	}

	{
		PugiXmlNode item_list_element = data_element.child("xmz_mengzhu_reward_list");
		if (item_list_element.empty())
		{
			return -121;
		}

		PugiXmlNode xmz_winner_item_element = item_list_element.child("xmz_mengzhu_reward");
		if (xmz_winner_item_element.empty())
		{
			return -122;
		}

		m_other_cfg.xmz_winner_item_count = 0;
		while (!xmz_winner_item_element.empty())
		{
			if (m_other_cfg.xmz_winner_item_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				break;
			}
			if (!m_other_cfg.xmz_winner_item_list[m_other_cfg.xmz_winner_item_count].ReadConfig(xmz_winner_item_element))
			{
				return -123;
			}
			++m_other_cfg.xmz_winner_item_count;
			xmz_winner_item_element = xmz_winner_item_element.next_sibling();
		}
	}

	{
		PugiXmlNode item_list_element = data_element.child("xmz_camp_reward_list");
		if (item_list_element.empty())
		{
			return -131;
		}

		PugiXmlNode xmz_guild_item_element = item_list_element.child("xmz_camp_reward");
		if (xmz_guild_item_element.empty())
		{
			return -132;
		}

		m_other_cfg.xmz_guild_item_count = 0;
		while (!xmz_guild_item_element.empty())
		{
			if (m_other_cfg.xmz_guild_item_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				break;
			}
			if (!m_other_cfg.xmz_guild_item_list[m_other_cfg.xmz_guild_item_count].ReadConfig(xmz_guild_item_element))
			{
				return -133;
			}
			++m_other_cfg.xmz_guild_item_count;
			xmz_guild_item_element = xmz_guild_item_element.next_sibling();
		}
	}

	{
		PugiXmlNode kill_boss_item_element = data_element.child("kill_boss_reward");
		if (kill_boss_item_element.empty())
		{
			return -142;
		}

		if (!m_other_cfg.kill_boss_reward_item.ReadConfig(kill_boss_item_element))
		{
			return -143;
		}

		PugiXmlNode boss_rank_master_reward_item_element = data_element.child("boss_rank_master_reward_item");
		if (boss_rank_master_reward_item_element.empty())
		{
			return -145;
		}

		if (!m_other_cfg.boss_rank_master_reward_item.ReadConfig(boss_rank_master_reward_item_element))
		{
			return -146;
		}

		PugiXmlNode boss_rank_member_reward_item_element = data_element.child("boss_rank_member_reward_item");
		if (boss_rank_member_reward_item_element.empty())
		{
			return -147;
		}

		if (!m_other_cfg.boss_rank_member_reward_item.ReadConfig(boss_rank_member_reward_item_element))
		{
			return -148;
		}

		if (!PugiGetSubNodeValue(data_element, "kill_boss_reward_cost", m_other_cfg.kill_boss_reward_cost) || m_other_cfg.kill_boss_reward_cost <= 0)
		{
			return -144;
		}
	}

	{
		PugiXmlNode login_accumute_item_element = data_element.child("login_accumulate_reward");
		if (login_accumute_item_element.empty())
		{
			return -152;
		}

		if (!m_login_gift_cfg.accumulate_reward.ReadConfig(login_accumute_item_element))
		{
			return -153;
		}

		if (!PugiGetSubNodeValue(data_element, "need_accumulate_days", m_login_gift_cfg.need_accumulate_days) || m_login_gift_cfg.need_accumulate_days <= 0)
		{
			return -154;
		}
	}


	{
		if (!PugiGetSubNodeValue(data_element, "touzi_jihua_buy_cost", m_other_cfg.touzi_jihua_buy_cost) || m_other_cfg.touzi_jihua_buy_cost <= 0)
		{
			return -155;
		}

		if (!PugiGetSubNodeValue(data_element, "touzi_jihua_buy_reward_gold", m_other_cfg.touzi_jihua_buy_reward_gold) || m_other_cfg.touzi_jihua_buy_reward_gold < 0)
		{
			return -156;
		}

		if (!PugiGetSubNodeValue(data_element, "buy_exp_grade", m_other_cfg.exp_refine_grades) || m_other_cfg.exp_refine_grades <= 0)
		{
			return -157;
		}

		if (!PugiGetSubNodeValue(data_element, "buy_exp_level_limit", m_other_cfg.exp_refine_level_limit) || m_other_cfg.exp_refine_level_limit < 0)
		{
			return -158;
		}

		if (!PugiGetSubNodeValue(data_element, "buy_exp_max_level", m_other_cfg.exp_refine_max_level) || m_other_cfg.exp_refine_max_level <= 0)
		{
			return -159;
		}
	}

	return 0;
}

int CombineServerActivityConfig::InitRankQianggouCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int qianggou_cfg_count = 0;

	while (!data_element.empty())
	{
		if (qianggou_cfg_count >= COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE)
		{
			return -1;
		}

		CSAQianggouItemConfig &qg_cfg = m_rank_qianggou_itemlist[qianggou_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "limit_num", qg_cfg.limit_num) || qg_cfg.limit_num <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "cost", qg_cfg.cost) || qg_cfg.cost <= 0)
		{
			return -3;
		}

		PugiXmlNode reward_item_element = data_element.child("stuff_item");
		if (reward_item_element.empty())
		{
			return -4;
		}

		if (!qg_cfg.stuff_item.ReadConfig(reward_item_element))
		{
			return -5;
		}

		++ qianggou_cfg_count;
		data_element = data_element.next_sibling();
	}

	if (COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE != qianggou_cfg_count)
	{
		return -20000;
	}

	return 0;
}

int CombineServerActivityConfig::InitRollCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (m_roll_cfg.cfg_count >= COMBINE_SERVER_ROLL_CFG_MAX_COUNT)
		{
			return -1;
		}

		CSARollConfigItem &cfg_item = m_roll_cfg.cfg_list[m_roll_cfg.cfg_count];

 		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_roll_cfg.cfg_count)
 		{
 			return -2;
 		}

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast))
		{
			return -3;
		}
		cfg_item.is_broadcast = (0 != is_broadcast);

		if (!PugiGetSubNodeValue(data_element, "weight", cfg_item.weight) || cfg_item.weight <= 0)
		{
			return -3;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -4;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item_element))
		{
			return -5;
		}

		m_roll_cfg.total_weight += cfg_item.weight;
		++ m_roll_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int CombineServerActivityConfig::InitSingleChargeCfg(PugiXmlNode RootElement)
{

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1;
	}

	int last_charge_value = 0;

	while (!data_element.empty())
	{
		if (m_single_charge_cfg.cfg_count >= COMBINE_SERVER_SINGLE_CHARGE_CONFIG_MAX_COUNT)
		{
			return -2;
		}

		CSASingleChargeCfg::ConfigItem *cfg_item = &m_single_charge_cfg.cfg_list[m_single_charge_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item->seq) || cfg_item->seq != m_single_charge_cfg.cfg_count)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "charge_value", cfg_item->charge_value) || cfg_item->charge_value <= last_charge_value)
		{
			return -4;
		}
		last_charge_value = cfg_item->charge_value;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (NULL == reward_item)
		{
			return -5;
		}
		if (!cfg_item->reward_item.ReadConfig(reward_item))
		{
			return -6;
		}

		++ m_single_charge_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}


int CombineServerActivityConfig::InitLoginGiftRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_login_gift_cfg.cfg_count = 0;

	int last_need_login_days = 0;

	while (!data_element.empty())
	{
		if (m_login_gift_cfg.cfg_count >= COMBINE_SERVER_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT)
		{
			return -1;
		}

		CSALoginGiftCfg::ConfigItem &cfg_item = m_login_gift_cfg.cfg_list[m_login_gift_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "need_login_days", cfg_item.need_login_days) || cfg_item.need_login_days <= last_need_login_days)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_login_gift_cfg.cfg_count)
		{
			return -3;
		}
		last_need_login_days = cfg_item.need_login_days;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (NULL == reward_item)
		{
			return -4;
		}

		if (!cfg_item.common_reward_item.ReadConfig(reward_item))
		{
			return -5;
		}

		PugiXmlNode vip_reward_item = data_element.child("vip_reward_item");
		if (NULL == vip_reward_item)
		{
			return -6;
		}

		if (!cfg_item.vip_reward_item.ReadConfig(vip_reward_item))
		{
			return -7;
		}

		++ m_login_gift_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}


int CombineServerActivityConfig::InitServerPanicBuyCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_server_panic_buy_cfg.cfg_count = 0;

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_server_panic_buy_cfg.cfg_count || seq >= COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT)
		{
			return -1;
		}

		CSAServerPanicBuyCfg::ConfigItem &cfg_item = m_server_panic_buy_cfg.cfg_list[m_server_panic_buy_cfg.cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "gold_price", cfg_item.gold_price) || cfg_item.gold_price <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "server_limit_buy_count", cfg_item.server_limit_buy_count) || cfg_item.server_limit_buy_count <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "personal_limit_buy_count", cfg_item.personal_limit_buy_count) || cfg_item.personal_limit_buy_count <= 0)
		{
			return -4;
		}

		PugiXmlNode rewardItemElement = data_element.child("reward_item");
		if (rewardItemElement.empty())
		{
			return -5;
		}

		if (!cfg_item.reward_item.ReadConfig(rewardItemElement))
		{
			return -6;
		}

		++ m_server_panic_buy_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int CombineServerActivityConfig::InitPersonalPanicBuyCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_personal_panic_buy_cfg.cfg_count = 0;

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_personal_panic_buy_cfg.cfg_count || seq >= COMBINE_SERVER_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT)
		{
			return -1;
		}

		CSAPersonalPanicBuyCfg::ConfigItem &cfg_item = m_personal_panic_buy_cfg.cfg_list[m_personal_panic_buy_cfg.cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "gold_price", cfg_item.gold_price) || cfg_item.gold_price <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_buy_count", cfg_item.limit_buy_count) || cfg_item.limit_buy_count <= 0)
		{
			return -3;
		}

		PugiXmlNode rewardItemElement = data_element.child("reward_item");
		if (rewardItemElement.empty())
		{
			return -4;
		}

		if (!cfg_item.reward_item.ReadConfig(rewardItemElement))
		{
			return -5;
		}

		++ m_personal_panic_buy_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int CombineServerActivityConfig::InitTouzijihuaCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		CSATouzijihuaCfg cfg;
		int login_day = 0;
		if (!PugiGetSubNodeValue(data_element, "login_day", login_day) || login_day <= 0 || login_day != static_cast<int>(m_touzijihua_reward_cfg_vec.size()) + 1)
		{
			return -1;
		}
		cfg.login_day = login_day;

		if (!PugiGetSubNodeValue(data_element, "reward_gold", cfg.reward_gold) || cfg.reward_gold < 0)
		{
			return -2;
		}

		m_touzijihua_reward_cfg_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CombineServerActivityConfig::InitFoundationCfg(PugiXmlNode RootElement)
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

		CSAFoundationConfig *cfg = nullptr;
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
			CSAFoundationConfig new_cfg;
			new_cfg.seq = seq;
			new_cfg.chongzhi_gold = chongzhi_gold;
			m_foundation_cfg_vec.push_back(new_cfg);
			if (static_cast<int>(m_foundation_cfg_vec.size()) > COMBINE_SERVER_MAX_FOUNDATION_TYPE)
			{
				return -101;
			}

			cfg = &(*m_foundation_cfg_vec.rbegin());
		}

		if (nullptr == cfg)
		{
			return -100;
		}

		CSAFoundationConfig::CSAFoundationRewardCfg reward_cfg;
		if (!PugiGetSubNodeValue(data_element, "need_up_level", reward_cfg.need_up_level) || reward_cfg.need_up_level <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_gold", reward_cfg.reward_gold) || reward_cfg.reward_gold < 0)
		{
			return -7;
		}

		cfg->reward_cfg_vec.push_back(reward_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CombineServerActivityConfig::InitExpRefineCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		CSAExpRefineRewardCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "level", cfg.level) || cfg.level <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "value", cfg.cost) || cfg.cost <= 0)
		{
			return -2;
		}

		if (!m_exp_refine_cfg_vec.empty() && m_exp_refine_cfg_vec.back().level >= cfg.level)
		{
			return -3;
		}

		m_exp_refine_cfg_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int CombineServerActivityConfig::InitKillBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		CSAKillBossCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "boss_id", cfg.boss_id) || cfg.boss_id <= 0 || !MONSTERPOOL->IsMonsterExist(cfg.boss_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "delta_lv_limit", cfg.delta_lv_limit) || cfg.delta_lv_limit <= 0)
		{
			return -2;
		}

		if (m_kill_boss_cfg.find(cfg.boss_id) != m_kill_boss_cfg.end())
		{
			return -3;
		}

		m_kill_boss_cfg[cfg.boss_id] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}
