#include "other/roleactivity/roleactivity.hpp"
#include "world.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "scene/activityshadow/activityshadow.hpp"

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
#include "config/sharedconfig/sharedconfig.h"
#include "other/vip/vipconfig.hpp"
#include "global/team/team.hpp"
#include "other/vip/vip.hpp"
#include "other/capability/capability.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "global/randactivity/randactivitymanager.hpp"
#include "global/randactivity/impl/randactivityholidayguard.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/mount/mountopconfig.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/wing/wingmanager.hpp"

// --------------------------------------------------------------------------------------------------------
// 随机活动结束时初发奖励
// --------------------------------------------------------------------------------------------------------
void RoleActivity::RACheckBipinCapTotalGiveReweard()
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_total_fetch_reward_flag) * 8);

	for (int i = 0; i < RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 == (m_activity_info.ra_bipin_cap_total_fetch_reward_flag & (1 << i)))
		{
			const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL, i);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_activity_info.ra_bipin_cap_total_history_max_value < cfg_item->need_capability)
			{
				break;
			}

			static MailContentParam contentparam; contentparam.Reset();
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, cfg_item->reward_gold_bind);

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 
			}	

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_all_reward_content, RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL);
			if (length > 0)
			{
				m_activity_info.ra_bipin_cap_total_fetch_reward_flag |= (1 << i);

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckBipinCapTotalGiveReweard::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
					m_role->GetUID(), m_role->GetName(), i, m_activity_info.ra_bipin_cap_total_fetch_reward_flag, 
					cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
			}
		}
	}
}

void RoleActivity::RACheckBipinCapEquipGiveReweard()
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_equip_fetch_reward_flag) * 8);

	for (int i = 0; i < RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 == (m_activity_info.ra_bipin_cap_equip_fetch_reward_flag & (1 << i)))
		{
			const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP, i);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_activity_info.ra_bipin_cap_equip_history_max_value < cfg_item->need_capability)
			{
				break;
			}

			static MailContentParam contentparam; contentparam.Reset();
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, cfg_item->reward_gold_bind);

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 
			}	

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_all_reward_content, RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP);
			if (length > 0)
			{
				m_activity_info.ra_bipin_cap_equip_fetch_reward_flag |= (1 << i);

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckBipinCapEquipGiveReweard::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
					m_role->GetUID(), m_role->GetName(), i, m_activity_info.ra_bipin_cap_equip_fetch_reward_flag, 
					cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
			}
		}
	}
}

void RoleActivity::RACheckBipinCapXiaNvGiveReweard()
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_xiannv_fetch_reward_flag) * 8);

	for (int i = 0; i < RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 == (m_activity_info.ra_bipin_cap_xiannv_fetch_reward_flag & (1 << i)))
		{
			const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV, i);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_activity_info.ra_bipin_cap_xiannv_history_max_value < cfg_item->need_capability)
			{
				break;
			}

			static MailContentParam contentparam; contentparam.Reset();
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, cfg_item->reward_gold_bind);

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 
			}	

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_all_reward_content, RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV);
			if (length > 0)
			{
				m_activity_info.ra_bipin_cap_xiannv_fetch_reward_flag |= (1 << i);

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckBipinCapXiaNvGiveReweard::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
					m_role->GetUID(), m_role->GetName(), i, m_activity_info.ra_bipin_cap_xiannv_fetch_reward_flag, 
					cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
			}
		}
	}
}

void RoleActivity::RACheckBipinCapJingMaiGiveReweard()
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_jingmai_fetch_reward_flag) * 8);

	for (int i = 0; i < RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 == (m_activity_info.ra_bipin_cap_jingmai_fetch_reward_flag & (1 << i)))
		{
			const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI, i);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_activity_info.ra_bipin_cap_jingmai_history_max_value < cfg_item->need_capability)
			{
				break;
			}

			static MailContentParam contentparam; contentparam.Reset();
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, cfg_item->reward_gold_bind);

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 
			}	

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_all_reward_content, RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI);
			if (length > 0)
			{
				m_activity_info.ra_bipin_cap_jingmai_fetch_reward_flag |= (1 << i);

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckBipinCapJingMaiGiveReweard::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
					m_role->GetUID(), m_role->GetName(), i, m_activity_info.ra_bipin_cap_jingmai_fetch_reward_flag, 
					cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
			}
		}
	}
}

void RoleActivity::RACheckBipinCapMountGiveReweard()
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_mount_fetch_reward_flag) * 8);

	for (int i = 0; i < RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 == (m_activity_info.ra_bipin_cap_mount_fetch_reward_flag & (1 << i)))
		{
			const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT, i);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_activity_info.ra_bipin_cap_mount_history_max_value < cfg_item->need_capability)
			{
				break;
			}

			static MailContentParam contentparam; contentparam.Reset();
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, cfg_item->reward_gold_bind);

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 
			}	

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_all_reward_content, RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT);
			if (length > 0)
			{
				m_activity_info.ra_bipin_cap_mount_fetch_reward_flag |= (1 << i);

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckBipinCapMountGiveReweard::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
					m_role->GetUID(), m_role->GetName(), i, m_activity_info.ra_bipin_cap_mount_fetch_reward_flag, 
					cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
			}
		}
	}
}

void RoleActivity::RACheckBipinCapWingGiveReweard()
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_wing_fetch_reward_flag) * 8);

	for (int i = 0; i < RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 == (m_activity_info.ra_bipin_cap_wing_fetch_reward_flag & (1 << i)))
		{
			const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING, i);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_activity_info.ra_bipin_cap_wing_history_max_value < cfg_item->need_capability)
			{
				break;
			}

			static MailContentParam contentparam; contentparam.Reset();
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, cfg_item->reward_gold_bind);

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 
			}	

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_all_reward_content, RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING);
			if (length > 0)
			{
				m_activity_info.ra_bipin_cap_wing_fetch_reward_flag |= (1 << i);

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckBipinCapWingGiveReweard::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
					m_role->GetUID(), m_role->GetName(), i, m_activity_info.ra_bipin_cap_wing_fetch_reward_flag, 
					cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
			}
		}
	}
}

void RoleActivity::RACheckConsumeGoldFanliGiveReward()
{
	if (m_activity_info.ra_consume_gold_fanli_consume_gold > 0)
	{
		int fanli_rate = LOGIC_CONFIG->GetRandActivityCfg().GetConsumeGoldFanliRate();

		int fanli_gold = static_cast<int>(fanli_rate * 1.0 / 100 * m_activity_info.ra_consume_gold_fanli_consume_gold);
		if (fanli_gold > 0)
		{
			static MailContentParam contentparam; contentparam.Reset();

			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, fanli_gold);

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_consume_gold_fanli_reward_content, 
				m_activity_info.ra_consume_gold_fanli_consume_gold, fanli_gold);

			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		m_activity_info.ra_consume_gold_fanli_consume_gold = 0;
	}
}

void RoleActivity::RACheckTotalConsumeGoldGiveReward()
{
	if (m_activity_info.ra_total_consume_gold_consume_num > 0)
	{
		const RandActivityTotalConsumeGoldCfg *diff_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetDiffTotalConsumeGoldRewardCfg();
		if (NULL == diff_cfg) return;

		for (int i = 0; i < diff_cfg->cfg_count; ++ i)
		{

			UNSTD_STATIC_CHECK(RAND_ACTIVITY_TOTAL_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_total_consume_gold_fetch_reward_flag) * 8);
			if (0 != (m_activity_info.ra_total_consume_gold_fetch_reward_flag & (1 << i)))
			{
				continue;
			}

			const RandActivityTotalConsumeGoldCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetTotalConsumeGoldRewardCfg(i);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_activity_info.ra_total_consume_gold_consume_num < cfg_item->need_consume_gold)
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

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_consume_gold_reward_content, cfg_item->need_consume_gold);
				if (length > 0)
				{
					m_activity_info.ra_total_consume_gold_fetch_reward_flag |= (1 << i);

					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}				
		}

		m_activity_info.ra_total_consume_gold_consume_num = 0;
		m_activity_info.ra_total_consume_gold_fetch_reward_flag = 0;
		this->SendRATotalConsumeGoldInfo();
	}
}

void RoleActivity::RACheckKillBossReward()
{
	const RandActivityKillBossCfg &kill_boss_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetKillBossRewardCfg();

	if (m_activity_info.ra_kill_boss_kill_count >= kill_boss_cfg.need_kill_boss_count)
	{
		if (kill_boss_cfg.need_kill_boss_count <= 0) return;

		int can_fetch_num = m_activity_info.ra_kill_boss_kill_count / kill_boss_cfg.need_kill_boss_count;
		if (can_fetch_num > 0)
		{
			static MailContentParam contentparam; contentparam.Reset();

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(kill_boss_cfg.reward_item.item_id);
			if (NULL != reward_item_base)
			{
				if (can_fetch_num >= reward_item_base->GetPileLimit())
				{
					can_fetch_num = reward_item_base->GetPileLimit();
				}

				contentparam.item_list[0].item_id = kill_boss_cfg.reward_item.item_id;
				contentparam.item_list[0].num = can_fetch_num;
				contentparam.item_list[0].is_bind = (kill_boss_cfg.reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_kill_boss_reward_content, m_activity_info.ra_kill_boss_kill_count);
				if (length > 0)
				{
					m_activity_info.ra_kill_boss_kill_count = 0;
					this->SendRAKillBossInfo();

					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}					
		}
	}
}

void RoleActivity::RACheckChestshopGiveReward()
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_CHESTSHOP_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_chestshop_fetch_reward_flag) * 8);

	if (m_activity_info.ra_chestshop_xunbao_times > 0)
	{
		int reward_count = LOGIC_CONFIG->GetRandActivityCfg().GetChestshopRewardCfgCount();
		for (int i = 0; i < reward_count; ++ i)
		{
			if (0 == (m_activity_info.ra_chestshop_fetch_reward_flag & (1 << i)))
			{
				const RandActivityChestShopCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetChestshopCfgBySeq(i);
				if (NULL != cfg_item && m_activity_info.ra_chestshop_xunbao_times >= cfg_item->need_chestshop_times)
				{
					const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
					if (NULL != reward_item_base)
					{
						static MailContentParam contentparam; contentparam.Reset();

						contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
						contentparam.item_list[0].num = cfg_item->reward_item.num;
						contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
						contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

						int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_chestshop_reward_content, cfg_item->need_chestshop_times);
						if (length > 0)
						{
							m_activity_info.ra_chestshop_fetch_reward_flag |= (1 << i);

							MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
						}
					}
				}
			}
		}

		m_activity_info.ra_chestshop_xunbao_times = 0;
		m_activity_info.ra_chestshop_fetch_reward_flag = 0;
		this->SendRAChestshopInfo();
	}
}

void RoleActivity::RACheckTotalChargeDayGiveReward()
{
	if (m_activity_info.ra_total_charge_day_count <= 0)
	{
		return;
	}

	for (int charge_day = m_activity_info.ra_total_charge_day_count; charge_day > 0; -- charge_day)
	{
		const RandActivityTotalChargeDayCfg::TotalChargeDayItem *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTotalChargeDayCfgWithDayCount(charge_day);
		if (NULL == cfg)
		{
			continue;
		}

		if (0 != (m_activity_info.ra_total_charge_day_reward_has_fetch_flag & (1 << cfg->seq)))
		{
			continue;
		}

		UNSTD_STATIC_CHECK(RandActivityTotalChargeDayCfg::TotalChargeDayItem::REWARD_ITEM_MAX_COUT <= MAX_ATTACHMENT_ITEM_NUM);

		// 发邮件奖励 - 扣除数据
		static MailContentParam contentparam; contentparam.Reset();

		int reward_item_count = 0;
		for (int i = 0; i < cfg->reward_count && reward_item_count < MAX_ATTACHMENT_ITEM_NUM; ++ i)
		{
			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg->reward_item_list[i].item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[reward_item_count].item_id = cfg->reward_item_list[i].item_id;
				contentparam.item_list[reward_item_count].num = cfg->reward_item_list[i].num;
				contentparam.item_list[reward_item_count].is_bind = (cfg->reward_item_list[i].is_bind) ? 1 : 0;
				contentparam.item_list[reward_item_count].invalid_time = reward_item_base->CalInvalidTime();
				++ reward_item_count;
			}
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_charge_day_reward_content,
			RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY, charge_day);
		if (length > 0)
		{
			m_activity_info.ra_total_charge_day_reward_has_fetch_flag |= (1 << cfg->seq);

			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			std::string reward_item_list_str;
			ItemConfigDataListLog(reward_item_list_str, cfg->reward_count, cfg->reward_item_list);

			gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckTotalChargeDayGiveReward::user[%d, %s], seq[%d], reward_flag[%d], reward_item_list[%s]",
				m_role->GetUID(), m_role->GetName(), cfg->seq, m_activity_info.ra_total_charge_day_reward_has_fetch_flag, reward_item_list_str.c_str());
		}
	}

	m_activity_info.ra_total_charge_day_count = 0;
	SendRATotalChargeDayInfo();
}


void RoleActivity::RACheckLoginGiftGiveReward()
{
	if (m_activity_info.ra_login_gift_login_days > 0)
	{
		// 累计奖励
		if (0 == m_activity_info.ra_login_gift_has_fetch_accumulate_reward)
		{
			const RandActivityLoginGiftCfg *login_gift_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLoginGiftCfg();
			if (NULL == login_gift_cfg)
			{
				return;
			}

			if (m_activity_info.ra_login_gift_login_days >= login_gift_cfg->need_accumulate_days)
			{
				const ItemBase *reward_item_base = ITEMPOOL->GetItem(login_gift_cfg->accumulate_reward.item_id);
				if (NULL != reward_item_base)
				{
					static MailContentParam contentparam; contentparam.Reset();

					contentparam.item_list[0].item_id = login_gift_cfg->accumulate_reward.item_id;
					contentparam.item_list[0].num = login_gift_cfg->accumulate_reward.num;
					contentparam.item_list[0].is_bind = (login_gift_cfg->accumulate_reward.is_bind) ? 1 : 0;
					contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_login_gift_reward_content, 
						m_activity_info.ra_login_gift_login_days);
					if (length > 0)
					{

						m_activity_info.ra_login_gift_has_fetch_accumulate_reward = 1;

						MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
					}
				}
			}
		}

		{
			// 普通奖励
			UNSTD_STATIC_CHECK(RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_login_gift_fetch_common_reward_flag) * 8);

			for (int seq = 0; seq < RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT; ++ seq)
			{
				if (0 != (m_activity_info.ra_login_gift_fetch_common_reward_flag & (1 << seq)))
				{
					continue;
				}

				const RandActivityLoginGiftCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetLoginGiftRewardCfg(seq);
				if (NULL == cfg_item)
				{
					break;
				}

				if (m_activity_info.ra_login_gift_login_days < cfg_item->need_login_days)
				{
					break;
				}

				const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->common_reward_item.item_id);
				if (NULL != reward_item_base)
				{
					static MailContentParam contentparam; contentparam.Reset();

					contentparam.item_list[0].item_id = cfg_item->common_reward_item.item_id;
					contentparam.item_list[0].num = cfg_item->common_reward_item.num;
					contentparam.item_list[0].is_bind = (cfg_item->common_reward_item.is_bind) ? 1 : 0;
					contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_login_gift_reward_content, 
						m_activity_info.ra_login_gift_login_days);
					if (length > 0)
					{

						m_activity_info.ra_login_gift_fetch_common_reward_flag |= (1 <<seq);

						MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
					}
				}
			}
		}

		{
			// vip奖励
			int vip_level = m_role->GetVip()->GetVipLevel();
			int can_fetch_reward = (0 != LOGIC_CONFIG->GetVipConfig().GetAuthParam(vip_level, VAT_RA_LOGIN_GIFT_VIP_REWARD));

			if (can_fetch_reward)
			{
				UNSTD_STATIC_CHECK(RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_login_gift_fetch_vip_reward_flag) * 8);

				for (int seq = 0; seq < RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT; ++ seq)
				{
					if (0 != (m_activity_info.ra_login_gift_fetch_vip_reward_flag & (1 << seq)))
					{
						continue;
					}

					const RandActivityLoginGiftCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetLoginGiftRewardCfg(seq);
					if (NULL == cfg_item)
					{
						break;
					}

					if (m_activity_info.ra_login_gift_login_days < cfg_item->need_login_days)
					{
						break;
					}

					const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->vip_reward_item.item_id);
					if (NULL != reward_item_base)
					{
						static MailContentParam contentparam; contentparam.Reset();

						contentparam.item_list[0].item_id = cfg_item->vip_reward_item.item_id;
						contentparam.item_list[0].num = cfg_item->vip_reward_item.num;
						contentparam.item_list[0].is_bind = (cfg_item->vip_reward_item.is_bind) ? 1 : 0;
						contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

						int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_login_gift_reward_content, 
							m_activity_info.ra_login_gift_login_days);
						if (length > 0)
						{

							m_activity_info.ra_login_gift_fetch_vip_reward_flag |= (1 <<seq);

							MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
						}
					}
				}
			}
		}
	}
}

void RoleActivity::RACheckLoginGiftGiveReward_0()
{
	if (m_activity_info.ra_login_gift_login_days_0 > 0)
	{
		// 普通奖励
		UNSTD_STATIC_CHECK(RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_login_gift_fetch_common_reward_flag_0) * 8);

		for (int seq = 0; seq < RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT; ++seq)
		{
			if (0 != (m_activity_info.ra_login_gift_fetch_common_reward_flag_0 & (1 << seq)))
			{
				continue;
			}

			const RandActivityLoginGiftCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetLoginGiftRewardCfg_0(seq);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_activity_info.ra_login_gift_login_days_0 < cfg_item->need_login_days)
			{
				break;
			}

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->common_reward_item.item_id);
			if (NULL != reward_item_base)
			{
				static MailContentParam contentparam; contentparam.Reset();

				contentparam.item_list[0].item_id = cfg_item->common_reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->common_reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->common_reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_login_gift_reward_content,
					m_activity_info.ra_login_gift_login_days_0);
				if (length > 0)
				{

					m_activity_info.ra_login_gift_fetch_common_reward_flag_0 |= (1 << seq);

					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
	}
}

void RoleActivity::RACheckStoneTotalLevelGiveReward()
{
	if (m_activity_info.ra_stone_uplevel_fetch_reward_flag == m_activity_info.ra_stone_uplevel_can_reward_flag)
	{
		return;
	}

	int reward_count = LOGIC_CONFIG->GetRandActivityCfg().GetStoneTotalLevelRewardCfgCount();
	for (int i = 0; i < reward_count; ++ i)
	{
		UNSTD_STATIC_CHECK(RAND_ACTIVITY_STONE_UPLEVEL_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_stone_uplevel_can_reward_flag) * 8);
		UNSTD_STATIC_CHECK(RAND_ACTIVITY_STONE_UPLEVEL_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_stone_uplevel_fetch_reward_flag) * 8);

		if (0 != (m_activity_info.ra_stone_uplevel_can_reward_flag & (LL_ONE_BIT << i)) && 
			0 == (m_activity_info.ra_stone_uplevel_fetch_reward_flag & (LL_ONE_BIT << i)))
		{
			const RandActivityStoneUplevelCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetStoneUplevelCfgBySeq(i);
			if (NULL != cfg_item)
			{
				const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
				if (NULL != reward_item_base)
				{
					static MailContentParam contentparam; contentparam.Reset();

					contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
					contentparam.item_list[0].num = cfg_item->reward_item.num;
					contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
					contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_stone_total_level_reward_content, 
						cfg_item->need_total_stone_level);
					if (length > 0)
					{
						m_activity_info.ra_stone_uplevel_fetch_reward_flag |= (LL_ONE_BIT << i);

						MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
					}
				}
			}
		}
	}

	m_activity_info.ra_stone_uplevel_can_reward_flag = 0;
	this->SendRAStoneUplevelInfo();
}

void RoleActivity::RACheckMountUpgradeGiveReward()
{
	if (m_activity_info.ra_mount_upgrade_fetch_reward_flag == m_activity_info.ra_mount_upgrade_can_reward_flag)
	{
		return;
	}
	
	int skill_level = m_role->GetMountManager()->GetMedalSkillLevel();
	int reward_count = LOGIC_CONFIG->GetRandActivityCfg().GetMountUpgradeRewardCfgCount();
	for (int i = 0; i < reward_count; ++ i)
	{
		UNSTD_STATIC_CHECK(RAND_ACTIVITY_MOUNT_UPGRADE_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_mount_upgrade_can_reward_flag) * 8);
		UNSTD_STATIC_CHECK(RAND_ACTIVITY_MOUNT_UPGRADE_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_mount_upgrade_fetch_reward_flag) * 8);

		if (0 != (m_activity_info.ra_mount_upgrade_can_reward_flag & (1 << i)) && 
			0 == (m_activity_info.ra_mount_upgrade_fetch_reward_flag & (1 << i)))
		{
			const RandActivityMountUpgradeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetMountUpgradeCfgBySeq(i);
			if (NULL != cfg_item)
			{
				const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
				if (NULL != reward_item_base)
				{
					static MailContentParam contentparam; contentparam.Reset();

					contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
					contentparam.item_list[0].num = cfg_item->reward_item.num;
					contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
					contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_mount_upgrade_reward_content, cfg_item->need_mount_grade, skill_level);
					if (length > 0)
					{
						m_activity_info.ra_mount_upgrade_fetch_reward_flag |= (1 << i);

						MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
					}
				}
			}
		}
	}

	m_activity_info.ra_mount_upgrade_can_reward_flag = 0;
	this->SendRAMountUpgradeInfo();
}

void RoleActivity::RACheckMentalityTotalLevelGiveReward()
{
	if (m_activity_info.ra_mentality_total_level_fetch_reward_flag == m_activity_info.ra_mentality_total_level_can_reward_flag)
	{
		return;
	}

	int reward_count = LOGIC_CONFIG->GetRandActivityCfg().GetMentalityTotalLevelRewardCfgCount();
	for (int i = 0; i < reward_count; ++ i)
	{
		UNSTD_STATIC_CHECK(RAND_ACTIVITY_MENTALITY_TOTAL_LEVEL_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_mentality_total_level_can_reward_flag) * 8);
		UNSTD_STATIC_CHECK(RAND_ACTIVITY_MENTALITY_TOTAL_LEVEL_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_mentality_total_level_fetch_reward_flag) * 8);

		if (0 != (m_activity_info.ra_mentality_total_level_can_reward_flag & (1 << i)) && 
			0 == (m_activity_info.ra_mentality_total_level_fetch_reward_flag & (1 << i)))
		{
			const RandActivityMentalityTotalLevelCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetMentalityTotalLevelCfgBySeq(i);
			if (NULL != cfg_item)
			{
				const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
				if (NULL != reward_item_base)
				{
					static MailContentParam contentparam; contentparam.Reset();

					contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
					contentparam.item_list[0].num = cfg_item->reward_item.num;
					contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
					contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_mentality_total_level_reward_content, 
						cfg_item->need_mentality_total_level);
					if (length > 0)
					{
						m_activity_info.ra_mentality_total_level_fetch_reward_flag |= (1 << i);

						MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
					}
				}
			}
		}
	}

	m_activity_info.ra_mentality_total_level_can_reward_flag = 0;
	this->SendRAMentalityTotalLevelInfo();
}

void RoleActivity::RACheckWingUpgradeGiveReward()
{
	if (m_activity_info.ra_wing_upgrade_fetch_reward_flag == m_activity_info.ra_wing_upgrade_can_reward_flag)
	{
		return;
	}

	int skill_level = m_role->GetWing()->GetMedalSkillLevel();
	int reward_count = LOGIC_CONFIG->GetRandActivityCfg().GetWingUpgradeRewardCfgCount();
	for (int i = 0; i < reward_count; ++ i)
	{
		UNSTD_STATIC_CHECK(RAND_ACTIVITY_WING_UPGRADE_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_wing_upgrade_can_reward_flag) * 8);
		UNSTD_STATIC_CHECK(RAND_ACTIVITY_WING_UPGRADE_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_wing_upgrade_fetch_reward_flag) * 8);

		if (0 != (m_activity_info.ra_wing_upgrade_can_reward_flag & (LL_ONE_BIT << i)) && 
			0 == (m_activity_info.ra_wing_upgrade_fetch_reward_flag & (LL_ONE_BIT << i)))
		{
			const RandActivityWingUpgradeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetWingUpgradeCfgBySeq(i);
			if (NULL != cfg_item)
			{
				const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
				if (NULL != reward_item_base)
				{
					static MailContentParam contentparam; contentparam.Reset();

					contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
					contentparam.item_list[0].num = cfg_item->reward_item.num;
					contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
					contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_wing_upgrade_reward_content, cfg_item->need_wing_grade, skill_level);
					if (length > 0)
					{
						m_activity_info.ra_wing_upgrade_fetch_reward_flag |= (LL_ONE_BIT << i);

						MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
					}
				}
			}
		}
	}

	m_activity_info.ra_wing_upgrade_can_reward_flag = 0;
	this->SendRAWingUpgradeInfo();
}

void RoleActivity::RACheckQuanminQifuGiveReward()
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_VIP_QIFU_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_quanmin_qifu_fetch_reward_flag) * 8);

	if (m_activity_info.ra_quanmin_qifu_qifu_times > 0)
	{
		int reward_count = LOGIC_CONFIG->GetRandActivityCfg().GetVipQifuRewardCfgCount();
		for (int i = 0; i < reward_count; ++ i)
		{
			if (0 == (m_activity_info.ra_quanmin_qifu_fetch_reward_flag & (1 << i)))
			{
				const RandActivityQuanminQifuCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetQuanminQifuCfgBySeq(i);
				if (NULL != cfg_item && m_activity_info.ra_quanmin_qifu_qifu_times >= cfg_item->need_qifu_times)
				{
					const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
					if (NULL != reward_item_base)
					{
						static MailContentParam contentparam; contentparam.Reset();

						contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
						contentparam.item_list[0].num = cfg_item->reward_item.num;
						contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
						contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

						int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_quanmin_qifu_reward_content, 
							cfg_item->need_qifu_times);
						if (length > 0)
						{
							m_activity_info.ra_chestshop_fetch_reward_flag |= (1 << i);

							MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
						}
					}
				}
			}
		}

		m_activity_info.ra_quanmin_qifu_qifu_times = 0;
		m_activity_info.ra_quanmin_qifu_fetch_reward_flag = 0;
		this->SendRAQuanminQifuInfo();
	}
}

void RoleActivity::RACheckBipinCapShenzhuangGiveReweard()
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_shenzhuang_fetch_reward_flag) * 8);

	for (int i = 0; i < RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 == (m_activity_info.ra_bipin_cap_shenzhuang_fetch_reward_flag & (1 << i))
			&& m_activity_info.ra_bipin_cap_shenzhuang_history_max_value > 0)
		{
			const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG, i);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENZHUANG) < cfg_item->need_capability)
			{
				break;
			}

			static MailContentParam contentparam;
			contentparam.Reset();
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, cfg_item->reward_gold_bind);

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 
			}	

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_all_reward_content, RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG);
			if (length > 0)
			{
				m_activity_info.ra_bipin_cap_shenzhuang_fetch_reward_flag |= (1 << i);

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckBipinCapShenzhuangGiveReweard::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
					m_role->GetUID(), m_role->GetName(), i, m_activity_info.ra_bipin_cap_shenzhuang_fetch_reward_flag, 
					cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
			}
		}
	}

	m_activity_info.ra_bipin_cap_shenzhuang_history_max_value = 0;
}

void RoleActivity::RACheckBipinCapJinglingGiveReweard()
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_jingling_fetch_reward_flag) * 8);

	for (int i = 0; i < RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 == (m_activity_info.ra_bipin_cap_jingling_fetch_reward_flag & (1 << i)) 
			&& m_activity_info.ra_bipin_cap_jingling_history_max_value > 0)
		{
			const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING, i);
			if (NULL == cfg_item)
			{
				break;
			}

			if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING) < cfg_item->need_capability)
			{
				break;
			}

			static MailContentParam contentparam;
			contentparam.Reset();
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, cfg_item->reward_gold_bind);

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 
			}	

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_all_reward_content, RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING);
			if (length > 0)
			{
				m_activity_info.ra_bipin_cap_jingling_fetch_reward_flag |= (1 << i);

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckBipinCapJinglingGiveReweard::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
					m_role->GetUID(), m_role->GetName(), i, m_activity_info.ra_bipin_cap_jingling_fetch_reward_flag, 
					cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
			}
		}
	}

	m_activity_info.ra_bipin_cap_jingling_history_max_value = 0;
}

void RoleActivity::RACheckBipinCapWashGiveReweard()
{
	UNSTD_STATIC_CHECK(RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT <= (int)sizeof(m_activity_info.ra_bipin_cap_wash_fetch_reward_flag) * 8);

	for (int i = 0; i < RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT; ++ i)
	{
		if (0 == (m_activity_info.ra_bipin_cap_wash_fetch_reward_flag & (1 << i)) 
			&& m_activity_info.ra_bipin_cap_wash_history_max_value > 0)
		{
			const RandActivityAllBipinCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetAllBipinCfg(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH, i);
			if (NULL == cfg_item)
			{
				break;
			}

			const int rank_val1 = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_MENTALITY);
			const int rank_val2 = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_GEM);
			const int rank_val3 = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_WASH);

			if ((rank_val1 + rank_val2 +rank_val3) < cfg_item->need_capability)
			{
				break;
			}

			static MailContentParam contentparam;
			contentparam.Reset();

			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, cfg_item->reward_gold_bind);

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg_item->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				contentparam.item_list[0].item_id = cfg_item->reward_item.item_id;
				contentparam.item_list[0].num = cfg_item->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg_item->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 
			}	

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_bipin_all_reward_content, RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH);
			if (length > 0)
			{
				m_activity_info.ra_bipin_cap_wash_fetch_reward_flag |= (1 << i);

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckBipinCapWashGiveReweard::user[%d, %s], seq[%d], reward_flag[%d], gold_bind[%d], reward_item[%s]",
					m_role->GetUID(), m_role->GetName(), i, m_activity_info.ra_bipin_cap_wash_fetch_reward_flag, 
					cfg_item->reward_gold_bind, ItemConfigDataLog(&cfg_item->reward_item, NULL));
			}
		}
	}

	m_activity_info.ra_bipin_cap_wash_history_max_value = 0;
}

void RoleActivity::RACheckTotalCharge3RewardCheck()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3))
	{
		return;
	}

	int day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3), EngineAdapter::Instance().Time());
	int count = LOGIC_CONFIG->GetRandActivityCfg().GetTotalCharge3CfgCount(day_index);

	for (int i = 0; i < count; ++ i)
	{
		const RandActivityTotalCharge3Cfg::ConfigItem *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetTotalCharge3Cfg(day_index, i);
		if (NULL == cfg)
		{
			continue;
		}

		if (m_activity_info.ra_total_charge_3_cur_total_charge < cfg->need_chongzhi_num)
		{
			continue;
		}

		if (0 != (m_activity_info.ra_total_charge_3_cur_has_fetch_flag & (1 << cfg->seq)))
		{
			continue;
		}

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

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_charge3_reward_content, cfg->need_chongzhi_num);
		if (length > 0)
		{
			// 消耗
			m_activity_info.ra_total_charge_3_cur_has_fetch_flag |= (1 << cfg->seq);

			// 给予
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_rand_activity.printf(LL_INFO, "RoleActivity::RACheckTotalCharge3RewardCheck user[%d, %s] chongzhi_num[%d], reward_flag[%d], reward_item_1[%s] reward_item_2[%s] reward_item_3[%s]",
				m_role->GetUID(), m_role->GetName(), cfg->need_chongzhi_num, m_activity_info.ra_total_charge_3_cur_has_fetch_flag, ItemConfigDataLog(&cfg->reward_item[0], NULL), 
				ItemConfigDataLog(&cfg->reward_item[1], NULL), ItemConfigDataLog(&cfg->reward_item[2], NULL));
		}
	}
}

void RoleActivity::RACheckRoleUplevelGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_ROLE_UPLEVEL);
}

void RoleActivity::RACheckPataGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_PATA);
}

void RoleActivity::RACheckExpFbGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_EXP_FB);
}

//void RoleActivity::RACheckUpgradeMountGiveReweard()
//{
//	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_MOUNT);
//}

//void RoleActivity::RACheckUpgradeHaloGiveReweard()
//{
//	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_HALO);
//}

void RoleActivity::RACheckUpgradeWingGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_WING);
}

void RoleActivity::RACheckUpgradeShengongGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG);
}

void RoleActivity::RACheckUpgradeShenyiGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_SHENYI);
}

void RoleActivity::RACheckFirstChargeTuanUpgradeGiveReweard()
{
//	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN);
}

void RoleActivity::RACheckDayTotalChargeUpgradeGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_DAY_TOTAL_CHARGE);
}

//void RoleActivity::RACheckMountUpgradeTotalGiveReweard()
//{
//	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_TOTAL);
//}
//
//void RoleActivity::RACheckHaloUpgradeTotalGiveReweard()
//{
//	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_HALO_TOTAL);
//}

void RoleActivity::RACheckWingUpgradeTotalGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_WING_TOTAL);
}

void RoleActivity::RACheckShengongUpgradeTotalGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_TOTAL);
}

void RoleActivity::RACheckShenyiUpgradeTotalGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_TOTAL);
}

void RoleActivity::RACheckMountUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK);
}

void RoleActivity::RACheckQilinbiUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK);
}

void RoleActivity::RACheckToushiUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK);
}

void RoleActivity::RACheckYaoshiUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK);
}

//新增时装、神兵、法宝、足迹
void RoleActivity::RACheckShizhuangUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK);
}

void RoleActivity::RACheckShenbinUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK);
}

void RoleActivity::RACheckFabaoUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK);
}

void RoleActivity::RACheckFootprintUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK);
}


void RoleActivity::RACheckHaloUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK);
}

void RoleActivity::RACheckWingUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK);
}

void RoleActivity::RACheckShengongUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK);
}

void RoleActivity::RACheckShenyiUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK);
}

void RoleActivity::RACheckLingtongUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK);
}

void RoleActivity::RACheckLinggongUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK);
}

void RoleActivity::RACheckLingqiUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK);
}

void RoleActivity::RACheckFlypetUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK);
}

void RoleActivity::RACheckWeiyanUpgradeRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK);
}

void RoleActivity::RACheckEquipStrengthTotalLevelGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL);
}

void RoleActivity::RACheckStoneTotalLevelGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL);
}

void RoleActivity::RACheckEquipStrengthTotalLevelRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK);
}

void RoleActivity::RACheckStoneTotalLevelRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK);
}

void RoleActivity::RACheckBossKillerGiveReweard()
{
	this->RACheckOpenServerBossKillerGiveReweard(RAND_ACTIVITY_TYPE_BOSS_KILLER);
}

void RoleActivity::RACheckXianNvTotalRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_XIANNV_RANK);
}

void RoleActivity::RACheckJingLingTotalRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_JINGLING_RANK);
}

void RoleActivity::RACheckFightMountTotalRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK);
}

void RoleActivity::RACheckRoleTotalCapRankGiveReweard()
{
	this->RACheckOpenServerGiveReweard(RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK);
}

void RoleActivity::RACheckXianyuanTreasMailReward()
{
	int count = LOGIC_CONFIG->GetRandActivityCfg().GetXianyuanTreasCount();
	for (int i = 0; i < count && i < RAND_ACTIVITY_XIANYUAN_TREAS_COUNT; ++i)
	{
		if (m_activity_info.ra_xianyuan_treas_list[i].buy_day_index <= 0)
		{
			continue;
		}

		int act_last_days = LOGIC_CONFIG->GetRandActivityCfg().GetActivityLastedDays(RAND_ACTIVITY_TYPE_XIANYUAN_TREAS);
		//if (m_activity_info.ra_xianyuan_treas_list[i].num >= (act_last_days - m_activity_info.ra_xianyuan_treas_list[i].buy_day_index))
		//{
		//	continue;
		//}

		const RandActivityXianyuanTreasConfig *xianyuan_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetXianyuanTreasCfg(i);
		if (NULL == xianyuan_cfg)
		{
			continue;
		}

		if (m_activity_info.ra_xianyuan_treas_list[i].num >= xianyuan_cfg->max_reward_day)
		{
			continue;
		}

		static MailContentParam contentparam; contentparam.Reset();

		int reward_num = xianyuan_cfg->max_reward_day - m_activity_info.ra_xianyuan_treas_list[i].num;
		for (int num = 0; num < reward_num && num < MAX_ATTACHMENT_ITEM_NUM; ++num)
		{
			contentparam.item_list[num].item_id = xianyuan_cfg->reward_item.item_id;
			contentparam.item_list[num].num = xianyuan_cfg->reward_item.num;
			contentparam.item_list[num].is_bind = (xianyuan_cfg->reward_item.is_bind) ? 1 : 0;
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_xianyuan_treas_reward_content,
			/*xianyuan_cfg->seq*/RAND_ACTIVITY_TYPE_XIANYUAN_TREAS);

		if (length > 0)
		{
			m_activity_info.ra_xianyuan_treas_list[i].num += reward_num;

			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::RaCheckXianyuanTReasMailReward user[%d, %s], seq[%d], act_last_days[%d], num[%d], item_id[%d]",
				m_role->GetUID(), m_role->GetName(), i, act_last_days, m_activity_info.ra_xianyuan_treas_list[i].num, xianyuan_cfg->reward_item.item_id);

		}
	}

	this->SendRaXianyuanTreasInfo();
}

void RoleActivity::RACheckKingdrawReturnReward()
{
	int join_day = GetDayIndex(m_activity_info.ra_kingdraw_join_timestamp, EngineAdapter::Instance().Time()) + 1;

	for (int level = 0; level < RA_KING_DRAW_LEVEL_COUNT; ++level)
	{
		for (int seq = 0; seq < LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawReturnRewardCfgCount(level) && seq < RA_KING_DRAW_MAX_RETURN_REWARD; ++seq)
		{
			const RandActivityKingDrawReturnRewardStageCfg *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawReturnRewardCfg(level, seq, join_day);
			if (NULL == cfg)
			{
				continue;
			}

			int flag_seq = seq + level * RandActivityKingDrawReturnRewardCfg::KING_DRAW_RETURN_REWARD_COUNT_PER_STAGE;
			if (static_cast<int>(m_activity_info.ra_kingdraw_draw_times[level]) >= cfg->draw_times && (0 == (m_activity_info.ra_kingdraw_reward_flag & (1 << flag_seq))))
			{
				static MailContentParam contentparam; contentparam.Reset();

				contentparam.item_list[0].item_id = cfg->reward_item.item_id;
				contentparam.item_list[0].num = cfg->reward_item.num;
				contentparam.item_list[0].is_bind = (cfg->reward_item.is_bind) ? 1 : 0;

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_king_draw_return_reward_content, cfg->draw_times);
				if (length > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

					m_activity_info.ra_kingdraw_reward_flag |= (1 << flag_seq);
				}
			}
		}
	}
}

void RoleActivity::RACheckLevelLotteryReward()
{
	int join_day = GetDayIndex(m_activity_info.ra_level_lottery_timestamp, EngineAdapter::Instance().Time()) + 1;
	const std::map<int, RandActivityLevelLotteryTotalTimesCfg > &total_times_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLevelLotteryOpengameDayRewardCfg(join_day);
	if (total_times_cfg.empty())
	{
		return;
	}

	for (std::map<int, RandActivityLevelLotteryTotalTimesCfg>::const_iterator iter = total_times_cfg.begin(); 
		iter != total_times_cfg.end(); ++iter)
	{
		if (m_activity_info.ra_lottery_buy_total_times < iter->second.total_times)
		{
			continue;
		}

		if (0 != (m_activity_info.ra_lottery_fetch_reward_flag & (1 << iter->second.reward_index)))
		{
			continue;
		}

		if (m_role->GetVip()->GetVipLevel() < iter->second.reward_index)
		{
			continue;
		}

		m_activity_info.ra_lottery_fetch_reward_flag |= (1 << iter->second.reward_index);

		static MailContentParam contentparam; contentparam.Reset();

		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			contentparam.item_list[i].item_id = iter->second.reward[i].item_id;
			contentparam.item_list[i].num = iter->second.reward[i].num;
			contentparam.item_list[i].is_bind = iter->second.reward[i].is_bind;
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward, RAND_ACTIVITY_TYPE_LEVEL_LOTTERY);

		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}
}

void RoleActivity::RACheckZhenbaogeReward()
{
	for (int seq = 0; seq < RAND_ACTIVITY_ZHENBAOGE_SERVER_MAX_SEQ && seq < 16; ++seq)
	{
		const RandActivityZhenBaoGeServerConfig *server_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeServerCfg(seq);
		if (NULL == server_cfg)
		{
			continue;
		}

		if (0 != (m_activity_info.ra_zhenbaoge_refresh_gift_fetch_flag & (1 << seq)))
		{
			continue;
		}

		int cur_times = m_activity_info.ra_zhenbaoge_refresh_times;
		if (cur_times < server_cfg->can_fetch_times)
		{
			continue;
		}

		if (m_role->GetVip()->GetVipLevel() < server_cfg->vip_limit)
		{
			continue;
		}

		static MailContentParam contentparam; contentparam.Reset();

		contentparam.item_list[0].item_id = server_cfg->reward_item.item_id;
		contentparam.item_list[0].num = server_cfg->reward_item.num;
		contentparam.item_list[0].is_bind = server_cfg->reward_item.is_bind;

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward, RAND_ACTIVITY_TYPE_ZHENBAOGE);

		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam); 
			m_activity_info.ra_zhenbaoge_refresh_gift_fetch_flag |= 1 << seq;
		}
	}
}

void RoleActivity::RACheckMoneyTreeReward()
{
	int join_day = GetDayIndex(m_activity_info.ra_money_tree_last_join_timestamp, EngineAdapter::Instance().Time()) + 1;
	for (int seq = 0; seq < RA_MONEY_TREE_SERVER_REWARD_CFG_COUNT && seq < 32; ++seq)
	{
		const RandActivityMoneyTreeServerRewardConfig *cur_server_money_tree_cfg 
			= LOGIC_CONFIG->GetRandActivityCfg().GetTreeServerCfgWithServerMoneyTreeTimes(m_activity_info.ra_money_tree_total_times, join_day);
		if (NULL == cur_server_money_tree_cfg)
		{
			continue;
		}

		if (seq > cur_server_money_tree_cfg->seq)
		{
			continue;
		}

		if ((m_activity_info.ra_money_tree_server_reward_has_fetch_reward_flag & (1 << seq)) > 0)
		{
			continue;
		}

		if (!m_role->GetKnapsack()->HaveSpace())
		{
			continue;
		}

		const RandActivityMoneyTreeServerRewardConfig *cur_tree_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetMoneyTreeServerCfgWithReq(seq, join_day);
		if (NULL == cur_tree_cfg)
		{
			continue;
		}

		if (m_role->GetVip()->GetVipLevel() < cur_tree_cfg->vip_limit)
		{
			continue;
		}

		static MailContentParam contentparam; contentparam.Reset();

		contentparam.item_list[0].item_id = cur_tree_cfg->reward_item.item_id;
		contentparam.item_list[0].num = cur_tree_cfg->reward_item.num;
		contentparam.item_list[0].is_bind = cur_tree_cfg->reward_item.is_bind;

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward, RAND_ACTIVITY_TYPE_MONEY_TREE);

		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			m_activity_info.ra_money_tree_server_reward_has_fetch_reward_flag |= (1 << seq);
		}
	}
}

void RoleActivity::RACheckConsumGiftReward()
{
	if (0 == m_activity_info.ra_consume_gift_consume_gold)
	{
		return;
	}

	int last_opengame_day = m_activity_info.ra_consume_gift_join_opengame_day;
	int act_theme = m_activity_info.ra_consume_gift_join_theme;     //补发上次活动主题的物品
	ItemConfigData reward_list[RAND_ACTIVITY_CONSUM_GIFT_MAX_REWARD_COUNT * RAND_ACTIVITY_CONSUM_GIFT_MAX_SEQ];
	int reward_count = 0;

	for (int i = 0; i < RAND_ACTIVITY_CONSUM_GIFT_MAX_SEQ; ++i)
	{
		if (0 != (m_activity_info.ra_consume_gift_reward_flag & (1 << i)))
		{
			continue;
		}

		const ConsumGiftRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetConsumGiftRewardCfg(last_opengame_day, act_theme, i);
		if (reward_cfg == nullptr || reward_cfg->item_count <= 0 || m_activity_info.ra_consume_gift_consume_gold < reward_cfg->need_gold)
		{
			continue;
		}

		for (int j = 0; j < reward_cfg->item_count && j < RAND_ACTIVITY_CONSUM_GIFT_MAX_REWARD_COUNT; j++)
		{
			reward_list[reward_count].item_id = reward_cfg->reward_item_list[j].item_id;
			reward_list[reward_count].num = reward_cfg->reward_item_list[j].num;
			reward_list[reward_count].is_bind = reward_cfg->reward_item_list[j].is_bind;
			++reward_count;
		}
	}
	
	if (reward_count > 0)
	{
		gamecommon::SendMultiMailToRole(m_role->GetUserId(), reward_list, reward_count, NULL, true, gamestring::g_rand_activity_check_reward_common_mail_content, RAND_ACTIVITY_TYPE_CONSUM_GIFT);
	}

	m_activity_info.ra_consume_gift_consume_gold = 0;
	m_activity_info.ra_consume_gift_today_join_times = 0;
	m_activity_info.ra_consume_gift_reward_flag = 0;
	m_activity_info.ra_consume_gift_join_theme = 0;
	m_activity_info.ra_consume_gift_join_opengame_day = 0;
}

void RoleActivity::RACheckHappyCumulChongzhiReward()
{
	if (m_activity_info.ra_happy_cumul_chongzhi_num <= 0)
	{
		return;
	}

	int act_theme = m_activity_info.ra_happy_cumul_chongzhi_join_theme;
	ItemConfigData reward_list[RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_REWARD_COUNT * RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_CFG];
	int reward_count = 0;

	for (int i = 0; i < RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_CFG; ++i)
	{
		if (0 != (m_activity_info.ra_happy_cumul_chongzhi_fetch_flag & (1 << i)))
		{
			continue;
		}

		const HappyCumulChongzhiConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetHappyCumulChongzhiCfg(act_theme, i);
		if (reward_cfg == nullptr || reward_cfg->reward_count <= 0 || m_activity_info.ra_happy_cumul_chongzhi_num < reward_cfg->need_chongzhi)
		{
			continue;
		}

		for (int j = 0; j < reward_cfg->reward_count && j < RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_REWARD_COUNT; j++)
		{
			reward_list[reward_count].item_id = reward_cfg->reward_item_list[j].item_id;
			reward_list[reward_count].num = reward_cfg->reward_item_list[j].num;
			reward_list[reward_count].is_bind = reward_cfg->reward_item_list[j].is_bind;
			++reward_count;
		}
	}

	if (reward_count > 0)
	{
		gamecommon::SendMultiMailToRole(m_role->GetUserId(), reward_list, reward_count, NULL, true, gamestring::g_rand_activity_check_reward_common_mail_content, RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE);
	}

	m_activity_info.ra_happy_cumul_chongzhi_num = 0;
	m_activity_info.ra_happy_cumul_chongzhi_fetch_flag = 0;
	m_activity_info.ra_happy_cumul_chongzhi_join_theme = 0;
}

void RoleActivity::ReissueMiJingXunBao3FetchPersonalReward()
{

	int total_reward_num = LOGIC_CONFIG->GetRandActivityCfg().GetMiJingXunBao3TotalRewardNum();
	for (int i = 0; i < total_reward_num; ++i)
	{
		if (0 != (m_activity_info.ra_mijingxunbao3_personal_reward_flag & (1 << i)))
		{
			continue;
		}
		else
		{
			const RandActivityMiJingXunBaoRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityMiJingXunBao3PersonalRewardCfg(i, m_activity_info.ra_mijingxunbao3_role_chou_times);
			if (nullptr == reward_cfg)
			{
				return;
			}

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				static MailContentParam contentparam; contentparam.Reset();

				contentparam.item_list[0].item_id = reward_cfg->reward_item.item_id;
				contentparam.item_list[0].num = reward_cfg->reward_item.num;
				contentparam.item_list[0].is_bind = (reward_cfg->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward_common_mail_content, RAND_ACTIVITY_TYPE_MIJINGXUNBAO3);
				if (length > 0)
				{
					m_activity_info.ra_mijingxunbao3_personal_reward_flag |= (1 << i);

					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
	}

	m_activity_info.ra_mijingxunbao3_role_chou_times = 0;
	m_activity_info.ra_mijingxunbao3_personal_reward_flag = 0;

	this->SendRAMiJingXunBao3Info();
}

void RoleActivity::RACheckLimitTimeRebateReward()
{
	const RandActivityLimitTimeRebateCfg *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLimitTimeRebateCfg();
	if (NULL != cfg)
	{
		for (int cfg_index = 0; cfg_index < cfg->cfg_count && cfg_index < RA_LIMIT_TIME_REBATE_MAX_CFG_COUNT; cfg_index++)
		{
			if (0 != (m_activity_info.ra_limit_time_rebate_reward_flag & (1 << cfg_index)))
			{
				continue;
			}

			int satisfied_day = 0;
			for (int day = 0; day < m_activity_info.ra_limit_time_rebate_total_chongzhi_days && day < RA_LIMIT_TIME_REBATE_MAX_DAY; day++)
			{
				if (m_activity_info.ra_limit_time_rebate_chongzhi_list[day] >= cfg->cfg_list[cfg_index].chongzhi_cout)
				{
					satisfied_day++;
				}
			}

			if (cfg->cfg_list[cfg_index].need_day <= satisfied_day)
			{
				static MailContentParam contentparam; contentparam.Reset();

				const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg->cfg_list[cfg_index].reward_item.item_id);
				if (NULL == reward_item_base)
				{
					continue;;
				}

				contentparam.item_list[0].item_id = cfg->cfg_list[cfg_index].reward_item.item_id;
				contentparam.item_list[0].num = cfg->cfg_list[cfg_index].reward_item.num;
				contentparam.item_list[0].is_bind = cfg->cfg_list[cfg_index].reward_item.is_bind ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward, RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE);
				if (length > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

					gamelog::g_log_roleactivity.printf(LL_INFO, "RACheckLimitTimeRebateReward::user[%d %s], reward[%s]", m_role->GetUID(), m_role->GetName(),
						ItemConfigDataLog(&cfg->cfg_list[cfg_index].reward_item, NULL));
				}
			}

			m_activity_info.ra_limit_time_rebate_reward_flag |= (1 << cfg_index);
		}
	}

	m_activity_info.ra_limit_time_rebate_cur_day_is_chongzhi = 0;
	m_activity_info.ra_limit_time_rebate_total_chongzhi_days = 0;
	m_activity_info.ra_limit_time_rebate_reward_flag = 0;
	m_activity_info.ra_limit_time_rebate_cur_day_chongzhi = 0;
	memset(m_activity_info.ra_limit_time_rebate_chongzhi_list, 0, sizeof(m_activity_info.ra_limit_time_rebate_chongzhi_list));

	this->SendLimitTimeRebateInfo();
}

void RoleActivity::RACheckTimeLimitGiftReward()
{
	const int vip_level = m_activity_info.ra_time_limit_gift_join_vip_level;
	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), m_activity_info.ra_time_limit_gift_begin_timestamp) + 1;
	long long old_recharge_gold = this->GetHistoryChongZhi() - this->GetDayChongzhi();	//除了今日充值的元宝数
	const RandActivityLimitTimeGiftCfg::SubItemCfg *tlg_sub_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetLimitTimeGiftRewardCfg(opengame_day, vip_level, old_recharge_gold);
	if (nullptr != tlg_sub_cfg)
	{
		if (0 != (m_activity_info.ra_time_limit_gift_reward_can_fetch_flag1 & (1 << tlg_sub_cfg->seq))
			&& 0 == (m_activity_info.ra_time_limit_gift_reward_has_fetch_flag1 & (1 << tlg_sub_cfg->seq)))
		{
			m_activity_info.ra_time_limit_gift_reward_has_fetch_flag1 |= (1 << tlg_sub_cfg->seq);
			m_activity_info.ra_time_limit_gift_reward_can_fetch_flag1 &= ~(1 << tlg_sub_cfg->seq);

			MailContentParam contentparam; contentparam.Reset();
			for (int item_count = 0; item_count < tlg_sub_cfg->item_count1 && item_count < MAX_ATTACHMENT_ITEM_NUM; ++item_count)
			{
				contentparam.AddItem(tlg_sub_cfg->reward_item_list1[item_count]);
			}

			int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_time_limit_gift_reward_mail);

			if (length_1 > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		if (0 != (m_activity_info.ra_time_limit_gift_reward_can_fetch_flag2 & (1 << tlg_sub_cfg->seq))
			&& 0 == (m_activity_info.ra_time_limit_gift_reward_has_fetch_flag2 & (1 << tlg_sub_cfg->seq)))
		{
			m_activity_info.ra_time_limit_gift_reward_has_fetch_flag2 |= (1 << tlg_sub_cfg->seq);
			m_activity_info.ra_time_limit_gift_reward_can_fetch_flag2 &= ~(1 << tlg_sub_cfg->seq);

			MailContentParam contentparam; contentparam.Reset();
			for (int item_count = 0; item_count < tlg_sub_cfg->item_count2 && item_count < MAX_ATTACHMENT_ITEM_NUM; ++item_count)
			{
				contentparam.AddItem(tlg_sub_cfg->reward_item_list2[item_count]);
			}

			int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_time_limit_gift_reward_mail);

			if (length_1 > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		//if (0 != (m_activity_info.ra_time_limit_gift_reward_can_fetch_flag3 & (1 << tlg_sub_cfg->seq))
		//	&& 0 == (m_activity_info.ra_time_limit_gift_reward_has_fetch_flag3 & (1 << tlg_sub_cfg->seq)))
		//{
		//	m_activity_info.ra_time_limit_gift_reward_has_fetch_flag3 |= (1 << tlg_sub_cfg->seq);
		//	m_activity_info.ra_time_limit_gift_reward_can_fetch_flag3 &= ~(1 << tlg_sub_cfg->seq);

		//	MailContentParam contentparam; contentparam.Reset();
		//	for (int item_count = 0; item_count < tlg_sub_cfg->item_count3 && item_count < MAX_ATTACHMENT_ITEM_NUM; ++item_count)
		//	{
		//		contentparam.AddItem(tlg_sub_cfg->reward_item_list3[item_count]);
		//	}

		//	int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_time_limit_gift_reward_mail);

		//	if (length_1 > 0)
		//	{
		//		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		//	}
		//}
	}

	m_activity_info.ra_time_limit_gift_join_vip_level = m_role->GetVip()->GetVipLevel();
	m_activity_info.ra_time_limit_gift_open_flag = 0;
	m_activity_info.ra_time_limit_gift_reward_can_fetch_flag1 = 0;
	m_activity_info.ra_time_limit_gift_reward_has_fetch_flag1 = 0;
	m_activity_info.ra_time_limit_gift_reward_can_fetch_flag2 = 0;
	m_activity_info.ra_time_limit_gift_reward_has_fetch_flag2 = 0;
	m_activity_info.ra_time_limit_gift_reward_can_fetch_flag3 = 0;
	m_activity_info.ra_time_limit_gift_reward_has_fetch_flag3 = 0;

	this->SendRATimeLimitGiftInfo();
}

void RoleActivity::RACheckCirculationChongzhiSecondReward()
{
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
	if (nullptr == item_base || item_base->GetPileLimit() <= 0)
	{
		return;
	}

	// 消耗
	m_activity_info.ra_circulation_chongzhi_2_cur_chongzhi_gold -= need_chongzhi * can_fetch_times;
	gamelog::g_log_rand_activity.printf(LL_INFO, "OnCirculationChongzhiSecondFetchReward uid:%d name:%s level:%d times:%d", 
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), can_fetch_times);

	// 给予
	static MailContentParam contentparam; contentparam.Reset();

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_circulation_chongzhi_not_ra_reward_content);
	if (length > 0)
	{
		int pile_limit = item_base->GetPileLimit();
		int real_item_count = item->num * can_fetch_times;
		int mail_count = real_item_count / (MAX_ATTACHMENT_ITEM_NUM * pile_limit) + 1;

		if (mail_count > 0)
		{
			for (int i = 0; i < mail_count; ++i)
			{
				bool is_send_finish = false;
				for (int item_count = 0; item_count < MAX_ATTACHMENT_ITEM_NUM; ++item_count)
				{
					ItemConfigData put_item(*item);
					if (real_item_count > pile_limit)
					{
						put_item.num = pile_limit;
						real_item_count -= pile_limit;

						contentparam.AddItem(put_item);
					}
					else
					{
						put_item.num = real_item_count;

						contentparam.AddItem(put_item);

						is_send_finish = true;
						break;
					}
				}

				MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);

				contentparam.item_count = 0;
				memset(contentparam.item_list, 0, sizeof(contentparam.item_list));

				if (is_send_finish)
				{
					break;
				}
			}

			this->SendCirculationChongzhiSecondInfo();
		}
	}
}

void RoleActivity::RACheckShakeMoneyGiveReward()
{
	if (m_activity_info.ra_shakemoney_history_chongzhi_num <= 0)
	{
		return;
	}

	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), m_activity_info.ra_shakemoney_begin_timestamp) + 1;
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
		return;
	}

	m_activity_info.ra_shakemoney_fetched_num = 0;
	m_activity_info.ra_shakemoney_history_chongzhi_num = 0;

	static MailContentParam contentparam; contentparam.Reset();
	contentparam.gold = can_fetch_num;

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_shake_money_reward_content);
	if (length > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

		gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckShakeMoneyGiveReward::user[%d, %s], gold[%d]",
			m_role->GetUID(), m_role->GetName(), contentparam.gold);
	}
}

void RoleActivity::RACheckConsumeGoldRewardAutoFetch()
{
	const RandActivityConsumeRewardStageCfg *cfg = LOGIC_CONFIG->GetRandActivityCfg().GetConsumeRewardCfg(
		m_activity_info.ra_consume_gold_reward_today_vip_level, m_activity_info.ra_consume_gold_reward_activity_day);
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

	// 给予
	static MailContentParam contentparam; contentparam.Reset();
	for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		contentparam.AddItem(cfg->reward_item_list[i]);
	}

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward, RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD);
	if (length > 0)
	{
		MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
	}

	// 日志
	gamelog::g_log_roleactivity.printf(LL_INFO, "OnConsumeGoldRewardFetchReward user[%d %s], vip_level[%d], consume_gold[%d]",
		m_role->GetUID(), m_role->GetName(), m_activity_info.ra_consume_gold_reward_today_vip_level, m_activity_info.ra_consume_gold_reward_consume_gold);
}

void RoleActivity::RACheckVersionTotalChargeGiveReward()
{
	int last_activity_got_max_seq = LOGIC_CONFIG->GetRandActivityCfg().GetVersionTotalChargeSeqWithCharge(m_activity_info.ra_version_total_charge_value);
	if (last_activity_got_max_seq < 0)
	{
		return;
	}

	for (int seq = 0; seq <= last_activity_got_max_seq; ++seq)
	{
		if (0 != (m_activity_info.ra_version_total_charge_reward_has_fetch_flag & (1 << seq)))
		{
			continue;
		}

		const RandActivityTotalChargeRewardConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetVersionTotalChargeRewardWithSeq(seq);
		if (nullptr == cfg_item)
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
				if (nullptr != reward_item_base)
				{
					contentparam.item_list[reward_item_count].item_id = cfg_item->item_list[i].item_id;
					contentparam.item_list[reward_item_count].num = cfg_item->item_list[i].num;
					contentparam.item_list[reward_item_count].is_bind = (cfg_item->item_list[i].is_bind) ? 1 : 0;
					contentparam.item_list[reward_item_count].invalid_time = reward_item_base->CalInvalidTime();
					++reward_item_count;
				}
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_total_charge_bu_reward_content,
				RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE);
			if (length > 0)
			{
				m_activity_info.ra_version_total_charge_reward_has_fetch_flag |= (1 << seq);
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		{
			// 日志
			std::string get_item_log;
			ItemConfigDataListLog(get_item_log, cfg_item->item_count, cfg_item->item_list);

			gamelog::g_log_roleactivity.printf(LL_INFO, "RACheckVersionTotalChargeGiveReward user[%d, %s], level[%d], reward_seq[%d], reward_flag[%d], item_list[%s]",
				m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_activity_info.ra_version_total_charge_reward_has_fetch_flag, get_item_log.c_str());
		}
	}
}

void RoleActivity::RACheckVersionContinueChongzhiGiveReward()
{
	bool is_first_log = false;

	// 通过邮箱发放达标未领取奖励
	for (int index = 1; index <= m_activity_info.ra_version_continue_chongzhi_days; ++index)
	{
		if (0 == (m_activity_info.ra_version_continue_chongzhi_can_fetch_reward_flag & (1 << index)))	// 奖励未达标
		{
			break;
		}

		if (0 != (m_activity_info.ra_version_continue_chongzhi_has_fetch_reward_flag & (1 << index)))	// 奖励已领取
		{
			continue;
		}

		static MailContentParam contentparam; contentparam.Reset();
		const RandActivityContinueChongzhiRewardConfig * continue_chongzhi_info_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetVersionContinueChargeRewardInfoWithdayindex(index, false, true);
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

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_version_continue_mail_content, RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE);
			if (length > 0)
			{

				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				if (!is_first_log)
				{
					is_first_log = true;

					gamelog::g_log_rand_activity.buff_printf("RACheckVersionContinueChongzhiGiveReward::user[%d, %s], reward: ",
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

	m_activity_info.ra_version_continue_chongzhi_can_fetch_reward_flag = 0;
	m_activity_info.ra_version_continue_chongzhi_has_fetch_reward_flag = 0;
}

void RoleActivity::RACheckHuanLeYaoJiang2FetchPersonalReward()
{
	int total_reward_num = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeYaoJiang2TotalRewardNum();
	for (int i = 0; i < total_reward_num; ++i)
	{
		if (0 != (m_activity_info.ra_huanleyaojiang_2_personal_reward_flag & (1 << i)))
		{
			continue;
		}
		else
		{
			const RandActivityMiJingXunBaoRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityHuanLeYaoJiang2PersonalRewardCfg(i, m_activity_info.ra_huanleyaojiang_2_role_chou_times);
			if (nullptr == reward_cfg)
			{
				return;
			}

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				static MailContentParam contentparam; contentparam.Reset();

				contentparam.item_list[0].item_id = reward_cfg->reward_item.item_id;
				contentparam.item_list[0].num = reward_cfg->reward_item.num;
				contentparam.item_list[0].is_bind = (reward_cfg->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward_common_mail_content, RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG);
				if (length > 0)
				{
					m_activity_info.ra_huanleyaojiang_2_personal_reward_flag |= (1 << i);

					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
	}

	m_activity_info.ra_huanleyaojiang_2_role_chou_times = 0;
	m_activity_info.ra_huanleyaojiang_2_personal_reward_flag = 0;

	this->SendRAHuanLeYaoJiang2Info();
}

void RoleActivity::CheckRAOfflineSingleChargeReward0()
{
	const RandActivityOfflineSingleChargeCfg::ConfigItem *sc_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetOfflineSingleChargeCfg0(m_activity_info.ra_offline_single_charge_max_value_0, m_activity_info.ra_offline_single_charge_join_timestamp_0);
	if (NULL == sc_cfg)
	{
		return;
	}

	// 拿最高档奖励
	if (RandActivityOfflineSingleChargeCfg::REWARD_TYPE_MAX_GIFT == sc_cfg->reward_type)
	{
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

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_offline_single_charge_reward_content, m_activity_info.ra_offline_single_charge_max_value_0);
		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			this->SendRAOfflineSingleChargeInfo();

			gamelog::g_log_rand_activity.printf(LL_INFO, "OnRASingleChargeCheckCharge::user[%d, %s], reward_item[%s]",
				m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&sc_cfg->reward_item, NULL));
		}
	}

	m_activity_info.ra_offline_single_charge_max_value_0 = -1;
	memset(m_activity_info.ra_offline_single_charge_reward_times_list_0, 0, sizeof(m_activity_info.ra_offline_single_charge_reward_times_list_0));
}

void RoleActivity::RACheckBuyOneGetOneFreeReward()
{
	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), (time_t)m_activity_info.ra_buy_one_get_one_free_join_timestamp) + 1;

	for (int i = 0; i < RAND_ACTIVITY_BUY_ONE_GET_ONE_FREE_ITEM_MAX_COUNT; i++)
	{
		if (0 != (m_activity_info.ra_buy_one_get_one_free_buy_flag & (LL_ONE_BIT << i)) && 0 == (m_activity_info.ra_buy_one_get_one_free_fetch_reward_flag & (LL_ONE_BIT << i)))
		{
			const BuyOneGetOneFreeItemCfg* cfg = LOGIC_CONFIG->GetRandActivityCfg().GetBuyOneGetOneFreeCfg(opengame_day, i);
			if (NULL != cfg)
			{
				static MailContentParam contentparam; contentparam.Reset();

				const ItemBase *reward_item_base = ITEMPOOL->GetItem(cfg->free_reward_item.item_id);
				if (NULL == reward_item_base)
				{
					continue;;
				}

				contentparam.item_list[0].item_id = cfg->free_reward_item.item_id;
				contentparam.item_list[0].num = cfg->free_reward_item.num;
				contentparam.item_list[0].is_bind = cfg->free_reward_item.is_bind ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward_common_mail_content, RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE);
				if (length > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

					gamelog::g_log_roleactivity.printf(LL_INFO, "RACheckBuyOneGetOneFreeReward::user[%d %s], reward[%s]", m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&cfg->free_reward_item, NULL));
				}
			}

			m_activity_info.ra_buy_one_get_one_free_fetch_reward_flag |= (LL_ONE_BIT << i);
		}
	}

	m_activity_info.ra_buy_one_get_one_free_buy_flag = 0;
	m_activity_info.ra_buy_one_get_one_free_fetch_reward_flag = 0;
}

void RoleActivity::OnRAEverydayNiceGiftReissueReward()
{
	if (0 == m_activity_info.ra_everyday_nice_gift_is_have_recharge_standard)
	{
		return;
	}

	const RandActivityEverydayNiceGiftCfg::ConfigItem *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetEveydayNiceGiftRewardCfg();
	if (nullptr == reward_cfg)
	{
		return;
	}

	static MailContentParam contentparam; contentparam.Reset();

	for (auto index = 0; index < reward_cfg->reward_count && index < RAND_ACTIVITY_EVERYDAY_NICE_GIFT_MAX_REWARD_COUNT; ++index)
	{
		if (0 == (m_activity_info.ra_everyday_nice_gift_can_fetch_reward_flag & (1 << index))) continue;

		if ((m_activity_info.ra_everyday_nice_gift_have_fetch_reward_flag & 1 << index) != 0) continue;

		const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg->reward_item_list[index].item_id);
		if (reward_item_base != nullptr && contentparam.item_count < MAX_ATTACHMENT_ITEM_NUM && reward_cfg->reward_item_list[index].item_id > 0)  // 防止发空邮件
		{
			contentparam.item_list[contentparam.item_count].item_id = reward_cfg->reward_item_list[index].item_id;
			contentparam.item_list[contentparam.item_count].num = reward_cfg->reward_item_list[index].num;
			contentparam.item_list[contentparam.item_count].is_bind = (reward_cfg->reward_item_list[index].is_bind) ? 1 : 0;
			contentparam.item_list[contentparam.item_count].invalid_time = reward_item_base->CalInvalidTime();
			++contentparam.item_count;
		}

		if (contentparam.item_count >= MAX_ATTACHMENT_ITEM_NUM || (index + 1 >= reward_cfg->reward_count && contentparam.item_count > 0))
		{
			const auto length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward_common_mail_content, RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT);
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_roleactivity.printf(LL_INFO, "OnRAEverydayNiceGiftReissueReward::user[%d %s], reward[%s]",
					m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&reward_cfg->reward_item_list[index], nullptr));
			}

			// 超过邮件数量  重置数据
			contentparam.Reset();
		}

		m_activity_info.ra_everyday_nice_gift_have_fetch_reward_flag |= 1 << index;
	}

	m_activity_info.ra_everyday_nice_gift_is_have_recharge_standard = 0;
}

void RoleActivity::RACheckGuaGuaAccReward()
{
	const RandActivityGuaGuaAccConfig *cfg = NULL;
	for (int reward_index = 0; reward_index < RA_GUAGUA_ACC_REWARD_COUNT; ++reward_index)
	{
		cfg = LOGIC_CONFIG->GetRandActivityCfg().GetGuaGuaAccRewardByIndex(reward_index);
		if ((NULL == cfg) || (m_activity_info.ra_guagua_acc_count < cfg->acc_count)) continue;						// 能否领取

		if (0 == (m_activity_info.ra_guagua_acc_reward_has_fetch_flag & (1 << reward_index)))						// 未领取
		{
			if (cfg->reward_item.num > 0)
			{
				MailContentParam contentparam; contentparam.Reset();
				contentparam.AddItem(cfg->reward_item.item_id, cfg->reward_item.num, cfg->reward_item.is_bind);
				int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_guagua_acc_reward_mail);
				if (length_1 > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
					m_activity_info.ra_guagua_acc_reward_has_fetch_flag |= (1 << reward_index);						// 已领取
				}
			}
		}
	}
	m_activity_info.ra_guagua_acc_count = 0;
}

void RoleActivity::OnHolidayGuardKillMonsterGiveReward()
{
	if (m_activity_info.ra_holiday_guard_kill_monster_count <= 0)
	{
		return;
	}

	bool is_give_rank_reward = false;
	int role_rank = -1, kill_monster_count = 0;

	RandActivityHolidayGuard *holiday_guard_activity = (RandActivityHolidayGuard*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_HOLIDAY_GUARD);
	if (nullptr != holiday_guard_activity)
	{
		role_rank = holiday_guard_activity->GetRankListInfo(m_role->GetUID(), kill_monster_count);

		if (role_rank >= 0)
		{
			is_give_rank_reward = true;
		}
	}

	m_activity_info.ra_holiday_guard_kill_monster_count = 0;

	RoleFBTowerDefend *tower_defend = m_role->GetFBTowerDefend();
	if (nullptr != tower_defend)
	{
		tower_defend->OnRefreshHolidayGuardFBKillMonsterCount();
	}

	if (is_give_rank_reward)
	{
		HolidayGuardKillRankReward::ConfigItem *m_kill_rank_reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetHolidayGuardKillRankRewardCfg(role_rank);
		if (nullptr == m_kill_rank_reward_cfg)
		{
			return;
		}

		gamelog::g_log_activity.buff_printf("RoleActivity::OnHolidayGuardKillMonsterGiveReward user[%d], rank[%d], kill_num[%d], reward_list(", m_role->GetUID(),
			role_rank + 1, kill_monster_count);

		static MailContentParam contentparam; contentparam.Reset();

		for (int j = 0; j < MAX_ATTACHMENT_ITEM_NUM; ++j)
		{
			const ItemBase *item_base = ITEMPOOL->GetItem(m_kill_rank_reward_cfg->reward_list[j].item_id);
			if (nullptr != item_base && contentparam.item_count < MAX_ATTACHMENT_ITEM_NUM)
			{
				contentparam.item_list[contentparam.item_count].item_id = m_kill_rank_reward_cfg->reward_list[j].item_id;
				contentparam.item_list[contentparam.item_count].num = m_kill_rank_reward_cfg->reward_list[j].num;
				contentparam.item_list[contentparam.item_count].is_bind = (m_kill_rank_reward_cfg->reward_list[j].is_bind) ? 1 : 0;
				contentparam.item_list[contentparam.item_count].invalid_time = item_base->CalInvalidTime();
				++contentparam.item_count;

				gamelog::g_log_activity.buff_printf("[%d, %d]", m_kill_rank_reward_cfg->reward_list[j].item_id, m_kill_rank_reward_cfg->reward_list[j].num);
			}
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_everyday_nice_gift_fetch_reward,
			RAND_ACTIVITY_TYPE_HOLIDAY_GUARD);
		if (length > 0)
		{
			MailRoute::MailToUser(IntToUid(m_role->GetUID()), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_activity.buff_print(")");
			gamelog::g_log_activity.commit_buff(LL_INFO);
		}
	}
	else
	{
		ItemConfigData &reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetHolidayGuardParticipationRewardCfg();

		static MailContentParam contentparam; contentparam.Reset();

		contentparam.AddItem(reward_cfg);

		const auto length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_everyday_nice_gift_fetch_reward, RAND_ACTIVITY_TYPE_HOLIDAY_GUARD);
		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_roleactivity.printf(LL_INFO, "OnHolidayGuardKillMonsterReissueReward::user[%d], reward[%d %d]",
				m_role->GetUID(), reward_cfg.item_id, reward_cfg.num);
		}
	}
}

void RoleActivity::OnRAExtremeChallengeGiveReward()
{
	gamelog::g_log_roleactivity.buff_printf("OnRAExtremeChallengeReissueReward::user[%d %s]",
		m_role->GetUID(), m_role->GetName());

	for (int i = 0; i < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM; ++i)
	{
		if (m_activity_info.ra_extreme_challenge_task_info_list[i].is_finish == 0 ||
			m_activity_info.ra_extreme_challenge_task_info_list[i].is_already_fetch != 0)
		{
			continue;
		}

		gamelog::g_log_roleactivity.buff_printf(" taskid:%d", m_activity_info.ra_extreme_challenge_task_info_list[i].task_id);

		static MailContentParam contentparam; contentparam.Reset();

		const RandActivityExtremeChallengeCfg *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengePerformTaskInfoCfg(m_activity_info.ra_extreme_challenge_task_info_list[i].task_id);
		if (nullptr != reward_cfg)
		{
			m_activity_info.ra_extreme_challenge_task_info_list[i].is_already_fetch = 1;

			for (int j = 0; j < MAX_ATTACHMENT_ITEM_NUM; ++j)
			{
				const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg->reward_list[j].item_id);
				if (reward_item_base != nullptr && contentparam.item_count < MAX_ATTACHMENT_ITEM_NUM)
				{
					contentparam.item_list[contentparam.item_count].item_id = reward_cfg->reward_list[j].item_id;
					contentparam.item_list[contentparam.item_count].num = reward_cfg->reward_list[j].num;
					contentparam.item_list[contentparam.item_count].is_bind = (reward_cfg->reward_list[j].is_bind) ? 1 : 0;
					contentparam.item_list[contentparam.item_count].invalid_time = reward_item_base->CalInvalidTime();
					++contentparam.item_count;

					gamelog::g_log_roleactivity.buff_printf("itemid:%d num:%d|", reward_cfg->reward_list[j].item_id, reward_cfg->reward_list[j].num);
				}
			}

			const auto length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward_common_mail_content, RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE);
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			}
		}
	}
	gamelog::g_log_roleactivity.commit_buff(LL_INFO);

	if (m_activity_info.ra_extreme_challenge_ultimate_have_fetch == 0)
	{
		int complete_task_num = 0;
		const int max_task_num = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengeMaxTaskNum();

		for (int i = 0; i < RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM && i < max_task_num; ++i)
		{
			if (m_activity_info.ra_extreme_challenge_task_info_list[i].is_finish > 0)
			{
				++complete_task_num;
			}
		}

		if (complete_task_num >= max_task_num)
		{
			m_activity_info.ra_extreme_challenge_ultimate_have_fetch = 1;

			static MailContentParam contentparam; contentparam.Reset();

			gamelog::g_log_activity.buff_printf(" RoleActivity::OnRAExtremeChallengeUltimateReward user[%d %s], reward_list(", m_role->GetUID(), m_role->GetName());

			const ItemConfigData *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetExtremeChallengeFinishAllTaskRewardCfg();
			if (nullptr != reward_cfg)
			{
				for (int j = 0; j < MAX_ATTACHMENT_ITEM_NUM; ++j)
				{
					const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg[j].item_id);
					if (reward_item_base != nullptr && contentparam.item_count < MAX_ATTACHMENT_ITEM_NUM)
					{
						contentparam.item_list[contentparam.item_count].item_id = reward_cfg[j].item_id;
						contentparam.item_list[contentparam.item_count].num = reward_cfg[j].num;
						contentparam.item_list[contentparam.item_count].is_bind = (reward_cfg[j].is_bind) ? 1 : 0;
						contentparam.item_list[contentparam.item_count].invalid_time = reward_item_base->CalInvalidTime();
						++contentparam.item_count;

						gamelog::g_log_activity.buff_printf("[itemid:%d, num:%d]", reward_cfg[j].item_id, reward_cfg[j].num);
					}
				}

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward_common_mail_content,
					RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE);
				if (length > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}

			gamelog::g_log_activity.buff_print(")");
			gamelog::g_log_activity.commit_buff(LL_INFO);
		}
	}
}

void RoleActivity::RACheckChongZhiGiftGiveReward()
{
	if (m_activity_info.ra_magic_shop_chongzhi_value <= 0) return;

	for (int i = 0; i < RA_CHONGZHI_GIFT_ITEM_MAX_INDEX; ++i)
	{
		if (0 != (m_activity_info.ra_magic_shop_fetch_reward_flag & (1 << i)))
		{
			continue;
		}

		const RandActivityChongZhiGiftConfig *chongzhi_gift_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetChongZhiGiftCfg(m_activity_info.ra_chongzhi_gift_join_act_day, i);
		if (NULL == chongzhi_gift_cfg) continue;

		if (m_activity_info.ra_magic_shop_chongzhi_value < (unsigned int)chongzhi_gift_cfg->need_gold)
		{
			continue;
		}

		{
			// 发邮件
			static MailContentParam contentparam; contentparam.Reset();

			contentparam.item_list[0].item_id = chongzhi_gift_cfg->reward_item.item_id;
			contentparam.item_list[0].num = chongzhi_gift_cfg->reward_item.num;
			contentparam.item_list[0].is_bind = (chongzhi_gift_cfg->reward_item.is_bind) ? 1 : 0;

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_magic_shop_reward_content);
			if (length > 0)
			{
				m_activity_info.ra_magic_shop_fetch_reward_flag |= (1 << i);
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		{
			// 日志
			gamelog::g_log_rand_activity.printf(LL_INFO, "RACheckChongZhiGiftGiveReward::user[%d, %s], index[%d], reward_flag[%d], reward_item[%s]",
				m_role->GetUID(), m_role->GetName(), i, m_activity_info.ra_magic_shop_fetch_reward_flag,
				ItemConfigDataLog(&chongzhi_gift_cfg->reward_item, NULL));

			ROLE_LOG_QUICK6(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT, m_role, i, m_activity_info.ra_magic_shop_buy_flag, NULL, NULL);
		}
	}
}

void RoleActivity::ReissueHuanLeYaoJiangFetchPersonalReward()
{
	int total_reward_num = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeYaoJiangTotalRewardNum();
	for (int i = 0; i < total_reward_num; ++i)
	{
		if (0 != (m_activity_info.ra_huanleyaojiang_personal_reward_flag & (1 << i)))
		{
			continue;
		}
		else
		{
			const RandActivityMiJingXunBaoRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityHuanLeYaoJiangPersonalRewardCfg(i, m_activity_info.ra_huanleyaojiang_role_chou_times);
			if (nullptr == reward_cfg)
			{
				return;
			}

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				static MailContentParam contentparam; contentparam.Reset();

				contentparam.item_list[0].item_id = reward_cfg->reward_item.item_id;
				contentparam.item_list[0].num = reward_cfg->reward_item.num;
				contentparam.item_list[0].is_bind = (reward_cfg->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward_common_mail_content, RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG);
				if (length > 0)
				{
					m_activity_info.ra_huanleyaojiang_personal_reward_flag |= (1 << i);

					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
	}

	m_activity_info.ra_huanleyaojiang_role_chou_times = 0;
	m_activity_info.ra_huanleyaojiang_personal_reward_flag = 0;

	this->SendRAHuanLeYaoJiangInfo();
}

void RoleActivity::ReissueHuanLeZaDanFetchPersonalReward()
{

	int total_reward_num = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeZaDanTotalRewardNum();
	for (int i = 0; i < total_reward_num; ++i)
	{
		if (0 != (m_activity_info.ra_huanlezadan_personal_reward_flag & (1 << i)))
		{
			continue;
		}
		else
		{
			const RandActivityMiJingXunBaoRewardConfig *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetRandActivityHuanLeZaDanPersonalRewardCfg(i, m_activity_info.ra_huanlezadan_role_chou_times);
			if (nullptr == reward_cfg)
			{
				return;
			}

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg->reward_item.item_id);
			if (NULL != reward_item_base)
			{
				static MailContentParam contentparam; contentparam.Reset();

				contentparam.item_list[0].item_id = reward_cfg->reward_item.item_id;
				contentparam.item_list[0].num = reward_cfg->reward_item.num;
				contentparam.item_list[0].is_bind = (reward_cfg->reward_item.is_bind) ? 1 : 0;
				contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime();

				int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward_common_mail_content, RAND_ACTIVITY_TYPE_HUANLE_ZADAN);
				if (length > 0)
				{
					m_activity_info.ra_huanlezadan_personal_reward_flag |= (1 << i);

					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
	}

	m_activity_info.ra_huanlezadan_role_chou_times = 0;
	m_activity_info.ra_huanlezadan_personal_reward_flag = 0;

	this->SendRAHuanLeZaDanInfo();
}

void RoleActivity::RACheckChangeRepayment()
{
	if (m_activity_info.ra_charge_repayment_history_charge <= 0)
	{
		return;
	}

	for (int i = 0; i < RAND_ACTIVITY_CHARGE_REPAYMENT_CONFIG_MAX_COUNT; ++i)
	{
		if (0 == (m_activity_info.ra_charge_repayment_reward_active_flag & (1 << i)) ||
			0 != (m_activity_info.ra_charge_repayment_reward_fetch_flag & (1 << i)))
		{
			continue;
		}

		const RandActivityChargeRepaymentCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetRandActivityCfg().GetChargeRepaymentCfgWithSeq(i);
		if (NULL == cfg_item || cfg_item->bind_gold_repayment <= 0)
		{
			continue;
		}

		static MailContentParam contentparam; contentparam.Reset();
		contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, cfg_item->bind_gold_repayment);

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward_common_mail_content, RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT);
		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}

		m_activity_info.ra_charge_repayment_reward_fetch_flag |= (1 << i);
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT))   // 活动结束，补领完最后一次后清除数据
	{
		m_activity_info.ra_charge_repayment_history_charge = 0;
	}
}

// 狂欢庆典
void RoleActivity::CheckRAKuangHaiQingDianGiveReward()
{
	for (int i = 0; i < RA_KUANG_HAI_REWARD_LIST_MAX_NUM; i++)
	{
		// 是否已兑换
		if ((m_activity_info.ra_kuang_hai_qing_dian_fetch_reward_flag & (0x01 << i)) != 0)
		{
			continue;
		}

		auto cfg_reward_list = LOGIC_CONFIG->GetRandActivityCfg().GetKuangHaiRewardItem(i);
		if (nullptr == cfg_reward_list)
		{
			continue;
		}

		// 检查嗨点是否足够
		int need_sceore = cfg_reward_list->need_score;
		if (need_sceore > m_activity_info.ra_kuang_hai_qing_dian_total_score)
		{
			// 嗨点不足
			return;
		}

		// 发邮件
		gamecommon::SendMultiMailToRole(m_role->GetUserId(), cfg_reward_list->reward_list, cfg_reward_list->reward_num, NULL, true,
			gamestring::g_rand_activity_check_reward, RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN);

		// 记录
		m_activity_info.ra_kuang_hai_qing_dian_fetch_reward_flag |= (0x01 << i);

		std::string get_item_log;
		ItemConfigDataListLog(get_item_log, cfg_reward_list->reward_num, cfg_reward_list->reward_list);

		gamelog::g_log_roleactivity.printf(LL_INFO, "RoleActivity::CheckRAKuangHaiQingDianGiveReward user[%d, %s], level[%d], list_idx[%d], item_list[%s]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), i, get_item_log.c_str());
	}

	return;
}

// 礼物收割
void RoleActivity::CheckRAGiftHarvestGiveReward()
{
	// 参与奖
	if (m_activity_info.ra_gift_harvest_score > 0)
	{
		static MailContentParam contentparam; contentparam.Reset();

		gamelog::g_log_activity.buff_printf(" RoleActivity::CheckRAGiftHarvestGiveReward user[%d], reward_list(", m_role->GetUID());

		const ItemConfigData *reward_cfg = LOGIC_CONFIG->GetRandActivityCfg().GetGiftHarvestJoinRewardCfg();
		if (nullptr != reward_cfg)
		{
			for (int j = 0; j < MAX_ATTACHMENT_ITEM_NUM; ++j)
			{
				const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg[j].item_id);
				if (reward_item_base != nullptr && contentparam.item_count < MAX_ATTACHMENT_ITEM_NUM)
				{
					contentparam.item_list[contentparam.item_count].item_id = reward_cfg[j].item_id;
					contentparam.item_list[contentparam.item_count].num = reward_cfg[j].num;
					contentparam.item_list[contentparam.item_count].is_bind = (reward_cfg[j].is_bind) ? 1 : 0;
					contentparam.item_list[contentparam.item_count].invalid_time = reward_item_base->CalInvalidTime();
					++contentparam.item_count;

					gamelog::g_log_activity.buff_printf("[%d, %d]", reward_cfg[j].item_id, reward_cfg[j].num);
				}
			}

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_rand_activity_check_reward_common_mail_content,
				RAND_ACTIVITY_TYPE_GIFT_HARVEST);
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		gamelog::g_log_activity.buff_print(")");
		gamelog::g_log_activity.commit_buff(LL_INFO);
	}

	// 
	m_activity_info.ra_gift_harvest_score = 0;
	m_activity_info.ra_gift_harvest_get_score_times = 0;
	m_activity_info.ra_gift_harvest_reserve_sh = 0;
}