#include "crossactivityteamfb.hpp"
#include "crossuser/crossuser.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "crossserver.h"
#include "crossserver/engineadapter.h"
#include "servercommon/serverconfig/crossbossconfig.hpp"
#include "servercommon/string/crossstr.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/errornum.h"

CrossActivityTeamFB::CrossActivityTeamFB(CrossActivityManager *cross_activity_manager) 
	: CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_TEAM_FB)
{
	memset(m_room_no_max, 0, sizeof(m_room_no_max));
}

CrossActivityTeamFB::~CrossActivityTeamFB()
{

}

void CrossActivityTeamFB::Init(const CrossActivityData &data)
{
	m_status_end_time = EngineAdapter::Instance().Time();
	m_is_init_finish = true;
}

void CrossActivityTeamFB::OnCrossUserLogin(CrossUser *cross_user)
{
	if (NULL == cross_user) return;

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		return;
	}

	UserID user_id = IntToUid(cross_user->GetCrossUid());
	if (CROSS_ACTIVITY_TYPE_TEAM_FB != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	crossgameprotocal::CrossGameCrossActivityGotoScene cgtzgs;
	cgtzgs.cross_uid = cross_user->GetCrossUid();
	cgtzgs.cross_activity_type = CROSS_ACTIVITY_TYPE_TEAM_FB;
	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&cgtzgs, sizeof(cgtzgs));
}

bool CrossActivityTeamFB::CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id)
{
	long long uuid = UniqueUserIDToLongLong(unique_user_id);
	CrossTeamFBUserInfoMapIt user_it = m_user_info.find(uuid);
	if (user_it != m_user_info.end())
	{
		CrossTeamFBRoomInfoMapIt room_it = m_room_info[user_it->second.layer].find(user_it->second.room);
		if (room_it != m_room_info[user_it->second.layer].end() && CROSS_TEAM_FB_STATE_TYPE_START == room_it->second.fb_state)
		{
			return true;
		}
	}

	return false;
}

void CrossActivityTeamFB::OnChangeToNextStatus()
{
	const static int ACTIVITY_OPEN_DUR_S = 3600 * 24 * 365; // 默认给开一年时间，理论上不会有隐藏服一年不重启的

	m_activity_status = ACTIVITY_STATUS_OPEN;
	m_status_end_time = EngineAdapter::Instance().Time() + ACTIVITY_OPEN_DUR_S;
}

void CrossActivityTeamFB::OnCrossTeamFBRoomOption(int option2_type, int layer, int room)
{
	if (option2_type <= CROSS_TEAM_FB_OPTION2_TYPE_INVALID || CROSS_TEAM_FB_OPTION2_TYPE_MAX <= option2_type) return;

	CrossTeamFBRoomInfoMapIt room_it = m_room_info[layer].find(room);
	if (room_it == m_room_info[layer].end()) return;

	switch (option2_type)
	{
	case CROSS_TEAM_FB_OPTION2_TYPE_DELETE_ROOM:
		{
			for (int i = 0; i < CROSS_TEAM_FB_ROLE_MAX; i++)
			{
				if (0 != room_it->second.user_uuid[i])
				{
					this->DelCrossTeamFBUserInfo(room_it->second.user_uuid[i]);
				}
			}

			// 删除房间信息
			m_room_info[layer].erase(room_it);
		}
		break;

	case CROSS_TEAM_FB_OPTION2_TYPE_FB_END:
		{
			if (CROSS_TEAM_FB_STATE_TYPE_START != room_it->second.fb_state)
			{
				// 副本未开启
				return;
			}

			for (int i = 0; i < CROSS_TEAM_FB_ROLE_MAX; i++)
			{
				if (0 != room_it->second.user_uuid[i])
				{
					this->DelCrossTeamFBUserInfo(room_it->second.user_uuid[i]);
				}
			}

			// 删除房间信息
			m_room_info[layer].erase(room_it);
		}
		break;

	default:
		break;
	}

	this->NoticeCrossTeamFBRoomInfoChange(layer, room, 0, CROSS_TEAM_FB_STATE_OPTION_QUIT);
}

void CrossActivityTeamFB::OnCrossTeamFBOption(int option_type, CrossTeamFBUserInfo info, int param, int param2, int param3)
{
	if (option_type <= CROSS_TEAM_FB_STATE_OPTION_INVALID || CROSS_TEAM_FB_STATE_OPTION_MAX <= option_type) return;

	switch (option_type)
	{
	case CROSS_TEAM_FB_STATE_OPTION_CREATE:
		{
			if (m_user_info.end() != m_user_info.find(info.uuid))
			{
				// 跨服用户已存在(已有房间)
				return;
			}

			if (info.layer < 0 || info.layer >= TEAM_FB_LAYER_LIMIT_MAX)
			{
				return;
			}

			// 获得空房间
			int room_no = m_room_no_max[info.layer];
			for (int i = 0; i < room_no && i < CROSS_TEAM_FB_ROOM_MAX; i++)
			{
				CrossTeamFBRoomInfoMapIt room_it = m_room_info[info.layer].find(i);
				if (room_it == m_room_info[info.layer].end())
				{
					room_no = i;
					break;
				}
			}

			if (room_no < 0 || room_no >= CROSS_TEAM_FB_ROOM_MAX)
			{
				// 已达最大房间数
				return;
			}

			// 设置玩家基本信息
			info.room = room_no;
			info.user_state = CROSS_TEAM_FB_STATE_TYPE_LEADER;
			this->AddCrossTeamFBUserInfo(info);

			UniqueUserID unique_user_id = LongLongToUniqueUserID(info.uuid);

			// 设置房间基本信息
			CrossTeamFBRoomInfo room_info;
			room_info.unique_server_id = UniqueServerID(unique_user_id.plat_type, unique_user_id.user_id.db_index);
			room_info.need_capability = param;
			room_info.password = param2;
			room_info.is_auto_start = param3;
			F_STRNCPY(room_info.leader_name, info.name, sizeof(info.name));
			room_info.leader_prof = info.prof;
			room_info.leader_sex = info.sex;
			room_info.user_uuid[0] = info.uuid;
			room_info.user_count = 1;
			m_room_info[info.layer][room_no] = room_info;

			if (room_no == m_room_no_max[info.layer])
			{
				++ m_room_no_max[info.layer];
			}

			this->NoticeCrossTeamFBRoomInfoChange(info.layer, info.room, info.uuid, option_type);
		}
		break;

	case CROSS_TEAM_FB_STATE_OPTION_JOIN:
		{
			if (m_user_info.end() != m_user_info.find(info.uuid))
			{
				// 跨服用户已存在(已有房间)
				return;
			}

			if (info.layer < 0 || info.layer >= TEAM_FB_LAYER_LIMIT_MAX)
			{
				return;
			}

			if (0 != param)
			{
				UniqueUserID unique_user_id = LongLongToUniqueUserID(info.uuid);
				UniqueServerID unique_server_id = UniqueServerID(unique_user_id.plat_type, unique_user_id.user_id.db_index);
				bool is_new_server = this->IsNewServer(unique_server_id);

				// 这里是快速加入
				bool is_find = false; int user_count = 0;
				CrossTeamFBRoomInfoMapIt room_it = m_room_info[info.layer].begin();
				for (; room_it != m_room_info[info.layer].end(); room_it++)
				{
					if (unique_server_id != room_it->second.unique_server_id)
					{
						if (is_new_server || this->IsNewServer(room_it->second.unique_server_id)) continue;
					}

					if (room_it->second.user_count >= CROSS_TEAM_FB_ROLE_MAX ||
						CROSS_TEAM_FB_STATE_TYPE_START == room_it->second.fb_state ||
						info.capability < room_it->second.need_capability || room_it->second.password > 0 || room_it->second.user_count <= user_count) continue;

					info.room = room_it->first;
					user_count = room_it->second.user_count;
					is_find = true;
				}

				if (!is_find)
				{
					// 没有可加入的房间，创建一个房间
					this->OnCrossTeamFBOption(CROSS_TEAM_FB_STATE_OPTION_CREATE, info, 0, 0, 1);
					return;
				}
			}

			if (m_room_no_max[info.layer] <= 0)
			{
				// 没有可加入房间
				return;
			}

			CrossTeamFBRoomInfoMapIt room_it = m_room_info[info.layer].find(info.room);
			if (room_it == m_room_info[info.layer].end() ||
				CROSS_TEAM_FB_STATE_TYPE_START == room_it->second.fb_state)
			{
				// 有玩家未准备
				Protocol::SCNoticeNum nn;
				nn.notice_num = errornum::EN_CROSS_TEAM_FB_HAS_START;
				CrossServer::GetInstCrossServer()->SendToUser(LongLongToUniqueUserID(info.uuid), (const char*)&nn, sizeof(nn));

				return;
			}

			if (room_it->second.user_count >= CROSS_TEAM_FB_ROLE_MAX)
			{
				// 房间已满，不可加入
				return;
			}

			if (room_it->second.need_capability > info.capability)
			{
				// 战力不足，不可加入
				return;
			}

			this->AddCrossTeamFBUserInfo(info);

			// 加入房间
			for (int i = 0; i < CROSS_TEAM_FB_ROLE_MAX; i++)
			{
				if (0 == room_it->second.user_uuid[i])
				{
					// 加入用户
					room_it->second.user_uuid[i] = info.uuid;
					++ room_it->second.user_count;
					break;
				}
			}

			if (room_it->second.is_auto_start > 0)
			{
				bool can_start = true;
				for (int i = 0; i < CROSS_TEAM_FB_ROLE_MAX; i++)
				{
					if (0 != room_it->second.user_uuid[i])
					{
						CrossTeamFBUserInfoMapIt temp_user_it = m_user_info.find(room_it->second.user_uuid[i]);
						if (temp_user_it == m_user_info.end() || CROSS_TEAM_FB_STATE_TYPE_WAIT == temp_user_it->second.user_state)
						{
							can_start = false;
						}
					}
					else
					{
						can_start = false;
					}
				}

				if (can_start)
				{
					CrossTeamFBUserInfo user_info;
					user_info.uuid = room_it->second.user_uuid[0];

					this->OnCrossTeamFBOption(CROSS_TEAM_FB_STATE_OPTION_START, user_info);
				}
			}

			this->NoticeCrossTeamFBRoomInfoChange(info.layer, info.room, info.uuid, option_type);
		}
		break;

	case CROSS_TEAM_FB_STATE_OPTION_REMOVE:
		{
			CrossTeamFBUserInfoMapIt user_it = m_user_info.find(info.uuid);
			if (m_user_info.end() == user_it)
			{
				// 跨服用户不存在(未有房间)
				return;
			}

			if (user_it->second.layer < 0 || user_it->second.layer >= TEAM_FB_LAYER_LIMIT_MAX)
			{
				return;
			}

			CrossTeamFBRoomInfoMapIt room_it = m_room_info[user_it->second.layer].find(user_it->second.room);
			if (room_it == m_room_info[user_it->second.layer].end())
			{
				// 房间不存在
				return;
			}

			if (param < 0 || param >= CROSS_TEAM_FB_ROLE_MAX)
			{
				return;
			}

			if (CROSS_TEAM_FB_STATE_TYPE_LEADER != user_it->second.user_state ||
				info.uuid == room_it->second.user_uuid[param])
			{
				// 只有队长能移除用户,并且不能移除自己
				return;
			}

			if (0 != room_it->second.user_uuid[param])
			{
				this->DelCrossTeamFBUserInfo(room_it->second.user_uuid[param]);
				room_it->second.user_uuid[param] = 0;
				-- room_it->second.user_count;
			}

			this->NoticeCrossTeamFBRoomInfoChange(user_it->second.layer, user_it->second.room, info.uuid, option_type);
		}
		break;

	case CROSS_TEAM_FB_STATE_OPTION_QUIT:
		{
			CrossTeamFBUserInfoMapIt user_it = m_user_info.find(info.uuid);
			if (m_user_info.end() == user_it)
			{
				// 跨服用户不存在(未有房间)
				return;
			}

			if (user_it->second.layer < 0 || user_it->second.layer >= TEAM_FB_LAYER_LIMIT_MAX)
			{
				return;
			}

			CrossTeamFBRoomInfoMapIt room_it = m_room_info[user_it->second.layer].find(user_it->second.room);
			if (room_it == m_room_info[user_it->second.layer].end())
			{
				// 房间不存在
				return;
			}

			// param不为1时就算开了也强制退出
			if (1 == param && CROSS_TEAM_FB_STATE_TYPE_START == room_it->second.fb_state)
			{
				// 副本开始了，这里不退出（如：角色掉线）
				return;
			}

			{
				// 避免信息不一致
				info.layer = user_it->second.layer;
				info.room = user_it->second.room;
			}

			if (CROSS_TEAM_FB_STATE_TYPE_LEADER == user_it->second.user_state)
			{
				// 队长离开则解散队伍
				for (int i = 0; i < CROSS_TEAM_FB_ROLE_MAX; i++)
				{
					if (0 != room_it->second.user_uuid[i])
					{
						this->DelCrossTeamFBUserInfo(room_it->second.user_uuid[i]);
					}
				}

				// 删除房间信息
				m_room_info[info.layer].erase(room_it);
			}
			else
			{
				for (int i = 0; i < CROSS_TEAM_FB_ROLE_MAX; i++)
				{
					if (info.uuid == room_it->second.user_uuid[i])
					{
						// 移除用户
						room_it->second.user_uuid[i] = 0;
						-- room_it->second.user_count;
						break;
					}
				}

				// 移除跨服组队用户
				this->DelCrossTeamFBUserInfo(info.uuid);
			}

			this->NoticeCrossTeamFBRoomInfoChange(info.layer, info.room, info.uuid, option_type);
		}
		break;

	case CROSS_TEAM_FB_STATE_OPTION_READY:
		{
			CrossTeamFBUserInfoMapIt user_it = m_user_info.find(info.uuid);
			if (m_user_info.end() == user_it)
			{
				// 跨服用户不存在(未有房间)
				return;
			}

			if (user_it->second.layer < 0 || user_it->second.layer >= TEAM_FB_LAYER_LIMIT_MAX)
			{
				return;
			}

			if (CROSS_TEAM_FB_STATE_TYPE_READY != user_it->second.user_state &&
				CROSS_TEAM_FB_STATE_TYPE_WAIT != user_it->second.user_state)
			{
				// 只能进行准备和取消准备的状态更改
				return;
			}

			user_it->second.user_state = (CROSS_TEAM_FB_STATE_TYPE_READY == user_it->second.user_state ? CROSS_TEAM_FB_STATE_TYPE_WAIT : CROSS_TEAM_FB_STATE_TYPE_READY);

			this->AddCrossTeamFBUserInfo(user_it->second);

			this->NoticeCrossTeamFBRoomInfoChange(user_it->second.layer, user_it->second.room, info.uuid, option_type, false);

			CrossTeamFBRoomInfoMapIt room_it = m_room_info[user_it->second.layer].find(user_it->second.room);
			if (room_it == m_room_info[user_it->second.layer].end())
			{
				// 房间不存在
				return;
			}

			if (room_it->second.is_auto_start > 0)
			{
				bool can_start = true;
				for (int i = 0; i < CROSS_TEAM_FB_ROLE_MAX; i++)
				{
					if (0 != room_it->second.user_uuid[i])
					{
						CrossTeamFBUserInfoMapIt temp_user_it = m_user_info.find(room_it->second.user_uuid[i]);
						if (temp_user_it == m_user_info.end() || CROSS_TEAM_FB_STATE_TYPE_WAIT == temp_user_it->second.user_state)
						{
							can_start = false;
						}
					}
					else
					{
						can_start = false;
					}
				}

				if (can_start)
				{
					CrossTeamFBUserInfo user_info;
					user_info.uuid = room_it->second.user_uuid[0];

					this->OnCrossTeamFBOption(CROSS_TEAM_FB_STATE_OPTION_START, user_info);
				}
			}
		}
		break;

	case CROSS_TEAM_FB_STATE_OPTION_START:
		{
			CrossTeamFBUserInfoMapIt user_it = m_user_info.find(info.uuid);
			if (m_user_info.end() == user_it)
			{
				// 跨服用户不存在(未有房间)
				return;
			}

			if (user_it->second.layer < 0 || user_it->second.layer >= TEAM_FB_LAYER_LIMIT_MAX)
			{
				return;
			}

			CrossTeamFBRoomInfoMapIt room_it = m_room_info[user_it->second.layer].find(user_it->second.room);
			if (room_it == m_room_info[user_it->second.layer].end())
			{
				// 房间不存在
				return;
			}

			if (CROSS_TEAM_FB_STATE_TYPE_LEADER != user_it->second.user_state)
			{
				// 只有队长能开启副本
				return;
			}

			if (CROSS_TEAM_FB_STATE_TYPE_START == room_it->second.fb_state)
			{
				// 副本已开始
				return;
			}

			for (int i = 0; i < CROSS_TEAM_FB_ROLE_MAX; i++)
			{
				if (0 != room_it->second.user_uuid[i])
				{
					CrossTeamFBUserInfoMapIt temp_user_it = m_user_info.find(room_it->second.user_uuid[i]);
					if (temp_user_it == m_user_info.end() || CROSS_TEAM_FB_STATE_TYPE_WAIT == temp_user_it->second.user_state)
					{
						if (param <= 0)
						{
							// 有玩家未准备
							Protocol::SCNoticeNum nn;
							nn.notice_num = errornum::EN_CROSS_TEAM_FB_NOT_READY;
							CrossServer::GetInstCrossServer()->SendToUser(LongLongToUniqueUserID(info.uuid), (const char*)&nn, sizeof(nn));

							return;
						}
						else
						{
							this->OnCrossTeamFBOption(CROSS_TEAM_FB_STATE_OPTION_REMOVE, info, i);	
						}
					}
				}
			}

			room_it->second.fb_state = CROSS_TEAM_FB_STATE_TYPE_START;
			for (int i = 0; i < CROSS_TEAM_FB_ROLE_MAX; i++)
			{
				if (0 != room_it->second.user_uuid[i])
				{
					this->NoticeUserCanEnterCross(room_it->second.user_uuid[i]);
				}
			}

			this->NoticeCrossTeamFBRoomInfoChange(user_it->second.layer, user_it->second.room, info.uuid, option_type);
		}
		break;

	case CROSS_TEAM_FB_STATE_OPTION_ENTER:
		{
			CrossTeamFBUserInfoMapIt user_it = m_user_info.find(info.uuid);
			if (m_user_info.end() == user_it)
			{
				// 跨服用户不存在(未有房间)
				this->NoticeCrossTeamFBRoomInfoChange(0, 0, info.uuid, CROSS_TEAM_FB_STATE_OPTION_QUIT);
				return;
			}

			if (user_it->second.layer < 0 || user_it->second.layer >= TEAM_FB_LAYER_LIMIT_MAX)
			{
				return;
			}

			CrossTeamFBRoomInfoMapIt room_it = m_room_info[user_it->second.layer].find(user_it->second.room);
			if (room_it == m_room_info[user_it->second.layer].end())
			{
				// 房间不存在
				this->NoticeCrossTeamFBRoomInfoChange(0, 0, info.uuid, CROSS_TEAM_FB_STATE_OPTION_QUIT);
				return;
			}

			// 副本未开始不可进入，发送房间信息，用来告诉上线用户所在房间信息
			this->NoticeCrossTeamFBRoomInfoChange(user_it->second.layer, user_it->second.room, info.uuid, option_type, false);
		}
		break;

	case CROSS_TEAM_FB_STATE_OPTION_FINDMATE:
		{
			CrossTeamFBUserInfoMapIt user_it = m_user_info.find(info.uuid);
			if (m_user_info.end() == user_it)
			{
				// 跨服用户不存在(未有房间)
				return;
			}

			if (user_it->second.layer < 0 || user_it->second.layer >= TEAM_FB_LAYER_LIMIT_MAX)
			{
				return;
			}

			CrossTeamFBRoomInfoMapIt room_it = m_room_info[user_it->second.layer].find(user_it->second.room);
			if (room_it == m_room_info[user_it->second.layer].end())
			{
				// 房间不存在
				return;
			}

			if (CROSS_TEAM_FB_STATE_TYPE_START == room_it->second.fb_state)
			{
				// 副本开始了，不可寻找
				return;
			}

			int length = 0;
			
			if (room_it->second.need_capability > 0)
			{
				length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_team_fb_fine_mate_notice, 
					UidToInt(LongLongToUniqueUserID(user_it->second.uuid).user_id), user_it->second.name, user_it->second.camp, user_it->second.layer,
					room_it->second.need_capability, user_it->second.layer, user_it->second.room);
			}
			else
			{
				length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_team_fb_fine_mate_not_capa_notice, 
					UidToInt(LongLongToUniqueUserID(user_it->second.uuid).user_id), user_it->second.name, user_it->second.camp, user_it->second.layer,
					user_it->second.layer, user_it->second.room);
			}

			if (length > 0)
			{
				UniqueUserID unique_user_id = LongLongToUniqueUserID(info.uuid);
				UniqueServerID unique_server_id = UniqueServerID(unique_user_id.plat_type, unique_user_id.user_id.db_index);
				m_cross_activity_manager->GetCrossServer()->SystemMsgToAllGlobalUser(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE, 0, 0, true, unique_server_id);
			}
		}
		break;

	case CROSS_TEAM_FB_STATE_OPTION_SET_AUTO_START:
		{
			CrossTeamFBUserInfoMapIt user_it = m_user_info.find(info.uuid);
			if (m_user_info.end() == user_it)
			{
				// 跨服用户不存在(未有房间)
				return;
			}

			if (user_it->second.layer < 0 || user_it->second.layer >= TEAM_FB_LAYER_LIMIT_MAX)
			{
				return;
			}

			CrossTeamFBRoomInfoMapIt room_it = m_room_info[user_it->second.layer].find(user_it->second.room);
			if (room_it == m_room_info[user_it->second.layer].end())
			{
				// 房间不存在
				return;
			}

			if (CROSS_TEAM_FB_STATE_TYPE_LEADER != user_it->second.user_state)
			{
				return;
			}

			if (CROSS_TEAM_FB_STATE_TYPE_START == room_it->second.fb_state)
			{
				return;
			}

			room_it->second.is_auto_start = param;

			this->NoticeCrossTeamFBRoomInfoChange(user_it->second.layer, user_it->second.room, info.uuid, option_type);
		}
		break;

	default:
		break;
	}
}

void CrossActivityTeamFB::NoticeUserCanEnterCross(long long uuid)
{
	static Protocol::SCNoticeCanEnterCross nucec;
	nucec.activity_type = CROSS_ACTIVITY_TYPE_TEAM_FB;
	CrossServer::GetInstCrossServer()->SendToUser(LongLongToUniqueUserID(uuid), (const char*)&nucec, sizeof(nucec));
}

void CrossActivityTeamFB::NoticeCrossTeamFBRoomInfoChange(int layer, int room, long long uuid, int opera_type, bool is_send_all_user)
{
	static Protocol::SCNoticeCrossTeamFBRoomInfoChange nctfbric;

	nctfbric.layer = layer;
	nctfbric.room = room;
	nctfbric.opera_uuid = uuid;
	nctfbric.opera_type = opera_type;

	if (!is_send_all_user)
	{
		CrossTeamFBRoomInfoMapIt room_it = m_room_info[layer].find(room);
		if (room_it == m_room_info[layer].end()) return;

		for (int i = 0; i < CROSS_TEAM_FB_ROLE_MAX; i++)
		{
			if (0 != room_it->second.user_uuid[i])
			{
				CrossServer::GetInstCrossServer()->SendToUser(LongLongToUniqueUserID(room_it->second.user_uuid[i]), (const char*)&nctfbric, sizeof(nctfbric));
			}
		}
	}
	else
	{
		CrossServer::GetInstCrossServer()->SendToAllUser(true, 0, (const char*)&nctfbric, sizeof(nctfbric));
	}
}

void CrossActivityTeamFB::GetCrossTeamFBRoomListInfo(NetID global_netid, const UniqueUserID &unique_user_id, int layer)
{
	if (layer < 0 || TEAM_FB_LAYER_LIMIT_MAX <= layer) return;

	UniqueServerID unique_server_id = UniqueServerID(unique_user_id.plat_type, unique_user_id.user_id.db_index);
	bool is_new_server = this->IsNewServer(unique_server_id);

	static Protocol::SCCrossTeamFBRoomListInfo gctfbrli;
	gctfbrli.room_count = 0;
	CrossTeamFBRoomInfoMapIt room_it = m_room_info[layer].begin();
	for (; room_it != m_room_info[layer].end(); room_it++)
	{
		if (gctfbrli.room_count >= CROSS_TEAM_FB_ROOM_MAX) break;
		if (CROSS_TEAM_FB_STATE_TYPE_START == room_it->second.fb_state)
		{
			continue;
		}

		if (unique_server_id != room_it->second.unique_server_id)
		{
			if (is_new_server || this->IsNewServer(room_it->second.unique_server_id)) continue;
		}

		gctfbrli.room_info[gctfbrli.room_count].need_capability = room_it->second.need_capability;
		gctfbrli.room_info[gctfbrli.room_count].password = room_it->second.password;
		gctfbrli.room_info[gctfbrli.room_count].is_auto_start = room_it->second.is_auto_start;
		F_STRNCPY(gctfbrli.room_info[gctfbrli.room_count].leader_name, room_it->second.leader_name, sizeof(GameName));
		gctfbrli.room_info[gctfbrli.room_count].leader_prof = room_it->second.leader_prof;
		gctfbrli.room_info[gctfbrli.room_count].leader_sex = room_it->second.leader_sex;
		gctfbrli.room_info[gctfbrli.room_count].fb_state = room_it->second.fb_state;
		gctfbrli.room_info[gctfbrli.room_count].user_count = room_it->second.user_count;
		gctfbrli.room_info[gctfbrli.room_count].room = room_it->first;
		gctfbrli.room_info[gctfbrli.room_count].layer = layer;
		
		++ gctfbrli.room_count;
	}

	int lenght = sizeof(gctfbrli) - ((CROSS_TEAM_FB_ROOM_MAX - gctfbrli.room_count) * sizeof(gctfbrli.room_info[0]));
	CrossServer::GetInstCrossServer()->SendToUser(global_netid, unique_user_id.user_id, (const char*)&gctfbrli, lenght);
}

void CrossActivityTeamFB::GetCrossTeamFBRoomInfo(NetID global_netid, const UniqueUserID &unique_user_id, int layer, int room)
{
	if (layer < 0 || TEAM_FB_LAYER_LIMIT_MAX <= layer || room < 0) return;

	static Protocol::SCCrossTeamFBRoomInfo gctfbri;

	gctfbri.layer = layer;
	gctfbri.room = room;
	gctfbri.user_count = 0;

	CrossTeamFBRoomInfoMapIt room_it = m_room_info[layer].find(room);
	if (room_it != m_room_info[layer].end())
	{
		gctfbri.is_auto_start = room_it->second.is_auto_start;
		gctfbri.fb_state = room_it->second.fb_state;

		for (int i = 0; i < CROSS_TEAM_FB_ROLE_MAX; i++)
		{
			if (0 != room_it->second.user_uuid[i])
			{
				CrossTeamFBUserInfoMapIt user_it = m_user_info.find(room_it->second.user_uuid[i]);
				if (user_it == m_user_info.end()) continue;

				gctfbri.User_info[gctfbri.user_count].uuid = user_it->second.uuid;
				gctfbri.User_info[gctfbri.user_count].prof = user_it->second.prof;
				gctfbri.User_info[gctfbri.user_count].sex = user_it->second.sex;
				gctfbri.User_info[gctfbri.user_count].camp = user_it->second.camp;
				gctfbri.User_info[gctfbri.user_count].user_state = user_it->second.user_state;
				F_STRNCPY(gctfbri.User_info[gctfbri.user_count].name, user_it->second.name, sizeof(GameName));
				gctfbri.User_info[gctfbri.user_count].capability = user_it->second.capability;
				gctfbri.User_info[gctfbri.user_count].index = i;

				++ gctfbri.user_count;
			}
		}
	}
	
	CrossServer::GetInstCrossServer()->SendToUser(global_netid, unique_user_id.user_id, (const char*)&gctfbri, sizeof(gctfbri));
}

void CrossActivityTeamFB::AddCrossTeamFBUserInfo(CrossTeamFBUserInfo info)
{
	crossgameprotocal::SyncCrossTeamFBUserInfo actfbui;

	CrossTeamFBUserInfoMapIt user_it = m_user_info.find(info.uuid);
	if (user_it == m_user_info.end())
	{
		m_user_info[info.uuid] = info;
	}
	actfbui.user_info = info;

	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&actfbui, sizeof(actfbui));
};

void CrossActivityTeamFB::DelCrossTeamFBUserInfo(long long uuid)
{
	crossgameprotocal::SyncCrossTeamFBUserInfo dctfbui;

	CrossTeamFBUserInfoMapIt user_it = m_user_info.find(uuid);
	if (user_it == m_user_info.end()) return;
	m_user_info.erase(user_it);

	dctfbui.user_info.uuid = uuid;
	dctfbui.user_info.layer = -1;

	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&dctfbui, sizeof(dctfbui));
};

bool CrossActivityTeamFB::IsNewServer(const UniqueServerID &unique_server_id)
{
	int day_index = CrossServer::GetInstCrossServer()->GetGameServerDayIndex(unique_server_id);
	return this->IsNewServerByIndex(day_index);
}

bool CrossActivityTeamFB::IsNewServerByIndex(int day_index)
{
	return day_index < 7;
}
