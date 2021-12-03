#include "specialactivityopengame.hpp"

#include <set>
#include "global/rank/rankmanager.hpp"
#include "item/itempool.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "world.h"
#include "obj/character/role.h"
#include "global/usercache/usercache.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"

SpecialActivityOpenGame::SpecialActivityOpenGame(SpecialActivityManager *special_activity_manager)
	: SpecialActivity(special_activity_manager, SPECIAL_ACTIVITY_TYPE_OPENGAME)
{

}

SpecialActivityOpenGame::~SpecialActivityOpenGame()
{

}

void SpecialActivityOpenGame::OnSpecialActivityStatusChange(int from_status, int to_status)
{
	if (ACTIVITY_STATUS_CLOSE == to_status)
	{
		/*
		{
			// 发送战力排行榜奖励
			const static int OGA_CAPABILITY_RANK_REWARD_MAX_NUM = 10;
			UserID user_id_list[OGA_CAPABILITY_RANK_REWARD_MAX_NUM];
			int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopUser(PERSON_RANK_TYPE_CAPABILITY_ALL, OGA_CAPABILITY_RANK_REWARD_MAX_NUM, user_id_list);

			for (int i = 0; i < rank_count && i < OGA_CAPABILITY_RANK_REWARD_MAX_NUM; ++ i)
			{
				const OGACapabilityRankRewardConfig::ConfigItem *reward_cfg = LOGIC_CONFIG->GetOpenGameActivityCfg().GetCapabilityRankReward(i + 1);
				if (NULL != reward_cfg)
				{
					if (reward_cfg->reward_gold <= 0)
					{
						continue;
					}

					// 发邮件
					static MailContentParam contentparam; contentparam.Reset();
					contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_GOLD] = reward_cfg->reward_gold;


					int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_open_game_activity_capability_rank_reward_subject);
					int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_open_game_activity_capability_rank_reward_content, i + 1);
					if (length1 > 0 && length2 > 0)
					{
						MailRoute::MailToUser(user_id_list[i], SYSTEM_MAIL_REASON_INVALID, contentparam);

						{
							// 日志
							GameName user_name; int level = 0;
							Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id_list[i]);
							if (NULL != role)
							{
								level = role->GetLevel();
								role->GetName(user_name);
							}
							else
							{
								UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(UidToInt(user_id_list[i]));
								if (NULL != user_node)
								{
									level = user_node->level;
									user_node->GetName(user_name);
								}
							}

							if (level > 0)
							{
								gamelog::g_log_roleactivity.printf(LL_INFO, "OpenGameActivity::TopCapabilityReward user[%d, %s] level[%d], rank[%d], reward_gold[%d]", 
									UidToInt(user_id_list[i]), user_name, level, i + 1, reward_cfg->reward_gold);
							}
						}
					}
				}
			}
		}
		*/

		/*
		{
			// 发送仙盟等级排行榜奖励
			GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
			if (NULL != guild_rank)
			{
				GuildID guild_id_list[OGAGuildLevelRankRewardConfig::CONFIG_ITEM_MAX_COUNT];
				int guild_id_count = guild_rank->GetRankTopGuild(GUILD_RANK_TYPE_LEVEL, OGAGuildLevelRankRewardConfig::CONFIG_ITEM_MAX_COUNT, guild_id_list);
				
				for (int i = 0; i < guild_id_count && i < OGAGuildLevelRankRewardConfig::CONFIG_ITEM_MAX_COUNT; ++ i)
				{
					Guild *guild = GuildManager::Instance().GetGuild(guild_id_list[i]);
					if (NULL == guild)
					{
						continue;
					}

					const OGAGuildLevelRankRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetOpenGameActivityCfg().GetGuildLevelRewardCfg(i);
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

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_open_game_activity_guild_level_rank_reward_content, i + 1);
					if (length > 0)
					{
						guild->GetGuildMemberManager().SystemMailToAll(contentparam);
					}
				}
			}
		}
		*/

		/*
		{
			// 发送仙盟击杀世界boss排行榜奖励
			GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
			if (NULL != guild_rank)
			{
				GuildID guild_id_list[OGAGuildWorldBossRankRewardConfig::CONFIG_ITEM_MAX_COUNT];
				int guild_id_count = guild_rank->GetRankTopGuild(GUILD_RANK_TYPE_KILL_WORLD_BOSS, OGAGuildWorldBossRankRewardConfig::CONFIG_ITEM_MAX_COUNT, guild_id_list);

				for (int i = 0; i < guild_id_count && i < OGAGuildWorldBossRankRewardConfig::CONFIG_ITEM_MAX_COUNT; ++ i)
				{
					Guild *guild = GuildManager::Instance().GetGuild(guild_id_list[i]);
					if (NULL == guild)
					{
						continue;
					}

					const OGAGuildWorldBossRankRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetOpenGameActivityCfg().GetGuildWorldBossCfg(i);
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

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_open_game_activity_guild_world_boss_rank_reward_content, i + 1);
					if (length > 0)
					{
						guild->GetGuildMemberManager().SystemMailToAll(contentparam);
					}
				}
			}
		}
		*/
	}

	SpecialActivity::OnSpecialActivityStatusChange(from_status, to_status);
}

void SpecialActivityOpenGame::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (ACTIVITY_STATUS_OPEN == m_special_activity_status)
	{

	}
}

void SpecialActivityOpenGame::Update(unsigned long interval, time_t now_second)
{
	if (ACTIVITY_STATUS_OPEN == m_special_activity_status)
	{

	}

	SpecialActivity::Update(interval, now_second);
}

