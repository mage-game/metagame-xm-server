#include "worldstatus2.hpp"

#include "global/rmibackobjdef.h"
#include "internalcomm.h"
#include "engineadapter.h"
#include "world.h"
#include "global/usercache/usercache.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "global/droplog/droplogmanager.hpp"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/commondata/commondataglobal.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "gameworld/obj/character/role.h"
#include "servercommon/serverconfig/crossliekunfbconfig.hpp"

WorldStatus2::WorldStatus2()
	: m_data_state(WORLDSTATE2_DATA_STATUS_INVALID)
{

}

WorldStatus2::~WorldStatus2()
{

}

void WorldStatus2::Init(const WorldStatus2Param &worldstatus2_param)
{
	memcpy(&m_world_status2_param, &worldstatus2_param, sizeof(WorldStatus2Param));

	//CrossRAManagerLocal::Instance().Init(m_param.cross_rand_activity_param, m_param.cross_rand_activity_big_param);

	DropLogManager::Instance().Init(m_world_status2_param.drop_log_list_param,m_world_status2_param.drop_log_fb_param);
	this->InitWorldAcquisitionLogList();

	this->LoadWorldStatus2Succ();
}

void WorldStatus2::OnServerStart()
{
	this->LoadWorldStatus2(0);
}

void WorldStatus2::OnServerStop()
{
	this->SaveWorldStatus2();
}

void WorldStatus2::Update(unsigned long interval, time_t now_second)
{
	if (WORLDSTATE2_DATA_STATUS_LOAD_FINISH != m_data_state) return;

	static time_t last_save_time = now_second;

	if (now_second >= last_save_time + WORLDSTATE2_SAVE_INTERVAL)
	{
		this->SaveWorldStatus2(); 

		last_save_time = now_second;
	}
}

void WorldStatus2::OnEnterScene(Role *role)
{

}

void WorldStatus2::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

	m_world_status2_param.wedding_param.wedding_yuyue_flag = 0;
	memset(m_world_status2_param.wedding_param.yuyue_info, 0, sizeof(m_world_status2_param.wedding_param.yuyue_info));
}

void WorldStatus2::OnResetData(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;
}

bool WorldStatus2::LoadWorldStatus2(long long id_from)
{
	if (WORLDSTATE2_DATA_STATUS_LOAD_FINISH == m_data_state) return false;

	m_data_state = WORLDSTATE2_DATA_STATUS_LOADING;

	RMIInitWorldStatus2BackObjectImpl *impl = new RMIInitWorldStatus2BackObjectImpl();

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return gc.InitWorldStatus2Asyn(id_from, impl);
}

void RMIInitWorldStatus2BackObjectImpl::InitWorldStatus2Ret(int ret, const WorldStatus2Param &worldstatus2_param)
{
	if (ret < 0)
	{
		printf(" RMIInitWorldStatus2BackObjectImpl::InitWorldStatus2Ret Failed \n");
		EngineAdapter::Instance().StopGame();
		return;
	}

	WorldStatus2::Instance().Init(worldstatus2_param);
}

void WorldStatus2::LoadWorldStatus2Succ()
{
	m_data_state = WORLDSTATE2_DATA_STATUS_LOAD_FINISH;
}

bool WorldStatus2::SaveWorldStatus2()
{
	//CrossRAManagerLocal::Instance().GetInitParam(&m_param.cross_rand_activity_param, &m_param.cross_rand_activity_big_param);

	DropLogManager::Instance().GetInitParam(&m_world_status2_param.drop_log_list_param, &m_world_status2_param.drop_log_fb_param);
	this->SaveWorldAcquisitionLogList();

	RMISaveWorldStatus2BackObjectImpl *impl = new RMISaveWorldStatus2BackObjectImpl();

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return gc.SaveWorldStatus2Asyn(m_world_status2_param, impl);
}

WorldStatus2 & WorldStatus2::Instance()
{
	static WorldStatus2 ws;
	return ws;
}

bool WorldStatus2::IsLoadFinish() const
{
	return (WORLDSTATE2_DATA_STATUS_LOAD_FINISH == m_data_state);
}

char WorldStatus2::GetWeedingYuYueSeqByUID(int uid)
{
	for (int i = WEDDING_YUYUE_TIME_MAX - 1; i >= 0; --i)
	{
		if (uid == m_world_status2_param.wedding_param.yuyue_info[i].role_id || uid == m_world_status2_param.wedding_param.yuyue_info[i].lover_role_id)
		{
			return m_world_status2_param.wedding_param.yuyue_info[i].wedding_yuyue_seq;
		}
	}

	return -1;
}

bool WorldStatus2::IsInWeddingGuests(int owner_uid, int guest_uid)
{
	for (int i = WEDDING_YUYUE_TIME_MAX - 1; i >= 0; --i)
	{
		if (owner_uid == m_world_status2_param.wedding_param.yuyue_info[i].role_id || owner_uid == m_world_status2_param.wedding_param.yuyue_info[i].lover_role_id)
		{
			for (int j = 0; j < WEDDING_GUESTS_MAX_NUM; j++)
			{
				if (guest_uid == m_world_status2_param.wedding_param.yuyue_info[i].guest_role_id_list[j])
				{
					return true;
				}
			}

			break;
		}
	}

	return false;
}

bool WorldStatus2::AddWeddingGeust(int owner_uid, int guest_uid)
{
	for (int i = WEDDING_YUYUE_TIME_MAX - 1; i >= 0; --i)
	{
		if (owner_uid == m_world_status2_param.wedding_param.yuyue_info[i].role_id || owner_uid == m_world_status2_param.wedding_param.yuyue_info[i].lover_role_id)
		{
			for (int j = 0; j < WEDDING_GUESTS_MAX_NUM; j++)
			{
				if (0 == m_world_status2_param.wedding_param.yuyue_info[i].guest_role_id_list[j])
				{
					m_world_status2_param.wedding_param.yuyue_info[i].guest_role_id_list[j] = guest_uid;

					return true;
				}
			}
			break;
		}
	}

	return false;
}

bool WorldStatus2::RemoveWeddingGeust(int owner_uid, int guest_uid)
{
	for (int i = WEDDING_YUYUE_TIME_MAX - 1; i >= 0; --i)
	{
		if (owner_uid == m_world_status2_param.wedding_param.yuyue_info[i].role_id || owner_uid == m_world_status2_param.wedding_param.yuyue_info[i].lover_role_id)
		{
			for (int j = 0; j < WEDDING_GUESTS_MAX_NUM; j++)
			{
				if (guest_uid == m_world_status2_param.wedding_param.yuyue_info[i].guest_role_id_list[j])
				{
					m_world_status2_param.wedding_param.yuyue_info[i].guest_role_id_list[j] = 0;

					return true;
				}
			}
			break;
		}
	}

	return false;
}

bool WorldStatus2::AddWeddingCanInviteGuestNum(int owner_uid)
{
	for (int i = WEDDING_YUYUE_TIME_MAX - 1; i >= 0; --i)
	{
		if (owner_uid == m_world_status2_param.wedding_param.yuyue_info[i].role_id || owner_uid == m_world_status2_param.wedding_param.yuyue_info[i].lover_role_id)
		{
			++m_world_status2_param.wedding_param.yuyue_info[i].can_invite_guest_num;
			return true;
		}
	}

	return false;
}

int WorldStatus2::GetWeddingHasInviteGuestNum(int owner_uid)
{
	int has_invite_guest_num = 0;
	for (int i = WEDDING_YUYUE_TIME_MAX - 1; i >= 0; --i)
	{
		if (owner_uid == m_world_status2_param.wedding_param.yuyue_info[i].role_id || owner_uid == m_world_status2_param.wedding_param.yuyue_info[i].lover_role_id)
		{
			for (int j = 0; j < WEDDING_GUESTS_MAX_NUM; j++)
			{
				if (0 != m_world_status2_param.wedding_param.yuyue_info[i].guest_role_id_list[j])
				{
					has_invite_guest_num++;
				}
			}
		}
	}

	return has_invite_guest_num;
}

int WorldStatus2::AddTotalWeddingNum()
{
	++m_world_status2_param.wedding_param.total_wedding_num;

	return m_world_status2_param.wedding_param.total_wedding_num;
}

void WorldStatus2::ClearWeddingGuests(int wedding_seq)
{
	if (wedding_seq < 0 || wedding_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return;
	}

	WeddingYuYueInfo &yuyue_info = m_world_status2_param.wedding_param.yuyue_info[wedding_seq];
	yuyue_info.role_id = 0;
	yuyue_info.lover_role_id = 0;
	yuyue_info.can_invite_guest_num = 0;
	memset(yuyue_info.guest_role_id_list, 0, sizeof(yuyue_info.guest_role_id_list));
}


void WorldStatus2::RecoredProfession(Role* role, PersonProfessItem& person_prfess_item)
{
	if (nullptr == role)
	{
		return;
	}

	// 当前要覆盖的表白索引
	short current_index = m_world_status2_param.global_profess_param.earliest_profess_index;
	if (current_index < 0 || current_index >= GLOBAL_PROFESS_MAX_NUM)
	{
		return;
	}

	// 覆盖最早表白的数据
	GlobalProfessItem& global_profess_item = m_world_status2_param.global_profess_param.profession_item_list[current_index];
	global_profess_item.role_id_from = UidToInt(role->GetUserId());
	global_profess_item.role_id_to = person_prfess_item.other_role_id;
	global_profess_item.profess_time = person_prfess_item.profess_time;
	global_profess_item.gife_type = person_prfess_item.gift_type;
	UNSTD_STATIC_CHECK(sizeof(global_profess_item.content) == sizeof(person_prfess_item.content));
	F_STRNCPY(global_profess_item.content, person_prfess_item.content, sizeof(person_prfess_item.content));

	// 指向下一个告白（最早的）
	m_world_status2_param.global_profess_param.earliest_profess_index++;
	m_world_status2_param.global_profess_param.earliest_profess_index %= GLOBAL_PROFESS_MAX_NUM;

	this->SendProfessionInfo(role);
}

void WorldStatus2::SendProfessionInfo(Role* role)
{
	static Protocol::SCGlobalProfessWallInfo profess_wall_info;

	// 发给客户端的表白item数量
	int item_count = 0;

	// 获取某时刻后的表白
	for (int i = 0; i < GLOBAL_PROFESS_MAX_NUM; i++)
	{
		GlobalProfessItem& global_item = m_world_status2_param.global_profess_param.profession_item_list[i];

		if (global_item.profess_time != 0)
		{
			Protocol::SCGlobalProfessWallInfo::ProfessItem& msg_item = profess_wall_info.items[item_count];
			{
				// 被表白者名字、头像
				if (Role* role_to = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(global_item.role_id_to)))
				{
					msg_item.avatar_timestamp_to = role_to->GetAvatarTimeStamp();
					F_STRNCPY(msg_item.role_name_to, role_to->GetName(), sizeof(GameName));
				}
				else if (UserCacheNode *role_node_to = UserCacheManager::Instance().GetUserNode(IntToUid(global_item.role_id_to)))
				{
					msg_item.avatar_timestamp_to = role_node_to->avatar_timestamp;
					F_STRNCPY(msg_item.role_name_to, role_node_to->user_name, sizeof(GameName));
				}
				else
				{
					continue;
				}
					
				// 表白自名字、头像
				if (Role* role_from = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(global_item.role_id_from)))
				{
					msg_item.avatar_timestamp = role_from->GetAvatarTimeStamp();
					F_STRNCPY(msg_item.role_name_from, role_from->GetName(), sizeof(GameName));
				}
				else if (UserCacheNode *role_node_from = UserCacheManager::Instance().GetUserNode(IntToUid(global_item.role_id_from)))
				{
					msg_item.avatar_timestamp = role_node_from->avatar_timestamp;
					F_STRNCPY(msg_item.role_name_from, role_node_from->user_name, sizeof(GameName));
				}
				else
				{
					continue;
				}

				// id、时间、礼物类型、表白内容
				msg_item.role_id_to = global_item.role_id_to;
				msg_item.role_id_from = global_item.role_id_from;
				msg_item.profess_time = global_item.profess_time;
				msg_item.gife_type = global_item.gife_type;
				UNSTD_STATIC_CHECK(sizeof(msg_item.content) == sizeof(global_item.content));
				F_STRNCPY(msg_item.content, global_item.content, sizeof(msg_item.content));
			}

			item_count++;
		}
	}

	profess_wall_info.profess_count = item_count;

	// 发送
	int send_size_byte = sizeof(Protocol::SCGlobalProfessWallInfo) - (GLOBAL_PROFESS_MAX_NUM - item_count) * sizeof(Protocol::SCGlobalProfessWallInfo::ProfessItem);
	if (send_size_byte > 0 && send_size_byte <= (int)sizeof(Protocol::SCGlobalProfessWallInfo))
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&profess_wall_info, send_size_byte);
	}
}

void WorldStatus2::OnRoleMuteClearProfession(int uid)
{
	std::vector<GlobalProfessItem> temp_vec;
	int need_delete_count = 0;
	for (int i = 0; i < GLOBAL_PROFESS_MAX_NUM; ++i)
	{
		GlobalProfessItem& item = m_world_status2_param.global_profess_param.profession_item_list[i];
		if (!item.IsVaild())
		{
			break;
		}

		if (item.role_id_from == uid)
		{
			++need_delete_count;
		}
		else
		{
			temp_vec.emplace_back(item);
		}
	}

	// 如果不需要删除，直接返回
	if (0 == need_delete_count)
	{
		return;
	}

	// 覆盖回去
	memset(m_world_status2_param.global_profess_param.profession_item_list, 0, sizeof(m_world_status2_param.global_profess_param.profession_item_list));
	int index = 0;
	for (auto iter = temp_vec.begin(); iter != temp_vec.end() && index < GLOBAL_PROFESS_MAX_NUM; ++iter)
	{
		m_world_status2_param.global_profess_param.profession_item_list[index++] = *iter;
	}

	m_world_status2_param.global_profess_param.earliest_profess_index = static_cast<short>(temp_vec.size());		// 设置下一个表白位置
}

void WorldStatus2::SetCrossLieKunFBGuildId(int zone, GuildID liekun_guild_id)
{
	if (zone < LIEKUN_ZONE_TYPE_MAIN || zone >= LIEKUN_ZONE_TYPE_COUNT)
	{
		return;
	}

	m_liekun_guild_id_list[zone] = liekun_guild_id;
}

void WorldStatus2::GetCrossLieKunFBGuildId(GuildID liekun_guild_id_list[LIEKUN_ZONE_TYPE_COUNT])
{
	for (int i = 0; i < LIEKUN_ZONE_TYPE_COUNT; i++)
	{
		liekun_guild_id_list[i] = m_liekun_guild_id_list[i];
	}
}

void WorldStatus2::ClearCrossLieKunFBGuildId(bool is_to_open)
{
	if(is_to_open)
	{
		m_world_status2_param.common_data_2.ling_kun_win_guild_id = 0;
	}
	memset(m_liekun_guild_id_list, 0, sizeof(m_liekun_guild_id_list));
}

GuildID WorldStatus2::GetCrossLieKunFBTopOneGuildId()
{
	return m_liekun_guild_id_list[LIEKUN_ZONE_TYPE_MAIN];
}

void WorldStatus2::SyncRoomActivityTitleInfo(Role *role, int reason)
{
	if (NULL == role)
	{
		return;
	}
	RoomActivityInfo actinfo;

	ARRAYCPY(actinfo.mess_battle_rank_first, m_world_status2_param.common_data_2.mess_battle_rank_first);
	ARRAYCPY(actinfo.mess_battle_rank_second, m_world_status2_param.common_data_2.mess_battle_rank_second);
	ARRAYCPY(actinfo.mess_battle_rank_third, m_world_status2_param.common_data_2.mess_battle_rank_third);

	ARRAYCPY(actinfo.night_fight_rank_first, m_world_status2_param.common_data_2.night_fight_rank_first);
	ARRAYCPY(actinfo.night_fight_rank_second, m_world_status2_param.common_data_2.night_fight_rank_second);
	ARRAYCPY(actinfo.night_fight_rank_third, m_world_status2_param.common_data_2.night_fight_rank_third);

	role->GetTitleManager()->OnRoomActivityTitleInfo(actinfo, reason);
}
void WorldStatus2::ClearMessBattleActiviyTopRank()
{
	int rank_first[CROSS_MESS_BATTLE_MAX_SCENE_NUM] = {0};
	int rank_second[CROSS_MESS_BATTLE_MAX_SCENE_NUM] = {0};
	int rank_third[CROSS_MESS_BATTLE_MAX_SCENE_NUM] = {0};

	ARRAYCPY(rank_first, m_world_status2_param.common_data_2.mess_battle_rank_first);
	ARRAYCPY(rank_second, m_world_status2_param.common_data_2.mess_battle_rank_second);
	ARRAYCPY(rank_third, m_world_status2_param.common_data_2.mess_battle_rank_third);
	m_world_status2_param.common_data_2.ResetMessbattleRankTop();
	for (int i = 0; i < CROSS_MESS_BATTLE_MAX_SCENE_NUM; ++i)
	{
		Role *fist_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(rank_first[i]));
		if (NULL != fist_role)
		{
			this->SyncRoomActivityTitleInfo(fist_role, SYNC_REASON_ACTIVITY);
		}

		Role *sec_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(rank_second[i]));
		if(NULL != sec_role)
		{
			this->SyncRoomActivityTitleInfo(sec_role, SYNC_REASON_ACTIVITY);
		}
		
		Role *third_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(rank_third[i]));
		if (NULL != third_role)
		{
			this->SyncRoomActivityTitleInfo(third_role, SYNC_REASON_ACTIVITY);
		}
	}
}
void WorldStatus2::SetMessBattleTopRankUid(int rank, int uid)
{
	if (rank < 1 || rank > MESS_BATTLE_TOP_RANK_MAX) return;

	if (1 == rank)
	{
		for (int i = 0; i < CROSS_MESS_BATTLE_MAX_SCENE_NUM; ++i)
		{
			if (0 == m_world_status2_param.common_data_2.mess_battle_rank_first[i] ||
				uid == m_world_status2_param.common_data_2.mess_battle_rank_first[i])
			{
				m_world_status2_param.common_data_2.mess_battle_rank_first[i] = uid;
				break;
			}
		}
	}
	else if(2 == rank)
	{
		for (int i = 0; i < CROSS_MESS_BATTLE_MAX_SCENE_NUM; ++i)
		{
			if (0 == m_world_status2_param.common_data_2.mess_battle_rank_second[i]||
				uid == m_world_status2_param.common_data_2.mess_battle_rank_second[i])
			{
				m_world_status2_param.common_data_2.mess_battle_rank_second[i] = uid;
				break;
			}
		}
	}
	else if (3 == rank)
	{
		for (int i = 0; i < CROSS_MESS_BATTLE_MAX_SCENE_NUM; ++i)
		{
			if (0 == m_world_status2_param.common_data_2.mess_battle_rank_third[i]||
				uid == m_world_status2_param.common_data_2.mess_battle_rank_third[i])
			{
				m_world_status2_param.common_data_2.mess_battle_rank_third[i] = uid;
				break;
			}
		}
	}

	Role *new_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (new_role != nullptr)
	{
		this->SyncRoomActivityTitleInfo(new_role,SYNC_REASON_ACTIVITY);
	}
}


void WorldStatus2::ClearNightFightActivityTopRank()
{
	int rank_first[CROSS_MESS_BATTLE_MAX_SCENE_NUM] = { 0 };
	int rank_second[CROSS_MESS_BATTLE_MAX_SCENE_NUM] = { 0 };
	int rank_third[CROSS_MESS_BATTLE_MAX_SCENE_NUM] = { 0 };

	ARRAYCPY(rank_first, m_world_status2_param.common_data_2.night_fight_rank_first);
	ARRAYCPY(rank_second, m_world_status2_param.common_data_2.night_fight_rank_second);
	ARRAYCPY(rank_third, m_world_status2_param.common_data_2.night_fight_rank_third);
	m_world_status2_param.common_data_2.ResetNightFightRankTop();
	for (int i = 0; i < CROSS_NIGHT_FIGHT_MAX_SCENE_NUM; ++i)
	{
		Role *fist_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(rank_first[i]));
		if (NULL != fist_role)
		{
			this->SyncRoomActivityTitleInfo(fist_role, SYNC_REASON_ACTIVITY);
		}

		Role *sec_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(rank_second[i]));
		if (NULL != sec_role)
		{
			this->SyncRoomActivityTitleInfo(sec_role, SYNC_REASON_ACTIVITY);
		}

		Role *third_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(rank_third[i]));
		if (NULL != third_role)
		{
			this->SyncRoomActivityTitleInfo(third_role, SYNC_REASON_ACTIVITY);
		}
	}
}
void WorldStatus2::SetNightFightTopRankUid(int rank, int uid)
{
	if (rank < 1 || rank > NIGHT_FIGHT_RANK_TOP_MAX) return;

	if (1 == rank)
	{
		for (int i = 0; i < CROSS_NIGHT_FIGHT_MAX_SCENE_NUM; ++i)
		{

			if (0 == m_world_status2_param.common_data_2.night_fight_rank_first[i] ||
				uid == m_world_status2_param.common_data_2.night_fight_rank_first[i])
			{
				m_world_status2_param.common_data_2.night_fight_rank_first[i] = uid;
				break;
			}
		}
	}
	else if (2 == rank)
	{
		for (int i = 0; i < CROSS_NIGHT_FIGHT_MAX_SCENE_NUM; ++i)
		{
			if (0 == m_world_status2_param.common_data_2.night_fight_rank_second[i] ||
				uid == m_world_status2_param.common_data_2.night_fight_rank_second[i])
			{
				m_world_status2_param.common_data_2.night_fight_rank_second[i] = uid;
				break;
			}
		}
	}
	else if (3 == rank)
	{
		for (int i = 0; i < CROSS_NIGHT_FIGHT_MAX_SCENE_NUM; ++i)
		{
			if (0 == m_world_status2_param.common_data_2.night_fight_rank_third[i] ||
				uid == m_world_status2_param.common_data_2.night_fight_rank_third[i])
			{
				m_world_status2_param.common_data_2.night_fight_rank_third[i] = uid;
				break;
			}
		}
	}

	Role *new_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (new_role != nullptr)
	{
		this->SyncRoomActivityTitleInfo(new_role, SYNC_REASON_ACTIVITY);
	}
}

void WorldStatus2::ClearCrossXiuluoTowerRankTitle()
{
	int rank[CROSS_ACTIVITY_XIULUO_TOWER_RANK_TITLE_NUM] = { 0 };
	ARRAYCPY(rank,m_world_status2_param.common_data_2.cross_xiuluotower_rank_title);
	memset(m_world_status2_param.common_data_2.cross_xiuluotower_rank_title, 0,sizeof(m_world_status2_param.common_data_2.cross_xiuluotower_rank_title));
	for (int i = 0; i < CROSS_ACTIVITY_XIULUO_TOWER_RANK_TITLE_NUM; ++i)
	{
		if(0== rank[i]) continue;
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(rank[i]));
		if (nullptr != role)
		{
			this->SyncCrossActivityTitleInfo(role, SYNC_REASON_ACTIVITY);
		}
	}
}
void WorldStatus2::SetCrossXiuluoTowerRankTitle(int rank, int uid)
{
	m_world_status2_param.common_data_2.cross_xiuluotower_rank_title[rank - 1] = uid;
	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (nullptr != role)
	{
		this->SyncCrossActivityTitleInfo(role, SYNC_REASON_ACTIVITY);
	}
}

void WorldStatus2::SyncCrossActivityTitleInfo(Role* role, int reason)
{
	if (nullptr == role) return;
	CrossActivityTitleInfo cati;
	ARRAYCPY(cati.cross_xiuluotower_rank_title, m_world_status2_param.common_data_2.cross_xiuluotower_rank_title);
	role->GetTitleManager()->OnCrossActivityTitleInfo(cati, reason);

	role->GetTitleManager()->OnChangeLieKunTitle(CrossLieKunFbConfig::Instance().GetOtherCfg().title_id, reason);
}

int WorldStatus2::GetTotalUpgradeQilinbiRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return 0;
	}
	return m_world_status2_param.common_data_2.total_upgrade_qilinbi_record_list[grade];
}



int WorldStatus2::GetTotalUpgradeToushiRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return 0;
	}
	return m_world_status2_param.common_data_2.total_upgrade_toushi_record_list[grade];
}

int WorldStatus2::GetTotalUpgradeYaoshiRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return 0;
	}
	return m_world_status2_param.common_data_2.total_upgrade_yaoshi_record_list[grade];
}

int WorldStatus2::GetTotalUpgradeFightMountRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return 0;
	}
	return m_world_status2_param.common_data_2.total_upgrade_fight_mount_record_list[grade];
}

int WorldStatus2::GetTotalUpgradeRecordList(int type, int grade)
{
	if (type < 0 || type >= UPGRADE_TYPE_MAX || type >= MAX_UPGRADE_TYPE)
	{
		return 0;
	}
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return 0;
	}
	return m_world_status2_param.common_data_2.total_upgrade_record_list[type][grade];
}

int WorldStatus2::GetTotalUpgradeFabaoRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return 0;
	}
	return m_world_status2_param.common_data_2.total_upgrade_fabao_record_list[grade];
}

int WorldStatus2::GetTotalUpgradeShizhuangRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return 0;
	}
	return m_world_status2_param.common_data_2.total_upgrade_shizhuang_record_list[grade];
}

int WorldStatus2::GetTotalUpgradeShenbingRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return 0;
	}
	return m_world_status2_param.common_data_2.total_upgrade_shenbing_record_list[grade];
}

int WorldStatus2::GetTotalUpgradeFootprintRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return 0;
	}
	return m_world_status2_param.common_data_2.total_upgrade_footprint_record_list[grade];
}

void WorldStatus2::IncTotalUpgradeQilinbiRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return;
	}
	++m_world_status2_param.common_data_2.total_upgrade_qilinbi_record_list[grade];
}

void WorldStatus2::IncTotalUpgradeToushiRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return;
	}
	++m_world_status2_param.common_data_2.total_upgrade_toushi_record_list[grade];
}

void WorldStatus2::IncTotalUpgradeYaoshiRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return;
	}
	++m_world_status2_param.common_data_2.total_upgrade_yaoshi_record_list[grade];
}

void WorldStatus2::IncTotalUpgradeFightMountRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return;
	}
	++m_world_status2_param.common_data_2.total_upgrade_fight_mount_record_list[grade];
}

void WorldStatus2::IncTotalUpgradeRecordList(int type, int grade)
{
	if (type < 0 || type >= UPGRADE_TYPE_MAX || type >= MAX_UPGRADE_TYPE)
	{
		return;
	}
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return;
	}
	++m_world_status2_param.common_data_2.total_upgrade_record_list[type][grade];
}

void WorldStatus2::IncTotalUpgradeFabaoRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return;
	}
	++m_world_status2_param.common_data_2.total_upgrade_fabao_record_list[grade];
}

void WorldStatus2::IncTotalUpgradeShizhuangRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return;
	}
	++m_world_status2_param.common_data_2.total_upgrade_shizhuang_record_list[grade];
}

void WorldStatus2::IncTotalUpgradeShenbingRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return;
	}
	++m_world_status2_param.common_data_2.total_upgrade_shenbing_record_list[grade];
}

void WorldStatus2::IncTotalUpgradeFootprintRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_LEVEL)
	{
		return;
	}
	++m_world_status2_param.common_data_2.total_upgrade_footprint_record_list[grade];
}

void WorldStatus2::GetImageCompetitionTypeAndId(int *type, int *id)
{
	if (type == nullptr || id == nullptr)
	{
		return;
	}

	*type = m_world_status2_param.common_data_2.image_competition_competition_type;
	*id = m_world_status2_param.common_data_2.image_competition_competition_image;
}

void WorldStatus2::SetImageCompetitionTypeAndId(int type, int id)
{
	m_world_status2_param.common_data_2.image_competition_competition_type = type;
	m_world_status2_param.common_data_2.image_competition_competition_image = id;
}

void WorldStatus2::SetLieKunTopRankGuildId(GuildID guild_id)
{
	m_world_status2_param.common_data_2.ling_kun_win_guild_id = guild_id;
}

void WorldStatus2::InitWorldAcquisitionLogList()
{
	// 全服收购
	m_world_acquisition_log_list.clear();
	for (int i = 0; i < MAX_SERVER_ACQUISITION_RECORD_NUM; i++)
	{
		auto & log = m_world_status2_param.world_acquisition_log_param.world_acquisition_log_list[i];
		if (log.IsValid())
		{
			m_world_acquisition_log_list.push_back(log);
		}
	}
}

void WorldStatus2::SaveWorldAcquisitionLogList()
{
	// 全服收购
	int log_index = 0; m_world_status2_param.world_acquisition_log_param.Reset();
	for (const auto &log : m_world_acquisition_log_list)
	{
		m_world_status2_param.world_acquisition_log_param.world_acquisition_log_list[log_index] = log;
		log_index++;
		if (log_index >= MAX_SERVER_ACQUISITION_RECORD_NUM)
		{
			break;
		}
	}
}

void WorldStatus2::AddWorldAcquisitionLog(const WorldAcquisitionLogItem &log)
{
	if (CrossConfig::Instance().IsHiddenServer()) return;
	if (!log.IsValid()) return;
	if (!ITEMPOOL->GetItem(log.item_id)) return;

	m_world_acquisition_log_list.push_back(log);
	if (m_world_acquisition_log_list.size() >= MAX_SERVER_ACQUISITION_RECORD_NUM)
	{
		m_world_acquisition_log_list.pop_front();
	}
}

void WorldStatus2::SendWorldAcquisitionLog(short notice_type, Role* role)
{
	static Protocol::SCWorldAcquisitionLog wal;

	wal.notice_type = notice_type;
	wal.count = 0;

	if (Protocol::SCWorldAcquisitionLog::NOTICE_TYPE_ADD == notice_type)
	{
		const auto it = m_world_acquisition_log_list.rbegin();
		if (it != m_world_acquisition_log_list.rend())
		{
			if (!it->IsValid()) return;
			UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(it->role_uid);
			if (NULL == uc)
			{
				return;
			}

			auto &log = wal.log_List[0]; log.Reset();

			uc->GetName(log.role_name);
			log.uuid = uc->uuid;
			log.log_type = it->log_type;
			log.log_str_id = it->log_str_id;
			log.item_id = it->item_id;
			log.timestamp = it->timestamp;
			UNSTD_STATIC_CHECK(sizeof(log.xianpin_type_list) == sizeof(it->xianpin_type_list));
			memcpy(log.xianpin_type_list, it->xianpin_type_list, sizeof(log.xianpin_type_list));

			wal.count = 1;
		}
	}
	else if (Protocol::SCWorldAcquisitionLog::NOTICE_TYPE_ALL == notice_type)
	{
		for (const auto &it : m_world_acquisition_log_list)
		{
			if (!it.IsValid()) continue;

			UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(it.role_uid);
			if (NULL == uc)
			{
				continue;
			}

			auto &log = wal.log_List[wal.count]; log.Reset();

			uc->GetName(log.role_name);
			log.uuid = uc->uuid;
			log.log_type = it.log_type;
			log.log_str_id = it.log_str_id;
			log.item_id = it.item_id;
			log.timestamp = it.timestamp;
			UNSTD_STATIC_CHECK(sizeof(log.xianpin_type_list) == sizeof(it.xianpin_type_list));
			memcpy(log.xianpin_type_list, it.xianpin_type_list, sizeof(log.xianpin_type_list));

			wal.count++;
			if (wal.count >= MAX_SERVER_ACQUISITION_RECORD_NUM) break;
		}
	}

	unsigned int sendlen = sizeof(wal) - sizeof(wal.log_List) + sizeof(wal.log_List[0]) * wal.count;
	if (nullptr == role)
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&wal, sendlen);
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&wal, sendlen);
	}
}

