#include <algorithm>
#include "personrank.hpp"

#include "rankmanager.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

#include "gamelog.h"
#include "engineadapter.h"
#include "internalcomm.h"
#include "obj/character/role.h"
#include "world.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/usercache/usercache.hpp"
#include "other/event/eventhandler.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "other/vip/vip.hpp"

PersonRank::PersonRank()
	: m_rank_manager(NULL), m_change_flag(false), m_load_flag(false), m_force_snapshot_time(0), m_last_snapshot_time(0), m_world_level(0), m_top_user_level(0), m_capability_rank_mincap(0)
{
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));
	memset(m_old_state, 0, sizeof(m_old_state));
}

PersonRank::~PersonRank()
{
}

void PersonRank::Init(const PersonRankParam &personrank_param)
{
	int count = 0;

	if (personrank_param.count < PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE)
	{
		count = personrank_param.count;
	}
	else 
	{
		count = PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE;
	}
	
	for (int i = count - 1; i >= 0; i--)
	{
		int rank_type = personrank_param.person_rank_list[i].rank_type;
		if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[PersonRank::Init INVALID RANK TYPE] [rank_type:%d]", (int)rank_type);
			continue;
		}

		int rank_index = personrank_param.person_rank_list[i].index;
		if (rank_index < 0 || rank_index >= PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[PersonRank::Init INVALID RANK INDEX] [rank_index:%d]", (int)rank_index);
			continue;
		}

		if (personrank_param.person_rank_list[i].uid <= 0) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[PersonRank::Init INVALID USER INFO] [user[%d %s]]", 
				(int)personrank_param.person_rank_list[i].uid, (const char*)personrank_param.person_rank_list[i].user_name);
			continue;
		}

		if (!m_rank_list[rank_index].Invalid()) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[PersonRank::Init RANK ITEM REPEAT] [rank_index:%d]", (int)rank_index);
			continue;
		}

		UserID user_id = IntToUid(personrank_param.person_rank_list[i].uid);

		m_rank_list[rank_index].user_id = user_id;
		F_STRNCPY(m_rank_list[rank_index].user_name, personrank_param.person_rank_list[i].user_name, sizeof(m_rank_list[0].user_name));
		F_STRNCPY(m_rank_list[rank_index].flexible_name, personrank_param.person_rank_list[i].flexible_name, sizeof(m_rank_list[0].flexible_name));

		m_rank_list[rank_index].sex = personrank_param.person_rank_list[i].sex;
		m_rank_list[rank_index].prof = personrank_param.person_rank_list[i].prof;
		m_rank_list[rank_index].camp = personrank_param.person_rank_list[i].camp;
		m_rank_list[rank_index].vip_level = personrank_param.person_rank_list[i].vip_level;

		m_rank_list[rank_index].exp = personrank_param.person_rank_list[i].exp;
		m_rank_list[rank_index].level = personrank_param.person_rank_list[i].level;
		m_rank_list[rank_index].rank_type = personrank_param.person_rank_list[i].rank_type;
		m_rank_list[rank_index].rank_value = personrank_param.person_rank_list[i].rank_value;
		m_rank_list[rank_index].record_index = personrank_param.person_rank_list[i].record_index;
		m_rank_list[rank_index].flexible_int = personrank_param.person_rank_list[i].flexible_int;
		m_rank_list[rank_index].flexible_ll = personrank_param.person_rank_list[i].flexible_ll;
	
		this->AddUserIndex(rank_type, user_id, rank_index);

		m_old_state[rank_index] = true;
	}

	m_load_flag = true;
	
	this->OnRankLoadFinish();
}

void PersonRank::Update(time_t now_second)
{
	if (!m_load_flag) return;

	if (now_second > m_last_snapshot_time + PERSON_RANK_SNAPSHOT_INTERVAL || 
		(0 != m_force_snapshot_time && now_second >= m_force_snapshot_time))
	{
		this->Snapshot();
	}
}

void PersonRank::OnUserLogin(Role *user)
{
	this->SendWorldLevelToUser(false, user);
}
//
//static int RankInfoSortCmp(const void *v1, const void *v2)
//{
//	PersonRankItem *item1 = *((PersonRankItem**)v1);
//	PersonRankItem *item2 = *((PersonRankItem**)v2);
//
//	if (NULL == item1 || NULL == item2) return false;
//
//	if (item2->rank_value == item1->rank_value)
//	{
//		return item2->reserved > item1->reserved ? 1 : -1;
//	}
//
//	return item2->rank_value > item1->rank_value ? 1 : -1;
//}

void PersonRank::Snapshot()
{
	m_last_snapshot_time = EngineAdapter::Instance().Time();
	m_force_snapshot_time = 0;

	this->RecalcWorldLevelInfo();

	static int old_snapshot_top_uid_list[PERSON_RANK_TYPE_MAX]; memset(old_snapshot_top_uid_list, 0, sizeof(old_snapshot_top_uid_list));
	{
		for (int rank_type = 0; rank_type < PERSON_RANK_TYPE_MAX; ++ rank_type)
		{
			int top_idx = rank_type * PERSON_RANK_MAX_SIZE;
			if (!m_rank_snapshot[top_idx].Invalid())
			{
				old_snapshot_top_uid_list[rank_type] = UidToInt(m_rank_snapshot[top_idx].user_id);
			}
		}
	}

	for (int rank_type = 0; rank_type < PERSON_RANK_TYPE_MAX; ++ rank_type)
	{
		if (!this->AllowSnapshot(rank_type))
		{
			continue;
		}

		int rank_start = rank_type * PERSON_RANK_MAX_SIZE;
		int rank_end =  rank_start + PERSON_RANK_MAX_SIZE;

		for (int i = rank_start; i < rank_end; ++ i)
		{
			m_rank_snapshot[i].Reset();
		}

		int temp_list_size = 0;
		static PersonRankItem *temp_list[PERSON_RANK_MAX_SIZE] = {NULL};
		memset(temp_list, 0, sizeof(temp_list));

		for (int i = rank_start; i < rank_end && temp_list_size < PERSON_RANK_MAX_SIZE; ++ i)
		{
			if (!m_rank_list[i].Invalid() && 0 != m_rank_list[i].rank_value)
			{
				temp_list[temp_list_size] = &m_rank_list[i];

				switch (rank_type)
				{
				case PERSON_RANK_TYPE_LEVEL:
					temp_list[temp_list_size]->reserved = m_rank_list[i].exp;
						break;
				case PERSON_RANK_TYPE_MOUNT:
				case PERSON_RANK_TYPE_WING:
				case PERSON_RANK_TYPE_HALO:
				case PERSON_RANK_TYPE_FABAO:
				case PERSON_RANK_TYPE_SHIZHUANG:
				case PERSON_RANK_TYPE_SHIZHUANG_WUQI:
				case PERSON_RANK_TYPE_FOOTPRINT:
				case PERSON_RANK_TYPE_SHENGONG:
				case PERSON_RANK_TYPE_SHENYI:
				case PERSON_RANK_TYPE_FIGHT_MOUNT:
				case PERSON_RANK_TYPE_YAOSHI:
				case PERSON_RANK_TYPE_TOUSHI:
				case PERSON_RANK_TYPE_QILINBI:
				case PERSON_RANK_TYPE_MASK:
				case PERSON_RANK_TYPE_LINGZHU:
				case PERSON_RANK_TYPE_XIANBAO:
				case PERSON_RANK_TYPE_LINGTONG:
				case PERSON_RANK_TYPE_LINGGONG:
				case PERSON_RANK_TYPE_LINGQI:
				case PERSON_RANK_TYPE_WEIYAN:
				case PERSON_RANK_TYPE_SHOUHUAN:
				case PERSON_RANK_TYPE_WEIBA:
				case PERSON_RANK_TYPE_FEICHONG:
				case PERSON_RANK_TYPE_CLOAK:
				case PERSON_RANK_TYPE_SHENBING:
					temp_list[temp_list_size]->reserved = m_rank_list[i].flexible_int;
					break;
				case PERSON_RANK_TYPE_RA_CHONGZHI:
				case PERSON_RANK_TYPE_RA_CHONGZHI2:
				case PERSON_RANK_TYPE_RA_CONSUME_GOLD:
				case PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP1:
				case PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP2:
				case PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP3:
				case PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP1:
				case PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP2:
				case PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP3:
				case PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP1:
				case PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP2:
				case PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP3:
				case PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP1:
				case PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP2:
				case PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP3:
				case PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP1:
				case PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP2:
				case PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP3:
				case PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP1:
				case PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP2:
				case PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP3:
				case PERSON_RANK_TYPE_RA_DAY_CHONGZHI_NUM:
				case PERSON_RANK_TYPE_RA_DAY_CONSUME_NUM:
				case PERSON_RANK_TYPE_WORLD_RIGHT_ANSWER:
				case PERSON_RANK_FISH_PLACE:
				case PERSON_RANK_STEAL_FISH_PLACE:
				case PERSON_RANK_TYPE_PLANTING_TREE_PLANTING:
				case PERSON_RANK_TYPE_PLANTING_TREE_WATERING:
				case PERSON_RANK_CRYSTA_DALARAN_STEAL_NUM:
				case PERSON_RANK_CRYSTA_DALARAN_BE_STEALED_NUM:
				case PERSON_RANK_TYPE_RA_PROFESS_MALE:
				case PERSON_RANK_TYPE_RA_PROFESS_FEMALE:
					temp_list[temp_list_size]->reserved = m_rank_list[i].flexible_ll;
					break;

				default:
					temp_list[temp_list_size]->reserved = PERSON_RANK_MAX_SIZE - i;
					break;
				}

				++ temp_list_size;
			}
		}

//		qsort(temp_list, temp_list_size, sizeof(temp_list[0]), RankInfoSortCmp);
		std::stable_sort(std::begin(temp_list), std::end(temp_list), [](const PersonRankItem* a, const PersonRankItem* b) -> bool {
			if (!a || !b)
			{
				return false;
			}

			if (a->rank_value > b->rank_value)
			{
				return true;
			}else if (a->rank_value == b->rank_value && a->reserved > b->reserved)
			{
				return true;
			}

			return false;
		});

		struct TopRankData
		{
			TopRankData() { this->Reset(); }
			void Reset() { user_id = INVALID_USER_ID; rank_value = 0; }

			UserID user_id; 
			long long rank_value;
		};

		int top_user_info_count = 0;
		const static int LOG_RECORD_TOP_RANK_USER_NUM = 10;
		static TopRankData top_user_info_list[LOG_RECORD_TOP_RANK_USER_NUM]; 

		for (int i = 0; i < LOG_RECORD_TOP_RANK_USER_NUM; ++ i)
		{
			top_user_info_list[i].Reset();
		}

		for (int i = 0; i < temp_list_size && i < PERSON_RANK_MAX_SIZE && rank_start + i < rank_end; ++ i)
		{
			if (NULL != temp_list[i])
			{
				if (temp_list[i]->user_id != m_rank_snapshot[rank_start + i].user_id)
				{
					EventHandler::Instance().OnPersonRankChange(temp_list[i]->user_id, i, temp_list[i]->rank_type, temp_list[i]->rank_value);
				}
				m_rank_snapshot[rank_start + i] = *temp_list[i];

				if (top_user_info_count < LOG_RECORD_TOP_RANK_USER_NUM)
				{
					top_user_info_list[top_user_info_count].user_id = m_rank_snapshot[rank_start + i].user_id;
					top_user_info_list[top_user_info_count].rank_value = m_rank_snapshot[rank_start + i].rank_value;
					++ top_user_info_count;
				}
			}
		}

		if (this->RankTypeNeedLog(rank_type))
		{
			UNSTD_STATIC_CHECK(LOG_RECORD_TOP_RANK_USER_NUM >= 10);

			gamelog::g_log_rank.printf(LL_INFO, 
				"PersonRank::Snapshot person_rank_type:%d, uid_value_list:"
				"(%d, %lld), (%d, %lld), (%d, %lld), (%d, %lld), (%d, %lld), (%d, %lld), (%d, %lld), (%d, %lld), (%d, %lld), (%d, %lld), ", 
				rank_type, 
				UidToInt(top_user_info_list[0].user_id), top_user_info_list[0].rank_value,
				UidToInt(top_user_info_list[1].user_id), top_user_info_list[1].rank_value,
				UidToInt(top_user_info_list[2].user_id), top_user_info_list[2].rank_value,
				UidToInt(top_user_info_list[3].user_id), top_user_info_list[3].rank_value,
				UidToInt(top_user_info_list[4].user_id), top_user_info_list[4].rank_value,
				UidToInt(top_user_info_list[5].user_id), top_user_info_list[5].rank_value,
				UidToInt(top_user_info_list[6].user_id), top_user_info_list[6].rank_value,
				UidToInt(top_user_info_list[7].user_id), top_user_info_list[7].rank_value,
				UidToInt(top_user_info_list[8].user_id), top_user_info_list[8].rank_value,
				UidToInt(top_user_info_list[9].user_id), top_user_info_list[9].rank_value);
		}
	}

	static int new_snapshot_top_uid_list[PERSON_RANK_TYPE_MAX]; memset(new_snapshot_top_uid_list, 0, sizeof(new_snapshot_top_uid_list));
	{
		for (int rank_type = 0; rank_type < PERSON_RANK_TYPE_MAX; ++ rank_type)
		{
			int top_idx = rank_type * PERSON_RANK_MAX_SIZE;
			if (!m_rank_snapshot[top_idx].Invalid())
			{
				new_snapshot_top_uid_list[rank_type] = UidToInt(m_rank_snapshot[top_idx].user_id);
			}
		}
	}

	EventHandler::Instance().OnPersonRankSnapshot(old_snapshot_top_uid_list, new_snapshot_top_uid_list);
}

void PersonRank::ForceSnapshot()
{
	if (0 == m_force_snapshot_time)
	{
		const static int FORCE_SNAPSHOT_MIN_INTERVAL = 3;

		m_force_snapshot_time = EngineAdapter::Instance().Time() + FORCE_SNAPSHOT_MIN_INTERVAL;
	}
}

void PersonRank::GetChangeRankItemList(PersonRankParam *personrank_param)
{
	if (NULL == personrank_param) return;

	if (!m_change_flag)
	{
		personrank_param->count = 0;
		return;
	}
	
	int count = 0;
	for (int i = 0; i < PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE && count < PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (m_rank_list[i].Invalid())
			{
				if (!m_old_state[i]) continue;

				personrank_param->person_rank_list[count].change_state = structcommon::CS_DELETE;
				personrank_param->person_rank_list[count].index = i;
			}
			else
			{
				personrank_param->person_rank_list[count].change_state = m_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT;
				personrank_param->person_rank_list[count].index = i;
				
				personrank_param->person_rank_list[count].uid = UidToInt(m_rank_list[i].user_id);
				F_STRNCPY(personrank_param->person_rank_list[count].user_name, m_rank_list[i].user_name, sizeof(personrank_param->person_rank_list[0].user_name));
				
				F_STRNCPY(personrank_param->person_rank_list[count].flexible_name, m_rank_list[i].flexible_name, sizeof(personrank_param->person_rank_list[0].flexible_name));
				
				personrank_param->person_rank_list[count].sex = m_rank_list[i].sex;
				personrank_param->person_rank_list[count].prof = m_rank_list[i].prof;
				personrank_param->person_rank_list[count].camp = m_rank_list[i].camp;
				personrank_param->person_rank_list[count].vip_level = m_rank_list[i].vip_level;
				
				personrank_param->person_rank_list[count].exp = m_rank_list[i].exp;
				personrank_param->person_rank_list[count].level = m_rank_list[i].level;
				personrank_param->person_rank_list[count].rank_type = m_rank_list[i].rank_type;
				personrank_param->person_rank_list[count].rank_value = m_rank_list[i].rank_value;
				personrank_param->person_rank_list[count].record_index = m_rank_list[i].record_index;
				personrank_param->person_rank_list[count].flexible_int = m_rank_list[i].flexible_int;
				personrank_param->person_rank_list[count].flexible_ll = m_rank_list[i].flexible_ll;
			}

			++count;
		}
	}

	personrank_param->count = count;
}

void PersonRank::ClearDirtyMark()
{
	if (!m_change_flag) return;

	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	for (int i = 0; i < PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE; ++i)
	{
		m_old_state[i] = !m_rank_list[i].Invalid();
	}

	m_change_flag = false;
}

void PersonRank::OnGetRankList(Role *role, Protocol::CSGetPersonRankList &rank_list_req)
{
	if (!m_load_flag) return;
	if (NULL == role) return;

	int rank_type = rank_list_req.rank_type;
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return;

	//if (!user.GetUserRegister()->CheckAndUpdatePersonRankGetStamp(rank_type, m_last_snapshot_time)) return;

	static Protocol::SCGetPersonRankListAck rank_list_ack;
	rank_list_ack.last_snapshot_time = static_cast<unsigned int>(m_last_snapshot_time);
	rank_list_ack.rank_type = rank_type;
	rank_list_ack.count = 0;

	int rank_start = rank_type * PERSON_RANK_MAX_SIZE;
	int rank_end =  rank_start + PERSON_RANK_MAX_SIZE;

	int rank_count = 0;

	for (int i = rank_start; i < rank_end && i < PERSON_RANK_MAX_SIZE * PERSON_RANK_TYPE_MAX && rank_count < PERSON_RANK_MAX_SIZE; ++i)
	{
		if (m_rank_snapshot[i].Invalid()) continue;

		rank_list_ack.rank_list[rank_count].user_id = UidToInt(m_rank_snapshot[i].user_id);
		F_STRNCPY(rank_list_ack.rank_list[rank_count].user_name, m_rank_snapshot[i].user_name, sizeof(rank_list_ack.rank_list[0].user_name));
		F_STRNCPY(rank_list_ack.rank_list[rank_count].flexible_name, m_rank_snapshot[i].flexible_name, sizeof(rank_list_ack.rank_list[0].flexible_name));
		rank_list_ack.rank_list[rank_count].sex = m_rank_snapshot[i].sex;
		rank_list_ack.rank_list[rank_count].prof = m_rank_snapshot[i].prof;
		rank_list_ack.rank_list[rank_count].camp = m_rank_snapshot[i].camp;
		rank_list_ack.rank_list[rank_count].vip_level = static_cast<char>(m_rank_snapshot[i].vip_level);
		rank_list_ack.rank_list[rank_count].record_index = static_cast<short>(m_rank_snapshot[i].record_index);

		rank_list_ack.rank_list[rank_count].exp = m_rank_snapshot[i].exp;
		rank_list_ack.rank_list[rank_count].level = m_rank_snapshot[i].level;
		rank_list_ack.rank_list[rank_count].rank_value = m_rank_snapshot[i].rank_value;
		rank_list_ack.rank_list[rank_count].flexible_int = m_rank_snapshot[i].flexible_int;
		rank_list_ack.rank_list[rank_count].flexible_ll = m_rank_snapshot[i].flexible_ll;
		rank_list_ack.rank_list[rank_count].jingjie_level= m_rank_snapshot[i].jingjie_level;
		rank_list_ack.rank_list[rank_count].avatar_timestamp = 0;

		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(rank_list_ack.rank_list[rank_count].user_id);
		if (NULL != user_node)
		{
			rank_list_ack.rank_list[rank_count].avatar_timestamp = user_node->avatar_timestamp;
		}

		++rank_count;
	}


	rank_list_ack.count = rank_count;
	int sendlen = sizeof(rank_list_ack) - (PERSON_RANK_MAX_SIZE - rank_list_ack.count) * sizeof(rank_list_ack.rank_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&rank_list_ack, sendlen);
}

void PersonRank::OnGetRealTimeRankList(Role *role, int rank_type)
{
	if (!m_load_flag) return;

	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return;

	static Protocol::SCGetPersonRankListAck rank_list_ack;
	rank_list_ack.last_snapshot_time = static_cast<unsigned int>(m_last_snapshot_time);
	rank_list_ack.rank_type = rank_type;
	rank_list_ack.count = 0;

	int rank_start = rank_type * PERSON_RANK_MAX_SIZE;
	int rank_end =  rank_start + PERSON_RANK_MAX_SIZE;

	int rank_count = 0;

	for (int i = rank_start; i < rank_end && i < PERSON_RANK_MAX_SIZE * PERSON_RANK_TYPE_MAX && rank_count < PERSON_RANK_MAX_SIZE; ++i)
	{
		if (m_rank_list[i].Invalid()) continue;

		rank_list_ack.rank_list[rank_count].user_id = UidToInt(m_rank_list[i].user_id);
		F_STRNCPY(rank_list_ack.rank_list[rank_count].user_name, m_rank_list[i].user_name, sizeof(rank_list_ack.rank_list[0].user_name));
		F_STRNCPY(rank_list_ack.rank_list[rank_count].flexible_name, m_rank_list[i].flexible_name, sizeof(rank_list_ack.rank_list[0].flexible_name));
		rank_list_ack.rank_list[rank_count].sex = m_rank_list[i].sex;
		rank_list_ack.rank_list[rank_count].prof = m_rank_list[i].prof;
		rank_list_ack.rank_list[rank_count].camp = m_rank_list[i].camp;
		rank_list_ack.rank_list[rank_count].vip_level = static_cast<char>(m_rank_list[i].vip_level);
		rank_list_ack.rank_list[rank_count].record_index = static_cast<short>(m_rank_list[i].record_index);

		rank_list_ack.rank_list[rank_count].exp = m_rank_list[i].exp;
		rank_list_ack.rank_list[rank_count].level = m_rank_list[i].level;
		rank_list_ack.rank_list[rank_count].rank_value = m_rank_list[i].rank_value;
		rank_list_ack.rank_list[rank_count].flexible_int = m_rank_list[i].flexible_int;
		rank_list_ack.rank_list[rank_count].flexible_ll = m_rank_list[i].flexible_ll;

		++rank_count;
	}

	rank_list_ack.count = rank_count;
	int sendlen = sizeof(rank_list_ack) - (PERSON_RANK_MAX_SIZE - rank_list_ack.count) * sizeof(rank_list_ack.rank_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&rank_list_ack, sendlen);
}

void PersonRank::OnGetRankTopUser(Role *role, Protocol::CSGetPersonRankTopUser &top_user_req)
{
	if (!m_load_flag) return;

	int rank_type = top_user_req.rank_type;
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return;

	int rank_start = rank_type * PERSON_RANK_MAX_SIZE;
	int rank_end =  rank_start + PERSON_RANK_MAX_SIZE;

	PersonRankItem top_user_item;

	for (int i = rank_start; i < rank_end; ++i)
	{
		if (!m_rank_list[i].Invalid() && m_rank_list[i].rank_value > top_user_item.rank_value)
		{
			top_user_item = m_rank_list[i];
		}
	}

	static Protocol::SCGetPersonRankTopUser top_user_ack;
	top_user_ack.rank_type = rank_type;
	top_user_ack.user_id = UidToInt(top_user_item.user_id);
	F_STRNCPY(top_user_ack.user_name, top_user_item.user_name, sizeof(top_user_item.user_name));
	top_user_ack.sex = top_user_item.sex;
	top_user_ack.prof = top_user_item.prof;
	top_user_ack.camp = top_user_item.camp;
	top_user_ack.reserved = 0;
	top_user_ack.level = top_user_item.level;
	top_user_ack.rank_value = top_user_item.rank_value;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&top_user_ack, sizeof(top_user_ack));
}

void PersonRank::OnBaseInfoChange(Role *role)
{
	for (int rank_type = 0; rank_type < PERSON_RANK_TYPE_MAX; ++rank_type)
	{
		UserIndexMapIt it = m_user_index_map[rank_type].find(role->GetUserId());
		if (it != m_user_index_map[rank_type].end())
		{
			for (std::set<int>::iterator index_it = it->second.begin(), index_end = it->second.end(); index_it != index_end;)
			{
				int rank_index = *index_it;

				if (this->IsValidIndex(rank_index))
				{
					if ((role->GetSex() != m_rank_list[rank_index].sex && this->SensitiveToSex(rank_type)) ||
						(role->GetCamp() != m_rank_list[rank_index].camp && this->SensitiveToCamp(rank_type)))
					{
						m_rank_list[rank_index].Reset();

						it->second.erase(index_it ++);
					}
					else
					{
						this->UpdateBaseInfo(m_rank_list[rank_index], role);
						++ index_it;
					}
					
					m_change_flag = true;
					m_dirty_mark[rank_index] = true;
				}
				else
				{
					++ index_it;
				}
			}
		}
	}
}

void PersonRank::OnRankInfoChange(const UserID &user_id, SyncPersonRankInfo &rank_info)
{
	if (rank_info.db_index < 0 || rank_info.role_id <= 0 || rank_info.item_count <= 0) return;

	PersonRankItem rank_item;
	rank_item.user_id = user_id;
	rank_item.exp = rank_info.exp;
	rank_item.level = rank_info.level;
	F_STRNCPY(rank_item.user_name, rank_info.user_name, sizeof(rank_item.user_name));
	rank_item.sex = rank_info.sex;
	rank_item.prof = rank_info.prof;
	rank_item.camp = rank_info.camp;
	rank_item.vip_level = rank_info.vip_level;
	rank_item.jingjie_level = rank_info.jingjie_level;

	for (int i = 0; i < rank_info.item_count && i < SYNC_PERSON_RANK_INFO_SIZE; ++i)
	{
		rank_item.rank_type = rank_info.items[i].rank_type;
		rank_item.rank_value = rank_info.items[i].rank_value;
		rank_item.record_index = rank_info.items[i].record_index;
		F_STRNCPY(rank_item.flexible_name, rank_info.items[i].flexible_name, sizeof(rank_item.flexible_name));
		rank_item.flexible_int = rank_info.items[i].flexible_int;
		rank_item.flexible_ll = rank_info.items[i].flexible_ll;
		this->UpdateRankList(rank_item);
	}
}

void PersonRank::OnLastBattleFieldRankChange(gglobalprotocal::SyncLastBattleFieldRankInfo &rank_info)
{
	if (rank_info.rank_count <= 0) return;

	PersonRankItem rank_item;
	rank_item.rank_type = rank_info.rank_type;

	for (int i = 0; i < rank_info.rank_count && i < PERSON_RANK_MAX_SIZE; ++i)
	{
		rank_item.user_id = rank_info.rank_list[i].user_id;
		rank_item.sex = rank_info.rank_list[i].sex;
		rank_item.prof = rank_info.rank_list[i].prof;
		rank_item.camp = rank_info.rank_list[i].camp;
		rank_item.level = rank_info.rank_list[i].level;
		rank_item.rank_value = rank_info.rank_list[i].rank_value;
		rank_item.flexible_ll = rank_info.rank_list[i].flexible_ll;
		rank_item.flexible_int = rank_info.rank_list[i].flexible_int;
		rank_item.reserved = rank_info.rank_list[i].reserved;
		F_STRNCPY(rank_item.user_name, rank_info.rank_list[i].user_name, sizeof(rank_item.user_name));
		F_STRNCPY(rank_item.flexible_name, rank_info.rank_list[i].flexible_name, sizeof(rank_item.flexible_name));
		this->UpdateRankList(rank_item);
	}
}

void PersonRank::OnCrossRankInfoReq(crossgameprotocal::CrossGameRankInfoReq &cross_rank_info_req)
{
	if (cross_rank_info_req.rank_type < 0 || cross_rank_info_req.rank_type >= PERSON_RANK_TYPE_MAX) return;

	if (0 == cross_rank_info_req.is_personal_rank_top_user)
	{
		if (!m_load_flag) return;

		int rank_type = cross_rank_info_req.rank_type;
		if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return;

		static Protocol::SCGetPersonRankListAck rank_list_ack;
		rank_list_ack.last_snapshot_time = static_cast<unsigned int>(m_last_snapshot_time);
		rank_list_ack.rank_type = rank_type;
		rank_list_ack.count = 0;

		int rank_start = rank_type * PERSON_RANK_MAX_SIZE;
		int rank_end =  rank_start + PERSON_RANK_MAX_SIZE;

		int rank_count = 0;

		for (int i = rank_start; i < rank_end && i < PERSON_RANK_MAX_SIZE * PERSON_RANK_TYPE_MAX && rank_count < PERSON_RANK_MAX_SIZE; ++i)
		{
			if (m_rank_snapshot[i].Invalid()) continue;

			rank_list_ack.rank_list[rank_count].user_id = UidToInt(m_rank_snapshot[i].user_id);
			F_STRNCPY(rank_list_ack.rank_list[rank_count].user_name, m_rank_snapshot[i].user_name, sizeof(rank_list_ack.rank_list[0].user_name));
			F_STRNCPY(rank_list_ack.rank_list[rank_count].flexible_name, m_rank_snapshot[i].flexible_name, sizeof(rank_list_ack.rank_list[0].flexible_name));
			rank_list_ack.rank_list[rank_count].sex = m_rank_snapshot[i].sex;
			rank_list_ack.rank_list[rank_count].prof = m_rank_snapshot[i].prof;
			rank_list_ack.rank_list[rank_count].camp = m_rank_snapshot[i].camp;
			rank_list_ack.rank_list[rank_count].vip_level = static_cast<char>(m_rank_snapshot[i].vip_level);
			rank_list_ack.rank_list[rank_count].record_index = static_cast<short>(m_rank_snapshot[i].record_index);

			rank_list_ack.rank_list[rank_count].exp = m_rank_snapshot[i].exp;
			rank_list_ack.rank_list[rank_count].level = m_rank_snapshot[i].level;
			rank_list_ack.rank_list[rank_count].rank_value = m_rank_snapshot[i].rank_value;
			rank_list_ack.rank_list[rank_count].flexible_int = m_rank_snapshot[i].flexible_int;
			rank_list_ack.rank_list[rank_count].flexible_ll = m_rank_snapshot[i].flexible_ll;
			rank_list_ack.rank_list[rank_count].avatar_timestamp = 0;

			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(rank_list_ack.rank_list[rank_count].user_id);
			if (NULL != user_node)
			{
				rank_list_ack.rank_list[rank_count].avatar_timestamp = user_node->avatar_timestamp;
			}

			++rank_count;
		}


		rank_list_ack.count = rank_count;
		int sendlen = sizeof(rank_list_ack) - (PERSON_RANK_MAX_SIZE - rank_list_ack.count) * sizeof(rank_list_ack.rank_list[0]);

		InternalComm::Instance().UserProtocalTransferToGame(cross_rank_info_req.uuid, (const char*)&rank_list_ack, sendlen);
	}
	else
	{
		int rank_type = cross_rank_info_req.rank_type;
		if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return;

		int rank_start = rank_type * PERSON_RANK_MAX_SIZE;
		int rank_end =  rank_start + PERSON_RANK_MAX_SIZE;

		PersonRankItem top_user_item;

		for (int i = rank_start; i < rank_end; ++i)
		{
			if (!m_rank_list[i].Invalid() && m_rank_list[i].rank_value > top_user_item.rank_value)
			{
				top_user_item = m_rank_list[i];
			}
		}

		static Protocol::SCGetPersonRankTopUser top_user_ack;
		top_user_ack.rank_type = rank_type;
		top_user_ack.user_id = UidToInt(top_user_item.user_id);
		F_STRNCPY(top_user_ack.user_name, top_user_item.user_name, sizeof(top_user_item.user_name));
		top_user_ack.sex = top_user_item.sex;
		top_user_ack.prof = top_user_item.prof;
		top_user_ack.camp = top_user_item.camp;
		top_user_ack.reserved = 0;
		top_user_ack.level = top_user_item.level;
		top_user_ack.rank_value = top_user_item.rank_value;

		InternalComm::Instance().UserProtocalTransferToGame(cross_rank_info_req.uuid, (const char*)&top_user_ack, sizeof(top_user_ack));
	}
}

void PersonRank::ClearRank(int rank_type)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return;

	int rank_start = rank_type * PERSON_RANK_MAX_SIZE;
	int rank_end =  rank_start + PERSON_RANK_MAX_SIZE;

	for (int i = rank_start; i < rank_end; ++i)
	{
		m_rank_list[i].Reset();
		m_dirty_mark[i] = true;
	}

	m_user_index_map[rank_type].clear();
	m_change_flag = true;
}

int PersonRank::GetUserRank(UserID user_id, int rank_type)
{
	if (INVALID_USER_ID == user_id || rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX)
	{
		return 0;
	}

	int rank_value = 0;
	UserID user_id_list[PERSON_RANK_MAX_SIZE];
	int rank_count = this->GetRankTopUser(rank_type, PERSON_RANK_MAX_SIZE, user_id_list);
	for (int rank_index = 0; rank_index < rank_count && rank_index < PERSON_RANK_MAX_SIZE; rank_index++)
	{
		if (user_id == user_id_list[rank_index])
		{
			rank_value = rank_index + 1;
			break;
		}
	}

	return rank_value;
}

bool PersonRank::GetRankUserInfo(int rank_type, const UserID &user_id, GameName user_name, char *user_sex, char *camp, int *level)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX || NULL == user_name || NULL == user_sex) return false;

	UserIndexMapIt it = m_user_index_map[rank_type].find(user_id);
	if (it != m_user_index_map[rank_type].end())
	{
		for (std::set<int>::iterator index_it = it->second.begin(), index_end = it->second.end(); index_it != index_end; ++ index_it)
		{
			int rank_index = *index_it;
			if (this->IsValidIndex(rank_index))
			{
				F_STRNCPY(user_name, m_rank_list[rank_index].user_name, sizeof(GameName));
				*user_sex = m_rank_list[rank_index].sex;
				if (NULL != camp) *camp = m_rank_list[rank_index].camp;
				if (NULL != level) *level = m_rank_list[rank_index].level;

				return true;
			}
		}
	}

	return false;
}

const PersonRankItem * PersonRank::GetPersonRankItem(int rank_type, const UserID &user_id, int index)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return NULL;

	UserIndexMapIt it = m_user_index_map[rank_type].find(user_id);
	if (it != m_user_index_map[rank_type].end())
	{
		if (index >= (int)(it->second.size())) return NULL;

		std::set<int>::iterator index_it = it->second.begin();
		std::advance(index_it, index);
		int rank_index = *index_it;

		if (this->IsValidIndex(rank_index))
		{
			return &m_rank_list[rank_index];
		}
	}

	return NULL;
}

int PersonRank::GetRankList(int rank_type, int count, PersonRankItem *rank_list)
{
	if (NULL == rank_list || count <= 0 || count > PERSON_RANK_MAX_SIZE) return 0;
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return 0;

	int rank_start = rank_type * PERSON_RANK_MAX_SIZE;
	int rank_end =  rank_start + PERSON_RANK_MAX_SIZE;

	int rank_count = 0;

	for (int i = rank_start; i < rank_end && i < PERSON_RANK_MAX_SIZE * PERSON_RANK_TYPE_MAX && rank_count < PERSON_RANK_MAX_SIZE && rank_count < count; ++i)
	{
		if (m_rank_snapshot[i].Invalid()) continue;

		rank_list[rank_count].user_id = m_rank_snapshot[i].user_id;
		F_STRNCPY(rank_list[rank_count].user_name, m_rank_snapshot[i].user_name, sizeof(rank_list[0].user_name));
		F_STRNCPY(rank_list[rank_count].flexible_name, m_rank_snapshot[i].flexible_name, sizeof(rank_list[0].flexible_name));
		rank_list[rank_count].sex = m_rank_snapshot[i].sex;
		rank_list[rank_count].prof = m_rank_snapshot[i].prof;
		rank_list[rank_count].camp = m_rank_snapshot[i].camp;
		rank_list[rank_count].vip_level = static_cast<char>(m_rank_snapshot[i].vip_level);
		rank_list[rank_count].record_index = static_cast<short>(m_rank_snapshot[i].record_index);

		rank_list[rank_count].exp = m_rank_snapshot[i].exp;
		rank_list[rank_count].level = m_rank_snapshot[i].level;
		rank_list[rank_count].rank_value = m_rank_snapshot[i].rank_value;
		rank_list[rank_count].flexible_int = m_rank_snapshot[i].flexible_int;
		rank_list[rank_count].flexible_ll = m_rank_snapshot[i].flexible_ll;

		++rank_count;
	}

	return rank_count;
}

PersonRankItem *  PersonRank::GetTopOneItemByRankType(int rank_type)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return 0;

	int rank_start = rank_type * PERSON_RANK_MAX_SIZE;
	if (INVALID_USER_ID == m_rank_snapshot[rank_start].user_id)
	{
		return NULL;
	}

	return &m_rank_snapshot[rank_start];
}

int PersonRank::GetRankTopUser(int rank_type, int max_user, UserID *user_list, int camp_limit)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX || max_user <= 0 || max_user > PERSON_RANK_MAX_SIZE || NULL == user_list) return 0;

	int rank_start = rank_type * PERSON_RANK_MAX_SIZE;
	int rank_end =  rank_start + PERSON_RANK_MAX_SIZE;

	static PersonRankItem *temp_list[PERSON_RANK_MAX_SIZE] = {NULL};
	memset(temp_list, 0, sizeof(temp_list));

	int temp_list_size = 0;
	for (int i = rank_start; i < rank_end && temp_list_size < PERSON_RANK_MAX_SIZE; ++i)
	{
		if (!m_rank_list[i].Invalid() && 0 != m_rank_list[i].rank_value)
		{
			temp_list[temp_list_size] = &m_rank_list[i];

			switch (rank_type)
			{
			case PERSON_RANK_TYPE_LEVEL:
				temp_list[temp_list_size]->reserved = m_rank_list[i].exp;
				break;
			case PERSON_RANK_TYPE_MOUNT:
			case PERSON_RANK_TYPE_WING:
			case PERSON_RANK_TYPE_HALO:
			case PERSON_RANK_TYPE_FABAO:
			case PERSON_RANK_TYPE_SHIZHUANG:
			case PERSON_RANK_TYPE_SHIZHUANG_WUQI:
			case PERSON_RANK_TYPE_FOOTPRINT:
			case PERSON_RANK_TYPE_SHENGONG:
			case PERSON_RANK_TYPE_SHENYI:
			case PERSON_RANK_TYPE_FIGHT_MOUNT:
			case PERSON_RANK_TYPE_YAOSHI:
			case PERSON_RANK_TYPE_TOUSHI:
			case PERSON_RANK_TYPE_QILINBI:
			case PERSON_RANK_TYPE_MASK:
			case PERSON_RANK_TYPE_LINGZHU:
			case PERSON_RANK_TYPE_XIANBAO:
			case PERSON_RANK_TYPE_LINGTONG:
			case PERSON_RANK_TYPE_LINGGONG:
			case PERSON_RANK_TYPE_LINGQI:
			case PERSON_RANK_TYPE_WEIYAN:
			case PERSON_RANK_TYPE_SHOUHUAN:
			case PERSON_RANK_TYPE_WEIBA:
			case PERSON_RANK_TYPE_FEICHONG:
			case PERSON_RANK_TYPE_CLOAK:
			case PERSON_RANK_TYPE_SHENBING:
				temp_list[temp_list_size]->reserved = m_rank_list[i].flexible_int;
				break;
			case PERSON_RANK_TYPE_RA_CHONGZHI:
			case PERSON_RANK_TYPE_RA_CHONGZHI2:
			case PERSON_RANK_TYPE_RA_CONSUME_GOLD:
			case PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP1:
			case PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP2:
			case PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP3:
			case PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP1:
			case PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP2:
			case PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP3:
			case PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP1:
			case PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP2:
			case PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP3:
			case PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP1:
			case PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP2:
			case PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP3:
			case PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP1:
			case PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP2:
			case PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP3:
			case PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP1:
			case PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP2:
			case PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP3:
			case PERSON_RANK_TYPE_RA_DAY_CHONGZHI_NUM:
			case PERSON_RANK_TYPE_RA_DAY_CONSUME_NUM:
			case PERSON_RANK_TYPE_WORLD_RIGHT_ANSWER:
			case PERSON_RANK_FISH_PLACE:
			case PERSON_RANK_STEAL_FISH_PLACE:
			case PERSON_RANK_TYPE_PLANTING_TREE_PLANTING:
			case PERSON_RANK_TYPE_PLANTING_TREE_WATERING:
			case PERSON_RANK_CRYSTA_DALARAN_STEAL_NUM:
			case PERSON_RANK_CRYSTA_DALARAN_BE_STEALED_NUM:
			case PERSON_RANK_TYPE_RA_PROFESS_MALE:
			case PERSON_RANK_TYPE_RA_PROFESS_FEMALE:
				temp_list[temp_list_size]->reserved = m_rank_list[i].flexible_ll;
				break;

			default:
				temp_list[temp_list_size]->reserved = PERSON_RANK_MAX_SIZE - i;
				break;
			}

			++ temp_list_size;
		}
	}

//	qsort(temp_list, temp_list_size, sizeof(temp_list[0]), RankInfoSortCmp);
	std::stable_sort(std::begin(temp_list), std::end(temp_list), [](const PersonRankItem* a, const PersonRankItem* b) -> bool {
		if (!a || !b)
		{
			return false;
		}

		if (a->rank_value > b->rank_value)
		{
			return true;
		}
		else if (a->rank_value == b->rank_value && a->reserved > b->reserved)
		{
			return true;
		}

		return false;
	});

	int count = 0;
	for (int i = 0; i < temp_list_size && count < max_user; ++i)
	{
		if (NULL != temp_list[i] &&
			(CAMP_TYPE_INVALID == camp_limit || camp_limit == temp_list[i]->camp))
		{
			user_list[count] = temp_list[i]->user_id;
			++ count;                                                             
		}
	}

	return count;
}

int PersonRank::GetRankTopItem(int rank_type, int item_count, PersonRankItem *item_list)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX || item_count <= 0 || item_count > PERSON_RANK_MAX_SIZE || NULL == item_list) return 0;

	int get_count = 0;
	int userid_count = 0;
	static UserID userid_list[PERSON_RANK_MAX_SIZE];

	userid_count = this->GetRankTopUser(rank_type, PERSON_RANK_MAX_SIZE, userid_list);
	if (userid_count > 0)
	{
		for (int i = 0; i < userid_count && i < PERSON_RANK_MAX_SIZE && get_count < item_count; ++ i)
		{
			const PersonRankItem * rank_item = this->GetPersonRankItem(rank_type, userid_list[i]);
			if (NULL != rank_item)
			{
				item_list[get_count] = *rank_item;
				++ get_count;
			}
		}
	}

	return get_count;
}

int PersonRank::GetSnapShotTopUser(int rank_type, int max_user, UserID *user_list)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX || max_user <= 0 || NULL == user_list)
	{
		return 0;
	}

	int rank_start = rank_type * PERSON_RANK_MAX_SIZE;
	int rank_end =  rank_start + PERSON_RANK_MAX_SIZE;

	int get_count = 0;
	for (int i = rank_start; i < rank_end && i < max_user; ++ i)
	{
		if (m_rank_snapshot[i].Invalid())
		{
			break;
		}

		user_list[get_count] = m_rank_snapshot[i].user_id;
		++ get_count;
	}

	return get_count;
}

int PersonRank::GetSnapShotTopItem(int rank_type, int item_count, PersonRankItem *item_list)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX || item_count <= 0 || NULL == item_list)
	{
		return 0;
	}

	int rank_start = rank_type * PERSON_RANK_MAX_SIZE;
	int rank_end =  rank_start + PERSON_RANK_MAX_SIZE;

	int get_count = 0;
	for (int i = rank_start; i < rank_end && i < item_count; ++ i)
	{
		if (m_rank_snapshot[i].Invalid())
		{
			break;
		}

		item_list[get_count] = m_rank_snapshot[i];
		++ get_count;
	}

	return get_count;
}

void PersonRank::GetBipinTopUserInfo(int rand_activity_type, int history_cap_type, Protocol::TopUserInfo topuser_list[CAMP_TYPE_MAX])
{
	if (rand_activity_type < RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL || rand_activity_type > RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING ||
		history_cap_type < HISTORY_CAPABILITY_TYPE_TAOTAL || history_cap_type >= HISTORY_CAPABILITY_TYPE_MAX)
	{
		return;
	}

	for (int i = CAMP_TYPE_INVALID; i < CAMP_TYPE_MAX; i++)
	{
		topuser_list[i].Reset();
	}

	UNSTD_STATIC_CHECK(10 == HISTORY_CAPABILITY_TYPE_MAX);
	UNSTD_STATIC_CHECK(4 == CAMP_TYPE_MAX);

	const static int camp1_historycap_ranktype_map[HISTORY_CAPABILITY_TYPE_MAX] = 
	{
		PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP1,
		PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP1,
		PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP1,
		PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP1,
		PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP1,
		PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP1,
	};

	const static int camp2_historycap_ranktype_map[HISTORY_CAPABILITY_TYPE_MAX] = 
	{
		PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP2,
		PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP2,
		PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP2,
		PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP2,
		PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP2,
		PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP2,
	};

	const static int camp3_historycap_ranktype_map[HISTORY_CAPABILITY_TYPE_MAX] = 
	{
		PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP3,
		PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP3,
		PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP3,
		PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP3,
		PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP3,
		PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP3,
	};

	const static int *camp_to_ranktype_array[CAMP_TYPE_MAX] = 
	{
		NULL, 
		camp1_historycap_ranktype_map,
		camp2_historycap_ranktype_map,
		camp3_historycap_ranktype_map,
	};

	for (int camp_i = CAMP_TYPE_INVALID + 1; camp_i < CAMP_TYPE_MAX; ++camp_i)
	{
		const int *camp_to_ranktype = camp_to_ranktype_array[camp_i];
		if (NULL != camp_to_ranktype)
		{
			int rank_type = camp_to_ranktype[history_cap_type];
			if (rank_type > 0 && rank_type < PERSON_RANK_TYPE_MAX)
			{
				int rank_start = rank_type * PERSON_RANK_MAX_SIZE;

				const RandActivityRankBipinCfg *bipin_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(rand_activity_type);
				if (NULL != bipin_cfg)
				{
					if (!m_rank_snapshot[rank_start].Invalid() && m_rank_snapshot[rank_start].rank_value >= bipin_cfg->need_capability)
					{
						topuser_list[camp_i].uid = UidToInt(m_rank_snapshot[rank_start].user_id);
						F_STRNCPY(topuser_list[camp_i].name, m_rank_snapshot[rank_start].user_name, sizeof(GameName));
						topuser_list[camp_i].capability = m_rank_snapshot[rank_start].rank_value;

						UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_rank_snapshot[rank_start].user_id);
						if (NULL != user_node)
						{
							topuser_list[camp_i].prof = user_node->prof;
							topuser_list[camp_i].avatar_timestamp = user_node->avatar_timestamp;
						}
					}
				}
			}
		}
	}
}

void PersonRank::UserResetName(const UserID &user_id, GameName name)
{
	for (int i = 0; i < PERSON_RANK_TYPE_MAX; ++i)
	{
		UserIndexMapIt it = m_user_index_map[i].find(user_id);
		if (it != m_user_index_map[i].end())
		{
			for (std::set<int>::iterator index_it = it->second.begin(), index_end = it->second.end(); index_it != index_end; ++ index_it)
			{
				int rank_index = *index_it;
				if (this->IsValidIndex(rank_index))
				{
					F_STRNCPY(m_rank_list[rank_index].user_name, name, sizeof(GameName));

					m_dirty_mark[rank_index] = true;
					m_change_flag = true;
				}
			}
		}
	}
}

void PersonRank::SetRankList(int rank_count, PersonRankItem *rank_item)
{
	if (rank_count <= 0 || NULL == rank_item)
	{
		return;
	}

	for (int i = 0; i < rank_count && i < PERSON_RANK_MAX_SIZE; ++i)
	{
		this->UpdateRankList(rank_item[i]);
	}
}

bool PersonRank::AllowSnapshot(int rank_type)
{
	int bipin_activity_type = LOGIC_CONFIG->GetRandActivityCfg().GetRelateBipinActivity(rank_type);
	if (bipin_activity_type >= RAND_ACTIVITY_TYPE_BEGIN && bipin_activity_type < RAND_ACTIVITY_TYPE_END)
	{
		bool is_allow = RandActivityManager::Instance().IsSyncRankData(bipin_activity_type);
		return is_allow;
	}

	return true;
}

void PersonRank::UpdateRankList(PersonRankItem &rank_item)
{
	// 1：先查询是否排行榜上 如果是 直接更新即可 
	// 2：如果不是 则取得当前玩家的信息跟排行榜上最后一名的玩家比较 是否淘汰最后一名的的玩家

	if (rank_item.rank_type < 0 || rank_item.rank_type >= PERSON_RANK_TYPE_MAX) return;

	int find_index = 0;

	bool is_find = this->GetUserRecordIndex(rank_item.rank_type, rank_item.user_id, rank_item.record_index, &find_index);
	if (is_find && this->IsValidIndex(find_index))
	{
		if (!this->CanUpdateUserRankValue(rank_item, find_index)) return;

		m_rank_list[find_index] = rank_item;

		m_change_flag = true;
		m_dirty_mark[find_index] = true;

		this->OnRankPosChange(rank_item.rank_type, rank_item.user_id, find_index);
	}
	else 
	{
		int pos = -1;

		if (this->GetRankTypeSize(rank_item.rank_type) >= PERSON_RANK_MAX_SIZE)
		{
			if (!this->GetSwapRankItemSlot(rank_item.rank_type, rank_item, &pos)) return;
			
			if (this->IsValidIndex(pos))
			{
				this->RemoveUserIndex(m_rank_list[pos].rank_type, m_rank_list[pos].user_id, pos);
			}
		}
		else 
		{
			if (!this->GetFreeRankItemSlot(rank_item.rank_type, &pos)) return;
		}

		if (pos >= rank_item.rank_type * PERSON_RANK_MAX_SIZE && pos < (rank_item.rank_type + 1) * PERSON_RANK_MAX_SIZE)
		{
			m_rank_list[pos] = rank_item;
			this->AddUserIndex(rank_item.rank_type, rank_item.user_id, pos);

			m_change_flag = true;
			m_dirty_mark[pos] = true;

			this->OnRankPosChange(rank_item.rank_type, rank_item.user_id, pos);
		}
	}
}

void PersonRank::UpdateBaseInfo(PersonRankItem &rank_item, Role *role)
{
	if (NULL == role)
	{
		return;
	}

	role->GetName(rank_item.user_name);
	rank_item.sex = role->GetSex();
	rank_item.prof = role->GetProf();
	rank_item.camp = role->GetCamp();
	rank_item.vip_level = role->GetVip()->GetVipLevel();
	rank_item.level = role->GetLevel();
}

bool PersonRank::GetSwapRankItemSlot(int rank_type, const PersonRankItem &rank_item, int *swap_pos)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return false;

	int begin_index = rank_type * PERSON_RANK_MAX_SIZE;
	int end_index = (rank_type + 1) * PERSON_RANK_MAX_SIZE;

	int min_pos = begin_index; 
	long long min_item_cmp_value = 0, next_item_cmp_value = 0, to_insert_item_cmp_value = 0;

	// 找出现在排行榜中排序因子最小的项
	for (int i = begin_index; i < end_index; i++)
	{
		switch (rank_type)
		{
		case PERSON_RANK_TYPE_LEVEL:
			{
				to_insert_item_cmp_value = rank_item.exp;
				min_item_cmp_value = m_rank_list[min_pos].exp;
				next_item_cmp_value = m_rank_list[i].exp;
			}
			break;

		case PERSON_RANK_TYPE_RA_CHONGZHI:
		case PERSON_RANK_TYPE_RA_CHONGZHI2:
		case PERSON_RANK_TYPE_RA_CONSUME_GOLD:
		case PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP1:
		case PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP2:
		case PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP3:
		case PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP1:
		case PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP2:
		case PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP3:
		case PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP1:
		case PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP2:
		case PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP3:
		case PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP1:
		case PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP2:
		case PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP3:
		case PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP1:
		case PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP2:
		case PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP3:
		case PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP1:
		case PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP2:
		case PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP3:
		case PERSON_RANK_TYPE_RA_DAY_CHONGZHI_NUM:
		case PERSON_RANK_TYPE_RA_DAY_CONSUME_NUM:
		case PERSON_RANK_TYPE_WORLD_RIGHT_ANSWER:
		case PERSON_RANK_TYPE_RA_PROFESS_MALE:
		case PERSON_RANK_TYPE_RA_PROFESS_FEMALE:
			{
				to_insert_item_cmp_value = rank_item.flexible_ll;
				min_item_cmp_value = m_rank_list[min_pos].flexible_ll;
				next_item_cmp_value = m_rank_list[i].flexible_ll;
			}
			break;

		default:
			{
				to_insert_item_cmp_value = rank_item.level;
				min_item_cmp_value = m_rank_list[min_pos].level;
				next_item_cmp_value = m_rank_list[i].level;
			}
			break;
		}

		if (m_rank_list[i].rank_value < m_rank_list[min_pos].rank_value ||
			(m_rank_list[i].rank_value == m_rank_list[min_pos].rank_value && next_item_cmp_value < min_item_cmp_value))
		{
			min_pos = i;
		} 
	}

	// 最小的项和将要插入的项比较
	if (m_rank_list[min_pos].rank_value < rank_item.rank_value || 
		(m_rank_list[min_pos].rank_value == rank_item.rank_value && min_item_cmp_value < to_insert_item_cmp_value))
	{
		if (swap_pos) *swap_pos = min_pos;
		return true;
	}

	return false;
}

bool PersonRank::GetFreeRankItemSlot(int rank_type, int *free_pos)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return false;

	int begin_index = rank_type * PERSON_RANK_MAX_SIZE;
	int end_index = (rank_type + 1) * PERSON_RANK_MAX_SIZE;

	int i = begin_index;
	for (; i < end_index; i++)
	{
		if (m_rank_list[i].Invalid())
		{
			break;
		}
	}

	if (i >= end_index) return false;

	if (free_pos) *free_pos = i;

	return true;
}

bool PersonRank::AddUserIndex(int rank_type, const UserID &user_id, int rank_index)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX || !this->IsValidIndex(rank_index)) return false;

	UserIndexMapIt it = m_user_index_map[rank_type].find(user_id);
	if (it != m_user_index_map[rank_type].end())
	{
		it->second.insert(rank_index);
	}
	else 
	{
		std::set<int> rank_index_vec; rank_index_vec.insert(rank_index);
		m_user_index_map[rank_type][user_id] = rank_index_vec;
	}

	return true;
}

bool PersonRank::RemoveUserIndex(int rank_type, const UserID &user_id, int rank_index)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX || !this->IsValidIndex(rank_index)) return false;

	UserIndexMapIt it = m_user_index_map[rank_type].find(user_id);
	if (it != m_user_index_map[rank_type].end())
	{
		it->second.erase(rank_index);
		if (it->second.empty()) m_user_index_map[rank_type].erase(user_id);
	}

	return true;
}

bool PersonRank::GetUserRecordIndex(int rank_type, const UserID &user_id, int record_index, int *get_rank_index)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return false;
	
	UserIndexMapIt it = m_user_index_map[rank_type].find(user_id);
	if (it != m_user_index_map[rank_type].end())
	{
		for (std::set<int>::iterator index_it = it->second.begin(), index_end = it->second.end(); index_it != index_end; ++ index_it)
		{
			int rank_index = *index_it;
			if (this->IsValidIndex(rank_index))
			{
				if (m_rank_list[rank_index].record_index == record_index)
				{
					if (NULL != get_rank_index) *get_rank_index = rank_index; 

					return true;
				}
			}
		}
	}

	return false;
}

int PersonRank::GetRankTypeSize(int rank_type)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX) return false;

	int rank_type_size = 0;
	for (UserIndexMapIt it = m_user_index_map[rank_type].begin(), end = m_user_index_map[rank_type].end(); it != end; ++ it)
	{
		rank_type_size += static_cast<int>(it->second.size());
	}

	return rank_type_size;
}

void PersonRank::OnRankLoadFinish()
{
	if (!this->IsRankLoadFinish()) return;
}

void PersonRank::OnRankPosChange(int rank_type, const UserID &user_id, int rank_index)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX || !this->IsValidIndex(rank_index)) return;

}

void PersonRank::RecalcWorldLevelInfo()
{
	if (!this->IsRankLoadFinish()) return;
	
	{
		const int WORLD_LEVEL_MAX_ROLE_NUM = 10;																		
		UserID user_id_list[WORLD_LEVEL_MAX_ROLE_NUM];															// 世界等级只统计排行榜前10名

		int rank_count = this->GetRankTopUser(PERSON_RANK_TYPE_LEVEL, WORLD_LEVEL_MAX_ROLE_NUM, user_id_list);
		int total_level = 0;
		int old_world_level = m_world_level;
		if (rank_count > 0 && rank_count <= WORLD_LEVEL_MAX_ROLE_NUM)
		{
			{
				int find_index = 0;
				bool is_find = this->GetUserRecordIndex(PERSON_RANK_TYPE_LEVEL, user_id_list[0], 0, &find_index);	// 等级只有一条记录 record_index填0
				if (is_find && this->IsValidIndex(find_index))
				{
					m_top_user_level = m_rank_list[find_index].level;
				}
			}

			{//世界等级取前n名的平均值
				for (int i = 0; i < rank_count && i < WORLD_LEVEL_MAX_ROLE_NUM; ++i)
				{
					int find_index = 0;
					bool is_find = this->GetUserRecordIndex(PERSON_RANK_TYPE_LEVEL, user_id_list[i], 0, &find_index);	// 等级只有一条记录 record_index填0
					if (is_find && this->IsValidIndex(find_index))
					{
						total_level += m_rank_list[find_index].level;
					}
				}
				m_world_level = total_level / rank_count;
			}
		}

		this->SyncWorldLevelInfoToGameWorld();

		if (old_world_level != m_world_level)
		{
			this->SendWorldLevelToUser(true);
		}
	}

	{
		static const int CAPABILITY_RANK_MAX_ROLE_NUM = 100; 
		UserID user_id_list[CAPABILITY_RANK_MAX_ROLE_NUM];	
		int	rank_count = this->GetRankTopUser(PERSON_RANK_TYPE_CAPABILITY_ALL, CAPABILITY_RANK_MAX_ROLE_NUM, user_id_list);

		if (0 < rank_count && rank_count <= CAPABILITY_RANK_MAX_ROLE_NUM)
		{
			int find_index = 0;
			bool is_find = this->GetUserRecordIndex(PERSON_RANK_TYPE_CAPABILITY_ALL, user_id_list[rank_count - 1], 0, &find_index);	// 等级只有一条记录 record_index填0
			if (is_find && this->IsValidIndex(find_index))
			{
				m_capability_rank_mincap = (int)m_rank_list[find_index].rank_value;
			}
		}
	}
}

void PersonRank::SendWorldLevelToUser(bool is_broadcast, Role *user)
{
	static Protocol::SCGetWorldLevelAck gwla;
	//gwla.world_level = m_world_level;
	gwla.world_level = WorldShadow::Instance().GetWorldLevel();
	gwla.top_user_level = m_top_user_level;

	if (NULL != user)
	{
		EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)&gwla, sizeof(gwla));
	}
	else 
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&gwla, sizeof(gwla));
	}
}

void PersonRank::SyncWorldLevelInfoToGameWorld()
{
	if (!this->IsRankLoadFinish()) return;

	static gglobalprotocal::SyncWorldLevelInfo  wli;
	wli.world_level = m_world_level;
	wli.top_user_level = m_top_user_level;
	wli.capability_rank_mincap = m_capability_rank_mincap;

	WorldShadow::Instance().OnSyncWorldLevelInfo(&wli);
}

bool PersonRank::CanUpdateUserRankValue(PersonRankItem &rank_item, int find_index)
{
	if (!this->IsValidIndex(find_index)) return false;

	/*switch (rank_item.rank_type)
	{

	default:
		return true;
	}*/


	return true;
}

bool PersonRank::SensitiveToSex(int rank_type)
{
	if (PERSON_RANK_TYPE_ALL_CHARM == rank_type )
	{
		return true;
	}

	return false;
}

bool PersonRank::SensitiveToCamp(int rank_type)
{
	switch (rank_type)
	{
	case PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP1:
	case PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP2:
	case PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP3:
	case PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP1:
	case PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP2:
	case PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP3:
	case PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP1:
	case PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP2:
	case PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP3:
	case PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP1:
	case PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP2:
	case PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP3:
	case PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP1:
	case PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP2:
	case PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP3:
	case PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP1:
	case PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP2:
	case PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP3:
	case PERSON_RANK_TYPE_CAPABILITY_CAMP_1:
	case PERSON_RANK_TYPE_CAPABILITY_CAMP_2:
	case PERSON_RANK_TYPE_CAPABILITY_CAMP_3:
		return true;
	}

	return false;
}

bool PersonRank::RankTypeNeedLog(int rank_type)
{
	return true;
}

bool PersonRank::IsUserOnRank(UserID user_id, int rank_type)
{
	if (rank_type < 0)
	{
		for (int i = 0; i < PERSON_RANK_TYPE_MAX; ++i)
		{
			UserIndexMapIt it = m_user_index_map[i].find(user_id);
			if (it != m_user_index_map[i].end())
			{
				return true;
			}
		}
	}
	else if (rank_type >= 0 && rank_type < PERSON_RANK_TYPE_MAX)
	{
		UserIndexMapIt it = m_user_index_map[rank_type].find(user_id);
		if (it != m_user_index_map[rank_type].end())
		{
			return true;
		}
	}

	return false;
}

void PersonRank::DeleteUserFromRank(UserID user_id, int rank_type)
{
	if (rank_type < 0 || rank_type >= PERSON_RANK_TYPE_MAX)
	{
		return;
	}

	UserIndexMapIt it = m_user_index_map[rank_type].find(user_id);
	if (it == m_user_index_map[rank_type].end())
	{	
		return;
	}

	for (std::set<int>::iterator index_it = it->second.begin(), index_end = it->second.end(); index_it != index_end; ++ index_it)
	{
		int rank_index = *index_it;
		if (rank_index >= 0 && rank_index < PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE)
		{
			m_rank_snapshot[rank_index].Reset();
			m_rank_list[rank_index].Reset();
			m_dirty_mark[rank_index] = true;
		}
	}

	m_user_index_map[rank_type].erase(it);
}

