#ifndef _USERCACHE_HPP_
#define _USERCACHE_HPP_

#include "servercommon/servercommon.h"
#include "servercommon/serverdef.h"
#include "servercommon/serverconfig/configstruct.h"
#include "servercommon/mysqlutility.hpp"

#include <map>
#include <string>

class Role;
struct UserCacheNode;

typedef bool (UserCacheFilterFunc)(const UserCacheNode &user_node, void **func_param);

struct UserCacheNode
{
	UserCacheNode() :uuid(0), uid(0), sex(0), prof(0), camp(0), vip_level(0), level(0), capability(0), avatar_timestamp(0), last_logout_timestamp(0),
		hunjie_item_id(0), hunjie_star_level(1), hunyan_buy_fb_join_times(0), last_buy_hunyan_fb_times_dayid(0), cur_use_avatar_type(0), cur_use_bubble_type(0), lover_id(0), qingyuan_cap(0),
		baby_cap(0), little_pet_cap(0),shenbing_cur_use_image(0),baojia_cur_use_image(0), limit_vip_time(0), guild_id(0), has_xianzunka_flag(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	void GetName(GameName name) const
	{
		if (NULL != name)
		{
			F_STRNCPY(name, user_name, sizeof(GameName));
		}
	}

	long long uuid;
	int uid;
	GameName user_name;
	char sex;
	char prof;
	char camp;
	char vip_level;
	short level;
	int capability;
	long long avatar_timestamp;
	unsigned int last_logout_timestamp;
	ItemID hunjie_item_id;
	short hunjie_star_level;
	char hunyan_buy_fb_join_times;
	unsigned int last_buy_hunyan_fb_times_dayid;

	short cur_use_avatar_type;				// 当前使用的头像框
	short cur_use_bubble_type;				// 当前使用的聊天框

	int lover_id;
	int qingyuan_cap;						// 情缘战力
	int baby_cap;							// 宝宝战力
	int little_pet_cap;						// 小宠物战力

	char shenbing_cur_use_image;			// 神兵当前使用id
	char baojia_cur_use_image;				// 宝甲当前使用id

	unsigned int limit_vip_time;			// VIP过期时间

	GuildID guild_id;						//工会id
	short has_xianzunka_flag;				//仙尊卡拥有标识（包括临时和永久）
};

struct UserStatusNode
{
	UserStatusNode() : uid(0), is_online(false) {}

	int uid;
	bool is_online;
};

class UserCacheManager
{
public:
	static UserCacheManager & Instance();

	bool LoadAllUser(TMysqlStatement &stMysqlStatement);

	UserCacheNode * GetUserNode(const UserID &user_id, bool trace_nonexist = true);
	UserCacheNode * GetUserNode(int uid, bool trace_nonexist = true);
	UserCacheNode * GetUserNodeByName(std::string user_name, bool *is_online, bool trace_nonexist = true);
	int RandomGetUidList(int *uid_list, int uid_list_len);
	void UpdateCacheNode(UserCacheNode &node);
	void GetUserName(const UserID &user_id, GameName user_name);
	void GetUserName(int uid, GameName user_name);
	
	void UpdateUserNameCache(std::string user_name, int uid, bool is_online);
	void UpdateUserNameCache(Role *role, bool is_online);
	void UpdateAvatarType(int uid, short avatar_type);
	void UpdateBubbleType(int uid, short bubble_type);
	void UpdateGuildId(int uid, GuildID guild_id);
	bool IsUserOnline(int uid);
	bool IsUserOnline(UserID user_id);

	int CountOfUser();

	void OnResetName(Role *role, GameName old_name);

	int GetUserIDList(int &from, int user_count, UserID *user_list, UserCacheFilterFunc filter_func = NULL, void **func_param = NULL);
	const std::map<int, UserCacheNode> & GetUserCacheMap() { return m_user_node_map; }

	int GetRoleNumByLevel(int min_lv, int max_lv);

private:
	UserCacheManager();
	~UserCacheManager();

	std::map<int, UserCacheNode> m_user_node_map;
	std::map<std::string, UserStatusNode> m_name_uid_map;
	std::map<int, UserStatusNode> m_uid_status_map;
};

#endif

