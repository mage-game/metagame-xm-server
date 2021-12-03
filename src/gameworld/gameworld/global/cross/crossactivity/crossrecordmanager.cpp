#include "crossrecordmanager.hpp"

#include "servercommon/serverclmempool.h"

#include "rmibackobjdef.h"
#include "internalcomm.h"
#include "engineadapter.h"

#include "obj/character/role.h"
#include "servercommon/serverconfig/serverconfigpool.h"

#include "servercommon/errornum.h"
#include "servercommon/string/globalstr.h"

#include "protocal/msgother.h"

#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "serverlogic.h"
#include "global/rmibackobjdef.h"
#include "servercommon/serverconfig/cross1v1commonconfig.hpp"
#include "global/mail/mailmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "gamelog.h"
#include "other/cross/rolecross.h"

CrossRecordManager & CrossRecordManager::Instance()
{
	static CrossRecordManager _instance;
	return _instance;
}

CrossRecordManager::CrossRecordManager()
	: m_data_state(CROSS_RECORD_MANAGER_DATA_STATE_INVALID), m_last_save_time(0)
{	
	memset(&m_change_list_param, 0, sizeof(m_change_list_param));
}

CrossRecordManager::~CrossRecordManager()
{
	for (UserCrossRecordMapIt it = m_user_cross_record_map.begin(), end = m_user_cross_record_map.end(); it != end; ++ it)
	{
		if (NULL != it->second)
		{
			delete it->second;
		}
	}

	m_user_cross_record_map.clear();
}

bool CrossRecordManager::OnServerStart()
{
	return this->LoadCrossRecord(0);
}

bool CrossRecordManager::LoadCrossRecord(long long id_from)
{
	if (this->IsLoadFinish()) return false;
	
	m_data_state = CROSS_RECORD_MANAGER_DATA_STATE_LOADING;
	
	RMIInitCrossRecordObjectImpl *impl = new RMIInitCrossRecordObjectImpl();

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());  
	return gc.InitCrossRecordAsyn(id_from, impl);

	return 0;
}

void RMIInitCrossRecordObjectImpl::InitCrossRecordRet(int ret, const CrossRecordListParam &crossrecord_param)
{
	if (0 != ret)
	{
		printf("CrossRecordManager::Init Failed ret:%d \n", ret);
		ServerLogic::GetInstServerLogic()->StopServer();
		return;
	}

	long long next_id_from = -1;

	for (int i = 0; i < crossrecord_param.count && i < CrossRecordListParam::CROSS_RECORD_MAX_COUNT; i++)
	{
		if (!CrossRecordManager::Instance().InitCrossRecord(crossrecord_param.cross_record_list[i]))
		{
			printf("CrossRecordManager::InitCrossRecord Failed\n");
			ServerLogic::GetInstServerLogic()->StopServer();
			return;
		}

		if (crossrecord_param.cross_record_list[i].id_cross_record > next_id_from)
		{
			next_id_from = crossrecord_param.cross_record_list[i].id_cross_record;
		}
	}

	if (crossrecord_param.count > 0)
	{
		CrossRecordManager::Instance().LoadCrossRecord(next_id_from);
	}
	else
	{
		CrossRecordManager::Instance().LoadCrossRecordSucc();
	}
}

bool CrossRecordManager::InitCrossRecord(const CrossRecordListParam::CrossRecordAttr &cross_record_attr)
{
	if (cross_record_attr.role_id <= 0)
	{
		gamelog::g_log_serious_error.printf(LL_CRITICAL, "[CrossRecordManager::Init][cross_record_id:%lld role_id:%d is invalid]", cross_record_attr.id_cross_record, cross_record_attr.role_id);
		return false;
	}

	if (m_user_cross_record_map.find(IntToUid(cross_record_attr.role_id)) != m_user_cross_record_map.end())
	{
		gamelog::g_log_serious_error.printf(LL_CRITICAL, "[CrossRecordManager::Init][cross_record_id:%lld role_id:%d repeat]", cross_record_attr.id_cross_record, cross_record_attr.role_id);
		return false;
	}

	CrossRecord *cross_record = new CrossRecord();
	cross_record->Init(cross_record_attr);
	m_user_cross_record_map[cross_record->user_id] = cross_record;

	return true;
}

void CrossRecordManager::LoadCrossRecordSucc()
{
	m_data_state = CROSS_RECORD_MANAGER_DATA_STATE_LOAD_FINISH;

	m_last_save_time = EngineAdapter::Instance().Time();
}

void CrossRecordManager::OnServerStop()
{
	this->CheckSave(EngineAdapter::Instance().Time() + CROSS_RECORD_SAVE_INTERVAL * 2);
}

void CrossRecordManager::Update(unsigned long interval, time_t now_second)
{
	if (!this->IsLoadFinish()) return;

	this->CheckSave(now_second);
}

void CrossRecordManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	for (UserCrossRecordMapIt it = m_user_cross_record_map.begin(), end = m_user_cross_record_map.end(); it != end; ++ it)
	{
		this->SaveCrossRecord(it->second, structcommon::CS_UPDATE);
	}
}

void CrossRecordManager::OnWeekChange()
{
	for (UserCrossRecordMapIt it = m_user_cross_record_map.begin(), end = m_user_cross_record_map.end(); it != end; ++ it)
	{
		this->SaveCrossRecord(it->second, structcommon::CS_UPDATE);
	}
}

bool CrossRecordManager::CheckSave(time_t now_second)
{
	if (!this->IsLoadFinish()) return false;

	if (now_second >= m_last_save_time + CROSS_RECORD_SAVE_INTERVAL || m_change_list_param.count >= CrossRecordListParam::CROSS_RECORD_MAX_COUNT)
	{
		if (m_change_list_param.count > 0)
		{	
			RMISaveCrossRecordObjectImpl *impl = new RMISaveCrossRecordObjectImpl();

			RMIGlobalClient gc;
			gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());  
			if (!gc.SaveCrossRecordAsyn(m_change_list_param, impl)) return false;

			m_change_list_param.count = 0;
		}

		m_last_save_time = now_second;
	}

	return true;
}

CrossRecord * CrossRecordManager::CreateCrossRecord(Role *user)
{
	CrossRecord *cross_record = this->GetCrossRecord(user->GetUserId());
	if (NULL != cross_record) return cross_record;

	cross_record = new CrossRecord();
	cross_record->user_id = user->GetUserId();
	user->GetName(cross_record->role_name);
	cross_record->level = user->GetLevel();
	cross_record->avatar = user->GetAvatar();
	cross_record->sex = user->GetSex();
	cross_record->prof = user->GetProf();
	cross_record->camp = user->GetCamp();
	cross_record->order_view_match_index = -1;

	m_user_cross_record_map[user->GetUserId()] = cross_record;

	this->SaveCrossRecord(cross_record, structcommon::CS_INSERT);

	return cross_record;
}

CrossRecord * CrossRecordManager::GetCrossRecord(const UserID &user_id)
{
	UserCrossRecordMapIt it = m_user_cross_record_map.find(user_id);
	if (it != m_user_cross_record_map.end())
	{
		CrossRecord *cross_record = it->second;
		if (NULL != cross_record && cross_record->user_id == user_id)
		{
			return cross_record;
		}
	}

	return NULL;
}

CrossRecord * CrossRecordManager::GetCrossRecord(Role *user, bool is_auto_create)
{
	CrossRecord *cross_record = this->GetCrossRecord(user->GetUserId());
	if (NULL != cross_record) return cross_record;
	
	if (is_auto_create)
	{
		return this->CreateCrossRecord(user);
	}

	return NULL;
}

void CrossRecordManager::OnSyncRoleBaseInfo(Role *user)
{
	CrossRecord *cross_record = this->GetCrossRecord(user->GetUserId());
	if (NULL == cross_record) return;

	if (cross_record->avatar != user->GetAvatar() || cross_record->sex != user->GetSex() ||
		cross_record->prof != user->GetProf() || cross_record->camp != user->GetCamp() || cross_record->level != user->GetLevel())
	{
		cross_record->avatar = user->GetAvatar();
		cross_record->sex = user->GetSex();
		cross_record->prof = user->GetProf();
		cross_record->camp = user->GetCamp();
		cross_record->level = user->GetLevel();

		this->SaveCrossRecord(cross_record, structcommon::CS_UPDATE);
	}
}

void CrossRecordManager::OnUserResetName(const UserID &user_id, GameName name)
{
	CrossRecord *cross_record = this->GetCrossRecord(user_id);
	if (NULL == cross_record) return;

	F_STRNCPY(cross_record->role_name, name, sizeof(GameName));

	this->SaveCrossRecord(cross_record, structcommon::CS_UPDATE);
}

void CrossRecordManager::OnConnectCrossServerSucc()
{

}

void CrossRecordManager::OnUserLogin(Role *user)
{
	CrossRecord *cross_record = this->GetCrossRecord(user);
	if (NULL != cross_record)
	{
	}
}

void CrossRecordManager::OnUpdateCrossRecord(const UserID &user_id, crossgameprotocal::CrossGameUpdateCrossRecord *cgucr)
{
	if (cgucr->update_cross_record_type <= crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_INVALID ||
		cgucr->update_cross_record_type >= crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_MAX)
	{
		return;
	}

	CrossRecord *cross_record = this->GetCrossRecord(user_id);
	if (NULL == cross_record) return;

	int need_save = true;

	switch (cgucr->update_cross_record_type)
	{
	case crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_1V1:
		{
			UNSTD_STATIC_CHECK(sizeof(cgucr->record_param_list) / sizeof(cgucr->record_param_list[0]) >= 12);

			cross_record->user_record_data.cross_score_1v1 = static_cast<int>(cgucr->record_param_list[0]);
			cross_record->user_record_data.cross_1v1_day_join_count = static_cast<int>(cgucr->record_param_list[1]);
			cross_record->user_record_data.user_capabilit = cgucr->record_param_list[2];
			cross_record->user_record_data.cross_1v1_dur_win_count = static_cast<short>(cgucr->record_param_list[3]);
			cross_record->user_record_data.cross_1v1_total_join_times = static_cast<short>(cgucr->record_param_list[5]);
			cross_record->user_record_data.cross_1v1_total_wim_times = static_cast<short>(cgucr->record_param_list[6]);
			cross_record->user_record_data.cross_1v1_gongxun = static_cast<int>(cgucr->record_param_list[7]);
			cross_record->user_appearance_data = cgucr->appearance;
			cross_record->user_record_data.has_sync_cross_1v1_record_to_role = 1;

			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
			if (NULL != role)
			{
				role->GetRoleCross()->OnSyncCross1V1Info();
				need_save = false;
			}
		}
		break;

	case crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_MULTIUSER_CHALLENGE:
		{
			UNSTD_STATIC_CHECK(sizeof(cgucr->record_param_list) / sizeof(cgucr->record_param_list[0]) >= 5);

			cross_record->user_record_data.cross_multiuser_challenge_score = static_cast<int>(cgucr->record_param_list[0]);
			cross_record->user_record_data.cross_multiuser_challenge_match_total_count += 1;
			cross_record->user_record_data.cross_multiuser_challenge_match_win_count += static_cast<short>(cgucr->record_param_list[1]);
			cross_record->user_record_data.cross_multiuser_challenge_mvp_count += static_cast<short>(cgucr->record_param_list[2]);
			cross_record->user_record_data.cross_multiuser_challenge_add_honor = static_cast<short>(cgucr->record_param_list[3]);
			cross_record->user_record_data.user_capabilit = cgucr->record_param_list[4];
			cross_record->user_record_data.cross_3v3_add_gongxun = static_cast<int>(cgucr->record_param_list[5]);

			cross_record->user_record_data.has_sync_cross_multiuser_challenge_to_role = 1;

			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
			if (NULL != role)
			{
				role->GetRoleCross()->SyncCrossMultiuserChallengeRecordInfo();
				need_save = false;
			}
		}
		break;

	case crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_TUANZHAN:
		{
			cross_record->user_record_data.cross_tuanzhan_score = cgucr->record_param_list[0];
			cross_record->user_record_data.cross_tuanzhan_is_win = cgucr->record_param_list[1];
			cross_record->user_record_data.cross_tuanzhan_side_rank = cgucr->record_param_list[2];
			cross_record->user_record_data.cross_tuanzhan_has_sync_to_role = 1;

			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
			if (NULL != role)
			{
				role->GetRoleCross()->SyncCrossTuanzhanRecordInfo();
				need_save = false;
			}
		}
		break;

	case crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_PASTURE:
		{
			cross_record->user_record_data.cross_pasture_get_score = cgucr->record_param_list[0];
			cross_record->user_record_data.cross_pasture_get_score_times = cgucr->record_param_list[1];
			cross_record->user_record_data.cross_pasture_need_sync_to_role = 1;

			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
			if (NULL != role)
			{
				role->GetRoleCross()->SyncCrossPastureRecordInfo();
				need_save = false;
			}
		}
		break;

	case crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_BOSS:
		{
			cross_record->user_record_data.tire_value = cgucr->record_param_list[0];
			cross_record->user_record_data.today_gather_ordinary_crystal_times = cgucr->record_param_list[1];
			cross_record->user_record_data.today_gather_treasure_crystal_times = cgucr->record_param_list[2];
			cross_record->user_record_data.cross_boss_need_sync_to_role = 1;

			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
			if (NULL != role)
			{
				role->GetRoleCross()->SyncCrossBossRecordInfo();
				role->GetRoleCross()->SetCrossBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[3]), 0);
				role->GetRoleCross()->SetCrossBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[4]), 1);
				role->GetRoleCross()->SetCrossBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[5]), 2);
				role->GetRoleCross()->SetCrossBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[6]), 3);
				role->GetRoleCross()->SetCrossBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[7]), 4);
				need_save = false;
			}
		}
		break;

	case crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_COMMON:
		{
			cross_record->user_record_data.cross_common_use_gold = cgucr->record_param_list[0];
			cross_record->user_record_data.cross_common_need_sync_to_role = 1;

			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
			if (NULL != role)
			{
				role->GetRoleCross()->SyncCrossCommonRecordInfo();
				need_save = false;
			}
		}
		break;
	case crossgameprotocal::CrossGameUpdateCrossRecord::UNDATE_CROSS_RECORD_CROSS_ATTEND:
	{
		cross_record->user_record_data.cross_attend_need_sync_to_role = 1;

		cross_record->user_record_data.attend_activity_num = static_cast<int>(cgucr->record_param_list[0]);

		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
		if (NULL != role)
		{
			role->GetRoleCross()->SyncCrossCommonAttendRecordInfo();
			need_save = false;
		}
	}
	break;

	case crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_MIZANG_BOSS:
	{
		cross_record->user_record_data.cross_mizang_boss_tire_value = cgucr->record_param_list[0];
		cross_record->user_record_data.cross_mizang_boss_today_gather_ordinary_crystal_times = cgucr->record_param_list[1];
		cross_record->user_record_data.cross_mizang_boss_today_gather_treasure_crystal_times = cgucr->record_param_list[2];
		cross_record->user_record_data.cross_mizang_boss_need_sync_to_role = 1;

		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
		if (NULL != role)
		{
			role->GetRoleCross()->SyncCrossMizangBossRecordInfo();
			role->GetRoleCross()->SetCrossMizangBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[3]), 0);
			role->GetRoleCross()->SetCrossMizangBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[4]), 1);
			role->GetRoleCross()->SetCrossMizangBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[5]), 2);
			role->GetRoleCross()->SetCrossMizangBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[6]), 3);
			role->GetRoleCross()->SetCrossMizangBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[7]), 4);
			need_save = false;
		}
	}
	break;

	case crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_YOUMING_BOSS:
	{
		cross_record->user_record_data.cross_youming_boss_tire_value = cgucr->record_param_list[0];
		cross_record->user_record_data.cross_youming_boss_today_gather_ordinary_crystal_times = cgucr->record_param_list[1];
		cross_record->user_record_data.cross_youming_boss_today_gather_treasure_crystal_times = cgucr->record_param_list[2];
		cross_record->user_record_data.cross_youming_boss_need_sync_to_role = 1;

		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
		if (NULL != role)
		{
			role->GetRoleCross()->SyncCrossYoumingBossRecordInfo();
			role->GetRoleCross()->SetCrossYoumingBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[3]), 0);
			role->GetRoleCross()->SetCrossYoumingBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[4]), 1);
			role->GetRoleCross()->SetCrossYoumingBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[5]), 2);
			role->GetRoleCross()->SetCrossYoumingBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[6]), 3);
			role->GetRoleCross()->SetCrossYoumingBossConcernFlag(static_cast<unsigned int>(cgucr->record_param_list[7]), 4);
			need_save = false;
		}
	}
	break;

	}

	if (need_save)
	{
		this->SaveCrossRecord(cross_record, structcommon::CS_UPDATE);
	}
}

void CrossRecordManager::OnGetCrossRecord(NetID cross_netid, const UserID &user_id, const UniqueUserID &req_unique_user_id)
{
	/*
	static const int PLAT_TYPE = LocalConfig::Instance().GetPlatType();

	static Protocol::SCUserCrossRecord ucr;
	ucr.plat_type = PLAT_TYPE;
	ucr.role_id = UidToInt(user_id);

	CrossRecord *cross_record = this->GetCrossRecord(user_id);
	if (NULL != cross_record)
	{
		F_STRNCPY(ucr.role_name, cross_record->role_name, sizeof(GameName));
		ucr.avatar = cross_record->avatar;
		ucr.sex = cross_record->sex;
		ucr.prof = cross_record->prof;
		ucr.camp = cross_record->camp;
		ucr.user_record_data = cross_record->user_record_data;
	}
	else
	{
		GlobalUser *user = m_global_server->GetUserManager()->GetUser(user_id);
		if (NULL == user) return;

		user->GetName(ucr.role_name);
		ucr.avatar = user->GetAvatar();
		ucr.sex = user->GetSex();
		ucr.prof = user->GetProf();
		ucr.camp = user->GetCamp();
		ucr.user_record_data.Reset();
	}

	m_global_server->SendToUserByCross(cross_netid, req_unique_user_id, (const char *)&ucr, sizeof(ucr));
	*/
}

void CrossRecordManager::OnCrossActivityStatusChange(int cross_activity_type, int from_status, int to_status)
{
	
}

void CrossRecordManager::ClearAllCrossChallengeOrderViewInfo(bool is_sync_to_role, int order_view_match_index)
{
	for (UserCrossRecordMapIt it = m_user_cross_record_map.begin(), end = m_user_cross_record_map.end(); it != end; ++ it)
	{
		if (it->second->order_view_match_index >= 0)
		{
			if (order_view_match_index < 0 || it->second->order_view_match_index == order_view_match_index)
			{
				it->second->order_view_match_index = -1;

				this->SaveCrossRecord(it->second, structcommon::CS_UPDATE);

				if (is_sync_to_role)
				{
				}
			}
		}
	}
}

void CrossRecordManager::OnGMAddCrossMultiuserChallengeScore(Role *user, int add_score)
{
	/*
	CrossRecord *cross_record = this->GetCrossRecord(user, true);
	if (NULL != cross_record)
	{
		cross_record->AddCrossMultiuserChallengeScore(add_score);

		this->SaveCrossRecord(cross_record, structcommon::CS_UPDATE);

		cross_record->SyncCrossMultiuserChallengeActivityInfo();
	}
	*/
}

bool CrossRecordManager::SaveCrossRecord(CrossRecord *cross_record, char change_state)
{
	if (NULL == cross_record || !this->CheckSave(EngineAdapter::Instance().Time())) return false;

	if (m_change_list_param.count < CrossRecordListParam::CROSS_RECORD_MAX_COUNT)
	{
		cross_record->GetAttr(&m_change_list_param.cross_record_list[m_change_list_param.count]);
		m_change_list_param.cross_record_list[m_change_list_param.count].change_state = change_state;

		++ m_change_list_param.count;
		return true;
	}

	return false;
}

