#include "other/roleactivity/roleactivity.hpp"
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
#include "other/vip/vipconfig.hpp"
#include "item/knapsack.h"
#include "other/vip/vip.hpp"
#include "other/sublock/sublock.hpp"
#include "global/combineserveractivity/combineserveractivitybossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialcombineserverboss.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "scene/worldshadow/worldshadow.hpp"

int ComparaCSAI1(const void *arg1, const void *arg2)
{
	CombineServerActivityRankUser* ci1 = (CombineServerActivityRankUser*)arg1;
	CombineServerActivityRankUser* ci2 = (CombineServerActivityRankUser*)arg2;

	if (ci1->rank_value < ci2->rank_value)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

// --------------------------------------------------------------------------------------------------------
// 合服活动
// --------------------------------------------------------------------------------------------------------
void RoleActivity::OnCombineServerActivityOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (0 == m_activity_info.last_join_combine_server_activity_timestamp)
	{
		m_activity_info.last_join_combine_server_activity_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}

	if (!ActivityShadow::Instance().IsInSpecialActivityOpenTime(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER, m_activity_info.last_join_combine_server_activity_timestamp))
	{
		// 检查补发合服奖励
		this->CheckGiveCombineReward();

		// 合服后数据清理,防止合服再合服数据混乱
		memset(m_activity_info.csa_qianggou_buynum_list, 0, sizeof(m_activity_info.csa_qianggou_buynum_list));
		m_activity_info.csa_roll_total_chongzhi = 0;
		m_activity_info.csa_roll_chongzhi_num = 0;
		m_activity_info.csa_chongzhi_rank_chongzhi_num = 0;
		m_activity_info.csa_consume_rank_consume_gold = 0;
		m_activity_info.csa_kill_boss_kill_count = 0;

		m_activity_info.csa_login_gift_login_days = 1;
		m_activity_info.csa_login_gift_has_fetch_accumulate_reward = 0;
		m_activity_info.csa_login_gift_fetch_common_reward_flag = 0;
		m_activity_info.csa_login_gift_fetch_vip_reward_flag = 0;
		m_activity_info.csa_login_gift_last_login_dayid = EngineAdapter::Instance().DayID();
		memset(m_activity_info.csa_personal_panic_buy_numlist, 0, sizeof(m_activity_info.csa_personal_panic_buy_numlist));
		memset(m_activity_info.csa_server_panic_buy_numlist, 0, sizeof(m_activity_info.csa_server_panic_buy_numlist));
		m_activity_info.last_join_combine_server_activity_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_activity_info.csa_boss_daily_kill_count = 0;

		// 投资计划，自动领取
		{
			m_activity_info.csa_touzijihua_buy_flag = 0;
			m_activity_info.csa_touzjihua_login_day = 0;
			m_activity_info.csa_touzijihua_total_fetch_flag = 0;
		}

		//成长基金
		memset(m_activity_info.csa_foundation_status, 0, sizeof(m_activity_info.csa_foundation_status));

		//经验炼制
		m_activity_info.csa_refine_exp_had_buy = 0;

		m_activity_info.csa_had_add_gongchengzhan_win_times = 0;
	}
}

void RoleActivity::SendCSARoleInfo()
{
	static Protocol::SCCSARoleInfo csari;
	memcpy(csari.rank_qianggou_buynum_list, m_activity_info.csa_qianggou_buynum_list, sizeof(csari.rank_qianggou_buynum_list));
	csari.roll_chongzhi_num = m_activity_info.csa_roll_chongzhi_num;
	csari.chongzhi_rank_chongzhi_num = m_activity_info.csa_chongzhi_rank_chongzhi_num;
	csari.consume_rank_consume_gold = m_activity_info.csa_consume_rank_consume_gold;
	csari.kill_boss_kill_count = m_activity_info.csa_kill_boss_kill_count;
	memcpy(csari.personal_panic_buy_numlist, m_activity_info.csa_personal_panic_buy_numlist, sizeof(csari.personal_panic_buy_numlist));
	memcpy(csari.server_panic_buy_numlist, m_activity_info.csa_server_panic_buy_numlist, sizeof(csari.server_panic_buy_numlist));

	csari.login_days = m_activity_info.csa_login_gift_login_days;
	csari.has_fetch_accumulate_reward = m_activity_info.csa_login_gift_has_fetch_accumulate_reward;
	csari.fetch_common_reward_flag = m_activity_info.csa_login_gift_fetch_common_reward_flag;
	csari.fetch_vip_reward_flag = m_activity_info.csa_login_gift_fetch_vip_reward_flag;
	csari.roll_total_chongzhi_num = m_activity_info.csa_roll_total_chongzhi;
	csari.rank_combine_chongzhi_num = 0;
	csari.rank_combine_consume_num = 0;

	const CombineServerActivityRankParam cbsarp = CombineServerActivityManager::Instance().GetCombineServerActivityRankParam();
	CombineServerActivityRankItem rankchongzhi = cbsarp.rank_item_list[CSA_RANK_TYPE_CHONGZHI];
	CombineServerActivityRankItem rankconsem = cbsarp.rank_item_list[CSA_RANK_TYPE_CONSUME];
	qsort(rankchongzhi.user_list, COMBINE_SERVER_CONSUME_RANK_COUNT, sizeof(rankchongzhi.user_list[0]), ComparaCSAI1);
	qsort(rankconsem.user_list, COMBINE_SERVER_CONSUME_RANK_COUNT, sizeof(rankconsem.user_list[0]), ComparaCSAI1);

	for (int j = 0; j < COMBINE_SERVER_CONSUME_RANK_COUNT; ++j)
	{
		if (m_role->GetUID() == rankchongzhi.user_list[j].role_id)
		{
			csari.rank_combine_chongzhi_num = j + 1;
		}
		if (m_role->GetUID() == rankconsem.user_list[j].role_id)
		{
			csari.rank_combine_consume_num = j + 1;
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&csari, sizeof(csari));
}

bool RoleActivity::OnCSARankQianggou(int item_idx)
{
	const CSAQianggouItemConfig *cfg_item = LOGIC_CONFIG->GetCombineServerActivityConfig().GetRankQianggouCfg(item_idx);
	if (NULL == cfg_item) return false;

	int all_buy_num = CombineServerActivityManager::Instance().GetRankQianggouBuyNum(item_idx);
	if (all_buy_num >= cfg_item->limit_num)
	{
		m_role->NoticeNum(errornum::EN_CSA_QIANGGOU_NUM_LIMIT);
		return false;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		return false;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	long long consume_gold = cfg_item->cost, consume_gold_bind = 0;
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cfg_item->cost, "CSARankQianggou"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	m_role->GetKnapsack()->Put(cfg_item->stuff_item, PUT_REASON_CSA_RANK_QIANGGOU);

	++ m_activity_info.csa_qianggou_buynum_list[item_idx];

	int total_buynum = 0;
	{
		for (int i = 0; i < COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE; ++ i)
		{
			total_buynum += m_activity_info.csa_qianggou_buynum_list[i];
		}
	}
	CombineServerActivityManager::Instance().OnCombineServerActivitySyncValue(CSA_RANK_TYPE_QIANGGOU, m_role->GetUID(), 
		total_buynum, static_cast<long long>(LLONG_MAX - EngineAdapter::Instance().Time()));

	CombineServerActivityManager::Instance().OnBuyRankQianggouItem(m_role, item_idx);

	gamelog::g_log_combine_server.printf(LL_INFO, "OnCSARankQianggou user %d %s, use_gold %lld, use_all_gold %lld, item_idx %d, buy_num %d, item_detail %s",
		m_role->GetUID(), m_role->GetName(), consume_gold, consume_gold_bind, item_idx, 
		m_activity_info.csa_qianggou_buynum_list[item_idx], ItemConfigDataLog(&cfg_item->stuff_item, NULL));

	return true;
}

bool RoleActivity::OnCSARollReq()
{
	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	const CSARollConfig &roll_cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetRollCfg();
	if (roll_cfg.total_weight <= 0) 
	{
		return false;
	}

	if (m_activity_info.csa_roll_chongzhi_num < roll_cfg.roll_cost)
	{
		m_role->NoticeNum(errornum::EN_CSA_ROLL_CHONGZHI_LIMIT);
		return false;
	}

	int rand_value = RandomNum(roll_cfg.total_weight);
	int reward_seq = -1;

	for (int i = 0; i < roll_cfg.cfg_count; ++ i)
	{
		if (rand_value < roll_cfg.cfg_list[i].weight)
		{
			reward_seq = i;
			break;
		}

		rand_value -= roll_cfg.cfg_list[i].weight;
	}

	if (reward_seq < 0 || reward_seq >= roll_cfg.cfg_count)
	{
		return false;
	}

	const CSARollConfigItem &reward_cfg = roll_cfg.cfg_list[reward_seq];

	m_activity_info.csa_roll_chongzhi_num -= roll_cfg.roll_cost;

	m_role->GetKnapsack()->Put(reward_cfg.reward_item, PUT_REASON_CSA_ROLL);

	static Protocol::SCCSARollResult csarr;
	csarr.ret_seq = reward_seq;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&csarr, sizeof(csarr));

	//if (reward_cfg.is_broadcast)
	//{
	//	int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_csa_roll_reward_notice, 
	//		m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), reward_cfg.reward_item.item_id);

	//	if (sendlen > 0)
	//	{
	//		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
	//	}
	//}

	//记录本次roll到的物品id等待客户端要求广播
	m_csa_roll_item_id = reward_cfg.reward_item.item_id;

	gamelog::g_log_combine_server.printf(LL_INFO, "OnCSARollReq user %d %s, roll_cost %d, remain %d, reward_seq %d,  item_detail %s",
		m_role->GetUID(), m_role->GetName(), roll_cfg.roll_cost, m_activity_info.csa_roll_chongzhi_num, reward_seq, 
		ItemConfigDataLog(&reward_cfg.reward_item, NULL));

	return true;
}

void RoleActivity::OnCSARollBroadcast(int item_id)
{
	if (m_csa_roll_item_id == 0)
	{//使用服务器自己记录的物品id
		return;
	}
	int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_csa_roll_reward_notice,
		m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), m_csa_roll_item_id);
	m_csa_roll_item_id = 0;

	if (sendlen > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_ROLL);
	}
}

bool RoleActivity::OnCSAFetchKillBossReward()
{
	if (m_activity_info.csa_kill_boss_kill_count <= 0)
	{
		return false;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	const CSAOtherConfig &other_cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetOtherCfg();
	if (other_cfg.kill_boss_reward_cost <= 0) return false;

	if (m_activity_info.csa_kill_boss_kill_count < other_cfg.kill_boss_reward_cost)
	{
		m_role->NoticeNum(errornum::EN_CSA_CAN_NOT_FETCH_REWARD);
		return false;
	}

	// 结算
	m_activity_info.csa_kill_boss_kill_count -= other_cfg.kill_boss_reward_cost;

	const ItemBase *item_base = ITEMPOOL->GetItem(other_cfg.kill_boss_reward_item.item_id);
	if (NULL == item_base) 
	{
		return false;
	}

	ItemConfigData reward_item = other_cfg.kill_boss_reward_item;

	m_role->GetKnapsack()->Put(reward_item, PUT_REASON_CSA_KILL_BOSS_REWARD);

	m_role->NoticeNum(noticenum::NT_CSA_FETCH_REWARD_SUCC);

	gamelog::g_log_combine_server.printf(LL_INFO, "OnCSAFetchKillBossReward user %d %s, cost %d, remain %d, item_detail %s",
		m_role->GetUID(), m_role->GetName(), other_cfg.kill_boss_reward_cost, m_activity_info.csa_kill_boss_kill_count, ItemConfigDataLog(&reward_item, NULL));

	return true;
}

void RoleActivity::OnCSASingleChargeCheckCharge(long long gold_num)
{
	if (gold_num <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		return;
	}

	if (!ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_SINGLE_CHARGE))
	{
		return;
	}

	const CSASingleChargeCfg::ConfigItem *sc_cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetSingleChargeCfg(gold_num);
	if (NULL == sc_cfg)
	{
		return;
	}

	// 单笔充值达到要求，发放奖励
	static MailContentParam contentparam; contentparam.Reset();

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(sc_cfg->reward_item.item_id);
	if (NULL != reward_item_base)
	{
		contentparam.item_list[0].item_id = sc_cfg->reward_item.item_id;
		contentparam.item_list[0].num = sc_cfg->reward_item.num;
		contentparam.item_list[0].is_bind = (sc_cfg->reward_item.is_bind) ? 1 : 0;
		contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 
	}	

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_csa_single_charge_reward_content, gold_num);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		gamelog::g_log_combine_server.printf(LL_INFO, "OnCSASingleChargeCheckCharge::user[%d, %s], chongzhi[%lld], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), gold_num, ItemConfigDataLog(&sc_cfg->reward_item, NULL));
	}
}

void RoleActivity::ONCSAChargeRewardDoubleCheckCharge(long long gold_num)
{
	if (gold_num <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		return;
	}

	if (!ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_CHARGE_REWARD_DOUBLE))
	{
		return;
	}

	static MailContentParam contentparam; contentparam.Reset();
	contentparam.gold = static_cast<int>(gold_num);

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_csa_charge_reward_double_content, gold_num);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		gamelog::g_log_combine_server.printf(LL_INFO, "ONCSAChargeRewardDoubleCheckCharge::user[%d, %s], chongzhi[%lld], reward_gold[%lld]",
			m_role->GetUID(), m_role->GetName(), gold_num, gold_num);
	}
}

bool RoleActivity::OnCSALoginGiftFetchCommonReward(int seq)
{
	UNSTD_STATIC_CHECK(COMBINE_SERVER_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.csa_login_gift_fetch_common_reward_flag) * 8);

	if (seq < 0 || seq >= COMBINE_SERVER_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT)
	{
		return false;
	}

	if (0 != (m_activity_info.csa_login_gift_fetch_common_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_CSA_HAS_FETCH_REWARD);
		return false;
	}

	const CSALoginGiftCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCombineServerActivityConfig().GetLoginGiftRewardCfg(seq);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_activity_info.csa_login_gift_login_days < cfg_item->need_login_days)
	{
		m_role->NoticeNum(errornum::EN_CSA_CAN_NOT_FETCH_REWARD);
		return false;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	m_activity_info.csa_login_gift_fetch_common_reward_flag |= (1 << seq);

	m_role->GetKnapsack()->Put(cfg_item->common_reward_item, PUT_REASON_CSA_LOGIN_GIFT);

	gamelog::g_log_combine_server.printf(LL_INFO, "OnCSALoginGiftFetchCommonReward user %d %s, login_days %d, seq %d, item_detail %s",
		m_role->GetUID(), m_role->GetName(), m_activity_info.csa_login_gift_login_days, seq, ItemConfigDataLog(&cfg_item->common_reward_item, NULL));

	return true;
}


bool RoleActivity::OnCSALoginGiftFetchVipReward(int seq)
{
 	UNSTD_STATIC_CHECK(COMBINE_SERVER_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.csa_login_gift_fetch_vip_reward_flag) * 8);
 
 	if (seq < 0 || seq >= COMBINE_SERVER_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT)
 	{
 		return false;
 	}
 
 	int vip_level = m_role->GetVip()->GetVipLevel();
 	int can_fetch_reward = (0 != LOGIC_CONFIG->GetVipConfig().GetAuthParam(vip_level, VAT_RA_LOGIN_GIFT_VIP_REWARD));
 
 	if (!can_fetch_reward)
 	{
 		m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGH);
 		return false;
 	}
 
 	if (0 != (m_activity_info.csa_login_gift_fetch_vip_reward_flag & (1 << seq)))
 	{
 		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
 		return false;
 	}
 
 	const CSALoginGiftCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCombineServerActivityConfig().GetLoginGiftRewardCfg(seq);
 	if (NULL == cfg_item)
 	{
 		return false;
 	}
 
 	if (m_activity_info.csa_login_gift_login_days < cfg_item->need_login_days)
 	{
 		m_role->NoticeNum(errornum::EN_CSA_CAN_NOT_FETCH_REWARD);
 		return false;
 	}
 
 	if (!m_role->GetKnapsack()->HaveSpace())
 	{
 		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
 		return false;
 	}
 
 	m_activity_info.csa_login_gift_fetch_vip_reward_flag |= (1 << seq);
 
 	m_role->GetKnapsack()->Put(cfg_item->vip_reward_item, PUT_REASON_CSA_LOGIN_GIFT);
 
 	gamelog::g_log_combine_server.printf(LL_INFO, "OnCSALoginGiftFetchVipReward user %d %s, login_days %d, seq %d, item_detail %s",
 		m_role->GetUID(), m_role->GetName(), m_activity_info.csa_login_gift_login_days, seq, ItemConfigDataLog(&cfg_item->vip_reward_item, NULL));
 
 	return true;
}

bool RoleActivity::OnCSALoginGiftFetchAccumulateReward()
{
	if (0 != m_activity_info.csa_login_gift_has_fetch_accumulate_reward)
	{
		m_role->NoticeNum(errornum::EN_CSA_HAS_FETCH_REWARD);
		return false;
	}

	const CSALoginGiftCfg &login_gift_cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetLoginGiftCfg();

	if (m_activity_info.csa_login_gift_login_days < login_gift_cfg.need_accumulate_days)
	{
		m_role->NoticeNum(errornum::EN_CSA_CAN_NOT_FETCH_REWARD);
		return false;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	m_activity_info.csa_login_gift_has_fetch_accumulate_reward = 1;

	m_role->GetKnapsack()->Put(login_gift_cfg.accumulate_reward, PUT_REASON_CSA_LOGIN_GIFT);

	gamelog::g_log_combine_server.printf(LL_INFO, "OnCSALoginGiftFetchAccumulateReward user %d %s, login_days %d, item_detail %s",
		m_role->GetUID(), m_role->GetName(), m_activity_info.csa_login_gift_login_days, ItemConfigDataLog(&login_gift_cfg.accumulate_reward, NULL));

	return true;
}

void RoleActivity::CheckCSALoginGiftGiveReward()
{
	std::vector<ItemConfigData> reward_item_vec;

	// 普通奖励
	for (int seq = 0; seq < COMBINE_SERVER_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT; seq ++)
	{
		if (0 != (m_activity_info.csa_login_gift_fetch_common_reward_flag & (1 << seq)))
		{
			continue;
		}

		const CSALoginGiftCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCombineServerActivityConfig().GetLoginGiftRewardCfg(seq);
		if (NULL == cfg_item)
		{
			continue;
		}

		if (m_activity_info.csa_login_gift_login_days < cfg_item->need_login_days)
		{
			continue;
		}

		ItemConfigData item;
		item.item_id = cfg_item->common_reward_item.item_id;
		item.is_bind = cfg_item->common_reward_item.is_bind;
		item.num = cfg_item->common_reward_item.num;
		auto it = std::find_if(reward_item_vec.begin(), reward_item_vec.end(), [item](const ItemConfigData &tmp_item) {
			return tmp_item.item_id == item.item_id && tmp_item.is_bind == item.is_bind;
		});

		if (it != reward_item_vec.end())
		{
			it->num += item.num;
		}
		else
		{
			reward_item_vec.push_back(item);
		}

		m_activity_info.csa_login_gift_fetch_common_reward_flag |= (1 << seq);

		gamelog::g_log_combine_server.printf(LL_INFO, "OnCSALoginGiftGiveCommonReward user %d %s, login_days %d, seq %d, item_detail %s",
			m_role->GetUID(), m_role->GetName(), m_activity_info.csa_login_gift_login_days, seq, ItemConfigDataLog(&cfg_item->common_reward_item, NULL));
	}

	// vip奖励
	for (int seq = 0; seq < COMBINE_SERVER_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT; seq++)
	{
		int vip_level = m_role->GetVip()->GetVipLevel();
		int can_fetch_reward = (0 != LOGIC_CONFIG->GetVipConfig().GetAuthParam(vip_level, VAT_RA_LOGIN_GIFT_VIP_REWARD));

		if (!can_fetch_reward)
		{
			continue;
		}

		if (0 != (m_activity_info.csa_login_gift_fetch_vip_reward_flag & (1 << seq)))
		{
			continue;
		}

		const CSALoginGiftCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCombineServerActivityConfig().GetLoginGiftRewardCfg(seq);
		if (NULL == cfg_item)
		{
			continue;
		}

		if (m_activity_info.csa_login_gift_login_days < cfg_item->need_login_days)
		{
			continue;
		}

		ItemConfigData item;
		item.item_id = cfg_item->vip_reward_item.item_id;
		item.is_bind = cfg_item->vip_reward_item.is_bind;
		item.num = cfg_item->vip_reward_item.num;
		auto it = std::find_if(reward_item_vec.begin(), reward_item_vec.end(), [item](const ItemConfigData &tmp_item) {
			return tmp_item.item_id == item.item_id && tmp_item.is_bind == item.is_bind;
		});

		if (it != reward_item_vec.end())
		{
			it->num += item.num;
		}
		else
		{
			reward_item_vec.push_back(item);
		}

		m_activity_info.csa_login_gift_fetch_vip_reward_flag |= (1 << seq);

		gamelog::g_log_combine_server.printf(LL_INFO, "OnCSALoginGiftGiveVipReward user %d %s, login_days %d, seq %d, item_detail %s",
			m_role->GetUID(), m_role->GetName(), m_activity_info.csa_login_gift_login_days, seq, ItemConfigDataLog(&cfg_item->vip_reward_item, NULL));
	}
	
	// 累计奖励
	if (0 == m_activity_info.csa_login_gift_has_fetch_accumulate_reward)
	{
		const CSALoginGiftCfg &login_gift_cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetLoginGiftCfg();

		if (m_activity_info.csa_login_gift_login_days >= login_gift_cfg.need_accumulate_days)
		{
			ItemConfigData item;
			item.item_id = login_gift_cfg.accumulate_reward.item_id;
			item.is_bind = login_gift_cfg.accumulate_reward.is_bind;
			item.num = login_gift_cfg.accumulate_reward.num;
			auto it = std::find_if(reward_item_vec.begin(), reward_item_vec.end(), [item](const ItemConfigData &tmp_item) {
				return tmp_item.item_id == item.item_id && tmp_item.is_bind == item.is_bind;
			});

			if (it != reward_item_vec.end())
			{
				it->num += item.num;
			}
			else
			{
				reward_item_vec.push_back(item);
			}

			m_activity_info.csa_login_gift_has_fetch_accumulate_reward = 1;

			gamelog::g_log_combine_server.printf(LL_INFO, "OnCSALoginGiftGiveAccumulateReward user %d %s, login_days %d, item_detail %s",
				m_role->GetUID(), m_role->GetName(), m_activity_info.csa_login_gift_login_days, ItemConfigDataLog(&login_gift_cfg.accumulate_reward, NULL));
		}
	}

	// 发奖励
	{
		int reward_count = 0;
		ItemConfigData reward_item_list[COMBINE_SERVER_MAX_LOGIN_GIFT_NUM]; memset(reward_item_list, 0, sizeof(reward_item_list));
		for (auto &it : reward_item_vec)
		{
			if (reward_count >= COMBINE_SERVER_MAX_LOGIN_GIFT_NUM)
			{
				break;
			}

			ItemConfigData &item = reward_item_list[reward_count];
			item.item_id = it.item_id;
			item.is_bind = it.is_bind;
			item.num = it.num;

			reward_count++;
		}

		if (reward_count > 0)
		{
			gamecommon::SendMultiMailToRole(m_role->GetUserId(), reward_item_list, reward_count, NULL, true, gamestring::g_csa_logingift_auto_fetch_reward_content);
		}
	}
}

bool RoleActivity::OnCSAServerPanicBuyReq(int item_idx)
{
	if (item_idx < 0 || item_idx >= COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT)
	{
		return false;
	}

	const CSAServerPanicBuyCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCombineServerActivityConfig().GetServerPanicBuyItemCfg(item_idx);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_activity_info.csa_server_panic_buy_numlist[item_idx] >= cfg_item->personal_limit_buy_count)
	{
		m_role->NoticeNum(errornum::EN_CSA_PERSONAL_BUY_NUM_LIMIT);
		return false;
	}

	if (CombineServerActivityManager::Instance().GetServerPanicBuyNum(item_idx) >= cfg_item->server_limit_buy_count)
	{
		m_role->NoticeNum(errornum::EN_CSA_SERVER_BUY_NUM_LIMIT);
		return false;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(cfg_item->gold_price))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cfg_item->gold_price, "CSAServerPanicBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	CombineServerActivityManager::Instance().OnServerPanicBuy(m_role, item_idx);
	++ m_activity_info.csa_server_panic_buy_numlist[item_idx];

	// 给予
	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_CSA_SERVER_PANIC_BUY);

	gamelog::g_log_combine_server.printf(LL_INFO, "OnCSAServerPanicBuyReq user %d %s, item_idx %d, person_buy_num %d, server_buy_num %d, item_detail %s",
		m_role->GetUID(), m_role->GetName(), item_idx, m_activity_info.csa_server_panic_buy_numlist[item_idx],
		CombineServerActivityManager::Instance().GetServerPanicBuyNum(item_idx), ItemConfigDataLog(&cfg_item->reward_item, NULL));

	return true;
}

bool RoleActivity::OnCSAPersonalPanicBuyReq(int item_idx)
{
	if (item_idx < 0 || item_idx >= COMBINE_SERVER_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT)
	{
		return false;
	}

	const CSAPersonalPanicBuyCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCombineServerActivityConfig().GetPersonalPanicBuyItemCfg(item_idx);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_activity_info.csa_personal_panic_buy_numlist[item_idx] >= cfg_item->limit_buy_count)
	{
		m_role->NoticeNum(errornum::EN_CSA_PERSONAL_BUY_NUM_LIMIT);
		return false;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(cfg_item->gold_price))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cfg_item->gold_price, "CSAPersonalPanicBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	++ m_activity_info.csa_personal_panic_buy_numlist[item_idx];

	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_CSA_PERSONAL_PANIC_BUY);

	gamelog::g_log_combine_server.printf(LL_INFO, "OnCSAPersonalPanicBuyReq user %d %s, item_idx %d, buy_num %d, item_detail %s",
		m_role->GetUID(), m_role->GetName(), item_idx, m_activity_info.csa_personal_panic_buy_numlist[item_idx],
		ItemConfigDataLog(&cfg_item->reward_item, NULL));

	return true;
}

void RoleActivity::OnCSAOperaReq(int sub_type, int param_1, int param_2)
{
	// 投资计划只限制活动期间购买，不限制领取
	if (CSA_SUB_TYPE_TOUZIJIHUA == sub_type)
	{
		this->OnCSATouzijihuaOpera(param_1, param_2);
		return;
	}

	// 成长基金只限制活动期间购买，不限制领取
	if (CSA_SUB_TYPE_FOUNDATION == sub_type)
	{
		this->OnCSAFoundationOpera(param_1, param_2);
		return;
	}

	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		return;
	}

	if (CSA_SUB_TYPE_INVALID == sub_type)
	{
		this->SendCSARoleInfo();
		return;
	}

	if (!ActivityShadow::Instance().IsCSASubActivityOpen(sub_type))
	{
		return;
	}

	bool ret = false;

	switch (sub_type)
	{
	case CSA_SUB_TYPE_RANK_QIANGGOU:
		{
			ret = this->OnCSARankQianggou(param_1);
		}
		break;

	case CSA_SUB_TYPE_ROLL:
		{
			if (Protocol::CSA_ROLL_OPERA_ROLL == param_1)
			{
				ret = this->OnCSARollReq();
			}
			else if (Protocol::CSA_ROLL_OPERA_BROADCAST == param_1)
			{
				this->OnCSARollBroadcast(param_2);
			}
			
		}
		break;

	case CSA_SUB_TYPE_KILL_BOSS:
		{
			ret = this->OnCSAFetchKillBossReward();
		}
		break;

	case CSA_SUB_TYPE_LOGIN_Gift:
		{
			if (Protocol::CSA_LOGIN_GIFT_OPERA_FETCH_COMMON_REWARD == param_1)
			{
				ret = this->OnCSALoginGiftFetchCommonReward(param_2);
			}
			else if (Protocol::CSA_LOGIN_GIFT_OPERA_FETCH_VIP_REWARD == param_1)
			{
				ret = this->OnCSALoginGiftFetchVipReward(param_2);
			}
			else
			{
				ret = this->OnCSALoginGiftFetchAccumulateReward();
			}
		}
		break;

	case CSA_SUB_TYPE_PERSONAL_PANIC_BUY:
		{
			ret = this->OnCSAPersonalPanicBuyReq(param_1);
		}
		break;

	case CSA_SUB_TYPE_SERVER_PANIC_BUY:
		{
			ret = this->OnCSAServerPanicBuyReq(param_1);
		}
		break;

	case CSA_SUB_TYPE_BOSS:
		{
			switch (param_1)
			{
				case Protocol::CSA_BOSS_OPERA_TYPE_ENTER:
				{
					this->OnCSABossEnter();
				}
				break;

				case Protocol::CSA_BOSS_OPERA_TYPE_INFO_REQ:
				{
					this->SendCSABossInfo();
				}
				break;

				case Protocol::CSA_BOSS_OPERA_TYPE_RANK_REQ:
				{
					this->SendCSABossRankInfo();
				}
				break;

				case Protocol::CSA_BOSS_OPERA_TYPE_ROLE_INFO_REQ:
				{
					this->SendCSABossRoleInfo();
				}
				break;
			default:
				break;
			}
		}
		break;

	case CSA_SUB_TYPE_BUY_EXP:
	{
		this->OnCSARefineOperaReq(param_1, param_2);
	}
	break;

	case CSA_SUB_TYPE_GONGCHENGZHAN:
	{
		this->SendCSAGongChengZhanInfo();
	}
	break;

	default:
		break;
	}

	if (ret) this->SendCSARoleInfo();
}

void RoleActivity::OnCSALoginGiftLogin()
{
	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		return;
	}

	if (!ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_LOGIN_Gift))
	{
		return;
	}

	if (EngineAdapter::Instance().DayID() != m_activity_info.csa_login_gift_last_login_dayid)
	{
		m_activity_info.csa_login_gift_last_login_dayid = EngineAdapter::Instance().DayID();
		++ m_activity_info.csa_login_gift_login_days;
		this->SendCSARoleInfo();
	}
}

void RoleActivity::OnCSABossEnter()
{
	if (!m_role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}
	
	SpecialCombineServerBoss *special = World::GetInstWorld()->GetSceneManager()->GetSpecialCSABoss(1, true);
	if (NULL != special)
	{
		Posi enterpos = LOGIC_CONFIG->GetCSABossConfig().GetCSABossOtherCfg().enter_pos;
		int scene_id = LOGIC_CONFIG->GetCSABossConfig().GetCSABossOtherCfg().scene_id;

		World::GetInstWorld()->GetSceneManager()->GoTo(m_role, scene_id, COMMON_SCENE_KEY, enterpos);
	}
}

void RoleActivity::SendCSABossInfo()
{
	SpecialCombineServerBoss *csa_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCSABoss(1);
	if (NULL != csa_boss)
	{
		csa_boss->SendBossInfo(m_role);
	}
}

void RoleActivity::SendCSABossRankInfo()
{
	CombineServerActivityManager::Instance().OnSendCSABossRankInfo(m_role);
}

void RoleActivity::SendCSABossRoleInfo()
{
	static Protocol::SCCSABossRoleInfo role_info;
	role_info.kill_boss_num = m_activity_info.csa_boss_daily_kill_count;
	int guild_id = m_role->GetGuildID();
	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL != guild)
	{
		role_info.guild_killl_boss_num = guild->GetGuildCommonDataParam()->kill_csa_boss_num;
	}
	else
	{
		role_info.guild_killl_boss_num = 0;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&role_info, sizeof(role_info));
}

void RoleActivity::OnAddCSAGongChengZhanWinTimes()
{
	if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER) && ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_GONGCHENGZHAN))
	{
		if (0 == m_activity_info.csa_had_add_gongchengzhan_win_times)
		{
			++m_activity_info.csa_gongchengzhan_win_times;
			m_activity_info.csa_had_add_gongchengzhan_win_times = 1;
			this->SendCSAGongChengZhanInfo();
		}
	}
}

void RoleActivity::SendCSAGongChengZhanInfo()
{
	static Protocol::SCCSAGongChengZhanInfo pro;
	pro.win_times = m_activity_info.csa_gongchengzhan_win_times;

	SEND_TO_ROLE(m_role, pro);
}

void RoleActivity::OnCSATouzijihuaOpera(int opera_type, int param)
{
	switch (opera_type)
	{
	case Protocol::CSA_TOUZIJIHUA_INFO_REQ:
	{
		this->SendCSATouzijihuaInfo();
	}
	break;

	case Protocol::CSA_TOUZIJIHUA_FETCH_REQ:
	{
		this->OnCSATouzijihuaFetch(param);
	}
	break;

	default:
		break;
	}
}

bool RoleActivity::OnCSATouzijihuaBuy(int chongzhi_num)
{
	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		return false;
	}

	if (!ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_TOUZIJIHUA))
	{
		return false;
	}

	const CSAOtherConfig &other_cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetOtherCfg();
	if (chongzhi_num != other_cfg.touzi_jihua_buy_cost)
	{
		return false;
	}

	if (0 != m_activity_info.csa_touzijihua_buy_flag)
	{
		m_role->NoticeNum(errornum::EN_CSA_TOUZIJIHUA_HAD_BUY);
		return false;
	}

	m_role->GetKnapsack()->GetMoney()->AddGold(other_cfg.touzi_jihua_buy_reward_gold, "OnCSATouzijihuaBuy");

	m_activity_info.csa_touzijihua_buy_flag = 1;

	int dayid = GetDayIndex(ActivityShadow::Instance().GetSpecialActivityBeginTime(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER), static_cast<unsigned int>(EngineAdapter::Instance().Time())) + 1;

	m_activity_info.csa_touzjihua_login_day = dayid;

	this->SendCSATouzijihuaInfo();

	gamelog::g_log_combine_server.printf(LL_INFO, "OnCSATouzijihuaBuy user %d %s, buy_cost %d, reward_gold %d",
		m_role->GetUID(), m_role->GetName(), other_cfg.touzi_jihua_buy_cost, other_cfg.touzi_jihua_buy_reward_gold);

	return true;
}

void RoleActivity::OnCSATouzijihuaFetch(int index)
{
	if (0 == m_activity_info.csa_touzijihua_buy_flag)
	{
		m_role->NoticeNum(errornum::EN_CSA_TOUZIJIHUA_NOT_BUY);
		return;
	}

	if (0 != (1 << index & m_activity_info.csa_touzijihua_total_fetch_flag))
	{
		m_role->NoticeNum(errornum::EN_CSA_TOUZIJIHUA_HAD_FETCH);
		return;
	}

	const CSATouzijihuaCfg *cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetTouzijihuaCfg(index);
	if (nullptr == cfg)
	{
		m_role->NoticeNum(errornum::EN_CSA_TOUZIJIHUA_FETCH_LIMIT);
		return;
	}

	ActivityShadow::Instance().GetSpecialActivityBeginTime(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER);

	if (m_activity_info.csa_touzjihua_login_day < cfg->login_day)
	{
		m_role->NoticeNum(errornum::EN_CSA_TOUZIJIHUA_FETCH_LIMIT);
		return;
	}

	m_activity_info.csa_touzijihua_total_fetch_flag |= 1 << index;

	m_role->GetKnapsack()->GetMoney()->AddGold(cfg->reward_gold, "OnCSATouzijihuaFetch");

	this->SendCSATouzijihuaInfo();

	gamelog::g_log_combine_server.printf(LL_INFO, "OnCSATouzijihuaFetch user %d %s, index %d, csa_login_day %d, reward_gold %d",
		m_role->GetUID(), m_role->GetName(), index, m_activity_info.csa_touzjihua_login_day, cfg->reward_gold);
}

void RoleActivity::CheckTouzijihuaFetch()
{
	if (0 == m_activity_info.csa_touzijihua_buy_flag)
	{
		return;
	}

	const int cfg_count = LOGIC_CONFIG->GetCombineServerActivityConfig().GetTouzijihuaCfgCount();
	int total_reward_gold = 0;
	for (int index = 0; index < cfg_count; ++index)
	{
		if (0 != (1 << index & m_activity_info.csa_touzijihua_total_fetch_flag))
		{
			continue;
		}

		const CSATouzijihuaCfg *cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetTouzijihuaCfg(index);
		if (nullptr == cfg)
		{
			continue;
		}

		total_reward_gold += cfg->reward_gold;

		m_activity_info.csa_touzijihua_total_fetch_flag |= 1 << index;

		gamelog::g_log_combine_server.printf(LL_INFO, "OnCSATouzijihuaAutoFetch user %d %s, index %d, reward_gold %d",
			m_role->GetUID(), m_role->GetName(), index, cfg->reward_gold);
	}

	if (total_reward_gold <= 0)
	{
		return;
	}

	static MailContentParam contentparam;
	contentparam.Reset();
	contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, total_reward_gold);

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_csa_touzijihua_auto_fetch_reward_content);
	if (length > 0)
	{
		//MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
	}
}

void RoleActivity::SendCSATouzijihuaInfo()
{
	static Protocol::SCCSATouzijihuaInfo pro;
	pro.csa_touzijihua_buy_flag = m_activity_info.csa_touzijihua_buy_flag;
	pro.csa_touzijihua_reserve_ch = 0;
	pro.csa_touzjihua_login_day = m_activity_info.csa_touzjihua_login_day;
	pro.csa_touzijihua_total_fetch_flag = m_activity_info.csa_touzijihua_total_fetch_flag;

	SEND_TO_ROLE(m_role, pro);
}

void RoleActivity::OnCSAFoundationOpera(int opera_type, int param)
{
	switch (opera_type)
	{
	case Protocol::CSA_FOUNDATION_INFO_REQ:
	{
		this->SendCSAFoundationInfo();
	}
	break;

	case Protocol::CSA_FOUNDATION_FETCH_REQ:
	{
		this->OnCSAFoundationFetch(param);
	}
	break;

	default:
		break;
	}
}

bool RoleActivity::OnCSAFoundationBuy(int chongzhi_num)
{
	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER))
	{
		return false;
	}

	if (!ActivityShadow::Instance().IsCSASubActivityOpen(CSA_SUB_TYPE_FOUNDATION))
	{
		return false;
	}

	const CSAFoundationConfig *cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetFoundationCfg(chongzhi_num);
	if (nullptr == cfg)
	{
		return false;
	}

	if (cfg->seq < 0 || cfg->seq >= COMBINE_SERVER_MAX_FOUNDATION_TYPE)
	{
		return false;
	}

	if (m_activity_info.csa_foundation_status[cfg->seq].buy_level > 0)
	{
		m_role->NoticeNum(errornum::EN_CSA_TOUZIJIHUA_HAD_BUY);
		return false;
	}

	m_activity_info.csa_foundation_status[cfg->seq].buy_level = m_role->GetLevel();

	m_role->GetKnapsack()->GetMoney()->AddGold(cfg->chongzhi_gold, "OnCSAFoundationBuy");

	this->SendCSAFoundationInfo();

	gamelog::g_log_combine_server.printf(LL_INFO, "OnCSAFoundationBuy user %d %s, reward_gold %d",
		m_role->GetUID(), m_role->GetName(), cfg->chongzhi_gold);

	return true;
}

void RoleActivity::OnCSAFoundationFetch(int index)
{
	int cfg_count = LOGIC_CONFIG->GetCombineServerActivityConfig().GetFoundationCfgCount();
	if (index < 0 || index >= COMBINE_SERVER_MAX_FOUNDATION_TYPE || index >= cfg_count)
	{
		return;
	}

	CSAFoundationStatus &status = m_activity_info.csa_foundation_status[index];
	if (status.buy_level <= 0)
	{
		m_role->NoticeNum(errornum::EN_CSA_TOUZIJIHUA_NOT_BUY);
		return;
	}

	int delta_level = m_role->GetLevel() - status.buy_level;

	int reward_gold = LOGIC_CONFIG->GetCombineServerActivityConfig().GetFoundationRewardGold(index, status.reward_phase, delta_level);
	if (reward_gold <= 0)
	{
		m_role->NoticeNum(errornum::EN_CSA_TOUZIJIHUA_FETCH_LIMIT);
		return;
	}

	++status.reward_phase;

	m_role->GetKnapsack()->GetMoney()->AddGold(reward_gold, "OnCSAFoundationFetch");

	this->SendCSAFoundationInfo();

	gamelog::g_log_combine_server.printf(LL_INFO, "OnCSAFoundationFetch user %d %s, index %d, reward_phase %d, reward_gold %d",
		m_role->GetUID(), m_role->GetName(), index, status.reward_phase - 1, reward_gold);
}

void RoleActivity::CheckFoundationFetch()
{
	int cfg_count = LOGIC_CONFIG->GetCombineServerActivityConfig().GetFoundationCfgCount();

	int total_reward_gold = 0;
	for (int index = 0; index < cfg_count && index < COMBINE_SERVER_MAX_FOUNDATION_TYPE; index ++)
	{
		int reward_count = LOGIC_CONFIG->GetCombineServerActivityConfig().GetFoundationRewardTimesCount(index);

		for (int reward_times = 0; reward_times < reward_count; reward_times ++)
		{
			CSAFoundationStatus &status = m_activity_info.csa_foundation_status[index];
			if (status.buy_level <= 0)
			{
				continue;
			}

			int delta_level = m_role->GetLevel() - status.buy_level;

			int reward_gold = LOGIC_CONFIG->GetCombineServerActivityConfig().GetFoundationRewardGold(index, status.reward_phase, delta_level);
			if (reward_gold <= 0)
			{
				continue;
			}

			total_reward_gold += reward_gold;
			++status.reward_phase;

			gamelog::g_log_combine_server.printf(LL_INFO, "CheckFoundationFetch user %d %s, index %d, reward_phase %d, reward_gold %d",
				m_role->GetUID(), m_role->GetName(), index, status.reward_phase - 1, reward_gold);
		}
	}

	if (total_reward_gold <= 0)
	{
		return;
	}

	static MailContentParam contentparam;
	contentparam.Reset();
	contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, total_reward_gold);

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_csa_foundation_auto_fetch_reward_content);
	if (length > 0)
	{
		//MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
	}
}

void RoleActivity::SendCSAFoundationInfo()
{
	static Protocol::SCCSAFoundationInfo pro;
	memcpy(pro.csa_foundation_status, m_activity_info.csa_foundation_status, sizeof(CSAFoundationStatus) * COMBINE_SERVER_MAX_FOUNDATION_TYPE);

	SEND_TO_ROLE(m_role, pro);
}

void RoleActivity::OnCSARefineOperaReq(int opera_type, int param)
{
	switch (opera_type)
	{
	case Protocol::CSA_EXP_REFINE_OPERA_TYPE_BUY_EXP:
	{
		this->OnCSARefineExpReq(param);
	}
	break;

	case Protocol::CSA_EXP_REFINE_OPERA_TYPE_GET_INFO:
	{
		this->SendCSAExpRefineInfo();
	}
	break;

	default:
		break;
	}
}

void RoleActivity::OnCSARefineExpReq(int seq)
{
	if (m_activity_info.csa_refine_exp_had_buy != 0)
	{
		m_role->NoticeNum(errornum::EN_CSA_TOUZIJIHUA_HAD_BUY);
		return;
	}

	const CSAOtherConfig &other_cfg = LOGIC_CONFIG->GetCombineServerActivityConfig().GetOtherCfg();
	if (seq < 0 || seq >= other_cfg.exp_refine_grades)
	{
		return;
	}

	int world_level = WorldShadow::Instance().GetWorldLevel();
	int role_level = m_role->GetLevel();

	// 检查是否能够购买
	int test_can_buy_level = world_level - role_level - other_cfg.exp_refine_level_limit;
	if (test_can_buy_level <= 0)
	{
		m_role->NoticeNum(errornum::EN_CSA_LEVEL_LIMIT);
		return;
	}

	int temp_delta_level = test_can_buy_level < other_cfg.exp_refine_max_level ? test_can_buy_level : other_cfg.exp_refine_max_level;
	int base_delta_level = temp_delta_level / other_cfg.exp_refine_grades;
	int delta_level = 0;

	// 最高档，直接是temp_delta_level
	if (seq + 1 == other_cfg.exp_refine_grades)
	{
		delta_level = temp_delta_level;
	}
	else if (0 == base_delta_level)
	{
		delta_level = temp_delta_level - (other_cfg.exp_refine_grades - seq - 1);
	}
	else
	{
		// 否则，按档位分配，向下取整
		delta_level = base_delta_level * (seq + 1);
	}

	// 不能超过等级上限
	if (role_level + delta_level >= MAX_ROLE_LEVEL_VERSION_LIMIT)
	{
		delta_level = MAX_ROLE_LEVEL_VERSION_LIMIT - role_level - 1;
	}

	int cost = LOGIC_CONFIG->GetCombineServerActivityConfig().GetExpRefineCost(role_level, delta_level);
	if (cost <= 0)
	{
		return;
	}

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cost, "RoleActivity::OnCSARefineExpReq"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 给予
	m_role->OnUpgradeLevel(role_level + delta_level);
	m_activity_info.csa_refine_exp_had_buy = 1;

	this->SendCSAExpRefineInfo();

	gamelog::g_log_combine_server.printf(LL_INFO, "RoleActivity::OnCSARefineExpReq user %d %s, world_level %d, old_level %d, delta_level %d, use_gold %d",
		m_role->GetUID(), m_role->GetName(), world_level, role_level, delta_level, cost);
}

void RoleActivity::SendCSAExpRefineInfo()
{
	Protocol::SCCSAExpRefineInfo efi;

	efi.had_buy = m_activity_info.csa_refine_exp_had_buy;
	efi.reserve_ch = 0;
	efi.reserve_sh = 0;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&efi, sizeof(efi));
}
