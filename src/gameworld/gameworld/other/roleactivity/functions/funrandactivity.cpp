#include "other/roleactivity/roleactivity.hpp"
#include "world.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "obj/character/role.h"
#include "obj/otherobj/gatherobj.h"
#include "obj/character/monster.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "monster/monsterpool.h"

#include "scene/scene.h"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "task/taskpool.h"
#include "gameworld/monster/drop/droppool.hpp"

#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"

#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "config/activityconfig/goldturntableconfig.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "config/activityconfig/opengameactivityconfig.hpp"

#include "config/sharedconfig/sharedconfig.h"

#include "global/randactivity/impl/randactivitytotalcharge4.hpp"

#include "protocal/msgactivity.hpp"
#include "protocal/msgrole.h"
#include "gameworld/gamelog.h"
#include "item/itempool.h"
#include "global/guild/guildmanager.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "gameworld/gameworld/global/guild/guild.hpp"
#include "gameworld/gameworld/global/usercache/usercache.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "global/rank/topranker.hpp"
#include "global/randactivity/impl/randactivityluckroll.hpp"
#include "global/randactivity/impl/randactivityserverpanicbuy.hpp"
#include "global/randactivity/impl/randactivityshouyouyuxiang.hpp"
#include "global/rank/rankmanager.hpp"
#include "global/randactivity/impl/randactivitycornucopia.hpp"
#include "global/randactivity/impl/randactivitylevellottery.hpp"
#include "global/randactivity/impl/randactivitychongzhiniuegg.hpp"
#include "global/randactivity/impl/randactivityzhenbaoge.hpp"
#include "global/randactivity/impl/randactivitymoneytree.hpp"
#include "global/randactivity/impl/randactivityspecialappearancepassiverank.hpp"
#include "global/randactivity/impl/randactivityspecialappearancerank.hpp"
#include "global/randactivity/impl/randactivitymine.hpp"
#include "global/randactivity/impl/randactivityarmyday.hpp"
#include "global/randactivity/impl/randactivitymoneytree2.hpp"
#include "global/randactivity/impl/randactivityzhenbaoge2.hpp"
#include "global/randactivity/impl/randactivitypromotingposition.hpp"
#include "global/randactivity/impl/randactivityblackmarket.hpp"
#include "global/randactivity/impl/randactivitymarryme.hpp"
#include "global/randactivity/impl/randactivitygoldenpig.hpp"
#include "global/randactivity/impl/randactivityrushbuying.hpp"
#include "global/randactivity/impl/randactivitytianmingdivination.hpp"
#include "global/randactivity/impl/randactivitylighttowerexplore.hpp"
#include "global/randactivity/impl/randactivitylottery1.hpp"
#include "gameworld/gameworld/item/itembase.h"
#include "gameworld/gameworld/item/gift/defgift.hpp"
#include "equipment/equipmentmanager.h"
#include "item/knapsack.h"
#include "other/rank/rankshadow.h"
#include "other/mentality/mentality.hpp"
#include "other/vip/vip.hpp"
#include "other/chestshop/rolechestshop.hpp"
#include "other/shop/roleshop.hpp"
#include "other/capability/capability.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/activedegree/activedegree.hpp"
#include "other/sublock/sublock.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "equipment/stonemananger.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/fb/rolepatafb.hpp"
#include "other/fb/roledailyfb.hpp"
#include "global/randactivity/impl/randactivityplantingtree.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "scene/map.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "global/randactivity/impl/randactivityguildbattle.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/footprint/footprint.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/fabao/fabaomanager.hpp"
#include "other/footprint/footprint.hpp"
#include "global/randactivity/impl/randactivityfanfan.hpp"
#include "gameworld/other/shizhuang/shizhuang.h"
#include "gameworld/other/fabao/fabaomanager.hpp"
#include "gameworld/other/footprint/footprint.hpp"
#include "gameworld/other/halo/halomanager.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "config/activityconfig/chongzhiconfig.hpp"
#include "gstr.h"
#include "global/randactivity/impl/randactivityguagua.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/mask/maskmanager.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/upgrade/upgrademanager.hpp"
#include "gameworld/config/activityconfig/randactivityopencfg.hpp"
#include "global/randactivity/impl/randactivitykingdraw.hpp"
#include "global/randactivity/impl/randactivityextremelucky.hpp"
#include "global/randactivity/impl/randactivitymaphunt.h"
#include "global/randactivity/impl/randactivityluckywish.hpp"
#include "global/randactivity/impl/randactivitygiftharvest.hpp"
#include "item/gift/selectgift.hpp"
#include "gameworld/scene/sceneregister/sceneregister.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialgiftharvest.hpp"
#include "config/activityconfig/giftharvestconfig.hpp"
#include "other/tianshuxunzhu/tianshuxunzhuconfig.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"
#include "global/randactivity/impl/randactivityluckycloudbuy.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "config/agentadapterconfig.hpp"
#include "global/randactivity/impl/randactivitycracybuy.hpp"

// --------------------------------------------------------------------------------------------------------
// 随机活动
// --------------------------------------------------------------------------------------------------------
// ------------------------------
// 巡游
// ------------------------------
void RoleActivity::OnMarryReserveReq(int reserve_index, int marry_level, int rune_index)
{

}

void RoleActivity::OnMarryXunyouOpera(int opera_type)
{

}

void RoleActivity::AddMarryXunyouMeiguiUseTimes(int times)
{

}

void RoleActivity::OnBuyXunyouOpera(int op_type)
{

}

void RoleActivity::SendXunyouOperaInfo()
{

}

// ------------------------------
// 约会
// ------------------------------
void RoleActivity::OnDatingInviteReq(const UserID &target_user_id, bool is_specific_invite)
{

}

void RoleActivity::OnDatingInviteCheckTarget(const UserID &req_user_id, bool is_specific_invite)
{

}

void RoleActivity::OnDatingInviteAck(const UserID &req_user_id, bool is_agree)
{

}

void RoleActivity::OnDatingConsume(ggprotocal::DatingConsume *dc)
{

}

void RoleActivity::OnDatingReward(ggprotocal::DatingReward *dr)
{

}

void RoleActivity::OnDatingUpdateAddExp(ggprotocal::DatingUpdateAddExp *duae)
{

}

void RoleActivity::OnDatingDaycountCheck(ggprotocal::DatingDaycountCheck *ddc)
{

}

void RoleActivity::OnMountUpgrade(int old_grade, int to_grade)
{
	if (to_grade > old_grade)
	{
		if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MOUNT_UPGRADE))
		{
			return;
		}

		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MOUNT_UPGRADE);
		this->CheckRAMountUpgradeReward();
	}
}

void RoleActivity::OnWingUpgrade(int old_grade, int to_grade)
{
	if (to_grade > old_grade)
	{
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_WING_UPGRADE))
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_WING_UPGRADE);
			this->CheckRAWingUpgradeReward();
		}
	}
}

void RoleActivity::OnVipQifu(int qifu_type, int qifu_times)
{
	if (qifu_type > QIFU_TYPE_INVALID && qifu_type < QIFU_TYPE_MAX)
	{
		this->RAQuanminQifuAddQifuTimes(qifu_times);
	}
}

void RoleActivity::OnJoinGuild(GuildID guild_id)
{
	this->SendGuildFbInfo();
	this->SyncGuildFbStatus();
	this->SyncGuildBeastInfo();
	this->SyncGuildBonfireStatus();
}

// ------------------------------
// 幸运转盘
// ------------------------------
void RoleActivity::OnRALuckyRollOperaReq(int opera_type)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LUCKY_ROLL))
	{
		m_role->NoticeNum(errornum::EN_CLOSE_LUCKYROLL_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LUCKY_ROLL);

	switch(opera_type)
	{
	case Protocol::RA_LUCKY_ROLL_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRALuckyRollActivityInfo();
		}
		break;

	case Protocol::RA_LUCKY_ROLL_OPERA_TYPE_ROLL:
		{
			// 抽奖逻辑
			this->OnRALuckyRollRoll();
		}
		break;

	case Protocol::RA_LUCKY_ROLL_OPERA_TYPE_10_COUNT_ROLL:
		{
			for (int i = 0; i < RAND_ACTIVITY_TEN_COUNT_ROLL; ++ i)
			{
				this->OnRALuckyRollRoll();
			}
		}
		break;

	case Protocol::RA_LUCKY_ROLL_OPERA_TYPE_FETCH_EXTRA_REWARD:
		{
			this->OnRALuckyRollFetchExtralReward();
		}
		break;

	default:
		break;
	}
}


void RoleActivity::OnRALuckyRollRoll()
{
	const RandActivityLuckyRollCfg *luckyroll_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyRollCfg();
	if (NULL == luckyroll_cfg)
	{
		return;
	}

	RandActivityLuckRoll *lucky_roll = RandActivityManager::Instance().GetRandActivityLuckRoll();
	if (NULL == lucky_roll)
	{
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(luckyroll_cfg->cfg.unit_roll_consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	int reward_seq = 0;

	const RandActivityLuckyRollRewardCfg *hit_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().RandLuckyRollYaojiangRewardCfg(lucky_roll->GetGoldPoll(), &reward_seq);
	if (NULL == hit_reward_cfg)
	{
		return;
	}

	long long reward_bind_gold = 0, old_gold_poll = lucky_roll->GetGoldPoll();
	if (LUCKY_ROLL_REWARD_TYPE_GOLD == hit_reward_cfg->reward_type)
	{
		reward_bind_gold = static_cast<long long>(lucky_roll->GetGoldPoll() * hit_reward_cfg->gold_percent * 1.0 / 100);
		if(reward_bind_gold <= 0)
		{
			return;
		}
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(luckyroll_cfg->cfg.unit_roll_consume_gold, "luckyroll_consume"))
	{
		return;
	}

	if (LUCKY_ROLL_REWARD_TYPE_GOLD == hit_reward_cfg->reward_type)
	{
		m_role->GetKnapsack()->GetMoney()->AddGold(reward_bind_gold, "luckyroll_receive");
		lucky_roll->AddGoldPoll(-reward_bind_gold);
	}
	else
	{
		m_role->GetKnapsack()->Put(hit_reward_cfg->reward_item, PUT_REASON_LUCKYROLL);
	}

	lucky_roll->AddGoldPoll(luckyroll_cfg->cfg.unit_enter_gold_pool_num);
	++ m_activity_info.lucky_roll_activity_roll_times;

	{
		// 日志
		gamelog::g_log_rand_activity.printf(LL_INFO, "OnLuckyRollActivityRoll::user[%d, %s], use_gold[%d], roll_times[%d], reward_type[%d], "
			"reward_gold_bind[%lld], old_gold_poll[%lld], new_gold_poll[%lld], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), luckyroll_cfg->cfg.unit_roll_consume_gold, m_activity_info.lucky_roll_activity_roll_times, hit_reward_cfg->reward_type,
			reward_bind_gold, old_gold_poll, lucky_roll->GetGoldPoll(), ItemConfigDataLog(&hit_reward_cfg->reward_item, NULL));
	}

	if (hit_reward_cfg->is_broadcast)
	{
		LuckyRollRewardRecord record;
		record.user_id = m_role->GetUID();
		m_role->GetName(record.user_name);
		record.reward_type = hit_reward_cfg->reward_type;
		record.num = hit_reward_cfg->reward_item.num;
		record.item_id = hit_reward_cfg->reward_item.item_id;
		record.gold = static_cast<int>(reward_bind_gold);
		record.time_stamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		lucky_roll->AddRewardRecord(record);

		//传闻
		{
			if (LUCKY_ROLL_REWARD_TYPE_GOLD == hit_reward_cfg->reward_type)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_luck_roll_reward_gold_notice, m_role->GetUID(),
					m_role->GetName(), (int)m_role->GetCamp(), static_cast<int>(reward_bind_gold));
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
				}
			}
			else
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_luck_roll_reward_item_notice, m_role->GetUID(),
					m_role->GetName(), (int)m_role->GetCamp(), (int)hit_reward_cfg->reward_item.item_id);
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
				}
			}
		}
	}

	this->SendRALuckyRollRollResult(reward_seq);
	this->SendRALuckyRollActivityInfo();
}


void RoleActivity::OnRALuckyRollFetchExtralReward()
{
	RandActivityLuckRoll *lucky_roll = RandActivityManager::Instance().GetRandActivityLuckRoll();
	if (NULL == lucky_roll)
	{
		return;
	}

	const RandActivityLuckyRollCfg *luckyroll_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyRollCfg();
	if (NULL == luckyroll_cfg)
	{
		return;
	}

	if (m_activity_info.lucky_roll_activity_roll_times < luckyroll_cfg->cfg.extra_reward_need_times)
	{
		m_role->NoticeNum(errornum::EN_LUCKYROLL_TIMES_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	int old_roll_times = m_activity_info.lucky_roll_activity_roll_times;

	m_activity_info.lucky_roll_activity_roll_times -= luckyroll_cfg->cfg.extra_reward_need_times;
	m_role->GetKnapsack()->Put(luckyroll_cfg->cfg.extra_reward_item, PUT_REASON_LUCKYROLL_EXTRAL);
	this->SendRALuckyRollActivityInfo();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnLuckyRollFetchExtralReward::user[%d, %s], old_roll_times[%d], new_roll_times[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), old_roll_times, m_activity_info.lucky_roll_activity_roll_times, ItemConfigDataLog(&luckyroll_cfg->cfg.extra_reward_item, NULL));
}

void RoleActivity::SendRALuckyRollActivityInfo()
{
	Protocol::SCLuckyRollActivityInfo lrai;
	lrai.roll_times = m_activity_info.lucky_roll_activity_roll_times;

	RandActivityLuckRoll *lucky_roll_activity = RandActivityManager::Instance().GetRandActivityLuckRoll();
	if (NULL != lucky_roll_activity)
	{
		lrai.gold_poll = static_cast<int>(lucky_roll_activity->GetGoldPoll());
		lrai.record_count = lucky_roll_activity->GetRewardRecordList(lrai.record_list);
	}

	int send_len = sizeof(lrai) - sizeof(lrai.record_list) + sizeof(lrai.record_list[0]) * lrai.record_count;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&lrai, send_len);
}

void RoleActivity::SendRALuckyRollRollResult(int reward_index)
{
	Protocol::SCLuckyRollActivityRollResult lrarr;
	lrarr.reward_index = reward_index;

	unsigned int send_len = sizeof(lrarr);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&lrarr, send_len);
}

// ------------------------------
// 全服疯狂抢购
// ------------------------------
void RoleActivity::OnRAServerPanicBuyOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SERVER_PANIC_BUY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SERVER_PANIC_BUY);

	switch(opera_type)
	{
	case Protocol::RA_SERVER_PANIC_BUY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAServerPanicBuyInfo();
		}
		break;

	case Protocol::RA_SERVER_PANIC_BUY_OPERA_TYPE_BUY_ITEM:
		{
			this->OnRAServerPanicBuyReq(param);
		}
		break;
	}
}

void RoleActivity::SendRAServerPanicBuyInfo()
{
	static Protocol::SCRAServerPanicBuyInfo spbai;

	RandActivityServerPanicBuy *rand_activity = RandActivityManager::Instance().GetRandActivityServerPanicBuy();
	if (NULL != rand_activity)
	{
		rand_activity->GetBuyItemNumList(spbai.server_buy_numlist);
	}

	memcpy(spbai.user_buy_numlist, m_activity_info.ra_server_panic_buy_numlist, sizeof(spbai.user_buy_numlist));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&spbai, sizeof(spbai));
}

void RoleActivity::OnRAServerPanicBuyReq(int item_idx)
{
	if (item_idx < 0 || item_idx >= RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT)
	{
		return;
	}

	const RandActivityServerPanicBuyCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetServerPanicBuyItemCfg(item_idx);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_server_panic_buy_numlist[item_idx] >= cfg_item->personal_limit_buy_count)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_PERSONAL_BUY_NUM_LIMIT);
		return;
	}

	RandActivityServerPanicBuy *rand_activity = RandActivityManager::Instance().GetRandActivityServerPanicBuy();
	if (NULL == rand_activity)
	{
		return;
	}

	if (rand_activity->GetBuyNum(item_idx) >= cfg_item->server_limit_buy_count)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_SERVER_BUY_NUM_LIMIT);
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(cfg_item->gold_price))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cfg_item->gold_price, "RandActivityServerPanicBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	rand_activity->OnBuyItem(item_idx);
	++ m_activity_info.ra_server_panic_buy_numlist[item_idx];

	// 给予
	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_SERVER_PANIC_BUY);

	this->SendRAServerPanicBuyInfo();
}

// ------------------------------
// 个人疯狂抢购
// ------------------------------
void RoleActivity::OnRAPersonalPanicBuyOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PERSONAL_PANIC_BUY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_PERSONAL_PANIC_BUY);

	switch(opera_type)
	{
	case Protocol::RA_PERSONAL_PANIC_BUY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAPersonalPanicBuyInfo();
		}
		break;

	case Protocol::RA_PERSONAL_PANIC_BUY_OPERA_TYPE_BUY_ITEM:
		{
			this->OnRAPersonalPanicBuyReq(param);
		}
		break;
	}
}

void RoleActivity::SendRAPersonalPanicBuyInfo()
{
	static Protocol::SCRAPersonalPanicBuyInfo ppbai;
	memcpy(ppbai.buy_numlist, m_activity_info.ra_personal_panic_buy_numlist, sizeof(ppbai.buy_numlist));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&ppbai, sizeof(ppbai));
}

void RoleActivity::OnRAPersonalPanicBuyReq(int item_idx)
{
	if (item_idx < 0 || item_idx >= RAND_ACTIVITY_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT)
	{
		return;
	}

	const RandActivityPersonalPanicBuyCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetPersonalPanicBuyItemCfg(item_idx);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_personal_panic_buy_numlist[item_idx] >= cfg_item->limit_buy_count)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_PERSONAL_BUY_NUM_LIMIT);
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(cfg_item->gold_price))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cfg_item->gold_price, "RandActivityPersonalPanicBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	{
		// 传闻;
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_panic_buy,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
		}
	}

	++ m_activity_info.ra_personal_panic_buy_numlist[item_idx];

	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_PERSONAL_PANIC_BUY);

	this->SendRAPersonalPanicBuyInfo();
}

// ------------------------------
// 单日充值返利
// ------------------------------
void RoleActivity::OnRADayChongZhiFanLiOperaReq(int opera_type,int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI);

	switch (opera_type)
	{
	case Protocol::RA_DAY_CHONGZHI_FANLI_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRADayChongZhiFanLiInfo();
		}
		break;

	case Protocol::RA_DAY_CHONGZHI_FANLI_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRADayChongZhiFanLiFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRADayChongZhiFanLiFetchReward(int reward_seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_DAY_CHONGZHI_FANLI_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_day_chongzhi_fanli_fetch_reward_flag) * 8);

	if (reward_seq < 0 || reward_seq >= RAND_ACTIVITY_DAY_CHONGZHI_FANLI_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_day_chongzhi_fanli_fetch_reward_flag & (1 << reward_seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityDayChongZhiFanLiCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetDayChongZhiFanLiRewardCfg(reward_seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_day_chongzhi_fanli_chongzhi_num < cfg_item->need_gold)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (NULL != ITEMPOOL->GetItem(cfg_item->reward_item.item_id))
	{
		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
	}

	// 消耗
	m_activity_info.ra_day_chongzhi_fanli_fetch_reward_flag |= (1 << reward_seq);

	// 结算
	{
		if (cfg_item->reward_gold > 0)
		{
			m_role->GetKnapsack()->GetMoney()->AddGold(cfg_item->reward_gold, "RandActivityDayChongzhifanli");
		}

		if (cfg_item->reward_bind_gold > 0)
		{
			m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg_item->reward_bind_gold, "RandActivityDayChongzhifanli");
		}

		if (NULL != ITEMPOOL->GetItem(cfg_item->reward_item.item_id))
		{
			m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_DAY_CHONGZHI_FANLI_REWARD);
		}
	}

	this->SendRADayChongZhiFanLiInfo();

	m_role->NoticeNum(noticenum::NT_RAND_ACTIVITY_FETCH_REWARD_SUCC);
}

void RoleActivity::RACheckDayChongZhiFanLiReward()
{
	if (m_activity_info.ra_day_chongzhi_fanli_chongzhi_num > 0)
	{
		int day_chongzhi_fanli_reward_count = LOGIC_CONFIG->GetRandActivityCfg().GetDayChongZhiFanLiRewardCfgCount();
		UNSTD_STATIC_CHECK(RAND_ACTIVITY_DAY_CHONGZHI_FANLI_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_day_chongzhi_fanli_fetch_reward_flag) * 8);

		for (int i = 0; i < day_chongzhi_fanli_reward_count; ++ i)
		{
			if (0 != (m_activity_info.ra_day_chongzhi_fanli_fetch_reward_flag & (1 << i)))
			{
				continue;
			}

			const RandActivityDayChongZhiFanLiCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetDayChongZhiFanLiRewardCfg(i);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_activity_info.ra_day_chongzhi_fanli_chongzhi_num < cfg_item->need_gold)
			{
				break;
			}

			static MailContentParam contentparam; contentparam.Reset();

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();
			}

			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, cfg_item->reward_gold);
			contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_BIND_GOLD, cfg_item->reward_bind_gold);

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_day_chongzhi_fanli_reward_content, RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI, cfg_item->need_gold);
			if (length > 0)
			{
				m_activity_info.ra_day_chongzhi_fanli_fetch_reward_flag |= (1 << i);

				//MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}

	m_activity_info.ra_day_chongzhi_fanli_chongzhi_num = 0;
	m_activity_info.ra_day_chongzhi_fanli_fetch_reward_flag = 0;
	this->SendRADayChongZhiFanLiInfo();
}

void RoleActivity::SendRADayChongZhiFanLiInfo()
{
	Protocol::SCRADayChongZhiFanLiInfo czflai;
	czflai.chongzhi_gold = m_activity_info.ra_day_chongzhi_fanli_chongzhi_num;
	czflai.fetch_reward_flag = m_activity_info.ra_day_chongzhi_fanli_fetch_reward_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&czflai, sizeof(czflai));
}

void RoleActivity::OnRADayChongZhiFanLiAddChongZhi(long long chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI);

	m_activity_info.ra_day_chongzhi_fanli_chongzhi_num += static_cast<int>(chongzhi);
	this->SendRADayChongZhiFanLiInfo();
}

// ------------------------------
// 单日消费
// ------------------------------
void RoleActivity::OnRADayConsumeGoldOperaReq(int opera_type,int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD);

	switch (opera_type)
	{
	case Protocol::RA_DAY_CONSUME_GOLD_TYPE_QUERY_INFO:
		{
			this->SendRADayConsumeGoldInfo();
		}
		break;

	case Protocol::RA_DAY_CONSUME_GOLD_TYPE_FETCH_REWARD:
		{
			this->OnRADayConsumeGoldFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRADayConsumeGoldFetchReward(int reward_seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_DAY_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_day_consume_gold_fetch_reward_flag) * 8);

	if (reward_seq < 0 || reward_seq >= RAND_ACTIVITY_DAY_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_day_consume_gold_fetch_reward_flag & (1 << reward_seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityDayConsumeGoldCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetDayConsumeGoldRewardCfg(reward_seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_day_consume_gold_consume_num < cfg_item->need_consume_gold)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 消耗
	m_activity_info.ra_day_consume_gold_fetch_reward_flag |= (1 << reward_seq);

	// 结算
	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_DAY_CONSUME_GOLD_REWARD);

	this->SendRADayConsumeGoldInfo();

	m_role->NoticeNum(noticenum::NT_RAND_ACTIVITY_FETCH_REWARD_SUCC);
}


void RoleActivity::SendRADayConsumeGoldInfo()
{
	Protocol::SCRADayConsumeGoldInfo dcgi;
	dcgi.consume_gold = m_activity_info.ra_day_consume_gold_consume_num;
	dcgi.fetch_reward_flag = m_activity_info.ra_day_consume_gold_fetch_reward_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&dcgi, sizeof(dcgi));
}

void RoleActivity::OnRADayConsumeGoldAddConsume(long long consume_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD);

	m_activity_info.ra_day_consume_gold_consume_num += static_cast<int>(consume_gold);
	this->SendRADayConsumeGoldInfo();
}

void RoleActivity::RACheckDayConsumeGoldReward()
{
	if (m_activity_info.ra_day_consume_gold_consume_num > 0)
	{
		int day_consume_gold_reward_count = LOGIC_CONFIG->GetRandActivityCfg().GetDayConsumeGoldRewardCfgCount();
		UNSTD_STATIC_CHECK(RAND_ACTIVITY_DAY_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_day_consume_gold_fetch_reward_flag) * 8);

		for(int i = 0; i < day_consume_gold_reward_count; ++ i)
		{
			if (0 != (m_activity_info.ra_day_consume_gold_fetch_reward_flag & (1 << i)))
			{
				continue;
			}

			const RandActivityDayConsumeGoldCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetDayConsumeGoldRewardCfg(i);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_activity_info.ra_day_consume_gold_consume_num < cfg_item->need_consume_gold)
			{
				break;
			}

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				static MailContentParam contentparam; contentparam.Reset();

				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_day_consume_gold_reward_content, RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD, cfg_item->need_consume_gold);
				if (length > 0)
				{
					m_activity_info.ra_day_consume_gold_fetch_reward_flag |= (1 << i);

					//MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
					MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
	}

	m_activity_info.ra_day_consume_gold_consume_num = 0;
	m_activity_info.ra_day_consume_gold_fetch_reward_flag = 0;

	this->SendRADayConsumeGoldInfo();
}

// ------------------------------
// 奇珍异宝
// ------------------------------
void RoleActivity::OnRAChestshopOperaReq(int opera_type,int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHESTSHOP))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHESTSHOP);

	switch(opera_type)
	{
	case Protocol::RA_CHESTSHOP_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAChestshopInfo();
		}
		break;

	case Protocol::RA_CHESTSHOP_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRAChestshopFetchReward(param);
		}
		break;
	}
}


void RoleActivity::SendRAChestshopInfo()
{
	Protocol::SCRAChestshopInfo csi;
	csi.chestshop_times = m_activity_info.ra_chestshop_xunbao_times;
	csi.fetch_reward_flag = m_activity_info.ra_chestshop_fetch_reward_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&csi, sizeof(csi));
}

void RoleActivity::OnRAChestshopFetchReward(int reward_seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_CHESTSHOP_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_chestshop_fetch_reward_flag) * 8);

	if (reward_seq < 0 || reward_seq >= RAND_ACTIVITY_CHESTSHOP_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_chestshop_fetch_reward_flag & (1 << reward_seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityChestShopCfg::ConfigItem *reward_cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetChestshopCfgBySeq(reward_seq);
	if (NULL == reward_cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_chestshop_xunbao_times < reward_cfg_item->need_chestshop_times)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_chestshop_fetch_reward_flag |= (1 << reward_seq);

	m_role->GetKnapsack()->Put(reward_cfg_item->reward_item, PUT_REASON_RA_CHESTSHOP_REWARD);

	this->SendRAChestshopInfo();
}

void RoleActivity::RAChestshopAddXunbaoTimes(int times)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHESTSHOP))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHESTSHOP);

	m_activity_info.ra_chestshop_xunbao_times += times;
	this->SendRAChestshopInfo();
}

// ------------------------------
// 单日活跃奖励
// ------------------------------
void RoleActivity::OnRADayActiveDegreeOperaReq(int opera_type,int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE);

	switch (opera_type)
	{
	case Protocol::RA_DAY_ACTIVE_DEGREE_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRADayActiveDegreeInfo();
		}
		break;

	case Protocol::RA_DAY_ACTIVE_DEGREE_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRADayActiveDegreeFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRADayActiveDegreeFetchReward(int reward_seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_DAY_ACTIVE_DEGREE_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_day_active_degree_fetch_reward_flag) * 8);

	if (reward_seq < 0 || reward_seq >= RAND_ACTIVITY_DAY_ACTIVE_DEGREE_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_day_active_degree_fetch_reward_flag & (1 << reward_seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityDayActiveDegreeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetDayActiveRewardRewardCfg(reward_seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_day_active_degree_active_count < cfg_item->need_active)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 消耗
	m_activity_info.ra_day_active_degree_fetch_reward_flag |= (1 << reward_seq);

	// 结算
	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_DAY_ACTIVE_DEGREE_REWARD);

	this->SendRADayActiveDegreeInfo();

	m_role->NoticeNum(noticenum::NT_RAND_ACTIVITY_FETCH_REWARD_SUCC);
}

void RoleActivity::SendRADayActiveDegreeInfo()
{
	Protocol::SCRADayActiveDegreeInfo dari;
	dari.active_degree = m_activity_info.ra_day_active_degree_active_count;
	dari.fetch_reward_flag = m_activity_info.ra_day_active_degree_fetch_reward_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&dari, sizeof(dari));
}

void RoleActivity::OnRADayActiveDegreeActiveDegreeChange()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE);

	// 	int total_active_degree = m_role->GetActiveDegree()->GetTodayCompeleteCount();
	//
	// 	if (total_active_degree > m_activity_info.ra_day_active_degree_active_count)
	// 	{
	// 		m_activity_info.ra_day_active_degree_active_count = total_active_degree;
	//
	// 		this->SendRADayActiveDegreeInfo();
	// 	}
}

void RoleActivity::RACheckDayActiveDegreeGiveReward()
{
	if (m_activity_info.ra_day_active_degree_active_count > 0)
	{
		int day_active_reward_reward_count = LOGIC_CONFIG->GetRandActivityCfg().GetDayActiveDegreeCfgRewardCfgCount();
		UNSTD_STATIC_CHECK(RAND_ACTIVITY_DAY_ACTIVE_DEGREE_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_day_active_degree_fetch_reward_flag) * 8);

		for(int i = 0; i < day_active_reward_reward_count; ++ i)
		{
			if (0 != (m_activity_info.ra_day_active_degree_fetch_reward_flag & (1 << i)))
			{
				continue;
			}

			const RandActivityDayActiveDegreeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetDayActiveRewardRewardCfg(i);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_activity_info.ra_day_active_degree_active_count < cfg_item->need_active)
			{
				break;
			}

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				static MailContentParam contentparam; contentparam.Reset();

				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_day_active_degree_reward_content, cfg_item->need_active);
				if (length > 0)
				{
					m_activity_info.ra_day_active_degree_fetch_reward_flag |= (1 << i);

					//MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
					MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
	}

	m_activity_info.ra_day_active_degree_active_count = 0;
	m_activity_info.ra_day_active_degree_fetch_reward_flag = 0;

	this->SendRADayActiveDegreeInfo();
}

void RoleActivity::SetRADayActivityDegree(int degree)
{
	m_activity_info.ra_day_active_degree_active_count = degree;

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE))
	{
		return;
	}

	this->SendRADayActiveDegreeInfo();
}

// ------------------------------
// 宝石升级
// ------------------------------
void RoleActivity::OnRAStoneUplevelOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_STONE_UPLEVEL))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_STONE_UPLEVEL);

	switch(opera_type)
	{
	case Protocol::RA_STONE_UPLEVEL_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAStoneUplevelInfo();
		}
		break;

	case Protocol::RA_STONE_UPLEVEL_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRAStoneUplevelFetchReward(param);
		}
		break;
	}
}

void RoleActivity::SendRAStoneUplevelInfo()
{
	Protocol::SCRAStoneUplevelInfo rasuli;
	rasuli.total_level = m_role->GetStoneManager()->GetTotalStoneLevel();
	rasuli.can_fetch_reward_flag = m_activity_info.ra_stone_uplevel_can_reward_flag;
	rasuli.fetch_reward_flag = m_activity_info.ra_stone_uplevel_fetch_reward_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&rasuli, sizeof(rasuli));
}

void RoleActivity::OnRAStoneUplevelFetchReward(int reward_seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_STONE_UPLEVEL_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_stone_uplevel_can_reward_flag) * 8);
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_STONE_UPLEVEL_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_stone_uplevel_fetch_reward_flag) * 8);

	if (reward_seq < 0 || reward_seq >= RAND_ACTIVITY_STONE_UPLEVEL_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_stone_uplevel_fetch_reward_flag & (LL_ONE_BIT << reward_seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const RandActivityStoneUplevelCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetStoneUplevelCfgBySeq(reward_seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_role->GetStoneManager()->GetTotalStoneLevel() < cfg_item->need_total_stone_level)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	m_activity_info.ra_stone_uplevel_fetch_reward_flag |= (LL_ONE_BIT << reward_seq);

	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_STONE_UPLEVEL_REWARD);

	this->SendRAStoneUplevelInfo();
}

void RoleActivity::CheckRAStoneUplevelReward()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_STONE_UPLEVEL))
	{
		return;
	}

	for (int i = 0; i < RAND_ACTIVITY_STONE_UPLEVEL_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 != (m_activity_info.ra_stone_uplevel_can_reward_flag & (LL_ONE_BIT << i)))
		{
			continue;
		}

		const RandActivityStoneUplevelCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetStoneUplevelCfgBySeq(i);
		if (NULL == cfg_item)
		{
			break;
		}

		if (m_role->GetStoneManager()->GetTotalStoneLevel() < cfg_item->need_total_stone_level)
		{
			break;
		}

		m_activity_info.ra_stone_uplevel_can_reward_flag |= (LL_ONE_BIT << cfg_item->seq);
	}

	this->SendRAStoneUplevelInfo();
}

// ------------------------------
// 坐骑进阶
// ------------------------------
void RoleActivity::OnRAMountUpgradeOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MOUNT_UPGRADE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MOUNT_UPGRADE);

	switch(opera_type)
	{
	case Protocol::RA_MOUNT_UPGRADE_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAMountUpgradeInfo();
		}
		break;

	case Protocol::RA_MOUNT_UPGRADE_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRAMountUpgradeFetchReward(param);
		}
		break;
	}
}

void RoleActivity::SendRAMountUpgradeInfo()
{
	Protocol::SCRAMountUpgradeInfo mugi;
	mugi.mount_grade = m_role->GetMountManager()->GetMountParam()->grade;
	mugi.can_fetch_reward_flag = m_activity_info.ra_mount_upgrade_can_reward_flag;
	mugi.fetch_reward_flag = m_activity_info.ra_mount_upgrade_fetch_reward_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mugi, sizeof(mugi));
}

void RoleActivity::OnRAMountUpgradeFetchReward(int reward_seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_MOUNT_UPGRADE_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_mount_upgrade_can_reward_flag) * 8);
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_MOUNT_UPGRADE_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_mount_upgrade_fetch_reward_flag) * 8);

	if (reward_seq < 0 || reward_seq >= RAND_ACTIVITY_MOUNT_UPGRADE_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_mount_upgrade_fetch_reward_flag & (1 << reward_seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const RandActivityMountUpgradeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetMountUpgradeCfgBySeq(reward_seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_role->GetMountManager()->GetMountParam()->grade < cfg_item->need_mount_grade)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	m_activity_info.ra_mount_upgrade_fetch_reward_flag |= (1 << reward_seq);

	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_MOUNT_UPGRADE_REWARD);

	this->SendRAMountUpgradeInfo();
}

void RoleActivity::CheckRAMountUpgradeReward()
{
	for (int i = 0; i < RAND_ACTIVITY_MOUNT_UPGRADE_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 != (m_activity_info.ra_mount_upgrade_can_reward_flag & (1 << i)))
		{
			continue;
		}

		const RandActivityMountUpgradeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetMountUpgradeCfgBySeq(i);
		if (NULL == cfg_item)
		{
			break;
		}

		MountManager *mount_mng = m_role->GetMountManager();
		if (NULL == mount_mng)
		{
			return;
		}

		MountParam *mount = mount_mng->GetMountParam();
		if (NULL == mount)
		{
			return;
		}

		if (mount->grade < cfg_item->need_mount_grade)
		{
			break;
		}

		m_activity_info.ra_mount_upgrade_can_reward_flag |= (1 << cfg_item->seq);

	}

	this->SendRAMountUpgradeInfo();
}

// ------------------------------
// 根骨全身等级
// ------------------------------
void RoleActivity::OnRAMentalityTotalLevelOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MENTALITY_TOTAL_LEVEL))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MENTALITY_TOTAL_LEVEL);

	switch(opera_type)
	{
	case Protocol::RA_MENTALITY_TOTAL_LEVEL_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAMentalityTotalLevelInfo();
		}
		break;

	case Protocol::RA_MENTALITY_TOTAL_LEVEL_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRAMentalityTotalLevelFetchReward(param);
		}
		break;
	}
}

void RoleActivity::SendRAMentalityTotalLevelInfo()
{
	Protocol::SCRAMentalityUplevelInfo muli;
	muli.total_mentality_level = m_role->GetMentality()->GetTotalGenguMaxLevel();
	muli.can_fetch_reward_flag = m_activity_info.ra_mentality_total_level_can_reward_flag;
	muli.fetch_reward_flag = m_activity_info.ra_mentality_total_level_fetch_reward_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&muli, sizeof(muli));
}

void RoleActivity::OnRAMentalityTotalLevelFetchReward(int reward_seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_MENTALITY_TOTAL_LEVEL_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_mentality_total_level_can_reward_flag) * 8);
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_MENTALITY_TOTAL_LEVEL_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_mentality_total_level_fetch_reward_flag) * 8);

	if (reward_seq < 0 || reward_seq >= RAND_ACTIVITY_MENTALITY_TOTAL_LEVEL_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_mentality_total_level_fetch_reward_flag & (1 << reward_seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_MENTALITY_HAS_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const RandActivityMentalityTotalLevelCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetMentalityTotalLevelCfgBySeq(reward_seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_role->GetMentality()->GetTotalGenguMaxLevel() < cfg_item->need_mentality_total_level)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	m_activity_info.ra_mentality_total_level_fetch_reward_flag |= (1 << reward_seq);

	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_MENTALITY_TOTAL_LEVEL_REWARD);

	this->SendRAMentalityTotalLevelInfo();
}

void RoleActivity::CheckRAMentalityTotalLevelReward()
{
	for (int i = 0; i < RAND_ACTIVITY_MENTALITY_TOTAL_LEVEL_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 != (m_activity_info.ra_mentality_total_level_can_reward_flag & (1 << i)))
		{
			continue;
		}

		const RandActivityMentalityTotalLevelCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetMentalityTotalLevelCfgBySeq(i);
		if (NULL == cfg_item)
		{
			break;
		}

		if (m_role->GetMentality()->GetTotalGenguMaxLevel() < cfg_item->need_mentality_total_level)
		{
			break;
		}

		m_activity_info.ra_mentality_total_level_can_reward_flag |= (1 << cfg_item->seq);
	}

	this->SendRAMentalityTotalLevelInfo();
}

// ------------------------------
// 羽翼进化
// ------------------------------
void RoleActivity::OnRAWingUpgradeOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_WING_UPGRADE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_WING_UPGRADE);

	switch(opera_type)
	{
	case Protocol::RA_WING_UPGRADE_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAWingUpgradeInfo();
		}
		break;

	case Protocol::RA_WING_UPGRADE_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRAWingUpgradeFetchReward(param);
		}
		break;
	}
}

void RoleActivity::SendRAWingUpgradeInfo()
{
	Protocol::SCRAWingUpgradeInfo wugi;
//	wugi.jinghua = m_role->GetWing()->GetJinghua();
	wugi.can_fetch_reward_flag = m_activity_info.ra_wing_upgrade_can_reward_flag;
	wugi.fetch_reward_flag = m_activity_info.ra_wing_upgrade_fetch_reward_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&wugi, sizeof(wugi));
}

void RoleActivity::OnRAWingUpgradeFetchReward(int reward_seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_WING_UPGRADE_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_wing_upgrade_can_reward_flag) * 8);
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_WING_UPGRADE_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_wing_upgrade_fetch_reward_flag) * 8);

	if (reward_seq < 0 || reward_seq >= RAND_ACTIVITY_WING_UPGRADE_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_wing_upgrade_fetch_reward_flag & (LL_ONE_BIT << reward_seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const RandActivityWingUpgradeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetWingUpgradeCfgBySeq(reward_seq);
	if (NULL == cfg_item)
	{
		return;
	}

// 	if (m_role->GetWing()->GetJinghua() < cfg_item->need_wing_grade)
// 	{
// 		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
// 		return;
// 	}

	m_activity_info.ra_wing_upgrade_fetch_reward_flag |= (LL_ONE_BIT << reward_seq);

	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_WING_UPGRADE_REWARD);

	this->SendRAWingUpgradeInfo();
}

void RoleActivity::CheckRAWingUpgradeReward()
{
	for (int i = 0; i < RAND_ACTIVITY_WING_UPGRADE_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 != (m_activity_info.ra_wing_upgrade_can_reward_flag & (LL_ONE_BIT << i)))
		{
			continue;
		}

		const RandActivityWingUpgradeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetWingUpgradeCfgBySeq(i);
		if (NULL == cfg_item)
		{
			break;
		}

// 		if (m_role->GetWing()->GetJinghua() < cfg_item->need_wing_grade)
// 		{
// 			break;
// 		}

		m_activity_info.ra_wing_upgrade_can_reward_flag |= (LL_ONE_BIT << cfg_item->seq);
	}

	this->SendRAWingUpgradeInfo();
}

// ------------------------------
// Boss击杀
// ------------------------------
void RoleActivity::OnRAKillBossOperaReq(int opera_type)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_KILL_BOSS))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_KILL_BOSS);

	switch (opera_type)
	{
	case Protocol::RA_KILL_BOSS_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAKillBossInfo();
		}
		break;

	case Protocol::RA_KILL_BOSS_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRAKillBossFetchReward();
		}
		break;
	}
}

void RoleActivity::OnRAKillBossFetchReward()
{
	if (m_activity_info.ra_kill_boss_kill_count <= 0)
	{
		return;
	}

	const RandActivityKillBossCfg &kill_boss_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetKillBossRewardCfg();

	if (m_activity_info.ra_kill_boss_kill_count < kill_boss_cfg.need_kill_boss_count)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 结算
	if (kill_boss_cfg.need_kill_boss_count <= 0) return;

	int can_fetch_num = m_activity_info.ra_kill_boss_kill_count / kill_boss_cfg.need_kill_boss_count;
	if (can_fetch_num <= 0)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(kill_boss_cfg.reward_item.item_id);
	if (NULL == item_base)
	{
		return;
	}

	ItemConfigData reward_item = kill_boss_cfg.reward_item;

	if (can_fetch_num >= item_base->GetPileLimit())
	{
		can_fetch_num = item_base->GetPileLimit();
	}

	reward_item.num = can_fetch_num;

	m_activity_info.ra_kill_boss_kill_count -= (kill_boss_cfg.need_kill_boss_count * can_fetch_num);

	m_role->GetKnapsack()->Put(reward_item, PUT_REASON_RA_KILL_BOSS_REWARD);

	this->SendRAKillBossInfo();

	m_role->NoticeNum(noticenum::NT_RAND_ACTIVITY_FETCH_REWARD_SUCC);
}

void RoleActivity::SendRAKillBossInfo()
{
	Protocol::SCRAKillBossInfo kbi;
	kbi.kill_count = m_activity_info.ra_kill_boss_kill_count;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&kbi, sizeof(kbi));
}

void RoleActivity::OnRAKillBossCountChange()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_KILL_BOSS))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_KILL_BOSS);

	m_activity_info.ra_kill_boss_kill_count += 1;

	this->SendRAKillBossInfo();
}

// ------------------------------
// 全民祈福
// ------------------------------
void RoleActivity::OnRAQuanminQifuOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_QUANMIN_QIFU))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_QUANMIN_QIFU);

	switch(opera_type)
	{
	case Protocol::RA_QUANMIN_QIFU_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAQuanminQifuInfo();
		}
		break;

	case Protocol::RA_QUANMIN_QIFU_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRAQuanminQifuFetchReward(param);
		}
		break;
	}
}

void RoleActivity::SendRAQuanminQifuInfo()
{
	Protocol::SCRAQuanminQifuInfo vqi;
	vqi.qifu_fetch_flag = m_activity_info.ra_quanmin_qifu_fetch_reward_flag;
	vqi.qifu_times = m_activity_info.ra_quanmin_qifu_qifu_times;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&vqi, sizeof(vqi));
}

void RoleActivity::OnRAQuanminQifuFetchReward(int reward_seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_VIP_QIFU_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_quanmin_qifu_fetch_reward_flag) * 8);

	if (reward_seq < 0 || reward_seq >= RAND_ACTIVITY_VIP_QIFU_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_quanmin_qifu_fetch_reward_flag & (1 << reward_seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityQuanminQifuCfg::ConfigItem *reward_cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetQuanminQifuCfgBySeq(reward_seq);
	if (NULL == reward_cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_quanmin_qifu_qifu_times < reward_cfg_item->need_qifu_times)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_quanmin_qifu_fetch_reward_flag |= (1 << reward_seq);

	m_role->GetKnapsack()->Put(reward_cfg_item->reward_item, PUT_REASON_RA_QUANMIN_QIFU_REWARD);

	this->SendRAQuanminQifuInfo();
}

void RoleActivity::RAQuanminQifuAddQifuTimes(int qifu_times)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_QUANMIN_QIFU))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_QUANMIN_QIFU);

	m_activity_info.ra_quanmin_qifu_qifu_times += qifu_times;
	this->SendRAQuanminQifuInfo();
}

// ------------------------------
// 手有余香
// ------------------------------
void RoleActivity::OnRAShouYouYuXiangOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG);  

	switch(opera_type)
	{
	case Protocol::RA_SHOUYOU_YUXIANG_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAShouYouYuXiangInfo();
		}
		break;

	case Protocol::RA_SHOUYOU_YUXIANG_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRAShouYouYuXiangFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRAShouYouYuXiangFetchReward(int reward_seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_SHOUYOU_YUXIANG_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_shouyou_yuxiang_fetch_reward_flag) * 8);

	if (0 == m_activity_info.ra_shouyou_yuxiang_give_flowers_flag)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_GIVE_FLOWER);
		return;
	}

	if (reward_seq < 0 || reward_seq >= RAND_ACTIVITY_SHOUYOU_YUXIANG_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_shouyou_yuxiang_fetch_reward_flag & (1 << reward_seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityShouYouYuXiangCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetShouYouYuXiangRewardCfg(reward_seq);
	if (NULL == cfg_item)
	{
		return;
	}

	RandActivityShouYouYuXiang *shouyou_yuxiang = RandActivityManager::Instance().GetRandActivityShouYouYuXiang();
	if (NULL == shouyou_yuxiang)
	{
		return;
	}

	if (shouyou_yuxiang->GetTotalFlowerNum() < cfg_item->need_flower)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 消耗
	m_activity_info.ra_shouyou_yuxiang_fetch_reward_flag |= (1 << reward_seq);

	// 结算
	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_SHOUYOU_YUXIANG_REWARD);

	this->SendRAShouYouYuXiangInfo();

	m_role->NoticeNum(noticenum::NT_RAND_ACTIVITY_FETCH_REWARD_SUCC);
}

void RoleActivity::SendRAShouYouYuXiangInfo()
{
	RandActivityShouYouYuXiang *shouyou_yuxiang = RandActivityManager::Instance().GetRandActivityShouYouYuXiang();
	if (NULL == shouyou_yuxiang)
	{
		return;
	}

	Protocol::SCRAShouYouYuXiangInfo syyxi;
	syyxi.shouyou_yuxiang_fetch_flag = m_activity_info.ra_shouyou_yuxiang_fetch_reward_flag;
	syyxi.shouyou_yuxiang_give_flower_flag = m_activity_info.ra_shouyou_yuxiang_give_flowers_flag;
	syyxi.shouyou_yuxiang_flower_num = shouyou_yuxiang->GetTotalFlowerNum();
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&syyxi, sizeof(syyxi));
}

void RoleActivity::OnRAShouYouYuXiangGiveFlower(int flower_num)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG);

	RandActivityShouYouYuXiang *shouyou_yuxiang = RandActivityManager::Instance().GetRandActivityShouYouYuXiang();
	if (NULL != shouyou_yuxiang)
	{
		shouyou_yuxiang->AddFlowerNum(flower_num);
	}

	m_activity_info.ra_shouyou_yuxiang_give_flowers_flag = 1;

	this->SendRAShouYouYuXiangInfo();
}

void RoleActivity::OnRAShouYouYuXiangOnDayChange()
{
	m_activity_info.ra_shouyou_yuxiang_fetch_reward_flag = 0;
	m_activity_info.ra_shouyou_yuxiang_give_flowers_flag = 0;
	this->SendRAShouYouYuXiangInfo();
}

// ------------------------------
// 比拼各种战力
// ------------------------------
void RoleActivity::OnRABipinSendForenotice()
{
	if (m_role->GetLevel() < 150)
	{
		return;
	}

	if (0 != m_activity_info.ra_today_has_send_bipin_mail)
	{
		return;
	}

	m_activity_info.ra_today_has_send_bipin_mail = 1;

	// 用户切换天的时候，根据开服第几天，通知用户
	int open_day_index = GetDayIndex(m_role->GetServerStartTime(), EngineAdapter::Instance().Time());
	if (open_day_index >= 1 && open_day_index <=  LOGIC_CONFIG->GetRandActivityOpenCfg().GetUpgradeActivityEndDay())
	{
		int ra_type = RandActivityManager::Instance().GetSevenDayBipinCurOpenType();
		if (-1 == ra_type) return;

		const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(ra_type, 1);
		if (NULL == cfg || 0 == cfg->reward_item_list[0].item_id)
		{
			return;
		}

		const ItemBase *item_base = ITEMPOOL->GetItem(cfg->reward_item_list[0].item_id);
		if (NULL != item_base && ItemBase::I_TYPE_GIFT == item_base->GetItemType())
		{
			// 确定是固定礼包
			GiftItem * gift_item = (GiftItem *)item_base;
			if (GiftItem::GIFT_TYPE_SELECT != gift_item->GetSubType()) return;

			const SelectGift * select_gift = dynamic_cast<const SelectGift*>(item_base);
			
			// 取礼包的倒数第二个物品id
			//int item_count = 0;
			//for (int i = 0; i < GiftItem::GIFT_ITEM_MAX_NUM; i++)
			//{
			//	ItemConfigData * gift_item_data = def_gift->GetGiftItemItem(i);
			//	if (NULL != gift_item_data && 0 != gift_item_data->item_id)
			//	{
			//		item_count++;
			//	}
			//}
			//
			//ItemConfigData * gift_item_data = def_gift->GetGiftItemItem(item_count - 2);

			// modify 2019-03-26 策划需求改为展示第一个物品
			//ItemConfigData * gift_item_data = def_gift->GetGiftItemItem(0);
			//if (NULL == gift_item_data || 0 == gift_item_data->item_id) return;
			
			// modify 2019-03-29 20:43:21 策划需求改为展示礼包
			if (nullptr == select_gift || 0 == select_gift->GetItemId())
			{
				return;
			}
			static MailContentParam contentparam; contentparam.Reset();

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_day_change_mail_content, ra_type, select_gift->GetItemId());
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}
}

void RoleActivity::OnRABipinCapTotalOperaReq(int opera_type, int param)
{
	int activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL;

	if (!ActivityShadow::Instance().IsRandActivityOpen(activity_type))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(activity_type);

	switch(opera_type)
	{
	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRABipinCapabilityInfo(activity_type);
		}
		break;

	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRABipinCapTotalFetchReward(param);
		}
		break;
	}
}


void RoleActivity::OnRABipinCapEquipOperaReq(int opera_type, int param)
{

	int activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP;

	if (!ActivityShadow::Instance().IsRandActivityOpen(activity_type))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(activity_type);

	switch(opera_type)
	{
	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRABipinCapabilityInfo(activity_type);
		}
		break;

	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRABipinCapEquipFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRABipinCapXianNvOperaReq(int opera_type, int param)
{

	int activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV;

	if (!ActivityShadow::Instance().IsRandActivityOpen(activity_type))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(activity_type);

	switch(opera_type)
	{
	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRABipinCapabilityInfo(activity_type);
		}
		break;

	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRABipinCapXianNvFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRABipinCapJingMaiOperaReq(int opera_type, int param)
{

	int activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI;

	if (!ActivityShadow::Instance().IsRandActivityOpen(activity_type))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	} 

	this->CheckInitRandActivity(activity_type);

	switch(opera_type)
	{
	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRABipinCapabilityInfo(activity_type);
		}
		break;

	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRABipinCapJingmaiFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRABipinCapMountOperaReq(int opera_type, int param)
{
	int activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT;

	if (!ActivityShadow::Instance().IsRandActivityOpen(activity_type))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(activity_type);

	switch(opera_type)
	{
	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRABipinCapabilityInfo(activity_type);
		}
		break;

	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRABipinCapMountFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRABipinCapWingOperaReq(int opera_type, int param)
{

	int activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING;

	if (!ActivityShadow::Instance().IsRandActivityOpen(activity_type))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(activity_type);

	switch(opera_type)
	{
	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRABipinCapabilityInfo(activity_type);
		}
		break;

	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRABipinCapWingFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRABipinCapShenzhuangOperaReq(int opera_type, int param)
{
	int activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG;

	if (!ActivityShadow::Instance().IsRandActivityOpen(activity_type))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(activity_type);

	switch(opera_type)
	{
	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRABipinCapabilityInfo(activity_type);
		}
		break;

	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRABipinCapShenzhuangFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRABipinCapJinglingOperaReq(int opera_type, int param)
{
	int activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING;

	if (!ActivityShadow::Instance().IsRandActivityOpen(activity_type))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(activity_type);

	switch(opera_type)
	{
	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRABipinCapabilityInfo(activity_type);
		}
		break;

	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRABipinCapJinglingFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRABipinCapWashlOperaReq(int opera_type, int param)
{
	int activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH;

	if (!ActivityShadow::Instance().IsRandActivityOpen(activity_type))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(activity_type);

	switch(opera_type)
	{
	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRABipinCapabilityInfo(activity_type);
		}
		break;

	case Protocol::RA_BIPIN_CAPABILITY_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRABipinCapWashlFetchReward(param);
		}
		break;
	}
}

void RoleActivity::SendRABipinCapabilityInfo(int activity_type)
{
	static Protocol::SCRABipinCapabilityInfo bpci;
	bpci.bipin_activity_type = activity_type;

	bool can_send = false;
	int rank_type = -1;

	switch(activity_type)
	{
	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL:
		{
			can_send = true;
			bpci.capability = m_activity_info.ra_bipin_cap_total_history_max_value;
			bpci.fetch_reward_flag = m_activity_info.ra_bipin_cap_total_fetch_reward_flag;
			rank_type = PERSON_RANK_TYPE_CAPABILITY_ALL;
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP:
		{
			can_send = true;
			bpci.capability = m_activity_info.ra_bipin_cap_equip_history_max_value;
			bpci.fetch_reward_flag = m_activity_info.ra_bipin_cap_equip_fetch_reward_flag;
			rank_type = PERSON_RANK_TYPE_EQUIP;
		}
		break;

	//case RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV:
	//	{
	//		can_send = true;
	//		bpci.capability = m_activity_info.ra_bipin_cap_xiannv_history_max_value;
	//		bpci.fetch_reward_flag = m_activity_info.ra_bipin_cap_xiannv_fetch_reward_flag;
	//		RankManager::Instance().GetPersonRank()->GetBipinTopUserInfo(activity_type, HISTORY_CAPABILITY_TYPE_XIANNV, bpci.all_camp_top_user_info_list);
	//	}
	//	break;

	//case RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI:
	//	{
	//		can_send = true;
	//		bpci.capability = m_activity_info.ra_bipin_cap_jingmai_history_max_value;
	//		bpci.fetch_reward_flag = m_activity_info.ra_bipin_cap_jingmai_fetch_reward_flag;
	//		RankManager::Instance().GetPersonRank()->GetBipinTopUserInfo(activity_type, HISTORY_CAPABILITY_TYPE_MENTALITY, bpci.all_camp_top_user_info_list);
	//	}
	//	break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT:
		{
			can_send = true;
			bpci.capability = m_activity_info.ra_bipin_cap_mount_history_max_value;
			bpci.fetch_reward_flag = m_activity_info.ra_bipin_cap_mount_fetch_reward_flag;
			rank_type = PERSON_RANK_TYPE_MOUNT;
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING:
		{
			can_send = true;
			bpci.capability = m_activity_info.ra_bipin_cap_wing_history_max_value;
			bpci.fetch_reward_flag = m_activity_info.ra_bipin_cap_wing_fetch_reward_flag;
			rank_type = PERSON_RANK_TYPE_WING;
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG:
		{
			can_send = true;
			const int rank_val1 = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENZHUANG);
			const int rank_val2 = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENZHOU_WEAPON);
			bpci.capability = rank_val1 + rank_val2;

			bpci.fetch_reward_flag = m_activity_info.ra_bipin_cap_shenzhuang_fetch_reward_flag;
			rank_type = PERSON_RANK_TYPE_CAPABILITY_SHENZHUANG;
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING:
		{
			can_send = true;
			bpci.capability =  m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING);
			bpci.fetch_reward_flag = m_activity_info.ra_bipin_cap_jingling_fetch_reward_flag;
			rank_type = PERSON_RANK_TYPE_CAPABILITY_JINGLING;
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH:
		{
			can_send = true;
			const int rank_val1 = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_MENTALITY);
			const int rank_val2 = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_GEM);
			const int rank_val3 = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_WASH);
			bpci.capability = rank_val1 + rank_val2 +rank_val3;
			bpci.fetch_reward_flag = m_activity_info.ra_bipin_cap_wash_fetch_reward_flag;
			rank_type = PERSON_RANK_TYPE_CAPABILITY_WASH;
		}
		break;
	}

	if (rank_type < 0)
	{
		return;
	}

	memset(bpci.top_user_info_list, 0, sizeof(bpci.top_user_info_list));
	this->GetBipinTopUserInfo(rank_type, Protocol::TOP_COUNT, bpci.top_user_info_list);

	if (can_send)
	{
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&bpci, sizeof(bpci));
	}
}

void RoleActivity::OnRABipinCapTotalFetchReward(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT) return;

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_total_fetch_reward_flag) * 8);

	if (0 != (m_activity_info.ra_bipin_cap_total_fetch_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL, seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_bipin_cap_total_history_max_value < cfg_item->need_capability)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
	if (NULL != item_base)
	{
		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
	}

	m_activity_info.ra_bipin_cap_total_fetch_reward_flag |= (1 << seq);

	if (NULL != item_base)
	{
		m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_BIPIN_CAPABILITY);
	}

	if (cfg_item->reward_gold_bind > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg_item->reward_gold_bind, "RABipinCapTotal");
	}

	this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRABipinCapTotalFetchReward::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), seq, m_activity_info.ra_bipin_cap_total_fetch_reward_flag,
		cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
}

void RoleActivity::OnRABipinCapEquipFetchReward(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT) return;

	int &cap_flag = m_activity_info.ra_bipin_cap_equip_fetch_reward_flag;
	int &cap_value = m_activity_info.ra_bipin_cap_equip_history_max_value;
	int activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP;

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(cap_flag) * 8);

	if (0 != (cap_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(activity_type, seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (cap_value < cfg_item->need_capability)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
	if (NULL != item_base)
	{
		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
	}

	cap_flag |= (1 << seq);

	if (NULL != item_base)
	{
		m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_BIPIN_CAPABILITY);
	}

	if (cfg_item->reward_gold_bind > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg_item->reward_gold_bind, "RABipinCapTotal");
	}

	this->SendRABipinCapabilityInfo(activity_type);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRABipinCapEquipFetchReward::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), seq, cap_flag,
		cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
}

void RoleActivity::OnRABipinCapXianNvFetchReward(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT) return;

	int &cap_flag = m_activity_info.ra_bipin_cap_xiannv_fetch_reward_flag;
	int &cap_value = m_activity_info.ra_bipin_cap_xiannv_history_max_value;
	int activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV;

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(cap_flag) * 8);

	if (0 != (cap_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(activity_type, seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (cap_value < cfg_item->need_capability)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
	if (NULL != item_base)
	{
		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
	}

	cap_flag |= (1 << seq);

	if (NULL != item_base)
	{
		m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_BIPIN_CAPABILITY);
	}

	if (cfg_item->reward_gold_bind > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg_item->reward_gold_bind, "RABipinCapTotal");
	}

	this->SendRABipinCapabilityInfo(activity_type);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRABipinCapXianNvFetchReward::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), seq, cap_flag,
		cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
}

void RoleActivity::OnRABipinCapJingmaiFetchReward(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT) return;

	int &cap_flag = m_activity_info.ra_bipin_cap_jingmai_fetch_reward_flag;
	int &cap_value = m_activity_info.ra_bipin_cap_jingmai_history_max_value;
	int activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI;

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(cap_flag) * 8);

	if (0 != (cap_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(activity_type, seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (cap_value < cfg_item->need_capability)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
	if (NULL != item_base)
	{
		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
	}

	cap_flag |= (1 << seq);

	if (NULL != item_base)
	{
		m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_BIPIN_CAPABILITY);
	}

	if (cfg_item->reward_gold_bind > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg_item->reward_gold_bind, "RABipinCapTotal");
	}

	this->SendRABipinCapabilityInfo(activity_type);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRABipinCapJingmaiFetchReward::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), seq, cap_flag,
		cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
}

void RoleActivity::OnRABipinCapWingFetchReward(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT) return;

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_wing_fetch_reward_flag) * 8);

	if (0 != (m_activity_info.ra_bipin_cap_wing_fetch_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING, seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_bipin_cap_wing_history_max_value < cfg_item->need_capability)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
	if (NULL != item_base)
	{
		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
	}

	m_activity_info.ra_bipin_cap_wing_fetch_reward_flag |= (1 << seq);

	if (NULL != item_base)
	{
		m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_BIPIN_CAPABILITY);
	}

	if (cfg_item->reward_gold_bind > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg_item->reward_gold_bind, "RABipinCapWing");
	}

	this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRABipinCapWingFetchReward::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), seq, m_activity_info.ra_bipin_cap_wing_fetch_reward_flag,
		cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
}

void RoleActivity::OnRABipinCapMountFetchReward(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT) return;

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_mount_fetch_reward_flag) * 8);

	if (0 != (m_activity_info.ra_bipin_cap_mount_fetch_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT, seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_bipin_cap_mount_history_max_value < cfg_item->need_capability)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
	if (NULL != item_base)
	{
		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
	}

	m_activity_info.ra_bipin_cap_mount_fetch_reward_flag |= (1 << seq);

	if (NULL != item_base)
	{
		m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_BIPIN_CAPABILITY);
	}

	if (cfg_item->reward_gold_bind > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg_item->reward_gold_bind, "RABipinCapMount");
	}

	this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRABipinCapMountFetchReward::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), seq, m_activity_info.ra_bipin_cap_mount_fetch_reward_flag,
		cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
}

void RoleActivity::OnRABipinCapShenzhuangFetchReward(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_shenzhuang_fetch_reward_flag) * 8);

	if (0 != (m_activity_info.ra_bipin_cap_shenzhuang_fetch_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG, seq);
	if (NULL == cfg_item)
	{
		return;
	}

	int sz_capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENZHUANG);
	int szlq_capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENZHOU_WEAPON);
	const int tottal_capability = sz_capability + szlq_capability;

	if (tottal_capability < cfg_item->need_capability)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
	if (NULL != item_base)
	{
		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
	}

	m_activity_info.ra_bipin_cap_shenzhuang_fetch_reward_flag |= (1 << seq);

	if (NULL != item_base)
	{
		m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_BIPIN_CAPABILITY);
	}

	if (cfg_item->reward_gold_bind > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg_item->reward_gold_bind, "RABipinCapShenzhuang");
	}

	this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRABipinCapShenzhuangFetchReward::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), seq, m_activity_info.ra_bipin_cap_shenzhuang_fetch_reward_flag,
		cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
}

void RoleActivity::OnRABipinCapJinglingFetchReward(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_jingling_fetch_reward_flag) * 8);

	if (0 != (m_activity_info.ra_bipin_cap_jingling_fetch_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING, seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING) < cfg_item->need_capability)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
	if (NULL != item_base)
	{
		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
	}

	m_activity_info.ra_bipin_cap_jingling_fetch_reward_flag |= (1 << seq);

	if (NULL != item_base)
	{
		m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_BIPIN_CAPABILITY);
	}

	if (cfg_item->reward_gold_bind > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg_item->reward_gold_bind, "RABipinCapJingling");
	}

	this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRABipinCapJinglingFetchReward::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), seq, m_activity_info.ra_bipin_cap_jingling_fetch_reward_flag,
		cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
}

void RoleActivity::OnRABipinCapWashlFetchReward(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_wash_fetch_reward_flag) * 8);

	if (0 != (m_activity_info.ra_bipin_cap_wash_fetch_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH, seq);
	if (NULL == cfg_item)
	{
		return;
	}

	const int rank_val1 = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_MENTALITY);
	const int rank_val2 = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_GEM);
	const int rank_val3 = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_WASH);

	if ((rank_val1 + rank_val2 +rank_val3) < cfg_item->need_capability)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
	if (NULL != item_base)
	{
		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
	}

	m_activity_info.ra_bipin_cap_wash_fetch_reward_flag |= (1 << seq);

	if (NULL != item_base)
	{
		m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_BIPIN_CAPABILITY);
	}

	if (cfg_item->reward_gold_bind > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg_item->reward_gold_bind, "RABipinCapWash");
	}

	this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRABipinCapWashlFetchReward::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), seq, m_activity_info.ra_bipin_cap_mount_fetch_reward_flag,
		cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
}

void RoleActivity::GetBipinTopUserInfo(int rank_type, const int max_user, Protocol::TopUserInfo topuser_list[Protocol::TOP_COUNT])
{
	if (rank_type < PERSON_RANK_TYPE_CAPABILITY_ALL || rank_type >= PERSON_RANK_TYPE_MAX || max_user > PERSON_RANK_MAX_SIZE)
	{
		return;
	}

	int activity_type = -1;
	switch(rank_type)
	{
	case PERSON_RANK_TYPE_CAPABILITY_ALL:
		{
			activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL;
		}
		break;

	case PERSON_RANK_TYPE_EQUIP:
		{
			activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP;
		}
		break;

	case PERSON_RANK_TYPE_MOUNT:
		{
			activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT;
		}
		break;

	case PERSON_RANK_TYPE_WING:
		{
			activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING;
		}
		break;

	case PERSON_RANK_TYPE_CAPABILITY_SHENZHUANG:
		{
			activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG;
		}
		break;

	case PERSON_RANK_TYPE_CAPABILITY_JINGLING:
		{
			activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING;
		}
		break;

	case PERSON_RANK_TYPE_CAPABILITY_WASH:
		{
			activity_type = RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH;
		}
		break;
	}

	if (activity_type < 0)
	{
		return;
	}

	UserID user_id_list[PERSON_RANK_MAX_SIZE];
	int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopUser(rank_type, max_user, user_id_list);

	const RandActivityRankBipinCfg *bipin_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(activity_type);
	if (NULL == bipin_cfg)
	{
		return;
	}

	if (!RandActivityManager::Instance().IsSyncRankData(activity_type))
	{
		PersonRankItem rank_item[Protocol::TOP_COUNT];
		RandActivityManager::Instance().GetRandActivityBipinRank3(activity_type, rank_item);

		for (int i = 0; i < rank_count && i < Protocol::TOP_COUNT && i < max_user; ++ i)
		{
			if (rank_item[i].rank_value < bipin_cfg->need_capability)
			{
				continue;
			}

			topuser_list[i].uid = UidToInt(rank_item[i].user_id);
			topuser_list[i].prof = rank_item[i].prof;
			UNSTD_STATIC_CHECK(sizeof(topuser_list[i].name) == sizeof(rank_item[i].user_name));
			memcpy(topuser_list[i].name, rank_item[i].user_name, sizeof(topuser_list[i].name));
			topuser_list[i].capability = rank_item[i].rank_value;
			topuser_list[i].avatar_timestamp = rank_item[i].flexible_ll;
		}
	}

	else
	{
		for (int i = 0; i < rank_count && i < Protocol::TOP_COUNT && i < max_user; ++ i)
		{
			const PersonRankItem *rank_item = RankManager::Instance().GetPersonRank()->GetPersonRankItem(rank_type, user_id_list[i]);
			if (NULL == rank_item)
			{
				continue;
			}

			if (rank_item->rank_value < bipin_cfg->need_capability)
			{
				continue;
			}

			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(rank_item->user_id);
			if (NULL == user_node)
			{
				continue;
			}

			topuser_list[i].uid = UidToInt(rank_item->user_id);
			topuser_list[i].prof = rank_item->prof;
			UNSTD_STATIC_CHECK(sizeof(topuser_list[i].name) == sizeof(rank_item->user_name));
			memcpy(topuser_list[i].name, rank_item->user_name, sizeof(topuser_list[i].name));
			topuser_list[i].capability = rank_item->rank_value;
			topuser_list[i].avatar_timestamp = user_node->avatar_timestamp;
		}
	}
}

void RoleActivity::GmResetBipinData()
{
	m_activity_info.ra_bipin_cap_total_begin_timestamp = 0;
	m_activity_info.ra_bipin_cap_total_fetch_reward_flag = 0;
	m_activity_info.ra_bipin_cap_equip_begin_timestamp = 0;
	m_activity_info.ra_bipin_cap_equip_fetch_reward_flag = 0;
	m_activity_info.ra_bipin_cap_mount_begin_timestamp = 0;
	m_activity_info.ra_bipin_cap_mount_fetch_reward_flag = 0;
	m_activity_info.ra_bipin_cap_wing_begin_timestamp = 0;
	m_activity_info.ra_bipin_cap_wing_fetch_reward_flag = 0;
	m_activity_info.ra_bipin_cap_shenzhuang_begin_timestamp = 0;
	m_activity_info.ra_bipin_cap_shenzhuang_fetch_reward_flag = 0;
	m_activity_info.ra_bipin_cap_jingling_begin_timestamp = 0;
	m_activity_info.ra_bipin_cap_jingling_fetch_reward_flag = 0;
	m_activity_info.ra_bipin_cap_wing_begin_timestamp = 0;
	m_activity_info.ra_bipin_cap_wash_fetch_reward_flag = 0;
}

void RoleActivity::OnRACappabilityChange(int cap_type)
{
	{
		// 随机活动比拼总战力
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL))
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL);

			const HistoryCapItem *history_cap_item = m_role->GetHistoryMaxCapability(HISTORY_CAPABILITY_TYPE_TAOTAL);
			if (NULL != history_cap_item)
			{
				if (history_cap_item->max_cap > m_activity_info.ra_bipin_cap_total_history_max_value)
				{
					m_activity_info.ra_bipin_cap_total_history_max_value = history_cap_item->max_cap;
					this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL);

					const RandActivityRankBipinCfg *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL);
					if (NULL != cfg_item && history_cap_item->max_cap >= cfg_item->need_capability)
					{
						m_role->GetRankShadow()->ForceSyncData();

						PersonRank *person_rank = RankManager::Instance().GetPersonRank();
						if (NULL != person_rank)
						{
							person_rank->ForceSnapshot();
						}
					}
				}
			}
		}
	}

	if (HISTORY_CAPABILITY_TYPE_EQUIPMENT == Capability::GetHistoryType(cap_type))
	{
		// 随机活动比拼装备战力
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP))
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP);

			const HistoryCapItem *history_cap_item = m_role->GetHistoryMaxCapability(HISTORY_CAPABILITY_TYPE_EQUIPMENT);
			if (NULL != history_cap_item)
			{
				if (history_cap_item->max_cap > m_activity_info.ra_bipin_cap_equip_history_max_value)
				{
					m_activity_info.ra_bipin_cap_equip_history_max_value = history_cap_item->max_cap;
					this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP);

					const RandActivityRankBipinCfg *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP);
					if (NULL != cfg_item && history_cap_item->max_cap >= cfg_item->need_capability)
					{
						m_role->GetRankShadow()->ForceSyncData();

						PersonRank *person_rank = RankManager::Instance().GetPersonRank();
						if (NULL != person_rank)
						{
							person_rank->ForceSnapshot();
						}
					}
				}
			}
		}
	}

	if (HISTORY_CAPABILITY_TYPE_XIANNV == Capability::GetHistoryType(cap_type))
	{
		// 随机活动比拼仙女战力
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV))
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV);

			const HistoryCapItem *history_cap_item = m_role->GetHistoryMaxCapability(HISTORY_CAPABILITY_TYPE_XIANNV);
			if (NULL != history_cap_item)
			{
				if (history_cap_item->max_cap > m_activity_info.ra_bipin_cap_xiannv_history_max_value)
				{
					m_activity_info.ra_bipin_cap_xiannv_history_max_value = history_cap_item->max_cap;
					this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV);

					const RandActivityRankBipinCfg *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV);
					if (NULL != cfg_item && history_cap_item->max_cap >= cfg_item->need_capability)
					{
						m_role->GetRankShadow()->ForceSyncData();

						PersonRank *person_rank = RankManager::Instance().GetPersonRank();
						if (NULL != person_rank)
						{
							person_rank->ForceSnapshot();
						}
					}
				}
			}
		}
	}

	if (HISTORY_CAPABILITY_TYPE_MENTALITY == Capability::GetHistoryType(cap_type))
	{
		// 随机活动比拼经脉战力
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI))
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI);

			const HistoryCapItem *history_cap_item = m_role->GetHistoryMaxCapability(HISTORY_CAPABILITY_TYPE_MENTALITY);
			if (NULL != history_cap_item)
			{
				if (history_cap_item->max_cap > m_activity_info.ra_bipin_cap_jingmai_history_max_value)
				{
					m_activity_info.ra_bipin_cap_jingmai_history_max_value = history_cap_item->max_cap;
					this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI);

					const RandActivityRankBipinCfg *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI);
					if (NULL != cfg_item && history_cap_item->max_cap >= cfg_item->need_capability)
					{
						m_role->GetRankShadow()->ForceSyncData();

						PersonRank *person_rank = RankManager::Instance().GetPersonRank();
						if (NULL != person_rank)
						{
							person_rank->ForceSnapshot();
						}
					}
				}
			}
		}
	}

	if (HISTORY_CAPABILITY_TYPE_WING == Capability::GetHistoryType(cap_type))
	{
		// 随机活动比拼羽翼战力
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING))
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING);

			const HistoryCapItem *history_cap_item = m_role->GetHistoryMaxCapability(HISTORY_CAPABILITY_TYPE_WING);
			if (NULL != history_cap_item)
			{
				if (history_cap_item->max_cap > m_activity_info.ra_bipin_cap_wing_history_max_value)
				{
					m_activity_info.ra_bipin_cap_wing_history_max_value = history_cap_item->max_cap;
					this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING);

					const RandActivityRankBipinCfg *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING);
					if (NULL != cfg_item && history_cap_item->max_cap >= cfg_item->need_capability)
					{
						m_role->GetRankShadow()->ForceSyncData();

						PersonRank *person_rank = RankManager::Instance().GetPersonRank();
						if (NULL != person_rank)
						{
							person_rank->ForceSnapshot();
						}
					}
				}
			}
		}
	}

	if (HISTORY_CAPABILITY_TYPE_MOUNT == Capability::GetHistoryType(cap_type))
	{
		// 随机活动比坐骑装战力
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT))
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT);

			const HistoryCapItem *history_cap_item = m_role->GetHistoryMaxCapability(HISTORY_CAPABILITY_TYPE_MOUNT);
			if (NULL != history_cap_item)
			{
				if (history_cap_item->max_cap > m_activity_info.ra_bipin_cap_mount_history_max_value)
				{
					m_activity_info.ra_bipin_cap_mount_history_max_value = history_cap_item->max_cap;
					this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT);

					const RandActivityRankBipinCfg *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT);
					if (NULL != cfg_item && history_cap_item->max_cap >= cfg_item->need_capability)
					{
						m_role->GetRankShadow()->ForceSyncData();

						PersonRank *person_rank = RankManager::Instance().GetPersonRank();
						if (NULL != person_rank)
						{
							person_rank->ForceSnapshot();
						}
					}
				}
			}
		}
	}

	if (HISTORY_CAPABILITY_TYPE_SHENZHUANG == Capability::GetHistoryType(cap_type))
	{
		// 随机活动比拼神装战力
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG))
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG);

			const HistoryCapItem *history_cap_item = m_role->GetHistoryMaxCapability(HISTORY_CAPABILITY_TYPE_SHENZHUANG);
			if (NULL != history_cap_item)
			{
				if (history_cap_item->max_cap > m_activity_info.ra_bipin_cap_shenzhuang_history_max_value)
				{
					m_activity_info.ra_bipin_cap_shenzhuang_history_max_value = history_cap_item->max_cap;
					this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG);

					const RandActivityRankBipinCfg *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG);
					if (NULL != cfg_item && history_cap_item->max_cap >= cfg_item->need_capability)
					{
						m_role->GetRankShadow()->ForceSyncData();

						PersonRank *person_rank = RankManager::Instance().GetPersonRank();
						if (NULL != person_rank)
						{
							person_rank->ForceSnapshot();
						}
					}
				}
			}
		}
	}

	if (HISTORY_CAPABILITY_TYPE_JINGLING == Capability::GetHistoryType(cap_type))
	{
		// 随机活动比拼精灵战力
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING))
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING);

			const HistoryCapItem *history_cap_item = m_role->GetHistoryMaxCapability(HISTORY_CAPABILITY_TYPE_JINGLING);
			if (NULL != history_cap_item)
			{
				if (history_cap_item->max_cap > m_activity_info.ra_bipin_cap_jingling_history_max_value)
				{
					m_activity_info.ra_bipin_cap_jingling_history_max_value = history_cap_item->max_cap;
					this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING);

					const RandActivityRankBipinCfg *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING);
					if (NULL != cfg_item && history_cap_item->max_cap >= cfg_item->need_capability)
					{
						m_role->GetRankShadow()->ForceSyncData();

						PersonRank *person_rank = RankManager::Instance().GetPersonRank();
						if (NULL != person_rank)
						{
							person_rank->ForceSnapshot();
						}
					}
				}
			}
		}
	}

	if (HISTORY_CAPABILITY_TYPE_WASH == Capability::GetHistoryType(cap_type))
	{
		// 随机活动比拼精炼战力
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH))
		{
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH);

			const HistoryCapItem *history_cap_item = m_role->GetHistoryMaxCapability(HISTORY_CAPABILITY_TYPE_WASH);
			if (NULL != history_cap_item)
			{
				if (history_cap_item->max_cap > m_activity_info.ra_bipin_cap_wash_history_max_value)
				{
					m_activity_info.ra_bipin_cap_wash_history_max_value = history_cap_item->max_cap;
					this->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH);

					const RandActivityRankBipinCfg *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetRankBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH);
					if (NULL != cfg_item && history_cap_item->max_cap >= cfg_item->need_capability)
					{
						m_role->GetRankShadow()->ForceSyncData();

						PersonRank *person_rank = RankManager::Instance().GetPersonRank();
						if (NULL != person_rank)
						{
							person_rank->ForceSnapshot();
						}
					}
				}
			}
		}
	}
}

// ------------------------------
// 充值回馈
// ------------------------------
void RoleActivity::OnRAChargeRepaymentOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT);

	switch(opera_type)
	{
	case Protocol::RA_CHARGE_REPAYMENT_OPERA_TYPE_QUERY_INFO:
		{
			this->SendChargeRepaymentInfo();
		}
		break;

	case Protocol::RA_CHARGE_REPAYMENT_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRAChargeRepaymentFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRAChargeRepaymentFetchReward(int seq)
{
	const RandActivityChargeRepaymentCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetChargeRepaymentCfgWithSeq(seq);
	if (NULL == cfg_item)
	{
		m_role->NoticeNum(errornum::EN_CHARGE_REPAYMENT_NO_CFG);
		return;
	}

	if (0 == (m_activity_info.ra_charge_repayment_reward_active_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_CHARGE_REPAYMENT_NOT_ACTIVE);
		return;
	}

	if (0 != (m_activity_info.ra_charge_repayment_reward_fetch_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_CHARGE_REPAYMENT_HAS_FETCHED);
		return;
	}

	{
		// 扣除次数
		m_activity_info.ra_charge_repayment_reward_fetch_flag |= (1 << seq);

		// 发放奖励
		m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg_item->bind_gold_repayment, "ChargeRepaymentFetchReward");

		this->SendChargeRepaymentInfo();
	}
}

void RoleActivity::OnRAChargeRewardActive(long long gold_num)
{
	if (gold_num <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT);

	m_activity_info.ra_charge_repayment_history_charge += static_cast<int>(gold_num);

	const RandActivityChargeRepaymentCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetChargeRepaymentCfgWithCharge(m_activity_info.ra_charge_repayment_history_charge);
	if (NULL == cfg_item)
	{
		this->SendChargeRepaymentInfo(); // 累积充值数改变了，要通知客户端
		return;
	}

	for (int seq = cfg_item->seq; seq >= 0; -- seq)
	{
		if (0 == (m_activity_info.ra_charge_repayment_reward_active_flag & (1 << seq)))
		{
			m_activity_info.ra_charge_repayment_reward_active_flag |= (1 << seq);
		}
	}

	this->SendChargeRepaymentInfo();
}

void RoleActivity::SendChargeRepaymentInfo()
{
	static Protocol::SCChargeRewardInfo cri;
	cri.reward_active_flag = m_activity_info.ra_charge_repayment_reward_active_flag;
	cri.reward_fetch_flag = m_activity_info.ra_charge_repayment_reward_fetch_flag;
	cri.history_charge_during_act = m_activity_info.ra_charge_repayment_history_charge;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cri, sizeof(cri));
}

// ------------------------------
// 聚宝盆
// ------------------------------
void RoleActivity::OnRACornucopiaOperaReq(int opera_type)
{
	int activity_type = RAND_ACTIVITY_TYPE_CORNUCOPIA;

	if (!ActivityShadow::Instance().IsRandActivityOpen(activity_type))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(activity_type);

	switch(opera_type)
	{
	case Protocol::RA_CORNUCOPIA_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRACornucopiaInfo();
		}
		break;

	case Protocol::RA_CORNUCOPIA_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRAFetchCornucopiaReward();
		}
		break;

	case Protocol::RA_CORNUCOPIA_OPERA_TYPE_FETCH_REWARD_INFO:
		{
			this->OnRAFetchCornucopiaRewardInfo();
		}
		break;
	}
}

void RoleActivity::SendRACornucopiaInfo()
{
	RandActivityCornucopia *ra_cornucopia = RandActivityManager::Instance().GetRandActivityCornucopia(RAND_ACTIVITY_TYPE_CORNUCOPIA);
	if (NULL != ra_cornucopia)
	{
		static Protocol::SCRACornucopiaFetchInfo cwmfmi;
		cwmfmi.history_chongzhi = m_activity_info.ra_cornucopia_history_charge;
		cwmfmi.reward_lun = m_activity_info.ra_cornucopia_current_lun;
		cwmfmi.record_count = ra_cornucopia->GetRewardRecordList(cwmfmi.record_list);

		int send_len = sizeof(cwmfmi) - sizeof(cwmfmi.record_list[0]) * (RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT - cwmfmi.record_count);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cwmfmi, send_len);
	}
}

void RoleActivity::OnRAFetchCornucopiaReward()
{
	const RandActivityCornucopiaCfg *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetCornucopiaCfg(m_activity_info.ra_cornucopia_current_lun);
	if (NULL == cfg_item)
	{
		m_role->NoticeNum(errornum::EN_ALL_REWARD_HAS_FETCHED);
		return;
	}

	if (cfg_item->need_total_charge > m_activity_info.ra_cornucopia_history_charge) // 当前充值额度未达到下级奖励要求
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_WANT_MONEY_HAS_NOT_ENOUGH_CHARGE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(cfg_item->need_put_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}
	
	if (0 == m_activity_info.current_lun_reward_gold && cfg_item->max_reward_gold - cfg_item->min_reward_gold + 1 > 0)
	{
		m_activity_info.current_lun_reward_gold = cfg_item->min_reward_gold + RandomNum(cfg_item->max_reward_gold - cfg_item->min_reward_gold + 1);
	}
	else {
		return;
	}

	if (m_activity_info.current_lun_reward_gold <= 0)
	{
		return;
	}

	m_role->GetKnapsack()->GetMoney()->UseGold(cfg_item->need_put_gold, "RACornucopiaFetchReward", false);

	Protocol::SCRACornucopiaFetchReward msg;
	msg.get_reward_gold = m_activity_info.current_lun_reward_gold;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, sizeof(msg));
}

void RoleActivity::OnRAFetchCornucopiaRewardInfo()
{
	const RandActivityCornucopiaCfg *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetCornucopiaCfg(m_activity_info.ra_cornucopia_current_lun);
	if (NULL == cfg_item)
	{
		m_role->NoticeNum(errornum::EN_ALL_REWARD_HAS_FETCHED);
		return;
	}

	if (cfg_item->need_total_charge > m_activity_info.ra_cornucopia_history_charge) // 当前充值额度未达到下级奖励要求
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_WANT_MONEY_HAS_NOT_ENOUGH_CHARGE);
		return;
	}

	if (0 == m_activity_info.current_lun_reward_gold)
	{
		return;
	}

	RandActivityCornucopia *ra_cornucopia = RandActivityManager::Instance().GetRandActivityCornucopia(RAND_ACTIVITY_TYPE_CORNUCOPIA);
	if (NULL == ra_cornucopia)
	{
		return;
	}

	int old_lun = m_activity_info.ra_cornucopia_current_lun;

	++m_activity_info.ra_cornucopia_current_lun;

	m_role->GetKnapsack()->GetMoney()->AddGold(m_activity_info.current_lun_reward_gold, "RACornucopiaReward");

	if (cfg_item->is_broadcast)
	{
		{
			// 保存记录
			static RACornucopiaRewardRecord record; record.Reset();
			record.user_id = m_role->GetUID();
			m_role->GetName(record.user_name);
			record.reward_gold = static_cast<short>(m_activity_info.current_lun_reward_gold);
			record.camp = m_role->GetCamp();
			record.need_put_gold = cfg_item->need_put_gold;
			ra_cornucopia->AddRewardRecord(record);
		}

		{
			// 传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_cornucopia_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), m_activity_info.current_lun_reward_gold);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}

		this->SendRACornucopiaInfo();
	}

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAFetchCornucopiaReward::user[%d, %s], lun[%d], reward_gold[%d]",
		m_role->GetUID(), m_role->GetName(), old_lun, m_activity_info.current_lun_reward_gold);

	m_activity_info.current_lun_reward_gold = 0;
}
// ------------------------------
// 新聚宝盆
// ------------------------------
void RoleActivity::OnRANewCornucopiaOperaReq(int opera_type, int param)
{
	if (0 == opera_type)
	{
		this->SendRANewCornucopiaInfo();
	}
	else
	{
		this->OnRAFetchNewCornucopiaReward(param);
	}
}

void RoleActivity::SendRANewCornucopiaInfo()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_CORNUCOPIA))
	{
		return;
	}

	this->NewCornucopiaUpdateTaskInfo();
	m_activity_info.new_cornucopia_day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_NEW_CORNUCOPIA), EngineAdapter::Instance().Time());

	Protocol::SCRANewCornucopiaInfo pro;
	pro.cornucopia_value = m_activity_info.new_cornucopia_value;
	pro.total_cornucopia_value = WorldStatus::Instance().GetCommonData().new_cornucopia_total_value;
	pro.cornucopia_day_index = m_activity_info.new_cornucopia_day_index;
	pro.cornucopia_total_reward_flag = m_activity_info.new_cornucopia_total_reward_flag;
	memcpy(pro.new_cornucopia_task_value_list, m_activity_info.new_cornucopia_task_value_list, sizeof(pro.new_cornucopia_task_value_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void RoleActivity::OnRAFetchNewCornucopiaReward(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_CORNUCOPIA))
	{
		return;
	}

	const RandActivityNewCornucopiaTotalConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityNewCornucopiaTotalConfig(seq);
	if (NULL == cfg)
	{
		return;
	}

	if (WorldStatus::Instance().GetCommonData().new_cornucopia_total_value < cfg->cornucopia_value)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CORNUCOPIA_VALUE_LIMIT);
		return;
	}

	if (m_activity_info.new_cornucopia_total_reward_flag & (1 << seq))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->Put(cfg->reward_item, PUT_REASON_NEW_CORNUCOPIA))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.new_cornucopia_total_reward_flag |= (1 << seq);

	this->SendRANewCornucopiaInfo();
}

void RoleActivity::NewCornucopiaUpdateTaskInfo()
{
	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_MOUNT_UPGRADE] = (char)m_role->GetMountManager()->GetMountParam()->grade;
	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_HALO_UPGRADE] = (char)m_role->GetHalo()->GetHaloParam()->grade;
	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_WING_UPGRADE] = (char)m_role->GetWing()->GetWingParam()->grade;
	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_SHENGONG_UPGRADE] = (char)m_role->GetShengong()->GetShengongParam()->grade;
	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_SHENYI_UPGRADE] = (char)m_role->GetShenyi()->GetShenyiParam()->grade;
}

void RoleActivity::OnAttackWorldBoss()
{
	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_WORLD_BOSS] = 1;
}

void RoleActivity::OnCompleteDailyTask(int complete_times)
{
	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_DAILY_TASK] += (char)complete_times;
}

void RoleActivity::OnCompleteGuildTask()
{
	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_GUILD_TASK]++;
}

void RoleActivity::OnEnterCrossTeamFb()
{
	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_CROSS_TEAM_FB]++;
}

void RoleActivity::OnAcceptHusongTask()
{
	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_HUSONG]++;
}

void RoleActivity::OnEnterCross1v1()
{
	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_CROSS_1V1]++;
}

void RoleActivity::OnEnterPhaseFB()
{
	m_activity_info.new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_UPGRADE_FB]++;
}

void RoleActivity::GmSetNewCornucopiaTaskValue(int cornucopia_task_type, int value)
{
	if (cornucopia_task_type < 0 || cornucopia_task_type >= RA_NEW_CORNUCOPIA_TASK_TYPE_MAX)
	{
		return;
	}

	m_activity_info.new_cornucopia_task_value_list[cornucopia_task_type] = (char)value;
	this->SendRANewCornucopiaInfo();
}

// ------------------------------
// 单笔充值（不同天数奖励不同）
// ------------------------------
void RoleActivity::OnRADanbiChongzhiOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DANBI_CHONGZHI))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DANBI_CHONGZHI);

	switch(opera_type)
	{
	case Protocol::RA_DANBI_CHONGZHI_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRADanbiChongzhiInfo();
		}
		break;

	case Protocol::RA_DANBI_CHONGZHI_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRADanbiChongzhiFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRADanbiChongzhiCheckChongzhi(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DANBI_CHONGZHI))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DANBI_CHONGZHI);

	int day_index = GetDayIndex(ActivityShadow::Instance().GetActivityBeginTime(RAND_ACTIVITY_TYPE_DANBI_CHONGZHI), EngineAdapter::Instance().Time());
	const RandActivityDanbiChongzhiRewardCfg *danbi_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetDanbiChongzhiRewardByChongzhi(day_index, chongzhi);

	if (NULL != danbi_cfg)
	{
		this->m_activity_info.ra_danbi_chongzhi_reward_can_fetch_flag |= (1 << danbi_cfg->reward_seq);

		this->SendRADanbiChongzhiInfo();
	}
}

void RoleActivity::SendRADanbiChongzhiInfo()
{
	static Protocol::SCRADanbiChongzhiInfo dbcz;
	dbcz.reward_can_fetch_flag = m_activity_info.ra_danbi_chongzhi_reward_can_fetch_flag;
	dbcz.reward_fetch_flag = m_activity_info.ra_danbi_chongzhi_reward_has_fetch_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&dbcz, sizeof(dbcz));
}

void RoleActivity::OnRADanbiChongzhiFetchReward(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_DANBI_CHONGZHI_REWARD_CFG_MAX_COUNT_PER_DAY)
	{
		return;
	}

	if (0 == (m_activity_info.ra_danbi_chongzhi_reward_can_fetch_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_DANBI_CHONGZHI_REWARD_HAS_NOT_ACTIVE);
		return;
	}

	if (0 != (m_activity_info.ra_danbi_chongzhi_reward_has_fetch_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_DANBI_CHONGZHI_REWARD_HAS_FETCH);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_DANBI_CHONGZHI), EngineAdapter::Instance().Time());
	const RandActivityDanbiChongzhiRewardCfg *danbi_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetDanbiChongzhiRewardBySeq(day_index, seq);
	if (NULL == danbi_cfg)
	{
		return;
	}

	// 消耗
	m_activity_info.ra_danbi_chongzhi_reward_has_fetch_flag |= (1 << seq);

	// 发奖励
	m_role->GetKnapsack()->Put(danbi_cfg->reward_item, PUT_REASON_RA_DANBI_CHONGZHI_REWARD);

	this->SendRADanbiChongzhiInfo();

	// 传闻
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chongzhi_danbi_chongzhi_reward_content,
		m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), danbi_cfg->need_chongzhi_num, danbi_cfg->reward_item.item_id);
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
	}

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRADanbiChongzhiFetchReward::user[%d, %s], seq[%d], reward_flag[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), seq, m_activity_info.ra_danbi_chongzhi_reward_has_fetch_flag,
		ItemConfigDataLog(&danbi_cfg->reward_item, NULL));
}

// ------------------------------
// 累计充值（天数）活动
// ------------------------------
void RoleActivity::OnRATotalChargeDayOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY);

	switch(opera_type)
	{
	case Protocol::RA_TOTAL_CHONGZHI_DAY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRATotalChargeDayInfo();
		}
		break;

	case Protocol::RA_TOTAL_CHONGZHI_DAY_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRATotalChargeDayFetchReward(param);
		}
		break;
	}
}

void RoleActivity::OnRATotalChargeDayCheckChongzhi(long long chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY);

	if (0 != m_activity_info.ra_total_charge_day_today_has_charged)
	{
		return;
	}

	m_activity_info.ra_total_charge_day_today_has_charged = 1;

	if (m_activity_info.ra_total_charge_day_count < 127)
	{
		++ m_activity_info.ra_total_charge_day_count;
	}

	SendRATotalChargeDayInfo();
}

void RoleActivity::SendRATotalChargeDayInfo()
{
	static Protocol::SCRATotalChargeDayInfo sc_info;
	sc_info.charge_day_count = m_activity_info.ra_total_charge_day_count;
	sc_info.reward_has_fetch_flag = m_activity_info.ra_total_charge_day_reward_has_fetch_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&sc_info, sizeof(sc_info));
}

void RoleActivity::OnRATotalChargeDayFetchReward(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	if (seq < 0 || seq > RAND_ACTIVITY_TOTAL_CHARGE_DAY_CFG_MAX_SEQ)
	{
		return;
	}

	if (0 != (m_activity_info.ra_total_charge_day_reward_has_fetch_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RA_TOTAL_CHARGE_DAY_REWARD_HAS_FETCH);
		return;
	}

	const RandActivityTotalChargeDayCfg::TotalChargeDayItem *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTotalChargeDayCfgWithSeq(seq);
	if (NULL == cfg)
	{
		m_role->NoticeNum(errornum::EN_RA_TOTAL_CHARGE_DAY_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (m_activity_info.ra_total_charge_day_count < cfg->need_recharge_day)
	{
		m_role->NoticeNum(errornum::EN_RA_TOTAL_CHARGE_DAY_REWARD_NOT_ACTIVE);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg->reward_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_total_charge_day_reward_has_fetch_flag |= (1 << seq);
	m_role->GetKnapsack()->PutList(cfg->reward_count, cfg->reward_item_list, PUT_REASON_RA_TOTAL_CHARGE_DAY);

	SendRATotalChargeDayInfo();

	std::string reward_item_list_str;
	ItemConfigDataListLog(reward_item_list_str, cfg->reward_count, cfg->reward_item_list);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRATotalChargeDayFetchReward::user[%d, %s], seq[%d], reward_flag[%d], reward_item_list[%s]",
		m_role->GetUID(), m_role->GetName(), seq, m_activity_info.ra_total_charge_day_reward_has_fetch_flag,
		reward_item_list_str.c_str());
}


void RoleActivity::OnRATomorrwRewardOperaReq(int opera_type)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOMORROW_REWARD))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TOMORROW_REWARD);

	switch(opera_type)
	{
	case Protocol::RA_TOMORROW_REWARD_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRATomorrowRewardInfo();
		}
		break;

	case Protocol::RA_TOMORROW_REWARD_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRATomorrowRewardFetchReward();
		}
		break;
	}
}

void RoleActivity::SendRATomorrowRewardInfo(int reward_index)
{
	Protocol::SCRATomorrowRewardInfo scratrif;
	scratrif.reward_count = m_activity_info.ra_tomorrow_reward_count;
	scratrif.reward_index = reward_index;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scratrif, sizeof(scratrif));
}

void RoleActivity::OnRATomorrowRewardFetchReward()
{
	if (m_activity_info.ra_tomorrow_reward_count <= 0)
	{
		m_role->NoticeNum(errornum::EN_RA_TOMORROW_REWARD_COUNT_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const RandActivityTomorrowRewardCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTomorrowRandomRewardCfg();
	if (NULL == reward_cfg)
	{
		return;
	}

	// 消耗，生效
	if (m_role->GetKnapsack()->Put(reward_cfg->reward_item, PUT_REASON_RA_TOMORROW_REWARD))
	{
		-- m_activity_info.ra_tomorrow_reward_count;

		SendRATomorrowRewardInfo(reward_cfg->index);

		if (reward_cfg->is_broadcast) // 传闻
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_tomorrow_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), reward_cfg->reward_item.item_id);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
			}
		}

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRATomorrowRewardFetchReward::user[%d, %s] tomorrow_reward_count[%d] reward_index[%d] reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), m_activity_info.ra_tomorrow_reward_count, reward_cfg->index, ItemConfigDataLog(&reward_cfg->reward_item, NULL));
	}
}

// ------------------------------
// 充值双倍奖励
// ------------------------------
void RoleActivity::OnRADoubleCharge(long long gold_num)
{
	int charge_num = static_cast<int>(gold_num);

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_DOUBLE_2))
	{
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_DOUBLE_2);

		// 双倍充值，发放奖励
		int precent = 0;

		if (1 == m_activity_info.ra_charge_reward_is_first)
		{
			precent =  LOGIC_CONFIG->GetRandActivityCfg().GetChargeRewardPrecent(0, charge_num);
		}
		else
		{
			precent =  LOGIC_CONFIG->GetRandActivityCfg().GetChargeRewardPrecent(1, charge_num);
		}

		m_activity_info.ra_charge_reward_is_first = 0;

		static MailContentParam contentparam; contentparam.Reset();

		if (precent > 0)
		{
			contentparam.gold = (charge_num * precent) / 100;
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_double_charge_content, charge_num, contentparam.gold);
		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_rand_activity.printf(LL_INFO, "OnRADoubleCharge::user[%d, %s], charge_reward_gold[%d]",
				m_role->GetUID(), m_role->GetName(), contentparam.gold);
		}
	}
}

// ------------------------------
// 充值排行
// ------------------------------
void RoleActivity::OnRAChongzhiRankChongzhi(int gold_num)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_RANK))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHONGZHI_RANK);

	m_activity_info.ra_chongzhi_rank_chongzhi_num += gold_num;
	m_activity_info.ra_chongzhi_rank_chongzhi_last_timestamp = static_cast<int>(EngineAdapter::Instance().Time());
	this->SendRAChongzhiRankInfo();
}

void RoleActivity::SendRAChongzhiRankInfo()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_RANK))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHONGZHI_RANK);

	Protocol::SCRAChongzhiRankInfo czai;
	czai.chongzhi_num = m_activity_info.ra_chongzhi_rank_chongzhi_num;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&czai, sizeof(czai));
}

// ------------------------------
// 充值排行2
// ------------------------------
void RoleActivity::OnRAChongzhiRank2Chongzhi(int gold_num)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_RANK2))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHONGZHI_RANK2);

	m_activity_info.ra_chongzhi_rank2_chongzhi_num += gold_num;
	m_activity_info.ra_chongzhi_rank2_chongzhi_last_timestamp = static_cast<int>(EngineAdapter::Instance().Time());
	this->SendRAChongzhiRank2Info();
}

void RoleActivity::SendRAChongzhiRank2Info()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_RANK2))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHONGZHI_RANK2);

	Protocol::SCRAChongzhiRank2Info czai;
	czai.chongzhi_num = m_activity_info.ra_chongzhi_rank2_chongzhi_num;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&czai, sizeof(czai));
}

// ------------------------------
// 消费排行
// ------------------------------
void RoleActivity::OnRAConsumeGoldRankConsumeGold(int consume_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK);

	m_activity_info.ra_consume_gold_rank_consume_gold_num += consume_gold;
	m_activity_info.ra_consume_gold_rank_consume_last_timestamp = static_cast<int>(EngineAdapter::Instance().Time());
	this->SendRAConsumeGoldRankInfo();
}

void RoleActivity::SendRAConsumeGoldRankInfo()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK);

	Protocol::SCRAConsumeGoldRankInfo cai;
	cai.consume_gold_num = m_activity_info.ra_consume_gold_rank_consume_gold_num;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cai, sizeof(cai));
}

// ------------------------------
// 每日消费排行
// ------------------------------
void RoleActivity::OnRADayConsumeRankConsumeGold(int gold_num)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_CONSUME_RANK))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAY_CONSUME_RANK);

	m_activity_info.ra_day_consume_rank_gold_num += gold_num;
	m_activity_info.ra_day_consume_rank_consume_last_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	this->SendRADayConsumeRankInfo();

	{
		m_role->GetRankShadow()->ForceSyncData();

		PersonRank *person_rank = RankManager::Instance().GetPersonRank();
		if (NULL != person_rank)
		{
			person_rank->ForceSnapshot();
		}
	}
}

void RoleActivity::SendRADayConsumeRankInfo()
{
	Protocol::SCRADayConsumeRankInfo radcri;
	radcri.gold_num = m_activity_info.ra_day_consume_rank_gold_num;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&radcri, sizeof(radcri));
}

void RoleActivity::RAResetDayConsumeRankGoldNum()
{
	m_activity_info.ra_day_consume_rank_gold_num = 0;
	this->SendRADayConsumeRankInfo();
	PersonRank* person_rank = RankManager::Instance().GetPersonRank();
	if (nullptr != person_rank)
	{
		person_rank->OnGetRealTimeRankList(m_role, PERSON_RANK_TYPE_RA_DAY_CONSUME_NUM);
	}
}

// ------------------------------
// 消费返利
// ------------------------------
void RoleActivity::OnRAConsumeGoldFanliConsumeGold(int consume_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUME_GOLD_FANLI))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONSUME_GOLD_FANLI);

	m_activity_info.ra_consume_gold_fanli_consume_gold += consume_gold;
	this->SendRAConsumeGoldFanliInfo();
}

void RoleActivity::SendRAConsumeGoldFanliInfo()
{
	Protocol::SCRAConsumeGoldFanliInfo flai;
	flai.consume_gold = m_activity_info.ra_consume_gold_fanli_consume_gold;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&flai, sizeof(flai));
}

// ------------------------------
// 登录奖励
// ------------------------------
void RoleActivity::OnRALoginGiftOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LOGIN_GIFT))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LOGIN_GIFT);

	switch(opera_type)
	{
	case Protocol::RA_LOGIN_GIFT_OPERA_TYPE_INFO:
		{
			this->SendRALoginGiftInfo();
		}
		break;

	case Protocol::RA_LOGIN_GIFT_OPERA_TYPE_FETCH_COMMON_REWARD:
		{
			this->OnRALoginGiftFetchCommonReward(param);
		}
		break;

	case Protocol::RA_LOGIN_GIFT_OPERA_TYPE_FETCH_VIP_REWARD:
		{
			this->OnRALoginGiftFetchVipReward(param);
		}
		break;

	case Protocol::RA_LOGIN_GIFT_OPERA_TYPE_FETCH_ACCUMULATE_REWARD:
		{
			this->OnRALoginGiftFetchAccumulateReward();
		}
		break;
	}
}

void RoleActivity::OnRALoginGiftLogin()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LOGIN_GIFT))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LOGIN_GIFT);

	if (EngineAdapter::Instance().DayID() != m_activity_info.ra_login_gift_last_login_dayid)
	{
		m_activity_info.ra_login_gift_last_login_dayid = EngineAdapter::Instance().DayID();
		++ m_activity_info.ra_login_gift_login_days;
		this->SendRALoginGiftInfo();
	}
}

void RoleActivity::SendRALoginGiftInfo()
{
	Protocol::SCRALoginGiftInfo raligi;
	raligi.login_days = m_activity_info.ra_login_gift_login_days;
	raligi.has_login = m_activity_info.ra_login_gift_last_login_dayid;
	raligi.has_fetch_accumulate_reward = m_activity_info.ra_login_gift_has_fetch_accumulate_reward;
	raligi.fetch_common_reward_flag = m_activity_info.ra_login_gift_fetch_common_reward_flag;
	raligi.fetch_vip_reward_flag = m_activity_info.ra_login_gift_fetch_vip_reward_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&raligi, sizeof(raligi));
}

void RoleActivity::OnRALoginGiftFetchCommonReward(int seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_login_gift_fetch_common_reward_flag) * 8);

	if (seq < 0 || seq >= RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_login_gift_fetch_common_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityLoginGiftCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetLoginGiftRewardCfg(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_login_gift_login_days < cfg_item->need_login_days)
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_login_gift_fetch_common_reward_flag |= (1 << seq);

	m_role->GetKnapsack()->Put(cfg_item->common_reward_item, PUT_REASON_RA_LOGIN_GIFT_REWARD);

	this->SendRALoginGiftInfo();
}

void RoleActivity::OnRALoginGiftFetchVipReward(int seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_login_gift_fetch_vip_reward_flag) * 8);

	if (seq < 0 || seq >= RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	int vip_level = m_role->GetVip()->GetVipLevel();
	int can_fetch_reward = (0 != LOGIC_CONFIG->GetVipConfig().GetAuthParam(vip_level, VAT_RA_LOGIN_GIFT_VIP_REWARD));

	if (!can_fetch_reward)
	{
		m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGH);
		return;
	}

	if (0 != (m_activity_info.ra_login_gift_fetch_vip_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityLoginGiftCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetLoginGiftRewardCfg(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_login_gift_login_days < cfg_item->need_login_days)
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_login_gift_fetch_vip_reward_flag |= (1 << seq);

	m_role->GetKnapsack()->Put(cfg_item->vip_reward_item, PUT_REASON_RA_LOGIN_GIFT_REWARD);

	this->SendRALoginGiftInfo();
}

void RoleActivity::OnRALoginGiftFetchAccumulateReward()
{
	if (0 != m_activity_info.ra_login_gift_has_fetch_accumulate_reward)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityLoginGiftCfg *login_gift_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLoginGiftCfg();
	if (NULL == login_gift_cfg)
	{
		return;
	}

	if (m_activity_info.ra_login_gift_login_days < login_gift_cfg->need_accumulate_days)
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_login_gift_has_fetch_accumulate_reward = 1;

	m_role->GetKnapsack()->Put(login_gift_cfg->accumulate_reward, PUT_REASON_RA_LOGIN_GIFT_REWARD);

	this->SendRALoginGiftInfo();
}

// ------------------------------
// 登录奖励 2217
// ------------------------------
void RoleActivity::OnRALoginGiftOperaReq_0(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LOGIN_GIFT_0))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LOGIN_GIFT_0);

	switch (opera_type)
	{
	case Protocol::RA_LOGIN_GIFT_OPERA_TYPE_INFO:
	{
		this->SendRALoginGiftInfo_0();
	}
	break;

	case Protocol::RA_LOGIN_GIFT_OPERA_TYPE_FETCH_COMMON_REWARD:
	{
		this->OnRALoginGiftFetchCommonReward_0(param);
	}
	break;

	case Protocol::RA_LOGIN_GIFT_OPERA_TYPE_FETCH_VIP_REWARD:
	{
		//this->OnRALoginGiftFetchVipReward_0(param);
	}
	break;
	}
}

void RoleActivity::OnRALoginGiftLogin_0()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LOGIN_GIFT_0))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LOGIN_GIFT_0);

	if (EngineAdapter::Instance().DayID() != m_activity_info.ra_login_gift_last_login_dayid_0)
	{
		m_activity_info.ra_login_gift_last_login_dayid_0 = EngineAdapter::Instance().DayID();
		++m_activity_info.ra_login_gift_login_days_0;
	}

	this->SendRALoginGiftInfo_0();
}

void RoleActivity::SendRALoginGiftInfo_0()
{
	Protocol::SCRALoginGiftInfo_0 raligi;
	raligi.login_days = m_activity_info.ra_login_gift_login_days_0;
	raligi.has_login = m_activity_info.ra_login_gift_last_login_dayid_0;
	//raligi.has_fetch_accumulate_reward = m_activity_info.ra_login_gift_has_fetch_accumulate_reward_0;
	raligi.fetch_common_reward_flag = m_activity_info.ra_login_gift_fetch_common_reward_flag_0;
	raligi.fetch_vip_reward_flag = m_activity_info.ra_login_gift_fetch_vip_reward_flag_0;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&raligi, sizeof(raligi));
}

void RoleActivity::OnRALoginGiftFetchCommonReward_0(int seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_login_gift_fetch_common_reward_flag_0) * 8);

	if (seq < 0 || seq >= RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_login_gift_fetch_common_reward_flag_0 & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityLoginGiftCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetLoginGiftRewardCfg_0(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_login_gift_login_days_0 < cfg_item->need_login_days)
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_login_gift_fetch_common_reward_flag_0 |= (1 << seq);

	m_role->GetKnapsack()->Put(cfg_item->common_reward_item, PUT_REASON_RA_LOGIN_GIFT_REWARD);

	this->SendRALoginGiftInfo_0();
}

void RoleActivity::OnRALoginGiftFetchVipReward_0(int seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_login_gift_fetch_vip_reward_flag_0) * 8);

	if (seq < 0 || seq >= RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	int vip_level = m_role->GetVip()->GetVipLevel();
	int can_fetch_reward = (0 != LOGIC_CONFIG->GetVipConfig().GetAuthParam(vip_level, VAT_RA_LOGIN_GIFT_VIP_REWARD));

	if (!can_fetch_reward)
	{
		m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGH);
		return;
	}

	if (0 != (m_activity_info.ra_login_gift_fetch_vip_reward_flag_0 & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityLoginGiftCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetLoginGiftRewardCfg_0(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_login_gift_login_days_0 < cfg_item->need_login_days)
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_login_gift_fetch_vip_reward_flag_0 |= (1 << seq);

	m_role->GetKnapsack()->Put(cfg_item->vip_reward_item, PUT_REASON_RA_LOGIN_GIFT_REWARD);

	this->SendRALoginGiftInfo_0();
}

void RoleActivity::OnRAEverydayNiceGiftReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT);

	switch (opera_type)
	{
	case Protocol::EVERYDAY_NICE_GIFT_SEND_INFO:
	{
		this->SendRAEverydayNiceGiftInfo();
	}
	break;

	case Protocol::EVERYDAY_NICE_GIFT_FETCH_REWARD:
	{
		this->OnRAEverydayNiceGiftFetchReward(param1);
	}
	break;
	}
}

void RoleActivity::OnRAEverydayNiceGiftRecharge(int recharge_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT);

	m_activity_info.ra_everyday_nice_gift_everydaty_recharge += recharge_gold;

	if (0 == m_activity_info.ra_everyday_nice_gift_is_have_recharge_standard)
	{
		RandActivityEverydayNiceGiftCfg::ConfigItem *configitem_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetEveydayNiceGiftRewardCfg();
		if (configitem_cfg != nullptr)
		{
			if (m_activity_info.ra_everyday_nice_gift_everydaty_recharge >= configitem_cfg->recharge_gold)
			{
				// 第一次达标时可以领取之前的奖励，之后的奖励需要登录才可以领取
				if (0 == m_activity_info.ra_everyday_nice_gift_is_have_recharge_standard)
				{
					unsigned int act_begin_timestamp = ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT);
					unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					int day_index = GetDayIndex(act_begin_timestamp, now_second);

					for (int i = 0; i <= day_index && i < RAND_ACTIVITY_EVERYDAY_NICE_GIFT_MAX_REWARD_COUNT; ++i)
					{
						m_activity_info.ra_everyday_nice_gift_can_fetch_reward_flag |= 1 << i;
					}

					// 记录达标充值时间戳
					m_activity_info.ra_everyday_nice_gift_recharge_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				}

				m_activity_info.ra_everyday_nice_gift_is_have_recharge_standard = 1;
			}
		}
	}

	this->SendRAEverydayNiceGiftInfo();
}

void RoleActivity::SendRAEverydayNiceGiftInfo()
{
	static Protocol::SCRAEverydayNiceGiftInfo engi;

	engi.is_active = m_activity_info.ra_everyday_nice_gift_is_have_recharge_standard;
	engi.everyday_recharge = m_activity_info.ra_everyday_nice_gift_everydaty_recharge;
	engi.can_fetch_reward_flag = m_activity_info.ra_everyday_nice_gift_can_fetch_reward_flag;
	engi.have_fetch_reward_flag = m_activity_info.ra_everyday_nice_gift_have_fetch_reward_flag;

	SEND_TO_ROLE(m_role, engi);
}

void RoleActivity::OnRAEverydayNiceGiftFetchReward(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_EVERYDAY_NICE_GIFT_MAX_REWARD_COUNT)
	{
		return;
	}

	if (0 == m_activity_info.ra_everyday_nice_gift_is_have_recharge_standard)
	{
		m_role->NoticeNum(errornum::EN_DANBI_CHONGZHI_REWARD_HAS_NOT_ACTIVE);
		return;
	}

	RandActivityEverydayNiceGiftCfg::ConfigItem *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetEveydayNiceGiftRewardCfg();
	if (nullptr == reward_cfg)
	{
		return;
	}

	if (seq >= reward_cfg->reward_count)
	{
		return;
	}

	if ((m_activity_info.ra_everyday_nice_gift_can_fetch_reward_flag & 1 << seq) == 0)
	{
		m_role->NoticeNum(errornum::EN_REWARD_NOT_SATISFY);
		return;
	}

	if ((m_activity_info.ra_everyday_nice_gift_have_fetch_reward_flag & 1 << seq) != 0)
	{
		m_role->NoticeNum(errornum::EN_REWARD_HAS_FETCH);
		return;
	}

	m_activity_info.ra_everyday_nice_gift_have_fetch_reward_flag |= 1 << seq;

	m_role->GetKnapsack()->PutOrMail(reward_cfg->reward_item_list[seq], PUT_REASON_EVERYDAY_NICE_GIFT_ERWARD);

	this->SendRAEverydayNiceGiftInfo();

	gamelog::g_log_roleactivity.printf(LL_INFO, "OnRAEverydayNiceGiftFetchReward::user[%d %s], reward[%s]",
		m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&reward_cfg->reward_item_list[seq], nullptr));
}

void RoleActivity::OnRAEverydayNiceGiftGiveRewardOnDayChange()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT))
	{
		return;
	}

	RandActivityEverydayNiceGiftCfg::ConfigItem *configitem_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetEveydayNiceGiftRewardCfg();
	if (nullptr == configitem_cfg)
	{
		return;
	}

	if (0 == m_activity_info.ra_everyday_nice_gift_is_have_recharge_standard)
	{
		return;
	}

	static MailContentParam contentparam; contentparam.Reset();

	for (int i = 0; i < configitem_cfg->reward_count && i < RAND_ACTIVITY_EVERYDAY_NICE_GIFT_MAX_REWARD_COUNT; ++i)
	{
		if (0 == (m_activity_info.ra_everyday_nice_gift_can_fetch_reward_flag & 1 << i))
		{
			continue;
		}

		if ((m_activity_info.ra_everyday_nice_gift_have_fetch_reward_flag & 1 << i) != 0)
		{
			continue;
		}

		m_activity_info.ra_everyday_nice_gift_have_fetch_reward_flag |= 1 << i;

		if (configitem_cfg->reward_item_list[i].item_id <= 0)
		{
			continue;  // 防止发空邮件
		}

		contentparam.Reset();
		const ItemBase *reward_item_base = ITEMPOOL->GetItem(configitem_cfg->reward_item_list[i].item_id);
		if (reward_item_base != nullptr)
		{
			contentparam.item_list[contentparam.item_count].item_id = configitem_cfg->reward_item_list[i].item_id;
			contentparam.item_list[contentparam.item_count].num = configitem_cfg->reward_item_list[i].num;
			contentparam.item_list[contentparam.item_count].is_bind = (configitem_cfg->reward_item_list[i].is_bind) ? 1 : 0;
			contentparam.item_list[contentparam.item_count].invalid_time = reward_item_base->CalInvalidTime();
			++contentparam.item_count;
		}

		const auto length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward_common_mail_content, RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT);
		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_roleactivity.printf(LL_INFO, "OnRAEverydayNiceGiftReussueFetchReward::user[%d %s], reward[%s]",
				m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&configitem_cfg->reward_item_list[i], nullptr));
		}
	}

	this->SendRAEverydayNiceGiftInfo();
}

// ------------------------------
// 仙盟比拼
// ------------------------------
void RoleActivity::SendRAXianMengBiPinInfo()
{
	Protocol::SCRAXianMengBiPinInfo xmbpi;
	xmbpi.xianmeng_bipin_kill_boss_count = 0;

	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetUserId());
	if (NULL != guild)
	{
		xmbpi.xianmeng_bipin_kill_boss_count = guild->GetXianmengBipinKillWorldBoss();
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&xmbpi, sizeof(xmbpi));
}

// ------------------------------
// 仙盟崛起
// ------------------------------
void RoleActivity::SendRAXianMengJueQiInfo()
{
	Protocol::SCRAXianMengJueQiInfo xmjqi;
	xmjqi.xianmeng_jueqi_increase_capability = 0;

	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetUserId());
	if (NULL != guild)
	{
		xmjqi.xianmeng_jueqi_increase_capability = guild->GetDayIncreaseCapability();
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&xmjqi, sizeof(xmjqi));
}

// ------------------------------
// 累计消费
// ------------------------------
void RoleActivity::OnRATotalConsumeGoldOperaReq(int opera_type,int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD);

	switch (opera_type)
	{
	case Protocol::RA_TOTAL_CONSUME_GOLD_TYPE_QUERY_INFO:
		{
			this->SendRATotalConsumeGoldInfo();
		}
		break;

	case Protocol::RA_TOTAL_CONSUME_GOLD_TYPE_FETCH_REWARD:
		{
			this->OnRATotalConsumeGoldFetchReward(param);
		}
		break;
	}
}

void RoleActivity::SendRATotalConsumeGoldInfo()
{
	Protocol::SCRATotalConsumeGoldInfo tcgi;
	tcgi.consume_gold = m_activity_info.ra_total_consume_gold_consume_num;
	tcgi.fetch_reward_flag = m_activity_info.ra_total_consume_gold_fetch_reward_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tcgi, sizeof(tcgi));
}

void RoleActivity::OnRATotalConsumeGoldAddConsume(long long consume_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD);

	m_activity_info.ra_total_consume_gold_consume_num += static_cast<int>(consume_gold);
	this->SendRATotalConsumeGoldInfo();
}

void RoleActivity::OnRATotalConsumeGoldFetchReward(int reward_seq)
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_TOTAL_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_total_consume_gold_fetch_reward_flag) * 8);

	if (reward_seq < 0 || reward_seq >= RAND_ACTIVITY_TOTAL_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_total_consume_gold_fetch_reward_flag & (1 << reward_seq)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const RandActivityTotalConsumeGoldCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetTotalConsumeGoldRewardCfg(reward_seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_total_consume_gold_consume_num < cfg_item->need_consume_gold)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 消耗
	m_activity_info.ra_total_consume_gold_fetch_reward_flag |= (1 << reward_seq);

	// 结算
	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_RA_TOTAL_CONSUME_GOLD_REWARD);

	this->SendRATotalConsumeGoldInfo();

	m_role->NoticeNum(noticenum::NT_RAND_ACTIVITY_FETCH_REWARD_SUCC);
}

// ------------------------------
// 单笔充值（充值某个固定额度就给）
// ------------------------------
void RoleActivity::OnRASingleChargeCheckCharge(long long gold_num)
{
	if (gold_num <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHARGE))
	{
		return;
	}

	const RandActivitySingleChargeCfg::ConfigItem *sc_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetSingleChargeCfg(gold_num);
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

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_single_charge_reward_content, gold_num);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRASingleChargeCheckCharge::user[%d, %s], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&sc_cfg->reward_item, NULL));
	}
}

void RoleActivity::OnRASingleChargeCheckCharge2(long long gold_num)
{
	if (gold_num <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHARGE_2))
	{
		return;
	}

	const RandActivitySingleChargeCfg::ConfigItem *sc_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetSingleChargeCfg2(gold_num);
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

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_single_charge_reward_content2, gold_num);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRASingleChargeCheckCharge2::user[%d, %s], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&sc_cfg->reward_item, NULL));
	}
}

void RoleActivity::OnRASingleChargeCheckCharge3(long long gold_num)
{
	if (gold_num <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHARGE_3))
	{
		return;
	}

	const RandActivitySingleChargeCfg::ConfigItem *sc_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetSingleChargeCfg3(gold_num);
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

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_single_charge_reward_content3, gold_num);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRASingleChargeCheckCharge3::user[%d, %s], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&sc_cfg->reward_item, NULL));
	}
}

void RoleActivity::OnRASingleChargeCheckCharge4(long long gold_num)
{
	if (gold_num <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHARGE_4))
	{
		return;
	}

	const RandActivitySingleChargeCfg::ConfigItem *sc_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetSingleChargeCfg4(gold_num);
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

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_single_charge_reward_content4, gold_num);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRASingleChargeCheckCharge4::user[%d, %s], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&sc_cfg->reward_item, NULL));
	}
}

void RoleActivity::OnRASingleChargeCheckCharge5(long long gold_num)
{
	if (gold_num <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHARGE_5))
	{
		return;
	}

	const RandActivitySingleChargeCfg::ConfigItem *sc_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetSingleChargeCfg5(gold_num);
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

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_single_charge_reward_content5, gold_num);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRASingleChargeCheckCharge5::user[%d, %s], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&sc_cfg->reward_item, NULL));
	}
}

void RoleActivity::OnRASingleChargeCheckChargeOne(long long gold_num)
{
	if (gold_num <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHARGE1))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SINGLE_CHARGE1);

	const RandActivitySingleChargeCfg::ConfigItem *sc_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetSingleCharge1Cfg(gold_num,m_activity_info.ra_single_charge1_reward_fetch_flag);
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

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_single_charge_reward_content_multi, 
		RAND_ACTIVITY_TYPE_SINGLE_CHARGE1, gold_num);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		m_activity_info.ra_single_charge1_reward_fetch_flag |= (1 << sc_cfg->seq);
		this->SendSingleChargeInfoOne();

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRASingleChargeCheckChargeOne::user[%d, %s], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&sc_cfg->reward_item, NULL));
	}
}

void RoleActivity::SendSingleChargeInfoOne()
{
	Protocol::SCSingleChargeInfoMulti info;

	info.activtiy_type = RAND_ACTIVITY_TYPE_SINGLE_CHARGE1;
	info.single_charge_reward_fetch_flag = m_activity_info.ra_single_charge1_reward_fetch_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&info, sizeof(info));
}

void RoleActivity::OnRASingleChargeCheckChargeTwo(long long gold_num)
{
	if (gold_num <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHARGE2))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SINGLE_CHARGE2);

	const RandActivitySingleChargeCfg::ConfigItem *sc_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetSingleCharge2Cfg(gold_num,m_activity_info.ra_single_charge2_reward_fetch_flag);
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

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_single_charge_reward_content_multi, 
		RAND_ACTIVITY_TYPE_SINGLE_CHARGE2,gold_num);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		m_activity_info.ra_single_charge2_reward_fetch_flag |= (1 << sc_cfg->seq);
		this->SendSingleChargeInfoTwo();

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRASingleChargeCheckChargeTwo::user[%d, %s], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&sc_cfg->reward_item, NULL));
	}
}

void RoleActivity::SendSingleChargeInfoTwo()
{
	Protocol::SCSingleChargeInfoMulti info;

	info.activtiy_type = RAND_ACTIVITY_TYPE_SINGLE_CHARGE2;
	info.single_charge_reward_fetch_flag = m_activity_info.ra_single_charge2_reward_fetch_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&info, sizeof(info));
}

void RoleActivity::OnRASingleChargeCheckChargeThree(long long gold_num)
{
	if (gold_num <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHARGE3))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SINGLE_CHARGE3);

	const RandActivitySingleChargeCfg::ConfigItem *sc_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetSingleCharge3Cfg(gold_num,m_activity_info.ra_single_charge3_reward_fetch_flag);
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

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_single_charge_reward_content_multi, 
		RAND_ACTIVITY_TYPE_SINGLE_CHARGE3, gold_num);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		m_activity_info.ra_single_charge3_reward_fetch_flag |= (1 << sc_cfg->seq);
		this->SendSingleChargeInfoThree();

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRASingleChargeCheckChargeThree::user[%d, %s], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&sc_cfg->reward_item, NULL));
	}
}

void RoleActivity::SendSingleChargeInfoThree()
{
	Protocol::SCSingleChargeInfoMulti info;

	info.activtiy_type = RAND_ACTIVITY_TYPE_SINGLE_CHARGE3;
	info.single_charge_reward_fetch_flag = m_activity_info.ra_single_charge3_reward_fetch_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&info, sizeof(info));
}

// ------------------------------
// 双倍元宝
// ------------------------------
void RoleActivity::OnRADoubleGetCheckCharge(long long gold_num)
{
	if (gold_num <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DOUBLE_GET))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DOUBLE_GET);

	const RandActivityDoubleGet::ConfigItem *sc_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetDoubleGetCfg((int)gold_num,m_activity_info.ra_double_get_reward_fetch_flag);
	if (NULL == sc_cfg)
	{
		return;
	}

	// 单笔充值达到要求，发放奖励
	static MailContentParam contentparam; contentparam.Reset();

	contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, sc_cfg->reward_gold);

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_double_get_reward_content,
		RAND_ACTIVITY_TYPE_DOUBLE_GET, gold_num);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		m_activity_info.ra_double_get_reward_fetch_flag |= 1 << sc_cfg->seq;

		this->SendRADoubleGetInfo();

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRADoubleGetCheckCharge::user[%d, %s], reward_gold[%d]",
			m_role->GetUID(), m_role->GetName(), sc_cfg->reward_gold);
	}
}

void RoleActivity::SendRADoubleGetInfo()
{
	Protocol::SCRADoubleGetInfo info;

	info.double_get_reward_fetch_flag = m_activity_info.ra_double_get_reward_fetch_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&info, sizeof(info));
}

// ------------------------------
// 累计充值 (区分前7天，7-15天等）
// ------------------------------
void RoleActivity::OnRATotalChargeOperaReq(int opera_type, int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CHARGE);

	switch(opera_type)
	{
	case Protocol::RA_TOTAL_CHARGE_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRATotalChargeInfo();
		}
		break;

	case Protocol::RA_TOTAL_CHARGE_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRATotalChargeFetchReward(seq);
		}
		break;
	}
}

void RoleActivity::OnRATotalChargeCheckChongzhi(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CHARGE);

	m_activity_info.ra_total_charge_value += (int)chongzhi;

	this->SendRATotalChargeInfo();
}

void RoleActivity::RACheckTotalChargeGiveReward()
{
	int last_activity_got_max_seq = LOGIC_CONFIG->GetRandActivityCfg().GetTotalChargeSeqWithCharge(m_activity_info.ra_total_charge_value);
	if (last_activity_got_max_seq < 0)
	{
		return;
	}

	for (int seq = 0; seq <= last_activity_got_max_seq; ++ seq)
	{
		if (0 != (m_activity_info.ra_total_charge_reward_has_fetch_flag & (1 << seq)))
		{
			continue;
		}

		const RandActivityTotalChargeRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetTotalChargeRewardWithSeq(seq);
		if (NULL == cfg_item)
		{
			return;
		}

		if (m_activity_info.ra_total_charge_value < cfg_item->need_chongzhi)
		{
			continue;
		}

		{
			// 奖励物品
			gamecommon::SendMultiMailToRole(m_role->GetUserId(), cfg_item->item_list, cfg_item->item_count, NULL, true, gamestring::g_rand_activity_total_charge_bu_reward_content, RAND_ACTIVITY_TYPE_TOTAL_CHARGE);
		}

		{
			m_activity_info.ra_total_charge_reward_has_fetch_flag |= (1 << seq);
		}

		{
			// 日志
			std::string get_item_log;
			ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

			gamelog::g_log_roleactivity.printf(LL_INFO, "RACheckTotalChargeGiveReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d], item_list[%s]",
				m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.ra_total_charge_reward_has_fetch_flag, get_item_log.c_str());

			ROLE_LOG_QUICK6(LOG_TYPE_RAND_ACTIVITY_TOTAL_CHONGZHI, m_role, seq, m_activity_info.ra_total_charge_reward_has_fetch_flag, NULL, NULL);
		}
	}
}

void RoleActivity::SendRATotalChargeInfo()
{
	Protocol::SCRATotalChargeInfo tcgi;
	tcgi.reward_has_fetch_flag = m_activity_info.ra_total_charge_reward_has_fetch_flag;
	tcgi.total_charge_value = m_activity_info.ra_total_charge_value;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tcgi, sizeof(tcgi));
}

void RoleActivity::OnRATotalChargeFetchReward(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE))
	{
		return;
	}

	if (seq < 0 || seq >= static_cast<int>(sizeof(m_activity_info.ra_total_charge_reward_has_fetch_flag) * 8))
	{
		return;
	}

	if (0 != (m_activity_info.ra_total_charge_reward_has_fetch_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_FETCH_REWARD);
		return;
	}

	const RandActivityTotalChargeRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetTotalChargeRewardWithSeq(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_total_charge_value < cfg_item->need_chongzhi)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_TOTAL_CHONGZHI_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg_item->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_total_charge_reward_has_fetch_flag |= (1 << seq);
	this->SendRATotalChargeInfo();

	m_role->GetKnapsack()->PutList(cfg_item->item_count, cfg_item->item_list, PUT_REASON_RA_TOTAL_CHARGE);

	if (cfg_item->item_count > 0)
	{
		// 累计充值传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_total_charge_reward_chuanwen_content,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), cfg_item->need_chongzhi);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	{
		// 日志
		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

		gamelog::g_log_roleactivity.printf(LL_INFO, "OnRATotalChargeFetchReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d], item_list[%s]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.ra_total_charge_reward_has_fetch_flag, get_item_log.c_str());

		ROLE_LOG_QUICK6(LOG_TYPE_RAND_ACTIVITY_TOTAL_CHONGZHI, m_role, seq, m_activity_info.ra_total_charge_reward_has_fetch_flag, NULL, NULL);
	}
}

// ------------------------------
// 每日充值排行榜
// ------------------------------
void RoleActivity::OnRADayChongzhiRankAddChongzhi(int gold_num)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_RANK))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_RANK);

	m_activity_info.ra_day_chongzhi_rank_gold_num += gold_num;
	m_activity_info.ra_day_chongzhi_rank_chongzhi_last_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	this->SendRADayChongzhiRankInfo();

	{
		m_role->GetRankShadow()->ForceSyncData();

		PersonRank *person_rank = RankManager::Instance().GetPersonRank();
		if (NULL != person_rank)
		{
			person_rank->ForceSnapshot();
		}
	}
}

void RoleActivity::SendRADayChongzhiRankInfo()
{
	Protocol::SCRADayChongzhiRankInfo radcri;
	radcri.gold_num = m_activity_info.ra_day_chongzhi_rank_gold_num;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&radcri, sizeof(radcri));
}

void RoleActivity::RAResetDayChongzhiRankGoldNum()
{
	m_activity_info.ra_day_chongzhi_rank_gold_num = 0;
	this->SendRADayChongzhiRankInfo();
}

// ------------------------------
// 装备强化
// ------------------------------
void RoleActivity::CheckRAEquipStrengthenReward(int strengthen_level)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EQUIP_STRENGTHEN))
	{
		return;
	}

	const RandActivityEquipStrengthenCfg::ConfigItem *item_config = LOGIC_CONFIG->GetRandActivityCfg().GetEquipStrengthRewardCfg(strengthen_level);
	if (NULL == item_config || 0 == item_config->item_count)
	{
		return;
	}

	static MailContentParam contentparam; contentparam.Reset();

	for (int i = 0; i < item_config->item_count && i < MAX_ATTACHMENT_ITEM_NUM; ++ i)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(item_config->item_list[i].item_id);
		if (NULL == item_base)
		{
			continue;
		}

		contentparam.item_list[i].item_id = item_config->item_list[i].item_id;
		contentparam.item_list[i].num = item_config->item_list[i].num;
		contentparam.item_list[i].is_bind = item_config->item_list[i].is_bind;
		contentparam.item_list[i].invalid_time = item_base->CalInvalidTime();
	}

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_strengthen_reward_content, strengthen_level);

	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
	}
}


void RoleActivity::OnGetTitleOwnerInfo()
{
	static Protocol::SCTitleOwnerInfo toi;
	toi.xianjiezhizhun_owner_uid = TopRanker::Instance().GetTopUser(RANK_TOP_TYPE_CAPABILITY, toi.xianjiezhizhun_owner_name);
	toi.junlintianxia_owner_uid = TopRanker::Instance().GetTopUser(RANK_TOP_TYPE_LEVEL, toi.junlintianxia_owner_name);
	toi.qingshihongyan_owner_uid = TopRanker::Instance().GetTopUser(RANK_TOP_TYPE_XIANNV, toi.qingshihongyan_owner_name);
	toi.fengliutitang_owner_uid = TopRanker::Instance().GetTopUser(RANK_TOP_TYPE_CHARM_MALE, toi.fengliutitang_owner_name);
	toi.guosetianxiang_owner_uid = TopRanker::Instance().GetTopUser(RANK_TOP_TYPE_CHARM_FEMALE, toi.guosetianxiang_owner_name);
	toi.kunlunzhanshen_owner_uid = TopRanker::Instance().GetTopUser(RANK_TOP_TYPE_CAPABILITY_CAMP_1, toi.kunlunzhanshen_owner_name);
	toi.penglaizhanshen_owner_uid = TopRanker::Instance().GetTopUser(RANK_TOP_TYPE_CAPABILITY_CAMP_2, toi.penglaizhanshen_owner_name);
	toi.cangqiongzhanshen_owner_uid = TopRanker::Instance().GetTopUser(RANK_TOP_TYPE_CAPABILITY_CAMP_3, toi.cangqiongzhanshen_owner_name);
	toi.weizhencangqiong_onwer_uid = TopRanker::Instance().GetTopUser(RANK_TOP_TYPE_QUNXIANLUANDOU, toi.weizhencangqiong_owner_name);
	toi.bosshunter_owner_uid = TopRanker::Instance().GetTopUser(RANK_TOP_TYPE_KILL_WORLDBOSS, toi.bosshunter_owner_name);
	toi.tianxiawushuang_owner_uid = TopRanker::Instance().GetTopUser(RANK_TOP_TYPE_ZHANSHENDIAN, toi.tianxiawushuang_owner_name);

	{
		toi.wangchengchengzhu_owner_uid = 0;
		memset(toi.wangchengchengzhu_owner_name, 0, sizeof(toi.wangchengchengzhu_owner_name));

		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(WorldStatus::Instance().GetGCZChengzhuID());
		if (NULL != user_node)
		{
			toi.wangchengchengzhu_owner_uid = user_node->uid;
			user_node->GetName(toi.wangchengchengzhu_owner_name);
		}
	}

	{
		toi.zuiqiangxianmeng_owner_uid = 0;
		memset(toi.zuiqiangxianmeng_owner_name, 0, sizeof(toi.zuiqiangxianmeng_owner_name));

		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(WorldStatus::Instance().GetXianmengzhanMengZhu());
		if (NULL != user_node)
		{
			toi.zuiqiangxianmeng_owner_uid = user_node->uid;
			user_node->GetName(toi.zuiqiangxianmeng_owner_name);
		}
	}

	{
		toi.xiongbatianxia_owner_uid = 0;
		memset(toi.xiongbatianxia_owner_name, 0, sizeof(toi.xiongbatianxia_owner_name));

		long long uuid = WorldStatus::Instance().GetChallengeFieldKingUserId();
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(LongLongToUniqueUserID(uuid).user_id);
		if (NULL != user_node)
		{
			toi.xiongbatianxia_owner_uid = user_node->uid;
			user_node->GetName(toi.xiongbatianxia_owner_name);
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&toi, sizeof(toi));
}

//--------------------------------------------------------
//限时积分兑换稀有装备
//--------------------------------------------------------
void RoleActivity::OnRATimeLimitExchangeEquiOperaReq(int oper_type, int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_EQUIP))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_EQUIP);

	if (Protocol::RA_TIME_LIMIT_EXCHANGE_OPERA_TYPE_QUERY == oper_type)
	{
		this->SendRATimeLimitExchangeEquiInfo();
	}

	else if (Protocol::RA_TIME_LIMIT_EXCHANGE_OPERA_TYPE_EXCHANGE == oper_type)
	{
		if (seq >= TIME_LIMIT_EXCHANGE_ITEM_COUNT || seq < 0)
		{
			return;
		}

		const RandActivityTimeLimitExchangeConfig *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetTimeLimitExchangeEquiCfg(seq);
		if (NULL == cfg_item)
		{
			return;
		}

		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}

		int need_score = -1;
		int exchange_count = m_activity_info.ra_time_limit_exchange_exchange_equi_times_list[seq];
		if (exchange_count >= 0 && cfg_item->double_count > 0)
		{
			int double_count = exchange_count / cfg_item->double_count;
			if (double_count >= 32)
			{
				return;
			}

			need_score = cfg_item->need_score << double_count;
		}

		if (need_score <= 0)
		{
			return;
		}

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(cfg_item->item.item_id);
		if (NULL == equip)
		{
			return;
		}

		//检查消耗寻宝装备积分
		{
			if (need_score > m_role->GetRoleShop()->GetChestShopShengwang())
			{
				m_role->NoticeNum(errornum::EN_SHENGWANG_NOT_ENOUGH);
				return;
			}

			m_role->GetRoleShop()->AddChestShopShengwang((-1 * need_score), "FunrandActivity");
		}

		++ m_activity_info.ra_time_limit_exchange_exchange_equi_times_list[seq];

		m_role->GetKnapsack()->Put(cfg_item->item, PUT_REASON_RA_CONVERT_SHOP);

		gamelog::g_log_rand_activity.printf(LL_INFO, "[OnRATimeLimitExchangeEquiOperaReq][user[%d %s] convert_item[id:%d, num:%d], need_score[%d], times[%d]",
			UidToInt(m_role->GetUserId()), m_role->GetName(), cfg_item->item.item_id, cfg_item->item.num, need_score, m_activity_info.ra_time_limit_exchange_exchange_equi_times_list[seq]);

		this->SendRATimeLimitExchangeEquiInfo();
	}
}

void RoleActivity::SendRATimeLimitExchangeEquiInfo()
{
	Protocol::SCRATimeLimitExchangeEquiInfo cmd;

	UNSTD_STATIC_CHECK(sizeof(cmd.ra_time_limit_exchange_exchange_times_list) == sizeof(m_activity_info.ra_time_limit_exchange_exchange_equi_times_list));
	memcpy(cmd.ra_time_limit_exchange_exchange_times_list, m_activity_info.ra_time_limit_exchange_exchange_equi_times_list, sizeof(cmd.ra_time_limit_exchange_exchange_times_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}

//随机活动--兑换精灵
void RoleActivity::OnRATimeLimitExchangeJLOperaReq(int oper_type, int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_JL))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_JL);

	if (Protocol::RA_TIME_LIMIT_EXCHANGE_OPERA_TYPE_QUERY == oper_type)
	{
		this->SendRATimeLimitExchangeJLInfo();
	}

	else if (Protocol::RA_TIME_LIMIT_EXCHANGE_OPERA_TYPE_EXCHANGE == oper_type)
	{
		if (seq >= TIME_LIMIT_EXCHANGE_ITEM_COUNT || seq < 0)
		{
			return;
		}

		const RandActivityTimeLimitExchangeConfig *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetTimeLimitExchangeJLCfg(seq);
		if (NULL == cfg_item)
		{
			return;
		}

		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}

		int need_score = 0;
		int exchange_count = m_activity_info.ra_time_limit_exchange_exchange_jl_times_list[seq];
		if (exchange_count >= 0 && cfg_item->double_count > 0)
		{
			int double_count = exchange_count / cfg_item->double_count;
			if (double_count >= 32)
			{
				return;
			}

			need_score = cfg_item->need_score << double_count;
		}

		if (need_score <= 0)
		{
			return;
		}

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(cfg_item->item.item_id);
		if (NULL == equip)
		{
			return;
		}

		//检查消耗猎取功勋积分
		{
			if (need_score > m_role->GetRoleShop()->GetChestShopGongXun())
			{
				m_role->NoticeNum(errornum::EN_GONGXUN_NOT_ENOUGH);
				return;
			}

			m_role->GetRoleShop()->AddChestShopGongXun((-1 * need_score), "FunrandActivity");
		}

		++ m_activity_info.ra_time_limit_exchange_exchange_jl_times_list[seq];

		//放入背包
		m_role->GetKnapsack()->Put(cfg_item->item, PUT_REASON_RA_CONVERT_SHOP);

		gamelog::g_log_rand_activity.printf(LL_INFO, "[OnRATimeLimitExchangeJLOperaReq][user[%d %s] convert_item[id:%d, num:%d], need_score[%d], times[%d]",
			UidToInt(m_role->GetUserId()), m_role->GetName(), cfg_item->item.item_id, cfg_item->item.num, need_score, m_activity_info.ra_time_limit_exchange_exchange_jl_times_list[seq]);

		this->SendRATimeLimitExchangeJLInfo();
	}
}

void RoleActivity::SendRATimeLimitExchangeJLInfo()
{
	Protocol::SCRATimeLimitExchangeJLInfo cmd;

	UNSTD_STATIC_CHECK(sizeof(cmd.ra_time_limit_exchange_exchange_times_list) == sizeof(m_activity_info.ra_time_limit_exchange_exchange_jl_times_list));
	memcpy(cmd.ra_time_limit_exchange_exchange_times_list, m_activity_info.ra_time_limit_exchange_exchange_jl_times_list, sizeof(cmd.ra_time_limit_exchange_exchange_times_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}

void RoleActivity::RAResetExchangTimes()
{
	memset(m_activity_info.ra_time_limit_exchange_exchange_equi_times_list, 0, sizeof(m_activity_info.ra_time_limit_exchange_exchange_equi_times_list));
	memset(m_activity_info.ra_time_limit_exchange_exchange_jl_times_list, 0, sizeof(m_activity_info.ra_time_limit_exchange_exchange_jl_times_list));

	this->SendRATimeLimitExchangeEquiInfo();
	this->SendRATimeLimitExchangeJLInfo();
}


//--------------------------------------------------------
//金银塔抽奖
//--------------------------------------------------------
void RoleActivity::OnRALevelLotteryOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LEVEL_LOTTERY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LEVEL_LOTTERY);

	switch(opera_type)
	{
	case Protocol::RA_LEVEL_LOTTERY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRALevelLotteryInfo();
		}
		break;

	case Protocol::RA_LEVEL_LOTTERY_OPERA_TYPE_DO_LOTTERY:
		{
			this->OnRALevelLotteryDoLottery(param);
		}
		break;

	case Protocol::RA_LEVEL_LOTTERY_OPERA_TYPE_FETCHE_TOTAL_REWARD:
		{
			this->OnRALevelLotteryFetchTotalTimesReward(param);
		}
		break;

	case Protocol::RA_LEVEL_LOTTERY_OPERA_TYPE_ACTIVITY_INFO:
		{
			this->SendRALevelLotteryActivityInfo();
		}
		break;

	}
}

void RoleActivity::SendRALevelLotteryInfo()
{
	RandActivityLevelLottery *level_lottery_activity = RandActivityManager::Instance().GetRandActivityLevelLottery();
	if (NULL != level_lottery_activity)
	{
		Protocol::SCRALevelLotteryInfo cmd;
		cmd.lottery_cur_level = m_activity_info.ra_level_lottery_cur_level;

		cmd.history_count = level_lottery_activity->GetLotteryHistoryList(cmd.history_list);

		if (cmd.history_count >= 0)
		{
			int send_len = sizeof(cmd) - (RA_LEVEL_LOTTERY_HISTORY_MAX_COUNT - cmd.history_count) * sizeof(cmd.history_list[0]);
			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char *)&cmd, send_len);
		}
	}
}

void RoleActivity::SendRALevelLotteryActivityInfo()
{
	Protocol::SCRALevelLotteryActivityInfo cmd;
	cmd.ra_level_lottery_free_buy_times = m_activity_info.ra_level_lottery_free_buy_times;
	cmd.ra_lottery_next_free_timestamp = m_activity_info.ra_lottery_next_free_timestamp;
	cmd.ra_lottery_buy_total_times = m_activity_info.ra_lottery_buy_total_times;
	cmd.ra_lottery_fetch_reward_flag = m_activity_info.ra_lottery_fetch_reward_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(Protocol::SCRALevelLotteryActivityInfo));
}

void RoleActivity::OnRALevelLotteryDoLottery(int times)
{
	if (times <= 0 || times > RA_LEVEL_LOTTERY_REWARD_MAX_COUNT)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LEVEL_LOTTERY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	bool is_use_item = false;
	if (times == 30)
	{
		if (m_role->GetKnapsack()->HasItem(LOGIC_CONFIG->GetRandActivityCfg().GetLevelLotteryConsumeItem()))
		{
			is_use_item = true;

			//if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(times))
			//{
			//	m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			//	return;
			//}

			if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, times))
			{
				m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_SPACE);
				return;
			}
		}
	}

	RandActivityLevelLottery *level_lottery_activity = RandActivityManager::Instance().GetRandActivityLevelLottery();
	if (NULL == level_lottery_activity)
	{
		return;
	}

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	const RandActivityLevelLotteryOtherCfg &other_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLevelLotteryOhterCfg();

	Protocol::SCRALevelLotteryRewardList cmd;
	cmd.reward_count = 0;

	int total_consume_gold = 0;
	for (int index = 0; index < times; ++index)
	{
		const int server_chou_times = level_lottery_activity->GetServerChouTimes();
		const int level = m_activity_info.ra_level_lottery_cur_level;

		// 如果没有保底奖励才进行随机奖励
		const RandActivityAdditionLotteryConfig::ConfigItem *lottery_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLevelLotteryBaodiRewardCfg(server_chou_times);
		if (NULL != lottery_cfg)
		{
			level_lottery_activity->ClearServerChouTimes();
		}
		else
		{
			lottery_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLevelLotteryRandomRewardCfg(level);
		}

		if (NULL == lottery_cfg)
		{
			continue;
		}

		int need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetLevelLotteryNeedGold(level);
		if (need_gold <= 0)
		{
			continue;
		}

		if (30 == times && is_use_item)
		{
			need_gold = 0;
		}

		if (1 == times && other_cfg.free_interval > 0 && other_cfg.free_times > 0)
		{
			if (m_activity_info.ra_level_lottery_free_buy_times < other_cfg.free_times && now_sec > m_activity_info.ra_lottery_next_free_timestamp)
			{
				need_gold = 0;
			}
		}

		if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			break;
		}

		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			break;
		}

		// 消耗
		if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "randactivity_level_lottery"))
		{
			return;
		}

		if (!is_use_item && 0 == need_gold)
		{
			m_activity_info.ra_level_lottery_free_buy_times++;

			if (m_activity_info.ra_level_lottery_free_buy_times >= other_cfg.free_times)
			{
				int next_day_interval = EngineAdapter::Instance().NextDayInterval(0, 0, 0);
				if (next_day_interval > other_cfg.free_interval)
				{
					m_activity_info.ra_lottery_next_free_timestamp = now_sec + next_day_interval;
				}
				else
				{
					m_activity_info.ra_lottery_next_free_timestamp = now_sec + other_cfg.free_interval;
				}
			}
			else
			{
				m_activity_info.ra_lottery_next_free_timestamp = now_sec + other_cfg.free_interval;
			}
		}

		// 结算
		m_role->GetKnapsack()->Put(lottery_cfg->reward_item, PUT_REASON_RA_LEVEL_LOTTERY);
		LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_LEVEL_LOTTERY, m_role->GetUID(), m_role->GetName(), lottery_cfg->reward_item.item_id, lottery_cfg->reward_item.num);
		m_activity_info.ra_level_lottery_cur_level = lottery_cfg->to_level;
		m_activity_info.ra_lottery_buy_total_times++;

		if (lottery_cfg->is_broadcast)
		{
			// 抽奖传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_level_lottery_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_LEVEL_LOTTERY, lottery_cfg->reward_item.item_id);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}

		// 非免费增加全服抽奖次数
		if (is_use_item || need_gold > 0)
		{
			level_lottery_activity->AddServerChouTimes(1);

			total_consume_gold += need_gold;
		}

		if (lottery_cfg->is_record_history)
		{
			// 增加抽奖记录
			level_lottery_activity->AddLotteryHistoryItem(m_role, lottery_cfg->reward_index);
		}

		cmd.lottery_reward_list[cmd.reward_count ++] = (char)lottery_cfg->reward_index;

		gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnRALevelLotteryDoLottery user[%d, %s], reward_index[%d], level [%d] -> [%d], reward[%s]",
			m_role->GetUID(), m_role->GetName(), lottery_cfg->reward_index, lottery_cfg->level, lottery_cfg->to_level, ItemConfigDataLog(&lottery_cfg->reward_item, NULL));

		LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_LEVEL_LOTTERY, m_role->GetUID(), m_role->GetName(), lottery_cfg->reward_item.item_id, lottery_cfg->reward_item.num);
	}

	//RandActivityLogManager::Instance().AddLogData(m_role, RAND_ACTIVITY_TYPE_LEVEL_LOTTERY, total_consume_gold, times);

	// 有抽到东西才下发协议
	if (cmd.reward_count > 0)
	{
		if (is_use_item)
		{
			m_role->GetKnapsack()->ConsumeItem(LOGIC_CONFIG->GetRandActivityCfg().GetLevelLotteryConsumeItem(), 1, "RoleActivity::LotteryDoLottery");
		}
	
		int send_len = sizeof(cmd) - (RA_LEVEL_LOTTERY_REWARD_MAX_COUNT - cmd.reward_count) * sizeof(cmd.lottery_reward_list[0]);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char *)&cmd, send_len);

		this->SendRALevelLotteryInfo();
	}

	this->SendRALevelLotteryActivityInfo();
}

void RoleActivity::OnRALevelLotteryFetchTotalTimesReward(int times)
{
	const RandActivityLevelLotteryTotalTimesCfg *lottery_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLevelLotteryTotalTimesRewardCfg(times);
	if (NULL == lottery_cfg) return;

	if (m_activity_info.ra_lottery_buy_total_times < lottery_cfg->total_times)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_LOTTERY_TOTAL_TIMES_LIMIT);
		return;
	}

	if (0 != (m_activity_info.ra_lottery_fetch_reward_flag & (1 << lottery_cfg->reward_index)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_LOTTERY_HAS_FETCH_LIMIT);
		return;
	}

	if (m_role->GetVip()->GetVipLevel() < lottery_cfg->vip_level_limit)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_LOTTERY_VIP_LIMIT);
		return;
	}

	m_activity_info.ra_lottery_fetch_reward_flag |= (1 << lottery_cfg->reward_index);
	m_role->GetKnapsack()->PutListOrMail(lottery_cfg->reward, PUT_REASON_RA_LEVEL_LOTTERY);

	this->SendRALevelLotteryActivityInfo();
}

//--------------------------------------------------------
// 充值扭蛋
//--------------------------------------------------------
void RoleActivity::OnRAChongZhiNiuEggOperaReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG);

	switch(opera_type)
	{
	case Protocol::RA_CHONGZHI_NIU_EGG_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRANiuEggInfo();
		}
		break;

	case Protocol::RA_CHONGZHI_NIU_EGG_OPERA_TYPE_CHOU:
		{
			this->OnRANiuEggChou(param);
		}
		break;

	case Protocol::RA_CHONGZHI_NIU_EGG_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRANiuEggFetchReward(param);
		}
		break;
	}
}

void RoleActivity::SendRANiuEggInfo()
{
	RandActivityChongzhiNiuEgg *niuegg = RandActivityManager::Instance().GetRandActivityNiuEgg();
	if (NULL != niuegg)
	{
		static Protocol::SCRANiuEggInfo cmd;
		cmd.server_total_niu_egg_times = m_activity_info.ra_niu_egg_times;
		cmd.cur_can_niu_egg_chongzhi_value = m_activity_info.ra_niu_egg_cur_chongzhi_value;
		cmd.server_reward_has_fetch_reward_flag = m_activity_info.ra_niu_egg_server_reward_has_fetch_reward_flag;

		cmd.history_count = niuegg->GetNiueggChouHistoryList(cmd.history_list);
		if (cmd.history_count >= 0)
		{
			int send_len = sizeof(cmd) - (RA_NIU_EGG_CHOU_HISTORY_MAX_COUNT - cmd.history_count) * sizeof(cmd.history_list[0]);
			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char *)&cmd, send_len);
		}
	}
}

void RoleActivity::OnRANiuEggChou(int times)
{
	if (times <= 0 || times > Protocol::SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT)
	{
		return;
	}

	const int need_charge_gold = LOGIC_CONFIG->GetRandActivityCfg().GetNiuEggNeedCharge() * times;
	if (need_charge_gold <= 0)
	{
		return;
	}

	if (m_activity_info.ra_niu_egg_cur_chongzhi_value < static_cast<unsigned int>(need_charge_gold))
	{
		m_role->NoticeNum(errornum::EN_RA_SERVER_NIUEGG_LEFT_CHARGE_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(times))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	RandActivityChongzhiNiuEgg *niu_egg_act = RandActivityManager::Instance().GetRandActivityNiuEgg();
	if (NULL == niu_egg_act)
	{
		return;
	}

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRANiuEggChou user[%d, %s], times[%d], consume_charge[%d], reward[",
		m_role->GetUID(), m_role->GetName(), times, need_charge_gold);

	//static RandActivityNiuEggChouRewardConfig reward_list[Protocol::SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT];
	int reward_count = 0;
	static short reward_seq_list[Protocol::SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT];
	memset(reward_seq_list, -1, sizeof(reward_seq_list));

	int server_chou_times = niu_egg_act->GetServerTotalNiuEggTimes();

	for (int i = 0; i < times; ++ i)
	{
		const RandActivityNiuEggChouRewardConfig::RANiuEggConfigItem *niu_egg_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetNiuEggBaodiRewardCfg(server_chou_times);
		if (NULL != niu_egg_cfg)
		{
			niu_egg_act->ClearNiuEggTimes();
		}
		else
		{
			niu_egg_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandomNiuEggRewardCfg();
		}

		if (NULL == niu_egg_cfg)
		{
			continue;
		}

		reward_seq_list[reward_count] = niu_egg_cfg->req;
		++reward_count;

		++server_chou_times;

		gamelog::g_log_roleactivity.buff_printf("(%s), ", ItemConfigDataLog(&niu_egg_cfg->reward_item, NULL));
	}

	if (reward_count != times)
	{
		gamelog::g_log_roleactivity.clear_buff();
		return;
	}

	// 消耗
	m_activity_info.ra_niu_egg_cur_chongzhi_value -= need_charge_gold;

	for (int i = 0; i < reward_count && i < Protocol::SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT; ++ i)
	{
		const RandActivityNiuEggChouRewardConfig::RANiuEggConfigItem *reward_item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetNiuRewardCfgWithReq(reward_seq_list[i]);
		if (NULL == reward_item_cfg || !m_role->GetKnapsack()->Put(reward_item_cfg->reward_item, PUT_REASON_RA_CHONGZHI_NIUEGG_REWARD))
		{
			continue;
		}

		LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG, m_role->GetUID(), m_role->GetName(), reward_item_cfg->reward_item.item_id, reward_item_cfg->reward_item.num);
		if (reward_item_cfg->is_broadcast) // 配置了需要发传闻
		{
			niu_egg_act->AddNiuEggChouHistoryItem(m_role, reward_item_cfg->req);

			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_chongzhi_niuegg_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG, reward_item_cfg->reward_item.item_id);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}
	}

	niu_egg_act->AddTotalNiuEggTimes(times);
	m_activity_info.ra_niu_egg_times += times;

	{
		static Protocol::SCRANiuEggChouResultInfo cmd;
		cmd.reward_req_list_count = reward_count;

		UNSTD_STATIC_CHECK(sizeof(cmd.reward_req_list) == sizeof(reward_seq_list));
		memcpy(cmd.reward_req_list, reward_seq_list, sizeof(cmd.reward_req_list));

		int len = sizeof(cmd) - (Protocol::SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT - cmd.reward_req_list_count) * sizeof(cmd.reward_req_list[0]);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&cmd, len);
	}

	this->SendRANiuEggInfo();

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);
}

void RoleActivity::OnRANiuEggFetchReward(int req)
{
	RandActivityChongzhiNiuEgg *niu_egg_act = (RandActivityChongzhiNiuEgg *)RandActivityManager::Instance().GetRandActivityNiuEgg();
	if (NULL == niu_egg_act)
	{
		return;
	}

	const RandActivityNiuEggServerRewardConfig::RANiuEggConfigItem *cur_server_niu_cfg = 
		LOGIC_CONFIG->GetRandActivityCfg().GetNiuServerCfgWithServerNiuTimes(m_activity_info.ra_niu_egg_times);
	if (NULL == cur_server_niu_cfg)
	{
		return;
	}

	if (req < 0 || req > cur_server_niu_cfg->req)
	{
		m_role->NoticeNum(errornum::EN_RA_SERVER_NIUEGG_TIMES_LIMIT);
		return;
	}

	if ((m_activity_info.ra_niu_egg_server_reward_has_fetch_reward_flag & (1 << req)) > 0)
	{
		m_role->NoticeNum(errornum::EN_RA_SERVER_NIUEGG_HAS_FATCHED);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const RandActivityNiuEggServerRewardConfig::RANiuEggConfigItem *cur_niu_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetNiuServerCfgWithReq(req);
	if (NULL == cur_niu_cfg)
	{
		return;
	}

	if (m_role->GetVip()->GetVipLevel() < cur_niu_cfg->vip_limit)
	{
		m_role->NoticeNum(errornum::EN_RA_SERVER_NIUEGG_VIP_LIMIT);
		return;
	}

	if (m_role->GetKnapsack()->Put(cur_niu_cfg->reward_item, PUT_REASON_RA_CHONGZHI_NIUEGG_REWARD))
	{
		m_activity_info.ra_niu_egg_server_reward_has_fetch_reward_flag |= (1 << req);
		this->SendRANiuEggInfo();

		gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRANiuEggFetchReward user[%d, %s], req[%d], reward[%s]",
			m_role->GetUID(), m_role->GetName(), req, ItemConfigDataLog(&cur_niu_cfg->reward_item, NULL));
	}
}

void RoleActivity::OnRaZhenbaogeOperReq(int opera, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ZHENBAOGE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_ZHENBAOGE);

	switch(opera)
	{
	case Protocol::RA_ZHENBAOGE_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRaZhenbaogeInfo();
		}
		break;

	case Protocol::RA_ZHENBAOGE_OPERA_TYPE_BUY:
		{
			this->OnRaZhenbaogeBuy(param);
		}
		break;

	case Protocol::RA_ZHENBAOGE_OPEAR_TYPE_BUY_ALL:
		{
			this->OnRaZhenbaogeBuyAll();
		}
		break;

	case Protocol::RA_ZHENBAOGE_OPEAR_TYPE_FLUSH:
		{
			this->OnRaZhenbaogeFlush(false, false);
		}
		break;

	case Protocol::RA_ZHENBAOGE_OPEAR_TYPE_RARE_FLUSH:
		{
			this->OnRaZhenbaogeRareFlush();
		}
		break;

	case Protocol::RA_ZHENBAOGE_OPERA_TYPE_FETCH_SERVER_GIFT:
		{
			this->OnRazhenbaogeFetchReward(param);
		}
		break;
	}
}

void RoleActivity::SendRaZhenbaogeInfo()
{
	Protocol::SCRAZhenbaogeInfo scrazbgi;

	RandActivityZhenBaoGe *zhenbaoge_activity = (RandActivityZhenBaoGe *)RandActivityManager::Instance().GetRandActivityZhenBaoGe();
	if (NULL != zhenbaoge_activity)
	{
		memcpy(scrazbgi.zhenbaoge_item_list, m_activity_info.ra_zhenbaoge_item_list, sizeof(scrazbgi.zhenbaoge_item_list));
		scrazbgi.zhenbaoge_refresh_gift_fetch_flag = m_activity_info.ra_zhenbaoge_refresh_gift_fetch_flag;
		scrazbgi.cur_flush_times = m_activity_info.ra_zhenbaoge_refresh_times;
		scrazbgi.reserve_sh = 0;
		scrazbgi.zhenbaoge_next_flush_timetamp = m_activity_info.ra_zhenbaoge_next_free_flush_timestamp;

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scrazbgi, sizeof(scrazbgi));
	}
}

void RoleActivity::OnRaZhenbaogeBuy(int index)
{
	if (index < 0 || index >= RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT)
	{
		return;
	}

	int seq = m_activity_info.ra_zhenbaoge_item_list[index];
	if (seq <= 0)
	{
		m_role->NoticeNum(errornum::EN_ZHENBAOGE_NOT_REFRESH);
		return;
	}

	const RandActivityZhenBaoGeConfig *zhenbaoge_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeCfg(seq);
	if (NULL == zhenbaoge_cfg)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(zhenbaoge_cfg->buy_consume_gold, "RoleActivity::OnRaZhenbaogeBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_activity_info.ra_zhenbaoge_item_list[index] = 0;

	bool ret = m_role->GetKnapsack()->Put(zhenbaoge_cfg->reward_item, PUT_REASON_RA_ZHENBAOGE_REWARD);

	this->SendRaZhenbaogeInfo();
	this->OnRaZhenbaogeCheckAllBuyGift();

	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRaZhenbaogeBuy user[%d, %s] index[%d] seq[%d] item[%s] ret[%d]",
		m_role->GetUID(), m_role->GetName(), index, seq, ItemConfigDataLog(&zhenbaoge_cfg->reward_item, NULL), ret ? 1 : 0);
}

void RoleActivity::OnRaZhenbaogeBuyAll()
{
	int total_consume_gold = 0;
	int count = 0;
	RandActivityZhenBaoGeConfig temp_item_list[RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT];

	for (int i = 0; i < RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT; ++ i)
	{
		if (m_activity_info.ra_zhenbaoge_item_list[i] > 0)
		{
			const RandActivityZhenBaoGeConfig *zhenbaoge_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeCfg(m_activity_info.ra_zhenbaoge_item_list[i]);
			if (NULL == zhenbaoge_cfg)
			{
				return;
			}

			temp_item_list[i] = *zhenbaoge_cfg;
			total_consume_gold += zhenbaoge_cfg->buy_consume_gold;
			++ count;
		}
	}

	if (total_consume_gold <= 0)
	{
		m_role->NoticeNum(errornum::EN_RA_ZHENBAOGE_HAVE_ALL_BUY);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(total_consume_gold, "RoleActivity::OnRaZhenbaogeBuyAll"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRaZhenbaogeBuyAll user[%d, %s] consume_gold[%d] item[",
		m_role->GetUID(), m_role->GetName(), total_consume_gold);

	for (int index = 0; index < RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT; ++ index)
	{
		if (temp_item_list[index].seq > 0)
		{
			if (m_role->GetKnapsack()->Put(temp_item_list[index].reward_item, PUT_REASON_RA_ZHENBAOGE_REWARD))
			{
				m_activity_info.ra_zhenbaoge_item_list[index] = 0;
				gamelog::g_log_roleactivity.buff_printf("%s", ItemConfigDataLog(&temp_item_list[index].reward_item, NULL));
			}
		}
	}

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);

	this->OnRaZhenbaogeCheckAllBuyGift();
	this->SendRaZhenbaogeInfo();
}

bool RoleActivity::OnRaZhenbaogeFlush(bool is_free, bool flush_for_rare)
{
	RandActivityZhenBaoGe *zhenbaoge_activity = (RandActivityZhenBaoGe *)RandActivityManager::Instance().GetRandActivityZhenBaoGe();
	if (NULL == zhenbaoge_activity)
	{
		return false;
	}

	int consume_gold = 0;
	bool has_get_rare_item = false;

	if (!is_free)
	{
		consume_gold = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeFlushConsumeGold();

		int replacement_item_id = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeFlushConsumeItem();

		if (m_role->GetKnapsack()->HasItem(replacement_item_id))
		{
			if (m_role->GetKnapsack()->ConsumeItem(replacement_item_id, 1, "OnRaZhenbaogeFlush"))
			{
				consume_gold = 0;
			}
		}
	}

	RandActivityZhenBaoGeConfig temp_result[RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT];

	const int server_chou_times = zhenbaoge_activity->GetServerReflushTimes();

	bool ret = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeRandomResult(m_activity_info.ra_zhenbaoge_add_weight_list,
		RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT, is_free, temp_result, RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT, server_chou_times);
	if (!ret)
	{
		return false;
	}

	// 消耗
	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "RoleActivity::OnRaZhenbaogeFlush"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	if (is_free)
	{
		//const tm *local_time = EngineAdapter::Instance().LocalTime();
		unsigned int interval = SECOND_PER_HOUR * 4;

		//if (0 == local_time->tm_hour % 2)
		//{
		//	interval = (EngineAdapter::Instance().NextHourInterval(0, 0)) + SECOND_PER_HOUR;
		//}
		//else
		//{
		//	interval = (EngineAdapter::Instance().NextHourInterval(0, 0));
		//}


		m_activity_info.ra_zhenbaoge_next_free_flush_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + interval;
	}

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRaZhenbaogeFlush user[%d, %s] is_free[%d] flush_for_rare[%d] consume_gold[%d] item_seq[",
		m_role->GetUID(), m_role->GetName(), is_free ? 1 : 0, flush_for_rare ? 1 : 0, consume_gold);

	// 给予
	for (int i = 0; i < RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT; ++ i)
	{
		m_activity_info.ra_zhenbaoge_item_list[i] = (short)temp_result[i].seq;

		if (temp_result[i].is_rare)
		{
			has_get_rare_item = true;

			// 刷到珍稀物品传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_zhenbaoge_zhenxi_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_ZHENBAOGE, temp_result[i].reward_item.item_id);

			if (length > 0 && !is_free)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}

		gamelog::g_log_roleactivity.buff_printf("(%d)", m_activity_info.ra_zhenbaoge_item_list[i]);
	}

	if (!is_free)
	{
		// 全服次数
		zhenbaoge_activity->AddServerReflushTimes();

		m_activity_info.ra_zhenbaoge_refresh_times++;
		this->OnRaZhenbaogeAddWeight();
	}

	if (!flush_for_rare)
	{
		this->SendRaZhenbaogeInfo();
	}

	if (!is_free && !flush_for_rare)
	{
		m_role->NoticeNum(errornum::EN_ZHENBAOGE_HAD_REFRESH);
	}

	this->OnRaZhenbaogeCheckResetWeight();

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);

	return (!has_get_rare_item);
}

void RoleActivity::OnRaZhenbaogeRareFlush()
{
	int times = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeAutoFlushTimes();
	int consume_gold = times * LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeFlushConsumeGold();

	int replacement_item_id = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeFlushConsumeItem();

	if (m_role->GetKnapsack()->HasItem(replacement_item_id))
	{
		int cur_item_num = m_role->GetKnapsack()->GetItemNum(replacement_item_id);

		if (times <= cur_item_num)
		{
			consume_gold = 0;
		}
		else
		{
			consume_gold = (times - cur_item_num) * LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeFlushConsumeGold();
		}
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(static_cast<long long>(consume_gold)))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	for (int i = 0; i < times; ++ i)
	{
		bool flush_has_rare = this->OnRaZhenbaogeFlush(false, true);
		if (!flush_has_rare)
		{
			break;
		}
	}

	m_role->NoticeNum(errornum::EN_ZHENBAOGE_HAD_REFRESH);
	this->SendRaZhenbaogeInfo();
}

void RoleActivity::OnRazhenbaogeFetchReward(int seq)
{
	const RandActivityZhenBaoGeServerConfig *server_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeServerCfg(seq);
	if (NULL == server_cfg)
	{
		return;
	}

	if ((m_activity_info.ra_zhenbaoge_refresh_gift_fetch_flag & (1 << seq)) > 0)
	{
		m_role->NoticeNum(errornum::EN_RA_ZHENBAOGE_HAVE_FETCH);
		return;
	}

	int cur_times = m_activity_info.ra_zhenbaoge_refresh_times;
	if (cur_times < server_cfg->can_fetch_times)
	{
		m_role->NoticeNum(errornum::EN_RA_ZHENBAOGE_TIMES_NOT_ENOUGH);
		return;
	}

	if (m_role->GetVip()->GetVipLevel() < server_cfg->vip_limit)
	{
		m_role->NoticeNum(errornum::EN_RA_ZHENBAOGE_VIP_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (m_role->GetKnapsack()->Put(server_cfg->reward_item, PUT_REASON_RA_ZHENBAOGE_REWARD))
	{
		m_activity_info.ra_zhenbaoge_refresh_gift_fetch_flag |= 1 << seq;
		this->SendRaZhenbaogeInfo();

		gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRazhenbaogeFetchReward user[%d, %s] reward_item[%s] flag[%d]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&server_cfg->reward_item, NULL), m_activity_info.ra_zhenbaoge_refresh_gift_fetch_flag);
	}
}

void RoleActivity::OnRaZhenbaogeCheckAllBuyGift()
{
	for (int i = 0; i < RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT; ++ i)
	{
		if (m_activity_info.ra_zhenbaoge_item_list[i] > 0)
		{
			return;
		}
	}

	const ItemConfigData reward_item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeAllBuyReward();

	m_role->GetKnapsack()->PutOrMail(reward_item_cfg, PUT_REASON_RA_ZHENBAOGE_REWARD);
}

void RoleActivity::OnRaZhenbaogeAddWeight()
{
	for (int i = 0; i < RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT; ++ i)
	{
		const RandActivityZhenBaoGeAddWeightConfig *add_weight_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeAddWeight(i);
		if (NULL == add_weight_cfg)
		{
			continue;
		}

		if (m_activity_info.ra_zhenbaoge_add_weight_list[i].seq == add_weight_cfg->seq)
		{
			m_activity_info.ra_zhenbaoge_add_weight_list[i].weight += add_weight_cfg->add_weight;
		}
	}
}

void RoleActivity::OnRaZhenbaogeCheckResetWeight()
{
	for (int item_index = 0; item_index < RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT; ++ item_index)
	{
		for(int weight_index = 0; weight_index < RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT; ++ weight_index)
		{
			if (m_activity_info.ra_zhenbaoge_item_list[item_index] == m_activity_info.ra_zhenbaoge_add_weight_list[weight_index].seq)
			{
				m_activity_info.ra_zhenbaoge_add_weight_list[weight_index].weight = 0;
			}
		}
	}
}

void RoleActivity::OnRaMiJingXunBaoOperReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MIJINGXUNBAO))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MIJINGXUNBAO);

	switch(opera_type)
	{
	case Protocol::RA_MIJINGXUNBAO_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAMiJingXunBaoInfo();
		}
		break;

	case Protocol::RA_MIJINGXUNBAO_OPERA_TYPE_TAO:
		{
			this->OnRAMiJingXunBaoTao(param);
		}
		break;

	case Protocol::RA_MIJINGXUNBAO_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRAMiJingXunBaoFetchPersonalReward(param);
		}
		break;

	}
}

void RoleActivity::OnRAMiJingXunBaoTao(int type)
{
	if (type < RA_MIJINGXUNBAO_CHOU_TYPE_1 || type >= RA_MIJINGXUNBAO_CHOU_TYPE_MAX)
	{
		return;
	}

	int consume_gold = 0;
	bool is_free_tao = false;
	int tao_times = 0;

	unsigned int nowtime = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	RandActivityConfig &randactivity_cfg = LOGIC_CONFIG->GetRandActivityCfg();

	unsigned int cooldown_interval = randactivity_cfg.GetMiJingXunBaoInterval() * SECOND_PER_MIN;							// 淘宝冷却时间

	consume_gold = randactivity_cfg.GetMiJingXunBaoTaoGold(type);
	if (consume_gold <= 0)
	{
		return;
	}

	bool has_item = false;
	ItemID item_id = 0;

	if (RA_MIJINGXUNBAO_CHOU_TYPE_1 == type)                    //获得淘宝花费的金币
	{
		tao_times = 1;

		if (nowtime >= m_activity_info.ra_mijingxunbao_next_free_tao_timestamp )
		{
			is_free_tao = true;
			consume_gold = 0;
		}
	}
	else if (RA_MIJINGXUNBAO_CHOU_TYPE_10 == type)
	{
		tao_times = 10;
	}
	else if (RA_MIJINGXUNBAO_CHOU_TYPE_50 == type)
	{
		tao_times = 50;

		item_id = LOGIC_CONFIG->GetRandActivityCfg().GetMiJingXunBaoTaoItem();
		if (m_role->GetKnapsack()->HasItem(item_id))
		{
			has_item = true;
			consume_gold = 0;
		}
	}
	else
	{
		return;
	}

	if(consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int reward_count = tao_times;

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, reward_count))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	if (reward_count > Protocol::SC_MiJingXunBaoTao_MAX_TIMES)
	{
		return;
	}

	static RandActivityMiJingXunBaoConfig reward_item_list[Protocol::SC_MiJingXunBaoTao_MAX_TIMES];

	bool ret = randactivity_cfg.GetMiJingXunBaoRandomResult(m_activity_info.ra_mijingxunbao_add_weight_list, randactivity_cfg.GetMiJingXunBaoAutoWeightAddCount(), is_free_tao, reward_item_list, reward_count);
	if (!ret)
	{
		return;
	}

	// 物品
	if (RA_MIJINGXUNBAO_CHOU_TYPE_50 == type)
	{
		if (has_item)
		{
			if (!m_role->GetKnapsack()->ConsumeItem(item_id, 1, "RoleActivity::OnRAMiJingXunBaoTao"))
			{
				return;
			}
		}
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "RoleActivity::OnRAMiJingXunBaoTao"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	//刷新下次免费淘宝时间
	if (is_free_tao)
	{
		m_activity_info.ra_mijingxunbao_next_free_tao_timestamp = nowtime + cooldown_interval;
		m_activity_info.ra_mijingxunbao_role_chou_times += tao_times;
		this->SendRAMiJingXunBaoInfo();

		// 免费单抽绑定
		reward_item_list[0].reward_item.is_bind = true;
	}

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAMiJingXunBaoTao user[%d, %s] is_free_tao[%d] consume_gold[%d] type[%d]",
		m_role->GetUID(), m_role->GetName(), (int)is_free_tao, consume_gold, type);

	static Protocol::SCRAMiJingXunBaoTaoResultInfo scramjxbtri;
	scramjxbtri.count = reward_count;

	ChestItemInfo item_list[Protocol::SC_MiJingXunBaoTao_MAX_TIMES];
	int list_num = 0;

	//给予
	for (int i = 0; i < reward_count && i < Protocol::SC_MiJingXunBaoTao_MAX_TIMES; ++ i)
	{
		item_list[i].Reset();

		item_list[i].item_id = reward_item_list[i].reward_item.item_id;
		item_list[i].num = reward_item_list[i].reward_item.num;
		item_list[i].is_bind = reward_item_list[i].reward_item.is_bind;

		++ list_num;
	}

//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, list_num, item_list, "OnRAMiJingXunBaoTao");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_MIJINGXUNBAO, list_num, item_list, "OnRAMiJingXunBaoTao");

	for (int index = 0; index < reward_count && index < Protocol::SC_MiJingXunBaoTao_MAX_TIMES; ++index)
	{
		gamelog::g_log_roleactivity.buff_printf("(%s), ", ItemConfigDataLog(&reward_item_list[index].reward_item, NULL));

		scramjxbtri.mijingxunbao_tao_seq[index] = reward_item_list[index].seq;
		if (reward_item_list[index].is_rare)
		{
			// 刷到珍稀物品传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_mijingxunbao_zhenxi_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), (int)reward_item_list[index].reward_item.item_id);

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}
	}

	//发送淘宝获得的物品信息
	{
		int send_len = sizeof(scramjxbtri) - sizeof(scramjxbtri.mijingxunbao_tao_seq[0]) * (Protocol::SC_MiJingXunBaoTao_MAX_TIMES - scramjxbtri.count);
		if (send_len > 0)
		{
			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scramjxbtri, send_len);
		}
	}

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);
}

void RoleActivity::OnRAMiJingXunBaoFetchPersonalReward(int seq)
{
	const RandActivityMiJingXunBaoRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityMiJingXunBaoPersonalRewardCfg(seq, m_activity_info.ra_mijingxunbao_role_chou_times);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (0 != (m_activity_info.ra_mijingxunbao_personal_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(1))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_mijingxunbao_personal_reward_flag |= 1 << seq;

	m_role->GetKnapsack()->Put(reward_cfg->reward_item, PUT_REASON_MIJINGXUNBAO_REWARD);

	this->SendRAMiJingXunBaoInfo();

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAMiJingXunBaoFetchPersonalReward user[%d, %s] reward_flag[%d] reward_item_id[%d] reward_item_num[%d]",
		m_role->GetUID(), m_role->GetName(), m_activity_info.ra_mijingxunbao_personal_reward_flag, reward_cfg->reward_item.item_id, reward_cfg->reward_item.num);
}

void RoleActivity::SendRAMiJingXunBaoInfo()
{
	Protocol::SCRAMiJingXunBaoInfo scramjxbi;
	scramjxbi.ra_mijingxunbao_next_free_tao_timestamp = m_activity_info.ra_mijingxunbao_next_free_tao_timestamp;
	scramjxbi.chou_times = m_activity_info.ra_mijingxunbao_role_chou_times;
	scramjxbi.reward_flag = m_activity_info.ra_mijingxunbao_personal_reward_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scramjxbi, sizeof(scramjxbi));
}

void RoleActivity::OnRaJiSuChongZhanGiveReward(const int charge_value)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_JISUCHPONGZHAN))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_JISUCHPONGZHAN);

	const RandActivityJiSuChongZhanConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetJiSuChongZhanRewardCfg(charge_value);
	if (NULL == reward_cfg)
	{
		return;
	}

	static MailContentParam contentparam;
	contentparam.Reset();

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg->reward_item.item_id);
	if (NULL != reward_item_base)
	{
		contentparam.item_list[0].item_id = reward_cfg->reward_item.item_id;
		contentparam.item_list[0].num = reward_cfg->reward_item.num;
		contentparam.item_list[0].is_bind = (reward_cfg->reward_item.is_bind) ? 1 : 0;
		contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();
	}

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_jisuchongzhan_reward_content, charge_value);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRaJiSuChongZhanGiveReward::user[%d, %s], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&reward_cfg->reward_item, NULL));
	}
}

void RoleActivity::OnRAChongzhiMoneyTreeOperReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MONEY_TREE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MONEY_TREE);

	switch(opera_type)
	{
	case Protocol::RA_MONEY_TREE_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRaMoneyTreeInfo();
		}
		break;

	case Protocol::RA_MONEY_TREE_OPERA_TYPE_CHOU:
		{
			this->OnRaMoneyTreeChou(param);
		}
		break;

	case Protocol::RA_MONEY_TREE_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRaMoneyTreeFetchReward(param);
		}
		break;
	}
}

void RoleActivity::SendRaMoneyTreeInfo()
{
	RandActivityMoneyTree *money_tree_act = (RandActivityMoneyTree *)RandActivityManager::Instance().GetRandActivityMoneyTree();
	if (NULL == money_tree_act)
	{
		return;
	}

	Protocol::SCRAMoneyTreeInfo ramti;
	ramti.server_reward_has_fetch_reward_flag = m_activity_info.ra_money_tree_server_reward_has_fetch_reward_flag;
	ramti.money_tree_total_times = m_activity_info.ra_money_tree_total_times;
	ramti.money_tree_free_timestamp = m_activity_info.ra_money_tree_free_timestamp;
	ramti.server_total_pool_gold = money_tree_act->GetServerTotalPoolGold();

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&ramti, sizeof(ramti));
}

void RoleActivity::OnRaMoneyTreeChou(int times)
{
	if (times <= 0 || times > Protocol::SC_RA_MONEYTREE_CHOU_MAX_COUNT_LIMIT)
	{
		return;
	}

	int need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTreeNeedGold() * times;
	if (need_gold <= 0)
	{
		return;
	}

	RandActivityMoneyTree *money_tree_act = (RandActivityMoneyTree *)RandActivityManager::Instance().GetRandActivityMoneyTree();
	if (NULL == money_tree_act)
	{
		return;
	}

	bool is_free = false;
	if (1 == times)
	{
		int free_times = LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTreeFreeTimes();
		if (m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_MONEY_TREE_COUNT, free_times))
		{
			int free_interval = LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTreeFreeInterval();
			if (static_cast<unsigned int >(EngineAdapter::Instance().Time()) >= m_activity_info.ra_money_tree_free_timestamp + free_interval)
			{
				m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_MONEY_TREE_COUNT);
				need_gold = 0;
				is_free = true;
				m_activity_info.ra_money_tree_free_timestamp = static_cast<unsigned int >(EngineAdapter::Instance().Time());
			}
		}
	}

	bool is_use_item = false;
	if (times == 30)
	{
		ItemID need_item = LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTreeConsumeItem();
		if (m_role->GetKnapsack()->HasItem(need_item))
		{
			is_use_item = true;
			need_gold = 0;
		}
	}

// 	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, times))
// 	{
// 		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_SPACE);
// 		return;
// 	}
	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(times))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 检查消耗
	if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRaMoneyTreeChou user[%d, %s], times[%d], need_gold[%d], reward[",
		m_role->GetUID(), m_role->GetName(), times, need_gold);

	static RandActivityMoneyTreeRewardConfig reward_list[Protocol::SC_RA_MONEYTREE_CHOU_MAX_COUNT_LIMIT];

	//int chest_item_count = 0;
// 	static ChestItemInfo chest_item_list[MODE_50_COUNT];
// 	memset(chest_item_list, 0, sizeof(chest_item_list));

	bool has_got_pool_gold = false;

	for (int i = 0; i < times; ++i)
	{
		// 保底奖励配置优先
		int server_chou_times = money_tree_act->GetServerTotalRockMoneyTreeTimes();
		const RandActivityServerBaodiConfig * baodi_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTreeBaodiRandReward(server_chou_times);
		if (NULL != baodi_cfg)
		{
			money_tree_act->ClearRockMoneyTreeTimes();
			reward_list[i].reward_item = baodi_cfg->reward_item;
			reward_list[i].is_broadcast = baodi_cfg->is_rare;
			reward_list[i].seq = baodi_cfg->seq;
			reward_list[i].prize_pool_percent = -1;
		}
		else
		{
			const RandActivityMoneyTreeRewardConfig *money_tree_cfg = NULL;
			if (has_got_pool_gold || is_free)
			{
				money_tree_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandomMoneyTreeItemRewardCfg();
			}
			else
			{
				money_tree_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandomMoneyTreeRewardCfg();

			}
			if (NULL == money_tree_cfg)
			{
				gamelog::g_log_roleactivity.clear_buff();
				return;
			}

			if (money_tree_cfg->prize_pool_percent > 0)
			{
				has_got_pool_gold = true;
			}
			reward_list[i] = *money_tree_cfg;
		}

		// 不一定给物品奖励需要过滤下
// 		if (chest_item_count < MODE_50_COUNT && money_tree_cfg->reward_item.item_id > 0)
// 		{
// 			chest_item_list[chest_item_count].item_id = money_tree_cfg->reward_item.item_id;
// 			chest_item_list[chest_item_count].num = money_tree_cfg->reward_item.num;
// 			chest_item_list[chest_item_count].is_bind = money_tree_cfg->reward_item.is_bind;
// 			++chest_item_count;
// 		}

		if (need_gold > 0)
		{
			money_tree_act->AddTotalTotalMoneyTreeTimes(1);
		}

		gamelog::g_log_roleactivity.buff_printf("(%s), ", ItemConfigDataLog(&reward_list[i].reward_item, NULL));
	}

	static Protocol::SCRAMoneyTreeChouResultInfo ramtcri;
	ramtcri.reward_req_list_count = times;

	// 消耗
	if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "RoleActivity::OnRaMoneyTreeChou"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (is_use_item)
	{
		if (!m_role->GetKnapsack()->ConsumeItem(LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTreeConsumeItem(), 1, "RoleActivity::OnRaMoneyTreeChou"))
		{
			return;
		}
	}

	// 给予
	int put_into_pool_gold = times * LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTreeIntoGold();

	for (int i = 0; i < times; ++ i)
	{
		int reward_gold = 0;
		if (reward_list[i].prize_pool_percent > 0)
		{
			reward_gold = money_tree_act->GetServerTotalPoolGold() * reward_list[i].prize_pool_percent / 100;

			money_tree_act->DecTotalPoolGold(reward_gold);
			m_role->GetKnapsack()->GetMoney()->AddGold(reward_gold, "RoleActivity::OnRaMoneyTreeChou");
		}
		else
		{
			m_role->GetKnapsack()->Put(reward_list[i].reward_item, PUT_REASON_RA_MONEY_TREE_REWARD);
		}

		ramtcri.reward_req_list[i] = static_cast<char>(reward_list[i].seq);
		
		
		if (reward_list[i].is_broadcast) // 配置了需要发传闻
		{
			// 传闻
			int length = 0;

			if (reward_list[i].prize_pool_percent > 0)	// 元宝
			{
				length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_money_tree_reward_content_gold,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_MONEY_TREE, (money_tree_act->GetServerTotalPoolGold() * reward_list[i].prize_pool_percent / 100));
			}
			else	// 道具
			{
				length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_money_tree_reward_content_item,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_MONEY_TREE, reward_list[i].reward_item.item_id);
			}

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}

		LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_MONEY_TREE, m_role->GetUID(), m_role->GetName(), reward_list[i].reward_item.item_id, reward_list[i].reward_item.num);
		gamelog::g_log_roleactivity.buff_printf("(%s) (gold: %d), ", ItemConfigDataLog(&reward_list[i].reward_item, NULL), reward_gold);
	}

	// 修改为放入仓库
	//m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, chest_item_count, chest_item_list, "RoleActivity::OnRaMoneyTreeChou");

	int len = sizeof(ramtcri) - (Protocol::SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT - ramtcri.reward_req_list_count) * sizeof(ramtcri.reward_req_list[0]);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&ramtcri, len);

	m_activity_info.ra_money_tree_total_times += times;
	if (need_gold > 0)
	{
		money_tree_act->AddTotalPoolGold(put_into_pool_gold);
	}

	this->SendRaMoneyTreeInfo();

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);
}

void RoleActivity::OnRaMoneyTreeFetchReward(int seq)
{
	RandActivityMoneyTree *money_tree_act = (RandActivityMoneyTree *)RandActivityManager::Instance().GetRandActivityMoneyTree();
	if (NULL == money_tree_act)
	{
		return;
	}

	const RandActivityMoneyTreeServerRewardConfig *cur_server_money_tree_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTreeServerCfgWithServerMoneyTreeTimes(m_activity_info.ra_money_tree_total_times);
	if (NULL == cur_server_money_tree_cfg)
	{
		return;
	}

	if (seq > cur_server_money_tree_cfg->seq)
	{
		m_role->NoticeNum(errornum::EN_RA_SERVER_MONEYTREE_TIMES_LIMIT);
		return;
	}

	if ((m_activity_info.ra_money_tree_server_reward_has_fetch_reward_flag & (1 << seq)) > 0)
	{
		m_role->NoticeNum(errornum::EN_RA_SERVER_MONEYTREE_HAS_FATCHED);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const RandActivityMoneyTreeServerRewardConfig *cur_tree_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTreeServerCfgWithReq(seq);
	if (NULL == cur_tree_cfg)
	{
		return;
	}

	if (m_role->GetVip()->GetVipLevel() < cur_tree_cfg->vip_limit)
	{
		m_role->NoticeNum(errornum::EN_RA_SERVER_MONEYTREE_VIP_LIMIT);
		return;
	}

	if (m_role->GetKnapsack()->Put(cur_tree_cfg->reward_item, PUT_REASON_RA_MONEY_TREE_REWARD))
	{
		m_activity_info.ra_money_tree_server_reward_has_fetch_reward_flag |= (1 << seq);
		this->SendRaMoneyTreeInfo();

		gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRaMoneyTreeFetchReward user[%d, %s], req[%d], reward[%s]",
			m_role->GetUID(), m_role->GetName(), seq, ItemConfigDataLog(&cur_tree_cfg->reward_item, NULL));
	}
}

void RoleActivity::OnRADailyLoveChongzhi(const int charge_value)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DAILY_LOVE))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAILY_LOVE);

	if (0 == m_activity_info.ra_daily_love_daily_first_flag)
	{
		return;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const int precent = LOGIC_CONFIG->GetRandActivityCfg().GetDailyLoveRewardPrecent(opengame_day);
	if (precent <= 0)
	{
		return;
	}

	m_activity_info.ra_daily_love_daily_first_flag = 0;

	static MailContentParam contentparam; contentparam.Reset();

	contentparam.gold = (charge_value * precent) / 100;

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_daily_love_reward_content, charge_value, contentparam.gold);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRADailyLoveChongzhi::user[%d, %s],charge_value[%d] charge_reward_gold[%d]",
			m_role->GetUID(), m_role->GetName(), charge_value, contentparam.gold);
	}

	this->OnSendDailyLoveChongzhiInfo();
}

void RoleActivity::OnSendDailyLoveChongzhiInfo()
{
	Protocol::SCLoveDailyInfo info;

	info.flag = m_activity_info.ra_daily_love_daily_first_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}

void RoleActivity::OnRAKingDrawOperaReq(int opera_type, int param1, int param2)	// param1是level，param2是times或翻牌下标
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_KING_DRAW))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_KING_DRAW);

	switch(opera_type)
	{
	case Protocol::RA_KING_DRAW_OPERA_TYPE_QUERY_INFO:
		{
			this->SendKingDrawInfo();
		}
		break;

	case Protocol::RA_KING_DRAW_OPERA_TYPE_PLAY_ONCE:
		{
			this->OnKingDrawPlayOnce(param1, param2);
		}
		break;

	case Protocol::RA_KING_DRAW_OPERA_TYPE_PLAY_TIMES:
		{
			this->OnKingDrawPlayTimes(param1, param2);
		}
		break;

	case Protocol::RA_KING_DRAW_OPERA_TYPE_REFRESH_CARD:
		{
			this->OnKingDrawRefreshCard(param1);
		}
		break;

	case Protocol::RA_KING_DRAW_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnKingDrawFetchReward(param1, param2);
		}
		break; 
	}
}

void RoleActivity::SendKingDrawInfo()
{
	static Protocol::SCRAKingDrawInfo kdi;

	memcpy(kdi.draw_times, m_activity_info.ra_kingdraw_draw_times, sizeof(kdi.draw_times));
	kdi.return_reward_flag = m_activity_info.ra_kingdraw_reward_flag;
	UNSTD_STATIC_CHECK(sizeof(kdi.card_list) == sizeof(m_activity_info.ra_kingdraw_card_list));
	memcpy(kdi.card_list, m_activity_info.ra_kingdraw_card_list, sizeof(kdi.card_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&kdi, sizeof(kdi));
}

void RoleActivity::OnKingDrawPlayOnce(int level, int index)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT || index < 0 || index >= RA_KING_DRAW_MAX_SHOWED_COUNT) // index 是翻牌下标，范围：[0, 8]
	{
		return;
	}

	// 判断该index是否已翻牌，-1代表未翻牌
	if (m_activity_info.ra_kingdraw_card_list[level].reward_list[index] != -1)
	{
		return;
	}

	RandActivityKingDraw *king_draw_act = (RandActivityKingDraw *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_KING_DRAW);
	if (NULL == king_draw_act)
	{
		return;
	}

	// 获得翻牌所需的元宝和随机奖励
	int need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawPlayOnceGold(level);

	// 保底奖励优先
	const int server_chou_times = king_draw_act->GetServerChouTimes();
	const RandActivityKingDrawRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawBaodiRewardCfg(level, server_chou_times);
	if (NULL != reward_cfg)
	{
		king_draw_act->ClearServerChouTimes();
	}
	else
	{
		reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawRandomRewardCfg(level, m_activity_info.ra_kingdraw_self_weight_list[level]);
	}

	if (NULL == reward_cfg || need_gold <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, 1))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "RoleActivity::OnKingDrawPlayOnce");

	// 修改翻牌的下标
	m_activity_info.ra_kingdraw_card_list[level].reward_list[index] = reward_cfg->seq;

	// 发奖励
	ChestItemInfo reward_item[MODE_1_COUNT];
	for (int i = 0; i < MODE_1_COUNT; ++ i)
	{
		reward_item[i].Reset();
		{
			reward_item[i].item_id = reward_cfg->reward_item.item_id;
			reward_item[i].num = reward_cfg->reward_item.num;
			reward_item[i].is_bind = reward_cfg->reward_item.is_bind;
		}
	}

//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, 1, reward_item, "OnKingDraw");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_KING_DRAW, 1, reward_item, "OnKingDraw");

	// 修改玩家的自增权重
	{
		bool is_reset = false;
		for (int i = 0; i < RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT; ++i)
		{
			if (reward_cfg->seq == m_activity_info.ra_kingdraw_self_weight_list[level][i].seq)
			{
				is_reset = true;
			}
		}

		if (is_reset)
		{
			this->KingDrawResetWeight(level);
		}
		else
		{
			this->KingDrawAddWeight(level);
		}
	}

	// 发传闻
	if (reward_cfg->is_rare)
	{
		int length = 0;

		length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_king_draw_reward_content,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), (int)reward_cfg->reward_item.item_id);

		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	// 全服次数
	king_draw_act->AddServerChouTimes(1);

	++m_activity_info.ra_kingdraw_draw_times[level];
	gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnKingDrawPlayOnce user[%d, %s], reward_seq[%d], level[%d], reward[%s]",
		m_role->GetUID(), m_role->GetName(), reward_cfg->seq, level, ItemConfigDataLog(&reward_cfg->reward_item, NULL));

	LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_KING_DRAW, m_role->GetUID(), m_role->GetName(), reward_cfg->reward_item.item_id, reward_cfg->reward_item.num);

	this->SendKingDrawInfo();

}

void RoleActivity::OnKingDrawPlayTimes(int level, int times)
{
	if (times < 2 || times > RA_KING_DRAW_MAX_REWARD_COUNT || level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)	// 一次的另有协议
	{
		return;
	}

	RandActivityKingDraw *king_draw_act = (RandActivityKingDraw *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_KING_DRAW);
	if (NULL == king_draw_act)
	{
		return;
	}

	static Protocol::SCRAKingDrawMultiReward kdmr;
	kdmr.reward_count = 0;

	static ChestItemInfo chest_shop_reward_item[RA_KING_DRAW_MAX_REWARD_COUNT];
	//static 变量一定要清理
	memset(chest_shop_reward_item, 0, sizeof(chest_shop_reward_item));

	// 查看宝物仓库是否还有空位
	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, times))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	// 50抽，可用道具
	bool is_use_item = false;
	if (times == RA_KING_DRAW_MAX_REWARD_COUNT)
	{
		if (m_role->GetKnapsack()->HasItem(LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawConsumeItem()))
		{
			is_use_item = true;
		}
	}

	gamelog::g_log_rand_activity.buff_printf("RoleActivity::onKingDrawPlayTimes user[%d, %s], play level[%d], times[%d], is_use_item[%d], rewards: ",
		m_role->GetUID(), m_role->GetName(), level, times, is_use_item ? 1 : 0);

	bool is_first_use = true;

	int server_chou_times = king_draw_act->GetServerChouTimes();

	int reward_count = 0;
	for (int index = 0; index < times; ++index)
	{
		// 获得翻牌所需的元宝和随机奖励
		int need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawPlayOnceGold(level);

		const RandActivityKingDrawRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawBaodiRewardCfg(level, server_chou_times);
		if (NULL != reward_cfg)
		{
			king_draw_act->ClearServerChouTimes();
		}
		else
		{
			reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawRandomRewardCfg(level, m_activity_info.ra_kingdraw_self_weight_list[level]);
		}

		if (NULL == reward_cfg || need_gold <= 0)
		{
			continue;
		}

		if (!is_use_item)
		{
			if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				break;
			}

			m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "RoleActivity::OnKingDrawPlayTimes");
		}
		else if (is_first_use)
		{
			is_first_use = false;

			if (!m_role->GetKnapsack()->ConsumeItem(LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawConsumeItem(), 1, "RoleActivity::OnKingDrawPlayTimes"))
			{
				is_use_item = false;
				continue;
			}
		}

		// 发奖励
		{
			chest_shop_reward_item[index].item_id = reward_cfg->reward_item.item_id;
			chest_shop_reward_item[index].num = reward_cfg->reward_item.num;
			chest_shop_reward_item[index].is_bind = reward_cfg->reward_item.is_bind;
		}

		if (is_use_item)
		{
			chest_shop_reward_item[reward_count].is_bind = true;
		}
		++ reward_count;

		if (kdmr.reward_count < RA_KING_DRAW_MAX_REWARD_COUNT)	// 判断是否越界
		{
			kdmr.reward_seq_list[kdmr.reward_count ++] = reward_cfg->seq;
		}

		// 修改玩家的自增权重
		{
			bool is_reset = false;
			for (int i = 0; i < RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT; ++i)
			{
				if (reward_cfg->seq == m_activity_info.ra_kingdraw_self_weight_list[level][i].seq)
				{
					is_reset = true;
				}
			}

			if (is_reset)
			{
				this->KingDrawResetWeight(level);
			}
			else
			{
				this->KingDrawAddWeight(level);
			}
		}

		++server_chou_times;

		// 发传闻
		if (reward_cfg->is_rare)
		{
			int length = 0;

			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_king_draw_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), (int)reward_cfg->reward_item.item_id);

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}

		gamelog::g_log_rand_activity.buff_printf("(reward_seq[%d] reward[%s])", reward_cfg->seq, ItemConfigDataLog(&reward_cfg->reward_item, NULL));
	
		LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_KING_DRAW, m_role->GetUID(), m_role->GetName(), reward_cfg->reward_item.item_id, reward_cfg->reward_item.num);
	}

	// 抽奖结果先下发再onbuyitemlist
	{
		static Protocol::SCChestShopItemListPerBuy per_buy_list;

		per_buy_list.shop_type = CHEST_SHOP_TYPE_EQUIP;
		per_buy_list.mode = 0;
		per_buy_list.count = times;
		memcpy(per_buy_list.item_list, chest_shop_reward_item, sizeof(chest_shop_reward_item));

		int length = sizeof(per_buy_list) - sizeof(per_buy_list.item_list) + sizeof(ChestItemInfo) * per_buy_list.count;
		if (length > 0)
		{
			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char *)&per_buy_list, length);
		}
	}

	//全服次数
	king_draw_act->AddServerChouTimes(times);

//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, reward_count, chest_shop_reward_item, "OnKingDraw");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_KING_DRAW, reward_count, chest_shop_reward_item, "OnKingDraw");
	m_activity_info.ra_kingdraw_draw_times[level] += times;

	gamelog::g_log_rand_activity.commit_buff(LL_INFO);

	if (kdmr.reward_count > 0)
	{
		kdmr.is_use_item = (int)is_use_item;
		int send_len = sizeof(kdmr) - (RA_KING_DRAW_MAX_REWARD_COUNT - kdmr.reward_count) * sizeof(kdmr.reward_seq_list[0]);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char *)&kdmr, send_len);
	}

	this->SendKingDrawInfo();
}

void RoleActivity::OnKingDrawRefreshCard(int level)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
	{
		return;
	}

	m_activity_info.ra_kingdraw_card_list[level].Reset();	// 刷新当前level的牌

	this->SendKingDrawInfo();
}

void RoleActivity::OnKingDrawFetchReward(int level, int seq)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
	{
		return;
	}

	int cfg_count = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawReturnRewardCfgCount(level);
	if (cfg_count <= 0 || seq < 0 || seq >= cfg_count || seq >= RandActivityKingDrawReturnRewardCfg::KING_DRAW_RETURN_REWARD_COUNT_PER_STAGE)
	{
		return;
	}

	const RandActivityKingDrawReturnRewardStageCfg *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawReturnRewardCfg(level, seq);
	if (NULL != cfg)
	{
		if (static_cast<int>(m_activity_info.ra_kingdraw_draw_times[level]) < cfg->draw_times)
		{
			return;
		}

		if (0 != (m_activity_info.ra_kingdraw_reward_flag & (1 << (seq + level * RandActivityKingDrawReturnRewardCfg::KING_DRAW_RETURN_REWARD_COUNT_PER_STAGE))))
		{
			return;
		}

		m_activity_info.ra_kingdraw_reward_flag |= (1 << (seq + level * RandActivityKingDrawReturnRewardCfg::KING_DRAW_RETURN_REWARD_COUNT_PER_STAGE));
		m_role->GetKnapsack()->PutOrMail(cfg->reward_item, PUT_REASON_KING_DRAW_RETURN_REWARD);

		LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_KING_DRAW, m_role->GetUID(), m_role->GetName(), cfg->reward_item.item_id, cfg->reward_item.num);
		
		this->SendKingDrawInfo();
	}
}

void RoleActivity::KingDrawAddWeight(int level)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
	{
		return;
	}

	int self_weigth_cfg_count = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawSelfWeigthCfgCount(level);

	for (int seq = 0; seq < RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT && seq < self_weigth_cfg_count; ++ seq)
	{
		const RandActivitySelfWeightConfig * weight_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawSelfWeightCfg(level, seq);
		if (NULL != weight_cfg)
		{
			m_activity_info.ra_kingdraw_self_weight_list[level][seq].auto_weight_add += weight_cfg->auto_add_weight;
		}
	}
}

void RoleActivity::KingDrawResetWeight(int level)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
	{
		return;
	}

	int self_weight_cfg_count = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawSelfWeigthCfgCount(level);

	for (int seq = 0; seq < self_weight_cfg_count && seq < RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT; ++ seq)
	{
		m_activity_info.ra_kingdraw_self_weight_list[level][seq].auto_weight_add = 0;
	}
}

void RoleActivity::OnRATotalCharge3CheckChongzhi(int chongzhi)
{
	if (chongzhi <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3);

	m_activity_info.ra_total_charge_3_cur_total_charge += chongzhi;

	this->RACheckTotalCharge3RewardCheck();

	this->OnSendTotalCharge3Info();
}

void RoleActivity::OnSendTotalCharge3Info()
{
	static Protocol::SCRATotalCharge3Info tci;
	tci.cur_total_charge = m_activity_info.ra_total_charge_3_cur_total_charge;
	tci.cur_total_charge_has_fetch_flag = m_activity_info.ra_total_charge_3_cur_has_fetch_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tci, sizeof(tci));
}

void RoleActivity::OnPassiveUseSpecialAppearanceCard()
{
	if (!RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK);

	++ m_activity_info.ra_special_appearance_card_passive_use_times;

	RandActivitySpecialAppearancePassiveRank *app_rank_activity = (RandActivitySpecialAppearancePassiveRank*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK);
	if (NULL != app_rank_activity)
	{
		app_rank_activity->UpdateRankUser(m_role, m_activity_info.ra_special_appearance_card_passive_use_times);
	}
}

void RoleActivity::OnUseSpecialAppearanceCard()
{
	if (!RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK);

	++ m_activity_info.ra_special_appearance_card_use_times;

	RandActivitySpecialAppearanceRank *app_rank_activity = (RandActivitySpecialAppearanceRank*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK);
	if (NULL != app_rank_activity)
	{
		app_rank_activity->UpdateRankUser(m_role, m_activity_info.ra_special_appearance_card_use_times);
	}
}


void RoleActivity::SendRASpecialAppearanceInfo()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK);

	if (!RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK))
	{
		return;
	}

	RandActivitySpecialAppearanceRank *app_rank_activity = (RandActivitySpecialAppearanceRank*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK);
	if (NULL != app_rank_activity)
	{
		Protocol::SCRASpecialAppearanceInfo rasai;
		rasai.role_change_times = m_activity_info.ra_special_appearance_card_use_times;
		rasai.rank_count = app_rank_activity->GetRankList(rasai.rank_list);

		int send_len = sizeof(rasai) - (RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT - rasai.rank_count) * sizeof(rasai.rank_list[0]);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&rasai, send_len);
	}
}

void RoleActivity::RACheckSpecialAppearacePassiveJoinReward()
{
	if (m_activity_info.ra_special_appearance_card_passive_use_times <= 0)
	{
		return;
	}

	if (m_activity_info.ra_special_appearance_passive_has_got_join_reward > 0)
	{
		return;
	}

	const ItemConfigData &join_reward = LOGIC_CONFIG->GetRandActivityCfg().GetAppearacePassiveRankJoinReward();
	const ItemBase *item_base = ITEMPOOL->GetItem(join_reward.item_id);
	if (NULL != item_base)
	{
		static MailContentParam contentparam; contentparam.Reset();
		contentparam.item_list[0].item_id = join_reward.item_id;
		contentparam.item_list[0].num = join_reward.num;
		contentparam.item_list[0].is_bind = join_reward.is_bind;
		contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_appearace_passive_rank_join_reward_content);
		if (length > 0)
		{
			m_activity_info.ra_special_appearance_passive_has_got_join_reward = 1;
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::RACheckSpecialAppearacePassiveJoinReward user %d %s, change_num %d, reward %s,",
				UidToInt(m_role->GetUserId()), m_role->GetName(), m_activity_info.ra_special_appearance_card_passive_use_times, ItemConfigDataLog(&join_reward, NULL));
		}
	}
}

void RoleActivity::RACheckSpecialAppearaceJoinReward()
{
	if (m_activity_info.ra_special_appearance_card_use_times <= 0)
	{
		return;
	}

	if (m_activity_info.ra_special_appearance_has_got_join_reward > 0)
	{
		return;
	}

	const ItemConfigData &join_reward = LOGIC_CONFIG->GetRandActivityCfg().GetAppearaceRankJoinReward();
	const ItemBase *item_base = ITEMPOOL->GetItem(join_reward.item_id);
	if (NULL != item_base)
	{
		static MailContentParam contentparam; contentparam.Reset();
		contentparam.item_list[0].item_id = join_reward.item_id;
		contentparam.item_list[0].num = join_reward.num;
		contentparam.item_list[0].is_bind = join_reward.is_bind;
		contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_appearace_rank_join_reward_content);
		if (length > 0)
		{
			m_activity_info.ra_special_appearance_has_got_join_reward = 1;
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::RACheckSpecialAppearaceJoinReward user %d %s, change_num %d, reward %s,",
				UidToInt(m_role->GetUserId()), m_role->GetName(), m_activity_info.ra_special_appearance_card_use_times, ItemConfigDataLog(&join_reward, NULL));
		}
	}
}


void RoleActivity::SendRASpecialAppearancePassiveInfo()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK);

	if (!RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK))
	{
		return;
	}

	RandActivitySpecialAppearancePassiveRank *app_rank_activity = (RandActivitySpecialAppearancePassiveRank *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK);
	if (NULL != app_rank_activity)
	{
		Protocol::SCRASpecialAppearancePassiveInfo rasai;
		rasai.role_change_times = m_activity_info.ra_special_appearance_card_passive_use_times;
		rasai.rank_count = app_rank_activity->GetRankList(rasai.rank_list);

		int send_len = sizeof(rasai) - (RA_SPECIAL_APPEARANCE_PASSIVE_RANK_MAX_COUNT - rasai.rank_count) * sizeof(rasai.rank_list[0]);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&rasai, send_len);
	}
}


void RoleActivity::OnKillAnyMonster(UInt16 monster_id)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ANY_MONSTER_DROP)
		&& !ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ANY_MONSTER_DROP2)
		&& !ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NATIONAL_DAY))
	{
		return;
	}

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	unsigned int next_drop_time = m_activity_info.ra_any_monster_drop_last_timestamp + LOGIC_CONFIG->GetRandActivityCfg().GetAnyMonsterDropLimitDropTime();

	if (now_time < next_drop_time)
	{
		return;
	}

	DROPPOOL->DropHelper(LOGIC_CONFIG->GetRandActivityCfg().GetAnyMonsterDropID(), m_role->GetScene(), m_role->GetPos(), m_role->GetUserId(), m_role->GetProf(), monster_id, false, false, NULL);

	m_activity_info.ra_any_monster_drop_last_timestamp = now_time;
}

bool RoleActivity::OnUseAnyMonsterDropActivityItem(ItemID item_id, int num)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ANY_MONSTER_DROP)
		&& !ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ANY_MONSTER_DROP2))
	{
		m_role->NoticeNum(errornum::EN_RA_MONSTER_DROP_ITEM_USE_LIMIT);
		return false;
	}

	const RandActivityAnyMonsterDropConfig *drop_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetAndyMonsterDropCfg(item_id);
	if (NULL == drop_cfg)
	{
		return false;
	}

	int day_limit_rate = LOGIC_CONFIG->GetRandActivityCfg().GetAnyMonsterDropDayLimitRate();
	if (m_activity_info.ra_any_monster_drop_today_add_rate >= day_limit_rate)
	{
		m_role->NoticeNum(errornum::EN_RA_MONSTER_DROP_ITEM_USE_TIMES_LIMIT);
		return false;
	}

	m_activity_info.ra_any_monster_drop_today_add_rate += (drop_cfg->add_attr_rate * num);
	m_activity_info.ra_any_monster_drop_total_add_rate += (drop_cfg->add_attr_rate * num);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_RAND_ACTIVITY);

	return true;
}

void RoleActivity::RACheckTotalCharge4RewardCheck()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4))
	{
		return;
	}
	
	int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4), EngineAdapter::Instance().Time());
	int count = LOGIC_CONFIG->GetRandActivityCfg().GetTotalCharge4CfgCount(day_index);

	for (int i = 0; i < count; ++ i)
	{
		const RandActivityTotalCharge4Cfg::ConfigItem *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTotalCharge4Cfg(day_index, i);
		if (NULL == cfg)
		{
			continue;
		}

		if (m_activity_info.ra_total_charge_4_cur_total_charge < cfg->need_chongzhi_num)
		{
			continue;
		}

		if (0 != (m_activity_info.ra_total_charge_4_cur_has_fetch_flag & (1 << cfg->seq)))
		{
			continue;
		}

		RandActivityTotalCharge4 *Charge4 = dynamic_cast<RandActivityTotalCharge4*>(RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4));
		if (nullptr == Charge4)
		{
			return;
		}

		if (Charge4->GetRemainReward() == MAX_REWARD)
		{
			return;                                                   //当全服奖励份数用完之后，对所有角色都不再下发奖励
		}

		Charge4->OnUseReward();

		static MailContentParam contentparam; contentparam.Reset();
		for (int reward_index = 0; reward_index < MAX_ATTACHMENT_ITEM_NUM; ++ reward_index)
		{
			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg->reward_item[reward_index].item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[reward_index].item_id = cfg->reward_item[reward_index].item_id;
				contentparam.item_list[reward_index].num = cfg->reward_item[reward_index].num;
				contentparam.item_list[reward_index].is_bind = (cfg->reward_item[reward_index].is_bind) ? 1 : 0;
				contentparam.item_list[reward_index].invalid_time = reward_item_base->CalInvalidTime();
			}
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_charge4_reward_content, RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4, cfg->need_chongzhi_num);
		if (length > 0)
		{
			// 消耗
			m_activity_info.ra_total_charge_4_cur_has_fetch_flag |= (1 << cfg->seq);

			// 给予
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::RACheckTotalCharge4RewardCheck user[%d, %s] chongzhi_num[%d], reward_flag[%d], reward_item_1[%s] reward_item_2[%s] reward_item_3[%s]",
				m_role->GetUID(), m_role->GetName(), cfg->need_chongzhi_num, m_activity_info.ra_total_charge_4_cur_has_fetch_flag, ItemConfigDataLog(&cfg->reward_item[0], NULL),
				ItemConfigDataLog(&cfg->reward_item[1], NULL), ItemConfigDataLog(&cfg->reward_item[2], NULL));
		}
	}
}

void RoleActivity::OnRATotalCharge4CheckChongzhi(int chongzhi)
{
	if (chongzhi <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4);

	m_activity_info.ra_total_charge_4_cur_total_charge += chongzhi;

	this->RACheckTotalCharge4RewardCheck();

	this->OnSendTotalCharge4Info();
}

void RoleActivity::OnSendTotalCharge4Info()
{
	RandActivityTotalCharge4 *Charge4 = dynamic_cast<RandActivityTotalCharge4*>(RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4));
	if (nullptr == Charge4)
	{
		return;
	}

	static Protocol::SCRATotalCharge4Info tci;
	tci.cur_total_charge = m_activity_info.ra_total_charge_4_cur_total_charge;
	tci.cur_consume_reward = Charge4->GetRemainReward();
	tci.ra_total_charge_4_cur_has_fetch_flag = m_activity_info.ra_total_charge_4_cur_has_fetch_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tci, sizeof(tci));
}

void RoleActivity::OnRAMineOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MINE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MINE);

	switch(opera_type)
	{
	case Protocol::RA_MINE_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAMineAllInfo();
		}
		break;

	case Protocol::RA_MINE_OPERA_REFRESH:
		{
			this->OnRAMineRefresh();
		}
		break;

	case Protocol::RA_MINE_OPERA_GATHER:
		{
			this->OnRAGatherMine(param1);
		}
		break;

	case Protocol::RA_MINE_OPERA_FETCH_SERVER_REWARD:
		{
			this->OnRAMineFetchReward(param1);
		}
		break;

	case Protocol::RA_MINE_OPERA_EXCHANGE_REWARD:
		{
			this->OnRAMineExchangeReward(param1);
		}
		break;
	}
}

void RoleActivity::SendRAMineAllInfo()
{
	static Protocol::SCRAMineAllInfo mai;
	RandActivityMine *mine_activity = (RandActivityMine*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_MINE);
	if (NULL == mine_activity)
	{
		return;
	}

	mai.total_refresh_times = mine_activity->GetTotalRefreshTimes();
	mai.role_refresh_times = m_activity_info.ra_mine_role_refresh_times;
	mai.free_gather_times = m_activity_info.ra_mine_free_gather_times;
	mai.next_refresh_time = m_activity_info.ra_mine_next_refresh_time;
	mai.cur_reward_fetch_flag = m_activity_info.ra_mine_cur_reward_fetch_flag;
	mai.reward_begin_index = mine_activity->GetRewardBeginIndex();
	mai.reward_end_index = mine_activity->GetRewardEndIndex();
	memcpy(mai.gather_count_list, m_activity_info.ra_mine_gather_count_list, sizeof(mai.gather_count_list));
	memcpy(mai.mine_cur_type_list, m_activity_info.ra_mine_type_list, sizeof(mai.mine_cur_type_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&mai, sizeof(mai));
}

void RoleActivity::OnRAMineRefresh(bool is_use_gold)
{
	if (is_use_gold)
	{
		// 检查
		if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(LOGIC_CONFIG->GetRandActivityCfg().GetMineRefreshGold()))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}
	else
	{
		if (EngineAdapter::Instance().Time() < m_activity_info.ra_mine_next_refresh_time)
		{
			return;
		}
	}

	RandActivityMine *mine_act = (RandActivityMine *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_MINE);
	if (nullptr == mine_act)
	{
		return ;
	}

	const int server_chou_times = mine_act->GetTotalRefreshTimes();

	// 获取
	memset(m_activity_info.ra_mine_type_list, 0, sizeof(m_activity_info.ra_mine_type_list));
	int mine_refresh_count = LOGIC_CONFIG->GetRandActivityCfg().GetMineRefreshCount();

	if (!LOGIC_CONFIG->GetRandActivityCfg().GetMineRefreshResult(m_activity_info.ra_mine_type_list, mine_refresh_count, m_activity_info.ra_mine_role_refresh_times, server_chou_times))
	{
		return;
	}

	if (is_use_gold)
	{
		// 消耗
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(LOGIC_CONFIG->GetRandActivityCfg().GetMineRefreshGold(), "RoleActivity::OnMineRefresh"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		++ m_activity_info.ra_mine_role_refresh_times;		// 增加个人换矿次数
	}
	else if (!is_use_gold)
	{
		// 刷新下一次自动刷时间
		m_activity_info.ra_mine_next_refresh_time = (unsigned int)EngineAdapter::Instance().Time() + RA_MINE_REFRESH_TIME_INTERVAL;
	}

	// 传闻
	for (int index = 0; index < RA_MINE_MAX_REFRESH_COUNT; ++ index)
	{
		const RandActivityMineInfoStageConfig *mine_info = LOGIC_CONFIG->GetRandActivityCfg().GetMineInfoCfg(m_activity_info.ra_mine_type_list[index] - RA_MINE_TYPES_BEGIN);
		if (NULL == mine_info)
		{
			continue;
		}

		if (mine_info->is_broadcast)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_mine_role_refresh_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), mine_info->seq);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}
	}

	// 非免费增加全服抽奖次数
	if (is_use_gold)
	{
		mine_act->AddTotalRefreshTimes();
	}

	// 发送
	this->SendRAMineAllInfo();

	// 日志
	gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnMineRefresh user[%d, %s], is_use_gold[%d], total_refresh_times[%d], role_refresh_times[%d]",
		m_role->GetUID(), m_role->GetName(), is_use_gold, m_activity_info.ra_mine_role_refresh_times, m_activity_info.ra_mine_role_refresh_times);
}

void RoleActivity::OnRAGatherMine(int index)
{
	if (index < 0 || index >= RA_MINE_MAX_REFRESH_COUNT)
	{
		return;
	}

	RandActivityMine *mine_activity = (RandActivityMine*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_MINE);
	if (NULL == mine_activity)
	{
		return;
	}

	int mine_type_index = m_activity_info.ra_mine_type_list[index] - RA_MINE_TYPES_BEGIN;
	if (mine_type_index < 0 || mine_type_index >= RandActivityMineInfoConfig::RA_MINE_INFO_STAGE_LIST_MAX_LEN) return;

	bool is_use_gold = false;

	if (m_activity_info.ra_mine_free_gather_times > 0)	// 免费挖
	{
		++ m_activity_info.ra_mine_gather_count_list[mine_type_index];
		m_activity_info.ra_mine_type_list[index] = 0;

		-- m_activity_info.ra_mine_free_gather_times;
	}
	else	// 元宝挖
	{
		const RandActivityMineInfoStageConfig *mine_info = LOGIC_CONFIG->GetRandActivityCfg().GetMineInfoCfg(mine_type_index);
		if (NULL == mine_info)
		{
			return;
		}

		// 消耗
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(mine_info->need_gold, "RoleActivity::OnGrabMine"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		is_use_gold = true;

		++ m_activity_info.ra_mine_gather_count_list[mine_type_index];
		m_activity_info.ra_mine_type_list[index] = 0;
	}

	m_role->NoticeNum(noticenum::NT_MINE_GATHER_SUCC);

	// 发送
	this->SendRAMineAllInfo();

	// 日志
	gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnGatherMine user[%d, %s], seq[%d], is_use_gold[%d]",
		m_role->GetUID(), m_role->GetName(), mine_type_index, is_use_gold);
}

void RoleActivity::OnRAMineFetchReward(int index)
{
	const RandActivityMineServerRewardStageConfig *server_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMineServerRewardCfg(index);
	if (NULL == server_reward_cfg)
	{
		return;
	}

	if (m_role->GetVip()->GetVipLevel() < server_reward_cfg->vip_limit)
	{
		m_role->NoticeNum(errornum::EN_RA_MINE_FETCH_SERVER_REWARD_VIP_LIMIT);
		return;
	}

	// 检查
	if (m_activity_info.ra_mine_role_refresh_times < server_reward_cfg->total_refresh_times)
	{
		return;
	}

	if (m_role->GetLevel() < LOGIC_CONFIG->GetRandActivityCfg().GetMineFetchServerRewardLevel())
	{
		m_role->NoticeNum(errornum::EN_RA_MINE_FETCH_SERVER_REWARD_LEVEL_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (0 == (m_activity_info.ra_mine_cur_reward_fetch_flag & (1 << index)))	// 如果未领取
	{

		m_activity_info.ra_mine_cur_reward_fetch_flag |= (1 << index);

		// 发放
		m_role->GetKnapsack()->Put(server_reward_cfg->reward_item, PUT_REASON_MINE_FETCH_SERVER_REWARD);

		// 发送
		this->SendRAMineAllInfo();

		// 日志
		gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnFetchServerReward user[%d, %s], seq[%d], fetch_flag[%d], reward[%s]",
			m_role->GetUID(), m_role->GetName(), index, m_activity_info.ra_mine_cur_reward_fetch_flag, ItemConfigDataLog(&server_reward_cfg->reward_item, NULL));
	}
}

void RoleActivity::OnRAMineExchangeReward(int index)
{
	if (index < 0 || index >= RA_MINE_MAX_TYPE_COUNT)
	{
		return;
	}

	const RandActivityMineInfoStageConfig *mine_info = LOGIC_CONFIG->GetRandActivityCfg().GetMineInfoCfg(index);
	if (NULL == mine_info)
	{
		return;
	}

	// 检查
	if (m_activity_info.ra_mine_gather_count_list[index] < mine_info->exchange_need_num)
	{
		m_role->NoticeNum(errornum::EN_RA_MINE_EXCHANGE_COUNT_NOT_ENOUGH);
		return;
	}

	// 消耗
	m_activity_info.ra_mine_gather_count_list[index] -= mine_info->exchange_need_num;

	// 发放
	if (!m_role->GetKnapsack()->HaveSpace())
	{
		// 背包不足，邮件发送
		static MailContentParam contentparam; contentparam.Reset();

		const ItemBase *reward_item_base = ITEMPOOL->GetItem(mine_info->exchange_item.item_id);
		if (NULL != reward_item_base)
		{
			contentparam.item_list[0].item_id = mine_info->exchange_item.item_id;
			contentparam.item_list[0].num = mine_info->exchange_item.num;
			contentparam.item_list[0].is_bind = (mine_info->exchange_item.is_bind) ? 1 : 0;
			contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_mine_exchange_reward_content);
		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}

		m_role->NoticeNum(errornum::EN_RA_MINE_EXCHANGE_REWARD_SEND_BY_MAIL);
	}
	else
	{
		m_role->GetKnapsack()->Put(mine_info->exchange_item, PUT_REASON_MINE_EXCANGE);
	}

	// 发送
	this->SendRAMineAllInfo();

	// 日志
	gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnExchangeReward user[%d, %s], seq[%d], reward[%s]",
		m_role->GetUID(), m_role->GetName(), index, ItemConfigDataLog(&mine_info->exchange_item, NULL));

	LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_MINE, m_role->GetUID(), m_role->GetName(), mine_info->exchange_item.item_id, mine_info->exchange_item.num);
}

void RoleActivity::OnRAGuaGuaOperaReq(int opera_type, int param1)	// param1是刮奖区域下标，或者是刮奖多次的类型（10次或50次）
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_GUAGUA))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_GUAGUA);

	switch (opera_type)
	{
	case Protocol::RA_GUAGUA_OPERA_TYPE_QUERY_INFO:
	{
		this->SendGuaGuaInfo();
	}
	break;

	case Protocol::RA_GUAGUA_OPERA_TYPE_PLAY_TIMES:
	{
		this->OnGuaGuaPlayTimes(param1);
	}
	break;

	case Protocol::RA_GUAGUA_OPREA_TYPE_FETCH_REWARD:
	{
		this->OnGuaGuaFetchReward((unsigned int)param1);
	}
	break;

	}
}

void RoleActivity::SendGuaGuaInfo()
{
	static Protocol::SCRAGuaGuaInfo ggi;

	ggi.guagua_acc_count = m_activity_info.ra_guagua_acc_count;
	ggi.guagua_acc_reward_has_fetch_flag = m_activity_info.ra_guagua_acc_reward_has_fetch_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&ggi, sizeof(ggi));
}

void RoleActivity::OnGuaGuaPlayTimes(int type)				// type是刮奖多次的类型（10或50）
{
	// 获取刮的次数
	int times = 0;

	switch (type)
	{
	case RA_GUAGUA_PLAY_ONE_TIME:
		times = 1;
		break;

	case RA_GUAGUA_PLAY_TEN_TIMES:
		times = 10;
		break;

	case RA_GUAGUA_PLAY_THIRTY_TIMES:
		times = 30;
		break;
	}

	if (times <= 0 || times > RA_GUAGUA_MAX_REWARD_COUNT)
	{
		return;
	}

	bool is_use_item = false;
	if (RA_GUAGUA_PLAY_THIRTY_TIMES == type)
	{
		if (m_role->GetKnapsack()->HasItem(LOGIC_CONFIG->GetRandActivityCfg().GetGuaGuaRollItemID()))
		{
			is_use_item = true;
		}
	}

	// 获得刮奖所需的元宝
	int need_gold = 0;

	if (!is_use_item)
	{
		need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetGuaGuaPlayTimesGold(type);
		if (need_gold <= 0)
		{
			return;
		}
	}

	RandActivityGuaGua *activity = (RandActivityGuaGua *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_GUAGUA);
	if (nullptr == activity)
	{
		return;
	}

	// 检查元宝是否足够
	if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 检查寻宝仓库背包是否足够
	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, RA_GUAGUA_REWARD_COUNT_PER_AREA * times))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	static Protocol::SCRAGuaGuaMultiReward ggmr;
	ggmr.reward_count = 0;
	ggmr.is_bind = 0;

	static RandActivityGuaGuaConfig guagua_cfg;
	int is_win = 0;

	gamelog::g_log_rand_activity.buff_printf("RoleActivity::OnGuaGuaPlayTimes user[%d, %s], play times[%d], need_gold[%d] rewards: ",
		m_role->GetUID(), m_role->GetName(), times, need_gold);

	static ChestItemInfo reward_item[RA_GUAGUA_MAX_REWARD_COUNT];
	for (int i = 0; i < RA_GUAGUA_MAX_REWARD_COUNT; ++i)
	{
		ggmr.reward_seq_list[i] = -1;
		reward_item[i].Reset();
	}

	// 获得随机奖励
	for (int i = 0; i < times && i < RA_GUAGUA_MAX_REWARD_COUNT; ++i)
	{
		activity->AddRollTimes();
		if (!LOGIC_CONFIG->GetRandActivityCfg().GetGuaGuaRandomRewardCfg(&guagua_cfg, &is_win, activity->GetRollTimes()))
		{
			return;
		}

		if (guagua_cfg.baodi)
		{
			activity->ClearRollTimes();
		}

		ggmr.reward_seq_list[i] = is_win ? guagua_cfg.seq : -1;

		if (NULL != ITEMPOOL->GetItem(guagua_cfg.reward_item.item_id))
		{
			reward_item[i].item_id = guagua_cfg.reward_item.item_id;
			reward_item[i].num = guagua_cfg.reward_item.num;
			reward_item[i].is_bind = guagua_cfg.reward_item.is_bind;
			reward_item[i].has_lucky = guagua_cfg.is_special;
		}

		gamelog::g_log_rand_activity.buff_printf("time[%d]: reward_seq[%d], ", i + 1, ggmr.reward_seq_list[ggmr.reward_count]);

		gamelog::g_log_rand_activity.buff_printf("(reward_%d[%s]) ", i, ItemConfigDataLog(&guagua_cfg.reward_item, NULL));

		if (ggmr.reward_count < RA_KING_DRAW_MAX_REWARD_COUNT) { ++ggmr.reward_count; }
	}

	int	reward_count = ggmr.reward_count;

	// 消耗道具
	if (is_use_item)
	{
		m_role->GetKnapsack()->ConsumeItem(LOGIC_CONFIG->GetRandActivityCfg().GetGuaGuaRollItemID(), 1, "OnGuaGuaPlayTimes");
	}
	// 消耗元宝
	else
	{
		m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "OnGuaGuaPlayTimes");
	}

	// 发奖励
//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, reward_count, reward_item, "OnGuaGuaPlayTimes");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_GUAGUA, reward_count, reward_item, "OnGuaGuaPlayTimes");

	// 传闻
	for (int i = 0; i < reward_count; ++i)
	{
		if (reward_item[i].has_lucky)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_guagua_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), reward_item[i].item_id);

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}

		LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_GUAGUA, m_role->GetUID(), m_role->GetName(), reward_item[i].item_id, reward_item[i].num);
	}
	// 抽奖累计次数
	m_activity_info.ra_guagua_acc_count += times;

	this->SendGuaGuaInfo();

	if (ggmr.reward_count > 0)
	{
		int send_len = sizeof(ggmr) - (RA_KING_DRAW_MAX_REWARD_COUNT - ggmr.reward_count) * sizeof(ggmr.reward_seq_list[0]);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&ggmr, send_len);
	}

	gamelog::g_log_rand_activity.commit_buff(LL_INFO);

	int total_gold = 0;
	int fifty_chou_times = 0;
	int ten_chou_times = 0;
	int one_chou_times = 0;

	if (50 == times)
	{
		fifty_chou_times++;
	}
	else if (10 == times)
	{
		ten_chou_times++;
	}
	else
	{
		one_chou_times++;
	}

	if (need_gold > 0)
	{
		total_gold += need_gold;
	}

	gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnGuaGuaPlayTimes, user[%d, %s], total_gold[%d], fifty_chou_times[%d], ten_chou_times[%d], one_chou_times[%d]",
		m_role->GetUID(), m_role->GetName(), total_gold, fifty_chou_times, ten_chou_times, one_chou_times);
}

void RoleActivity::OnGuaGuaFetchReward(unsigned int reward_index)
{
	const RandActivityGuaGuaAccConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetGuaGuaAccRewardByIndex(reward_index);
	if ((reward_index < 0) || (NULL == cfg) || (m_activity_info.ra_guagua_acc_count < cfg->acc_count))
	{
		m_role->NoticeNum(errornum::EN_DANBI_CHONGZHI_REWARD_HAS_NOT_ACTIVE);
		return;
	}

	if (0 != (m_activity_info.ra_guagua_acc_reward_has_fetch_flag & (1 << reward_index)))	// reward_index: 0~n
	{
		m_role->NoticeNum(errornum::EN_DANBI_CHONGZHI_REWARD_HAS_FETCH);
		return;
	}

	/*
	* 1、通过索引访问配置的累计次数
	* 2、配置的累计次数与数据库的累计次数比较
	* 3、将相应位置1, 表示可以领取
	* 4、发送新的数据给客户端
	*/
	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}
	m_activity_info.ra_guagua_acc_reward_has_fetch_flag |= (1 << reward_index);

	// 发奖励
	static  ItemConfigData reward_item;
	reward_item.item_id = cfg->reward_item.item_id;
	reward_item.num = cfg->reward_item.num;
	reward_item.is_bind = cfg->reward_item.is_bind;
	m_role->GetKnapsack()->Put(reward_item, PUT_REASON_GUAGUA);
	this->SendGuaGuaInfo();
}

void RoleActivity::OnRATianMingDivinationOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TIANMING_DIVINATION))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TIANMING_DIVINATION);

	switch(opera_type)
	{
	case Protocol::RA_TIANMING_DIVINATION_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRATianMingDivinationActivityInfo();
		}
		break;

	case Protocol::RA_TIANMING_DIVINATION_OPERA_TYPE_ADD_LOT_TIMES:
		{
			this->OnRATianMingDivinationAddLotTimes(param1);	// param1是竹签下标
		}
		break;

	case Protocol::RA_TIANMING_DIVINATION_OPERA_TYPE_RESET_ADD_LOT_TIMES:
		{
			this->OnRATianMingDivinationResetAddLotTimes();
		}
		break;

	case Protocol::RA_TIANMING_DIVINATION_OPERA_TYPE_START_CHOU:
		{
			this->OnRATianMingDivinationStartChou();
		}
		break;
	}

}

int RoleActivity::GetRATianMingDivinationTotalAddLotTimes()
{
	int total_times = 0;
	for(int i = 0; i < RA_TIANMING_LOT_COUNT; ++i)
	{
		total_times += (m_activity_info.ra_tianming_add_lot_times_list[i] - 1);
	}

	return total_times;
}

void RoleActivity::SendRATianMingDivinationActivityInfo()
{
	static Protocol::SCRATianMingDivinationInfo tmdi;
	tmdi.total_add_lot = this->GetRATianMingDivinationTotalAddLotTimes();
	tmdi.free_chou_times = m_activity_info.ra_tianming_free_chou_times;

	for(int i = 0; i < RA_TIANMING_LOT_COUNT; ++i)
	{
		tmdi.add_lots_list[i] = m_activity_info.ra_tianming_add_lot_times_list[i];
	}

	for(int i = 0; i < RA_TIANMING_REWARD_HISTORY_COUNT; ++i)
	{
		tmdi.reward_history_item_count_list[i].seq = m_activity_info.ra_tianming_reward_history_list[i].seq;
		tmdi.reward_history_item_count_list[i].add_lot = m_activity_info.ra_tianming_reward_history_list[i].add_lot;
	}

	tmdi.reward_history_list_cur_index = m_activity_info.ra_tianming_reward_history_list_cur_index;

	unsigned int send_len = sizeof(tmdi);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tmdi, send_len);

}

void RoleActivity::OnRATianMingDivinationAddLotTimes(int lot_index)
{
	if (lot_index < 0 || lot_index >= RA_TIANMING_LOT_COUNT)
	{
		return;
	}

	if (m_activity_info.ra_tianming_add_lot_times_list[lot_index] == RAND_ACTIVITY_TIANMING_DIVINATION_EVERY_LOT_MAX_ADD_TIMES + 1)	// 初始倍数为1
	{
		m_role->NoticeNum(errornum::EN_RA_TIAN_MING_DIVINATION_MAX_ADD_LOT);
		return;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	int total_add_lot = this->GetRATianMingDivinationTotalAddLotTimes();
	const RandActivityTianMingDivinationGoldConsumeConfig * tianming_gold_consume_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTianMingDivinationGoldConsumeCfg(total_add_lot + 1);
	if (NULL == tianming_gold_consume_cfg)
	{
		m_role->NoticeNum(errornum::EN_RA_TIAN_MING_DIVINATION_ALL_MAX_ADD_LOT);
		return;
	}

	int need_gold = tianming_gold_consume_cfg->add_consume_gold;
	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "RoleActivity::OnRATianMingDivinationAddLotTimes"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	++ m_activity_info.ra_tianming_add_lot_times_list[lot_index];
	this->SendRATianMingDivinationActivityInfo();
}

void RoleActivity::OnRATianMingDivinationResetAddLotTimes()
{
	for(int i = 0; i < RA_TIANMING_LOT_COUNT; ++i)
	{
		m_activity_info.ra_tianming_add_lot_times_list[i] = 1;
	}

	this->SendRATianMingDivinationActivityInfo();
}

void RoleActivity::OnRATianMingDivinationStartChou()
{
	int total_add_lot = this->GetRATianMingDivinationTotalAddLotTimes();
	const RandActivityTianMingDivinationGoldConsumeConfig * tianming_gold_consume_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTianMingDivinationGoldConsumeCfg(total_add_lot);
	if (NULL == tianming_gold_consume_cfg)
	{
		return;
	}

	RandActivityTianMingDivination *tianming_activity = (RandActivityTianMingDivination *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_TIANMING_DIVINATION);
	if (NULL == tianming_activity)
	{
		return;
	}

	int begin_index = tianming_activity->GetRefreshBeginIndex();
	int end_index = tianming_activity->GetRefreshEndIndex();
	if (end_index - begin_index >= RAND_ACTIVITY_TIANMING_DIVINATION_OPENGAME_DAY_INTERVAL || end_index <= begin_index)
	{
		return;
	}

	int need_gold = tianming_gold_consume_cfg->chou_consume_gold;
	bool use_free_chou = false;

	if (0 == total_add_lot && m_activity_info.ra_tianming_free_chou_times < LOGIC_CONFIG->GetRandActivityCfg().GetTianMingDivinationFreeChouTimes())
	{
		use_free_chou = true;
		need_gold = 0;
	}

	if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int reward_seq = 0;
	int lot_index = -1;

	// 保底奖励优先
	const int server_chou_times = tianming_activity->GetServerChouTimes();
	const RandActivityTianMingDivinationRewardConfig *tianming_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTianMingDivinationBaodiRewardCfg(server_chou_times);

	if (NULL != tianming_reward_cfg)
	{
		reward_seq = tianming_reward_cfg->seq;
		if (tianming_reward_cfg->can_add_lot)
		{
			++lot_index;
		}

		tianming_activity->ClearServerChouTimes();
	}
	else
	{
		int max_seq = LOGIC_CONFIG->GetRandActivityCfg().GetTianMingDivinationRewardMaxSeq();

		int total_weight = 0;
		for (int seq = begin_index; seq < end_index && seq < max_seq && seq < RAND_ACTIVITY_TIANMING_DIVINATION_REWARD_MAX_SEQ; ++seq)
		{
			tianming_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTianMingDivinationRewardCfg(seq);
			if (NULL == tianming_reward_cfg)
			{
				continue;
			}

			total_weight += tianming_reward_cfg->weight;
		}

		if (total_weight <= 0) return;

		int rand_value = rand() % total_weight;
		for (int seq = begin_index; seq < end_index && seq < max_seq && seq < RAND_ACTIVITY_TIANMING_DIVINATION_REWARD_MAX_SEQ; ++seq)
		{
			tianming_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTianMingDivinationRewardCfg(seq);
			if (NULL == tianming_reward_cfg)
			{
				continue;
			}

			if (tianming_reward_cfg->can_add_lot)
			{
				++lot_index;
			}

			if (rand_value < tianming_reward_cfg->weight)
			{
				reward_seq = seq;
				break;
			}

			rand_value -= tianming_reward_cfg->weight;
		}
	}

	if (reward_seq < 0)
	{
		return;
	}

	int reward_lot = 1; // 奖励倍数，默认1是未加注
	if (total_add_lot > 0 && tianming_reward_cfg->can_add_lot && lot_index >= 0 && lot_index < RA_TIANMING_LOT_COUNT)
	{
		reward_lot = m_activity_info.ra_tianming_add_lot_times_list[lot_index];
	}

	int reward_count = tianming_reward_cfg->reward_item.num * reward_lot;

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(tianming_reward_cfg->reward_item.item_id);
	if (NULL == reward_item_base || reward_item_base->GetPileLimit() <= 0)
	{
		return;
	}

	if (reward_item_base->GetPileLimit() <= 0) return;

	int need_empty_slot_num = reward_count / reward_item_base->GetPileLimit();
	if (reward_count % reward_item_base->GetPileLimit() > 0)
	{
		++ need_empty_slot_num;
	}

	// 检查寻宝仓库背包是否足够
	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, need_empty_slot_num))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	if (use_free_chou)	// 消耗免费次数
	{
		++ m_activity_info.ra_tianming_free_chou_times;
	}
	else
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "RoleActivity::OnRATianMingDivinationStartChou"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		// 增加全服次数
		tianming_activity->AddServerChouTimes(1);
	}

	static ChestItemInfo reward_item;
	reward_item.item_id = tianming_reward_cfg->reward_item.item_id;
	reward_item.num = reward_count;
	reward_item.is_bind = tianming_reward_cfg->reward_item.is_bind;
	if (use_free_chou) reward_item.is_bind = true;

	// 发放奖励到寻宝仓库
//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, 1, &reward_item, "OnRATianMingDivinationStartChou");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_TIANMING_DIVINATION, 1, &reward_item, "OnRATianMingDivinationStartChou");

	LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_TIANMING_DIVINATION, m_role->GetUID(), m_role->GetName(), tianming_reward_cfg->reward_item.item_id, reward_count);

	// 10倍加注传闻
	if (reward_lot > RAND_ACTIVITY_TIANMING_DIVINATION_EVERY_LOT_MAX_ADD_TIMES)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_tian_ming_divination_reward_content,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), (int)tianming_reward_cfg->reward_item.item_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	//珍稀传闻
	if (tianming_reward_cfg->is_broadcast)
	{
		int length_1 = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_tian_ming_divination_rare_reward,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), (int)tianming_reward_cfg->reward_item.item_id);
		if (length_1 > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length_1, SYS_MSG_CENTER_ROLL);
		}
	}

	// 更新卜卦奖励历史记录
	{
		m_activity_info.ra_tianming_reward_history_list[(int)m_activity_info.ra_tianming_reward_history_list_cur_index].seq = reward_seq;
		m_activity_info.ra_tianming_reward_history_list[(int)m_activity_info.ra_tianming_reward_history_list_cur_index].add_lot = reward_lot;
		++ m_activity_info.ra_tianming_reward_history_list_cur_index;
		m_activity_info.ra_tianming_reward_history_list_cur_index %= RA_TIANMING_REWARD_HISTORY_COUNT;
	}

	this->SendRATianMingDivinationStartChouResult(reward_seq);
	this->SendRATianMingDivinationActivityInfo();

	// 活动参与日志
	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRATianMingDivinationStartChou user[%d, %s], reward_seq[%d], add_lot[%d] reward_item[%s] free_chou_times[%d]",
		m_role->GetUID(), m_role->GetName(), reward_seq, reward_lot, ItemConfigDataLog(&tianming_reward_cfg->reward_item, NULL), m_activity_info.ra_tianming_free_chou_times);
}

void RoleActivity::SendRATianMingDivinationStartChouResult(int reward_index)
{
	static Protocol::SCTianMingDivinationActivityStartChouResult tmdascr;
	tmdascr.reward_index = reward_index;
	tmdascr.reserve_sh = 0;

	unsigned int send_len = sizeof(tmdascr);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tmdascr, send_len);
}

void RoleActivity::OnRAFanFanOperaReq(int opera_type, int param1, int param2)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_FANFAN))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_FANFAN);

	switch(opera_type)
	{
	case Protocol::RA_FANFAN_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAFanFanAllInfo();
		}
		break;

	case Protocol::RA_FANFAN_OPERA_TYPE_FAN_ONCE:
		{
			this->OnRAFanFanPlayOnce(param1);
		}
		break;

	case Protocol::RA_FANFAN_OPERA_TYPE_FAN_ALL:
		{
			this->OnRAFanFanGetAll();
		}
		break;

	case Protocol::RA_FANFAN_OPERA_TYPE_REFRESH:
		{
			if (param1 > 0 && param2 >= 0)
			{
				int is_get_target = 0;

				bool ret = this->OnRAFanFanRefresh(true, param2, &is_get_target);
				this->OnRAFanFanPlayOnce(RandomNum(RA_FANFAN_CARD_COUNT));		// 自动翻一次

				if (!ret || is_get_target > 0)
				{
					m_role->SendOperateResult(Protocol::SCOperateResult::OP_RA_FANFAN_REFRESH, 0);
					return;
				}

				m_role->SendOperateResult(Protocol::SCOperateResult::OP_RA_FANFAN_REFRESH, 1);
			}
			else
			{
				this->OnRAFanFanRefresh(0 != param1);
			}
		}
		break;

	case Protocol::RA_FANFAN_OPERA_TYPE_WORD_EXCHANGE:
		{
			this->OnRAFanFanWordExchange(param1);
		}
		break;

	case Protocol::RA_FANFAN_OPERA_TYPE_LEICHOU_EXCHANGE:
		{
			this->OnRAFanFanLeichouExchange(param1);
		}
		break;
	}
}

void RoleActivity::SendRAFanFanAllInfo()
{
	static Protocol::SCRAFanFanAllInfo ffai;

	ffai.ra_fanfan_next_refresh_time = m_activity_info.ra_fanfan_next_refresh_time;
	ffai.ra_fanfan_hidden_word_info = m_activity_info.ra_fanfan_hidden_word_info;
	ffai.ra_fanfan_cur_free_times = m_activity_info.ra_fanfan_cur_free_times;
	ffai.ra_fanfan_leichou_times = m_activity_info.ra_fanfan_leichou_times;
	ffai.ra_fanfan_reward_flag = m_activity_info.ra_fanfan_reward_flag;
	memcpy(ffai.ra_fanfan_card_type_list, m_activity_info.ra_fanfan_card_type_list, sizeof(ffai.ra_fanfan_card_type_list));
	memcpy(ffai.ra_fanfan_word_active_info_list, m_activity_info.ra_fanfan_word_active_info_list, sizeof(ffai.ra_fanfan_word_active_info_list));
	memset(ffai.reserve_ch, 0, sizeof(ffai.reserve_ch));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ffai, sizeof(ffai));
}

void RoleActivity::OnRAFanFanPlayOnce(int index)
{
	// 检查
	if (index < 0 || index >= RA_FANFAN_CARD_COUNT)
	{
		return;
	}

	if (m_activity_info.ra_fanfan_card_type_list[index] != RA_FANFAN_CARD_TYPE_HIDDEN)
	{
		return;
	}

	int fan_once_need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanOnceNeedGold();
	bool is_free_fan = (m_activity_info.ra_fanfan_cur_free_times > 0);

	if (!is_free_fan && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(fan_once_need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	RAFanFanHiddenWordInfo &hidden_word_info = m_activity_info.ra_fanfan_hidden_word_info;
	if (!hidden_word_info.IsValid())
	{
		return;
	}

	int hidden_word_real_seq = hidden_word_info.hidden_word_seq - RA_FANFAN_CARD_TYPE_WORD_BEGIN;
	RAFanFanWordActiveInfo &word_active_info = m_activity_info.ra_fanfan_word_active_info_list[hidden_word_real_seq];

	if (word_active_info.active_count > RAFanFanWordActiveInfo::MAX_WORD_ACTIVE_COUNT)
	{
		m_role->NoticeNum(errornum::EN_RA_FANFAN_WORD_REWARD_FULL);
		return;
	}

	// 重算概率
	int letter_occur_rate_tmp = m_activity_info.ra_fanfan_cur_letter_occur_rate;
	{
		int cur_fan_cards = 0;
		for (int i = 0; i < RA_FANFAN_CARD_COUNT; ++ i)
		{
			if (m_activity_info.ra_fanfan_card_type_list[i] != RA_FANFAN_CARD_TYPE_HIDDEN)
			{
				++ cur_fan_cards;
			}
		}

		if (cur_fan_cards >= RA_FANFAN_CARD_COUNT)
		{
			return;
		}

		// 每10次才会出一个字
		int active_phase = cur_fan_cards / 10;
		bool is_letter_active = word_active_info.IsLetterActive(active_phase);

		if (is_letter_active)
		{
			letter_occur_rate_tmp = 0;
		}
		else
		{
			if (m_activity_info.ra_fanfan_hidden_word_info.GetHiddenLetterCount() > 0)
			{
				if (0 == (cur_fan_cards + 1) % 10)
				{
					letter_occur_rate_tmp = 100;
				}
				else
				{
					letter_occur_rate_tmp += LOGIC_CONFIG->GetRandActivityCfg().GetFanFanWordIncreaseRate();
				}
			}
		}
	}

	// 获取翻牌情况
	int is_reward_word = 0;
	int reward_seq = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanRandomRewardSeq(letter_occur_rate_tmp, &is_reward_word);
	if (reward_seq < 0)
	{
		return;
	}

	const RandActivityFanFanItemInfo *item_info = NULL;
	if (!is_reward_word)
	{
		if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, 1))
		{
			m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
			return;
		}

		item_info = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanItemInfoCfg(reward_seq - RA_FANFAN_CARD_TYPE_ITEM_BEGIN);
		if (NULL == item_info)
		{
			return;
		}
	}

	// 消耗
	if (is_free_fan)
	{
		-- m_activity_info.ra_fanfan_cur_free_times;
	}
	else
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(fan_once_need_gold, "RoleActivity::OnRAFanFanPlayOnce"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	// 给予
	if (is_reward_word)
	{
		int letter_active_index = word_active_info.ActiveWord();
		hidden_word_info.hidden_letter_pos_list[letter_active_index] = index + RAFanFanHiddenWordInfo::POS_BEGIN;

		m_activity_info.ra_fanfan_card_type_list[index] = hidden_word_info.hidden_word_seq;
		m_activity_info.ra_fanfan_cur_letter_occur_rate = 0;
	}
	else
	{
		//ItemConfigData reward_item = item_info->reward_item;
		static ChestItemInfo item_list;
		item_list.Reset();

		item_list.item_id =  item_info->reward_item.item_id;
		item_list.num = item_info->reward_item.num;
		item_list.is_bind = item_info->reward_item.is_bind;

//		m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, 1, &item_list, "RoleActivity::OnRAFanFanPlayOnce");
		this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_FANFAN, 1, &item_list, "RoleActivity::OnRAFanFanPlayOnce");

		// 传闻
		if (item_info->is_broadcast)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_fanfan_reward_item_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), (int)item_info->reward_item.item_id);

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}

		m_activity_info.ra_fanfan_card_type_list[index] = reward_seq;
		m_activity_info.ra_fanfan_cur_letter_occur_rate = letter_occur_rate_tmp;
	}

	++m_activity_info.ra_fanfan_leichou_times;
	// 发送
	this->SendRAFanFanAllInfo();

	// 日志
	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRAFanFanPlayOnce user[%d, %s], index[%d], fan_once_need_gold[%d], is_free_fan[%d], free_times[%d], cur_letter_occur_rate[%d], is_reward_word[%d], reward_seq[%d], hidden_word_seq[%d], word_active_flag[%d], word_active_count[%d]",
		m_role->GetUID(), m_role->GetName(), index, fan_once_need_gold, (int)is_free_fan, m_activity_info.ra_fanfan_cur_free_times, m_activity_info.ra_fanfan_cur_letter_occur_rate,
		(int)is_reward_word, reward_seq, hidden_word_info.hidden_word_seq, word_active_info.active_flag, word_active_info.active_count);
}

void RoleActivity::OnRAFanFanGetAll()
{
	// 检查
	for (int index = 0; index < RA_FANFAN_MAX_WORD_COUNT; ++ index)
	{
		RAFanFanWordActiveInfo &word_active_info = m_activity_info.ra_fanfan_word_active_info_list[index];

		if (word_active_info.active_count > RAFanFanWordActiveInfo::MAX_WORD_ACTIVE_COUNT)
		{
			m_role->NoticeNum(errornum::EN_RA_FANFAN_WORD_REWARD_FULL);
			return;
		}
	}

	int fan_once_need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanOnceNeedGold();
	int need_fan_times = 0;
	int need_gold_fan_times = 0;

	for (int index = 0; index < RA_FANFAN_CARD_COUNT; ++ index)
	{
		if (RA_FANFAN_CARD_TYPE_HIDDEN == m_activity_info.ra_fanfan_card_type_list[index])
		{
			++ need_fan_times;
		}
	}

	if (0 == need_fan_times)
	{
		m_role->NoticeNum(errornum::EN_RA_FANFAN_CARD_LIST_FULL);
		return;
	}

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, need_fan_times))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	if (need_fan_times > m_activity_info.ra_fanfan_cur_free_times)
	{
		need_gold_fan_times = need_fan_times - m_activity_info.ra_fanfan_cur_free_times;
	}

	if (need_gold_fan_times > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(fan_once_need_gold * need_gold_fan_times))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 临时卡组，模拟给予
	unsigned char card_type_list_tmp[RA_FANFAN_CARD_COUNT];
	memcpy(card_type_list_tmp, m_activity_info.ra_fanfan_card_type_list, sizeof(card_type_list_tmp));

	RAFanFanHiddenWordInfo hidden_word_tmp; hidden_word_tmp.Reset();
	hidden_word_tmp = m_activity_info.ra_fanfan_hidden_word_info;
	if (!hidden_word_tmp.IsValid())
	{
		return;
	}

	{
		// 给字组
		int hidden_word_count = m_activity_info.ra_fanfan_hidden_word_info.GetHiddenLetterCount();
		for (int index = 0, put_word_times = 0; index < RA_FANFAN_CARD_COUNT && put_word_times < hidden_word_count; ++ index)
		{
			if (RA_FANFAN_CARD_TYPE_HIDDEN == card_type_list_tmp[index])
			{
				card_type_list_tmp[index] = hidden_word_tmp.hidden_word_seq;

				int put_word_index = RA_FANFAN_LETTER_COUNT_PER_WORD - put_word_times - 1;
				hidden_word_tmp.hidden_letter_pos_list[put_word_index] = index + RAFanFanHiddenWordInfo::POS_BEGIN;

				++ put_word_times;
			}
		}

		// 给物品
		for (int index = 0; index < RA_FANFAN_CARD_COUNT; ++ index)
		{
			if (RA_FANFAN_CARD_TYPE_HIDDEN == card_type_list_tmp[index])
			{
				int reward_seq = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanRandomRewardSeq(0, NULL);
				if (reward_seq <= 0)
				{
					return;
				}

				card_type_list_tmp[index] = reward_seq;
			}
		}

		// 打乱单字顺序
		int swap_times = 0;
		int loop_times = 0;
		while (swap_times < hidden_word_count && loop_times ++ < 50)
		{
			int rand_pos = RandomNum(RA_FANFAN_CARD_COUNT);

			if (card_type_list_tmp[rand_pos] >= RA_FANFAN_CARD_TYPE_ITEM_BEGIN && card_type_list_tmp[rand_pos] < RA_FANFAN_CARD_TYPE_WORD_BEGIN &&
				RA_FANFAN_CARD_TYPE_HIDDEN == m_activity_info.ra_fanfan_card_type_list[rand_pos])
			{
				int put_word_index = RA_FANFAN_LETTER_COUNT_PER_WORD - swap_times - 1;
				int put_word_real_pos = hidden_word_tmp.hidden_letter_pos_list[put_word_index] - RAFanFanHiddenWordInfo::POS_BEGIN;

				int car_type_tmp = card_type_list_tmp[rand_pos];
				card_type_list_tmp[rand_pos] = card_type_list_tmp[put_word_real_pos];
				card_type_list_tmp[put_word_real_pos] = car_type_tmp;

				hidden_word_tmp.hidden_letter_pos_list[put_word_index] = rand_pos + RAFanFanHiddenWordInfo::POS_BEGIN;

				++ swap_times;
			}
		}
	}

	static ChestItemInfo item_list[RA_FANFAN_CARD_COUNT];
	for (int i = 0; i < RA_FANFAN_CARD_COUNT; ++ i)
	{
		item_list[i].Reset();
	}

	int reward_count = 0;
	for (; reward_count < RA_FANFAN_CARD_COUNT; ++ reward_count)
	{
		if (RA_FANFAN_CARD_TYPE_HIDDEN == card_type_list_tmp[reward_count])
		{
			return;
		}

		int reward_seq = card_type_list_tmp[reward_count];
		if (reward_seq >= RA_FANFAN_CARD_TYPE_ITEM_BEGIN && reward_seq < RA_FANFAN_CARD_TYPE_ITEM_BEGIN + RA_FANFAN_MAX_ITEM_COUNT)
		{
			const RandActivityFanFanItemInfo *item_info = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanItemInfoCfg(reward_seq - RA_FANFAN_CARD_TYPE_ITEM_BEGIN);
			if (NULL == item_info)
			{
				return;
			}

			item_list[reward_count].item_id = item_info->reward_item.item_id;
			item_list[reward_count].num = item_info->reward_item.num;
			item_list[reward_count].is_bind = item_info->reward_item.is_bind;
		}
	}

	// 消耗
	if (need_gold_fan_times > 0)
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(fan_once_need_gold * need_gold_fan_times, "RoleActivity::OnRAFanFanGetAll"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		m_activity_info.ra_fanfan_cur_free_times = 0;
	}
	else
	{
		m_activity_info.ra_fanfan_cur_free_times -= need_fan_times;
	}

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAFanFanGetAll user[%d %s], need_fan_times[%d], need_gold_fan_times[%d], cur_free_times[%d], once_need_gold[%d], reward_count[%d], hidden_word_seq[%d] reward: ",
		m_role->GetUID(), m_role->GetName(), need_fan_times, need_gold_fan_times, m_activity_info.ra_fanfan_cur_free_times, fan_once_need_gold, reward_count, hidden_word_tmp.hidden_word_seq);

	// 给予
//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, reward_count, item_list, "RoleActivity::OnRAFanFanGetAll");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_FANFAN, reward_count, item_list, "RoleActivity::OnRAFanFanGetAll");

	// 修改卡组
	RAFanFanWordActiveInfo &word_active_info = m_activity_info.ra_fanfan_word_active_info_list[hidden_word_tmp.hidden_word_seq - RA_FANFAN_CARD_TYPE_WORD_BEGIN];
	word_active_info.ClearActiveFlag();
	++ word_active_info.active_count;

	// 如果原本就没有隐藏字，那么不应该多加一次激活次数
	if (m_activity_info.ra_fanfan_hidden_word_info.GetHiddenLetterCount() <= 0)
	{
		-- word_active_info.active_count;
	}

	m_activity_info.ra_fanfan_hidden_word_info = hidden_word_tmp;
	memcpy(m_activity_info.ra_fanfan_card_type_list, card_type_list_tmp, sizeof(m_activity_info.ra_fanfan_card_type_list));

	// 清零概率
	m_activity_info.ra_fanfan_cur_letter_occur_rate = 0;
	m_activity_info.ra_fanfan_leichou_times += need_fan_times;
	// 发送
	this->SendRAFanFanAllInfo();

	// 循环只做传闻和日志用
	for (int index = 0; index < RA_FANFAN_CARD_COUNT; ++ index)
	{
		int reward_seq = card_type_list_tmp[index];
		if (reward_seq >= RA_FANFAN_CARD_TYPE_ITEM_BEGIN && reward_seq < RA_FANFAN_CARD_TYPE_ITEM_BEGIN + RA_FANFAN_MAX_ITEM_COUNT)
		{
			const RandActivityFanFanItemInfo *item_info = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanItemInfoCfg(reward_seq - RA_FANFAN_CARD_TYPE_ITEM_BEGIN);
			if (NULL == item_info)
			{
				continue;
			}

			if (item_info->is_broadcast)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_fanfan_reward_item_content,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), (int)item_info->reward_item.item_id);

				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
				}
			}

			gamelog::g_log_roleactivity.buff_printf("(%s) ", ItemConfigDataLog(&item_info->reward_item, NULL));
		}
	}

	// 日志
	gamelog::g_log_roleactivity.buff_printf("word_active_flag[%d], word_active_count[%d]", word_active_info.active_flag, word_active_info.active_count);
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);
}

//void RoleActivity::OnRAFanFanRefresh(bool is_use_gold)
//{
//	// 检查
//	if (!is_use_gold && EngineAdapter::Instance().Time() < m_activity_info.ra_fanfan_next_refresh_time)
//	{
//		return;
//	}
//
//	int refresh_need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanRefreshNeedGold();
//	if (is_use_gold && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(refresh_need_gold))
//	{
//		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
//		return;
//	}
//
//	// 获取临时隐藏字组
//	RAFanFanHiddenWordInfo hidden_word_info_tmp = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanRandomHiddenWord(m_activity_info.ra_fanfan_role_refresh_times);
//	if (!hidden_word_info_tmp.IsValid())
//	{
//		return;
//	}
//
//	// 消耗
//	if (is_use_gold && !m_role->GetKnapsack()->GetMoney()->UseGold(refresh_need_gold, "RoleActivity::OnRAFanFanRefresh"))
//	{
//		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
//		return;
//	}
//
//	// 给予
//	m_activity_info.ra_fanfan_hidden_word_info = hidden_word_info_tmp;
//
//	for (int index = 0; index < RA_FANFAN_MAX_WORD_COUNT; ++ index)
//	{
//		m_activity_info.ra_fanfan_word_active_info_list[index].ClearActiveFlag();
//	}
//
//	for (int index = 0; index < RA_FANFAN_CARD_COUNT; ++ index)
//	{
//		m_activity_info.ra_fanfan_card_type_list[index] = RA_FANFAN_CARD_TYPE_HIDDEN;
//	}
//
//	if (is_use_gold)
//	{
//		++ m_activity_info.ra_fanfan_role_refresh_times;
//	}
//	else
//	{
//		m_activity_info.ra_fanfan_next_refresh_time = (unsigned int)EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetRandActivityCfg().GetFanFanAutoRefreshInterval() * SECOND_PER_HOUR;
//	}
//
//	// 发送
//	this->SendRAFanFanAllInfo();
//
//	// 日志
//	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRAFanFanRefresh user[%d, %s], is_use_gold[%d], refresh_need_gold[%d], fanfan_next_refresh_time[%d], hidden_word_seq[%d], role_refresh_times[%d]",
//		m_role->GetUID(), m_role->GetName(), is_use_gold, refresh_need_gold, m_activity_info.ra_fanfan_next_refresh_time, hidden_word_info_tmp.hidden_word_seq,	m_activity_info.ra_fanfan_role_refresh_times);
//}

bool RoleActivity::OnRAFanFanRefresh(bool is_use_gold, int target, int *is_get_target)
{
	// 检查
	if (!is_use_gold && EngineAdapter::Instance().Time() < m_activity_info.ra_fanfan_next_refresh_time)
	{
		return false;
	}

	int refresh_need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanRefreshNeedGold();
	if (is_use_gold && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(refresh_need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	RandActivityFanFan *fanfan_act = (RandActivityFanFan *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_FANFAN);
	if (nullptr == fanfan_act)
	{
		return false;
	}

	const int server_chou_times = fanfan_act->GetServerChouTimes();

	// 获取临时隐藏字组
	RAFanFanHiddenWordInfo hidden_word_info_tmp = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanRandomHiddenWord(m_activity_info.ra_fanfan_role_refresh_times, server_chou_times);
	if (!hidden_word_info_tmp.IsValid())
	{
		return false;
	}

	// 消耗
	if (is_use_gold && !m_role->GetKnapsack()->GetMoney()->UseGold(refresh_need_gold, "RoleActivity::OnRAFanFanRefresh"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	// 给予
	m_activity_info.ra_fanfan_hidden_word_info = hidden_word_info_tmp;

	if (target >= 0 && nullptr != is_get_target)
	{
		if (hidden_word_info_tmp.hidden_word_seq - RA_FANFAN_CARD_TYPE_WORD_BEGIN == target)
		{
			*is_get_target = 1;
		}
	}

	for (int index = 0; index < RA_FANFAN_MAX_WORD_COUNT; ++index)
	{
		m_activity_info.ra_fanfan_word_active_info_list[index].ClearActiveFlag();
	}

	for (int index = 0; index < RA_FANFAN_CARD_COUNT; ++index)
	{
		m_activity_info.ra_fanfan_card_type_list[index] = RA_FANFAN_CARD_TYPE_HIDDEN;
	}

	if (is_use_gold)
	{
		// 增加全服次数
		fanfan_act->IncreaseServerChouTimes();

		++m_activity_info.ra_fanfan_role_refresh_times;

		//RandActivityLogManager::Instance().AddLogData(m_role, RAND_ACTIVITY_TYPE_FANFAN, refresh_need_gold, 1);
	}
	else
	{
		m_activity_info.ra_fanfan_next_refresh_time = (unsigned int)EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetRandActivityCfg().GetFanFanAutoRefreshInterval() * SECOND_PER_HOUR;
	}

	// 发送
	this->SendRAFanFanAllInfo();

	// 日志
	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRAFanFanRefresh user[%d, %s], is_use_gold[%d], refresh_need_gold[%d], fanfan_next_refresh_time[%d], hidden_word_seq[%d], role_refresh_times[%d]",
		m_role->GetUID(), m_role->GetName(), is_use_gold, refresh_need_gold, m_activity_info.ra_fanfan_next_refresh_time, hidden_word_info_tmp.hidden_word_seq,	m_activity_info.ra_fanfan_role_refresh_times);

	return true;
}

void RoleActivity::OnRAFanFanWordExchange(int index)
{
	// 检查
	if (index < 0 || index >= RA_FANFAN_MAX_WORD_COUNT)
	{
		return;
	}

	RAFanFanWordActiveInfo &word_active_info = m_activity_info.ra_fanfan_word_active_info_list[index];
	if (word_active_info.active_count <= 0)
	{
		m_role->NoticeNum(errornum::EN_RA_FANFAN_WORD_NOT_ACTIVE);
		return;
	}

	const RandActivityFanFanWordInfo *word_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanWordInfoCfg(index);
	if (NULL == word_info_cfg)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 消耗
	-- word_active_info.active_count;

	// 给予
	m_role->GetKnapsack()->Put(word_info_cfg->exchange_item, PUT_REASON_FANFAN_EXCHANGE_WORD_REWARD);

	// 发送
	static Protocol::SCRAFanFanWordExchangeResult ffwer;
	ffwer.index = (short)index;
	ffwer.active_count = word_active_info.active_count;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ffwer, sizeof(ffwer));

	// 日志
	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRAFanFanWordExchange user[%d, %s], index[%d], active_count[%d], reward[%s]",
		m_role->GetUID(), m_role->GetName(), index, word_active_info.active_count, ItemConfigDataLog(&word_info_cfg->exchange_item, NULL));
}

void RoleActivity::OnRAFanFanLeichouExchange(int index)
{
	if (index < 0)
	{
		return;
	}

	int choujiang_times = m_activity_info.ra_fanfan_leichou_times;

	const RandActivityFanFanBaodiConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanBaodi(index);
	if (NULL != cfg && 0 == (m_activity_info.ra_fanfan_reward_flag &(1 << index)) && cfg->choujiang_times <= choujiang_times)
	{
		if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(1))
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
		else
		{
			m_role->GetKnapsack()->Put(cfg->reward_item, PUT_REASON_FANFAN_EXCHANGE_WORD_REWARD);
			m_activity_info.ra_fanfan_reward_flag |= (1 << index);
			this->SendRAFanFanAllInfo();
		}
	}

	return;
}

void RoleActivity::RACheckFanFanGiveReward()
{
	bool is_reward = false;
	bool is_first_log = true;
	static MailContentParam contentparam; contentparam.Reset();

	for (int index = 0; index < RA_FANFAN_MAX_WORD_COUNT; ++ index)
	{
		// 检查字组激活情况
		RAFanFanWordActiveInfo &word_active_info = m_activity_info.ra_fanfan_word_active_info_list[index];
		if (word_active_info.active_count <= 0)
		{
			continue;
		}

		const RandActivityFanFanWordInfo *word_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanWordInfoCfg(index);
		if (NULL == word_info_cfg)
		{
			continue;
		}

		const ItemBase *reward_item_base = ITEMPOOL->GetItem(word_info_cfg->exchange_item.item_id);
		if (NULL == reward_item_base)
		{
			continue;
		}

		is_reward = true;

		// 发邮件
		int item_index = 0;
		for (int i = 0; i < word_active_info.active_count; ++ i)
		{
			contentparam.item_list[item_index].item_id = word_info_cfg->exchange_item.item_id;
			contentparam.item_list[item_index].num = word_info_cfg->exchange_item.num;
			contentparam.item_list[item_index].is_bind = (word_info_cfg->exchange_item.is_bind) ? 1 : 0;
			contentparam.item_list[item_index].invalid_time = reward_item_base->CalInvalidTime();

			++ item_index;

			if (item_index >= MAX_ATTACHMENT_ITEM_NUM)
			{
				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_fanfan_exchange_word_reward_content, index);
				if (length > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}

				item_index = 0;
				contentparam.Reset();
			}
		}

		if (item_index > 0)
		{
			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_fanfan_exchange_word_reward_content, index);
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		{
			// 可以把奖励叠加的算法，较繁琐，不用
			/*int total_item_num = word_info_cfg->exchange_item.num * word_active_info.active_count;
			int pile_count = 0;

			pile_count = total_item_num / reward_item_base->GetPileLimit();
			if (total_item_num % reward_item_base->GetPileLimit() != 0)
			{
				++ pile_count;
			}

			static MailContentParam contentparam; contentparam.Reset();
			int item_index = 0;

			contentparam.item_list[item_index].item_id = word_info_cfg->exchange_item.item_id;
			contentparam.item_list[item_index].num = total_item_num - reward_item_base->GetPileLimit() * (pile_count - 1);
			contentparam.item_list[item_index].is_bind = (word_info_cfg->exchange_item.is_bind) ? 1 : 0;
			contentparam.item_list[item_index].invalid_time = reward_item_base->CalInvalidTime();

			++ item_index;

			for (int i = 1; i < pile_count; ++ i)
			{
				contentparam.item_list[item_index].item_id = word_info_cfg->exchange_item.item_id;
				contentparam.item_list[item_index].num = reward_item_base->GetPileLimit();
				contentparam.item_list[item_index].is_bind = (word_info_cfg->exchange_item.is_bind) ? 1 : 0;
				contentparam.item_list[item_index].invalid_time = reward_item_base->CalInvalidTime();

				++ item_index;
				if (item_index >= MAX_ATTACHMENT_ITEM_NUM)
				{
					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_fanfan_exchange_word_reward_content, index);
					if (length > 0)
					{
						MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
					}

					item_index = 0;
					contentparam.Reset();
				}
			}

			if (item_index > 0)
			{
				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_fanfan_exchange_word_reward_content, index);
				if (length > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}*/
		}

		// 日志
		if (is_first_log)
		{
			gamelog::g_log_roleactivity.buff_printf("RoleActivity::RACheckFanFanGiveReward user[%d, %s] give_reward: ", m_role->GetUID(), m_role->GetName());
			is_first_log = false;
		}

		gamelog::g_log_roleactivity.buff_printf("(index[%d], reward_count[%d], reward[%s]) ",
			index, word_active_info.active_count, ItemConfigDataLog(&word_info_cfg->exchange_item, NULL));

		// 清零激活次数，避免下次重复领
		word_active_info.active_count = 0;
	}

	if (is_reward)
	{
		gamelog::g_log_roleactivity.commit_buff(LL_INFO);
	}
}

void RoleActivity::OnRAContinueChongzhiOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI);

	switch(opera_type)
	{
	case Protocol::RA_CONTINUE_CHONGZHI_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAContinueChongzhiActivityInfo();
		}
		break;

	case Protocol::RA_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_REWARD:
		{
			this->OnRAContinueChongzhiFetchReward(param1);
		}
		break;

	case Protocol::RA_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_EXTRA_REWARD:
		{
			this->OnRAContinueChongzhiFetchExtraReward();
		}
		break;
	}
}

void RoleActivity::OnRAContinueChongzhiAddChongzhi(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI);

	m_activity_info.ra_continue_chongzhi_today_chongzhi += (unsigned int)chongzhi;

	if (m_activity_info.ra_continue_chongzhi_is_activity_over)
	{
		this->SendRAContinueChongzhiActivityInfo();
		return;
	}

	// 获取当前天
	int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI), EngineAdapter::Instance().Time()) + 1;

	const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindex(day_index);
	if (NULL == continue_chongzhi_info_cfg)
	{
		return;
	}

	if (m_activity_info.ra_continue_chongzhi_today_chongzhi >= continue_chongzhi_info_cfg->need_chongzhi && m_activity_info.ra_continue_chongzhi_days < day_index)
	{
		m_activity_info.ra_continue_chongzhi_days = day_index;

		// 激活特殊奖励
		if (m_activity_info.ra_continue_chongzhi_days == LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiFetchExtraRewardNeedDays())
		{
			m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag |= (1 << 0);
		}

		// 激活每日充值达标奖励
		m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag |= (1 << day_index);
	}

	this->SendRAContinueChongzhiActivityInfo();
}

void RoleActivity::SendRAContinueChongzhiActivityInfo()
{
	static Protocol::SCRAContinueChongzhiInfo cczi;
	cczi.today_chongzhi = m_activity_info.ra_continue_chongzhi_today_chongzhi;
	cczi.can_fetch_reward_flag = m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag;
	cczi.has_fetch_reward_falg = m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag;
	cczi.continue_chongzhi_days = m_activity_info.ra_continue_chongzhi_days;
	cczi.is_activity_over = m_activity_info.ra_continue_chongzhi_is_activity_over;
	cczi.reserve_sh = 0;

	unsigned int send_len = sizeof(cczi);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cczi, send_len);
}

void RoleActivity::OnRAContinueChongzhiFetchReward(int day_index)
{
	if (day_index <= 0 || day_index > RA_CONTINUE_CHONGZHI_MAX_DAY_INDEX)
	{
		return;
	}

	if (0 == (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag & (1 << day_index)))	// 对应的位为1代表可领取
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_NOT_ACTIVE);
		return;
	}

	if (0 != (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag & (1 << day_index)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_HAS_FETCH);
		return;
	}

	const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindex(day_index);
	if (NULL == continue_chongzhi_info_cfg)
	{
		return;
	}

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_chongzhi_info_cfg->reward_item.item_id);
	if (NULL == reward_item_base || reward_item_base->GetPileLimit() <= 0)
	{
		return;
	}

	if (reward_item_base->GetPileLimit() <= 0) return;

	int need_empty_slot_num = 1;
	need_empty_slot_num = continue_chongzhi_info_cfg->reward_item.num / reward_item_base->GetPileLimit();
	if (continue_chongzhi_info_cfg->reward_item.num % reward_item_base->GetPileLimit() > 0)
	{
		++ need_empty_slot_num;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_empty_slot_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(continue_chongzhi_info_cfg->reward_item, PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag |= (1 << day_index);

	this->SendRAContinueChongzhiActivityInfo();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAContinueChongzhiFetchReward::user[%d, %s], index[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), day_index, ItemConfigDataLog(&continue_chongzhi_info_cfg->reward_item, NULL));
}

void RoleActivity::OnRAContinueChongzhiFetchExtraReward()
{
	if (0 == (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag & (1 << 0)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_NOT_ACTIVE);
		return;
	}

	if (0 != (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag & (1 << 0)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_HAS_FETCH);
		return;
	}

	const ItemConfigData & item_data = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChonghziExtraReward();

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(item_data.item_id);
	if (NULL == reward_item_base || reward_item_base->GetPileLimit() <= 0)
	{
		return;
	}

	if (reward_item_base->GetPileLimit() <= 0) return;

	int need_empty_slot_num = 1;
	need_empty_slot_num = item_data.num / reward_item_base->GetPileLimit();
	if (item_data.num % reward_item_base->GetPileLimit() > 0)
	{
		++ need_empty_slot_num;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_empty_slot_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(item_data, PUT_REASON_CONTINUE_CHONGZHI_FETCH_EXTRA_REWARD))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag |= (1 << 0);	// 将特殊奖励领取标记置0

	this->SendRAContinueChongzhiActivityInfo();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAContinueChongzhiFetchExtraReward::user[%d, %s], index[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), 0, ItemConfigDataLog(&item_data, NULL));
}

void RoleActivity::OnRAContinueConsumeOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME);

	switch(opera_type)
	{
	case Protocol::RA_CONTINUME_CONSUME_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAContinueConsumeActivityInfo();
		}
		break;

	case Protocol::RA_CONTINUE_CONSUME_OPEAR_TYPE_FETCH_REWARD:
		{
			this->OnRAContinueConsumeFetchTodayReward();
		}
		break;

	case Protocol::RA_CONTINUE_CONSUME_OPEAR_TYPE_FETCH_EXTRA_REWARD:
		{
			this->OnRAContinueConsumeFetchExtraReward();
		}
		break;
	}
}

void RoleActivity::OnRAContinueConsumeConsumeGold(int consume_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME);

	unsigned int old_total_gold = m_activity_info.ra_continue_consume_today_consume_gold_total;
	m_activity_info.ra_continue_consume_today_consume_gold_total += consume_gold;
	m_activity_info.ra_continue_consume_cur_consume_gold += consume_gold;

	m_activity_info.ra_continue_consume_the_last_time_consume_day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME), EngineAdapter::Instance().Time()) + 1;

	const RandActivityContinueConsumeRewardConfig * continue_consume_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueConsumeRewardInfoWithdayindex(m_activity_info.ra_continue_consume_the_last_time_consume_day_index);
	if (NULL == continue_consume_reward_cfg)
	{
		return;
	}

	if ((m_activity_info.ra_continue_consume_today_consume_gold_total >= continue_consume_reward_cfg->need_consume_gold) && (old_total_gold < continue_consume_reward_cfg->need_consume_gold))
	{
		++ m_activity_info.ra_continue_consume_total_days;
		int need_days = LOGIC_CONFIG->GetRandActivityCfg().GetContinueConsumeFetchExtraRewardNeedDays();
		if (need_days <= 0)
		{
			return;
		}

		if ((m_activity_info.ra_continue_consume_days % need_days) == (need_days - 1))
		{
			++ m_activity_info.ra_continue_consume_extra_reward_num;
		}

		++ m_activity_info.ra_continue_consume_days;
	}

	this->SendRAContinueConsumeActivityInfo();
}

void RoleActivity::SendRAContinueConsumeActivityInfo()
{
	static Protocol::SCRAContinueConsumeInfo cci;
	cci.today_consume_gold_total = m_activity_info.ra_continue_consume_today_consume_gold_total;
	cci.cur_consume_gold = m_activity_info.ra_continue_consume_cur_consume_gold;
	cci.continue_consume_total_days = m_activity_info.ra_continue_consume_total_days;
	cci.continue_consume_days = m_activity_info.ra_continue_consume_days;
	cci.current_day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME), EngineAdapter::Instance().Time()) + 1;
	cci.extra_reward_num = m_activity_info.ra_continue_consume_extra_reward_num;

	unsigned int send_len = sizeof(cci);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cci, send_len);
}

void RoleActivity::OnRAContinueConsumeFetchTodayReward()
{
	int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME), EngineAdapter::Instance().Time()) + 1;
	if (m_activity_info.ra_continue_consume_the_last_time_consume_day_index != day_index)
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CONSUME_NOT_CONSUME_ANY_GOLD);
		return;
	}

	const RandActivityContinueConsumeRewardConfig * continue_consume_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueConsumeRewardInfoWithdayindex(day_index);
	if (NULL == continue_consume_reward_cfg)
	{
		return;
	}

	if (m_activity_info.ra_continue_consume_cur_consume_gold < continue_consume_reward_cfg->need_consume_gold)
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CONSUME_NOT_REACH_NEED_CONSUME_GOLD);
		return;
	}

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_consume_reward_cfg->reward_item.item_id);
	if (NULL == reward_item_base || reward_item_base->GetPileLimit() <= 0)
	{
		return;
	}

	if (continue_consume_reward_cfg->need_consume_gold <= 0 || reward_item_base->GetPileLimit() <= 0) return;

	int reward_num = m_activity_info.ra_continue_consume_cur_consume_gold / continue_consume_reward_cfg->need_consume_gold;

	int need_empty_slot_num = 1;
	need_empty_slot_num = (continue_consume_reward_cfg->reward_item.num * reward_num) / reward_item_base->GetPileLimit();
	if (continue_consume_reward_cfg->reward_item.num % reward_item_base->GetPileLimit() > 0)
	{
		++ need_empty_slot_num;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_empty_slot_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	for(int i = 0; i < reward_num; i++)
	{
		m_role->GetKnapsack()->Put(continue_consume_reward_cfg->reward_item, PUT_REASON_CONTINUE_CONSUME_FETCH_REWARD);
	}

	int consume_gold_to_log = m_activity_info.ra_continue_consume_cur_consume_gold;
	m_activity_info.ra_continue_consume_cur_consume_gold -= (reward_num * continue_consume_reward_cfg->need_consume_gold);
	if (m_activity_info.ra_continue_consume_cur_consume_gold < 0)
	{
		m_activity_info.ra_continue_consume_cur_consume_gold = 0;
	}

	this->SendRAContinueConsumeActivityInfo();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAContinueConsumeFetchTodayReward::user[%d, %s], reward_num[%d] reward_item[%s], day_index[%d], need_consume_gold[%d], real_consume_gold[%d]",
		m_role->GetUID(), m_role->GetName(), reward_num, ItemConfigDataLog(&continue_consume_reward_cfg->reward_item, NULL), day_index, continue_consume_reward_cfg->need_consume_gold, consume_gold_to_log);
}

void RoleActivity::OnRAContinueConsumeFetchExtraReward()
{
	if (m_activity_info.ra_continue_consume_extra_reward_num <= 0)
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CONSUME_REWARD_NOT_ACTIVE);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(m_activity_info.ra_continue_consume_extra_reward_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const ItemConfigData * item_data = LOGIC_CONFIG->GetRandActivityCfg().GetContinueConsumeExtraReward();
	if (NULL == item_data)
	{
		return;
	}

	for (int i = 0; i < m_activity_info.ra_continue_consume_extra_reward_num; ++ i)
	{
		m_role->GetKnapsack()->Put(*item_data, PUT_REASON_CONTINUE_CONSUME_FETCH_EXTRA_REWARD);
	}

	int reward_count_to_log = m_activity_info.ra_continue_consume_extra_reward_num;
	m_activity_info.ra_continue_consume_extra_reward_num = 0;

	this->SendRAContinueConsumeActivityInfo();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAContinueConsumeFetchExtraReward::user[%d, %s], reward_item[%s], reward_count[%d]",
		m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(item_data, NULL), reward_count_to_log);
}

void RoleActivity::RACheckContinueChongzhiGiveReward()
{
	bool is_first_log = false;

	// 邮箱发放特殊奖励
	if (0 != (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag & (1 << 0)) &&
		0 == (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag & (1 << 0)))
	{
		const ItemConfigData & item_data = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChonghziExtraReward();
		static MailContentParam contentparam; contentparam.Reset();

		const ItemBase *reward_item_base = ITEMPOOL->GetItem( item_data.item_id);
		if (NULL != reward_item_base)
		{
			contentparam.item_list[0].item_id = item_data.item_id;
			contentparam.item_list[0].num = item_data.num;
			contentparam.item_list[0].is_bind = (item_data.is_bind) ? 1 : 0;
			contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_continue_chongzhi_reward_content, RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI);
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				is_first_log = true;

				gamelog::g_log_rand_activity.buff_printf("RACheckContinueChongzhiGiveReward::user[%d, %s], index[%d], reward_item[%s] ",
					m_role->GetUID(), m_role->GetName(), 0, ItemConfigDataLog(&item_data, NULL));
			}
		}
	}

	// 通过邮箱发放达标未领取奖励
	for(int index = 1; index <= m_activity_info.ra_continue_chongzhi_days; ++ index)
	{
		if (0 == (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag & (1 << index)))	// 奖励未达标
		{
			break;
		}

		if (0 != (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag & (1 << index)))	// 奖励已领取
		{
			continue;
		}

		static MailContentParam contentparam; contentparam.Reset();
		const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindex(index);
		if (NULL == continue_chongzhi_info_cfg)
		{
			continue;
		}

		const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_chongzhi_info_cfg->reward_item.item_id);
		if (NULL != reward_item_base)
		{
			contentparam.item_list[0].item_id = continue_chongzhi_info_cfg->reward_item.item_id;
			contentparam.item_list[0].num = continue_chongzhi_info_cfg->reward_item.num;
			contentparam.item_list[0].is_bind = (continue_chongzhi_info_cfg->reward_item.is_bind) ? 1 : 0;
			contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_continue_chongzhi_reward_content, RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI);
			if (length > 0)
			{

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				if (!is_first_log)
				{
					is_first_log = true;

					gamelog::g_log_rand_activity.buff_printf("RACheckContinueChongzhiGiveReward::user[%d, %s], reward: ",
						m_role->GetUID(), m_role->GetName());
				}

				gamelog::g_log_rand_activity.buff_printf("(index[%d], reward_item[%s]) ", index, ItemConfigDataLog(&continue_chongzhi_info_cfg->reward_item, NULL));
			}
		}
	}

	if (is_first_log)
	{
		gamelog::g_log_rand_activity.commit_buff(LL_INFO);
	}

	m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag = 0;
	m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag = 0;
}

void RoleActivity::RACheckContinueChongzhiGiveRewardChu()
{
	bool is_first_log = false;

	// 邮箱发放特殊奖励
// 	if (0 != (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_chu & (1 << 0)) &&
// 		0 == (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_chu & (1 << 0)))
// 	{
// 		const ItemConfigData & item_data = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChonghziChuExtraReward();
// 		static MailContentParam contentparam; contentparam.Reset();
// 
// 		const ItemBase *reward_item_base = ITEMPOOL->GetItem(item_data.item_id);
// 		if (NULL != reward_item_base)
// 		{
// 			contentparam.item_list[0].item_id = item_data.item_id;
// 			contentparam.item_list[0].num = item_data.num;
// 			contentparam.item_list[0].is_bind = (item_data.is_bind) ? 1 : 0;
// 			contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();
// 
// 			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_continue_chongzhi_reward_content, RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU);
// 			if (length > 0)
// 			{
// 				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 
// 				is_first_log = true;
// 
// 				gamelog::g_log_rand_activity.buff_printf("RACheckContinueChongzhiGiveReward::user[%d, %s], index[%d], reward_item[%s] ",
// 					m_role->GetUID(), m_role->GetName(), 0, ItemConfigDataLog(&item_data, NULL));
// 			}
// 		}
// 	}

	// 通过邮箱发放达标未领取奖励
	for (int index = 1; index <= m_activity_info.ra_continue_chongzhi_days_chu; ++index)
	{
		if (0 == (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_chu & (1 << index)))	// 奖励未达标
		{
			break;
		}

		if (0 != (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_chu & (1 << index)))	// 奖励已领取
		{
			continue;
		}

		static MailContentParam contentparam; contentparam.Reset();
		const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindexChu(index, false, true);
		if (NULL == continue_chongzhi_info_cfg)
		{
			continue;
		}

		const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_chongzhi_info_cfg->reward_item.item_id);
		if (NULL != reward_item_base)
		{
			contentparam.item_list[0].item_id = continue_chongzhi_info_cfg->reward_item.item_id;
			contentparam.item_list[0].num = continue_chongzhi_info_cfg->reward_item.num;
			contentparam.item_list[0].is_bind = (continue_chongzhi_info_cfg->reward_item.is_bind) ? 1 : 0;
			contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_continue_chongzhi_reward_content, RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU);
			if (length > 0)
			{

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				if (!is_first_log)
				{
					is_first_log = true;

					gamelog::g_log_rand_activity.buff_printf("RACheckContinueChongzhiGiveRewardChu::user[%d, %s], reward: ",
						m_role->GetUID(), m_role->GetName());
				}

				gamelog::g_log_rand_activity.buff_printf("(index[%d], reward_item[%s]) ", index, ItemConfigDataLog(&continue_chongzhi_info_cfg->reward_item, NULL));
			}
		}
	}

	if (is_first_log)
	{
		gamelog::g_log_rand_activity.commit_buff(LL_INFO);
	}

	m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_chu = 0;
	m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_chu = 0;
}

void RoleActivity::RACheckContinueChongzhiGiveRewardGao()
{
	bool is_first_log = false;

	// 邮箱发放特殊奖励
// 	if (0 != (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_gao & (1 << 0)) &&
// 		0 == (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_gao & (1 << 0)))
// 	{
// 		const ItemConfigData & item_data = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChonghziGaoExtraReward();
// 		static MailContentParam contentparam; contentparam.Reset();
// 
// 		const ItemBase *reward_item_base = ITEMPOOL->GetItem(item_data.item_id);
// 		if (NULL != reward_item_base)
// 		{
// 			contentparam.item_list[0].item_id = item_data.item_id;
// 			contentparam.item_list[0].num = item_data.num;
// 			contentparam.item_list[0].is_bind = (item_data.is_bind) ? 1 : 0;
// 			contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();
// 
// 			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_continue_chongzhi_reward_content, RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO);
// 			if (length > 0)
// 			{
// 				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 
// 				is_first_log = true;
// 
// 				gamelog::g_log_rand_activity.buff_printf("RACheckContinueChongzhiGiveReward::user[%d, %s], index[%d], reward_item[%s] ",
// 					m_role->GetUID(), m_role->GetName(), 0, ItemConfigDataLog(&item_data, NULL));
// 			}
// 		}
// 	}

	// 通过邮箱发放达标未领取奖励
	for (int index = 1; index <= m_activity_info.ra_continue_chongzhi_days_gao; ++index)
	{
		if (0 == (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_gao & (1 << index)))	// 奖励未达标
		{
			break;
		}

		if (0 != (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_gao & (1 << index)))	// 奖励已领取
		{
			continue;
		}

		static MailContentParam contentparam; contentparam.Reset();
		const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindexGao(index, false, true);
		if (NULL == continue_chongzhi_info_cfg)
		{
			continue;
		}

		const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_chongzhi_info_cfg->reward_item.item_id);
		if (NULL != reward_item_base)
		{
			contentparam.item_list[0].item_id = continue_chongzhi_info_cfg->reward_item.item_id;
			contentparam.item_list[0].num = continue_chongzhi_info_cfg->reward_item.num;
			contentparam.item_list[0].is_bind = (continue_chongzhi_info_cfg->reward_item.is_bind) ? 1 : 0;
			contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_continue_chongzhi_reward_content, RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO);
			if (length > 0)
			{

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				if (!is_first_log)
				{
					is_first_log = true;

					gamelog::g_log_rand_activity.buff_printf("RACheckContinueChongzhiGiveRewardGao::user[%d, %s], reward: ",
						m_role->GetUID(), m_role->GetName());
				}

				gamelog::g_log_rand_activity.buff_printf("(index[%d], reward_item[%s]) ", index, ItemConfigDataLog(&continue_chongzhi_info_cfg->reward_item, NULL));
			}
		}
	}

	if (is_first_log)
	{
		gamelog::g_log_rand_activity.commit_buff(LL_INFO);
	}

	m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_gao = 0;
	m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_gao = 0;
}

void RoleActivity::RACheckContinueChongzhiGiveReward2()
{
	bool is_first_log = false;

	// 邮箱发放特殊奖励
	if (0 != (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_2 & (1 << 0)) &&
		0 == (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_2 & (1 << 0)))
	{
		const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindex2(1, false, true);
		if (NULL != continue_chongzhi_info_cfg)
		{
			static MailContentParam contentparam; contentparam.Reset();

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_chongzhi_info_cfg->extra_reward.item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[0].item_id = continue_chongzhi_info_cfg->extra_reward.item_id;
				contentparam.item_list[0].num = continue_chongzhi_info_cfg->extra_reward.num;
				contentparam.item_list[0].is_bind = (continue_chongzhi_info_cfg->extra_reward.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_continue_chongzhi_reward_content, RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2);
				if (length > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

					is_first_log = true;

					gamelog::g_log_rand_activity.buff_printf("RACheckContinueChongzhi2GiveReward::user[%d, %s], index[%d], reward_item[%s] ",
						m_role->GetUID(), m_role->GetName(), 0, ItemConfigDataLog(&continue_chongzhi_info_cfg->extra_reward, NULL));
				}
			}
		}
	}


	// 通过邮箱发放达标未领取奖励
	for (int index = 1; index <= m_activity_info.ra_continue_chongzhi_days_2; ++index)
	{
		if (0 == (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_2 & (1 << index)))	// 奖励未达标
		{
			break;
		}

		if (0 != (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_2 & (1 << index)))	// 奖励已领取
		{
			continue;
		}

		static MailContentParam contentparam; contentparam.Reset();
		const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindex2(index, false, true);
		if (NULL == continue_chongzhi_info_cfg)
		{
			continue;
		}

		const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_chongzhi_info_cfg->reward_item.item_id);
		if (NULL != reward_item_base)
		{
			contentparam.item_list[0].item_id = continue_chongzhi_info_cfg->reward_item.item_id;
			contentparam.item_list[0].num = continue_chongzhi_info_cfg->reward_item.num;
			contentparam.item_list[0].is_bind = (continue_chongzhi_info_cfg->reward_item.is_bind) ? 1 : 0;
			contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_continue_chongzhi_reward_content, RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2);
			if (length > 0)
			{

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				if (!is_first_log)
				{
					is_first_log = true;

					gamelog::g_log_rand_activity.buff_printf("RACheckContinueChongzhi2GiveReward::user[%d, %s], reward: ",
						m_role->GetUID(), m_role->GetName());
				}

				gamelog::g_log_rand_activity.buff_printf("(index[%d], reward_item[%s]) ", index, ItemConfigDataLog(&continue_chongzhi_info_cfg->reward_item, NULL));
			}
		}
	}

	if (is_first_log)
	{
		gamelog::g_log_rand_activity.commit_buff(LL_INFO);
	}

	m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_2 = 0;
	m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_2 = 0;
}

void RoleActivity::RACheckContinueConsumeGiveReward(bool is_activity_over)
{
	bool is_first_log = false;

	const RandActivityContinueConsumeRewardConfig * continue_consume_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueConsumeRewardInfoWithdayindex(m_activity_info.ra_continue_consume_the_last_time_consume_day_index);
	if (NULL == continue_consume_reward_cfg)
	{
		return;
	}

	if (m_activity_info.ra_continue_consume_cur_consume_gold < continue_consume_reward_cfg->need_consume_gold)
	{
		return;
	}

	if (continue_consume_reward_cfg->need_consume_gold <= 0) return;

	int reward_num = m_activity_info.ra_continue_consume_cur_consume_gold / continue_consume_reward_cfg->need_consume_gold;

	static MailContentParam contentparam; contentparam.Reset();
	const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_consume_reward_cfg->reward_item.item_id);
	if (NULL != reward_item_base)
	{
		contentparam.item_list[0].item_id = continue_consume_reward_cfg->reward_item.item_id;
		contentparam.item_list[0].num = continue_consume_reward_cfg->reward_item.num * reward_num;
		contentparam.item_list[0].is_bind = (continue_consume_reward_cfg->reward_item.is_bind) ? 1 : 0;
		contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_continue_consume_reward_content, RAND_ACTIVITY_TYPE_CONTINUE_CONSUME);
		if (length > 0)
		{
			m_activity_info.ra_continue_consume_cur_consume_gold = 0;

			//MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);

			is_first_log = true;

			gamelog::g_log_rand_activity.buff_printf("RACheckContinueConsumeGiveReward::user[%d, %s] reward_num[%d] reward_item[%s] ",
				m_role->GetUID(), m_role->GetName(), reward_num, ItemConfigDataLog(&continue_consume_reward_cfg->reward_item, NULL));
		}
	}

	// 邮箱发放特殊奖励
	if (m_activity_info.ra_continue_consume_extra_reward_num > 0 && is_activity_over)
	{
		const ItemConfigData * item_data = LOGIC_CONFIG->GetRandActivityCfg().GetContinueConsumeExtraReward();
		if (NULL == item_data)
		{
			return;
		}

		static MailContentParam contentparam; contentparam.Reset();

		const ItemBase *reward_item_base = ITEMPOOL->GetItem( item_data->item_id);
		if (NULL != reward_item_base)
		{
			contentparam.item_list[0].item_id = item_data->item_id;
			contentparam.item_list[0].num = item_data->num * m_activity_info.ra_continue_consume_extra_reward_num;
			contentparam.item_list[0].is_bind = (item_data->is_bind) ? 1 : 0;
			contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_continue_consume_reward_content, RAND_ACTIVITY_TYPE_CONTINUE_CONSUME);
			if (length > 0)
			{
				int reward_count_to_log = m_activity_info.ra_continue_consume_extra_reward_num;
				m_activity_info.ra_continue_consume_extra_reward_num = 0;

				//MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);

				if (!is_first_log)
				{
					is_first_log = true;
					gamelog::g_log_rand_activity.buff_printf("RACheckContinueConsumeGiveReward::user[%d, %s], ", m_role->GetUID(), m_role->GetName());
				}

				gamelog::g_log_rand_activity.buff_printf("extra_reward_item[%s], extra_reward_count[%d]", ItemConfigDataLog(item_data, NULL), reward_count_to_log);
			}
		}
	}

	if (is_first_log)
	{
		gamelog::g_log_rand_activity.commit_buff(LL_INFO);
	}
}

void RoleActivity::OnRaArmyDayOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ARMY_DAY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_ARMY_DAY);

	switch(opera_type)
	{
	case Protocol::RA_ARMY_DAY_OPERA_TYPE_INFO:
		{
			this->SendArmyDayInfo();
		}
		break;

	case Protocol::RA_ARMY_DAY_OPERA_TYPE_EXCHANGE_FLAG:
		{
			this->OnRaArmyDayExchangeFlag(param1);
		}
		break;

	case Protocol::RA_ARMY_DAY_OPERA_TYPE_EXCHANGE_ITEM:
		{
			this->OnRaArmyDayExchangeItem(param1);
		}
		break;
	}
}

void RoleActivity::SendArmyDayInfo()
{
	RandActivityArmyDay * army_day_activity = (RandActivityArmyDay *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_ARMY_DAY);
	if (NULL == army_day_activity)
	{
		return;
	}

	Protocol::SCRAArmyDayInfo cmd;

	cmd.belong_army_side = m_activity_info.ra_army_day_belong_army_side;
	cmd.exchange_flag = m_activity_info.ra_army_day_active_degree_exchange_flag;
	UNSTD_STATIC_CHECK(sizeof(cmd.army_day_own_flags_list) == sizeof(m_activity_info.ra_army_day_flags_list));
	memcpy(cmd.army_day_own_flags_list, m_activity_info.ra_army_day_flags_list, sizeof(cmd.army_day_own_flags_list));
	memcpy(cmd.army_day_own_score_list, m_activity_info.ra_army_day_score_list, sizeof(cmd.army_day_own_score_list));

	army_day_activity->GetAllArmyScoreInfoList(cmd.army_day_all_score_list);

	unsigned int send_len = sizeof(cmd);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, send_len);
}

void RoleActivity::OnRaArmyDayExchangeFlag(int seq)
{
	if (seq < 0 || seq >= RA_ARMY_DAY_ACTIVE_DEGREE_EXCHANGE_NUM)
	{
		return;
	}

	if (0 != (m_activity_info.ra_army_day_active_degree_exchange_flag & (1<<seq)))
	{
		m_role->NoticeNum(errornum::EN_ARMY_DAY_EXCHANGE_LIMIT);
		return;
	}

	const RandActivityArmyDayExchangeFlagConfig *flag_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetArmyDayExhangeFlagCfg(seq);
	if (NULL == flag_cfg)
	{
		return;
	}

	if (m_role->GetActiveDegree()->GetTotalDegree() < flag_cfg->need_active_degree)
	{
		m_role->NoticeNum(errornum::EN_ARMY_DAY_ACTIVITY_DEGREE_NOT_ENOUGH);
		return;
	}

	RandActivityArmyDay *army_day_activity = (RandActivityArmyDay *) RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_ARMY_DAY);
	if (NULL == army_day_activity)
	{
		return;
	}

	// 消耗、给予
	m_activity_info.ra_army_day_active_degree_exchange_flag |= (1<<seq);

	for (int i = 0; i < flag_cfg->reward_flag_num && RA_ARMY_DAY_ARMY_SIDE_NUM > 0; ++ i)
	{

		int rand_army_side = RandomNum(RA_ARMY_DAY_ARMY_SIDE_NUM);

		++ m_activity_info.ra_army_day_flags_list[rand_army_side];
	}

	this->SendArmyDayInfo();
}

void RoleActivity::OnRaArmyDayExchangeItem(int army_side)
{
	if (army_side < 0 || army_side >= RA_ARMY_DAY_ARMY_SIDE_NUM)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const RandActivityArmyDayExchangeItemConfig *exchange_item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetArmyDayExhangeItemCfg(army_side);
	if (NULL == exchange_item_cfg)
	{
		return;
	}

	RandActivityArmyDay *army_day_activity = (RandActivityArmyDay *) RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_ARMY_DAY);
	if (NULL == army_day_activity)
	{
		return;
	}

	// 消耗、给予
	int temp = m_activity_info.ra_army_day_flags_list[army_side] - 1;
	if (temp < 0)
	{
		m_role->NoticeNum(errornum::EN_ARMY_DAY_FLAG_NOT_ENOUGH);
		return;
	}

	-- m_activity_info.ra_army_day_flags_list[army_side];
	++ m_activity_info.ra_army_day_score_list[army_side];
	army_day_activity->OnAddArmyScore(army_side, 1);

	if (!m_role->GetKnapsack()->Put(exchange_item_cfg->reward_item, PUT_REASON_ARMY_DAY_EXCHANGE_REWARD))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	this->OnRaArmyDayCalBelongSide();
	this->SendArmyDayInfo();
}

void RoleActivity::OnRaArmyDayCalBelongSide()
{
	int belong_side = INVALID_ARMY_SIDE;
	int max_score_count = 0;

	for (int i  = 0; i < RA_ARMY_DAY_ARMY_SIDE_NUM; ++ i)
	{
		if (m_activity_info.ra_army_day_score_list[i] > max_score_count)
		{
			max_score_count = m_activity_info.ra_army_day_score_list[i];
			belong_side = i;
		}
	}

	m_activity_info.ra_army_day_belong_army_side = belong_side;
}

void RoleActivity::OnRaCheckArmyDayWinnerGiveReward()
{
	if (INVALID_ARMY_SIDE == m_activity_info.ra_army_day_belong_army_side)
	{
		return;
	}

	RandActivityArmyDay *army_day_activity = (RandActivityArmyDay *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_ARMY_DAY);
	if (NULL == army_day_activity)
	{
		return;
	}

	if (INVALID_ARMY_SIDE != army_day_activity->GetWinAmrySide() && m_activity_info.ra_army_day_belong_army_side == army_day_activity->GetWinAmrySide())
	{
		const ItemConfigData reward_item = LOGIC_CONFIG->Instance()->GetRandActivityCfg().GetArmyDayWinReward();

		static MailContentParam contentparam; contentparam.Reset();
		contentparam.item_list[0].item_id = reward_item.item_id;
		contentparam.item_list[0].num = reward_item.num;
		contentparam.item_list[0].is_bind= reward_item.is_bind;

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_army_day_win_army_reward_content);
		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_rand_activity.printf(LL_INFO, "OnRaArmyDayGiveWinSideReward::user[%d, %s], reward_item[%s]",
				m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&reward_item, NULL));
		}

		m_activity_info.ra_army_day_belong_army_side = INVALID_ARMY_SIDE;		// 领取后重置以免重复领取
	}
}

void RoleActivity::OnRACirculationChongzhiOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI);

	switch(opera_type)
	{
	case Protocol::RA_CIRCULATION_CHONGZHI_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRACirculationChongzhiActivityInfo();
		}
		break;

	case Protocol::RA_CIRCULATION_CHONGZHI_OPEAR_TYPE_FETCH_REWARD:
		{
			this->OnRACirculationChongzhiFetchReward(param1);
		}
		break;
	}
}

void RoleActivity::OnRACirculationChongzhiChongzhiGold(int chongzhi_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI);

	m_activity_info.ra_circulation_chongzhi_total_chongzhi += chongzhi_gold;
	m_activity_info.ra_circulation_chongzhi_cur_chongzhi_gold += chongzhi_gold;

	this->SendRACirculationChongzhiActivityInfo();
}

void RoleActivity::SendRACirculationChongzhiActivityInfo()
{
	static Protocol::SCRACirculationChongzhiInfo cci;
	cci.total_chongzhi = m_activity_info.ra_circulation_chongzhi_total_chongzhi;
	cci.cur_chongzhi = m_activity_info.ra_circulation_chongzhi_cur_chongzhi_gold;

	unsigned int send_len = sizeof(cci);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cci, send_len);
}

void RoleActivity::OnRACirculationChongzhiFetchReward(int reward_seq)
{
	const RandActivityCirculationChongzhiStageRewardConfig * reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetCirculationChongzhiRewardCfg(reward_seq);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (m_activity_info.ra_circulation_chongzhi_cur_chongzhi_gold < reward_cfg->need_chongzhi_gold)
	{
		m_role->NoticeNum(errornum::EN_CIRCULATION_CHONGZHI_NOT_REACH_NEED_CHONGZHI_GOLD);
		return;
	}

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg->reward_item.item_id);
	if (NULL == reward_item_base)
	{
		return;
	}

	if (reward_cfg->need_chongzhi_gold <= 0) return;
	int reward_num = m_activity_info.ra_circulation_chongzhi_cur_chongzhi_gold / reward_cfg->need_chongzhi_gold;
	int can_reward_num = reward_item_base->GetPileLimit() / reward_cfg->reward_item.num;

	reward_num = reward_num > can_reward_num ? can_reward_num : reward_num;

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	ItemConfigData item_to_give(reward_cfg->reward_item.item_id, reward_cfg->reward_item.is_bind, reward_num * reward_cfg->reward_item.num);

	m_role->GetKnapsack()->Put(item_to_give, PUT_REASON_CIRCULATION_CHONGZHI_FETCH_REWARD);

	int chongzhi_gold_to_log = m_activity_info.ra_circulation_chongzhi_cur_chongzhi_gold;
	m_activity_info.ra_circulation_chongzhi_cur_chongzhi_gold -= (reward_num * reward_cfg->need_chongzhi_gold);

	if (m_activity_info.ra_circulation_chongzhi_cur_chongzhi_gold < 0)
	{
		m_activity_info.ra_circulation_chongzhi_cur_chongzhi_gold = 0;
	}

	this->SendRACirculationChongzhiActivityInfo();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRACirculationChongzhiFetchReward::user[%d, %s], reward_num[%d] reward_item[%s], reward_seq[%d], need_chongzhi_gold[%d], real_chongzhi_gold[%d]",
		m_role->GetUID(), m_role->GetName(), reward_num, ItemConfigDataLog(&reward_cfg->reward_item, NULL), reward_seq, reward_cfg->need_chongzhi_gold, chongzhi_gold_to_log);
}

void RoleActivity::RACheckCirculationChongzhiGiveReward()
{
	const RandActivityCirculationChongzhiStageRewardConfig * reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetCirculationChongzhiRewardCfg(0);		// 默认只给首个物品奖励
	if (NULL == reward_cfg)
	{
		return;
	}

	if (m_activity_info.ra_circulation_chongzhi_cur_chongzhi_gold < reward_cfg->need_chongzhi_gold)
	{
		return;
	}

	if (reward_cfg->need_chongzhi_gold <= 0) return;
	int reward_num = m_activity_info.ra_circulation_chongzhi_cur_chongzhi_gold / reward_cfg->need_chongzhi_gold;

	static MailContentParam contentparam; contentparam.Reset();
	const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg->reward_item.item_id);
	if (NULL != reward_item_base)
	{
		contentparam.item_list[0].item_id = reward_cfg->reward_item.item_id;
		contentparam.item_list[0].num = reward_cfg->reward_item.num * reward_num;
		contentparam.item_list[0].is_bind = (reward_cfg->reward_item.is_bind) ? 1 : 0;
		contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_circulation_chongzhi_reward_content, RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI);
		if (length > 0)
		{
			m_activity_info.ra_circulation_chongzhi_cur_chongzhi_gold = 0;

			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckCirculationChongzhiGiveReward::user[%d, %s] reward_num[%d] reward_item[%s] ",
				m_role->GetUID(), m_role->GetName(), reward_num, ItemConfigDataLog(&reward_cfg->reward_item, NULL));
		}
	}
}

void RoleActivity::OnRAExtremeLuckyOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EXTREME_LUCKY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_EXTREME_LUCKY);

	switch(opera_type)
	{
	case Protocol::RA_EXTREME_LUCKY_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAExtremeLuckyAllInfo();
		}
		break;

	case Protocol::RA_EXTREME_LUCKY_OPERA_TYPE_GLOD_FLUSH:
		{
			this->OnRAExtremeLuckyFlush(false);
		}
		break;

	case Protocol::RA_EXTREME_LUCKY_OPERA_TYPE_DRAW:
		{
			this->OnRAExtremeLuckyDraw();
		}
		break;

	case Protocol::RA_EXTREME_LUCKY_OPREA_TYPE_AUTO_FLUSH:
		{
			this->ExtremeLuckyCheckAndFlush();
		}
		break;

	case Protocol::RA_EXTREME_LUCKY_OPREA_TYPE_FETCH_REWARD:
		{
			this->RAExtremeLuckyCheckReturnReward(param1);
		}
		break;
	}
}

void RoleActivity::SendRAExtremeLuckyAllInfo()
{
	Protocol::SCRAExtremeLuckyAllInfo cmd;
	cmd.next_flush_timestamp = m_activity_info.ra_extreme_lucky_next_flush_timestamp;
	cmd.free_draw_times = m_activity_info.ra_extreme_lucky_day_free_draw_times;
	cmd.cur_total_draw_times = m_activity_info.ra_extreme_lucky_cur_total_draw_times;
	cmd.gold_draw_times = m_activity_info.ra_extreme_lucky_cur_gold_draw_times;
	cmd.total_draw_times = m_activity_info.ra_extreme_lucky_total_draw_times;
	cmd.return_reward_flag = m_activity_info.ra_extreme_lucky_return_reward_flag;
	memcpy(cmd.cur_item_info_list, m_activity_info.ra_extreme_lucky_item_list, sizeof(cmd.cur_item_info_list));

	unsigned int send_len = sizeof(cmd);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char *)&cmd, send_len);
}

void RoleActivity::SendRAExtremeLuckySingleInfo(int index)
{
	Protocol::SCRAExtremeLuckySingleInfo cmd;
	cmd.cur_total_draw_times = m_activity_info.ra_extreme_lucky_cur_total_draw_times;
	cmd.gold_draw_times = m_activity_info.ra_extreme_lucky_cur_gold_draw_times;
	cmd.free_draw_times = m_activity_info.ra_extreme_lucky_day_free_draw_times;
	cmd.total_draw_times = m_activity_info.ra_extreme_lucky_total_draw_times;
	cmd.return_reward_flag = m_activity_info.ra_extreme_lucky_return_reward_flag;
	memcpy(&cmd.single_item_info, &m_activity_info.ra_extreme_lucky_item_list[index], sizeof(cmd.single_item_info));

	unsigned int length = sizeof(cmd);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char *)&cmd, length);
}

void RoleActivity::OnRAExtremeLuckyFlush(bool is_free)
{
	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	int flush_need_gold = 0;
	unsigned int time_now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (is_free)
	{
		if (m_activity_info.ra_extreme_lucky_next_flush_timestamp > time_now)
		{
			return;
		}
	}
	else
	{
		flush_need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeLuckyFlushNeedGold();

		if (flush_need_gold <= 0 || !knapsack->GetMoney()->GoldMoreThan(flush_need_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	RandActivityExtremeLucky *exterme_lucky_act = (RandActivityExtremeLucky *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_EXTREME_LUCKY);
	if (NULL == exterme_lucky_act)
	{
		return;
	}

	static RAExtremeLuckyRewardInfo temp_list[RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT];
	memset(temp_list, 0, sizeof(temp_list));

	const int server_flush_times = exterme_lucky_act->GetServerFlushTimes();

	if (!LOGIC_CONFIG->GetRandActivityCfg().GetExtremLuckyFlushRewardList(m_activity_info.ra_extreme_lucky_flush_times, temp_list,
		RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT, server_flush_times))
	{
		return;
	}

	// 消耗
	if (is_free)
	{
		m_activity_info.ra_extreme_lucky_next_flush_timestamp = time_now + RA_EXTREME_LUCKY_NEXT_FLUSH_INTERVAL * SECOND_PER_HOUR;
	}
	else
	{
		if (!knapsack->GetMoney()->UseGold(flush_need_gold, "OnRAExtremeLuckyFlush"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		//增加全服次数
		exterme_lucky_act->AddServerFlushTimes(1);

		++m_activity_info.ra_extreme_lucky_flush_times;
	}

	m_activity_info.ra_extreme_lucky_cur_total_draw_times = 0;		// 重置当前抽奖次数
	m_activity_info.ra_extreme_lucky_cur_gold_draw_times = 0;
	memcpy(m_activity_info.ra_extreme_lucky_item_list, temp_list, sizeof(m_activity_info.ra_extreme_lucky_item_list));

	this->SendRAExtremeLuckyAllInfo();
}

void RoleActivity::OnRAExtremeLuckyForceFlush()
{
	RandActivityExtremeLucky *exterme_lucky_act = (RandActivityExtremeLucky *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_EXTREME_LUCKY);
	if (NULL == exterme_lucky_act)
	{
		return;
	}

	static RAExtremeLuckyRewardInfo temp_list[RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT];
	memset(temp_list, 0, sizeof(temp_list));

	const int server_flush_times = exterme_lucky_act->GetServerFlushTimes();

	if (!LOGIC_CONFIG->GetRandActivityCfg().GetExtremLuckyFlushRewardList(m_activity_info.ra_extreme_lucky_flush_times, temp_list,
		RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT, server_flush_times))
	{
		return;
	}

	exterme_lucky_act->AddServerFlushTimes(1);

	++ m_activity_info.ra_extreme_lucky_flush_times;
	m_activity_info.ra_extreme_lucky_cur_total_draw_times = 0;		// 重置当前抽奖次数
	m_activity_info.ra_extreme_lucky_cur_gold_draw_times = 0;

	memcpy(m_activity_info.ra_extreme_lucky_item_list, temp_list, sizeof(m_activity_info.ra_extreme_lucky_item_list));

	this->SendRAExtremeLuckyAllInfo();
}

void RoleActivity::ExtremeLuckyCheckAndFlush()
{
	int valid_count = 0;

	for (int i = 0; i < RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT; ++ i)
	{
		if (0 == m_activity_info.ra_extreme_lucky_item_list[i].has_fetch)
		{
			++ valid_count;
		}
	}

	if (valid_count <= 1)
	{
		this->OnRAExtremeLuckyForceFlush();
	}
}

void RoleActivity::OnRAExtremeLuckyDraw()
{
	if (m_activity_info.ra_extreme_lucky_cur_total_draw_times >= RA_EXTREME_LUCKY_DRAW_CONSUME_GOLD_CFG_COUNT)
	{
		return;
	}

	int draw_need_glod = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeLuckyDrawConsumeByDrawTimes(m_activity_info.ra_extreme_lucky_cur_gold_draw_times + 1);
	bool is_free = false;

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, 1))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (m_activity_info.ra_extreme_lucky_day_free_draw_times < LOGIC_CONFIG->GetRandActivityCfg().GetExtremeLuckyFreeDrawTimes())
	{
		draw_need_glod = 0;
		is_free = true;
	}

	if (draw_need_glod > 0 && !knapsack->GetMoney()->GoldMoreThan(draw_need_glod))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 模拟抽奖
	int total_weight = 0;

	for (int i = 0; i < RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT; ++ i)
	{
		if (0 == m_activity_info.ra_extreme_lucky_item_list[i].has_fetch && 0 == m_activity_info.ra_extreme_lucky_item_list[i].invalid)
		{
			total_weight += m_activity_info.ra_extreme_lucky_item_list[i].weight;	// 可抽取的权重
		}
	}

	if (total_weight <= 0)
	{
		return;
	}

	int result_index = -1;
	int rand_weight = RandomNum(total_weight);

	for (int j = 0; j < RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT; ++ j)
	{
		if (0 == m_activity_info.ra_extreme_lucky_item_list[j].invalid && 0 == m_activity_info.ra_extreme_lucky_item_list[j].has_fetch)
		{
			if (rand_weight < m_activity_info.ra_extreme_lucky_item_list[j].weight)
			{
				result_index = j;
				break;
			}

			rand_weight -= m_activity_info.ra_extreme_lucky_item_list[j].weight;
		}
	}

	if (result_index < 0 || result_index >= RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT)
	{
		return;
	}

	const RandActivityExtremeLuckyDrawRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeLuckyRewardCfg(m_activity_info.ra_extreme_lucky_item_list[result_index].seq);
	if (NULL == reward_cfg)
	{
		return;
	}

	// 消耗
	{
		if (is_free)
		{
			++ m_activity_info.ra_extreme_lucky_day_free_draw_times;
		}
		else
		{
			if (draw_need_glod <= 0 || !knapsack->GetMoney()->UseGold(draw_need_glod, "OnRAExtremeLuckyDraw"))
			{
//				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			++m_activity_info.ra_extreme_lucky_cur_gold_draw_times;
		}
        ++m_activity_info.ra_extreme_lucky_total_draw_times;

		++m_activity_info.ra_extreme_lucky_cur_total_draw_times;
		m_activity_info.ra_extreme_lucky_item_list[result_index].has_fetch = 1;
	}

	// 给予
	static ChestItemInfo item_list[MODE_1_COUNT];

	item_list[0].item_id =  reward_cfg->reward_item.item_id;
	item_list[0].num = reward_cfg->reward_item.num;
	item_list[0].is_bind = reward_cfg->reward_item.is_bind;

//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, MODE_1_COUNT, item_list, "RoleActivity::OnRAExtremeLuckyDraw");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_EXTREME_LUCKY, MODE_1_COUNT, item_list, "RoleActivity::OnRAExtremeLuckyDraw");

	LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_EXTREME_LUCKY, m_role->GetUID(), m_role->GetName(), reward_cfg->reward_item.item_id, reward_cfg->reward_item.num);
	if (reward_cfg->is_rare)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_extreme_lucky_reward_content,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), (int)reward_cfg->reward_item.item_id);

		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	this->SendRAExtremeLuckySingleInfo(result_index);
}

void RoleActivity::RAExtremeLuckyCheckReturnReward(int seq)
{
	const RandActivityExtremeLuckyReturnRewardStageCfg *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeLuckyReturnRewardCfg(seq);
	if (NULL != cfg)
	{
		if (m_activity_info.ra_extreme_lucky_total_draw_times < cfg->draw_times)
		{
			return;
		}

		if (0 != (m_activity_info.ra_extreme_lucky_return_reward_flag & (1 << seq)))
		{
			return;
		}

		m_activity_info.ra_extreme_lucky_return_reward_flag |= (1 << seq);
		m_role->GetKnapsack()->PutOrMail(cfg->reward_item, PUT_REASON_EXTREME_LUCKY_RETURN_REWARD);

		this->SendRAExtremeLuckySingleInfo(seq);
	}
}

void RoleActivity::OnRaMiJingXunBao2OperReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MIJINGXUNBAO2))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MIJINGXUNBAO2);

	switch(opera_type)
	{
	case Protocol::RA_MIJINGXUNBAO_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRAMiJingXunBao2Info();
		}
		break;

	case Protocol::RA_MIJINGXUNBAO_OPERA_TYPE_TAO:
		{
			this->OnRAMiJingXunBao2Tao(param);
		}
		break;

	}
}

void RoleActivity::OnRAMiJingXunBao2Tao(int type)
{
	if (type < RA_MIJINGXUNBAO_CHOU_TYPE_1 || type >= RA_MIJINGXUNBAO_CHOU_TYPE_MAX)
	{
		return;
	}

	int consume_gold = 0;
	bool is_free_tao = false;
	int tao_times = 0;

	unsigned int nowtime = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	RandActivityConfig &randactivity_cfg = LOGIC_CONFIG->GetRandActivityCfg();

	unsigned int cooldown_interval = randactivity_cfg.GetMiJingXunBao2Interval() * SECOND_PER_MIN;							// 淘宝冷却时间

	consume_gold = randactivity_cfg.GetMiJingXunBao2TaoGold(type);
	if (consume_gold <= 0)
	{
		return;
	}

	bool has_item = false;
	ItemID item_id = 0;

	if (RA_MIJINGXUNBAO_CHOU_TYPE_1 == type)                    //获得淘宝花费的金币
	{
		tao_times = 1;

		if (nowtime >= m_activity_info.ra_mijingxunbao2_next_free_tao_timestamp )
		{
			is_free_tao = true;
			consume_gold = 0;
		}
	}
	else if (RA_MIJINGXUNBAO_CHOU_TYPE_10 == type)
	{
		tao_times = 10;
	}
	else if (RA_MIJINGXUNBAO_CHOU_TYPE_50 == type)
	{
		tao_times = 50;

		item_id = LOGIC_CONFIG->GetRandActivityCfg().GetMiJingXunBao2TaoItem();
		if (m_role->GetKnapsack()->HasItem(item_id))
		{
			has_item = true;
			consume_gold = 0;
		}
	}
	else
	{
		return;
	}

	if(consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int reward_count = tao_times;

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, reward_count))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_SPACE);
		return;
	}

	if (reward_count > Protocol::SC_MiJingXunBaoTao_MAX_TIMES)
	{
		return;
	}

	static RandActivityMiJingXunBaoConfig reward_item_list[Protocol::SC_MiJingXunBaoTao_MAX_TIMES];

	bool ret = randactivity_cfg.GetMiJingXunBao2RandomResult(m_activity_info.ra_mijingxunbao2_add_weight_list,
		randactivity_cfg.GetMiJingXunBao2AutoWeightAddCount(), is_free_tao, reward_item_list, reward_count);
	if (!ret)
	{
		return;
	}

	// 物品
	if (RA_MIJINGXUNBAO_CHOU_TYPE_50 == type)
	{
		if (has_item)
		{
			if (!m_role->GetKnapsack()->ConsumeItem(item_id, 1, "RoleActivity::OnRAMiJingXunBaoTao2"))
			{
				return;
			}
		}
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "RoleActivity::OnRAMiJingXunBaoTao2"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	//刷新下次免费淘宝时间
	if (is_free_tao)
	{
		m_activity_info.ra_mijingxunbao2_next_free_tao_timestamp = nowtime + cooldown_interval;
		this->SendRAMiJingXunBao2Info();

		// 免费单抽绑定
		reward_item_list[0].reward_item.is_bind = true;
	}

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAMiJingXunBao2Tao user[%d, %s] is_free_tao[%d] consume_gold[%d] type[%d]",
		m_role->GetUID(), m_role->GetName(), (int)is_free_tao, consume_gold, type);

	static Protocol::SCRAMiJingXunBao2TaoResultInfo scramjxbtri;
	scramjxbtri.count = reward_count;

	ChestItemInfo item_list[Protocol::SC_MiJingXunBaoTao_MAX_TIMES];
	int list_num = 0;

	//给予
	for (int i = 0; i < reward_count && i < Protocol::SC_MiJingXunBaoTao_MAX_TIMES; ++ i)
	{
		item_list[i].Reset();

		item_list[i].item_id = reward_item_list[i].reward_item.item_id;
		item_list[i].num = reward_item_list[i].reward_item.num;
		item_list[i].is_bind = reward_item_list[i].reward_item.is_bind;

		++ list_num;
	}

//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, list_num, item_list, "OnRAMiJingXunBaoTao2");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_MIJINGXUNBAO2, list_num, item_list, "RoleActivity::OnRAMiJingXunBaoTao2");

	for (int index = 0; index < reward_count; ++index)
	{
		gamelog::g_log_roleactivity.buff_printf("(%s), ", ItemConfigDataLog(&reward_item_list[index].reward_item, NULL));

		scramjxbtri.mijingxunbao_tao_seq[index] = reward_item_list[index].seq;
		if (reward_item_list[index].is_rare)
		{
			// 刷到珍稀物品传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_mijingxunbao2_zhenxi_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), (int)reward_item_list[index].reward_item.item_id);

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}
	}

	//发送淘宝获得的物品信息
	{
		int send_len = sizeof(scramjxbtri) - sizeof(scramjxbtri.mijingxunbao_tao_seq[0]) * (Protocol::SC_MiJingXunBaoTao_MAX_TIMES - scramjxbtri.count);
		if (send_len > 0)
		{
			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scramjxbtri, send_len);
		}
	}

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);
}

void RoleActivity::SendRAMiJingXunBao2Info()
{
	Protocol::SCRAMiJingXunBao2Info scramjxbi;
	scramjxbi.ra_mijingxunbao_next_free_tao_timestamp = m_activity_info.ra_mijingxunbao2_next_free_tao_timestamp;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scramjxbi, sizeof(scramjxbi));
}

void RoleActivity::OnRAChongzhiMoneyTree2OperReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MONEY_TREE2))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MONEY_TREE2);

	switch(opera_type)
	{
	case Protocol::RA_MONEY_TREE_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRaMoneyTree2Info();
		}
		break;

	case Protocol::RA_MONEY_TREE_OPERA_TYPE_CHOU:
		{
			this->OnRaMoneyTree2Chou(param);
		}
		break;

	case Protocol::RA_MONEY_TREE_OPERA_TYPE_FETCH_REWARD:
		{
			this->OnRaMoneyTree2FetchReward(param);
		}
		break;
	}
}

void RoleActivity::SendRaMoneyTree2Info()
{
	RandActivityMoneyTree2 *money_tree2_act = (RandActivityMoneyTree2 *)RandActivityManager::Instance().GetRandActivityMoneyTree2();
	if (NULL == money_tree2_act)
	{
		return;
	}

	Protocol::SCRAMoneyTree2Info ramti;
	ramti.server_reward_has_fetch_reward_flag = m_activity_info.ra_money_tree2_server_reward_has_fetch_reward_flag;
	ramti.server_total_money_tree_times = money_tree2_act->GetServerTotalRockMoneyTreeTimes();
	ramti.server_total_pool_gold = money_tree2_act->GetServerTotalPoolGold();

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&ramti, sizeof(ramti));
}

void RoleActivity::OnRaMoneyTree2Chou(int times)
{
	if (times <= 0 || times > Protocol::SC_RA_MONEYTREE_CHOU_MAX_COUNT_LIMIT)
	{
		return;
	}

	int need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTree2NeedGold() * times;
	if (need_gold <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(times))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 检查消耗
	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRaMoneyTree2Chou user[%d, %s], times[%d], need_gold[%d], reward[",
		m_role->GetUID(), m_role->GetName(), times, need_gold);

	static RandActivityMoneyTreeRewardConfig reward_list[Protocol::SC_RA_MONEYTREE_CHOU_MAX_COUNT_LIMIT];

	bool has_got_pool_gold = false;

	for (int i = 0; i < times; ++ i)
	{
		const RandActivityMoneyTreeRewardConfig *money_tree_cfg = NULL;
		if (has_got_pool_gold)
		{
			money_tree_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandomMoneyTree2ItemRewardCfg();
		}
		else
		{
			money_tree_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandomMoneyTree2RewardCfg();
		}

		if (NULL == money_tree_cfg)
		{
			gamelog::g_log_roleactivity.clear_buff();
			return;
		}

		if (money_tree_cfg->prize_pool_percent > 0)
		{
			has_got_pool_gold = true;
		}

		reward_list[i] = *money_tree_cfg;
		gamelog::g_log_roleactivity.buff_printf("(%s), ", ItemConfigDataLog(&money_tree_cfg->reward_item, NULL));
	}

	RandActivityMoneyTree2 *money_tree2_act = (RandActivityMoneyTree2 *)RandActivityManager::Instance().GetRandActivityMoneyTree2();
	if (NULL == money_tree2_act)
	{
		return;
	}

	static Protocol::SCRAMoneyTree2ChouResultInfo ramtcri;
	ramtcri.reward_req_list_count = times;

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "RoleActivity::OnRaMoneyTree2Chou"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 给予
	int put_into_pool_gold = times * LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTree2IntoGold();

	for (int i = 0; i < times; ++ i)
	{
		int reward_gold = 0;
		if (reward_list[i].prize_pool_percent > 0)
		{
			reward_gold = money_tree2_act->GetServerTotalPoolGold() * reward_list[i].prize_pool_percent / 100;

			money_tree2_act->DecTotalPoolGold(reward_gold);
			m_role->GetKnapsack()->GetMoney()->AddGold(reward_gold, "RoleActivity::OnRaMoneyTree2Chou");
		}
		else
		{
			m_role->GetKnapsack()->Put(reward_list[i].reward_item, PUT_REASON_RA_MONEY_TREE2_REWARD);
		}

		ramtcri.reward_req_list[i] = static_cast<char>(reward_list[i].seq);

		if (reward_list[i].is_broadcast) // 配置了需要发传闻
		{
			// 传闻
			int length = 0;

			if (reward_list[i].prize_pool_percent > 0)	// 元宝
			{
				length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_money_tree2_reward_content_gold,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_MONEY_TREE2, (money_tree2_act->GetServerTotalPoolGold() * reward_list[i].prize_pool_percent / 100));
			}
			else	// 道具
			{
				length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_money_tree2_reward_content_item,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_MONEY_TREE2, reward_list[i].reward_item.item_id);
			}

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}

		gamelog::g_log_roleactivity.buff_printf("(%s) (gold: %d), ", ItemConfigDataLog(&reward_list[i].reward_item, NULL), reward_gold);
	}

	int len = sizeof(ramtcri) - (Protocol::SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT - ramtcri.reward_req_list_count) * sizeof(ramtcri.reward_req_list[0]);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&ramtcri, len);

	money_tree2_act->AddTotalTotalMoneyTreeTimes(times);
	if (need_gold > 0)
	{
		money_tree2_act->AddTotalPoolGold(put_into_pool_gold);
	}

	this->SendRaMoneyTree2Info();

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);

}

void RoleActivity::OnRaMoneyTree2FetchReward(int seq)
{
	RandActivityMoneyTree2 *money_tree2_act = (RandActivityMoneyTree2 *)RandActivityManager::Instance().GetRandActivityMoneyTree2();
	if (NULL == money_tree2_act)
	{
		return;
	}

	const RandActivityMoneyTreeServerRewardConfig *cur_server_money_tree_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTreeServerCfgWithServerMoneyTree2Times(
		money_tree2_act->GetServerTotalRockMoneyTreeTimes());
	if (NULL == cur_server_money_tree_cfg)
	{
		return;
	}

	if (seq > cur_server_money_tree_cfg->seq)
	{
		m_role->NoticeNum(errornum::EN_RA_SERVER_MONEYTREE_TIMES_LIMIT);
		return;
	}

	if ((m_activity_info.ra_money_tree2_server_reward_has_fetch_reward_flag & (1 << seq)) > 0)
	{
		m_role->NoticeNum(errornum::EN_RA_SERVER_MONEYTREE_HAS_FATCHED);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const RandActivityMoneyTreeServerRewardConfig *cur_tree_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTree2ServerCfgWithReq(seq);
	if (NULL == cur_tree_cfg)
	{
		return;
	}

	if (m_role->GetVip()->GetVipLevel() < cur_tree_cfg->vip_limit)
	{
		m_role->NoticeNum(errornum::EN_RA_SERVER_MONEYTREE_VIP_LIMIT);
		return;
	}

	if (m_role->GetKnapsack()->Put(cur_tree_cfg->reward_item, PUT_REASON_RA_MONEY_TREE_REWARD))
	{
		m_activity_info.ra_money_tree2_server_reward_has_fetch_reward_flag |= (1 << seq);
		this->SendRaMoneyTree2Info();

		gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRaMoneyTree2FetchReward user[%d, %s], req[%d], reward[%s]",
			m_role->GetUID(), m_role->GetName(), seq, ItemConfigDataLog(&cur_tree_cfg->reward_item, NULL));
	}
}

void RoleActivity::OnRaNationalDayOpeReq(int opera, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NATIONAL_DAY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_NATIONAL_DAY);

	switch (opera)
	{
	case Protocol::RA_NATIONAL_DAY_OPERA_TYPE_EXCHANGE:
		{
			this->OnRaNationalDayExchange(param);
		}
		break;
	}
}

void RoleActivity::OnRaNationalDayExchange(int seq)
{
	const RandActivityNationalDayConfig *reward = LOGIC_CONFIG->GetRandActivityCfg().GetNationalDayRewardCfg(seq);
	if (NULL == reward)
	{
		return;
	}

	if (reward->consume_count < 0)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	int stuff_count = 0;

	for (; stuff_count < reward->consume_count;)
	{
		stuff_list[stuff_count].item_id = reward->consume_item_list[stuff_count].item_id;
		stuff_list[stuff_count].num = reward->consume_item_list[stuff_count].num;
		stuff_list[stuff_count].buyable = false;
		++ stuff_count;
	}

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "RoleActivity::OnRaNationalDayExchange"))
	{
		return;
	}

	knapsack->Put(reward->reward_item, PUT_REASON_RA_NATIONAL_DAY_EXCHANGE);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRaNationalDayExchange::user[%d, %s], item[%s]", m_role->GetUID(), m_role->GetName(),
		ItemConfigDataLog(&reward->reward_item, NULL));
}

void RoleActivity::OnRaZhenbaoge2OperReq(int opera, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ZHENBAOGE2))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_ZHENBAOGE2);

	switch(opera)
	{
	case Protocol::RA_ZHENBAOGE_OPERA_TYPE_QUERY_INFO:
		{
			this->SendRaZhenbaoge2Info();
		}
		break;

	case Protocol::RA_ZHENBAOGE_OPERA_TYPE_BUY:
		{
			this->OnRaZhenbaoge2Buy(param);
		}
		break;

	case Protocol::RA_ZHENBAOGE_OPEAR_TYPE_BUY_ALL:
		{
			this->OnRaZhenbaoge2BuyAll();
		}
		break;

	case Protocol::RA_ZHENBAOGE_OPEAR_TYPE_FLUSH:
		{
			this->OnRaZhenbaoge2Flush(false, false);
		}
		break;

	case Protocol::RA_ZHENBAOGE_OPEAR_TYPE_RARE_FLUSH:
		{
			this->OnRaZhenbaoge2RareFlush();
		}
		break;

	case Protocol::RA_ZHENBAOGE_OPERA_TYPE_FETCH_SERVER_GIFT:
		{
			this->OnRazhenbaoge2FetchReward(param);
		}
		break;
	}
}

void RoleActivity::SendRaZhenbaoge2Info()
{
	Protocol::SCRAZhenbaoge2Info scrazbgi;

	RandActivityZhenBaoGe2 *zhenbaoge2_activity = (RandActivityZhenBaoGe2 *)RandActivityManager::Instance().GetRandActivityZhenBaoGe2();
	if (NULL != zhenbaoge2_activity)
	{
		memcpy(scrazbgi.zhenbaoge_item_list, m_activity_info.ra_zhenbaoge2_item_list, sizeof(scrazbgi.zhenbaoge_item_list));
		scrazbgi.zhenbaoge_server_fetch_flag = m_activity_info.ra_zhenbaoge2_server_fetch_flag;
		scrazbgi.cur_server_flush_times = m_activity_info.ra_zhenbaoge2_flush_times;
		scrazbgi.reserve_sh = 0;
		scrazbgi.zhenbaoge_next_flush_timetamp = m_activity_info.ra_zhenbaoge2_next_free_flush_timestamp;

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scrazbgi, sizeof(scrazbgi));
	}
}

void RoleActivity::OnRaZhenbaoge2Buy(int index)
{
	if (index < 0 || index >= RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT)
	{
		return;
	}

	int seq = m_activity_info.ra_zhenbaoge2_item_list[index];
	if (seq <= 0)
	{
		return;
	}

	const RandActivityZhenBaoGeConfig *zhenbaoge2_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaoge2Cfg(seq);
	if (NULL == zhenbaoge2_cfg)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(zhenbaoge2_cfg->buy_consume_gold, "RoleActivity::OnRaZhenbaoge2Buy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_activity_info.ra_zhenbaoge2_item_list[index] = 0;

	bool ret = m_role->GetKnapsack()->Put(zhenbaoge2_cfg->reward_item, PUT_REASON_RA_ZHENBAOGE2_REWARD);
	LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_ZHENBAOGE2, m_role->GetUID(), m_role->GetName(), zhenbaoge2_cfg->reward_item.item_id, zhenbaoge2_cfg->reward_item.num);
	this->SendRaZhenbaoge2Info();
	this->OnRaZhenbaoge2CheckAllBuyGift();

	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRaZhenbaoge2Buy user[%d, %s] index[%d] seq[%d] item[%s] ret[%d]",
		m_role->GetUID(), m_role->GetName(), index, seq, ItemConfigDataLog(&zhenbaoge2_cfg->reward_item, NULL), ret? 1 : 0);
}

void RoleActivity::OnRaZhenbaoge2BuyAll()
{
	int total_consume_gold = 0;
	int count = 0;
	RandActivityZhenBaoGeConfig temp_item_list[RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT];

	for (int i = 0; i < RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT; ++ i)
	{
		if (m_activity_info.ra_zhenbaoge2_item_list[i] > 0)
		{
			const RandActivityZhenBaoGeConfig *zhenbaoge_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaoge2Cfg(m_activity_info.ra_zhenbaoge2_item_list[i]);
			if (NULL == zhenbaoge_cfg)
			{
				return;
			}

			temp_item_list[i] = *zhenbaoge_cfg;
			total_consume_gold += zhenbaoge_cfg->buy_consume_gold;
			++ count;
		}
	}

	if (total_consume_gold <= 0)
	{
		m_role->NoticeNum(errornum::EN_RA_ZHENBAOGE_HAVE_ALL_BUY);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(total_consume_gold, "RoleActivity::OnRaZhenbaoge2BuyAll"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRaZhenbaoge2BuyAll user[%d, %s] consume_gold[%d] item[",
		m_role->GetUID(), m_role->GetName(), total_consume_gold);

	for (int index = 0; index < RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT; ++ index)
	{
		if (temp_item_list[index].seq > 0)
		{
			if (m_role->GetKnapsack()->Put(temp_item_list[index].reward_item, PUT_REASON_RA_ZHENBAOGE2_REWARD))
			{
				m_activity_info.ra_zhenbaoge2_item_list[index] = 0;
				gamelog::g_log_roleactivity.buff_printf("%s", ItemConfigDataLog(&temp_item_list[index].reward_item, NULL));
				LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_ZHENBAOGE2, m_role->GetUID(), m_role->GetName(), temp_item_list[index].reward_item.item_id, temp_item_list[index].reward_item.num);
			}
		}
	}

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);

	this->OnRaZhenbaoge2CheckAllBuyGift();
	this->SendRaZhenbaoge2Info();
}

bool RoleActivity::OnRaZhenbaoge2Flush(bool is_free, bool flush_for_rare)
{
	RandActivityZhenBaoGe2 *zhenbaoge2_activity = (RandActivityZhenBaoGe2 *)RandActivityManager::Instance().GetRandActivityZhenBaoGe2();
	if (NULL == zhenbaoge2_activity)
	{
		return false;
	}

	int consume_gold = 0;
	bool has_get_rare_item = false;

	if (!is_free)
	{
		consume_gold = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaoge2FlushConsumeGold();
	}

	const int server_chou_times = zhenbaoge2_activity->GetServerReflushTimes();

	RandActivityZhenBaoGeConfig temp_result[RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT];

	bool ret = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaoge2RandomResult(m_activity_info.ra_zhenbaoge2_add_weight_list,
		RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT, is_free, temp_result, RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT, server_chou_times);
	if (!ret)
	{
		return false;
	}

	// 消耗
	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "RoleActivity::OnRaZhenbaoge2Flush"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	if (is_free)
	{
		const tm *local_time = EngineAdapter::Instance().LocalTime();
		unsigned int interval = 0;

		if (0 == local_time->tm_hour % 2)
		{
			interval = (EngineAdapter::Instance().NextHourInterval(0, 0)) + SECOND_PER_HOUR;
		}
		else
		{
			interval = (EngineAdapter::Instance().NextHourInterval(0, 0));
		}

		m_activity_info.ra_zhenbaoge2_next_free_flush_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + interval;
	}

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRaZhenbaoge2Flush user[%d, %s] is_free[%d] flush_for_rare[%d] consume_gold[%d] item_seq[",
		m_role->GetUID(), m_role->GetName(), is_free, flush_for_rare, consume_gold);

	// 给予
	for (int i = 0; i < RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT; ++ i)
	{
		m_activity_info.ra_zhenbaoge2_item_list[i] = (short)temp_result[i].seq;

		if (temp_result[i].is_rare)
		{
			has_get_rare_item = true;

			// 刷到珍稀物品传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_zhenbaoge2_zhenxi_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_ZHENBAOGE2, temp_result[i].reward_item.item_id);

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}

		gamelog::g_log_roleactivity.buff_printf("(%d)", m_activity_info.ra_zhenbaoge2_item_list[i]);
	}

	if (!is_free)
	{
		++m_activity_info.ra_zhenbaoge2_flush_times;
		zhenbaoge2_activity->AddServerReflushTimes();
		this->OnRaZhenbaoge2AddWeight();
	}

	if (!flush_for_rare)
	{
		this->SendRaZhenbaoge2Info();
	}

	this->OnRaZhenbaoge2CheckResetWeight();

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);

	return (!has_get_rare_item);
}

void RoleActivity::OnRaZhenbaoge2RareFlush()
{
	int times = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaoge2AutoFlushTimes();
	int consume_gold = times * LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaoge2FlushConsumeGold();
	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(static_cast<long long>(consume_gold)))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	for (int i = 0; i < times; ++ i)
	{
		bool flush_has_rare = this->OnRaZhenbaoge2Flush(false, true);
		if (!flush_has_rare)
		{
			break;
		}
	}

	this->SendRaZhenbaoge2Info();
}

void RoleActivity::OnRazhenbaoge2FetchReward(int seq)
{
	const RandActivityZhenBaoGeServerConfig *server_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaoge2ServerCfg(seq);
	if (NULL == server_cfg)
	{
		return;
	}

	RandActivityZhenBaoGe2 *zhenbaoge2_activity = (RandActivityZhenBaoGe2 *)RandActivityManager::Instance().GetRandActivityZhenBaoGe2();
	if (NULL == zhenbaoge2_activity)
	{
		return;
	}

	if ((m_activity_info.ra_zhenbaoge2_server_fetch_flag & (1 << seq)) > 0)
	{
		m_role->NoticeNum(errornum::EN_RA_ZHENBAOGE_HAVE_FETCH);
		return;
	}

	int cur_times = m_activity_info.ra_zhenbaoge2_flush_times;
	if (cur_times < server_cfg->can_fetch_times)
	{
		m_role->NoticeNum(errornum::EN_RA_ZHENBAOGE_TIMES_NOT_ENOUGH);
		return;
	}

	if (m_role->GetVip()->GetVipLevel() < server_cfg->vip_limit)
	{
		m_role->NoticeNum(errornum::EN_RA_ZHENBAOGE_VIP_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (m_role->GetKnapsack()->Put(server_cfg->reward_item, PUT_REASON_RA_ZHENBAOGE2_REWARD))
	{
		m_activity_info.ra_zhenbaoge2_server_fetch_flag |= (1 << seq);
		this->SendRaZhenbaoge2Info();

		gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRazhenbaoge2FetchReward user[%d, %s] reward_item[%s] flag[%d]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&server_cfg->reward_item, NULL), m_activity_info.ra_zhenbaoge2_server_fetch_flag);
	}
}

void RoleActivity::OnRaZhenbaoge2CheckAllBuyGift()
{
	for (int i = 0; i < RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT; ++ i)
	{
		if (m_activity_info.ra_zhenbaoge2_item_list[i] > 0)
		{
			return;
		}
	}

	static MailContentParam contentparam; contentparam.Reset();
	const ItemConfigData reward_item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaoge2AllBuyReward();

	contentparam.item_list[0].item_id = reward_item_cfg.item_id;
	contentparam.item_list[0].num = reward_item_cfg.num;
	contentparam.item_list[0].is_bind = (reward_item_cfg.is_bind) ? 1 : 0;

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_xianyuan_treas_all_buy_content, RAND_ACTIVITY_TYPE_ZHENBAOGE2);

	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRaZhenbaoge2CheckAllBuyGift user[%d, %s] reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&reward_item_cfg, NULL));
	}
}

void RoleActivity::OnRaZhenbaoge2AddWeight()
{
	for (int i = 0; i < RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT; ++ i)
	{
		const RandActivityZhenBaoGeAddWeightConfig *add_weight_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaoge2AddWeight(i);
		if (NULL == add_weight_cfg)
		{
			continue;
		}

		if (m_activity_info.ra_zhenbaoge2_add_weight_list[i].seq == add_weight_cfg->seq)
		{
			m_activity_info.ra_zhenbaoge2_add_weight_list[i].weight += add_weight_cfg->add_weight;
		}
	}
}

void RoleActivity::OnRaZhenbaoge2CheckResetWeight()
{
	for (int item_index = 0; item_index < RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT; ++ item_index)
	{
		for(int weight_index = 0; weight_index < RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT; ++ weight_index)
		{
			if (m_activity_info.ra_zhenbaoge2_item_list[item_index] == m_activity_info.ra_zhenbaoge2_add_weight_list[weight_index].seq)
			{
				m_activity_info.ra_zhenbaoge2_add_weight_list[weight_index].weight = 0;
			}
		}
	}
}

void RoleActivity::OnRAPromotingPositionOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PROMOTING_POSITION))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_PROMOTING_POSITION);

	switch(opera_type)
	{
	case Protocol::RA_PROMOTING_POSITION_OPERA_TYPE_ALL_INFO:
		{
			this->SendRAPromotingPositionAllInfo();
		}
		break;

	case Protocol::RA_PROMOTING_POSITION_OPERA_TYPE_PLAY:
		{
			this->OnRAPromotingPositionPlay(param1);
		}
		break;

	case Protocol::RA_PROMOTING_POSITION_OPERA_FETCH_REWARD:
		{
			this->OnRAPromotingPositionFetchReward(param1);
		}
		break; 
	}
}

void RoleActivity::SendRAPromotingPositionAllInfo()
{
	static Protocol::SCRAPromotingPositionAllInfo ppai;

	ppai.next_free_timestamp = m_activity_info.ra_promoting_position_next_free_timestamp;
	ppai.play_times = m_activity_info.ra_promoting_position_play_times;
	ppai.reward_flag = static_cast<int>(m_activity_info.ra_promoting_position_reward_flag);
	ppai.start_pos = m_activity_info.ra_promoting_position_start_pos;
	if (RA_PROMOTING_POSITION_CIRCLE_TYPE_INSIDE == m_activity_info.ra_promoting_position_start_pos.circle_type)
	{
		m_activity_info.ra_promoting_position_start_pos.Reset();		// 内圈抽奖完，重置抽奖位置
	}

	int send_len = sizeof(ppai);

	RandActivityPromotingPosition *activity = (RandActivityPromotingPosition*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_PROMOTING_POSITION);
	if (NULL != activity)
	{
		ppai.records_count = activity->GetRewardRecordsList(ppai.record_list);

		if (ppai.records_count >= 0)
		{
			send_len -= (RA_PROMOTION_POSITION_REWARD_RECORDS_MAX_COUNT - ppai.records_count) * sizeof(ppai.record_list[0]);
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&ppai, send_len);
}

void RoleActivity::OnRAPromotingPositionPlay(int times)
{
	if (1 != times && RA_PROMOTION_POSITION_MULTIPLAY_TIMES != times)
	{
		return;
	}

	// 检查寻宝仓库位置是否足够
	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, times * 2))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	RandActivityPromotingPosition *promoting_position_activity =
		(RandActivityPromotingPosition *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_PROMOTING_POSITION);
	if (NULL == promoting_position_activity)
	{
		return;
	}

	int server_chou_times = promoting_position_activity->GetServerChouTimes();

	// 检查消耗物并消耗，然后获取抽奖类型（不同类型权重不一样）
	bool is_use_gold = false;
	bool is_use_item = false;
	int play_type = RA_PROMOTING_POSITION_PLAY_TYPE_INVALID;
	int use_gold = 0;

	if (1 == times)		// 单次抽奖优先级：免费 > 使用额外次数 > 使用元宝
	{
		if (m_activity_info.ra_promoting_position_next_free_timestamp <= static_cast<unsigned int>(EngineAdapter::Instance().Time()))
		{
			m_activity_info.ra_promoting_position_next_free_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionFreePlayInterval() * SECOND_PER_HOUR;
			play_type = RA_PROMOTING_POSITION_PLAY_TYPE_FREE;
		}
		else
		{
			use_gold = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionPlayOnceGold();

			if (!m_role->GetKnapsack()->GetMoney()->UseGold(use_gold, "RoleActivity::OnRAPromotingPositionPlay"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
			else
			{
				is_use_gold = true;
				play_type = RA_PROMOTING_POSITION_PLAY_TYPE_GOLD;
			}
		}
	}
	else if(RA_PROMOTION_POSITION_MULTIPLAY_TIMES == times)		// 多次抽奖优先级：使用额外次数 > 使用物品 > 使用元宝
	{
		if (m_role->GetKnapsack()->ConsumeItem(LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPosition10TimesUseItem(), 1, "RoleActivity::OnRAPromotingPositionPlay"))
		{
			is_use_item = true;
			play_type = RA_PROMOTING_POSITION_PLAY_TYPE_USE_ITEM;
		}
		else
		{
			use_gold = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionPlay10TimesGold();

			if (!m_role->GetKnapsack()->GetMoney()->UseGold(use_gold, "RoleActivity::OnRAPromotingPositionPlay"))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
			else
			{
				is_use_gold = true;
				play_type = RA_PROMOTING_POSITION_PLAY_TYPE_GOLD;
			}
		}
	}

	int split_position = 0;		// 双开位置，仅发送给客户端用
	int reward_count = 0;
	RAPromotingPositionRewardInfo reward_info_list[RA_PROMOTION_POSITION_MULTIPLAY_TIMES * 2]; memset(reward_info_list, 0, sizeof(reward_info_list));

	// 抽取time次奖励
	for (int round = 0; round < times; ++ round)
	{
		// 由于现在抽中内圈立马自动在内圈抽一个而且需要重置位置--但客户端需要知道在内圈转，所以之前的人在发协议之后清掉
		// 告诉客户端的理应用其他协议发送才不会导致bug
		if (RA_PROMOTING_POSITION_CIRCLE_TYPE_INSIDE == m_activity_info.ra_promoting_position_start_pos.circle_type)
		{
			m_activity_info.ra_promoting_position_start_pos.Reset();
		}

		// 首先检测保底抽奖
		const RandActivityPromotingPositionConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionBaodiReward(server_chou_times);
		if (NULL != reward_cfg)
		{
			//保底的一定是内圈的，运营要求不区分内外墙保底
			this->RAPromotingPositionGiveRewardHelper(reward_cfg->seq, RA_PROMOTING_POSITION_CIRCLE_TYPE_INSIDE, reward_info_list, reward_count);

			++m_activity_info.ra_promoting_position_play_times;
		}
		else
		{
			const RandActivityPromotingPositionConfig * outside_cfg = NULL;
			if (RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE == m_activity_info.ra_promoting_position_start_pos.circle_type)
			{
				outside_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionOutsideCfg(static_cast<int>(m_activity_info.ra_promoting_position_start_pos.position));
				if (NULL != outside_cfg && RA_PROMOTING_POSITION_REWARD_TYPE_INSIDE == outside_cfg->reward_type)	//如果是内圈奖励
				{
					m_activity_info.ra_promoting_position_start_pos.circle_type = RA_PROMOTING_POSITION_CIRCLE_TYPE_INSIDE;
				}
			}

		if (RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE == m_activity_info.ra_promoting_position_start_pos.circle_type)
		{
			int begin_index = promoting_position_activity->GetOutsideBeginIndex();
			int end_index = promoting_position_activity->GetOutsideEndIndex();
			if (end_index - begin_index >= RA_PROMOTING_POSITION_OUTSIDE_REWARD_MAX_SEQ || end_index <= begin_index)
			{
				return;
			}

			const RandActivityPromotingPositionConfig * reward_cfg = 
				LOGIC_CONFIG->GetRandActivityCfg().GetRandomPromotingPositionReward(begin_index, end_index, play_type, RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE);
			if (NULL != reward_cfg)
			{
				++m_activity_info.ra_promoting_position_play_times;

				if( RA_PROMOTING_POSITION_REWARD_TYPE_SPLIT == reward_cfg->reward_type)			// 对于双开的奖励，重新摇两次奖
				{
					split_position = 1;
					m_activity_info.ra_promoting_position_start_pos.circle_type = RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE;
					m_activity_info.ra_promoting_position_start_pos.position = static_cast<unsigned char>(reward_cfg->seq);
					this->SendRAPromotingPositionAllInfo();
					m_activity_info.ra_promoting_position_start_pos.Reset();

					for (int i = 0; i < 2; ++ i)
					{
						const RandActivityPromotingPositionConfig * reward_cfg_extra = 
							LOGIC_CONFIG->GetRandActivityCfg().GetRandomPromotingPositionReward(begin_index, end_index, play_type, RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE, true);

						if (NULL != reward_cfg_extra)
						{
							this->RAPromotingPositionGiveRewardHelper(reward_cfg_extra->seq, RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE, reward_info_list, reward_count);
						}
					}
				}
				else		// 对于其它类型的奖励，直接给予
				{
					this->RAPromotingPositionGiveRewardHelper(reward_cfg->seq, RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE, reward_info_list, reward_count);
				}
			}
		}
		else if (RA_PROMOTING_POSITION_CIRCLE_TYPE_INSIDE == m_activity_info.ra_promoting_position_start_pos.circle_type)
		{
			int begin_index = promoting_position_activity->GetInsideBeginIndex();
			int end_index = promoting_position_activity->GetInsideEndIndex();
			if (end_index - begin_index >= RA_PROMOTING_POSITION_INSIDE_REWARD_MAX_SEQ || end_index <= begin_index)
			{
				return;
			}

			const RandActivityPromotingPositionConfig * reward_cfg = 
				LOGIC_CONFIG->GetRandActivityCfg().GetRandomPromotingPositionReward(begin_index, end_index, play_type, RA_PROMOTING_POSITION_CIRCLE_TYPE_INSIDE);
			if (reward_cfg != NULL)
			{
				++m_activity_info.ra_promoting_position_play_times;

					this->RAPromotingPositionGiveRewardHelper(reward_cfg->seq, RA_PROMOTING_POSITION_CIRCLE_TYPE_INSIDE, reward_info_list, reward_count);
				}
			}
		}

		++server_chou_times;
	}

	// 增加全服抽奖次数
	if (is_use_gold || is_use_item)
	{
		promoting_position_activity->AddServerChouTimes(times);

		//RandActivityLogManager::Instance().AddLogData(m_role, RAND_ACTIVITY_TYPE_PROMOTING_POSITION, use_gold, times);
	}

	// 日志
	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAPromotingPositionPlay user[%d %s], play_times[%d], play_type[%d], use_gold[%d], cur_extra_times[%d], use_item[%d], reward_seq[",
		m_role->GetUID(), m_role->GetName(), times, play_type, (is_use_gold ? use_gold : 0), m_activity_info.ra_promoting_position_extra_times, (is_use_item ? LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPosition10TimesUseItem() : 0));

	for (int count = 0; count < reward_count; ++ count)
	{
		gamelog::g_log_roleactivity.buff_printf("(%d %s)", static_cast<int>(reward_info_list[count].seq), (RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE == reward_info_list[count].circle_type) ? "out" : "in");
	}

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);

	// 发送
	{
		if (0 == split_position)
		{
			this->SendRAPromotingPositionAllInfo();
		}

		static Protocol::SCRAPromotingPositionRewardInfo ppri;

		ppri.split_position = split_position;
		ppri.reward_count = reward_count;
		memcpy(ppri.reward_info_list, reward_info_list, sizeof(ppri.reward_info_list));

		int send_len = sizeof(ppri) - (RA_PROMOTION_POSITION_MULTIPLAY_TIMES * 2 - ppri.reward_count) * sizeof(ppri.reward_info_list[0]);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ppri, send_len);
	}
}

void RoleActivity::RAPromotingPositionGiveRewardHelper(int seq, int circle_type, RAPromotingPositionRewardInfo reward_info_list[RA_PROMOTION_POSITION_MULTIPLAY_TIMES * 2], int &reward_count)
{
	if (NULL == reward_info_list || reward_count < 0)
	{
		return;
	}

	const RandActivityPromotingPositionConfig * reward_cfg = NULL;
	if (RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE == circle_type)
	{
		reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionOutsideCfg(seq);
	}
	else
	{
		reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionInsideCfg(seq);
	}

	if (NULL == reward_cfg)
	{
		return;
	}

	switch(reward_cfg->reward_type)
	{
	case RA_PROMOTING_POSITION_REWARD_TYPE_GOLD:
		{
			m_activity_info.ra_promoting_position_start_pos.circle_type = circle_type;
			m_activity_info.ra_promoting_position_start_pos.position = static_cast<unsigned char>(reward_cfg->seq);		// 设置下一次的开始位置

			int add_gold = static_cast<int>((1 + reward_cfg->reward_gold_rate * 0.01) * LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionPlayOnceGold());
			m_role->GetKnapsack()->GetMoney()->AddGold(add_gold, "RoleActivity::CheckRAPromotingPositionGiveReward");

			if (reward_cfg->is_broadcast)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_promoting_position_reward_gold_content,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), add_gold);
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
				}

				// 增加抽奖记录
				RandActivityPromotingPosition *activity = (RandActivityPromotingPosition*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_PROMOTING_POSITION);
				if (NULL != activity)
				{
					RAPromotingPositionRewardInfo reward_info;
					reward_info.circle_type = circle_type;
					reward_info.seq = static_cast<unsigned char>(seq);

					activity->AddRewardRecord(m_role, reward_info);
				}
			}
		}
		break;

	case RA_PROMOTING_POSITION_REWARD_TYPE_ITEM:
		{
			m_activity_info.ra_promoting_position_start_pos.circle_type = circle_type;
			m_activity_info.ra_promoting_position_start_pos.position = static_cast<unsigned char>(reward_cfg->seq);		// 设置下一次的开始位置

			//ItemConfigData reward_item = reward_cfg->reward_item;
			ChestItemInfo reward_item[MODE_1_COUNT]; memset(reward_item, 0, sizeof(reward_item));

			int count = 0;
			reward_item[count].item_id = reward_cfg->reward_item.item_id;
			reward_item[count].num = reward_cfg->reward_item.num;
			reward_item[count].is_bind = reward_cfg->reward_item.is_bind;
			++ count;

//			m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, count, reward_item, "RoleActivity::CheckRAPromotingPositionGiveReward");
			this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_PROMOTING_POSITION, count, reward_item, "RoleActivity::CheckRAPromotingPositionGiveReward");
			LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_PROMOTING_POSITION, m_role->GetUID(), m_role->GetName(), reward_cfg->reward_item.item_id, reward_cfg->reward_item.num);
			if (reward_cfg->is_broadcast)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_promoting_position_reward_item_content,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), reward_cfg->reward_item.item_id);
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
				}

				// 增加抽奖记录
				RandActivityPromotingPosition *activity = (RandActivityPromotingPosition*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_PROMOTING_POSITION);
				if (NULL != activity)
				{
					RAPromotingPositionRewardInfo reward_info;
					reward_info.circle_type = circle_type;
					reward_info.seq = static_cast<unsigned char>(seq);

					activity->AddRewardRecord(m_role, reward_info);
				}
			}
		}
		break;

	case RA_PROMOTING_POSITION_REWARD_TYPE_INSIDE:		// 内圈奖励，仅把抽奖位置移动到内圈起点
		{
			m_activity_info.ra_promoting_position_start_pos.circle_type = RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE;
			m_activity_info.ra_promoting_position_start_pos.position = static_cast<unsigned char>(seq);
		}
		break;
	}

	// 存储奖励信息
	if (reward_count < RA_PROMOTION_POSITION_MULTIPLAY_TIMES * 2)
	{
		reward_info_list[reward_count].seq = static_cast<unsigned char>(seq);
		reward_info_list[reward_count].circle_type = circle_type;
		reward_count ++;
	}
}

void RoleActivity::OnRAPromotingPositionChongzhiGold(int chongzhi_gold)
{
	//if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PROMOTING_POSITION))
	//{
	//	return;
	//}

	//this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_PROMOTING_POSITION);

	//m_activity_info.ra_promoting_position_chongzhi_gold += chongzhi_gold;

	//int one_time_need_chongzhi_gold = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionExtraTimePerChongzhi();
	//int add_times = 0;
	//if (one_time_need_chongzhi_gold > 0)
	//{
	//	add_times = m_activity_info.ra_promoting_position_chongzhi_gold / one_time_need_chongzhi_gold;
	//}

	//if (add_times > 0)
	//{
	//	int extra_times = m_activity_info.ra_promoting_position_extra_times + add_times;
	//	if (extra_times > USHRT_MAX)
	//	{
	//		extra_times = USHRT_MAX;
	//	}
	//	m_activity_info.ra_promoting_position_extra_times = extra_times;

	//	m_activity_info.ra_promoting_position_chongzhi_gold -= add_times * one_time_need_chongzhi_gold;

	//	this->SendRAPromotingPositionAllInfo();
	//}

	//gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRAPromotingPositionChongzhiGold user[%d %s], chongzhi_gold[%d], cur_chongzhi_gold[%d], cur_extra_times[%d]",
	//	m_role->GetUID(), m_role->GetName(), chongzhi_gold, m_activity_info.ra_promoting_position_chongzhi_gold, m_activity_info.ra_promoting_position_extra_times);
}

void RoleActivity::OnRAPromotingPositionFetchReward(int seq)
{
	RandActivityPromotingPosition *promoting_position_activity =
		(RandActivityPromotingPosition *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_PROMOTING_POSITION);
	if (NULL == promoting_position_activity)
	{
		return;
	}

	int begin_index = promoting_position_activity->GetRewardBeginIndex();
	int end_index = promoting_position_activity->GetRewardEndIndex();
	if (end_index - begin_index >= RA_PROMOTION_POSITION_REWARD_CFG_INTERVAL || end_index <= begin_index ||
		seq >= end_index || seq < begin_index)
	{
		return;
	}

	const RandActivityPromotingPositionRewardCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetPromotingPositionRewardCfg(seq);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (m_role->GetVip()->GetVipLevel() < reward_cfg->vip_limit)
	{
		return;
	}

	// 检查
	if (m_activity_info.ra_promoting_position_play_times < reward_cfg->play_times)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (0 == (m_activity_info.ra_promoting_position_reward_flag & (1 << (seq - begin_index))))	// 如果未领取
	{
		m_activity_info.ra_promoting_position_reward_flag |= (1 << (seq - begin_index));

		// 发放
		m_role->GetKnapsack()->Put(reward_cfg->reward_item, PUT_REASON_PROMOTING_POSITION_REWARD);

		// 发送
		this->SendRAPromotingPositionAllInfo();

		// 日志
		gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnRAPromotingPositionFetchReward user[%d, %s], seq[%d], fetch_flag[%d], reward[%s]",
			m_role->GetUID(), m_role->GetName(), seq, m_activity_info.ra_promoting_position_reward_flag, ItemConfigDataLog(&reward_cfg->reward_item, NULL));
	}
}

void RoleActivity::OnRABlackMarketOperaReq(int opera_type, int param1, int param2)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BLACK_MARKET))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BLACK_MARKET);

	switch(opera_type)
	{
	case Protocol::RA_BLACK_MARKET_OPERA_TYPE_ALL_INFO:
		{
			this->SendRABlackMarketAllInfo();
		}
		break;

	case Protocol::RA_BLACK_MARKET_OPERA_TYPE_OFFER:
		{
			this->OnRABlackMarketOfferItem(param1, param2);
		}
		break;
	}
}

void RoleActivity::SendRABlackMarketAllInfo()
{
	static Protocol::SCRABlackMarketAllInfo bmai;

	RandActivityBlackMarket* activity = (RandActivityBlackMarket*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_BLACK_MARKET);
	if (NULL != activity)
	{
		RABlackMarketItemInfo item_info_list[RA_BLACK_MARKET_MAX_ITEM_COUNT];

		activity->GetItemInfoList(item_info_list);

		for (int index = 0; index < RA_BLACK_MARKET_MAX_ITEM_COUNT; ++ index)
		{
			bmai.item_info_list[index].seq = item_info_list[index].seq;
			bmai.item_info_list[index].cur_price = item_info_list[index].cur_price;
			bmai.item_info_list[index].buyer_uid = item_info_list[index].buyer_uid;
			memset(bmai.item_info_list[index].buyer_name, 0, sizeof(bmai.item_info_list[index].buyer_name));

			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(item_info_list[index].buyer_uid, false);
			if (NULL != user_node)
			{
				user_node->GetName(bmai.item_info_list[index].buyer_name);
			}
		}

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&bmai, sizeof(bmai));
	}
}

void RoleActivity::OnRABlackMarketOfferItem(int index, int price)
{
	RandActivityBlackMarket* activity = (RandActivityBlackMarket*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_BLACK_MARKET);
	if (NULL == activity)
	{
		return;
	}

	if (activity->OnRoleOfferItem(m_role, index, price))
	{
		this->SendRABlackMarketAllInfo();
	}
}

void RoleActivity::OnRaTreasuresMallOperaReq(int opera_type, int param1, int param2)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TREASURES_MALL))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BLACK_MARKET);

	switch (opera_type)
	{
	case Protocol::RA_TREASURES_MALL_OPERA_TYPE_REQ_INFO:
		{
			this->SendTreasuresMallAllInfo();
		}
		break;

	case Protocol::RA_TREASURES_MALL_OPERA_TYPE_BUY:
		{
			this->OnRaTreasuresMallBuy(param1);
		}
		break;

	case Protocol::RA_TREASURES_MALL_OPERA_TYPE_EXCHANGE:
		{
			this->OnRaTreasuresMallExchange(param1);
		}
		break;
	}
}

void RoleActivity::SendTreasuresMallAllInfo()
{
	static Protocol::SCRATreasuresMallAllInfo cmd;

	cmd.cur_score = m_activity_info.ra_treasures_mall_score;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void RoleActivity::OnRaTreasuresMallBuy(int item_index)
{
	const RandActivityTreasuresMallBuyConfig::BuyItemConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTreasuresMallBuyRewardCfg(item_index);
	if (NULL == reward_cfg)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!knapsack->GetMoney()->GoldMoreThan(reward_cfg->cost_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!knapsack->GetMoney()->UseGold(reward_cfg->cost_gold, "OnRaTreasuresMallBuy"))
	{
		return;
	}

	knapsack->Put(reward_cfg->reward_item, PUT_REASON_TREASURES_MALL_BUY);

	m_activity_info.ra_treasures_mall_score += reward_cfg->get_score;

	gamelog::g_log_roleactivity.printf(LL_INFO, "OnRaTreasuresMallBuy user[%d %s], reward_item[%s] now_score[%d] add_score[%d]",
		m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&reward_cfg->reward_item, NULL), m_activity_info.ra_treasures_mall_score, reward_cfg->get_score);

	this->SendTreasuresMallAllInfo();
}

void RoleActivity::OnRaTreasuresMallExchange(int exchange_index)
{
	const RandActivityTreasuresMallExchangeConfig::ExchangeItemConfig *exchange_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTreasuresMallExchangeRewardCfg(exchange_index);
	if (NULL == exchange_cfg)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (m_activity_info.ra_treasures_mall_score - exchange_cfg->need_score < 0)
	{
		m_role->NoticeNum(errornum::EN_RA_ZHENBAOSHANGCHENG_SCORE_NOT_ENOUGH);
		return;
	}

	m_activity_info.ra_treasures_mall_score -= exchange_cfg->need_score;

	knapsack->Put(exchange_cfg->reward_item, PUT_REASON_TREASURES_MALL_EXCHANGE);

	gamelog::g_log_roleactivity.printf(LL_INFO, "OnRaTreasuresMallBuy user[%d %s], reward_item[%s] now_score[%d] dec_score[%d]",
		m_role->GetUID(), m_role->GetName(),ItemConfigDataLog(&exchange_cfg->reward_item, NULL),m_activity_info.ra_treasures_mall_score, exchange_cfg->need_score);

	this->SendTreasuresMallAllInfo();
}

//-------------------------------
// 开服活动
//-------------------------------
void RoleActivity::OnRAOpenServerOperaReq(int ra_type, int opera_type, int param1, int param2)
{
	if (ra_type < RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN || ra_type > RAND_ACTIVITY_TYPE_OPEN_SERVER_END)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(ra_type))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(ra_type);

	int sub_index = ra_type - RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN;
	if (sub_index < 0 || sub_index >= OPEN_SERVER_ACTIVITY_COUNT)
	{
		return;
	}

	OpenServerSubActivityInfo &info = m_activity_info.open_server_sub_info_list[sub_index];

	switch (opera_type)
	{
	case Protocol::RA_OPEN_SERVER_OPERA_TYPE_REQ_INFO:
		this->SendRAOpenServerInfo(ra_type, info.reward_flag);
		break;

	case Protocol::RA_OPEN_SERVER_OPERA_TYPE_FETCH:
		{
			int reward_seq = param1;
			const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(ra_type, reward_seq);
			if (NULL == cfg)
			{
				return;
			}

			if (info.reward_flag & (1 << reward_seq))
			{
				m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_FETCHED);
				return;
			}

			if (!this->RACheckOpenServerFetchCond(ra_type, cfg))
			{
				m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_COND_LIMIT);
				return;
			}

			if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_OPENGAME_ACTIVITY))
			{
				m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
				return;
			}

			info.reward_flag |= (1 << reward_seq);

			gamelog::g_log_roleactivity.printf(LL_INFO, "RAOpenServerFetchReward user[%d, %s], level[%d], ra_type[%d], reward_flag[%d]",
				m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), ra_type, (int)info.reward_flag);

			this->SendRAOpenServerInfo(ra_type, info.reward_flag);
		}
		break;

	case Protocol::RA_OPEN_SERVER_OPERA_TYPE_REQ_BOSS_INFO:
		{
			this->SendRAOpenServerBossInfo();
		}
		break;

	case Protocol::RA_OPEN_SERVER_OPERA_TYPE_FETCH_BOSS:
		{
			this->OnFetchOpenGameKillBossReward(param1);
		}
		break;

	case Protocol::RA_OPEN_SERVER_OPERA_TYPE_FETCH_BATTE_INFO:
		{
			this->SendRAOpenServerBattleInfo();
		}
		break;
	}
}

void RoleActivity::SendRAOpenServerInfo(int ra_type, int reward_flag)
{
	{
		Protocol::SCRAOpenServerRankInfo pro_rank;
		pro_rank.rand_activity_type = ra_type;
		int top_rank_type = RANK_TOP_TYPE_MOUNT;

		if (RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_MOUNT;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_HALO;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_WING;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_SHENGONG;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_SHENYI;
		}
		else if (RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_EQUIPT_STRENGTH_LEVEL;
		}
		else if (RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_STONE_TOTAL_LEVEL;
		}
		else if (RAND_ACTIVITY_TYPE_XIANNV_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_XIANNV;
		}
		else if (RAND_ACTIVITY_TYPE_JINGLING_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_JINGLING;
		}
		else if (RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_FIGHT_MOUNT;
		}
		else if (RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_CAPABILITY;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK == ra_type)
		{
			top_rank_type = PERSON_RANK_TYPE_SHIZHUANG;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK == ra_type)
		{
			top_rank_type = PERSON_RANK_TYPE_SHIZHUANG_WUQI;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK == ra_type)
		{
			top_rank_type = PERSON_RANK_TYPE_FABAO;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK == ra_type)
		{
			top_rank_type = PERSON_RANK_TYPE_FOOTPRINT;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_QILINBI;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_TOUSHI;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_YAOSHI;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_LINGTONG;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_LINGGONG;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_LINGQI;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_FlYPET;
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK == ra_type)
		{
			top_rank_type = RANK_TOP_TYPE_WEIYAN;
		}

		{
			Protocol::SCRAOpenServerInfo pro;
			pro.rand_activity_type = ra_type;
			pro.reward_flag = reward_flag;
			pro.complete_flag = 0;
			//pro.today_chongzhi_role_count = WorldStatus::Instance().GetTodayFirstChongzhiCount();
			//检查平台
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

		//新加排行榜活动的特殊处理（时装、神兵、法宝、足迹）
		if (ra_type == RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK
			|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK)
		{
			PersonRankItem user_item_list[10];
			int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopItem(top_rank_type, 10, user_item_list);

			pro_rank.self_rank = -1;

			for (int i = 0; i < 10; i++)
			{
				if (user_item_list[i].user_id == m_role->GetUserId())
				{
					pro_rank.self_rank = i;
					break;
				}
			}

			if (pro_rank.self_rank == -1)
			{
				pro_rank.self_rank = 9999999;
			}

			pro_rank.top1_param = 0;
			pro_rank.top1_grade = 0;
			pro_rank.top1_uid = 0;
			memset(pro_rank.top1_name, 0, sizeof(pro_rank.top1_name));

			pro_rank.top2_grade = 0;
			pro_rank.top2_uid = 0;
			memset(pro_rank.top2_name, 0, sizeof(pro_rank.top2_name));

			pro_rank.top3_grade = 0;
			pro_rank.top3_uid = 0;
			memset(pro_rank.top3_name, 0, sizeof(pro_rank.top3_name));

			for (int index = 0; index < rank_count && index < 3; ++index)
			{
				switch (index)
				{
				case 0:
				{
					pro_rank.top1_param = user_item_list[index].flexible_int;

					pro_rank.top1_grade = (int)user_item_list[index].rank_value;
					pro_rank.top1_uid = user_item_list[index].GetRoleId();
					memcpy(pro_rank.top1_name, user_item_list[index].user_name, sizeof(pro_rank.top1_name));
				}
				break;

				case 1:
				{
					pro_rank.top2_grade = (int)user_item_list[index].rank_value;
					pro_rank.top2_uid = user_item_list[index].GetRoleId();
					memcpy(pro_rank.top2_name, user_item_list[index].user_name, sizeof(pro_rank.top2_name));
				}
				break;

				case 2:
				{
					pro_rank.top3_grade = (int)user_item_list[index].rank_value;
					pro_rank.top3_uid = user_item_list[index].GetRoleId();
					memcpy(pro_rank.top3_name, user_item_list[index].user_name, sizeof(pro_rank.top3_name));
				}
				break;

				default:
					break;
				}
			}

			//PersonRankItem * rank_top1_item = RankManager::Instance().GetPersonRank()->GetTopOneItemByRankType(top_rank_type);
			//if (NULL != rank_top1_item)
			//{
			//	pro_rank.top1_uid = rank_top1_item->GetRoleId();
			//	pro_rank.top1_param = (int)(rank_top1_item->rank_value);
			//}

			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro_rank, sizeof(pro_rank));
		}

		else
		{
			pro_rank.self_rank = TopRanker::Instance().GetTopRankIndex(top_rank_type, m_role->GetUID());
			//pro_rank.top1_uid = TopRanker::Instance().GetTopUser(top_rank_type, pro_rank.top1_name, &pro_rank.top1_sex, &pro_rank.top1_prof, &pro_rank.top1_avatar_timestamp);

			//pro_rank.top1_param = 0;
			//PersonRankItem * rank_top1_item = RankManager::Instance().GetPersonRank()->GetTopOneItemByRankType(TopRanker::Instance().GetPersonRankType(top_rank_type));
			//if (NULL != rank_top1_item)
			//{
			//	pro_rank.top1_param = (int)(rank_top1_item->rank_value);
			//}

			pro_rank.top1_param = 0;
			pro_rank.top1_grade = 0;
			pro_rank.top1_uid = 0;
			memset(pro_rank.top1_name, 0, sizeof(pro_rank.top1_name));

			pro_rank.top2_grade = 0;
			pro_rank.top2_uid = 0;
			memset(pro_rank.top2_name, 0, sizeof(pro_rank.top2_name));

			pro_rank.top3_grade = 0;
			pro_rank.top3_uid = 0;
			memset(pro_rank.top3_name, 0, sizeof(pro_rank.top3_name));

			PersonRankItem top3_rank_list[3];

			int count = 0;

			count = RankManager::Instance().GetPersonRank()->GetRankList(TopRanker::Instance().GetPersonRankType(top_rank_type), 3, top3_rank_list);

			for (int index = 0; index < count && index < 3; ++index)
			{
				switch (index)
				{
				case 0:
				{
					pro_rank.top1_param = top3_rank_list[index].flexible_int;

					pro_rank.top1_grade = (int)top3_rank_list[index].rank_value;
					pro_rank.top1_uid = top3_rank_list[index].GetRoleId();
					memcpy(pro_rank.top1_name, top3_rank_list[index].user_name, sizeof(pro_rank.top1_name));
				}
				break;

				case 1:
				{
					pro_rank.top2_grade = (int)top3_rank_list[index].rank_value;
					pro_rank.top2_uid = top3_rank_list[index].GetRoleId();
					memcpy(pro_rank.top2_name, top3_rank_list[index].user_name, sizeof(pro_rank.top2_name));
				}
				break;

				case 2:
				{
					pro_rank.top3_grade = (int)top3_rank_list[index].rank_value;
					pro_rank.top3_uid = top3_rank_list[index].GetRoleId();
					memcpy(pro_rank.top3_name, top3_rank_list[index].user_name, sizeof(pro_rank.top3_name));
				}
				break;

				default:
					break;
				}
			}

			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro_rank, sizeof(pro_rank));
		}
	} 

	// 全服进阶补充信息
	{
		Protocol::SCRAOpenServerUpgradeInfo pro;
		pro.rand_activity_type = ra_type;

		//if (RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_TOTAL == ra_type)
		//{
		//	memcpy(pro.total_upgrade_record_list, WorldStatus::Instance().GetCommonData().total_upgrade_mount_record_list, sizeof(pro.total_upgrade_record_list));
		//}
		//else if (RAND_ACTIVITY_TYPE_UPGRADE_HALO_TOTAL == ra_type)
		//{
		//	memcpy(pro.total_upgrade_record_list, WorldStatus::Instance().GetCommonData().total_upgrade_holo_record_list, sizeof(pro.total_upgrade_record_list));
		//}
		if (RAND_ACTIVITY_TYPE_UPGRADE_WING_TOTAL == ra_type)
		{
			memcpy(pro.total_upgrade_record_list, WorldStatus::Instance().GetCommonData().total_upgrade_wing_record_list, sizeof(pro.total_upgrade_record_list));
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_TOTAL == ra_type)
		{
			memcpy(pro.total_upgrade_record_list, WorldStatus::Instance().GetCommonData().total_upgrade_shengong_record_list, sizeof(pro.total_upgrade_record_list));
		}
		else if (RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_TOTAL == ra_type)
		{
			memcpy(pro.total_upgrade_record_list, WorldStatus::Instance().GetCommonData().total_upgrade_shenyi_record_list, sizeof(pro.total_upgrade_record_list));
		}
		else
		{
			return;
		}

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
	}
}

void RoleActivity::SendRAOpenServerBossInfo()
{
	Protocol::SCRAOpenServerBossInfo boss_info;

	boss_info.oga_kill_boss_flag = m_activity_info.oga_kill_boss_flag;
	boss_info.oga_kill_boss_reward_flag = m_activity_info.oga_kill_boss_reward_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&boss_info, sizeof(boss_info));
}

void RoleActivity::SendOpenGameGiftShopBuyInfo()
{
	Protocol::SCRAOpenGameGiftShopBuyInfo gift_info;

	gift_info.oga_gift_shop_flag = m_activity_info.oga_gift_shop_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&gift_info, sizeof(gift_info));
}

void RoleActivity::SendOpenGameGiftShopBuy2Info()
{
	Protocol::SCRAOpenGameGiftShopBuy2Info gift_info;

	gift_info.buy_flag = m_activity_info.oga_gift_shop2_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&gift_info, sizeof(gift_info));
}

void RoleActivity::SendRAOpenServerBattleInfo()
{
	Protocol::SCRAOpenServerBattleInfo battel_info;

	battel_info.qunxian_uid = WorldStatus::Instance().GetOpenServerQunxianID();
	battel_info.guildbatte_uid = WorldStatus::Instance().GetOpenServerGuildBattelID();
	battel_info.gongchengzhan_uid = WorldStatus::Instance().GetOpenServerGCZID();
	battel_info.territorywar_uid = WorldStatus::Instance().GetOpenServerTerritoryWarID();

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&battel_info, sizeof(battel_info));
}

bool RoleActivity::RACheckOpenServerFetchCond(int ra_type, const RandActivityOpenServerRewardItemConfig *cfg)
{
	if (NULL == cfg)
	{
		return false;
	}

	switch (ra_type)
	{
	case RAND_ACTIVITY_TYPE_ROLE_UPLEVEL:
		if (m_role->GetLevel() < cfg->cond1) return false;
		break;

	case RAND_ACTIVITY_TYPE_PATA:
		if (m_role->GetRolePataFB()->GetPassLevel() < cfg->cond1) return false;
		break;

	case RAND_ACTIVITY_TYPE_EXP_FB:
		if (m_role->GetRoleDailyFb()->GetExpFbHistoryEnterTimes() < cfg->cond1) return false;
		break;

	//case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT:
	//	if (m_role->GetMountManager()->GetMountParam()->grade < cfg->cond1) return false;
	//	break;

	//case RAND_ACTIVITY_TYPE_UPGRADE_HALO:
	//	if (m_role->GetHalo()->GetHaloParam()->grade < cfg->cond1) return false;
	//	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_WING:
		if (m_role->GetWing()->GetWingParam()->grade < cfg->cond1) return false;
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG:
		if (m_role->GetShengong()->GetShengongParam()->grade < cfg->cond1) return false;
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI:
		if (m_role->GetShenyi()->GetShenyiParam()->grade < cfg->cond1) return false;
		break;

	case RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN:
		if (m_activity_info.day_chongzhi < cfg->cond1) return false;
		if (WorldStatus::Instance().GetTodayFirstChongzhiCount() < cfg->cond2)
		{
			bool is_open_fake = false;

			const char *plat = LocalConfig::Instance().GetPlatName();
			if (NULL != plat)
			{
				const std::string plat_name(plat);
				is_open_fake = LOGIC_CONFIG->GetAgentAdapterConfig().GetOpenFirstChargeFakeCfgByPlatName(plat_name);
			}

			if (is_open_fake)
			{
				if (WorldStatus::Instance().GetTodayFirstChongzhiFakeCount() < cfg->cond2)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
			
		break;

	case RAND_ACTIVITY_TYPE_DAY_TOTAL_CHARGE:
		if (m_activity_info.day_chongzhi < cfg->cond1) return false;
		break;

	//case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_TOTAL:
	//	if (cfg->cond1 <= 0 || cfg->cond1 >= MAX_UPGRADE_RECORD_COUNT) return false;
	//	if (WorldStatus::Instance().GetCommonData().total_upgrade_mount_record_list[cfg->cond1] < cfg->cond2) return false;
	//	break;

	//case RAND_ACTIVITY_TYPE_UPGRADE_HALO_TOTAL:
	//	if (cfg->cond1 <= 0 || cfg->cond1 >= MAX_UPGRADE_RECORD_COUNT) return false;
	//	if (WorldStatus::Instance().GetCommonData().total_upgrade_holo_record_list[cfg->cond1] < cfg->cond2) return false;
	//	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_WING_TOTAL:
		if (cfg->cond1 <= 0 || cfg->cond1 >= MAX_UPGRADE_RECORD_COUNT) return false;
		if (WorldStatus::Instance().GetTotalUpgradeWingRecordList(cfg->cond1) < cfg->cond2) return false;
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_TOTAL:
		if (cfg->cond1 <= 0 || cfg->cond1 >= MAX_UPGRADE_RECORD_COUNT) return false;
		if (WorldStatus::Instance().GetTotalUpgradeShengongRecordList(cfg->cond1) < cfg->cond2) return false;
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_TOTAL:
		if (cfg->cond1 <= 0 || cfg->cond1 >= MAX_UPGRADE_RECORD_COUNT) return false;
		if (WorldStatus::Instance().GetTotalUpgradeShenyiRecordList(cfg->cond1) < cfg->cond2) return false;
		break;

	case RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL:
		if (m_role->GetEquipmentManager()->GetTotalStrengthen() < cfg->cond1) return false;
		break;

	case RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL:
		if (m_role->GetStoneManager()->GetTotalStoneLevel() < cfg->cond1) return false;
		break;

	case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK:
		{
			if (cfg->cond2 == 0 || m_role->GetMountManager()->GetMountParam()->grade < cfg->cond2)
			{
				return false;
			}
		}
		break;
	case RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK:
		{
			if (cfg->cond2 == 0 || m_role->GetHalo()->GetHaloParam()->grade < cfg->cond2)
			{
				return false;
			}
		}
		break;
	case RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK:
		{
			if (cfg->cond2 == 0 || m_role->GetWing()->GetWingParam()->grade < cfg->cond2)
			{
				return false;
			}
		}
		break;
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK:
		{
			if (cfg->cond2 == 0 || m_role->GetShengong()->GetShengongParam()->grade < cfg->cond2)
			{
				return false;
			}
		}
		break;
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK:
		{
			if (cfg->cond2 == 0 || m_role->GetShenyi()->GetShenyiParam()->grade < cfg->cond2)
			{
				return false;
			}
		}
		break;
	case RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK:
		{
			if (cfg->cond2 == 0 || m_role->GetEquipmentManager()->GetTotalStrengthen() < cfg->cond2)
			{
				return false;
			}
		}
		break;
	case RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK:
		{
			if (cfg->cond2 == 0 || m_role->GetStoneManager()->GetTotalStoneLevel() < cfg->cond2)
			{
				return false;
			}
		}
		break;
	case RAND_ACTIVITY_TYPE_XIANNV_RANK:
		{
			if (cfg->cond2 == 0 || m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_XIANNV) < cfg->cond2)
			{
				return false;
			}
		}
		break;
	case RAND_ACTIVITY_TYPE_JINGLING_RANK:
		{
			if (cfg->cond2 == 0 || m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING) < cfg->cond2)
			{
				return false;
			}
		}
		break;
	case RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK:
		{
			if (cfg->cond2 == 0 || m_role->GetFightMountManager()->GetFightMountParam()->grade < cfg->cond2)
			{
				return false;
			}
		}
		break;
	case RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK:
		{
			if (cfg->cond2 == 0 || m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_TOTAL) < cfg->cond2)
			{
				return false;
			}
		}
		break;

	//新增时装、神兵、法宝、足迹
	case RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK:
	{
		if (cfg->cond2 == 0 || m_role->GetShizhuang()->GetShiZhuangParam()->item_list[SHIZHUANG_TYPE_BODY].grade< cfg->cond2)
		{
			return false;
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK:
	{
		if (cfg->cond2 == 0 || m_role->GetShizhuang()->GetShiZhuangParam()->item_list[SHIZHUANG_TYPE_WUQI].grade < cfg->cond2)
		{
			return false;
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK:
	{
		if (cfg->cond2 == 0 || m_role->GetFabaoManager()->GetParam()->grade < cfg->cond2)
		{
			return false;
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK:
	{
		if (cfg->cond2 == 0 || m_role->GetFootPrint()->GetFootprintParam()->grade < cfg->cond2)
		{
			return false;
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK:
	{
		if (cfg->cond2 == 0 || m_role->GetQiLinBiManager()->GetQiLinBiParam()->grade < cfg->cond2)
		{
			return false;
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK:
	{
		if (cfg->cond2 == 0 || m_role->GetTouShiManager()->GetTouShiParam()->grade < cfg->cond2)
		{
			return false;
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK:
	{
		if (cfg->cond2 == 0 || m_role->GetYaoShiManager()->GetYaoShiParam()->grade < cfg->cond2)
		{
			return false;
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK:
	{
		if (cfg->cond2 == 0 || m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_2) < cfg->cond2)
		{
			return false;
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK:
	{
		if (cfg->cond2 == 0 || m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_3) < cfg->cond2)
		{
			return false;
		}
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK:
	{
		if (cfg->cond2 == 0 || m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_4) < cfg->cond2)
		{
			return false;
		}
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK:
	{
		if (cfg->cond2 == 0 || m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_8) < cfg->cond2)
		{
			return false;
		}
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK:
	{
		if (cfg->cond2 == 0 || m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_5) < cfg->cond2)
		{
			return false;
		}
	}
	break;

	case RAND_ACTIVITY_TYPE_QUANMINJINJIE:			//这里的判断是客户端排序需要
		{
			if (!this->RACheckOpenServerUpgradeCompleteFlag(cfg))
			{
				return false;
			}
		}
		break;

	case RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN:	//这里的判断是客户端排序需要
		{	
			if (!this->RACheckOpenServerUpgradeGroupCompleteFlag(cfg))
			{
				return false;
			}
		}
		break;
	
	case RAND_ACTIVITY_TYPE_BOSS_KILLER:
		return false;
		break;
	}

	return true;
}

void RoleActivity::RACheckOpenServerGiveReweard(int ra_type)
{
	int sub_index = ra_type - RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN;
	if (sub_index < 0 || sub_index >= OPEN_SERVER_ACTIVITY_COUNT)
	{
		return;
	}

	OpenServerSubActivityInfo &info = m_activity_info.open_server_sub_info_list[sub_index];
	if (0 == info.join_timestamp)
	{
		return;
	}

	info.join_timestamp = 0;

	const RandActivityOpenServerRewardItemConfig *cfg = NULL;

	for (int i = 1; i < RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX; i++)
	{
		if (0 == (info.reward_flag & (1 << i)))
		{
			cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(ra_type, i);
			if (NULL == cfg)
			{
				break;
			}

			if (!this->RACheckOpenServerFetchCond(ra_type, cfg))
			{
				continue;
			}

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

			int length = 0;
			switch (ra_type)
			{
			case RAND_ACTIVITY_TYPE_ROLE_UPLEVEL:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_role_uplevel_content);
				break;

			case RAND_ACTIVITY_TYPE_PATA:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_pata_content);
				break;

			case RAND_ACTIVITY_TYPE_EXP_FB:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_exp_fb_content);
				break;

			//case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT:
			//	length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_mount_content);
			//	break;

			//case RAND_ACTIVITY_TYPE_UPGRADE_HALO:
			//	length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_halo_content);
			//	break;

			case RAND_ACTIVITY_TYPE_UPGRADE_WING:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_wing_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_shengong_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_shenyi_content);
				break;

			case RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_first_charge_tuan_content);
				break;

			case RAND_ACTIVITY_TYPE_DAY_TOTAL_CHARGE:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_day_total_charge_content);
				break;

			//case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_TOTAL:
			//	length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_mount_total_content);
			//	break;

			//case RAND_ACTIVITY_TYPE_UPGRADE_HALO_TOTAL:
			//	length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_halo_total_content);
			//	break;

			case RAND_ACTIVITY_TYPE_UPGRADE_WING_TOTAL:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_wing_total_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_TOTAL:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_shengong_total_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_TOTAL:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_shenyi_total_content);
				break;

			case RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_equip_strength_level_total_content);
				break;

			case RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_stone_level_total_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_mount_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_halo_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_wing_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_shengong_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_shenyi_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_equip_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_stone_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_XIANNV_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_xiannv_cap_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_JINGLING_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_jingling_cap_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_fight_mount_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_cap_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_shizhuang_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_shenbing_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_fabao_rank_unclaimed_content);
				break;

			case RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_footprint_rank_unclaimed_content);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_qilinbi_rank_unclaimed_content);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_toushi_rank_unclaimed_content);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_yaoshi_rank_unclaimed_content);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_lingtong_rank_unclaimed_content);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_linggong_rank_unclaimed_content);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_lingqi_rank_unclaimed_content);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_flypet_rank_unclaimed_content);
				break;
			case RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK:
				length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_upgrade_weiyan_rank_unclaimed_content);
				break;
			}

			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}

	gamelog::g_log_roleactivity.printf(LL_INFO, "RACheckOpenServerGiveReweard user[%d, %s], level[%d], ra_type[%d], reward_flag[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), ra_type, (int)info.reward_flag);
}

void RoleActivity::RAActivityOpenNotification(int ra_type)
{
	static MailContentParam contentparam; contentparam.Reset();
	int length = 0;

	switch (ra_type)
	{
	case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_mount_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_wing_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_shizhuang_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_shenbing_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_fabao_national_competition_content);
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_footprint_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_halo_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_qilinbi_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_toushi_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_yaoshi_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_shengong_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_lingtong_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_linggong_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_lingqi_national_competition_content);
	}
	break;
	case RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_fight_mount_national_competition_content);
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_flypet_national_competition_content);
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_weiyan_national_competition_content);
	}
	break;

	case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK:
	{
		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_shenyi_national_competition_content);
	}
	break;
	default : break;
	}
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		m_activity_info.ra_open_server_mail_send_flag = 1;
	}
}

void RoleActivity::RACheckActivityOpenNotification()
{
	int activity_type = RandActivityManager::Instance().GetSevenDayBipinCurOpenType();
	if (-1 == activity_type)
	{
		return;
	}
	const RandActivityOpenDetail *open_cfg = LOGIC_CONFIG->GetRandActivityOpenCfg().GetOpenCfg(activity_type);
	//今天没发过邮件才通知
	if (0 == m_activity_info.ra_open_server_mail_send_flag && nullptr != open_cfg && m_role->GetLevel() >= open_cfg->send_mail_need_level && 
		RandActivityManager::Instance().IsRandActivityOpen(activity_type))
	{
		this->RAActivityOpenNotification(activity_type);
	}
}

void RoleActivity::RACheckOpenServerBossKillerGiveReweard(int ra_type)
{
	if (RAND_ACTIVITY_TYPE_BOSS_KILLER != ra_type)
	{
		return;
	}

	if (0 == m_activity_info.oga_kill_boss_join_timestamp)
	{
		return;
	}

	m_activity_info.oga_kill_boss_join_timestamp = 0;

	for(int i = 0; i < OGA_BOSS_REWARD_MAX_SEQ && i < 32; i++)
	{
		if ((m_activity_info.oga_kill_boss_reward_flag & (1 << i)) != 0)
		{
			continue;
		}

		const OGAKillBossReward *reward = LOGIC_CONFIG->GetOpenGameActivityCfg().GetKillBossReward(i);
		if (NULL == reward || reward->reward_item.item_id <= 0)
		{
			continue;
		}

		if (0 < reward->boss_seq_1 && reward->boss_seq_1 < OGA_BOSS_REWARD_MAX_SEQ)
		{
			if (0 == (m_activity_info.oga_kill_boss_flag & (static_cast<long long>(1) << reward->boss_seq_1)))
			{
				continue;
			}
		}

		if (0 < reward->boss_seq_2 && reward->boss_seq_2 < OGA_BOSS_REWARD_MAX_SEQ)
		{
			if (0 == (m_activity_info.oga_kill_boss_flag & (static_cast<long long>(1) << reward->boss_seq_2)))
			{
				continue;
			}
		}

		if (0 < reward->boss_seq_3 && reward->boss_seq_3 < OGA_BOSS_REWARD_MAX_SEQ)
		{
			if (0 == (m_activity_info.oga_kill_boss_flag & (static_cast<long long>(1) << reward->boss_seq_3)))
			{
				continue;
			}
		}

		if (0 < reward->boss_seq_4 && reward->boss_seq_4 < OGA_BOSS_REWARD_MAX_SEQ)
		{
			if (0 == (m_activity_info.oga_kill_boss_flag & (static_cast<long long>(1) << reward->boss_seq_4)))
			{
				continue;
			}
		}
		m_activity_info.oga_kill_boss_reward_flag |= (1 << i);

		// 邮件补发boss猎手奖励
		{
			static MailContentParam contentparam; contentparam.Reset();
			contentparam.AddItem(reward->reward_item.item_id, reward->reward_item.num, reward->reward_item.is_bind);

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_boss_killer_total_content);

			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		gamelog::g_log_roleactivity.printf(LL_INFO, "RACheckOpenServerGiveReweard user[%d, %s], level[%d], ra_type[%d], reward_flag[%d]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), ra_type, (int)m_activity_info.oga_kill_boss_reward_flag);
	}

}

bool RoleActivity::RACheckOpenServerUpgradeCompleteFlag(const RandActivityOpenServerRewardItemConfig *cfg)
{
	if (cfg == NULL)
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
			if ((mount_param->grade - 1) < cfg->cond2)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_WING:
	{
		WingParam *wing_param = m_role->GetWing()->GetWingParam();
		if (NULL != wing_param)
		{
			if ((wing_param->grade - 1) < cfg->cond2)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_SHIZHUANG_TYPE_BODY:
	{
		ShizhuangParam *shizhuang_param = m_role->GetShizhuang()->GetParam();
		if (NULL != shizhuang_param)
		{
			if ((shizhuang_param->item_list[SHIZHUANG_TYPE_BODY].grade - 1) < cfg->cond2)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_SHIZHUANG_TYPE_WUQI:
	{
		ShizhuangParam *shizhuang_param = m_role->GetShizhuang()->GetParam();
		if (NULL != shizhuang_param)
		{
			if ((shizhuang_param->item_list[SHIZHUANG_TYPE_WUQI].grade - 1) < cfg->cond2)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_FABAO:
	{
		FabaoParam *fabaoparam = m_role->GetFabaoManager()->GetParam();
		if (NULL != fabaoparam)
		{
			if ((fabaoparam->grade - 1) < cfg->cond2)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_FOOTPRINT:
	{
		FootprintParam *footprint_param = m_role->GetFootPrint()->GetFootprintParam();
		if (NULL != footprint_param)
		{
			if ((footprint_param->grade - 1) < cfg->cond2)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_HALO:
	{
		HaloParam *halo_param = m_role->GetHalo()->GetHaloParam();
		if (NULL != halo_param)
		{
			if ((halo_param->grade - 1) < cfg->cond2)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_QILINBI:
	{
		QiLinBiParam *qilinbi_param = m_role->GetQiLinBiManager()->GetQiLinBiParam();
		if (NULL != qilinbi_param)
		{
			if ((qilinbi_param->grade - 1) < cfg->cond2)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_SHENGONG:
	{
		ShengongParam *shengong_param = m_role->GetShengong()->GetShengongParam();
		if (NULL != shengong_param)
		{
			if ((shengong_param->grade - 1) < cfg->cond2)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_TOUSHI:
	{
		TouShiParam *toushi_param = m_role->GetTouShiManager()->GetTouShiParam();
		if (NULL != toushi_param)
		{
			if ((toushi_param->grade - 1) < cfg->cond2)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_YAOSHI:
	{
		YaoShiParam *yaoshi_param = m_role->GetYaoShiManager()->GetYaoShiParam();
		if (NULL != yaoshi_param)
		{
			if ((yaoshi_param->grade - 1) < cfg->cond2)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_LINGTONG:
	{
		if ((m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_2) - 1) < cfg->cond2)
		{
			return false;
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_LINGGONG:
	{
		if ((m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_3) - 1) < cfg->cond2)
		{
			return false;
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_LINGQI:
	{
		if ((m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_4) - 1) < cfg->cond2)
		{
			return false;
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_FIGHT_MOUNT:
	{
		FightMountParam *fight_mount_param = m_role->GetFightMountManager()->GetFightMountParam();
		if (NULL != fight_mount_param)
		{
			if ((fight_mount_param->grade - 1) < cfg->cond2)
			{
				return false;
			}
		}
	}
	break;
	}
	return true;
}

bool RoleActivity::RACheckOpenServerUpgradeGroupCompleteFlag(const RandActivityOpenServerRewardItemConfig *cfg)
{
	if (cfg == NULL)
	{
		return false;
	}

	switch (cfg->cond1)
	{
	case Protocol::UPGRADE_TYPE_MOUNT:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus::Instance().GetTotalUpgradeMountRecordList(cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_WING:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus::Instance().GetTotalUpgradeWingRecordList(cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_SHIZHUANG_TYPE_BODY:
	{
		if (cfg->cond2 < MAX_UPGRADE_LEVEL - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeShizhuangRecordList(cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_SHIZHUANG_TYPE_WUQI:
	{
		if (cfg->cond2 < MAX_UPGRADE_LEVEL - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeShenbingRecordList(cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_FABAO:
	{
		if (cfg->cond2 < MAX_UPGRADE_LEVEL - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeFabaoRecordList(cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_FOOTPRINT:
	{
		if (cfg->cond2 < MAX_UPGRADE_LEVEL - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeFootprintRecordList(cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_HALO:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus::Instance().GetTotalUpgradeHaloRecordList(cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_QILINBI:
	{
		if (cfg->cond2 < MAX_UPGRADE_LEVEL - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeQilinbiRecordList(cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_SHENGONG:
	{
		if (cfg->cond2 < MAX_UPGRADE_RECORD_COUNT - 1)
		{
			if (WorldStatus::Instance().GetTotalUpgradeShengongRecordList(cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_TOUSHI:
	{
		if (cfg->cond2 < MAX_UPGRADE_LEVEL - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeToushiRecordList(cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_YAOSHI:
	{
		if (cfg->cond2 < MAX_UPGRADE_LEVEL - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeYaoshiRecordList(cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_FIGHT_MOUNT:
	{
		if (cfg->cond2 < MAX_UPGRADE_LEVEL - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeFightMountRecordList(cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_LINGTONG:
	{
		if (cfg->cond2 < MAX_UPGRADE_LEVEL - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeRecordList(UPGRADE_TYPE_2, cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_LINGGONG:
	{
		if (cfg->cond2 < MAX_UPGRADE_LEVEL - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeRecordList(UPGRADE_TYPE_3, cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	case Protocol::UPGRADE_TYPE_LINGQI:
	{
		if (cfg->cond2 < MAX_UPGRADE_LEVEL - 1)
		{
			if (WorldStatus2::Instance().GetTotalUpgradeRecordList(UPGRADE_TYPE_4, cfg->cond2 + 1) < cfg->cond3)
			{
				return false;
			}
		}
	}
	break;
	}
	return true;
}

void RoleActivity::OnRAItemCollectionOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ITEM_COLLECTION))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	switch(opera_type)
	{
	case Protocol::RA_ITEM_COLLECTION_OPERA_TYPE_QUERY_INFO:
		{
			this->SendCollectExchangeInfo();
		}
		break;

	case Protocol::RA_ITEM_COLLECTION_OPERA_TYPE_EXCHANGE:
		{
			this->OnRAItemCollectionExchange(param1);
		}
		break;
	}
}

void RoleActivity::OnRAItemCollectionExchange(int seq)
{
	if (m_role->GetSublock()->CheckHasLock()) 
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const RandActivityItemCollectionRewardCfg::ConfigItem *reward_config = LOGIC_CONFIG->GetRandActivityCfg().GetItemCollectionRewardCfg(seq);
	if (NULL == reward_config) return;

	if (seq < 0 || seq >= RAND_ACTIVITY_ITEM_COLLECTION_REWARD_MAX_COUNT)
	{
		return;
	}

	if (m_activity_info.collection_exchange_times[seq] >= reward_config->exchange_times_limit)
	{
		m_role->NoticeNum(errornum::NT_CONVERTSHOP_CONVERT_COUNT_LIMIT);
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	short stuff_count = 0;
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

	for (int i = 0; i < reward_config->stuff_count && i < RandActivityItemCollectionRewardCfg::MAX_COLLECTION_STUFF_COUNT; i++)
	{
		if (reward_config->stuff_id_list[i].item_id > 0)
		{
			stuff_list[stuff_count].item_id = reward_config->stuff_id_list[i].item_id;
			stuff_list[stuff_count].num = reward_config->stuff_id_list[i].num;
			stuff_list[stuff_count].buyable = false;
			++ stuff_count;
		}
	}

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
	{
		return;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "RoleActivity::OnRAItemCollectionExchange"))
	{
		return;
	}

	int grid_index = -1;
	if (!m_role->GetKnapsack()->Put(reward_config->reward_item, PUT_REASON_ITEM_COLLECTION_REWARD, &grid_index))
	{
		return;
	}

	m_activity_info.collection_exchange_times[seq]++;
	this->SendCollectExchangeInfo();

	m_role->NoticeNum(noticenum::NT_CONVERTSHOP_CONVERT_SUCC);

	// 日志
	gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnRAItemCollectionOperaReq user[%d, %s], seq[%d], reward[%s]",
		m_role->GetUID(), m_role->GetName(), seq, ItemConfigDataLog(&reward_config->reward_item, NULL));
}

void RoleActivity::OnRaRefineOperaReq(int opera_type)
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_EXP_REFINE);

	switch (opera_type)
	{
	case Protocol::RA_EXP_REFINE_OPERA_TYPE_BUY_EXP:
		{
			this->OnRARefineExpReq();
		}
		break;

	case Protocol::RA_EXP_REFINE_OPERA_TYPE_FETCH_REWARD_GOLD:
		{
			this->OnRAFetchRewardGold();
		}
		break;

	case Protocol::RA_EXP_REFINE_OPERA_TYPE_GET_INFO:
		{
			this->SendRAExpRefineInfo();
		}
		break;
	}
}

void RoleActivity::OnRARefineExpReq()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EXP_REFINE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	const RandActivityExpRefineRewardCfg *exp_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityExpRefineRewardCfg(m_activity_info.refine_today_buy_time);
	if (NULL == exp_cfg)
	{
		m_role->NoticeNum(errornum::EN_EXP_REFINE_TODAY_TIME_MAX);
		return;
	}

	if (exp_cfg->consume_gold <= 0 || exp_cfg->reward_exp <= 0)
	{
		return;
	}

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(exp_cfg->consume_gold, "ExpRefine"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int old_exchange_times = m_activity_info.refine_today_buy_time;
	++ m_activity_info.refine_today_buy_time;

	// 给予
	m_role->AddExp(exp_cfg->reward_exp, "ExpRefine", Role::EXP_ADD_REASON_EXP_REFINE);
	m_activity_info.refine_reward_gold += exp_cfg->consume_gold;

	this->SendRAExpRefineInfo();

	gamelog::g_log_rand_activity.printf(LL_INFO, "ExpRefine user %d %s, old_exchange_times %d, consume_gold %d reward_exp %lld",
		m_role->GetUID(), m_role->GetName(), old_exchange_times, exp_cfg->consume_gold, exp_cfg->reward_exp);
}

void RoleActivity::OnRAFetchRewardGold()
{
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EXP_REFINE))
	{
		return;
	}

	int reward_gold = m_activity_info.refine_reward_gold;
	if (0 >= reward_gold)
	{
		return;
	}

	m_role->GetKnapsack()->GetMoney()->AddGold(reward_gold, "ExpRefine");
	m_activity_info.refine_reward_gold = 0;

	this->SendRAExpRefineInfo();

	gamelog::g_log_rand_activity.printf(LL_INFO, "ExpRefine user %d %s, fetch_reward_gold_bind %d", m_role->GetUID(), m_role->GetName(),reward_gold);
}

void RoleActivity::SendRAExpRefineInfo()
{
	Protocol::SCRAExpRefineInfo efi;

	efi.refine_today_buy_time = m_activity_info.refine_today_buy_time;
	efi.reserve_ch = 0;
	efi.refine_reward_gold = m_activity_info.refine_reward_gold;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&efi, sizeof(efi));
}

void RoleActivity::OnRARedEnvelopeGiftConsumeGold(int consume_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_RED_ENVELOPE_GIFT))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_RED_ENVELOPE_GIFT);

	// 获取当前天
	int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_RED_ENVELOPE_GIFT), EngineAdapter::Instance().Time());
	if (day_index >= 0 && day_index < RAND_ACTIVITY_HONGBAO_HAOLI_MAX_COUNT)
	{
		m_activity_info.ra_red_envelope_gift_consume_gold_num_list[day_index] += consume_gold;
	}
	this->SendRARedEnvelopeGiftInfo();
}

void RoleActivity::FetchRedEnvelopeGiftDiamond(int day_index)
{	
	// 活动还在开启中，不可以领
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_RED_ENVELOPE_GIFT))
	{
		return;
	}

	if (day_index < 0 || day_index >= RAND_ACTIVITY_HONGBAO_HAOLI_MAX_COUNT)
	{
		return;
	}

	if (0 != (m_activity_info.ra_red_envelope_gift_reward_flag & (1 << day_index)))
	{
		m_role->NoticeNum(errornum::EN_CHARGE_REPAYMENT_HAS_FETCHED);
		return;
	}

	m_activity_info.ra_red_envelope_gift_reward_flag |= (1 << day_index);

	// 发放奖励
	const RandActivityRedEnvelopeGiftCfg &red_envelope_gift_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRedEnvelopeGiftCfg();
	m_role->GetKnapsack()->GetMoney()->AddGold(static_cast<int>(m_activity_info.ra_red_envelope_gift_consume_gold_num_list[day_index] * red_envelope_gift_cfg.percent / 10000), "FetchRedEnvelopeGiftDiamond");

	this->SendRARedEnvelopeGiftInfo();
}

void RoleActivity::SendRARedEnvelopeGiftInfo()
{
	Protocol::SCRARedEnvelopeGiftInfo pro;
	for (int i = 0; i < RAND_ACTIVITY_HONGBAO_HAOLI_MAX_COUNT; i++)
	{
		pro.consume_gold_num_list[i] = m_activity_info.ra_red_envelope_gift_consume_gold_num_list[i];
	}
	pro.reward_flag = m_activity_info.ra_red_envelope_gift_reward_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void RoleActivity::OnRAMarryMeOperaReq(int opera_type)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MARRY_ME))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MARRY_ME);

	switch (opera_type)
	{
	case Protocol::RA_MARRYME_REQ_INFO:
		{
			this->SendRAMarryMeAllInfo();
		}
		break;

	default:
		break;
	}
}

void RoleActivity::SendRAMarryMeAllInfo()
{
	static Protocol::SCRAMarryMeAllInfo ramm;

	RandActivityMarryMe* activity = (RandActivityMarryMe*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_MARRY_ME);
	if (NULL != activity)
	{
		CoupleInfo couple_list[RA_MARRY_SHOW_COUPLE_COUNT_MAX];

		int count = activity->GetCoupleInfoList(couple_list);
		
		int real_cout = 0;
		for (int index = 0; index < count && index < RA_MARRY_SHOW_COUPLE_COUNT_MAX; ++ index)
		{
			ramm.counp_list[index].propose_id = couple_list[index].propose_id;
			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(couple_list[index].propose_id, false);
			if (NULL != user_node)
			{
				user_node->GetName(ramm.counp_list[index].propose_name);

				ramm.counp_list[index].proposer_sex = user_node->sex;
			}

			ramm.counp_list[index].accept_proposal_id = couple_list[index].accept_proposal_id;
			user_node = UserCacheManager::Instance().GetUserNode(couple_list[index].accept_proposal_id, false);
			if (NULL != user_node)
			{
				user_node->GetName(ramm.counp_list[index].accept_proposal_name);

				ramm.counp_list[index].accept_proposal_sex = user_node->sex;
			}

			real_cout ++;
		}

		ramm.cur_couple_count = real_cout;
		int send_len = sizeof(ramm) - sizeof(ramm.counp_list[0]) * (RA_MARRY_SHOW_COUPLE_COUNT_MAX - real_cout);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ramm, send_len);
	}
}

void RoleActivity::OnMarry(Role *target)
{
	if (NULL == target)
	{
		return;
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MARRY_ME))
	{
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MARRY_ME);

		RandActivityMarryMe* activity = (RandActivityMarryMe*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_MARRY_ME);
		if (NULL != activity)
		{
			const ItemConfigData &reward_item = LOGIC_CONFIG->GetRandActivityCfg().GetMarryMeRewardItem();
			if (!activity->IsRewardCountLimit())
			{
				if (activity->OnRoleAcceptProposal(target, m_role))
				{
					const ItemBase *item_base = ITEMPOOL->GetItem(reward_item.item_id);
					if (NULL != item_base)
					{
						static MailContentParam contentparam; contentparam.Reset();
						contentparam.item_list[0].item_id = reward_item.item_id;
						contentparam.item_list[0].num = reward_item.num;
						contentparam.item_list[0].is_bind = (reward_item.is_bind) ? 1 : 0;
						contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

						int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_marry_me, reward_item.item_id);
						if (length > 0)
						{
							if (m_activity_info.ra_marry_me_is_fetch_reward == 0)										//智扬需求，一个玩家在同一次活动只能获取一次奖励
							{
								MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

								m_activity_info.ra_marry_me_is_fetch_reward = 1;
							}
							
							if (target->GetRoleActivity()->GetParam()->ra_marry_me_is_fetch_reward == 0)
							{
								MailRoute::MailToUser(target->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

								target->GetRoleActivity()->GetParam()->ra_marry_me_is_fetch_reward = 1;
							}
						}
					}
				}
			}
		}
	}
}

void RoleActivity::AddTurntableScore(long long consume_gold)
{
	const GoldTurntableOtherCfg *cfg = LOGIC_CONFIG->GetGoldTurntableConfig().GetOtherCfg();
	if (NULL == cfg || consume_gold <= 0)
	{
		return;
	}

	m_activity_info.gold_turntable_score += static_cast<int>(consume_gold);
	WorldStatus::Instance().AddGoldTurntablePool(cfg->per_transform * consume_gold / 100);

	if (cfg->sigle_cj_gold > 0 || m_activity_info.gold_turntable_score / cfg->sigle_cj_gold > 0)
	{
		this->SendGoldTurntableInfo();
	}
}

void RoleActivity::SendGoldTurntableInfo()
{
	Protocol::SCGoldTurntableInfo pro;
	pro.gold_num = WorldStatus::Instance().GetGoldTurntablePool();
	const GoldTurntableOtherCfg *cfg = LOGIC_CONFIG->GetGoldTurntableConfig().GetOtherCfg();
	if (pro.gold_num < cfg->min_pool)
	{
		WorldStatus::Instance().SetGoldTurntablePool(cfg->min_pool);
		pro.gold_num = cfg->min_pool;
	}

	pro.chou_jiang_times = 0;
	if (cfg->sigle_cj_gold > 0)
	{
		pro.chou_jiang_times = m_activity_info.gold_turntable_score / cfg->sigle_cj_gold;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void RoleActivity::GoldTurntableChouJiang()
{
	const GoldTurntableOtherCfg *cfg = LOGIC_CONFIG->GetGoldTurntableConfig().GetOtherCfg();
	if (NULL == cfg) return;

	if (m_activity_info.gold_turntable_score < cfg->sigle_cj_gold || m_activity_info.gold_turntable_score <= 0) return;

	m_activity_info.gold_turntable_score -= cfg->sigle_cj_gold;

	const GoldTurntableRewardPoolCfg *reward = LOGIC_CONFIG->GetGoldTurntableConfig().RandRewardCfg();
	if (NULL != reward)
	{
		long long add_gold = reward->rewrad_gold;
		add_gold += reward->jianchi_per * WorldStatus::Instance().GetGoldTurntablePool() / 100;
		if (add_gold > 0)
		{
			m_role->GetKnapsack()->GetMoney()->AddGold(add_gold, "ZhuanPan");

			if (0 != reward->broadcast)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_gold_turntable_notice,
					m_role->GetUID(), m_role->GetName(), m_role->GetCamp());
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
				}
			}
		}

		WorldStatus::Instance().AddGoldTurntablePool(-add_gold);
		if (WorldStatus::Instance().GetGoldTurntablePool() < cfg->min_pool)
		{
			WorldStatus::Instance().SetGoldTurntablePool(cfg->min_pool);
		}

		if (0 != reward->item.item_id)
		{
			m_role->GetKnapsack()->PutOrMail(reward->item, PUT_REASON_YUANBAO_ZHUANPAN);
		}

		Protocol::SCGoldTurntableReward pro;
		pro.index = reward->index;
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));

		this->SendGoldTurntableInfo();
	}
}

// 连充特惠初-------------------------
void RoleActivity::OnRAContinueChongzhiOperaReqChu(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU);

	switch (opera_type)
	{
	case Protocol::RA_CONTINUE_CHONGZHI_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRAContinueChongzhiActivityInfoChu();
	}
	break;

	case Protocol::RA_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_REWARD:
	{
		this->OnRAContinueChongzhiFetchRewardChu(param1);
	}
	break;

	case Protocol::RA_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_EXTRA_REWARD:
	{
		this->OnRAContinueChongzhiChuFetchExtraReward();
	}
	break;

	}
}

void RoleActivity::OnRAContinueChongzhiAddChongzhiChu(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU);

	m_activity_info.ra_continue_chongzhi_today_chongzhi_chu += (unsigned int)chongzhi;

	int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU), EngineAdapter::Instance().Time()) + 1;
	if (m_activity_info.ra_continue_chongzhi_days_chu >= day_index || 1 == m_activity_info.ra_is_lei_chongzhi_chu)
	{
		this->SendRAContinueChongzhiActivityInfoChu();
		return;
	}

	const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindexChu(m_activity_info.ra_continue_chongzhi_days_chu + 1, true, false);
	if (NULL == continue_chongzhi_info_cfg)
	{
		return;
	}

	if (m_activity_info.ra_continue_chongzhi_today_chongzhi_chu >= continue_chongzhi_info_cfg->need_chongzhi)
	{
		m_activity_info.ra_continue_chongzhi_days_chu++;
		m_activity_info.ra_is_lei_chongzhi_chu = 1;

		// 激活每日充值达标奖励
		m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_chu |= (1 << m_activity_info.ra_continue_chongzhi_days_chu);
	}

	this->SendRAContinueChongzhiActivityInfoChu();
}

void RoleActivity::SendRAContinueChongzhiActivityInfoChu()
{
	static Protocol::SCRAContinueChongzhiInfoChu cczi;
	cczi.today_chongzhi = m_activity_info.ra_continue_chongzhi_today_chongzhi_chu;
	cczi.can_fetch_reward_flag = m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_chu;
	cczi.has_fetch_reward_falg = m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_chu;
	cczi.continue_chongzhi_days = m_activity_info.ra_continue_chongzhi_days_chu;
	cczi.reserve1 = 0;
	cczi.reserve2 = 0;

	unsigned int send_len = sizeof(cczi);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cczi, send_len);
}

void RoleActivity::OnRAContinueChongzhiFetchRewardChu(int day_index)
{
	if (day_index <= 0 || day_index > static_cast<int>(m_activity_info.ra_continue_chongzhi_days_chu))
	{
		return;
	}

	if (0 == (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_chu & (1 << day_index)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_NOT_ACTIVE);
		return;
	}

	if (0 != (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_chu & (1 << day_index)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_HAS_FETCH);
		return;
	}

	const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindexChu(day_index, false, false);
	if (NULL == continue_chongzhi_info_cfg)
	{
		return;
	}

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_chongzhi_info_cfg->reward_item.item_id);
	if (NULL == reward_item_base || reward_item_base->GetPileLimit() <= 0)
	{
		return;
	}

	if (reward_item_base->GetPileLimit() <= 0) return;

	int need_empty_slot_num = 1;
	need_empty_slot_num = continue_chongzhi_info_cfg->reward_item.num / reward_item_base->GetPileLimit();
	if (continue_chongzhi_info_cfg->reward_item.num % reward_item_base->GetPileLimit() > 0)
	{
		++need_empty_slot_num;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_empty_slot_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(continue_chongzhi_info_cfg->reward_item, PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD_CHU))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_chu |= (1 << day_index);

	this->SendRAContinueChongzhiActivityInfoChu();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAContinueChongzhiFetchRewardChu::user[%d, %s], index[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), day_index, ItemConfigDataLog(&continue_chongzhi_info_cfg->reward_item, NULL));
}

void RoleActivity::OnRAContinueChongzhiChuFetchExtraReward()
{
	if (0 == (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_chu & (1 << 0)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_NOT_ACTIVE);
		return;
	}

	if (0 != (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_chu & (1 << 0)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_HAS_FETCH);
		return;
	}

	const ItemConfigData & item_data = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChonghziChuExtraReward();

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(item_data.item_id);
	if (NULL == reward_item_base || reward_item_base->GetPileLimit() <= 0)
	{
		return;
	}

	if (reward_item_base->GetPileLimit() <= 0) return;

	int need_empty_slot_num = 1;
	need_empty_slot_num = item_data.num / reward_item_base->GetPileLimit();
	if (item_data.num % reward_item_base->GetPileLimit() > 0)
	{
		++need_empty_slot_num;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_empty_slot_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(item_data, PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD_CHU))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_chu |= (1 << 0);	// 将特殊奖励领取标记置0

	this->SendRAContinueChongzhiActivityInfoChu();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAContinueChongzhiChuFetchExtraReward::user[%d, %s], index[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), 0, ItemConfigDataLog(&item_data, NULL));
}

// 连充特惠高-----------------------------------
void RoleActivity::OnRAContinueChongzhiOperaReqGao(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO);

	switch (opera_type)
	{
	case Protocol::RA_CONTINUE_CHONGZHI_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRAContinueChongzhiActivityInfoGao();
	}
	break;

	case Protocol::RA_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_REWARD:
	{
		this->OnRAContinueChongzhiFetchRewardGao(param1);
	}
	break;

	case Protocol::RA_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_EXTRA_REWARD:
	{
		this->OnRAContinueChongzhiGaoFetchExtraReward();
	}
	break;
	}
}

void RoleActivity::OnRAContinueChongzhiAddChongzhiGao(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO);

	m_activity_info.ra_continue_chongzhi_today_chongzhi_gao += (unsigned int)chongzhi;

	int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO), EngineAdapter::Instance().Time()) + 1;
	if (m_activity_info.ra_continue_chongzhi_days_gao >= day_index || 1 == m_activity_info.ra_is_lei_chongzhi_gao)
	{
		this->SendRAContinueChongzhiActivityInfoGao();
		return;
	}

	const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindexGao(m_activity_info.ra_continue_chongzhi_days_gao + 1, true, false);
	if (NULL == continue_chongzhi_info_cfg)
	{
		return;
	}

	if (m_activity_info.ra_continue_chongzhi_today_chongzhi_gao >= continue_chongzhi_info_cfg->need_chongzhi)
	{
		m_activity_info.ra_continue_chongzhi_days_gao++;
		m_activity_info.ra_is_lei_chongzhi_gao = 1;

		// 激活每日充值达标奖励
		m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_gao |= (1 << m_activity_info.ra_continue_chongzhi_days_gao);
	}

	this->SendRAContinueChongzhiActivityInfoGao();
}

void RoleActivity::SendRAContinueChongzhiActivityInfoGao()
{
	static Protocol::SCRAContinueChongzhiInfoGao cczi;
	cczi.today_chongzhi = m_activity_info.ra_continue_chongzhi_today_chongzhi_gao;
	cczi.can_fetch_reward_flag = m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_gao;
	cczi.has_fetch_reward_falg = m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_gao;
	cczi.continue_chongzhi_days = m_activity_info.ra_continue_chongzhi_days_gao;
	cczi.reserve1 = 0;
	cczi.reserve2 = 0;

	unsigned int send_len = sizeof(cczi);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cczi, send_len);
}

void RoleActivity::OnRAContinueChongzhiFetchRewardGao(int day_index)
{
	if (day_index <= 0 || day_index > static_cast<int>(m_activity_info.ra_continue_chongzhi_days_gao))
	{
		return;
	}

	if (0 == (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_gao & (1 << day_index)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_NOT_ACTIVE);
		return;
	}

	if (0 != (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_gao & (1 << day_index)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_HAS_FETCH);
		return;
	}

	const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindexGao(day_index, false, false);
	if (NULL == continue_chongzhi_info_cfg)
	{
		return;
	}

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_chongzhi_info_cfg->reward_item.item_id);
	if (NULL == reward_item_base || reward_item_base->GetPileLimit() <= 0)
	{
		return;
	}

	if (reward_item_base->GetPileLimit() <= 0) return;

	int need_empty_slot_num = 1;
	need_empty_slot_num = continue_chongzhi_info_cfg->reward_item.num / reward_item_base->GetPileLimit();
	if (continue_chongzhi_info_cfg->reward_item.num % reward_item_base->GetPileLimit() > 0)
	{
		++need_empty_slot_num;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_empty_slot_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(continue_chongzhi_info_cfg->reward_item, PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD_GAO))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_gao |= (1 << day_index);

	this->SendRAContinueChongzhiActivityInfoGao();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAContinueChongzhiFetchRewardGao::user[%d, %s], index[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), day_index, ItemConfigDataLog(&continue_chongzhi_info_cfg->reward_item, NULL));
}

void RoleActivity::OnRAContinueChongzhiGaoFetchExtraReward()
{
	if (0 == (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_gao & (1 << 0)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_NOT_ACTIVE);
		return;
	}

	if (0 != (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_gao & (1 << 0)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_HAS_FETCH);
		return;
	}

	const ItemConfigData & item_data = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChonghziGaoExtraReward();

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(item_data.item_id);
	if (NULL == reward_item_base || reward_item_base->GetPileLimit() <= 0)
	{
		return;
	}

	if (reward_item_base->GetPileLimit() <= 0) return;

	int need_empty_slot_num = 1;
	need_empty_slot_num = item_data.num / reward_item_base->GetPileLimit();
	if (item_data.num % reward_item_base->GetPileLimit() > 0)
	{
		++need_empty_slot_num;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_empty_slot_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(item_data, PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD_GAO))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_gao |= (1 << 0);	// 将特殊奖励领取标记置0

	this->SendRAContinueChongzhiActivityInfoGao();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAContinueChongzhiGaoFetchExtraReward::user[%d, %s], index[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), 0, ItemConfigDataLog(&item_data, NULL));
}

// 限时连充-----------------------------------
void RoleActivity::OnRAContinueChongzhiOperaReq2(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2);

	switch (opera_type)
	{
	case Protocol::RA_CONTINUE_CHONGZHI_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRAContinueChongzhiActivityInfo2();
	}
	break;

	case Protocol::RA_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_REWARD:
	{
		this->OnRAContinueChongzhiFetchReward2(param1);
	}
	break;

	case Protocol::RA_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_EXTRA_REWARD:
	{
		this->OnRAContinueChongzhi2FetchExtraReward();
	}
	break;
	}
}

void RoleActivity::OnRAContinueChongzhiAddChongzhi2(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2);

	m_activity_info.ra_continue_chongzhi_today_chongzhi_2 += (unsigned int)chongzhi;

	int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2), EngineAdapter::Instance().Time()) + 1;
	if (m_activity_info.ra_continue_chongzhi_days_2 >= day_index || 1 == m_activity_info.ra_continue_chongzhi_2_is_lei_chongzhi)
	{
		this->SendRAContinueChongzhiActivityInfo2();
		return;
	}

	const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindex2(day_index, true, false);
	if (NULL == continue_chongzhi_info_cfg)
	{
		return;
	}

	if (m_activity_info.ra_continue_chongzhi_today_chongzhi_2 >= continue_chongzhi_info_cfg->need_chongzhi)
	{
		m_activity_info.ra_continue_chongzhi_days_2 ++;
		m_activity_info.ra_continue_chongzhi_2_is_lei_chongzhi = 1;

		// 激活特殊奖励
		if (m_activity_info.ra_continue_chongzhi_days_2 == continue_chongzhi_info_cfg->fetch_extra_reward_need_days)
		{
			m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_2 |= (1 << 0);
		}

		// 激活每日充值达标奖励
		m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_2 |= (1 << m_activity_info.ra_continue_chongzhi_days_2);
	}

	this->SendRAContinueChongzhiActivityInfo2();
}

void RoleActivity::SendRAContinueChongzhiActivityInfo2()
{
	static Protocol::SCRAContinueChongzhiInfo2 cczi;
	cczi.today_chongzhi = m_activity_info.ra_continue_chongzhi_today_chongzhi_2;
	cczi.can_fetch_reward_flag = m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_2;
	cczi.has_fetch_reward_falg = m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_2;
	cczi.continue_chongzhi_days = m_activity_info.ra_continue_chongzhi_days_2;
	cczi.reserve1 = 0;
	cczi.reserve2 = 0;

	unsigned int send_len = sizeof(cczi);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cczi, send_len);
}

void RoleActivity::OnRAContinueChongzhiFetchReward2(int day_index)
{
	if (day_index <= 0 || day_index > static_cast<int>(m_activity_info.ra_continue_chongzhi_days_2))
	{
		return;
	}

	if (0 == (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_2 & (1 << day_index)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_NOT_ACTIVE);
		return;
	}

	if (0 != (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_2 & (1 << day_index)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_HAS_FETCH);
		return;
	}

	const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindex2(day_index, false, false);
	if (NULL == continue_chongzhi_info_cfg)
	{
		return;
	}

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_chongzhi_info_cfg->reward_item.item_id);
	if (NULL == reward_item_base || reward_item_base->GetPileLimit() <= 0)
	{
		return;
	}

	if (reward_item_base->GetPileLimit() <= 0) return;

	int need_empty_slot_num = 1;
	need_empty_slot_num = continue_chongzhi_info_cfg->reward_item.num / reward_item_base->GetPileLimit();
	if (continue_chongzhi_info_cfg->reward_item.num % reward_item_base->GetPileLimit() > 0)
	{
		++need_empty_slot_num;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_empty_slot_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(continue_chongzhi_info_cfg->reward_item, PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD_2))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_2 |= (1 << day_index);

	this->SendRAContinueChongzhiActivityInfo2();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAContinueChongzhiFetchReward2::user[%d, %s], index[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), day_index, ItemConfigDataLog(&continue_chongzhi_info_cfg->reward_item, NULL));
}

void RoleActivity::OnRAContinueChongzhi2FetchExtraReward()
{
	if (0 == (m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_2 & (1 << 0)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_NOT_ACTIVE);
		return;
	}

	if (0 != (m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_2 & (1 << 0)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_HAS_FETCH);
		return;
	}

	const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetContinueChongzhiRewardInfoWithdayindex2(1, false, false);
	if (NULL == continue_chongzhi_info_cfg)
	{
		return;
	}

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_chongzhi_info_cfg->extra_reward.item_id);
	if (NULL == reward_item_base || reward_item_base->GetPileLimit() <= 0)
	{
		return;
	}

	if (reward_item_base->GetPileLimit() <= 0) return;

	int need_empty_slot_num = 1;
	need_empty_slot_num = continue_chongzhi_info_cfg->extra_reward.num / reward_item_base->GetPileLimit();
	if (continue_chongzhi_info_cfg->extra_reward.num % reward_item_base->GetPileLimit() > 0)
	{
		++need_empty_slot_num;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_empty_slot_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(continue_chongzhi_info_cfg->extra_reward, PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD_2))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_2 |= (1 << 0);	// 将特殊奖励领取标记置0

	this->SendRAContinueChongzhiActivityInfo2();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAContinueChongzhi2FetchExtraReward::user[%d, %s], index[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), 0, ItemConfigDataLog(&continue_chongzhi_info_cfg->extra_reward, NULL));
}

void RoleActivity::OnRAGoldenPigOperateReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_GOLDEN_PIG))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	switch (opera_type)
	{
	case Protocol::CSGoldenPigOperateReq::GOLDEN_PIG_OPERATE_TYPE_REQ_INFO:
		this->SendRAGoldenPigSummonInfo();
		break;

	case Protocol::CSGoldenPigOperateReq::GOLDEN_PIG_OPERATE_TYPE_SUMMON:
		this->OnRASummonGoldenPigReq(param);
		break;

	default:
		break;
	}
}

void RoleActivity::SendRAGoldenPigSummonInfo()
{
	RandActivityGoldenPig *activity = (RandActivityGoldenPig *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_GOLDEN_PIG);
	if (NULL == activity)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	const RandActivityGoldenPigOtherCfg *other_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAGoldenPigOtherCfg();
	if (NULL == other_cfg || other_cfg->gold_consume <= 0)
	{
		return;
	}

	Protocol::SCGoldenPigSendInfo golden_pig_info;
	golden_pig_info.summon_credit = m_activity_info.ra_golden_pig_summon_credit / other_cfg->gold_consume;
	golden_pig_info.current_chongzhi = m_activity_info.ra_golden_pig_chongzhi;
	golden_pig_info.activity_end_time = activity->GetEndTime();

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&golden_pig_info, sizeof(golden_pig_info));
}

void RoleActivity::RAGoldenPigSendBossState()
{
	RandActivityGoldenPig *activity = (RandActivityGoldenPig *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_GOLDEN_PIG);
	if (NULL == activity)
	{
		return;
	}

	Protocol::SCGoldenPigBossState golden_pig_boss_state;
	golden_pig_boss_state.boss_state[Protocol::CSGoldenPigOperateReq::GOLDEN_PIG_SUMMON_TYPE_JUNIOR] = 
		activity->GetGoldenPigBossState(Protocol::CSGoldenPigOperateReq::GOLDEN_PIG_SUMMON_TYPE_JUNIOR);
	golden_pig_boss_state.boss_state[Protocol::CSGoldenPigOperateReq::GOLDEN_PIG_SUMMON_TYPE_MEDIUM] =
		activity->GetGoldenPigBossState(Protocol::CSGoldenPigOperateReq::GOLDEN_PIG_SUMMON_TYPE_MEDIUM);
	golden_pig_boss_state.boss_state[Protocol::CSGoldenPigOperateReq::GOLDEN_PIG_SUMMON_TYPE_SENIOR] =
		activity->GetGoldenPigBossState(Protocol::CSGoldenPigOperateReq::GOLDEN_PIG_SUMMON_TYPE_SENIOR);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&golden_pig_boss_state, sizeof(golden_pig_boss_state));
}

void RoleActivity::OnRASummonGoldenPigReq(int param)
{
	if (param < Protocol::CSGoldenPigOperateReq::GOLDEN_PIG_SUMMON_TYPE_JUNIOR || param > Protocol::CSGoldenPigOperateReq::GOLDEN_PIG_SUMMON_TYPE_SENIOR)
	{
		return;
	}

	const RandActivityGoldenPigOtherCfg *other_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAGoldenPigOtherCfg();
	if (NULL == other_cfg)
	{
		return;
	}

	if (m_activity_info.ra_golden_pig_summon_credit < other_cfg->gold_consume * other_cfg->summon_cfg_list[param].summon_consume)
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	RandActivityGoldenPig *activity = (RandActivityGoldenPig *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_GOLDEN_PIG);
	if (NULL == activity)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	if (RandActivityGoldenPig::GOLDEN_PIG_STATE_EXIST == activity->GetGoldenPigBossState(param))
	{
		m_role->NoticeNum(errornum::EN_GOLDEN_PIG_EXIST);
		return;
	}

	// 消耗
	m_activity_info.ra_golden_pig_summon_credit -= other_cfg->gold_consume * other_cfg->summon_cfg_list[param].summon_consume;

	// 给与
	int boss_id = -1;
	ItemConfigData *item = LOGIC_CONFIG->GetRandActivityCfg().GetRAGoldenPigRewardCfg(param, RandActivityGoldenPigRewardCfg::GOLDEN_PIG_REWARD_TYPE_SUMMON, boss_id);

	if (NULL != item && MONSTERPOOL->IsMonsterExist(boss_id))
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg->scene_id, 0);
		if (NULL != scene)
		{
			Monster *monster = MONSTERPOOL->CreateMonster(boss_id, scene, other_cfg->summon_cfg_list[param].summon_pos);
			if (NULL != monster)
			{
				activity->SetGoldenPigBossSummonTime(param, static_cast<unsigned int>(EngineAdapter::Instance().Time()));
				activity->SetGoldenPigBossState(param, RandActivityGoldenPig::GOLDEN_PIG_STATE_EXIST);
				activity->SetGoldenPigBossObjID(param, monster->GetId());

				static MailContentParam contentparam;
				contentparam.Reset();
				int item_count = 0;

				contentparam.item_list[item_count].item_id = item->item_id;
				contentparam.item_list[item_count].num = item->num;
				contentparam.item_list[item_count].is_bind = (item->is_bind) ? 1 : 0;

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_summon_golden_pig_reward_content, boss_id);

				if (length > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}

				length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_summon_golden_pig,
					m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), boss_id, param);

				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
				}

				this->SendRAGoldenPigSummonInfo();
				World::GetInstWorld()->GetSceneManager()->OnRAGoldenPigSendBossState();
			}
		}
	}
}

void RoleActivity::OnKillGoldenPigBoss(int summon_type)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_GOLDEN_PIG))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	const RandActivityGoldenPigOtherCfg *other_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAGoldenPigOtherCfg();
	if (NULL == other_cfg)
	{
		return;
	}

	int boss_id = -1;
	ItemConfigData *item = LOGIC_CONFIG->GetRandActivityCfg().GetRAGoldenPigRewardCfg(summon_type, RandActivityGoldenPigRewardCfg::GOLDEN_PIG_REWARD_TYPE_KILLER, boss_id);

	if (NULL == item || boss_id == -1)
	{
		return;
	}

	RandActivityGoldenPig *activity = (RandActivityGoldenPig *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_GOLDEN_PIG);
	if (NULL == activity)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	activity->SetGoldenPigBossSummonTime(summon_type, 0);
	activity->SetGoldenPigBossState(summon_type, RandActivityGoldenPig::GOLDEN_PIG_STATE_NOT_EXIST);

	static MailContentParam contentparam;
	contentparam.Reset();
	int item_count = 0;

	contentparam.item_list[item_count].item_id = item->item_id;
	contentparam.item_list[item_count].num = item->num;
	contentparam.item_list[item_count].is_bind = (item->is_bind) ? 1 : 0;

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_kill_golden_reward_content, boss_id);

	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
	}

	World::GetInstWorld()->GetSceneManager()->OnRAGoldenPigSendBossState();
}

void RoleActivity::OnRAOpenServerInvestOperateReq(int opera_type, int param1, int param2)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_OPEN_SERVER_INVEST))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	switch (opera_type)
	{
	case Protocol::RA_OPEN_SERVER_INVEST_TYPE_INFO_REQ:
		this->SendRAOpenServerInvestInfo();
		break;
	case Protocol::RA_OPEN_SERVER_INVEST_TYPE_INVEST:
		this->OnRAInvest(param1);
		break;
	case Protocol::RA_OPEN_SERVER_INVEST_TYPE_FECTH_REWARD:
		this->OnRAOpenServerInvestFetchReward(param1, param2);
		break;
	default:
		break;
	}
}

void RoleActivity::SendRAOpenServerInvestInfo()
{
	Protocol::SCOpenServerInvestInfo invest_info;
	invest_info.reward_flag = m_activity_info.ra_open_server_invest_reward_flag;

	RandActivity *activty = RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_OPEN_SERVER_INVEST);
	if (NULL == activty)
	{
		return;
	}

	for (int i = 0; i < RandActivityOpenServerInvestOtherCfg::MAX_INVEST_TYPE; ++i)
	{
		const RandActivityOpenServerInvestOtherCfg *other_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAServerInvestOtherCfg(i);
		if (NULL != other_cfg)
		{
			invest_info.time_limit[i] = other_cfg->invest_limit_time * 60 * 60 + activty->GetBeginTime();
		}

		invest_info.finish_param[i] = m_activity_info.ra_open_server_invest_reward_param[i];
	}

	invest_info.finish_param[RandActivityOpenServerInvestOtherCfg::INVEST_TPYE_BIPIN]++;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&invest_info, sizeof(invest_info));
}

void RoleActivity::OnRAInvest(int type)
{
	const RandActivityOpenServerInvestOtherCfg *other_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAServerInvestOtherCfg(type);
	if (NULL == other_cfg)
	{
		return;
	}

	if (0 != (m_activity_info.ra_open_server_invest_reward_flag & (1 << (type * 8))))
	{
		m_role->NoticeNum(errornum::EN_OPEN_SERVER_INVEST_HAD_INVEST);
		return;
	}

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg->consume, "OpenserverInvest"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 给予
	m_activity_info.ra_open_server_invest_reward_flag |= (1 << type * 8);

	{
		// 传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_openserver_invest,
			m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), type);

		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	m_role->GetKnapsack()->PutOrMail(other_cfg->reward_item, PUT_REASON_OPEN_SERVER_INVEST);

	this->AddOpenServerInvestFinishParam(RandActivityOpenServerInvestOtherCfg::INVEST_TYPE_ACTIVE, m_role->GetActiveDegree()->GetTotalDegree());

	this->SendRAOpenServerInvestInfo();
}

void RoleActivity::OnRAOpenServerInvestFetchReward(int type, int seq)
{
	const RandActivityOpenServerInvestRewardCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAServerInvestRewardCfg(type, seq);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (0 != (m_activity_info.ra_open_server_invest_reward_flag & (1 << (type * 8 + seq + 1))))
	{
		m_role->NoticeNum(errornum::EN_OPEN_SERVER_INVEST_REWARD);
		return;
	}

	if (seq > 0 && 0 == (m_activity_info.ra_open_server_invest_reward_flag & (1 << (type * 8 + seq))))
	{
		m_role->NoticeNum(errornum::EN_OPEN_SERVER_INVEST_LAST_REWARD);
		return;
	}

	if (seq == 0 && 0 == (m_activity_info.ra_open_server_invest_reward_flag & (1 << (type * 8))))
	{
		m_role->NoticeNum(errornum::EN_OPEN_SERVER_INVEST_NOT_INVEST);
		return;
	}

	// 检查限制
	if (static_cast<int>(m_activity_info.ra_open_server_invest_reward_param[type]) < reward_cfg->param && 
			RandActivityOpenServerInvestOtherCfg::INVEST_TPYE_BIPIN != type)
	{
		m_role->NoticeNum(errornum::EN_OPEN_SERVER_INVEST_UNFINISH);
		return;
	}

	if (static_cast<int>(m_activity_info.ra_open_server_invest_reward_param[type]) + 1 < reward_cfg->param &&
		RandActivityOpenServerInvestOtherCfg::INVEST_TPYE_BIPIN == type)
	{
		m_role->NoticeNum(errornum::EN_OPEN_SERVER_INVEST_UNFINISH);
		return;
	}

	// 给予
	m_activity_info.ra_open_server_invest_reward_flag |= (1 << (type * 8 + seq + 1));
	if (NULL != reward_cfg->reward_item_list)
	{
		m_role->GetKnapsack()->PutListOrMail(reward_cfg->reward_item_list, PUT_REASON_OPEN_SERVER_INVEST);
	}

	{
		// 传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_openserver_invest_reward,
			m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), type);

		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	this->SendRAOpenServerInvestInfo();
}

void RoleActivity::AddOpenServerInvestFinishParam(int type, int active_score, bool err_notice)
{
	if (err_notice && !ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_OPEN_SERVER_INVEST))
	{
		//m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	const RandActivityOpenServerInvestOtherCfg *other_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAServerInvestOtherCfg(type);
	if (NULL == other_cfg)
	{
		return;
	}

	if (type < RandActivityOpenServerInvestOtherCfg::INVEST_TYPE_BOSS || type >= RandActivityOpenServerInvestOtherCfg::MAX_INVEST_TYPE)
	{
		return;
	}

	if (0 == (m_activity_info.ra_open_server_invest_reward_flag & (1 << (type * 8))))
	{
		return;
	}

	const RandActivityOpenServerInvestRewardCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAServerInvestRewardCfg(type, RAND_ACTIVITY_OPEN_SERVER_INVEST_MAX_COUNT - 1);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (RandActivityOpenServerInvestOtherCfg::INVEST_TYPE_ACTIVE == type)
	{
		if (active_score >= other_cfg->active_reward_limit && m_activity_info.ra_open_server_invest_reward_param[type] <= reward_cfg->param
				&& 0 == m_activity_info.ra_open_server_invest_is_add_active_param)
		{
			m_activity_info.ra_open_server_invest_is_add_active_param = 1;
			m_activity_info.ra_open_server_invest_reward_param[type]++;

			this->SendRAOpenServerInvestInfo();
		}
	}
	else if(m_activity_info.ra_open_server_invest_reward_param[type] <= reward_cfg->param)
	{
		m_activity_info.ra_open_server_invest_reward_param[type]++;

		this->SendRAOpenServerInvestInfo();
	}
}

void RoleActivity::AddXingzuoYijiGatherBox(int type)
{
	if (type >= 0 && type < XINGZUOYIJI_BOX_TYPE_MAX)
	{
		m_activity_info.xingzuoyiji_gather_box_num[type]++;
	}
}

void RoleActivity::OnRASingleChongZhiOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI))
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::RA_SINGLE_CHONGZHI_OPERA_TYPE_INFO:
		this->SendSingleChongZhiAllInfo();
		break;

	case Protocol::RA_SINGLE_CHONGZHI_OPERA_TYPE_FETCH_REWARD:
		this->OnRASingleChongZhiFetchReward(param1);
		break;
	}
}

void RoleActivity::OnRASingleChongZhiFetchReward(int seq)
{
	if (!(m_activity_info.ra_single_chongzhi_reward_flag & (1 << seq)))
	{
		return;
	}

	const RandActivitySingleChongZhiRewardCfg::ItemConfig *item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetSingleChongZhiCfgBySeq(seq);
	if (NULL == item_cfg)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_role->GetKnapsack()->GetMoney()->AddGold(item_cfg->reward_gold, "RandActivitySingleChongZhi");
	m_role->GetKnapsack()->Put(item_cfg->reward_item, PUT_REASON_SINGLE_CHONGZHI_REWAED);

	m_activity_info.ra_single_chongzhi_reward_flag &= ~(1 << seq);
	m_activity_info.ra_single_chongzhi_is_reward_flag |= (1 << seq);

	this->SendSingleChongZhiAllInfo();

	return;
}

void RoleActivity::OnRASingleChongZhiGiveReward()
{
	for (int i = 0; i < RAND_ACTIVITY_SINGLE_CHONGZHI_MAX_CFG; ++i)
	{
		if (m_activity_info.ra_single_chongzhi_reward_flag & (1 << i))
		{
			const RandActivitySingleChongZhiRewardCfg::ItemConfig *item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetSingleChongZhiCfgBySeq(i);
			if (NULL == item_cfg)
			{
				continue;
			}

			const ItemBase *item_base = ITEMPOOL->GetItem(item_cfg->reward_item.item_id);
			if (NULL != item_base)
			{
				static MailContentParam contentparam; contentparam.Reset();
				contentparam.item_list[0].item_id = item_cfg->reward_item.item_id;
				contentparam.item_list[0].num = item_cfg->reward_item.num;
				contentparam.item_list[0].is_bind = (item_cfg->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();
				contentparam.gold = item_cfg->reward_gold;

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_single_chongzhi_reward_content, item_cfg->need_gold);
				if (length > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

					gamelog::g_log_rand_activity.printf(LL_INFO, "RandActivitySingleChongzhiRoll::Reward user %d %s, reward_item %d, reward_num %d, reward_gold %d",
						m_role->GetUID(), m_role->GetName(), item_cfg->reward_item.item_id, item_cfg->reward_item.num, item_cfg->reward_gold);

					m_activity_info.ra_single_chongzhi_reward_flag &= ~(1 << i);
					m_activity_info.ra_single_chongzhi_is_reward_flag |= (1 << i);
				}
			}
		}
	}

	m_activity_info.ra_single_chongzhi_reward_flag = 0;
	m_activity_info.ra_single_chongzhi_is_reward_flag = 0;
	this->SendSingleChongZhiAllInfo();

	return;
}

void RoleActivity::SendSingleChongZhiAllInfo()
{
	static Protocol::SCRASingleChongZhiInfo sczi;

	sczi.fetch_reward_flag = m_activity_info.ra_single_chongzhi_reward_flag;
	sczi.is_fetch_reward_flag = m_activity_info.ra_single_chongzhi_is_reward_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sczi, sizeof(sczi));
	return;
}

void RoleActivity::OnRASingleChongZhiAddChongZhi(int gold_num)
{
	if (gold_num < 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI))
	{
		return;
	}

	const RandActivitySingleChongZhiRewardCfg::ItemConfig *item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetSingleChongZhiCfgByGoldNum(gold_num);
	if (NULL == item_cfg)
	{
		return;
	}

	//for (int i = 0; i <= item_cfg->seq; i++)
	//{
	//	if (m_activity_info.ra_single_chongzhi_is_reward_flag & (1 << i))
	//	{
	//		continue;
	//	}
	//	m_activity_info.ra_single_chongzhi_reward_flag |= (1 << i);
	//}

	if (m_activity_info.ra_single_chongzhi_is_reward_flag & (1 << item_cfg->seq))
	{
		return;
	}
	m_activity_info.ra_single_chongzhi_reward_flag |= (1 << (item_cfg->seq));

	this->SendSingleChongZhiAllInfo();

	return;
}

void RoleActivity::OnRaXianyuanTreasOperReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_XIANYUAN_TREAS))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_XIANYUAN_TREAS);

	switch (opera_type)
	{
	case Protocol::RA_XIANYUAN_TREAS_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRaXianyuanTreasInfo();
	}
	break;

	case Protocol::RA_XIANYUAN_TREAS_OPERA_TYPE_BUY:
	{
		this->OnRaXianyuanTreasBuy(param);
	}
	break;

	case Protocol::RA_XIANYUAN_TREAS_OPERA_TYPE_BUY_ALL:
	{
		this->OnRaXianyuanTreasBuyAll();
	}
	break;

	case Protocol::RA_XIANYUAN_TREAS_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnRaXianyuanTreasFetchReward(param);
	}
	break;
	}
}

void RoleActivity::SendRaXianyuanTreasInfo()
{
	Protocol::SCRAXianyuanTreasInfo scraxyti;

	scraxyti.all_buy_gift_fetch_flag = m_activity_info.ra_xianyuan_treas_all_buy_gift_fetch_flag;
	memcpy(scraxyti.xianyuan_list, m_activity_info.ra_xianyuan_treas_list, sizeof(scraxyti.xianyuan_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scraxyti, sizeof(scraxyti));
}

void RoleActivity::OnRaXianyuanTreasBuy(int seq)
{
	const RandActivityXianyuanTreasConfig *xianyuan_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetXianyuanTreasCfg(seq);
	if (NULL == xianyuan_cfg)
	{
		return;
	}

	if (m_activity_info.ra_xianyuan_treas_list[seq].buy_day_index > 0)
	{
		m_role->NoticeNum(errornum::EN_RA_XIANYUAN_TREAS_ALREADY_BUY);
		return;
	}

	int act_last_days = LOGIC_CONFIG->GetRandActivityCfg().GetActivityLastedDays(RAND_ACTIVITY_TYPE_XIANYUAN_TREAS);
	if (act_last_days > LOGIC_CONFIG->GetRandActivityCfg().GetXianyuanTreasBuyLimitDay())
	{
		m_role->NoticeNum(errornum::EN_RA_XIANYUAN_TREAS_PASS_BUY_DAY);
		return;
	}

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(xianyuan_cfg->consume_gold, "RoleActivity::OnRaXianyuanTreasBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_activity_info.ra_xianyuan_treas_list[seq].buy_day_index = act_last_days;
	m_activity_info.ra_xianyuan_treas_list[seq].num = 0;

	this->OnRaXianyuanTreasFetchAllBuyGift();

	this->SendRaXianyuanTreasInfo();

	// log
// 	ROLE_LOG_QUICK10(LOG_TYPE_RAND_ACTIVITY_XIANYUAN_TREAS, m_role, seq, NULL, "single_buy",
// 		NULL, xianyuan_cfg->consume_gold, act_last_days, 0, 0);

	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRaXianyuanTreasBuy user[%d, %s], seq[%d], act_real_open_day[%d], buy_day_index[%d], consume_gold[%d]",
		m_role->GetUID(), m_role->GetName(), seq, act_last_days, m_activity_info.ra_xianyuan_treas_list[seq].buy_day_index, xianyuan_cfg->consume_gold);
}

void RoleActivity::OnRaXianyuanTreasBuyAll()
{
	int total_consume_gold = 0;

	int act_last_days = LOGIC_CONFIG->GetRandActivityCfg().GetActivityLastedDays(RAND_ACTIVITY_TYPE_XIANYUAN_TREAS);
	if (act_last_days > LOGIC_CONFIG->GetRandActivityCfg().GetXianyuanTreasBuyLimitDay())
	{
		m_role->NoticeNum(errornum::EN_RA_XIANYUAN_TREAS_PASS_BUY_DAY);
		return;
	}

	int count = LOGIC_CONFIG->GetRandActivityCfg().GetXianyuanTreasCount();
	for (int i = 0; i < count && i < RAND_ACTIVITY_XIANYUAN_TREAS_COUNT; ++i)
	{
		const RandActivityXianyuanTreasConfig *xianyuan_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetXianyuanTreasCfg(i);
		if (NULL == xianyuan_cfg)
		{
			return;
		}

		if (m_activity_info.ra_xianyuan_treas_list[i].buy_day_index <= 0)
		{
			total_consume_gold += xianyuan_cfg->consume_gold;
		}
	}

	if (total_consume_gold <= 0)
	{
		m_role->NoticeNum(errornum::EN_RA_XIANYUAN_TREAS_HAVE_ALL_BUY);
		return;
	}

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(total_consume_gold, "RoleActivity::OnRaXianyuanTreasBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	for (int index = 0; index < count && index < RAND_ACTIVITY_XIANYUAN_TREAS_COUNT; ++index)
	{
		if (m_activity_info.ra_xianyuan_treas_list[index].buy_day_index <= 0)
		{
			m_activity_info.ra_xianyuan_treas_list[index].buy_day_index = act_last_days;
		}
	}

	this->OnRaXianyuanTreasFetchAllBuyGift();

	this->SendRaXianyuanTreasInfo();

	// log
// 	ROLE_LOG_QUICK10(LOG_TYPE_RAND_ACTIVITY_XIANYUAN_TREAS, m_role, NULL, NULL, "all_buy",
// 		NULL, total_consume_gold, act_last_days, 0, 0);

	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRaXianyuanTreasBuyAll user[%d, %s], total_consume_gold[%d], act_last_days[%d]",
		m_role->GetUID(), m_role->GetName(), total_consume_gold, act_last_days);
}

void RoleActivity::OnRaXianyuanTreasFetchReward(int seq)
{
	const RandActivityXianyuanTreasConfig *xianyuan_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetXianyuanTreasCfg(seq);
	if (NULL == xianyuan_cfg)
	{
		return;
	}

	if (m_activity_info.ra_xianyuan_treas_list[seq].buy_day_index <= 0)
	{
		m_role->NoticeNum(errornum::EN_RA_XIANYUAN_TREAS_NO_BUY);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	int act_last_days = LOGIC_CONFIG->GetRandActivityCfg().GetActivityLastedDays(RAND_ACTIVITY_TYPE_XIANYUAN_TREAS);
	if (m_activity_info.ra_xianyuan_treas_list[seq].num > (act_last_days - m_activity_info.ra_xianyuan_treas_list[seq].buy_day_index))
	{
		m_role->NoticeNum(errornum::EN_RA_XIANYUAN_TREAS_ALREADY_FETCH);
		return;
	}

	if (m_activity_info.ra_xianyuan_treas_list[seq].num >= xianyuan_cfg->max_reward_day)
	{
		m_role->NoticeNum(errornum::EN_RA_XIANYUAN_TREAS_GIFT_FULL);
		return;
	}

	if (m_role->GetKnapsack()->Put(xianyuan_cfg->reward_item, PUT_REASON_RA_XIANYUAN_TREAS_REWARD))
	{
		++m_activity_info.ra_xianyuan_treas_list[seq].num;
		this->SendRaXianyuanTreasInfo();
	}

	// log
	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRaXianyuanTreasFetchReward user[%d, %s], seq[%d], act_last_days[%d], num[%d]",
		m_role->GetUID(), m_role->GetName(), seq, act_last_days, m_activity_info.ra_xianyuan_treas_list[seq].num);
}

void RoleActivity::OnRaXianyuanTreasFetchAllBuyGift()
{
	if (m_activity_info.ra_xianyuan_treas_all_buy_gift_fetch_flag >= 1)
	{
		return;
	}

	for (int i = 0; i < LOGIC_CONFIG->GetRandActivityCfg().GetXianyuanTreasCount(); ++i)
	{
		if (m_activity_info.ra_xianyuan_treas_list[i].buy_day_index <= 0)
		{
			return;
		}
	}

	static MailContentParam contentparam; contentparam.Reset();
	const ItemConfigData* reward_item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetXianyuanTreasAllBuyReward();
	if (!reward_item_cfg)
	{
		return;
	}

	contentparam.item_list[0].item_id = reward_item_cfg->item_id;
	contentparam.item_list[0].num = reward_item_cfg->num;
	contentparam.item_list[0].is_bind = (reward_item_cfg->is_bind) ? 1 : 0;

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_xianyuan_treas_all_buy_content,
		RAND_ACTIVITY_TYPE_XIANYUAN_TREAS);

	if (length > 0)
	{
		m_activity_info.ra_xianyuan_treas_all_buy_gift_fetch_flag = 1;
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
	}

	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::RaCheckXianyuanTReasMailReward user[%d, %s], xianyuan_treas_all_buy_gift_fetch_flag[%d]",
		m_role->GetUID(), m_role->GetName(), m_activity_info.ra_xianyuan_treas_all_buy_gift_fetch_flag);
}

void RoleActivity::OnRARushBuyingOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_RUSH_BUYING))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_RUSH_BUYING);

	switch (opera_type)
	{
	case Protocol::RA_RUSH_BUYING_OPERA_TYPE_QUERY_ALL_INFO:
	{
		this->SendRARushBuyingAllInfo();
	}
	break;

	case Protocol::RA_RUSH_BUYING_OPERA_TYPE_BUY_ITEM:
	{
		this->OnRARushBuyingBuyItem(param1);
	}
	break;
	}
}

void RoleActivity::SendRARushBuyingAllInfo()
{
	RandActivityRushBuying *rush_buying_act = (RandActivityRushBuying*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_RUSH_BUYING);
	if (NULL == rush_buying_act)
	{
		return;
	}

	static Protocol::SCRARushBuyingAllInfo rrbai;

	rrbai.reserve_sh = 0;
	rrbai.buy_phase = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingPhase(EngineAdapter::Instance().Time());
	rrbai.item_count = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingSaleItemsCount(rrbai.buy_phase);
	rrbai.buy_end_timestamp = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingPhaseBeginTimestamp(rrbai.buy_phase) + LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingDuration() * SECOND_PER_MIN;

	short server_items_buy_times[RA_RUSH_BUYING_SALES_ITEMS_COUNT]; memset(server_items_buy_times, 0, sizeof(server_items_buy_times));
	rush_buying_act->GetServerItemBuyTimes(server_items_buy_times, RA_RUSH_BUYING_SALES_ITEMS_COUNT);

	for (int i = 0; i < rrbai.item_count && i < RA_RUSH_BUYING_SALES_ITEMS_COUNT; ++i)
	{
		rrbai.item_buy_times_list[i].server_buy_times = server_items_buy_times[i];
		rrbai.item_buy_times_list[i].role_buy_times = m_activity_info.ra_rush_buying_item_buying_times[i];
	}

	int send_len = sizeof(rrbai) - (RA_RUSH_BUYING_SALES_ITEMS_COUNT - rrbai.item_count) * sizeof(rrbai.item_buy_times_list[0]);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rrbai, send_len);
}

void RoleActivity::OnRARushBuyingBuyItem(int index)
{
	// 检查
	if (m_activity_info.ra_rush_buying_cur_pahse < 0)
	{
		m_role->NoticeNum(errornum::EN_RA_RUSH_BUYING_NOT_IN_TIME);
		return;
	}

	int items_count = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingSaleItemsCount(m_activity_info.ra_rush_buying_cur_pahse);

	if (index < 0 || index >= items_count || index >= RA_RUSH_BUYING_SALES_ITEMS_COUNT)
	{
		return;
	}

	{
		unsigned int cur_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		unsigned int buying_begin_timestamp = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingPhaseBeginTimestamp(m_activity_info.ra_rush_buying_cur_pahse);
		unsigned int buying_duration = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingDuration() * SECOND_PER_MIN;

		if (cur_timestamp < buying_begin_timestamp || cur_timestamp >= buying_begin_timestamp + buying_duration)
		{
			m_role->NoticeNum(errornum::EN_RA_RUSH_BUYING_NOT_IN_TIME);
			return;
		}
	}

	RandActivityRushBuying *rush_buying_act = (RandActivityRushBuying*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_RUSH_BUYING);
	if (NULL == rush_buying_act)
	{
		return;
	}

	const RandActivityRushBuyingPhaseConfig::SaleItemConfig * sale_item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingSaleItemCfg(m_activity_info.ra_rush_buying_cur_pahse, index);
	if (NULL == sale_item_cfg)
	{
		return;
	}

	if (m_activity_info.ra_rush_buying_item_buying_times[index] >= sale_item_cfg->role_buy_times_limit || m_activity_info.ra_rush_buying_item_buying_times[index] >= RA_RUSH_BUYING_MAX_ROLE_BUYING_TIMES)
	{
		m_role->NoticeNum(errornum::EN_RA_RUSH_BUYING_ROLE_BUY_TIMES_FULL);
		return;
	}

	if (rush_buying_act->GetServerItemBuyTimes(index) >= sale_item_cfg->server_buy_times_limit)
	{
		m_role->NoticeNum(errornum::EN_RA_RUSH_BUYING_SERVER_BUY_TIMES_FULL);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
		return;
	}

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(sale_item_cfg->sale_price, "RoleActivity::OnRARushBuyingBuyItem"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 给予
	m_role->GetKnapsack()->Put(sale_item_cfg->sale_item, PUT_REASON_RA_RUSH_BUYING_BUY_ITEM);

	m_role->NoticeNum(noticenum::NT_RUSH_BUYING_BUY_ITEM_SUCC);

	++m_activity_info.ra_rush_buying_item_buying_times[index];
	rush_buying_act->AddServerItemBuyTimes(index);

	// 发送
	this->SendRARushBuyingAllInfo();

	// 日志
	gamelog::g_log_roleactivity.printf(LL_INFO, " RoleActivity::OnRARushBuyingBuyItem user[%d %s], buy_phase[%d], item_index[%d], item[%s], use_gold[%d], role_buy_times[%d], server_buy_times[%d]", m_role->GetUID(), m_role->GetName(),
		m_activity_info.ra_rush_buying_cur_pahse, index, ItemConfigDataLog(&sale_item_cfg->sale_item, NULL), sale_item_cfg->sale_price, m_activity_info.ra_rush_buying_item_buying_times[index], rush_buying_act->GetServerItemBuyTimes(index));
}

void RoleActivity::OnRaMapHuntOperaReq(int opera_type, int param1, int param2)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MAP_HUNT))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MAP_HUNT);

	switch (opera_type)
	{
	case Protocol::RA_MAP_HUNT_OPERA_TYPE_ALL_INFO:
		{
			this->SendRAMapHuntInfo();
		}
		break;

	case Protocol::RA_MAP_HUNT_OPERA_TYPE_FLUSH:
		{
			bool is_get = true;
			this->OnRaMapHuntFlush(false, is_get);
		}
		break;

	case Protocol::RA_MAP_HUNT_OPERA_TYPE_AUTO_FLUSH:
		{
			for (int i = 0; i < param2; ++i)
			{
				bool is_get = false;
				bool ret = this->OnRaMapHuntQuicklyFlush(param1, is_get);
				if (!ret || is_get)
				{
					m_role->SendOperateResult(Protocol::SCOperateResult::OP_RA_MAPHUNT_AUTO_FLUSH, 0);
					return;
				}
			}

			if (param2 > 0)
			{
				m_role->SendOperateResult(Protocol::SCOperateResult::OP_RA_MAPHUNT_AUTO_FLUSH, 1);
			}

			if (0 == param2)
			{
				m_role->SendOperateResult(Protocol::SCOperateResult::OP_RA_MAPHUNT_AUTO_FLUSH, 0);
			}
		}
		break;

	case Protocol::RA_MAP_HUNT_OPERA_TYPE_HUNT:
		{
			this->OnRaMapHuntXunbao(param1);
		}
		break;

	case Protocol::RA_MAP_HUNT_OPERA_TYPE_FETCH_RETURN_REWARD:
		{
			this->OnRaMapHuntFetchReturnReward(param1);
		}
		break;
	}
}

void RoleActivity::SendRAMapHuntInfo(int extern_reward_route_id)
{
	Protocol::SCRAMapHuntAllInfo cmd;
	cmd.route_info.route_index = m_activity_info.ra_map_hunt_cur_route_info.route_index;
	cmd.route_info.route_active_flag = m_activity_info.ra_map_hunt_cur_route_info.route_active_flag;
	memcpy(cmd.route_info.city_list, m_activity_info.ra_map_hunt_cur_route_info.city_list, sizeof(cmd.route_info.city_list));
	cmd.route_info.city_fetch_flag = m_activity_info.ra_map_hunt_cur_route_info.city_fetch_flag;

	cmd.flush_times = m_activity_info.ra_map_hunt_play_times;
	cmd.next_flush_timestamp = m_activity_info.ra_map_hunt_next_flush_timestamp;
	cmd.return_reward_fetch_flag = m_activity_info.ra_map_hunt_return_reward_fetch_flag;
	cmd.cur_has_used_free_count = m_activity_info.ra_map_hunt_has_used_free_xunbao_count;
	cmd.can_extern_reward = extern_reward_route_id;

	int length = sizeof(cmd);
	if (length > 0)
	{
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, length);
	}
}

bool RoleActivity::OnRaMapHuntFlush(bool is_free, bool &is_get, const int city_id)
{
	int need_consume_gold = 0;
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (is_free)
	{
		if (now < m_activity_info.ra_map_hunt_next_flush_timestamp)
		{
			return false;
		}
	}
	else
	{
		need_consume_gold = LOGIC_CONFIG->GetRandActivityCfg().GetMapHuntFlushNeedGold();
		if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_consume_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return false;
		}
	}

	RandActivityMapHunt *map_hunt_act = (RandActivityMapHunt *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_MAP_HUNT);
	if (nullptr == map_hunt_act)
	{
		return false;
	}

	const int server_chou_times = map_hunt_act->GetServerTotalFlushTimes();

	RandActivityMapHuntCityConfig::CityCfg *temp_cfg_list[MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE] = { NULL };
	bool iRet = LOGIC_CONFIG->GetRandActivityCfg().GetMapHuntRandCityCfg(MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE, temp_cfg_list, server_chou_times);
	if (!iRet)
	{
		return false;
	}

	for (int i = 0; i < MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE; ++i)
	{
		if (NULL != temp_cfg_list[i])
		{
			m_activity_info.ra_map_hunt_cur_route_info.city_list[i] = temp_cfg_list[i]->city_id;

			if (!is_get && city_id >= 0 && (temp_cfg_list[i]->city_id == city_id || temp_cfg_list[i]->is_broadcast))
			{
				is_get = true;
			}

			if (temp_cfg_list[i]->is_broadcast)
			{
				m_role->NoticeNum(noticenum::NT_MAP_HUNT_GET_RARE_CITY);

				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_map_hunt_get_rare_city_content,
					m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), temp_cfg_list[i]->city_id);
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
				}
			}
		}
	}

	if (is_free)
	{
		m_activity_info.ra_map_hunt_next_flush_timestamp = (now + LOGIC_CONFIG->GetRandActivityCfg().GetMapHuntFlushInterval() * SECOND_PER_MIN);
	}
	else
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_consume_gold, "RoleActivity::OnRaMapHuntFlush"))
		{
			return false;
		}
		++m_activity_info.ra_map_hunt_play_times;

		// 增加全服刷新次数
		map_hunt_act->AddFlushTimes(1);

		//ROLE_LOG_QUICK6(LOG_TYPE_RANDACTIVITY_ROLE_CONSUME_GOLD, m_role, RAND_ACTIVITY_TYPE_MAP_HUNT, m_activity_info.ra_role_map_hunt_consume_gold, NULL, NULL);
	}

	m_activity_info.ra_map_hunt_cur_route_info.city_fetch_flag = 0;

	this->SendRAMapHuntInfo();

	if (city_id < 0)
	{
		gamelog::g_log_roleactivity.printf(LL_INFO, "OnRaMapHuntFlush user[%d %s], consume_gold[%d], reward_count[%d]",
			m_role->GetUID(), m_role->GetName(), need_consume_gold, MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE);
	}

	return true;
}

bool RoleActivity::OnRaMapHuntQuicklyFlush(const int city_id, bool &is_get)
{
	if (is_get)
	{
		return false;
	}

	bool iRet = this->OnRaMapHuntFlush(false, is_get, city_id);
	if (!iRet)
	{
		return false;
	}

	int need_consume_gold = LOGIC_CONFIG->GetRandActivityCfg().GetMapHuntFlushNeedGold();
	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	gamelog::g_log_roleactivity.printf(LL_INFO, "OnRaMapHuntQuicklyFlush user[%d %s], consume_gold[%d], reward_count[%d]",
		m_role->GetUID(), m_role->GetName(), need_consume_gold, MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE);

	//ROLE_LOG_QUICK6(LOG_TYPE_RANDACTIVITY_ROLE_CONSUME_GOLD, m_role, RAND_ACTIVITY_TYPE_MAP_HUNT, m_activity_info.ra_role_map_hunt_consume_gold, NULL, NULL);

	return true;
}

void RoleActivity::OnRaMapHuntXunbao(const int city_id)
{
	int need_gold = 0;
	bool is_free = false;

	if (LOGIC_CONFIG->GetRandActivityCfg().GetMapHuntFreeXunbaoCount() - m_activity_info.ra_map_hunt_has_used_free_xunbao_count > 0)
	{
		is_free = true;
	}

	if (!is_free)
	{
		need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetMapHuntXunbaoGold();
		if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	RAMapHuntRouteInfo &route_info = m_activity_info.ra_map_hunt_cur_route_info;

	short cur_not_active_idx = 0;
	for (; cur_not_active_idx < MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE; ++cur_not_active_idx)
	{
		if ((route_info.route_active_flag & (1 << cur_not_active_idx)) <= 0)
		{
			break;
		}
	}

	short cur_city_list_idx = 0;
	for (; cur_city_list_idx < MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE; ++cur_city_list_idx)
	{
		if (route_info.city_list[cur_city_list_idx] == city_id)
		{
			break;
		}
	}

	if ((m_activity_info.ra_map_hunt_cur_route_info.city_fetch_flag & (1 << cur_city_list_idx)) > 0)
	{
		return;
	}

	//一个基础奖励和一个路线终极奖励
	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(2))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const ItemConfigData *base_reward = LOGIC_CONFIG->GetRandActivityCfg().GetMapHuntBaseReward(city_id);
	if (NULL == base_reward)
	{
		return;
	}

	if (!is_free)
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "OnRaMapHuntXunbao"))
		{
			return;
		}
	}
	else
	{
		++m_activity_info.ra_map_hunt_has_used_free_xunbao_count;
	}

	bool is_first = true;
	if (route_info.route_index != 0 && route_info.route_active_flag != 0)
	{
		is_first = false;
	}

	const RandActivityMapHuntRouteConfig::RouteCfg *route_cfg = NULL;
	if (!is_first)
	{
		route_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMapHuntRouteCfg(route_info.route_index);
		if (NULL == route_cfg)
		{
			return;
		}
	}

	// 第一次寻宝或者不在路线下一个要激活的城市
	if (is_first || (NULL != route_cfg && route_cfg->city_list[cur_not_active_idx] != city_id))
	{
		const int idx = LOGIC_CONFIG->GetRandActivityCfg().GetMapHuntRouteIndex(city_id);
		if (idx > 0)
		{
			route_info.route_index = idx;
			route_info.route_active_flag = 1;
			route_info.city_fetch_flag |= (1 << cur_city_list_idx);
		}
	}
	else
	{
		route_info.route_active_flag |= (1 << cur_not_active_idx);
		route_info.city_fetch_flag |= (1 << cur_city_list_idx);
	}

	m_role->GetKnapsack()->Put(*base_reward, PUT_REASON_MAP_HUNT_BASE_REWARD);
	LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_MAP_HUNT, m_role->GetUID(), m_role->GetName(), base_reward->item_id, base_reward->num);

	gamelog::g_log_roleactivity.buff_printf("OnRaMapHuntXunbao user[%d %s]", m_role->GetUID(), m_role->GetName());
	gamelog::g_log_roleactivity.buff_printf(" base_reward[%s]", ItemConfigDataLog(base_reward, NULL));

	bool is_all_active = true;
	for (int i = 0; i < MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE; ++i)
	{
		if ((route_info.route_active_flag & (1 << i)) <= 0)
		{
			is_all_active = false;
		}
	}

	if (is_all_active && NULL != route_cfg)
	{
		m_role->GetKnapsack()->Put(route_cfg->reward_item, PUT_REASON_MAP_HUNT_EXTERN_REWARD);
		this->SendRAMapHuntInfo(route_cfg->route_index);

		gamelog::g_log_roleactivity.buff_printf(" extern_reward[%s]", ItemConfigDataLog(&route_cfg->reward_item, NULL));
	}
	else
	{
		this->SendRAMapHuntInfo();
	}

	gamelog::g_log_roleactivity.commit_buff(LL_INFO);

	gamelog::g_log_roleactivity.printf(LL_INFO, "OnRaMapHuntXunbao user[%d %s], need_gold[%d]",
		m_role->GetUID(), m_role->GetName(), need_gold);
}

void RoleActivity::OnRaMapHuntFetchReturnReward(int index)
{
	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if ((m_activity_info.ra_map_hunt_return_reward_fetch_flag & (1 << index)) > 0)
	{
		m_role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		return;
	}

	const RandActivityMapHuntServerRewardConfig::RewardCfg *temp_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMapHuntServerRewardCfg(index);
	if (NULL == temp_cfg)
	{
		return;
	}

	if (m_activity_info.ra_map_hunt_play_times < temp_cfg->need_flush_count)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_XUNBAO_LIMIT_TIME);
		return;
	}

	m_activity_info.ra_map_hunt_return_reward_fetch_flag |= (1 << index);

	m_role->GetKnapsack()->Put(temp_cfg->reward_item, PUT_REASON_MAP_HUNT_RETURN_REWARD);
	this->SendRAMapHuntInfo();

	gamelog::g_log_roleactivity.printf(LL_INFO, "OnRaMapHuntFetchServerReward user[%d %s], fetch_flag[%d], item[%s]",
		m_role->GetUID(), m_role->GetName(), m_activity_info.ra_map_hunt_return_reward_fetch_flag, ItemConfigDataLog(&temp_cfg->reward_item, NULL));
}

void RoleActivity::OnRaLightTowerExploreOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE);

	switch (opera_type)
	{
	case Protocol::RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_INFO:
	{
		this->SendLightTowerExploreInfo();
	}
	break;

	case Protocol::RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_DRAW:
	{
		this->OnRALightTowerExploreDraw(param1);
	}
	break;

	case Protocol::RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_FETCH_EXTERN_REWARD:
	{
		this->OnRALightTowerExploreFetchExternReward(param1);
	}
	break;

	case Protocol::RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_FETCH_SERVER_REWARD:
	{
		this->OnRALightTowerExploreFetchReturnReward(param1);
	}
	break;
	}
}

void RoleActivity::OnRALightTowerExploreDraw(int times)
{
	int need_gold = 0;
	if (1 == times)
	{
		need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetLightTowerExploreDrawOneGold();
	}
	else if (10 == times)
	{
		need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetLightTowerExploreDrawTenGold();
	}

	if (need_gold <= 0)
	{
		return;
	}

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, times))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_SPACE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int cur_layer = m_activity_info.ra_light_tower_cur_layer;
	int cur_index = m_activity_info.ra_light_tower_cur_index;
	int loop_times = 0;

	int reward_count = 0;
	ChestItemInfo reward_list[MAX_LIGHT_TOWER_EXPLORE_REWARD_COUNT];
	for (int i = 0; i < MAX_LIGHT_TOWER_EXPLORE_REWARD_COUNT; ++i)
	{
		reward_list[i].Reset();
	}

	const RandActivityLightTowerExploreCfg::LayerConfig *reward_cfg = NULL;

	Protocol::SCRALightTowerExploreDrawInfo cmd;
	cmd.reward_count = 0;

	gamelog::g_log_roleactivity.buff_printf("OnRALightTowerExploreDraw: user [%d][%s], item_list[", m_role->GetUID(), m_role->GetName());

	for (; reward_count < times && reward_count < MAX_LIGHT_TOWER_EXPLORE_REWARD_COUNT && loop_times < 1024;)
	{
		++loop_times;

		reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLightTowerExploreReward(cur_layer);
		if (NULL == reward_cfg)
		{
			continue;
		}

		if (LIGHT_TOWER_EXPLORE_REWAED_TYPE_GOTO_LAYER == reward_cfg->reward_type)			//抽到跳转，给目标层的第一个奖励,否则直接给奖励
		{
			cur_layer = reward_cfg->goto_layer;
			reward_cfg = NULL;
			reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLightTowerExploreReward(cur_layer, true);
			if (NULL == reward_cfg)
			{
				continue;
			}
		}

		reward_list[reward_count].item_id = reward_cfg->reward_item.item_id;
		reward_list[reward_count].num = reward_cfg->reward_item.num;
		reward_list[reward_count].is_bind = reward_cfg->reward_item.is_bind;
		++reward_count;

		cmd.reward_list[cmd.reward_count].layer = cur_layer;
		cmd.reward_list[cmd.reward_count].index = reward_cfg->index;
		++cmd.reward_count;

		if (reward_cfg->is_broadcast)
		{
			int notice_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_light_tower_explore_reward_content,
				m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), reward_cfg->reward_item.item_id);

			if (notice_len > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, notice_len, SYS_MSG_CENTER_ROLL);
			}
		}

		cur_index = reward_cfg->index;
		m_activity_info.ra_light_tower_info[cur_layer] |= (1 << cur_index);

		gamelog::g_log_roleactivity.buff_printf("(%s)", ItemConfigDataLog(&reward_cfg->reward_item, NULL));
	}

	RandActivityLightTowerExplore *activity = (RandActivityLightTowerExplore *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE);
	if (NULL == activity)
	{
		gamelog::g_log_roleactivity.clear_buff();
		return;
	}
	m_activity_info.ra_light_tower_draw_times += times;

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "OnRaLightTowerExploreDraw"))
	{
		gamelog::g_log_roleactivity.clear_buff();
		return;
	}

	m_activity_info.ra_light_tower_cur_layer = cur_layer;
	m_activity_info.ra_light_tower_cur_index = cur_index;

	// 给予
//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, reward_count, reward_list, "Randactivity LightTowerExplore");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE, reward_count, reward_list, "RoleActivity::LightTowerExplore");

	if (sizeof(cmd) > 0)
	{
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
	}

	this->SendLightTowerExploreInfo();

	RandActivityLightTowerExplore *light_tower_expore_act = (RandActivityLightTowerExplore *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE);
	if (NULL != light_tower_expore_act)
	{
		//if (0 == m_activity_info.ra_tower_expore_join_flag)
		//{
		//	m_activity_info.ra_tower_expore_join_flag = 1;
		//	light_tower_expore_act->GetActivityParam()->join_uer_count++;
		//}

		if (0 == times)
		{
			light_tower_expore_act->GetActivityParam()->chou_one_time_count++;
		}
		else if (10 == times)
		{
			light_tower_expore_act->GetActivityParam()->chou_ten_time_count++;
		}

		if (need_gold > 0)
		{
			light_tower_expore_act->GetActivityParam()->cost_total_gold += need_gold;
		}
	}

	int log_join_user_cout = light_tower_expore_act->GetActivityParam()->join_uer_count;
	int log_total_gold = light_tower_expore_act->GetActivityParam()->cost_total_gold;
	int log_one_time_count = light_tower_expore_act->GetActivityParam()->chou_one_time_count;
	int log_ten_time_count = light_tower_expore_act->GetActivityParam()->chou_ten_time_count;
	int log_fifty_time_count = 0;

	char log_quick_str[LOG_BASE_LENGTH] = { 0 };
	SNPRINTF(log_quick_str, LOG_BASE_LENGTH, "join_user:%d, one_chou:%d, ten_chou:%d, fifty_chou:%d", log_join_user_cout, log_one_time_count, log_ten_time_count, log_fifty_time_count);

	LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_RANDACTIVITY_TOTAL_DATA, UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetPlatName(), RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE, log_total_gold, log_quick_str, NULL);

	gamelog::g_log_roleactivity.commit_buff(LL_INFO);
}

void RoleActivity::OnRALightTowerExploreFetchExternReward(int layer)
{
	if (!this->CheckCanFetchLightTowerExploreExternReward(layer))
	{
		m_role->NoticeNum(errornum::EN_REWARD_NOT_SATISFY);
		return;
	}

	if ((m_activity_info.ra_light_tower_layer_extern_reward_fetch_flag & (1 << layer)) > 0)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (NULL == knapsack || knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
	}

	const RandActivityLightTowerExploreExternCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLightTowerExploreExternReward(layer);
	if (NULL == reward_cfg)
	{
		return;
	}

	m_activity_info.ra_light_tower_layer_extern_reward_fetch_flag |= 1 << layer;

	knapsack->Put(reward_cfg->reward_item, PUT_REASON_LIGHT_TOWER_EXPLORE_EXTERN_REWARD);

	this->SendLightTowerExploreInfo();
}

void RoleActivity::OnRALightTowerExploreFetchReturnReward(int index)
{
	if ((m_activity_info.ra_light_tower_return_reward_fetch_flag & (1 << index)) > 0)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
		return;
	}

	const RandActivityLightTowerExploreReturnCfg *server_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLightTowerExploreReturnReward(index);
	if (NULL == server_cfg)
	{
		return;
	}

	RandActivityLightTowerExplore *activity = (RandActivityLightTowerExplore*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE);
	if (NULL == activity)
	{
		return;
	}

	if (m_activity_info.ra_light_tower_draw_times < server_cfg->need_count)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_XUNBAO_LIMIT_TIME);
		return;
	}

	m_activity_info.ra_light_tower_return_reward_fetch_flag |= (1 << index);

	m_role->GetKnapsack()->Put(server_cfg->reward_item, PUT_REASON_LIGHT_TOWER_EXPLORE_SERVER_REWARD);

	this->SendLightTowerExploreInfo();
}

bool RoleActivity::CheckCanFetchLightTowerExploreExternReward(int layer)
{
	const RandActivityLightTowerExploreCfg *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLightTowerExploreLayerCfg(layer);
	if (NULL == cfg)
	{
		return false;
	}

	for (int i = 0; i < RA_LIGHT_TOWER_EXPLORE_MAX_PER_LAYER_REWAED_COUNT; ++i)
	{
		if (LIGHT_TOWER_EXPLORE_REWAED_TYPE_ITEM == cfg->reward_list[i].reward_type)
		{
			if ((m_activity_info.ra_light_tower_info[layer] & (1 << i)) <= 0)
			{
				return false;
			}
		}
	}

	return true;
}

void RoleActivity::SendLightTowerExploreInfo()
{
	RandActivityLightTowerExplore *activity = (RandActivityLightTowerExplore *)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE);
	if (NULL == activity)
	{
		return;
	}

	Protocol::SCRALightTowerExploreInfo cmd;
	UNSTD_STATIC_CHECK(sizeof(cmd.light_tower_info) == sizeof(m_activity_info.ra_light_tower_info));
	memcpy(cmd.light_tower_info, m_activity_info.ra_light_tower_info, sizeof(cmd.light_tower_info));

	cmd.extern_reward_fetch_flag = m_activity_info.ra_light_tower_layer_extern_reward_fetch_flag;
	cmd.server_reward_fetcht_flag = m_activity_info.ra_light_tower_return_reward_fetch_flag;
	cmd.cur_index = m_activity_info.ra_light_tower_cur_index;
	cmd.cur_layer = m_activity_info.ra_light_tower_cur_layer;
	cmd.draw_times = m_activity_info.ra_light_tower_draw_times;
	int length = sizeof(cmd);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, length);
}

// ------------------------------
// 新累计充值
// ------------------------------
void RoleActivity::OnRANewTotalChargeOperaReq(int opera_type, int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE);

	switch (opera_type)
	{
	case Protocol::RA_NEW_TOTAL_CHARGE_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRANewTotalChargeInfo();
	}
	break;

	case Protocol::RA_NEW_TOTAL_CHARGE_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnRANewTotalChargeFetchReward(seq);
	}
	break;
	}
}

void RoleActivity::OnRANewTotalChargeCheckChongzhi(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE);

	m_activity_info.ra_new_total_charge_value += (int)chongzhi;

	this->SendRANewTotalChargeInfo();
}

void RoleActivity::RACheckNewTotalChargeGiveReward()
{
	int last_activity_got_max_seq = LOGIC_CONFIG->GetRandActivityCfg().GetNewTotalChargeSeqWithCharge(m_activity_info.ra_new_total_charge_value);
	if (last_activity_got_max_seq < 0)
	{
		return;
	}

	for (int seq = 0; seq <= last_activity_got_max_seq; ++seq)
	{
		if (0 != (m_activity_info.ra_new_total_charge_reward_has_fetch_flag & (1 << seq)))
		{
			continue;
		}

		const RandActivityNewTotalChargeRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetNewTotalChargeRewardWithSeq(seq);
		if (NULL == cfg_item)
		{
			return;
		}

		{
			// 发邮件
			static MailContentParam contentparam; contentparam.Reset();

			int reward_item_count = 0;
			for (int i = 0; i < cfg_item->item_count && reward_item_count < MAX_ATTACHMENT_ITEM_NUM; ++i)
			{
				const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->item_list[i].item_id);
				if (NULL != reward_item_base)
				{
					contentparam.item_list[reward_item_count].item_id = cfg_item->item_list[i].item_id;
					contentparam.item_list[reward_item_count].num = cfg_item->item_list[i].num;
					contentparam.item_list[reward_item_count].is_bind = (cfg_item->item_list[i].is_bind) ? 1 : 0;
					contentparam.item_list[reward_item_count].invalid_time = reward_item_base->CalInvalidTime();
					++reward_item_count;
				}
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_charge_bu_reward_content,
				RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE);
			if (length > 0)
			{
				m_activity_info.ra_new_total_charge_reward_has_fetch_flag |= (1 << seq);
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		{
			// 日志
			std::string get_item_log;
			ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

			gamelog::g_log_roleactivity.printf(LL_INFO, "RACheckNewTotalChargeGiveReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d], item_list[%s]",
				m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.ra_new_total_charge_reward_has_fetch_flag, get_item_log.c_str());

			ROLE_LOG_QUICK6(LOG_TYPE_RAND_ACTIVITY_TOTAL_CHONGZHI, m_role, seq, m_activity_info.ra_new_total_charge_reward_has_fetch_flag, NULL, NULL);
		}
	}
}

void RoleActivity::SendRANewTotalChargeInfo()
{
	Protocol::SCRANewTotalChargeInfo tcgi;
	tcgi.reward_has_fetch_flag = m_activity_info.ra_new_total_charge_reward_has_fetch_flag;
	tcgi.total_charge_value = m_activity_info.ra_new_total_charge_value;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tcgi, sizeof(tcgi));
}

void RoleActivity::OnRANewTotalChargeFetchReward(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE))
	{
		return;
	}

	if (seq < 0 || seq >= static_cast<int>(sizeof(m_activity_info.ra_new_total_charge_reward_has_fetch_flag) * 8))
	{
		return;
	}

	if (0 != (m_activity_info.ra_new_total_charge_reward_has_fetch_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_FETCH_REWARD);
		return;
	}

	const RandActivityNewTotalChargeRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetNewTotalChargeRewardWithSeq(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_new_total_charge_value < cfg_item->need_chongzhi)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_TOTAL_CHONGZHI_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg_item->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_new_total_charge_reward_has_fetch_flag |= (1 << seq);
	this->SendRANewTotalChargeInfo();

	m_role->GetKnapsack()->PutList(cfg_item->item_count, cfg_item->item_list, PUT_REASON_RA_TOTAL_CHARGE);

	if (cfg_item->item_count > 0)
	{
		// 累计充值传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_new_total_charge_reward_chuanwen_content,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), cfg_item->need_chongzhi);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	{
		// 日志
		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

		gamelog::g_log_roleactivity.printf(LL_INFO, "OnRANewTotalChargeFetchReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d], item_list[%s]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.ra_new_total_charge_reward_has_fetch_flag, get_item_log.c_str());

		ROLE_LOG_QUICK6(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE, m_role, seq, m_activity_info.ra_new_total_charge_reward_has_fetch_flag, NULL, NULL);
	}
}

// ------------------------------
// 新累计充值1
// ------------------------------
void RoleActivity::OnRANewTotalCharge1OperaReq(int opera_type, int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1);

	switch (opera_type)
	{
	case Protocol::RA_NEW_TOTAL_CHARGE_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRANewTotalCharge1Info();
	}
	break;

	case Protocol::RA_NEW_TOTAL_CHARGE_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnRANewTotalCharge1FetchReward(seq);
	}
	break;
	}
}

void RoleActivity::OnRANewTotalChargeCheckChongzhi1(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1);

	m_activity_info.ra_new_total_charge1_value += (int)chongzhi;

	this->SendRANewTotalCharge1Info();
}

void RoleActivity::RACheckNewTotalCharge1GiveReward()
{
	//屏蔽补发奖励
	return;
	int last_activity_got_max_seq = LOGIC_CONFIG->GetRandActivityCfg().GetNewTotalCharge1SeqWithCharge(m_activity_info.ra_new_total_charge1_value);
	if (last_activity_got_max_seq < 0)
	{
		return;
	}

	for (int seq = 0; seq <= last_activity_got_max_seq; ++seq)
	{
		if (0 != (m_activity_info.ra_new_total_charge1_reward_has_fetch_flag & (1 << seq)))
		{
			continue;
		}

		const RandActivityNewTotalChargeRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetNewTotalCharge1RewardWithSeq(seq);
		if (NULL == cfg_item)
		{
			return;
		}

		{
			// 发邮件
			static MailContentParam contentparam; contentparam.Reset();

			int reward_item_count = 0;
			for (int i = 0; i < cfg_item->item_count && reward_item_count < MAX_ATTACHMENT_ITEM_NUM; ++i)
			{
				const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->item_list[i].item_id);
				if (NULL != reward_item_base)
				{
					contentparam.item_list[reward_item_count].item_id = cfg_item->item_list[i].item_id;
					contentparam.item_list[reward_item_count].num = cfg_item->item_list[i].num;
					contentparam.item_list[reward_item_count].is_bind = (cfg_item->item_list[i].is_bind) ? 1 : 0;
					contentparam.item_list[reward_item_count].invalid_time = reward_item_base->CalInvalidTime();
					++reward_item_count;
				}
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_charge_bu_reward_content,
				RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1);
			if (length > 0)
			{
				m_activity_info.ra_new_total_charge1_reward_has_fetch_flag |= (1 << seq);
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		{
			// 日志
			std::string get_item_log;
			ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

			gamelog::g_log_roleactivity.printf(LL_INFO, "RACheckNewTotalCharge1GiveReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d], item_list[%s]",
				m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.ra_new_total_charge1_reward_has_fetch_flag, get_item_log.c_str());

			ROLE_LOG_QUICK6(LOG_TYPE_RAND_ACTIVITY_TOTAL_CHONGZHI, m_role, seq, m_activity_info.ra_new_total_charge1_reward_has_fetch_flag, NULL, NULL);
		}
	}
}

void RoleActivity::SendRANewTotalCharge1Info()
{
	Protocol::SCRATotalChargeMultiInfo tcgi;
	tcgi.activity_type = RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1;
	tcgi.reward_has_fetch_flag = m_activity_info.ra_new_total_charge1_reward_has_fetch_flag;
	tcgi.total_charge_value = m_activity_info.ra_new_total_charge1_value;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tcgi, sizeof(tcgi));
}

void RoleActivity::OnRANewTotalCharge1FetchReward(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1))
	{
		return;
	}

	if (seq < 0 || seq >= static_cast<int>(sizeof(m_activity_info.ra_new_total_charge1_reward_has_fetch_flag) * 8))
	{
		return;
	}

	if (0 != (m_activity_info.ra_new_total_charge1_reward_has_fetch_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_FETCH_REWARD);
		return;
	}

	const RandActivityNewTotalChargeRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetNewTotalCharge1RewardWithSeq(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_new_total_charge1_value < cfg_item->need_chongzhi)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_TOTAL_CHONGZHI_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg_item->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_new_total_charge1_reward_has_fetch_flag |= (1 << seq);
	this->SendRANewTotalCharge1Info();

	m_role->GetKnapsack()->PutList(cfg_item->item_count, cfg_item->item_list, PUT_REASON_RA_TOTAL_CHARGE);

	if (cfg_item->item_count > 0)
	{
		// 累计充值传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_new_total_charge_reward_chuanwen_content1,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), cfg_item->need_chongzhi);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	{
		// 日志
		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

		gamelog::g_log_roleactivity.printf(LL_INFO, "OnRANewTotalCharge1FetchReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d], item_list[%s]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.ra_new_total_charge1_reward_has_fetch_flag, get_item_log.c_str());

		ROLE_LOG_QUICK6(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE, m_role, seq, m_activity_info.ra_new_total_charge1_reward_has_fetch_flag, NULL, NULL);
	}
}

// ------------------------------
// 新累计充值2
// ------------------------------
void RoleActivity::OnRANewTotalCharge2OperaReq(int opera_type, int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2);

	switch (opera_type)
	{
	case Protocol::RA_NEW_TOTAL_CHARGE_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRANewTotalCharge2Info();
	}
	break;

	case Protocol::RA_NEW_TOTAL_CHARGE_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnRANewTotalCharge2FetchReward(seq);
	}
	break;
	}
}

void RoleActivity::OnRANewTotalChargeCheckChongzhi2(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2);

	m_activity_info.ra_new_total_charge2_value += (int)chongzhi;

	this->SendRANewTotalCharge2Info();
}

void RoleActivity::RACheckNewTotalCharge2GiveReward()
{
	return;//屏蔽补发奖励
	int last_activity_got_max_seq = LOGIC_CONFIG->GetRandActivityCfg().GetNewTotalCharge2SeqWithCharge(m_activity_info.ra_new_total_charge2_value);
	if (last_activity_got_max_seq < 0)
	{
		return;
	}

	for (int seq = 0; seq <= last_activity_got_max_seq; ++seq)
	{
		if (0 != (m_activity_info.ra_new_total_charge2_reward_has_fetch_flag & (1 << seq)))
		{
			continue;
		}

		const RandActivityNewTotalChargeRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetNewTotalCharge2RewardWithSeq(seq);
		if (NULL == cfg_item)
		{
			return;
		}

		{
			// 发邮件
			static MailContentParam contentparam; contentparam.Reset();

			int reward_item_count = 0;
			for (int i = 0; i < cfg_item->item_count && reward_item_count < MAX_ATTACHMENT_ITEM_NUM; ++i)
			{
				const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->item_list[i].item_id);
				if (NULL != reward_item_base)
				{
					contentparam.item_list[reward_item_count].item_id = cfg_item->item_list[i].item_id;
					contentparam.item_list[reward_item_count].num = cfg_item->item_list[i].num;
					contentparam.item_list[reward_item_count].is_bind = (cfg_item->item_list[i].is_bind) ? 1 : 0;
					contentparam.item_list[reward_item_count].invalid_time = reward_item_base->CalInvalidTime();
					++reward_item_count;
				}
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_charge_bu_reward_content,
				RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2);
			if (length > 0)
			{
				m_activity_info.ra_new_total_charge2_reward_has_fetch_flag |= (1 << seq);
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		{
			// 日志
			std::string get_item_log;
			ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

			gamelog::g_log_roleactivity.printf(LL_INFO, "RACheckNewTotalChargeGiveReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d], item_list[%s]",
				m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.ra_new_total_charge2_reward_has_fetch_flag, get_item_log.c_str());

			ROLE_LOG_QUICK6(LOG_TYPE_RAND_ACTIVITY_TOTAL_CHONGZHI, m_role, seq, m_activity_info.ra_new_total_charge2_reward_has_fetch_flag, NULL, NULL);
		}
	}
}

void RoleActivity::SendRANewTotalCharge2Info()
{
	Protocol::SCRATotalChargeMultiInfo tcgi;
	tcgi.activity_type = RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2;
	tcgi.reward_has_fetch_flag = m_activity_info.ra_new_total_charge2_reward_has_fetch_flag;
	tcgi.total_charge_value = m_activity_info.ra_new_total_charge2_value;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tcgi, sizeof(tcgi));
}

void RoleActivity::OnRANewTotalCharge2FetchReward(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2))
	{
		return;
	}

	if (seq < 0 || seq >= static_cast<int>(sizeof(m_activity_info.ra_new_total_charge2_reward_has_fetch_flag) * 8))
	{
		return;
	}

	if (0 != (m_activity_info.ra_new_total_charge2_reward_has_fetch_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_FETCH_REWARD);
		return;
	}

	const RandActivityNewTotalChargeRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetNewTotalCharge2RewardWithSeq(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_new_total_charge2_value < cfg_item->need_chongzhi)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_TOTAL_CHONGZHI_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg_item->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_new_total_charge2_reward_has_fetch_flag |= (1 << seq);
	this->SendRANewTotalCharge2Info();
	    
	m_role->GetKnapsack()->PutList(cfg_item->item_count, cfg_item->item_list, PUT_REASON_RA_TOTAL_CHARGE);

	if (cfg_item->item_count > 0)
	{
		// 累计充值传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_new_total_charge_reward_chuanwen_content2,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), cfg_item->need_chongzhi);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	{
		// 日志
		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

		gamelog::g_log_roleactivity.printf(LL_INFO, "OnRANewTotalCharge2FetchReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d], item_list[%s]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.ra_new_total_charge2_reward_has_fetch_flag, get_item_log.c_str());

		ROLE_LOG_QUICK6(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE, m_role, seq, m_activity_info.ra_new_total_charge2_reward_has_fetch_flag, NULL, NULL);
	}
}

// ------------------------------
// 新累计充值3
// ------------------------------
void RoleActivity::OnRANewTotalCharge3OperaReq(int opera_type, int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3);

	switch (opera_type)
	{
	case Protocol::RA_NEW_TOTAL_CHARGE_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRANewTotalCharge3Info();
	}
	break;

	case Protocol::RA_NEW_TOTAL_CHARGE_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnRANewTotalCharge3FetchReward(seq);
	}
	break;
	}
}

void RoleActivity::OnRANewTotalChargeCheckChongzhi3(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3);

	m_activity_info.ra_new_total_charge3_value += (int)chongzhi;

	this->SendRANewTotalCharge3Info();
}

void RoleActivity::RACheckNewTotalCharge3GiveReward()
{
	return;//屏蔽补发奖励
	int last_activity_got_max_seq = LOGIC_CONFIG->GetRandActivityCfg().GetNewTotalCharge3SeqWithCharge(m_activity_info.ra_new_total_charge3_value);
	if (last_activity_got_max_seq < 0)
	{
		return;
	}

	for (int seq = 0; seq <= last_activity_got_max_seq; ++seq)
	{
		if (0 != (m_activity_info.ra_new_total_charge3_reward_has_fetch_flag & (1 << seq)))
		{
			continue;
		}

		const RandActivityNewTotalChargeRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetNewTotalCharge3RewardWithSeq(seq);
		if (NULL == cfg_item)
		{
			return;
		}

		{
			// 发邮件
			static MailContentParam contentparam; contentparam.Reset();

			int reward_item_count = 0;
			for (int i = 0; i < cfg_item->item_count && reward_item_count < MAX_ATTACHMENT_ITEM_NUM; ++i)
			{
				const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->item_list[i].item_id);
				if (NULL != reward_item_base)
				{
					contentparam.item_list[reward_item_count].item_id = cfg_item->item_list[i].item_id;
					contentparam.item_list[reward_item_count].num = cfg_item->item_list[i].num;
					contentparam.item_list[reward_item_count].is_bind = (cfg_item->item_list[i].is_bind) ? 1 : 0;
					contentparam.item_list[reward_item_count].invalid_time = reward_item_base->CalInvalidTime();
					++reward_item_count;
				}
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_charge_bu_reward_content,
				RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3);
			if (length > 0)
			{
				m_activity_info.ra_new_total_charge3_reward_has_fetch_flag |= (1 << seq);
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		{
			// 日志
			std::string get_item_log;
			ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

			gamelog::g_log_roleactivity.printf(LL_INFO, "RACheckNewTotalCharge3GiveReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d], item_list[%s]",
				m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.ra_new_total_charge3_reward_has_fetch_flag, get_item_log.c_str());

			ROLE_LOG_QUICK6(LOG_TYPE_RAND_ACTIVITY_TOTAL_CHONGZHI, m_role, seq, m_activity_info.ra_new_total_charge3_reward_has_fetch_flag, NULL, NULL);
		}
	}
}

void RoleActivity::SendRANewTotalCharge3Info()
{
	Protocol::SCRATotalChargeMultiInfo tcgi;
	tcgi.activity_type = RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3;
	tcgi.reward_has_fetch_flag = m_activity_info.ra_new_total_charge3_reward_has_fetch_flag;
	tcgi.total_charge_value = m_activity_info.ra_new_total_charge3_value;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tcgi, sizeof(tcgi));
}

void RoleActivity::OnRANewTotalCharge3FetchReward(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3))
	{
		return;
	}

	if (seq < 0 || seq >= static_cast<int>(sizeof(m_activity_info.ra_new_total_charge3_reward_has_fetch_flag) * 8))
	{
		return;
	}

	if (0 != (m_activity_info.ra_new_total_charge3_reward_has_fetch_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_FETCH_REWARD);
		return;
	}

	const RandActivityNewTotalChargeRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetNewTotalCharge3RewardWithSeq(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_activity_info.ra_new_total_charge3_value < cfg_item->need_chongzhi)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_TOTAL_CHONGZHI_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg_item->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_new_total_charge3_reward_has_fetch_flag |= (1 << seq);
	this->SendRANewTotalCharge3Info();

	m_role->GetKnapsack()->PutList(cfg_item->item_count, cfg_item->item_list, PUT_REASON_RA_TOTAL_CHARGE);

	if (cfg_item->item_count > 0)
	{
		// 累计充值传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_new_total_charge_reward_chuanwen_content3,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), cfg_item->need_chongzhi);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	{
		// 日志
		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

		gamelog::g_log_roleactivity.printf(LL_INFO, "OnRANewTotalCharge3FetchReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d], item_list[%s]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.ra_new_total_charge3_reward_has_fetch_flag, get_item_log.c_str());

		ROLE_LOG_QUICK6(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE, m_role, seq, m_activity_info.ra_new_total_charge3_reward_has_fetch_flag, NULL, NULL);
	}
}

void RoleActivity::OnRAMagicShopOperaReq(int opera_type, int param_1, int param_2)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MAGIC_SHOP))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MAGIC_SHOP);

	switch (opera_type)
	{
	case Protocol::RA_MAGIC_SHOP_OPERA_TYPE_ALL_INFO:
		{
			this->SendRAMagicShopAllInfo();
		}
		break;

	case Protocol::RA_MAGIC_SHOP_OPERA_TYPE_BUY:
		{
			this->OnRAMagicShopBuyOpera(param_1, param_2);
		}
		break;

	default:
		break;
	}
}

void RoleActivity::OnRAMagicShopBuyOpera(int type, int index)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MAGIC_SHOP))
	{
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	switch (type)
	{
	case RandActivityMagicShopConfig::RA_MAGIC_SHOP_BUY_TYPE_BUY:
		{
			this->OnRAMagicShopBuyOne(index);
		}
		break;

	case RandActivityMagicShopConfig::RA_MAGIC_SHOP_BUY_TYPE_BUY3:	// 一键购买3个，8折
		{
			this->OnRAMagicShopBuyAll();
		}
		break;

	default:
		break;
	}

	this->SendRAMagicShopAllInfo();
}

void RoleActivity::OnRAMagicShopBuyOne(int index)
{
	if (index < 0 || index >= RA_MAGIC_SHOP_ITEM_MAX_INDEX) return;

	const RandActivityMagicShopConfig *magic_shop_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMagicShopCfg(index);
	if (NULL == magic_shop_cfg)
	{
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (0 != (m_activity_info.ra_magic_shop_buy_flag & (1 << index)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_BUY);
		return;
	}

	// 检查元宝
	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(magic_shop_cfg->need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 消耗元宝
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(magic_shop_cfg->need_gold, "RoleActivity::OnRAMagicShopBuyOrFetchOpera"))
	{
		return;
	}

	m_activity_info.ra_magic_shop_buy_flag |= (1 << index);

	m_role->GetKnapsack()->Put(magic_shop_cfg->reward_item, PUT_REASON_MAGIC_SHOP_BUY);

	{
		// 日志
		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAMagicShopBuyOne::user[%d, %s], index[%d], reward_flag[%d], gold[%d], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), index, m_activity_info.ra_magic_shop_buy_flag,
			magic_shop_cfg->need_gold, ItemConfigDataLog(&magic_shop_cfg->reward_item, NULL));

		ROLE_LOG_QUICK6(RAND_ACTIVITY_TYPE_MAGIC_SHOP, m_role, index, m_activity_info.ra_magic_shop_buy_flag, NULL, NULL);
	}
}

void RoleActivity::OnRAMagicShopBuyAll()
{
	static const int all_count = 3;
	const RandActivityMagicShopConfig *magic_shop_cfg = nullptr;

	// 是否已购买
	for (int i = 0; i < all_count; i++)
	{
		if (0 != (m_activity_info.ra_magic_shop_buy_flag & (1 << i)))
		{
			m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_BUY_ONE);
			return;
		}
	}

	// 检查元宝
	int all_need_gold = 0;
	for (int i = 0; i < all_count; i++)
	{
		magic_shop_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMagicShopCfg(i);
		if (NULL == magic_shop_cfg) return;

		all_need_gold += magic_shop_cfg->need_gold;
	}

	all_need_gold = static_cast<int>(all_need_gold * 0.8f);	//8折
	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(all_need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 消耗元宝
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(all_need_gold, "RoleActivity::OnRAMagicShopBuyAll"))
	{
		return;
	}

	for (int i = 0; i < all_count; i++)
	{
		magic_shop_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMagicShopCfg(i);
		if (NULL == magic_shop_cfg) return;

		// 记录购买标志
		m_activity_info.ra_magic_shop_buy_flag |= (1 << i);

		// 获得物品
		m_role->GetKnapsack()->PutOrMail(magic_shop_cfg->reward_item, PUT_REASON_MAGIC_SHOP_BUY);

		// 日志
		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAMagicShopBuyAll::user[%d, %s], index[%d], reward_flag[%d], gold[%d], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), i, m_activity_info.ra_magic_shop_buy_flag,
			magic_shop_cfg->need_gold, ItemConfigDataLog(&magic_shop_cfg->reward_item, NULL));

		ROLE_LOG_QUICK6(RAND_ACTIVITY_TYPE_MAGIC_SHOP, m_role, i, m_activity_info.ra_magic_shop_buy_flag, NULL, NULL);
	}
}

void RoleActivity::SendRAMagicShopAllInfo()
{
	static Protocol::SCRAMagicShopAllInfo msai;

	msai.magic_shop_buy_flag = m_activity_info.ra_magic_shop_buy_flag;
	msai.activity_day = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_MAGIC_SHOP), EngineAdapter::Instance().Time()) + 1;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&msai, sizeof(msai));
}

//----------------------------------
//趣味钓鱼
//----------------------------------
void RoleActivity::OnRAFishingOperaReq(int opera_type, int prama1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_FISHING))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_FISHING);

	switch (opera_type)
	{
	case Protocol::RA_FISHING_REQ:
	{
		this->OnFishingReq(prama1);
	}
	break;

	case Protocol::RA_FISHING_ACK:
	{
		this->FishingAck();
	}
	break;

	case Protocol::RA_FISHING_STEAL:
	{
		this->StealFishReq(prama1);
	}
	break;

	case Protocol::RA_FISHING_STEAL_ACK:
	{
		this->StealFishAck(prama1);
	}
	break;

	case Protocol::RA_FISHING_EXCHANGE:
	{
		this->OnExchangeFishReward(prama1);
	}
	break;

	case Protocol::RA_FISHING_CANCEL_AUTO_FISH:
	{
		this->CancelAutoFishing();
	}
	break;

	case Protocol::RA_FISHING_EXIT_FISHING:
	{
		this->ExitFishing();
	}
	break;

	case Protocol::RA_FISHING_ALL_INFO:
	{
		this->SendAllFishingInfo(0, 0);
	}
	break;

	default:
		break;
	}
}

void RoleActivity::OnFishingReq(int param1)
{
	if (LOGIC_CONFIG->GetRandActivityCfg().IsFishingTimeLimit(m_activity_info.ra_fishing_times))
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_FISHING_REQ, 0);
		m_role->NoticeNum(noticenum::NT_FISHING_TIME_LIMIT);
		return;
	}

	if (0 != param1)
	{
		m_is_auto_fishing = true;
	}

	m_is_fishing = true;
	m_role->SendOperateResult(Protocol::SCOperateResult::OP_FISHING_REQ, 1);
	m_role->SendFishingStation();
}

void RoleActivity::FishingAck()
{
	if (!m_is_fishing && !m_is_auto_fishing)
	{
		return;
	}

	if (LOGIC_CONFIG->GetRandActivityCfg().IsFishingTimeLimit(m_activity_info.ra_fishing_times))
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_FISHING_REQ, 0);
		m_role->NoticeNum(noticenum::NT_FISHING_TIME_LIMIT);
		return;
	}

	RandActivityFishingInfoCfg *fish_info = LOGIC_CONFIG->GetRandActivityCfg().GetFishinfo();
	if (NULL == fish_info)
	{
		return;
	}

	//设置钓鱼相关数据
	m_activity_info.ra_fishing_times++;
	m_activity_info.ra_fishing_score += fish_info->fish_score;
	m_activity_info.ra_fish_info_list[fish_info->fish_type] += 1;
	m_activity_info.ra_fishing_fishing_timestamp = (unsigned int)EngineAdapter::Instance().Time();

	this->SendAllFishingInfo(fish_info->fish_type, 0);

	//传闻
	if (fish_info->is_broadcast)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_fishing_broadcast,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), fish_info->fish_type);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}
}

void RoleActivity::StealFishReq(int uid)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (NULL == target_role)
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_STEAL_FISH_RESULT, 0);
		m_role->NoticeNum(noticenum::NT_ROLE_LOGOUT);
		return;
	}

	if (!target_role->GetRoleActivity()->m_is_auto_fishing && !target_role->GetRoleActivity()->m_is_fishing)
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_STEAL_FISH_RESULT, 0);
		m_role->NoticeNum(noticenum::NT_IS_NOT_FISHING);
		return;
	}

	if (LOGIC_CONFIG->GetRandActivityCfg().IsStealFishTimeLimit(m_activity_info.ra_steal_fish_times))
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_STEAL_FISH_RESULT, 0);
		m_role->NoticeNum(noticenum::NT_STEAL_FISH_TIEM_LIMIT);
		return;
	}

	if (LOGIC_CONFIG->GetRandActivityCfg().IsBeStealFishTimeLimit(target_role->GetRoleActivity()->m_activity_info.ra_be_steal_fish_times))
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_STEAL_FISH_RESULT, 0);
		m_role->NoticeNum(noticenum::NT_BE_STEAL_FISH_TIME_LIMIT);
		return;
	}

	m_role->SendOperateResult(Protocol::SCOperateResult::OP_STEAL_FISH_RESULT, 1);
}

void RoleActivity::StealFishAck(int uid)
{
	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (NULL == target_role)
	{
		return;
	}

	if (LOGIC_CONFIG->GetRandActivityCfg().IsStealFishTimeLimit(m_activity_info.ra_steal_fish_times))
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_STEAL_FISH_RESULT, 0);
		m_role->NoticeNum(noticenum::NT_STEAL_FISH_TIEM_LIMIT);
		return;
	}

	//只要读条过来了就扣除次数
	m_activity_info.ra_steal_fish_times++;
	target_role->GetRoleActivity()->m_activity_info.ra_be_steal_fish_times++;

	const short steal_fish_persent = LOGIC_CONFIG->GetRandActivityCfg().GetStealFishPersent();
	int rand_num = RandomNum(100);
	if (rand_num > steal_fish_persent)
	{
		this->SendAllFishingInfo(0, -1);
		target_role->GetRoleActivity()->SendAllFishingInfo(0, 0);
		m_role->NoticeNum(noticenum::NT_STEAL_FISH_FAIL);
		return;
	}

	short fish_type_num = 0;
	short fish_type = 0;
	short fish_type_list[RA_FISHING_FISH_TYPE_COUNT + 1];
	memset(fish_type_list, 0, sizeof(fish_type_list));
	for (int i = 0; i < RA_FISHING_FISH_TYPE_COUNT; i++)
	{
		if (target_role->GetRoleActivity()->m_activity_info.ra_fish_info_list[i] > 0)
		{
			fish_type_list[fish_type_num] = i;
			fish_type_num++;
		}
	}

	if (fish_type_num <= 0)
	{
		this->SendAllFishingInfo(0, -1);
		target_role->GetRoleActivity()->SendAllFishingInfo(0, 0);
		m_role->NoticeNum(noticenum::NT_HAVE_NO_FISH);

		//没偷到鱼，只记录首偷时间
		if (m_activity_info.ra_fishing_steal_timestamp == 0)
		{
			m_activity_info.ra_fishing_steal_timestamp = (unsigned int)EngineAdapter::Instance().Time();
		}
		return;
	}
	else
	{
		int rand_num = RandomNum(fish_type_num);
		fish_type = fish_type_list[rand_num];

		RandActivityFishingInfoCfg *fish_info = LOGIC_CONFIG->GetRandActivityCfg().GetFishinfobyType(fish_type);
		if (NULL == fish_info)
		{
			return;
		}

		m_activity_info.ra_fish_info_list[fish_type] += 1;
		m_activity_info.ra_steal_fish_score += fish_info->be_steal_fish_score;
		m_activity_info.ra_fishing_steal_timestamp = (unsigned int)EngineAdapter::Instance().Time();
		target_role->GetRoleActivity()->m_activity_info.ra_fish_info_list[fish_type] -= 1;

		this->SendAllFishingInfo(0, fish_type);
	}

	target_role->GetRoleActivity()->SendAllFishingInfo(0, 0);

	//发邮件提醒被偷玩家
	static MailContentParam contentparam; contentparam.Reset();

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_be_steal_fish_content,
		m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), fish_type);
	if (length > 0)
	{
		MailRoute::MailToUser(target_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
	}
}

void RoleActivity::OnExchangeFishReward(int reward_type)
{
	if (reward_type < 0 || reward_type > RA_FISHING_FISH_TYPE_COUNT)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (NULL == knapsack || !knapsack->HaveSpace() || knapsack->IsLock())
	{
		return;
	}

	RandActivityFishingInfoCfg *fish_info = LOGIC_CONFIG->GetRandActivityCfg().GetFishinfobyType(reward_type);	//奖励类型与鱼的类型对应
	if (NULL == fish_info)
	{
		return;
	}

	if (m_activity_info.ra_fish_info_list[reward_type] < fish_info->fish_num)
	{
		m_role->NoticeNum(noticenum::NT_FISH_NOT_ENOUGH);
		return;
	}

	m_activity_info.ra_fish_info_list[reward_type] -= fish_info->fish_num;

	knapsack->PutOrMail(fish_info->reward_item, PUT_REASON_FISHING_EXCHANGE);

	this->SendAllFishingInfo(0, 0);

	gamelog::g_log_fishing_exchange.printf(LL_INFO, "OnExchangeFishReward::item_id[%d], num[%d], fish_type[%d]",
		fish_info->reward_item.item_id, fish_info->reward_item.num, reward_type);
}

void RoleActivity::CancelAutoFishing()
{
	m_is_auto_fishing = false;
	m_role->SendFishingStation();
}

void RoleActivity::ExitFishing()
{
	m_is_fishing = false;
	m_is_auto_fishing = false;
	m_role->SendFishingStation();
}

short RoleActivity::GetFishingscore()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_FISHING);
	return m_activity_info.ra_fishing_score;
}

short RoleActivity::GetStealfishscore()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_FISHING);
	return m_activity_info.ra_steal_fish_score;
}

unsigned int RoleActivity::GetFishingFishingTimestamp()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_FISHING);
	return m_activity_info.ra_fishing_fishing_timestamp;
}

unsigned int RoleActivity::GetFishingStealTimestamp()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_FISHING);
	return m_activity_info.ra_fishing_steal_timestamp;
}

void RoleActivity::SendAllFishingInfo(short fish_type, short steal_fish_type)
{
	static Protocol::SCRAFishAllinfo fai;

	fai.fish_times = m_activity_info.ra_fishing_times;
	fai.steal_fish_times = m_activity_info.ra_steal_fish_times;
	fai.be_steal_fish_times = m_activity_info.ra_be_steal_fish_times;
	fai.steal_fish_type = steal_fish_type;
	fai.fish_score = m_activity_info.ra_fishing_score;
	fai.steal_fish_score = m_activity_info.ra_steal_fish_score;
	fai.fish_type = fish_type;
	memcpy(fai.fish_info, m_activity_info.ra_fish_info_list, sizeof(fai.fish_info));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&fai, sizeof(Protocol::SCRAFishAllinfo));
}

char RoleActivity::GetFishingStation()
{
	char fishing_station = 0;
	if (m_is_auto_fishing || m_is_fishing)
	{
		fishing_station = 1;
	}

	return fishing_station;
}

void RoleActivity::RACheckPlantingTreeCanyuReward()
{
	if (m_activity_info.ra_planting_tree_planting_times <= 0 && m_activity_info.ra_planting_tree_watering_times <= 0)
	{
		return;
	}

	// 判断是否有排行奖励
	{
		bool is_in_rank = false;
		int planting_rank = RankManager::Instance().GetPersonRank()->GetUserRank(m_role->GetUserId(), PERSON_RANK_TYPE_PLANTING_TREE_PLANTING);
		int watering_rank = RankManager::Instance().GetPersonRank()->GetUserRank(m_role->GetUserId(), PERSON_RANK_TYPE_PLANTING_TREE_WATERING);
		if (planting_rank > 0 && planting_rank <= RA_TREE_PLANTING_RANK_MAX_COUNT)
		{
			is_in_rank = true;
		}

		if (watering_rank > 0 && watering_rank <= RA_TREE_PLANTING_RANK_MAX_COUNT)
		{
			is_in_rank = true;
		}

		if (is_in_rank)
		{
			return;
		}
	}

	ItemConfigData canyu_reward = LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeCanyuReward();

	static MailContentParam contentparam; contentparam.Reset();

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(canyu_reward.item_id);
	if (NULL == reward_item_base)
	{
		return;
	}

	contentparam.item_list[0].item_id = canyu_reward.item_id;
	contentparam.item_list[0].num = canyu_reward.num;
	contentparam.item_list[0].is_bind = (canyu_reward.is_bind) ? 1 : 0;
	contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_planting_tree_canyu_reward, RAND_ACTIVITY_TYPE_PLANTING_TREE);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		// 发送完奖励清零活动数据
		m_activity_info.ra_planting_tree_planting_times = 0;
		m_activity_info.ra_planting_tree_watering_times = 0;

		gamelog::g_log_roleactivity.printf(LL_INFO, "RACheckPlantingTreeCanyuReward::user[%d %s], reward[%s]", m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&canyu_reward, NULL));
	}
}

void RoleActivity::OnRAPlantingTreeOperaReq(int opera_type, int param1, int param2)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PLANTING_TREE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_PLANTING_TREE);

	switch (opera_type)
	{
	case Protocol::RA_PLANTING_TREE_OPERA_TYPE_RANK_INFO:
	{
		this->SendRAPlantingTreeRankInfo(param1);
	}
	break;

	case Protocol::RA_PLANTING_TREE_OPERA_TYPE_TREE_INFO:
	{
		this->SendRAPlantingTreeTreeInfo(param1, param2);
	}
	break;

	case Protocol::RA_PLANTING_TREE_OPERA_MINI_TYPE_MAP_INFO:
	{
		this->SendRAPlantingTreeMiniMapInfo(param1);
	}
	break;
	}
}

void RoleActivity::SendRAPlantingTreeRankInfo(int rank_type)
{
	static Protocol::SCPlantingTreeRankInfo ptai;

	static PersonRankItem planting_rank_list[RA_TREE_PLANTING_RANK_MAX_COUNT];
	int count = 0;

	switch (rank_type)
	{
	case PERSON_RANK_TYPE_PLANTING_TREE_PLANTING:
	{
		count = RankManager::Instance().GetPersonRank()->GetRankList(PERSON_RANK_TYPE_PLANTING_TREE_PLANTING, RA_TREE_PLANTING_RANK_MAX_COUNT, planting_rank_list);

		ptai.rank_type = PERSON_RANK_TYPE_PLANTING_TREE_PLANTING;
		ptai.opera_times = m_activity_info.ra_planting_tree_planting_times;
	}
	break;

	case PERSON_RANK_TYPE_PLANTING_TREE_WATERING:
	{
		count = RankManager::Instance().GetPersonRank()->GetRankList(PERSON_RANK_TYPE_PLANTING_TREE_WATERING, RA_TREE_PLANTING_RANK_MAX_COUNT, planting_rank_list);

		ptai.rank_type = PERSON_RANK_TYPE_PLANTING_TREE_WATERING;
		ptai.opera_times = m_activity_info.ra_planting_tree_watering_times;
	}
	break;
	}

	ptai.rank_list_count = count;

	for (int index = 0; index < count && index < RA_TREE_PLANTING_RANK_MAX_COUNT; ++index)
	{
		ptai.rank_list[index].uid = UidToInt(planting_rank_list[index].user_id);
		F_STRNCPY(ptai.rank_list[index].name, planting_rank_list[index].user_name, sizeof(GameName));
		ptai.rank_list[index].opera_items = (int)planting_rank_list[index].rank_value;
		ptai.rank_list[index].prof = planting_rank_list[index].prof;
		ptai.rank_list[index].sex = planting_rank_list[index].sex;
	}

	int send_len = sizeof(ptai) - (RA_TREE_PLANTING_RANK_MAX_COUNT - ptai.rank_list_count) * sizeof(ptai.rank_list[0]);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ptai, send_len);
}

void RoleActivity::SendRAPlantingTreeTreeInfo(int scene_id, ObjID obj_id)
{
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (NULL == scene)
	{
		return;
	}

	Obj *obj = scene->GetObj(obj_id);
	if (NULL == obj)
	{
		return;
	}

	if (Obj::OBJ_TYPE_GATHER != obj->GetObjType())
	{
		return;
	}

	GatherObj *tree = (GatherObj*)obj;
	if (tree->GatherId() != LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeGatherID())
	{
		return;
	}

	RandActivityPlantingTree *planting_tree_act = (RandActivityPlantingTree*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_PLANTING_TREE);
	if (NULL == planting_tree_act)
	{
		return;
	}

	const RandActivityPlantingTree::TreeInfo *tree_info = planting_tree_act->GetTreeInfo(ConvertParamToLongLong(scene->GetSceneID(), tree->GetId()));
	if (NULL == tree_info)
	{
		return;
	}

	static Protocol::SCPlantingTreeTreeInfo ptti;
	F_STRNCPY(ptti.owner_name, tree_info->owner_name, sizeof(GameName));
	ptti.vanish_time = tree_info->vanish_time;
	ptti.watering_times = tree_info->watering_times;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ptti, sizeof(ptti));
}

void RoleActivity::SendRAPlantingTreeMiniMapInfo(int scene_id)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PLANTING_TREE))
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (NULL == scene)
	{
		return;
	}

	if (!Scene::IsStaticScene(scene->GetSceneType()))
	{
		return;
	}

	static Protocol::SCPlantingTreeMiniMapInfo ptmmi;

	Axis rang_x = 0;
	Axis rang_y = 0;
	scene->GetMap()->GetArea(&rang_x, &rang_y);

	static ObjID obj_list[256] = { 0 };
	int count = scene->GetZoneMatrix()->GetObjByArea(Posi(0, 0), rang_x, rang_y, obj_list, 256);

	int tree_count = 0;
	for (int i = 0; tree_count < Protocol::PLANTING_TREE_MINI_MAP_MAX_TREE_COUNT && i < count; ++i)
	{
		Obj *obj = scene->GetObj(obj_list[i]);
		if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
		{
			GatherObj *gather_obj = (GatherObj*)obj;
			if (LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeGatherID() == gather_obj->GatherId())
			{
				ptmmi.tree_info_list[tree_count].obj_id = obj->GetId();
				ptmmi.tree_info_list[tree_count].pos_x = obj->GetPos().x;
				ptmmi.tree_info_list[tree_count].pos_y = obj->GetPos().y;

				++tree_count;
			}
		}
	}

	if (tree_count > 0)
	{
		ptmmi.tree_count = tree_count;

		int send_len = sizeof(ptmmi) - (Protocol::PLANTING_TREE_MINI_MAP_MAX_TREE_COUNT - ptmmi.tree_count) * sizeof(ptmmi.tree_info_list[0]);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ptmmi, send_len);
	}
}

bool RoleActivity::OnRAPlantingTreePlanting(ItemID item_id)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PLANTING_TREE))
	{
		if (m_role->IsInCross())
		{
			gstr::SendError(m_role, "planting_tree_not_suit_scene");
		}
		else
		{
			gstr::SendError(m_role, "planting_tree_activity_not_begin");
		}

		return false;
	}

	// 检查道具id
	if (item_id != LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeNorexItemID())
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return false;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_PLANTING_TREE);

	if (CrossConfig::Instance().IsHiddenServer())
	{
		gstr::SendError(m_role, "planting_tree_can_not_planting");
		return false;
	}

	// 检查玩家状态
	if (Role::MOVE_MODE_FLY == m_role->GetMoveMode())
	{
		gstr::SendError(m_role, "planting_tree_can_not_planting");
		return false;
	}

	if (RoleStatusManager::ROLE_ACTION_STATUS_IDLE != m_role->GetRoleStatusManager()->GetRoleActionStatus())
	{
		gstr::SendError(m_role, "planting_tree_can_not_planting");
		return false;
	}

	// 检查场景
	Scene *scene = m_role->GetScene();
	if (NULL == scene)
	{
		return false;
	}

	if (scene->GetSceneID() != LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeSceneid())
	{
		gstr::SendError(m_role, "planting_tree_not_suit_scene");
		return false;
	}

	RandActivityPlantingTree *planting_tree_act = (RandActivityPlantingTree*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_PLANTING_TREE);
	if (NULL == planting_tree_act)
	{
		return false;
	}

	// 限制个人当前种的树的数目
	const static int MAX_PLANTING_COUNT = 30;
	if (planting_tree_act->GetRolePlantingTreeCount(m_role->GetUID()) >= MAX_PLANTING_COUNT)
	{
		gstr::SendError(m_role, "planting_tree_planting_count_full");
		return false;
	}

	// 计算欲种树的坐标
	int pos_x = 0;
	int pos_y = 0;
	{
		int offset_x = 0;
		int offset_y = 0;
		static const int offset_val = 3;

		Dir dir = m_role->GetDir();
		static const float angle_45_to_redian = Dir_Max / 4;
		static const float angle_135_to_redian = Dir_Max / 4 * 3;

		if (dir >= -angle_45_to_redian && dir < angle_45_to_redian)				// 面朝右
		{
			offset_x = offset_val;
		}
		else if (dir > angle_45_to_redian && dir <= angle_135_to_redian)		// 面朝上
		{
			offset_y = offset_val;
		}
		else if (dir > angle_135_to_redian && dir <= Dir_Max)					// 面朝左
		{
			offset_x = -offset_val;
		}
		else if (dir > -Dir_Max && dir <= -angle_135_to_redian)
		{
			offset_x = -offset_val;
		}
		else if (dir > -angle_135_to_redian && dir < -angle_45_to_redian)		// 面朝下
		{
			offset_y = -offset_val;
		}

		pos_x = m_role->GetPos().x + offset_x;
		pos_y = m_role->GetPos().y + offset_y;

		// 不能种植在非法区域
		if ((!scene->GetMap()->Validate(m_role->GetObjType(), pos_x, pos_y)) || (dir != Dir_S && fabs(dir) > Dir_Max) /* _dir >= Dir_Count*/)
		{
			gstr::SendError(m_role, "planting_tree_can_not_planting");
			return false;
		}
	}

	// 检查种树位置是否有已经有其它采集物了，避免重叠
	{
		static ObjID obj_list[256] = { 0 };
		const static int TREE_AOI_RANGE = 3;
		int count = scene->GetZoneMatrix()->GetObjByArea(Posi(pos_x, pos_y), TREE_AOI_RANGE, TREE_AOI_RANGE, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));
		for (int i = 0; i < count; i++)
		{
			Obj *obj = scene->GetObj(obj_list[i]);
			if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
			{
				gstr::SendError(m_role, "planting_tree_can_not_planting");
				return false;
			}
		}
	}

	// 种下一棵树
	{
		const int watering_time_s = LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeWaterTime();

		ObjID gather_id = LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeGatherID();
		const static int gather_time = watering_time_s * 1000;
		unsigned int living_time = LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeTreeLivingTime() * SECOND_PER_MIN;
		int max_watering_times = LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeMaxWateringTimes();

		GatherObj *gather_obj = new GatherObj();
		gather_obj->Init(NULL, gather_id, gather_time, 0, true, living_time, max_watering_times);
		gather_obj->SetPos(Posi(pos_x, pos_y));
		scene->AddObj(gather_obj);

		GameName owner_name; m_role->GetName(owner_name);
		planting_tree_act->OnPlantingTree(scene->GetSceneID(), gather_obj->GetId(), owner_name, m_role->GetUID());

		++m_activity_info.ra_planting_tree_planting_times;
	}

	// 刷新排行榜
	{
		m_role->GetRankShadow()->ForceSyncData();

		PersonRank *person_rank = RankManager::Instance().GetPersonRank();
		if (NULL != person_rank)
		{
			person_rank->ForceSnapshot();
		}
	}

	gstr::SendNotice(m_role, "planting_tree_planting_succ");

	// 日志
	gamelog::g_log_roleactivity.printf(LL_INFO, "OnRAPlantingTreePlanting user[%d %s], scene_id[%d], pos[(%d, %d)], planting_times[%d]", m_role->GetUID(), m_role->GetName(),
		scene->GetSceneID(), pos_x, pos_y, m_activity_info.ra_planting_tree_planting_times);

	return true;
}

void RoleActivity::OnRAPlantingTreeWatering(int gather_id, ObjID obj_id)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PLANTING_TREE))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_PLANTING_TREE);

	// 检查
	if (gather_id != LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeGatherID())
	{
		return;
	}

	RandActivityPlantingTree *planting_tree_act = (RandActivityPlantingTree*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_PLANTING_TREE);
	if (NULL == planting_tree_act)
	{
		return;
	}

	Scene *scene = m_role->GetScene();
	if (NULL == scene)
	{
		return;
	}

	if (!planting_tree_act->OnWateringTree(scene->GetSceneID(), obj_id, m_role->GetUID()))
	{
		return;
	}

	// 给予
	++m_activity_info.ra_planting_tree_watering_times;

	// 刷新排行榜
	{
		m_role->GetRankShadow()->ForceSyncData();

		PersonRank *person_rank = RankManager::Instance().GetPersonRank();
		if (NULL != person_rank)
		{
			person_rank->ForceSnapshot();
		}
	}

	gstr::SendNotice(m_role, "planting_tree_watering_succ");

	// 日志
	gamelog::g_log_roleactivity.printf(LL_INFO, "OnRAPlantingTreeWatering user[%d %s], watering_times[%d]", m_role->GetUID(), m_role->GetName(), m_activity_info.ra_planting_tree_watering_times);
}

bool RoleActivity::CanRAPlantingTreeWatering(int gather_id, int obj_id)
{
	// 浇水（采集）对象必须是树
	if (gather_id == LOGIC_CONFIG->GetRandActivityCfg().GetPlantingTreeGatherID())
	{
		// 活动必须开启
		if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PLANTING_TREE))
		{
			gstr::SendError(m_role, "planting_tree_activity_not_begin");
			return false;
		}

		Scene *scene = m_role->GetScene();
		if (NULL == scene)
		{
			return false;
		}

		RandActivityPlantingTree *planting_tree_act = (RandActivityPlantingTree*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_PLANTING_TREE);
		if (NULL == planting_tree_act)
		{
			return false;
		}

		// 玩家对此树浇过水，则不能再浇（采集）
		if (planting_tree_act->HasRoleWateredTree(m_role->GetUID(), ConvertParamToLongLong(scene->GetSceneID(), obj_id)))
		{
			gstr::SendError(m_role, "planting_tree_can_not_watering");
			return false;
		}
	}

	return true;
}

void RoleActivity::OnRACrystalDalaranOperaReq(int opera_type, int prama1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN);

	switch (opera_type)
	{
	case Protocol::RA_STEAL_CRTSTAL_DALARAN_REQ_INFO:
		{
			this->SendStealCrystalDalaranInfo();
		}
		break;

	case Protocol::RA_STEAL_CRYSTAL_DALARAN_REQ:
		{
			this->StealCrystalDalaran(prama1);
		}
		break;

	default:
		break;
	}
}

void RoleActivity::SendStealCrystalDalaranInfo()
{
	static Protocol::SCRAStealCrystalDalatanInfo sc_info;

	sc_info.steal_count = m_activity_info.ra_steal_dalaran_count;
	sc_info.be_steal_count = m_activity_info.ra_dalaran_be_stealed_count;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sc_info, sizeof(sc_info));
}

void RoleActivity::StealCrystalDalaran(int uid)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (NULL == target_role)
	{
		return;
	}

	if (LOGIC_CONFIG->GetRandActivityCfg().IsStealTimesLimit(m_activity_info.ra_steal_crystal_dalaran_timestamp))
	{
		m_role->NoticeNum(noticenum::NT_STEAL_TIMES_LIMIT);
		return;
	}

	short be_steal_times = target_role->GetRoleActivity()->m_activity_info.ra_crystal_dalaran_be_stealed_times;
	if (LOGIC_CONFIG->GetRandActivityCfg().IsBeStealTimesLimit(be_steal_times))
	{
		m_role->NoticeNum(noticenum::NT_STEAL_COIN_LIMIT);
		return;
	}

	const int target_be_steal_times = this->GetStealTimesByUid(uid);
	if (LOGIC_CONFIG->GetRandActivityCfg().IsOnePeopleBeStealTimesLimit(target_be_steal_times) || target_be_steal_times < 0)
	{
		m_role->NoticeNum(noticenum::NT_STEAL_ONE_PEOPLE_COUNT_LIMIT);
		return;
	}

	RoleActivity *target_role_act = target_role->GetRoleActivity();
	if (NULL == target_role_act)
	{
		return;
	}

	unsigned int coin_num = target_role_act->GetCrytstalDalaranCount();
	const short steal_percent = LOGIC_CONFIG->GetRandActivityCfg().GetStealCrystalDalaranPercent();
	const int steal_num_limit = LOGIC_CONFIG->GetRandActivityCfg().GetStealCrystalDalaranCountLimit();
	unsigned int tmp_num = static_cast<unsigned int>(coin_num * steal_percent / 100.0);
	if (tmp_num <= 0)
	{
		m_role->NoticeNum(noticenum::NT_HAS_NOT_ENOUGH_CRYSTAL_DALARAN);
		return;
	}

	m_activity_info.ra_steal_crystal_dalaran_times++;
	this->SetBeStealInfoList(uid);
	m_activity_info.ra_steal_crystal_dalaran_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	target_role_act->m_activity_info.ra_crystal_dalaran_be_stealed_times++;
	target_role_act->m_activity_info.ra_crystal_dalaran_be_stealed_timestamp = m_activity_info.ra_steal_crystal_dalaran_timestamp;

	unsigned int real_count = static_cast<unsigned int>(tmp_num > static_cast<unsigned int>(steal_num_limit) ? steal_num_limit : tmp_num);
	if (real_count > 0)
	{
		m_role->GetRoleActivity()->OnCrystalDalaranChange(real_count, "steal_add");
		m_activity_info.ra_steal_dalaran_count += real_count;
		target_role_act->OnCrystalDalaranChange(real_count, "be_stealed_reduce");
		target_role_act->m_activity_info.ra_dalaran_be_stealed_count += real_count;
	}

	static MailContentParam contentparam; contentparam.Reset();

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_be_steal_mail_content,
		m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), real_count);
	if (length > 0)
	{
		MailRoute::MailToUser(target_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
	}

	const short broadcast_percent = LOGIC_CONFIG->GetRandActivityCfg().GetStealCrystalDalaranBroadcastpercent();
	int radom = RandomNum(100);
	if (radom <= broadcast_percent)
	{
		int len = 0;
		int num = RandomNum(2);
		switch (num)
		{
		case 0:
		{
			len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_steal_crystal_dalaran_broadcast1,
				UidToInt(target_role->GetUserId()), target_role->GetName(), target_role->GetCamp(), UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), real_count);

			if (len > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
			}
		}
		break;

		case 1:
		{
			len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_steal_crystal_dalaran_broadcast2,
				UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), UidToInt(target_role->GetUserId()), target_role->GetName(), target_role->GetCamp(), real_count);
			if (len > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
			}
		}
		break;

		default:
			break;
		}
	}

	if (LOGIC_CONFIG->GetRandActivityCfg().IsBeStealTimesLimit(be_steal_times + 1))
	{
		int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_steal_crystal_dalaran_broadcast3,
			UidToInt(target_role->GetUserId()), target_role->GetName(), target_role->GetCamp());

		if (len > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
		}
	}
}

const int RoleActivity::GetStealTimesByUid(int uid)
{
	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (NULL == target_role)
	{
		return -1;
	}

	for (int i = 0; i < RA_CRYSTA_DALARAN_STEAL_USER_MAX_PER_DAY; i++)
	{
		if (uid == m_activity_info.ra_be_stealed_info[i].uid)
		{
			return m_activity_info.ra_be_stealed_info[i].be_steal_times;
		}
	}

	return 0;
}

void RoleActivity::SetBeStealInfoList(int uid)
{
	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (NULL == target_role)
	{
		return;
	}

	for (int i = 0; i < RA_CRYSTA_DALARAN_STEAL_USER_MAX_PER_DAY; i++)
	{
		if (uid == m_activity_info.ra_be_stealed_info[i].uid)
		{
			m_activity_info.ra_be_stealed_info[i].be_steal_times++;
			break;
		}

		else if (0 == m_activity_info.ra_be_stealed_info[i].uid)
		{
			m_activity_info.ra_be_stealed_info[i].uid = uid;
			m_activity_info.ra_be_stealed_info[i].be_steal_times = 1;
			break;
		}
	}
}

void RoleActivity::OnCrystalDalaranChange(int change_val, const char *reason)
{
	if (0 == change_val || NULL == reason) return;

	m_activity_info.ra_crystal_dalaran_count += change_val;

	if (m_activity_info.ra_crystal_dalaran_count < 0)
	{
		m_activity_info.ra_crystal_dalaran_count = 0;
	}

	gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnCrystalDalaranChange user[%d, %s] change_val[%d], after_val[%d]",
		m_role->GetUID(), m_role->GetName(), change_val, m_activity_info.ra_crystal_dalaran_count);
}

void RoleActivity::OnRAChongzhiCrazyRebate(long long gold_num)
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHONGZHI_CRAZY_REBATE);

	int charge_num = static_cast<int>(gold_num);

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_CRAZY_REBATE))
	{
		m_activity_info.ra_chongzhi_crazy_rebate_day_chongzhi_total_count += charge_num;

		int total_chongzhi = m_activity_info.ra_chongzhi_crazy_rebate_day_chongzhi_total_count;
		int cur_rebate_chongzhi = m_activity_info.ra_chongzhi_crazy_rebate_had_rebete_count;
		int rebate_gold = LOGIC_CONFIG->GetRandActivityCfg().GetChongzhiCrazyRebateRewardGold(total_chongzhi, cur_rebate_chongzhi);

		if (rebate_gold > 0)
		{
			static MailContentParam contentparam; contentparam.Reset();
			contentparam.gold = rebate_gold;

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_chongzhi_crazy_rebate, charge_num, contentparam.gold);
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAChongzhiCrazyRebate::user[%d, %s], charge_reward_gold[%d]",
					m_role->GetUID(), m_role->GetName(), contentparam.gold);
			}

			m_activity_info.ra_chongzhi_crazy_rebate_had_rebete_count = m_activity_info.ra_chongzhi_crazy_rebate_day_chongzhi_total_count;
		}
	}

	this->SendRaCrazyRebateChongzhiInfo();
}

void RoleActivity::SendRaCrazyRebateChongzhiInfo()
{
	Protocol::SCRaCrazyRebateChongInfo info;

	info.chongzhi_count = m_activity_info.ra_chongzhi_crazy_rebate_day_chongzhi_total_count;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}

void RoleActivity::OnNewGuildBattle(int opear_type)
{
	switch (opear_type)
	{
	case OGA_GUILD_BATTLE_OPERA_TYPE_INFO:
	{
		this->OnGuidBattleResultUpdate();
		this->SendOpenGameActivityInfo();
	}
	break;

	case OGA_GUILD_BATTLE_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnFetchOGATotalGuidBattleReward();
		this->SendOpenGameActivityInfo();
	}
	break;
	}
}

void RoleActivity::OnGuidBattleResultUpdate()
{
	if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE))
	{
		return;
	}

	//获取奖励配置
	const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(RAND_ACTIVITY_TYPE_GUILDBATTLE, 1);
	if (NULL == cfg)
	{
		return;
	}

	Guild * guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (guild == NULL)
	{
		return;
	}

	//奖励类型
	int type = -1;
	if (guild->GetJoinNewGuildBattleFlag() == 1)
	{
		if (guild->GetGuildID()== WorldStatus::Instance().GetGBMengzhuGuildID())
		{
			if (UidToInt(m_role->GetUserId()) == WorldStatus::Instance().GetOpenServerGuildBattelID())
			{
				type = OGA_GUILD_BATTLE_REWARD_TYPE_WINNER_BANGZHU;
			}
			else
			{
				type = OGA_GUILD_BATTLE_REWARD_TYPE_WINNER_OWENER;
			}
		}
		else if (UidToInt(m_role->GetUserId()) == guild->GetTuanzhuangRoleId())
		{
			type = OGA_GUILD_BATTLE_REWARD_TYPE_LOSER_BANGZHU;
		}
		else
		{
			type = OGA_GUILD_BATTLE_REWARD_TYPE_LOSER_OTHER;
		}
	}

	m_activity_info.ra_guild_battle_reward_type = type;
}

bool RoleActivity::OnFetchOGATotalGuidBattleReward()
{
	if (0 != m_activity_info.ra_guild_battle_reward_flag)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_FETCH_REWARD);
		return false;
	}

	if (m_activity_info.ra_guild_battle_reward_type < 0)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_GUILD_BATTLE_NOT_END);
	}

	//获取奖励配置
	const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(RAND_ACTIVITY_TYPE_GUILDBATTLE, 
																																m_activity_info.ra_guild_battle_reward_type);
	if (NULL == cfg)
	{
		return false;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg->reward_item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	if (!m_role->GetKnapsack()->PutList(cfg->reward_item_count, cfg->reward_item_list, PUT_REASON_OPENGAME_ACTIVITY))
	{
		return false;
	}

	m_activity_info.ra_guild_battle_reward_flag = 1;

	{
		// 日志
		gamelog::g_log_roleactivity.printf(LL_INFO, "[RoleActivity::OnFetchOGATotalGuidBattleReward] user[%s %d] item_configdata[%s] type[%d]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), ItemConfigDataLog(&cfg->reward_item_list[0], NULL), m_activity_info.ra_guild_battle_reward_type);
	}

	return true;
}

void RoleActivity::OnOGACheckGuildBattleReward()
{
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_GUILDBATTLE))
	{
		return;
	}

	this->OnGuidBattleResultUpdate();

	if (m_activity_info.ra_guild_battle_reward_type < 0)
	{
		return;
	}

	//获取奖励配置
	const RandActivityOpenServerRewardItemConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityOpenServerRewardConfig(RAND_ACTIVITY_TYPE_GUILDBATTLE,
		m_activity_info.ra_guild_battle_reward_type);
	if (NULL == cfg)
	{
		return;
	}

	if (0 != m_activity_info.ra_guild_battle_reward_flag)
	{
		return;
	}

	{
		// 发邮件
		static MailContentParam contentparam; contentparam.Reset();

		int reward_item_count = 0;
		for (int i = 0; i < cfg->reward_item_count && reward_item_count < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg->reward_item_list[i].item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[reward_item_count].item_id = cfg->reward_item_list[i].item_id;
				contentparam.item_list[reward_item_count].num = cfg->reward_item_list[i].num;
				contentparam.item_list[reward_item_count].is_bind = (cfg->reward_item_list[i].is_bind) ? 1 : 0;
				contentparam.item_list[reward_item_count].invalid_time = reward_item_base->CalInvalidTime();
				++reward_item_count;
			}
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_charge_bu_reward_content,
			RAND_ACTIVITY_TYPE_GUILDBATTLE);
		if (length > 0)
		{
			m_activity_info.ra_guild_battle_reward_flag = 1;
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}
}

// 消费领奖
void RoleActivity::OnRAConsumeGiftConsumeGold(int consume_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUM_GIFT))
	{
		return;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONSUM_GIFT);

	m_activity_info.ra_consume_gift_consume_gold += consume_gold;
	m_activity_info.ra_consume_gift_join_theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CONSUM_GIFT);
	m_activity_info.ra_consume_gift_join_opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	this->SendRAConsumeGiftInfo();
}

void RoleActivity::OnRAConsumeGiftOpera(int param1, int param2)
{
	switch (param1)
	{
	case Protocol::RA_CONSUM_GIFT_OPERA_TYPE_INFO:
		{
			this->SendRAConsumeGiftInfo();
		}
		break;

	case Protocol::RA_CONSUM_GIFT_OPERA_TYPE_FETCH:
		{
			this->OnRAConsumeGiftFetchReward(param2);
		}
		break;

	case Protocol::RA_CONSUM_GIFT_OPERA_TYPE_ROLL:
		{
			this->OnRAConsumeGiftRoll();
		}
		break;

	case Protocol::RA_CONSUM_GIFT_OPERA_TYPE_ROLL_REWARD:
		{
			this->OnRAConsumeGiftRollReward();
		}
		break;
	case Protocol::RA_CONSUM_GIFT_OPERA_TYPE_ROLL_TEN:
		{
			this->OnRAConsumeGiftRollTen();
		}
		break;
	case Protocol::RA_CONSUM_GIFT_OPERA_TYPE_ROLL_REWARD_TEN:
	{
		this->OnRAConsumeGiftRollRewardTen();
	}
	break;
	default:
		break;
	}
}

void RoleActivity::SendRAConsumeGiftInfo()
{
	int act_theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CONSUM_GIFT);
	const int roll_need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRollGoldNum();
	const int roll_times_limit = LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRollTimesLimit();

	int can_roll_times = m_activity_info.ra_consume_gift_consume_gold / roll_need_gold;
	can_roll_times = can_roll_times > roll_times_limit ? roll_times_limit : can_roll_times;
	int left_roll_times = can_roll_times - m_activity_info.ra_consume_gift_today_join_times;
	left_roll_times = left_roll_times < 0 ? 0 : left_roll_times;

	static Protocol::SCRAConsumGift racg;
	racg.consum_gold = m_activity_info.ra_consume_gift_consume_gold;
	racg.act_theme = (short)act_theme;
	racg.left_roll_times = (short)left_roll_times;
	racg.reward_fetch_flag = m_activity_info.ra_consume_gift_reward_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&racg, sizeof(racg));
}

void RoleActivity::OnRAConsumeGiftFetchReward(int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUM_GIFT))
	{
		m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
		return;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (param < 0 || param >= RAND_ACTIVITY_CONSUM_GIFT_MAX_SEQ)
	{
		return;
	}

	if (0 != (m_activity_info.ra_consume_gift_reward_flag & (1 << param)))
	{
		return;
	}

	int act_theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CONSUM_GIFT);
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const ConsumGiftRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRewardCfg(opengame_day, act_theme, param);
	if (reward_cfg == nullptr || reward_cfg->item_count <= 0)
	{
		return;
	}

	if (m_activity_info.ra_consume_gift_consume_gold < reward_cfg->need_gold)
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CONSUME_NOT_REACH_NEED_CONSUME_GOLD);
		return;
	}

	if (!m_role->GetKnapsack()->PutList(reward_cfg->item_count, reward_cfg->reward_item_list, PUT_REASON_CONSUM_GIFT_REWARD))
	{
		return;
	}
	
	m_activity_info.ra_consume_gift_reward_flag |= (1 << param);

	this->SendRAConsumeGiftInfo();
}

void RoleActivity::OnRAConsumeGiftRollReward()
{
	if (m_activity_info.ra_consume_gift_roll_num <= 0)
	{
		return;
	}

	int act_theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CONSUM_GIFT);
	const ConsumGiftRollRewardPool *roll_reward = LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRollReward(act_theme, m_activity_info.ra_consume_gift_roll_seq);
	if (roll_reward == nullptr)
	{
		return;
	}

	ItemConfigData reward_item;
	reward_item.item_id = roll_reward->roll_reward_item.item_id;
	reward_item.is_bind = roll_reward->roll_reward_item.is_bind;
	reward_item.num = m_activity_info.ra_consume_gift_roll_num;

	if (m_role->GetKnapsack()->PutOrMail(reward_item, PUT_REASON_CONSUM_GIFT_ROLL_REWARD))
	{
		++m_activity_info.ra_consume_gift_today_join_times;
		m_activity_info.ra_consume_gift_roll_num = 0;
		this->SendRAConsumeGiftInfo();
	}

	{
		// 日志
		gamelog::g_log_roleactivity.printf(LL_INFO, "[RoleActivity::OnRAConsumeGiftRollReward] user[%s %d] consume_gold[%d] today_join_times[%d]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), m_activity_info.ra_consume_gift_consume_gold, m_activity_info.ra_consume_gift_today_join_times);
	}
}

void RoleActivity::OnRAConsumeGiftRollRewardTen()
{
	bool flag = false;
	int act_theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CONSUM_GIFT);
	for (int i = 0; i < RA_CONSUM_GIFT_MAX_ROLL_COUNT; ++i)
	{
		if (m_activity_info.ra_consume_gift_roll_num_list[i] <= 0)
		{
			continue;
		}

		const ConsumGiftRollRewardPool *roll_reward = LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRollReward(act_theme, m_activity_info.ra_consume_gift_roll_seq_list[i]);
		if (roll_reward == nullptr)
		{
			continue;
		}

		ItemConfigData reward_item;
		reward_item.item_id = roll_reward->roll_reward_item.item_id;
		reward_item.is_bind = roll_reward->roll_reward_item.is_bind;
		reward_item.num = m_activity_info.ra_consume_gift_roll_num_list[i];

		if (m_role->GetKnapsack()->PutOrMail(reward_item, PUT_REASON_CONSUM_GIFT_ROLL_REWARD))
		{
			++m_activity_info.ra_consume_gift_today_join_times;
			m_activity_info.ra_consume_gift_roll_num_list[i] = 0;
			flag = true;
		}

		{
			// 日志
			gamelog::g_log_roleactivity.printf(LL_INFO, "[RoleActivity::OnRAConsumeGiftRollReward] user[%s %d] consume_gold[%d] today_join_times[%d]",
				m_role->GetName(), UidToInt(m_role->GetUserId()), m_activity_info.ra_consume_gift_consume_gold, m_activity_info.ra_consume_gift_today_join_times);
		}
	}

	if (true == flag)
	{
		this->SendRAConsumeGiftInfo();
	}
}

void RoleActivity::OnRAConsumeGiftRoll()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUM_GIFT))
	{
		m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
		return;
	}

	const int roll_times_limit = LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRollTimesLimit();
	const int roll_need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRollGoldNum();
	int can_roll_times = m_activity_info.ra_consume_gift_consume_gold / roll_need_gold;
	can_roll_times = can_roll_times > roll_times_limit ? roll_times_limit : can_roll_times;
	int left_roll_times = can_roll_times - m_activity_info.ra_consume_gift_today_join_times;
	if (left_roll_times <= 0)
	{
		m_role->NoticeNum(errornum::EN_CONSUM_GIFT_ROOL_INSUFFICIENT_NUMBER);
		return;
	}

	int act_theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CONSUM_GIFT);
	const ConsumGiftRollRewardPool *roll_reward = LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRollNumType(act_theme);
	if (roll_reward == nullptr)
	{
		return;
	}
	
	int decade_num = -1, digit_num = -1;
	if (!LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRollRewardNumByType(roll_reward->num_type, decade_num, digit_num))
	{
		return;
	}
	
	m_activity_info.ra_consume_gift_roll_seq = (char)roll_reward->seq;
	m_activity_info.ra_consume_gift_roll_num = decade_num * 10 + digit_num;

	{   // 先下发结果给客户端显示，等客户端请求领奖再发送奖励物品
		static Protocol::SCRAConsumGiftRollReward racgrr;
		racgrr.seq = (char)roll_reward->seq;
		racgrr.decade = (char)decade_num;
		racgrr.units_digit = (char)digit_num;

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&racgrr, sizeof(racgrr));
	}
}

void RoleActivity::OnRAConsumeGiftRollTen()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUM_GIFT))
	{
		m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
		return;
	}

	const int roll_need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRollGoldNum();
	const int roll_times_limit = LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRollTimesLimit();
	int act_theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CONSUM_GIFT);
	int can_roll_times = m_activity_info.ra_consume_gift_consume_gold / roll_need_gold;
	can_roll_times = can_roll_times > roll_times_limit ? roll_times_limit : can_roll_times;
	int left_roll_times = can_roll_times - m_activity_info.ra_consume_gift_today_join_times;
	if (left_roll_times < RA_CONSUM_GIFT_MAX_ROLL_COUNT)
	{
		m_role->NoticeNum(errornum::EN_CONSUM_GIFT_ROOL_INSUFFICIENT_NUMBER);
		return;
	}

	static Protocol::SCRAConsumGiftRollRewardTen racgrrt;
	for (int i = 0; i < RA_CONSUM_GIFT_MAX_ROLL_COUNT; ++i)
	{
		const ConsumGiftRollRewardPool *roll_reward = LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRollNumType(act_theme);
		if (roll_reward == nullptr)
		{
			return;
		}

		int decade_num = -1, digit_num = -1;
		if (!LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRollRewardNumByType(roll_reward->num_type, decade_num, digit_num))
		{
			return;
		}

		m_activity_info.ra_consume_gift_roll_seq_list[i] = (char)roll_reward->seq;
		m_activity_info.ra_consume_gift_roll_num_list[i] = decade_num * 10 + digit_num;
		{   // 先下发结果给客户端显示，等客户端请求领奖再发送奖励物品
			racgrrt.seq[i] = (char)roll_reward->seq;
			racgrrt.decade[i] = (char)decade_num;
			racgrrt.units_digit[i] = (char)digit_num;
		}
	}
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&racgrrt, sizeof(racgrrt));
}


void RoleActivity::OnRADailyLimitBuyOpera(int param1, int param2)
{
	switch (param1)
	{
	case Protocol::RA_LIMIT_BUY_OPERA_TYPE_INFO:
	{
		this->SendRADailyLimitBuyInfo();
	}
	break;

	case Protocol::RA_LIMIT_BUY_OPERA_TYPE_BUY:
	{
		this->OnRADailyLimitBuyBuy(param2);
	}
	break;

	default:
		break;
	}
}

void RoleActivity::OnRADailyLimitBuyBuy(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BUY_LIMIT))
	{
		m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BUY_LIMIT);

	int act_type = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_BUY_LIMIT);
	const DailyLimitBuyConfig * cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLimitBuyCfg(act_type, seq);
	if (cfg == nullptr)
	{
		return;
	}

	if (m_activity_info.ra_limit_buy_had_buy_count[seq] >= cfg->limit_buy_count)
	{
		m_role->NoticeNum(errornum::EN_SHOP_HAS_BOUGHT_LIMIT_ITEM);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cfg->gold_price, "RoleActivity::OnRADailyLimitBuyBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (m_role->GetKnapsack()->PutOrMail(cfg->reward_item, PUT_REASON_LIMIT_BUY_BUY_ITEM))
	{
		++m_activity_info.ra_limit_buy_had_buy_count[seq];
		this->SendRADailyLimitBuyInfo();
	}
}

void RoleActivity::SendRADailyLimitBuyInfo()
{
	static Protocol::SCRALimitBuyInfo lbi;
	lbi.act_type = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_BUY_LIMIT);
	memcpy(lbi.had_buy_count, m_activity_info.ra_limit_buy_had_buy_count, sizeof(lbi.had_buy_count));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&lbi, sizeof(lbi));
}

void RoleActivity::OnRACollectTreasureOperaReq(int param1, int param2)
{
	switch (param1)
	{
	case Protocol::RA_COLLECT_TREASURE_OPERA_TYPE_INFO:
	{
		this->SendRACollectTreasureInfo();
	}
	break;

	case Protocol::RA_COLLECT_TREASURE_OPERA_TYPE_ROLL:
	{
		this->OnRACollectTreasureRoll();
	}
	break;

	case Protocol::RA_COLLECT_TREASURE_OPERA_TYPE_REWARD:
	{
		this->OnRACollectTreasureReward();
	}
	break;

	default:
		break;
	}
}

void RoleActivity::OnRACollectTreasureChongZhi(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_COLLECT_TREASURE))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_COLLECT_TREASURE);

	int need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetCollectTreasureGold();
	if (chongzhi < need_gold)
	{
		return;
	}

	int can_join_times = LOGIC_CONFIG->GetRandActivityCfg().GetCollectTreasureCanJoinTimes();
	if (m_activity_info.ra_collect_treasure_can_join_times < can_join_times)
	{
		++m_activity_info.ra_collect_treasure_can_join_times;
		this->SendRACollectTreasureInfo();
	}
}

void RoleActivity::SendRACollectTreasureInfo()
{
	static Protocol::SCRACollectTreasureInfo cti;
	cti.left_roll_times = m_activity_info.ra_collect_treasure_can_join_times - m_activity_info.ra_collect_treasure_join_times;
	cti.had_join_times = m_activity_info.ra_collect_treasure_join_times;
	cti.record_num = 0;

	ActivityShadow::Instance().GetRACollectTreasureRecord(cti.join_record_list, cti.record_num);

	int send_len = sizeof(cti) - sizeof(cti.join_record_list) + sizeof(cti.join_record_list[0]) * cti.record_num;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cti, send_len);
}

void RoleActivity::OnRACollectTreasureRoll()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_COLLECT_TREASURE))
	{
		m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
		return;
	}

	if (m_activity_info.ra_collect_treasure_join_times >= m_activity_info.ra_collect_treasure_can_join_times)
	{
		m_role->NoticeNum(errornum::EN_NO_JOIN_TIMES);
		return;
	}
	
	const CollectTreasureConfig * cfg = LOGIC_CONFIG->GetRandActivityCfg().GetCollectTreasureCfg();
	if (cfg == nullptr)
	{
		return;
	}
	m_activity_info.ra_collect_treasure_roll_seq = cfg->seq;

	static Protocol::SCRACollectTreasureResult ctr;
	ctr.seq = cfg->seq;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ctr, sizeof(ctr));
}

void RoleActivity::OnRACollectTreasureReward()
{
	if (m_activity_info.ra_collect_treasure_roll_seq < 0)
	{
		return;
	}

	const CollectTreasureConfig * cfg = LOGIC_CONFIG->GetRandActivityCfg().GetCollectTreasureCfgBySeq(m_activity_info.ra_collect_treasure_roll_seq);
	if (cfg == nullptr)
	{
		return;
	}

	int need_gold = LOGIC_CONFIG->GetRandActivityCfg().GetCollectTreasureGold();

	int get_gold_bind = (int)(need_gold * cfg->units_digit * 0.1f);
	m_role->GetKnapsack()->GetMoney()->AddGoldBind(get_gold_bind, "OnRACollectTreasureReward");

	{
		CollectTreasureRollRecord record_item;
		record_item.role_id = m_role->GetUID();
		m_role->GetName(record_item.role_name);
		record_item.roll_mul = cfg->units_digit;

		ActivityShadow::Instance().AddRACollectTreasureRecord(record_item);
	}

	++m_activity_info.ra_collect_treasure_join_times;
	m_activity_info.ra_collect_treasure_roll_seq = -1;
	this->SendRACollectTreasureInfo();

	{
		// 日志
		gamelog::g_log_roleactivity.printf(LL_INFO, "[RoleActivity::OnRACollectTreasureReward] user[%s %d] can_join_times[%d] today_join_times[%d]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), m_activity_info.ra_collect_treasure_can_join_times, m_activity_info.ra_collect_treasure_join_times);
	}
}

void RoleActivity::OnRAHappyCumulChongzhiGold(int chongzhi_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE);

	m_activity_info.ra_happy_cumul_chongzhi_num += chongzhi_gold;
	m_activity_info.ra_happy_cumul_chongzhi_join_theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE);
	this->SendRAHappyCumulChongzhiInfo();
}

void RoleActivity::OnRAHappyCumulChongzhiOpera(int param1, int param2)
{
	switch (param1)
	{
	case Protocol::RA_HAPPY_CUMUL_CHONGZHI_OPERA_TYPE_INFO:
	{
		this->SendRAHappyCumulChongzhiInfo();
	}
	break;

	case Protocol::RA_HAPPY_CUMUL_CHONGZHI_OPERA_TYPE_FETCH:
	{
		this->OnRAHappyCumulChongzhiReward(param2);
	}
	break;

	default:
		break;
	}
}

void RoleActivity::SendRAHappyCumulChongzhiInfo()
{
	static Protocol::SCRAHappyCumulChongzhiInfo hcczi;
	hcczi.act_type = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE);
	hcczi.chongzhi_num = m_activity_info.ra_happy_cumul_chongzhi_num;
	hcczi.fetch_reward_flag = m_activity_info.ra_happy_cumul_chongzhi_fetch_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&hcczi, sizeof(hcczi));
}

void RoleActivity::OnRAHappyCumulChongzhiReward(int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE))
	{
		m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
		return;
	}

	int theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE);
	const HappyCumulChongzhiConfig * cfg = LOGIC_CONFIG->GetRandActivityCfg().GetHappyCumulChongzhiCfg(theme, param);

	if (cfg == nullptr) return;

	if (m_activity_info.ra_happy_cumul_chongzhi_num < cfg->need_chongzhi)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_TOTAL_CHONGZHI_NOT_ENOUGH);
		return;
	}

	if (0 != (m_activity_info.ra_happy_cumul_chongzhi_fetch_flag & (1 << param)))
	{
		m_role->NoticeNum(errornum::EN_CHARGE_REPAYMENT_HAS_FETCHED);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg->reward_count) || !m_role->GetKnapsack()->PutList(cfg->reward_count, cfg->reward_item_list, PUT_REASON_HAPPY_CUMUL_CHONGZHI_REWARD))
	{
		gamecommon::SendMultiMailToRole(m_role->GetUserId(), cfg->reward_item_list, cfg->reward_count, nullptr, false, "onrahappycumulchongzhi");
	}

	m_activity_info.ra_happy_cumul_chongzhi_fetch_flag |= (1 << param);
	this->SendRAHappyCumulChongzhiInfo();
}

void RoleActivity::OnRACriticalStrikeOperaReq()
{
	static Protocol::SCRACriticalStrikeInfo csi;
	csi.act_type = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&csi, sizeof(csi));
}

int RoleActivity::GetCriticalStrikeDayMul(int type)
{
	int mul = 1;
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY))
	{
		int now_type = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY);
		if (now_type == type)   // 今日主题是所调用系统的主题
		{
			mul = LOGIC_CONFIG->GetRandActivityCfg().GetCriticalStrikeDayMul(type);
		}
	}
	
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY_2))
	{
		int now_type = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY_2);
		if (now_type == type)   // 今日主题是所调用系统的主题
		{
			mul = LOGIC_CONFIG->GetRandActivityCfg().GetCriticalStrikeDay2Mul(type);
		}
	}

	return mul;
}

int RoleActivity::GetRACriticalStrikeDayRate(int type)
{
	int rate = 0;
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY))
	{
		int now_type = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY);
		if (now_type == type)   // 今日主题是所调用系统的主题
		{
			rate += LOGIC_CONFIG->GetRandActivityCfg().GetCriticalStrikeDayRate(type);
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY_2))
	{
		int now_type = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY_2);
		if (now_type == type)   // 今日主题是所调用系统的主题
		{
			rate += LOGIC_CONFIG->GetRandActivityCfg().GetCriticalStrikeDay2Rate(type);
		}
	}

	return rate;
}

void RoleActivity::OnRaJinJieOpera(int type, short grade, short old_grade)
{
	if (type <= JINJIE_ACT_THEME_TYPE_INVALID || type >= JINJIE_ACT_THEME_TYPE_MAX) return;

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN))
	{
		int theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN);
		if (type == theme) // 今日的主题
		{
			//this->CheckRAJinJieOldAllReward(theme, old_grade);
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN);

			for (int i = 0; i < RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG; i++)
			{
				const JinJieReturnConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieReturnCfg(theme, i);
				if (cfg == nullptr) continue;

				if ((short)grade >= cfg->need_grade)     // 重置奖励
				{
					m_activity_info.ra_jinjie_return_fetch_flag &= ~(1 << i);
				}
			}
			this->SendRAJinJieReturnInfo();
		}
	}

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2))
	{
		int theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2);
		if (type == theme) // 今日的主题
		{
			//this->CheckRAJinJieOldAllReward2(theme, old_grade);
			this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2);

			for (int i = 0; i < RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG; i++)
			{
				const JinJieReturnConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieReturn2Cfg(theme, i);
				if (cfg == nullptr) continue;

				if ((short)grade >= cfg->need_grade)     // 重置奖励
				{
					m_activity_info.ra_jinjie_return2_fetch_flag &= ~(1 << i);
				}
			}
			this->SendRAJinJieReturn2Info();
		}
	}
}

void RoleActivity::OnRAJinJieReturnOpera(int param1, int param2)
{
	switch (param1)
	{
	case Protocol::RA_JINJIE_RETURN_OPERA_TYPE_INFO:
	{
		this->SendRAJinJieReturnInfo();
	}
	break;

	case Protocol::RA_JINJIE_RETURN_OPERA_TYPE_FETCH:
	{
		this->OnRAJinJieReturnReward(param2);
	}
	break;

	default:
		break;
	}
}

void RoleActivity::SendRAJinJieReturnInfo()
{
	static Protocol::SCRAJinJieReturnInfo jjri;
	jjri.act_type = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN);
	jjri.fetch_reward_flag = m_activity_info.ra_jinjie_return_fetch_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&jjri, sizeof(jjri));
}

void RoleActivity::OnRAJinJieReturnReward(int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN))
	{
		m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN);

	int theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN);
	const JinJieReturnConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieReturnCfg(theme, param);
	if (cfg == nullptr) return;

	int grade = this->JinJieActGetGradeByTheme(theme);
	if (grade < cfg->need_grade)
	{
		m_role->NoticeNum(errornum::EN_FETCH_CONDITION_NOENOUGH);
		return;
	}

	if (0 != (m_activity_info.ra_jinjie_return_fetch_flag & (1 << param)))
	{
		m_role->NoticeNum(errornum::EN_CHARGE_REPAYMENT_HAS_FETCHED);
		return;
	}

	ItemDataWrapper reward_item;
	reward_item.item_id = cfg->reward_item.item_id;
	reward_item.is_bind = cfg->reward_item.is_bind;
	reward_item.num = cfg->reward_item.num;

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(reward_item, PUT_REASON_JINJIE_RETURN_REWARD))
	{
		return;
	}

	m_activity_info.ra_jinjie_return_fetch_flag |= (1 << param);
	this->SendRAJinJieReturnInfo();
}

void RoleActivity::CheckRAJinJieOldAllReward(int theme, int old_grade)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN);
	
	int re_theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN);
	if (re_theme != theme)
	{
		return;
	}

	int grade = this->JinJieActGetGradeByTheme(theme);
	if (old_grade >= grade)
	{
		return;
	}

	int old_fetch_flag = m_activity_info.ra_jinjie_return_fetch_flag;

	MailContentParam contentparam; contentparam.Reset();
	for (int i = 0; i < RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG; ++i)
	{
		const JinJieReturnConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieReturnCfg(theme,i);
		if (cfg == nullptr) continue;

		if (old_grade <= cfg->need_grade) break;
	
		if (0 != (m_activity_info.ra_jinjie_return_fetch_flag & (1 << i))) continue;

		if(contentparam.item_list[0].item_id !=0 && contentparam.item_list[0].item_id != cfg->reward_item.item_id) continue;

		contentparam.item_list[0].item_id = cfg->reward_item.item_id;
		contentparam.item_list[0].is_bind = cfg->reward_item.is_bind;
		contentparam.item_list[0].num += cfg->reward_item.num;

		m_activity_info.ra_jinjie_return_fetch_flag |= (1 << i);
	}

	if (contentparam.item_list[0].item_id <= 0 || contentparam.item_list[0].num <= 0)  return;

	const ItemBase *item_base = ITEMPOOL->GetItem(contentparam.item_list[0].item_id);
	if (NULL != item_base)
	{
		contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();
	}
	else
	{
		return;
	}

	int lenth = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_uplevel_return_reward_mail,theme,old_grade,theme);

	if (lenth > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
	}

	m_role->NoticeNum(noticenum::NT_UPLEVEL_RETURN_RWARD_MAIL);
	
	gamelog::g_log_roleactivity.printf(LL_INFO, "CheckRAJinJieOldAllReward user[%d], theme[%d], old_grade[%d],old_reward_flag[%d],reward_flag[%d]",
		m_role->GetUID(), theme,old_grade,old_fetch_flag, m_activity_info.ra_jinjie_return_fetch_flag);
}

int RoleActivity::JinJieActGetGradeByTheme(int theme)
{
	int grade = 0;

	switch (theme)
	{
	case JINJIE_ACT_THEME_TYPE_MOUNT:
	{
		grade = m_role->GetMountManager()->GetMountParam()->grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_WING:
	{
		grade = m_role->GetWing()->GetWingParam()->grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_FABAO:
	{
		grade = m_role->GetFabaoManager()->GetParam()->grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_SHENBING:
	{
		grade = m_role->GetShizhuang()->GetShiZhuangParam()->item_list[SHIZHUANG_TYPE_WUQI].grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_FOOTSPRINT:
	{
		grade = m_role->GetFootPrint()->GetFootprintParam()->grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_GUANGHUAN:
	{
		grade = m_role->GetHalo()->GetHaloParam()->grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_FASHION:
	{
		grade = m_role->GetShizhuang()->GetShiZhuangParam()->item_list[SHIZHUANG_TYPE_BODY].grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_FIGHT_MOUNT:
	{
		grade = m_role->GetFightMountManager()->GetFightMountParam()->grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_TOUSHI:
	{
		grade = m_role->GetTouShiManager()->GetTouShiParam()->grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_MASK:
	{
		grade = m_role->GetMaskManager()->GetMaskParam()->grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_YAOSHI:
	{
		grade = m_role->GetYaoShiManager()->GetYaoShiParam()->grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_QILINBI:
	{
		grade = m_role->GetQiLinBiManager()->GetQiLinBiParam()->grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_LINGTONG:
	{
		grade = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_2);
	}
	break;

	case JINJIE_ACT_THEME_TYPE_LINGGONG:
	{
		grade = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_3);
	}
	break;

	case JINJIE_ACT_THEME_TYPE_LINGQI:
	{
		grade = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_4);
	}
	break;

	case JINJIE_ACT_THEME_TYPE_SHENGONG:
	{
		grade = m_role->GetShengong()->GetShengongParam()->grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_SHENYI:
	{
		grade = m_role->GetShenyi()->GetShenyiParam()->grade;
	}
	break;

	case JINJIE_ACT_THEME_TYPE_FLYPET:
	{
		grade = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_8);
	}
	break;

	case JINJIE_ACT_THEME_TYPE_WEIYAN:
	{
		grade = m_role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_5);
	}
	break;

	default:
		break;
	}

	return grade;
}

int RoleActivity::JinJieActThemeGetByUpgrade(int upgrade_type)
{
	int theme = 0;
	switch (upgrade_type)
	{
	case UPGRADE_TYPE_2:
	{
		theme = JINJIE_ACT_THEME_TYPE_LINGTONG;

	}
	break;
	case UPGRADE_TYPE_3:
	{
		theme = JINJIE_ACT_THEME_TYPE_LINGGONG;
	}
	break;
	case UPGRADE_TYPE_4:
	{
		theme = JINJIE_ACT_THEME_TYPE_LINGQI;
	}
	break;
	case UPGRADE_TYPE_5:
	{
		theme = JINJIE_ACT_THEME_TYPE_WEIYAN;
	}
	break;

	case UPGRADE_TYPE_8:
	{
		theme = JINJIE_ACT_THEME_TYPE_FLYPET;
	}
	break;

	default:
		break;
	}
	return theme;
}

void RoleActivity::OnRAConsumeForGiftOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT))
	{
		//m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT);

	switch (opera_type)
	{
	case Protocol::RA_CONSUME_FOR_GIFT_OPERA_TYPE_ALL_INFO:
	{
		this->SendRAConsumeForGiftAllInfo();
	}
	break;

	case Protocol::RA_CONSUME_FOR_GIFT_OPERA_TYPE_EXCHANGE_ITEM:
	{
		this->OnRAConsumeForGiftExchangeItem(param1);
	}
	break;
	}
}

void RoleActivity::SendRAConsumeForGiftAllInfo()
{
	static Protocol::SCRAConsumeForGiftAllInfo cfgai;

	cfgai.total_consume_gold = m_activity_info.ra_consume_for_gift_total_consume_gold;
	cfgai.cur_points = m_activity_info.ra_consume_for_gift_cur_points;
	cfgai.item_count = LOGIC_CONFIG->GetRandActivityCfg().GetConsumeForGiftItemCount();
	memcpy(cfgai.item_exchange_times, m_activity_info.ra_consume_for_gift_item_exchange_times, sizeof(cfgai.item_exchange_times));

	int send_len = sizeof(cfgai) - (RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT - cfgai.item_count) * sizeof(cfgai.item_exchange_times[0]);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cfgai, send_len);
}

void RoleActivity::OnRAConsumeForGiftExchangeItem(int index)
{
	// 检查
	if (index < 0 || index >= RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT)
	{
		return;
	}

	const RandActivityConsumeForGiftItemConfig *item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetConsumeForGiftItemCfg(index);
	if (NULL == item_cfg)
	{
		return;
	}

	int need_points = item_cfg->need_points;
	unsigned char &cur_exchange_times = m_activity_info.ra_consume_for_gift_item_exchange_times[index];
	if (cur_exchange_times >= item_cfg->double_points_need_ex_times)
	{
		need_points *= 2;
	}

	if (m_activity_info.ra_consume_for_gift_cur_points < need_points)
	{
		m_role->NoticeNum(errornum::EN_RA_CONSUME_FOR_GIFT_POINTS_NOT_ENOUGH);
		return;
	}

	// 消耗
	m_activity_info.ra_consume_for_gift_cur_points -= need_points;

	if (cur_exchange_times < UCHAR_MAX)
	{
		++cur_exchange_times;
	}

	// 邮件发放
	if (!m_role->GetKnapsack()->HaveSpace())							// 背包满时邮件发放：这是你在消费有礼兑换的奖励，请注意查收。
	{
		static MailContentParam contentparam; contentparam.Reset();

		const ItemBase *reward_item_base = ITEMPOOL->GetItem(item_cfg->exchange_item.item_id);
		if (NULL != reward_item_base)
		{
			contentparam.item_list[0].item_id = item_cfg->exchange_item.item_id;
			contentparam.item_list[0].num = item_cfg->exchange_item.num;
			contentparam.item_list[0].is_bind = item_cfg->exchange_item.is_bind ? 1 : 0;
			contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward_common_mail_content, RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT);
		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}
	else
	{
		// 给予
		m_role->GetKnapsack()->Put(item_cfg->exchange_item, PUT_REASON_RA_CONSUME_FOR_GIFT_EXCHANGE_ITEM);
	}

	// 发送
	this->SendRAConsumeForGiftAllInfo();

	// 日志
	gamelog::g_log_roleactivity.printf(LL_INFO, "OnRAConsumeForGiftExchangeItem user[%d %s], index[%d], item[%s], cur_points[%d], cur_exchange_times[%d]",
		m_role->GetUID(), m_role->GetName(), index, ItemConfigDataLog(&item_cfg->exchange_item, NULL), m_activity_info.ra_consume_for_gift_cur_points, cur_exchange_times);
}

void RoleActivity::OnRAConsumeForGiftConsumeGold(int consume_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT);

	int add_points_per_gold = LOGIC_CONFIG->GetRandActivityCfg().GetConsumeForGiftPointsPerGold();
	if (add_points_per_gold <= 0)
	{
		return;
	}

	m_activity_info.ra_consume_for_gift_total_consume_gold += consume_gold;
	m_activity_info.ra_consume_for_gift_cur_points += consume_gold * add_points_per_gold;

	this->SendRAConsumeForGiftAllInfo();
}

void RoleActivity::OnRaMiJingXunBao3OperReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3);

	switch (opera_type)
	{
	case Protocol::RA_MIJINGXUNBAO3_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRAMiJingXunBao3Info();
	}
	break;

	case Protocol::RA_MIJINGXUNBAO3_OPERA_TYPE_TAO:
	{
		this->OnRAMiJingXunBao3Tao(param);
	}
	break;

	case Protocol::RA_MIJINGXUNBAO3_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnRAMiJingXunBao3FetchPersonalReward(param);
	}
	break;

	}
}

void RoleActivity::SendRAMiJingXunBao3Info()
{
	Protocol::SCRAMiJingXunBaoInfo3 scramjxbi;
	scramjxbi.ra_mijingxunbao3_next_free_tao_timestamp = m_activity_info.ra_mijingxunbao3_next_free_tao_timestamp;
	scramjxbi.reward_flag = m_activity_info.ra_mijingxunbao3_personal_reward_flag;
	scramjxbi.chou_times = m_activity_info.ra_mijingxunbao3_role_chou_times;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scramjxbi, sizeof(scramjxbi));
}

void RoleActivity::OnRAMiJingXunBao3FetchPersonalReward(int seq)
{
	const RandActivityMiJingXunBaoRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityMiJingXunBao3PersonalRewardCfg(seq, m_activity_info.ra_mijingxunbao3_role_chou_times);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (0 != (m_activity_info.ra_mijingxunbao3_personal_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(1))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_mijingxunbao3_personal_reward_flag |= 1 << seq;

	m_role->GetKnapsack()->Put(reward_cfg->reward_item, PUT_REASON_MIJINGXUNBAO_REWARD);

	this->SendRAMiJingXunBao3Info();

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAMiJingXunBao3FetchPersonalReward user[%d, %s] reward_flag[%d] reward_item_id[%d] reward_item_num[%d]",
		m_role->GetUID(), m_role->GetName(), m_activity_info.ra_mijingxunbao3_personal_reward_flag, reward_cfg->reward_item.item_id, reward_cfg->reward_item.num);
}

void RoleActivity::OnRAMiJingXunBao3Tao(int type)
{
	if (type < RA_MIJINGXUNBAO3_CHOU_TYPE_1 || type >= RA_MIJINGXUNBAO3_CHOU_TYPE_MAX)
	{
		return;
	}

	int consume_gold = 0;
	bool is_free_tao = false;
	int tao_times = 0;

	unsigned int nowtime = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	RandActivityConfig &randactivity_cfg = LOGIC_CONFIG->GetRandActivityCfg();

	unsigned int cooldown_interval = randactivity_cfg.GetMiJingXunBao3Interval() * SECOND_PER_MIN;							// 淘宝冷却时间

	consume_gold = randactivity_cfg.GetMiJingXunBao3TaoGold(type);
	if (consume_gold <= 0)
	{
		return;
	}

	bool has_item = false;
	ItemID item_id = 0;

	if (RA_MIJINGXUNBAO3_CHOU_TYPE_1 == type)                    // 获得淘宝花费的元宝
	{
		tao_times = 1;

		if (nowtime >= m_activity_info.ra_mijingxunbao3_next_free_tao_timestamp && m_activity_info.ra_mijingxunbao3_daily_free_times >= 1)
		{
			is_free_tao = true;
			consume_gold = 0;
		}
	}
	else if (RA_MIJINGXUNBAO3_CHOU_TYPE_10 == type)
	{
		tao_times = 10;
	}
	else if (RA_MIJINGXUNBAO3_CHOU_TYPE_30 == type)
	{
		tao_times = 30;

		item_id = LOGIC_CONFIG->GetRandActivityCfg().GetMiJingXunBao3TaoItem();
		if (m_role->GetKnapsack()->HasItem(item_id))
		{
			has_item = true;
			consume_gold = 0;
		}
	}
	else
	{
		return;
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int reward_count = tao_times;

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, reward_count))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	if (reward_count > Protocol::SC_MiJingXunBaoTao3_MAX_TIMES)
	{
		return;
	}

	static RandActivityMiJingXunBaoConfig reward_item_list[Protocol::SC_MiJingXunBaoTao3_MAX_TIMES];

	bool ret = randactivity_cfg.GetMiJingXunBao3RandomResult(m_activity_info.ra_mijingxunbao3_add_weight_list, randactivity_cfg.GetMiJingXunBao3AutoWeightAddCount(), is_free_tao, reward_item_list, reward_count);
	if (!ret)
	{
		return;
	}

	// 物品
	if (RA_MIJINGXUNBAO3_CHOU_TYPE_30 == type)
	{
		if (has_item)
		{
			if (!m_role->GetKnapsack()->ConsumeItem(item_id, 1, "RoleActivity::OnRAMiJingXunBao3Tao"))
			{
				return;
			}
		}
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "RoleActivity::OnRAMiJingXunBao3Tao"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	//刷新下次免费淘宝时间
	if (is_free_tao)
	{
		--m_activity_info.ra_mijingxunbao3_daily_free_times;
		m_activity_info.ra_mijingxunbao3_next_free_tao_timestamp = nowtime + cooldown_interval;
		//m_activity_info.ra_mijingxunbao3_role_chou_times += tao_times;
		//this->SendRAMiJingXunBao3Info();

		// 免费单抽绑定
		reward_item_list[0].reward_item.is_bind = true;

		if (m_activity_info.ra_mijingxunbao3_daily_free_times == 0)
		{
			m_activity_info.ra_mijingxunbao3_next_free_tao_timestamp = 0;
		}
	}

	m_activity_info.ra_mijingxunbao3_role_chou_times += tao_times;

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAMiJingXunBao3Tao user[%d, %s] is_free_tao[%d] consume_gold[%d] type[%d]",
		m_role->GetUID(), m_role->GetName(), (int)is_free_tao, consume_gold, type);

	static Protocol::SCRAMiJingXunBaoTaoResultInfo3 scramjxbtri;
	scramjxbtri.count = reward_count;

	ChestItemInfo item_list[Protocol::SC_MiJingXunBaoTao3_MAX_TIMES];
	int list_num = 0;

	//给予
	for (int i = 0; i < reward_count && i < Protocol::SC_MiJingXunBaoTao3_MAX_TIMES; ++i)
	{
		item_list[i].Reset();

		item_list[i].item_id = reward_item_list[i].reward_item.item_id;
		item_list[i].num = reward_item_list[i].reward_item.num;
		item_list[i].is_bind = reward_item_list[i].reward_item.is_bind;

		++list_num;
	}

//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, list_num, item_list, "OnRAMiJingXunBao3Tao");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3, list_num, item_list, "RoleActivity::OnRAMiJingXunBao3Tao");

	for (int index = 0; index < reward_count && index < Protocol::SC_MiJingXunBaoTao3_MAX_TIMES; ++index)
	{
		gamelog::g_log_roleactivity.buff_printf("(%s), ", ItemConfigDataLog(&reward_item_list[index].reward_item, NULL));

		scramjxbtri.mijingxunbao3_tao_seq[index] = reward_item_list[index].seq;
		if (reward_item_list[index].is_rare)
		{
			// 刷到珍稀物品传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_mijingxunbao3_zhenxi_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_MIJINGXUNBAO3, (int)reward_item_list[index].reward_item.item_id);

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}

		LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3, m_role->GetUID(), m_role->GetName(), reward_item_list[index].reward_item.item_id, reward_item_list[index].reward_item.num);
	}

	//发送淘宝获得的物品信息
	{
		int send_len = sizeof(scramjxbtri) - sizeof(scramjxbtri.mijingxunbao3_tao_seq[0]) * (Protocol::SC_MiJingXunBaoTao3_MAX_TIMES - scramjxbtri.count);
		if (send_len > 0)
		{
			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scramjxbtri, send_len);
		}
	}

	this->SendRAMiJingXunBao3Info();

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);
}

void RoleActivity::OnRaHuanLeZaDanOperReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_HUANLE_ZADAN))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_HUANLE_ZADAN);

	switch (opera_type)
	{
	case Protocol::RA_HUANLEZADAN_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRAHuanLeZaDanInfo();
	}
	break;

	case Protocol::RA_HUANLEZADAN_OPERA_TYPE_TAO:
	{
		this->OnRAHuanLeZaDanTao(param);
	}
	break;

	case Protocol::RA_HUANLEZADAN_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnRAHuanLeZaDanFetchPersonalReward(param);
	}
	break;

	}
}

void RoleActivity::OnRAHuanLeZaDanTao(int type)
{
	if (type < RA_MIJINGXUNBAO3_CHOU_TYPE_1 || type >= RA_MIJINGXUNBAO3_CHOU_TYPE_MAX)
	{
		return;
	}

	int consume_gold = 0;
	bool is_free_tao = false;
	int tao_times = 0;

	unsigned int nowtime = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	RandActivityConfig &randactivity_cfg = LOGIC_CONFIG->GetRandActivityCfg();

	unsigned int cooldown_interval = randactivity_cfg.GetHuanLeZaDanInterval() * SECOND_PER_MIN;							// 淘宝冷却时间

	consume_gold = randactivity_cfg.GetHuanLeZaDanTaoGold(type);
	if (consume_gold <= 0)
	{
		return;
	}

	bool has_item = false;
	ItemID item_id = 0;

	if (RA_MIJINGXUNBAO3_CHOU_TYPE_1 == type)                    //获得淘宝花费的金币
	{
		tao_times = 1;

		if (nowtime >= m_activity_info.ra_huanlezadan_next_free_tao_timestamp && m_activity_info.ra_huanlezadan_daily_free_times >= 1)
		{
			is_free_tao = true;
			consume_gold = 0;
		}
	}
	else if (RA_MIJINGXUNBAO3_CHOU_TYPE_10 == type)
	{
		tao_times = 10;
	}
	else if (RA_MIJINGXUNBAO3_CHOU_TYPE_30 == type)
	{
		tao_times = 30;

		item_id = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeZaDanTaoItem();
		if (m_role->GetKnapsack()->HasItem(item_id))
		{
			has_item = true;
			consume_gold = 0;
		}
	}
	else
	{
		return;
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int reward_count = tao_times;

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, reward_count))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	if (reward_count > Protocol::SC_HUANLEZADAN_MAX_TIMES)
	{
		return;
	}

	static RandActivityMiJingXunBaoConfig reward_item_list[Protocol::SC_HUANLEZADAN_MAX_TIMES];

	bool ret = randactivity_cfg.GetHuanLeZaDanRandomResult(m_activity_info.ra_huanlezadan_add_weight_list, randactivity_cfg.GetHuanLeZaDanAutoWeightAddCount(), is_free_tao, reward_item_list, reward_count);
	if (!ret)
	{
		return;
	}

	// 物品
	if (RA_MIJINGXUNBAO3_CHOU_TYPE_30 == type)
	{
		if (has_item)
		{
			if (!m_role->GetKnapsack()->ConsumeItem(item_id, 1, "RoleActivity::OnRAHuanLeZaDanTao"))
			{
				return;
			}
		}
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "RoleActivity::OnRAHuanLeZaDanTao"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	//刷新下次免费淘宝时间
	if (is_free_tao)
	{
		--m_activity_info.ra_huanlezadan_daily_free_times;
		m_activity_info.ra_huanlezadan_next_free_tao_timestamp = nowtime + cooldown_interval;
		//m_activity_info.ra_huanlezadan_role_chou_times += tao_times;
		//this->SendRAHuanLeZaDanInfo();

		// 免费单抽绑定
		reward_item_list[0].reward_item.is_bind = true;

		if (m_activity_info.ra_huanlezadan_daily_free_times == 0)
		{
			m_activity_info.ra_huanlezadan_next_free_tao_timestamp = 0;
		}
	}

	m_activity_info.ra_huanlezadan_role_chou_times += tao_times;

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAHuanLeZaDanTao user[%d, %s] is_free_tao[%d] consume_gold[%d] type[%d]",
		m_role->GetUID(), m_role->GetName(), (int)is_free_tao, consume_gold, type);

	static Protocol::SCRAHuanLeZaDanTaoResultInfo scramjxbtri;
	scramjxbtri.count = reward_count;

	ChestItemInfo item_list[Protocol::SC_HUANLEZADAN_MAX_TIMES];
	int list_num = 0;

	//给予
	for (int i = 0; i < reward_count && i < Protocol::SC_HUANLEZADAN_MAX_TIMES; ++i)
	{
		item_list[i].Reset();

		item_list[i].item_id = reward_item_list[i].reward_item.item_id;
		item_list[i].num = reward_item_list[i].reward_item.num;
		item_list[i].is_bind = reward_item_list[i].reward_item.is_bind;

		++list_num;
	}

//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, list_num, item_list, "OnRAHuanLeZaDanTao");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_HUANLE_ZADAN, list_num, item_list, "RoleActivity::OnRAHuanLeZaDanTao");

	for (int index = 0; index < reward_count && index < Protocol::SC_HUANLEZADAN_MAX_TIMES; ++index)
	{
		gamelog::g_log_roleactivity.buff_printf("(%s), ", ItemConfigDataLog(&reward_item_list[index].reward_item, NULL));

		scramjxbtri.huanlezadan_tao_seq[index] = reward_item_list[index].seq;
		if (reward_item_list[index].is_rare)
		{
			// 刷到珍稀物品传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_huanlezadan_zhenxi_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_HUANLE_ZADAN, (int)reward_item_list[index].reward_item.item_id);

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}
		LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_HUANLE_ZADAN, m_role->GetUID(), m_role->GetName(), reward_item_list[index].reward_item.item_id, reward_item_list[index].reward_item.num);
	}

	//发送淘宝获得的物品信息
	{
		int send_len = sizeof(scramjxbtri) - sizeof(scramjxbtri.huanlezadan_tao_seq[0]) * (Protocol::SC_HUANLEZADAN_MAX_TIMES - scramjxbtri.count);
		if (send_len > 0)
		{
			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scramjxbtri, send_len);
		}
	}

	this->SendRAHuanLeZaDanInfo();

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);
}

void RoleActivity::OnRAHuanLeZaDanFetchPersonalReward(int seq)
{
	const RandActivityMiJingXunBaoRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityHuanLeZaDanPersonalRewardCfg(seq, m_activity_info.ra_huanlezadan_role_chou_times);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (0 != (m_activity_info.ra_huanlezadan_personal_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(1))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_huanlezadan_personal_reward_flag |= 1 << seq;

	m_role->GetKnapsack()->Put(reward_cfg->reward_item, PUT_REASON_HUANLEZADAN_REWARD);

	this->SendRAHuanLeZaDanInfo();

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAHuanLeZaDanFetchPersonalReward user[%d, %s] reward_flag[%d] reward_item_id[%d] reward_item_num[%d]",
		m_role->GetUID(), m_role->GetName(), m_activity_info.ra_huanlezadan_personal_reward_flag, reward_cfg->reward_item.item_id, reward_cfg->reward_item.num);
}

void RoleActivity::SendRAHuanLeZaDanInfo()
{
	Protocol::SCRAHuanLeZaDanInfo scramjxbi;
	scramjxbi.ra_huanlezadan_next_free_tao_timestamp = m_activity_info.ra_huanlezadan_next_free_tao_timestamp;
	scramjxbi.reward_flag = m_activity_info.ra_huanlezadan_personal_reward_flag;
	scramjxbi.chou_times = m_activity_info.ra_huanlezadan_role_chou_times;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scramjxbi, sizeof(scramjxbi));
}

void RoleActivity::OnRaHuanLeYaoJiangOperReq(int opera_type, int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG);

	switch (opera_type)
	{
	case Protocol::RA_HUANLEYAOJIANG_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRAHuanLeYaoJiangInfo();
	}
	break;

	case Protocol::RA_HUANLEYAOJIANG_OPERA_TYPE_TAO:
	{
		this->OnRAHuanLeYaoJiangTao(param);
	}
	break;

	case Protocol::RA_HUANLEYAOJIANG_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnRAHuanLeYaoJiangFetchPersonalReward(param);
	}
	break;

	}
}

void RoleActivity::OnRAHuanLeYaoJiangTao(int type)
{
	if (type < RA_MIJINGXUNBAO3_CHOU_TYPE_1 || type >= RA_MIJINGXUNBAO3_CHOU_TYPE_MAX)
	{
		return;
	}

	int consume_gold = 0;
	bool is_free_tao = false;
	int tao_times = 0;

	unsigned int nowtime = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	RandActivityConfig &randactivity_cfg = LOGIC_CONFIG->GetRandActivityCfg();

	unsigned int cooldown_interval = randactivity_cfg.GetHuanLeYaoJiangInterval() * SECOND_PER_MIN;							// 免费淘宝冷却时间

	consume_gold = randactivity_cfg.GetHuanLeYaoJiangTaoGold(type);
	if (consume_gold <= 0)
	{
		return;
	}

	bool has_item = false;
	ItemID item_id = 0;

	if (RA_MIJINGXUNBAO3_CHOU_TYPE_1 == type)                    //获得淘宝花费的金币
	{
		tao_times = 1;

		if (nowtime >= m_activity_info.ra_huanleyaojiang_next_free_tao_timestamp && m_activity_info.ra_huanleyaojiang_daily_free_times >= 1)
		{
			is_free_tao = true;
			consume_gold = 0;
		}
	}
	else if (RA_MIJINGXUNBAO3_CHOU_TYPE_10 == type)
	{
		tao_times = 10;
	}
	else if (RA_MIJINGXUNBAO3_CHOU_TYPE_30 == type)
	{
		tao_times = 30;

		item_id = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeYaoJiangTaoItem();
		if (m_role->GetKnapsack()->HasItem(item_id))
		{
			has_item = true;
			consume_gold = 0;
		}
	}
	else
	{
		return;
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int reward_count = tao_times;

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, reward_count))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	if (reward_count > Protocol::SC_HUANLE_YAOJIANG_MAX_TIMES)
	{
		return;
	}

	static RandActivityMiJingXunBaoConfig reward_item_list[Protocol::SC_HUANLE_YAOJIANG_MAX_TIMES];

	bool ret = randactivity_cfg.GetHuanLeYaoJiangRandomResult(m_activity_info.ra_huanleyaojiang_add_weight_list, randactivity_cfg.GetHuanLeYaoJiangAutoWeightAddCount(), is_free_tao, reward_item_list, reward_count);
	if (!ret)
	{
		return;
	}

	// 物品
	if (RA_MIJINGXUNBAO3_CHOU_TYPE_30 == type)
	{
		if (has_item)
		{
			if (!m_role->GetKnapsack()->ConsumeItem(item_id, 1, "RoleActivity::OnRAhuanleyaojiangTao"))
			{
				return;
			}
		}
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "RoleActivity::OnRAhuanleyaojiangTao"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	//刷新下次免费淘宝时间
	if (is_free_tao)
	{
		--m_activity_info.ra_huanleyaojiang_daily_free_times;
		m_activity_info.ra_huanleyaojiang_next_free_tao_timestamp = nowtime + cooldown_interval;
		//m_activity_info.ra_huanleyaojiang_role_chou_times += tao_times;
		//this->SendRAHuanLeYaoJiangInfo();

		// 免费单抽绑定
		reward_item_list[0].reward_item.is_bind = true;

		if (m_activity_info.ra_huanleyaojiang_daily_free_times == 0)
		{
			m_activity_info.ra_huanleyaojiang_next_free_tao_timestamp = 0;
		}
	}

	m_activity_info.ra_huanleyaojiang_role_chou_times += tao_times;

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAHuanLeYaoJiangTao user[%d, %s] is_free_tao[%d] consume_gold[%d] type[%d]",
		m_role->GetUID(), m_role->GetName(), (int)is_free_tao, consume_gold, type);

	static Protocol::SCRAHuanLeYaoJiangTaoResultInfo scramjxbtri;
	scramjxbtri.count = reward_count;

	ChestItemInfo item_list[Protocol::SC_HUANLE_YAOJIANG_MAX_TIMES];
	int list_num = 0;

	//给予
	for (int i = 0; i < reward_count && i < Protocol::SC_HUANLE_YAOJIANG_MAX_TIMES; ++i)
	{
		item_list[i].Reset();

		item_list[i].item_id = reward_item_list[i].reward_item.item_id;
		item_list[i].num = reward_item_list[i].reward_item.num;
		item_list[i].is_bind = reward_item_list[i].reward_item.is_bind;

		++list_num;
	}

//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, list_num, item_list, "OnRAhuanleyaojiangTao");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG, list_num, item_list, "RoleActivity::OnRAhuanleyaojiangTao");

	for (int index = 0; index < reward_count && index < Protocol::SC_HUANLE_YAOJIANG_MAX_TIMES; ++index)
	{
		gamelog::g_log_roleactivity.buff_printf("(%s), ", ItemConfigDataLog(&reward_item_list[index].reward_item, NULL));

		scramjxbtri.huanleyaojiang_tao_seq[index] = reward_item_list[index].seq;
		if (reward_item_list[index].is_rare)
		{
			// 刷到珍稀物品传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_huanleyaojiang_zhenxi_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG, (int)reward_item_list[index].reward_item.item_id);

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}

		LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG, m_role->GetUID(), m_role->GetName(), reward_item_list[index].reward_item.item_id, reward_item_list[index].reward_item.num);
	}

	//发送淘宝获得的物品信息
	{
		int send_len = sizeof(scramjxbtri) - sizeof(scramjxbtri.huanleyaojiang_tao_seq[0]) * (Protocol::SC_HUANLE_YAOJIANG_MAX_TIMES - scramjxbtri.count);
		if (send_len > 0)
		{
			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scramjxbtri, send_len);
		}
	}

	this->SendRAHuanLeYaoJiangInfo();

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);
}

void RoleActivity::OnRAHuanLeYaoJiangFetchPersonalReward(int seq)
{
	const RandActivityMiJingXunBaoRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityHuanLeYaoJiangPersonalRewardCfg(seq, m_activity_info.ra_huanleyaojiang_role_chou_times);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (0 != (m_activity_info.ra_huanleyaojiang_personal_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(1))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_huanleyaojiang_personal_reward_flag |= 1 << seq;

	m_role->GetKnapsack()->Put(reward_cfg->reward_item, PUT_REASON_HUANLEYAOJIANG_REWARD);

	this->SendRAHuanLeYaoJiangInfo();

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAHuanLeYaoJiangFetchPersonalReward user[%d, %s] reward_flag[%d] reward_item_id[%d] reward_item_num[%d]",
		m_role->GetUID(), m_role->GetName(), m_activity_info.ra_huanleyaojiang_personal_reward_flag, reward_cfg->reward_item.item_id, reward_cfg->reward_item.num);
}

void RoleActivity::SendRAHuanLeYaoJiangInfo()
{
	Protocol::SCRAHuanLeYaoJiangInfo scramjxbi;
	scramjxbi.ra_huanleyaojiang_next_free_tao_timestamp = m_activity_info.ra_huanleyaojiang_next_free_tao_timestamp;
	scramjxbi.chou_times = m_activity_info.ra_huanleyaojiang_role_chou_times;
	scramjxbi.reward_flag = m_activity_info.ra_huanleyaojiang_personal_reward_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scramjxbi, sizeof(scramjxbi));
}

void RoleActivity::OnRALimitTimeRebateOperaReq(int opera_type, int param)
{
	switch (opera_type)
	{
	case Protocol::RA_LIMIT_TIME_REBATE_OPERA_TYPE_INFO:
	{
		this->SendLimitTimeRebateInfo();
	}
	break;

	case Protocol::RA_LIMIT_TIME_REBATE_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnRaLimitTimeRebateFetchReward(param);
	}
	break;
	}
}

void RoleActivity::OnRaLimitTimeRebateFetchReward(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE);

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const LimitTimeRebateItem *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLimitTimeRebateItemCfg(seq);
	if (NULL == cfg)
	{
		return;
	}

	if (0 != (m_activity_info.ra_limit_time_rebate_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_FETCHED);
		return;
	}

	int satisfied_day = 0;
	for (int i = 0; i < m_activity_info.ra_limit_time_rebate_total_chongzhi_days && i < RA_LIMIT_TIME_REBATE_MAX_DAY; i++)
	{
		if (m_activity_info.ra_limit_time_rebate_chongzhi_list[i] >= cfg->chongzhi_cout)
		{
			satisfied_day++;
		}
	}

	if (satisfied_day < cfg->need_day)
	{
		m_role->NoticeNum(errornum::EN_RA_LIMIT_TIME_REBATE_CHONGZHI_DAY_LIMIT);
		return;
	}

	knapsack->Put(cfg->reward_item, PUT_RA_LIMIT_TIME_REBATE_REWARD);

	m_activity_info.ra_limit_time_rebate_reward_flag |= (1 << seq);
	this->SendLimitTimeRebateInfo();
}

void RoleActivity::OnRaLimitTimeRebateAddchongzhi(int chongzhi_count)
{
	if (chongzhi_count <= 0) return;

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE);

	if (0 == m_activity_info.ra_limit_time_rebate_cur_day_is_chongzhi)
	{
		m_activity_info.ra_limit_time_rebate_cur_day_is_chongzhi = 1;
		m_activity_info.ra_limit_time_rebate_total_chongzhi_days++;
	}

	m_activity_info.ra_limit_time_rebate_cur_day_chongzhi += chongzhi_count;

	if (m_activity_info.ra_limit_time_rebate_total_chongzhi_days > 0 && m_activity_info.ra_limit_time_rebate_total_chongzhi_days <= RA_LIMIT_TIME_REBATE_MAX_DAY)
	{
		m_activity_info.ra_limit_time_rebate_chongzhi_list[m_activity_info.ra_limit_time_rebate_total_chongzhi_days - 1] += chongzhi_count;
	}

	this->SendLimitTimeRebateInfo();
	{
		// 日志
		gamelog::g_log_roleactivity.printf(LL_INFO, "[RoleActivity::OnRaLimitTimeRebateAddchongzhi] user[%s %d] cur_day_chongzhi[%d] total_chongzhi_days[%d]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), m_activity_info.ra_limit_time_rebate_cur_day_chongzhi, m_activity_info.ra_limit_time_rebate_total_chongzhi_days);
	}
}

void RoleActivity::SendLimitTimeRebateInfo()
{
	Protocol::SCRALimitTimeRebateInfo msg;
	msg.chongzhi_days = m_activity_info.ra_limit_time_rebate_total_chongzhi_days;
	msg.cur_day_chongzhi = m_activity_info.ra_limit_time_rebate_cur_day_chongzhi;
	msg.reward_flag = m_activity_info.ra_limit_time_rebate_reward_flag;
	memcpy(msg.chongzhi_day_list, m_activity_info.ra_limit_time_rebate_chongzhi_list, sizeof(msg.chongzhi_day_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&msg, sizeof(msg));
}

void RoleActivity::OnRATimeLimitGiftOperaReq(int opera_type, int param, int param2)
{
	switch (opera_type)
	{
	case Protocol::RA_TIMELIMIT_GIFT_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRATimeLimitGiftInfo();
	}
	break;

	case Protocol::RA_TIMELIMIT_GIFT_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnRATimeLimitGiftFetchReward(param, param2);
	}
	break;
	}
}

void RoleActivity::OnRATimeLimitGiftCheckChongzhi(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT);

	const int vip_level = m_activity_info.ra_time_limit_gift_join_vip_level;
	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), m_activity_info.ra_time_limit_gift_begin_timestamp) + 1;
	long long old_recharge_gold = this->GetHistoryChongZhi() - this->GetDayChongzhi();	//除了今日充值的元宝数
	const RandActivityLimitTimeGiftCfg::SubItemCfg *ltg_sub_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLimitTimeGiftRewardCfg(opengame_day, vip_level, old_recharge_gold);
	if (nullptr == ltg_sub_cfg)
	{
		return;
	}

	if (0 != this->m_activity_info.ra_time_limit_gift_reward_can_fetch_flag1 || 0 != m_activity_info.ra_time_limit_gift_reward_has_fetch_flag1)
	{
		if (chongzhi >= ltg_sub_cfg->charge_value2)
		{
			m_activity_info.ra_time_limit_gift_reward_can_fetch_flag2 |= (1 << ltg_sub_cfg->seq);				// 第二次可以领取
			this->SendRATimeLimitGiftInfo();
		}

		//if (ltg_sub_cfg->charge_value3 > 0 && ltg_sub_cfg->gift_value3 > 0)
		//{
		//	if (chongzhi >= ltg_sub_cfg->charge_value3)
		//	{
		//		m_activity_info.ra_time_limit_gift_reward_can_fetch_flag3 |= (1 << ltg_sub_cfg->seq);				// 第三次可以领取
		//		this->SendRATimeLimitGiftInfo();
		//	}
		//}
	}

	else if (chongzhi >= ltg_sub_cfg->charge_value1)
	{
		m_activity_info.ra_time_limit_gift_reward_can_fetch_flag1 |= (1 << ltg_sub_cfg->seq);				// 第一次可以领取
		this->SendRATimeLimitGiftInfo();
	}

	{
		// 日志
		gamelog::g_log_roleactivity.printf(LL_INFO, "[RoleActivity::OnRATimeLimitGiftCheckChongzhi] user[%s %d] can_fetch_flag1[%d] can_fetch_flag2[%d] can_fetch_flag3[%d]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), m_activity_info.ra_time_limit_gift_reward_can_fetch_flag1, m_activity_info.ra_time_limit_gift_reward_can_fetch_flag2,
			m_activity_info.ra_time_limit_gift_reward_can_fetch_flag3);
	}
}

void RoleActivity::SendRATimeLimitGiftInfo()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT))
	{
		return;
	}

	static Protocol::SCRATimeLimitGiftInfo tlgi;
	tlgi.reward_can_fetch_flag1 = m_activity_info.ra_time_limit_gift_reward_can_fetch_flag1;
	tlgi.reward_fetch_flag1 = m_activity_info.ra_time_limit_gift_reward_has_fetch_flag1;
	tlgi.reward_can_fetch_flag2 = m_activity_info.ra_time_limit_gift_reward_can_fetch_flag2;
	tlgi.reward_fetch_flag2 = m_activity_info.ra_time_limit_gift_reward_has_fetch_flag2;
	tlgi.join_vip_level = m_activity_info.ra_time_limit_gift_join_vip_level;
	tlgi.open_flag = m_activity_info.ra_time_limit_gift_open_flag;
	tlgi.begin_timestamp = m_activity_info.ra_time_limit_gift_begin_timestamp;
	tlgi.reward_can_fetch_flag3 = m_activity_info.ra_time_limit_gift_reward_can_fetch_flag3;
	tlgi.reward_fetch_flag3 = m_activity_info.ra_time_limit_gift_reward_has_fetch_flag3;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tlgi, sizeof(tlgi));
}

void RoleActivity::OnRATimeLimitGiftFetchReward(int seq, int fetch_seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT);

	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), m_activity_info.ra_time_limit_gift_begin_timestamp) + 1;

	const RandActivityLimitTimeGiftCfg::SubItemCfg *tlg_sub_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLimitTimeGiftRewardCfgBySeq(opengame_day, seq);
	if (nullptr == tlg_sub_cfg)
	{
		return;
	}

	if (fetch_seq < Protocol::RA_TIMELIMIT_GIFT_FETCH_FIRST || fetch_seq >= Protocol::RA_TIMELIMIT_GIFT_FETCH_END)
	{
		return;
	}

	int chongzhi_num = 0;
	int gift_value = 0;
	int *can_fetch = NULL;
	int *has_fetch = NULL;
	const ItemConfigData *item_config = NULL;
	int item_count = 0;

	switch (fetch_seq)
	{
	case Protocol::RA_TIMELIMIT_GIFT_FETCH_FIRST:
	{
		can_fetch = &m_activity_info.ra_time_limit_gift_reward_can_fetch_flag1;
		has_fetch = &m_activity_info.ra_time_limit_gift_reward_has_fetch_flag1;
		item_config = tlg_sub_cfg->reward_item_list1;
		chongzhi_num = tlg_sub_cfg->charge_value1;
		gift_value = tlg_sub_cfg->gift_value1;
		item_count = tlg_sub_cfg->item_count1;
	}
	break;

	case Protocol::RA_TIMELIMIT_GIFT_FETCH_SECOND:
	{
		can_fetch = &m_activity_info.ra_time_limit_gift_reward_can_fetch_flag2;
		has_fetch = &m_activity_info.ra_time_limit_gift_reward_has_fetch_flag2;
		item_config = tlg_sub_cfg->reward_item_list2;
		chongzhi_num = tlg_sub_cfg->charge_value2;
		gift_value = tlg_sub_cfg->gift_value2;
		item_count = tlg_sub_cfg->item_count2;
	}
	break;

	//case Protocol::RA_TIMELIMIT_GIFT_FETCH_THIRD:
	//{
	//	can_fetch = &m_activity_info.ra_time_limit_gift_reward_can_fetch_flag3;
	//	has_fetch = &m_activity_info.ra_time_limit_gift_reward_has_fetch_flag3;
	//	item_config = tlg_sub_cfg->reward_item_list3;
	//	chongzhi_num = tlg_sub_cfg->charge_value3;
	//	gift_value = tlg_sub_cfg->gift_value3;
	//	item_count = tlg_sub_cfg->item_count3;
	//}
	//break;
	}

	if (NULL == can_fetch || NULL == has_fetch || NULL == item_config)
	{
		return;
	}

	if (0 == (*can_fetch & (1 << tlg_sub_cfg->seq)))
	{
		m_role->NoticeNum(errornum::EN_DANBI_CHONGZHI_REWARD_HAS_NOT_ACTIVE);
		return;
	}

	if (0 != (*has_fetch & (1 << tlg_sub_cfg->seq)))
	{
		m_role->NoticeNum(errornum::EN_DANBI_CHONGZHI_REWARD_HAS_FETCH);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 消耗
	*has_fetch |= (1 << tlg_sub_cfg->seq);
	*can_fetch &= ~(1 << tlg_sub_cfg->seq);

	// 发奖励
	m_role->GetKnapsack()->PutListOrMail(item_config, PUT_REASON_TIME_LIMIT_GIFT);

	if (fetch_seq + 1 >= Protocol::RA_TIMELIMIT_GIFT_FETCH_END)
	{
		m_activity_info.ra_time_limit_gift_open_flag = 0;
	}

	this->SendRATimeLimitGiftInfo();

	// 传闻
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_time_limit_gift_reward_content,
		m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), chongzhi_num, gift_value);
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
	}
}

void RoleActivity::OnRACirculationChongzhiSecondOperaReq(int opera_type)
{
	switch (opera_type)
	{
	case Protocol::CIRCULATION_CHONGZHI_OPERA_TYPE_QUERY_INFO:
	{
		this->SendCirculationChongzhiSecondInfo();
	}
	break;

	case Protocol::CIRCULATION_CHONGZHI_OPEAR_TYPE_FETCH_REWARD:
	{
		this->OnCirculationChongzhiSecondFetchReward();
	}
	break;
	}
}

void RoleActivity::OnRACirculationChongzhiSecondAddchongzhi(int chongzhi_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2);

	m_activity_info.ra_circulation_chongzhi_2_total_chongzhi_gold += chongzhi_gold;
	m_activity_info.ra_circulation_chongzhi_2_cur_chongzhi_gold += chongzhi_gold;

	this->SendCirculationChongzhiSecondInfo();
}

void RoleActivity::SendCirculationChongzhiSecondInfo()
{
	static Protocol::SCCirculationChongzhiInfo cci;
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2);

	cci.total_chongzhi = m_activity_info.ra_circulation_chongzhi_2_total_chongzhi_gold;
	cci.cur_chongzhi = m_activity_info.ra_circulation_chongzhi_2_cur_chongzhi_gold;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cci, sizeof(cci));
}

void RoleActivity::OnCirculationChongzhiSecondFetchReward()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2);

	int need_chongzhi = LOGIC_CONFIG->GetRandActivityCfg().GetCirculationChongzhiSecondNeedChongzhi();
	if (need_chongzhi <= 0)
	{
		return;
	}

	int can_fetch_times = m_activity_info.ra_circulation_chongzhi_2_cur_chongzhi_gold / need_chongzhi;
	if (can_fetch_times <= 0)
	{
		return;
	}

	const ItemConfigData *item = LOGIC_CONFIG->GetRandActivityCfg().GetCirculationChongzhiSecondRewardItem(m_role->GetLevel());
	if (nullptr == item || item->num <= 0)
	{
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(item->item_id);
	if (nullptr == item_base)
	{
		return;
	}

	if (item->num * can_fetch_times > item_base->GetPileLimit())
	{
		can_fetch_times = item_base->GetPileLimit() / item->num;
	}

	// 消耗
	m_activity_info.ra_circulation_chongzhi_2_cur_chongzhi_gold -= need_chongzhi * can_fetch_times;

	// 给与
	ItemConfigData put_item(*item);
	put_item.num = item->num * can_fetch_times;

	m_role->GetKnapsack()->PutOrMail(put_item, PUT_REASON_CIRCULATION_CHONGZHI);

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnCirculationChongzhiSecondFetchReward uid:%d name:%s level:%d times:%d", 
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), can_fetch_times);

	this->SendCirculationChongzhiSecondInfo();
}

// ------------------------------
// 疯狂摇钱树
// ------------------------------
void RoleActivity::OnRAShakeMoneyOperaReq(int opera_type, int param)
{
	switch (opera_type)
	{
	case Protocol::RA_SHAKEMONEY_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRAShakeMoneyInfo();
	}
	break;

	case Protocol::RA_SHAKEMONEY_OPERA_TYPE_FETCH_GOLD:
	{
		this->OnRAShakeMoneyFetchGold();
	}
	break;
	}
}

void RoleActivity::OnRAShakeMoneyCheckChongzhi(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SHAKE_MONEY))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SHAKE_MONEY);

	m_activity_info.ra_shakemoney_history_chongzhi_num += chongzhi;
	this->SendRAShakeMoneyInfo();
}

void RoleActivity::OnRAShakeMoneyFetchGold()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SHAKE_MONEY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SHAKE_MONEY);

	const unsigned int timestamp = RandActivityManager::Instance().GetRanadActivityOpenTime(RAND_ACTIVITY_TYPE_SHAKE_MONEY);
	if (timestamp <= 0)
	{
		return;
	}

	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), timestamp) + 1;
	const RandActivityShakeMoneyConfig *shake_money_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetShakeMoneyCfg(opengame_day);
	if (NULL == shake_money_cfg)
	{
		return;
	}

	int can_fetch_num = (m_activity_info.ra_shakemoney_history_chongzhi_num * shake_money_cfg->chongzhi_return / 100) - m_activity_info.ra_shakemoney_fetched_num;
	int max_can_fetch_num = shake_money_cfg->return_max - m_activity_info.ra_shakemoney_fetched_num;
	can_fetch_num = can_fetch_num > max_can_fetch_num ? max_can_fetch_num : can_fetch_num;

	if (can_fetch_num <= 0)
	{
		gamelog::g_log_rand_activity.printf(LL_INFO, "server_shakemoney_chongzhi_limit");
		return;
	}

	m_activity_info.ra_shakemoney_fetched_num += can_fetch_num;

	m_role->GetKnapsack()->GetMoney()->AddGold(can_fetch_num, "OnRAShakeMoneyAddGold");

	this->SendRAShakeMoneyInfo();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAShakeMoneyAddGold::user[%d, %s], chongzhi_gold[%d]",
		m_role->GetUID(), m_role->GetName(), can_fetch_num);
}

void RoleActivity::SendRAShakeMoneyInfo()
{
	static Protocol::SCRAShakeMoneyInfo info;
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SHAKE_MONEY);
	info.total_chongzhi_gold = m_activity_info.ra_shakemoney_history_chongzhi_num;
	info.chongzhi_gold = m_activity_info.ra_shakemoney_fetched_num;
	info.seq = 1;

	const unsigned int timestamp = RandActivityManager::Instance().GetRanadActivityOpenTime(RAND_ACTIVITY_TYPE_SHAKE_MONEY);
	if (timestamp > 0)
	{
		int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), timestamp) + 1;
		const RandActivityShakeMoneyConfig *shake_money_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetShakeMoneyCfg(opengame_day);
		if (shake_money_cfg != nullptr)
		{
			info.seq = shake_money_cfg->seq;
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&info, sizeof(info));
}

// ------------------------------
// 限时豪礼
// ------------------------------

void RoleActivity::OnRATimeLimitLuxuryGiftBagOperaReq(int opera_type, int param)
{
	switch (opera_type)
	{
	case Protocol::RA_TIMELIMIT_LUXURY_GIFT_BAG_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRATimeLimitLuxuryGiftBagInfo();
	}
	break;

	case Protocol::RA_TIMELIMIT_LUXURY_GIFT_BAG_OPERA_TYPE_BUY:
	{
		this->OnRATimeLimitLuxuryGiftBagBuy(param);
	}
	break;
	}
}

void RoleActivity::OnRATimeLimitLuxuryGiftBagBuy(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LIMIT_LUXURY_GIFT_BAG_GIFT))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LIMIT_LUXURY_GIFT_BAG_GIFT);


	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), m_activity_info.ra_time_limit_luxury_gift_bag_begin_timestamp) + 1;

	const RandActivityLimitTimeLuxuryGiftBagCfg::SubItemCfg *tlg_sub_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLimitTimeLuxuryGiftBagRewardCfgBySeq(opengame_day, seq);
	if (nullptr == tlg_sub_cfg)
	{
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(tlg_sub_cfg->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	Money* money = m_role->GetKnapsack()->GetMoney();

	if (!money->GoldMoreThan(tlg_sub_cfg->need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	// 消耗
	if (!money->UseGold(tlg_sub_cfg->need_gold, "OnRATimeLimitLuxuryGiftBagBuy"))
	{
		return;
	}

	m_activity_info.ra_time_limit_luxury_gift_bag_is_already_buy = 1;
	m_activity_info.ra_time_limit_luxury_gift_open_flag = 0;
	// 发奖励
	m_role->GetKnapsack()->PutList(MAX_ATTACHMENT_ITEM_NUM, tlg_sub_cfg->reward_item_list, PUT_REASON_TIME_LIMIT_LIUXURY_GIFT_BAG);
	this->SendRATimeLimitLuxuryGiftBagInfo();

	// 传闻
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_time_limit_luxury_gifts_content,
		m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), tlg_sub_cfg->need_gold, tlg_sub_cfg->gift_value);
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
	}
}

void RoleActivity::SendRATimeLimitLuxuryGiftBagInfo()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LIMIT_LUXURY_GIFT_BAG_GIFT))
	{
		return;
	}

	static Protocol::SCRATimeLimitLuxuryGiftBagInfo tlgi;
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LIMIT_LUXURY_GIFT_BAG_GIFT);
	tlgi.is_already_buy = m_activity_info.ra_time_limit_luxury_gift_bag_is_already_buy;
	tlgi.join_vip_level = m_activity_info.ra_time_limit_luxury_gift_bag_join_vip_level;
	tlgi.begin_timestamp = m_activity_info.ra_time_limit_luxury_gift_bag_begin_timestamp;
	tlgi.time_limit_luxury_gift_open_flag = m_activity_info.ra_time_limit_luxury_gift_open_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tlgi, sizeof(tlgi));
}

void RoleActivity::OnRADoubleChongzhiOpera(int opera_type)
{
	switch (opera_type)
	{
	case Protocol::RA_DOUBLE_CHONGZHI_OPERA_TYPE_INFO:
	{
		this->SendRADoubleChongzhiInfo();
	}
	break;
	}
}

void RoleActivity::OnRADoubleChongzhiChongzhi(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI);

	const ChongzhiReward *reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(chongzhi);
	if (nullptr == reward_cfg)
	{
		return;
	}

	if (CHONGZHI_EXTRA_REWARD_TYPE_GOLD_BIND != reward_cfg->reward_type)
	{
		return;
	}

	if (0 != (1 << reward_cfg->seq & m_activity_info.ra_reset_double_chongzhi_reward_flag))
	{
		return;
	}

	if (reward_cfg->openserver_extra_gold_bind > 0)
	{
		static MailContentParam contentparam; contentparam.Reset();
		
		contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, reward_cfg->openserver_extra_gold_bind);

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_double_chongzhi_reward_content,
			RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI, chongzhi, reward_cfg->openserver_extra_gold_bind);

		if (length > 0)
		{
			m_activity_info.ra_reset_double_chongzhi_reward_flag |= (1 << reward_cfg->seq);

			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			this->SendRADoubleChongzhiInfo();

			gamelog::g_log_rand_activity.printf(LL_INFO, "OnRADoubleChongzhiChongzhi::user[%d, %s], chongzhi_gold %d, reward_bind_gold %d",
				m_role->GetUID(), m_role->GetName(), chongzhi, reward_cfg->openserver_extra_gold_bind);
		}
	}
}

void RoleActivity::SendRADoubleChongzhiInfo()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI);

	static Protocol::SCRAResetDoubleChongzhi pro;
	pro.chongzhi_reward_flag = m_activity_info.ra_reset_double_chongzhi_reward_flag;
	pro.open_flag = m_activity_info.ra_reset_double_chongzhi_open_flag;
	m_activity_info.ra_reset_double_chongzhi_open_flag = 0;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void RoleActivity::OnRARmbBugChestShopReq()
{
	this->SendRARmbBugChestShopInfo();
}

bool RoleActivity::OnRARmbBugChestShopChongzhi(const int chongzhi_num)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_RMB_BUT_CHEST_SHOP))
	{
		return false;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_RMB_BUT_CHEST_SHOP);

	auto *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRmbBuyChestShopCfg(chongzhi_num);
	if (nullptr == cfg)
	{
		return false;
	}

	if (cfg->index < 0 || cfg->index >= RA_RMB_BUY_CHEST_SHOP_MAX_BUY_COUNT)
	{
		return false;
	}

	auto &count = m_activity_info.ra_rmb_buy_chest_shop_buy_count_list[cfg->index];
	if (count >= cfg->count_limit)
	{
		return false;
	}

	m_role->GetKnapsack()->PutOrMail(cfg->reward_item, PUT_REASON_RA_RMB_BUY_CHEST_SHOP);
	++count;

	this->SendRARmbBugChestShopInfo();

	return true;
}

void RoleActivity::SendRARmbBugChestShopInfo()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_RMB_BUT_CHEST_SHOP);

	static Protocol::SCRARmbBugChestShopInfo rbci;
	memcpy(rbci.buy_count_list, m_activity_info.ra_rmb_buy_chest_shop_buy_count_list, sizeof(rbci.buy_count_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&rbci, sizeof(rbci));
}

void RoleActivity::OnRAConsumeGoldRewardOpera(int opera_type)
{
	switch (opera_type)
	{
	case Protocol::RA_CONSUME_GOLD_REWARD_OPERATE_TYPE_INFO:
	{
		this->SendRAConsumeGoldRewardInfo();
	}
	break;

	case Protocol::RA_CONSUME_GOLD_REWARD_OPERATE_TYPE_FETCH:
	{
		this->OnConsumeGoldRewardFetchReward();
	}
	break;

	default:
		break;
	}
}

void RoleActivity::OnRAConsumeGoldRewardConsumeGold(int consume_gold)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD);

	m_activity_info.ra_consume_gold_reward_consume_gold += consume_gold;

	if (0 == m_activity_info.ra_consume_gold_reward_activity_day)
	{
		const RandActivityConsumeRewardStageCfg *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetConsumeRewardCfg(m_activity_info.ra_consume_gold_reward_today_vip_level);
		if (nullptr != cfg)
		{
			// 达到目标了，记录下来这个天数
			if (m_activity_info.ra_consume_gold_reward_consume_gold >= cfg->consume_gold)
			{
				time_t begin_time = ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD);
				time_t begin_zero_time = GetZeroTime(begin_time);

				m_activity_info.ra_consume_gold_reward_activity_day = GetDayIndex(begin_zero_time, EngineAdapter::Instance().Time()) + 1;
			}
		}
	}

	this->SendRAConsumeGoldRewardInfo();
}

void RoleActivity::SendRAConsumeGoldRewardInfo()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD);

	time_t begin_time = ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD);
	time_t begin_zero_time = GetZeroTime(begin_time);
	int activity_day = GetDayIndex(begin_zero_time, EngineAdapter::Instance().Time());

	static Protocol::SCRAConsumeGoldRewardInfo pro;
	pro.consume_gold = m_activity_info.ra_consume_gold_reward_consume_gold;
	pro.vip_level = m_activity_info.ra_consume_gold_reward_today_vip_level;
	pro.fetch_reward_flag = m_activity_info.ra_consume_gold_reward_fetch_flag;
	pro.activity_day = static_cast<short>(activity_day);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void RoleActivity::OnConsumeGoldRewardFetchReward()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD);

	const RandActivityConsumeRewardStageCfg *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetConsumeRewardCfg(m_activity_info.ra_consume_gold_reward_today_vip_level);
	if (nullptr == cfg)
	{
		return;
	}

	if (m_activity_info.ra_consume_gold_reward_consume_gold < cfg->consume_gold)
	{
		return;
	}

	if (0 != m_activity_info.ra_consume_gold_reward_fetch_flag)
	{
		return;
	}

	m_activity_info.ra_consume_gold_reward_fetch_flag = 1;
	m_role->GetKnapsack()->PutListOrMail(cfg->reward_item_list, PUT_REASON_RA_CONSUME_GOLD_REWARD);

	// 日志
	gamelog::g_log_roleactivity.printf(LL_INFO, "OnConsumeGoldRewardFetchReward user[%d %s], vip_level[%d], consume_gold[%d]",
		m_role->GetUID(), m_role->GetName(), m_activity_info.ra_consume_gold_reward_today_vip_level, m_activity_info.ra_consume_gold_reward_consume_gold);
}

void RoleActivity::OnRAItemCollectionSecondOperaReq(int opera_type, int param1)
{
	switch (opera_type)
	{
	case Protocol::RA_ITEM_COLLECTION_SECOND_OPERA_TYPE_QUERY_INFO:
	{
		this->SendCollectSecondExchangeInfo();
	}
	break;

	case Protocol::RA_ITEM_COLLECTION_SECOND_OPERA_TYPE_EXCHANGE:
	{
		this->OnRAItemCollectionSecondExchange(param1);
	}
	break;

	default:
		break;
	}
}

void RoleActivity::OnRAItemCollectionSecondExchange(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2);

	if (m_role->GetSublock()->CheckHasLock())
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (seq < 0 || seq >= RAND_ACTIVITY_ITEM_COLLECTION_SECOND_REWARD_MAX_COUNT)
	{
		return;
	}

	const RandActivityItemCollectionSecondRewardCfg::ConfigItem *reward_config = LOGIC_CONFIG->GetRandActivityCfg().GetItemCollectionSecondRewardCfg(seq);
	if (NULL == reward_config) return;

	if (m_activity_info.collection_exchange_2_times[seq] >= reward_config->exchange_times_limit)
	{
		m_role->NoticeNum(errornum::NT_CONVERTSHOP_CONVERT_COUNT_LIMIT);
		return;
	}

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	short stuff_count = 0;
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

	for (int i = 0; i < reward_config->stuff_count && i < RandActivityItemCollectionSecondRewardCfg::MAX_COLLECTION_STUFF_COUNT; i++)
	{
		if (reward_config->stuff_id_list[i].item_id > 0)
		{
			stuff_list[stuff_count].item_id = reward_config->stuff_id_list[i].item_id;
			stuff_list[stuff_count].num = reward_config->stuff_id_list[i].num;
			stuff_list[stuff_count].buyable = false;
			++stuff_count;
		}
	}

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
	{
		return;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "RoleActivity::OnRAItemCollectionSecondExchange"))
	{
		return;
	}

	int grid_index = -1;
	if (!m_role->GetKnapsack()->Put(reward_config->reward_item, PUT_REASON_ITEM_COLLECTION_SECOND_REWARD, &grid_index))
	{
		return;
	}

	m_activity_info.collection_exchange_2_times[seq]++;
	this->SendCollectSecondExchangeInfo();

	this->OnRAExtremeChallengeAddPlan(RA_EXTREME_CHALLENGE_COMPOUND, 1);
	m_role->NoticeNum(noticenum::NT_CONVERTSHOP_CONVERT_SUCC);

	// 日志
	gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnRAItemCollectionSecondExchange user[%d, %s], seq[%d], reward[%s]",
		m_role->GetUID(), m_role->GetName(), seq, ItemConfigDataLog(&reward_config->reward_item, NULL));
}

void RoleActivity::SendCollectSecondExchangeInfo()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2);

	Protocol::SCCollectSecondExchangeInfo cmd;

	for (int i = 0; i < RAND_ACTIVITY_ITEM_COLLECTION_SECOND_REWARD_MAX_COUNT; i++)
	{
		cmd.collection_exchange_times[i] = m_activity_info.collection_exchange_2_times[i];
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}

void RoleActivity::OnRAVersionTotalChargeOperaReq(int opera_type, int param)
{
	switch (opera_type)
	{
	case Protocol::RA_VERSION_TOTAL_CHARGE_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRAVersionTotalChargeInfo();
	}
	break;

	case Protocol::RA_VERSION_TOTAL_CHARGE_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnRAVersionTotalChargeFetchReward(param);
	}
	break;
	}
}

void RoleActivity::OnRAVersionTotalChargeCheckChongzhi(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE);

	m_activity_info.ra_version_total_charge_value += (int)chongzhi;

	this->SendRAVersionTotalChargeInfo();
}

void RoleActivity::SendRAVersionTotalChargeInfo()
{
	Protocol::SCRAVersionTotalChargeInfo tcgi;
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE);
	tcgi.reward_has_fetch_flag = m_activity_info.ra_version_total_charge_reward_has_fetch_flag;
	tcgi.total_charge_value = m_activity_info.ra_version_total_charge_value;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tcgi, sizeof(tcgi));
}

void RoleActivity::OnRAVersionTotalChargeFetchReward(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE);

	const RandActivityTotalChargeRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetVersionTotalChargeRewardWithSeq(seq);
	if (nullptr == cfg_item)
	{
		return;
	}

	if (0 != (m_activity_info.ra_version_total_charge_reward_has_fetch_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_FETCH_REWARD);
		return;
	}

	if (m_activity_info.ra_version_total_charge_value < cfg_item->need_chongzhi)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_TOTAL_CHONGZHI_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg_item->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_version_total_charge_reward_has_fetch_flag |= (1 << seq);
	this->SendRAVersionTotalChargeInfo();

	m_role->GetKnapsack()->PutList(cfg_item->item_count, cfg_item->item_list, PUT_REASON_VERSION_GRAND_TOTAL_CHARGE);

	if (cfg_item->item_count > 0)
	{
		// 累计充值传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_version_total_charge_reward_content,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), cfg_item->need_chongzhi);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	{
		// 日志
		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

		gamelog::g_log_roleactivity.printf(LL_INFO, "OnRAVersionTotalChargeFetchReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d], item_list[%s]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.ra_version_total_charge_reward_has_fetch_flag, get_item_log.c_str());
	}
}

void RoleActivity::OnRAVersionContinueGhargeOperaReq(int opera_type, int param1)
{
	switch (opera_type)
	{
	case Protocol::RA_VERSION_CONTINUE_CHONGZHI_OPERA_TYPE_QUERY_INFO:
	{
		this->SendRAVersionContinueChargeActivityInfo();
	}
	break;

	case Protocol::RA_VERSION_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_REWARD:
	{
		this->OnRAVersionContinueChargeFetchReward(param1);
	}
	break;
	}
}

void RoleActivity::OnRAVersionContinueChargeAddChongzhi(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE);

	m_activity_info.ra_version_continue_chongzhi_today_chongzhi += (unsigned int)chongzhi;

	int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE), EngineAdapter::Instance().Time()) + 1;
	if (m_activity_info.ra_version_continue_chongzhi_days >= day_index || 1 == m_activity_info.ra_version_is_lei_chongzhi)
	{
		this->SendRAVersionContinueChargeActivityInfo();
		return;
	}

	const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetVersionContinueChargeRewardInfoWithdayindex(m_activity_info.ra_version_continue_chongzhi_days + 1, true, false);
	if (nullptr == continue_chongzhi_info_cfg)
	{
		return;
	}

	if (m_activity_info.ra_version_continue_chongzhi_today_chongzhi >= continue_chongzhi_info_cfg->need_chongzhi)
	{
		m_activity_info.ra_version_continue_chongzhi_days++;
		m_activity_info.ra_version_is_lei_chongzhi = 1;

		// 激活每日充值达标奖励
		m_activity_info.ra_version_continue_chongzhi_can_fetch_reward_flag |= (1 << m_activity_info.ra_version_continue_chongzhi_days);
	}

	this->SendRAVersionContinueChargeActivityInfo();
}

void RoleActivity::SendRAVersionContinueChargeActivityInfo()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE);
	static Protocol::SCRAVersionContinueChongzhiInfo cczi;
	cczi.today_chongzhi = m_activity_info.ra_version_continue_chongzhi_today_chongzhi;
	cczi.can_fetch_reward_flag = m_activity_info.ra_version_continue_chongzhi_can_fetch_reward_flag;
	cczi.has_fetch_reward_falg = m_activity_info.ra_version_continue_chongzhi_has_fetch_reward_flag;
	cczi.continue_chongzhi_days = m_activity_info.ra_version_continue_chongzhi_days;
	cczi.reserve1 = 0;
	cczi.reserve2 = 0;

	unsigned int send_len = sizeof(cczi);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cczi, send_len);
}

void RoleActivity::OnRAVersionContinueChargeFetchReward(int day_index)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE);

	if (day_index <= 0 || day_index > static_cast<int>(m_activity_info.ra_version_continue_chongzhi_days))
	{
		return;
	}

	if (0 == (m_activity_info.ra_version_continue_chongzhi_can_fetch_reward_flag & (1 << day_index)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_NOT_ACTIVE);
		return;
	}

	if (0 != (m_activity_info.ra_version_continue_chongzhi_has_fetch_reward_flag & (1 << day_index)))
	{
		m_role->NoticeNum(errornum::EN_CONTINUE_CHONGZHI_REWARD_HAS_FETCH);
		return;
	}

	const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetVersionContinueChargeRewardInfoWithdayindex(day_index, false, false);
	if (nullptr == continue_chongzhi_info_cfg)
	{
		return;
	}

	const ItemBase *reward_item_base = ITEMPOOL->GetItem(continue_chongzhi_info_cfg->reward_item.item_id);
	if (nullptr == reward_item_base || reward_item_base->GetPileLimit() <= 0)
	{
		return;
	}

	int need_empty_slot_num = 1;
	need_empty_slot_num = continue_chongzhi_info_cfg->reward_item.num / reward_item_base->GetPileLimit();
	if (continue_chongzhi_info_cfg->reward_item.num % reward_item_base->GetPileLimit() > 0)
	{
		++need_empty_slot_num;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_empty_slot_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(continue_chongzhi_info_cfg->reward_item, PUT_REASON_VERSION_GRAND_TOTAL_CHARGE))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_version_continue_chongzhi_has_fetch_reward_flag |= (1 << day_index);

	this->SendRAVersionContinueChargeActivityInfo();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAVersionContinueChargeFetchReward::user[%d, %s], index[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), day_index, ItemConfigDataLog(&continue_chongzhi_info_cfg->reward_item, nullptr));
}

void RoleActivity::OnRaHuanLeYaoJiang2OperReq(int opera_type, int param)
{
	switch (opera_type)
	{
	case Protocol::RA_HUANLEYAOJIANG_OPERA_2_TYPE_QUERY_INFO:
	{
		this->SendRAHuanLeYaoJiang2Info();
	}
	break;

	case Protocol::RA_HUANLEYAOJIANG_OPERA_2_TYPE_TAO:
	{
		this->OnRAHuanLeYaoJiang2Tao(param);
	}
	break;

	case Protocol::RA_HUANLEYAOJIANG_OPERA_2_TYPE_FETCH_REWARD:
	{
		this->OnRAHuanLeYaoJiang2FetchPersonalReward(param);
	}
	break;

	}
}

void RoleActivity::OnRAHuanLeYaoJiang2Tao(int type)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2);

	if (type < RA_MIJINGXUNBAO3_CHOU_TYPE_1 || type >= RA_MIJINGXUNBAO3_CHOU_TYPE_MAX)
	{
		return;
	}

	int consume_gold = 0;
	bool is_free_tao = false;
	int tao_times = 0;

	unsigned int nowtime = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	RandActivityConfig &randactivity_cfg = LOGIC_CONFIG->GetRandActivityCfg();

	unsigned int cooldown_interval = randactivity_cfg.GetHuanLeYaoJiang2Interval() * SECOND_PER_MIN;							// 免费淘宝冷却时间

	consume_gold = randactivity_cfg.GetHuanLeYaoJiang2TaoGold(type);
	if (consume_gold <= 0)
	{
		return;
	}

	bool has_item = false;
	ItemID item_id = 0;

	if (RA_MIJINGXUNBAO3_CHOU_TYPE_1 == type)                    //获得淘宝花费的金币
	{
		tao_times = 1;

		if (nowtime >= m_activity_info.ra_huanleyaojiang_2_next_free_tao_timestamp && m_activity_info.ra_huanleyaojiang_2_daily_free_times >= 1)
		{
			--m_activity_info.ra_huanleyaojiang_2_daily_free_times;
			is_free_tao = true;
			consume_gold = 0;
		}
	}
	else if (RA_MIJINGXUNBAO3_CHOU_TYPE_10 == type)
	{
		tao_times = 10;
	}
	else if (RA_MIJINGXUNBAO3_CHOU_TYPE_30 == type)
	{
		tao_times = 30;

		item_id = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeYaoJiang2TaoItem();
		if (m_role->GetKnapsack()->HasItem(item_id))
		{
			has_item = true;
			consume_gold = 0;
		}
	}
	else
	{
		return;
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int reward_count = tao_times;

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, reward_count))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE);
		return;
	}

	if (reward_count > Protocol::SC_HUANLE_YAOJIANG_2_MAX_TIMES)
	{
		return;
	}

	static RandActivityMiJingXunBaoConfig reward_item_list[Protocol::SC_HUANLE_YAOJIANG_2_MAX_TIMES];

	bool ret = randactivity_cfg.GetHuanLeYaoJiang2RandomResult(m_activity_info.ra_huanleyaojiang_2_add_weight_list, randactivity_cfg.GetHuanLeYaoJiang2AutoWeightAddCount(), is_free_tao, reward_item_list, reward_count);
	if (!ret)
	{
		return;
	}

	// 物品
	if (RA_MIJINGXUNBAO3_CHOU_TYPE_30 == type)
	{
		if (has_item)
		{
			if (!m_role->GetKnapsack()->ConsumeItem(item_id, 1, "RoleActivity::OnRAhuanleyaojiang2Tao"))
			{
				return;
			}
		}
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "RoleActivity::OnRAhuanleyaojiang2Tao"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	//刷新下次免费淘宝时间
	if (is_free_tao)
	{
		m_activity_info.ra_huanleyaojiang_2_next_free_tao_timestamp = nowtime + cooldown_interval;

		// 免费单抽绑定
		reward_item_list[0].reward_item.is_bind = true;

		if (m_activity_info.ra_huanleyaojiang_2_daily_free_times == 0)
		{
			m_activity_info.ra_huanleyaojiang_2_next_free_tao_timestamp = 0;
		}
	}

	m_activity_info.ra_huanleyaojiang_2_role_chou_times += tao_times;

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAHuanLeYaoJiang2Tao user[%d, %s] is_free_tao[%d] consume_gold[%d] type[%d]",
		m_role->GetUID(), m_role->GetName(), (int)is_free_tao, consume_gold, type);

	static Protocol::SCRAHuanLeYaoJiangTwoTaoResultInfo scramjxbtri;
	scramjxbtri.count = reward_count;

	ChestItemInfo item_list[Protocol::SC_HUANLE_YAOJIANG_2_MAX_TIMES];
	int list_num = 0;

	//给予
	for (int i = 0; i < reward_count && i < Protocol::SC_HUANLE_YAOJIANG_2_MAX_TIMES; ++i)
	{
		item_list[i].Reset();

		item_list[i].item_id = reward_item_list[i].reward_item.item_id;
		item_list[i].num = reward_item_list[i].reward_item.num;
		item_list[i].is_bind = reward_item_list[i].reward_item.is_bind;

		++list_num;

		LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2, m_role->GetUID(), m_role->GetName(), reward_item_list[i].reward_item.item_id, reward_item_list[i].reward_item.num);
	}

//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, list_num, item_list, "OnRAhuanleyaojiangTao2");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2, list_num, item_list, "RoleActivity::OnRAhuanleyaojiangTao2");

	for (int index = 0; index < reward_count && index < Protocol::SC_HUANLE_YAOJIANG_2_MAX_TIMES; ++index)
	{
		gamelog::g_log_roleactivity.buff_printf("(%s), ", ItemConfigDataLog(&reward_item_list[index].reward_item, NULL));

		scramjxbtri.huanleyaojiang_tao_seq[index] = reward_item_list[index].seq;
		if (reward_item_list[index].is_rare)
		{
			// 刷到珍稀物品传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_rand_activity_huanleyaojiang_2_zhenxi_reward_content,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2, (int)reward_item_list[index].reward_item.item_id);

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}
		}
	}

	//发送淘宝获得的物品信息
	{
		int send_len = sizeof(scramjxbtri) - sizeof(scramjxbtri.huanleyaojiang_tao_seq[0]) * (Protocol::SC_HUANLE_YAOJIANG_2_MAX_TIMES - scramjxbtri.count);
		if (send_len > 0)
		{
			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scramjxbtri, send_len);
		}
	}

	if (consume_gold > 0)
	{
		EventHandler::Instance().OnBanBenChouJiang(m_role, tao_times);
	}

	this->SendRAHuanLeYaoJiang2Info();

	gamelog::g_log_roleactivity.buff_printf("]");
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);
}

void RoleActivity::SendRAHuanLeYaoJiang2Info()
{
	Protocol::SCRAHuanLeYaoJiangTwoInfo scramjxbi;
	scramjxbi.ra_huanleyaojiang_next_free_tao_timestamp = m_activity_info.ra_huanleyaojiang_2_next_free_tao_timestamp;
	scramjxbi.reward_flag = m_activity_info.ra_huanleyaojiang_2_personal_reward_flag;
	scramjxbi.chou_times = m_activity_info.ra_huanleyaojiang_2_role_chou_times;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scramjxbi, sizeof(scramjxbi));
}

void RoleActivity::OnRAHuanLeYaoJiang2FetchPersonalReward(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2);

	const RandActivityMiJingXunBaoRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityHuanLeYaoJiang2PersonalRewardCfg(seq, m_activity_info.ra_huanleyaojiang_2_role_chou_times);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (0 != (m_activity_info.ra_huanleyaojiang_2_personal_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(1))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_activity_info.ra_huanleyaojiang_2_personal_reward_flag |= 1 << seq;

	m_role->GetKnapsack()->Put(reward_cfg->reward_item, PUT_REASON_HUANLE_YAOJIANG2);

	this->SendRAHuanLeYaoJiang2Info();

	gamelog::g_log_roleactivity.buff_printf("RoleActivity::OnRAHuanLeYaoJiang2FetchPersonalReward user[%d, %s] reward_flag[%d] reward_item_id[%d] reward_item_num[%d]",
		m_role->GetUID(), m_role->GetName(), m_activity_info.ra_huanleyaojiang_2_personal_reward_flag, reward_cfg->reward_item.item_id, reward_cfg->reward_item.num);
}

void RoleActivity::SendRAOfflineSingleChargeInfo()
{
	static Protocol::SCRAOfflineSingleChargeInfo0 pro;
	pro.activity_type = RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0;
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0);
	pro.charge_max_value = m_activity_info.ra_offline_single_charge_max_value_0;
	memcpy(pro.reward_times, m_activity_info.ra_offline_single_charge_reward_times_list_0, sizeof(pro.reward_times));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void RoleActivity::OnRAOfflineSingleChargeChongZhi(int chongzhi_num)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0);

	const RandActivityOfflineSingleChargeCfg::ConfigItem *sc_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetOfflineSingleChargeCfg0(chongzhi_num);
	if (NULL == sc_cfg)
	{
		return;
	}

	// 拿最高档奖励
	if (RandActivityOfflineSingleChargeCfg::REWARD_TYPE_MAX_GIFT == sc_cfg->reward_type)
	{
		m_activity_info.ra_offline_single_charge_max_value_0 =
			chongzhi_num > m_activity_info.ra_offline_single_charge_max_value_0 ? chongzhi_num : m_activity_info.ra_offline_single_charge_max_value_0;

		this->SendRAOfflineSingleChargeInfo();
		return;
	}

	if (sc_cfg->seq < 0 || sc_cfg->seq >= RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT)
	{
		return;
	}

	if (m_activity_info.ra_offline_single_charge_reward_times_list_0[sc_cfg->seq] >= sc_cfg->reward_limit)
	{
		return;
	}

	++m_activity_info.ra_offline_single_charge_reward_times_list_0[sc_cfg->seq];

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

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_offline_single_charge_reward_content, chongzhi_num);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		this->SendRAOfflineSingleChargeInfo();

		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAOfflineSingleChargeCheckCharge0::user[%d, %s], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&sc_cfg->reward_item, NULL));
	}
}

void RoleActivity::OnRABuyOneGetOneFreeOperaReq(int opera_type, int prama1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE);

	switch (opera_type)
	{
	case Protocol::RA_BUY_ONE_GET_ONE_FREE_OPERA_TYPE_INFO:
	{
		this->SendRABuyOneGetOneFreeInfo();
	}
	break;

	case Protocol::RA_BUY_ONE_GET_ONE_FREE_OPERA_TYPE_BUY:
	{
		this->RABuyOneGetOneFreeBuy(prama1);
	}
	break;

	case Protocol::RA_BUY_ONE_GET_ONE_FREE_OPERA_TYPE_FETCH_REWARD:
	{
		this->RABuyOneGetOneFreeFetchReward(prama1);
	}
	break;

	default:
		break;
	}
}

void RoleActivity::SendRABuyOneGetOneFreeInfo()
{
	Protocol::SCBuyOneGetOneFreeInfo msg;

	msg.buy_flag = m_activity_info.ra_buy_one_get_one_free_buy_flag;
	msg.free_reward_flag = m_activity_info.ra_buy_one_get_one_free_fetch_reward_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&msg, sizeof(msg));
}

void RoleActivity::RABuyOneGetOneFreeBuy(int item_index)
{
	if (item_index < 0 || item_index >= RAND_ACTIVITY_BUY_ONE_GET_ONE_FREE_ITEM_MAX_COUNT)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (0 != (m_activity_info.ra_buy_one_get_one_free_buy_flag & (LL_ONE_BIT << item_index)))
	{
		gstr::SendError(m_role, "ra_buy_one_get_one_free_has_buy");
		return;
	}

	int opengame_day = LOGIC_CONFIG->GetRandActivityCfg().GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE);
	const BuyOneGetOneFreeItemCfg* cfg = LOGIC_CONFIG->GetRandActivityCfg().GetBuyOneGetOneFreeCfg(opengame_day, item_index);
	if (NULL == cfg)
	{
		return;
	}

	if (!knapsack->GetMoney()->UseGold(cfg->price_gold, "RABuyOneGetOneFreeBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	knapsack->Put(cfg->buy_item, PUT_RA_BUY_ONE_GET_ONE_FREE_REWARD);

	m_activity_info.ra_buy_one_get_one_free_buy_flag |= (LL_ONE_BIT << item_index);
	this->SendRABuyOneGetOneFreeInfo();

	gamelog::g_log_rand_activity.printf(LL_INFO, "BuyOneGetOneFree user %d %s, price_gold %d", m_role->GetUID(), m_role->GetName(), cfg->price_gold);
}

void RoleActivity::RABuyOneGetOneFreeFetchReward(int item_index)
{
	if (item_index < 0 || item_index >= RAND_ACTIVITY_BUY_ONE_GET_ONE_FREE_ITEM_MAX_COUNT)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (0 == (m_activity_info.ra_buy_one_get_one_free_buy_flag & (LL_ONE_BIT << item_index)))
	{
		gstr::SendError(m_role, "ra_buy_one_get_one_free_can_not_fetch");
		return;
	}

	if (0 != (m_activity_info.ra_buy_one_get_one_free_fetch_reward_flag & (LL_ONE_BIT << item_index)))
	{
		m_role->NoticeNum(errornum::EN_REWARD_HAS_FETCH);
		return;
	}

	int opengame_day = LOGIC_CONFIG->GetRandActivityCfg().GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE);
	const BuyOneGetOneFreeItemCfg* cfg = LOGIC_CONFIG->GetRandActivityCfg().GetBuyOneGetOneFreeCfg(opengame_day, item_index);
	if (NULL == cfg)
	{
		return;
	}

	knapsack->Put(cfg->free_reward_item, PUT_RA_BUY_ONE_GET_ONE_FREE_REWARD);

	m_activity_info.ra_buy_one_get_one_free_fetch_reward_flag |= (LL_ONE_BIT << item_index);
	this->SendRABuyOneGetOneFreeInfo();
}

void RoleActivity::OnHolidayGuardAddKillMonsterCount(int kill_count)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_HOLIDAY_GUARD))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_HOLIDAY_GUARD);

	m_activity_info.ra_holiday_guard_kill_monster_count += kill_count;
}

// 累计充值5
void RoleActivity::RACheckTotalCharge5RewardCheck()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE))
	{
		return;
	}

	int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE), EngineAdapter::Instance().Time());
	int count = LOGIC_CONFIG->GetRandActivityCfg().GetTotalCharge5CfgCount(day_index);

	for (int i = 0; i < count; ++i)
	{
		const RandActivityTotalCharge5Cfg::ConfigItem *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTotalCharge5Cfg(day_index, i);
		if (NULL == cfg)
		{
			continue;
		}

		if (m_activity_info.ra_total_charge_5_cur_total_charge < cfg->need_chongzhi_num)
		{
			continue;
		}

		if (0 != (m_activity_info.ra_total_charge_5_cur_has_fetch_flag & (1 << cfg->seq)))
		{
			continue;
		}

		static MailContentParam contentparam; contentparam.Reset();

		for (int reward_index = 0; reward_index < MAX_ATTACHMENT_ITEM_NUM; ++reward_index)
		{
			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg->reward_item[reward_index].item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[reward_index].item_id = cfg->reward_item[reward_index].item_id;
				contentparam.item_list[reward_index].num = cfg->reward_item[reward_index].num;
				contentparam.item_list[reward_index].is_bind = (cfg->reward_item[reward_index].is_bind) ? 1 : 0;
				contentparam.item_list[reward_index].invalid_time = reward_item_base->CalInvalidTime();
			}
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_charge5_reward_content, cfg->need_chongzhi_num);
		if (length > 0)
		{
			// 消耗
			m_activity_info.ra_total_charge_5_cur_has_fetch_flag |= (1 << cfg->seq);

			// 给予
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::RACheckTotalCharge5RewardCheck user[%d, %s] chongzhi_num[%d], reward_flag[%d], reward_item_1[%s] reward_item_2[%s] reward_item_3[%s]",
				m_role->GetUID(), m_role->GetName(), cfg->need_chongzhi_num, m_activity_info.ra_total_charge_5_cur_has_fetch_flag, ItemConfigDataLog(&cfg->reward_item[0], NULL),
				ItemConfigDataLog(&cfg->reward_item[1], NULL), ItemConfigDataLog(&cfg->reward_item[2], NULL));
		}
	}
}

void RoleActivity::OnRATotalCharge5CheckChongzhi(int chongzhi)
{
	if (chongzhi <= 0)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE);

	m_activity_info.ra_total_charge_5_cur_total_charge += chongzhi;

	this->RACheckTotalCharge5RewardCheck();

	this->OnSendTotalCharge5Info();
}

void RoleActivity::OnSendTotalCharge5Info()
{
	static Protocol::SCRATotalChargeFiveInfo tci;
	tci.cur_total_charge = m_activity_info.ra_total_charge_5_cur_total_charge;
	tci.cur_total_charge_has_fetch_flag = m_activity_info.ra_total_charge_5_cur_has_fetch_flag;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&tci, sizeof(tci));
}


void RoleActivity::OnRAExtremeChallengeReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE);

	switch (opera_type)
	{
	case Protocol::EXTREMECHALLENGE_INFO:
	{
		this->SendRAExtremeChallengeTaskInfo();
	}
	break;

	case Protocol::EXTREMECHALLENGE_REFRESH_TASK:
	{
		this->OnRAExtremeChallengeRefreshTask(param1);
	}
	break;

	case Protocol::EXTREMECHALLENGE_FETCH_REWARD:
	{
		this->OnRAExtremeChallengeFetchReward(param1);
	}
	break;

	case Protocol::EXTREMECHALLENGE_INIT_TASK:
	{
		this->OnRAExtremeChallengeRefreshTask();
	}
	break;

	case Protocol::EXTREMECHALLENGE_FETCH_ULTIMATE_REWARD:
	{
		this->OnRAExtremeChallengeFetchUltimateReward();
	}
	break;

	default:
		break;
	}
}

void RoleActivity::OnRAExtremeChallengeRefreshTask(int task_id)
{
	int seq = 0;
	for (; seq < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM; ++seq)
	{
		if (m_activity_info.ra_extreme_challenge_task_info_list[seq].task_id == task_id)
		{
			break;
		}
	}

	if (m_activity_info.ra_extreme_challenge_task_info_list[seq].task_id != task_id)
	{
		return;
	}

	if (m_activity_info.ra_extreme_challenge_task_info_list[seq].is_finish)
	{
		return;
	}

	const int need_consume_gold = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengeRefreshNeedGold();
	const int max_task_num = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengeMaxTaskNum();

	int tmp_var = 0;
	for (int i = 0; i < max_task_num && i < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM; ++i)
	{
		if (m_activity_info.ra_extreme_challenge_task_info_list[i].is_finish)
		{
			if (++tmp_var >= max_task_num)
			{
				return;
			}
		}
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_consume_gold, "OnRAExtremeChallengeRefreshTask"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int tmp_task_type = -1;
	int tmp_task_id = 0;

	for (int i = 0; i < 50; ++i)
	{
		bool is_took = false;
		tmp_task_type = RandomNum(RA_EXTREME_CHALLENGE_MAX_NUM);

		tmp_task_id = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengeRandTaskByType(tmp_task_type);

		if (tmp_task_id <= 0)
		{
			continue;
		}

		for (int j = 0; j < max_task_num && j < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM; ++j)
		{
			if (m_activity_info.ra_extreme_challenge_task_info_list[j].task_id == tmp_task_id)
			{
				is_took = true;
				break;
			}
		}

		if (is_took)
		{
			continue;
		}

		if (tmp_task_id != m_activity_info.ra_extreme_challenge_task_info_list[seq].task_id)
		{
			m_activity_info.ra_extreme_challenge_task_info_list[seq].Reset();
			m_activity_info.ra_extreme_challenge_task_info_list[seq].task_id = tmp_task_id;
			m_activity_info.ra_extreme_challenge_task_info_list[seq].task_type = tmp_task_type;
			break;
		}
	}

	this->SendRAExtremeChallengeTaskInfo();
}

void RoleActivity::OnRAExtremeChallengeRefreshTask()
{
	const int max_task_num = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengeMaxTaskNum();

	for (int i = 0; i < max_task_num && i < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM; ++i)
	{
		if (m_activity_info.ra_extreme_challenge_task_info_list[i].task_id != 0)
		{
			return;
		}
	}

	int task_type = -1;
	int task_id = 0;
	int task_index = 0;

	for (int i = 0; i < 100; ++i)
	{
		bool is_took = false;
		task_type = RandomNum(RA_EXTREME_CHALLENGE_MAX_NUM);

		for (int j = 0; j < max_task_num && j < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM; ++j)
		{
			if (m_activity_info.ra_extreme_challenge_task_info_list[j].task_type == task_type)
			{
				is_took = true;
			}
		}

		if (is_took)
		{
			continue;
		}

		task_id = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengeRandTaskByType(task_type);

		if (task_id <= 0)
		{
			continue;
		}

		for (int j = 0; j < max_task_num && j < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM; ++j)
		{
			if (m_activity_info.ra_extreme_challenge_task_info_list[j].task_id == task_id)
			{
				is_took = true;
			}
		}

		if (is_took)
		{
			continue;
		}

		if (task_index >= 0 && task_index < max_task_num && task_index < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM)
		{
			m_activity_info.ra_extreme_challenge_task_info_list[task_index].task_id = task_id;
			m_activity_info.ra_extreme_challenge_task_info_list[task_index].task_type = task_type;
			++task_index;

			if (task_index >= max_task_num)
			{
				break;
			}
		}
	}

	this->SendRAExtremeChallengeTaskInfo();
}

void RoleActivity::OnRAExtremeChallengeFetchReward(int task_id)
{
	for (int i = 0; i < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM; ++i)
	{
		if (m_activity_info.ra_extreme_challenge_task_info_list[i].task_id != task_id)
		{
			continue;
		}

		if (!m_activity_info.ra_extreme_challenge_task_info_list[i].is_finish || m_activity_info.ra_extreme_challenge_task_info_list[i].is_already_fetch)
		{
			break;
		}

		const RandActivityExtremeChallengeCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengePerformTaskInfoCfg(m_activity_info.ra_extreme_challenge_task_info_list[i].task_id);
		if (nullptr != reward_cfg)
		{
			for (int j = 0; j < MAX_ATTACHMENT_ITEM_NUM; ++j)
			{
				m_activity_info.ra_extreme_challenge_task_info_list[i].is_already_fetch = 1;
				m_role->GetKnapsack()->PutOrMail(reward_cfg->reward_list[j], PUT_REASON_EXTREME_CHALLENGE_REWARD);

				gamelog::g_log_roleactivity.printf(LL_INFO, "OnRAExtremeChallengeFetchReward::user[%d %s], reward[%s]",
					m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&reward_cfg->reward_list[j], nullptr));
			}

			break;
		}
	}

	this->SendRAExtremeChallengeTaskInfo();
}

void RoleActivity::OnRAExtremeChallengeFetchUltimateReward()
{
	if (m_activity_info.ra_extreme_challenge_ultimate_have_fetch != 0)
	{
		return;
	}

	const int max_task_num = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengeMaxTaskNum();

	for (int i = 0; i < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM && i < max_task_num; ++i)
	{
		if (m_activity_info.ra_extreme_challenge_task_info_list[i].is_finish == 0)
		{
			return;
		}
	}

	m_activity_info.ra_extreme_challenge_ultimate_have_fetch = 1;

	const ItemConfigData *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengeFinishAllTaskRewardCfg();
	if (nullptr != reward_cfg)
	{
		for (int j = 0; j < MAX_ATTACHMENT_ITEM_NUM; ++j)
		{
			m_role->GetKnapsack()->PutOrMail(reward_cfg[j], PUT_REASON_EXTREME_CHALLENGE_REWARD);

			gamelog::g_log_roleactivity.printf(LL_INFO, "OnRAExtremeChallengeFetchUltimateReward::user[%d, %s], reward[%d, %d]",
				m_role->GetUID(), m_role->GetName(), reward_cfg->item_id, reward_cfg->num);
		}
	}

	this->SendRAExtremeChallengeTaskInfo();
}

void RoleActivity::SendRAExtremeChallengeTaskInfo()
{
	static Protocol::SCRAExtremeChallengeTaskInfo scraecti;

	memcpy(scraecti.task_state, m_activity_info.ra_extreme_challenge_task_info_list, sizeof(scraecti.task_state));

	scraecti.is_have_fetch_ultimate_reward = m_activity_info.ra_extreme_challenge_ultimate_have_fetch;

	SEND_TO_ROLE(m_role, scraecti);
}

void RoleActivity::OnRAExtremeChallengeAddPlan(int opera_type, int param1, int param2)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE);

	const int max_task_num = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengeMaxTaskNum();

	for (int i = 0; i < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM && i < max_task_num; ++i)
	{
		if (m_activity_info.ra_extreme_challenge_task_info_list[i].is_finish != 0)
		{
			continue;
		}

		if (m_activity_info.ra_extreme_challenge_task_info_list[i].task_type == opera_type)
		{
			const RandActivityExtremeChallengeCfg *task_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengePerformTaskInfoCfg(m_activity_info.ra_extreme_challenge_task_info_list[i].task_id);
			if (nullptr != task_cfg)
			{
				if (opera_type == RA_EXTREME_CHALLENGE_COMPOUND && param2 != task_cfg->param2)
				{
					return;
				}

				m_activity_info.ra_extreme_challenge_task_info_list[i].task_plan += param1;

				if (m_activity_info.ra_extreme_challenge_task_info_list[i].task_plan >= static_cast<unsigned int>(task_cfg->param1))
				{
					m_activity_info.ra_extreme_challenge_task_info_list[i].is_finish = 1;
				}

				this->SendRAExtremeChallengeTaskInfo();

				break;
			}
		}
	}
}

void RoleActivity::OnRAChongZhiGiftOperaReq(int opera_type, int param_1, int param_2)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT);

	switch (opera_type)
	{
	case Protocol::CHONGZHI_GIFT_OPER_TYPE_INFO:
	{
		this->SendRAChongZhiGiftAllInfo();
	}
	break;

	case Protocol::CHONGZHI_GIFT_OPER_TYPE_FETCH:
	{
		this->OnRAChongZhiGiftFetchOpera(param_1);
	}
	break;

	default:
		break;
	}
}

void RoleActivity::SendRAChongZhiGiftAllInfo()
{
	static Protocol::SCRAChongZhiGiftInfo czgi;

	czgi.magic_shop_fetch_reward_flag = m_activity_info.ra_magic_shop_fetch_reward_flag;
	czgi.magic_shop_chongzhi_value = m_activity_info.ra_magic_shop_chongzhi_value;
	czgi.activity_day = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT), EngineAdapter::Instance().Time()) + 1;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&czgi, sizeof(czgi));
}

void RoleActivity::OnRAChongZhiGiftFetchOpera(int index)
{
	if (index < 0 || index >= RA_CHONGZHI_GIFT_ITEM_MAX_INDEX) return;

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const RandActivityChongZhiGiftConfig *chongzhi_gift_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetChongZhiGiftByIndex(index);
	if (NULL == chongzhi_gift_cfg) return;

	if (0 != (m_activity_info.ra_magic_shop_fetch_reward_flag & (1 << index)))
	{
		m_role->NoticeNum(errornum::EN_CHARGE_REPAYMENT_HAS_FETCHED);
		return;
	}

	if (m_activity_info.ra_magic_shop_chongzhi_value < (unsigned int)chongzhi_gift_cfg->need_gold)
	{
		m_role->NoticeNum(errornum::EN_CHONGZHI_TOTAL_RECHARGE_LIMIT);
		return;
	}

	m_activity_info.ra_magic_shop_fetch_reward_flag |= (1 << index);

	m_role->GetKnapsack()->Put(chongzhi_gift_cfg->reward_item, PUT_REASON_MAGIC_SHOP_FETCH);

	{
		// 日志
		gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAChongZhiGiftFetchOpera::user[%d, %s], index[%d], reward_flag[%d], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), index, m_activity_info.ra_magic_shop_fetch_reward_flag,
			ItemConfigDataLog(&chongzhi_gift_cfg->reward_item, NULL));

		ROLE_LOG_QUICK6(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT, m_role, index, m_activity_info.ra_magic_shop_fetch_reward_flag, NULL, NULL);
	}

	this->SendRAChongZhiGiftAllInfo();
}

void RoleActivity::OnRAChongZhiGiftCheckChongzhi(int chongzhi)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT);

	m_activity_info.ra_magic_shop_chongzhi_value += (int)chongzhi;
	m_activity_info.ra_chongzhi_gift_join_act_day = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT), EngineAdapter::Instance().Time()) + 1;
	this->SendRAChongZhiGiftAllInfo();
}

// 表白排行榜活动

void RoleActivity::OnRAProfessAddScore(int add_score, bool is_initiative)
{
	// 检测
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PROFESS_RANK))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_PROFESS_RANK);


	// 记录相关活动数据
	m_activity_info.ra_profess_score += add_score;
	m_activity_info.ra_profess_rank_last_profess_timestamp = static_cast<int>(EngineAdapter::Instance().Time());

	if (is_initiative)
	{
		m_activity_info.ra_profess_to_num++;
	}
	else
	{
		m_activity_info.ra_profess_from_num++;
	}

	this->SendRAProfessRankInfo();

	gamelog::g_log_rand_activity.printf(LL_INFO, "OnRAProfessAddScore::user[%d, %s], ra_profess_to_num[%d], ra_profess_from_num[%d]",
		m_role->GetUID(), m_role->GetName(), m_activity_info.ra_profess_to_num, m_activity_info.ra_profess_from_num);
}

void RoleActivity::SendRAProfessRankInfo()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PROFESS_RANK))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_PROFESS_RANK);

	Protocol::SCRAProfessRankInfo rpri;

	int rank_type = (FEMALE == m_role->GetSex() ? PERSON_RANK_TYPE_RA_PROFESS_FEMALE : PERSON_RANK_TYPE_RA_PROFESS_MALE);
	rpri.rank = RankManager::Instance().GetPersonRank()->GetUserRank(m_role->GetUserId(), rank_type);

	rpri.ra_profess_score = m_activity_info.ra_profess_score;
	rpri.ra_profess_from_num = m_activity_info.ra_profess_from_num;
	rpri.ra_profess_to_num = m_activity_info.ra_profess_to_num;

	SEND_TO_ROLE(m_role, rpri);
}

void RoleActivity::OnRAJinJieReturn2Opera(int param1, int param2)
{
	switch (param1)
	{
	case Protocol::RA_JINJIE_RETURN2_OPERA_TYPE_INFO:
	{
		this->SendRAJinJieReturn2Info();
	}
	break;

	case Protocol::RA_JINJIE_RETURN2_OPERA_TYPE_FETCH:
	{
		this->OnRAJinJieReturn2Reward(param2);
	}
	break;

	default:
		break;
	}
}

void RoleActivity::SendRAJinJieReturn2Info()
{
	static Protocol::SCRAJinJieReturnInfo2 jjri2;
	jjri2.act_type = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2);
	jjri2.fetch_reward_flag = m_activity_info.ra_jinjie_return2_fetch_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&jjri2, sizeof(jjri2));
}

void RoleActivity::OnRAJinJieReturn2Reward(int param)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2))
	{
		m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2);

	int theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2);
	const JinJieReturnConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieReturn2Cfg(theme, param);
	if (cfg == nullptr) return;

	int grade = this->JinJieActGetGradeByTheme(theme);
	if (grade < cfg->need_grade)
	{
		m_role->NoticeNum(errornum::EN_FETCH_CONDITION_NOENOUGH);
		return;
	}

	if (0 != (m_activity_info.ra_jinjie_return2_fetch_flag & (1 << param)))
	{
		m_role->NoticeNum(errornum::EN_CHARGE_REPAYMENT_HAS_FETCHED);
		return;
	}

	ItemDataWrapper reward_item;
	reward_item.item_id = cfg->reward_item.item_id;
	reward_item.is_bind = cfg->reward_item.is_bind;
	reward_item.num = cfg->reward_item.num;

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->Put(reward_item, PUT_REASON_JINJIE_RETURN_REWARD))
	{
		return;
	}

	m_activity_info.ra_jinjie_return2_fetch_flag |= (1 << param);
	this->SendRAJinJieReturn2Info();
}

void RoleActivity::CheckRAJinJieOldAllReward2(int theme, int old_grade)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2);

	int re_theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2);
	if (re_theme != theme)
	{
		return;
	}

	int grade = this->JinJieActGetGradeByTheme(theme);
	if (old_grade >= grade)
	{
		return;
	}

	int old_fetch_flag = m_activity_info.ra_jinjie_return2_fetch_flag;

	MailContentParam contentparam; contentparam.Reset();
	for (int i = 0; i < RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG; ++i)
	{
		const JinJieReturnConfig *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieReturn2Cfg(theme, i);
		if (cfg == nullptr) continue;

		if (old_grade <= cfg->need_grade) break;

		if (0 != (m_activity_info.ra_jinjie_return2_fetch_flag & (1 << i))) continue;

		if (contentparam.item_list[0].item_id != 0 && contentparam.item_list[0].item_id != cfg->reward_item.item_id) continue;

		contentparam.item_list[0].item_id = cfg->reward_item.item_id;
		contentparam.item_list[0].is_bind = cfg->reward_item.is_bind;
		contentparam.item_list[0].num += cfg->reward_item.num;

		m_activity_info.ra_jinjie_return2_fetch_flag |= (1 << i);
	}

	if (contentparam.item_list[0].item_id <= 0 || contentparam.item_list[0].num <= 0)  return;

	const ItemBase *item_base = ITEMPOOL->GetItem(contentparam.item_list[0].item_id);
	if (NULL != item_base)
	{
		contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();
	}
	else
	{
		return;
	}


	int lenth = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_uplevel_return_reward_mail, theme, old_grade, theme);

	if (lenth > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
	}

	m_role->NoticeNum(noticenum::NT_UPLEVEL_RETURN_RWARD_MAIL);

	gamelog::g_log_roleactivity.printf(LL_INFO, "CheckRAJinJieOldAllReward2 user[%d], theme[%d], old_grade[%d],old_reward_flag[%d],reward_flag[%d]",
		m_role->GetUID(), theme, old_grade, old_fetch_flag, m_activity_info.ra_jinjie_return2_fetch_flag);
}

void RoleActivity::OnRACriticalStrike2OperaReq()
{
	static Protocol::SCRACriticalStrike2Info csi2;
	csi2.act_type = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY_2);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&csi2, sizeof(csi2));
}

void RoleActivity::OnRACombineBuyOpera(int opera_type, int param_0)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_COMBINE_BUY))
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::RA_COMBINE_BUY_OPERA_TYPE_INFO:
	{
		this->SendRACombineBuyItemInfo();
		this->SendRACombineBuyBucketInfo();
	}
	break;

	case Protocol::RA_COMBINE_BUY_OPERA_TYPE_ADD_IN_BUCKET:
	{
		this->OnRACombineBuyAddInBucket(param_0);
	}
	break;

	case Protocol::RA_COMBINE_BUY_OPERA_TYPE_REMOVE_BUCKET:
	{
		this->OnRACombineBuyRemoveBucket(param_0);
	}
	break;

	case Protocol::RA_COMBINE_BUY_OPERA_TYPE_BUY:
	{
		this->OnRACombineBuyItem();
	}
	break;

	default:
		break;
	}
}

void RoleActivity::OnRACombineBuyAddInBucket(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_COMBINE_BUY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_COMBINE_BUY);

	if (seq < 0 || seq >= RA_COMBINE_BUY_MAX_ITEM_COUNT)
	{
		return;
	}

	const RandActivityCombineBuyItemCfg::ItemCfg *item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetCombineBuyItemCfg(seq);
	if (nullptr == item_cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	if (0 != item_cfg->buy_limit)
	{
		int item_count = 1;
		for (int i = 0; i < RA_COMBINE_BUY_BUCKET_ITEM_COUNT; ++i)
		{
			if (m_activity_info.ra_combine_buy_bucket_item_seq_list[i] == seq)
			{
				++item_count;
			}
		}

		const int buy_num = static_cast<int>(m_activity_info.ra_combine_buy_item_buy_times_list[seq]) + item_count;
		if (buy_num > item_cfg->buy_limit || buy_num > RA_COMBINE_BUY_LIMIT_BUY_ITEM_COUNT)
		{
			m_role->NoticeNum(errornum::EN_RA_COMBINE_BUY_ITEM_TOO_MUCH);
			return;
		}
	}

	for (int i = 0; i < RA_COMBINE_BUY_BUCKET_ITEM_COUNT; ++i)
	{
		if (m_activity_info.ra_combine_buy_bucket_item_seq_list[i] < 0)
		{
			m_activity_info.ra_combine_buy_bucket_item_seq_list[i] = seq;
			break;
		}
	}

	this->SendRACombineBuyBucketInfo();
}

void RoleActivity::OnRACombineBuyRemoveBucket(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_COMBINE_BUY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_COMBINE_BUY);

	if (seq < 0 || seq >= RA_COMBINE_BUY_BUCKET_ITEM_COUNT)
	{
		return;
	}

	if (m_activity_info.ra_combine_buy_bucket_item_seq_list[seq] < 0)
	{
		return;
	}

	m_activity_info.ra_combine_buy_bucket_item_seq_list[seq] = -1;

	this->SendRACombineBuyBucketInfo();
}

void RoleActivity::OnRACombineBuyItem()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_COMBINE_BUY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_COMBINE_BUY);

	// 计算购买数量
	int cost = 0;
	int buy_item_count = 0;
	std::map<int, int> seq_to_itemcount_map;

	for (int i = 0; i < RA_COMBINE_BUY_BUCKET_ITEM_COUNT; ++i)
	{
		char seq = m_activity_info.ra_combine_buy_bucket_item_seq_list[i];
		if (seq >= 0)
		{
			const RandActivityCombineBuyItemCfg::ItemCfg *item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetCombineBuyItemCfg(seq);
			if (nullptr != item_cfg)
			{
				int &item_count = seq_to_itemcount_map[seq];
				++item_count;

				cost += item_cfg->price;
				++buy_item_count;
			}
		}
	}

	// 购物车是空的
	if (cost <= 0 || buy_item_count <= 0)
	{
		m_role->NoticeNum(errornum::EN_RA_COMBINE_BUY_NO_ITEM);
		return;
	}

	// 判断购买数量, 构造购买物品列表
	int put_list_count = 0;
	ItemConfigData put_list[RA_COMBINE_BUY_BUCKET_ITEM_COUNT];
	bool is_limit_buy = true;	// 配置为0时，不限制购买数量
	for (const auto &pair : seq_to_itemcount_map)
	{
		if (pair.first >= 0 && pair.first < RA_COMBINE_BUY_MAX_ITEM_COUNT)
		{
			const RandActivityCombineBuyItemCfg::ItemCfg *item_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetCombineBuyItemCfg(pair.first);
			if (nullptr != item_cfg)
			{
				is_limit_buy = item_cfg->buy_limit > 0;
				if (is_limit_buy)
				{
					int tmp_buy_count = static_cast<int>(m_activity_info.ra_combine_buy_item_buy_times_list[pair.first]) + pair.second;
					if (tmp_buy_count > item_cfg->buy_limit || tmp_buy_count > RA_COMBINE_BUY_LIMIT_BUY_ITEM_COUNT)
					{
						m_role->NoticeNum(errornum::EN_RA_COMBINE_BUY_ITEM_TOO_MUCH);
						return;
					}
				}

				// 可能会超出物品堆叠上限，交给后面put去处理
				put_list[put_list_count] = item_cfg->item;
				put_list[put_list_count].num *= pair.second;
				++put_list_count;
			}
		}
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(buy_item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 计算消耗
	int discount = LOGIC_CONFIG->GetRandActivityCfg().GetCombineBuyDiscount(buy_item_count);
	if (0 == discount)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	// 元宝折后向上取整
	float tmp_cost = discount / 100.0f * cost;
	cost = static_cast<int>(tmp_cost);
	if (tmp_cost - static_cast<float>(cost) > 0.01f)
	{
		cost += 1;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cost, "RoleActivity::OnRACombineBuyItem"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (put_list_count > 0)
	{
		// 发道具
		m_role->GetKnapsack()->PutList(put_list_count, put_list, PUT_REASON_RA_COMBINE_BUY);

		// 清空购物车
		memset(m_activity_info.ra_combine_buy_bucket_item_seq_list, -1, sizeof(m_activity_info.ra_combine_buy_bucket_item_seq_list));

		for (const auto &pair : seq_to_itemcount_map)
		{
			if (is_limit_buy && pair.first >= 0 && pair.first < RA_COMBINE_BUY_MAX_ITEM_COUNT)
			{
				m_activity_info.ra_combine_buy_item_buy_times_list[pair.first] += static_cast<unsigned char>(pair.second);
			}
		}

		this->SendRACombineBuyItemInfo();
		this->SendRACombineBuyBucketInfo();
		m_role->NoticeNum(noticenum::NT_SHOP_BUY_SUCC);
	}

	// 日志
	std::string item_list_str;
	ItemConfigDataListLog(item_list_str, buy_item_count, put_list);

	gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnRACombineBuyItem user[%d, %s], use_gold[%d], item_list[%s]",
		m_role->GetUID(), m_role->GetName(), cost, item_list_str.c_str());
}

void RoleActivity::SendRACombineBuyBucketInfo()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_COMBINE_BUY);

	static Protocol::SCRACombineBuyBucketInfo pro;
	for (int i = 0; i < RA_COMBINE_BUY_BUCKET_ITEM_COUNT; ++i)
	{
		pro.seq_list[i] = static_cast<int>(m_activity_info.ra_combine_buy_bucket_item_seq_list[i]);
	}

	SEND_TO_ROLE(m_role, pro);
}

void RoleActivity::SendRACombineBuyItemInfo()
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_COMBINE_BUY);

	static Protocol::SCRACombineBuyItemInfo pro;
	memcpy(pro.buy_item_num, m_activity_info.ra_combine_buy_item_buy_times_list, sizeof(pro.buy_item_num));

	SEND_TO_ROLE(m_role, pro);
}

void RoleActivity::OnDayChongzhiRewardAddChongzhi()
{
	const ChongzhiDayReward *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDayChongzhiRewardCfgByChongzhiDay(m_activity_info.total_chongzhi_day_count);
	if (nullptr != cfg)
	{
		if (0 == m_activity_info.had_add_chongzhi_day_count && m_activity_info.day_chongzhi >= cfg->need_gold)
		{
			++m_activity_info.total_chongzhi_day_count;
			m_activity_info.had_add_chongzhi_day_count = 1;
			this->SendDayChongzhiRewardInfo();
		}
	}
}

void RoleActivity::OnDayChongzhiRewardOpera(short opera_type, short param_0)
{
	switch (opera_type)
	{
	case Protocol::DAY_CHONGZHI_REWARD_OPERA_TYPE_QUERY_INFO:
	{
		this->SendDayChongzhiRewardInfo();
	}
	break;

	case Protocol::DAY_CHONGZHI_REWARD_OPERA_TYPE_FETCH_REWARD:
	{
		this->OnFetchDayChongzhiReward(param_0);
	}
	break;

	case Protocol::DAY_CHONGZHI_REWARD_OPERA_TYPE_FETCH_RARE_REWARD:
	{
		this->OnFetchDayChongzhiRareReward(param_0);
	}
	break;

	default:
		break;
	}
}

void RoleActivity::OnFetchDayChongzhiReward(int seq)
{
	const int list_index = seq / 32;			//int 有32个bit
	const int check_seq = seq % 32;

	if (list_index < 0 || list_index >= DAY_CHONGZHI_REWARD_FLAG_LIST_LEN)
	{
		return;
	}

	unsigned int &fetch_flag = m_activity_info.chongzhi_day_reward_fetch_flag_list[list_index];
	if (0 != (fetch_flag & (1 << check_seq)))
	{
		m_role->NoticeNum(errornum::EN_DAY_CHONGZHI_REWARD_HAD_FETCH_REWARD);
		return;
	}

	const ChongzhiDayReward *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDayChongzhiRewardCfg(seq);
	if (nullptr == cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	if (m_activity_info.total_chongzhi_day_count < cfg->need_chongzhi_day)
	{
		m_role->NoticeNum(errornum::EN_DAY_CHONGZHI_REWARD_DAY_COUNT_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (cfg->reward_bind_gold > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg->reward_bind_gold, "RoleActivity::OnFetchDayChongzhiReward");
	}

	if (cfg->reward_gold > 0)
	{
		m_role->GetKnapsack()->GetMoney()->AddGold(cfg->reward_gold, "RoleActivity::OnFetchDayChongzhiReward");
	}

	if (cfg->item_count > 0)
	{
		m_role->GetKnapsack()->PutList(cfg->item_count, cfg->reward_item_list, PUT_REASON_DAY_CHONGZHI_REWARD);

		fetch_flag |= 1 << check_seq;
		this->SendDayChongzhiRewardInfo();
	}

	std::string reward_item_list_str;
	ItemConfigDataListLog(reward_item_list_str, cfg->item_count, cfg->reward_item_list);

	gamelog::g_log_other.printf(LL_INFO, "RoleActivity::FetchDailyTotalChongzhiReward user[%d, %s], seq[%d], fetch_flag[%d], total_chongzhi_day_count %d, item_list[%s]",
		m_role->GetUID(), m_role->GetName(), seq, fetch_flag, m_activity_info.total_chongzhi_day_count, reward_item_list_str.c_str());
}

void RoleActivity::OnFetchDayChongzhiRareReward(int seq)
{
	const int list_index = seq / 32;			//int 有32个bit
	const int check_seq = seq % 32;

	if (list_index < 0 || list_index >= DAY_CHONGZHI_REWARD_FLAG_LIST_LEN)
	{
		return;
	}

	unsigned int &fetch_flag = m_activity_info.chongzhi_day_rare_reward_fetch_flag_list[list_index];
	if (0 != (fetch_flag & (1 << check_seq)))
	{
		m_role->NoticeNum(errornum::EN_DAY_CHONGZHI_REWARD_HAD_FETCH_REWARD);
		return;
	}

	const ChongzhiDayReward *cfg = LOGIC_CONFIG->GetChongzhiCfg().GetDayChongzhiRewardCfg(seq);
	if (nullptr == cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	if (m_activity_info.total_chongzhi_day_count < cfg->need_chongzhi_day)
	{
		m_role->NoticeNum(errornum::EN_DAY_CHONGZHI_REWARD_DAY_COUNT_NOT_ENOUGH);
		return;
	}

	if (cfg->rare_item_count > 0)
	{
		if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg->rare_item_count))
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}

		m_role->GetKnapsack()->PutList(cfg->rare_item_count, cfg->rare_reward_item_list, PUT_REASON_DAY_CHONGZHI_REWARD);

		fetch_flag |= 1 << check_seq;
		this->SendDayChongzhiRewardInfo();

		std::string reward_item_list_str;
		ItemConfigDataListLog(reward_item_list_str, cfg->rare_item_count, cfg->rare_reward_item_list);

		gamelog::g_log_other.printf(LL_INFO, "RoleActivity::OnFetchDayChongzhiRareReward user[%d, %s], seq[%d], fetch_flag[%d], total_chongzhi_day_count %d, item_list[%s]",
			m_role->GetUID(), m_role->GetName(), seq, fetch_flag, m_activity_info.total_chongzhi_day_count, reward_item_list_str.c_str());
	}
}

void RoleActivity::SendDayChongzhiRewardInfo()
{
	static Protocol::SCDayChongzhiRewardInfo pro;
	pro.day_count = m_activity_info.total_chongzhi_day_count;
	memcpy(pro.reward_flag_list, m_activity_info.chongzhi_day_reward_fetch_flag_list, sizeof(pro.reward_flag_list));
	memcpy(pro.rare_reward_flag_list, m_activity_info.chongzhi_day_rare_reward_fetch_flag_list, sizeof(pro.rare_reward_flag_list));

	SEND_TO_ROLE(m_role, pro);
}

void RoleActivity::OnRALuckyWishOpera(int opera_type, int param_1)
{
	switch (opera_type)
	{
	case Protocol::RA_LUCKY_WISH_OPERA_TYPE_ALL_INFO:
	{
		Protocol::SCLuckyWishInfo info;
		info.type = Protocol::LUCKY_WISH_TYPE_ONLY_LUCKY_VALUE;
		info.item_list_count = 0;
		info.lucky_value = this->GetRALuckyWishValue();
		this->SendRALuckyWishInfo(info);
	}
	break;
	case Protocol::RA_LUCKY_WISH_OPERA_TYPE_WISH:
	{
		this->OnRALuckyWishLottery(param_1);
	}
	break;
	default: break;
	}
}

void RoleActivity::SendRALuckyWishInfo(const Protocol::SCLuckyWishInfo &info)
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LUCKY_WISH);

	SEND_TO_ROLE(m_role, info);
}

void RoleActivity::OnRALuckyWishResetLuckyValue()
{
	const RandActivityLuckyWishLuckyCfg *lucky_cfg = LOGIC_CONFIG->Instance()->GetRandActivityCfg().GetLuckyWishLuckyCfg();
	if (nullptr == lucky_cfg)
	{
		return;
	}
	m_activity_info.ra_lucky_wish_lucky_value = 0;
	m_activity_info.ra_lucky_wish_fetch_reward_need_lucky_value = RandomNum(lucky_cfg->lucky_min, lucky_cfg->lucky_max);	//每次达标后重新随下次中奖需要的幸运值
}

void RoleActivity::OnRALuckyWishAddLuckyValue(int value)
{
	m_activity_info.ra_lucky_wish_lucky_value += value;
}

int RoleActivity::GetRALuckyWishValue()
{
	return m_activity_info.ra_lucky_wish_lucky_value;
}

bool RoleActivity::OnRALuckyWishIsLucky()
{
	//没达到幸运值
	if (m_activity_info.ra_lucky_wish_lucky_value < m_activity_info.ra_lucky_wish_fetch_reward_need_lucky_value)
	{
		return false;
	}
	return true;
}

bool RoleActivity::OnRALuckyWishLottery(int repeat_times)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LUCKY_WISH))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return false;
	}
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LUCKY_WISH);

	RandActivityLuckyWish * rand_act = dynamic_cast<RandActivityLuckyWish *>(RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_LUCKY_WISH));
	if (NULL == rand_act)
	{
		return false;
	}

	const RandActivityLuckyWishLuckyCfg *lucky_cfg = LOGIC_CONFIG->Instance()->GetRandActivityCfg().GetLuckyWishLuckyCfg();
	if (nullptr == lucky_cfg)
	{
		return false;
	}
	int consume_gold = 0;
	bool is_use_item = false;

	//许愿30次
	if (repeat_times == LUCKY_WISH_MAX_ITEM_COUNT)
	{
		consume_gold = lucky_cfg->consume_gold_30;

		int consume_item_id = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyWisk30TimesConsumeItem();
		if (m_role->GetKnapsack()->ConsumeItem(consume_item_id, 1, "RoleActivity::OnRALuckyWishLottery"))
		{
			consume_gold = 0;
			is_use_item = true;
		}
	}
	//许愿一次
	else
	{
		repeat_times = 1;	//对客户端传过来的参数作修正
		consume_gold = lucky_cfg->consume_gold_1;
	}
	
	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, repeat_times))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_SPACE);
		return false;
	}

	int total_weight = LOGIC_CONFIG->Instance()->GetRandActivityCfg().GetLuckyWishLotteryTotalWeight();
	const std::vector<RandActivityLuckyWishLotteryCfg> *lottery_vec_ptr = LOGIC_CONFIG->Instance()->GetRandActivityCfg().GetLuckyWishLotteryCfgVec();
	if (0 == total_weight || nullptr == lottery_vec_ptr || 0 == lottery_vec_ptr->size())
	{
		return false;
	}
	Protocol::SCLuckyWishInfo lucky_wish_info;
	lucky_wish_info.type = Protocol::LUCKY_WISH_TYPE_COMMON_ITEM;
	lucky_wish_info.item_list_count = 0;
	short &count = lucky_wish_info.item_list_count;

	ChestItemInfo item_list[LUCKY_WISH_MAX_ITEM_COUNT];
	int item_count = 0;
	for (int i = 0; i < repeat_times; ++i)
	{
		int rand_value = RandomNum(total_weight);
		int now_weight = 0;
		int serve_chou_times = rand_act->GetServerChouTimes();

		for (size_t j = 0; j < lottery_vec_ptr->size(); ++j)
		{
			now_weight += (*lottery_vec_ptr)[j].weight;
			//根据权重随一个奖励
			if (rand_value >= now_weight)
			{
				continue;
			}
			item_list[i].Reset();
			this->OnRALuckyWishAddLuckyValue(lucky_cfg->add_lucky_value);		//增加幸运值
			if (true == this->OnRALuckyWishIsLucky())		//幸运值达标
			{
				this->OnRALuckyWishResetLuckyValue();		//幸运值清零
				item_list[i].item_id = lucky_cfg->item.item_id;
				item_list[i].num = lucky_cfg->item.is_bind;
				item_list[i].is_bind = lucky_cfg->item.num;
				if (count >= 0 && count < LUCKY_WISH_MAX_ITEM_COUNT)
				{
					lucky_wish_info.reward_item[count].item_id = lucky_cfg->item.item_id;
					lucky_wish_info.reward_item[count].num = lucky_cfg->item.num;
					lucky_wish_info.reward_item[count].is_bind = lucky_cfg->item.is_bind;
					count++;
				}
				lucky_wish_info.type = Protocol::LUCKY_WISH_TYPE_LUCKY_ITEM;
			}
			else
			{
				const RandActivityLuckyWishLotteryCfg * baodi_cfg = LOGIC_CONFIG->Instance()->GetRandActivityCfg().GetLuckyWishLotteryBaodiCfg(serve_chou_times);
				if (baodi_cfg != nullptr && (consume_gold > 0 || is_use_item))
				{
					item_list[i].item_id = baodi_cfg->item.item_id;
					item_list[i].num = baodi_cfg->item.num;
					item_list[i].is_bind = baodi_cfg->item.is_bind;
					if (count >= 0 && count < LUCKY_WISH_MAX_ITEM_COUNT)
					{
						lucky_wish_info.reward_item[count].item_id = baodi_cfg->item.item_id;
						lucky_wish_info.reward_item[count].num = baodi_cfg->item.num;
						lucky_wish_info.reward_item[count].is_bind = baodi_cfg->item.is_bind;
						count++;
					}
					rand_act->ClearServerChouTimes();

					LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_LUCKY_WISH, m_role->GetUID(), m_role->GetName(), baodi_cfg->item.item_id, baodi_cfg->item.num);
				}
				else
				{
					item_list[i].item_id = (*lottery_vec_ptr)[j].item.item_id;
					item_list[i].num = (*lottery_vec_ptr)[j].item.num;
					item_list[i].is_bind = (*lottery_vec_ptr)[j].item.is_bind;
					if (count >= 0 && count < LUCKY_WISH_MAX_ITEM_COUNT)
					{
						lucky_wish_info.reward_item[count].item_id = (*lottery_vec_ptr)[j].item.item_id;
						lucky_wish_info.reward_item[count].num = (*lottery_vec_ptr)[j].item.num;
						lucky_wish_info.reward_item[count].is_bind = (*lottery_vec_ptr)[j].item.is_bind;
						count++;
					}
					if ((*lottery_vec_ptr)[j].is_baodi)
					{
						rand_act->ClearServerChouTimes();
					}

					if ((*lottery_vec_ptr)[j].is_rare)
					{
						const ItemBase *itembase = ITEMPOOL->GetItem((*lottery_vec_ptr)[j].item.item_id);
						if (NULL != itembase)
						{
							int sendlen = 0;
							sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chuji_equip_chestshop_item_broadcast,
								m_role->GetUID(), (const char*)m_role->GetName(), (int)m_role->GetCamp(), itembase->GetItemName());
							if (sendlen > 0)
							{
								World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
							}
						}
					}
				}
			}

			if (consume_gold > 0 || is_use_item)
			{
				rand_act->AddServerChouTimes(1);
			}
			++item_count;
			break;
		}//for size_t
	}
	if (item_count > LUCKY_WISH_MAX_ITEM_COUNT)
	{
		item_count = LUCKY_WISH_MAX_ITEM_COUNT;
	}

	if (!is_use_item)
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "RoleActivity::OnRALuckyWishLottery"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return false;
		}
	}

	// 给予
//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, item_count, item_list, "Randactivity OnRALuckyWishLottery");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_LUCKY_WISH, item_count, item_list, "RoleActivity::OnRALuckyWishLottery");

	lucky_wish_info.lucky_value = this->GetRALuckyWishValue();
	this->SendRALuckyWishInfo(lucky_wish_info);
	return false;
}

void RoleActivity::OnLottery1OperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LOTTERY_1))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LOTTERY_1);

	switch (opera_type)
	{
	case Protocol::RA_LOTTERY_1_OPERA_TYPE_INFO:
	{
		this->SendLottery1Info(NULL, 0, 0);
	}
	break;
	case Protocol::RA_LOTTERY_1_OPERA_TYPE_DO_LOTTERY:
	{
		this->OnLottery1ChouReq(param1);
	}
	break;

	case Protocol::RA_LOTTERY_1_OPERA_TYPE_FETCH_PERSON_REWARD:
	{
		this->OnFetchLottery1PersonRewardReq(param1);
	}
	break;

	default: break;
	}
}

void RoleActivity::OnLottery1ChouReq(int chou_type)
{
	if (chou_type <= RA_LOTTERY_1_TYPE_INVALID || chou_type >= RA_LOTTERY_1_TYPE_MAX)
	{
		gstr::SendError(m_role, "illegal_opera");
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	const unsigned int open_timestamp = RandActivityManager::Instance().GetRanadActivityOpenTime(RAND_ACTIVITY_TYPE_LOTTERY_1);
	if (open_timestamp <= 0) return;
	
	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), open_timestamp) + 1;
	const RALottery1RewardCfg *weight_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLottery1RewardWeightCfg(opengame_day);
	if (nullptr == weight_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return;
	}

	RandActivityLottery1 * rand_act = RandActivityManager::Instance().GetRandActivityLottery1();
	if (nullptr == rand_act)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	unsigned int chou_times = (chou_type == RA_LOTTERY_1_TYPE_ONE) ? RA_LOTTERY_1_ONECHOU_COUNT : RA_LOTTERY_1_TEN_CHOU_COUNT;

	if (!m_role->GetRoleChestShop()->CheckHaveSpace(CHEST_SHOP_TYPE_EQUIP, chou_times))
	{
		m_role->NoticeNum(errornum::EN_CHESTSHOP_NOT_ENOUGH_SPACE);
		return;
	}

	RALottery1ChouItem chou_item_list[RA_LOTTERY_1_TEN_CHOU_COUNT];
	memset(chou_item_list, 0, sizeof(chou_item_list));
	ChestItemInfo item_list[RA_LOTTERY_1_TEN_CHOU_COUNT];
	unsigned int item_count = 0;

	bool is_free = false;
	bool has_item = false;
	ItemID consume_item_id = 0;
	int consume_gold = 0;
	const RALottery1OtherCfg &lottery1_other_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLottery1OtherCfg();
	switch (chou_type)
	{
	case RA_LOTTERY_1_TYPE_ONE:
	{
		if (now >= m_activity_info.ra_lottery1_next_free_timestamp)
		{
			is_free = true;
		}

		if (!is_free)
		{
			consume_gold = lottery1_other_cfg.lottery1_consume_gold;
			consume_item_id = lottery1_other_cfg.lottery1_one_consume_item;
			if (m_role->GetKnapsack()->HasItem(consume_item_id))
			{
				has_item = true;
				consume_gold = 0;
			}
		}
	}
	break;

	case RA_LOTTERY_1_TYPE_TEN:
	{
		consume_gold = lottery1_other_cfg.lottery1_ten_consume_gold;
		consume_item_id = lottery1_other_cfg.lottery1_ten_consume_item;
		if (m_role->GetKnapsack()->HasItem(consume_item_id))
		{
			has_item = true;
			consume_gold = 0;
		}
	}
	break;
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int tmp_server_total_times = rand_act->GetServerChouTimes();
	for (unsigned int i = 0; i < chou_times; i ++)
	{
		ItemConfigData reward_item;
//		bool is_record = false;
		bool is_broadcast = false;

		//如果达到全服保底奖励次数
		tmp_server_total_times++;
		const RALottery1ServerRewardItem *server_total_reward = LOGIC_CONFIG->GetRandActivityCfg().GetLottery1ServerRewardCfg(opengame_day, tmp_server_total_times);
		if (nullptr != server_total_reward)
		{
			reward_item.item_id = server_total_reward->reward_item.item_id;
			reward_item.num = server_total_reward->reward_item.num;
			reward_item.is_bind = server_total_reward->reward_item.is_bind;

//			is_record = (1 == server_total_reward->is_record_history);
			is_broadcast = (1 == server_total_reward->is_broadcast);

		}
		else
		{
			const RALottery1RewardWeight *hit_cfg = (chou_times == RA_LOTTERY_1_ONECHOU_COUNT) ? gamecommon::RandListItem(weight_cfg->one_lottery) : gamecommon::RandListItem(weight_cfg->ten_lottery);
			if (nullptr != hit_cfg)
			{
				const RALottery1RewardItem *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLottery1RewardCfg(opengame_day, hit_cfg->seq);
				if (nullptr != reward_cfg)
				{
					reward_item.item_id = reward_cfg->reward_item.item_id;
					reward_item.num = reward_cfg->reward_item.num;
					reward_item.is_bind = reward_cfg->reward_item.is_bind;

//					is_record = (1 == reward_cfg->is_record_history);
					is_broadcast = (1 == reward_cfg->is_broadcast);
				}
			}
		}

		if (reward_item.item_id <= 0)
		{
			continue;
		}

		// 记录
		if (item_count >= 0 && item_count < static_array_size(item_list) && item_count < static_array_size(chou_item_list))
		{
			item_list[item_count].item_id = reward_item.item_id;
			item_list[item_count].num = reward_item.num;
			item_list[item_count].is_bind = reward_item.is_bind ? 1 : 0;

			chou_item_list[item_count].reward_item = reward_item.item_id;
			chou_item_list[item_count].reward_num = reward_item.num;
			chou_item_list[item_count].is_bind = reward_item.is_bind ? 1: 0;

			++item_count;
		}

		// 幸运儿日志
		LuckyLogManager::Instance().AddLog(RAND_ACTIVITY_TYPE_LOTTERY_1, m_role->GetUID(), m_role->GetName(), reward_item.item_id, reward_item.num);

		if (is_broadcast)
		{
			int send_len = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "rand_act_reward_notice",
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), RAND_ACTIVITY_TYPE_LOTTERY_1, reward_item.item_id);
			if (send_len > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, send_len, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	//消耗
	if (is_free)
	{
		int free_interval_s = LOGIC_CONFIG->GetRandActivityCfg().GetLottery1OtherCfg().lottery1_free_interval_s;
		m_activity_info.ra_lottery1_next_free_timestamp = now + free_interval_s;
	}

	if (has_item)
	{
		if (!m_role->GetKnapsack()->ConsumeItem(consume_item_id, 1, "RoleActivity::OnLottery1ChouReq"))
		{
			return;
		}
	}

	if (consume_gold > 0)
	{
		m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "OnLottery1ChouReq");
	}

	// 给予
	m_activity_info.ra_lottery1_person_chou_times += chou_times;
	rand_act->AddServerChouTimes(chou_times);
//	m_role->GetRoleChestShop()->OnBuyItemList(CHEST_SHOP_TYPE_EQUIP, item_count, item_list, "RoleActivity::OnLottery1ChouReq");
	this->OnRALotteryPutChestStore(RAND_ACTIVITY_TYPE_LOTTERY_1, item_count, item_list, "RoleActivity::OnLottery1ChouReq");

	// 随机属性
	for (unsigned int i = 0; i < item_count && i < static_array_size(chou_item_list) && i < static_array_size(item_list); i ++)
	{
		for (unsigned int type_index = 0; type_index < MAX_CHEST_XIANPIN_NUM; type_index ++)
		{
			chou_item_list[i].xianpin_type_list[type_index] = item_list[i].xianpin_type_list[type_index];
		}
	}

	this->SendLottery1Info(chou_item_list, sizeof(chou_item_list), item_count);

	//log
	gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnLottery1ChouReq::user[%d, %s], chou_times[%d], use_gold[%d]",
		m_role->GetUID(), m_role->GetName(), chou_times, consume_gold);
}

void RoleActivity::OnFetchLottery1PersonRewardReq(int seq)
{
	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		m_role->NoticeNum(errornum::EN_HIDDEN_SERVER_OPERA_ERROR);
		return;
	}

	RandActivityLottery1 * rand_act = RandActivityManager::Instance().GetRandActivityLottery1();
	if (nullptr == rand_act)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), now) + 1;
	const RALottery1PersonRewardCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLottery1PersonRewardCfg(opengame_day, seq);
	if (nullptr == reward_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return;
	}

	if (IS_BIT_SET(m_activity_info.ra_lottery1_fetch_person_reward_flag, seq))
	{
		m_role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		return;
	}

	if (m_activity_info.ra_lottery1_person_chou_times < static_cast<unsigned int>(reward_cfg->person_chou_times))
	{
		gstr::SendError(m_role, "person_total_chou_not_enough");
		return;
	}

	if (m_role->GetVip()->GetVipLevel() < reward_cfg->vip_limit)
	{
		m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGH);
		return;
	}

	// 给予
	SET_BIT(m_activity_info.ra_lottery1_fetch_person_reward_flag, seq);

	m_role->GetKnapsack()->PutList(reward_cfg->reward_count, reward_cfg->reward_item_list, PUT_REASON_LOTTERY_1_SERVER_REWARD);

	this->SendLottery1Info(NULL, 0, 0);

	//log
	std::string reward_item_list_str;
	ItemConfigDataListLog(reward_item_list_str, reward_cfg->reward_count, reward_cfg->reward_item_list);

	gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnFetchServerRewardReq::user[%d, %s], ra_lottery1_fetch_server_reward_flag[%d], reward_item_list[%s]",
		m_role->GetUID(), m_role->GetName(), (int)m_activity_info.ra_lottery1_fetch_person_reward_flag, reward_item_list_str.c_str());
}

void RoleActivity::SendLottery1Info(const RALottery1ChouItem *chou_item_list, int size, int count)
{
	static Protocol::SCRALottery1Info msg;
	msg.next_free_time = m_activity_info.ra_lottery1_next_free_timestamp;
	msg.fetch_person_reward_flag = m_activity_info.ra_lottery1_fetch_person_reward_flag;
	msg.person_total_chou_times = m_activity_info.ra_lottery1_person_chou_times;
	msg.server_total_chou_times = 0;
	msg.item_count = count;
	memset(msg.chou_item_list, 0, sizeof(msg.chou_item_list));

	RandActivityLottery1 * rand_act = RandActivityManager::Instance().GetRandActivityLottery1();
	if (nullptr != rand_act)
	{
		msg.server_total_chou_times = rand_act->GetServerChouTimes();
	}

	if (nullptr != chou_item_list && sizeof(msg.chou_item_list) == size)
	{
		memcpy(&msg.chou_item_list, chou_item_list, sizeof(msg.chou_item_list));
	}

	int send_len = sizeof(msg) - (static_array_size(msg.chou_item_list) - msg.item_count) * sizeof(msg.chou_item_list[0]);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, send_len);
}

void RoleActivity::OnRAZeroBuyReturnOperaReq(int opera_type, int param1)
{
	switch (opera_type)
	{
	case Protocol::RA_ZERO_BUY_RETURN_OPERA_TYPE_INFO:
	{
		this->SendRAZeroBuyReturnInfo();
	}
	break;
	case Protocol::RA_ZERO_BUY_RETURN_OPERA_TYPE_BUY:
	{
		this->OnRAZeroBuyReturnOperaBuy(param1);
	}
	break;

	case Protocol::RA_ZERO_BUY_RETURN_OPERA_TYPE_FETCH_YUANBAO:
	{
		this->OnRAZeroBuyReturnOperaFecth(param1);
	}
	break;

	default: break;
	}
}

void RoleActivity::OnRAZeroBuyReturnOperaBuy(int buy_type)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_ZERO_BUY_RETURN))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	if (buy_type < 0 || buy_type >= RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE)
	{
		return;
	}

	const RAZeroBuyReturnConfig *zero_buy_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAZeroBuyReturnConfig(buy_type);
	if (NULL == zero_buy_cfg)
	{
		return;
	}

	if (m_activity_info.ra_zero_buy_return_buy_timestamp[buy_type] > 0)
	{
		m_role->NoticeNum(errornum::EN_ZERO_BUY_RETURN_ALREADY_BUY);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 消耗
	if (zero_buy_cfg->need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(zero_buy_cfg->need_gold, "RoleActivity::OnRAZeroBuyReturnOperaBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_activity_info.ra_zero_buy_return_buy_timestamp[buy_type] = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_role->GetKnapsack()->Put(zero_buy_cfg->buy_reward, PUT_REASON_ZERO_BUY_RETURN_REWARD);

	this->SendRAZeroBuyReturnInfo();
	TianShuXunZhu::CheckShiXueTianShu(m_role, CHENGZHANG_TIANSHU_TYPE_ZERO_BUY_RETURN, zero_buy_cfg->need_gold);
	// log
	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRAZeroBuyReturnOperaBuy user[%d, %s], buy_type[%d], buy_timestamp[%u], consume_gold[%d]",
		m_role->GetUID(), m_role->GetName(), buy_type, m_activity_info.ra_zero_buy_return_buy_timestamp[buy_type], zero_buy_cfg->need_gold);
}

void RoleActivity::OnRAZeroBuyReturnOperaFecth(int buy_type)
{
	if (buy_type < 0 || buy_type >= RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE)
	{
		return;
	}

	const RAZeroBuyReturnConfig *zero_buy_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRAZeroBuyReturnConfig(buy_type);
	if (NULL == zero_buy_cfg)
	{
		return;
	}

	if (m_activity_info.ra_zero_buy_return_buy_timestamp[buy_type] <= 0)
	{
		m_role->NoticeNum(errornum::EN_ZERO_BUY_RETURN_NOT_BUY);
		return;
	}

	// 从购买那天领取到今天（直到最大可领取天数）
	int fecth_day = GetDayIndex(m_activity_info.ra_zero_buy_return_buy_timestamp[buy_type], EngineAdapter::Instance().Time()) + 1;
	if (fecth_day > 0)
	{
		for (int day = 0; day < fecth_day && day < RAND_ACTICITY_ZERO_BUY_RETURN_MAX_DAY; ++ day)  // 最多20天
		{
			if (0 != (m_activity_info.ra_zero_buy_return_day_fetch_flag[buy_type] & (1 << day)))
			{
				continue;
			}

			if (zero_buy_cfg->daily_reward_gold[day] > 0)
			{
				// 增加绑定元宝
				m_role->GetKnapsack()->GetMoney()->AddGoldBind(zero_buy_cfg->daily_reward_gold[day], "RoleActivity::OnRAZeroBuyReturnOperaFecth");
				m_activity_info.ra_zero_buy_return_day_fetch_flag[buy_type] |= (1 << day);
			}
		}
	}

	this->SendRAZeroBuyReturnInfo();

	// log
	gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::OnRAZeroBuyReturnOperaFecth user[%d, %s], buy_type[%d], buy_timestamp[%u], fecth_flag[%d]",
		m_role->GetUID(), m_role->GetName(), buy_type, 
		m_activity_info.ra_zero_buy_return_buy_timestamp[buy_type], m_activity_info.ra_zero_buy_return_day_fetch_flag[buy_type]);
}

void RoleActivity::SendRAZeroBuyReturnInfo()
{
	Protocol::SCRAZeroBuyReturnInfo zbri;

	UNSTD_STATIC_CHECK(sizeof(zbri.zero_buy_return_buy_timestamp) == sizeof(m_activity_info.ra_zero_buy_return_buy_timestamp));
	memcpy(zbri.zero_buy_return_buy_timestamp, m_activity_info.ra_zero_buy_return_buy_timestamp, sizeof(zbri.zero_buy_return_buy_timestamp));

	UNSTD_STATIC_CHECK(sizeof(zbri.zero_buy_return_day_fetch_flag) == sizeof(m_activity_info.ra_zero_buy_return_day_fetch_flag));
	memcpy(zbri.zero_buy_return_day_fetch_flag, m_activity_info.ra_zero_buy_return_day_fetch_flag, sizeof(zbri.zero_buy_return_day_fetch_flag));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&zbri, sizeof(zbri));
}

void RoleActivity::OnSendDailyLove2ChongzhiInfo()
{
	Protocol::SCLoveDaily2Info info;

	info.flag = m_activity_info.ra_daily_love_daily_2_has_charge;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}

// ------------------------------
// 狂嗨庆典
// ------------------------------
void RoleActivity::SendKuangHaiInfo()
{
	int total_task_num = LOGIC_CONFIG->GetRandActivityCfg().GetKuangHaiTaskNum();
	int role_levle = m_role->GetLevel();
	int role_can_join_task_num = 0;

	Protocol::SCRAKuangHaiInfo msg;
	msg.reward_flag = m_activity_info.ra_kuang_hai_qing_dian_fetch_reward_flag;
	msg.current_score = m_activity_info.ra_kuang_hai_qing_dian_total_score;

	for (int i = 0; i < total_task_num && i < RA_KUANG_HAI_TASK_ITEM_MAX_NUM; i++)
	{
		auto cfg_task_ptr = LOGIC_CONFIG->GetRandActivityCfg().GetKuangHaiTaskItem(i);
		if (nullptr == cfg_task_ptr)
		{
			continue;
		}

		if (cfg_task_ptr->index != i)
			continue;

		if (role_levle < cfg_task_ptr->open_level)
			continue;

		auto& msg_task_item = msg.task_item_list[role_can_join_task_num++];
		msg_task_item.item_idx = cfg_task_ptr->index;
		msg_task_item.get_score = m_activity_info.ra_kuang_hai_qing_dian_score_list[cfg_task_ptr->index];
	}

	msg.task_item_num = role_can_join_task_num;

	int send_length_byte = sizeof(msg) - (RA_KUANG_HAI_TASK_ITEM_MAX_NUM - msg.task_item_num) * sizeof(Protocol::SCRAKuangHaiInfo::TaskItem);
	if (send_length_byte < 0 || (unsigned int)send_length_byte > sizeof(msg))
	{
		return;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, send_length_byte);
}
void RoleActivity::KuangHaiGetReward(int reward_list_seq)
{
	int total_reward_list_num = LOGIC_CONFIG->GetRandActivityCfg().GetKuangHaiRewardListNum();

	if (reward_list_seq < 0 || reward_list_seq >= total_reward_list_num || reward_list_seq >= RA_KUANG_HAI_REWARD_LIST_MAX_NUM)
	{
		return;
	}

	auto cfg_reward_list = LOGIC_CONFIG->GetRandActivityCfg().GetKuangHaiRewardItem(reward_list_seq);
	if (nullptr == cfg_reward_list)
	{
		return;
	}

	// 是否已兑换
	if ((m_activity_info.ra_kuang_hai_qing_dian_fetch_reward_flag & (0x01 << reward_list_seq)) != 0)
	{
		gstr::SendError(m_role, "en_ra_kuang_hai_has_fetch_reward");
		return;
	}

	// 检查嗨点是否足够
	int need_sceore = cfg_reward_list->need_score;
	if (need_sceore > m_activity_info.ra_kuang_hai_qing_dian_total_score)
	{
		// 嗨点不足
		gstr::SendError(m_role, "en_ra_kuang_hai_not_enought_score");
		return;
	}

	// 发放奖励
	m_role->GetKnapsack()->PutList(cfg_reward_list->reward_num, cfg_reward_list->reward_list, PUT_REASON_RA_KUANG_HAI_REWARD);

	m_role->NoticeNum(noticenum::NT_RAND_ACTIVITY_FETCH_REWARD_SUCC);

	// 记录购买
	m_activity_info.ra_kuang_hai_qing_dian_fetch_reward_flag |= (0x01 << reward_list_seq);

	SendKuangHaiInfo();
}
void RoleActivity::OnKuangHaiCommonOpera(int opera_type, int param_1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN);

	switch (opera_type)
	{
	case Protocol::RA_KUANG_HAI_OPERA_TYPE_INFO:
	{
		this->SendKuangHaiInfo();
	}
	break;

	case Protocol::RA_KUANG_HAI_OPERA_TYPE_FETCH_REWARD:
	{
		this->KuangHaiGetReward(param_1);
	}
	break;

	default:
		break;
	}
}
void RoleActivity::OnKuangHaiQingDianRecore(int task_type, int param)
{

	if (task_type <= RA_KUANG_HAI_TASK_TYPE_BEGIN || task_type >= RA_KUANG_HAI_TASK_TYPE_MAX_NUM)
		return;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	// 在原服则检查活动开启状态
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN);
	m_activity_info.ra_kuang_hai_qing_dian_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	auto task_vec_ptr = LOGIC_CONFIG->GetRandActivityCfg().GetKuangHaiTaskAllItems(task_type);
	if (nullptr == task_vec_ptr)
	{
		return;
	}

	auto& task_vec = *task_vec_ptr;

	switch (task_type)
	{
		// 特殊任务特殊处理 
	case RA_KUANG_HAI_TASK_TYPE_TARGET_RECHARGE:
	{
		for (auto* task_ptr : task_vec)// （指针）实际上，就循环1-3次
		{
			if (task_ptr == nullptr)
			{
				return;
			}

			if (task_ptr->index < 0 || task_ptr->index >= RA_KUANG_HAI_TASK_ITEM_MAX_NUM)
			{
				return;
			}

			// 需要达到每个任务的等级限制
			if (m_role->GetLevel() < task_ptr->open_level)
				continue;

			// 每个task所得的嗨点 不得超过 该task所能得的最大嗨点
			if (m_activity_info.ra_kuang_hai_qing_dian_score_list[task_ptr->index] >= task_ptr->max_score)
				continue;

			// 达成目标，赠送嗨点，重置进度
			if (param == task_ptr->task_arg && task_ptr->task_arg > 0)
			{
				m_activity_info.ra_kuang_hai_qing_dian_total_score += task_ptr->per_score;					// 记录总嗨点
				m_activity_info.ra_kuang_hai_qing_dian_score_list[task_ptr->index] += task_ptr->per_score;	// 记录单个item嗨点
			}
		}
	}
	break;

	// 通用处理
	default:
	{
		for (auto* task_ptr : task_vec)// （指针）实际上，就循环1-3次
		{
			if (task_ptr == nullptr)
			{
				return;
			}

			if (task_ptr->index < 0 || task_ptr->index >= RA_KUANG_HAI_TASK_ITEM_MAX_NUM)
			{
				return;
			}

			// 需要达到每个任务的等级限制
			if (m_role->GetLevel() < task_ptr->open_level)
				continue;

			// 每个task所得的嗨点 不得超过 该task所能得的最大嗨点
			if (m_activity_info.ra_kuang_hai_qing_dian_score_list[task_ptr->index] >= task_ptr->max_score)
				continue;

			// 记录任务进度
			m_activity_info.ra_kuang_hai_qing_dian_task_info_list[task_ptr->index] += param;

			// 达成进度，赠送嗨点，重置进度
			while (m_activity_info.ra_kuang_hai_qing_dian_task_info_list[task_ptr->index] >= task_ptr->task_arg && task_ptr->task_arg > 0)
			{
				m_activity_info.ra_kuang_hai_qing_dian_total_score += task_ptr->per_score;					// 记录总嗨点
				m_activity_info.ra_kuang_hai_qing_dian_score_list[task_ptr->index] += task_ptr->per_score;	// 记录单个item嗨点
				m_activity_info.ra_kuang_hai_qing_dian_task_info_list[task_ptr->index] -= task_ptr->task_arg; // 单个任务item完成1次

																											  // 检查上限
				if (m_activity_info.ra_kuang_hai_qing_dian_score_list[task_ptr->index] >= task_ptr->max_score)
				{
					m_activity_info.ra_kuang_hai_qing_dian_task_info_list[task_ptr->index] = 0;
					break;
				}
			}
		}
	}
	break;
	}
}

// 礼物收割

void RoleActivity::OnEnterGiftHarvestScene()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_GIFT_HARVEST))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_GIFT_HARVEST);

	Posi enterpos;

	int fb_key = ActivityShadow::Instance().GetFBKeyByRoomIndex(0);

	if (SpecialGiftHarvest *gift_harvest_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialGiftHarvest(fb_key, true))
	{
		int activity_sceneid = LOGIC_CONFIG->GetGiftHarvestConfig().GetEnterInfo(&enterpos);

		if (activity_sceneid > 0)
		{
			// 本轮尚未开启
			if (!gift_harvest_logic->CanEnter())
			{
				gstr::SendError(m_role, "en_gift_harvest_round_not_open");
				return;
			}

			// 能否进入场景
			if (!m_role->GetRoleStatusManager()->CanEnterFB())
			{
				return;
			}

			//场景等级限制，目前只限制在活动场景
			if (!SceneRegister::Instance().CheckSceneLevelLimit(m_role, activity_sceneid))
			{
				return;
			}

			World::GetInstWorld()->GetSceneManager()->GoTo(m_role, activity_sceneid, fb_key, enterpos);
		}
	}
}

void RoleActivity::OnGiftHarvestReq(int opera_type)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_GIFT_HARVEST))
	{
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_GIFT_HARVEST);

	switch (opera_type)
	{
	case Protocol::RA_GIFT_HARVEST_OPERA_TYPE_INFO:
	{
		Protocol::SCRAGiftHarvestPlayerInfo msg;
		{
			msg.get_score = this->GetRAGiftHarvestScore();
			msg.kill_num = this->GetRAGiftHarvestGetScoreTimes();
			msg.round = 0;
			if (auto *ra_gift_harvest = RandActivityManager::Instance().GetRandActivityGiftHarvest())
			{
				msg.round = ra_gift_harvest->GetCurRound();
			}
		}
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&msg, sizeof(msg));
	}
	break;

	case Protocol::RA_GIFT_HARVEST_OPERA_TYPE_RANK_INFO:
	{
		if (auto *ra_gift_harvest = RandActivityManager::Instance().GetRandActivityGiftHarvest())
		{
			ra_gift_harvest->SendRankInfo(m_role);
		}
	}
	break;

	case Protocol::RA_GIFT_HARVEST_OPERA_TYPE_ENTER_SCENE:
	{
		this->OnEnterGiftHarvestScene();
	}
	break;

	case Protocol::RA_GIFT_HARVEST_OPERA_TYPE_ROUND_INFO:
	{
		if (auto *ra_gift_harvest = RandActivityManager::Instance().GetRandActivityGiftHarvest())
		{
			ra_gift_harvest->SendRoundInfo(m_role);
		}
	}
	break;

	default:
		break;
	}
}

// 幸运云购
void RoleActivity::OnOpenLuckyCloudBuyPanel(unsigned int timestamp)
{
	auto * luckybuy_activity = RandActivityManager::Instance().GetRandActivityLuckyCloudBuy();
	if (nullptr == luckybuy_activity)
	{
		return;
	}
	luckybuy_activity->RecordOpenState(m_role);
	luckybuy_activity->SendBuyRecordInfo(m_role, timestamp);
}

void RoleActivity::OnCloseLuckyCloudBuyPanel()
{
	auto * luckybuy_activity = RandActivityManager::Instance().GetRandActivityLuckyCloudBuy();
	if (nullptr == luckybuy_activity)
	{
		return;
	}
	luckybuy_activity->RecordCloseState(m_role);
}

void RoleActivity::SendLuckyCloudBuyInfo()
{
	auto * luckybuy_activity = RandActivityManager::Instance().GetRandActivityLuckyCloudBuy();
	if (nullptr == luckybuy_activity)
	{
		return;
	}

	int round_index = luckybuy_activity->GetThisRoundIndex();
	if (round_index == -1)
	{
		round_index = 0;
	}

	auto* round_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyCloudBuyCfg(round_index);
	if (nullptr == round_cfg)
	{
		return; // 
	}

	Protocol::SCRALuckyCloudBuyInfo msg;
	msg.seq = round_cfg->seq;
	msg.buy_self = m_activity_info.ra_lucky_cloud_buy_count;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, sizeof(msg));
}

bool RoleActivity::LuckyCloudBuy()
{
	// 购买限制检查
	if (m_activity_info.ra_lucky_cloud_buy_count >= LOGIC_CONFIG->GetRandActivityCfg().GetLuckyBuylimit())
	{
		if (!LOGIC_CONFIG->GetRandActivityCfg().IsLuckyBuyUnlimitTime())
		{
			return false;// 超过每轮购买限制，且未到无限制时间
		}
	}

	// 活动情况检查
	auto * luckybuy_activity = RandActivityManager::Instance().GetRandActivityLuckyCloudBuy();
	if (nullptr == luckybuy_activity)
	{
		return false;	// 无此活动
	}

	if (luckybuy_activity->IsThisRoundAllSell())
	{
		m_role->NoticeNum(errornum::EN_RA_LUCKY_CLOUD_BUY_ALL_SELL);
		return false;// 本轮库存已售罄，请等待下一轮开启
	}

	if (luckybuy_activity->IsAllRoundFinish())
	{
		m_role->NoticeNum(errornum::EN_RA_LUCKY_CLOUD_ALL_ROUND_FINISH);
		return false;// 全轮刷完了，活动结束
	}

	if (!luckybuy_activity->CanJoin())
	{
		m_role->NoticeNum(errornum::EN_RA_LUCKY_CLOUD_THIS_ROUND_NO_OPEN);
		return false;// 下一轮尚未开始
	}

	// 配置检查
	const RandLuckyCloudBuyCfg::RoundItem* round_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLuckyCloudBuyCfg(luckybuy_activity->GetThisRoundIndex());
	if (nullptr == round_cfg)
	{
		return false; // 配置错误
	}

	// 保底奖励
	const ItemConfigData* item = round_cfg->GetMinRewardItem();
	if (nullptr == item)
	{
		return false; // 保底奖励抽取错误
	}

	// 元宝检查及消耗
	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(round_cfg->need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(round_cfg->need_gold, "LuckyCloudBuy"))
	{
		return false;
	}

	// 记录购买
	m_activity_info.ra_lucky_cloud_buy_count++;
	m_activity_info.ra_lucky_cloud_buy_price = round_cfg->need_gold;
	m_activity_info.ra_lucky_cloud_buy_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());//记录时间，初始时次数不清零
	luckybuy_activity->RecordBuy(m_role);

	// 获取保底奖励
	//m_role->GetKnapsack()->PutOrMail(*item,PUT_REASON_LUCKY_CLOUD_BUY_MIN_REWARD);
	{
		if (m_role->GetKnapsack()->HaveSpace())
		{
			m_role->GetKnapsack()->Put(*item, PUT_REASON_LUCKY_CLOUD_BUY_MIN_REWARD);
		}
		else
		{
			m_role->NoticeNum(errornum::EN_RA_LUCKY_CLOUD_KNAPSACK_NO_SPACE);
			const ItemBase *itembase = ITEMPOOL->GetItem(item->item_id);
			if (nullptr != itembase)
			{
				static MailContentParam contentparam; contentparam.Reset();
				contentparam.item_list[0].item_id = item->item_id;
				contentparam.item_list[0].is_bind = (short)item->is_bind;
				contentparam.item_list[0].num = item->num;
				contentparam.item_list[0].invalid_time = itembase->CalInvalidTime();

				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_knapsack_put_subiect);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_knapsack_put_content);
				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}// end if
	}

	// 通知玩家
	SendLuckyCloudBuyInfo();
	return true;
}

void RoleActivity::OnLuckyCloudBuyOperaReq(int opera_type, int param1)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY);

	switch (opera_type)
	{
	case Protocol::RA_LUCKY_CLOUD_BUY_OPERA_TYPE_INFO:
	{
		this->SendLuckyCloudBuyInfo();
	}
	break;

	case Protocol::RA_LUCKY_CLOUD_BUY_OPERA_TYPE_BUY:
	{
		const int LUCKY_MAX_BUY_NUM = 999;

		for (int i = 0; i < param1 && i < LUCKY_MAX_BUY_NUM; i++)
		{
			if (false == this->LuckyCloudBuy())
				break;
		}
	}
	break;

	case Protocol::RA_LUCKY_CLOUD_BUY_OPERA_TYPE_OOPEN:
	{
		this->OnOpenLuckyCloudBuyPanel((unsigned int)param1);
	}
	break;

	case Protocol::RA_LUCKY_CLOUD_BUY_OPERA_TYPE_CLOSE:
	{
		this->OnCloseLuckyCloudBuyPanel();
	}
	break;

	case Protocol::RA_LUCKY_CLOUD_BUY_OPERA_TYPE_LUCKY:
	{
		LuckyLogManager::Instance().SendLog(m_role, RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY);
	}
	break;

	}
}

void RoleActivity::CheckRALuckyCloudBuyReturn()
{
	if (m_activity_info.ra_lucky_cloud_buy_count <= 0)
	{
		ResetLuckyCloudBuy();
		return;
	}

	// 检查是否还在本轮云购中
	auto * luckybuy_activity = RandActivityManager::Instance().GetRandActivityLuckyCloudBuy();
	if (luckybuy_activity != nullptr && luckybuy_activity->IsInCurrentRound(m_role->GetUID()))
	{
		return; // 参与的本轮云购尚未结束
	}

	// 云购结束了，活动数据清零
	ResetLuckyCloudBuy();
	return;

	//// 检查是否可以领取返还元宝
	//CommonDataGlobal *common_data_global = GlobalRoleManager::Instance().GetCommonDataGlobal(m_role->GetUID());
	//if (nullptr == common_data_global)
	//{
	//	return;
	//}

	//// 邮件通知（领取返还元宝）
	//if (1 == common_data_global->GetLuckyCloudBuyReturn())
	//{
	//	int gold = m_activity_info.ra_lucky_cloud_buy_count * m_activity_info.ra_lucky_cloud_buy_price;
	//	static MailContentParam contentparam; contentparam.Reset();
	//	{
	//		contentparam.gold = gold;

	//		int length = 0;
	//		length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_lucky_cloud_buy_return_mail);

	//		if (length > 0)
	//		{
	//			MailRoute::MailToUser(IntToUid(m_role->GetUID()), SYSTEM_MAIL_REASON_INVALID, contentparam);
	//		}
	//	}
	//}

	//// 清空购买记录
	//ResetLuckyCloudBuy();
	//common_data_global->SetLuckyCloudBuyReturn(0);
}

void RoleActivity::ResetLuckyCloudBuy()
{
	// 重置
	m_activity_info.ra_lucky_cloud_buy_count = 0;
	m_activity_info.ra_lucky_cloud_buy_price = 0;
}

void RoleActivity::OnRACracyBuyOperaReq(int opera_type, int seq)
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CRACYBUY);

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CRACYBUY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	switch (opera_type)
	{
	case Protocol::RA_CRACYBUY_ALL_INFO:
	{
		this->OnRAACracyBuySendTakePartInMessage();
	}
	break;

	case Protocol::RA_CRACYBUY_LIMIT_INFO:
	{
		this->OnRAACracyBuySendItemLimitMessage();
	}
	break;

	case Protocol::RA_CRACYBUY_BUY:
	{
		this->OnRAACracyBuyToBuy(seq);
	}
	break;

	default:
		break;
	}
}

void RoleActivity::OnRAACracyBuySendItemLimitMessage()
{
	RandActivityCracyBuyToAll *rand_activity_crazy_buy = RandActivityManager::Instance().GetRandActivityCracyBuy();
	if (nullptr == rand_activity_crazy_buy)
	{
		return;
	}

	static Protocol::SCRACracyBuyLimitInfo msg;

	for (int i = 0; i < RA_MAX_CRACY_BUY_NUM_LIMIT; ++i)
	{
		msg.item_limit_info[i].all_limit = rand_activity_crazy_buy->GetLimit(i);
		msg.item_limit_info[i].person_limit = m_activity_info.ra_cracy_buy_limit[i];
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, sizeof(msg));
}

void RoleActivity::OnRAACracyBuySendTakePartInMessage()
{
	static Protocol::SCRACrazyBuyAllInfo msg;

	msg.chongzhi = m_activity_info.ra_cracy_buy_chongzhi;
	msg.level = m_activity_info.ra_cracy_buy_level;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, sizeof(msg));
}

void RoleActivity::OnRAACracyBuyToBuy(int seq)
{
	this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CRACYBUY);

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CRACYBUY))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}

	RandActivityCracyBuyToAll *rand_activity_crazy_buy = RandActivityManager::Instance().GetRandActivityCracyBuy();
	if (nullptr == rand_activity_crazy_buy)
	{
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (seq < 0 || seq >= RAND_ACTIVITY_CRACY_BUY_MAX_NUM_TO_OPENDAY)
	{
		return;
	}

	const RandActivityCrazyBuy::ContentCfg *cracybuy_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetCracyBuyCfg(seq);
	if (nullptr == cracybuy_cfg)
	{
		return;
	}

	int server_limit_num = rand_activity_crazy_buy->GetLimit(seq);

	if (m_activity_info.ra_cracy_buy_limit[seq] >= cracybuy_cfg->role_buy_times_limit || server_limit_num >= cracybuy_cfg->server_buy_times_limit || m_activity_info.ra_cracy_buy_level < cracybuy_cfg->level)
	{
		m_role->NoticeNum(errornum::EN_CRACY_BUY_LEVEL_LESS_THAN_ACTIVITY);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(cracybuy_cfg->gold_buy))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cracybuy_cfg->gold_buy, "RandActivityCracyBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 给予
	m_role->GetKnapsack()->PutOrMail(cracybuy_cfg->reward_item, PUT_REASON_CRACYBUY_REWARD);

	++m_activity_info.ra_cracy_buy_limit[seq];
	rand_activity_crazy_buy->SetLimit(seq);

	this->OnRAACracyBuySendItemLimitMessage();

	if (cracybuy_cfg->is_broadcast)
	{
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "rand_act_cracybuy_reward", UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(),
			(int)cracybuy_cfg->reward_item.item_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::OnRAACracyBuyToBuy user[%d, %s], seq[%d], gold[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), seq, cracybuy_cfg->gold_buy, ItemConfigDataLog(&cracybuy_cfg->reward_item, nullptr));
}

void RoleActivity::OnRACracyBuyChongZhi(int money)
{
	int activity_type = RAND_ACTIVITY_TYPE_CRACYBUY;
	if (!ActivityShadow::Instance().IsRandActivityOpen(activity_type))
	{
		return;
	}

	this->CheckInitRandActivity(activity_type);

	m_activity_info.ra_cracy_buy_chongzhi += money;

	int level = LOGIC_CONFIG->GetRandActivityCfg().GetCracyBuyLevelCfg(m_activity_info.ra_cracy_buy_chongzhi);

	m_activity_info.ra_cracy_buy_level = level;
	this->OnRAACracyBuySendTakePartInMessage();
}