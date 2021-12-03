#include "randactivitygiftharvest.hpp"
#include "gameworld/config/activityconfig/giftharvestconfig.hpp"
#include "gameworld/other/roleactivity/roleactivity.hpp"

#include "config/logicconfigmanager.hpp"
#include "item/itempool.h"
#include "other/route/mailroute.hpp"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "other/capability/capability.hpp"
#include "gameworld/global/usercache/usercache.hpp"
#include "gameworld/scene/activityshadow/activityshadow.hpp"
#include <algorithm>

static time_t GetTargetTimestamp(time_t now_second, int hhmm)
{
	struct tm  _tm;
#ifdef _MSC_VER
	localtime_s(&_tm, &now_second);
#else
	localtime_r(&now_second, &_tm);
#endif
	_tm.tm_hour = int(hhmm / 100);
	_tm.tm_min = int(hhmm % 100);
	_tm.tm_sec = 0;
	return mktime(&_tm);
}

static bool RankSortFunc(const RandActivityGiftHarvestParam::RankItem &item1, const RandActivityGiftHarvestParam::RankItem &item2)
{
	if (item1.score == item2.score)
	{
		return item1.score < item2.score;
	}

	return item1.score > item2.score;
}


RandActivityGiftHarvest::RandActivityGiftHarvest(RandActivityManager *activity_manager)
	:RandActivity(activity_manager, RAND_ACTIVITY_TYPE_GIFT_HARVEST), 
	m_is_init(false), m_cur_round(0),m_cur_round_status(GIFT_HARVEST_ROUND_STATUS_CLOSE),
	m_cur_round_ready_timestamp(0),m_cur_round_open_timestamp(0), m_cur_round_close_timestamp(0),
	/*m_cur_rank_num(0),*/rank_update_flag(true)
{
	memset(&m_param, 0, sizeof(m_param));
	//memset(m_cur_round_rank, 0, sizeof(m_cur_round_rank));
}

RandActivityGiftHarvest::~RandActivityGiftHarvest()
{

}

void RandActivityGiftHarvest::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_param) <= sizeof(data.data));
	memcpy(&m_param, data.data, sizeof(m_param));
}

void RandActivityGiftHarvest::GetInitParam(RandActivityData *data)
{
	if(nullptr == data) return;
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_param) <= sizeof(data->data));
	memcpy(data->data, &m_param, sizeof(m_param));
}

void RandActivityGiftHarvest::Update(unsigned long interval, time_t now_second)
{
	RandActivity::Update(interval,now_second);

	if (this->GetStatus() != ACTIVITY_STATUS_OPEN) return;

	// 初始化检查
	if (!m_is_init)
	{
		this->_Init(now_second);

		// 通知场景活动开始，并初始化
		this->_SyncActivityStatusToGameworld(0);

		m_is_init = true;
	}

	// 本轮准备
	if (now_second >= m_cur_round_ready_timestamp && m_cur_round_ready_timestamp != 0)
	{
		this->_ReadyCurRound();
		m_cur_round_ready_timestamp = 0;
		this->SendRoundInfo(); // 发送准备状态、本轮开始时间戳
	}

	// 本轮开始
	if (now_second >= m_cur_round_open_timestamp && m_cur_round_open_timestamp != 0)
	{
		this->_StartCurRound();
		m_cur_round_open_timestamp = 0;
		this->SendRoundInfo(); // 发送开始状态、本轮结束时间戳
	}

	// 本轮结束（设置下一轮时间）
	if (now_second >= m_cur_round_close_timestamp && m_cur_round_close_timestamp != 0)
	{
		this->_StopCurRound();
		this->SendRoundInfo(); // 发送结束状态、下一轮准备时间戳
	}

}

void RandActivityGiftHarvest::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	this->_StopCurRound();
	this->_ResetRoundData(1);
}

void RandActivityGiftHarvest::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		this->_ResetRoundData(1);
		m_is_init = true;

		// 清空排行榜
		m_param.rank_num = 0;
		memset(m_param.m_rank_list, 0, sizeof(m_param.m_rank_list));
	}
	else if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		this->_DrawRankReward();	// 派发排行奖励
		this->_ResetRoundData(0);
	}
}

void RandActivityGiftHarvest::SendRankInfo(Role* role)
{
	if (nullptr == role) return;
	
	static Protocol::SCRAGiftHarvestRankInfo rank_msg;

	// 排行item
	if (rank_update_flag)
	{
		for (int i = 0; i< m_param.rank_num && i < GIFT_HARVEST_PERSON_RANK_MAX; i++)
		{
			auto& item = m_param.m_rank_list[i];
			auto& msg_item = rank_msg.rank_list[i];

			msg_item.role_id = item.role_id;
			msg_item.cur_get_score = item.score;

			if (Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(item.role_id)))
			{
				msg_item.sex = tmp_role->GetSex();
				msg_item.prof = tmp_role->GetProf();
				msg_item.avatar_timestamp = tmp_role->GetAvatarTimeStamp();
				F_STRNCPY(msg_item.name, tmp_role->GetName(), sizeof(GameName));
			}
			else if (UserCacheNode *role_node = UserCacheManager::Instance().GetUserNode(IntToUid(item.role_id)))
			{
				msg_item.sex = role_node->sex;
				msg_item.prof = role_node->prof;
				msg_item.avatar_timestamp = role_node->avatar_timestamp;
				F_STRNCPY(msg_item.name, role_node->user_name, sizeof(GameName));
			}
		}

		rank_update_flag = false;
	}

	// 排行数量
	rank_msg.rank_num = m_param.rank_num;

	//
	int send_size_byte = sizeof(rank_msg) - sizeof(Protocol::GiftHarvestRankItem) * (GIFT_HARVEST_PERSON_RANK_MAX - m_param.rank_num);
	if (send_size_byte > 0 && (unsigned int)send_size_byte <= sizeof(rank_msg))
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rank_msg, send_size_byte);
	}
}

void RandActivityGiftHarvest::SendRoundInfo(Role* role)
{
	//if (nullptr == role) return;

	Protocol::SCGiftHarvestRoundNotice round_msg;

	round_msg.round = m_cur_round;
	switch (m_cur_round_status)
	{
	case GIFT_HARVEST_ROUND_STATUS_STANDY:
	{
		round_msg.round_state = GIFT_HARVEST_ROUND_STATUS_STANDY;
		round_msg.next_state_timestamp = (unsigned int)m_cur_round_open_timestamp;
	}
	break;

	case GIFT_HARVEST_ROUND_STATUS_OPEN:
	{
		round_msg.round_state = GIFT_HARVEST_ROUND_STATUS_OPEN;
		round_msg.next_state_timestamp = (unsigned int)m_cur_round_close_timestamp;
	}
	break;

	case GIFT_HARVEST_ROUND_STATUS_CLOSE:
	{
		round_msg.round_state = GIFT_HARVEST_ROUND_STATUS_CLOSE;
		round_msg.next_state_timestamp = (unsigned int)m_cur_round_ready_timestamp;
	}
	break;

	default:
	{
		round_msg.round_state = GIFT_HARVEST_ROUND_STATUS_CLOSE;
		round_msg.next_state_timestamp = 0;
	}
	break;

	}
	
	if (nullptr == role)
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&round_msg, sizeof(round_msg));
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&round_msg, sizeof(round_msg));
	}
}

void RandActivityGiftHarvest::UpdateRankInfo(Role* role, int score_add)
{
	if(nullptr == role || 0 == score_add) return;

	bool can_sort = false;
	
	// 玩家个人信息记录
	role->GetRoleActivity()->RAGiftHarvestAddScore(score_add);
	int total_role_score = role->GetRoleActivity()->GetRAGiftHarvestScore();
	int role_id = role->GetUID();

	// 在榜
	if (auto * item = this->_GetRankItem(role_id))
	{
		// 直接更新数据
		item->score = total_role_score;
		can_sort = true;
	}
	else // 不在榜
	{
		if (m_param.rank_num < GIFT_HARVEST_PERSON_RANK_MAX)// 未满，则往后添加
		{
			m_param.m_rank_list[m_param.rank_num].role_id = role_id;
			m_param.m_rank_list[m_param.rank_num].score = total_role_score;
			++m_param.rank_num;
			can_sort = true;
		}
		else// 已满，分数需要大于榜尾分数才能上榜
		{
			auto * rankend = this->_GetRankEndItem();
			if (rankend != nullptr && total_role_score > rankend->score)
			{
				rankend->role_id = role_id;
				rankend->score = total_role_score;
				can_sort = true;
			}
		}
	}

	if (can_sort)
	{
		std::sort(m_param.m_rank_list, m_param.m_rank_list + GIFT_HARVEST_PERSON_RANK_MAX, RankSortFunc);
		rank_update_flag = true;
	}
}


void RandActivityGiftHarvest::_Init(time_t now_second)
{
	int round_count = LOGIC_CONFIG->GetGiftHarvestConfig().GetRoundCount();
	for (int i = 1; i <= round_count && i <= GIFT_HARVEST_OPEN_TIME_MAX_COUNT; i++)
	{
		if (const auto* round_time_cfg = LOGIC_CONFIG->GetGiftHarvestConfig().GetGiftHarvestActivityOpenTimeInfo(i))
		{
			if (GetTargetTimestamp(now_second, round_time_cfg->round_end_time) > now_second)
			{
				this->_ResetRoundData(i);
				break;
			}
		}
	}
}

void RandActivityGiftHarvest::_ResetRoundData(int round)
{
	time_t now_sec = EngineAdapter::Instance().Time();

	// 本轮数据
	if (const GiftHarvestRoundOpenTimeInfo * roun_cfg = LOGIC_CONFIG->GetGiftHarvestConfig().GetGiftHarvestActivityOpenTimeInfo(round))
	{
		m_cur_round = round;
		m_cur_round_status = GIFT_HARVEST_ROUND_STATUS_CLOSE;
		m_cur_round_ready_timestamp = GetTargetTimestamp(now_sec, roun_cfg->round_ready_time);
		m_cur_round_open_timestamp = GetTargetTimestamp(now_sec, roun_cfg->round_start_time);
		m_cur_round_close_timestamp = GetTargetTimestamp(now_sec, roun_cfg->round_end_time);
		//m_cur_rank_num = 0;
		//memset(m_cur_round_rank, 0, sizeof(m_cur_round_rank));
	}
	// 重置数据
	else
	{
		m_cur_round = 0;
		m_cur_round_status = GIFT_HARVEST_ROUND_STATUS_CLOSE;
		m_cur_round_open_timestamp = 0;
		m_cur_round_close_timestamp = 0;
		m_cur_round_ready_timestamp = 0;
		//m_cur_rank_num = 0;
		//memset(m_cur_round_rank, 0, sizeof(m_cur_round_rank));
	}
}

void RandActivityGiftHarvest::_DrawRankReward()
{
	// 无人上榜
	if (m_param.rank_num <= 0) return;

	// 遍历排行榜，邮件发奖
	for (int i = 0; i < m_param.rank_num && i < GIFT_HARVEST_PERSON_RANK_MAX; i++)
	{
		auto& rank_item = m_param.m_rank_list[i];
		if (rank_item.role_id <= 0) continue;

		const auto* rank_reward_item = LOGIC_CONFIG->GetRandActivityCfg().GetGiftHarvestRankItem(i);
		if (nullptr != rank_reward_item)
		{
			// 发邮件
			gamecommon::SendMultiMailToRole(IntToUid(rank_item.role_id), rank_reward_item->reward_list, rank_reward_item->num, NULL, false,
				"rand_activity_gift_harvest_rank_mail", i + 1);

			std::string get_item_log;
			ItemConfigDataListLog(get_item_log, rank_reward_item->num, rank_reward_item->reward_list);

			gamelog::g_log_roleactivity.printf(LL_INFO, "RandActivityGiftHarvest::_DrawRankReward user[%d], rank_idx[%d], item_list[%s]",
				rank_item.role_id, i, get_item_log.c_str());
		}
	}

	// 清空排行榜
	m_param.rank_num = 0;
	memset(m_param.m_rank_list, 0, sizeof(m_param.m_rank_list));
}

void RandActivityGiftHarvest::_StopCurRound()
{
	// 已关闭
	if (GIFT_HARVEST_ROUND_STATUS_CLOSE == m_cur_round_status)
		return;

	// 本轮结束
	m_cur_round_status = GIFT_HARVEST_ROUND_STATUS_END;

	// 通知场景结束状态
	this->_SyncActivityStatusToGameworld(GIFT_HARVEST_ROUND_STATUS_END);

	// 设置下轮数据
	this->_ResetRoundData(m_cur_round + 1);

	// 本轮关闭
	m_cur_round_status = GIFT_HARVEST_ROUND_STATUS_CLOSE;
}

void RandActivityGiftHarvest::_StartCurRound()
{
	// 要求从准备状态，到开始状态
	if (GIFT_HARVEST_ROUND_STATUS_STANDY != m_cur_round_status)
		return;

	// 通知场景本轮开启状态
	this->_SyncActivityStatusToGameworld(GIFT_HARVEST_ROUND_STATUS_OPEN);

	// 本轮开启
	m_cur_round_status = GIFT_HARVEST_ROUND_STATUS_OPEN;
}

void RandActivityGiftHarvest::_ReadyCurRound()
{
	// 要求从结束状态，到准备状态
	if (GIFT_HARVEST_ROUND_STATUS_CLOSE != m_cur_round_status)
		return;

	// 通知场景本轮准备状态
	this->_SyncActivityStatusToGameworld(GIFT_HARVEST_ROUND_STATUS_STANDY);

	// 本轮准备状态
	m_cur_round_status = GIFT_HARVEST_ROUND_STATUS_STANDY;
}

void RandActivityGiftHarvest::_SyncActivityStatusToGameworld(int round_status)
{
	// 用于轮次的状态更新，所以不走SceneManager的OnSyncActivityStatus
	static ActivityStatus ggas;
	ggas.activity_type = (short)m_rand_activity_type;
	ggas.status = m_rand_activity_status;
	ggas.activity_begin_time = m_begin_time;
	ggas.activity_end_time = m_end_time;
	ggas.data = round_status;
	ActivityShadow::Instance().OnSyncActivityStatus(&ggas);
	//World::GetInstWorld()->GetSceneManager()->OnSyncActivityStatus(&ggas);
}

RandActivityGiftHarvestParam::RankItem * RandActivityGiftHarvest::_GetRankItem(int role_id)
{
	if (role_id <= 0)
	{
		return nullptr;
	}

	if (m_param.rank_num <= 0 || m_param.rank_num > GIFT_HARVEST_PERSON_RANK_MAX)
	{
		return nullptr;
	}

	int index = 0;
	for (auto& rank_item : m_param.m_rank_list)
	{
		if (++index > m_param.rank_num)
		{
			return nullptr;
		}

		if (rank_item.role_id == role_id)
		{
			return &rank_item;
		}
	}

	return nullptr;
}

RandActivityGiftHarvestParam::RankItem * RandActivityGiftHarvest::_GetRankEndItem()
{
	if (m_param.rank_num <= 0 || m_param.rank_num > GIFT_HARVEST_PERSON_RANK_MAX)
	{
		return nullptr;
	}

	return &m_param.m_rank_list[m_param.rank_num - 1];
}
