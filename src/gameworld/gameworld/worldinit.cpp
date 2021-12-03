#include "world.h"
#include "utility/configpath.h"
#include "internalcomm.h"
#include "globalconfig/globalconfig.h"
#include "config/restconfig.hpp"
#include "item/itempool.h"
#include "task/taskpool.h"
#include "monster/monsterpool.h"
#include "monster/drop/droppool.hpp"
#include "chat/chatmanager.h"

#include "gamelog.h"
#include "gstr.h"

#include "servercommon/internalprotocal/lgprotocal.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/namefilter.hpp"
#include "servercommon/serverconfig/crosscommonconfig.hpp"
#include "servercommon/serverconfig/crossxiuluotowercommonconfig.hpp"
#include "checkresourcecenter.hpp"
#include "skill/skillpool.hpp"
#include "other/welfare/welfareconfig.hpp"
#include "other/convertshop/convertshop.hpp"
#include "config/mentalityconfig.hpp"
#include "scene/sceneregister/sceneregister.hpp"
#include "config/logicconfigmanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "global/usercache/usercache.hpp"
#include "global/usercache/relationcache.hpp"
#include "servercommon/serverconfig/cross1v1commonconfig.hpp"
#include "servercommon/serverconfig/crossmultiuserchallengeconfig.hpp"
#include "servercommon/serverconfig/crosstuanzhanconfig.hpp"
#include "servercommon/serverconfig/crosspastureconfig.hpp"
#include "servercommon/serverconfig/crossbossconfig.hpp"
#include "servercommon/serverconfig/crosshotspringconfig.hpp"
#include "servercommon/serverconfig/crossshuijingconfig.hpp"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"
#include "servercommon/serverconfig/crossdarknightconfig.hpp"
#include "servercommon/serverconfig/cross3v3config.hpp"
#include "servercommon/serverconfig/crossfishingconfig.hpp"
#include "servercommon/serverconfig/crossliekunfbconfig.hpp"
#include "servercommon/serverconfig/crossrankrewardconfig.hpp"
#include "servercommon/serverconfig/crossvipbossconfig.hpp"
#include "servercommon/serverconfig/crossmizangbossconfig.hpp"
#include "servercommon/serverconfig/crossyoumingbossconfig.hpp"
#include "servercommon/serverconfig/cloudpurchaseconfig.hpp"
#include "servercommon/serverconfig/crossbianjingzhidiconfig.h"
#include "servercommon/serverconfig/husongshuijingconfig.hpp"

void World::RecalNextResetDataTimeStamp()
{
	time_t last_reset_module_data_timestamp = m_last_reset_role_module_timestamp;

	const tm *last_reset_tm = ::localtime(&last_reset_module_data_timestamp);
	if (NULL != last_reset_tm)
	{
		time_t zero_timestamp = GetZeroTime(last_reset_module_data_timestamp);
		if (last_reset_tm->tm_hour >= ROLE_RESET_MODULE_DATA_TM_HOUR)
		{
			m_next_reset_role_module_timestamp = zero_timestamp + SECOND_PER_DAY + ROLE_RESET_MODULE_DATA_TM_HOUR * SECOND_PER_HOUR;
		}
		else
		{
			m_next_reset_role_module_timestamp = zero_timestamp + ROLE_RESET_MODULE_DATA_TM_HOUR * SECOND_PER_HOUR;
		}
	}
}

bool World::ListenForGateway()
{
	int backlog = 64;
	EngineAdapter::Instance().ConfigSyncValue(ROOT/"World"/"GameServer"/"Backlog", &backlog, backlog);
	bool ret = m_network->Listen(m_listen_port, backlog);
	if (!ret)
	{
		gamelog::g_log_world.print(LL_MAINTANCE, "NetListen return FAIL!");
		return false;
	}
	gamelog::g_log_world.printf(LL_MAINTANCE, "Server Listen[%d] suc.", m_listen_port);
	return true;
}

bool World::ConnectToDB()
{
	// 角色相关部分
	std::string db_server_ip = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBAccounter.module_addr.ip;
	Port db_server_port = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBAccounter.module_addr.port;

	rmi::Session accounter_session = EngineAdapter::Instance().RMICreateSession(db_server_ip.c_str(), db_server_port);
	if (accounter_session.handle == 0)
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "Connect Accounter DBLayer[%s:%d] Fail!", db_server_ip.c_str(), (int)db_server_port);
		return false;
	}

	m_db_rmi_session_list.push_back(accounter_session);

	unsigned int role_db_num = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBRoleCount;
	if (role_db_num == 0 || role_db_num > (unsigned int)DBHandler::MAX_ROLE_DB_INDEX)
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "Role DBLayer NUM:%d is NOT valid!", role_db_num);
		return false;
	}
	rmi::Session role_db_session_list[DBHandler::MAX_ROLE_DB_INDEX];

	for (unsigned int i = 0; i < role_db_num; ++i)
	{
		std::string role_db_server_ip = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBRoleList[i].module_addr.ip;
		Port role_db_server_port = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBRoleList[i].module_addr.port;

		rmi::Session role_db = EngineAdapter::Instance().RMICreateSession(role_db_server_ip.c_str(), role_db_server_port);
		if (role_db.handle == 0)
		{
			gamelog::g_log_world.printf(LL_MAINTANCE, "Connect Role DBLayer[%s:%d] Fail!", role_db_server_ip.c_str(), (int)role_db_server_port);
			return false;
		}
		m_db_rmi_session_list.push_back(role_db);
		role_db_session_list[i] = role_db;
	}

	// 全局相关部分
	std::string global_db_server_ip = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBGlobal.module_addr.ip;
	Port global_db_server_port = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBGlobal.module_addr.port;

	rmi::Session global_session = EngineAdapter::Instance().RMICreateSession(global_db_server_ip.c_str(), global_db_server_port);
	if (global_session.handle == 0)
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "Connect Center DBLayer[%s:%d] Fail!", global_db_server_ip.c_str(), (int)global_db_server_port);
		return false;
	}
	gamelog::g_log_world.printf(LL_MAINTANCE, "Connect Center DBLayer[%s:%d] Suc!", global_db_server_ip.c_str(), (int)global_db_server_port);

	std::string system_db_server_ip = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBSystem.module_addr.ip;
	Port system_db_server_port = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBSystem.module_addr.port;

	rmi::Session system_session = EngineAdapter::Instance().RMICreateSession(system_db_server_ip.c_str(), system_db_server_port);
	if (system_session.handle == 0)
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "Connect System DBLayer[%s:%d] Fail!", system_db_server_ip.c_str(), (int)system_db_server_port);
		return false;
	}
	gamelog::g_log_world.printf(LL_MAINTANCE, "Connect System DBLayer[%s:%d] Suc!", system_db_server_ip.c_str(), (int)system_db_server_port);


	InternalComm::Instance().SetDBSession(accounter_session, role_db_session_list, role_db_num, global_session, system_session, rmi::Session());

	gamelog::g_log_world.printf(LL_MAINTANCE, "Connect to DataAccessServer suc. Accounter[%s:%d], Role_db_num:%d", db_server_ip.c_str(), (int)db_server_port, role_db_num);
	return true;
}

bool World::LoadFromDBDirectly()
{
	unsigned int role_db_num = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBRoleCount;
	if (role_db_num != 1)
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "LoadFromDBDirectly Role DBLayer NUM:%d is NOT valid!", role_db_num);
		return false;
	}
	
	DBModuleAddr &db_cfg = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBRoleList[0];


	TMysqlStatement stMysqlStatement;
	CMysqlUtility::Init(&stMysqlStatement);

	char szErrMsg[512];
	int iRet = CMysqlUtility::ConnectDB(&stMysqlStatement,
				db_cfg.db_config.mysql_addr.ip.c_str(), db_cfg.db_config.mysql_db_username.c_str(),
				db_cfg.db_config.mysql_db_password.c_str(), db_cfg.db_config.mysql_db_name.c_str(),
				db_cfg.db_config.mysql_addr.port, szErrMsg);

	if (0 != iRet)
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "UserCacheManager::LoadAllUser failed :%d---------------- \n",iRet);
		CMysqlUtility::CloseDatabase(&stMysqlStatement);
		return false;
	}

	bool ret = true;
	
	{
		// 加载角色缓存
		ret = UserCacheManager::Instance().LoadAllUser(stMysqlStatement);
		if (!ret)
		{
			CMysqlUtility::CloseDatabase(&stMysqlStatement);

			gamelog::g_log_world.printf(LL_MAINTANCE, "LoadAllUser Fail!");
			return false;
		}
	}

	{
		// 加载关系缓存
		ret = RelationCacheManager::Instance().LoadAllRelation(stMysqlStatement);
		if (!ret)
		{
			CMysqlUtility::CloseDatabase(&stMysqlStatement);

			gamelog::g_log_world.printf(LL_MAINTANCE, "LoadAllRelation Fail!");
			return false;
		}
	}

	CMysqlUtility::CloseDatabase(&stMysqlStatement);

	return true;
}

bool World::ConnectToLoginServer()
{
	int game_thread = 0;
	std::string login_server_ip = ServerConfigPool::Instance().common_config.ipconfigs.login_server_addr.inner_connect_addr.server_ip;
	Port login_server_port = ServerConfigPool::Instance().common_config.ipconfigs.login_server_addr.inner_connect_addr.for_gameserver.port;

	EngineAdapter::Instance().ConfigSyncValue(ROOT/"GameThread", &game_thread, game_thread);

	bool ret = InternalComm::Instance().NetConnect(login_server_ip.c_str(), login_server_port, &m_login_server_netid);
	if (!ret)
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "Connect To LoginServer[%s:%d] Fail!", login_server_ip.c_str(), (int)login_server_port);
		return false;
	}

	InternalComm::Instance().m_login_server_id = m_login_server_netid;
	EngineAdapter::Instance().m_game_thread = game_thread;

	gamelog::g_log_world.printf(LL_MAINTANCE, "Connect to LoginServer[%s:%d] suc.", login_server_ip.c_str(), (int)login_server_port);

	// 将自己注册到登陆服务器
	lgprotocal::GLRegisterGS rgs;
	memset(rgs.ip, 0, sizeof(rgs.ip));
	STRNCPY(rgs.ip, m_local_ip.c_str(), sizeof(rgs.ip));
	rgs.index = m_server_index;
	rgs.port = m_listen_port;
	memset(rgs.scene_list, 0, sizeof(rgs.scene_list));
	rgs.scene_count = m_scene_manager.GetSceneList(rgs.scene_list, MAX_SCENE_LIST_NUM);
	InternalComm::Instance().NetSend(InternalComm::Instance().m_login_server_id, (const char *)&rgs, sizeof(lgprotocal::GLRegisterGS));

	return true;
}

bool World::InitGameWorld(std::string config_dir)
{
	std::string error;

	// 加载cross配置
	if (!CrossConfig::Instance().Init(config_dir + "cross.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossConfig::Instance().Init Return Fail! Error At:%s", error.c_str());
		return false;
	}

	// 初始化字符串资源
	{
		if (!gamestring::StringInit((config_dir + "/serverconfig/string.xml").c_str(), &error))
		{
			gamelog::g_log_world.printf(LL_MAINTANCE, "gamestring::StringInit Return Fail! Error At:%s", error.c_str());
			return false;
		}

		if (!gstr::Init(config_dir + "/serverconfig/gstr.xml", &error))
		{
			gamelog::g_log_world.printf(LL_MAINTANCE, "gstr::Init Return Fail! Error At:%s", error.c_str());
			return false;
		}
	}

	// 初始化敏感词
	if (!NameFilter::Instance().Init((config_dir + "/serverconfig/namefilter.txt").c_str(), &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "NameFilter Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!SHAREDCONFIG->Init(config_dir, &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "SharedConfig::Init Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!LocalConfig::Instance().Init(config_dir + "local.xml", &error))
	{
		gamelog::g_log_world.printf(LL_ERROR, error.c_str());
		gamelog::g_log_world.printf(LL_ERROR, "World::InitGameWorld, LocalConfig::Init return FALSE.");

		return false;
	}

	// 初始化cross配置
	if (!CrossCommonConfig::Instance().Init(config_dir + "crossserver/cross_common.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossCommonConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossXiuluoTowerCommonConfig::Instance().Init(config_dir + "crossserver/cross_xiuluotower.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossXiuluoTowerCommonConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!Cross1V1CommonConfig::Instance().Init(config_dir + "crossserver/cross_1v1.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "Cross1V1CommonConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

// 	if (!CrossMultiUserChallengeConfig::Instance().Init(config_dir + "crossserver/cross_mutiuserchallenge.xml", &error))
// 	{
// 		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossMultiUserChallengeConfig Return Fail! Error At:%s", error.c_str());
// 		return false;
// 	}

	if (!Cross3v3Config::Instance().Init(config_dir + "crossserver/kuafu_tvt.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "Cross3v3Config Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossTuanzhanConfig::Instance().Init(config_dir + "crossserver/cross_tuanzhan.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossTuanzhanConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossPastureConfig::Instance().Init(config_dir + "crossserver/cross_pasture.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossPastureConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossBossConfig::Instance().Init(config_dir + "crossserver/cross_boss.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossBossConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossHotSpringConfig::Instance().Init(config_dir + "crossserver/cross_hotspring.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossHotSpringConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossShuiJingConfig::Instance().Init(config_dir + "crossserver/cross_shuijing.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossShuiJingConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}


	if (!CrossGuildBattleConfig::Instance().Init(config_dir + "crossserver/cross_guildbattle.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossGuildBattleConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossDarkNightConfig::Instance().Init(config_dir + "crossserver/cross_dark_night.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossDarkNightConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossFishingConfig::Instance().Init(config_dir + "crossserver/cross_fishing.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossFishingConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossLieKunFbConfig::Instance().Init(config_dir + "crossserver/cross_liekun.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossLieKunFbConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossRankRewardConfig::Instance().Init(config_dir + "crossserver/crossrank_reward.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossRankRewardConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossVipBossConfig::Instance().Init(config_dir + "crossserver/cross_vip_boss.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossVipBossConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossMizangBossConfig::Instance().Init(config_dir + "crossserver/cross_mizang_boss.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossMizangBossConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossYoumingBossConfig::Instance().Init(config_dir + "crossserver/cross_youming_boss.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossYoumingBossConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CloudPurchaseConfig::Instance().Init(config_dir + "crossserver/cloud_purchase.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CloudPurchaseConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!CrossBianJingZhiDiConfig::Instance().Init(config_dir + "crossserver/cross_bianjingzhidi.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "CrossBianJingZhiDiConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	if (!HusongShuijingConfig::Instance().Init(config_dir + "crossserver/cross_husong_shuijing.xml", &error))
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "HusongShuijingConfig Return Fail! Error At:%s", error.c_str());
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	config_dir += "gameworld/";

	if (!InitSkill(config_dir))					// 加载技能
	{
		return false;
	}

	if (!InitItem(config_dir))					// 需要检查【技能id、宠物ID】，所以必须保证【技能池】【宠物池】已经初始完
	{
		return false;
	}

	if (!InitDrop(config_dir))					// 需要检查【物品】
	{
		return false;
	}

	if (!InitMonster(config_dir))				// 需要检查【掉落、BOSS技能条件】
	{
		return false;
	}

	if (!InitTask(config_dir))					// 需要检查【技能、物品】，所以必须保证【技能池、物品池】已经初始完 不做【场景】检查 由外部编辑器检查
	{
		return false;
	}

	if (!InitGlobalConfig(config_dir))			// 需要检查【特殊物品id-转职、复活】 所以必须保证【物品池】已经初始完
	{
		return false;
	}

	if (!InitScene(config_dir))					// 需要检查【技能id、掉落物品id、任务id】 所以必须保证【技能池、物品池、任务池】已经初始完
	{
		return false;
	}	

	if (!InitLogicConfig(config_dir))			// 逻辑配置 放最后
	{
		return false;
	}

	std::string err;
	if (!CheckResourceCenter::Instance().Check(&err))
	{
		gamelog::g_log_world.printf(LL_ERROR, err.c_str());
		gamelog::g_log_world.printf(LL_ERROR, "World On INIT, CheckResourceCenter::Check return FALSE.");
		return false;
	}

	return true;
}

bool World::InitGlobalConfig(const std::string &config_dir)
{
	// 加载全局配置
	std::string err;
	std::string config_name = "globalconfigmanager.xml";
	clock_t beg_time = clock();

	if (!GLOBALCONFIG->Init(config_dir, config_name, &err))
	{
		gamelog::g_log_world.printf(LL_ERROR, err.c_str());
		gamelog::g_log_world.printf(LL_ERROR, "World On INIT, GlobalConfig::Init return FALSE.");
		return false;
	}
	clock_t end_time = clock();
	gamelog::g_log_world.printf(LL_INFO, "World On INIT, GlobalConfig::Init Success. cost time : %f s", (end_time - beg_time) * 1.0 / CLOCKS_PER_SEC);

	return true;
}

bool World::InitSkill(const std::string &config_dir)
{
	// 加载技能配置
	std::string skill_err;
	clock_t beg_time = clock();
	if (!SKILLPOOL->Init(config_dir.c_str(), &skill_err))
	{
		gamelog::g_log_world.printf(LL_ERROR, skill_err.c_str());
		gamelog::g_log_world.printf(LL_ERROR, "World On INIT, SkillPool::Init return FALSE.");
		return false;
	}
	clock_t end_time = clock();
	gamelog::g_log_world.printf(LL_INFO, "World On INIT, SkillPool::Init Success. cost time : %f s", (end_time - beg_time) * 1.0 / CLOCKS_PER_SEC);

	return true;
}

bool World::InitItem(const std::string &config_dir)
{
	std::string err_info;
	std::string config_name;

	// 加载物品配置
	config_name = "itemmanager.xml";
	clock_t beg_time = clock();

	if (!ITEMPOOL->Init(config_dir, config_name, &err_info))
	{
		gamelog::g_log_world.printf(LL_ERROR, err_info.c_str());
		gamelog::g_log_world.printf(LL_ERROR, "World On INIT, ItemPool::Init return FALSE.");
		return false;
	}
	clock_t end_time = clock();
	gamelog::g_log_world.printf(LL_INFO, "World On INIT, ItemPool::Init Success. cost time : %f s", (end_time - beg_time) * 1.0 / CLOCKS_PER_SEC);

	return true;
}

bool World::InitMonster(const std::string &config_dir)
{
	// 加载怪物
	std::string err_info;
	std::string config_name = "monstermanager.xml";
	clock_t beg_time = clock();

	if (!MONSTERPOOL->Init(config_dir, config_name, &err_info))
	{
		gamelog::g_log_world.printf(LL_ERROR, err_info.c_str());
		gamelog::g_log_world.printf(LL_ERROR, "World On INIT, MonsterPool::Init return FALSE.");
		return false;
	}
	clock_t end_time = clock();
	gamelog::g_log_world.printf(LL_INFO, "World On INIT, MonsterPool::Init Success. cost time : %f s", (end_time - beg_time) * 1.0 / CLOCKS_PER_SEC);

	return true;
}

bool World::InitScene(const std::string &config_dir)
{
	// 加载场景和地图配置
	std::string scenemanager_config_name = "scenemanager.xml";
	clock_t beg_time = clock();

	SceneManagerConfig scenemanagerconfig;
	if (!scenemanagerconfig.Load(config_dir, scenemanager_config_name, m_total_server, m_server_index))
	{
		gamelog::g_log_world.printf(LL_ERROR, "World On INIT, Load Scene and Map Config Error:\n%s", scenemanagerconfig.GetError().c_str());
		return false;
	}

	std::string err;
	if (!SceneRegister::Instance().LoadSceneLineConfig(config_dir + "logicconfig/scene_line_config.xml", &err))
	{
		gamelog::g_log_world.printf(LL_ERROR, "World On INIT, Error:%s", err.c_str());
		return false;
	}

	bool ret = m_scene_manager.Init(scenemanagerconfig, m_total_server, m_server_index);

	clock_t end_time = clock();
	gamelog::g_log_world.printf(LL_INFO, "World On INIT, Scene and Map Config Load Success. cost time : %f s", (end_time - beg_time) * 1.0 / CLOCKS_PER_SEC);

	return ret;
}

bool World::InitTask(const std::string &config_dir)
{
	std::string err_info;
	std::string config_name;
	clock_t beg_time = clock();

	// 加载任务配置
	config_name = "tasklist.xml";

	if (!TASKPOOL->Init(config_dir, config_name, &err_info))
	{
		gamelog::g_log_world.printf(LL_ERROR, err_info.c_str());
		gamelog::g_log_world.printf(LL_ERROR, "World On INIT, TaskPool::Init return FALSE.");
		return false;
	}
	clock_t end_time = clock();
	gamelog::g_log_world.printf(LL_INFO, "World On INIT, TaskPool::Init Success. cost time : %f s", (end_time - beg_time) * 1.0 / CLOCKS_PER_SEC);

	return true;
}

bool World::InitDrop(const std::string &config_dir)
{
	std::string err_info;
	std::string config_name;
	clock_t beg_time = clock();

	// 加载掉落配置
	config_name = "dropmanager.xml";

	if (!DROPPOOL->Init(config_dir, config_name, &err_info))
	{
		gamelog::g_log_world.printf(LL_ERROR, err_info.c_str());
		gamelog::g_log_world.printf(LL_ERROR, "World On INIT, DropPool::Init return FALSE.");
		return false;
	}
	clock_t end_time = clock();
	gamelog::g_log_world.printf(LL_INFO, "World On INIT, DropPool::Init Success. cost time : %f s", (end_time - beg_time) * 1.0 / CLOCKS_PER_SEC);

	return true;
}

bool World::InitLogicConfig(const std::string &config_dir)
{
	std::string err_info;
	clock_t beg_time = clock();

	if (!LogicConfigManager::Instance()->Init(config_dir, &err_info))
	{
		gamelog::g_log_world.printf(LL_ERROR, err_info.c_str());
		gamelog::g_log_world.printf(LL_ERROR, "World On INIT, LogicConfig::Init return FALSE.");
		return false;
	}
	clock_t end_time = clock();
	gamelog::g_log_world.printf(LL_INFO, "World On INIT, LogicConfig::Init Success. cost time : %f s", (end_time - beg_time) * 1.0 / CLOCKS_PER_SEC);

	return true;
}

bool World::InitChannel()
{
	for (int i = CAMP_TYPE_INVALID + 1; i < CAMP_TYPE_MAX; ++i)
	{
		int cid = chatdef::CHANNEL_ID_CAMP_BEGIN + i;

		ChatManager::Instance().CreateChatChannel(cid, CHANNEL_CD_TIME_CAMP, chatdef::CHANNEL_TYPE_CAMP, "Camp");
	}

	return true;
}

bool World::ReloadScene(const std::string &config_dir, std::string *err)
{
	std::string scenemanager_config_name = "scenemanager.xml";

	SceneManagerConfig scenemanagerconfig;
	if (!scenemanagerconfig.Load(config_dir, scenemanager_config_name, m_total_server, m_server_index))
	{
		*err = scenemanagerconfig.GetError();
		return false;
	}

	if (!SceneRegister::Instance().LoadSceneLineConfig(config_dir + "logicconfig/scene_line_config.xml", err))
	{
		return false;
	}

	bool ret = m_scene_manager.ReInit(scenemanagerconfig, m_total_server, m_server_index);

	return ret;
}


