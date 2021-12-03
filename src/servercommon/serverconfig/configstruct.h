#ifndef __CONFIGSTRUCT_H__
#define __CONFIGSTRUCT_H__

#include <string>

#include <servercommon/configcommon.h>

// ��ַ�ṹ
class NetAddr:public ConfigElementBase
{
public:
	NetAddr(bool show_ip = true) : ip("127.0.0.1"), port(7777),is_show_ip(show_ip) {}
	virtual ~NetAddr(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	std::string ip;							// ip��ַ
	unsigned short port;					// �˿�
	bool is_show_ip;
};

// ���ݿ����ýṹ
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

	std::string fast_db_name;				// fast db��
	unsigned int fast_db_init_size_MB;			// fast db ���ʹ�ù����ڴ��С
	std::string mysql_db_username;			// ���ݿ��û���
	std::string mysql_db_password;			// ���ݿ�����
	NetAddr mysql_addr;						// ip�Ͷ˿�
	std::string mysql_db_charset;			// ���ݿ������ַ���
	std::string mysql_db_name;				// ���ݿ���
	unsigned short flushinterval;			// ˢ�����ݿ�ʱ����
};

// ���ݲ����õ�Ԫ
class DBModuleAddr:public ConfigElementBase
{
public:
	DBModuleAddr(bool show_index=true):index(0),db_config(),module_addr(),is_show_index(show_index){}
	virtual ~DBModuleAddr(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	unsigned short index;					// ���ڱ�ʶ�����ݿ�ģ���˳���±�
	DBConfig db_config;
	NetAddr module_addr;
	bool is_show_index;
};

// ���ݱ����б�
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

	std::string TableNames[MaxTableNameCount];				// ���ݱ���
};

// �±��б�
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

// ���ݲ��������ַ
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
		LDBLT_ACCOUNTER,	// �ʺ����ݿ�
		LDBLT_GLOBAL,		// �����ݿ�
		LDBLT_ROLE,			// ��ɫ���ݿ�
		LDBLT_SYSTEM,		// ϵͳ���ݿ�
		LDBLT_NAME,			// �������ݿ�
		LDBLT_CROSS,		// ������ݿ�
		LDBLT_COUNT,
	};

	int DBRoleCount;
	TableNameList NoCacheTables;								// ���������ݱ����б�
	bool IsDelayLoad;											// �Ƿ��ӳټ���
	TableNameList NoDelayTables;								// ���ӳټ��ص����ݱ���
	DBModuleAddr DBAccounter;									// �ʺ����ݿ�
	DBModuleAddr DBName;										// �����������ݿ�
	DBModuleAddr DBGlobal;
	DBModuleAddr DBSystem;										// �������ݿ�
	IndexList DBIndexCreateRole;								// ���ڴ�����ɫ�����ݿ��±�
	DBModuleAddr DBRoleList[MaxDBRoleCount];					// ��ɫ���ݿ�
	DBModuleAddr DBCross;										// ������ݿ�
};

// ���а��������ַ
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

// ���ķ�������ַ
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

// ��¼���������ڲ����ӵ�ַ
class LoginServerInnerAddr:public ConfigElementBase
{
public:
	LoginServerInnerAddr():server_ip("127.0.0.1"),for_global(false),for_gameserver(false),for_gateway(false){}
	virtual ~LoginServerInnerAddr(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	std::string server_ip;								// ��¼��������ip��ַ
	NetAddr for_global;
	NetAddr for_gameserver;
	NetAddr for_gateway;
};


// ��¼��������ַ
class LoginServerAddr:public ConfigElementBase
{
public:
	LoginServerAddr():inner_connect_addr(),user_connect_addr(false){}
	virtual ~LoginServerAddr(){}

	virtual ConfigElementBase *offset() { return this + 1;}

	virtual bool read();
	virtual bool write();

	LoginServerInnerAddr inner_connect_addr;			// �����ڲ�������
	NetAddr user_connect_addr;							// �����ͻ���
};


// ��������Ĺ���ip����(�������ݿ���������)
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

// ���з������ĳ�ʱ����
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

	bool server_check;					// �������Ƿ���з�������֤
	bool check_repeate;					// һ�����֤���Ƿ�������֤����ʺ�
	int timer_interval;					// ͳ�Ƽ��ʱ��
	int kick_time;						// ���߶�ú�������
	int notice_ahead_of_time;			// �߳�ǰ�೤ʱ��֪ͨ
	int one_hour_notice_time;			// ��һ�η�����֪ͨ��ʱ��(1Сʱ)
	int two_hour_notice_time;			// �ڶ��η�����֪ͨ��ʱ��(2Сʱ)
	int anti_wallow_rest_time;			// ��ֹ��½ʱ�����ʱ��
	bool is_open_anti_wallow;			// �ɶ�̬�ı�ķ����Կ���
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
		PLAT_TYPE_YOUKE = 0,	// �ο�
		PLAT_TYPE_360,			// 360
		PLAT_TYPE_37W,			// 37��
		PLAT_TYPE_YYG,			// YY
		PLAT_TYPE_GTS,			// ̨��tt
		PLAT_TYPE_YYB,			// Ӧ�ñ�
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

