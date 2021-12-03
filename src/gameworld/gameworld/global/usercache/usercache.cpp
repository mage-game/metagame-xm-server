#include "obj/character/role.h"
#include "servercommon/commondata.hpp"
#include "global/team/teammanager.hpp"
#include "world.h"
#include "usercache.hpp"
#include "gamelog.h"
#include "common/bintohex.h"
#include "friend/friendmanager.h"
#include "friend/teaminfo.hpp"
#include "protocal/msgrole.h"
#include "servercommon/serverconfig/crossconfig.hpp"

UserCacheManager::UserCacheManager()
{

}

UserCacheManager::~UserCacheManager()
{

}

UserCacheManager & UserCacheManager::Instance()
{
	static UserCacheManager ws;
	return ws;
}

bool UserCacheManager::LoadAllUser(TMysqlStatement &stMysqlStatement)
{
	char szErrMsg[512];
	int iRet = 0;

	int iErrno;
	stMysqlStatement.iQueryType = SQL_TYPE_SELECT;
	strncpy(stMysqlStatement.szSQLString, 
		"SELECT `role_id`, `role_name`, `sex`, `prof`, `camp`, `level`, `vip_type`, `capability`, `avatar_timestamp`, `common_data`, `vip_data`, `guild_id` FROM `role_attr_detail` WHERE `role_id` > 0",
		sizeof(stMysqlStatement.szSQLString) - 1);
	
	iRet = CMysqlUtility::ExecSelect(&stMysqlStatement, &iErrno, szErrMsg);
	if(0 != iRet)
	{
		CMysqlUtility::FreeResult(&stMysqlStatement);

		gamelog::g_log_world.printf(LL_MAINTANCE, "UserCacheManager::LoadAllUser select failed with :%d errmsg:%s",
			iErrno, szErrMsg);

		return false;
	}

	int iFetchRows = stMysqlStatement.iResNum;

	enum field
	{
		field_uid = 0,
		field_name,
		field_sex,
		field_prof,
		field_camp,
		field_level,
		field_viplevel,
		field_capability,
		field_avatar_timestamp,
		field_common_data,
		field_vip_data,
		field_guild_id,
	};

	for (int i = 0; i < iFetchRows; ++i)
	{
		iRet = CMysqlUtility::FetchRow(&stMysqlStatement, szErrMsg);
		if(0 != iRet)
		{
			gamelog::g_log_world.printf(LL_MAINTANCE, "UserCacheManager::LoadAllUser when fetched data has error :%s,index:%d",
				szErrMsg, i);

			break;
		}

		UserCacheNode user_node;

		if(stMysqlStatement.stRow[field_uid])
		{
			user_node.uid = atoi(stMysqlStatement.stRow[field_uid]);
		}

		if(stMysqlStatement.stRow[field_name] && stMysqlStatement.alFieldsLength[field_name] > 0)
		{
			const char* user_name = stMysqlStatement.stRow[field_name];
			SNPRINTF(user_node.user_name, sizeof(user_node.user_name) - 1, "%s", user_name);
		}

		if(stMysqlStatement.stRow[field_sex])
		{
			user_node.sex = (char)atoi(stMysqlStatement.stRow[field_sex]);
		}

		if(stMysqlStatement.stRow[field_prof])
		{
			user_node.prof = (char)atoi(stMysqlStatement.stRow[field_prof]);
		}

		if(stMysqlStatement.stRow[field_camp])
		{
			user_node.camp = (char)atoi(stMysqlStatement.stRow[field_camp]);
		}

		if(stMysqlStatement.stRow[field_level])
		{
			user_node.level = atoi(stMysqlStatement.stRow[field_level]);
		}

		if(stMysqlStatement.stRow[field_viplevel])
		{
			user_node.vip_level = (char)atoi(stMysqlStatement.stRow[field_viplevel]);
		}

		if(stMysqlStatement.stRow[field_capability])
		{
			user_node.capability = atoi(stMysqlStatement.stRow[field_capability]);
		}

		if(stMysqlStatement.stRow[field_avatar_timestamp])
		{
			#ifdef _WIN32
				sscanf(stMysqlStatement.stRow[field_avatar_timestamp], "%lld", &user_node.avatar_timestamp);
			#else	
				user_node.avatar_timestamp = atoll(stMysqlStatement.stRow[field_avatar_timestamp]);
			#endif
		}

		if (stMysqlStatement.stRow[field_common_data] && stMysqlStatement.alFieldsLength[field_common_data] > 0)
		{
			unsigned long field_len = stMysqlStatement.alFieldsLength[field_common_data];
			if (field_len > sizeof(CommonDataParam) * 2)
			{
				return false;
			}

			static CommonDataParam commom_data_param; commom_data_param.Reset();
			HexToBin(stMysqlStatement.stRow[field_common_data], field_len, (char *)&commom_data_param);
			user_node.last_logout_timestamp = commom_data_param.last_logout_timestamp;
			user_node.hunjie_item_id = commom_data_param.hunjie_item_id;
			user_node.hunjie_star_level = commom_data_param.hunjie_star_level;
			user_node.hunyan_buy_fb_join_times = commom_data_param.hunyan_buy_fb_join_times;
			user_node.last_buy_hunyan_fb_times_dayid = commom_data_param.last_buy_hunyan_fb_times_dayid;
			user_node.cur_use_avatar_type = commom_data_param.cur_use_avatar_type;
			user_node.cur_use_bubble_type = commom_data_param.cur_use_bubble_type;
			user_node.lover_id = commom_data_param.lover_uid;
			user_node.qingyuan_cap = commom_data_param.qingyuan_capablity;
			user_node.baby_cap = commom_data_param.baby_capablity;
			user_node.little_pet_cap = commom_data_param.little_pet_capablity;
			user_node.shenbing_cur_use_image = commom_data_param.shenbing_use_image_id;
			user_node.baojia_cur_use_image = commom_data_param.baojia_use_image_id;
			user_node.uuid= commom_data_param.uuid;
			user_node.has_xianzunka_flag = commom_data_param.has_xianzunka_flag;
		}

		if (stMysqlStatement.stRow[field_vip_data] && stMysqlStatement.alFieldsLength[field_vip_data] > 0)
		{
			unsigned long field_len = stMysqlStatement.alFieldsLength[field_vip_data];
			if (field_len > sizeof(VipParam) * 2)
			{
				return false;
			}

			static VipParam vip_param; vip_param.Reset();
			HexToBin(stMysqlStatement.stRow[field_vip_data], field_len, (char *)&vip_param);
			user_node.limit_vip_time = vip_param.time_limit_vip_time;
		}

		if (stMysqlStatement.stRow[field_guild_id])
		{
			user_node.guild_id = atoi(stMysqlStatement.stRow[field_guild_id]);
		}

		this->UpdateCacheNode(user_node);
		this->UpdateUserNameCache(user_node.user_name, user_node.uid, false);
	}

	CMysqlUtility::FreeResult(&stMysqlStatement);

	if (0 != iRet)
	{
		return false;
	}

	gamelog::g_log_world.printf(LL_MAINTANCE, "UserCacheManager::LoadAllUser total user count:%d",
		this->CountOfUser());

	return true;
}

UserCacheNode * UserCacheManager::GetUserNode(const UserID &user_id, bool trace_nonexist)
{
	return this->GetUserNode(UidToInt(user_id), trace_nonexist);
}

UserCacheNode * UserCacheManager::GetUserNode(int uid, bool trace_nonexist)
{
	//if (CrossConfig::Instance().IsHiddenServer())
	//{
	//	return NULL;
	//}

	if (uid == UidToInt(INVALID_USER_ID) || uid == UidToInt(VIRTUAL_USER_ID))
	{
		return nullptr;
	}

	std::map<int, UserCacheNode>::iterator iter = m_user_node_map.find(uid);
	if (iter != m_user_node_map.end())
	{
		return &(iter->second);
	}

	if (trace_nonexist)
	{
		gamelog::g_log_serious_error.printf(LL_INFO, "UserCache no uid:%d", uid);
	}

	return NULL;
}

int UserCacheManager::RandomGetUidList(int *uid_list, int uid_list_len)
{
	if (NULL == uid_list || m_user_node_map.empty())
	{
		return 0;
	}

	std::set<int> uid_repeat_checker;

	int random_count = 0, uid_count = 0;
	while (random_count++ < 1000 && uid_count < uid_list_len)
	{
		int rand_index = static_cast<int>(RandomNum(m_user_node_map.size()));

		std::map<int, UserCacheNode>::iterator it = m_user_node_map.begin();
		std::advance(it, rand_index);

		if (!uid_repeat_checker.insert(it->first).second) continue;

		uid_list[uid_count++] = it->first;
	}

	return uid_count;
}

UserCacheNode * UserCacheManager::GetUserNodeByName(std::string user_name, bool *is_online, bool trace_nonexist)
{
	if (NULL == is_online)
	{
		return NULL;
	}

	*is_online = false;

	std::map<std::string, UserStatusNode>::iterator name_uid_it = m_name_uid_map.find(user_name);
	if (name_uid_it != m_name_uid_map.end())
	{
		int uid = name_uid_it->second.uid;
		*is_online = name_uid_it->second.is_online;

		std::map<int, UserCacheNode>::iterator iter = m_user_node_map.find(uid);
		if (iter != m_user_node_map.end())
		{
			return &(iter->second);
		}
	}

	if (trace_nonexist)
	{
		gamelog::g_log_serious_error.printf(LL_INFO, "UserCache no name %s", user_name.c_str());
	}

	return NULL;
}

void UserCacheManager::UpdateCacheNode(UserCacheNode &node)
{
	int uid = node.uid;
	m_user_node_map[uid] = node;
}

void UserCacheManager::GetUserName(const UserID &user_id, GameName user_name)
{
	UserCacheNode *uesr_node = this->GetUserNode(user_id);
	if (NULL != uesr_node) uesr_node->GetName(user_name);
}

void UserCacheManager::GetUserName(int uid, GameName user_name)
{
	UserCacheNode *uesr_node = this->GetUserNode(uid);
	if (NULL != uesr_node) uesr_node->GetName(user_name);
}

void UserCacheManager::UpdateUserNameCache(std::string user_name, int uid, bool is_online)
{
	UserStatusNode node;
	node.uid = uid;
	node.is_online = is_online;

	m_name_uid_map[user_name] = node;
	m_uid_status_map[uid] = node;

	if (!is_online)
	{
		if (CrossConfig::Instance().IsHiddenServer())
		{
			m_user_node_map.erase(uid);
			m_name_uid_map.erase(user_name);
			m_uid_status_map.erase(uid);
			return;
		}
		UserCacheNode *user_node = this->GetUserNode(uid);
		if (NULL != user_node)
		{
			user_node->last_logout_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		}
	}
}

void UserCacheManager::UpdateUserNameCache(Role *role, bool is_online)
{
	if (NULL == role)
	{
		return;
	}

	std::string user_name = role->GetName();
	int uid = role->GetUID();

	this->UpdateUserNameCache(user_name, uid, is_online);

	const static int NET_ID_MAX_COUNT = 512;

	int netid_count = 0;
	static GSNetID netid_list[NET_ID_MAX_COUNT];

	{
		std::set<UserID> related_userid_list;

		{
			// 队友
			if (role->GetTeamInfo()->InTeam())
			{
				Team *team = role->GetTeamInfo()->GetMyTeam();
				if (NULL != team)
				{
					int member_uid_list[MAX_TEAM_MEMBER_NUM] = {0};
					int member_count = team->GetMemberUidList(member_uid_list, MAX_TEAM_MEMBER_NUM);

					for (int i = 0; i < member_count; ++ i)
					{
						if (member_uid_list[i] != role->GetUID())
						{
							related_userid_list.insert(IntToUid(member_uid_list[i]));
						}
					}
				}
			}
		}
		
		{
			// 好友和仇人
			static FriendListParam friend_p;
			role->GetFriendManager()->GetFriendList(&friend_p);

			for (int i = 0; i < friend_p.count; ++ i)
			{
				related_userid_list.insert(IntToUid(friend_p.friend_list[i].friend_uid));
			}

			static EnemyListParam enemy_p;
			role->GetFriendManager()->GetEnemyList(&enemy_p);
			
			for (int i = 0; i < enemy_p.count; ++ i)
			{
				related_userid_list.insert(IntToUid(enemy_p.enemy_list[i].enemy_uid));
			}
		}

		for (std::set<UserID>::iterator it = related_userid_list.begin(); it != related_userid_list.end() && netid_count < NET_ID_MAX_COUNT; ++ it)
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(*it);
			if (NULL != role)
			{
				netid_list[netid_count] = role->GetNetId();
				++ netid_count;
			}
		}
	}

	if (netid_count > 0)
	{
		Protocol::SCOtherUserOnlineStatus ouos;
		ouos.user_id = uid;
		ouos.is_online = (is_online ? 1 : 0);
		ouos.reserve_ch = 0;
		ouos.reserve_sh = 0;

		UserCacheNode *user_node = this->GetUserNode(uid);
		if (NULL != user_node)
		{
			ouos.last_logout_timestamp = user_node->last_logout_timestamp;
		}

		EngineAdapter::Instance().NetMulSend(netid_list, netid_count, (const char*)&ouos, sizeof(ouos));
	}
}

void UserCacheManager::UpdateAvatarType(int uid, short avatar_type)
{
	UserCacheNode *user_node = this->GetUserNode(uid);
	if (NULL != user_node)
	{
		user_node->cur_use_avatar_type = avatar_type;
	}
}

void UserCacheManager::UpdateBubbleType(int uid, short bubble_type)
{
	UserCacheNode *user_node = this->GetUserNode(uid);
	if (NULL != user_node)
	{
		user_node->cur_use_bubble_type = bubble_type;
	}
}

void UserCacheManager::UpdateGuildId(int uid, GuildID guild_id)
{
	UserCacheNode *user_node = this->GetUserNode(uid);
	if (NULL != user_node)
	{
		user_node->guild_id = guild_id;
	}
}

bool UserCacheManager::IsUserOnline(int uid)
{
	std::map<int, UserStatusNode>::iterator uid_status_it = m_uid_status_map.find(uid);
	if (uid_status_it != m_uid_status_map.end())
	{
		return uid_status_it->second.is_online;
	}

	return false;
}

bool UserCacheManager::IsUserOnline(UserID user_id)
{
	return this->IsUserOnline(UidToInt(user_id));
}

int UserCacheManager::CountOfUser()
{
	int count = (int)m_user_node_map.size();
	return count;
}

void UserCacheManager::OnResetName(Role *role, GameName old_name)
{
	if (NULL == role)
	{
		return;
	}

	m_name_uid_map.erase(old_name);
	this->UpdateUserNameCache(role, true);

	std::map<int, UserCacheNode>::iterator it = m_user_node_map.find(role->GetUID());
	if (m_user_node_map.end() != it)
	{
		role->GetName(it->second.user_name);
	}
}

int UserCacheManager::GetUserIDList(int &from, int user_count, UserID *user_list, UserCacheFilterFunc filter_func, void **func_param)
{
	if (from < 0 || user_count <= 0 || NULL == user_list || m_uid_status_map.empty() || from >= (int)m_uid_status_map.size())
	{
		return 0;
	}

	int get_count = 0;

	std::map<int, UserCacheNode>::iterator it = m_user_node_map.begin();
	advance(it, from);

	for (; it != m_user_node_map.end() && get_count < user_count; ++ it)
	{
		++ from;

		if (NULL != filter_func && !filter_func(it->second, func_param))
		{
			continue;
		}

		user_list[get_count] = IntToUid(it->first);
		++ get_count;
	}

	return get_count;
}

int UserCacheManager::GetRoleNumByLevel(int min_lv, int max_lv)
{
	if (m_user_node_map.empty())
	{
		return 0;
	}

	if (min_lv <= 0) min_lv = 1;
	if (max_lv > MAX_ROLE_LEVEL) max_lv = MAX_ROLE_LEVEL;

	unsigned int yesterday_zero_time = static_cast<unsigned int>(GetZeroTime(EngineAdapter::Instance().Time())) - SECOND_PER_DAY;

	int count = 0;
	for (std::map<int, UserCacheNode>::const_iterator it = m_user_node_map.begin(); it != m_user_node_map.end(); ++it)
	{
		if (it->second.last_logout_timestamp >= yesterday_zero_time || UserCacheManager::Instance().IsUserOnline(it->first))
		{
			if (it->second.level >= min_lv && it->second.level <= max_lv)
			{
				++count;
			}
		}
	}

	return count;
}


