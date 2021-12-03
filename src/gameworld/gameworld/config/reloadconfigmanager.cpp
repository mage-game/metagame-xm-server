#include "reloadconfigmanager.hpp"

#include "servercommon/performancecatch.hpp"
#include "config/logicconfigmanager.hpp"

#include "gstr.h"
#include "gamelog.h"
#include "engineadapter.h"

#include "utility/configpath.h"

#include "globalconfig/globalconfig.h"
#include "skill/skillpool.hpp"
#include "item/itempool.h"
#include "task/taskpool.h"
#include "monster/monsterpool.h"
#include "monster/drop/droppool.hpp"

#include "world.h"
#include "config/sharedconfig/sharedconfig.h"
#include "global/randactivity/randactivitymanager.hpp"

ReloadConfigManager::ReloadConfigManager()
{

}

ReloadConfigManager::~ReloadConfigManager()
{

}

ReloadConfigManager & ReloadConfigManager::Instance()
{
	static ReloadConfigManager inst;
	return inst;
}

bool ReloadConfigManager::ReloadConfig(int reload_cfg_type, int param1, int param2)
{
	std::string config_dir = "../config";
	EngineAdapter::Instance().ConfigSyncValue(ROOT/"ConfigDir", &config_dir, config_dir);
	if (config_dir[config_dir.length() - 1] != '/')
	{
		config_dir += '/';
	}

	if (RELOAD_CONFIG_TYPE_SHARED != reload_cfg_type)
	{
		config_dir += "gameworld/";
	}

	//PerformanceCatch::Instance().SimplePerformanceCatchBegin(PERFORMANCE_CATCH_TYPE_RELOAD_CONFIG);

	std::string err;

	switch (reload_cfg_type)
	{
	case RELOAD_CONFIG_TYPE_GLOBAL_CONFIG: // 重读全局配置 globalconfig.xml 
		{
			std::string config_name = "globalconfigmanager.xml"; 

			if (!GlobalConfig::Reload(config_dir, config_name, &err))
			{
				gamelog::g_log_world.printf(LL_ERROR, err.c_str());
				gamelog::g_log_world.printf(LL_ERROR, "GlobalConfig::Reload return FALSE.");
				return false;
			}

			gamelog::g_log_world.printf(LL_INFO, "GlobalConfig::Reload Success.");
		}
		break;

	case RELOAD_CONFIG_TYPE_SKILL: // 重读技能配置
		{
			if (!SkillPool::Reload(config_dir.c_str(), &err))	
			{
				gamelog::g_log_world.printf(LL_ERROR, err.c_str());
				gamelog::g_log_world.printf(LL_ERROR, "SkillPoll::Reload return FALSE.");
				return false;
			}
			gamelog::g_log_world.printf(LL_INFO, "SkillPoll::Reload Success.");
		}
		break;

	case RELOAD_CONFIG_TYPE_TASK:	// 重读任务配置
		{
			std::string config_name = "tasklist.xml";

			if (!TaskPool::Reload(config_dir.c_str(), config_name, &err))	
			{
				gamelog::g_log_world.printf(LL_ERROR, err.c_str());
				gamelog::g_log_world.printf(LL_ERROR, "TaskPool::Reload return FALSE.");
				return false;
			}
			gamelog::g_log_world.printf(LL_INFO, "TaskPool::Reload Success.");
		}
		break;

	case RELOAD_CONFIG_TYPE_MONSTER: // 重读怪物配置
		{
			std::string config_name = "monstermanager.xml";

			if (!MonsterPool::Reload(config_dir.c_str(), config_name, &err))	
			{
				gamelog::g_log_world.printf(LL_ERROR, err.c_str());
				gamelog::g_log_world.printf(LL_ERROR, "Monster::Reload return FALSE.");
				return false;
			}
			gamelog::g_log_world.printf(LL_INFO, "Monster::Reload Success.");
		}
		break;

	case RELOAD_CONFIG_TYPE_ITEM: // 重读物品配置
		{
			std::string config_name = "itemmanager.xml";

			if (!ItemPool::Reload(config_dir, config_name, &err))
			{
				gamelog::g_log_world.printf(LL_ERROR, err.c_str());
				gamelog::g_log_world.printf(LL_ERROR, "ItemPool::Reload return FALSE.");
				return false;
			}
			gamelog::g_log_world.printf(LL_INFO, "ItemPool::Reload Success.");
		}
		break;

	case RELOAD_CONFIG_TYPE_LOGIC_CONFIG: // 重读逻辑配置
		{
			if (!LogicConfigManager::Reload(config_dir, &err))
			{
				gamelog::g_log_world.printf(LL_ERROR, err.c_str());
				gamelog::g_log_world.printf(LL_ERROR, "LogicConfig::Reload return FALSE.");
				return false;
			}
			gamelog::g_log_world.printf(LL_INFO, "LogicConfig::Reload Success.");
		}
		break;

	case RELOAD_CONFIG_TYPE_DROP: // 重读掉落配置
		{
			std::string config_name = "dropmanager.xml";

			if (!DropPool::Reload(config_dir, config_name, &err))
			{
				gamelog::g_log_world.printf(LL_ERROR, err.c_str());
				gamelog::g_log_world.printf(LL_ERROR, "Droppool::Reload return FALSE.");
				return false;
			}
			gamelog::g_log_world.printf(LL_INFO, "Droppool::Reload Success.");
		}
		break;

	case RELOAD_CONFIG_TYPE_SCENE: // 重读场景配置
		{
			if (!World::GetInstWorld()->ReloadScene(config_dir, &err))
			{
				gamelog::g_log_world.printf(LL_ERROR, err.c_str());
				gamelog::g_log_world.printf(LL_ERROR, "SceneConfig::Reload return FALSE.");
				return false;
			}
			gamelog::g_log_world.printf(LL_INFO, "SceneConfig::Reload Success.");
		}
		break;

	case RELOAD_CONFIG_TYPE_SHARED:
		{
			if (!SharedConfig::Reload(config_dir, &err))
			{
				gamelog::g_log_world.printf(LL_ERROR, err.c_str());
				gamelog::g_log_world.printf(LL_ERROR, "SharedConfig::Reload return FALSE.");
				return false;
			}
			gamelog::g_log_world.printf(LL_INFO, "SharedConfig::Reload Success.");
		}
		break;

	case RELOAD_CONFIG_TYPE_ONE_ITEM:
		{
			if (!ITEMPOOL->ReloadOneItem(config_dir, param1, param2, &err))
			{
				gamelog::g_log_world.printf(LL_ERROR, err.c_str());
				gamelog::g_log_world.printf(LL_ERROR, "ItemPool::ReloadOneItem return FALSE.");
				return false;
			}
			gamelog::g_log_world.printf(LL_INFO, "ItemPool::ReloadOneItem Success.");
		}
		break;

	case RELOAD_CONFIG_TYPE_GSTR:	// 重读字符串资源配置
		{
			if (!gstr::Reload(config_dir + "../serverconfig/gstr.xml", &err))
			{
				gamelog::g_log_world.printf(LL_ERROR, err.c_str());
				gamelog::g_log_world.printf(LL_ERROR, "gstr::Reload return FALSE.");
				return false;
			}
			gamelog::g_log_world.printf(LL_INFO, "gstr::Reload Success.");
		}
		break;
	}

	//PerformanceCatch::Instance().SimplePerformanceCatchEnd(PERFORMANCE_CATCH_TYPE_RELOAD_CONFIG);

	return true;
}

