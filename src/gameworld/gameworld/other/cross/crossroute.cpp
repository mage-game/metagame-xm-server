#include "crossroute.hpp"

#include "internalcomm.h"

#include "gamecommon.h"
#include "gstr.h"

#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/serverconfig/cross3v3config.hpp"

#include "servercommon/internalprotocal/crossglobalprotocal.h"

#include "servercommon/string/globalstr.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "rmiclient/rmigroleclient.h"
#include "servercommon/maildef.hpp"
#include "servercommon/string/crossstr.h"
#include "other/route/mailroute.hpp"
#include "gamelog.h"
#include "global/usercache/usercache.hpp"
#include "servercommon/serverconfig/cross1v1commonconfig.hpp"
#include "servercommon/struct/global/worldstatus2param.hpp"
#include "gameworld/gameworld/global/worldstatus2/worldstatus2.hpp"

#include "gameworld/gameworld/item/itempool.h"

CrossRoute::CrossRoute()
{

}

CrossRoute::~CrossRoute()
{

}

CrossRoute & CrossRoute::Instance()
{
	static CrossRoute cr;
	return cr;
}

void CrossRoute::OnCrossMultiuserChallengeScoreRankReward(crossgameprotocal::CrossGameMultiuserChallengeScoreRankReward *cgmcsrr)
{
	if (cgmcsrr->uid <= 0) return;

	UserID reward_user_id = IntToUid(cgmcsrr->uid);

	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(cgmcsrr->uid);
	if (NULL == user_node)
	{
		gamelog::g_log_cross.printf(LL_INFO, "OnCrossMultiuserChallengeScoreRankReward the user[%d] does not exist", cgmcsrr->uid);
	}
	else
	{
		// 注意：3V3跨服回传的排行名次rank_pos，第一名为1

		///////////////////////// 发放赛季排行奖励  //////////////////////////////////

		const Cross3v3ScoreRankReward *reward_cfg = Cross3v3Config::Instance().GetChallengeScoreRankReward(cgmcsrr->rank_pos, cgmcsrr->socre);
		if (NULL != reward_cfg)
		{
			int v_item_list[MAIL_VIRTUAL_ITEM_MAX]; memset(v_item_list, 0, sizeof(v_item_list));
			v_item_list[MAIL_VIRTUAL_ITEM_BIND_GOLD] = reward_cfg->bind_gold;
			v_item_list[MAIL_VIRTUAL_ITEM_CONVERTSHOP_MOJING] = reward_cfg->mojing; // 3v3排行奖励-魔晶

			gamecommon::SendMultiMailToRole(reward_user_id, reward_cfg->reward_item_list, reward_cfg->reward_count, v_item_list, 
				true, gamestring::g_cross_multiuser_challenge_score_rank_reward_content, cgmcsrr->rank_pos);

			gamelog::g_log_cross.printf(LL_INFO, "OnCrossMultiuserChallengeScoreRankReward : user(%d, %s), rank[%d], score[%d], bind_gold:%d, mojing:%d", 
				user_node->uid, user_node->user_name, cgmcsrr->rank_pos, cgmcsrr->socre, reward_cfg->bind_gold, reward_cfg->mojing);
		}
		else
		{
			gamelog::g_log_cross.printf(LL_INFO, "OnCrossMultiuserChallengeScoreRankReward Fail : user(%d, %s), rank_pos[%d], score[%d]",
				user_node->uid, user_node->user_name, cgmcsrr->rank_pos, cgmcsrr->socre);
		}

		///////////////////////// 发放赛季排行令牌  //////////////////////////////////

		WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();
		int grade = CROSS_ACTIVITY_3V3_GRADE_TYPE_MAX;
		const int MAX_OPEN_SEASON = Cross3v3Config::Instance().GetMaxSeason();
		const Cross3v3SeasonRewardConfig *season_cfg = NULL;

		if (common_data_2.cross3v3_season_count <= MAX_OPEN_SEASON)
		{
			season_cfg = Cross3v3Config::Instance().GetSeasonCfgBySeasonGrade(common_data_2.cross3v3_season_count, grade, cgmcsrr->rank_pos);
			if ((NULL == reward_cfg || NULL == season_cfg) && cgmcsrr->rank_pos > 0)
			{
				// 如果没有获取排行令牌，那么要给一份对应的段位令牌和段位积分奖励. 因为上了榜的玩家在RoleCross::Cross3v3GiveDivisionReward里面不会给令牌
				const Cross3V3GradeScoreConfig* grade_cfg = Cross3v3Config::Instance().GetDivisionConfigWithScore(cgmcsrr->socre);
				if (NULL != grade_cfg)
				{
					grade = grade_cfg->grade;
					season_cfg = Cross3v3Config::Instance().GetSeasonCfgBySeasonGrade(common_data_2.cross3v3_season_count, grade);

					// 补发段位积分奖励
					int v_item_list[MAIL_VIRTUAL_ITEM_MAX]; memset(v_item_list, 0, sizeof(v_item_list));
					v_item_list[MAIL_VIRTUAL_ITEM_BIND_GOLD] = grade_cfg->bind_gold;
					v_item_list[MAIL_VIRTUAL_ITEM_CONVERTSHOP_MOJING] = grade_cfg->mojing;

					gamecommon::SendMultiMailToRole(reward_user_id, grade_cfg->reward_item_list, grade_cfg->reward_count, v_item_list,
						true, gamestring::g_cross_multiuser_challenge_season_score_reward_content);

					gamelog::g_log_cross.printf(LL_INFO, "OnCrossMultiuserChallengeScoreRankReward[Grade] 1: user(%d, %s), division_grade:%d, bind_gold:%d, mojing:%d",
						user_node->uid, user_node->user_name, grade_cfg->grade, grade_cfg->bind_gold, grade_cfg->mojing);
				}
			}
		}
		else
		{
			// N赛季后，不再给令牌，改为另一份奖励配置
			season_cfg = Cross3v3Config::Instance().GetAfterSeasonCfgBySeasonGrade(common_data_2.cross3v3_season_count, grade, cgmcsrr->rank_pos);
			if ((NULL == reward_cfg || NULL == season_cfg) && cgmcsrr->rank_pos > 0)
			{
				// 如果没有获取排行令牌，那么要给一份对应的段位令牌和段位积分奖励. 因为上了榜的玩家在RoleCross::Cross3v3GiveDivisionReward里面不会给令牌
				const Cross3V3GradeScoreConfig* grade_cfg = Cross3v3Config::Instance().GetDivisionConfigWithScore(cgmcsrr->socre);
				if (NULL != grade_cfg)
				{
					grade = grade_cfg->grade;
					season_cfg = Cross3v3Config::Instance().GetAfterSeasonCfgBySeasonGrade(common_data_2.cross3v3_season_count, grade);

					// 补发段位积分奖励
					int v_item_list[MAIL_VIRTUAL_ITEM_MAX]; memset(v_item_list, 0, sizeof(v_item_list));
					v_item_list[MAIL_VIRTUAL_ITEM_BIND_GOLD] = grade_cfg->bind_gold;
					v_item_list[MAIL_VIRTUAL_ITEM_CONVERTSHOP_MOJING] = grade_cfg->mojing;

					gamecommon::SendMultiMailToRole(reward_user_id, grade_cfg->reward_item_list, grade_cfg->reward_count, v_item_list,
						true, gamestring::g_cross_multiuser_challenge_season_score_reward_content);

					gamelog::g_log_cross.printf(LL_INFO, "OnCrossMultiuserChallengeScoreRankReward[Grade] 2: user(%d, %s), division_grade:%d, bind_gold:%d, mojing:%d",
						user_node->uid, user_node->user_name, grade_cfg->grade, grade_cfg->bind_gold, grade_cfg->mojing);
				}
			}
		}

		if (NULL != season_cfg)
		{
			grade = season_cfg->grade;

			static MailContentParam contentparam; contentparam.Reset();
			const ItemConfigData &item_cfg = season_cfg->reward_item;
			const ItemBase *item_base = ITEMPOOL->GetItem(item_cfg.item_id);
			if (NULL == item_base) return;

			contentparam.item_list[0].item_id = item_cfg.item_id;
			contentparam.item_list[0].num = item_cfg.num;
			contentparam.item_list[0].is_bind = item_cfg.is_bind;
			contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_multiuser_challenge_season_card_reward_content);
			if (length > 0)
			{
				MailRoute::MailToUser(reward_user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_cross.printf(LL_INFO, "OnCrossMultiuserChallengeScoreRankReward[Card] : user(%d, %s), season:%d, rank_pos:%d, score[%d], grade[%d], card_item_id:%d, card_item_num:%d",
					user_node->uid, user_node->user_name, common_data_2.cross3v3_season_count, cgmcsrr->rank_pos, cgmcsrr->socre, grade, item_cfg.item_id, item_cfg.num);
			}
		}
		else
		{
			gamelog::g_log_cross.printf(LL_INFO, "OnCrossMultiuserChallengeScoreRankReward[Card] Fail: user(%d, %s), season:%d, rank_pos:%d, score[%d], grade[%d]",
				user_node->uid, user_node->user_name, common_data_2.cross3v3_season_count, cgmcsrr->rank_pos, cgmcsrr->socre, grade);
		}
	}
	
}

void CrossRoute::OnCross1V1ScoreRankReward(crossgameprotocal::CrossGameCross1V1ScoreRankReward * cgcsrr)
{
	//if (cgcsrr->uid <= 0) return;

	UserID reward_user_id = IntToUid(cgcsrr->uid);
	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(cgcsrr->uid);
	if (NULL == user_node)
	{
		gamelog::g_log_cross.printf(LL_INFO, "OnCross1V1ScoreRankReward not exist this user: %d", cgcsrr->uid);
	}
	else
	{
		// 注意：1V1跨服回传的排行名次rank_pos，第一名为0

		///////////////////////// 发放赛季排行奖励  //////////////////////////////////

		const Cross1V1RankRewardConfig *reward_cfg = Cross1V1CommonConfig::Instance().GetScoreRankRewardByRankPos(cgcsrr->rank_pos, cgcsrr->score);
		if (NULL != reward_cfg)
		{
			int v_item_list[MAIL_VIRTUAL_ITEM_MAX]; memset(v_item_list, 0, sizeof(v_item_list));
			v_item_list[MAIL_VIRTUAL_ITEM_BIND_GOLD] = reward_cfg->bind_gold;
			v_item_list[MAIL_VIRTUAL_ITEM_BIND_COIN] = reward_cfg->coin;

			gamecommon::SendMultiMailToRole(reward_user_id, reward_cfg->reward_item_list, reward_cfg->reward_count, v_item_list, false, "cross_1v1_score_rank_reward_content", cgcsrr->score, cgcsrr->rank_pos + 1);
			
			gamelog::g_log_cross.printf(LL_INFO, "OnCross1V1ScoreRankReward : user(%d, %s), rank[%d], score[%d], reward_item_count[%d]",
				user_node->uid, user_node->user_name, cgcsrr->rank_pos, cgcsrr->score, reward_cfg->reward_count);
		}
		else
		{
			gamelog::g_log_cross.printf(LL_INFO, "OnCross1V1ScoreRankReward Fail: user(%d, %s), rank[%d], score[%d]",
				user_node->uid, user_node->user_name, cgcsrr->rank_pos, cgcsrr->score);
		}

		///////////////////////// 发放赛季排行戒指  //////////////////////////////////
		
		WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();

		int grade = 0; // 排行戒指的段位(一定是大于0的)
		if (NULL != reward_cfg)
		{
			grade = reward_cfg->rank_ring_grade;
		}
		
		// 没有拿到排行奖励，但是又上了榜，那么要给发段位奖励和段位戒指.因为上了榜的玩家在RoleCross::Cross1v1GiveScoreReward里面不会给戒指
		if (0 == grade && cgcsrr->rank_pos >= 0)
		{
			grade = Cross1V1CommonConfig::Instance().GetGradeByScore(cgcsrr->score);
		}

		if (0 == grade)
		{
			gamelog::g_log_cross.printf(LL_INFO, "OnCross1V1ScoreRankReward[Ring] Fail 1: user(%d, %s), season:%d, score[%d], grade[%d]",
				user_node->uid, user_node->user_name, common_data_2.cross1v1_season_count, cgcsrr->score, grade);

			return;
		}

		if (NULL == reward_cfg && cgcsrr->rank_pos >= 0)
		{
			// 没有拿到排行奖励的，但是又上了榜，补发段位积分奖励
			const Cross1V1GradeScoreConfig *grade_score_cfg = Cross1V1CommonConfig::Instance().GetGradeScoreCfgByScore(cgcsrr->score);
			if (NULL != grade_score_cfg)
			{
				static MailContentParam contentparam; contentparam.Reset();
				int reward_count = 0;
				for (int i = 0; i < grade_score_cfg->reward_count && reward_count < MAX_ATTACHMENT_ITEM_NUM; i++)
				{
					const ItemConfigData &item_cfg = grade_score_cfg->reward_item_list[i];
					const ItemBase *item_base = ITEMPOOL->GetItem(item_cfg.item_id);
					if (NULL == item_base) return;

					contentparam.item_list[reward_count].item_id = item_cfg.item_id;
					contentparam.item_list[reward_count].num = item_cfg.num;
					contentparam.item_list[reward_count].is_bind = item_cfg.is_bind;
					contentparam.item_list[reward_count].invalid_time = item_base->CalInvalidTime();
					++reward_count;
				}
				contentparam.gold_bind = grade_score_cfg->bind_gold;
				contentparam.coin = grade_score_cfg->coin;

				int length = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "cross_1v1_season_score_reward_content");
				if (length > 0)
				{
					MailRoute::MailToUser(reward_user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);

					gamelog::g_log_cross.printf(LL_INFO, "OnCross1V1ScoreRankReward[Grade] : user(%d, %s), score:%d, reward_item_num:%d, coin:%d, gold_bind:%d",
						user_node->uid, user_node->user_name, cgcsrr->score, reward_count, grade_score_cfg->coin, grade_score_cfg->bind_gold);
				}
			}
			else
			{
				gamelog::g_log_cross.printf(LL_INFO, "OnCross1V1ScoreRankReward[Grade] Fail: user(%d, %s), score:%d",
					user_node->uid, user_node->user_name, cgcsrr->score);
			}
		}

		const int MAX_OPEN_SEASON = Cross1V1CommonConfig::Instance().GetMaxSeason();
		const Cross1V1SeasonRewardConfig *season_cfg = NULL;

		if (common_data_2.cross1v1_season_count <= MAX_OPEN_SEASON)
		{
			season_cfg = Cross1V1CommonConfig::Instance().GetSeasonCfgBySeasonGrade(common_data_2.cross1v1_season_count, grade);
		}
		else
		{
			season_cfg = Cross1V1CommonConfig::Instance().GetAfterSeasonCfgBySeasonGrade(common_data_2.cross1v1_season_count, grade);
		}

		if (NULL != season_cfg)
		{
			static MailContentParam contentparam; contentparam.Reset();
			const ItemConfigData &item_cfg = season_cfg->reward_item;
			const ItemBase *item_base = ITEMPOOL->GetItem(item_cfg.item_id);
			if (NULL == item_base) return;

			contentparam.item_list[0].item_id = item_cfg.item_id;
			contentparam.item_list[0].num = item_cfg.num;
			contentparam.item_list[0].is_bind = item_cfg.is_bind;
			contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

			int length = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "cross_1v1_season_ring_reward_content");
			if (length > 0)
			{
				MailRoute::MailToUser(reward_user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_cross.printf(LL_INFO, "OnCross1V1ScoreRankReward[Ring] : user %d %s, season:%d, score %d, reward_item_id %d, reward_item_num %d",
					user_node->uid, user_node->user_name, common_data_2.cross1v1_season_count, cgcsrr->score, item_cfg.item_id, item_cfg.num);
			}
		}
		else
		{
			gamelog::g_log_cross.printf(LL_INFO, "OnCross1V1ScoreRankReward[Ring] Fail 2: user(%d, %s), season:%d, score[%d], grade[%d]",
				user_node->uid, user_node->user_name, common_data_2.cross1v1_season_count, cgcsrr->score, grade);
		}

	}
}

