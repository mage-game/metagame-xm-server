#include "husongconfig.hpp"

#include "servercommon/configcommon.h"
#include "task/taskpool.h"
#include "item/itempool.h"
#include "checkresourcecenter.hpp"

HuSongConfig::HuSongConfig()
	: m_flush_itemid(0), m_free_refresh_times(0), m_realive_scene_id(0), 
	m_realive_pos(0, 0), m_pretask_id(0), m_max_rob_count(0), m_rob_no_reward_level_limit(0), m_shield_continue_times(0),m_dec_hurt_per(0), m_dec_speed_percent(0), m_task_rewardcfg_count(0), m_buytimes_cfgcount(0), m_exp_factor(0), m_rob_exp_percent(0),
	m_be_rob_exp_percent(0)
{
	memset(m_refresh_tocolor_maxtimes, 0, sizeof(m_refresh_tocolor_maxtimes));
}

HuSongConfig::~HuSongConfig()
{

}

bool HuSongConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "HuSongConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo, "Load XianBang config [%s] Error.cannot find root node.", path.c_str());
		*err = errinfo;
		return false;
	}

	{
		// 读取 杂项配置 other
		PugiXmlNode otherElement = RootElement.child("other");
		if (otherElement.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->other]", path.c_str());
			*err = errinfo;
			return false;
		}

		PugiXmlNode dataElement = otherElement.child("data");
		if (dataElement.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data]", path.c_str());
			*err =  errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "flush_itemid", m_flush_itemid) || NULL == ITEMPOOL->GetItem(m_flush_itemid))
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->flush_itemid: %d ]", path.c_str(), m_flush_itemid);
			*err = errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "free_refresh_times", m_free_refresh_times) || m_free_refresh_times < 0)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->free_refresh_times: %d ]", path.c_str(), m_flush_itemid);
			*err = errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "realive_scene_id", m_realive_scene_id) || m_realive_scene_id <= 0)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->realive_scene_id: %d ]", path.c_str(), m_realive_scene_id);
			*err = errinfo;
			return false;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(m_realive_scene_id);

		if (!PugiGetSubNodeValue(dataElement, "realive_pos_x", m_realive_pos.x) || m_realive_pos.x <= 0)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->realive_pos_x: %d ]", path.c_str(), m_realive_pos.x);
			*err = errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "realive_pos_y", m_realive_pos.y) || m_realive_pos.y <= 0)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->realive_pos_y: %d ]", path.c_str(), m_realive_pos.y);
			*err = errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "pretask_id", m_pretask_id) || m_pretask_id <= 0)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->pretask_id: %d ]", path.c_str(), m_pretask_id);
			*err = errinfo;
			return false;
		}

		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(m_pretask_id);
		if (NULL == task_cfg)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->pretask_id not exist]", path.c_str());
			*err = errinfo;
			return false;
		}
		if (TASK_TYPE_TRUNK != task_cfg->task_type)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->pretask_id is not trunk task]", path.c_str());
			*err = errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_rob_count", m_max_rob_count) || m_max_rob_count <= 0)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->max_rob_count: %d ]", path.c_str(), m_max_rob_count);
			*err = errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "rob_no_reward_level_limit", m_rob_no_reward_level_limit) || m_rob_no_reward_level_limit <= 0 || m_rob_no_reward_level_limit >= MAX_ROLE_LEVEL)
		{
			sprintf(errinfo, "%s:xml node error in node [config->other->data->rob_no_reward_level_limit: %d ]", path.c_str(), m_rob_no_reward_level_limit);
			*err = errinfo;
			return false;
		}


		if (!PugiGetSubNodeValue(dataElement, "shield_continue_times", m_shield_continue_times) || m_shield_continue_times < 0)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->shield_continue_times: %d ]", path.c_str(), m_shield_continue_times);
			*err = errinfo;
			return false;
		}


		if (!PugiGetSubNodeValue(dataElement, "dec_hurt_per", m_dec_hurt_per) || m_dec_hurt_per < 0)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->dec_hurt_per: %d ]", path.c_str(), m_dec_hurt_per);
			*err = errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "dec_speed_percent", m_dec_speed_percent) || m_dec_speed_percent <= 0)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->dec_speed_percent: %d ]", path.c_str(), m_dec_speed_percent);
			*err = errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "exp_factor", m_exp_factor) || m_exp_factor < 0)
		{
			sprintf(errinfo, "%s:xml node error in node [config->other->data->exp_factor: %f ]", path.c_str(), m_exp_factor);
			*err = errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "rob_percent", m_rob_exp_percent) || m_rob_exp_percent < 0 || m_rob_exp_percent > 100)
		{
			sprintf(errinfo, "%s:xml node error in node [config->other->data->rob_percent: %d ]", path.c_str(), m_rob_exp_percent);
			*err = errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "be_rob_percent", m_be_rob_exp_percent) || m_be_rob_exp_percent < 0 || m_be_rob_exp_percent > 100)
		{
			sprintf(errinfo, "%s:xml node error in node [config->other->data->be_rob_percent: %d ]", path.c_str(), m_be_rob_exp_percent);
			*err = errinfo;
			return false;
		}
	}

	{
		// 读取任务颜色 刷新概率 task_color_flush_prob
		PugiXmlNode taskColorFlushProbElement = RootElement.child("task_color_flush_prob");
		if (taskColorFlushProbElement.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_color_flush_prob]", path.c_str());
			*err = errinfo;
			return false;
		}

		PugiXmlNode dataElement = taskColorFlushProbElement.child("data");
		if (dataElement.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_color_flush_prob->data]", path.c_str());
			*err =  errinfo;
			return false;
		}

		int last_color = TASK_COLOR_INVALID;
		while (!dataElement.empty())
		{
			int color = 0;
			if (!PugiGetSubNodeValue(dataElement, "task_color", color) || color <= TASK_COLOR_INVALID || color >= TASK_COLOR_MAX || last_color + 1 != color)
			{
				sprintf(errinfo,"%s:xml node error in node [config->task_color_flush_prob->data->task_color: %d ]", path.c_str(), color);
				*err = errinfo;
				return false;
			}
			last_color = color;

			int *color_problist = m_taskcolor_problist[color].color_problist;

			if (!PugiGetSubNodeValue(dataElement, "white", color_problist[TASK_COLOR_WHITE]) || color_problist[TASK_COLOR_WHITE] < 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->task_color_flush_prob->data->white: %d ]", path.c_str(), color_problist[TASK_COLOR_WHITE]);
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(dataElement, "green", color_problist[TASK_COLOR_GREEN]) || color_problist[TASK_COLOR_GREEN] < 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->task_color_flush_prob->data->green: %d ]", path.c_str(), color_problist[TASK_COLOR_GREEN]);
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(dataElement, "blue", color_problist[TASK_COLOR_BLUE]) || color_problist[TASK_COLOR_BLUE] < 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->task_color_flush_prob->data->blue: %d ]", path.c_str(), color_problist[TASK_COLOR_BLUE]);
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(dataElement, "purple", color_problist[TASK_COLOR_PURPLE]) || color_problist[TASK_COLOR_PURPLE] < 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->task_color_flush_prob->data->purple: %d ]", path.c_str(), color_problist[TASK_COLOR_PURPLE]);
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(dataElement, "orange", color_problist[TASK_COLOR_ORANGE]) || color_problist[TASK_COLOR_ORANGE] < 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->task_color_flush_prob->data->orange: %d ]", path.c_str(), color_problist[TASK_COLOR_ORANGE]);
				*err = errinfo;
				return false;
			}

			dataElement = dataElement.next_sibling();
		}

		for (int i = TASK_COLOR_WHITE; i <= TASK_COLOR_ORANGE; ++i)
		{
			if (!m_taskcolor_problist[i].CheckCfgValid())
			{
				sprintf(errinfo,"%s:xml the config of config->task_color_flush_prob->data->task_color[%d] is invalid ", path.c_str(), i);
				*err = errinfo;
				return false;
			}
		}
	}

	{
		// 读取自动刷到指定颜色的 保底次数
		PugiXmlNode refreshToColorMaxTimes = RootElement.child("refresh_tocolor_maxtimes");
		if (NULL == refreshToColorMaxTimes)
		{
			sprintf(errinfo,"%s:xml node error in node [config->refresh_tocolor_maxtimes]", path.c_str());
			*err = errinfo;
			return false;
		}

		PugiXmlNode dataElement = refreshToColorMaxTimes.child("data");
		if (dataElement.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->refresh_tocolor_maxtimes->data]", path.c_str());
			*err =  errinfo;
			return false;
		}

		int last_color = TASK_COLOR_INVALID;
		while (!dataElement.empty())
		{
			int color = 0;
			if (!PugiGetSubNodeValue(dataElement, "to_color", color) || color <= TASK_COLOR_INVALID || color >= TASK_COLOR_MAX || last_color + 1 != color)
			{
				sprintf(errinfo,"%s:xml node error in node [config->refresh_tocolor_maxtimes->data->task_color: %d ]", path.c_str(), color);
				*err = errinfo;
				return false;
			}
			last_color = color;

			if (!PugiGetSubNodeValue(dataElement, "max_flushtimes", m_refresh_tocolor_maxtimes[color]) || m_refresh_tocolor_maxtimes[color] <= 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->refresh_tocolor_maxtimes->data->task_color: %d ]", path.c_str(), color);
				*err = errinfo;
				return false;
			}

			dataElement = dataElement.next_sibling();
		}

		for (int i = TASK_COLOR_WHITE; i < TASK_COLOR_MAX; ++i)
		{
			if (m_refresh_tocolor_maxtimes[i] <= 0)
			{
				sprintf(errinfo,"%s:xml m_refresh_tocolor_maxtimes[%d] = %d", path.c_str(), i, m_refresh_tocolor_maxtimes[i]);
				*err = errinfo;
				return false;
			}
		}
	}

	{	
		// 读取任务奖励配置
		PugiXmlNode taskRewardListElement = RootElement.child("task_reward_list");
		if (taskRewardListElement.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_reward_list]", path.c_str());
			*err = errinfo;
			return false;
		}

		PugiXmlNode dataElement = taskRewardListElement.child("data");
		if (dataElement.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_reward_list->data]", path.c_str());
			*err =  errinfo;
			return false;
		}

		m_task_rewardcfg_count = 0; int last_max_level = 0;
		while (!dataElement.empty())
		{
			if (m_task_rewardcfg_count >= HUSONG_LEVEL_REWARD_CFG_MAX_COUNT) 
			{
				sprintf(errinfo,"%s:xml m_task_rewardcfg_count[%d] is invalid", path.c_str(), m_task_rewardcfg_count);
				*err = errinfo;
				return false;
			}

			TaskRewardCfg &task_rewrd_cfg = m_task_rewardcfg_list[m_task_rewardcfg_count];
			++ m_task_rewardcfg_count;

			if (!PugiGetSubNodeValue(dataElement, "min_limit_level", task_rewrd_cfg.min_limitlevel) || task_rewrd_cfg.min_limitlevel != last_max_level + 1)
			{
				sprintf(errinfo,"%s:xml node error in node [config->task_reward_list->data->min_limit_level: %d ]", path.c_str(), task_rewrd_cfg.min_limitlevel);
				*err = errinfo;
				return false;
			}
			if (!PugiGetSubNodeValue(dataElement, "max_limit_level", task_rewrd_cfg.max_limitlevel) || task_rewrd_cfg.max_limitlevel < task_rewrd_cfg.min_limitlevel)
			{
				sprintf(errinfo,"%s:xml node error in node [config->task_reward_list->data->max_limit_level: %d ]", path.c_str(), task_rewrd_cfg.max_limitlevel);
				*err = errinfo;
				return false;
			}
			last_max_level = task_rewrd_cfg.max_limitlevel;

			if (!PugiGetSubNodeValue(dataElement, "commit_exp", task_rewrd_cfg.commit_exp) || task_rewrd_cfg.commit_exp <= 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->task_reward_list->data->commit_exp: %d ]", path.c_str(), task_rewrd_cfg.commit_exp);
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(dataElement, "commit_bind_coin", task_rewrd_cfg.commit_bind_coin) || task_rewrd_cfg.commit_bind_coin < 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->task_reward_list->data->commit_bind_coin: %d ]", path.c_str(), task_rewrd_cfg.commit_bind_coin);
				*err = errinfo;
				return false;
			}

			{
				task_rewrd_cfg.item_count = ItemConfigData::ReadConfigList(dataElement, std::string("reward_item"), task_rewrd_cfg.reward_item, MAX_ATTACHMENT_ITEM_NUM);
				if (task_rewrd_cfg.item_count < 0)
				{
					sprintf(errinfo, "%s:xml node error in node [config->task_reward_list->data->reward_item: %d ]", path.c_str(), task_rewrd_cfg.item_count);
					*err = errinfo;
					return false;
				}
			}

			dataElement = dataElement.next_sibling();
		}

// 		if (last_max_level < MAX_ROLE_LEVEL)
// 		{
// 			sprintf(errinfo,"%s:xml node error in node [config->task_reward_list->data] last max_limit_level[%d] can not less than MAX_ROLE_LEVEL", path.c_str(), last_max_level);
// 			*err = errinfo;
// 			return false;
// 		}
	}

	{
		// 任务颜色与奖励系数
		PugiXmlNode taskRewardFactorList = RootElement.child("task_reward_factor_list");
		if (NULL == taskRewardFactorList)
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_reward_factor_list]", path.c_str());
			*err = errinfo;
			return false;
		}

		PugiXmlNode dataElement = taskRewardFactorList.child("data");
		if (dataElement.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_reward_factor_list->data]", path.c_str());
			*err =  errinfo;
			return false;
		}

		int last_color = TASK_COLOR_INVALID;
		while (!dataElement.empty())
		{
			int color = 0;
			if (!PugiGetSubNodeValue(dataElement, "task_color", color) || color <= TASK_COLOR_INVALID || color >= TASK_COLOR_MAX || last_color + 1 != color)
			{
				sprintf(errinfo,"%s:xml node error in node [config->task_reward_factor_list->data->task_color: %d ]", path.c_str(), color);
				*err = errinfo;
				return false;
			}
			last_color = color;

			if (!PugiGetSubNodeValue(dataElement, "factor", m_task_reward_factor_list[color]) || m_task_reward_factor_list[color] <= 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->task_reward_factor_list->data->white: %d ]", path.c_str(), m_task_reward_factor_list[color]);
				*err = errinfo;
				return false;
			}

			dataElement = dataElement.next_sibling();
		}

		if (TASK_COLOR_MAX - 1 != last_color)
		{
			sprintf(errinfo,"%s:xml node error in node [config->task_reward_factor_list] last_color[%d] is not equal TASK_COLOR_MAX - 1", path.c_str(), last_color);
			*err = errinfo;
			return false;
		}
	}

	{
		// 购买次数配置
		PugiXmlNode buyTimesCfgElement = RootElement.child("buy_times_cfg");
		if (buyTimesCfgElement.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->buy_times_cfg]", path.c_str());
			*err = errinfo;
			return false;
		}

		PugiXmlNode dataElement = buyTimesCfgElement.child("data");
		if (dataElement.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->buy_times_cfg->data]", path.c_str());
			*err =  errinfo;
			return false;
		}

		int last_buy_times = 0;

		while (!dataElement.empty())
		{
			if (m_buytimes_cfgcount >= HUSONG_BUY_TIMES_CFG_MAX_COUNT)
			{
				sprintf(errinfo,"%s:xml node error in node [config->buy_times_cfg->data] too many config", path.c_str());
				*err =  errinfo;
				return false;
			}

			BuyConfigItem &cfg_item = m_buytimes_cfglist[m_buytimes_cfgcount];

			if (!PugiGetSubNodeValue(dataElement, "buy_times", cfg_item.buy_times) || cfg_item.buy_times <= last_buy_times)
			{
				sprintf(errinfo,"%s:xml node error in node [config->buy_times_cfg->data->buy_times]", path.c_str());
				*err = errinfo;
				return false;
			}
			last_buy_times = cfg_item.buy_times;

			if (!PugiGetSubNodeValue(dataElement, "gold_cost", cfg_item.need_gold) || cfg_item.need_gold <= 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->buy_times_cfg->data->gold_cost]", path.c_str());
				*err = errinfo;
				return false;
			}

			++ m_buytimes_cfgcount;
			dataElement = dataElement.next_sibling();
		}
	}

	return true;
}

const HuSongConfig::TaskColorFlushProbItem * HuSongConfig::GetTaskColorFlushProb(int task_color)
{
	if (task_color <= TASK_COLOR_INVALID || task_color >= TASK_COLOR_MAX) return NULL;

	return &m_taskcolor_problist[task_color];
}

int HuSongConfig::GetRefreshToColorMaxTimes(int color)
{
	if (color <= TASK_COLOR_INVALID || color >= TASK_COLOR_MAX) return -1;

	return m_refresh_tocolor_maxtimes[color];
}

int HuSongConfig::GetRealivePos(Posi *pos)
{
	if (NULL != pos) *pos = m_realive_pos;
	return m_realive_scene_id;
}

const HuSongConfig::TaskRewardCfg * HuSongConfig::GetTaskRewardCfg(int level)
{
	for (int i = 0; i < m_task_rewardcfg_count && i < HUSONG_LEVEL_REWARD_CFG_MAX_COUNT; i ++)
	{
		if (m_task_rewardcfg_list[i].InLimitLevel(level))
		{
			return &m_task_rewardcfg_list[i];
		}
	}

	return NULL;
}

int HuSongConfig::GetTaskRewardFactor(int task_color)
{
	if (task_color <= TASK_COLOR_INVALID || task_color >= TASK_COLOR_MAX) return 0;
	return m_task_reward_factor_list[task_color];
}

int HuSongConfig::GetBuyTimesCost(int times)
{
	for (int i = m_buytimes_cfgcount - 1; i >= 0; -- i)
	{
		if (times >= m_buytimes_cfglist[i].buy_times)
		{
			return m_buytimes_cfglist->need_gold;
		}
	}

	return 0;
}

double HuSongConfig::GetExpFactorFind()
{
	double color_factor = (double)this->GetTaskRewardFactor(2) / 100;
	return color_factor*m_exp_factor;
}
