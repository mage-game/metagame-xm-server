#include "roleactivity.hpp"

#include "world.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "obj/character/role.h"
#include "obj/otherobj/gatherobj.h"
#include "obj/otherobj/fallingitem.h"
#include "obj/character/monster.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "scene/marryshadow/marryshadow.h"
#include "monster/monsterpool.h"

#include "scene/scene.h"
#include "scene/speciallogic/worldspecial/specialzhuxie.hpp"
#include "scene/speciallogic/worldspecial/specialqunxianluandou.hpp"
#include "scene/speciallogic/worldspecial/specialxianmengzhan.hpp"
#include "scene/speciallogic/worldspecial/specialnationalboss.hpp"
#include "scene/speciallogic/worldspecial/specialyizhandaodi.hpp"
#include "scene/speciallogic/worldspecial/specialwuxingguaji.hpp"
#include "scene/speciallogic/worldspecial/specialshuijing.hpp"
#include "scene/speciallogic/worldspecial/specialgongchengzhan.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "task/taskpool.h"

#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"

#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"

#include "globalconfig/globalconfig.h"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "config/activityconfig/closedbetaconfig.hpp"
#include "global/combineserveractivity/combineserveractivityconfig.hpp"

#include "config/sharedconfig/sharedconfig.h"

#include "global/team/team.hpp"

#include "protocal/msgscene.h"
#include "protocal/msgactivity.hpp"
#include "protocal/msgserver.h"
#include "gameworld/gamelog.h"
#include "item/itempool.h"
#include "internalcomm.h"
#include "item/other/otheritem.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/roleguild.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "gameworld/protocal/msgfb.h"
#include "gameworld/gameworld/global/guild/guild.hpp"
#include "gameworld/gameworld/global/usercache/usercache.hpp"
#include "config/activityconfig/chongzhiconfig.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "global/rank/topranker.hpp"
#include "global/randactivity/impl/randactivityluckroll.hpp"
#include "global/randactivity/impl/randactivityserverpanicbuy.hpp"
#include "global/combineserveractivity/combineserveractivitymanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "global/randactivity/impl/randactivityshouyouyuxiang.hpp"
#include "gameworld/gameworld/global/guild/guildmanager.hpp"
#include "global/rank/rankmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "global/randactivity/impl/randactivitycornucopia.hpp"
#include "config/activityconfig/opengameactivityconfig.hpp"
#include "global/randactivity/impl/randactivitymine.hpp"
#include "config/updatenoticeconfig.hpp"
#include "item/knapsack.h"
#include "other/vip/vip.hpp"
#include "other/capability/capability.hpp"
#include "other/cross/rolecross.h"
#include "other/fb/nightfight/nightfightfbconfig.hpp"
#include "other/fb/messbattle/messbattleconfig.hpp"
#include "other/chestshop/rolechestshop.hpp"
#include "doublesidefb/doublesidefbmanager.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"
#include "global/randactivity/impl/randactivityperfectlover.hpp"
#include "gameworld/other/mount/mountmanager.hpp"
#include "gameworld/other/wing/wingmanager.hpp"
#include "gameworld/other/shizhuang/shizhuang.h"
#include "gameworld/other/fabao/fabaomanager.hpp"
#include "gameworld/other/footprint/footprint.hpp"
#include "gameworld/other/halo/halomanager.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "config/activityconfig/randactivityopencfg.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/upgrade/upgrademanager.hpp"
#include "equipment/equipmentmanager.h"
#include "other/todaythemereward/todaythemerewardconfig.hpp"
#include "global/cross/crossmanager.h"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"
#include "other/tianshuxunzhu/tianshuxunzhuconfig.hpp"

RoleActivity::RoleActivity()
	: m_role(NULL), m_temp_zhanzhang_side(-1), m_xianmengzhan_defend_area(-1), m_is_first_update(true),
	m_yizhandaodi_jisha_count(0), m_is_auto_fishing(false), m_is_fishing(false), m_csa_roll_item_id(0)
{

}

RoleActivity::~RoleActivity()
{

}

void RoleActivity::Init(Role *role, const RoleActivityParam &p)
{
	m_role = role;

	memcpy(&m_activity_info, &p, sizeof(RoleActivityParam));

	if (m_activity_info.has_clear_chongzhi_reward_fetch_flag <= 0)
	{
		m_activity_info.has_clear_chongzhi_reward_fetch_flag = 1;
		m_activity_info.chongzhi_reward_fetch_flag = 0;
	}

	// 判断合服活动攻城战胜利次数
	if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER) && ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_GONGCHENGZHAN))
	{
		int winner_id = CombineServerActivityManager::Instance().GetCSAGongchengzhanChengzhuUID();
		int winner_lover_id = CombineServerActivityManager::Instance().GetCSAGongchengzhanChengzhuLoverUID();

		if (winner_id == m_role->GetUID() || winner_lover_id == m_role->GetUID())
		{
			if (0 == m_activity_info.csa_had_add_gongchengzhan_win_times)
			{
				++m_activity_info.csa_gongchengzhan_win_times;
				m_activity_info.csa_had_add_gongchengzhan_win_times = 1;
			}
		}
	}
}

void RoleActivity::GetInitParam(RoleActivityParam *p)
{
	memcpy(p, &m_activity_info, sizeof(RoleActivityParam));
}

/*
 * 写此函数时要考虑跨服里面执行这个函数时的情况！回到原服就不会执行这个函数，而且在跨服里，随机活动是不开启的
 * 所以要求：新加的活动，除非策划要求，否则默认不在清理数据之前判断活动是否开启
 */
void RoleActivity::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

	{
		// 普通数据
		m_activity_info.guild_monster_finish_seq = 0;

		m_activity_info.day_consume_coin = 0;
		m_activity_info.day_consume_gold = 0;

		this->SendGuildMonsterInfo();
		this->SendServerTime();
	}

	{
		// 仙盟副本 重置后可进其它仙盟打仙盟副本
		m_activity_info.guildfb_first_enter_guild_id = 0;
	}

	{
		//活跃奖励活动活跃度度
		m_activity_info.ra_day_active_degree_active_count = 0;
		m_activity_info.ra_day_active_degree_fetch_reward_flag = 0;
	}

	{
		//随机活动
		this->RACheckDayActiveDegreeGiveReward();
	}

	ClockReturnDayItem *old_day = (ClockReturnDayItem*)&old_dayid;
	ClockReturnDayItem *now_day = (ClockReturnDayItem*)&now_dayid;
	if (NULL == old_day || NULL == now_day)
	{
		return;
	}

	int day_change_count = CalcDayIndexWithClockDayItem(old_day, now_day); // 切天数

	{
		// 充值相关

// 		// 检查日常累计充值阶段
// 		{
// 			this->CheckDailyTotalChongzhiStage();
// 		}
// 
// 		// 每日充值奖励相关的逻辑
// 		if (m_activity_info.day_chongzhi > 0)
// 		{
// 			if (0 != m_activity_info.is_daily_first_chongzhi_open && 0 == m_activity_info.is_daily_first_chongzhi_fetch_reward)
// 			{
// 				int week_dayidx = m_activity_info.daily_chongzhi_week_day_idx;
// 				const ChongzhiDailyFirstRewardConfig::ConfigItem *daily_cfg_item = LOGIC_CONFIG->GetChongzhiCfg().GetDailyFirstReward(week_dayidx);
// 				if (NULL != daily_cfg_item)
// 				{
// 					const ItemBase *item_base = ITEMPOOL->GetItem(daily_cfg_item->first_recharge_reward_item.item_id);
// 					if (NULL != item_base)
// 					{
// 						// 发送每日首冲奖励
// 						static MailContentParam contentparam; contentparam.Reset();
// 						contentparam.item_list[0].item_id = daily_cfg_item->first_recharge_reward_item.item_id;
// 						contentparam.item_list[0].num = daily_cfg_item->first_recharge_reward_item.num;
// 						contentparam.item_list[0].is_bind = (daily_cfg_item->first_recharge_reward_item.is_bind) ? 1 : 0;
// 						contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();	
// 
// 						// 额外奖励
// 						const ChongzhiOtherConfig &other_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetOtherCfg();
// 						if (m_activity_info.daily_first_chongzhi_times >= other_cfg.daily_first_recharge_extra_reward_need_times)
// 						{
// 							const ItemBase *extra_item_base = ITEMPOOL->GetItem(other_cfg.daily_first_recharge_extra_reward_item.item_id);
// 							if (NULL != extra_item_base)
// 							{
// 								contentparam.item_list[1].item_id = other_cfg.daily_first_recharge_extra_reward_item.item_id;
// 								contentparam.item_list[1].num = other_cfg.daily_first_recharge_extra_reward_item.num;
// 								contentparam.item_list[1].is_bind = (other_cfg.daily_first_recharge_extra_reward_item.is_bind) ? 1 : 0;
// 								contentparam.item_list[1].invalid_time = extra_item_base->CalInvalidTime();
// 							}
// 						}
// 
// 						int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_chongzhi_daily_first_recharge_reward_content);
// 						if (length > 0)
// 						{
// 							MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 							m_activity_info.is_daily_first_chongzhi_fetch_reward = 1;
// 
// 							if (m_activity_info.daily_first_chongzhi_times >= other_cfg.daily_first_recharge_extra_reward_need_times)
// 							{
// 								m_activity_info.daily_first_chongzhi_times -= other_cfg.daily_first_recharge_extra_reward_need_times;
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 
// 		// 特殊首冲相关逻辑
// 		{
// 			bool all_reward_fetch = true; 
// 			{
// 				int total_reward_count = LOGIC_CONFIG->GetChongzhiCfg().GetSpecialFirstRechargeRewardCount();
// 				for (int i = 0; i < total_reward_count; ++ i)
// 				{
// 					if (0 == (m_activity_info.special_first_chongzhi_fetch_reward_flag & (1 << i)))
// 					{
// 						all_reward_fetch = false;
// 					}
// 				}
// 			}
// 		}
// 
// 		if (0 != m_activity_info.special_first_chongzhi_timestamp)
// 		{
// 			m_activity_info.is_daily_first_chongzhi_open  = 1;
// 			m_activity_info.diff_wd_chongzhi_is_open = 1;
// 		}
// 
// 		{
// 			//每日累充检查邮件发放奖励并重置数据
// 			this->CheckDiffWeekdayTotalChongzhiRewrad();
// 
// 			m_activity_info.diff_wd_chongzhi_stage_fetch_flag = 0;
// 			m_activity_info.diff_wd_chongzhi_value = 0;
// 		}

		// 完成了首充才清理每日充值数据  // 2019-04-03 去除首充限制每日首充
		//if (m_activity_info.first_chongzhi_fetch_reward_flag > 0)
		{
			if (m_activity_info.daily_activity_chongzhi > 0)
			{
				static MailContentParam contentparam; contentparam.Reset();
				for(int i = 0; i < MAX_DAILY_CHONGZHI_REWARD_COUNT; i++)
				{
					const DailyChongzhiRewradConfig *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiRewradConfig(m_role->GetLevel(), i);
					if (NULL == cfg)
					{
						break;;
					}

					if (m_activity_info.daily_activity_chongzhi >= cfg->need_total_chongzhi && (m_activity_info.daily_chongzhi_fetch_reward_flag & (1 << i)) == 0)
					{
						contentparam.AddItem(cfg->reward_item.item_id, cfg->reward_item.num, cfg->reward_item.is_bind);
					}
				}

				if (contentparam.item_count > 0)
				{
					SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_chongzhi_daily_first_recharge_reward_content);
					MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
			m_activity_info.daily_chongzhi_fetch_reward_flag = 0;
			m_activity_info.daily_activity_chongzhi = 0;
		}

		{
			// 每日充值2
			if (m_activity_info.day_chongzhi > 0)
			{
				static MailContentParam contentparam; contentparam.Reset();
				const DailyReward2ConfigMapInMap &cfg_map = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiRewrad2ConfigMap();
				//for(int i = 0; i < DAILY_CHONGZHI_REWRAD2_REQ_MAX; i++)
				for (auto cfg_map_it : cfg_map)
				{
					//const DailyChongzhiRewrad2Config *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiRewrad2Config(m_role->GetLevel(), cfg_map_it.first);
					const DailyChongzhiRewrad2Config *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiRewrad2ConfigByLastChongzhiTime(m_role->GetLevel(), cfg_map_it.first, m_activity_info.last_chongzhi_day);
					if (NULL == cfg)
					{
						break;;
					}

					if (m_activity_info.day_chongzhi >= cfg->need_chongzhi && (m_activity_info.daily_chongzhi_fetch_reward2_flag & (1 << cfg_map_it.first)) == 0)
					{
						for (int k = 0; k < cfg->reward_item_count && k < DAILY_CHONGZHI_REWRAD2_ITEM_COUNT_MAX; k++)
						{
							int invalid_time = 0;
							const ItemBase * item_base = ITEMPOOL->GetItem(cfg->reward_item_list[k].item_id);
							if (NULL != item_base)
							{
								invalid_time = item_base->CalInvalidTime();
							}
							contentparam.AddItem(cfg->reward_item_list[k].item_id, cfg->reward_item_list[k].num, cfg->reward_item_list[k].is_bind,invalid_time);
							if (contentparam.item_count >= MAX_ATTACHMENT_ITEM_NUM)
							{
								SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_chongzhi_daily_recharge_reward2_content);
								MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
								contentparam.Reset();
							}
						}
					}
				}

				if (contentparam.item_count > 0)
				{
					SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_chongzhi_daily_recharge_reward2_content);
					MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
			m_activity_info.daily_chongzhi_fetch_reward2_flag = 0;
		}

		m_activity_info.daily_chongzhi_week_day_idx = -1;
		m_activity_info.is_daily_first_chongzhi_fetch_reward = 0;
		m_activity_info.ra_charge_reward_is_first = 1;

		// 七日返利领取标记
		m_activity_info.chongzhi_7day_reward_is_fetch = 0;
	}

	{
		memset(m_activity_info.collection_exchange_times, 0, sizeof(m_activity_info.collection_exchange_times));
		this->SendCollectExchangeInfo();
	}

	{
		// 水晶幻境
		this->ResetShuijingData();
	}

	{
		// 封测活动相关
		m_activity_info.cba_fetch_online_reward_flag = 0;
		m_activity_info.cba_has_fetch_login_reward = 0;
		m_activity_info.cba_join_activity_flag = 0;
		m_activity_info.cba_fetch_activity_reward_flag = 0;
		this->SendCloseBetaActivityInfo();
	}

	{
		// 随机活动
		this->RACheckDayChongZhiFanLiReward();
		this->RACheckDayConsumeGoldReward();
		this->OnRALoginGiftLogin();
		this->OnRALoginGiftLogin_0();
		this->OnCSALoginGiftLogin();
		this->OnRAShouYouYuXiangOnDayChange();
		
		m_activity_info.ra_today_has_send_bipin_mail = 0;
		this->OnRABipinSendForenotice();

		{
			// 金银塔
			m_activity_info.ra_level_lottery_free_buy_times = 0;
		}

		{
			// 单笔充值随机活动
			m_activity_info.ra_danbi_chongzhi_reward_can_fetch_flag = 0;
			m_activity_info.ra_danbi_chongzhi_reward_has_fetch_flag = 0;
			SendRADanbiChongzhiInfo();
		}

		{
			this->RAResetDayChongzhiRankGoldNum();
			this->RAResetDayConsumeRankGoldNum();
		}

		{
			this->RACheckChangeRepayment();                                 // 充值反馈补领前一天未领取奖励
			m_activity_info.ra_charge_repayment_reward_fetch_flag = 0;      // 充值反馈清理每日领取标记
			this->SendChargeRepaymentInfo();
		}

		{
			// 累计充值（天数）活动，每天重置充值状态
			m_activity_info.ra_total_charge_day_today_has_charged = 0;
		}

		{
			//限时兑换活动,每天重置兑换次数
			this->RAResetExchangTimes();
		}

		{	
			//每日爱一次
			m_activity_info.ra_daily_love_daily_first_flag = 1;
		}

		{
			// 累计充值3活动，切换阶段，重置总充值额度
			//if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3))
			{
				//bool too_long = (day_change_count > 1); // 说明相隔上次登录差了1天以上，需要强制清理一下数据
				//int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3), EngineAdapter::Instance().Time());
				//if (too_long || LOGIC_CONFIG->GetRandActivityCfg().CheckIsResetTotalCharge3Day(day_index))
				{
					m_activity_info.ra_total_charge_3_cur_total_charge = 0;
					m_activity_info.ra_total_charge_3_cur_has_fetch_flag = 0;

					this->OnSendTotalCharge3Info();
				}
			}
		}

		{
			// 怪物掉落活动
			m_activity_info.ra_any_monster_drop_today_add_rate = 0;
		}

		{
			// 累计充值4活动，切换阶段，重置总充值额度
			//if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4))
			{
				//bool too_long = (day_change_count > 1); // 说明相隔上次登录差了1天以上，需要强制清理一下数据
				//int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4), EngineAdapter::Instance().Time());
				//if (too_long || LOGIC_CONFIG->GetRandActivityCfg().CheckIsResetTotalCharge4Day(day_index))
				{
					m_activity_info.ra_total_charge_4_cur_total_charge = 0;
					m_activity_info.ra_total_charge_4_cur_has_fetch_flag = 0;

					this->OnSendTotalCharge4Info();
				}
			}
		}

		{
			// 挖矿重置免费挖次数
			m_activity_info.ra_mine_free_gather_times = LOGIC_CONFIG->GetRandActivityCfg().GetMineFreeGatherTimes();

			// 若全服换矿次数前一天清零了，则清空玩家领取标记
			//RandActivityMine *mine_activity = (RandActivityMine*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_MINE);
			//if (NULL == mine_activity)
			//{
			//	return;
			//}

			//if (mine_activity->IsPreDayClearRefreshTimes() && m_activity_info.ra_mine_cur_reward_fetch_flag != 0)
			//{
			//	m_activity_info.ra_mine_cur_reward_fetch_flag = 0;
			//}
		}

		{	// 天命卜卦每日免费卜卦次数
			//if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TIANMING_DIVINATION))
			{
				m_activity_info.ra_tianming_free_chou_times = 0;
			}
		}

		{
			// 翻翻转重置免费翻次数
			//if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_FANFAN))
			{
				m_activity_info.ra_fanfan_cur_free_times = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanFreeTimesPerDay();
			}
		}

		// 连续充值
//		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI))
		{
			int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI), EngineAdapter::Instance().Time());
			if (m_activity_info.ra_continue_chongzhi_days != day_index)
			{
				m_activity_info.ra_continue_chongzhi_is_activity_over = 1;
			}

			m_activity_info.ra_continue_chongzhi_today_chongzhi = 0;

			this->SendRAContinueChongzhiActivityInfo();
		}

		// 连充特惠初
		//if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU))
		{
			m_activity_info.ra_is_lei_chongzhi_chu = 0;
			m_activity_info.ra_continue_chongzhi_today_chongzhi_chu = 0;

			this->SendRAContinueChongzhiActivityInfoChu();
		}

		// 连充特惠高
		//if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO))
		{
			m_activity_info.ra_is_lei_chongzhi_gao = 0;
			m_activity_info.ra_continue_chongzhi_today_chongzhi_gao = 0;

			this->SendRAContinueChongzhiActivityInfoGao();
		}

		// 限时连充
		{
// 			int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2), EngineAdapter::Instance().Time());
// 			if (m_activity_info.ra_continue_chongzhi_days_2 != day_index)
// 			{
// 				m_activity_info.ra_continue_chongzhi_2_is_lei_chongzhi = 1;
// 			}

			m_activity_info.ra_continue_chongzhi_2_is_lei_chongzhi = 0;
			m_activity_info.ra_continue_chongzhi_today_chongzhi_2 = 0;

			this->SendRAContinueChongzhiActivityInfo2();
		}

		// 连续消费
//		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME))
		{
			const RandActivityContinueConsumeRewardConfig * continue_consume_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueConsumeRewardInfoWithdayindex(m_activity_info.ra_continue_consume_the_last_time_consume_day_index);
			if (NULL != continue_consume_reward_cfg)
			{
				this->RACheckContinueConsumeGiveReward(false);

				int day_index = GetDayIndex(
					ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME),
					EngineAdapter::Instance().Time()) + 1;

				// 消费中断
				if (day_index - m_activity_info.ra_continue_consume_the_last_time_consume_day_index > 1)
				{
					m_activity_info.ra_continue_consume_days = 0;
				}
				else
				{
					if (m_activity_info.ra_continue_consume_today_consume_gold_total < continue_consume_reward_cfg->need_consume_gold)
					{
						m_activity_info.ra_continue_consume_days = 0;
					}
				}

				m_activity_info.ra_continue_consume_today_consume_gold_total = 0;
				m_activity_info.ra_continue_consume_cur_consume_gold = 0;

				this->SendRAContinueConsumeActivityInfo();
			}
		}

		// 军歌嘹亮
		{
			//if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ARMY_DAY))
			{
				m_activity_info.ra_army_day_active_degree_exchange_flag = 0;
				this->SendArmyDayInfo();
			}
		}

		// 个人抢购
		//if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PERSONAL_PANIC_BUY))
		{
			memset(m_activity_info.ra_personal_panic_buy_numlist, 0, sizeof(m_activity_info.ra_personal_panic_buy_numlist));
			this->SendRAPersonalPanicBuyInfo();
		}

		// 至尊幸运
		{
			//if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EXTREME_LUCKY))
			{
				m_activity_info.ra_extreme_lucky_day_free_draw_times = 0;
				m_activity_info.ra_extreme_lucky_flush_times = 0;
				this->SendRAExtremeLuckyAllInfo();
			}
		}

		// 开服活动
		{
			// 首充团购
//			if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN))
			{
				int index = RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN - RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN;
				if (index >= 0 && index < OPEN_SERVER_ACTIVITY_COUNT)
				{
					OpenServerSubActivityInfo &info = m_activity_info.open_server_sub_info_list[index];
					if (info.join_timestamp > 0)
					{
						const RandActivityOpenServerRewardItemConfig *cfg = NULL;

						for (int i = 1; i < RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX; i++)
						{
							if (0 == (info.reward_flag & (1 << i)))
							{
								cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN, i, true);
								if (NULL == cfg)
								{
									break;
								}

								if (m_activity_info.day_chongzhi < cfg->cond1 || WorldStatus::Instance().GetCommonData().yesterday_chongzhi_role_count < cfg->cond2)
									continue;

								info.reward_flag |= (1 << i);

								static MailContentParam contentparam; contentparam.Reset();

								for (int k = 0; k < cfg->reward_item_count && k < MAX_ATTACHMENT_ITEM_NUM; k++)
								{
									const ItemBase *item_base = ITEMPOOL->GetItem(cfg->reward_item_list[k].item_id);
									if (NULL != item_base)
									{
										contentparam.item_list[contentparam.item_count].item_id = cfg->reward_item_list[k].item_id;
										contentparam.item_list[contentparam.item_count].num = cfg->reward_item_list[k].num;
										contentparam.item_list[contentparam.item_count].is_bind = cfg->reward_item_list[k].is_bind;
										contentparam.item_list[contentparam.item_count].invalid_time = item_base->CalInvalidTime();
										contentparam.item_count++;
									}
								}

								int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_first_charge_tuan_content);

								if (length > 0)
								{
									MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
								}
							}
						}

						info.reward_flag = 0;
					}
				}
			}

			// 每日累计充值
//			if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_TOTAL_CHARGE))
			{
				int index = RAND_ACTIVITY_TYPE_DAY_TOTAL_CHARGE - RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN;
				if (index >= 0 && index < OPEN_SERVER_ACTIVITY_COUNT)
				{
					m_activity_info.open_server_sub_info_list[index].reward_flag = 0;
				}
			}

			//全民进阶领奖标志
			{
				m_activity_info.ra_upgrade_reward_flag = 0;
			}

			//全民总动员领奖标志
			{
				m_activity_info.ra_upgrade_group_reward_flag = 0;
			}
		}

		{
			// 新聚宝盆
			if (m_activity_info.new_cornucopia_value > 0)
			{
				this->NewCornucopiaUpdateTaskInfo();

				int percent = LOGIC_CONFIG->GetRandActivityCfg().GetNewCornucopiaPercent();
				const RandActivityNewCornucopiaConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityNewCornucopiaConfig(m_activity_info.new_cornucopia_day_index);
				if (NULL != cfg)
				{
					for (int i = 0; i < RA_NEW_CORNUCOPIA_TASK_TYPE_MAX; i++)
					{
						if (cfg->item_list[i].task_value > 0 && m_activity_info.new_cornucopia_task_value_list[i] >= cfg->item_list[i].task_value)
						{
							percent += cfg->item_list[i].add_percent;
						}
					}
				}

				int reward_bind_gold = m_activity_info.new_cornucopia_value * percent / 100;

				static MailContentParam contentparam; contentparam.Reset();
				contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, reward_bind_gold);
				
				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_new_cornucopia_reward_content);
				if (length > 0)
				{
					//MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
					MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
				}

				m_activity_info.new_cornucopia_value = 0;
				m_activity_info.new_cornucopia_total_reward_flag = 0;
				memset(m_activity_info.new_cornucopia_task_value_list, 0, sizeof(m_activity_info.new_cornucopia_task_value_list));
				this->NewCornucopiaUpdateTaskInfo();

				this->SendRANewCornucopiaInfo();
			}
		}
	}

	{
		// 经验炼制
		m_activity_info.refine_today_buy_time = 0;
		this->SendRAExpRefineInfo();
	}

	{
		// 开服投资
		this->AddOpenServerInvestFinishParam(RandActivityOpenServerInvestOtherCfg::INVEST_TPYE_BIPIN, 0);
		m_activity_info.ra_open_server_invest_is_add_active_param = 0;
	}

	{
		memset(m_activity_info.xingzuoyiji_gather_box_num, 0, sizeof(m_activity_info.xingzuoyiji_gather_box_num));
	}

	// 限时秒杀
	//		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_RUSH_BUYING))
	{
		m_activity_info.ra_rush_buying_cur_pahse = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingPhase(EngineAdapter::Instance().Time());
		memset(m_activity_info.ra_rush_buying_item_buying_times, 0, sizeof(m_activity_info.ra_rush_buying_item_buying_times));

		this->SendRARushBuyingAllInfo();
	}

	//合服活动
	this->OnCombiendServerActivityDayChange();

	// 幻装商店
	{
		m_activity_info.ra_magic_shop_buy_flag = 0;

		this->SendRAMagicShopAllInfo();
	}

	// 你充我送
	{
		this->RACheckChongZhiGiftGiveReward();
		m_activity_info.ra_magic_shop_fetch_reward_flag = 0;
		m_activity_info.ra_magic_shop_chongzhi_value = 0;
		this->SendRAChongZhiGiftAllInfo();
	}

	//秘境寻宝
	{
		m_activity_info.ra_mijingxunbao3_next_free_tao_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_activity_info.ra_mijingxunbao3_daily_free_times = LOGIC_CONFIG->GetRandActivityCfg().GetMiJingXunBao3FreeTimes();
		this->SendRAMiJingXunBao3Info();
	}

	//欢乐砸蛋
	{
		m_activity_info.ra_huanlezadan_next_free_tao_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_activity_info.ra_huanlezadan_daily_free_times = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeZaDanFreeTimes();
		this->SendRAHuanLeZaDanInfo();
	}

	//欢乐摇奖
	{
		m_activity_info.ra_huanleyaojiang_next_free_tao_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_activity_info.ra_huanleyaojiang_daily_free_times = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeYaoJiangFreeTimes();
		this->SendRAHuanLeYaoJiangInfo();
	}

	{
		//趣味钓鱼
//		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_FISHING))
		{
			m_activity_info.ra_fishing_times = 0;
			m_activity_info.ra_steal_fish_times = 0;
			m_activity_info.ra_be_steal_fish_times = 0;
		}
	}

	{
		// 狂返元宝
		m_activity_info.ra_chongzhi_crazy_rebate_day_chongzhi_total_count = 0;
		m_activity_info.ra_chongzhi_crazy_rebate_had_rebete_count = 0;
		this->SendRaCrazyRebateChongzhiInfo();
	}


	//{//天尊卡  刷新每日领取元宝
	//	FairyBuddhaCardOnDayChange();
	//}

	m_activity_info.day_chongzhi = 0;
	m_activity_info.last_chongzhi_day = 0;

	{ // 消费领奖
		this->RACheckConsumGiftReward();

		//if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUM_GIFT))
		{
			this->SendRAConsumeGiftInfo();
		}
	}

	{//每日限购
		memset(m_activity_info.ra_limit_buy_had_buy_count, 0, sizeof(m_activity_info.ra_limit_buy_had_buy_count));
		this->SendRADailyLimitBuyInfo();
	}

	{ // 聚宝盆
		m_activity_info.ra_collect_treasure_can_join_times = 0;
		m_activity_info.ra_collect_treasure_join_times = 0;
		this->SendRACollectTreasureInfo();
	}

	{// 欢乐累充
		this->RACheckHappyCumulChongzhiReward();
		this->SendRAHappyCumulChongzhiInfo();
	}

	{// 进阶返还
		m_activity_info.ra_jinjie_return_fetch_flag = 0;
		this->SendRAJinJieReturnInfo();

		m_activity_info.ra_jinjie_return2_fetch_flag = 0;
		this->SendRAJinJieReturn2Info();
	}
	
	{// 限时反馈
		m_activity_info.ra_limit_time_rebate_cur_day_is_chongzhi = 0;
		m_activity_info.ra_limit_time_rebate_cur_day_chongzhi = 0;
		this->SendLimitTimeRebateInfo();
	}

	
	{// 限时礼包
		this->RACheckTimeLimitGiftReward();
		m_activity_info.ra_time_limit_gift_open_flag = 1;

		//if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT))
		{
			m_activity_info.ra_time_limit_gift_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		}
	}

	{// 循环充值2
		this->RACheckCirculationChongzhiSecondReward();
	}

	{// 疯狂摇钱树
		this->RACheckShakeMoneyGiveReward();
	}

	{// 限时豪礼
		m_activity_info.ra_time_limit_luxury_gift_open_flag = 1;
		m_activity_info.ra_time_limit_luxury_gift_bag_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		//this->SendRATimeLimitLuxuryGiftBagInfo();
	}
	
	{// 消费返礼
		this->RACheckConsumeGoldRewardAutoFetch();
		m_activity_info.ra_consume_gold_reward_consume_gold = 0;
		m_activity_info.ra_consume_gold_reward_activity_day = 0;
		m_activity_info.ra_consume_gold_reward_fetch_flag = 0;
		m_activity_info.ra_consume_gold_reward_today_vip_level = m_role->GetVip()->GetVipLevel();
		this->SendRAConsumeGoldRewardInfo();
	}

	{// 集月饼活动
		memset(m_activity_info.collection_exchange_2_times, 0, sizeof(m_activity_info.collection_exchange_2_times));
		this->SendCollectSecondExchangeInfo();
	}

	{// 版本连续充值
		m_activity_info.ra_version_is_lei_chongzhi = 0;
		m_activity_info.ra_version_continue_chongzhi_today_chongzhi = 0;
		this->SendRAVersionContinueChargeActivityInfo();
	}

	{
		// 累计充值5活动，切换阶段，重置总充值额度
		bool too_long = (day_change_count > 1); // 说明相隔上次登录差了1天以上，需要强制清理一下数据
		int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE), EngineAdapter::Instance().Time());

		// 跨服钟活动没有开启, 用角色身上的数据
		if (m_role->IsInCross())
		{
			day_index = m_activity_info.ra_total_charge_5_join_day_index + 1;
		}

		if (too_long || LOGIC_CONFIG->GetRandActivityCfg().CheckIsResetTotalCharge5Day(day_index))
		{
			m_activity_info.ra_total_charge_5_cur_total_charge = 0;
			m_activity_info.ra_total_charge_5_cur_has_fetch_flag = 0;

			this->OnSendTotalCharge5Info();
		}

		m_activity_info.ra_total_charge_5_cur_total_charge = 0;
	}

	{	// 每日好礼	每日充值数量置0，检查昨天奖励是否领取，增加可领取奖励标记
		m_activity_info.ra_everyday_nice_gift_everydaty_recharge = 0;
		this->OnRAEverydayNiceGiftGiveRewardOnDayChange();

		if (m_activity_info.ra_everyday_nice_gift_is_have_recharge_standard != 0)
		{
			unsigned int begin_timestamp = 0;
			if (m_role->IsInCross())
			{
				begin_timestamp = m_activity_info.ra_everyday_nice_gift_recharge_timestamp;
			}
			else
			{
				begin_timestamp = ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT);
			}

			unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			int day_index = GetDayIndex(begin_timestamp, now_second);

			if (day_index > 0 && day_index < RAND_ACTIVITY_EVERYDAY_NICE_GIFT_MAX_REWARD_COUNT)
			{
				m_activity_info.ra_everyday_nice_gift_can_fetch_reward_flag |= 1 << day_index;
			}
		}

		this->SendRAEverydayNiceGiftInfo();
	}

	{// 组合购买
		memset(m_activity_info.ra_combine_buy_item_buy_times_list, 0, sizeof(m_activity_info.ra_combine_buy_item_buy_times_list));
		this->SendRACombineBuyItemInfo();
	}

	{// 天天返利
		m_activity_info.had_add_chongzhi_day_count = 0;
	}

	{//开服比拼邮件通知标志
		m_activity_info.ra_open_server_mail_send_flag = 0;
		this->RACheckActivityOpenNotification();
	}

	//幸运许愿
	{
		this->OnRALuckyWishResetLuckyValue();
		Protocol::SCLuckyWishInfo info;
		info.type = Protocol::LUCKY_WISH_TYPE_ONLY_LUCKY_VALUE;
		info.item_list_count = 0;
		info.lucky_value = this->GetRALuckyWishValue();
		this->SendRALuckyWishInfo(info);
	}

	//百倍返利
	{
		this->SendBaiBeiFanLiInfo();
	}

	// 狂嗨庆典
	{
		memset(m_activity_info.ra_kuang_hai_qing_dian_score_list, 0, sizeof(m_activity_info.ra_kuang_hai_qing_dian_score_list));
		memset(m_activity_info.ra_kuang_hai_qing_dian_task_info_list, 0, sizeof(m_activity_info.ra_kuang_hai_qing_dian_task_info_list));

		// 检查登陆任务
		this->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_LOGIN, 1);

		this->SendKuangHaiInfo();
	}

	this->SendChongZhiInfo();
}

void RoleActivity::OnWeekChange()
{
	
}

void RoleActivity::OnRoleLevelChange(int level)
{
	this->RACheckActivityOpenNotification();	// 检查开服活动邮件通知
}

void RoleActivity::OnDie(ObjID killer_objid)
{
	
}

void RoleActivity::OnRoleLogin()
{
	this->RandActivityOnRoleLogin();
	this->OnRALoginGiftLogin();
	this->OnCSALoginGiftLogin();

	this->OnRALoginGiftLogin_0();														//登陆好礼0

// 	this->SyncGuildBeastInfo();

	this->SyncGuildFbStatus();
	this->SyncGuildBonfireStatus();
	this->CheckRALuckyCloudBuyReturn();

	if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		this->OnCombineServerActivityOpen();
		if (ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_GONGCHENGZHAN) || ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_XIANMENGZHAN))
		{
			CombineServerActivityManager::Instance().OnSendActivityInfo(m_role);
		}
	}
	else if (!CrossConfig::Instance().IsHiddenServer())
	{//不在活动时间补发奖励
		this->CheckTouzijihuaFetch();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_GOLDEN_PIG))
	{
		this->RAGoldenPigSendBossState();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_OPEN_SERVER_INVEST))
	{
		this->SendRAOpenServerInvestInfo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUM_GIFT))   // 消费领奖
	{
		this->SendRAConsumeGiftInfo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE))
	{
		m_activity_info.ra_extreme_challenge_last_update_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}

	this->RACheckActivityOpenNotification();

	// 检查登陆任务
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		this->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_LOGIN, 1);
	}

	//this->SendFairyBuddhaCardInfo();

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DOUBLE_GET))
	{
		this->SendRADoubleGetInfo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHARGE1))
	{
		this->SendSingleChargeInfoOne();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHARGE2))
	{
		this->SendSingleChargeInfoTwo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHARGE3))
	{
		this->SendSingleChargeInfoThree();
	}
}

void RoleActivity::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		const RandActivityAnyMonsterDropConfig *drop_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetAndyMonsterDropCfg();
		if (NULL != drop_cfg)
		{
			int rate = m_activity_info.ra_any_monster_drop_total_add_rate;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += drop_cfg->maxhp * rate;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += drop_cfg->gongji * rate;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += drop_cfg->fangyu * rate;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += drop_cfg->mingzhong * rate;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += drop_cfg->shanbi * rate;
		}

		{//首充属性加成
			this->ReCalcAttrByFirstRecharge(m_attrs_add);
		}
		
		

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_ACTIVITY, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void RoleActivity::ReCalcAttrByFirstRecharge(CharIntAttrs & base_add)
{
	int first_chongzhi_max_count = LOGIC_CONFIG->GetChongzhiCfg().GetFirstChongzhiFirstMaxCount();
	for (int i = 0; i < first_chongzhi_max_count && i < MAX_FIRST_CHONGZHI_REWARD_COUNT; ++i)
	{
		const FirstChongzhiRewradConfig *first_chongzhi_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetFirstChongzhiRewradConfig(i);
		if (nullptr != first_chongzhi_cfg && m_activity_info.ra_is_has_first_recharge_attr_add)
		{
			if (first_chongzhi_cfg->attribute_type.size() != first_chongzhi_cfg->attribute_value.size())
			{
				continue;
			}
			for (size_t j = 0; j < first_chongzhi_cfg->attribute_value.size(); ++j)
			{
				if (first_chongzhi_cfg->attribute_value[j] <= 0)
				{
					continue;
				}
				base_add.AddValue(first_chongzhi_cfg->attribute_type[j], first_chongzhi_cfg->attribute_value[j]);
			}
		}
	}
}

void RoleActivity::SyncFirstRechargeBuffInfo()
{
	Protocol::SCFirstRechargeBuffFlag info;
	info.ra_is_has_first_recharge_attr_add = m_activity_info.ra_is_has_first_recharge_attr_add;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}


void RoleActivity::Update(unsigned int now_second)
{
	if (m_is_first_update)
	{
		m_is_first_update = false;

		this->CheckDailyTotalChongzhiStage();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ZHENBAOGE))
	{
		if (m_activity_info.ra_zhenbaoge_next_free_flush_timestamp > 0 && now_second > m_activity_info.ra_zhenbaoge_next_free_flush_timestamp)
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_ZHENBAOGE);
			this->OnRaZhenbaogeFlush(true, false);
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MINE))
	{
		if (m_activity_info.ra_mine_next_refresh_time > 0 && now_second > m_activity_info.ra_mine_next_refresh_time)
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MINE);
			this->OnRAMineRefresh(false);
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_FANFAN))
	{
		if (m_activity_info.ra_fanfan_next_refresh_time > 0 && now_second > m_activity_info.ra_fanfan_next_refresh_time)
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_FANFAN);
			this->OnRAFanFanRefresh(false);
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EXTREME_LUCKY))
	{
		if (m_activity_info.ra_extreme_lucky_next_flush_timestamp > 0 && now_second > m_activity_info.ra_extreme_lucky_next_flush_timestamp)
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_EXTREME_LUCKY);
			this->OnRAExtremeLuckyFlush(true);
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ZHENBAOGE2))
	{
		if (m_activity_info.ra_zhenbaoge2_next_free_flush_timestamp > 0 && now_second > m_activity_info.ra_zhenbaoge2_next_free_flush_timestamp)
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_ZHENBAOGE2);
			this->OnRaZhenbaoge2Flush(true, false);
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_RUSH_BUYING))
	{
		int cur_phase = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingPhase(now_second);

		if (m_activity_info.ra_rush_buying_cur_pahse != cur_phase)
		{
			m_activity_info.ra_rush_buying_cur_pahse = cur_phase;
			memset(m_activity_info.ra_rush_buying_item_buying_times, 0, sizeof(m_activity_info.ra_rush_buying_item_buying_times));

			this->SendRARushBuyingAllInfo();
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MAP_HUNT))
	{
		if (now_second > m_activity_info.ra_map_hunt_next_flush_timestamp && m_activity_info.ra_map_hunt_next_flush_timestamp > 0)
		{
			bool is_get = true;
			this->OnRaMapHuntFlush(true, is_get);
		}
	}


	//仙尊卡
	/*int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
	if (m_role->GetLevel() >= FAIRY_BUDDHA_CARD_START_LEVEL && opengame_day >= FAIRY_BUDDHA_CARD_START_DAY)
	{
		this->OnFairyBuddhaCardUpdate(now_second);
	}*/

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT) && m_activity_info.ra_time_limit_gift_open_flag > 0)
	{
		const int vip_level = m_activity_info.ra_time_limit_gift_join_vip_level;
		int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), m_activity_info.ra_time_limit_gift_begin_timestamp) + 1;
		long long old_recharge_gold = this->GetHistoryChongZhi() - this->GetDayChongzhi();	//除了今日充值的元宝数
		const RandActivityLimitTimeGiftCfg::SubItemCfg *tlg_sub_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLimitTimeGiftRewardCfg(opengame_day, vip_level, old_recharge_gold);
		if (nullptr != tlg_sub_cfg)
		{
			const int interval = now_second - m_activity_info.ra_time_limit_gift_begin_timestamp;
			if (interval > tlg_sub_cfg->limit_time)
			{
				this->RACheckTimeLimitGiftReward();
			}
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LIMIT_LUXURY_GIFT_BAG_GIFT) && m_activity_info.ra_time_limit_luxury_gift_open_flag > 0)
	{
		const int vip_level = m_activity_info.ra_time_limit_luxury_gift_bag_join_vip_level;
		int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), m_activity_info.ra_time_limit_luxury_gift_bag_begin_timestamp) + 1;
		const RandActivityLimitTimeLuxuryGiftBagCfg::SubItemCfg *tlg_sub_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLimitTimeLuxuryGiftBagRewardCfg(opengame_day, vip_level);

		if (nullptr != tlg_sub_cfg)
		{
			const int interval = now_second - m_activity_info.ra_time_limit_luxury_gift_bag_begin_timestamp;

			if (interval > tlg_sub_cfg->limit_time)
			{
				m_activity_info.ra_time_limit_luxury_gift_open_flag = 0;
				this->SendRATimeLimitLuxuryGiftBagInfo();
			}
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE))
	{
		static const int EXTREME_CHALLENGE_UPDATE_INTERVAL = 10;

		if (now_second >= m_activity_info.ra_extreme_challenge_last_update_timestamp + EXTREME_CHALLENGE_UPDATE_INTERVAL)
		{
			this->OnRAExtremeChallengeAddPlan(RA_EXTREME_CHALLENGE_ON_LINE, now_second - m_activity_info.ra_extreme_challenge_last_update_timestamp);
			m_activity_info.ra_extreme_challenge_last_update_timestamp = now_second;
		}
	}
}
// --------------------------------------------------------------------------------------------------------
// 充值
// --------------------------------------------------------------------------------------------------------

void RoleActivity::CheckChongzhiExtraRewardByNum(int chongzhi_num)
{
	if (SHAREDCONFIG->GetOpenServerConfig().IsIosExamine())
	{
		return;
	}

	// 每种充值金额额外奖励
	UNSTD_STATIC_CHECK(CHONGZHI_REWARD_CONFIG_MAX_NUM <= (int)sizeof(m_activity_info.chongzhi_reward_fetch_flag) * 8);

	const ChongzhiReward *reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(chongzhi_num);
	if (NULL != reward_cfg)
	{
		int chongzhi_flag = (1 << reward_cfg->seq);

		this->CheckChongzhiExtraRewardByFlag(chongzhi_flag);	
	}
}

void RoleActivity::CheckChongzhiExtraRewardByFlag(int chongzhi_flag)
{
	bool need_send = false;

	for (int seq = 0; seq < LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardCfgCount(); ++ seq)
	{
		if (0 == (chongzhi_flag & (1 << seq)))
		{
			continue;
		}

		const ChongzhiReward *reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardBySeq(seq);
		if (NULL == reward_cfg)
		{
			continue;
		}

		// 充值-七日返利
		if (0 == m_activity_info.chongzhi_7day_reward_timestamp && LOGIC_CONFIG->GetChongzhiCfg().GetOtherCfg().chongzhi_seven_day_need_gold == reward_cfg->chongzhi)
		{
			m_activity_info.chongzhi_7day_reward_timestamp = (unsigned int)EngineAdapter::Instance().Time();
			need_send = true;
			continue;
		}

		{
			// 单笔充值活动
			OnRADanbiChongzhiCheckChongzhi(reward_cfg->chongzhi);
		}

		if (0 != (m_activity_info.chongzhi_reward_fetch_flag & (1 << seq)))
		{
			continue;
		}

		if (CHONGZHI_EXTRA_REWARD_TYPE_COIN_BIND == reward_cfg->reward_type)
		{
			if (reward_cfg->extra_bind_coin > 0)
			{
				// 发额外绑定元宝奖励
				static MailContentParam contentparam; contentparam.Reset();

				contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_COIN, reward_cfg->extra_bind_coin);

				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_chongzhi_reward_subject);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_chongzhi_reward_coin_content, reward_cfg->chongzhi, reward_cfg->extra_bind_coin);

				if (length1 > 0 && length2 > 0)
				{
					m_activity_info.chongzhi_reward_fetch_flag |= (1 << reward_cfg->seq);

					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
		else if (CHONGZHI_EXTRA_REWARD_TYPE_GOLD_BIND == reward_cfg->reward_type)
		{
			// 开服前7天才有额外绑元奖励
			int open_day = GetDayIndex(m_role->GetServerStartTime(), EngineAdapter::Instance().Time());
			int limit_day = LOGIC_CONFIG->GetChongzhiCfg().GetOtherCfg().chongzhi_extra_reward_limit_day;
			if (reward_cfg->extra_bind_gold > 0 && open_day < limit_day && open_day >= 0)
			{
				// 发额外绑定元宝奖励
				static MailContentParam contentparam; contentparam.Reset();
			
				contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, reward_cfg->extra_bind_gold);

				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_chongzhi_reward_subject);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_chongzhi_reward_content, reward_cfg->chongzhi, reward_cfg->extra_bind_gold);

				if (length1 > 0 && length2 > 0)
				{
					m_activity_info.chongzhi_reward_fetch_flag |= (1 << reward_cfg->seq);

					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
		else if (CHONGZHI_EXTRA_REWARD_TYPE_MONTH_CARD == reward_cfg->reward_type)
		{
			m_activity_info.chongzhi_reward_fetch_flag |= (1 << reward_cfg->seq);

			EventHandler::Instance().OnBuyMonthCard(m_role);
		}
		else if (CHONGZHI_EXTRA_REWARD_TYPE_GOLD == reward_cfg->reward_type)
		{
			if (reward_cfg->extra_gold > 0)
			{
				// 发额外元宝奖励
				static MailContentParam contentparam; contentparam.Reset();
				contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, reward_cfg->extra_gold);

				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_chongzhi_reward_subject);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_chongzhi_reward_content, reward_cfg->chongzhi, reward_cfg->extra_gold);

				if (length1 > 0 && length2 > 0)
				{
					m_activity_info.chongzhi_reward_fetch_flag |= (1 << reward_cfg->seq);

					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}

		need_send = true;
	}

	int open_day = m_role->GetOpenServerDay();

	if (open_day > 7)
	{
		m_activity_info.chongzhi_reward_fetch_flag = 0;
	}

	if (need_send)
	{
		this->SendChongZhiInfo();
	}
}

void RoleActivity::OnAddChongZhi(long long chongzhi)
{
	if (chongzhi <= 0) return;

	const int open_day = m_role->GetOpenServerDay();
	{
		// 日常累计充值相关
		{
			if (LOGIC_CONFIG->GetChongzhiCfg().IsVaildDailyTotalChongzhiStage(m_activity_info.daily_total_chongzhi_stage) && 
				LOGIC_CONFIG->GetChongzhiCfg().IsValidDailyTotalChongzhiDay(open_day))
			{
				m_activity_info.daily_total_chongzhi_stage_chongzhi += static_cast<int>(chongzhi);
			}
		}

		// 特殊首冲
		if (0 == m_activity_info.is_daily_first_chongzhi_open && 0 == m_activity_info.special_first_chongzhi_timestamp)
		{
			m_activity_info.special_first_chongzhi_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			{
				// 首次充值传闻
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chongzhi_first_chongzhi_reward_content, 
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
				if (length > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
				}
			}
		}
	}

	if (0 != m_activity_info.is_daily_first_chongzhi_open)
	{
		// 计算充值当天是星期几
		if (m_activity_info.daily_chongzhi_week_day_idx < 0)
		{
			const tm *local_time = EngineAdapter::Instance().LocalTime();
			m_activity_info.daily_chongzhi_week_day_idx = static_cast<char>(local_time->tm_wday);
		}

		// 增加首冲次数
		if (m_activity_info.day_chongzhi <= 0)
		{
			++ m_activity_info.daily_first_chongzhi_times;
		}
	}

	//每日累充
	if (1 == m_activity_info.diff_wd_chongzhi_is_open)
	{
		m_activity_info.diff_wd_chongzhi_value += static_cast<int>(chongzhi);
	}

	{
		// 再充值奖励，再充达到目标，不对三充做检查
		if (!this->CheckZaiChongzhiRewardState(chongzhi))
		{
			this->CheckThirdChongzhiRewardState(chongzhi);
		}
	}

	{
		// 随机活动 - 充值回馈
		this->OnRAChargeRewardActive(chongzhi);

		// 随机活动 - 单笔充值
		this->OnRASingleChargeCheckCharge(chongzhi);

		// 随机活动 - 单笔充值(主界面特殊显示、极速冲战)
		this->OnRASingleChargeCheckCharge2(chongzhi);

		// 随机活动 - 单笔充值3(主界面特殊显示、极速冲战2)
		this->OnRASingleChargeCheckCharge3(chongzhi);

		// 随机活动 - 单笔充值4(主界面特殊显示、 极速冲战3)
		this->OnRASingleChargeCheckCharge4(chongzhi);

		// 随机活动 - 单笔充值5(主界面特殊显示、 极速冲战4)
		this->OnRASingleChargeCheckCharge5(chongzhi);

		// 随机活动 - 双倍充值
		this->OnRADoubleCharge(chongzhi);

		// 随机活动 - 累计充值（天数）
		this->OnRATotalChargeDayCheckChongzhi(chongzhi);

		// 随机活动 - 每日充值
		this->OnRADayChongzhiRankAddChongzhi((int)chongzhi);

		// 随机活动 - 累计充值
		this->OnRATotalChargeCheckChongzhi((int)chongzhi);

		// 随机活动 - 每日爱一次
		this->OnRADailyLoveChongzhi((int)chongzhi);

		// 随机活动 - 累计充值3
		this->OnRATotalCharge3CheckChongzhi((int)chongzhi);

		// 随机活动 - 连续充值
		this->OnRAContinueChongzhiAddChongzhi((int)chongzhi);

		// 随机活动 - 连充特惠初
		this->OnRAContinueChongzhiAddChongzhiChu((int)chongzhi);

		// 随机活动 - 连充特惠高
		this->OnRAContinueChongzhiAddChongzhiGao((int)chongzhi);

		// 随机活动 - 限时连充
		this->OnRAContinueChongzhiAddChongzhi2((int)chongzhi);

		// 随机活动 - 循环充值
		this->OnRACirculationChongzhiChongzhiGold((int)chongzhi);

		// 随机活动 - 步步高升
		this->OnRAPromotingPositionChongzhiGold((int)chongzhi);

		// 随机活动 - 新累计充值
		this->OnRANewTotalChargeCheckChongzhi((int)chongzhi);

		// 随机活动 - 你充我送
		this->OnRAChongZhiGiftCheckChongzhi((int)chongzhi);

		// 狂返元宝
		this->OnRAChongzhiCrazyRebate(chongzhi);

		// 限时反馈
		this->OnRaLimitTimeRebateAddchongzhi((int)chongzhi);

		// 限时礼包
		this->OnRATimeLimitGiftCheckChongzhi((int)chongzhi);

		// 循环充值2
		this->OnRACirculationChongzhiSecondAddchongzhi((int)chongzhi);

		// 疯狂摇钱树
		this->OnRAShakeMoneyCheckChongzhi((int)chongzhi);

		// 普天同庆
		this->OnRADoubleChongzhiChongzhi((int)chongzhi);

		// 神秘商店
		this->OnRARmbBugChestShopChongzhi((int)chongzhi);

		// 版本累计充值
		this->OnRAVersionTotalChargeCheckChongzhi((int)chongzhi);

		// 版本连续充值
		this->OnRAVersionContinueChargeAddChongzhi((int)chongzhi);

		// 版本单笔充值
		this->OnRAOfflineSingleChargeChongZhi((int)chongzhi);

		// 版本活动 累计充值5
		this->OnRATotalCharge5CheckChongzhi((int)chongzhi);
	}

	if (m_activity_info.day_chongzhi <= 0)
	{
		// 今日首次充值人数加1
		WorldStatus::Instance().AddTodayFirstChongzhiCount(1);

		//聚宝盆
		this->OnRACollectTreasureChongZhi((int)chongzhi);
	}

	{
		m_activity_info.day_chongzhi += (int)chongzhi;
		m_activity_info.history_chongzhi += chongzhi;
		++ m_activity_info.history_chongzhi_count;

		//m_activity_info.last_chongzhi_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
		m_activity_info.last_chongzhi_day = open_day;
	}

	{//天天返利
		this->OnDayChongzhiRewardAddChongzhi();
	}

	//首充团购信息更新
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN))
	{
		OpenServerSubActivityInfo &info = m_activity_info.open_server_sub_info_list[RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN - RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN];
		this->SendRAOpenServerInfo(RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN, info.reward_flag);
	}

	{
		// 我要元宝充值后达到领取要求，通知客户端【要放在history_chongzhi值更新后，不然发过去的数据是旧的】
		const TotalChongzhiWantMoneyConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetChongzhiCfg().GetWantMoneyCfg(m_activity_info.total_chongzhi_want_money_state);
		if (NULL != cfg_item)
		{
			this->GetTotalChongzhiWantMoneyInfo();
		}
	}

	{
		// 聚宝盆活动
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CORNUCOPIA))
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CORNUCOPIA);

			m_activity_info.ra_cornucopia_history_charge += (int)chongzhi;
			this->SendRACornucopiaInfo();
		}

		// 新聚宝盆活动
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_CORNUCOPIA))
		{
			m_activity_info.new_cornucopia_value += (int)chongzhi;
			WorldStatus::Instance().GetCommonData().new_cornucopia_total_value += (int)chongzhi;
			m_activity_info.new_cornucopia_day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_NEW_CORNUCOPIA), EngineAdapter::Instance().Time());
			this->SendRANewCornucopiaInfo();
		}
	}

	{
		//充值扭蛋活动--可扭蛋额度
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG))
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG);

			m_activity_info.ra_niu_egg_cur_chongzhi_value += (int)chongzhi;
			this->SendRANiuEggInfo();
		}
	}

	this->SendChongZhiInfo();
	this->SendOpenGameActivityInfo();

	{
		// 随机活动
		this->OnRAChongzhiRankChongzhi((int)chongzhi);
		this->OnRAChongzhiRank2Chongzhi((int)chongzhi);
		this->OnRADayChongZhiFanLiAddChongZhi(chongzhi);
		this->OnRAHappyCumulChongzhiGold((int)chongzhi);
		// 运营活动，每日好礼
		this->OnRAEverydayNiceGiftRecharge((int)chongzhi);
	}

	{
		//急速冲战
		this->OnRaJiSuChongZhanGiveReward((int)chongzhi);
	}

	{
		//累计充值4(新三件套)
		this->OnRATotalCharge4CheckChongzhi((int)chongzhi);
	}

	{
		// 合服活动
		if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
		{
			if (ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_ROLL))
			{
				m_activity_info.csa_roll_chongzhi_num += static_cast<int>(chongzhi);
				m_activity_info.csa_roll_total_chongzhi += static_cast<int>(chongzhi);
				this->SendCSARoleInfo();
			}

			if (ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_CHONGZHI_RANK))
			{
				m_activity_info.csa_chongzhi_rank_chongzhi_num += static_cast<int>(chongzhi);
				this->SendCSARoleInfo();

				int rank_type = LOGIC_CONFIG->GetCombineServerActivityConfig().GetRelatedRankType(CSA_SUB_TYPE_CHONGZHI_RANK);

				CombineServerActivityManager::Instance().OnCombineServerActivitySyncValue(rank_type, m_role->GetUID(),
					m_activity_info.csa_chongzhi_rank_chongzhi_num, static_cast<long long>(LLONG_MAX - EngineAdapter::Instance().Time()));
			}
		}

		this->OnCSASingleChargeCheckCharge(chongzhi);
		this->ONCSAChargeRewardDoubleCheckCharge(chongzhi);
	}

	{
		// 每日充值  // 2019-04-03 去除首充限制每日首充
		//if (0 != m_activity_info.first_chongzhi_active_flag)
		{
			int old_daily_chongzhi = m_activity_info.daily_activity_chongzhi;
			if (m_activity_info.daily_activity_chongzhi + chongzhi >= INT_MAX16)
			{
				m_activity_info.daily_activity_chongzhi = INT_MAX16;
			}
			else
			{
				m_activity_info.daily_activity_chongzhi += static_cast<short>(chongzhi);
			}
			
			int min_chongzhi = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiMinChongzhi();
			if (old_daily_chongzhi < min_chongzhi && m_activity_info.daily_activity_chongzhi >= min_chongzhi)
			{
				m_activity_info.daily_chongzhi_complete_days++;
			}

			this->SendChongZhiInfo();
		}
	}

	{
		// 首冲奖励
		bool is_send_first_chongzhi = false;
		int max_seq = LOGIC_CONFIG->GetChongzhiCfg().GetFirstChongzhiFirstMaxCount();
		const time_t now_sec = EngineAdapter::Instance().Time();
		const time_t role_create_time = m_role->GetCreateRoleTimestamp();
		const time_t create_zero_timestamp = GetZeroTime(role_create_time);
		for (int seq = 0; seq < max_seq && seq < MAX_FIRST_CHONGZHI_REWARD_COUNT; seq++)
		{
			const FirstChongzhiRewradConfig *first_chongzhi_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetFirstChongzhiRewradConfig(seq);
			if (NULL == first_chongzhi_cfg) continue;

			if (0 != (m_activity_info.first_chongzhi_active_flag & (1 << seq))) continue;

			if (m_activity_info.history_chongzhi >= first_chongzhi_cfg->need_total_chongzhi)
			{
				if (!m_activity_info.ra_is_has_first_recharge_attr_add && first_chongzhi_cfg->active_attr_time_limit > 0 && now_sec <= create_zero_timestamp + first_chongzhi_cfg->active_attr_time_limit)
				{
					m_activity_info.ra_is_has_first_recharge_attr_add = 1;
					this->SyncFirstRechargeBuffInfo();
				}
				m_activity_info.first_chongzhi_active_flag |= (1 << seq);
				is_send_first_chongzhi = true;
			}
		}

		if (is_send_first_chongzhi)
		{
			this->SendChongZhiInfo();
			TianShuXunZhu::CheckShiXueTianShu(m_role, CHENGZHANG_TIANSHU_TYPE_FINISH_SHOUCHONG);
		}
	}

	{
		//金猪召唤
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_GOLDEN_PIG))
		{
			m_activity_info.ra_golden_pig_chongzhi += static_cast<int>(chongzhi); 
			m_activity_info.ra_golden_pig_summon_credit += static_cast<int>(chongzhi);
			this->SendRAGoldenPigSummonInfo();
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI))
	{
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI);

		this->OnRASingleChongZhiAddChongZhi((int)chongzhi);
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAILY_LOVE_2))
	{
		this->OnRADailyLove2Chongzhi((int)chongzhi);
	}
	// 随机活动 - 狂欢大乐购
	this->OnRACracyBuyChongZhi((int)chongzhi);

	// 随机活动 - 单笔充值
	this->OnRASingleChargeCheckChargeOne(chongzhi);
	this->OnRASingleChargeCheckChargeTwo(chongzhi);
	this->OnRASingleChargeCheckChargeThree(chongzhi);

	// 随机活动 - 累计充值
	this->OnRANewTotalChargeCheckChongzhi1((int)chongzhi);
	this->OnRANewTotalChargeCheckChongzhi2((int)chongzhi);
	this->OnRANewTotalChargeCheckChongzhi3((int)chongzhi);

	this->OnRADoubleGetCheckCharge(chongzhi);
}

void RoleActivity::SendChongZhiInfo()
{
	Protocol::SCChongZhiInfo czi;
	czi.history_chongzhi = m_activity_info.history_chongzhi;
	czi.history_chongzhi_count = m_activity_info.history_chongzhi_count;
	czi.today_chongzhi = m_activity_info.day_chongzhi;
	czi.chongzhi_reward_fetch_flag = m_activity_info.chongzhi_reward_fetch_flag;

	czi.special_first_chongzhi_timestamp = m_activity_info.special_first_chongzhi_timestamp;
	czi.is_daily_first_chongzhi_open = m_activity_info.is_daily_first_chongzhi_open;
	czi.is_daily_first_chongzhi_fetch_reward = m_activity_info.is_daily_first_chongzhi_fetch_reward;
	czi.daily_first_chongzhi_times = m_activity_info.daily_first_chongzhi_times;
	czi.daily_total_chongzhi_fetch_reward_flag = m_activity_info.daily_total_chongzhi_fetch_reward_flag;
	czi.daily_total_chongzhi_stage = m_activity_info.daily_total_chongzhi_stage;
	czi.daily_total_chongzhi_stage_chongzhi = m_activity_info.daily_total_chongzhi_stage_chongzhi;
	czi.special_first_chongzhi_fetch_reward_flag = m_activity_info.special_first_chongzhi_fetch_reward_flag;
	czi.zai_chongzhi_reward_flag = m_activity_info.zai_chongzhi_reward_state ;
	czi.third_chongzhi_reward_flag = m_activity_info.third_chongzhi_reward_state;
	czi.diff_weekday_chongzhi_is_open = m_activity_info.diff_wd_chongzhi_is_open;
	czi.diff_weekday_chongzhi_stage_fetch_flag = static_cast<short>(m_activity_info.diff_wd_chongzhi_stage_fetch_flag);
	czi.diff_wd_chongzhi_value = m_activity_info.diff_wd_chongzhi_value;
	czi.daily_chongzhi_value = m_activity_info.daily_activity_chongzhi;
	czi.first_chongzhi_active_reward_flag = m_activity_info.first_chongzhi_active_flag;
	czi.first_chongzhi_fetch_reward_flag = m_activity_info.first_chongzhi_fetch_reward_flag;
	czi.daily_chongzhi_fetch_reward_flag = m_activity_info.daily_chongzhi_fetch_reward_flag;
	czi.daily_chongzhi_complete_days = m_activity_info.daily_chongzhi_complete_days;
	czi.daily_chongzhi_times_fetch_reward_flag = m_activity_info.daily_chongzhi_times_fetch_reward_flag;
	czi.chongzhi_7day_reward_timestamp = m_activity_info.chongzhi_7day_reward_timestamp;
	czi.chongzhi_7day_reward_fetch_day = m_activity_info.chongzhi_7day_reward_fetch_day;
	czi.chongzhi_7day_reward_is_fetch = m_activity_info.chongzhi_7day_reward_is_fetch;
	czi.daily_chongzhi_fetch_reward2_flag = m_activity_info.daily_chongzhi_fetch_reward2_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&czi, sizeof(czi));
}

void RoleActivity::FetchSpecialFirstChongzhiReward(int index)
{
	if (0 == m_activity_info.special_first_chongzhi_timestamp)
	{
		return;
	}

	UNSTD_STATIC_CHECK(ChongzhiSpecialFirstRechargeConfig::CONFIG_ITEM_MAX_COUNT <= 8);

	if (index < 0 || index >= (int)sizeof(m_activity_info.special_first_chongzhi_fetch_reward_flag) * 8)
	{
		return;
	}

	if (0 != (m_activity_info.special_first_chongzhi_fetch_reward_flag & (1 << index)))
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_HAS_FETCH_REWARD);
		return;
	}

	int chongzhi_pass_day = GetDayIndex(m_activity_info.special_first_chongzhi_timestamp, EngineAdapter::Instance().Time());
	if (index > chongzhi_pass_day)
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_CAN_NOT_FETCH_REWARD);
		return;
	}

	const ChongzhiSpecialFirstRechargeConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetChongzhiCfg().GetSpecialFirstRechargeReward(index);
	if (NULL == cfg_item)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.special_first_chongzhi_fetch_reward_flag |= (1 << index);

	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_CHONGZHI_REWARD);

	this->SendChongZhiInfo();
}

void RoleActivity::FetchDailyTotalChongzhiReward(int seq)
{
	UNSTD_STATIC_CHECK(ChongzhiDailyTotalStageConfig::REWARD_ITEM_MAX_COUNT <= ((int)sizeof(m_activity_info.daily_total_chongzhi_fetch_reward_flag) * 8));

	if (seq < 0 || seq >= (int)sizeof(m_activity_info.daily_total_chongzhi_fetch_reward_flag) * 8)
	{
		return;
	}

	if (0 != (m_activity_info.daily_total_chongzhi_fetch_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_HAS_FETCH_REWARD);
		return;
	}

	if (!LOGIC_CONFIG->GetChongzhiCfg().IsVaildDailyTotalChongzhiStage(m_activity_info.daily_total_chongzhi_stage))
	{
		return;
	}

	const ChongzhiDailyTotalStageConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetChongzhiCfg().GetDailyTotalStageByStage(m_activity_info.daily_total_chongzhi_stage);
	if (NULL == cfg_item)
	{
		return;
	}

	if (seq < 0 || seq >= cfg_item->total_recharge_reward_item_count)
	{
		return;
	}

	if (m_activity_info.daily_total_chongzhi_stage_chongzhi < cfg_item->total_recharge_reward_need_chongzhi[seq])
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_TOTAL_RECHARGE_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.daily_total_chongzhi_fetch_reward_flag |= (1 << seq);

	m_role->GetKnapsack()->Put(cfg_item->total_recharge_reward_item_list[seq], PUT_REASON_CHONGZHI_REWARD);

	gamelog::g_log_other.printf(LL_INFO, "FetchDailyTotalChongzhiReward user[%d, %s], level[%d], fetch_flag[%d], seq[%d], reward_item[%s]", 
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), m_activity_info.daily_total_chongzhi_fetch_reward_flag, seq, ItemConfigDataLog(&cfg_item->total_recharge_reward_item_list[seq], NULL));

	ROLE_LOG_QUICK6(LOG_TYPE_DAILY_TOTAL_CHONGZHI_REWARD, m_role, seq, m_activity_info.daily_total_chongzhi_fetch_reward_flag, NULL, NULL);

	this->SendChongZhiInfo();
}

void RoleActivity::FetchDailyFirstChongzhiReward()
{
	if (0 == m_activity_info.is_daily_first_chongzhi_open)
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_FIRST_RECHARGE_NOT_OPEN);
		return;
	}

	if (m_activity_info.day_chongzhi <= 0)
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_NOT_RECHARGE_TODAY);
		return;
	}

	if (0 != m_activity_info.is_daily_first_chongzhi_fetch_reward)
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_HAS_FETCH_REWARD);
		return;
	}

	const ChongzhiOtherConfig &other_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetOtherCfg();

	int need_space = 1; bool give_extra_reward = false;
	if (m_activity_info.daily_first_chongzhi_times >= other_cfg.daily_first_recharge_extra_reward_need_times)
	{
		need_space += 1;
		give_extra_reward = true;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_space))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const ChongzhiDailyFirstRewardConfig::ConfigItem *daily_cfg_item = LOGIC_CONFIG->GetChongzhiCfg().GetDailyFirstReward(m_activity_info.daily_chongzhi_week_day_idx);
	if (NULL == daily_cfg_item) 
	{
		return;
	}

	// 消耗
	m_activity_info.is_daily_first_chongzhi_fetch_reward = 1;

	if (give_extra_reward)
	{
		m_activity_info.daily_first_chongzhi_times -= other_cfg.daily_first_recharge_extra_reward_need_times;
	}

	// 给予
	m_role->GetKnapsack()->Put(daily_cfg_item->first_recharge_reward_item, PUT_REASON_CHONGZHI_REWARD);

	if (give_extra_reward)
	{
		m_role->GetKnapsack()->Put(other_cfg.daily_first_recharge_extra_reward_item, PUT_REASON_CHONGZHI_REWARD);
	}

	this->SendChongZhiInfo();
}

// 充值-我要变元宝-获取玩家当前充值状态数据
void RoleActivity::GetTotalChongzhiWantMoneyInfo()
{
	/*
	Protocol::SCTotalChongzhiWantMoneyFetchInfo cwmfmi;
	cwmfmi.history_chongzhi = m_activity_info.history_chongzhi;
	cwmfmi.reward_state = m_activity_info.total_chongzhi_want_money_state;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cwmfmi, sizeof(cwmfmi));
	*/
}

// 充值-我要变元宝-获取玩家当前随机出来的奖励
void RoleActivity::FetchTotalChongzhiWantMoneyReward()
{
	/*
	const TotalChongzhiWantMoneyConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetChongzhiCfg().GetWantMoneyCfg(m_activity_info.total_chongzhi_want_money_state);
	if (NULL == cfg_item) // 所有奖励都已领完
	{
		m_role->NoticeNum(errornum::EN_ALL_REWARD_HAS_FETCHED);
		return;
	}

	if (cfg_item->need_chongzhi > m_activity_info.history_chongzhi) // 当前充值额度未达到下级奖励要求
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_WANT_MONEY_HAS_NOT_ENOUGH_CHARGE);
		return;
	}

	if (m_role->GetKnapsack()->GetMoney()->GetGold() < cfg_item->need_chongzhi) // 当前剩余元宝数未达到下级奖励要求
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_WANT_MONEY_HAS_NOT_ENOUGH);
		return;
	}

	int rand_base = cfg_item->random_max - cfg_item->random_min + 1;
	if (rand_base <= 0) 
	{
		return;
	}

	++ m_activity_info.total_chongzhi_want_money_state;

	int rand_gold_bind = RandomNum(rand_base) + cfg_item->random_min;

	m_role->GetKnapsack()->GetMoney()->AddGoldBind(rand_gold_bind,"TotalChongzhiWantMoneyReward");


	Protocol::SCTotalChongzhiWantMoneyFetchReward msg;
	msg.get_gold_bind = rand_gold_bind;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, sizeof(msg));

	this->GetTotalChongzhiWantMoneyInfo();
	*/
}


void RoleActivity::FetchZaiChongzhiReward()
{
	if (Protocol::ZAI_CHONGZHI_STATE_HAS_FETCH == m_activity_info.zai_chongzhi_reward_state)
	{
		m_role->NoticeNum(errornum::EN_ZAI_CHONGZHI_HAS_FETCH);
		return;
	}

	if (Protocol::ZAI_CHONGZHI_STATE_CAN_FETCH != m_activity_info.zai_chongzhi_reward_state)
	{
		m_role->NoticeNum(errornum::EN_ZAI_CHONGZHI_FETCH_LIMIT);
		return;
	}

	const ChongzhiOtherConfig &other_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetOtherCfg();
	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(other_cfg.zai_chongzhi_reward_item.num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.zai_chongzhi_reward_state = Protocol::ZAI_CHONGZHI_STATE_HAS_FETCH;
	m_role->GetKnapsack()->Put(other_cfg.zai_chongzhi_reward_item, PUT_REASON_ZAI_CHONGZHI_REWARD);

	gamelog::g_log_rand_activity.printf(LL_INFO, "FetchZaiChongzhiReward::user[%d, %s], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(),ItemConfigDataLog(&other_cfg.zai_chongzhi_reward_item, NULL));

	this->SendChongZhiInfo();
}

void RoleActivity::FetchThirdChongzhiReward()
{
	if (Protocol::ZAI_CHONGZHI_STATE_HAS_FETCH == m_activity_info.third_chongzhi_reward_state)
	{
		m_role->NoticeNum(errornum::EN_ZAI_CHONGZHI_HAS_FETCH);
		return;
	}

	if (Protocol::ZAI_CHONGZHI_STATE_CAN_FETCH != m_activity_info.third_chongzhi_reward_state)
	{
		m_role->NoticeNum(errornum::EN_ZAI_CHONGZHI_FETCH_LIMIT);
		return;
	}

	const ChongzhiOtherConfig &other_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetOtherCfg();
	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(other_cfg.third_chongzhi_reward_item.num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.third_chongzhi_reward_state = Protocol::ZAI_CHONGZHI_STATE_HAS_FETCH;
	m_role->GetKnapsack()->Put(other_cfg.third_chongzhi_reward_item, PUT_REASON_ZAI_CHONGZHI_REWARD);

	gamelog::g_log_rand_activity.printf(LL_INFO, "FetchThirdChongzhiReward::user[%d, %s], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(),ItemConfigDataLog(&other_cfg.third_chongzhi_reward_item, NULL));

	this->SendChongZhiInfo();
}

bool RoleActivity::CheckZaiChongzhiRewardState(long long gold_num)
{
	if (Protocol::ZAI_CHONGZHI_STATE_INVALID == this->m_activity_info.zai_chongzhi_reward_state &&
		m_activity_info.history_chongzhi > 0)
	{
		const ChongzhiOtherConfig &other_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetOtherCfg();
		if (gold_num >= other_cfg.zai_chongzhi_need_chongzhi)
		{
			m_activity_info.zai_chongzhi_reward_state = Protocol::ZAI_CHONGZHI_STATE_CAN_FETCH;

			// 再冲传闻
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chongzhi_zai_chongzhi_reward_content, 
// 				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
// 			if (length > 0) 
// 			{
// 				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
// 			}

			return true;
		}
	}

	return false;
}

bool RoleActivity::CheckThirdChongzhiRewardState(long long gold_num)
{
	if (Protocol::ZAI_CHONGZHI_STATE_INVALID == this->m_activity_info.third_chongzhi_reward_state &&
		m_activity_info.history_chongzhi > 0)
	{
		const ChongzhiOtherConfig &other_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetOtherCfg();
		if (gold_num >= other_cfg.third_chongzhi_need_chongzhi)
		{
			m_activity_info.third_chongzhi_reward_state = Protocol::ZAI_CHONGZHI_STATE_CAN_FETCH;

			// 再冲传闻
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chongzhi_san_chongzhi_reward_content, 
// 				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
// 			if (length > 0) 
// 			{
// 				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
// 			}

			return true;
		}
	}

	return false;
}

void RoleActivity::CheckDailyTotalChongzhiStage()
{
	//int open_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
	const int open_day = m_role->GetOpenServerDay();
	int min_start_day = LOGIC_CONFIG->GetChongzhiCfg().GetDailyTotalChongzhiMinStartDay();

	if (open_day < min_start_day)													// 日常累计充值阶段还没开启，完全不用理会
	{
		return;
	}
	// 这个角色的阶段已经不合法了，无需理会了
	if (m_activity_info.daily_total_chongzhi_stage > 0 &&							
		!LOGIC_CONFIG->GetChongzhiCfg().IsVaildDailyTotalChongzhiStage(m_activity_info.daily_total_chongzhi_stage)) 
	{
		return;
	}

	bool need_send = false;

	if (m_activity_info.daily_total_chongzhi_stage <= 0) // stage <= 0的情况下，仅仅需要设置stage的值
	{
		need_send = true;

		if (LOGIC_CONFIG->GetChongzhiCfg().IsValidDailyTotalChongzhiDay(open_day))
		{
			const ChongzhiDailyTotalStageConfig::ConfigItem *today_stage_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyTotalStageByDay(open_day);
			if (NULL != today_stage_cfg)				// 不可能为空
			{
				m_activity_info.daily_total_chongzhi_stage = today_stage_cfg->stage;
			}
		}
		else
		{
			m_activity_info.daily_total_chongzhi_stage = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiMaxStage() + 1;
		}
	}
	else
	{
		int today_stage = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiMaxStage() + 1;
		{
			if (LOGIC_CONFIG->GetChongzhiCfg().IsValidDailyTotalChongzhiDay(open_day))
			{
				const ChongzhiDailyTotalStageConfig::ConfigItem *today_stage_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyTotalStageByDay(open_day);
				if (NULL != today_stage_cfg)				// 不可能为空
				{
					today_stage = today_stage_cfg->stage;
				}
			}
		}

		if (today_stage > m_activity_info.daily_total_chongzhi_stage) // 阶段改变了，检查发奖励
		{
			need_send = true;

			const ChongzhiDailyTotalStageConfig::ConfigItem *old_stage_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyTotalStageByStage(m_activity_info.daily_total_chongzhi_stage);
			if (NULL != old_stage_cfg)
			{
				for (int i = 0; i < old_stage_cfg->total_recharge_reward_item_count; ++ i)
				{
					if (0 != (m_activity_info.daily_total_chongzhi_fetch_reward_flag & (1 << i)))
					{
						continue;
					}

					if (m_activity_info.daily_total_chongzhi_stage_chongzhi < old_stage_cfg->total_recharge_reward_need_chongzhi[i])
					{
						break;
					}

					const ItemConfigData &item_data = old_stage_cfg->total_recharge_reward_item_list[i];
					const ItemBase *item_base = ITEMPOOL->GetItem(item_data.item_id);

					if (NULL != item_base)
					{
						static MailContentParam contentparam; contentparam.Reset();
						contentparam.item_list[0].item_id = item_data.item_id;
						contentparam.item_list[0].num = item_data.num;
						contentparam.item_list[0].is_bind = (item_data.is_bind) ? 1 : 0;
						contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();	

						int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_chongzhi_daily_total_recharge_reward_content);
						if (length > 0)
						{
							//MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
							MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);

							m_activity_info.daily_total_chongzhi_fetch_reward_flag |= (1 << i);	

							gamelog::g_log_other.printf(LL_INFO, "CheckDailyTotalChongzhiStage user[%d, %s], level[%d], fetch_flag[%d], seq[%d], reward_item[%s]", 
								m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), m_activity_info.daily_total_chongzhi_fetch_reward_flag, 
								i, ItemConfigDataLog(&old_stage_cfg->total_recharge_reward_item_list[i], NULL));

							ROLE_LOG_QUICK6(LOG_TYPE_DAILY_TOTAL_CHONGZHI_REWARD, m_role, i, m_activity_info.daily_total_chongzhi_fetch_reward_flag, NULL, NULL);
						}
					}
				}
			}

			m_activity_info.daily_total_chongzhi_stage = today_stage;
			m_activity_info.daily_total_chongzhi_fetch_reward_flag = 0;
			m_activity_info.daily_total_chongzhi_stage_chongzhi = 0;
		}
	}

	if (need_send)
	{
		this->SendChongZhiInfo();
	}
}

void RoleActivity::FetchDiffWeekdayTotalChongzhiRewad(int stage)
{
	if (0 == m_activity_info.diff_wd_chongzhi_is_open)
	{
		return;
	}

	if (stage < 0 || stage >= DIFF_WD_CHONGZHI_MAX_STAGE)
	{
		return;
	}

	if (m_activity_info.diff_wd_chongzhi_stage_fetch_flag & (1 << stage))
	{
		m_role->NoticeNum(errornum::EN_DIFF_WEEKDAY_TOTAL_CHONGZHI_HAS_FETCH);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const DiffWeekdayChongzhiConfig::RewardItemCfg *cfg_item = LOGIC_CONFIG->GetChongzhiCfg().GetDiffWeekdayChongzhiCfg(EngineAdapter::Instance().LocalTime()->tm_wday, stage);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.diff_wd_chongzhi_value < cfg_item->need_chongzhi)
	{
		m_role->NoticeNum(errornum::EN_DIFF_WEEKDAY_TOTAL_CHONGZHI_NOT_ENOUGH);
		return;
	}

	m_activity_info.diff_wd_chongzhi_stage_fetch_flag |= (1 << stage);

	if (!m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_DIFF_WEEKDAY_CHONGZHI_REWARD))
	{
		return;
	}

	gamelog::g_log_other.printf(LL_INFO, "FetchDiffWeekdayTotalChongzhiRewad user[%d, %s], fetch_flag[%d], stage[%d], reward_item[%s]", 
		m_role->GetUID(), m_role->GetName(), m_activity_info.diff_wd_chongzhi_stage_fetch_flag, 
		stage, ItemConfigDataLog(&(cfg_item->reward_item), NULL));

	this->SendChongZhiInfo();
}

void RoleActivity::CheckDiffWeekdayTotalChongzhiRewrad()
{
	if (0 == m_activity_info.diff_wd_chongzhi_is_open)
	{
		return;
	}

	if (m_activity_info.diff_wd_chongzhi_value <= 0)
	{
		return;
	}

	int weekday = EngineAdapter::Instance().LocalTime()->tm_wday - 1;
	if (-1 == weekday)
	{
		weekday = 6;
	}

	const DiffWeekdayChongzhiConfig *cfg_item = LOGIC_CONFIG->GetChongzhiCfg().GetDiffWeekdayChongzhiCfgList(weekday);
	if (NULL == cfg_item)
	{
		return;
	}

	const int count = cfg_item->stage_cfg_count;

	for (int i = 0; i < count && i < DIFF_WD_CHONGZHI_MAX_STAGE; ++ i)
	{
		int is_fetched = m_activity_info.diff_wd_chongzhi_stage_fetch_flag & (1 << i);

		if (0 == is_fetched &&
			m_activity_info.diff_wd_chongzhi_value - cfg_item->stage_item_list[i].need_chongzhi >= 0)
		{
			m_activity_info.diff_wd_chongzhi_stage_fetch_flag |= (1 << i);

			static MailContentParam contentparam;
			contentparam.Reset();
			
			const ItemConfigData &item_data = cfg_item->stage_item_list[i].reward_item;
			
			contentparam.item_list[0].item_id = item_data.item_id;
			contentparam.item_list[0].num = item_data.num;
			contentparam.item_list[0].is_bind = (item_data.is_bind) ? 1 : 0;
			const ItemBase * item_base = ITEMPOOL->GetItem(item_data.item_id);
			if (NULL != item_base)
			{
				contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_chongzhi_diff_weeekday_chongzhi_reward_content);
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_other.printf(LL_INFO, "CheckDiffWeekdayTotalChongzhiRewrad user[%d, %s], fetch_flag[%d], weekday[%d]", 
					m_role->GetUID(), m_role->GetName(), m_activity_info.diff_wd_chongzhi_stage_fetch_flag, weekday);
			}
		}
	}
}

void RoleActivity::FetchFirstChongzhiReward(int level, int seq)
{
	if (seq < 0 || seq >= LOGIC_CONFIG->GetChongzhiCfg().GetFirstChongzhiFirstMaxCount())
	{
		return;
	}

	if (0 == (m_activity_info.first_chongzhi_active_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_COND_LIMIT);
		return;
	}

	if (0 != (m_activity_info.first_chongzhi_fetch_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_HAS_FETCH);
		return;
	}

	const FirstChongzhiRewradConfig *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetFirstChongzhiRewradConfig(seq);
	if (NULL == cfg)
	{
		return;
	}

	if (cfg->first_reward_item.item_id <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(cfg->first_reward_item, PUT_REASON_CHONGZHI_REWARD))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.first_chongzhi_fetch_reward_flag |= (1 << seq);
	
	gamelog::g_log_roleactivity.printf(LL_INFO, "FetchFirstChongzhiReward user[%d, %s], level[%d], reward_flag[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), (int)m_activity_info.first_chongzhi_fetch_reward_flag);

	this->SendChongZhiInfo();
	m_role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_DAY_FIRST_CHONGZHI, seq);
}

void RoleActivity::FetchDailyChongzhiReward(int seq, int reward_index)
{
	if (m_activity_info.first_chongzhi_active_flag <= 0)
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_COND_LIMIT);
		return;
	}

	const DailyChongzhiRewradConfig *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiRewradConfig(m_role->GetLevel(), seq);
	if (NULL == cfg)
	{
		return;
	}

	if (m_activity_info.daily_activity_chongzhi < cfg->need_total_chongzhi)
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_COND_LIMIT);
		return;
	}

	if (m_activity_info.daily_chongzhi_fetch_reward_flag & (1 << seq))
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_HAS_FETCH);
		return;
	}

	if (cfg->reward_item.item_id <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(cfg->reward_item, PUT_REASON_CHONGZHI_REWARD))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.daily_chongzhi_fetch_reward_flag |= (1 << seq);

	gamelog::g_log_roleactivity.printf(LL_INFO, "FetchDailyChongzhiReward user[%d, %s], level[%d], reward_flag[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), (int)m_activity_info.daily_chongzhi_fetch_reward_flag);

	this->SendChongZhiInfo();
}

void RoleActivity::FetchDailyChongzhiTimesReward(int seq)
{
	const DailyChongzhiTimesRewradConfig *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiTimesRewradConfig(seq);
	if (NULL == cfg)
	{
		return;
	}

	if (m_activity_info.daily_chongzhi_complete_days < cfg->complete_days)
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_COND_LIMIT);
		return;
	}

	if (m_activity_info.daily_chongzhi_times_fetch_reward_flag & (1 << seq))
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_HAS_FETCH);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(cfg->reward_item, PUT_REASON_CHONGZHI_REWARD))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.daily_chongzhi_times_fetch_reward_flag |= (1 << seq);

	int daily_chongzhi_times_max_day = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiTimesMaxDay();
	if (m_activity_info.daily_chongzhi_complete_days >= daily_chongzhi_times_max_day)
	{
		bool is_total_fetch = true;
		for (int i = 0; i < daily_chongzhi_times_max_day; i++)
		{
			const DailyChongzhiTimesRewradConfig *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiTimesRewradConfig(i);
			if (NULL == cfg)
			{
				break;
			}

			if ((m_activity_info.daily_chongzhi_times_fetch_reward_flag & (1 << i)) == 0)
			{
				is_total_fetch = false;
			}
		}

		if (is_total_fetch)
		{
			m_activity_info.daily_chongzhi_complete_days -= daily_chongzhi_times_max_day;
			m_activity_info.daily_chongzhi_times_fetch_reward_flag = 0;
		}
	}

	gamelog::g_log_roleactivity.printf(LL_INFO, "FetchDailyChongzhiTimesReward user[%d, %s], level[%d], reward_flag[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), (int)m_activity_info.daily_chongzhi_times_fetch_reward_flag);

	this->SendChongZhiInfo();
}

void RoleActivity::FetchChongzhi7DayReward()
{
	if (m_activity_info.chongzhi_7day_reward_timestamp <= 0)
	{
		return;
	}

	if (0 != m_activity_info.chongzhi_7day_reward_is_fetch)
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_7DAY_TODAY_FETCH);
		return;
	}

	const ChongzhiOtherConfig &other_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetOtherCfg();

	m_activity_info.chongzhi_7day_reward_fetch_day++;
	m_activity_info.chongzhi_7day_reward_is_fetch = 1;

	// 领完重置
	/*if (m_activity_info.chongzhi_7day_reward_fetch_day >= other_cfg.chongzhi_seven_day_reward_day)
	{
		m_activity_info.chongzhi_7day_reward_timestamp = 0;
		m_activity_info.chongzhi_7day_reward_fetch_day = 0;
		m_activity_info.chongzhi_7day_reward_is_fetch = 0;
	}*/

	m_role->GetKnapsack()->GetMoney()->AddGoldBind(other_cfg.chongzhi_seven_day_reward_bind_gold, "Chongzhi7Day");

	this->SendChongZhiInfo();
}

void RoleActivity::FetchDailyChongzhiReward2(int seq)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		m_role->NoticeNum(errornum::EN_HIDDEN_SERVER_OPERA_ERROR);
		return;
	}

	const DailyChongzhiRewrad2Config *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDailyChongzhiRewrad2Config(m_role->GetLevel(), seq);
	if (NULL == cfg)
	{
		return;
	}

	if (m_activity_info.day_chongzhi < cfg->need_chongzhi)
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_COND_LIMIT);
		return;
	}

	if (m_activity_info.daily_chongzhi_fetch_reward2_flag & (1 << seq))
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_HAS_FETCH);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg->reward_item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_CHONGZHI_REWARD))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.daily_chongzhi_fetch_reward2_flag |= (1 << seq);

	{
		// 传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chongzhi_daily_total_recharge_send_to_world,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), cfg->need_chongzhi);
		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
		}
	}

	gamelog::g_log_roleactivity.printf(LL_INFO, "FetchDailyChongzhiReward2 user[%d, %s], level[%d], reward_flag[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), (int)m_activity_info.daily_chongzhi_fetch_reward2_flag);

	this->SendChongZhiInfo();
	m_role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_DAY_TOTOAL_CHONGZHI, seq);
}

void RoleActivity::SendCollectExchangeInfo()
{
	Protocol::SCCollectExchangeInfo cmd;

	for (int i = 0; i < RAND_ACTIVITY_ITEM_COLLECTION_REWARD_MAX_COUNT; i++)
	{
		cmd.collection_exchange_times[i] = m_activity_info.collection_exchange_times[i];
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}

// --------------------------------------------------------------------------------------------------------
// 其他
// --------------------------------------------------------------------------------------------------------
void RoleActivity::SendServerTime()
{
	Protocol::SCTimeAck ta;
	ta.server_time = (unsigned int)EngineAdapter::Instance().Time();

	if (m_role->IsInCross())
	{
		const UniqueServerID unique_server_id = m_role->GetRoleCross()->GetOriginalUniqueServerID();
		ta.server_real_start_time = CrossManager::Instance().GetGameServerStartTime(unique_server_id);
		ta.server_real_combine_time = CrossManager::Instance().GetGameServerCombineTime(unique_server_id);
		ta.open_days = CrossManager::Instance().GetGameServerDayIndex(unique_server_id) + 1;
	}
	else
	{
		ta.server_real_start_time = (unsigned int)SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime();
		ta.server_real_combine_time = (unsigned int)SHAREDCONFIG->GetOpenServerConfig().GetServerRealCombineTime();
		ta.open_days = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&ta, sizeof(Protocol::SCTimeAck));
}

void RoleActivity::SendUpdateNoticeInfo()
{
	Protocol::SCUpdateNoticeInfo uni;
	uni.server_version = LOGIC_CONFIG->GetUpdateNoticeCfg().GetOtherCfg().version;
	uni.fetch_reward_version = m_activity_info.update_notice_fetch_reward_version;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&uni, sizeof(uni));
}

void RoleActivity::OnFetchUpdateNoticeReward()
{
	int server_version = LOGIC_CONFIG->GetUpdateNoticeCfg().GetOtherCfg().version;

	if (m_activity_info.update_notice_fetch_reward_version >= server_version)
	{
		return;
	}
	
	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.update_notice_fetch_reward_version = server_version;

	m_role->GetKnapsack()->Put(LOGIC_CONFIG->GetUpdateNoticeCfg().GetOtherCfg().reward_item, PUT_REASON_UPDATE_NOTICE);

	this->SendUpdateNoticeInfo();
}

void RoleActivity::OnConsumeGold(long long consume_gold)
{
	m_activity_info.day_consume_gold += (int)consume_gold;


	{
		// 随机活动
		this->OnRADayConsumeGoldAddConsume(consume_gold);
		this->OnRATotalConsumeGoldAddConsume(consume_gold);

		this->OnRAConsumeGoldRankConsumeGold((int)consume_gold);
		this->OnRAConsumeGoldFanliConsumeGold((int)consume_gold);
		this->OnRADayConsumeRankConsumeGold((int)consume_gold);
		this->OnRAContinueConsumeConsumeGold((int)consume_gold);
		this->OnRARedEnvelopeGiftConsumeGold((int)consume_gold);
		this->OnRAConsumeGiftConsumeGold((int)consume_gold);
		this->OnRAConsumeGoldRewardConsumeGold((int)consume_gold);
		this->OnRAConsumeForGiftConsumeGold((int)consume_gold);
	}

	{
		// 合服活动
		if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
		{
			if (ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_CONSUME_RANK))
			{
				m_activity_info.csa_consume_rank_consume_gold += static_cast<int>(consume_gold);

				int rank_type = LOGIC_CONFIG->GetCombineServerActivityConfig().GetRelatedRankType(CSA_SUB_TYPE_CONSUME_RANK);

				CombineServerActivityManager::Instance().OnCombineServerActivitySyncValue(rank_type, m_role->GetUID(),
					m_activity_info.csa_consume_rank_consume_gold, static_cast<long long>(LLONG_MAX - EngineAdapter::Instance().Time()));
			}
		}
	}
}

void RoleActivity::OnConsumeCoin(long long consume_coin)
{

}

void RoleActivity::OnMentalityGenguLevelUp()
{
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MENTALITY_TOTAL_LEVEL))
	{
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MENTALITY_TOTAL_LEVEL);
		this->CheckRAMentalityTotalLevelReward();
	}
}

void RoleActivity::OnMentalityBaseLevelUp()
{
	
}

void RoleActivity::OnPutonEquip()
{
	
}

void RoleActivity::OnStrengthenEquip(int equip_index, int old_level, int to_level)
{
	if (to_level > old_level)
	{
		this->CheckRAEquipStrengthenReward(to_level);
	}

	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_STRENGTHEN]++;
	if (m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_STRENGTHEN] > 100)
	{
		m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_STRENGTHEN] = 100;
	}
}

void RoleActivity::OnChestShopXunBao(int count, bool is_free)
{
	this->RAChestshopAddXunbaoTimes(count);
}

void RoleActivity::OnStoneUplevel(int stone_slot, int old_level, int to_level)
{
	if (to_level > old_level)
	{
		this->SendOpenGameActivityInfo();
		this->CheckRAStoneUplevelReward();
	}
}

void RoleActivity::OnStrengthenMount(int strength_level)
{
	
}

void RoleActivity::OnStrengthenShiZhuang(int strength_level)
{
	
}

void RoleActivity::OnEquipUpLevel()
{
	
}

void RoleActivity::OnRoleUpgradeLevel(int ole_level, int level)
{
	{
		// 封测活动
		if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_CLOSEDBETA))
		{
			const CloseBetaVipLevelCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCloseBetaCfg().GetVipLevelCfg(level);
			if (NULL != cfg_item)
			{
				if (m_role->GetVip()->GetVipLevel() < cfg_item->vip_level)
				{
					m_role->GetVip()->SetVipLevel(cfg_item->vip_level);
				}
			}
		}
	}

	this->OnRABipinSendForenotice();
}

void RoleActivity::OnActiveDegreeComplete(int active_degree_type)
{
	this->OnRADayActiveDegreeActiveDegreeChange();
}

void RoleActivity::OnKillWorldBoss(Role *role)
{
	{
		// 随机活动
		this->OnRAKillBossCountChange();
	}
}

void RoleActivity::OnKillBoss(int boss_id)
{
	int boss_seq = LOGIC_CONFIG->GetOpenGameActivityCfg().GetKillBossSeq(boss_id);
	if (0 < boss_seq && boss_seq < OGA_BOSS_MAX_SEQ)
	{
		if (0 == (m_activity_info.oga_kill_boss_flag & (static_cast<long long>(1) << boss_seq)))
		{
			m_activity_info.oga_kill_boss_flag |= (static_cast<long long>(1) << boss_seq);
			this->SendRAOpenServerBossInfo();
		}
	}
}

void RoleActivity::OnCombineServerKillBossActivity(int boss_id, int boss_level)
{
	if (boss_id <= 0 || boss_level <= 0)
	{
		return;
	}

	// 在跨服
	if (CrossConfig::Instance().IsHiddenServer())
	{
		const CSAKillBossCfg *kill_boss_cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetKillBossCfg(boss_id);
		if (nullptr != kill_boss_cfg)
		{
			if (boss_level + kill_boss_cfg->delta_lv_limit > m_role->GetLevel())
			{
				this->OnAddCombineKillBossCountInHidden();
			}
		}

		return;
	}

	// 合服活动
	if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		if (ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_KILL_BOSS))	
		{
			const CSAKillBossCfg *kill_boss_cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetKillBossCfg(boss_id);
			if (nullptr != kill_boss_cfg)
			{
				if (boss_level + kill_boss_cfg->delta_lv_limit > m_role->GetLevel())
				{
					++m_activity_info.csa_kill_boss_kill_count;
					this->SendCSARoleInfo();
				}
			}
		}
	}
}

void RoleActivity::OnAddCombineKillBossCountInHidden()
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(m_role->GetRoleCross()->GetCrossOriginalUniqueUserID());

	crossgameprotocal::GameCrossWorldCommonDataChange cross_msg;

	cross_msg.uuid = uuid;
	cross_msg.change_type = crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_COMBINE_KILL_BOSS;
	cross_msg.param_2 = 1;

	InternalComm::Instance().SendToCross((const char*)&cross_msg, sizeof(cross_msg));
}

void RoleActivity::OnSyncCombineKillBossCount(int count)
{
	if (count <= 0) return;

	if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		if (ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_KILL_BOSS))
		{
			m_activity_info.csa_kill_boss_kill_count += count;
			this->SendCSARoleInfo();
		}
	}
}

void RoleActivity::OnGiveFlower(int flower_num)
{
	this->OnRAShouYouYuXiangGiveFlower(flower_num);
}


void RoleActivity::OnChangeCapability(int cap_type)
{
	this->OnRACappabilityChange(cap_type);
}

void RoleActivity::SendRandActivityInfo()
{
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MAGIC_SHOP))
	{
		this->SendRAMagicShopAllInfo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_CRAZY_REBATE))
	{
		this->SendRaCrazyRebateChongzhiInfo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAILY_LOVE))
	{
		this->OnSendDailyLoveChongzhiInfo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI))
	{
		this->SendRADoubleChongzhiInfo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2))
	{
		this->SendCollectSecondExchangeInfo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2))
	{
		this->SendRAHuanLeYaoJiang2Info();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0))
	{
		this->SendRAOfflineSingleChargeInfo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE))
	{
		this->SendRABuyOneGetOneFreeInfo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI))
	{
		m_activity_info.ra_reset_double_chongzhi_open_flag = 1;
		this->SendRADoubleChongzhiInfo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT))
	{
		this->SendRAChongZhiGiftAllInfo();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SUPPER_GIFT_2))
	{
		this->SendOpenGameGiftShopBuy2Info();
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAILY_LOVE_2))
	{
		this->OnSendDailyLove2ChongzhiInfo();
	}

	this->SendRAConsumeGoldRankInfo();

	this->SendDayChongzhiRewardInfo();

	// 特殊处理，合服投资计划在活动结束后还可以领取，不做限制
	this->SendCSATouzijihuaInfo();
}

// add by wwd
void RoleActivity::CheckRandActivity(int rand_act_type, bool is_force)
{
	CheckInitRandActivity(rand_act_type, is_force);
}

void RoleActivity::SyncFishingGearNumList(int gear_num_list[FISHING_GEAR_MAX_COUNT], bool is_init)
{
	if (NULL == gear_num_list)
	{
		return;
	}

	if (is_init)
	{
		memcpy(gear_num_list, m_activity_info.fish_gear_num_list, sizeof(gear_num_list[0]) * FISHING_GEAR_MAX_COUNT);
	}
	else
	{
		memcpy(m_activity_info.fish_gear_num_list, gear_num_list, sizeof(m_activity_info.fish_gear_num_list));
	}
}

int RoleActivity::GetFishingScore()
{
	return m_activity_info.fish_score;
}

void RoleActivity::AddFishingScore(int score)
{
	if (score > 0)
	{
		m_activity_info.fish_score += score;

		this->SendFishingScoreInfo();

		gamelog::g_log_fishing.printf(LL_INFO, "AddFishingScore user[%d %s], add_scrore[%d], total_score[%d]",
			m_role->GetUID(), m_role->GetName(), score, m_activity_info.fish_score);
	}
}

bool RoleActivity::DecFishingScore(int score)
{
	if (score > 0)
	{
		m_activity_info.fish_score -= score;
		if (m_activity_info.fish_score < 0)
		{
			m_activity_info.fish_score = 0;
		}

		this->SendFishingScoreInfo();

		gamelog::g_log_fishing.printf(LL_INFO, "DecFishingScore user[%d %s], add_scrore[%d], total_score[%d]",
			m_role->GetUID(), m_role->GetName(), score, m_activity_info.fish_score);
	}

	return true;
}

void RoleActivity::SendFishingScoreInfo()
{
	Protocol::SCFishingScoreInfo fsi;
	fsi.fishing_score = m_activity_info.fish_score;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&fsi, sizeof(fsi));
}

void RoleActivity::OnNightFightEnter()
{
	if (NULL == m_role)
	{
		return;
	}

	const NightFightFbOtherConfig &other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();

	const int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
	const int open_cross_day = other_cfg.open_cross_begin_day;

	// 根据天数检测
	if (opengame_day < open_cross_day)
	{
		if (ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_NIGHT_FIGHT_FB))
		{
			m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
			return;
		}
	}
	else
	{
		if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB))
		{
			m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
			return;
		}
	}

	// 检查等级

	if (m_role->GetLevel() < other_cfg.limit_level)
	{
		m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_FB_LEVEL_LIMIT);
		return;
	}

	// 根据天数检测
	if (opengame_day < open_cross_day)
	{
		int scene_key = 0;
		DoubleSideFBManager::Instance().RequireEnterFb(m_role, ACTIVITY_TYPE_NIGHT_FIGHT_FB, false, scene_key);
	}
	else
	{
		m_role->GetRoleCross()->OnStartCrossReq(CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB);
	}
}

void RoleActivity::ResetShuijingData()
{
	m_activity_info.shuijing_gather_count = 0;
	m_activity_info.shuijing_free_relive_count = 0;
	m_activity_info.shuijing_day_bind_gold = 0;
	m_activity_info.shuijing_day_mojing = 0;
	m_activity_info.shuijing_day_shengwang = 0;
	SpecialShuiJing *shuijing_logic = dynamic_cast<SpecialShuiJing*>(m_role->GetScene()->GetSpecialLogic());
	if (NULL != shuijing_logic && Scene::SCENE_TYPE_SHUIJING == m_role->GetScene()->GetSceneType())
	{
		shuijing_logic->SendPlayerInfoToPlayer(m_role);
	}
}

void RoleActivity::OnCombiendServerActivityDayChange()
{
	if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		//合服boss
		if (ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_BOSS))
		{
			m_activity_info.csa_boss_daily_kill_count = 0;
		}
	}

	// 投资
	{
		++m_activity_info.csa_touzjihua_login_day;
		this->SendCSATouzijihuaInfo();
	}

	// 活动结束时
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (!ActivityShadow::Instance().IsInSpecialActivityOpenTime(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER, now_second) && !CrossConfig::Instance().IsHiddenServer())
	{
		this->CheckGiveCombineReward();
	}
}

void RoleActivity::SendRAOpenServerSingleInfo(int ra_type)
{
	int sub_index = ra_type - RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN;
	if (sub_index < 0 || sub_index >= OPEN_SERVER_ACTIVITY_COUNT)
	{
		return;
	}

	OpenServerSubActivityInfo &info = m_activity_info.open_server_sub_info_list[sub_index];

	Protocol::SCRAOpenServerInfo pro;
	pro.rand_activity_type = ra_type;
	pro.reward_flag = info.reward_flag;
	pro.complete_flag = 0;
	//pro.today_chongzhi_role_count = WorldStatus::Instance().GetTodayFirstChongzhiCount();
	if (WorldStatus::Instance().GetTodayFirstChongzhiCount() < WorldStatus::Instance().GetTodayFirstChongzhiFakeCount())	//策划需求，实际充值小于假人数显示假人数
	{
		pro.today_chongzhi_role_count = WorldStatus::Instance().GetTodayFirstChongzhiFakeCount();
	}
	else
	{
		pro.today_chongzhi_role_count = WorldStatus::Instance().GetTodayFirstChongzhiCount();
	}

	for (int i = 1; i < RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX; i++)
	{
		const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(ra_type, i);
		if (NULL == cfg)
		{
			break;
		}

		if (this->RACheckOpenServerFetchCond(ra_type, cfg))
		{
			pro.complete_flag |= (1 << i);
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void RoleActivity::OnMessBattleEnter()
{
	if (NULL == m_role)
	{
		return;
	}

	const MessBattleFbOtherConfig& other_cfg =LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();
	const int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
	const int open_cross_day = other_cfg.open_cross_begin_day;

	// 根据天数检测
	if (opengame_day < open_cross_day)
	{
		if (ActivityShadow::Instance().IsActivtyClose(ACTIVITY_TYPE_MESS_BATTLE_FB))
		{
			m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
			return;
		}
	}
	else
	{
		if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB))
		{
			m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
			return;
		}
	}

	if (m_role->GetLevel() < other_cfg.limit_level)
	{
		m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_FB_LEVEL_LIMIT);
		return;
	}

	//增加嗨点
	//EventHandler::Instance().OnFinishHighMission(m_role, REWARD_ENTER_IN_LUANDOU_ZHANCHANG_FB);

	// 根据天数检测
	if (opengame_day < open_cross_day)
	{
		int scene_key = 0;
		DoubleSideFBManager::Instance().RequireEnterFb(m_role, ACTIVITY_TYPE_MESS_BATTLE_FB, false, scene_key);
	}
	else
	{
		m_role->GetRoleCross()->OnStartCrossReq(CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB);
	}
}

void RoleActivity::CheckGiveCombineReward()
{
	this->CheckTouzijihuaFetch();
	this->CheckFoundationFetch();
	this->CheckCSALoginGiftGiveReward();
}

//完美情人
bool RoleActivity::OnRAPerfectMarry(int marry_type, int lover_uid)
{
	if (marry_type < 0 || marry_type >= QingyuanHunyanConfig::CONFIG_ITEM_MAX_COUNT)
	{
		return false;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PERFECT_LOVER))
	{
		return false;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_PERFECT_LOVER);

	if (0 != m_activity_info.ra_perfect_lover_type_fetch_flag)
	{
		return false;
	}

	if (lover_uid != m_activity_info.ra_perfect_lover_last_marry_role_id)
	{
		m_activity_info.ra_perfect_lover_type_record_flag = 0;
		m_activity_info.ra_perfect_lover_last_marry_role_id = lover_uid;
	}

	if (0 != (m_activity_info.ra_perfect_lover_type_record_flag & (1 << marry_type)))
	{
		return false;
	}

	m_activity_info.ra_perfect_lover_type_record_flag |= (1 << marry_type);

	bool is_all_finish = true;
//	int type_num = LOGIC_CONFIG->GetQingyuanCfg().GetMarryTypeCount();

	const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(RAND_ACTIVITY_TYPE_PERFECT_LOVER, 1);
	if (NULL == cfg)
	{
		return false;
	}

	int marry_count = 0;
	for (int type = 0; type < QingyuanHunyanConfig::CONFIG_ITEM_MAX_COUNT; type++)
	{
		if (0 != (m_activity_info.ra_perfect_lover_type_record_flag & (1 << type)))
		{
// 			is_all_finish = false;
// 			break;
			marry_count++;
		}
	}

	if (marry_count < cfg->cond1)
	{
		is_all_finish = false;
	}

	if (is_all_finish)
	{
		//const ItemConfigData *item_data = LOGIC_CONFIG->GetRandActivityCfg().GetPerfectLoverReward();
		//if (NULL == item_data)
		//{
		//	return false;
		//}

		if (0 == cfg->reward_item_list[0].item_id)
		{
			return false;
		}

		const ItemBase *item_base = ITEMPOOL->GetItem(cfg->reward_item_list->item_id);
		if (NULL != item_base)
		{
			// 发邮件
			static MailContentParam contentparam; contentparam.Reset();
			contentparam.item_list[0].item_id = cfg->reward_item_list->item_id;
			contentparam.item_list[0].num = cfg->reward_item_list->num;
			contentparam.item_list[0].is_bind = (cfg->reward_item_list->is_bind) ? 1 : 0;
			contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

			RandActivityPerfectLover *rand_activity = RandActivityManager::Instance().GetRandActivityPerfectLover();
			if (rand_activity == NULL)
			{
				return false;
			}
			int rank = rand_activity->GetPerfectLoverCoupleNum() + 1;

			UserCacheNode *lover_role_node = UserCacheManager::Instance().GetUserNode(m_role->GetLoverUid());
			if (NULL == lover_role_node)
			{
				return false;
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_perfect_lover_reward_item_content, lover_role_node->uid, lover_role_node->user_name, (int)lover_role_node->camp, rank);
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}		
		}

		m_activity_info.ra_perfect_lover_type_fetch_flag = 1;
	}

	this->SendPerfectLoverInfo();

	{
		//log
		char log_quick_str[LOG_BASE_LENGTH] = { 0 };
		SNPRINTF(log_quick_str, LOG_BASE_LENGTH, "record_flag:%d, fetch_flag:%d, marry_role_id:%d", (int)m_activity_info.ra_perfect_lover_type_record_flag, (int)m_activity_info.ra_perfect_lover_type_fetch_flag, lover_uid);
		ROLE_LOG_QUICK6(LOG_TYPE_RANDACTIVITY_TOTAL_DATA, m_role, RAND_ACTIVITY_TYPE_PERFECT_LOVER, 0, log_quick_str, NULL);
	}

	return is_all_finish;
}

void RoleActivity::PerfectLoverCheckAndSendInfo()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_PERFECT_LOVER);
	this->SendPerfectLoverInfo();
}


void RoleActivity::Brocast()
{
	UserCacheNode *lover_role_node = UserCacheManager::Instance().GetUserNode(m_role->GetLoverUid());
	if (NULL == lover_role_node)
	{
		return;
	}
	RandActivityPerfectLover *rand_activity = RandActivityManager::Instance().GetRandActivityPerfectLover();
	if (rand_activity == NULL)
	{
		return;
	}

	const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(RAND_ACTIVITY_TYPE_PERFECT_LOVER, 1);
	if (NULL == cfg)
	{
		return;
	}

	int rank = rand_activity->GetPerfectLoverCoupleNum() + 1;
	// 传闻
	{
		int send_len = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "rand_act_perfect_lover_notice",
			m_role->GetName(), lover_role_node->user_name, rank, cfg->reward_item_list[0].item_id);
		if (send_len > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, send_len, SYS_MSG_CENTER_NOTICE);
		}
	}
}

void RoleActivity::SendPerfectLoverInfo()
{
	static Protocol::SCRAPerfectLoverInfo pli;
	pli.Reset();

	pli.perfect_lover_type_record_flag = m_activity_info.ra_perfect_lover_type_record_flag;
	RandActivityPerfectLover *rand_activity = RandActivityManager::Instance().GetRandActivityPerfectLover();
	if (rand_activity == NULL)
	{
		return;
	}

	UserCacheNode *user_cache_node = UserCacheManager::Instance().GetUserNode(m_role->GetLoverUid());
	if (NULL != user_cache_node)
	{
		F_STRNCPY(pli.lover_name, user_cache_node->user_name, sizeof(pli.lover_name));
	}

	int count = rand_activity->GetPerfectLoverCoupleNum();
	for (int i = 0; i < count * 2 && i < RAND_PERFECT_LOVER_MAX_RECORD_COUPLE_COUNT * 2; i++)
	{
		UserCacheNode *user_cache_node = UserCacheManager::Instance().GetUserNode(rand_activity->GetPerfectLoverIdByIndex(i));
		if (NULL == user_cache_node) continue;

		if (m_role->GetUID() == rand_activity->GetPerfectLoverIdByIndex(i))
		{
			pli.my_rank = (i + 1) % 2 > 0 ? (i + 1) / 2 + 1 : (i + 1) / 2;
		}

		F_STRNCPY(pli.ra_perfect_lover_name_list[i], user_cache_node->user_name, sizeof(pli.ra_perfect_lover_name_list[i]));
		++pli.ra_perfect_lover_count;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pli, sizeof(pli));
}

void RoleActivity::OnQuanMinJinJie(int opear_type, int seq)
{
	switch (opear_type)
	{
		case 0:
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_QUANMINJINJIE);
			this->SendQuanMinJinJieInfo(seq);
		}
		break;

		case 1:
		{
			if (!this->OnFetchUpgradeReward(seq))
			{
				m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_COND_LIMIT);
				return;
			}

			this->SendQuanMinJinJieInfo(seq);
		}
		break;
	}
}

bool RoleActivity::OnFetchUpgradeReward(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_QUANMINJINJIE))
	{
		return false;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_QUANMINJINJIE);

	if (0 != (m_activity_info.ra_upgrade_reward_flag & (1 << seq)))
	{
		return false;
	}

	//获得进阶类型
	const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(RAND_ACTIVITY_TYPE_QUANMINJINJIE, seq);
	if (NULL == cfg)
	{
		return false;
	}

	switch (cfg->cond1)
	{
		case Protocol::UPGRADE_TYPE_MOUNT:
		{
			MountParam *mount_param = m_role->GetMountManager()->GetMountParam();
			if (NULL != mount_param)
			{
				if (mount_param->grade >= cfg->cond2)
				{
					if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
					{
						m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
						return false;
					}

					m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
					return true;
				}
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_WING:
		{
			WingParam *wing_param = m_role->GetWing()->GetWingParam();
			if (NULL != wing_param)
			{
				if (wing_param->grade >= cfg->cond2)
				{
					if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
					{
						m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
						return false;
					}

					m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
					return true;
				}
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_SHIZHUANG_TYPE_BODY:
		{
			ShizhuangParam *shizhuang_param = m_role->GetShizhuang()->GetParam();
			if (NULL != shizhuang_param)
			{
				if (shizhuang_param->item_list[SHIZHUANG_TYPE_BODY].grade >= cfg->cond2)
				{
					if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
					{
						m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
						return false;
					}

					m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
					return true;
				}
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_SHIZHUANG_TYPE_WUQI:
		{
			ShizhuangParam *shizhuang_param = m_role->GetShizhuang()->GetParam();
			if (NULL != shizhuang_param)
			{
				if (shizhuang_param->item_list[SHIZHUANG_TYPE_WUQI].grade >= cfg->cond2)
				{
					if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
					{
						m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
						return false;
					}

					m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
					return true;
				}
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_FABAO:
		{
			FabaoParam *fabaoparam = m_role->GetFabaoManager()->GetParam();
			if (NULL != fabaoparam)
			{
				if (fabaoparam->grade >= cfg->cond2)
				{
					if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
					{
						m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
						return false;
					}

					m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
					return true;
				}
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_FOOTPRINT:
		{
			FootprintParam *footprint_param = m_role->GetFootPrint()->GetFootprintParam();
			if (NULL != footprint_param)
			{
				if (footprint_param->grade >= cfg->cond2)
				{
					if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
					{
						m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
						return false;
					}

					m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
					return true;
				}
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_HALO:
		{
			HaloParam *halo_param = m_role->GetHalo()->GetHaloParam();
			if (NULL != halo_param)
			{
				if (halo_param->grade >= cfg->cond2)
				{
					if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
					{
						m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
						return false;
					}

					m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
					return true;
				}
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_QILINBI:
		{
			QiLinBiParam *qilinbi_param = m_role->GetQiLinBiManager()->GetQiLinBiParam();
			if (NULL != qilinbi_param)
			{
				if (qilinbi_param->grade >= cfg->cond2)
				{
					if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
					{
						m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
						return false;
					}

					m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
					return true;
				}
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_SHENGONG:
		{
			ShengongParam *shengong_param = m_role->GetShengong()->GetShengongParam();
			if (NULL != shengong_param)
			{
				if (shengong_param->grade >= cfg->cond2)
				{
					if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
					{
						m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
						return false;
					}

					m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
					return true;
				}
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_TOUSHI:
		{
			TouShiParam *toushi_param = m_role->GetTouShiManager()->GetTouShiParam();
			if (NULL != toushi_param)
			{
				if (toushi_param->grade >= cfg->cond2)
				{
					if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
					{
						m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
						return false;
					}

					m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
					return true;
				}
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_YAOSHI:
		{
			YaoShiParam *yaoshi_param = m_role->GetYaoShiManager()->GetYaoShiParam();
			if (NULL != yaoshi_param)
			{
				if (yaoshi_param->grade >= cfg->cond2)
				{
					if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
					{
						m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
						return false;
					}

					m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
					return true;
				}
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_LINGTONG:
		{
			if (m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_2) >= cfg->cond2)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}

				m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
				return true;
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_LINGGONG:
		{
			if (m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_3) >= cfg->cond2)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}

				m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
				return true;
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_LINGQI:
		{
			if (m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_4) >= cfg->cond2)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}

				m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
				return true;
			}
		}
		break;

		case Protocol::UPGRADE_TYPE_FIGHT_MOUNT:
		{
			FightMountParam *fightmount_param = m_role->GetFightMountManager()->GetFightMountParam();
			if (NULL != fightmount_param)
			{
				if (fightmount_param->grade >= cfg->cond2)
				{
					if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
					{
						m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
						return false;
					}

					m_activity_info.ra_upgrade_reward_flag |= (1 << seq);
					return true;
				}
			}
		}
		break;
	}
	return false;
}

void RoleActivity::SendQuanMinJinJieInfo(int seq)
{
	{
		Protocol::SCQuanMinJinJieInfo quanminjinjie_info;
		quanminjinjie_info.grade = this->GetPersonalGrade();
		//quanminjinjie_info.reward_flag = m_activity_info.ra_upgrade_reward_flag;
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&quanminjinjie_info, sizeof(quanminjinjie_info));
	}

	//客户端排序需要
	{
		Protocol::SCRAOpenServerInfo reward_info;
		reward_info.complete_flag = 0;
		reward_info.rand_activity_type = RAND_ACTIVITY_TYPE_QUANMINJINJIE;
		for (int i = 1; i < RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX; i++)
		{
			const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(RAND_ACTIVITY_TYPE_QUANMINJINJIE, i);
			if (NULL == cfg)
			{
				break;
			}

			if (this->RACheckOpenServerFetchCond(RAND_ACTIVITY_TYPE_QUANMINJINJIE, cfg))
			{
				reward_info.complete_flag |= (1 << i);
			}
		}
		reward_info.reward_flag = m_activity_info.ra_upgrade_reward_flag;
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&reward_info, sizeof(reward_info));
	}
}

int RoleActivity::GetPersonalGrade()
{
	//获得进阶类型(同一个开服天数下只能有一种进阶活动类型)
	const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(RAND_ACTIVITY_TYPE_QUANMINJINJIE, 1);
	if (NULL == cfg)
	{
		return 0;
	}

	int grade = 0;

	switch (cfg->cond1)
	{
	case Protocol::UPGRADE_TYPE_MOUNT:
	{
		MountParam *mount_param = m_role->GetMountManager()->GetMountParam();
		if (NULL != mount_param)
		{
			if (mount_param->grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
			{
				return grade;
			}
			else
			{
				grade = mount_param->grade;
				return grade;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_WING:
	{
		WingParam *wing_param = m_role->GetWing()->GetWingParam();
		if (NULL != wing_param)
		{
			if (wing_param->grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
			{
				return grade;
			}
			else
			{
				grade = wing_param->grade;
				return grade;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_SHIZHUANG_TYPE_BODY:
	{
		ShizhuangParam *shizhuang_param = m_role->GetShizhuang()->GetParam();
		if (NULL != shizhuang_param)
		{
			if (shizhuang_param->item_list[SHIZHUANG_TYPE_BODY].grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
			{
				return grade;
			}
			else
			{
				grade = shizhuang_param->item_list[SHIZHUANG_TYPE_BODY].grade;
				return grade;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_SHIZHUANG_TYPE_WUQI:
	{
		ShizhuangParam *shizhuang_param = m_role->GetShizhuang()->GetParam();
		if (NULL != shizhuang_param)
		{
			if (shizhuang_param->item_list[SHIZHUANG_TYPE_WUQI].grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
			{
				return grade;
			}
			else
			{
				grade = shizhuang_param->item_list[SHIZHUANG_TYPE_WUQI].grade;
				return grade;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_FABAO:
	{
		FabaoParam *fabao_param = m_role->GetFabaoManager()->GetParam();
		if (NULL != fabao_param)
		{
			if (fabao_param->grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
			{
				return grade;
			}
			else
			{
				grade = fabao_param->grade;
				return grade;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_FOOTPRINT:
	{
		FootprintParam *footprint_param = m_role->GetFootPrint()->GetFootprintParam();
		if (NULL != footprint_param)
		{
			if (footprint_param->grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
			{
				return grade;
			}
			else
			{
				grade = footprint_param->grade;
				return grade;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_HALO:
	{
		HaloParam *halo_param = m_role->GetHalo()->GetHaloParam();
		if (NULL != halo_param)
		{
			if (halo_param->grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
			{
				return grade;
			}
			else
			{
				grade = halo_param->grade;
				return grade;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_QILINBI:
	{
		QiLinBiParam *qilinbi_param = m_role->GetQiLinBiManager()->GetQiLinBiParam();
		if (NULL != qilinbi_param)
		{
			if (qilinbi_param->grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
			{
				return grade;
			}
			else
			{
				grade = qilinbi_param->grade;
				return grade;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_SHENGONG:
	{
		ShengongParam *shengong_param = m_role->GetShengong()->GetShengongParam();
		if (NULL != shengong_param)
		{
			if (shengong_param->grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
			{
				return grade;
			}
			else
			{
				grade = shengong_param->grade;
				return grade;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_TOUSHI:
	{
		TouShiParam *toushi_param = m_role->GetTouShiManager()->GetTouShiParam();
		if (NULL != toushi_param)
		{
			if (toushi_param->grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
			{
				return grade;
			}
			else
			{
				grade = toushi_param->grade;
				return grade;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_YAOSHI:
	{
		YaoShiParam *yaoshi_param = m_role->GetYaoShiManager()->GetYaoShiParam();
		if (NULL != yaoshi_param)
		{
			if (yaoshi_param->grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
			{
				return grade;
			}
			else
			{
				grade = yaoshi_param->grade;
				return grade;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_LINGTONG:
	{
		int upgrade_grade = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_2);
		if (upgrade_grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
		{
			return grade;
		}
		else
		{
			grade = upgrade_grade;
			return grade;
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_LINGGONG:
	{
		int upgrade_grade = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_3);
		if (upgrade_grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
		{
			return grade;
		}
		else
		{
			grade = upgrade_grade;
			return grade;
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_LINGQI:
	{
		int upgrade_grade = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_4);
		if (upgrade_grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
		{
			return grade;
		}
		else
		{
			grade = upgrade_grade;
			return grade;
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_FIGHT_MOUNT:
	{
		FightMountParam *fightmount_param = m_role->GetFightMountManager()->GetFightMountParam();
		if (NULL != fightmount_param)
		{
			if (fightmount_param->grade < 2)		//特殊处理，客户端显示的阶数与服务端阶数有偏移量
			{
				return grade;
			}
			else
			{
				grade = fightmount_param->grade;
				return grade;
			}
		}
	}
	break;

	}

	return 0;
}

void RoleActivity::OnUpgradeGroupe(int opear_type, int seq)
{
	switch (opear_type)
	{
		case 0:
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN);
			this->SendUpgradeGroupeInfo(seq);
		}
		break;

		case 1:
		{
			if (!this->OnFetchUpgradeGroupeReward(seq))
			{
				m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_COND_LIMIT);
				return;
			}

			this->SendUpgradeGroupeInfo(seq);
		}
		break;
	}
}

bool RoleActivity::OnFetchUpgradeGroupeReward(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN))
	{
		return false;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN);

	if (0 != (m_activity_info.ra_upgrade_group_reward_flag & (1 << seq)))
	{
		return false;
	}

	//获得进阶类型
	const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN, seq);
	if (NULL == cfg)
	{
		return false;
	}

	switch (cfg->cond1)
	{
	case Protocol::UPGRADE_TYPE_MOUNT:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus::Instance().GetTotalUpgradeMountRecordList(cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_WING:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus::Instance().GetTotalUpgradeWingRecordList(cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_SHIZHUANG_TYPE_BODY:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeShizhuangRecordList(cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_SHIZHUANG_TYPE_WUQI:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeShenbingRecordList(cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_FABAO:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeFabaoRecordList(cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_FOOTPRINT:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeFootprintRecordList(cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_HALO:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus::Instance().GetTotalUpgradeHaloRecordList(cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_QILINBI:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeQilinbiRecordList(cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_SHENGONG:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus::Instance().GetTotalUpgradeShengongRecordList(cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_TOUSHI:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeToushiRecordList(cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_YAOSHI:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeYaoshiRecordList(cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_LINGTONG:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeRecordList(UPGRADE_TYPE_2, cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_LINGGONG:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeRecordList(UPGRADE_TYPE_3, cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_LINGQI:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeRecordList(UPGRADE_TYPE_4, cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;

	case Protocol::UPGRADE_TYPE_FIGHT_MOUNT:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeFightMountRecordList(cfg->cond2 + 1) >= cfg->cond3)
			{
				if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_RA_UPGRADE_REWARD))
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					return false;
				}
				m_activity_info.ra_upgrade_group_reward_flag |= (1 << seq);
				return true;
			}
		}
	}
	break;
	}
	return false;
}

void RoleActivity::SendUpgradeGroupeInfo(int seq)
{
	{
		Protocol::SCUpgradeGroupeInfo upgrade_group_info;
		const RandActivityOpenServerRewardConfig::RewardDayConfig *activity_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetOpenServerRewardConfigByTypeAndOGDay(RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN);
		if (NULL == activity_cfg)
		{
			return;
		}

		upgrade_group_info.ra_upgrade_group_reward_flag = 0;
		memset(upgrade_group_info.count, 0, sizeof(upgrade_group_info.count));

		switch (activity_cfg->item_list[1].cond1)	//配置里面seq从1开始，同一个开服天数下活动类型相同，故取item_list[1].cond1
		{
		case Protocol::UPGRADE_TYPE_MOUNT:
		{
			memcpy(upgrade_group_info.count, WorldStatus::Instance().GetCommonData().total_upgrade_mount_record_list, sizeof(upgrade_group_info.count));
		}
		break;

		case Protocol::UPGRADE_TYPE_WING:
		{
			memcpy(upgrade_group_info.count, WorldStatus::Instance().GetCommonData().total_upgrade_wing_record_list, sizeof(upgrade_group_info.count));
		}
		break;

		case Protocol::UPGRADE_TYPE_SHIZHUANG_TYPE_BODY:
		{
			memcpy(upgrade_group_info.count, WorldStatus2::Instance().GetCommonData2().total_upgrade_shizhuang_record_list, sizeof(upgrade_group_info.count));
		}
		break;

		case Protocol::UPGRADE_TYPE_SHIZHUANG_TYPE_WUQI:
		{
			memcpy(upgrade_group_info.count, WorldStatus2::Instance().GetCommonData2().total_upgrade_shenbing_record_list, sizeof(upgrade_group_info.count));
		}
		break;

		case Protocol::UPGRADE_TYPE_FABAO:
		{
			memcpy(upgrade_group_info.count, WorldStatus2::Instance().GetCommonData2().total_upgrade_fabao_record_list, sizeof(upgrade_group_info.count));
		}
		break;

		case Protocol::UPGRADE_TYPE_FOOTPRINT:
		{
			memcpy(upgrade_group_info.count, WorldStatus2::Instance().GetCommonData2().total_upgrade_footprint_record_list, sizeof(upgrade_group_info.count));
		}
		break;

		case Protocol::UPGRADE_TYPE_HALO:
		{
			memcpy(upgrade_group_info.count, WorldStatus::Instance().GetCommonData().total_upgrade_holo_record_list, sizeof(upgrade_group_info.count));
		}
		break;

		case Protocol::UPGRADE_TYPE_QILINBI:
		{
			memcpy(upgrade_group_info.count, WorldStatus2::Instance().GetCommonData2().total_upgrade_qilinbi_record_list, sizeof(upgrade_group_info.count));
		}
		break;

		case Protocol::UPGRADE_TYPE_SHENGONG:
		{
			memcpy(upgrade_group_info.count, WorldStatus::Instance().GetCommonData().total_upgrade_shengong_record_list, sizeof(upgrade_group_info.count));
		}
		break;

		case Protocol::UPGRADE_TYPE_TOUSHI:
		{
			memcpy(upgrade_group_info.count, WorldStatus2::Instance().GetCommonData2().total_upgrade_toushi_record_list, sizeof(upgrade_group_info.count));
		}
		break;

		case Protocol::UPGRADE_TYPE_YAOSHI:
		{
			memcpy(upgrade_group_info.count, WorldStatus2::Instance().GetCommonData2().total_upgrade_yaoshi_record_list, sizeof(upgrade_group_info.count));
		}
		break;

		case Protocol::UPGRADE_TYPE_LINGTONG:
		{
			memcpy(upgrade_group_info.count, WorldStatus2::Instance().GetCommonData2().total_upgrade_record_list[UPGRADE_TYPE_2], sizeof(upgrade_group_info.count));
		}
		break;
		case Protocol::UPGRADE_TYPE_LINGGONG:
		{
			memcpy(upgrade_group_info.count, WorldStatus2::Instance().GetCommonData2().total_upgrade_record_list[UPGRADE_TYPE_3], sizeof(upgrade_group_info.count));
		}
		break;
		case Protocol::UPGRADE_TYPE_LINGQI:
		{
			memcpy(upgrade_group_info.count, WorldStatus2::Instance().GetCommonData2().total_upgrade_record_list[UPGRADE_TYPE_4], sizeof(upgrade_group_info.count));
		}
		break;
		case Protocol::UPGRADE_TYPE_FIGHT_MOUNT:
		{
			memcpy(upgrade_group_info.count, WorldStatus2::Instance().GetCommonData2().total_upgrade_fight_mount_record_list, sizeof(upgrade_group_info.count));
		}
		break;
		}
		//upgrade_group_info.ra_upgrade_group_reward_flag = m_activity_info.ra_upgrade_group_reward_flag;
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&upgrade_group_info, sizeof(upgrade_group_info));
	}

	//客户端排序需要
	{
		Protocol::SCRAOpenServerInfo reward_info;
		reward_info.complete_flag = 0;

		for (int i = 1; i < RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX; i++)
		{
			const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN, i);
			if (NULL == cfg)
			{
				break;
			}

			if (this->RACheckOpenServerFetchCond(RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN, cfg))
			{
				reward_info.complete_flag |= (1 << i);
			}
		}
		reward_info.rand_activity_type = RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN;
		reward_info.reward_flag = m_activity_info.ra_upgrade_group_reward_flag;

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&reward_info, sizeof(reward_info));
	}
}

void RoleActivity::OnCheckCrossConsumeGold(long long consume_gold)
{
	{
		// 随机活动

		this->OnRATotalConsumeGoldAddConsume(consume_gold);

		this->OnRAConsumeGoldRankConsumeGold((int)consume_gold);
		this->OnRAConsumeGoldFanliConsumeGold((int)consume_gold);

		this->OnRAContinueConsumeConsumeGold((int)consume_gold);
		this->OnRARedEnvelopeGiftConsumeGold((int)consume_gold);

		this->OnRAConsumeGoldRewardConsumeGold((int)consume_gold);
		this->OnRAConsumeForGiftConsumeGold((int)consume_gold);

	}

	{
		// 合服活动
		if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
		{
			if (ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_CONSUME_RANK))
			{
				m_activity_info.csa_consume_rank_consume_gold += static_cast<int>(consume_gold);

				int rank_type = LOGIC_CONFIG->GetCombineServerActivityConfig().GetRelatedRankType(CSA_SUB_TYPE_CONSUME_RANK);

				CombineServerActivityManager::Instance().OnCombineServerActivitySyncValue(rank_type, m_role->GetUID(),
					m_activity_info.csa_consume_rank_consume_gold, static_cast<long long>(LLONG_MAX - EngineAdapter::Instance().Time()));
			}
		}
	}
}

void RoleActivity::OnCheckCrossDailyConsumeGold(long long daily_consume_gold)
{
	// 检查每日消费活动的激活
	this->OnRADayConsumeGoldAddConsume(daily_consume_gold);
	this->OnRADayConsumeRankConsumeGold((int)daily_consume_gold);
}

void RoleActivity::OnRALotteryPutChestStore(int activity_type, int list_num, ChestItemInfo item_list[MODE_MAX_COUNT], const char *reason)
{
	if (NULL == reason) return;

	for (int item_index = 0; item_index < list_num && item_index < MODE_MAX_COUNT; ++item_index)
	{
		if (0 == item_list[item_index].item_id)
		{
			continue;
		}

		// 星级装备处理
		const ItemBase *item_base = ITEMPOOL->GetItem(item_list[item_index].item_id);
		if (NULL != item_base && ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType())
		{
			static ItemGridData item_grid_data; item_grid_data.Reset();
			item_grid_data.item_id = item_list[item_index].item_id;
			item_grid_data.param = new EquipmentParam();

			if (EquipmentManager::GetFixSpecialAttr(&item_grid_data, Equipment::EQUIPMENT_FIX_GETWAY_CHEST_SHOP, activity_type))
			{
				EquipmentParam *equip_param = dynamic_cast<EquipmentParam *>(item_grid_data.param);
				for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; ++i)
				{
					item_list[item_index].xianpin_type_list[i] = char(equip_param->xianpin_type_list[i]);
				}
			}

			// 释放
			if (item_grid_data.param)
			{
				delete item_grid_data.param;
				item_grid_data.param = nullptr;
			}
		}
	}

	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, list_num, item_list, reason);
}

void RoleActivity::FetchTodayThemeReward(int seq)
{
	if (seq < 0 || seq >= TODAY_THEME_REWARD_MAX_COUNT)
	{
		return;
	}
	if (BitmapFunction::IsBitSet(m_activity_info.today_theme_reward_flag, sizeof(m_activity_info.today_theme_reward_flag), seq))
	{
		m_role->NoticeNum(errornum::EN_TODAY_THEME_FETCH_FAIL);
		return;
	}
	const ItemConfigData *config_item = LOGIC_CONFIG->GetTodayThemeRewardConfig().GetRewardConfig(seq);
	if (nullptr == config_item)
	{
		return;
	}
	BitmapFunction::SetBit(m_activity_info.today_theme_reward_flag, sizeof(m_activity_info.today_theme_reward_flag), seq);
	m_role->GetKnapsack()->PutOrMail(*config_item, PUT_REASON_TODAY_THEME_REWARD);
	this->SendFetchTodayThemeRewardInfo();
}

void RoleActivity::SendFetchTodayThemeRewardInfo()
{
	Protocol::SCTodayThemeRewardFlagInfo info;
	UNSTD_STATIC_CHECK(sizeof(info.fetch_flag) == sizeof(m_activity_info.today_theme_reward_flag));
	memcpy(info.fetch_flag, m_activity_info.today_theme_reward_flag, sizeof(info.fetch_flag));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&info, sizeof(info));
}

void RoleActivity::OnRADailyLove2Chongzhi(const int chongzhi_num)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAILY_LOVE_2))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAILY_LOVE_2);

	if (0 != m_activity_info.ra_daily_love_daily_2_has_charge)
	{
		return;
	}

	const RandActivityDailyLoveConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetDailyLove2RewardPrecent();
	if (nullptr == reward_cfg)
	{
		return;
	}

	m_activity_info.ra_daily_love_daily_2_has_charge = 1;

	static MailContentParam contentparam; contentparam.Reset();

	contentparam.gold = chongzhi_num * reward_cfg->reward_gold_per / 100;

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_daily_love_2_reward_content,
		RAND_ACTIVITY_TYPE_DAILY_LOVE_2, chongzhi_num, contentparam.gold);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRADailyLove2Chongzhi::user[%d, %s],charge_value[%d] charge_reward_gold[%d]",
			m_role->GetUID(), m_role->GetName(), chongzhi_num, contentparam.gold);

		//RandActivityLogManager::Instance().AddLogData(m_role, RAND_ACTIVITY_TYPE_DAILY_LOVE_2, chongzhi_num, 0);
	}

	this->OnSendDailyLove2ChongzhiInfo();
}