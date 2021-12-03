#include "randactivityxianmengjueqi.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "global/rank/rankmanager.hpp"
#include "item/itempool.h"
#include "global/guild/guildmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "global/guild/guild.hpp"
#include "scene/activityshadow/activityshadow.hpp"

RandActivityXianmengJueQi::RandActivityXianmengJueQi(RandActivityManager *rand_activity_manager) 
	: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_XIANMENG_JUEQI)
{

}

RandActivityXianmengJueQi::~RandActivityXianmengJueQi()
{

}

void RandActivityXianmengJueQi::Init(const RandActivityData &data)
{
	RandActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data;
	m_data_param = *pTemp;
}

void RandActivityXianmengJueQi::GetInitParam(RandActivityData *data)
{
	RandActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data;
	*pTemp = m_data_param;
}

void RandActivityXianmengJueQi::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_OPEN == to_status)
	{
		m_data_param.need_send_reward_flag = 1;
	}
}

void RandActivityXianmengJueQi::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (ACTIVITY_STATUS_OPEN == m_rand_activity_status || 0 != m_data_param.need_send_reward_flag)
	{
		// 发送仙盟战力增加排行榜奖励
		GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
		if (NULL != guild_rank)
		{
			GuildID guild_id_list[GUILD_RANK_MAX_SIZE];
			int guild_id_count = guild_rank->GetRankTopGuild(GUILD_RANK_TYPE_DAY_INCREASE_CAPABILITY, GUILD_RANK_MAX_SIZE, guild_id_list);

			for (int i = 0; i < guild_id_count && i < GUILD_RANK_MAX_SIZE; ++ i)
			{
				Guild *guild = GuildManager::Instance().GetGuild(guild_id_list[i]);
				if (NULL == guild)
				{
					continue;
				}

				const RandActivityXianmengJueQiCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetXianmengJueQiCfg(i);
				if (NULL == cfg_item)
				{
					continue;
				}

				const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
				if (NULL == item_base)
				{
					continue;
				}

				// 发邮件
				static MailContentParam contentparam; contentparam.Reset();
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = cfg_item->reward_item.is_bind;
				contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_xianmeng_jueqi_reward_content, i + 1);
				if (length > 0)
				{
					guild->GetGuildMemberManager().SystemMailToAll(contentparam);
				}
			}
		}

		guild_rank->ClearRank(GUILD_RANK_TYPE_DAY_INCREASE_CAPABILITY);
	}

	m_data_param.need_send_reward_flag = (ACTIVITY_STATUS_OPEN == m_rand_activity_status ? 1 : 0);
}

