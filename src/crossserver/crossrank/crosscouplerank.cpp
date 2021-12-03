#include "crosscouplerank.hpp"
#include "crossserver/crossserver.h"
#include "servercommon/serverconfig/crossrankrewardconfig.hpp"
#include "protocal/msgcross.h"
#include "crossserver/crosslog.hpp"
#include "servercommon/string/crossstr.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include <algorithm>

CrossCoupleRank::CrossCoupleRank() : m_cross_server(nullptr)
{
	memset(m_next_rank_reward_time, 0, sizeof(m_next_rank_reward_time));
	memset(m_rank_list, 0, sizeof(m_rank_list));
}

void CrossCoupleRank::OnServerStart()
{
	this->ForceSnap();
	this->CalcNextRankRewardTime();
}

void CrossCoupleRank::OnDayChange()
{
	this->CalcNextRankRewardTime();
}

void CrossCoupleRank::CalcNextRankRewardTime()
{
	time_t now_sec = EngineAdapter::Instance().Time();
	for (int i = CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP; i < CROSS_COUPLE_RANK_TYPE_MAX; i++)
	{
		time_t &next_clear_time = m_next_rank_reward_time[i];
		if (now_sec >= next_clear_time)
		{
			const CrossRankRewardDateCfg *date_cfg = CrossRankRewardConfig::Instance().GetDateCfg(i + CROSS_COUPLE_RANK_TYPE_BEGIN);
			if (nullptr != date_cfg)
			{
				const tm *tm_now = EngineAdapter::Instance().LocalTime();
				if (nullptr != tm_now)
				{
					if (0 != next_clear_time)
					{
						this->GiveRankReward(i);
						//this->ClearRank(i);
					}

					int day_count = 0;
					for (int day = tm_now->tm_wday + 1; day < DAY_PER_WEEK; ++day)
					{
						if (date_cfg->CanReward(day))
						{
							day_count = day - tm_now->tm_wday;
							break;
						}
					}

					if (0 == day_count)
					{
						for (int day = 0; day <= tm_now->tm_wday; ++day)
						{
							if (date_cfg->CanReward(day))
							{
								day_count = day + DAY_PER_WEEK - tm_now->tm_wday;
								break;
							}
						}
					}

					next_clear_time = now_sec + EngineAdapter::Instance().NextDayInterval(0, 0, 0) + (day_count - 1) * SECOND_PER_DAY;
				}
			}
		}
	}
}

void CrossCoupleRank::OnSyncGameWorldRankList(crossgameprotocal::GameCrossSyncCrossCoupleRankInfo *pro)
{
	this->Merge(pro);
}

void CrossCoupleRank::OnGetCoupleRankList(NetID global_netid, const UniqueUserID &unique_user_id, int rank_type)
{
	if (rank_type < 0 || rank_type >= CROSS_COUPLE_RANK_TYPE_MAX) return;

	static Protocol::SCGetCrossCoupleRankListAck rank_list_ack;
	rank_list_ack.rank_type = rank_type;
	rank_list_ack.count = 0;

	int rank_start = rank_type * CROSS_COUPLE_RANK_MAX_SIZE;
	int rank_end = rank_start + CROSS_COUPLE_RANK_MAX_SIZE;

	int rank_count = 0;

	for (int i = rank_start; i < rank_end && i < CROSS_COUPLE_RANK_MAX_SIZE * CROSS_COUPLE_RANK_TYPE_MAX && rank_count < CROSS_COUPLE_RANK_MAX_SIZE; ++i)
	{
		if (!m_rank_list[i].IsValid())
		{
			break;
		}

		rank_list_ack.rank_list[rank_count].plat_type = m_rank_list[i].plat_type;
		rank_list_ack.rank_list[rank_count].server_id = m_rank_list[i].rank_item.server_id;
		rank_list_ack.rank_list[rank_count].female_uid = m_rank_list[i].rank_item.female_uid;
		rank_list_ack.rank_list[rank_count].male_uid = m_rank_list[i].rank_item.male_uid;
		rank_list_ack.rank_list[rank_count].male_prof = m_rank_list[i].rank_item.male_prof;
		rank_list_ack.rank_list[rank_count].female_prof = m_rank_list[i].rank_item.female_prof;
		rank_list_ack.rank_list[rank_count].male_rank_value = m_rank_list[i].rank_item.male_rank_value;
		rank_list_ack.rank_list[rank_count].female_rank_value = m_rank_list[i].rank_item.female_rank_value;
		F_STRNCPY(rank_list_ack.rank_list[rank_count].male_name, m_rank_list[i].rank_item.male_name, sizeof(m_rank_list[i].rank_item.male_name));
		F_STRNCPY(rank_list_ack.rank_list[rank_count].female_name, m_rank_list[i].rank_item.female_name, sizeof(m_rank_list[i].rank_item.female_name));

		++rank_count;
	}

	rank_list_ack.count = rank_count;
	int sendlen = sizeof(rank_list_ack) - (CROSS_COUPLE_RANK_MAX_SIZE - rank_list_ack.count) * sizeof(rank_list_ack.rank_list[0]);
	CrossServer::GetInstCrossServer()->SendToUser(global_netid, unique_user_id.user_id, (const char*)&rank_list_ack, sendlen);
}

void CrossCoupleRank::GmGiveRankReward(int rank_type, bool clear_rank)
{
	if (rank_type < CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP || rank_type >= CROSS_COUPLE_RANK_TYPE_MAX)
	{
		return;
	}

	if (clear_rank)
	{
		m_next_rank_reward_time[rank_type] = EngineAdapter::Instance().Time();
		this->CalcNextRankRewardTime();
	}
	else
	{
		this->GiveRankReward(rank_type);
	}
}

void CrossCoupleRank::OnDivorce(int plat_type, int uid)
{
	int rank_pos = -1;

	for (int i = 0; i < CROSS_COUPLE_RANK_TYPE_MAX; ++i)
	{
		int start = i * CROSS_COUPLE_RANK_MAX_SIZE;
		int end = start + CROSS_COUPLE_RANK_MAX_SIZE;
		for (int j = start; j < end; ++j)
		{
			if (m_rank_list[j].plat_type == plat_type)
			{
				if (m_rank_list[j].rank_item.male_uid == uid || m_rank_list[j].rank_item.female_uid == uid)
				{
					rank_pos = j;
					break;
				}
			}
		}

		// 在排行榜，则在排行榜上删掉，并且在用户字典中删掉
		if (rank_pos >= start && rank_pos < end)
		{
			for (int j = rank_pos; j < end - 1; ++j)
			{
				if (!m_rank_list[j].IsValid())
				{
					break;
				}

				m_rank_list[j] = m_rank_list[j + 1];
			}

			m_rank_list[end - 1].Reset();

			long long key = ConvertParamToLongLong(m_rank_list[rank_pos].plat_type, m_rank_list[rank_pos].rank_item.male_uid);
			auto iter = m_rank_user_dict_list[i].find(key);
			if (iter != m_rank_user_dict_list[i].end())
			{
				m_rank_user_dict_list[i].erase(iter);
			}
		}
	}
}

void CrossCoupleRank::GmFakeUser(int rank_type)
{
	static crossgameprotocal::GameCrossSyncCrossCoupleRankInfo pro;
	memset(pro.rank_item_list, 0, sizeof(pro.rank_item_list));
	pro.plat_type = 2000171776;
	pro.rank_type = rank_type;
	pro.item_count = COUPLE_RANK_MAX_SIZE;
	for (int i = 0; i < pro.item_count; ++i)
	{
		pro.rank_item_list[i].male_prof = 1;
		pro.rank_item_list[i].female_prof = 2;

		SNPRINTF(pro.rank_item_list[i].male_name, sizeof(GameName), "robot_m_%d", i);
		SNPRINTF(pro.rank_item_list[i].female_name, sizeof(GameName), "robot_f_%d", i);

		pro.rank_item_list[i].male_uid = i * 999 + 1;
		pro.rank_item_list[i].female_uid = i * 1000 + 1;

		pro.rank_item_list[i].server_id = 3;
		pro.rank_item_list[i].male_rank_value = RandomNum(0, 100000);
		pro.rank_item_list[i].female_rank_value = RandomNum(0, 100000);
	}

	this->Merge(&pro);
}

int CrossCoupleRank::GetCrossRankTypeToGameWorldRankType(int game_world_rank_type)
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

int CrossCoupleRank::GetGameWorldRankTypeToCrossRankType(int cross_world_rank_type)
{
	int type = -1;
	switch (cross_world_rank_type)
	{
	case CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP:
	{
		type = COUPLE_RANK_TYPE_TOTAL_QINGYUAN_CAP;
	}
	break;

	default:
		break;
	}

	return type;
}

void CrossCoupleRank::OnRegisterGameServer(NetID global_netid)
{
	//this->ForceSnap(global_netid);
}

void CrossCoupleRank::ForceSnap(NetID net_id)
{
	static crossgameprotocal::CrossGameSyncCrossCoupleForceSnap pro;
	if (net_id == UINT_MAX)
	{
		m_cross_server->SendToAllGameWorld((const char *)&pro, sizeof(pro));
	}
	else
	{
		m_cross_server->SendToGameWorld(net_id, (const char *)&pro, sizeof(pro));
	}
}

void CrossCoupleRank::Merge(crossgameprotocal::GameCrossSyncCrossCoupleRankInfo *pro)
{
	if (nullptr != pro)
	{
		int cross_rank_type = GetCrossRankTypeToGameWorldRankType(pro->rank_type);
		if (cross_rank_type >= CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP && cross_rank_type < CROSS_COUPLE_RANK_TYPE_MAX)
		{
			// 先更新数据
			auto &dict = m_rank_user_dict_list[cross_rank_type];
			for (int i = 0; i < pro->item_count && i < CROSS_COUPLE_RANK_MAX_SIZE; ++i)
			{
				long long key = ConvertParamToLongLong(pro->plat_type, pro->rank_item_list[i].male_uid);
				auto &dict_value = dict[key];
				dict_value = CrossrCoupleRankItem(pro->rank_type, pro->plat_type, pro->rank_item_list[i]);
			}

			// 合并之后，最多是2倍长度
			CrossrCoupleRankItem sort_rank_list[CROSS_COUPLE_RANK_MAX_SIZE * 2];

			auto iter = dict.begin();
			int valid_length = 0;
			for (int i = 0; iter != dict.end() && i < CROSS_COUPLE_RANK_MAX_SIZE * 2; ++iter, ++i)
			{
				sort_rank_list[i] = iter->second;
				++valid_length;
			}

			std::sort(sort_rank_list, sort_rank_list + valid_length, [](const CrossrCoupleRankItem &lhs, const CrossrCoupleRankItem &rhs) -> bool
			{
				return lhs.rank_item.male_rank_value + lhs.rank_item.female_rank_value > rhs.rank_item.male_rank_value + rhs.rank_item.female_rank_value;
			});

			int start_index = cross_rank_type * CROSS_COUPLE_RANK_MAX_SIZE;
			int end_index = start_index + CROSS_COUPLE_RANK_MAX_SIZE;

			if(cross_rank_type == CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP)
			{
				for (int i = 0; i < 3 && start_index + i < end_index; ++i)
				{
					if (sort_rank_list[i].IsValid())
					{
						if (sort_rank_list[i].rank_item.male_uid != m_rank_list[start_index + i].rank_item.male_uid)
						{
							bool is_broadcast_to_guild = true;

							for (int last_pos = 0; last_pos < 3; last_pos++)
							{
								if (sort_rank_list[i].rank_item.male_uid == m_rank_list[start_index + last_pos].rank_item.male_uid)
								{
									if (i > last_pos)
									{
										is_broadcast_to_guild = false;
										break;
									}
								}
							}

							if (is_broadcast_to_guild)
							{
								UserID target_userid = IntToUid(sort_rank_list[i].rank_item.male_uid);
								UniqueUserID target_unique_userid(sort_rank_list[i].plat_type, target_userid);

								crossgameprotocal::SyncCrossRankInfoToOriginGs info;

								info.cross_rank_type = cross_rank_type + CROSS_COUPLE_RANK_TYPE_BEGIN;
								info.param1 = sort_rank_list[i].rank_item.male_uid;
								info.param2 = sort_rank_list[i].rank_item.female_uid;
								info.rank_value = sort_rank_list[i].rank_item.female_rank_value + sort_rank_list[i].rank_item.male_rank_value;
								info.rank_pos = i + 1;

								m_cross_server->SendToGameWorld(target_unique_userid, (const char*)&info, sizeof(info));
							}
						}
					}
				}
			}

			memcpy(&m_rank_list[start_index], sort_rank_list, sizeof(CrossrCoupleRankItem) * CROSS_COUPLE_RANK_MAX_SIZE);

			// 把排行榜的数据插进去
			dict.clear();
			dict.reserve(valid_length < CROSS_COUPLE_RANK_MAX_SIZE ? valid_length : CROSS_COUPLE_RANK_MAX_SIZE);
			for (int i = start_index; i < end_index; ++i)
			{
				if (!m_rank_list[i].IsValid())
				{
					break;
				}

				long long key = ConvertParamToLongLong(m_rank_list[i].plat_type, m_rank_list[i].rank_item.male_uid);
				dict[key] = m_rank_list[i];
			}
		}
	}
}

void CrossCoupleRank::GiveRankReward(int rank_type)
{
	const char *content = nullptr;
	switch (rank_type)
	{
	case CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP:
	{
		content = gamestring::g_cross_add_couple_rank_qingyuan_content;
	}
	break;

	default:
		break;
	}

	if (nullptr == content)
	{
		return;
	}

	int start_index = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
	int end_index = start_index + CROSS_PERSON_RANK_MAX_SIZE;
	for (int i = start_index; i < end_index; ++i)
	{
		int rank_pos = i - start_index + 1;
		const CrossrCoupleRankItem &item = m_rank_list[i];
		if (!m_rank_list[i].IsValid())
		{
			// 造映像的时候已经排序，如果非法，没必要继续了
			break;
		}

		const CrossRankReward *reward_cfg = CrossRankRewardConfig::Instance().GetReward(rank_type + CROSS_COUPLE_RANK_TYPE_BEGIN, rank_pos);
		if (nullptr != reward_cfg)
		{
			static MailContentParam contentparam; contentparam.Reset();

			if (reward_cfg->reward_item.item_id > 0)
			{
				contentparam.AddItem(reward_cfg->reward_item);
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), content, rank_pos);
			if (length > 0 && contentparam.item_count > 0)
			{
				PlatName plat_name;
				LocalConfig::GetPlatName(item.plat_type, plat_name);

				UserID male_user_id = IntToUid(item.rank_item.male_uid);
				UniqueUserID male_uuid = UniqueUserID(item.plat_type, male_user_id);

				UserID female_user_id = IntToUid(item.rank_item.female_uid);
				UniqueUserID female_uuid = UniqueUserID(item.plat_type, female_user_id);

				m_cross_server->MailToGameWorldUser(male_uuid, contentparam);
				m_cross_server->MailToGameWorldUser(female_uuid, contentparam);

				bool server_connected = m_cross_server->IsUserLocateServerConnect(male_uuid) && m_cross_server->IsUserLocateServerConnect(female_uuid);

				crosslog::g_log_cross_rank.printf(LL_INFO,
					"CrossCoupleRank::GiveRankReward : server_connected %d, rank %d, plat_id %d, plat_name[%s], server_id %d male_uid %d, male_rank_value %d, female_uid %d, female_rank_value %d",
					(server_connected ? 1 : 0), i, item.plat_type, plat_name, male_user_id.db_index,
					item.rank_item.male_uid, item.rank_item.male_rank_value, item.rank_item.female_uid, item.rank_item.female_rank_value);
			}

			if (rank_pos <= 3)
			{
				UniqueUserID target_unique_userid(item.plat_type, IntToUid(item.rank_item.male_uid));

				crossgameprotocal::SyncCrossRankInfoToOriginGs info;

				info.cross_rank_type = rank_type + CROSS_COUPLE_RANK_TYPE_BEGIN;
				info.param1 = item.rank_item.male_uid;
				info.param2 = item.rank_item.female_uid;
				info.rank_value = item.rank_item.male_rank_value + item.rank_item.female_rank_value;
				info.rank_pos = rank_pos;

				m_cross_server->SendToGameWorld(target_unique_userid, (const char*)&info, sizeof(info));
			}
		}
	}

	this->SyncRankFirstUidToGameWorld(rank_type);
}

void CrossCoupleRank::ClearRank(int rank_type)
{
	if (rank_type < CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP || rank_type >= CROSS_COUPLE_RANK_TYPE_MAX)
	{
		return;
	}

	int start_index = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
	int end_index = start_index + CROSS_PERSON_RANK_MAX_SIZE;
	for (int i = start_index; i < end_index; ++i)
	{
		m_rank_list[i].Reset();
	}
}

void CrossCoupleRank::SyncRankFirstUidToGameWorld(int rank_type)
{
	if (rank_type >= CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP && rank_type < CROSS_COUPLE_RANK_TYPE_MAX)
	{
		static crossgameprotocal::CrossGameSyncCoupleRankFirst pro;
		pro.rank_type = rank_type;

		int start_index = rank_type * CROSS_PERSON_RANK_MAX_SIZE;
		int end_index = start_index + CROSS_PERSON_RANK_MAX_SIZE;
		int pro_list_count = 0;
		for (int i = start_index; i < end_index && pro_list_count < CROSS_RANK_REWARD_COUNT; ++i)
		{
			UserID male_user_id = IntToUid(m_rank_list[i].rank_item.male_uid);
			UniqueUserID male_uuid = UniqueUserID(m_rank_list[i].plat_type, male_user_id);

			UserID female_user_id = IntToUid(m_rank_list[i].rank_item.female_uid);
			UniqueUserID female_uuid = UniqueUserID(m_rank_list[i].plat_type, female_user_id);

			pro.male_uuid_list[pro_list_count] = UniqueUserIDToLongLong(male_uuid);
			pro.female_uuid_list[pro_list_count] = UniqueUserIDToLongLong(female_uuid);
			++pro_list_count;

			//log
			PlatName plat_name;
			LocalConfig::GetPlatName(m_rank_list[i].plat_type, plat_name);

			bool server_connected = m_cross_server->IsUserLocateServerConnect(male_uuid) && m_cross_server->IsUserLocateServerConnect(female_uuid);

			crosslog::g_log_cross_rank.printf(LL_INFO,
				"CrossCoupleRank::GiveRankReward : server_connected %d, rank %d, plat_id %d, plat_name[%s], server_id %d male_uid %d, male_rank_value %d, female_uid %d, female_rank_value %d",
				(server_connected ? 1 : 0), i, m_rank_list[i].plat_type, plat_name, male_user_id.db_index,
				m_rank_list[i].rank_item.male_uid, m_rank_list[i].rank_item.male_rank_value, m_rank_list[i].rank_item.female_uid, m_rank_list[i].rank_item.female_rank_value);
		}

		m_cross_server->SendToAllGameWorld((const char *)&pro, sizeof(pro));
	}
}
