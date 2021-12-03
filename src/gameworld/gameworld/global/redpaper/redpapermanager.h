#ifndef __RED_PAPER_MANAGER_H__
#define __RED_PAPER_MANAGER_H__

class Role;

#include <map>
#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"

enum RED_PAPER_TYPE
{
	RED_PAPER_TYPE_INVALID = 0,
	RED_PAPER_TYPE_COMMON,
	RED_PAPER_TYPE_RAND,
	RED_PAPER_TYPE_GLOBAL,
	RED_PAPER_TYPE_GUILD_PAPER,
	RED_PAPER_TYPE_COMMAND_SPEAKER,

	RED_PAPER_TYPE_MAX,
};

const static int RED_PAPER_MAX_VALID_TIME_S = 30 * 60;
const static int RED_PAPER_MAX_CAN_FETCH_TIMES = 100;
const static int RED_PAPER_ONCE_MAX_GOLD_NUM = 200;
const static int RED_PAPER_MAX_GOLD_NUM = 1000;
const static int RED_PAPER_COMMAND_CONTEXT_MAX_LENGTH = 512;

struct RedPaper
{
	RedPaper() : id(0), type(RED_PAPER_TYPE_INVALID), is_bind_gold(1), total_gold_num(0), fetch_gold_num(0), can_fetch_times(0),
		timeount_timestamp(0), creater_uid(0), creater_guild_id(0), creater_avator_timestamp(0), creater_sex(0), creater_prof(0), reserve(0),
		boss_id(0)
	{
		memset(creater_name, 0, sizeof(creater_name));
		memset(command_context, 0, sizeof(command_context));
	}

	struct UserInfo
	{
		UserInfo() : gold_num(0), avator_timestamp(0)
		{
			memset(user_name, 0, sizeof(user_name));
		}

		int gold_num;
		long long avator_timestamp;
		GameName user_name;
	};

	int id;
	int type;
	int is_bind_gold;
	int total_gold_num;
	int fetch_gold_num;
	int can_fetch_times;
	time_t timeount_timestamp;

	int creater_uid;
	GameName creater_name;
	int creater_guild_id;
	long long creater_avator_timestamp;
	char creater_sex;
	char creater_prof;
	short reserve;
	int boss_id;

	char command_context[RED_PAPER_COMMAND_CONTEXT_MAX_LENGTH];

	std::map<int, UserInfo> fetch_user_map;
};

class RedPaperManager
{
public:
	static RedPaperManager & Instance();

	int OnCreateRedPaper(Role *role, int red_paper_type, int gold_num, int can_fetch_times, bool is_system_provide, int is_bind_gold = 1, int boss_id = 0);
	int OnCreateSceneRedPaper(Role *role, int red_paper_type,int gold_num, int can_fetch_times, bool is_system_provide, int is_bind_gold = 1, int boss_id = 0);
	
	int OnCreateCommandRedPaper(Role *role, void* msg, int length);
	void OnFecthCommandRedPaper(Role *role, int red_paper_id);
	void OnCheckCommandRedPaper(Role *role, int red_paper_id);

	void OnFetchRedPaper(Role *role, int red_paper_id);
	void OnQueryRedPaperDetailInfo(Role *role, int red_paper_id);
	
	void Update(unsigned long interval, time_t now_second);
	void OnStopServer();

	int CreateRedPaperID() { return ++ m_red_paper_id; }

	void SendRedPaperRoleInfo(Role *role);

	std::map<int, RedPaper> & GetRedPaperMap() { return m_red_paper_map; }

private:
	RedPaperManager();
	~RedPaperManager();

	void SendRedPaperDetailInfo(Role *role, int guild_id, int red_paper_id, int notify_reason = 0, bool is_scene = false);
	void CheckInvalidRedPaper(time_t now_second);

	typedef std::map<int, RedPaper> RedPaperMap;
	typedef std::map<int, RedPaper>::iterator RedPaperMapIt;

	RedPaperMap m_red_paper_map;

	int m_red_paper_id;

	const static int CHECK_INVALID_RED_PAPER_INTERVAL = 2;

	time_t m_last_check_invalid_red_paper_timestamp;
};

#endif

