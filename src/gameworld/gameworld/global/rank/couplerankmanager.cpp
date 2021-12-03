#include "couplerankmanager.hpp"
#include <set>
#include <algorithm>

#include "gameworld/gameworld/engineadapter.h"
#include "gameworld/obj/character/role.h"
#include "gameworld/global/usercache/usercache.hpp"
#include "servercommon/struct/roleglobalmoduleinfo.hpp"
#include "gameworld/gameworld/world.h"
#include "gameworld/global/worldstatus/worldstatus.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "protocal/msgscrank.h"
#include "protocal/msgcsrank.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "internalcomm.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/serverconfig/crossrankrewardconfig.hpp"

CoupleRankManager::CoupleRankManager() : m_last_snapshot_timestamp(0)
{
	memset(m_top_item_list, 0, sizeof(m_top_item_list));
}

CoupleRankManager::~CoupleRankManager()
{
	
}

CoupleRankManager & CoupleRankManager::Instance()
{
	static CoupleRankManager _couple_rank;
	return _couple_rank;
}

void CoupleRankManager::Update(unsigned long interval, time_t now_second)
{
	if (now_second > m_last_snapshot_timestamp + COUPLE_RANK_SNASHOT_INTERVAL)
	{
		{
			for (int rank_type = COUPLE_RANK_TYPE_QINGYUAN_CAP; rank_type < COUPLE_RANK_TYPE_MAX; ++rank_type)
			{
				this->ClearRank(rank_type);
			}
		}

		const std::map<int, UserCacheNode> &user_cache_map = UserCacheManager::Instance().GetUserCacheMap();
		for (std::map<int, UserCacheNode>::const_iterator user_it = user_cache_map.begin(); user_it != user_cache_map.end(); ++user_it)
		{
			const UserCacheNode &male_user_node = user_it->second;
			// 每次都会清榜，以男方为key更新，然后根据伴侣id找到女方(这样能减少重复更新)
			if (MALE != male_user_node.sex)
			{
				continue;
			}

			UserCacheNode *famale_user_node = UserCacheManager::Instance().GetUserNode(male_user_node.lover_id);
			if (nullptr == famale_user_node || famale_user_node->lover_id != male_user_node.uid) //存在主动离婚方，再结婚，被离婚方一直不在线导致一个人有两个伴侣
			{
				continue;
			}

			static SyncCoupleRankInfo couple_rank_info;
			couple_rank_info.item_count = 0;

			couple_rank_info.male_uid = male_user_node.uid;
			male_user_node.GetName(couple_rank_info.male_name);
			couple_rank_info.male_avatar = male_user_node.avatar_timestamp;
			couple_rank_info.male_prof = male_user_node.prof;

			couple_rank_info.female_uid = famale_user_node->uid;
			famale_user_node->GetName(couple_rank_info.female_name);
			couple_rank_info.female_avatar = famale_user_node->avatar_timestamp;
			couple_rank_info.female_prof = famale_user_node->prof;

			for (int i = 0; i < COUPLE_RANK_TYPE_MAX && couple_rank_info.item_count < COUPLE_RANK_TYPE_MAX; ++i)
			{
				couple_rank_info.rank_item_list[couple_rank_info.item_count].rank_type = i;
			
				int male_rank_value = 0;
				int female_rank_value = 0;

				switch (i)
				{
				case COUPLE_RANK_TYPE_QINGYUAN_CAP:
				{
					male_rank_value = male_user_node.qingyuan_cap;
					female_rank_value = famale_user_node->qingyuan_cap;
				}
				break;

				case COUPLE_RANK_TYPE_BABY_CAP:
				{
					male_rank_value = male_user_node.baby_cap;
					female_rank_value = famale_user_node->baby_cap;
				}
				break;

				case COUPLE_RANL_TYPE_LITTLE_PET:
				{
					male_rank_value = male_user_node.little_pet_cap;
					female_rank_value = famale_user_node->little_pet_cap;
				}
				break;

				case COUPLE_RANK_TYPE_TOTAL_QINGYUAN_CAP:
				{
					male_rank_value = male_user_node.capability;
					female_rank_value = famale_user_node->capability;
				}
				break;

				}

				if (male_rank_value > 0 || female_rank_value > 0)
				{
					couple_rank_info.rank_item_list[couple_rank_info.item_count].male_rank_value = male_rank_value;
					couple_rank_info.rank_item_list[couple_rank_info.item_count].female_rank_value = female_rank_value;

					++ couple_rank_info.item_count;
				}
			}

			if (couple_rank_info.item_count > 0)
			{
				this->OnRankInfoChange(couple_rank_info);
			}
		}

		this->Snapshot();
	}
}

void CoupleRankManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{	
	// 排行一次结算
	this->Snapshot();
	this->OnTopChange();
}

void CoupleRankManager::OnGetRankList(Role *role, Protocol::CSGetCoupleRankList &rank_list_req)
{
	if (nullptr == role) return;

	if (rank_list_req.rank_type < COUPLE_RANK_TYPE_MIN || rank_list_req.rank_type >= COUPLE_RANK_TYPE_MAX) return;

	static Protocol::SCGetCoupleRankListAck crla;

	crla.rank_type = rank_list_req.rank_type;
	crla.item_count = 0;

	int start_idx = crla.rank_type * COUPLE_RANK_MAX_SIZE;
	int end_idx = start_idx + COUPLE_RANK_MAX_SIZE;

	for (int i = start_idx; i < end_idx && crla.item_count < COUPLE_RANK_MAX_SIZE; ++i)
	{
		if (m_rank_snapshot_list[i].Invalid()) continue;

		crla.rank_item_list[crla.item_count].male_uid = m_rank_snapshot_list[i].male_uid;
		crla.rank_item_list[crla.item_count].female_uid = m_rank_snapshot_list[i].female_uid;
		F_STRNCPY(crla.rank_item_list[crla.item_count].male_name, m_rank_snapshot_list[i].male_name, sizeof(GameName));
		F_STRNCPY(crla.rank_item_list[crla.item_count].female_name, m_rank_snapshot_list[i].female_name, sizeof(GameName));
		crla.rank_item_list[crla.item_count].male_avatar = m_rank_snapshot_list[i].male_avatar;
		crla.rank_item_list[crla.item_count].female_avatar = m_rank_snapshot_list[i].female_avatar;
		crla.rank_item_list[crla.item_count].male_prof = m_rank_snapshot_list[i].male_prof;
		crla.rank_item_list[crla.item_count].female_prof = m_rank_snapshot_list[i].female_prof;
		crla.rank_item_list[crla.item_count].male_rank_value = m_rank_snapshot_list[i].male_rank_value;
		crla.rank_item_list[crla.item_count].female_rank_value = m_rank_snapshot_list[i].female_rank_value;

		++crla.item_count;
	}

	unsigned int send_len = sizeof(crla) - (COUPLE_RANK_MAX_SIZE - crla.item_count) * sizeof(crla.rank_item_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&crla, send_len);
}

void CoupleRankManager::OnDivorce(Role *role)
{
	if (nullptr == role) return;

	this->DeleteUserFromRank(role->GetUID());
}

void CoupleRankManager::ClearRank(int rank_type)
{
	if (rank_type < 0 || rank_type >= COUPLE_RANK_TYPE_MAX) return;

	int start_index = rank_type * COUPLE_RANK_MAX_SIZE;
	int end_index = start_index + COUPLE_RANK_MAX_SIZE;

	for (int i = start_index; i < end_index && i < COUPLE_RANK_TYPE_MAX * COUPLE_RANK_MAX_SIZE; ++i)
	{
		m_rank_list[i].Reset();
	}

	m_couple_index_map_list[rank_type].clear();
}

void CoupleRankManager::OnRankInfoChange(const SyncCoupleRankInfo &rank_info)
{
	if (rank_info.item_count <= 0)
	{
		return;
	}

	if (0 == rank_info.male_uid || 0 == rank_info.female_uid)
	{
		this->DeleteUserFromRank(rank_info.male_uid);
	}
	else
	{
		long long key = ConvertParamToLongLong(rank_info.male_uid, rank_info.female_uid);

		for (int i = 0; i < rank_info.item_count; ++i)
		{
			const SyncCoupleRankItem &rank_info_item = rank_info.rank_item_list[i];
			int rank_type = rank_info_item.rank_type;

			if (rank_type < COUPLE_RANK_TYPE_QINGYUAN_CAP || rank_type >= COUPLE_RANK_TYPE_MAX) continue;

			int start_idx = rank_type * COUPLE_RANK_MAX_SIZE;
			int end_idx = start_idx + COUPLE_RANK_MAX_SIZE;

			CoupleRankIndexMapIt it = m_couple_index_map_list[rank_type].find(key);
			if (it != m_couple_index_map_list[rank_type].end())
			{
				int rank_idx = it->second;
				int real_idx = rank_type * COUPLE_RANK_MAX_SIZE + rank_idx;

				if (real_idx >= start_idx && real_idx < end_idx)	// 更新完 退出 
				{
					CoupleRankItem &rank_item = m_rank_list[real_idx];

					if (rank_item.male_uid != rank_info.male_uid)
					{
						continue;
					}

					F_STRNCPY(rank_item.male_name, rank_info.male_name, sizeof(rank_item.male_name));
					F_STRNCPY(rank_item.female_name, rank_info.female_name, sizeof(rank_item.female_name));
					rank_item.male_avatar = rank_info.male_avatar;
					rank_item.female_avatar = rank_info.female_avatar;
					rank_item.male_prof = rank_info.male_prof;
					rank_item.female_prof = rank_info.female_prof;

					rank_item.male_rank_value = rank_info_item.male_rank_value;
					rank_item.female_rank_value = rank_info_item.female_rank_value;
					rank_item.flexible_int = rank_info_item.flexible_int;
					rank_item.flexible_ll = rank_info_item.flexible_ll;
				}
			}
			else 
			{
				// UPDATE 进去
				CoupleRankItem rank_item;

				rank_item.rank_type = rank_type;
				rank_item.male_uid = rank_info.male_uid;
				rank_item.female_uid = rank_info.female_uid;
				F_STRNCPY(rank_item.male_name, rank_info.male_name, sizeof(rank_item.male_name));
				F_STRNCPY(rank_item.female_name, rank_info.female_name, sizeof(rank_item.female_name));
				rank_item.male_avatar = rank_info.male_avatar;
				rank_item.female_avatar = rank_info.female_avatar;
				rank_item.male_prof = rank_info.male_prof;
				rank_item.female_prof = rank_info.female_prof;

				rank_item.male_rank_value = rank_info_item.male_rank_value;
				rank_item.female_rank_value = rank_info_item.female_rank_value;
				rank_item.flexible_int = rank_info_item.flexible_int;
				rank_item.flexible_ll = rank_info_item.flexible_ll;

				// 找到一个可插入的位置
				int insert_idx = this->GetInsertSlot(rank_type);

				if (insert_idx >= start_idx && insert_idx < end_idx)
				{
					// 删除旧的key
					key = this->GetRankKey(m_rank_list[insert_idx]);
					CoupleRankIndexMapIt it = m_couple_index_map_list[rank_type].find(key);
					if (it != m_couple_index_map_list[rank_type].end())
					{
						m_couple_index_map_list[rank_type].erase(it);
					}

					m_rank_list[insert_idx] = rank_item;

					// 更新key
					key = this->GetRankKey(m_rank_list[insert_idx]);
					m_couple_index_map_list[rank_type].emplace(std::make_pair(key, insert_idx - start_idx));
				}
			}
		}
	}
}

void CoupleRankManager::Snapshot()
{
	m_last_snapshot_timestamp = EngineAdapter::Instance().Time();

	for (int i = 0; i < COUPLE_RANK_TYPE_MAX * COUPLE_RANK_MAX_SIZE; ++i)
	{
		m_rank_snapshot_list[i].Reset();
	}

	for (int rank_type = 0; rank_type < COUPLE_RANK_TYPE_MAX; ++rank_type)
	{
		int rank_start = rank_type * COUPLE_RANK_MAX_SIZE;
		int rank_end = rank_start + COUPLE_RANK_MAX_SIZE;

		int temp_list_size = 0;
		static CoupleRankItem *temp_list[COUPLE_RANK_MAX_SIZE] = { NULL };
		memset(temp_list, 0, sizeof(temp_list));

		for (int i = rank_start; i < rank_end && temp_list_size < COUPLE_RANK_MAX_SIZE; ++i)
		{
			if (!m_rank_list[i].Invalid())
			{
				temp_list[temp_list_size] = &m_rank_list[i];
				++temp_list_size;
			}
		}

		std::stable_sort(temp_list, temp_list + temp_list_size, [](const CoupleRankItem *r_item, const CoupleRankItem *l_item)->bool {

			long long r_rank_value = r_item->male_rank_value + r_item->female_rank_value;
			long long l_rank_value = l_item->male_rank_value + l_item->female_rank_value;

			return r_rank_value > l_rank_value;
		});

		for (int i = 0; i < temp_list_size && i < GUILD_RANK_MAX_SIZE && rank_start + i < rank_end; ++i)
		{
			if (NULL != temp_list[i])
			{
				Role *male_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(temp_list[i]->male_uid));
				Role *female_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(temp_list[i]->female_uid));
				if (male_role != NULL && female_role != NULL)
				{
					if (temp_list[i]->male_uid != m_rank_snapshot_list[rank_start + i].male_uid)				//以男方排名变化检查更新
					{
						if (male_role->GetGuildID() != female_role->GetGuildID())
						{
							EventHandler::Instance().OnCoupleRankChange(IntToUid(temp_list[i]->male_uid), i, temp_list[i]->rank_type, temp_list[i]->male_rank_value);
							EventHandler::Instance().OnCoupleRankChange(IntToUid(temp_list[i]->female_uid), i, temp_list[i]->rank_type, temp_list[i]->female_rank_value);
						}
						else
						{
							EventHandler::Instance().OnCoupleRankChange(IntToUid(temp_list[i]->male_uid), i, temp_list[i]->rank_type, temp_list[i]->male_rank_value);
						}
					}
				}

				m_rank_snapshot_list[rank_start + i] = *temp_list[i];
			}
		}
	}
	this->AfterSnapshot();
}

void CoupleRankManager::DeleteUserFromRank(int uid)
{
	for (int rank_type = 0; rank_type < COUPLE_RANK_TYPE_MAX; ++rank_type)
	{
		int rank_start = rank_type * COUPLE_RANK_MAX_SIZE;
		int rank_end = rank_start + COUPLE_RANK_MAX_SIZE;

		for (int i = rank_start; i < rank_end; ++i)
		{
			if (m_rank_list[i].male_uid == uid || m_rank_list[i].female_uid == uid)
			{
				long long key = this->GetRankKey(m_rank_list[i]);

				CoupleRankIndexMapIt it = m_couple_index_map_list[rank_type].find(key);
				if (it != m_couple_index_map_list[rank_type].end())
				{
					m_couple_index_map_list[rank_type].erase(it);
				}

				m_rank_list[i].Reset();
			}
		}
	}
}

long long CoupleRankManager::GetRankKey(CoupleRankItem &couple_rank_item)
{
	if (-1 == couple_rank_item.rank_type) return 0;

	return ConvertParamToLongLong(couple_rank_item.male_uid, couple_rank_item.female_uid);
}

int CoupleRankManager::GetInsertSlot(int rank_type)
{
	int insert_idx = -1;

	if (rank_type < COUPLE_RANK_TYPE_MIN || rank_type >= COUPLE_RANK_TYPE_MAX)
	{
		return insert_idx;
	}

	int start_idx = rank_type * COUPLE_RANK_MAX_SIZE;
	int end_idx = start_idx + COUPLE_RANK_MAX_SIZE;

	// 榜未满找到第一个可用的，榜满了找最小的替换
	int rank_size = static_cast<int>(m_couple_index_map_list[rank_type].size());
	if (rank_size < COUPLE_RANK_MAX_SIZE)
	{
		for (int i = start_idx; i < end_idx; ++i)
		{
			if (m_rank_list[i].Invalid())
			{
				insert_idx = i;
				break;
			}
		}
	}
	else
	{
		for (int i = start_idx; i < end_idx; ++i)
		{
			if (-1 == insert_idx)
			{
				insert_idx = i;
			}
			else if (insert_idx >= start_idx && insert_idx < end_idx)
			{
				long long item1_rank_value = m_rank_list[insert_idx].male_rank_value + m_rank_list[insert_idx].female_rank_value;
				long long item2_rank_value = m_rank_list[i].male_rank_value + m_rank_list[i].female_rank_value;

				if (item2_rank_value < item1_rank_value)
				{
					insert_idx = i;
				}
			}
		}
	}

	return insert_idx;
}

void CoupleRankManager::OnTopChange()
{
	memset(m_top_item_list, 0, sizeof(m_top_item_list));

	for (int rank_type = COUPLE_RANK_TYPE_QINGYUAN_CAP; rank_type < COUPLE_RANK_TYPE_MAX; ++rank_type)
	{
		int start_idx = rank_type * COUPLE_RANK_MAX_SIZE;
		int end_idx = start_idx + COUPLE_RANK_TOP_COUNT;

		for (int i = start_idx; i < end_idx; ++i)
		{
			if (m_rank_snapshot_list[i].Invalid())
			{
				continue;
			}

			const int top_index = rank_type * COUPLE_RANK_TOP_COUNT + i - start_idx;

			m_top_item_list[top_index].male_uid = m_rank_snapshot_list[i].male_uid;
			m_top_item_list[top_index].female_uid = m_rank_snapshot_list[i].female_uid;
		}
	}

	for (int rank_type = COUPLE_RANK_TYPE_QINGYUAN_CAP; rank_type < COUPLE_RANK_TYPE_MAX; ++rank_type)
	{
		int top_idx = rank_type * COUPLE_RANK_TOP_COUNT;

		if (COUPLE_RANK_TYPE_QINGYUAN_CAP == rank_type)
		{
			WorldStatus::Instance().OnSyncQingyuanCapTop(m_top_item_list[top_idx].male_uid, m_top_item_list[top_idx].female_uid);
		}
		else if (COUPLE_RANK_TYPE_BABY_CAP == rank_type)
		{
			WorldStatus::Instance().OnSyncBabyCapTop(m_top_item_list[top_idx].male_uid, m_top_item_list[top_idx].female_uid);
		}
		else if (COUPLE_RANL_TYPE_LITTLE_PET == rank_type)
		{
			WorldStatus::Instance().OnSyncLittePetCapTop(m_top_item_list[top_idx].male_uid, m_top_item_list[top_idx].female_uid);
		}
	}
}

void CoupleRankManager::AfterSnapshot()
{
	this->SyncRankListToCross(COUPLE_RANK_TYPE_TOTAL_QINGYUAN_CAP);
}

void CoupleRankManager::SyncRankListToCross(int rank_type)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (rank_type < COUPLE_RANK_TYPE_MIN || rank_type >= COUPLE_RANK_TYPE_MAX)
	{
		return;
	}

	int cross_rank_type = GetCrossRankTypeToGameWorldRankType(rank_type);
	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
	int limi_open_day = CrossRankRewardConfig::Instance().GetCrossRankLimitOpenDay(cross_rank_type);
	if (limi_open_day > 0 && opengame_day < limi_open_day)
	{
		return;
	}

	static crossgameprotocal::GameCrossSyncCrossCoupleRankInfo pro;
	pro.item_count = 0;
	pro.rank_type = rank_type;
	pro.plat_type = LocalConfig::Instance().GetPlatType();

	int start_idx = rank_type * COUPLE_RANK_MAX_SIZE;
	int end_idx = start_idx + COUPLE_RANK_MAX_SIZE;

	for (int i = start_idx; i < end_idx; ++i)
	{
		if (m_rank_snapshot_list[i].Invalid()) break;

		pro.rank_item_list[pro.item_count].server_id = IntToUid(m_rank_snapshot_list[i].male_uid).db_index;
		pro.rank_item_list[pro.item_count].male_uid = m_rank_snapshot_list[i].male_uid;
		pro.rank_item_list[pro.item_count].female_uid = m_rank_snapshot_list[i].female_uid;
		F_STRNCPY(pro.rank_item_list[pro.item_count].male_name, m_rank_snapshot_list[i].male_name, sizeof(GameName));
		F_STRNCPY(pro.rank_item_list[pro.item_count].female_name, m_rank_snapshot_list[i].female_name, sizeof(GameName));
		pro.rank_item_list[pro.item_count].male_prof = m_rank_snapshot_list[i].male_prof;
		pro.rank_item_list[pro.item_count].female_prof = m_rank_snapshot_list[i].female_prof;
		pro.rank_item_list[pro.item_count].male_rank_value = m_rank_snapshot_list[i].male_rank_value;
		pro.rank_item_list[pro.item_count].female_rank_value = m_rank_snapshot_list[i].female_rank_value;

		++pro.item_count;
	}

	int send_len = sizeof(pro) - sizeof(pro.rank_item_list) + pro.item_count * sizeof(pro.rank_item_list[0]);
	InternalComm::Instance().SendToCross((const char *)&pro, send_len, CROSS_FUNCTION_TYPE_CROSS_RANK);
}

int CoupleRankManager::GetCrossRankTypeToGameWorldRankType(int game_world_rank_type)
{
	int type = -1;
	switch (game_world_rank_type)
	{
	case COUPLE_RANK_TYPE_TOTAL_QINGYUAN_CAP:
	{
		type = CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP;
	}
	break;

	default:
		break;
	}

	return type;
}
