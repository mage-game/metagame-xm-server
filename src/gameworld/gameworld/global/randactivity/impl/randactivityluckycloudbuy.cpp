#include "randactivityluckycloudbuy.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "gameworld/world.h"
#include "gameworld/global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/commondata/commondataglobal.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "gameworld/item/itempool.h"
#include "gameworld/other/route/mailroute.hpp"
#include "servercommon/string/gameworldstr.h"
#include "global/usercache/usercache.hpp"
#include "gameworld/global/luckylog/luckylogmanager.hpp"
#include "gameworld/protocal/msgactivity.hpp"
#include <time.h>
#include "obj/character/role.h"
#include "global/worldstatus3/worldstatus3.hpp"

time_t GetTargetTimestamp(time_t now_second, int hour, int minu)
{
	struct tm  _tm;
#ifdef _MSC_VER
	localtime_s(&_tm, &now_second);
#else
	localtime_r(&now_second, &_tm);
#endif
	_tm.tm_hour = hour;//int(begin_time / 100);
	_tm.tm_min = minu;//int(begin_time % 100);
	_tm.tm_sec = 0;
	return mktime(&_tm);
}

RandActivityLuckyCloudBuy::RandActivityLuckyCloudBuy(RandActivityManager *rand_activity_manager)
	: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY)
{
	m_rounds_num = 0;
	m_is_init = false;
	m_round_index = 0;
	m_this_round_sale_num = 0;
	m_this_round_buy_num = 0;
	m_this_round_end_tick = 0;
	m_next_round_begin_tick = 0;
	m_last_buy_timestamp = 0;
	m_can_buy = false;
	m_is_this_round_end = true;
}

RandActivityLuckyCloudBuy::~RandActivityLuckyCloudBuy()
{

}

void RandActivityLuckyCloudBuy::Update(unsigned long interval, time_t now_second)
{
	RandActivity::Update(interval, now_second);

	if (ACTIVITY_STATUS_OPEN != m_rand_activity_status)
	{
		return;
	}

	// 若活动期间，服务器重启，导致数据未初始化，则进行初始化
	if (!m_is_init)
	{
		InitToday();
	}

	// 轮次结束时间到，计算奖励
	if (now_second >= m_this_round_end_tick && m_this_round_end_tick > 0)
	{
		// 结束本轮（不清空本轮数据，仅结算，及标志本轮结束状态）
		StopCurrentRound();
		m_is_this_round_end = true;
		m_this_round_end_tick = 0;
	}

	// 下一轮时间道，开启
	if (now_second >= m_next_round_begin_tick && m_next_round_begin_tick > 0)
	{
		// 开启下一轮（数据重置，及标志本轮开始状态）
		StartNextRound(now_second);
		m_is_this_round_end = false;
	}

	// （新需求）卖完立即开奖，然后继续购买
	if (m_this_round_buy_num >= m_this_round_sale_num && m_this_round_sale_num != 0
		&& false == m_is_this_round_end)
	{
		int lucky_role_id = GetLotteryDrawing();
		if (lucky_role_id > 0)
		{
			RewardLucky(lucky_role_id);
			m_this_round_buy_num = 0;
			m_role_id_buy_map.clear();
			m_can_buy = true;
			NoticeAllCanBuy(1);
		}
	}
}
void RandActivityLuckyCloudBuy::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	RandActivity::OnDayChange(old_dayid, now_dayid);

	if (ACTIVITY_STATUS_OPEN != m_rand_activity_status)
	{
		return;
	}

	InitToday();
}
void RandActivityLuckyCloudBuy::OnUserLogin(Role *user)
{
	RandActivity::OnUserLogin(user);

	if (ACTIVITY_STATUS_OPEN != m_rand_activity_status)
	{
		return;
	}

	Protocol::SCRALuckyCloudBuyOpenInfo msg;
	msg.is_open = m_is_this_round_end ? 0 : 1;
	EngineAdapter::Instance().NetSend(user->GetNetId(), (const char *)&msg, sizeof(msg));
}
void RandActivityLuckyCloudBuy::OnUserLogout(Role *user)
{
	if (ACTIVITY_STATUS_OPEN != m_rand_activity_status)
	{
		return;
	}

	// 玩家离线，则表示关闭了面板
	RecordCloseState(user);
}
void RandActivityLuckyCloudBuy::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		// 本轮尚未结束,则直接强制结束
		if (m_this_round_end_tick >= EngineAdapter::Instance().Time())
		{
			StopCurrentRound();
		}

		// 数据清空
		m_round_index = 0;
		m_this_round_sale_num = 0;
		m_this_round_buy_num = 0;
		m_this_round_end_tick = 0;
		m_next_round_begin_tick = 0;
		m_last_buy_timestamp = 0;
		m_role_id_buy_map.clear();
		m_buy_record.clear();
		m_can_buy = false;

		RandActivityLuckyCloudBuyParam & param = WorldStatus3::Instance().GetRandActivityLuckyCloudBuyParam();
		param.Reset();
	}
	else if (ACTIVITY_STATUS_OPEN == to_status)
	{
		InitToday();
	}

}

void RandActivityLuckyCloudBuy::InitActivityData()
{
	RandActivityLuckyCloudBuyParam & param = WorldStatus3::Instance().GetRandActivityLuckyCloudBuyParam();
	if (param.round_sale_num <= 0 || param.round_end_tick <= 0)
	{
		return;
	}

	m_is_init = true;
	m_rounds_num = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyBuyRoundsNum(param.round_end_tick);
	m_round_index = param.round_index;
	m_this_round_buy_num = param.round_buy_num;
	m_this_round_sale_num = param.round_sale_num;
	m_this_round_end_tick = param.round_end_tick;
	m_next_round_begin_tick = param.next_round_begin_tick;
	m_can_buy = true;
	m_is_this_round_end = false;

	for (int i = 0; i < LUCKY_BUY_ROLE_RECORD_MAX_NUM; ++i)
	{
		if (param.role_buy[i].role_id == 0)
		{
			break;
		}
		m_role_id_buy_map.insert(std::make_pair(param.role_buy[i].role_id, param.role_buy[i].buy_num));
	}

	m_buy_record.clear();
	for (int i = 0; i < LUCKY_BUY_RECORD_MAX_NUM; ++i)
	{
		if (param.buy_log[i].role_id == 0)
		{
			break;
		}
		m_buy_record.push_back(param.buy_log[i]);
	}

// 	if (m_this_round_end_tick > 0 && EngineAdapter::Instance().Time() > m_this_round_end_tick && m_this_round_buy_num >0)
// 	{
// 		int total_num = 0;
// 		for (auto it = m_role_id_buy_map.begin(); it != m_role_id_buy_map.end(); ++it)
// 		{
// 			total_num += it->second;
// 		}
// 
// 		int lucky_role_id = 0;
// 		{
// 			int rand_select = RandomNum(0, total_num);
// 			int rand_sum = 0;
// 			auto itr = m_role_id_buy_map.begin();
// 			for (; itr != m_role_id_buy_map.end(); ++itr)
// 			{
// 				rand_sum += itr->second;
// 				if (rand_sum >= rand_select)
// 				{
// 					lucky_role_id = itr->first;
// 					break;
// 				}
// 			}
// 		}
// 
// 		// 发奖
// 		if (lucky_role_id > 0)
// 		{
// 			this->RewardLucky(lucky_role_id);
// 		}
// 	}
// 
// 	if (m_this_round_end_tick > 0 && EngineAdapter::Instance().Time() > m_this_round_end_tick)
// 	{
// 		InitToday();
// 	}
	param.Reset();
}

void RandActivityLuckyCloudBuy::SaveActivityData()
{
	if (ACTIVITY_STATUS_OPEN != m_rand_activity_status)
	{
		return;
	}

	RandActivityLuckyCloudBuyParam & param = WorldStatus3::Instance().GetRandActivityLuckyCloudBuyParam();
	
	if (!m_is_this_round_end)
	{
		param.round_index = m_round_index;
		param.round_buy_num = m_this_round_buy_num;
		param.round_sale_num = m_this_round_sale_num;
		param.round_end_tick = static_cast<unsigned int>(m_this_round_end_tick);
		param.next_round_begin_tick = static_cast<unsigned int>(m_next_round_begin_tick);
		memset(param.role_buy, 0, sizeof(param.role_buy));
		memset(param.buy_log, 0, sizeof(param.buy_log));

		std::vector<RandActivityLuckyCloudBuyParam::RoleBuy> buy_vec;
		for (auto it = m_role_id_buy_map.begin(); it != m_role_id_buy_map.end(); ++it)
		{
			RandActivityLuckyCloudBuyParam::RoleBuy role_buy;
			role_buy.role_id = it->first;
			role_buy.buy_num = it->second;
			buy_vec.push_back(role_buy);
		}
		std::sort(buy_vec.begin(), buy_vec.end(), [](const RandActivityLuckyCloudBuyParam::RoleBuy & role1, const RandActivityLuckyCloudBuyParam::RoleBuy & role2)->bool {return role1.buy_num > role2.buy_num; });
		for (unsigned int i = 0; i < buy_vec.size() && i < LUCKY_BUY_ROLE_RECORD_MAX_NUM; ++i)
		{
			param.role_buy[i] = buy_vec[i];
		}

		int num = 0;
		for (auto & item : m_buy_record)
		{
			if (num < LUCKY_BUY_RECORD_MAX_NUM)
			{
				param.buy_log[num] = item;
			}
			num++;
		}
	}
	else
	{
		param.Reset();
	}
}

bool RandActivityLuckyCloudBuy::IsInCurrentRound(int role_id)
{
	return  m_role_id_buy_map[role_id] > 0;
}
bool RandActivityLuckyCloudBuy::IsAllRoundFinish()
{
	if (0 == m_rounds_num)
	{
		return true;
	}

	if (m_round_index + 1 == m_rounds_num
		&& m_this_round_buy_num == m_this_round_sale_num
		&& !m_can_buy)
	{
		return true;
	}

	return false;
}
bool RandActivityLuckyCloudBuy::CanJoin()
{
	return m_can_buy;
}
bool RandActivityLuckyCloudBuy::IsThisRoundAllSell()
{
	if (m_this_round_buy_num == m_this_round_sale_num)
	{
		return true;
	}
	return false;
}
short RandActivityLuckyCloudBuy::GetThisRoundBuyNum()
{
	return m_this_round_buy_num;
}
short RandActivityLuckyCloudBuy::GetThisRoundIndex()
{
	return m_round_index;
}

void RandActivityLuckyCloudBuy::SendBuyRecordInfo(Role *role, unsigned int last_open_timestamp)
{
	// 发送购买记录（只发打开活动面板的玩家）
	Protocol::SCRALuckyCloudBuyBuyList buy_list_info;
	int num = 0;
	for (auto& item : m_buy_record)
	{
		if (num >= LUCKY_BUY_RECORD_MAX_NUM || last_open_timestamp > m_last_buy_timestamp) break;

		if (item.timestamp >= last_open_timestamp)  // 只发时间戳之后的
		{
			memcpy(&buy_list_info.name_list[num], item.name, sizeof(GameName));
			num++;
		}
	}

	buy_list_info.ret_timestamp = m_last_buy_timestamp + 1;
	buy_list_info.total_buy = m_this_round_buy_num;
	buy_list_info.num = num;
	int send_size = sizeof(buy_list_info) - sizeof(GameName) * (LUCKY_BUY_RECORD_MAX_NUM - num);

	if (nullptr == role)
	{
		for (auto role_id : m_open_panel_role)
		{
			if (Role* target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id)))
			{
				EngineAdapter::Instance().NetSend(target_role->GetNetId(), (const char *)&buy_list_info, send_size);
				target_role->GetRoleActivity()->CheckRALuckyCloudBuyReturn();
			}
			else
			{
				auto seach = m_open_panel_role.find(role_id);
				if (seach != m_open_panel_role.end())
				{
					m_open_panel_role.erase(seach);
				}
			}
		}
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&buy_list_info, send_size);
	}
}
void RandActivityLuckyCloudBuy::RecordBuy(Role* role)
{
	if (nullptr == role)
	{
		return;
	}

	if (IsThisRoundAllSell())
	{
		return;
	}

	// 记录购买的时间戳
	m_last_buy_timestamp = (unsigned int)EngineAdapter::Instance().Time();
	m_this_round_buy_num++;

	// 记录购买log
	RandActivityLuckyCloudBuyParam::BuyLogItem buy_log_item;
	{
		buy_log_item.role_id = role->GetUID();
		memcpy(buy_log_item.name, role->GetName(), sizeof(GameName));
		buy_log_item.timestamp = m_last_buy_timestamp;

		if (m_buy_record.size() >= LUCKY_BUY_RECORD_MAX_NUM)
		{
			m_buy_record.pop_front();
		}
	}
	m_buy_record.push_back(buy_log_item);
	m_role_id_buy_map[role->GetUID()]++;

	// 发送1条最新的购买记录
	Protocol::SCRALuckyCloudBuyBuyList buy_list_info;
	memcpy(&buy_list_info.name_list[0], role->GetName(), sizeof(GameName));
	buy_list_info.ret_timestamp = m_last_buy_timestamp + 1;
	buy_list_info.total_buy = m_this_round_buy_num;
	buy_list_info.num = 1;
	int send_size = sizeof(buy_list_info) - sizeof(GameName) * (LUCKY_BUY_RECORD_MAX_NUM - 1);
	for (auto role_id : m_open_panel_role)
	{
		if (Role* target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id)))
		{
			EngineAdapter::Instance().NetSend(target_role->GetNetId(), (const char *)&buy_list_info, send_size);
			target_role->GetRoleActivity()->CheckRALuckyCloudBuyReturn();
		}
		else
		{
			auto seach = m_open_panel_role.find(role_id);
			if (seach != m_open_panel_role.end())
			{
				m_open_panel_role.erase(seach);
			}
		}
	}

	// 本轮全部购完
	if (IsThisRoundAllSell())
	{
		m_can_buy = false;
	}
}
void RandActivityLuckyCloudBuy::RecordOpenState(Role* role)
{
	if (nullptr == role)
	{
		return;
	}
	m_open_panel_role.insert(role->GetUID());
}
void RandActivityLuckyCloudBuy::RecordCloseState(Role* role)
{
	if (nullptr == role)
	{
		return;
	}

	auto seach = m_open_panel_role.find(role->GetUID());
	if (seach != m_open_panel_role.end())
	{
		m_open_panel_role.erase(seach);
	}
}

void RandActivityLuckyCloudBuy::InitToday()
{
	// 今日将进行的总轮数
	m_rounds_num = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyBuyRoundsNum();
	m_round_index = 0;
	m_next_round_begin_tick = 0;

	time_t now_sec = EngineAdapter::Instance().Time();

	// 设置第1轮开启时间
	for (int i = 0; i < LUCKY_CLOUD_BUY_ROUND_MAX_NUM; i++)
	{
		auto *first_round_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyCloudBuyCfg(i);
		if (nullptr == first_round_cfg)
		{
			m_next_round_begin_tick = 0;
			break;
		}

		//time_t next_round_open_time = GetTargetTimestamp(now_sec, first_round_cfg->begin_time_hour, first_round_cfg->begin_time_min);
		//if (next_round_open_time >= now_sec)
		//{
		//	m_next_round_begin_tick = next_round_open_time;
		//	m_round_index = i - 1;
		//	break;
		//}

		// 需求 改为立即开启
		time_t next_round_end_time = GetTargetTimestamp(now_sec, first_round_cfg->end_time_hour, first_round_cfg->end_time_min);
		if (next_round_end_time >= now_sec)
		{
			time_t next_round_open_time = GetTargetTimestamp(now_sec, first_round_cfg->begin_time_hour, first_round_cfg->begin_time_min);
			m_next_round_begin_tick = next_round_open_time;
			m_round_index = i - 1;
			break;
		}
	}

	// 数据清空
	m_this_round_sale_num = 0;
	m_this_round_buy_num = 0;
	m_this_round_end_tick = 0;
	m_last_buy_timestamp = 0;
	m_role_id_buy_map.clear();
	m_can_buy = false;
	m_is_this_round_end = true;

	// 
	m_is_init = true;
}

void RandActivityLuckyCloudBuy::NoticeAllCanBuy(int open_flag)
{
	Protocol::SCRALuckyCloudBuyOpenInfo msg;
	msg.is_open = open_flag;
	World::GetInstWorld()->SendToAllGateway((const char *)&(msg), sizeof(msg));
}

int RandActivityLuckyCloudBuy::GetLotteryDrawing()
{
	// 检查是否买完
	if (!this->IsThisRoundAllSell())
	{
		return 0;
	}

	// 选取随机点选中的玩家
	int total_num = m_this_round_sale_num;
	int lucky_role_id = 0;
	{
		int rand_select = RandomNum(0, total_num);
		int rand_sum = 0;
		auto itr = m_role_id_buy_map.begin();

		//新需求，买满后，排行第一名的人如果超过X%的占比，就直接给大奖
		{
			int max_buy_num = 0;
			for (const auto &it_top1: m_role_id_buy_map)
			{
				if (it_top1.second > max_buy_num && it_top1.second > static_cast<int>(this->GetThisRoundBuyNum() * LOGIC_CONFIG->GetRandActivityCfg().GetLuckyBuyLuckyRewardBuyPercent() * 0.01))
				{
					lucky_role_id = it_top1.first;
					max_buy_num = it_top1.second;
				}
			}
		}

		if (lucky_role_id == 0)
		{
			for (; itr != m_role_id_buy_map.end(); ++itr)
			{
				rand_sum += itr->second;
				if (rand_sum >= rand_select)
				{
					lucky_role_id = itr->first;
					break;
				}
			}
		}
	}

	return lucky_role_id;
}
void RandActivityLuckyCloudBuy::RewardLucky(int role_id, unsigned int join_timestamp)
{
	if (role_id <= 0)
	{
		return;
	}

	auto *round_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyCloudBuyCfg(m_round_index);
	if (round_cfg == nullptr)
	{
		return;
	}

	// 为幸运儿发奖
	const ItemBase *item_base = ITEMPOOL->GetItem(round_cfg->big_reward_item.item_id);
	if (NULL != item_base)
	{
		static MailContentParam contentparam; contentparam.Reset();
		contentparam.item_list[0].item_id = round_cfg->big_reward_item.item_id;
		contentparam.item_list[0].num = round_cfg->big_reward_item.num;
		contentparam.item_list[0].is_bind = round_cfg->big_reward_item.is_bind;
		contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

		int length = 0;

		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_lucky_cloud_buy_reward_mail,
			item_base->GetItemName());

		if (length > 0)
		{
			MailRoute::MailToUser(IntToUid(role_id), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}

	// 记录中奖幸运儿
	GameName lucky_name = {};
	{
		if (Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id)))
		{
			memcpy(lucky_name, role->GetName(), sizeof(GameName));
			LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY, role_id, role->GetName(), round_cfg->big_reward_item.item_id, 1);
		}
		else if (UserCacheNode *role_node = UserCacheManager::Instance().GetUserNode(IntToUid(role_id)))
		{
			memcpy(lucky_name, role_node->user_name, sizeof(GameName));
			LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY, role_id, role_node->user_name, round_cfg->big_reward_item.item_id, 1);
		}
	}

	// 传闻
	if (nullptr != item_base && lucky_name[0] != '\0')
	{
		int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_lucky_cloud_buy_reward,
			round_cfg->big_reward_item.item_id, role_id, lucky_name, 1);
		if (sendlen > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_NOT_CHAT);
		}
	}

	// 所有玩家
	auto itr = m_role_id_buy_map.begin();
	for (; itr != m_role_id_buy_map.end(); ++itr)
	{
		// 重置玩家活动数据
		if (Role* role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(itr->first)))
		{
			role->GetRoleActivity()->ResetLuckyCloudBuy();
		}

		// 邮件通知
		if (role_id == itr->first) continue;
		static MailContentParam contentparam; contentparam.Reset();
		{
			int length = 0;

			length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_lucky_cloud_buy_no_reward_mail);

			if (length > 0)
			{
				MailRoute::MailToUser(IntToUid(itr->first), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}
	m_role_id_buy_map.clear();
}
void RandActivityLuckyCloudBuy::UnRewardLucky()
{
	auto *round_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyCloudBuyCfg(m_round_index);
	if (round_cfg == nullptr)
	{
		return;
	}

	// 所有玩家
	auto itr = m_role_id_buy_map.begin();
	for (; itr != m_role_id_buy_map.end(); ++itr)
	{
		// 重置玩家活动数据
		if (Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(itr->first)))
		{
			role->GetRoleActivity()->ResetLuckyCloudBuy();
		}

		// 邮件通知
		static MailContentParam contentparam; contentparam.Reset();
		{
			int length = 0;
			length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_lucky_cloud_buy_return_mail);

			if (length > 0)
			{
				MailRoute::MailToUser(IntToUid(itr->first), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}

	// 传闻
	{
		int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_lucky_cloud_buy_noreward);
		if (sendlen > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_NOT_CHAT);
		}
	}
	m_role_id_buy_map.clear();
}
void RandActivityLuckyCloudBuy::StopCurrentRound()
{
	int lucky_role_id = GetLotteryDrawing();
	if (0 == lucky_role_id)
	{
		// 无人中奖
		UnRewardLucky();
	}
	else
	{
		// 有人中奖
		RewardLucky(lucky_role_id);
	}

	m_can_buy = false;
	m_this_round_buy_num = 0;

	// 通知结束购买
	NoticeAllCanBuy(0);
}
void RandActivityLuckyCloudBuy::StartNextRound(time_t now_second)
{
	// 下一轮的索引
	int next_times_index_today = m_round_index + 1;
	auto* round_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyCloudBuyCfg(next_times_index_today);
	if (nullptr == round_cfg)
		return;

	// 重置本轮数据
	m_round_index = next_times_index_today;
	m_this_round_sale_num = round_cfg->sale_count;
	m_this_round_buy_num = 0;
	m_this_round_end_tick = GetTargetTimestamp(now_second, round_cfg->end_time_hour, round_cfg->end_time_min);
	m_role_id_buy_map.clear();
	m_can_buy = true;

	// 下一轮开启时间
	auto* next_round_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyCloudBuyCfg(1 + m_round_index);
	if (nullptr == next_round_cfg)
		m_next_round_begin_tick = 0;
	else
		m_next_round_begin_tick = GetTargetTimestamp(now_second, next_round_cfg->begin_time_hour, next_round_cfg->begin_time_min);

	// 通知可以开始购买
	NoticeAllCanBuy(1);

	// 向已打开面板的玩家更新数据
	SendBuyRecordInfo(nullptr, (unsigned int)now_second);

	// 传闻
	{
		int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_lucky_cloud_buy_can_join);
		if (sendlen > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_NOT_CHAT);
		}
	}
}
