#include "worldstatus3.hpp"
#include "global/rmibackobjdef.h"
#include "internalcomm.h"
#include "engineadapter.h"
#include "world.h"
#include "global/usercache/usercache.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "obj/character/role.h"
#include "other/shizhuang/shizhuang.h"
#include "other/wing/wingmanager.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/rank/topranker.hpp"
#include "global/guild/guildmanager.hpp"
#include "servercommon/rolelotterycountdef.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "global/randactivity/impl/randactivityluckycloudbuy.hpp"

WorldStatus3::WorldStatus3()
	: m_data_state(WORLDSTATE3_DATA_STATUS_INVALID)
{

}

WorldStatus3::~WorldStatus3()
{

}

void WorldStatus3::Init(const WorldStatus3Param &worldstatus3_param)
{
	memcpy(&m_param, &worldstatus3_param, sizeof(WorldStatus3Param));
	TopRanker::Instance().Init(worldstatus3_param.toprank_param);
	RandActivityManager::Instance().Init(worldstatus3_param.rand_activity_param);

	RandActivityLuckyCloudBuy * rand_act = RandActivityManager::Instance().GetRandActivityLuckyCloudBuy();
	if (NULL != rand_act)
	{
		rand_act->InitActivityData();
	}

	this->CheckClearOldCoupleRankData();

	this->LoadWorldStatus3Succ();
}

void WorldStatus3::OnServerStart()
{
	this->LoadWorldStatus3(0);
}

void WorldStatus3::OnServerStop()
{
	RandActivityLuckyCloudBuy * rand_act = RandActivityManager::Instance().GetRandActivityLuckyCloudBuy();
	if (NULL != rand_act)
	{
		rand_act->SaveActivityData();
	}
	this->SaveWorldStatus3();
}

void WorldStatus3::Update(unsigned long interval, time_t now_second)
{
	if (WORLDSTATE3_DATA_STATUS_LOAD_FINISH != m_data_state) return;

	static time_t last_save_time = now_second;

	if (now_second >= last_save_time + WORLDSTATE3_SAVE_INTERVAL)
	{
		this->SaveWorldStatus3();

		last_save_time = now_second;
	}
}

void WorldStatus3::OnEnterScene(Role *role)
{

}

void WorldStatus3::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

}

void WorldStatus3::OnResetData(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;
}

bool WorldStatus3::LoadWorldStatus3(long long id_from)
{
	if (WORLDSTATE3_DATA_STATUS_LOAD_FINISH == m_data_state) return false;

	m_data_state = WORLDSTATE3_DATA_STATUS_LOADING;

	RMIInitWorldStatus3BackObjectImpl *impl = new RMIInitWorldStatus3BackObjectImpl();

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return gc.InitWorldStatus3Asyn(id_from, impl);
}

void RMIInitWorldStatus3BackObjectImpl::InitWorldStatus3Ret(int ret, const WorldStatus3Param &worldstatus3_param)
{
	if (ret < 0)
	{
		printf(" RMIInitWorldStatus3BackObjectImpl::InitWorldStatus3Ret Failed \n");
		EngineAdapter::Instance().StopGame();
		return;
	}

	WorldStatus3::Instance().Init(worldstatus3_param);
}

void WorldStatus3::LoadWorldStatus3Succ()
{
	m_data_state = WORLDSTATE3_DATA_STATUS_LOAD_FINISH;
}

bool WorldStatus3::SaveWorldStatus3()
{
	RandActivityLuckyCloudBuy * rand_act = RandActivityManager::Instance().GetRandActivityLuckyCloudBuy();
	if (NULL != rand_act)
	{
		rand_act->SaveActivityData();
	}

	RMISaveWorldStatus3BackObjectImpl *impl = new RMISaveWorldStatus3BackObjectImpl();

	TopRanker::Instance().GetInitParam(&m_param.toprank_param);
	RandActivityManager::Instance().GetInitParam(&m_param.rand_activity_param);

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return gc.SaveWorldStatus3Asyn(m_param, impl);
}

WorldStatus3 & WorldStatus3::Instance()
{
	static WorldStatus3 ws;
	return ws;
}

bool WorldStatus3::IsLoadFinish() const
{
	return (WORLDSTATE3_DATA_STATUS_LOAD_FINISH == m_data_state);
}

void WorldStatus3::SetCrossAddCapRankFirst(int uid_list[], int list_count)
{
	if (list_count != CROSS_RANK_REWARD_COUNT)
	{
		return;
	}

	int old_uid_list[CROSS_RANK_REWARD_COUNT] = { 0 };
	memcpy(old_uid_list, m_param.common_data.cross_add_cap_rank_uid_list, sizeof(old_uid_list));
	memcpy(m_param.common_data.cross_add_cap_rank_uid_list, uid_list, sizeof(m_param.common_data.cross_add_cap_rank_uid_list));

	for (int i = 0; i < list_count; ++i)
	{
		if (old_uid_list[i] != uid_list[i])
		{
			Role *old_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_uid_list[i]));
			if (nullptr != old_role)
			{
				old_role->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_CROSS_ADD_CAP);
			}

			Role *new_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid_list[i]));
			if (nullptr != new_role)
			{
				new_role->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_CROSS_ADD_CAP);
			}
		}

		//log
		gamelog::g_log_rank.printf(LL_INFO,
			"CrossPersonRank::OldCrossAddCapRankFirst : rank %d, role_id %d", CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY, old_uid_list[i]);

		gamelog::g_log_rank.printf(LL_INFO,
				"CrossPersonRank::NewCrossAddCapRankFirst : rank %d, role_id %d", CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY, uid_list[i]);
	}
}

void WorldStatus3::GetCrossAddCapRankFirst(int uid_list[], int list_count)
{
	if (list_count != CROSS_RANK_REWARD_COUNT)
	{
		return;
	}

	memcpy(uid_list, m_param.common_data.cross_add_cap_rank_uid_list, sizeof(int) * list_count);
}

void WorldStatus3::SetCrossAddCharmRankFirst(int uid_list[], int list_count)
{
	if (list_count != CROSS_RANK_REWARD_COUNT)
	{
		return;
	}

	int old_uid_list[CROSS_RANK_REWARD_COUNT] = { 0 };
	memcpy(old_uid_list, m_param.common_data.cross_add_charm_rank_uid_list, sizeof(old_uid_list));
	memcpy(m_param.common_data.cross_add_charm_rank_uid_list, uid_list, sizeof(m_param.common_data.cross_add_charm_rank_uid_list));

	for (int i = 0; i < list_count; ++i)
	{
		if (old_uid_list[i] != uid_list[i])
		{
			Role *old_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_uid_list[i]));
			if (nullptr != old_role)
			{
				old_role->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_CROSS_ADD_CHARM);
			}

			Role *new_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid_list[i]));
			if (nullptr != new_role)
			{
				new_role->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_CROSS_ADD_CHARM);
			}
		}

		//log
		gamelog::g_log_rank.printf(LL_INFO,
			"CrossPersonRank::OldCrossAddCharmRankFirst : rank %d, role_id %d", SHIZHUANG_CHECK_ACTIVE_TYPE_CROSS_ADD_CHARM, old_uid_list[i]);

		gamelog::g_log_rank.printf(LL_INFO,
			"CrossPersonRank::NewCrossAddCahrmRankFirst : rank %d, role_id %d", SHIZHUANG_CHECK_ACTIVE_TYPE_CROSS_ADD_CHARM, uid_list[i]);
	}
}

void WorldStatus3::GetCrossAddCharmRankFirst(int uid_list[], int list_count)
{
	if (list_count != CROSS_RANK_REWARD_COUNT)
	{
		return;
	}

	memcpy(uid_list, m_param.common_data.cross_add_charm_rank_uid_list, sizeof(int) * list_count);
}

void WorldStatus3::CheckClearOldCoupleRankData()
{
	for (unsigned int i = 0; i < CROSS_RANK_REWARD_COUNT; i++)
	{
		int plat_type = LocalConfig::Instance().GetPlatType();
		int male_uid = m_param.common_data.cross_qingyuan_cap_rank_male_uid_list[i];
		int female_uid = m_param.common_data.cross_qingyuan_cap_rank_female_uid_list[i];

		if (male_uid > 0)
		{
			UniqueUserID male_unique_userid(plat_type, IntToUid(male_uid));
			m_param.common_data.cross_qingyuan_cap_rank_male_uuid_list[i] = UniqueUserIDToLongLong(male_unique_userid);
		}

		if (female_uid > 0)
		{
			UniqueUserID female_unique_userid(plat_type, IntToUid(female_uid));
			m_param.common_data.cross_qingyuan_cap_rank_female_uuid_list[i] = UniqueUserIDToLongLong(female_unique_userid);
		}
	}

	memset(m_param.common_data.cross_qingyuan_cap_rank_male_uid_list, 0, sizeof(m_param.common_data.cross_qingyuan_cap_rank_male_uid_list));
	memset(m_param.common_data.cross_qingyuan_cap_rank_female_uid_list, 0, sizeof(m_param.common_data.cross_qingyuan_cap_rank_female_uid_list));
}

void WorldStatus3::SetCrossQingyuanCapRankFirstMale(long long uuid_list[], int list_count)
{
	if (list_count != CROSS_RANK_REWARD_COUNT)
	{
		return;
	}

	long long old_uuid_list[CROSS_RANK_REWARD_COUNT] = { 0 };
	memcpy(old_uuid_list, m_param.common_data.cross_qingyuan_cap_rank_male_uuid_list, sizeof(old_uuid_list));
	memcpy(m_param.common_data.cross_qingyuan_cap_rank_male_uuid_list, uuid_list, sizeof(m_param.common_data.cross_qingyuan_cap_rank_male_uuid_list));

	for (int i = 0; i < list_count; ++i)
	{
		if (old_uuid_list[i] != uuid_list[i])
		{
			Role *old_role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(old_uuid_list[i]);
			if (nullptr != old_role)
			{
				old_role->GetWing()->CheckActiveSpecialImg();
			}

			Role *new_role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(uuid_list[i]);
			if (nullptr != new_role)
			{
				new_role->GetWing()->CheckActiveSpecialImg();
			}
		}

		//log
		UniqueUserID old_unique_userid = LongLongToUniqueUserID(old_uuid_list[i]);
		UniqueUserID new_unique_userid = LongLongToUniqueUserID(uuid_list[i]);

		gamelog::g_log_rank.printf(LL_INFO,
			"CrossCoupleRank::OldCrossQingyuanCapRankFirstMale : rank %d, role_id %d", CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP + CROSS_COUPLE_RANK_TYPE_BEGIN, UidToInt(old_unique_userid.user_id));

		gamelog::g_log_rank.printf(LL_INFO,
			"CrossCoupleRank::NewCrossQingyuanCapRankFirstMale : rank %d, role_id %d", CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP + CROSS_COUPLE_RANK_TYPE_BEGIN, UidToInt(new_unique_userid.user_id));
	}
}

void WorldStatus3::GetCrossQingyuanCapRankFirstMale(long long uuid_list[], int list_count)
{
	if (list_count != CROSS_RANK_REWARD_COUNT)
	{
		return;
	}

	memcpy(uuid_list, m_param.common_data.cross_qingyuan_cap_rank_male_uuid_list, sizeof(long long) * list_count);
}

void WorldStatus3::SetCrossQingyuanCapRankFirstFemale(long long uuid_list[], int list_count)
{
	if (list_count != CROSS_RANK_REWARD_COUNT)
	{
		return;
	}

	long long old_uuid_list[CROSS_RANK_REWARD_COUNT] = { 0 };
	memcpy(old_uuid_list, m_param.common_data.cross_qingyuan_cap_rank_female_uuid_list, sizeof(old_uuid_list));
	memcpy(m_param.common_data.cross_qingyuan_cap_rank_female_uuid_list, uuid_list, sizeof(m_param.common_data.cross_qingyuan_cap_rank_female_uuid_list));

	for (int i = 0; i < list_count; ++i)
	{
		if (old_uuid_list[i] != uuid_list[i])
		{
			Role *old_role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(old_uuid_list[i]);
			if (nullptr != old_role)
			{
				old_role->GetWing()->CheckActiveSpecialImg();
			}

			Role *new_role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(uuid_list[i]);
			if (nullptr != new_role)
			{
				new_role->GetWing()->CheckActiveSpecialImg();
			}
		}

		//log
		UniqueUserID old_unique_userid = LongLongToUniqueUserID(old_uuid_list[i]);
		UniqueUserID new_unique_userid = LongLongToUniqueUserID(uuid_list[i]);

		gamelog::g_log_rank.printf(LL_INFO,
			"CrossCoupleRank::OldCrossQingyuanCapRankFirstFemale : rank %d, role_id %d", CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP + CROSS_COUPLE_RANK_TYPE_BEGIN, UidToInt(old_unique_userid.user_id));

		gamelog::g_log_rank.printf(LL_INFO,
			"CrossCoupleRank::NewCrossQingyuanCapRankFirstFemale : rank %d, role_id %d", CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP + CROSS_COUPLE_RANK_TYPE_BEGIN, UidToInt(new_unique_userid.user_id));
	}
}

void WorldStatus3::GetCrossQingyuanCapRankFirstFemale(long long uuid_list[], int list_count)
{
	if (list_count != CROSS_RANK_REWARD_COUNT)
	{
		return;
	}

	memcpy(uuid_list, m_param.common_data.cross_qingyuan_cap_rank_female_uuid_list, sizeof(long long) * list_count);
}

void WorldStatus3::SetCrossGuildKillRankFirst(int guild_list[], int list_count)
{
	if (list_count != CROSS_RANK_REWARD_COUNT)
	{
		return;
	}

	int old_guild_list[CROSS_RANK_REWARD_COUNT] = { 0 };
	memcpy(old_guild_list, m_param.common_data.cross_guild_id_list, sizeof(old_guild_list));
	memcpy(m_param.common_data.cross_guild_id_list, guild_list, sizeof(m_param.common_data.cross_guild_id_list));

	for (int i = 0; i < list_count; ++i)
	{
		if (old_guild_list[i] != guild_list[i])
		{
			GuildManager::Instance().OnSyncCrossGuildKillRank(old_guild_list[i]);
			GuildManager::Instance().OnSyncCrossGuildKillRank(guild_list[i]);
		}

		//log
		gamelog::g_log_rank.printf(LL_INFO,
			"CrossPersonRank::OldCrossGuildKillRankFirst : rank %d, role_id %d", CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS, old_guild_list[i]);

		gamelog::g_log_rank.printf(LL_INFO,
			"CrossPersonRank::NewCrossGuildKillRankFirst : rank %d, role_id %d", CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS, guild_list[i]);
	}
}

void WorldStatus3::GetCrossGuildKillRankFirst(int guild_list[], int list_count)
{
	if (list_count != CROSS_RANK_REWARD_COUNT)
	{
		return;
	}

	memcpy(guild_list, m_param.common_data.cross_guild_id_list, sizeof(int) * list_count);
}

int WorldStatus3::GetWorldLotteryCount(int lottery_type)
{
	if (lottery_type <= LOTTERY_REPLACE_TYPE_INVALID || lottery_type >= LOTTERY_REPLACE_TYPE_MAX)
	{
		return 0;
	}
	return m_param.lottery_count_param.lottery_count[lottery_type];
}

bool WorldStatus3::ResetWorldLotteryCount(int lottery_type)
{
	if (lottery_type <= LOTTERY_REPLACE_TYPE_INVALID || lottery_type >= LOTTERY_REPLACE_TYPE_MAX)
	{
		return false;
	}
	m_param.lottery_count_param.lottery_count[lottery_type] = 0;
	return true;
}

bool WorldStatus3::AddWorldLotteryCount(int lottery_type, UserLotteryCount count)
{
	if (lottery_type <= LOTTERY_REPLACE_TYPE_INVALID || lottery_type >= LOTTERY_REPLACE_TYPE_MAX || count == 0)
	{
		return false;
	}
	UserLotteryCount new_count = m_param.lottery_count_param.lottery_count[lottery_type] + count;
	if (new_count < m_param.lottery_count_param.lottery_count[lottery_type])
	{
		return false;
	}
	m_param.lottery_count_param.lottery_count[lottery_type] = new_count;
	return true;
}

RandActivityLuckyCloudBuyParam & WorldStatus3::GetRandActivityLuckyCloudBuyParam()
{
	return m_param.lucky_cloud_buy_param;
}