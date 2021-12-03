#include "randactivityxianmengbipin.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "global/rank/rankmanager.hpp"
#include "item/itempool.h"
#include "global/guild/guildmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "global/guild/guild.hpp"

RandActivityXianmengBipin::RandActivityXianmengBipin(RandActivityManager *rand_activity_manager) : RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_XIANMENG_BIPIN)
{

}

RandActivityXianmengBipin::~RandActivityXianmengBipin()
{

}


void RandActivityXianmengBipin::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	RandActivity::OnSpecialActivityStatusChange(from_status, to_status);

	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		// ·¢ËÍÏÉÃË±ÈÆ´»÷É±ÊÀ½çbossÅÅÐÐ°ñ½±Àø
		GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
		if (NULL != guild_rank)
		{
			GuildID guild_id_list[GUILD_RANK_MAX_SIZE];
			int guild_id_count = guild_rank->GetRankTopGuild(GUILD_RANK_TYPE_XIANMENG_BIPIN_KILL_WORLD_BOSS, GUILD_RANK_MAX_SIZE, guild_id_list);

			for (int i = 0; i < guild_id_count && i < GUILD_RANK_MAX_SIZE; ++ i)
			{
				Guild *guild = GuildManager::Instance().GetGuild(guild_id_list[i]);
				if (NULL == guild)
				{
					continue;
				}

				const RandActivityXianmengBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetXianmengBipinCfg(i);
				if (NULL == cfg_item)
				{
					continue;
				}

				const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
				if (NULL == item_base)
				{
					continue;
				}

				// ·¢ÓÊ¼þ
				static MailContentParam contentparam; contentparam.Reset();
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = cfg_item->reward_item.is_bind;
				contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_xianmeng_bipin_reward_content, i + 1);
				if (length > 0)
				{
					guild->GetGuildMemberManager().SystemMailToAll(contentparam);
				}
			}

			GuildManager::Instance().ClearXianmengBipinKillBossCount();
			guild_rank->ClearRank(GUILD_RANK_TYPE_XIANMENG_BIPIN_KILL_WORLD_BOSS);
		}
	}
}


