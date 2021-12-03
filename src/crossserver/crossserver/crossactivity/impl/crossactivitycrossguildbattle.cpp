#include "crossactivitycrossguildbattle.hpp"
#include "crossuser/crossuser.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "crossserver.h"
#include "crossserver/engineadapter.h"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"
#include "servercommon/string/crossstr.h"

#include "internalcomm.h"

CrossActivityGuildBattle::CrossActivityGuildBattle(CrossActivityManager *cross_activity_manager) 
	: CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE), m_open_cfg_idx(0),m_drop_log_count(0)
{

}

CrossActivityGuildBattle::~CrossActivityGuildBattle()
{

}

void CrossActivityGuildBattle::Init(const CrossActivityData &data)
{
	m_status_end_time = this->CalcActivityNextOpenTime();
	m_is_init_finish = true;
}

time_t CrossActivityGuildBattle::CalcActivityNextOpenTime()
{
	if (!CrossGuildBattleConfig::Instance().GetOtherCfg().is_open)
	{
		return 0;
	}

	CrossGuildBattleActivityOpenTimeInfo cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];
	int cfg_count = CrossGuildBattleConfig::Instance().GetOpenActivityConfigList(cfg_list);

	int min_interval = INT_MAX;
	int min_locate_idx = 0;
	for (int index = 0; index < cfg_count; ++ index)
	{
		CrossGuildBattleActivityOpenTimeInfo &open_cfg = cfg_list[index];
		int interval = EngineAdapter::Instance().NextWeekInterval(open_cfg.activity_week_day, 
			open_cfg.activity_ready_time / 100, open_cfg.activity_ready_time % 100, 0);

		if (interval < min_interval)
		{
			min_interval = interval;
			min_locate_idx = index;
		}
	}

	m_open_cfg_idx = min_locate_idx;

	return EngineAdapter::Instance().Time() + min_interval;
}

void CrossActivityGuildBattle::OnCrossUserLogin(CrossUser *cross_user)
{
	if (NULL == cross_user) return;

	UserID user_id = IntToUid(cross_user->GetCrossUid());
	if (CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	crossgameprotocal::CrossGameCrossActivityGotoScene cgtzgs;
	cgtzgs.cross_uid = cross_user->GetCrossUid();
	cgtzgs.cross_activity_type = CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE;
	CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&cgtzgs, sizeof(cgtzgs));
}

void CrossActivityGuildBattle::OnChangeToNextStatus()
{
	if (!CrossGuildBattleConfig::Instance().GetOtherCfg().is_open)
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;
		m_status_end_time = 0;
		return;
	}

	// 设置默认的等待时间跟开启时间
	int ready_dur_s = CROSS_TUANZHAN_DEFAULT_READY_TIME_S;
	int open_dur_s = CROSS_TUANZHAN_DEFAULT_OPEN_TIME_S;

	CrossGuildBattleActivityOpenTimeInfo *open_cfg = CrossGuildBattleConfig::Instance().GetActivityOpenTimeInfo(m_open_cfg_idx);
	if (NULL != open_cfg)
	{
		ready_dur_s = open_cfg->ready_dur_s;
		open_dur_s = open_cfg->open_dur_s;
	}

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		int standby_time = ready_dur_s;
		if (standby_time > 0)
		{
			m_activity_status = ACTIVITY_STATUS_STANDY;
			m_status_end_time = EngineAdapter::Instance().Time() + ready_dur_s;
		}
		else
		{
			m_activity_status = ACTIVITY_STATUS_OPEN;
			m_status_end_time = EngineAdapter::Instance().Time() + open_dur_s;
		}
	}
	else if (ACTIVITY_STATUS_STANDY == m_activity_status)
	{
		m_activity_status = ACTIVITY_STATUS_OPEN;
		m_status_end_time = EngineAdapter::Instance().Time() + open_dur_s;
	}
	else
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;
		m_status_end_time = this->CalcActivityNextOpenTime();
	}
}

void CrossActivityGuildBattle::Update(unsigned long interval, time_t now_second)
{
	CrossActivity::Update(interval, now_second);

	const CrossGuildBattleOtherCfg &other_cfg = CrossGuildBattleConfig::Instance().GetOtherCfg();
	if (!other_cfg.is_open)
	{
		return;
	}

	const tm *local_time = EngineAdapter::Instance().LocalTime();

	if (local_time->tm_hour == other_cfg.special_open_time / 100 && local_time->tm_min == other_cfg.special_open_time % 100 && local_time->tm_sec == 0)
	{
		this->SendSpecialTimeNotice();
	}
	else if (local_time->tm_hour == other_cfg.special_end_time / 100 && local_time->tm_min == other_cfg.special_end_time % 100 && local_time->tm_sec == 0)
	{
		this->SendSpecialTimeNotice();
	}
}

void CrossActivityGuildBattle::OnRegisterGameServer(NetID global_netid)
{
	CrossActivity::OnRegisterGameServer(global_netid);

	NetID hidden_netid = CrossServer::GetInstCrossServer()->GetHiddenServerNetid();
	if (global_netid == hidden_netid)
	{
		return;
	}

	crossgameprotocal::CrossGameGuildBattleRegisterSync  cgrs;
	cgrs.gs_netid = global_netid;

	InternalComm::Instance().NetSend(hidden_netid, (const char *)&cgrs, sizeof(cgrs));
}

void CrossActivityGuildBattle::OnRecvGuildBattleInfo(crossgameprotocal::GameCrossGuildBattleInfoSync *cmd)
{
	if (NULL == cmd)
	{
		return;
	}
	
	crossgameprotocal::CrossGameGuildBattleInfoSync msg;
	
	// 结算
	{
		typedef std::map<UniqueUserID, int> OwnNumMap;
		OwnNumMap own_num_map;
		const int MAX_OWN_NUM = 2;

		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++ i)
		{
			CrossGuildBattleSceneInfo &scene_info = cmd->scene_list[i];
			for (int n = 0; n < CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM; ++ n)
			{
				CrossGuildBattleRankItem &item = scene_info.rank_list[n];
				if (INVALID_UNIQUE_USER_ID != item.guild_id && item.score > 0)
				{
					OwnNumMap::iterator iter = own_num_map.find(item.guild_id);
					if (own_num_map.end() == iter)
					{
						own_num_map.insert(std::make_pair(item.guild_id, 0));
						iter = own_num_map.find(item.guild_id);
					}

					if (iter->second < MAX_OWN_NUM) // 占有数不能超过N个
					{
						UNSTD_STATIC_CHECK(sizeof(msg.owner_list[i]) == sizeof(item));
						memcpy(&msg.owner_list[i], &item, sizeof(msg.owner_list[i]));

						own_num_map[item.guild_id] = iter->second + 1;
						break;
					}
				}
			}
		}
	}
	
	// 传闻
	{
		int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = {0};
		CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);

		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++ i)
		{
			CrossGuildBattleRankItem &item = msg.owner_list[i];
			
			if (INVALID_UNIQUE_USER_ID != item.guild_id && item.score > 0)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_guildbattle_own_city_succc,
					item.guild_id.user_id.db_index, item.guild_name, scene_id_list[i]);
				if (length > 0)
				{
					//m_cross_activity_manager->GetCrossServer()->SystemMsgToAllGlobalUser(gamestring::GAMESTRING_BUF, length);
					m_cross_activity_manager->GetCrossServer()->SystemMsgToAllGlobalUser(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}

	//同步数据
	msg.count = 0;
	for (auto item: cmd->user_core_rank_list)
	{
		if (msg.count >= CROSS_GUILDBATTLE_MAX_USER_SCORE_RANK_NUM) break;

		msg.user_core_rank_list[msg.count].guild_id = item.guild_id;
		msg.user_core_rank_list[msg.count].role_id = item.role_id;
		msg.user_core_rank_list[msg.count].score = item.score;

		msg.count++;
	}

	// 同步回原服
	CrossServer::GetInstCrossServer()->SendToAllGameWorld((const char *)&msg, sizeof(msg));
}


void CrossActivityGuildBattle::OnRecvGuildBattleRankLogInfo(crossgameprotocal::GameCrossGuildBattleRankLogSync *log_cmd)
{
	// 重置数据
	{
		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
		{
			this->m_cross_guildbattle_scene_rank_log_list[i].Reset();
		}
	}
	
	// 目前暂时先做缓存数据
	memcpy(m_cross_guildbattle_scene_rank_log_list, log_cmd->scene_list, sizeof(m_cross_guildbattle_scene_rank_log_list));
}

void CrossActivityGuildBattle::OnRecvGuildBattleGetRankLogInfoReq(NetID global_netid, crossgameprotocal::GameCrossGetGuildBattleRankLogReq *req_cmd)
{
	crossgameprotocal::CrossGameGetGuildBattleRankLogResp resp_msg;
	memcpy(&resp_msg.uni_user_id, &req_cmd->uni_user_id, sizeof(resp_msg.uni_user_id));
	memcpy(resp_msg.scene_list, this->m_cross_guildbattle_scene_rank_log_list, sizeof(resp_msg.scene_list));
	
	InternalComm::Instance().NetSend(global_netid, (const char *)&resp_msg, sizeof(resp_msg));
}

void CrossActivityGuildBattle::OnGetDropLog(const UniqueUserID &unique_user_id)
{
	static Protocol::SCCrossGuildBattleDropLog pro;
	pro.log_count = 0;

	if (m_drop_log_count < CROSS_ACTIVITY_GUILD_BATTLE_DROP_LOG_COUNT_MAX)
	{
		for (; pro.log_count < m_drop_log_count && pro.log_count < CROSS_ACTIVITY_GUILD_BATTLE_DROP_LOG_COUNT_MAX; ++pro.log_count)
		{
			UNSTD_STATIC_CHECK(sizeof(pro.log_list[pro.log_count].name) == sizeof(m_drop_log_list[pro.log_count].name));
			memcpy(pro.log_list[pro.log_count].name, m_drop_log_list[pro.log_count].name, sizeof(m_drop_log_list[pro.log_count].name));
			pro.log_list[pro.log_count].timestamp = m_drop_log_list[pro.log_count].timestamp;
			pro.log_list[pro.log_count].item_id = m_drop_log_list[pro.log_count].item_id;
			pro.log_list[pro.log_count].item_num = m_drop_log_list[pro.log_count].item_num;
		}
	}
	else
	{
		int  lasted_index = m_drop_log_count % CROSS_ACTIVITY_GUILD_BATTLE_DROP_LOG_COUNT_MAX;
		for (int i = lasted_index; i < lasted_index + CROSS_ACTIVITY_GUILD_BATTLE_DROP_LOG_COUNT_MAX && pro.log_count < CROSS_ACTIVITY_GUILD_BATTLE_DROP_LOG_COUNT_MAX; ++i)
		{
			int cur_index = i % CROSS_ACTIVITY_GUILD_BATTLE_DROP_LOG_COUNT_MAX;

			UNSTD_STATIC_CHECK(sizeof(pro.log_list[pro.log_count].name) == sizeof(m_drop_log_list[cur_index].name));
			memcpy(pro.log_list[pro.log_count].name, m_drop_log_list[cur_index].name, sizeof(m_drop_log_list[cur_index].name));
			pro.log_list[pro.log_count].timestamp = m_drop_log_list[cur_index].timestamp;
			pro.log_list[pro.log_count].item_id = m_drop_log_list[cur_index].item_id;
			pro.log_list[pro.log_count].item_num = m_drop_log_list[cur_index].item_num;

			++pro.log_count;
		}
	}

	int send_len = UNSTD_OFFSETOFF(Protocol::SCCrossGuildBattleDropLog, log_list) + sizeof(pro.log_list[0]) * pro.log_count;

	CrossServer::GetInstCrossServer()->SendToUser(unique_user_id, (const char *)&pro, send_len);
}

void CrossActivityGuildBattle::OnSyncDropLog(const UniqueUserID unique_user_id, crossgameprotocal::GameCrossSyncGuildBattleDropInfo *gcdi)
{
	int insert_index = m_drop_log_count % CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX;

	if (insert_index >= 0 && insert_index < CROSS_ACTIVITY_XIULUO_TOWER_DROP_LOG_COUNT_MAX)
	{
		memcpy(m_drop_log_list[insert_index].name, gcdi->name, sizeof(gcdi->name));
		m_drop_log_list[insert_index].timestamp = gcdi->timestamp;
		m_drop_log_list[insert_index].item_id = gcdi->item_id;
		m_drop_log_list[insert_index].item_num = gcdi->item_num;

		++m_drop_log_count;
	}
}

void CrossActivityGuildBattle::SendSpecialTimeNotice()
{
	crossgameprotocal::CrossGameGuildBattleSpecialTimeNotice notice;
	CrossServer::GetInstCrossServer()->SendToAllGameWorld((const char*)&notice, sizeof(notice));
}