#include "randactivitydayonlinegift.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "gameworld/gameworld/config/activityconfig/randactivityconfig.hpp"
#include "globalconfig/globalconfig.h"
#include "config/activityconfig/randactivityconfig.hpp"
#include "item/itempool.h"
#include "config/logicconfigmanager.hpp"
#include "other/route/mailroute.hpp"
#include "gameworld/gameworld/engineadapter.h"
#include "world.h"
#include "gameworld/gameworld/global/usercache/usercache.hpp"

RandActivityDayOnlineGift::RandActivityDayOnlineGift(RandActivityManager *rand_activity_manager)
	: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_DAY_ONLINE_GIFT), m_next_flush_gift_time(0)
{

}

RandActivityDayOnlineGift::~RandActivityDayOnlineGift()
{

}

void RandActivityDayOnlineGift::Update(unsigned long interval, time_t now_second)
{
	RandActivity::Update(interval, now_second);

	if (ACTIVITY_STATUS_OPEN == m_rand_activity_status)
	{
		if (0 != m_next_flush_gift_time && now_second >= m_next_flush_gift_time)
		{
			this->CalcNextSendRewardTime();

			int lucky_role_list[SCENE_MANAGER_RANDOM_GET_ROLE_MAX_COUNT]; memset(lucky_role_list, 0, sizeof(lucky_role_list));					
			int lucky_role_count = World::GetInstWorld()->GetSceneManager()->RandomGetRoleList(lucky_role_list);	

			static const int RAND_ACTIVITY_DAY_ONLINE_GIFT_ROLE_MAX_COUNT = 5;						// 随机奖励人物个数
			UNSTD_STATIC_CHECK(RAND_ACTIVITY_DAY_ONLINE_GIFT_ROLE_MAX_COUNT <= SCENE_MANAGER_RANDOM_GET_ROLE_MAX_COUNT);
			
			const ItemConfigData *reward_item = LOGIC_CONFIG->GetRandActivityCfg().GetDayOnlineGiftCfgRewardItem();
			if (NULL == reward_item)
			{
				return;
			}

			int all_name_len = 0;
			char all_name_str[512] = {0};
			int rank_count = 0;

			for (int i = 0; i < lucky_role_count && i < RAND_ACTIVITY_DAY_ONLINE_GIFT_ROLE_MAX_COUNT; ++ i)
			{
				static MailContentParam contentparam; contentparam.Reset();

				const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_item->item_id);
				if (NULL != reward_item_base)
				{
					contentparam.item_list[0].item_id = reward_item->item_id;
					contentparam.item_list[0].is_bind = (reward_item->is_bind) ? 1 : 0;
					contentparam.item_list[0].num = reward_item->num;
					contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_day_online_gift_reward_content);
					if (length > 0)
					{
						MailRoute::MailToUser(IntToUid(lucky_role_list[i]), SYSTEM_MAIL_REASON_INVALID, contentparam);

						{
							// 传闻名字
							UserCacheNode * user_node = UserCacheManager::Instance().GetUserNode(lucky_role_list[i]); 
							if (NULL != user_node)
							{
								char member_name_str[64] = {0};

								int namelen = SNPRINTF(member_name_str, sizeof(member_name_str), "{r;%d;%s;%d} ", user_node->uid, user_node->user_name, (int)user_node->camp);
								if (namelen > 0 && all_name_len + namelen < (int)sizeof(all_name_str))
								{
									STRNCPY(all_name_str + all_name_len, member_name_str, sizeof(all_name_str) - all_name_len);
									all_name_len += namelen;
								}

								++ rank_count;
							}
						}
					}
				}
			}
			
			// 传闻
			if(rank_count > 0)
			{
				all_name_str[sizeof(all_name_str) - 1] = '\0';

				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_day_online_gift_reward_notice, 
					all_name_str, reward_item->item_id);

				if (length > 0)
				{
					//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER);
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}
}

void RandActivityDayOnlineGift::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		this->CalcNextSendRewardTime();
	}
}

void RandActivityDayOnlineGift::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	this->CalcNextSendRewardTime();
}

void RandActivityDayOnlineGift::CalcNextSendRewardTime()
{
	m_next_flush_gift_time = 0;

	const tm *now_tm = EngineAdapter::Instance().LocalTime();
	if (NULL == now_tm)
	{
		return;
	}

	const RandActivityDayOnlineGiftCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetNextDayOnlineGiftCfg(now_tm->tm_hour);
	if (NULL == cfg_item)
	{
		return;
	}
	
	int next_give_reward_interval = EngineAdapter::Instance().NextDayInterval(cfg_item->send_reward_hour, 0, 0);
	if (next_give_reward_interval <= 0)
	{
		return;
	}

	m_next_flush_gift_time = EngineAdapter::Instance().Time() + next_give_reward_interval;
}

