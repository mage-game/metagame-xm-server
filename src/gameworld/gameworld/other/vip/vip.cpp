#include "vip.hpp"

#include "gamelog.h"
#include "engineadapter.h"
#include "world.h"
#include "protocal/msgrole.h"

#include "globalconfig/globalconfig.h"
#include "obj/character/role.h"
#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "scene/scene.h"
#include "other/event/eventhandler.hpp"

#include "other/route/mailroute.hpp"
#include "servercommon/string/globalstr.h"
#include "servercommon/string/gameworldstr.h"

#include "other/daycounter/daycounter.hpp"

#include "item/itempool.h"
#include "config/logicconfigmanager.hpp"
#include "config/guildconfig.hpp"
#include "other/vip/vipconfig.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "global/guild/roleguild.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "other/welfare/welfare.hpp"

Vip::Vip()
	: m_role(NULL)
{
}

Vip::~Vip()
{

}

void Vip::Init(Role *role, const VipParam &vip_param)
{
	m_role = role; 
	m_vip_param = vip_param;
}

void Vip::GetOtherInitParam(VipParam *vip_param)
{
	*vip_param = m_vip_param;
}

void Vip::Update(time_t now_second)
{
	if (m_vip_param.vip_level != 0 && m_vip_param.time_limit_vip_time > 0 && now_second > m_vip_param.time_limit_vip_time)
	{
		this->SetVipLevel(0);
	}

	int create_role_day_index = 0;
	create_role_day_index = GetDayIndex(m_role->GetCreateRoleTimestamp(), EngineAdapter::Instance().Time());

	if (create_role_day_index >= LOGIC_CONFIG->GetVipConfig().GetOtherCfg().exp_day_open)
	{
		const OnlineVipExpRewradCfg *reward_cfg = LOGIC_CONFIG->GetVipConfig().GetOnlineRewardVipExp(m_role->GetTodayRealTimeOnlineS() / 60);		//在线奖励vip经验
		if (reward_cfg != NULL)
		{
			int reward_vip_exp = reward_cfg->vip_exp;

			if ((m_vip_param.reward_vip_exp_flag & (1 << reward_cfg->seq)) == 0)
			{
				this->AddVipExp(reward_vip_exp);
				m_vip_param.reward_vip_exp_flag |= (1 << reward_cfg->seq);
			}
		}
	}
}

void Vip::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	const VipBuffConfig *vip_cfg = LOGIC_CONFIG->GetVipConfig().GetVipBuffCfg(static_cast<int>(m_vip_param.vip_level));

	if (NULL == vip_cfg)
	{
		return;
	}

	if (is_recalc)
	{
		m_attrs_add.Reset();

		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += vip_cfg->gongji;
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += vip_cfg->fangyu;
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += vip_cfg->maxhp;

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_VIPBUFF, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void Vip::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_vip_param.gold_buycoin_times = 0;
		m_vip_param.gold_buyyuanli_times = 0;
		m_vip_param.gold_buyxianhun_times = 0;
		m_vip_param.free_buycoin_times = 0;
		m_vip_param.free_buyyuanli_times = 0;
		m_vip_param.free_buyxianhun_times = 0;
		m_vip_param.fetch_qifu_buycoin_reward_flag = 0;
		m_vip_param.fetch_qifu_buyyuanli_reward_flag = 0;
		m_vip_param.fetch_qifu_buyxianhun_reward_flag = 0;
		m_vip_param.reward_vip_exp_flag = 0;

		this->SendVipInfo(false);
	}
}

int Vip::GetVipLevel()
{
	return m_vip_param.vip_level;
}

bool Vip::AddVipExp(int inc_exp)
{	
	if (inc_exp <= 0) return false;
	
	int old_vip_level = m_vip_param.vip_level;
	if (this->IsTimeLimitVip())
	{
		old_vip_level = 0;
	}

	int round_count = 0;
	while (inc_exp > 0 && round_count ++ < 64)
	{
		int now_vip_level = m_vip_param.vip_level;
		if (this->IsTimeLimitVip())
		{
			now_vip_level = 0;
		}

		int max_exp = LOGIC_CONFIG->GetVipConfig().GetVipExpMaxByLevel(now_vip_level);
		if (max_exp <= 0) break;

		if (m_vip_param.vip_exp + inc_exp >= max_exp)
		{
			inc_exp -= (max_exp - m_vip_param.vip_exp);			
			if (!this->UpgradeVipLevel(now_vip_level + 1)) // 升级vip
			{
				m_vip_param.vip_exp = max_exp;
				break;	
			}
			m_vip_param.time_limit_vip_time = 0;
		}
		else 
		{
			m_vip_param.vip_exp += inc_exp;
			inc_exp = 0;
			break;
		}
	}

	this->SendVipInfo();

	m_role->OnRoleBaseInfoChange();

	if (old_vip_level != m_vip_param.vip_level && m_vip_param.vip_level >= 3 && !this->IsTimeLimitVip())
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::vip_uplevel,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), (int)m_vip_param.vip_level);

		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
		}

		int length2 = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::vip_uplevel_personal, m_vip_param.vip_level);
		if (length2 > 0)
		{
			m_role->SendPersonalMsg(length2, gamestring::GAMESTRING_BUF);
		}
	}

	return true;
}

bool Vip::IsTimeLimitVip()
{
	if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) - 1 < m_vip_param.time_limit_vip_time)
	{
		return true;
	}

	return false;
}

void Vip::FetchLevelReward(int seq)
{
	if (this->IsTimeLimitVip())
	{
		m_role->NoticeNum(errornum::EN_TIME_LIMIT_NOT_FETCH_REWARD);
		return;
	}

	if (seq < 0 || seq >= VipLevelReward::ITEM_CONFIG_MAX_COUNT || seq >= static_cast<int>(sizeof(m_vip_param.fetch_level_reward_flag) * 8))
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (0 != (m_vip_param.fetch_level_reward_flag & (1 << seq)))
	{
		m_role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
		return;
	}

	const VipLevelReward::ConfigItem *cfg_item = LOGIC_CONFIG->GetVipConfig().GetLevelReward(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_vip_param.vip_level < cfg_item->need_level)
	{
		m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGH);
		return;
	}

	m_vip_param.fetch_level_reward_flag |= (1 << seq);

	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_VIP_LEVEL_REWARD);

	this->SendVipInfo(false);

	gamelog::g_log_vip.printf(LL_INFO, "FetchLevelReward::user[%d, %s], level[%d], seq[%d],  flag[%d], reward_item[%s]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, m_vip_param.fetch_level_reward_flag, ItemConfigDataLog(&cfg_item->reward_item, NULL));

	ROLE_LOG_QUICK6(LOG_TYPE_VIP_BUY_LEVEL_REWARD, m_role, seq, m_vip_param.fetch_level_reward_flag, NULL, NULL);
}

void Vip::FetchVipWeekReward()
{
	if (this->IsTimeLimitVip())
	{
		m_role->NoticeNum(errornum::EN_TIME_LIMIT_NOT_FETCH_REWARD);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if(m_vip_param.vip_level <= 0)
	{
		return;
	}
	
	const VipLevelReward::ConfigItem *cfg_item = LOGIC_CONFIG->GetVipConfig().GetLevelReward(m_vip_param.vip_level - 1);
	if (NULL == cfg_item)
	{
		return;
	}

	if (m_vip_param.vip_level < cfg_item->need_level)
	{
		m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGH);
		return;
	}
	
	const VipOtherConfig &other_cfg = LOGIC_CONFIG->GetVipConfig().GetOtherCfg();

	ItemConfigData reward_item;
	reward_item.is_bind = true;
	reward_item.item_id = other_cfg.week_reward_id;
	reward_item.num = cfg_item->week_reward_num - m_vip_param.vip_week_gift_times;
	if(reward_item.num > 0)
	{
		m_vip_param.vip_week_gift_times += reward_item.num;// 记录已经领了多少个

		m_role->GetKnapsack()->Put(reward_item, PUT_REASON_VIP_WEEK_REWARD);

		this->SendVipInfo(false);

		gamelog::g_log_vip.printf(LL_INFO, "FetchVipWeekReward::user[%d, %s], level[%d], seq[%d],  flag[%d], reward_item[%s]",
			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), m_vip_param.vip_level - 1, reward_item.num, ItemConfigDataLog(&reward_item, NULL));

		ROLE_LOG_QUICK6(PUT_REASON_VIP_WEEK_REWARD, m_role, m_vip_param.vip_level - 1, reward_item.num, NULL, NULL);
	}
}

void Vip::FetchTimeLimitVip()
{
	if(m_vip_param.vip_level > 0 || m_vip_param.time_limit_vip_time != 0)
	{
		return;
	}

	m_vip_param.time_limit_vip_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetVipConfig().GetOtherCfg().time_limit_vip_time;
	this->SetVipLevel(1, true);
	
	// 激活传闻
	int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_time_limit_vip_active_content, m_role->GetUID(), m_role->GetName(),
		(int)m_role->GetCamp());
	if (len > 0) 
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
	}
}

void Vip::SendVipInfo(bool is_broadcast)
{
	static Protocol::SCVipInfo vi;
	vi.obj_id = m_role->GetId();
	vi.vip_level = this->GetVipLevel();
	vi.vip_exp = m_vip_param.vip_exp;
	vi.gold_buycoin_times = m_vip_param.gold_buycoin_times;
	vi.gold_buyxianhun_times = m_vip_param.gold_buyxianhun_times;
	vi.gold_buyyuanli_times = m_vip_param.gold_buyyuanli_times;
	vi.fetch_level_reward_flag = m_vip_param.fetch_level_reward_flag;
	vi.fetch_qifu_buycoin_reward_flag = m_vip_param.fetch_qifu_buycoin_reward_flag;
	vi.fetch_qifu_buyxianhun_reward_flag = m_vip_param.fetch_qifu_buyxianhun_reward_flag;
	vi.fetch_qifu_buyyuanli_reward_flag = m_vip_param.fetch_qifu_buyyuanli_reward_flag;
	vi.free_buycoin_times = m_vip_param.free_buycoin_times;
	vi.free_buyyuanli_times = m_vip_param.free_buyyuanli_times;
	vi.free_buyxianhun_times = m_vip_param.free_buyxianhun_times;
	vi.last_free_buycoin_timestamp = m_vip_param.qifu_last_free_buycoin_timestamp;
	vi.last_free_buyyuanli_timestamp = m_vip_param.qifu_last_free_buyyuanli_timestamp;
	vi.last_free_buyxianhun_timestamp = m_vip_param.qifu_last_free_buyxianhun_timestamp;
	// 这周已经领了多少个周礼包
	vi.vip_week_gift_times = m_vip_param.vip_week_gift_times;
	vi.time_limit_vip_time = m_vip_param.time_limit_vip_time;

	if (is_broadcast)
	{
		if (NULL != m_role->GetScene())
		{
			m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&vi, sizeof(vi));
		}
	}
	else
	{
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&vi, sizeof(vi));
	}

	m_role->GetWelfare()->SendWelfareInfo();
}

void Vip::SetVipLevel(int vip_level, bool is_time_limit_vip)
{
	int old_vip_level = m_vip_param.vip_level;

	if (vip_level >= 0 && vip_level <= MAX_VIP_LEVEL)
	{
		m_vip_param.vip_level = (char)vip_level;
	}

	if (!is_time_limit_vip && vip_level > 0)
	{
		m_vip_param.time_limit_vip_time = 0;
	}

	this->SendVipInfo();

	if (old_vip_level != m_vip_param.vip_level)
	{
		EventHandler::Instance().OnVipLevelChange(m_role);
	}
}

bool Vip::UpgradeVipLevel(int to_vip_level)
{
	if (to_vip_level <= 0 || to_vip_level > MAX_VIP_LEVEL) return false;

	m_vip_param.vip_level = to_vip_level;
	m_vip_param.vip_exp = 0;

	m_role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_VIP_LEVEL_UP, to_vip_level);
	EventHandler::Instance().OnVipLevelChange(m_role);

	ROLE_LOG_QUICK6(LOG_TYPE_VIP_UPLEVEL, m_role, m_vip_param.vip_level, 0, NULL, NULL);

	return true;
}

void Vip::OnWeekChange()
{
	m_vip_param.vip_week_gift_times = 0;
	
	this->SendVipInfo();
}
