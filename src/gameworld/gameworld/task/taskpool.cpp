#include "taskpool.h"
#include "globalconfig/globalconfig.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "engineadapter.h"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "obj/character/role.h"
#include "item/itempool.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TaskPool::TaskPool() : m_daily_level_interval_count(0), m_paohuan_level_interval_count(0), m_task_star_max_level(0), m_task_star_total_weight(0)
{
}

TaskPool::~TaskPool()
{
	
}

TaskPool * gs_task_pool_config = NULL;

TaskPool * TaskPool::Instance()
{
	if (NULL == gs_task_pool_config)
	{
		gs_task_pool_config = new TaskPool();
	}

	return gs_task_pool_config;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TaskPool::Init(const std::string &dir, const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};

	TiXmlDocument m_document;

	if (configname == "" || !m_document.LoadFile(dir+configname))
	{
		SNPRINTF(errinfo, sizeof(errinfo), "%s: Load Config Error, %s.", (dir+configname).c_str(), m_document.ErrorDesc());
		*err = errinfo;
		return false;
	}

	TiXmlElement *RootElement = m_document.RootElement();
	if (NULL == RootElement)
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		TiXmlElement *currentElement = RootElement->FirstChildElement("task_list");
		if (NULL == currentElement)
		{
			*err = configname + ": no [task_list].";
			return false;
		}

		int iRet = InitTaskCfg(currentElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTaskCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *otherElement = RootElement->FirstChildElement("other");
		if (NULL == otherElement)
		{
			*err = configname + ": no [other].";
			return false;
		}

		int iRet = InitOther(otherElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOther failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *dailyRewardElement = RootElement->FirstChildElement("daily_task_reward");
		if (NULL == dailyRewardElement)
		{
			*err = configname + ": no [daily_task_reward].";
			return false;
		}

		int iRet = InitDailyTaskReward(dailyRewardElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDailyTaskReward failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *guildRewardElement = RootElement->FirstChildElement("guild_task_reward");
		if (NULL == guildRewardElement)
		{
			*err = configname + ": no [guild_task_reward].";
			return false;
		}

		int iRet = InitGuildTaskReward(guildRewardElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGuildTaskReward failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *GuildTaskListElement = RootElement->FirstChildElement("guild_task_list");
		if (NULL == GuildTaskListElement)
		{
			*err = configname + ": no [guild_task_list].";
			return false;
		}

		int iRet = InitGuildTaskList(GuildTaskListElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGuildTaskList failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *TaskStarListElement = RootElement->FirstChildElement("task_star");
		if (NULL == TaskStarListElement)
		{
			*err = configname + ": no [task_star].";
			return false;
		}

		int iRet = InitTaskStar(TaskStarListElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitTaskStar failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *PaoHuanTaskRewardElement = RootElement->FirstChildElement("paohuan_task_reward");
		if (NULL == PaoHuanTaskRewardElement)
		{
			*err = configname + ": no [paohuan_task_list].";
			return false;
		}

		int iRet = InitPaohuanTaskRewardCfg(PaoHuanTaskRewardElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPaohuanTaskRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *PaoHuanRewardFixElement = RootElement->FirstChildElement("paohuan_reward_fix");
		if (NULL == PaoHuanRewardFixElement)
		{
			*err = configname + ": no [paohuan_reward_fix].";
			return false;
		}

		int iRet = InitPaohuanTaskLvFixCfg(PaoHuanRewardFixElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPaohuanTaskLvFixCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *PaoHuanRollElement = RootElement->FirstChildElement("paohuan_rolllist");
		if (NULL == PaoHuanRollElement)
		{
			*err = configname + ": no [paohuan_rolllist].";
			return false;
		}

		int iRet = InitPaoHuanRollCfg(PaoHuanRollElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPaoHuanRollCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	
	{
		TiXmlElement *task_random_element = RootElement->FirstChildElement("task_roll");
		if (NULL == task_random_element)
		{
			*err = configname + ": no [task_random_element].";
			return false;
		}

		int iRet = InitTaskRandomCfg(task_random_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: task_random_element failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		TiXmlElement *ZhuanzhiTaskListElement = RootElement->FirstChildElement("zhaunzhi_task_list");
		if (NULL == ZhuanzhiTaskListElement)
		{
			*err = configname + ": no [zhuanzhi_task_list].";
			return false;
		}

		int iRet = InitZhuanzhiTaskList(ZhuanzhiTaskListElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitZhuanzhiTaskList failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	for (std::set<int>::iterator iter = m_task_chk_set.begin(); iter != m_task_chk_set.end(); ++iter)
	{
		int task_id = *iter;

		const TaskCfg *cfg = this->GetCfgByTaskID(task_id);
		if (NULL == cfg)
		{
			sprintf(errinfo,"%s: chk task failed of task %d", configname.c_str(), task_id);
			*err = errinfo;
			return false;
		}
	}
	m_task_chk_set.clear();

	return true;
}

bool TaskPool::Reload(const std::string &dir, const std::string &filename, std::string *err)
{
	TaskPool *temp = new TaskPool();

	if (!temp->Init(dir, filename, err))
	{
		delete temp; temp = NULL;
		return false;
	}

	if (NULL != gs_task_pool_config) delete gs_task_pool_config;

	gs_task_pool_config = temp;

	return true;
}

int TaskPool::InitTaskCfg(TiXmlElement *RootElement)
{
	m_daily_level_interval_count = 0;
	m_paohuan_level_interval_count = 0;

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		TaskCfg cfg;

		if (!GetSubNodeValue(dataElement, "task_id", cfg.task_id) || cfg.task_id <= 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "task_type", cfg.task_type) || cfg.task_type <= TASK_TYPE_INVALID || cfg.task_type >= TASK_COUNT)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "ver", cfg.ver) || cfg.ver < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "min_level", cfg.min_level) || cfg.min_level <= 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "max_level", cfg.max_level) || cfg.max_level < cfg.min_level)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "camp", cfg.camp) || cfg.camp < 0 || cfg.camp >= CAMP_TYPE_MAX)
		{
			return -7;
		}

		{
			// 前置任务
			if (!GetSubNodeValue(dataElement, "pretaskid", cfg.pretaskid))
			{
				return -6;
			}

			// 日常任务不可以设置前置任务，它的前置任务在other里边设置
			if (TASK_TYPE_DAILY == cfg.task_type && INVALID_TASK_ID != cfg.pretaskid)
			{
				return -7;
			}

			if (INVALID_TASK_ID != cfg.pretaskid)
			{
				m_task_chk_set.insert(cfg.pretaskid);
			}
		}
	
		{
			// 接受任务npc
			bool has_accept_npc = GetSubNodeValue(dataElement, "accept_npc", cfg.accept_npc);
			if (!has_accept_npc)
			{
				if (TASK_TYPE_HUSONG == cfg.task_type)
				{
					return -20001;
				}
			}

			// 提交任务npc
			bool has_commit_npc = GetSubNodeValue(dataElement, "commit_npc", cfg.commit_npc);
			if (!has_commit_npc)
			{
				if (TASK_TYPE_TRUNK == cfg.task_type || TASK_TYPE_HUSONG == cfg.task_type)
				{
					return -8;
				}
			}

			GetSubNodeValue(dataElement, "auto_commit_time", cfg.auto_commit_time);
			GetSubNodeValue(dataElement, "auto_commit_openday", cfg.auto_commit_openday);
		}

		{
			if (!GetSubNodeValue(dataElement, "condition", cfg.condition) || cfg.condition <= TASK_CON_INVALID || cfg.condition >= TASK_CON_MAX)
			{
				return -9;
			}

			if (!GetSubNodeValue(dataElement, "c_param1", cfg.c_param1))
			{
				return -10;
			}

			if (!GetSubNodeValue(dataElement, "c_param2", cfg.c_param2))
			{
				return -11;
			}

			if (!GetSubNodeValue(dataElement, "c_param3", cfg.c_param3))
			{
				return -12;
			}

			switch (cfg.condition)
			{
			case TASK_CON_TALK:
				{
					if (0 == cfg.c_param1)
					{
						return -10020;
					}
				}
				break;

			case TASK_CON_KILL_MONSTER:
				{
					if (!MONSTERPOOL->IsMonsterExist(cfg.c_param1))
					{
						return -10000;
					}

					if (cfg.c_param2 <= 0)
					{
						return -10001;
					}
				}
				break;

			case TASK_CON_KILL_BOSS_TYPE:
				{
					if (cfg.c_param1 < MonsterInitParam::BOSS_TYPE_TASK_INVALID_TYPE || cfg.c_param1 >= MonsterInitParam::BOSS_TYPE_TASK_MAX_TYPE)
					{
						return -10010;
					}

					if (cfg.c_param2 <= 0)
					{
						return -10011;
					}
				}
				break;

			case TASK_CON_GATHER:
				{
					if (0 == cfg.c_param1)
					{
						return -10020;
					}

					if (cfg.c_param2 <= 0)
					{
						return -10021;
					}
				}
				break;

			case TASK_CON_PASS_FB:
				{

				}
				break;

			case TASK_CON_ENTER_SCENE:
				{
					if (cfg.c_param1 <= 0)
					{
						return -10030;
					}

					CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.c_param1);
				}
				break;

			case TASK_CON_COMMIT_TASK:
				{
					if (cfg.c_param1 <= TASK_TYPE_INVALID || cfg.c_param1 >= TASK_COUNT)
					{
						return -10041;
					}
				}
				break;

			case TASK_CON_PASS_FB_LAYER:
				{

				}
				break;

			case TASK_CON_PASS_SPECIAL_TASK:
				{
					if(cfg.c_param1 != 1)
					{
						return -10050;
					}
					
				}
				break;

			case TASK_CON_BIANSHEN_TALK:
			{
				if (cfg.c_param1 <= Role::CHANGE_MODE_TASK_TYPE_INVALID || cfg.c_param1 >= Role::CHANGE_MODE_APPEARN_MAX)
				{
					return -10060;
				}
			}
			break;

			case TASK_CON_COMMIT_ITEM:
			{
				if (cfg.c_param1 <= 0 || cfg.c_param2 <= 0)
				{
					return -10070;
				}

				if (NULL == ITEMPOOL->GetItem(cfg.c_param1))
				{
					return -10071;
				}
			}
			break;

			}
		}

		{
			// 接受任务时对角色的行为
			bool has_accept_op = GetSubNodeValue(dataElement, "accept_op", cfg.accept_op);
			if (has_accept_op && cfg.accept_op > TASK_ACCEPT_OP_INVALID && cfg.accept_op < TASK_ACCEPT_OP_MAX)
			{
				if (!GetSubNodeValue(dataElement, "a_param1", cfg.a_param1))
				{
					return -21;
				}

				if (!GetSubNodeValue(dataElement, "a_param2", cfg.a_param2))
				{
					return -22;
				}

				if (!GetSubNodeValue(dataElement, "a_param3", cfg.a_param3))
				{
					return -23;
				}

				switch (cfg.accept_op)
				{	
				case TASK_ACCEPT_OP_MOVE_NORMAL_ON_REMOVE:
					break;

				case TASK_ACCEPT_OP_FLY:
				case TASK_ACCEPT_OP_FLY_ON_ACCEPT:
					{
						if (cfg.a_param1 <= Role::MOVE_MODE_FLY_PARAM_INVALID || cfg.a_param1 >= Role::MOVE_MODE_FLY_PARAM_MAX)
						{
							return -10100;
						}
					}
					break;

				case TASK_ACCEPT_OP_MOVE:
					{
						if (cfg.a_param1 <= 0)
						{
							return -10200;
						}

						if (cfg.a_param2 <= 0)
						{
							return -10201;
						}

						if (cfg.a_param3 <= 0)
						{
							return -10202;
						}

						CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.a_param1);
					}
					break;

				case TASK_ACCEPT_OP_ADD_SKILL_ON_ACCEPT:
					{
						if (cfg.a_param1 < 0 || cfg.a_param1 >= SKILL_COUNT)
						{
							return -10300;
						}
					}
					break;

				case TASK_ACCEPT_OP_ADD_SKILL_ON_REMOVE:
					{
						if (cfg.a_param1 < 0 || cfg.a_param1 >= SKILL_COUNT)
						{
							return -10400;
						}
					}
					break;

				case TASK_ACCEPT_OP_ENTER_GUIDE_FB:
				case TASK_ACCEPT_OP_OPEN_GUIDE_VIEW:
				case TASK_ACCEPT_OP_ENTER_DAILY_FB:
				case TASK_ACCEPT_OP_CLIENT_PARAM:
					{
						
					}
					break;

				default:
					{
						return -20000;
					}
					break;
				}
			}
		}

		{
			if (!GetSubNodeValue(dataElement, "coin_bind", cfg.coin_bind) || cfg.coin_bind < 0)
			{
				return -30;
			}

			if (!GetSubNodeValue(dataElement, "exp", cfg.exp) || cfg.exp < 0)
			{
				return -31;
			}

			if (!GetSubNodeValue(dataElement, "nv_wa_shi", cfg.nv_wa_shi) || cfg.nv_wa_shi < 0)
			{
				return -32;
			}

			if (!GetSubNodeValue(dataElement, "yuanli", cfg.yuanli) || cfg.yuanli < 0)
			{
				return -33;
			}
		}

		{
			// 通用物品奖励
			TiXmlElement *normal_item_list_element = dataElement->FirstChildElement("item_list_list");
			if (NULL != normal_item_list_element)
			{
				TiXmlElement *reward_item_element = normal_item_list_element->FirstChildElement("item_list");
				if (NULL != reward_item_element)
				{
					int item_count = 0;
					while (NULL != reward_item_element)
					{
						if (item_count >= TaskCfg::MAX_REWARD_ITEM_COUNT)
						{
							return -41;
						}

						if (!cfg.normal_item_list[item_count].ReadConfig(reward_item_element))
						{
							return -42;
						}

						++ item_count;
						reward_item_element = reward_item_element->NextSiblingElement();
					}
					cfg.normal_item_count = item_count;
				}
			}			
		}

		{
			// 职业1 物品奖励
			TiXmlElement *prof_item_list_element_1 = dataElement->FirstChildElement("prof_list1_list");
			if (NULL != prof_item_list_element_1)
			{
				TiXmlElement *reward_item_element = prof_item_list_element_1->FirstChildElement("prof_list1");
				if (NULL != reward_item_element)
				{
					int item_count = 0;
					while (NULL != reward_item_element)
					{
						if (item_count >= TaskCfg::MAX_REWARD_ITEM_COUNT)
						{
							return -51;
						}

						if (!cfg.prof_item_list_1[item_count].ReadConfig(reward_item_element))
						{
							return -52;
						}

						++ item_count;
						reward_item_element = reward_item_element->NextSiblingElement();
					}
					cfg.prof_item_count_1 = item_count;
				}
			}
		}

		{
			// 职业2 物品奖励
			TiXmlElement *prof_item_list_element_2 = dataElement->FirstChildElement("prof_list2_list");
			if (NULL != prof_item_list_element_2)
			{
				TiXmlElement *reward_item_element = prof_item_list_element_2->FirstChildElement("prof_list2");
				if (NULL != reward_item_element)
				{
					int item_count = 0;
					while (NULL != reward_item_element)
					{
						if (item_count >= TaskCfg::MAX_REWARD_ITEM_COUNT)
						{
							return -61;
						}

						if (!cfg.prof_item_list_2[item_count].ReadConfig(reward_item_element))
						{
							return -62;
						}

						++ item_count;
						reward_item_element = reward_item_element->NextSiblingElement();
					}
					cfg.prof_item_count_2 = item_count;
				}
			}	
		}

		{
			// 职业3 物品奖励
			TiXmlElement *prof_item_list_element_3 = dataElement->FirstChildElement("prof_list3_list");
			if (NULL != prof_item_list_element_3)
			{
				TiXmlElement *reward_item_element = prof_item_list_element_3->FirstChildElement("prof_list3");
				if (NULL != reward_item_element)
				{
					int item_count = 0;
					while (NULL != reward_item_element)
					{
						if (item_count >= TaskCfg::MAX_REWARD_ITEM_COUNT)
						{
							return -71;
						}

						if (!cfg.prof_item_list_3[item_count].ReadConfig(reward_item_element))
						{
							return -72;
						}

						++ item_count;
						reward_item_element = reward_item_element->NextSiblingElement();
					}
					cfg.prof_item_count_3 = item_count;
				}
			}
		}

		{
			// 职业4 物品奖励
			TiXmlElement *prof_item_list_element_4 = dataElement->FirstChildElement("prof_list4_list");
			if (NULL != prof_item_list_element_4)
			{
				TiXmlElement *reward_item_element = prof_item_list_element_4->FirstChildElement("prof_list4");
				if (NULL != reward_item_element)
				{
					int item_count = 0;
					while (NULL != reward_item_element)
					{
						if (item_count >= TaskCfg::MAX_REWARD_ITEM_COUNT)
						{
							return -81;
						}

						if (!cfg.prof_item_list_4[item_count].ReadConfig(reward_item_element))
						{
							return -82;
						}

						++ item_count;
						reward_item_element = reward_item_element->NextSiblingElement();
					}
					cfg.prof_item_count_4 = item_count;
				}
			}
		}

		if (TASK_TYPE_GUILD == cfg.task_type)
		{
			this->PutGuildTask(cfg.task_id);

			if (cfg.pretaskid > 0)
			{
				if (cfg.task_id <= cfg.pretaskid)
				{
					return -2001;
				}

				this->PutNextTask(cfg.pretaskid, cfg.task_id);
			}
		}
		else if (TASK_TYPE_ZHUANZHI == cfg.task_type)
		{
			this->PutZhuanzhiTask(cfg.task_id);

			if (cfg.pretaskid > 0)
			{
				if (cfg.task_id <= cfg.pretaskid)
				{
					return -20010;
				}

				this->PutNextTask(cfg.pretaskid, cfg.task_id);
			}
		}
		else if (TASK_TYPE_DAILY == cfg.task_type)
		{
			int ret = this->PutDailyTask(&cfg);
			if (0 != ret)
			{
				return -10000 + ret;
			}

			if (0 == m_daily_other_cfg.example_task_id)
			{
				m_daily_other_cfg.example_task_id = cfg.task_id;
			}
		}
		else if (TASK_TYPE_PAOHUAN == cfg.task_type)
		{
			int ret = this->PutPaohuanTask(&cfg);
			if (0 != ret)
			{
				return -20000 + ret;
			}
		}
		else if (TASK_TYPE_TRUNK == cfg.task_type || TASK_TYPE_BRANCH == cfg.task_type)
		{
			if (TASK_TYPE_TRUNK == cfg.task_type)
			{
				this->PutLevelTask(cfg.task_id, cfg.min_level);
			}
			else
			{
				for (int level = cfg.min_level; level <= cfg.max_level; ++ level)
				{
					this->PutLevelTask(cfg.task_id, level);
				}
			}

			if (cfg.pretaskid > 0)
			{
				if (cfg.task_id <= cfg.pretaskid)
				{
					return -2000;
				}

				this->PutNextTask(cfg.pretaskid, cfg.task_id);
			}
		}

		if (m_task_map.end() != m_task_map.find(cfg.task_id))
		{
			return -3000;
		}

		m_task_map[cfg.task_id] = cfg;

		dataElement = dataElement->NextSiblingElement();
	}

	for (int i = 1; i <= MAX_ROLE_LEVEL; ++ i)
	{
		if ((int)m_level_task_list[i].size() > MAX_NEXT_TASK_COUNT)
		{
			return -11001;
		}
	}

	// 检查日常
	for (int i = 0; i < m_daily_level_interval_count; ++i) // 严格保证所有等级段必须有任务 
	{
		if (m_daily_level_taskinfo[i].task_list.empty())
		{
			return -11000;
		}
	}

	// 检查跑环
	for (int i = 0; i < m_paohuan_level_interval_count; ++i) // 严格保证所有等级段必须有任务 
	{
		if (m_paohuan_level_taskinfo[i].task_list.empty())
		{
			return -11002;
		}
	}

	return 0;
}

int TaskPool::InitOther(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL != dataElement)
	{
		if (!GetSubNodeValue(dataElement, "daily_pretask", m_daily_other_cfg.daily_pretask) || m_daily_other_cfg.daily_pretask <= 0)
		{
			return -1;
		}

		const TaskCfg *pre_task_cfg = this->GetCfgByTaskID(m_daily_other_cfg.daily_pretask);
		if (NULL == pre_task_cfg)
		{
			return -1000;
		}
		if (TASK_TYPE_TRUNK != pre_task_cfg->task_type)
		{
			return -1001;
		}

		if (!GetSubNodeValue(dataElement, "fix_first_daily_task", m_daily_other_cfg.fix_first_daily_task) || m_daily_other_cfg.fix_first_daily_task <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "fix_daily_task_level", m_daily_other_cfg.fix_daily_task_level) || m_daily_other_cfg.fix_daily_task_level < 0)
		{
			return -1003;
		}

		const TaskCfg *trial_task_cfg = this->GetCfgByTaskID(m_daily_other_cfg.fix_first_daily_task);
		if (NULL == trial_task_cfg)
		{
			return -2000;
		}
		if (TASK_TYPE_DAILY != trial_task_cfg->task_type)
		{
			return -2001;
		}

		if (!GetSubNodeValue(dataElement, "daily_double_gold", m_daily_other_cfg.double_gold) || m_daily_other_cfg.double_gold <= 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "free_double_vip_level", m_daily_other_cfg.free_double_vip_level) || m_daily_other_cfg.free_double_vip_level <= 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "daily_treble_gold", m_daily_other_cfg.daily_treble_gold) || m_daily_other_cfg.daily_treble_gold <= 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "exp_ball_limit", m_daily_other_cfg.exp_ball_limit) || m_daily_other_cfg.exp_ball_limit < 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "valid_time_h", m_daily_other_cfg.valid_time_h) || m_daily_other_cfg.valid_time_h <= 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "daily_double_maxstar_gold",  m_daily_other_cfg.double_maxstar_gold) || m_daily_other_cfg.double_maxstar_gold <= 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "max_star_cost_coin", m_daily_other_cfg.max_star_cost_coin) || m_daily_other_cfg.max_star_cost_coin <= 0)
		{
			return -33;
		}
	
		if (!GetSubNodeValue(dataElement, "guild_task_gold", m_guild_other_cfg.guild_task_gold) || m_guild_other_cfg.guild_task_gold <= 0)
		{
			return -10;
		}

		if (!GetSubNodeValue(dataElement, "guild_task_special_count_limit_level", m_guild_other_cfg.guild_task_special_count_limit_level) || m_guild_other_cfg.guild_task_special_count_limit_level < 0)
		{
			return -11;
		}

		if (!GetSubNodeValue(dataElement, "one_task_need_gold", m_paohuan_other_cfg.one_task_need_gold) || m_paohuan_other_cfg.one_task_need_gold <= 0)
		{
			return -12;
		}

		if (!GetSubNodeValue(dataElement, "one_key_commit_task_count", m_paohuan_other_cfg.one_key_commit_task_count) || m_paohuan_other_cfg.one_key_commit_task_count <= 0 || m_paohuan_other_cfg.one_key_commit_task_count > PAOHUAN_ONEDAY_COMMIT_MAX)
		{
			return -13;
		}

		TiXmlElement *guild_task_complete_all_reward_element = dataElement->FirstChildElement("guild_task_complete_all_reward_item");
		if (NULL == guild_task_complete_all_reward_element)
		{
			return -101;
		}

		if (!GetSubNodeValue(dataElement, "guild_double_reward_cost", m_guild_other_cfg.guild_double_reward_cost) || m_guild_other_cfg.guild_double_reward_cost <= 0)
		{
			return -14;
		}

		if (!m_guild_other_cfg.guild_task_complete_all_reward_item.ReadConfig(guild_task_complete_all_reward_element))
		{
			return -15;
		}

		if (!GetSubNodeValue(dataElement, "skip_paohuan_task_limit_level", m_paohuan_other_cfg.skip_paohuan_task_limit_level) || m_paohuan_other_cfg.skip_paohuan_task_limit_level <= 0)
		{
			return -16;
		}

		if (!GetSubNodeValue(dataElement, "task_interva_jackpot", this->m_task_roll_finish_time) || this->m_task_roll_finish_time <= 0)
		{
			return -17;
		}

		if (!GetSubNodeValue(dataElement, "exp_factor_dailytask", m_daily_other_cfg.exp_factor_dailytask) || m_daily_other_cfg.exp_factor_dailytask < 0)
		{
			return -18;
		}

		if (!GetSubNodeValue(dataElement, "exp_factor_guildtask", m_daily_other_cfg.exp_factor_guildtask) || m_daily_other_cfg.exp_factor_guildtask < 0)
		{
			return -19;
		}

		if (!GetSubNodeValue(dataElement, "exp_factor_huantask", m_daily_other_cfg.exp_factor_huantask) || m_daily_other_cfg.exp_factor_huantask < 0)
		{
			return -20;
		}
	}

	return 0;
}

int TaskPool::InitDailyTaskReward(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -1000000;
	}

	int last_max_level = 0;
	while (NULL != dataElement)
	{
		int min_level = 0;
		if (!GetSubNodeValue(dataElement, "level", min_level) || min_level > MAX_ROLE_LEVEL || min_level - last_max_level != 1)
		{
			return -1;
		}

		int max_level =0;
		if (!GetSubNodeValue(dataElement, "level_max", max_level) || max_level > MAX_ROLE_LEVEL)
		{
			return -101;
		}
		last_max_level = max_level;

		DailyTaskReward task_reward;
		task_reward.min_level = min_level;
		task_reward.max_level = max_level;

		if (!GetSubNodeValue(dataElement, "exp", task_reward.exp) || task_reward.exp < 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "bind_coin", task_reward.bind_coin) || task_reward.bind_coin < 0)
		{
			return -22;
		}

		if (!GetSubNodeValue(dataElement, "complete_all_daily_reward_coin", task_reward.complete_all_daily_reward_coin) || task_reward.complete_all_daily_reward_coin < 0)
		{
			return -3;
		}

		TiXmlElement *reward_item_list_element = dataElement->FirstChildElement("reward_item_list");
		if (NULL == reward_item_list_element)
		{
			return -4;
		}

		TiXmlElement *reward_item_element = reward_item_list_element->FirstChildElement("reward_item");
		if (NULL == reward_item_element)
		{
			return -5;
		}

		task_reward.complete_all_daily_reward_itemcount = 0;

		while (NULL != reward_item_element)
		{
			if (task_reward.complete_all_daily_reward_itemcount >= COMPLETE_ALL_DAILY_REWARD_ITEM_MAX_COUNT)
			{
				return -6;
			}

			if (!task_reward.complete_all_daily_reward_itemlist[task_reward.complete_all_daily_reward_itemcount].ReadConfig(reward_item_element))
			{
				return -7;
			}

			++ task_reward.complete_all_daily_reward_itemcount;
			reward_item_element = reward_item_element->NextSiblingElement();
		}

		m_daily_task_reward_list.push_back(task_reward);
		dataElement = dataElement->NextSiblingElement();
	}

	//先屏掉，后补回
// 	if (MAX_ROLE_LEVEL != last_level)
// 	{
// 		return -2000000;
// 	}

	return 0;
}

int TaskPool::InitGuildTaskReward(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -100000;
	}

	int last_level = 0;

	while (NULL != dataElement)
	{
		int level = 0;
		if (!GetSubNodeValue(dataElement, "level", level) || level != last_level + 1 || level > MAX_ROLE_LEVEL)
		{
			return -1;
		}

		last_level = level;

		GuildTaskRewardCfg &task_reward = m_guild_task_reward_list[level];
		task_reward.level = level;

		if (!GetSubNodeValue(dataElement, "exp", task_reward.exp) || task_reward.exp <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "gongxian", task_reward.gongxian) || task_reward.gongxian <= 0)
		{
			return -3;
		}

		{
			task_reward.item_count = ItemConfigData::ReadConfigList(dataElement, std::string("reward_item"),
				task_reward.item_list, GuildTaskRewardCfg::GUILD_TASK_REWARD_ITEM_MAX);
			if (task_reward.item_count < 0)
			{
				return -200 + task_reward.item_count;
			}
		}

		dataElement = dataElement->NextSiblingElement();
	}

// 	if (MAX_ROLE_LEVEL != last_level)
// 	{
// 		return -20000;
// 	}

	return 0;
}

int TaskPool::InitGuildTaskList(TiXmlElement *RootElement)
{
	m_guild_task_list.cfg_count = 0;
	std::set<TaskID> first_task_id_check;

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while(NULL != dataElement)
	{
		if (m_guild_task_list.cfg_count < 0 || m_guild_task_list.cfg_count >= GuildTaskConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1000;
		}
		
		GuildTaskConfig::ConfigItem &item_cfg = m_guild_task_list.cfg_list[m_guild_task_list.cfg_count];

		if (!GetSubNodeValue(dataElement, "first_task", item_cfg.first_task) || item_cfg.first_task <= 0)
		{
			return -1;
		}
		
		if (first_task_id_check.end() != first_task_id_check.find(item_cfg.first_task))
		{
			return -2;
		}
		first_task_id_check.insert(item_cfg.first_task);

		if (!GetSubNodeValue(dataElement, "end_task", item_cfg.end_task) || item_cfg.end_task <= 0)
		{
			return -3;
		}

		{
			// 检查任务合法性
			const TaskCfg *first_task_cfg_p = this->GetCfgByTaskID(item_cfg.first_task);
			if (NULL == first_task_cfg_p || TASK_TYPE_GUILD != first_task_cfg_p->task_type)
			{
				return -10000001;
			}

			const TaskCfg *pre_guild_task_p = this->GetCfgByTaskID(first_task_cfg_p->pretaskid);
			if (NULL == pre_guild_task_p || TASK_TYPE_TRUNK != pre_guild_task_p->task_type)
			{
				return -10000002;
			}

			int check_count = 0; TaskID temp_task = item_cfg.end_task; int series_count = 0;

			while(item_cfg.first_task != temp_task && check_count < 100)
			{
				++ check_count;

				const TaskCfg *task_cfg = this->GetCfgByTaskID(temp_task);
				if (NULL == task_cfg || TASK_TYPE_GUILD != task_cfg->task_type)
				{
					return -10000003;
				}

				temp_task = task_cfg->pretaskid;

				++ series_count;
			}

			if (item_cfg.first_task != temp_task)
			{
				return -10000004;
			}

			if (GuildTaskConfig::GUILD_TASK_SERIES_LIMIT_COUNT != series_count + 1)
			{
				return - 10000005;
			}
 		}

		{
			// 把首个任务加入到等级 任务映射表里边，用于计算可接任务
			const TaskCfg *first_task_cfg_p = this->GetCfgByTaskID(item_cfg.first_task);
			if (NULL != first_task_cfg_p)
			{
				for (int level = first_task_cfg_p->min_level; level <= first_task_cfg_p->max_level; ++ level)
				{
					this->PutLevelTask(first_task_cfg_p->task_id, level);
				}
			}
		}

		++ m_guild_task_list.cfg_count;

		dataElement = dataElement->NextSiblingElement();
	}
	{
		for (std::map<int, TaskCfg>::const_iterator it = m_task_map.begin(); it != m_task_map.end(); ++it)
		{
			if (TASK_TYPE_GUILD == it->second.task_type)
			{
				if (this->IsGuildFirstTaskID(it->second.task_id))
				{
					const TaskCfg *pre_task_p = this->GetCfgByTaskID(it->second.pretaskid);
					if (NULL == pre_task_p)
					{
						return -10000005;
					}

					if (TASK_TYPE_TRUNK != pre_task_p->task_type)
					{
						return -10000006;
					}
				}
				else
				{
// 					if (MAX_ROLE_LEVEL != it->second.max_level)
// 					{
// 						return -10000007;
// 					}
				}
			}
		}

		int last_level = 0;
		for (int i = 0; i < m_guild_task_list.cfg_count && i < GuildTaskConfig::CONFIG_ITEM_MAX_COUNT; ++ i)
		{
			const TaskCfg *first_task_p = this->GetCfgByTaskID(m_guild_task_list.cfg_list[i].first_task);
			if (NULL == first_task_p)
			{
				return -10000008;
			}

			if (i > 0)
			{
				if (first_task_p->min_level != last_level + 1)
				{
					return -10000009;
				}
			}

			last_level = first_task_p->max_level;
		}
	}

	return 0;
}

int TaskPool::InitTaskStar(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	int last_level = 0;
	m_task_star_max_level = 0;
	m_task_star_total_weight = 0;

	while(NULL != dataElement)
	{
		int level = 0;
		if (!GetSubNodeValue(dataElement, "level", level) || level <= 0 || level - last_level != 1)
		{
			return -1;
		}
		last_level = level;

		TaskStarCfg &item = m_task_star_map[level];
		item.star_level = level;

		if (!GetSubNodeValue(dataElement, "weight", item.weight) || item.weight <= 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "reward_rate", item.reward_rate) || item.reward_rate <= 0)
		{
			return -3;
		}

		m_task_star_max_level = level;
		m_task_star_total_weight += item.weight;

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

int TaskPool::InitPaohuanTaskRewardCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	
	while (NULL != data_element)
	{
		int paohuan_reward_cfg_idx = 0;
		if (!GetSubNodeValue(data_element, "huan",  paohuan_reward_cfg_idx) || paohuan_reward_cfg_idx < 0 || paohuan_reward_cfg_idx >= PAOHUAN_ONEDAY_COMMIT_MAX)
		{
			return -1000;
		}

		int &reward_nvwashi = m_paohuan_reward_cfg.nvwashi_list[paohuan_reward_cfg_idx];
		if (!GetSubNodeValue(data_element, "reward_nvwashi",  reward_nvwashi) || reward_nvwashi <= 0)
		{
			return -1;
		}

		long long &exp = m_paohuan_reward_cfg.exp_list[paohuan_reward_cfg_idx];
		if (!GetSubNodeValue(data_element, "exp", exp) || exp < 0)
		{
			return -2;
		}

		TiXmlElement *item_element = data_element->FirstChildElement("reward_item");
		if (NULL != item_element)
		{
			if (!m_paohuan_reward_cfg.reward_item[paohuan_reward_cfg_idx].ReadConfig(item_element))
			{
				return -3;
			}
		}

		++m_paohuan_reward_cfg.paohuan_task_count;

		data_element = data_element->NextSiblingElement();
	}
	return 0;
}

int TaskPool::InitPaohuanTaskLvFixCfg(TiXmlElement *RootElement)
{
	int paohuan_lvfix_cfg_idx = 0;
	int last_min_level = 0, last_max_level = 0;

	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	while (NULL != data_element)
	{
		if (paohuan_lvfix_cfg_idx < 0 || paohuan_lvfix_cfg_idx >= PAOHUAN_LV_FIX_COUNT)
		{
			return 1000;
		}

		PaohuanTaskLvFixCfg *lv_fix_cfg = &m_paohuan_lvfix_cfg[paohuan_lvfix_cfg_idx];

		if (!GetSubNodeValue(data_element, "min_level", lv_fix_cfg->min_level) || lv_fix_cfg->min_level <= 0 || lv_fix_cfg->min_level <= last_min_level)
		{
			return -1;
		}
		if (!GetSubNodeValue(data_element, "max_level",  lv_fix_cfg->max_level) || lv_fix_cfg->max_level <= 0 || lv_fix_cfg->max_level <= last_max_level)
		{
			return -2;
		}
		if (lv_fix_cfg->min_level > lv_fix_cfg->max_level)
		{
			return -10;
		}

		last_min_level = lv_fix_cfg->min_level;
		last_max_level = lv_fix_cfg->max_level;

		if (!GetSubNodeValue(data_element, "nvwashi_addpercent",  lv_fix_cfg->nvwashi_addpercent) || lv_fix_cfg->nvwashi_addpercent < 100)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "exp_addpercent", lv_fix_cfg->exp_addpercent) || lv_fix_cfg->exp_addpercent < 100)
		{
			return -4;
		}

		++ paohuan_lvfix_cfg_idx;

		data_element = data_element->NextSiblingElement();
	}
	return 0;
}

int TaskPool::InitPaoHuanRollCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");

	int count_in_pool = 0;
	int last_rolllistid = 0;

	while (NULL != data_element)
	{
		PaoHuanRollItem cfg;

		TiXmlElement *Itemelement = data_element->FirstChildElement("item");
		if (NULL != Itemelement)
		{
			if (!cfg.item_cfg.ReadConfig(Itemelement))
			{
				return -1;
			}
		}

		if (!GetSubNodeValue(data_element, "rolllist_id", cfg.rolllist_id) || cfg.rolllist_id < 0)
		{
			return -20;
		}

		if (cfg.rolllist_id != last_rolllistid)
		{
			if (cfg.rolllist_id - last_rolllistid != 1)
			{
				return -200;
			}

			if (last_rolllistid != 0 && count_in_pool != PAOHUAN_ROLLPOOL_TOTAL_COUNT)
			{
				return -201;
			}

			last_rolllistid = cfg.rolllist_id;
			count_in_pool = 0;
		}

		if (!GetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight <= 0)
		{
			return -30;
		}

		if (count_in_pool >= PAOHUAN_ROLLPOOL_TOTAL_COUNT)
		{
			return -300;
		}

		PaoHuanRollPool &roll_list = m_paohuan_roll_map[cfg.rolllist_id];
		roll_list.item_list[count_in_pool] = cfg;

		++ count_in_pool;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

bool TaskPool::IsInGuildTaskList(TaskID first_task, TaskID task_id)
{
	if (task_id <= 0 || first_task <= 0)
	{
		return false;
	}

	if (!this->IsGuildFirstTaskID(first_task))
	{
		return false;
	}

	TaskID temp_task_id = this->GetEndTaskByFirstTask(first_task);
	if (0 == temp_task_id)
	{
		return false;
	}

	int check_count = 0;
	while (task_id != temp_task_id && check_count < 100)
	{
		++check_count;

		const TaskCfg *temp_task_cfg_p = this->GetCfgByTaskID(temp_task_id);
		if (NULL == temp_task_cfg_p || TASK_TYPE_GUILD != temp_task_cfg_p->task_type)
		{
			return false;
		}

		temp_task_id = temp_task_cfg_p->pretaskid;
	}

	if (temp_task_id != task_id)
	{
		return false;
	}

	return true;
}

const TaskCfg * TaskPool::GetCfgByTaskID(int task_id) const
{
	std::map<int, TaskCfg>::const_iterator iter = m_task_map.find(task_id);
	if (iter == m_task_map.end())
	{
		return NULL;
	}

	return &iter->second;
}

void TaskPool::GetNextTask(int task_id, short &task_count, int task_id_list[MAX_NEXT_TASK_COUNT]) const
{
	task_count = 0;

	std::map<int, TaskList>::const_iterator iter = m_next_map.find(task_id);
	if (iter == m_next_map.end())
	{
		return;
	}

	const TaskList &task_list = iter->second;

	for (int i = 0; i < MAX_NEXT_TASK_COUNT && i < static_cast<int>(task_list.size()) && task_count < MAX_NEXT_TASK_COUNT; ++i)
	{
		task_id_list[task_count++] = task_list[i];
	}
}

void TaskPool::GetTaskAtLevel(int role_level, short &task_count, int task_id_list[MAX_NEXT_TASK_COUNT]) const
{
	task_count = 0;

	if (role_level <= 0 || role_level > MAX_ROLE_LEVEL)
	{
		return;
	}

	const TaskList &task_list = m_level_task_list[role_level];
	int total_count = (int)task_list.size();

	for (int i = 0; i < total_count && task_count < MAX_NEXT_TASK_COUNT; ++i)
	{
		task_id_list[task_count++] = task_list[i];
	}
}

void TaskPool::PutNextTask(int pretaskid, int nexttaskid)
{
	TaskList &task_list = m_next_map[pretaskid];

	int total_count = (int)task_list.size();

	for (int i = 0; i < total_count; ++i)
	{
		if (task_list[i] == nexttaskid)
		{
			return;
		}
	}
	task_list.push_back(nexttaskid);
}

void TaskPool::PutLevelTask(int taskid, int level)
{
	if (level <= 0 || level > MAX_ROLE_LEVEL)
	{
		return;
	}

	TaskList &task_list = m_level_task_list[level];

	int total_count = (int)task_list.size();

	for (int i = 0; i < total_count; ++i)
	{
		if (task_list[i] == taskid)
		{
			return;
		}
	}

	task_list.push_back(taskid);
}

int TaskPool::PutDailyTask(TaskCfg *task_cfg)
{
	if (NULL == task_cfg)
	{
		return -1;
	}

	bool find = false;

	for (int i = 0; i < m_daily_level_interval_count; ++i)
	{
		if (m_daily_level_taskinfo[i].min_level == task_cfg->min_level && m_daily_level_taskinfo[i].max_level == task_cfg->max_level)
		{
			m_daily_level_taskinfo[i].task_list.insert(task_cfg->task_id);
			find = true;
		}
	}			

	if (!find)
	{
		if (m_daily_level_interval_count >= DAILY_LEVEL_INTERVAL_MAX)
		{
			return -2;
		}

		m_daily_level_taskinfo[m_daily_level_interval_count].min_level = task_cfg->min_level;
		m_daily_level_taskinfo[m_daily_level_interval_count].max_level = task_cfg->max_level;
		m_daily_level_taskinfo[m_daily_level_interval_count].task_list.insert(task_cfg->task_id);

		++ m_daily_level_interval_count;
	}		

	return 0;
}

int TaskPool::PutPaohuanTask(TaskCfg *task_cfg)
{
	if (NULL == task_cfg)
	{
		return -1;
	}

	bool find = false;

	for (int i = 0; i < m_paohuan_level_interval_count; ++i)
	{
		if (m_paohuan_level_taskinfo[i].min_level == task_cfg->min_level && m_paohuan_level_taskinfo[i].max_level == task_cfg->max_level)
		{
			m_paohuan_level_taskinfo[i].task_list.insert(task_cfg->task_id);
			find = true;
		}
	}			

	if (!find)
	{
		if (m_paohuan_level_interval_count >= PAOHUAN_LEVEL_INTERVAL_MAX)
		{
			return -2;
		}

		m_paohuan_level_taskinfo[m_paohuan_level_interval_count].min_level = task_cfg->min_level;
		m_paohuan_level_taskinfo[m_paohuan_level_interval_count].max_level = task_cfg->max_level;
		m_paohuan_level_taskinfo[m_paohuan_level_interval_count].task_list.insert(task_cfg->task_id);

		++ m_paohuan_level_interval_count;
	}		

	return 0;
}

void TaskPool::PutGuildTask(int task_id)
{
	m_guild_taskid_set.insert(task_id);
}

void TaskPool::PutZhuanzhiTask(int task_id)
{
	m_zhuanzhi_taskid_set.insert(task_id);
}


bool TaskPool::RandDailyTaskByRoleLevel(int role_level, int *out_taskid, int except_param)
{
	if (NULL == out_taskid) return false;

	int temp_task_count = 0;
	static int daily_temp_task_list[DAILY_LEVEL_TEMP_TASK_MAX];
	memset(daily_temp_task_list, 0, sizeof(daily_temp_task_list));

	for (int i = 0; i < m_daily_level_interval_count && i < DAILY_LEVEL_INTERVAL_MAX; ++i)
	{
		if (m_daily_level_taskinfo[i].LevelInRange(role_level))
		{
			std::set<TaskID> &task_list = m_daily_level_taskinfo[i].task_list;
			if (!task_list.empty())
			{
				std::set<TaskID>::iterator it = task_list.begin();
				for(;it != task_list.end(); it++)
				{
					const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(*it);
					if (NULL != task_cfg && TASK_CON_PASS_DAILY_TASK_FB == task_cfg->condition && except_param == task_cfg->c_param1)
					{
						continue;
					}

					daily_temp_task_list[temp_task_count++] = *it;
				}
			}
		}
	}

	if (temp_task_count > 0)
	{
		int rand_index = RandomNum(temp_task_count);

		if (0 <= rand_index && rand_index < DAILY_LEVEL_TEMP_TASK_MAX)
		{
			*out_taskid = daily_temp_task_list[rand_index];
			return true;
		}
	}

	return false;
}

const DailyTaskReward * TaskPool::GetDailyReward(int level)
{
	int num = static_cast<int>(m_daily_task_reward_list.size());

	for (int i = 0; i < num; ++ i)
	{
		DailyTaskReward *reward = &m_daily_task_reward_list[i];
		if (level >= reward->min_level && level <= reward->max_level)
		{
			return reward;
		}
	}
	
	return NULL;
}

bool TaskPool::GetPaohuanTaskReward(int role_lv, int complete_tasknum, int *nvwashi, long long &exp, ItemConfigData &item)
{
	if (complete_tasknum < 0 || complete_tasknum >= PAOHUAN_ONEDAY_COMMIT_MAX || NULL == nvwashi)
	{
		return false;
	}

	const int give_nvwashi = m_paohuan_reward_cfg.nvwashi_list[complete_tasknum];

	for (int i = 0; i < PAOHUAN_LV_FIX_COUNT; i++)
	{
		if (role_lv >= m_paohuan_lvfix_cfg[i].min_level && role_lv <= m_paohuan_lvfix_cfg[i].max_level)
		{
			*nvwashi = static_cast<int>(give_nvwashi * m_paohuan_lvfix_cfg[i].nvwashi_addpercent / 100.0f);
			exp = static_cast<int>(m_paohuan_reward_cfg.exp_list[complete_tasknum] * m_paohuan_lvfix_cfg[i].exp_addpercent / 100.0f);
			item = m_paohuan_reward_cfg.reward_item[complete_tasknum];

			return true;
		}
	}

	return false;
}

bool TaskPool::RandPaohuanTaskByRoleLevel(int role_level, int *out_taskid)
{
	if (NULL == out_taskid) return false;

	int temp_task_count = 0;
	static int paohun_temp_task_list[PAOHUAN_LEVEL_TEMP_TASK_MAX];
	memset(paohun_temp_task_list, 0, sizeof(paohun_temp_task_list));

	for (int i = 0; i < m_paohuan_level_interval_count && i < PAOHUAN_LEVEL_INTERVAL_MAX; ++i)
	{
		if (m_paohuan_level_taskinfo[i].LevelInRange(role_level))
		{
			std::set<TaskID> &task_list = m_paohuan_level_taskinfo[i].task_list;
			if (!task_list.empty())
			{
				std::set<TaskID>::iterator it = task_list.begin();
				for(;it != task_list.end(); it++)
				{
					paohun_temp_task_list[temp_task_count++] = *it;
				}
			}
		}
	}

	if (temp_task_count > 0)
	{
		int rand_index = RandomNum(temp_task_count);

		if (0 <= rand_index && rand_index < PAOHUAN_LEVEL_TEMP_TASK_MAX)
		{
			*out_taskid = paohun_temp_task_list[rand_index];
			return true;
		}
	}

	return false;
}

bool TaskPool::GetPaoHuanRollPool(PaoHuanRollItem roll_list[PAOHUAN_ROLLPOOL_TOTAL_COUNT], int rolllist_id)
{
	std::map<int, PaoHuanRollPool>::const_iterator iter = m_paohuan_roll_map.find(rolllist_id);
	if (m_paohuan_roll_map.end() == iter)
	{
		return false;
	}

	memcpy(roll_list, iter->second.item_list, sizeof(roll_list[0]) * PAOHUAN_ROLLPOOL_TOTAL_COUNT);

	for (int i = 0; i < PAOHUAN_ROLLPOOL_TOTAL_COUNT; ++ i)
	{
		int rand_idx = RandomNum(PAOHUAN_ROLLPOOL_TOTAL_COUNT);

		PaoHuanRollItem tmp_item = roll_list[i];
		roll_list[i] = roll_list[rand_idx];
		roll_list[rand_idx] = tmp_item;
	}

	return true;
}

const GuildTaskRewardCfg * TaskPool::GetGuildReward(int level)
{
	if (level <= 0 || level > MAX_ROLE_LEVEL)
	{
		return NULL;
	}

	return &m_guild_task_reward_list[level];
}

TaskID TaskPool::GetEndTaskByFirstTask(TaskID first_task)
{
	if (first_task <= 0)
	{
		return 0;
	}

	for (int i = 0; i < m_guild_task_list.cfg_count; i++)
	{
		if (first_task == m_guild_task_list.cfg_list[i].first_task)
		{
			return m_guild_task_list.cfg_list[i].end_task;
		}
	}

	return 0;
}

bool TaskPool::IsGuildFirstTaskID(TaskID task_id)
{
	for (int i = 0; i < m_guild_task_list.cfg_count; i++)
	{
		if (task_id == m_guild_task_list.cfg_list[i].first_task)
		{
			return true;
		}
	}

	return false;
}

int TaskPool::GetRandomTaskStar()
{
	if (m_task_star_total_weight <= 0) return 1;

	int rand_value = RandomNum(m_task_star_total_weight);
	
	std::map<int, TaskStarCfg>::const_iterator iter;
	for (iter = m_task_star_map.begin(); m_task_star_map.end() != iter; ++ iter)
	{
		if (iter->second.weight >= rand_value)
		{
			return iter->second.star_level;
		}
		rand_value -= iter->second.weight;
	}

	return 1;
}

int TaskPool::GetTaskStarRewardPer(int level)
{
	std::map<int, TaskStarCfg>::const_iterator iter = m_task_star_map.find(level);
	if (m_task_star_map.end() == iter)
	{
		return 100;
	}

	return iter->second.reward_rate;
}

bool TaskPool::IsInZhuanzhiTaskList(TaskID first_task, TaskID task_id)
{
	if (task_id <= 0 || first_task <= 0)
	{
		return false;
	}

	if (!this->IsZhuanzhiFirstTaskID(first_task))
	{
		return false;
	}

	TaskID temp_task_id = this->GetZhuanzhiEndTaskByFirstTask(first_task);
	if (0 == temp_task_id)
	{
		return false;
	}

	int check_count = 0;
	while (task_id != temp_task_id && check_count < 100)
	{
		++check_count;

		const TaskCfg *temp_task_cfg_p = this->GetCfgByTaskID(temp_task_id);
		if (NULL == temp_task_cfg_p || TASK_TYPE_ZHUANZHI != temp_task_cfg_p->task_type)
		{
			return false;
		}

		temp_task_id = temp_task_cfg_p->pretaskid;
	}

	if (temp_task_id != task_id)
	{
		return false;
	}

	return true;
}

bool TaskPool::IsZhuanzhiFirstTaskID(TaskID task_id)
{
	for (int i = 0; i < m_zhuanzhi_task_list.cfg_count; i++)
	{
		if (task_id == m_zhuanzhi_task_list.cfg_list[i].first_task)
		{
			return true;
		}
	}

	return false;
}

TaskID TaskPool::GetZhuanzhiEndTaskByFirstTask(TaskID first_task)
{
	if (first_task <= 0)
	{
		return 0;
	}

	for (int i = 0; i < m_zhuanzhi_task_list.cfg_count; i++)
	{
		if (first_task == m_zhuanzhi_task_list.cfg_list[i].first_task)
		{
			return m_zhuanzhi_task_list.cfg_list[i].end_task;
		}
	}

	return 0;
}

const ZhuanzhiTaskConfig::ConfigItem * TaskPool::GetZhuanzhiTaskConfigByProfLevel(int prof_level)
{
	if (prof_level <= 0 || prof_level > MAX_PROF_LEVEL)
	{
		return NULL;
	}

	int index = prof_level - 1;
	return &m_zhuanzhi_task_list.cfg_list[index];
}

int TaskPool::InitTaskRandomCfg(TiXmlElement *RootElement)
{
	TiXmlElement *data_element = RootElement->FirstChildElement("data");

	while (NULL != data_element)
	{
		int task_type = -1;
		if (!GetSubNodeValue(data_element, "task_type", task_type) || task_type <= 0)
		{
			return -30;
		}
		TaskRandomReward& reward_cfg = task_random_config_map[task_type];
		if ((unsigned int)reward_cfg.count >= static_array_size(reward_cfg.per_reward_list))
		{
			return -29;
		}
		PerRandomReward& perward_cfg = reward_cfg.per_reward_list[reward_cfg.count++];
		if (!GetSubNodeValue(data_element, "task_type", perward_cfg.task_type) || perward_cfg.task_type <= 0)
		{
			return -31;
		}
		if (!GetSubNodeValue(data_element, "weight", perward_cfg.weight) || perward_cfg.weight < 0)
		{
			return -32;
		}
		if (!GetSubNodeValue(data_element, "task_index", perward_cfg.index) || perward_cfg.index < 0)
		{
			return -33;
		}
		reward_cfg.weight_sum += perward_cfg.weight;

		{
			// 通用物品奖励
			TiXmlElement *reward_item_element = data_element->FirstChildElement("item");
			if (NULL != reward_item_element)
			{
				if (!perward_cfg.item_data.ReadConfig(reward_item_element))
				{
					return -42;
				}
			}
		}
		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int TaskPool::InitZhuanzhiTaskList(TiXmlElement *RootElement)
{
	m_zhuanzhi_task_list.cfg_count = 0;
	std::set<TaskID> first_task_id_check;

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		if (m_zhuanzhi_task_list.cfg_count < 0 || m_zhuanzhi_task_list.cfg_count >= ZhuanzhiTaskConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1000;
		}

		ZhuanzhiTaskConfig::ConfigItem &item_cfg = m_zhuanzhi_task_list.cfg_list[m_zhuanzhi_task_list.cfg_count];

		if (!GetSubNodeValue(dataElement, "first_task", item_cfg.first_task) || item_cfg.first_task <= 0)
		{
			return -1;
		}

		if (first_task_id_check.end() != first_task_id_check.find(item_cfg.first_task))
		{
			return -2;
		}
		first_task_id_check.insert(item_cfg.first_task);

		if (!GetSubNodeValue(dataElement, "end_task", item_cfg.end_task) || item_cfg.end_task <= 0)
		{
			return -3;
		}

		{
			// 检查任务合法性
			const TaskCfg *first_task_cfg_p = this->GetCfgByTaskID(item_cfg.first_task);
			if (NULL == first_task_cfg_p || TASK_TYPE_ZHUANZHI != first_task_cfg_p->task_type)
			{
				return -10000001;
			}

			int check_count = 0; TaskID temp_task = item_cfg.end_task; int series_count = 0;

			while (item_cfg.first_task != temp_task && check_count < 100)
			{
				++check_count;

				const TaskCfg *task_cfg = this->GetCfgByTaskID(temp_task);
				if (NULL == task_cfg || TASK_TYPE_ZHUANZHI != task_cfg->task_type)
				{
					return -10000003;
				}

				temp_task = task_cfg->pretaskid;

				++series_count;
			}

			if (item_cfg.first_task != temp_task)
			{
				return -10000004;
			}
		}

		{
			// 把首个任务加入到等级 任务映射表里边，用于计算可接任务
			const TaskCfg *first_task_cfg_p = this->GetCfgByTaskID(item_cfg.first_task);
			if (NULL != first_task_cfg_p)
			{
				for (int level = first_task_cfg_p->min_level; level <= first_task_cfg_p->max_level; ++level)
				{
					this->PutLevelTask(first_task_cfg_p->task_id, level);
				}
			}
		}

		++m_zhuanzhi_task_list.cfg_count;

		dataElement = dataElement->NextSiblingElement();
	}

	{
		int last_level = 0;
		for (int i = 0; i < m_zhuanzhi_task_list.cfg_count && i < ZhuanzhiTaskConfig::CONFIG_ITEM_MAX_COUNT; ++i)
		{
			const TaskCfg *first_task_p = this->GetCfgByTaskID(m_zhuanzhi_task_list.cfg_list[i].first_task);
			if (NULL == first_task_p)
			{
				return -10000008;
			}

			if (i > 0)
			{
				if (first_task_p->min_level <= last_level)
				{
					return -10000009;
				}
			}

			last_level = first_task_p->min_level;
		}
	}

	return 0;
}

PerRandomReward * TaskPool::GetRandomReward(int task_type)
{
	if (task_random_config_map.find(task_type) == task_random_config_map.end())
	{
		return NULL;
	}

	TaskRandomReward &task_random_cfg = task_random_config_map[task_type];
	if (task_random_cfg.weight_sum <= 0)
	{
		return NULL;
	}

	int rand_value = RandomNum(task_random_cfg.weight_sum);
	for (int i = 0; i < task_random_cfg.count && i < MAX_TASK_RANDOM_CONFIG; i++)
	{
		if (rand_value <= task_random_cfg.per_reward_list[i].weight)
		{
			return &task_random_cfg.per_reward_list[i];
		}
		rand_value -= task_random_cfg.per_reward_list[i].weight;
	}

	return NULL;
}