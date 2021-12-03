#include "yaoshouguangchangmgr.hpp"

#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "config/activityconfig/yaoshouguangchangconfig.hpp"

#include "obj/character/role.h"

#include "servercommon/errornum.h"
#include "servercommon/commondata.hpp"

#include "world.h"
#include "global/worldstatus//worldstatus.hpp"

#include "scene/speciallogic/worldspecial/specialyaoshouguangchang.hpp"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "other/vip/vip.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"

YaoShouGuangChangMgr::YaoShouGuangChangMgr()
	: m_status(YAOSHOUGUANGCHANG_STATUS_TYPE_INVALID), m_next_status_time(0)
{
	memset(m_lv_interval_player_count_list, 0, sizeof(m_lv_interval_player_count_list));
}

YaoShouGuangChangMgr::~YaoShouGuangChangMgr()
{

}

YaoShouGuangChangMgr & YaoShouGuangChangMgr::Instance()
{
	static YaoShouGuangChangMgr ysgcm;
	return ysgcm;
}

void YaoShouGuangChangMgr::OnServerStart()
{
	m_status = YAOSHOUGUANGCHANG_STATUS_TYPE_INVALID;
	m_next_status_time = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetNextStatusTime(m_status);
}

void YaoShouGuangChangMgr::Update(unsigned long interval, time_t now_second)
{
	if (now_second >= m_next_status_time)
	{
		if (YAOSHOUGUANGCHANG_STATUS_TYPE_INVALID == m_status || YAOSHOUGUANGCHANG_STATUS_TYPE_STOP == m_status)
		{
			m_status = YAOSHOUGUANGCHANG_STATUS_TYPE_STANDBY;
			this->OnActivityStandBy();
		}
		else if (YAOSHOUGUANGCHANG_STATUS_TYPE_STANDBY == m_status)
		{
			m_status = YAOSHOUGUANGCHANG_STATUS_TYPE_START;
			this->OnActivityStart();
		}
		else if (YAOSHOUGUANGCHANG_STATUS_TYPE_START == m_status)
		{
			m_status = YAOSHOUGUANGCHANG_STATUS_TYPE_STOP;
			this->OnActivityStop();
		}

		m_next_status_time = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetNextStatusTime(m_status);

		this->OnGetYSGCState();
	}
}

void YaoShouGuangChangMgr::OnGetYSGCState(Role *role)
{
	static Protocol::SCYaoShouGuangChangState ysgcs;
	ysgcs.status = m_status;
	ysgcs.next_status_time = m_next_status_time;
	ysgcs.next_standy_time = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetNextStandbyTime();
	ysgcs.next_stop_time = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetNextStopTime();
	ysgcs.datais_valid = 0;

	if (NULL != role)
	{
		ysgcs.datais_valid = 1;
		ysgcs.ysgc_max_score = role->GetCommonDataParam()->yaoshouguangchang_max_score;
		ysgcs.quanfu_topscore = WorldStatus::Instance().GetCommonData().ysgc_topuserinfo.quanfu_topscore;
		ysgcs.quanfu_topscore_uid = WorldStatus::Instance().GetCommonData().ysgc_topuserinfo.quanfu_topscore_uid;
		F_STRNCPY(ysgcs.quanfu_topscore_name, WorldStatus::Instance().GetCommonData().ysgc_topuserinfo.quanfu_topscore_name, sizeof(GameName));

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ysgcs, sizeof(ysgcs));
	}
	else 
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&ysgcs, sizeof(ysgcs));
	}
}

void YaoShouGuangChangMgr::OnGetYSGCReward(Role *role, int times)
{
	CommonDataParam *common_p = role->GetCommonDataParam();
	if (NULL != common_p && common_p->yaoshouguangchang_score > 0 && common_p->yaoshouguangchang_lv > 0 && times > 0)
	{
		YaoShouGuangChangLogicCfg *logic_cfg = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetYaoShouGuangChangLogicCfgByLv(common_p->yaoshouguangchang_lv);
		if (NULL != logic_cfg && logic_cfg->total_score > 0)
		{
			int factor = 1;

			int cost_gold = 0;
			if (2 == times)
			{
				factor = 2;
				cost_gold = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetRewardDoubleCost();
			}
			else if(3 == times)
			{
				factor = 3;
				cost_gold = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetRewardThreeCost();
			}

			if (cost_gold > 0 && !role->GetKnapsack()->GetMoney()->UseGold(cost_gold, "YaoShouGuangChangMgr::OnGetYSGCReward"))
			{
				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			double score_factor = (common_p->yaoshouguangchang_score * 1.0 / logic_cfg->total_score);
			if (score_factor > 1.0) score_factor = 1.0;

			long long exp = static_cast<long long>(logic_cfg->reward_base_exp * 0.6 + logic_cfg->reward_base_exp * 0.4 * score_factor);
			role->AddExp(exp * factor, "YaoShouGuangChangMgr::OnGetYSGCReward", Role::EXP_ADD_REASON_DEFAULT);
				
			int bind_coin = static_cast<int>(logic_cfg->reward_base_coin * 0.6 + logic_cfg->reward_base_coin * 0.4 * score_factor);
			role->GetKnapsack()->GetMoney()->AddCoinBind(bind_coin, "YaoShouGuangChangMgr::OnGetYSGCReward");

			common_p->yaoshouguangchang_score = 0;
			common_p->yaoshouguangchang_lv = 0;

			this->NotifyYSGCReward(role, true);
		}
	}
}

void YaoShouGuangChangMgr::NotifyYSGCReward(Role *role, bool force)
{
	CommonDataParam *common_p = role->GetCommonDataParam();
	if (NULL != common_p)
	{
		static Protocol::SCNotifyYaoShouGuangChangReward nysgcr;
		nysgcr.score = 0;
		nysgcr.exp = 0;
		nysgcr.bind_coin = 0;

		if (common_p->yaoshouguangchang_score > 0 && common_p->yaoshouguangchang_lv > 0)
		{
			YaoShouGuangChangLogicCfg *logic_cfg = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetYaoShouGuangChangLogicCfgByLv(common_p->yaoshouguangchang_lv);
			if (NULL != logic_cfg && logic_cfg->total_score > 0)
			{
				float score_factor = (common_p->yaoshouguangchang_score * 1.0f / logic_cfg->total_score);
				if (score_factor > 1.0f) score_factor = 1.0f;

				nysgcr.score = common_p->yaoshouguangchang_score;
				nysgcr.exp = static_cast<int>(logic_cfg->reward_base_exp * 0.6f + logic_cfg->reward_base_exp * 0.4f * score_factor);
				nysgcr.bind_coin = static_cast<int>(logic_cfg->reward_base_coin * 0.6f + logic_cfg->reward_base_coin * 0.4f * score_factor);
			}
		}

		bool is_send = (common_p->yaoshouguangchang_score > 0 && common_p->yaoshouguangchang_lv > 0) || force;
		if (is_send)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&nysgcr, sizeof(nysgcr));
		}
	}
}

void YaoShouGuangChangMgr::OnJoinActivity(Role *role, bool is_buy_times)
{
	if (YAOSHOUGUANGCHANG_STATUS_TYPE_STANDBY != m_status)
	{
		role->NoticeNum(errornum::EN_YAOSHOUGUANGCHANG_NOT_STANDBY);
		return;
	}

	if (m_user_room_key_map.find(role->GetUserId()) != m_user_room_key_map.end())
	{
		role->NoticeNum(errornum::EN_YAOSHOUGUANGCHANG_THIS_TIME_ALREADY_JOIN);
		return;
	}

	if (!role->GetRoleStatusManager()->CanEnterFB()) 
	{
		return;
	}

	const int free_join_times = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetFreeJoinTimes();
	
	int real_cost = 0;

	if (!role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_JOIN_YAOSHOUGUANGCHANG, free_join_times))
	{
		if (!is_buy_times)
		{
			role->NoticeNum(errornum::EN_YAOSHOUGUANGCHANG_JOIN_LIMIT);
			return;
		}

		const int vip_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_BUY_YSGC_TIMES);
		
		if (!role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_JOIN_YAOSHOUGUANGCHANG, free_join_times + vip_buy_times))
		{
			role->NoticeNum(errornum::EN_BUY_YAOSHOUGUANGCHANG_TIME_LIMIT);
			return;
		}

		const int buy_times_cost = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetBuyTimesCost();
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
		stuff_list[0].item_id = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetJuanZhouItemID();
		stuff_list[0].num = 1;
		stuff_list[0].buyable = false;

		if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list))
		{
			role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}
	}

	role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_JOIN_YAOSHOUGUANGCHANG);
	EventHandler::Instance().OnEnterYaoShouGuangChang(role);

	if (real_cost > 0 && !role->GetKnapsack()->GetMoney()->UseGold(real_cost, "YaoShouGuangChangMgr::OnJoinActivity"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// ÏûºÄ
	if (!ItemExtern::ConsumeItem(role, consume_list, "YaoShouGuangChangMgr::OnJoinActivity"))
	{
		return;
	}

	int lv_interval_idx = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetLvIntervalIdxByRoleLv(role->GetLevel());
	if (lv_interval_idx >= 0 && lv_interval_idx < YAOSHOUGUANGCHANG_LEVEL_INTERVAL_MAX)
	{
		m_lv_interval_player_count_list[lv_interval_idx] ++;
		
		Posi enter_pos(0, 0);
		int scene_id = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetEnterInfo(&enter_pos);
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

int YaoShouGuangChangMgr::GetUserRoomKey(const UserID & user_id)
{
	if (YAOSHOUGUANGCHANG_STATUS_TYPE_START == m_status || YAOSHOUGUANGCHANG_STATUS_TYPE_STANDBY == m_status)
	{
		UserRoomMapIt it = m_user_room_key_map.find(user_id);
		if (it != m_user_room_key_map.end())
		{
			return it->second;
		}
	}

	return 0;
}

void YaoShouGuangChangMgr::GmForceNextStatus()
{
	m_next_status_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
}

void YaoShouGuangChangMgr::OnActivityStop()
{
	for (int i = 0; i < YAOSHOUGUANGCHANG_LEVEL_INTERVAL_MAX; i++)
	{
		int min_key = this->GetLvIntervalRoomMinKey(i);
		int max_key = this->GetLvIntervalRoomMaxKey(i);

		for (int j = min_key; j <= max_key; j++)
		{
			SpecialYaoShouGuangChang *special_ysgc = World::GetInstWorld()->GetSceneManager()->GetSpecialYaoShouGuangChang(j);
			if (NULL != special_ysgc)
			{
				special_ysgc->OnActivityStop();
			}
		}
	}

	memset(m_lv_interval_player_count_list, 0, sizeof(m_lv_interval_player_count_list));
	m_user_room_key_map.clear();
}

void YaoShouGuangChangMgr::OnActivityStandBy()
{
	memset(m_lv_interval_player_count_list, 0, sizeof(m_lv_interval_player_count_list));
	m_user_room_key_map.clear();
}

void YaoShouGuangChangMgr::OnActivityStart()
{
	for (int i = 0; i < YAOSHOUGUANGCHANG_LEVEL_INTERVAL_MAX; i++)
	{
		int min_key = this->GetLvIntervalRoomMinKey(i);
		int max_key = this->GetLvIntervalRoomMaxKey(i);

		for (int j = min_key; j <= max_key; j++)
		{
			SpecialYaoShouGuangChang *special_ysgc = World::GetInstWorld()->GetSceneManager()->GetSpecialYaoShouGuangChang(j);
			if (NULL != special_ysgc)
			{
				special_ysgc->OnActivityStart();
			}
		}
	}
}

int YaoShouGuangChangMgr::GetLvIntervalRoomMinKey(int lv_interval_idx)
{
	if (lv_interval_idx >= 0 && lv_interval_idx < YAOSHOUGUANGCHANG_LEVEL_INTERVAL_MAX) 
	{
		return this->GetLvIntervalRoomKeyHelper(lv_interval_idx, 0);
	}

	return 0;
}

int YaoShouGuangChangMgr::GetLvIntervalRoomMaxKey(int lv_interval_idx)
{
	if (lv_interval_idx >= 0 && lv_interval_idx < YAOSHOUGUANGCHANG_LEVEL_INTERVAL_MAX) 
	{
		int player_count = m_lv_interval_player_count_list[lv_interval_idx];
		if (player_count > 0) 
		{
			const int room_max_user = LOGIC_CONFIG->GetYaoShouGuangChangConfig().GetRoomMaxUser();
			if (room_max_user > 0)
			{
				return this->GetLvIntervalRoomKeyHelper(lv_interval_idx, (player_count - 1) / room_max_user);
			}
		}
	}
	
	return 0;
}

int YaoShouGuangChangMgr::GetLvIntervalRoomKeyHelper(int lv_interval_idx, int room_idx)
{
	int room_key = lv_interval_idx * 1000 + room_idx + 1;

	return room_key;
}





