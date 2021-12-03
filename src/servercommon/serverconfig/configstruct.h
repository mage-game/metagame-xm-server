#ifndef __CONFIGSTRUCT_H__
#define __CONFIGSTRUCT_H__

#include <string>

#include <servercommon/configcommon.h>

// 地址结构
class NetAddr:public ConfigElementBase
{
public:
	NetAddr(bool show_ip = true) : ip("127.0.0.1"), port(7777),is_show_ip(show_ip) {}
	virtual ~NetAddr(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	std::string ip;							// ip地址
	unsigned short port;					// 端口
	bool is_show_ip;
};

// 数据库配置结构
class DBConfig:public ConfigElementBase
{
public:
	DBConfig():fast_db_name("dbname")
		  , fast_db_init_size_MB(1024)
		  ,mysql_db_username("root")
		  ,mysql_db_password("root")
		  ,mysql_addr()
		  ,mysql_db_charset("utf8")
		  ,mysql_db_name("mmoarpg")
		  ,flushinterval(500) {}
	virtual ~DBConfig(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	std::string fast_db_name;				// fast db名
	unsigned int fast_db_init_size_MB;			// fast db 最大使用共享内存大小
	std::string mysql_db_username;			// 数据库用户名
	std::string mysql_db_password;			// 数据库密码
	NetAddr mysql_addr;						// ip和端口
	std::string mysql_db_charset;			// 数据库连接字符集
	std::string mysql_db_name;				// 数据库名
	unsigned short flushinterval;			// 刷入数据库时间间隔
};

// 数据层配置单元
class DBModuleAddr:public ConfigElementBase
{
public:
	DBModuleAddr(bool show_index=true):index(0),db_config(),module_addr(),is_show_index(show_index){}
	virtual ~DBModuleAddr(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	unsigned short index;					// 用于标识多数据库模块的顺序下标
	DBConfig db_config;
	NetAddr module_addr;
	bool is_show_index;
};

// 数据表名列表
class TableNameList:public ConfigElementBase
{
public:
	TableNameList():TableCount(1)
	{
		TableNames[0] = "no_cache_table_name_0";
	}
	virtual ~TableNameList(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	const static unsigned short MaxTableNameCount = 64;
	int TableCount;

	std::string TableNames[MaxTableNameCount];				// 数据表名
};

// 下标列表
class IndexList:public ConfigElementBase
{
public:
	IndexList():IndexCount(1)
	{
		memset(Indexs, 0, sizeof(Indexs));
	}
	virtual ~IndexList(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	const static unsigned short MAX_INDEX_COUNT = 64;
	int IndexCount;

	int Indexs[MAX_INDEX_COUNT];
};

// 数据层服务器地址
class DBServerAddr:public ConfigElementBase
{
public:
	DBServerAddr():DBRoleCount(1),NoCacheTables(),IsDelayLoad(true), NoDelayTables(),
		DBAccounter(false), DBName(false), DBGlobal(false), DBSystem(false) {}
	virtual ~DBServerAddr(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();
	void check();

	const static unsigned short MaxDBRoleCount = 32;

	enum LOCALDBLAYERTYPE
	{
		LDBLT_INVALID = -1,
		LDBLT_ACCOUNTER,	// 帐号数据库
		LDBLT_GLOBAL,		// 线数据库
		LDBLT_ROLE,			// 角色数据库
		LDBLT_SYSTEM,		// 系统数据库
		LDBLT_NAME,			// 名字数据库
		LDBLT_CROSS,		// 跨服数据库
		LDBLT_COUNT,
	};

	int DBRoleCount;
	TableNameList NoCacheTables;								// 不缓存数据表名列表
	bool IsDelayLoad;											// 是否延迟加载
	TableNameList NoDelayTables;								// 不延迟加载的数据表名
	DBModuleAddr DBAccounter;									// 帐号数据库
	DBModuleAddr DBName;										// 名字索引数据库
	DBModuleAddr DBGlobal;
	DBModuleAddr DBSystem;										// 管理数据库
	IndexList DBIndexCreateRole;								// 用于创建角色的数据库下标
	DBModuleAddr DBRoleList[MaxDBRoleCount];					// 角色数据库
	DBModuleAddr DBCross;										// 跨服数据库
};

// 排行榜服务器地址
class RankServerAddr:public ConfigElementBase
{
public:
	RankServerAddr():addr(){}
	virtual ~RankServerAddr(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	NetAddr addr;
};

// 中心服务器地址
class CenterServerAddr:public ConfigElementBase
{
public:
	CenterServerAddr():addr(){}
	virtual ~CenterServerAddr(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	NetAddr addr;
};

// 登录服务器的内部连接地址
class LoginServerInnerAddr:public ConfigElementBase
{
public:
	LoginServerInnerAddr():server_ip("127.0.0.1"),for_global(false),for_gameserver(false),for_gateway(false){}
	virtual ~LoginServerInnerAddr(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	std::string server_ip;								// 登录服务器的ip地址
	NetAddr for_global;
	NetAddr for_gameserver;
	NetAddr for_gateway;
};


// 登录服务器地址
class LoginServerAddr:public ConfigElementBase
{
public:
	LoginServerAddr():inner_connect_addr(),user_connect_addr(false){}
	virtual ~LoginServerAddr(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	LoginServerInnerAddr inner_connect_addr;			// 监听内部服务器
	NetAddr user_connect_addr;							// 监听客户端
};


// 各个服务的共享ip配置(包括数据库连接配置)
class IPConfig:public ConfigElementBase
{
public:
	IPConfig():db_server_addr(),rank_server_addr(),center_server_addr(),login_server_addr(),m_document(){}
	virtual ~IPConfig(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	DBServerAddr db_server_addr;
	RankServerAddr rank_server_addr;
	CenterServerAddr center_server_addr;
	LoginServerAddr login_server_addr;

protected:
	TiXmlDocument m_document;
};

// 所有服务器的超时配置
class ServerTimeOut: public ConfigElementBase
{
public:
	ServerTimeOut():is_check_timeout(true),check_timeout_interval_ms(1000),
		login_server_heartbeat_dead_time_ms(1000),gateway_heartbeat_dead_time_ms(1000),gameworld_heartbeat_dead_time_ms(1000),
		login_server_max_server_time_ms(1000){}
	virtual ~ServerTimeOut(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	bool is_check_timeout;
	unsigned int check_timeout_interval_ms;

	unsigned int login_server_heartbeat_dead_time_ms;
	unsigned int gateway_heartbeat_dead_time_ms;
	unsigned int gameworld_heartbeat_dead_time_ms;

	unsigned int login_server_max_server_time_ms;
};


class ChatChannelCD : public ConfigElementBase
{
public:

	ChatChannelCD():world_cd(30000), country_cd(15000){}
	virtual ~ChatChannelCD(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	unsigned int world_cd;
	unsigned int country_cd;
};

class WallowConfig : public ConfigElementBase
{
public:
	WallowConfig():server_check(true), check_repeate(true){}
	virtual ~WallowConfig(){}

	virtual ConfigElementBase *offset() { return this + 1; }

	virtual bool read();
	virtual bool write();

	bool server_check;					// 服务器是否进行反沉迷验证
	bool check_repeate;					// 一个身份证号是否允许验证多个帐号
	int timer_interval;					// 统计间隔时间
	int kick_time;						// 在线多久后踢下线
	int notice_ahead_of_time;			// 踢出前多长时间通知
	int one_hour_notice_time;			// 第一次防沉迷通知的时间(1小时)
	int two_hour_notice_time;			// 第二次防沉迷通知的时间(2小时)
	int anti_wallow_rest_time;			// 禁止登陆时间持续时间
	bool is_open_anti_wallow;			// 可动态改变的防沉迷开关
};

class CommonConfig : public ConfigElementBase
{
public:
	CommonConfig() : ipconfigs(),
		sessionkey("123456"),global_server_maxlogininterval_s(1000),login_server_minlogininterval_s(1),login_server_cannotloginglobaltime_s(1),
		login_server_rolenumrecordinterval_s(60), login_server_account_check_type(0), game_world_scene_status_log_interval_s(60), system_cmd_check_interval_s(15), 
		youke_anti_wallow(1), timeout(), namefilter_open_flag(true), is_micro_pc_open(false) {}
	virtual ~CommonConfig() {}

	virtual ConfigElementBase *offset() { return this + 1;}

	bool LoadConfig(const char* filename,char *err_info);

	virtual bool read();
	virtual bool write();

	IPConfig ipconfigs;

	std::string sessionkey;

	unsigned int global_server_maxlogininterval_s;
	unsigned int login_server_minlogininterval_s;
	unsigned int login_server_cannotloginglobaltime_s;
	unsigned int login_server_rolenumrecordinterval_s;

	enum CHECK_TYPE
	{
		PLAT_TYPE_YOUKE = 0,	// 游客
		PLAT_TYPE_360,			// 360
		PLAT_TYPE_37W,			// 37玩
		PLAT_TYPE_YYG,			// YY
		PLAT_TYPE_GTS,			// 台湾tt
		PLAT_TYPE_YYB,			// 应用宝
		PLAT_TYPE_COUNT,	
	};

	unsigned int login_server_account_check_type;

	unsigned int game_world_scene_status_log_interval_s;
	unsigned int system_cmd_check_interval_s;
	unsigned int youke_anti_wallow;
	ServerTimeOut timeout;
	ChatChannelCD chat_channel_cd;
	WallowConfig wallow_config;
	std::string dynamic_cfg_path;
	bool namefilter_open_flag;
	bool is_micro_pc_open;

protected:
	TiXmlDocument m_document;
};

#endif // __CONFIGSTRUCT_H__

