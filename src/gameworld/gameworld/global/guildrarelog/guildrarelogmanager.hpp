#ifndef __GUILD_RARE_LOG_HPP__
#define __GUILD_RARE_LOG_HPP__

#include "gameworld/engineadapter.h"
#include "servercommon/serverdef.h"

static const int GUILD_RARE_LOG_MAX_NUM = 100;

struct GuildRareLogItem
{
	GuildRareLogItem() : uuid(0), item_id(0), item_num(0), is_from_gift(0), gift_item_id(0), timestamp(0), scene_id(0), monster_id(0)
	{
		memset(role_name, 0, sizeof(GameName));
		memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
	}

	long long uuid;
	GameName role_name;
	ItemID item_id;
	short item_num;
	short is_from_gift;
	ItemID gift_item_id;
	unsigned int timestamp;
	int	scene_id;
	int monster_id;

	unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];	// 仙品属性类型
};

class GuildRareLogManager
{
	using GuildRareLogItemDeque = std::deque<GuildRareLogItem>;
	using GuildRareLogItemMap = std::map<GuildID, GuildRareLogItemDeque>;

public:
	GuildRareLogManager();
	~GuildRareLogManager();

	static GuildRareLogManager & Instance();

	void AddLog(GuildID guild_id, GuildRareLogItem item);
	void AddLog(GuildID guild_id, int uid, const GameName role_name, ItemID item_id, int item_num, int scene_id, int monster_id);
	GuildRareLogItemDeque * GetLog(GuildID guild_id);
	void SendAllLog(Role *role);
	void SendOneLog(GuildID guild_id, GuildRareLogItem temp_item);

	void SendOneLogToRole(Role *role, GuildRareLogItem item);

protected:

	GuildRareLogItemMap m_log_map;			// 日志列表

};

#endif // __GUILD_RARE_LOG_HPP__

