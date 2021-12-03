#include "suoyaotamgr.hpp"

#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "config/activityconfig/suoyaotaconfig.hpp"

#include "obj/character/role.h"

#include "servercommon/errornum.h"
#include "servercommon/commondata.hpp"

#include "world.h"
#include "global/worldstatus/worldstatus.hpp"

#include "scene/speciallogic/worldspecial/specialsuoyaota.hpp"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "other/vip/vip.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "protocal/msgfb.h"

SuoYaoTaMgr::SuoYaoTaMgr()
	: m_status(SUOYAOTA_STATUS_TYPE_INVALID), m_next_status_time(0)
{
	memset(m_lv_interval_player_count_list, 0, sizeof(m_lv_interval_player_count_list));
}

SuoYaoTaMgr::~SuoYaoTaMgr()
{

}

SuoYaoTaMgr & SuoYaoTaMgr::Instance()
{
	static SuoYaoTaMgr sytm;
	return sytm;
}

void SuoYaoTaMgr::OnServerStart()
{
	m_status = SUOYAOTA_STATUS_TYPE_INVALID;
	m_next_status_time = LOGIC_CONFIG->GetSuoYaoTaConfig().GetNextStatusTime(m_status);
}

void SuoYaoTaMgr::Update(unsigned long interval, time_t now_second)
{
	if (now_second >= m_next_status_time)
	{
		if (SUOYAOTA_STATUS_TYPE_INVALID == m_status || SUOYAOTA_STATUS_TYPE_STOP == m_status)
		{
			m_status = SUOYAOTA_STATUS_TYPE_STANDBY;
			this->OnActivityStandBy();
		}
		else if (SUOYAOTA_STATUS_TYPE_STANDBY == m_status)
		{
			m_status = SUOYAOTA_STATUS_TYPE_START;
			this->OnActivityStart();
		}
		else if (SUOYAOTA_STATUS_TYPE_START == m_status)
		{
			m_status = SUOYAOTA_STATUS_TYPE_STOP;
			this->OnActivityStop();
		}

		m_next_status_time = LOGIC_CONFIG->GetSuoYaoTaConfig().GetNextStatusTime(m_status);

		this->OnGetSYTState();
	}
}

void SuoYaoTaMgr::OnGetSYTState(Role *role)
{
	static Protocol::SCSuoYaoTaState syts;
	syts.status = m_status;
	syts.next_status_time = m_next_status_time;
	syts.next_standby_time = LOGIC_CONFIG->GetSuoYaoTaConfig().GetNextStandbyTime();
	syts.next_stop_time = LOGIC_CONFIG->GetSuoYaoTaConfig().GetNextStopTime();
	syts.datais_valid = 0;

	if (NULL != role)
	{	
		syts.datais_valid = 1;
		syts.syt_max_score = role->GetCommonDataParam()->suoyaota_max_score;
		syts.quanfu_topscore = WorldStatus::Instance().GetCommonData().syt_topuserinfo.quanfu_topscore;
		syts.quanfu_topscore_uid = WorldStatus::Instance().GetCommonData().syt_topuserinfo.quanfu_topscore_uid;
		F_STRNCPY(syts.quanfu_topscore_name, WorldStatus::Instance().GetCommonData().syt_topuserinfo.quanfu_topscore_name, sizeof(GameName));

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&syts, sizeof(syts));
	}
	else 
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&syts, sizeof(syts));
	}
}

void SuoYaoTaMgr::OnGetSYTReward(Role *role, int times)
{
	CommonDataParam *common_p = role->GetCommonDataParam();
	if (NULL != common_p && common_p->suoyaota_score > 0 && common_p->suoyaota_lv > 0 && times > 0)
	{
		SuoYaoTaLogicCfg *logic_cfg = LOGIC_CONFIG->GetSuoYaoTaConfig().GetSuoYaoTaLogicCfgByLv(common_p->suoyaota_lv);
		if (NULL != logic_cfg && logic_cfg->total_score > 0)
		{
			int factor = 1;

			int cost_gold = 0;
			if (2 == times)
			{
				factor = 2;
				cost_gold = LOGIC_CONFIG->GetSuoYaoTaConfig().GetRewardDoubleCost();
			}
			else if(3 == times)
			{
				factor = 3;
				cost_gold = LOGIC_CONFIG->GetSuoYaoTaConfig().GetRewardThreeCost();
			}

			if (cost_gold > 0 && !role->GetKnapsack()->GetMoney()->UseGold(cost_gold, "SuoYaoTaMgr::OnGetSYTReward"))
			{
				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			double score_factor = (common_p->suoyaota_score * 1.0 / logic_cfg->total_score);
			if (score_factor > 1.0) score_factor = 1.0;

			long long exp = static_cast<long long>(logic_cfg->reward_base_exp * 0.6 + logic_cfg->reward_base_exp * 0.4 * score_factor);
			role->AddExp(exp * factor, "SuoYaoTaMgr::OnGetSYTReward", Role::EXP_ADD_REASON_DEFAULT);
				
			int bind_coin = static_cast<int>(logic_cfg->reward_base_coin * 0.6f + logic_cfg->reward_base_coin * 0.4f * score_factor);
			role->GetKnapsack()->GetMoney()->AddCoinBind(bind_coin, "SuoYaoTaMgr::OnGetSYTReward");

			common_p->suoyaota_score = 0;
			common_p->suoyaota_lv = 0;

			this->NotifySYTReward(role, true);
		}
	}
}

void SuoYaoTaMgr::NotifySYTReward(Role *role, bool force)
{
	CommonDataParam *common_p = role->GetCommonDataParam();
	if (NULL != common_p)
	{
		static Protocol::SCNotifySuoYaoTaReward nsytr;
		nsytr.score = 0;
		nsytr.exp = 0;
		nsytr.bind_coin = 0;

		if (common_p->suoyaota_score > 0 && common_p->suoyaota_lv > 0)
		{
			SuoYaoTaLogicCfg *logic_cfg = LOGIC_CONFIG->GetSuoYaoTaConfig().GetSuoYaoTaLogicCfgByLv(common_p->suoyaota_lv);
			if (NULL != logic_cfg && logic_cfg->total_score > 0)
			{
				float score_factor = (common_p->suoyaota_score * 1.0f / logic_cfg->total_score);
				if (score_factor > 1.0f) score_factor = 1.0f;

				nsytr.score = common_p->suoyaota_score;
				nsytr.exp = static_cast<int>(logic_cfg->reward_base_exp * 0.6f + logic_cfg->reward_base_exp * 0.4f * score_factor);
				nsytr.bind_coin = static_cast<int>(logic_cfg->reward_base_coin * 0.6f + logic_cfg->reward_base_coin * 0.4f * score_factor);
			}
		}

		bool is_send = (common_p->suoyaota_score > 0 && common_p->suoyaota_lv > 0) || force;
		if (is_send)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&nsytr, sizeof(nsytr));
		}
	}
}

void SuoYaoTaMgr::OnJoinActivity(Role *role, bool is_buy_times)
{
	if (SUOYAOTA_STATUS_TYPE_STANDBY != m_status)
	{
		role->NoticeNum(errornum::EN_SUOYAOTA_NOT_STANDBY);
		return;
	}

	if (m_user_room_key_map.find(role->GetUserId()) != m_user_room_key_map.end())
	{
		role->NoticeNum(errornum::EN_SUOYAOTA_THIS_TIME_ALREADY_JOIN);
		return;
	}

	if (!role->GetRoleStatusManager()->CanEnterFB()) 
	{
		return;
	}

	int real_cost = 0;

	const int free_join_times = LOGIC_CONFIG->GetSuoYaoTaConfig().GetFreeJoinTimes();
	
	if (!role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_JOIN_SUOYAOTA, free_join_times))
	{
		if (!is_buy_times)
		{
			role->NoticeNum(errornum::EN_SUOYAOTA_JOIN_LIMIT);
			return;
		}

		const int vip_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_BUY_SYT_TIMES);
		
		if (!role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_JOIN_SUOYAOTA, free_join_times + vip_buy_times))
		{
			role->NoticeNum(errornum::EN_BUY_SUOYAOTA_TIME_LIMIT);
			return;
		}

		const int buy_times_cost = LOGIC_CONFIG->GetSuoYaoTaConfig().GetBuyTimesCost();
		if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(buy_times_cost))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		real_cost = buy_times_cost;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	{
		consume_list.Reset();

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = LOGIC_CONFIG->GetSuoYaoTaConfig().GetJuanZhouItemID();
		stuff_list[0].num = 1;
		stuff_list[0].buyable = false;

		if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list))
		{
			role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}
	}

	role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_JOIN_SUOYAOTA);
	EventHandler::Instance().OnEnterSuoYaoTa(role);

	if (real_cost > 0 && !role->GetKnapsack()->GetMoney()->UseGold(real_cost, "SuoYaoTaMgr::OnJoinActivity"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!ItemExtern::ConsumeItem(role, consume_list, "SuoYaoTaMgr::OnJoinActivity"))
	{
		return;
	}

	int lv_interval_idx = LOGIC_CONFIG->GetSuoYaoTaConfig().GetLvIntervalIdxByRoleLv(role->GetLevel());
	if (lv_interval_idx >= 0 && lv_interval_idx < SUOYAOTA_LEVEL_INTERVAL_MAX)
	{
		m_lv_interval_player_count_list[lv_interval_idx] ++;
		
		Posi enter_pos(0, 0);
		int scene_id = LOGIC_CONFIG->GetSuoYaoTaConfig().GetEnterInfo(&enter_pos);
		if (scene_id > 0)
		{
			int room_key = this->GetLvIntervalRoomMaxKey(lv_interval_idx);
			if (room_key > 0)
			{
				m_user_room_key_map[role->GetUserId()] = room_key;

				World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, room_key, enter_pos);
			}
		}
	}
}

int SuoYaoTaMgr::GetUserRoomKey(const UserID & user_id)
{
	if (SUOYAOTA_STATUS_TYPE_START == m_status || SUOYAOTA_STATUS_TYPE_STANDBY == m_status)
	{
		UserRoomMapIt it = m_user_room_key_map.find(user_id);
		if (it != m_user_room_key_map.end())
		{
			return it->second;
		}
	}

	return 0;
}

void SuoYaoTaMgr::GmForceNextStatus()
{
	m_next_status_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
}

void SuoYaoTaMgr::OnActivityStop()
{
	for (int i = 0; i < SUOYAOTA_LEVEL_INTERVAL_MAX; i++)
	{
		int min_key = this->GetLvIntervalRoomMinKey(i);
		int max_key = this->GetLvIntervalRoomMaxKey(i);

		for (int j = min_key; j <= max_key; j++)
		{
			SpecialSuoYaoTa *special_syt = World::GetInstWorld()->GetSceneManager()->GetSpecialSuoYaoTa(j);
			if (NULL != special_syt)
			{
				special_syt->OnActivityStop();
			}
		}
	}

	memset(m_lv_interval_player_count_list, 0, sizeof(m_lv_interval_player_count_list));
	m_user_room_key_map.clear();
}

void SuoYaoTaMgr::OnActivityStandBy()
{
	memset(m_lv_interval_player_count_list, 0, sizeof(m_lv_interval_player_count_list));
	m_user_room_key_map.clear();
}

void SuoYaoTaMgr::OnActivityStart()
{
	for (int i = 0; i < SUOYAOTA_LEVEL_INTERVAL_MAX; i++)
	{
		int min_key = this->GetLvIntervalRoomMinKey(i);
		int max_key = this->GetLvIntervalRoomMaxKey(i);

		for (int j = min_key; j <= max_key; j++)
		{
			SpecialSuoYaoTa *special_syt = World::GetInstWorld()->GetSceneManager()->GetSpecialSuoYaoTa(j);
			if (NULL != special_syt)
			{
				special_syt->OnActivityStart();
			}
		}
	}
}

int SuoYaoTaMgr::GetLvIntervalRoomMinKey(int lv_interval_idx)
{
	if (lv_interval_idx >= 0 && lv_interval_idx < SUOYAOTA_LEVEL_INTERVAL_MAX) 
	{
		return this->GetLvIntervalRoomKeyHelper(lv_interval_idx, 0);
	}

	return 0;
}

int SuoYaoTaMgr::GetLvIntervalRoomMaxKey(int lv_interval_idx)
{
	if (lv_interval_idx >= 0 && lv_interval_idx < SUOYAOTA_LEVEL_INTERVAL_MAX) 
	{
		int player_count = m_lv_interval_player_count_list[lv_interval_idx];
		if (player_count > 0) 
		{
			const int room_max_user = LOGIC_CONFIG->GetSuoYaoTaConfig().GetRoomMaxUser();
			if (room_max_user > 0)
			{
				return this->GetLvIntervalRoomKeyHelper(lv_interval_idx, (player_count - 1) / room_max_user);
			}
		}
	}
	
	return 0;
}

int SuoYaoTaMgr::GetLvIntervalRoomKeyHelper(int lv_interval_idx, int room_idx)
{
	int room_key = lv_interval_idx * 1000 + room_idx + 1;

	return room_key;
}





