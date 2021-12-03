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
#include "config/activityconfig/opengameactivityconfig.hpp"

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
#include "equipment/equipmentmanager.h"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "other/sublock/sublock.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"
#include "other/tianshuxunzhu/tianshuxunzhuconfig.hpp"
#include "global/cross/crossmanager.h"
#include "other/cross/rolecross.h"

// --------------------------------------------------------------------------------------------------------
// 开服活动
// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
// 开服活动
// --------------------------------------------------------------------------------------------------------
// 同步开服活动信息
void RoleActivity::SendOpenGameActivityInfo()
{
	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_OPENGAME))
	{
		return;
	}

	WorldStatusCommonDataParam &common_data = WorldStatus::Instance().GetCommonData();

	static Protocol::SCOpenGameActivityInfo cmd;

	cmd.oga_capability_reward_flag = m_activity_info.oga_capability_reward_flag;
	cmd.oga_rolelevel_reward_flag = m_activity_info.oga_rolelevel_reward_flag;
	cmd.oga_puton_equipment_reward_flag = m_activity_info.oga_puton_equipment_reward_flag;
	cmd.oga_buy_equipmentgift_flag = m_activity_info.oga_buy_equipment_gift_flag;
	cmd.total_chongzhi = static_cast<int>(m_activity_info.history_chongzhi);
	cmd.puton_equipment_act_flag = m_role->GetEquipmentManager()->GetPutonEquipmentActFlag();

	UNSTD_STATIC_CHECK(sizeof(cmd.oga_putonequipment_fetch_times) == sizeof(common_data.oga_putonequipment_fetch_times));
	memcpy(cmd.oga_putonequipment_fetch_times, common_data.oga_putonequipment_fetch_times, sizeof(cmd.oga_putonequipment_fetch_times));

	UNSTD_STATIC_CHECK(sizeof(cmd.oga_capability_fetch_times) == sizeof(common_data.oga_capability_fetch_times));
	memcpy(cmd.oga_capability_fetch_times, common_data.oga_capability_fetch_times, sizeof(cmd.oga_capability_fetch_times));

	UNSTD_STATIC_CHECK(sizeof(cmd.oga_rolelevel_fetch_times) == sizeof(common_data.oga_rolelevel_fetch_times));
	memcpy(cmd.oga_rolelevel_fetch_times, common_data.oga_rolelevel_fetch_times, sizeof(cmd.oga_rolelevel_fetch_times));

	cmd.oga_guild_battle_reward_type = m_activity_info.ra_guild_battle_reward_type;		//帮派争霸
	cmd.oga_guild_battle_reward_flag = m_activity_info.ra_guild_battle_reward_flag;		//帮派争霸

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

// 请求领取各个开服活动奖励
void RoleActivity::OnFetchOpenGameActivityReward(int reward_type, int reward_seq)
{
	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_OPENGAME))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_NOT_OPEN);
		return;
	}

	if (reward_type < 0 || reward_type >= Protocol::CSOpenGameActivityFetchReward::REWARD_TYPE_MAX)
	{
		return;
	}

	bool need_send = false;

	switch(reward_type)
	{
	case Protocol::CSOpenGameActivityFetchReward::REWARD_TYPE_PUTON_EQUIPMENT:
		{
			need_send = this->OnFetchOGAPutonEquipmentReward(reward_seq);
		}
		break;

	case Protocol::CSOpenGameActivityFetchReward::REWARD_TYPE_CAPABILITY:
		{
			need_send = this->OnFetchOGACapabilityReward(reward_seq);
		}
		break;

	case Protocol::CSOpenGameActivityFetchReward::REWARD_TYPE_ROLELEVEL:
		{
			need_send = this->OnFetchOGARoleLevelReward(reward_seq);
		}
		break;
	}

	if (need_send)
	{
		this->SendOpenGameActivityInfo();
	}
}

// 领取装备收集奖励
bool RoleActivity::OnFetchOGAPutonEquipmentReward(int seq)
{
	if (seq < 0 || seq >= static_cast<int>(sizeof(m_activity_info.oga_puton_equipment_reward_flag) * 8) ||
		seq >= OGA_PUTONEQUIPMENT_REWARD_MAX_SEQ)
	{
		return false;
	}

	if (0 != (m_activity_info.oga_puton_equipment_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_FETCH_REWARD);
		return false;
	}

	const OGAPutonEquipmentRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetOpenGameActivityCfg().GetPutonEquipmentReward(seq);
	if (NULL == cfg_item)
	{
		return false;
	}

	int act_flag = m_role->GetEquipmentManager()->GetPutonEquipmentActFlag();
	if (1 != (1 & (act_flag >> seq)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_PUTON_EQUIPMENT_LIMIT);
		return false;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg_item->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	WorldStatusCommonDataParam &world_status_common_data = WorldStatus::Instance().GetCommonData();
	int global_fetch_times = world_status_common_data.oga_putonequipment_fetch_times[seq];
	if (global_fetch_times >= cfg_item->global_fetch_times)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_GLOBAL_FETCH_TIMES_LIMIT);
		return true;
	}

	m_activity_info.oga_puton_equipment_reward_flag |= (1 << seq);
	++ world_status_common_data.oga_putonequipment_fetch_times[seq];

	m_role->GetKnapsack()->PutList(cfg_item->item_count, cfg_item->item_list, PUT_REASON_OPENGAME_ACTIVITY);

	{
		// 日志
		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

		gamelog::g_log_roleactivity.printf(LL_INFO, "OnFetchOGAShenFangjuReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d],  item_list[%s], global_fetch_times[%d]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.oga_puton_equipment_reward_flag, get_item_log.c_str(), world_status_common_data.oga_putonequipment_fetch_times[seq]);
	}

	return true;
}

// 领取战力冲刺奖励
bool RoleActivity::OnFetchOGACapabilityReward(int seq)
{
	if (seq < 0 || seq >= static_cast<int>(sizeof(m_activity_info.oga_capability_reward_flag) * 8) ||
		seq >= OGA_CAPABILITY_REWARD_MAX_SEQ)
	{
		return false;
	}

	if (0 != (m_activity_info.oga_capability_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_FETCH_REWARD);
		return false;
	}

	const OGACapabilityRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetOpenGameActivityCfg().GetCapabiblityReward(seq);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_role->GetCapability()->GetTotalCapability() < cfg_item->capability)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_CAPABILITY_LIMIT);
		return false;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg_item->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	WorldStatusCommonDataParam &world_status_common_data = WorldStatus::Instance().GetCommonData();
	int global_fetch_times = world_status_common_data.oga_capability_fetch_times[seq];
	if (global_fetch_times >= cfg_item->global_fetch_times)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_GLOBAL_FETCH_TIMES_LIMIT);
		return true;
	}

	m_activity_info.oga_capability_reward_flag |= (1 << seq);
	++ world_status_common_data.oga_capability_fetch_times[seq];

	m_role->GetKnapsack()->PutList(cfg_item->item_count, cfg_item->item_list, PUT_REASON_OPENGAME_ACTIVITY);

	{
		// 日志
		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

		gamelog::g_log_roleactivity.printf(LL_INFO, "OnFetchOGACapabilityReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d],  item_list[%s], global_fetch_times[%d]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.oga_capability_reward_flag, get_item_log.c_str(), world_status_common_data.oga_capability_fetch_times[seq]);
	}

	return true;
}

// 领取等级冲刺奖励
bool RoleActivity::OnFetchOGARoleLevelReward(int seq)
{
	if (seq < 0 || seq >= static_cast<int>(sizeof(m_activity_info.oga_rolelevel_reward_flag) * 8) ||
		seq >= OGA_ROLELEVEL_REWARD_MAX_SEQ)
	{
		return false;
	}

	if (0 != (m_activity_info.oga_rolelevel_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_FETCH_REWARD);
		return false;
	}

	const OGARoleLevelRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetOpenGameActivityCfg().GetRolelevelReward(seq);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_role->GetLevel() < cfg_item->level)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_CAPABILITY_LIMIT);
		return false;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(cfg_item->item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	WorldStatusCommonDataParam &world_status_common_data = WorldStatus::Instance().GetCommonData();
	int global_fetch_times = world_status_common_data.oga_rolelevel_fetch_times[seq];
	if (global_fetch_times >= cfg_item->global_fetch_times)
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_GLOBAL_FETCH_TIMES_LIMIT);
		return true;
	}

	m_activity_info.oga_rolelevel_reward_flag |= (1 << seq);
	++ world_status_common_data.oga_rolelevel_fetch_times[seq];

	m_role->GetKnapsack()->PutList(cfg_item->item_count, cfg_item->item_list, PUT_REASON_OPENGAME_ACTIVITY);

	{
		// 日志
		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

		gamelog::g_log_roleactivity.printf(LL_INFO, "OnFetchOGARoleLevelReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d],  item_list[%s], global_fetch_times[%d]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.oga_rolelevel_reward_flag, get_item_log.c_str(), world_status_common_data.oga_rolelevel_fetch_times[seq]);
	}

	return true;
}

// 购买装备礼包
void RoleActivity::OnOGABuyEquipmentGift(int seq)
{
	if (seq < 0 || seq >= static_cast<int>(sizeof(m_activity_info.oga_buy_equipment_gift_flag) * 8))
	{
		return;
	}

	if (0 != (m_activity_info.oga_buy_equipment_gift_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_ALREADY_BUY);
		return;
	}

	if (!ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_OPENGAME))
	{
		m_role->NoticeNum(errornum::EN_OPEN_GAME_ACTIVITY_NOT_OPEN);
		return;
	}

	UNSTD_STATIC_CHECK(PROF_TYPE_PROF_ZHUANZHI_BASE != 0);
	int prof = m_role->GetProf() % PROF_TYPE_PROF_ZHUANZHI_BASE;

	const OGAEquipmentGiftConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetOpenGameActivityCfg().GetEquipmentGiftItemCfg(seq, prof);
	if (NULL == cfg_item)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cfg_item->price, "OnOGABuyEquipmentGift"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_activity_info.oga_buy_equipment_gift_flag |= (1 << seq);

	m_role->GetKnapsack()->Put(cfg_item->item, PUT_REASON_OPENGAME_ACTIVITY);
	this->SendOpenGameActivityInfo();

	gamelog::g_log_roleactivity.printf(LL_INFO, "OnOGABuyEquipmentGift user[%d, %s], level[%d], reward_seq[%d], buy_flag[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.oga_buy_equipment_gift_flag);
}

void RoleActivity::SendBaiBeiFanLiInfo()
{
	unsigned int close_time = static_cast<unsigned int>(m_role->GetServerStartTime()) + (LOGIC_CONFIG->GetOpenGameActivityCfg().GetOtherConfig().baibeifanli_end_time - 1) * 86400;
	unsigned int zero_time = static_cast<unsigned int>(GetZeroTime(close_time));

	Protocol::SCBaiBeiFanLiInfo pro;
	pro.is_buy = m_activity_info.baibeifanli_flag;
	pro.close_time = zero_time;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void RoleActivity::OnBaiBeiFanLiBuy()
{
	unsigned int close_time = static_cast<unsigned int>(m_role->GetServerStartTime()) + (LOGIC_CONFIG->GetOpenGameActivityCfg().GetOtherConfig().baibeifanli_end_time - 1) * 86400;
	unsigned int zero_time = static_cast<unsigned int>(GetZeroTime(close_time));

	if (0 != m_activity_info.baibeifanli_flag || EngineAdapter::Instance().Time() >= zero_time)
	{
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

	const OGAOtherConfig &other_cfg = LOGIC_CONFIG->GetOpenGameActivityCfg().GetOtherConfig();
	if (m_role->GetLevel() < other_cfg.baibeifanli_level_limit)
	{
		m_role->NoticeNum(errornum::EN_ITEM_LEVEL_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.baibeifanli_price))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg.baibeifanli_price, "BaiBeiFanLiBuy"))
	{
		return;
	}

	m_role->GetKnapsack()->Put(other_cfg.baibeifanli_item, PUT_REASON_OPENGAME_ACTIVITY);

	m_activity_info.baibeifanli_flag = 1;

	m_role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_BUY_HUNDRED_REWARD, 0);

	gamelog::g_log_roleactivity.printf(LL_INFO, "OnBaiBeiFanLiBuy user[%d, %s], level[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel());

	this->SendBaiBeiFanLiInfo();
}

void RoleActivity::SendBaiBeiFanLi2Info()
{
	const OGAOtherConfig &other_cfg = LOGIC_CONFIG->GetOpenGameActivityCfg().GetOtherConfig();

	unsigned int time_interval = (other_cfg.baibeifanli2_open_time - 1) * 86400;
	unsigned int open_time = static_cast<unsigned int>(GetZeroTime(static_cast<unsigned int>(m_role->GetServerStartTime()) + time_interval));
	unsigned int close_time = open_time + (other_cfg.baibeifanli2_end_time - other_cfg.baibeifanli2_open_time) * 86400;
	unsigned int real_close_time = static_cast<unsigned int>(GetZeroTime(close_time));

	Protocol::SCBaiBeiFanLi2Info pro;
	pro.is_buy = m_activity_info.baibeifanli2_flag;
	pro.close_time = real_close_time;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void RoleActivity::OnBaiBeiFanLi2Buy()
{
	const OGAOtherConfig &other_cfg = LOGIC_CONFIG->GetOpenGameActivityCfg().GetOtherConfig();

	unsigned int time_interval = (LOGIC_CONFIG->GetOpenGameActivityCfg().GetOtherConfig().baibeifanli2_open_time - 1) * 86400;
	unsigned int open_time = static_cast<unsigned int>(GetZeroTime(static_cast<unsigned int>(m_role->GetServerStartTime()) + time_interval));
	unsigned int close_time = open_time + (other_cfg.baibeifanli2_end_time - other_cfg.baibeifanli2_open_time) * 86400;
	unsigned int real_close_time = static_cast<unsigned int>(GetZeroTime(close_time));

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (now_time < open_time || now_time > real_close_time)
	{
		return;
	}

	if (0 != m_activity_info.baibeifanli2_flag)
	{
		m_role->NoticeNum(errornum::EN_BAIBEIFANLI2_ALREADY_BUY);
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

	//const OGAOtherConfig &other_cfg = LOGIC_CONFIG->GetOpenGameActivityCfg().GetOtherConfig();
	if (m_role->GetLevel() < other_cfg.baibeifanli2_level_limit)
	{
		m_role->NoticeNum(errornum::EN_ITEM_LEVEL_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.baibeifanli2_price))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg.baibeifanli2_price, "BaiBeiFanLi2Buy"))
	{
		return;
	}

	m_role->GetKnapsack()->Put(other_cfg.baibeifanli2_item, PUT_REASON_OPENGAME_ACTIVITY);

	m_activity_info.baibeifanli2_flag = 1;

	m_role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_BUY_HUNDRED2_REWARD, 0);

	gamelog::g_log_roleactivity.printf(LL_INFO, "OnBaiBeiFanLi2Buy user[%d, %s], level[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel());

	this->SendBaiBeiFanLi2Info();
}

void RoleActivity::OnFetchOpenGameKillBossReward(int seq)
{
	if (seq < 0 || seq >= OGA_BOSS_REWARD_MAX_SEQ)
	{
		return;
	}

	if ((m_activity_info.oga_kill_boss_reward_flag & (1 << seq)) != 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
		return;
	}

	const OGAKillBossReward *reward = LOGIC_CONFIG->GetOpenGameActivityCfg().GetKillBossReward(seq);
	if (NULL == reward || reward->reward_item.item_id <= 0)
	{
		return;
	}

	if (0 < reward->boss_seq_1 && reward->boss_seq_1 < OGA_BOSS_REWARD_MAX_SEQ)
	{
		if (0 == (m_activity_info.oga_kill_boss_flag & (static_cast<long long>(1) << reward->boss_seq_1)))
		{
			return;
		}
	}

	if (0 < reward->boss_seq_2 && reward->boss_seq_2 < OGA_BOSS_REWARD_MAX_SEQ)
	{
		if (0 == (m_activity_info.oga_kill_boss_flag & (static_cast<long long>(1) << reward->boss_seq_2)))
		{
			return;
		}
	}

	if (0 < reward->boss_seq_3 && reward->boss_seq_3 < OGA_BOSS_REWARD_MAX_SEQ)
	{
		if (0 == (m_activity_info.oga_kill_boss_flag & (static_cast<long long>(1) << reward->boss_seq_3)))
		{
			return;
		}
	}

	if (0 < reward->boss_seq_4 && reward->boss_seq_4 < OGA_BOSS_REWARD_MAX_SEQ)
	{
		if (0 == (m_activity_info.oga_kill_boss_flag & (static_cast<long long>(1) << reward->boss_seq_4)))
		{
			return;
		}
	}

	if (!m_role->GetKnapsack()->Put(reward->reward_item, PUT_REASON_OPENGAME_ACTIVITY))
	{
		return;
	}

	m_activity_info.oga_kill_boss_reward_flag |= (1 << seq);

	this->SendRAOpenServerBossInfo();
}

void RoleActivity::OnOpenGameGiftShopBuy(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SUPPER_GIFT))
	{
		return;
	}

	if (seq < 0 || seq >= OGA_GIFT_MAX_SEQ)
	{
		return;
	}

	const OGAGiftShopConfig *config_item = LOGIC_CONFIG->GetOpenGameActivityCfg().GetGiftShopConfig(m_role->GetLevel(), seq);
	if (NULL == config_item)
	{
		return;
	}

	if ((m_activity_info.oga_gift_shop_flag & (1 << seq)) != 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(config_item->price))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int item_num = 0;
	ItemConfigData item_list[OGA_GIFT_MAX_TYPE];
	for (int i = 0; i < OGA_GIFT_MAX_TYPE; i++)
	{
		if (config_item->gift_item_list[i].item_id > 0)
		{
			item_list[item_num].item_id = config_item->gift_item_list[i].item_id;
			item_list[item_num].num = config_item->gift_item_list[i].num;
			item_list[item_num].is_bind = config_item->gift_item_list[i].is_bind;
			item_num++;
		}
	}

	if (item_num <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(item_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(config_item->price, "OpenGameGiftShop"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->PutList(item_num, item_list, PUT_REASON_OPENGAME_ACTIVITY))
	{
		return;
	}


	{
		// 传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_time_limit_gift,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), seq);
		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	m_activity_info.oga_gift_shop_flag |= (1 << seq);

	this->SendOpenGameGiftShopBuyInfo();

	TianShuXunZhu::CheckShiXueTianShu(m_role, CHENGZHANG_TIANSHU_TYPE_SUPPER_GIFT);
}

void RoleActivity::OnOpenGameGiftShopBuy2(int seq)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SUPPER_GIFT_2))
	{
		m_role->NoticeNum(errornum::EN_GIFTSHOP_BUY2_NOT_START);
		return;
	}

	if (seq < 0 || seq >= OGA_GIFT_MAX_SEQ)
	{
		m_role->NoticeNum(errornum::EN_GIFTSHOP_BUY2_SEQ_ERROR);
		return;
	}

	const OGAGiftShopConfig *config_item = LOGIC_CONFIG->GetOpenGameActivityCfg().GetGiftShop2Config(m_role->GetLevel(), seq);
	if (NULL == config_item)
	{
		return;
	}

	if ((m_activity_info.oga_gift_shop2_flag & (1 << seq)) != 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(config_item->price))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int item_num = 0;
	ItemConfigData item_list[OGA_GIFT_MAX_TYPE];
	for (int i = 0; i < OGA_GIFT_MAX_TYPE; i++)
	{
		if (config_item->gift_item_list[i].item_id > 0)
		{
			item_list[item_num].item_id = config_item->gift_item_list[i].item_id;
			item_list[item_num].num = config_item->gift_item_list[i].num;
			item_list[item_num].is_bind = config_item->gift_item_list[i].is_bind;
			item_num++;
		}
	}

	if (item_num <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(item_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(config_item->price, "OpenGameGiftShop2"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->PutList(item_num, item_list, PUT_REASON_OPENGAME_ACTIVITY))
	{
		return;
	}


	{
		// 传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_time_limit_gift_2,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), seq);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	m_activity_info.oga_gift_shop2_flag |= (1 << seq);

	this->SendOpenGameGiftShopBuy2Info();

	
}