
#include "dataaccess.h"

#include "inetworkmodule.h"
#include "ilogmodule.h"
#include "igamemodule.h"
#include "iconfigmodule.h"
#include "irmimodule.h"
#include "istdcommand.h"

#include "utility/configpath.h"
#include "common/platform/socket/pisocket.h"
#include "common/syspath.h"

#include "databaselayer.h"
#include "datalayer/db/mysql/mysqlconnfactroy.h"

#include "darmiinit.h"
#include "engineadapter.h"
#include "servercommon/servercommon.h"

#include "servercommon/serverconfig/serverconfigpool.h"

#include "dataaccesslog.hpp"

#include "rolecachemanager.hpp"

#include "gameserver/game.h"
#include "servercommon/struct/roleotherinitparam.hpp"
#include "datalayer/tabledef/tablefieldid.h"

class DataAccessNetworkCallback : public IEngineNetCallback
{
public:
	DataAccessNetworkCallback(DataAccess *host):m_host(host), m_allow_ip_prefix("")
	{
	}
	~DataAccessNetworkCallback(){}

	void SetAllowIpPrefix(const std::string &allow_ip_prefix) { m_allow_ip_prefix = allow_ip_prefix; }

	virtual void OnAccept(Port listen_port, NetID netid, IP ip, Port port)
	{
		m_host->m_log.printf(LL_DEBUG, "Network OnAccept netid:%d.", netid);

		// 检查IP前缀是否合法
		sockaddr_in host_adr;
		memset(&host_adr, 0, sizeof(sockaddr_in));
		host_adr.sin_family = AF_INET;
		host_adr.sin_addr.s_addr = htonl(ip);
		std::string ip_str = inet_ntoa(host_adr.sin_addr);
		std::string ip_record = ip_str;
		ip_str.resize(m_allow_ip_prefix.size());
		if (ip_str == m_allow_ip_prefix)
		{
			m_host->m_log.printf(LL_DEBUG, "Network Accept ip[%s].", ip_record.c_str());
		}
		else
		{
			m_host->m_log.printf(LL_WARNING, "Network Accept NOT allow ip[%s].", ip_record.c_str());
			m_host->m_network->Disconnect(netid);
			return;
		}
	}

	virtual void OnRecv(NetID netid, const char *data, int length)
	{
		//m_host->m_log.printf("Network OnRecv netid:%d data:%s.", netid, data);

	}

	virtual void OnDisconnect(NetID netid)
	{
		//m_host->m_log.printf(LL_DEBUG, "Network OnDisconnect netid:%d.", netid);
	}
	virtual void OnConnect(bool result, int handle, NetID netid, IP ip, Port port)
	{
	}

private:
	DataAccess *m_host;
	std::string m_allow_ip_prefix;
};

class DataAccessCmd : public ICmdCallback
{
public:
	DataAccessCmd(DataAccess *server):m_server(server){}
	virtual ~DataAccessCmd(){}
	virtual void OnCmd(const char * cmd)
	{
		if (std::string("stop") == cmd)
		{
			m_server->m_game->StopGame();
		}
	}
protected:
	DataAccess *m_server;
};

DataAccess::DataAccess()
:m_game(0), m_network(0), m_network_callback(0), m_conn_factroy(0), m_cmd_callback(0), m_rmi_port(0)
{
	m_network_callback = new DataAccessNetworkCallback(this);
	m_cmd_callback = new DataAccessCmd(this);
}

DataAccess::~DataAccess()
{
	delete m_network_callback;
	delete m_cmd_callback;
}


void DataAccess::Free()
{
	delete this;
}

void DataAccess::StopServer()
{
	Game *game = (Game *)this->Interface();
	game->Stop();
}

int DataAccess::Init()
{
	DEPEND_ON_INIT(LOG_MODULE)
	DEPEND_ON_INIT(CONFIG_MODULE)
	DEPEND_ON_INIT(CLOCK_MODULE)
	DEPEND_ON_INIT(TIMER_MODULE)

	ITimerModule *timer = dynamic_cast<ITimerModule *>(Interface()->QueryModule(TIMER_MODULE));
	IClockModule *clock = dynamic_cast<IClockModule *>(Interface()->QueryModule(CLOCK_MODULE));

	EngineAdapter::Instance().Init(clock, timer);

	ILogModule *log = dynamic_cast<ILogModule*>(Interface()->QueryModule(LOG_MODULE));

	m_log.SetLogModule(log);
	m_log.SetCatagory(DATA_ACCESS_SERVER);

	IConfigModule *config = dynamic_cast<IConfigModule *>(Interface()->QueryModule(CONFIG_MODULE));

	// 获取配置目录
	std::string config_dir = "../config";
	config->SyncValue(ROOT/"ConfigDir", &config_dir, config_dir);
	if (config_dir[config_dir.length() - 1] != '/')
	{
		config_dir += '/';
	}

	// 初始化全局配置
	char err_info[1024]={0};
	if(!ServerConfigPool::Instance().Init(config_dir,err_info))
	{
		m_log.printf(LL_ERROR, err_info);
		m_log.printf(LL_ERROR, "DataBaseLayer On Init, ServerConfigPool::Init return FALSE.");

		this->StopServer();

		return Fail;
	}
	m_log.printf(LL_INFO, "DataBaseLayer On Init, ServerConfigPool::Init Success.");


	std::string fastdbname = "dbname";
	unsigned long flush_interval = 500;

	std::string dbusername = "root";
	std::string dbpassword = "root";
	std::string dbhost = "localhost";
	unsigned short dbport = 3306;
	std::string dbcharset = "utf8";
	std::string databasename = "conntest";
	unsigned int db_init_size_MB = 1 * (1023 - 1);

	int type = 0;
	int index = 0;
	config->SyncValue(ROOT/DATA_ACCESS_SERVER/"Type", &type, type);
	config->SyncValue(ROOT/DATA_ACCESS_SERVER/"Index", &index, index);

	m_log.printf(LL_MAINTANCE, "DBLayer type:%d index:%d.", type, index);

	std::string module_ip;
	if (!ServerConfigPool::Instance().GetLocalDBLConfig(type,index,fastdbname, db_init_size_MB, flush_interval,dbusername,dbpassword,dbhost,dbport,dbcharset,databasename, module_ip, m_rmi_port))
	{
		m_log.printf(LL_ERROR, "DataBaseLayer On Init, Read ServerConfigPool Config return FALSE.");

		this->StopServer();

		return Fail;
	}

	bool is_delay_load = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.IsDelayLoad;

	std::vector<std::string> not_cache_table_list;  // 不Cache的表 
	if (!ServerConfigPool::Instance().GetLocalDBNoCacheTables(not_cache_table_list))
	{
		m_log.printf(LL_MAINTANCE, "Read NoCache Table List Return FAIL!!!");

		config->write();

		this->StopServer();

		return Fail;
	}

	std::vector<std::string> whole_load_table_list;	// 全部预加载的表
	if (is_delay_load)
	{
		if (!ServerConfigPool::Instance().GetLocalDBNoDelayLoadTables(whole_load_table_list))
		{
			m_log.printf(LL_MAINTANCE, "Read NO Delay Load Table List Return FAIL!!!");

			config->write();

			this->StopServer();

			return Fail;
		}
	}

	if (this->CalcTableSizeIsTooLong())
	{
		m_log.printf(LL_MAINTANCE, "CalcTableSizeTooLongt Return FAIL!!!");

		config->write();

		this->StopServer();

		return Fail;
	}
	
	m_log.printf(LL_MAINTANCE, "host:%s:%d user_name:%s charset:%s database:%s", 
		dbhost.c_str(), dbport, dbusername.c_str(), dbcharset.c_str(), databasename.c_str());

	m_conn_factroy = new MysqlConnFactroy(dbusername.c_str(), dbpassword.c_str(), dbhost.c_str(), 
		dbport, dbcharset.c_str(), databasename.c_str());

	m_log.print(LL_MAINTANCE, "Create MysqlConnFactroy...");

	ConnPool::Instance()->Init(m_conn_factroy);
	
	size_t init_size_MB = (size_t)db_init_size_MB;

	std::string log_dir = "log/";
	config->SyncValue(ROOT/"LogDir", &log_dir, log_dir);
	if (log_dir[log_dir.length() - 1] != '/')
	{
		log_dir += '/';
	}

	m_log.print(LL_MAINTANCE, "Init DataBaseLayer...");
	if (!DataBaseLayer::Instance().Init(log, log_dir, fastdbname.c_str(), init_size_MB, is_delay_load, not_cache_table_list, whole_load_table_list, flush_interval))
	{
		m_log.printf(LL_MAINTANCE, "Init DataBaseLayer Fail!!!");

		this->StopServer();

		return IModule::Fail;
	}

	m_log.print(LL_MAINTANCE, "DataAccess Init suc.");

	return IModule::Succeed;
}

int DataAccess::Start()
{
	DEPEND_ON_START(INTERNAL_NETWORK)
	DEPEND_ON_START(GAME_MODULE)
	DEPEND_ON_START(RMI_MODULE)
	// DEPEND_ON_START(STD_COMMAND_MODULE)
	DEPEND_ON_START(CONFIG_MODULE)

	// IStdCommandModule *cmd_module = dynamic_cast<IStdCommandModule *>(Interface()->QueryModule(STD_COMMAND_MODULE));
	// cmd_module->RegisterCallback(m_cmd_callback);
	
	ILogModule *log = dynamic_cast<ILogModule *>(Interface()->QueryModule(LOG_MODULE));
	IConfigModule *config = dynamic_cast<IConfigModule *>(Interface()->QueryModule(CONFIG_MODULE));
	m_network = dynamic_cast<INetworkModule*>(Interface()->QueryModule(INTERNAL_NETWORK));
	m_game = dynamic_cast<IGameModule*>(Interface()->QueryModule(GAME_MODULE));
	m_rmi = dynamic_cast<IRMIModule*>(Interface()->QueryModule(RMI_MODULE));

	//m_rmi->StartAsyncThread(10);

	DARMIInit rmiinit;
	rmiinit.Init(m_rmi);

	std::string allow_ip_prefix = "127.0.0.1";
	if (config != 0)
	{
		config->SyncValue(ROOT/DATA_ACCESS_SERVER/"AllowIPPrefix", &allow_ip_prefix, allow_ip_prefix);
	}

	bool ret = m_rmi->StartServer(m_rmi_port);
	if (!ret)
	{
		m_log.printf(LL_MAINTANCE, "Start RMI Server[Port:%d] Fail!!!", (int)m_rmi_port);
		return IModule::Fail;
	}

	m_log.printf(LL_MAINTANCE, "Start RMI Server[Port:%d].", (int)m_rmi_port);
	
	// 获取配置目录
	std::string log_dir = "log/";
	if (config != 0)
	{
		config->SyncValue(ROOT/"LogDir", &log_dir, log_dir);
	}
	dataaccesslog::LogInit(log, log_dir);

	m_network_callback->SetAllowIpPrefix(allow_ip_prefix);
	m_network->RegisterCallback(m_network_callback);
	
	config->write();

	SysPath::CreateFileByPath("status/db.start");

	return IModule::Succeed;
}

int DataAccess::Update()
{
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	RoleCacheManager::Instance().OnUpdate(now_second);

	return IModule::Pending;
}

int DataAccess::Stop()
{
	return IModule::Succeed;
}

int DataAccess::Release()
{
	DataBaseLayer::Instance().Release();
	ConnPool::Instance()->Release();
	delete m_conn_factroy;

	m_log.printf(LL_MAINTANCE, "DataAccess::Release!!!");

	SysPath::CreateFileByPath("status/db.end");
	return IModule::Succeed;
}

//数据库现用引擎一个表不能超过8000多个字节, 一旦超过将会严重回档。所以加上下面的检查以防万一
bool DataAccess::CalcTableSizeIsTooLong()
{
	static int TABLE_MAX_DATA_SIZE_ON_UPDATE = 7900;
	static int TABLE_MAX_ROW_FIX_SIZE = 768;		// 768字节的BLOB存储限制

	{
		// 人物表1 请不要新加字段
		UNSTD_STATIC_CHECK(tabledef::TABLE_ROLE_ATTR_DETAIL_MAXINDEX == 66);
	}

	{
		// 人物表2 每次数量改变 改数组里的内容
		UNSTD_STATIC_CHECK(tabledef::TABLE_ROLE_ATTR_DETAIL2_MAXINDEX == 16);
		int size_list[tabledef::TABLE_ROLE_ATTR_DETAIL2_MAXINDEX] = {
			4,
			4,
			RoleOtherInitParam::ROLE_CHEST_SHOP_BUFF_LENGTH_HEX,
			RoleOtherInitParam::EFFECT_BUFFER_LENGTH_HEX,
			sizeof(EquipFBParamHex),
			sizeof(GuildTaskParamHex),
			sizeof(NewEquipFBParamHex),
			sizeof(StoneParamHex),
			sizeof(GuildXianshuParamHex),
			RoleOtherInitParam::MAX_TASK_RECORD_BUFF_LEN_HEX,
			sizeof(TaskParamHex),
			sizeof(RoleSupplyParamHex),
			sizeof(MazeParamHex),
			sizeof(XiannvParamHex),
			sizeof(StoryFBParamHex),
			sizeof(ConvertRecordParamHex),
		};

		int total_size = 0;
		for (int i = 0; i < tabledef::TABLE_ROLE_ATTR_DETAIL2_MAXINDEX; ++ i)
		{
			int field_size = size_list[i] > TABLE_MAX_ROW_FIX_SIZE ? TABLE_MAX_ROW_FIX_SIZE : size_list[i];
			total_size += field_size;
		}

		if (total_size > TABLE_MAX_DATA_SIZE_ON_UPDATE)
		{
			return true;
		}
	}
	
	{
		// 人物表3 每次数量改变 改数组里的内容
		UNSTD_STATIC_CHECK(tabledef::TABLE_ROLE_ATTR_DETAIL3_MAXINDEX == 19);
		int size_list[tabledef::TABLE_ROLE_ATTR_DETAIL3_MAXINDEX] = {
			4,
			4,
			sizeof(OtherModuleDataParamHex),
			sizeof(QingyuanParamHex),
			sizeof(RoleGoalParamHex),
			sizeof(HistoryCapParamHex),
			sizeof(SkillOtherParamHex),
			sizeof(RoleGuildParamHex),
			sizeof(ShengWangParamHex),
			sizeof(ChengJiuParamHex),
			sizeof(DailyFbParamHex),
			sizeof(LifeSkillParamHex),
			sizeof(BabyParamHex),
			sizeof(ShengeSystemParamHex),
			sizeof(ZeroGiftParamHex),
			sizeof(MysteriousShopInMallParamHex),
			sizeof(FriendExpBottleParamHex),
			sizeof(ShenshouParamHex),
			sizeof(ImgFulingParamHex),
		};
		
		int total_size = 0;
		for (int i = 0; i < tabledef::TABLE_ROLE_ATTR_DETAIL3_MAXINDEX; ++ i)
		{
			int field_size = size_list[i] > TABLE_MAX_ROW_FIX_SIZE ? TABLE_MAX_ROW_FIX_SIZE : size_list[i];
			total_size += field_size;
		}
		
		if (total_size > TABLE_MAX_DATA_SIZE_ON_UPDATE)
		{
			return true;
		}
	}

	{
		// 人物表4 每次数量改变 改数组里的内容
		UNSTD_STATIC_CHECK(17 == tabledef::TABLE_ROLE_ATTR_DETAIL4_MAXINDEX);
		int size_list[tabledef::TABLE_ROLE_ATTR_DETAIL4_MAXINDEX] = {
			4,
			4,
			sizeof(TuHaoJinParamHex),
			sizeof(BigChatFaceParamHex),
			sizeof(PetParamHex),
			sizeof(PhaseFbParamHex),
			sizeof(VipFBParamHex),
			sizeof(HuashenParamHex),
			sizeof(MagicCardParamHex),
			sizeof(MitamaParamHex),
			sizeof(EquipmentGridParamHex),
			sizeof(ChineseZodiacParamHex),
			sizeof(FriendGiftParamHex),
			sizeof(FollowBossParamHex),
			sizeof(GoldVipParamHex),
			sizeof(RuneSystemParamHex),
			sizeof(MagicalPreciousParamHex),
		};

		int total_size = 0;
		for (int i = 0; i < tabledef::TABLE_ROLE_ATTR_DETAIL4_MAXINDEX; ++ i)
		{
			int field_size = size_list[i] > TABLE_MAX_ROW_FIX_SIZE ? TABLE_MAX_ROW_FIX_SIZE : size_list[i];
			total_size += field_size;
		}

		if (total_size > TABLE_MAX_DATA_SIZE_ON_UPDATE)
		{
			return true;
		}
	}

	{
		// 人物表5 每次数量改变 改数组里的内容
		UNSTD_STATIC_CHECK(19 == tabledef::TABLE_ROLE_ATTR_DETAIL5_MAXINDEX);
		int param_size_list[tabledef::TABLE_ROLE_ATTR_DETAIL5_MAXINDEX] = {
			4,
			4,
			sizeof(MultiMountParamHex),
			sizeof(PersonalizeWindowParamHex),
			sizeof(XunZhangParamHex),
			sizeof(ZhiBaoParamHex),
			sizeof(JingLingParamHex),
			sizeof(XiannvShouhuParamHex),
			sizeof(JinglingFazhenParamHex),
			sizeof(JinglingGuanghuanParamHex),
			sizeof(ShenzhouWeaponParamHex),
			sizeof(LieMingSlotParamHex),
			sizeof(LieMingHunshouBagParamHex),
			sizeof(CardzuParamHex),
			sizeof(ZhuanShengEquipParamHex),
			sizeof(ShenBingParamHex),
			sizeof(TuituFbParamHex),
			sizeof(CloakParamHex),
			sizeof(PreciousBossParamHex),
		};

		int table_size = 0;
		for (int i = 0; i < tabledef::TABLE_ROLE_ATTR_DETAIL5_MAXINDEX; ++ i)
		{
			int field_size = param_size_list[i] > TABLE_MAX_ROW_FIX_SIZE ? TABLE_MAX_ROW_FIX_SIZE : param_size_list[i];
			table_size += field_size;
		}

		if (table_size > TABLE_MAX_DATA_SIZE_ON_UPDATE)
		{
			return true;
		}
	}

	{
		// 人物表6 每次数量改变 改数组里的内容
		UNSTD_STATIC_CHECK(15 == tabledef::TABLE_ROLE_ATTR_DETAIL6_MAXINDEX);
		int param_size_list[tabledef::TABLE_ROLE_ATTR_DETAIL6_MAXINDEX] = {
			4,
			4,
			sizeof(CoupleMountParamHex),
			sizeof(ShenYinParamHex),
			sizeof(ElementHeartParamHex),
			sizeof(ShengxingzhuliParamHex),
			sizeof(ShizhuangParamHex),
			sizeof(FabaoParamHex),
			sizeof(FeiXianParamHex),
			sizeof(TalentParamHex),
			sizeof(JinJieTalentParamHex),
			sizeof(ArmorDefendParamHex),
			sizeof(BossCardParamHex),
			sizeof(ZhuanzhiTaskParamHex),
			sizeof(DiscountBuyParamHex),
		};

		int table_size = 0;
		for (int i = 0; i < tabledef::TABLE_ROLE_ATTR_DETAIL6_MAXINDEX; ++i)
		{
			int field_size = param_size_list[i] > TABLE_MAX_ROW_FIX_SIZE ? TABLE_MAX_ROW_FIX_SIZE : param_size_list[i];
			table_size += field_size;
		}

		if (table_size > TABLE_MAX_DATA_SIZE_ON_UPDATE)
		{
			return true;
		}
	}

	{
		// 人物表7 每次数量改变 改数组里的内容
		UNSTD_STATIC_CHECK(12 == tabledef::TABLE_ROLE_ATTR_DETAIL7_MAXINDEX);
		int param_size_list[tabledef::TABLE_ROLE_ATTR_DETAIL7_MAXINDEX] = {
			4,
			4,
			sizeof(RoleCrossGlobalDataHex),
			sizeof(ZhuanzhiEquipParamHex),
			sizeof(RoleCrossParamHex),
			sizeof(HeShenLuoShuParamHex),
			sizeof(ShengqiParamHex),
			sizeof(SealParamHex),
			sizeof(DressingRoomParamHex),
			sizeof(ChallengeFBParamHex),
			sizeof(RoleCrossRandActivityParamHex),
			sizeof(CrossGoalParamHex)
		};

		int table_size = 0;
		for (int i = 0; i < tabledef::TABLE_ROLE_ATTR_DETAIL7_MAXINDEX; ++i)
		{
			int field_size = param_size_list[i] > TABLE_MAX_ROW_FIX_SIZE ? TABLE_MAX_ROW_FIX_SIZE : param_size_list[i];
			table_size += field_size;
		}

		if (table_size > TABLE_MAX_DATA_SIZE_ON_UPDATE)
		{
			return true;
		}
	}

	{
		// 人物表8 每次数量改变 改数组里的内容
		UNSTD_STATIC_CHECK(16 == tabledef::TABLE_ROLE_ATTR_DETAIL8_MAXINDEX);
		int param_size_list[tabledef::TABLE_ROLE_ATTR_DETAIL8_MAXINDEX] = {
			4,
			4,
			sizeof(UpgradeManagerParamHex),
			sizeof(ShenqiParamHex),
			sizeof(LieMingParam2Hex),
			sizeof(PersonProfessParamHex),
			sizeof(XianZunKaParamHex),
			sizeof(EquipBaptizeParamHex),
			sizeof(MojieParamHex),
			sizeof(PersonBossParamHex),
			sizeof(GreateSoldierParamHex),
			sizeof(RoleLotteryCountParamHex),
			sizeof(UpgradeCardBuyParamHex),
			sizeof(MythParamHex),
			sizeof(ZodiacParamHex),
			sizeof(CrossEquipParamHex),
		};

		int table_size = 0;
		for (int i = 0; i < tabledef::TABLE_ROLE_ATTR_DETAIL8_MAXINDEX; ++i)
		{
			int field_size = param_size_list[i] > TABLE_MAX_ROW_FIX_SIZE ? TABLE_MAX_ROW_FIX_SIZE : param_size_list[i];
			table_size += field_size;
		}

		if (table_size > TABLE_MAX_DATA_SIZE_ON_UPDATE)
		{
			return true;
		}
	}

	{
		// 人物表9 每次数量改变 改数组里的内容
		UNSTD_STATIC_CHECK(16 == tabledef::TABLE_ROLE_ATTR_DETAIL9_MAXINDEX);
		int param_size_list[tabledef::TABLE_ROLE_ATTR_DETAIL9_MAXINDEX] = {
			4,
			4,
			sizeof(RedEquipCollectParamHex),
			sizeof(MountParamHex),
			sizeof(FootprintParamHex),
			sizeof(HaloParamHex),
			sizeof(HuguozhiliParamHex),
			sizeof(BaizhanEquipParamHex),
			sizeof(FightMountParamHex),
			sizeof(MojieFBParamHex),
			sizeof(EquipSuitParamHex),
			sizeof(ShenZhuangParamHex),
			sizeof(CSAEquipParamHex),
			sizeof(ShengongParamHex),
			sizeof(ShenyiParamHex),
			sizeof(DisCountShopParamHex),
		};

		int table_size = 0;
		for (int i = 0; i < tabledef::TABLE_ROLE_ATTR_DETAIL9_MAXINDEX; ++i)
		{
			int field_size = param_size_list[i] > TABLE_MAX_ROW_FIX_SIZE ? TABLE_MAX_ROW_FIX_SIZE : param_size_list[i];
			table_size += field_size;
		}

		if (table_size > TABLE_MAX_DATA_SIZE_ON_UPDATE)
		{
			return true;
		}
	}

	{
		// 人物表10 每次数量改变 改数组里的内容
		UNSTD_STATIC_CHECK(11 == tabledef::TABLE_ROLE_ATTR_DETAIL10_MAXINDEX);
		int param_size_list[tabledef::TABLE_ROLE_ATTR_DETAIL10_MAXINDEX] = {
			4,
			4,
			sizeof(OrangeEquipCollectParamHex),
			sizeof(YaoShiParamHex),
			sizeof(TouShiParamHex),
			sizeof(QiLinBiParamHex),
			sizeof(MaskParamHex),
			sizeof(JinjieSysRewardParamHex),
			sizeof(RoleBigSmallGoalRewardParamHex),
			//BLOB_LEN + BLOB
		};

		int table_size = 0;
		for (int i = 0; i < tabledef::TABLE_ROLE_ATTR_DETAIL10_MAXINDEX; ++i)
		{
			int field_size = param_size_list[i] > TABLE_MAX_ROW_FIX_SIZE ? TABLE_MAX_ROW_FIX_SIZE : param_size_list[i];
			table_size += field_size;
		}

		if (table_size > TABLE_MAX_DATA_SIZE_ON_UPDATE)
		{
			return true;
		}
	}

	return false;
}

