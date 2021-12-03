#include "zerogift.hpp"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "config/guildconfig.hpp"
#include "other/zerogift/zerogiftconfig.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "global/guild/roleguild.hpp"
#include "item/knapsack.h"
#include "other/sublock/sublock.hpp"
#include "servercommon/serverconfig/openserverconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"
#include "other/tianshuxunzhu/tianshuxunzhuconfig.hpp"

ZeroGift::ZeroGift()
	: m_role(NULL)
{

}

ZeroGift::~ZeroGift()
{
}

void ZeroGift::Init(Role *role, const ZeroGiftParam &param)
{
	m_role = role;
	m_param = param;

	this->CheckZeroGiftOpen();
}

void ZeroGift::GetInitParam(ZeroGiftParam *param)
{
	if (NULL != param)
	{
		*param = m_param;
	}
}

void ZeroGift::Update(time_t now_second)
{

}

void ZeroGift::OnDayChange()
{
	this->SendZeroGiftInfo();
}

void ZeroGift::ZeroGiftOperate(int operate_type, int param_1, int param_2)
{
	switch (operate_type)
	{
	case Protocol::CSZeroGiftOperate::ZERO_GIFT_GET_INFO:
	{
		this->SendZeroGiftInfo();
	}
	break;

	case Protocol::CSZeroGiftOperate::ZERO_GIFT_BUY:
	{
		this->OnBuy(param_1);
	}
	break;

	case Protocol::CSZeroGiftOperate::ZERO_GIFT_FETCH_REWARD_ITEM:
	{
		this->OnFetchRewardItem(param_1, param_2);
	}
	break;

	case Protocol::CSZeroGiftOperate::ZERO_GIFT_BUY_GOD_COSTUME:
	{
		this->OnBuyGodCostume(param_1);
	}
	break;

	case Protocol::CSZeroGiftOperate::ZERO_GIFT_FETCH_GOD_COSTUME_REWARD_ITEM:
	{
		this->OnFetchGodCostume(param_1, param_2);
	}
	break;
	}
}

void ZeroGift::SendZeroGiftInfo()
{
	Protocol::SCZeroGiftInfo dbi;

	for (int i = 0; i < ZERO_GIFT_PHASE_MAX; ++i)
	{
		dbi.phase_list[i].state = m_param.zero_gift_list[i].state;
		dbi.phase_list[i].reward_flag = m_param.fetch_flag_list[i];
		dbi.phase_list[i].timestamp = m_param.zero_gift_list[i].timestamp;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&dbi, sizeof(dbi));
}

void ZeroGift::SendZeroGiftGodCostumeInfo()
{
	Protocol::SCZeroGiftGodCostumeInfo info;

	UNSTD_STATIC_CHECK(sizeof(info.zero_gift_god_costume_info) == sizeof(m_param.god_costume_list));
	memcpy(info.zero_gift_god_costume_info, m_param.god_costume_list, sizeof(info.zero_gift_god_costume_info));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}

void ZeroGift::OnRoleUpLevel(int old_level, int level)
{
	if (level > old_level)
	{
		this->CheckZeroGiftOpen();
	}
}

void ZeroGift::CheckZeroGiftOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	int phase_cfg_count = LOGIC_CONFIG->GetZeroGiftCfg().GetPhaseNum();

	bool need_send = false;

	for (int i = 0; i < phase_cfg_count && i < ZERO_GIFT_PHASE_MAX; ++i)
	{
		if (ZeroGiftPhaseInfo::UN_ACTIVE_STATE != m_param.zero_gift_list[i].state) // 已经激活，不需要再计算
		{
			continue;
		}

		const ZeroGiftPhaseCfg* cfg_item = LOGIC_CONFIG->GetZeroGiftCfg().GetPhaseCfg(i);
		if (NULL == cfg_item)
		{
			continue;
		}
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
		if (opengame_day < cfg_item->open_day)
		{
			continue;
		}
		if (m_role->GetLevel() < cfg_item->active_level)
		{
			continue;
		}
		unsigned int open_server_zero_time = static_cast<unsigned int>(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime());
		unsigned int zero_time = static_cast<unsigned int>(GetZeroTime(open_server_zero_time + (cfg_item->open_day - 1) * SECOND_PER_DAY));
		m_param.zero_gift_list[i].state = ZeroGiftPhaseInfo::ACTIVE_STATE;
		m_param.zero_gift_list[i].timestamp = static_cast<unsigned int>(zero_time + cfg_item->vaild_buy_time);

		need_send = true;
	}

	if (need_send)
	{
		this->SendZeroGiftInfo();
	}
}

void ZeroGift::OnBuy(int seq)
{
	if (seq < 0 || seq >= ZERO_GIFT_PHASE_MAX) return;

	if (CrossConfig::Instance().IsHiddenServer())
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

	const ZeroGiftPhaseCfg *cfg_item = LOGIC_CONFIG->GetZeroGiftCfg().GetPhaseCfg(seq);
	if (NULL == cfg_item)
	{
		return;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	if (opengame_day < cfg_item->open_day || opengame_day >= cfg_item->open_day + ZeroGiftPhaseCfg::REWARD_DAY_COUNT)
	{
		m_role->NoticeNum(errornum::EN_ZERO_GIFT_BUY_DAY_LIMIT);
		return;
	}

	if (m_role->GetLevel() < cfg_item->level_limit)
	{
		m_role->NoticeNum(errornum::EN_ZERO_GIFT_BUY_LEVEL_LIMIT);
		return;
	}

	ZeroGiftPhaseInfo &phase = m_param.zero_gift_list[seq];

	if (ZeroGiftPhaseInfo::ACTIVE_STATE != phase.state)
	{
		return;
	}

	//unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	//if (now_sec > phase.timestamp)
	//{
	//	m_role->NoticeNum(errornum::EN_ZERO_GIFT_BUY_TIME_LIMIT);
	//	return;
	//}
	unsigned int open_server_zero_time = static_cast<unsigned int>(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime());
	unsigned int zero_time = static_cast<unsigned int>(GetZeroTime(open_server_zero_time + (cfg_item->open_day - 1) * SECOND_PER_DAY));		//此函数可能返回0
	if (zero_time <= 0)
	{
		return;
	}

	if (cfg_item->buy_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(cfg_item->buy_gold, "ZeroGiftBuy"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	phase.state = ZeroGiftPhaseInfo::HAD_BUY_STATE;
	phase.timestamp = zero_time;

	m_role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_BUY_ZERO_GIFT, seq);

	{
		// 传闻
	/*	if (0 == cfg_item->buy_gold)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_zero_gift,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_NOT_CHAT);
			}
		}*/
	}

	this->SendZeroGiftInfo();
	TianShuXunZhu::CheckShiXueTianShu(m_role, CHENGZHANG_TIANSHU_TYPE_ZEROGIFT);

	gamelog::g_log_other.printf(LL_INFO, "BuyZeroGift user[%d, %s], level[%d], use_gold[%d], seq[%d], reward_item_1[%s], reward_item_1[%s], reward_item_1[%s]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), cfg_item->buy_gold, seq, ItemConfigDataLog(&cfg_item->gift_item[0], NULL),
		ItemConfigDataLog(&cfg_item->gift_item[1], NULL), ItemConfigDataLog(&cfg_item->gift_item[2], NULL));
}

void ZeroGift::OnBuyGodCostume(int seq)
{
	if (seq < 0 || seq >= ZERO_GIFT_PHASE_MAX) return;

	if (CrossConfig::Instance().IsHiddenServer())
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

	const ZeroGiftGodCostumeCfg *cfg_item = LOGIC_CONFIG->GetZeroGiftCfg().GetGodCostumeCfg(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	int &buy_state = m_param.god_costume_list[seq].buy_state;
	if (ZeroGiftGodCostumeInfo::HAD_BUY_STATE == buy_state)
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	if (opengame_day < cfg_item->open_day || opengame_day >= cfg_item->open_day + ZERO_GIFT_GOD_LEVEL_PHASE)
	{
		m_role->NoticeNum(errornum::EN_ZERO_GIFT_BUY_DAY_LIMIT);
		return;
	}

	if (cfg_item->buy_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(cfg_item->buy_gold, "ZeroGiftBuyGodCostume"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	buy_state = ZeroGiftGodCostumeInfo::HAD_BUY_STATE;

	this->SendZeroGiftGodCostumeInfo();

	gamelog::g_log_other.printf(LL_INFO, "ZeroGiftBuyGodCostume user[%d, %s], level[%d], use_gold[%d], seq[%d], reward_item_1[%s], reward_item_1[%s], reward_item_1[%s]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), cfg_item->buy_gold, seq, ItemConfigDataLog(&cfg_item->gift_item[0], NULL),
		ItemConfigDataLog(&cfg_item->gift_item[1], NULL), ItemConfigDataLog(&cfg_item->gift_item[2], NULL));
}

void ZeroGift::OnFetchRewardGold(int seq)
{
	if (seq < 0 || seq >= ZERO_GIFT_PHASE_MAX)
	{
		return;
	}

	const ZeroGiftPhaseCfg * cfg_item = LOGIC_CONFIG->GetZeroGiftCfg().GetPhaseCfg(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	ZeroGiftPhaseInfo &phase = m_param.zero_gift_list[seq];

	if (ZeroGiftPhaseInfo::HAD_BUY_STATE != phase.state)
	{
		return;
	}

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (phase.timestamp <= 0 || now_sec < phase.timestamp)
	{
		m_role->NoticeNum(errornum::EN_ZERO_GIFT_FETCH_TIME_LIMIT);
		return;
	}

	if (0 != cfg_item->is_bind_gold)
	{
		m_role->GetKnapsack()->GetMoney()->AddGoldBind(cfg_item->reward_gold, "ZeroGiftReturnGold");
	}
	else
	{
		m_role->GetKnapsack()->GetMoney()->AddGold(cfg_item->reward_gold, "ZeroGiftReturnGold");
	}

	phase.state = ZeroGiftPhaseInfo::HAD_FETCH_STATE;
	phase.timestamp = 0;

	this->SendZeroGiftInfo();

	m_role->NoticeNum(noticenum::NT_WELFARE_GET_PLAT_VIP_GIFT_SUCCESS);
}

void ZeroGift::OnFetchRewardItem(int seq, int day_count)
{
	if (seq < 0 || seq >= ZERO_GIFT_PHASE_MAX)
	{
		return;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (day_count < 0 || day_count >= ZERO_GIFT_GOD_LEVEL_PHASE)
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

	const ZeroGiftPhaseCfg *cfg_item = LOGIC_CONFIG->GetZeroGiftCfg().GetPhaseCfg(seq);
	if (nullptr == cfg_item)
	{
		return;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	int day_diff = opengame_day - cfg_item->open_day;
	if (opengame_day < cfg_item->open_day)
	{
		m_role->NoticeNum(errornum::EN_ZERO_GIFT_BUY_DAY_LIMIT);
		return;
	}
	if (day_count > day_diff)
	{
		m_role->NoticeNum(errornum::EN_ZERO_GIFT_FETCH_TIME_LIMIT);
		return;
	}

	ZeroGiftPhaseInfo &phase = m_param.zero_gift_list[seq];

	if (phase.state < ZeroGiftPhaseInfo::HAD_BUY_STATE)
	{
		return;
	}

	if (ZeroGiftPhaseInfo::HAD_FETCH_STATE == phase.state)
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	if (0 != (m_param.fetch_flag_list[seq] & (1 << day_count)))
	{
		return;
	}

	/*unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (phase.timestamp <= 0 || now_sec < phase.timestamp + SECOND_PER_DAY * day_count)
	{
		m_role->NoticeNum(errornum::EN_ZERO_GIFT_FETCH_TIME_LIMIT);
		return;
	}*/

	

	//phase.state = ZeroGiftPhaseInfo::HAD_FETCH_STATE;
	m_param.fetch_flag_list[seq] |= (1 << day_count);

	m_role->GetKnapsack()->PutListOrMail(cfg_item->reward_item_list[day_count], PUT_REASON_ZERO_GIFT);

	this->SendZeroGiftInfo();

	gamelog::g_log_other.printf(LL_INFO, "FetchZeroGift user[%d, %s], level[%d], seq[%d], reward_item_1[%s], reward_item_1[%s], reward_item_1[%s]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, ItemConfigDataLog(&cfg_item->reward_item_list[day_count][0], nullptr),
		ItemConfigDataLog(&cfg_item->reward_item_list[day_count][1], nullptr), ItemConfigDataLog(&cfg_item->reward_item_list[day_count][2], nullptr));
}

void ZeroGift::OnFetchGodCostume(int seq, int level_phase)
{
	if (seq < 0 || seq >= ZERO_GIFT_PHASE_MAX)
	{
		return;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (level_phase < 0 || level_phase >= ZERO_GIFT_GOD_LEVEL_PHASE)
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

	if (m_param.god_costume_list[seq].buy_state != ZeroGiftGodCostumeInfo::HAD_BUY_STATE)
	{
		return;
	}

	int &reward_flag = m_param.god_costume_list[seq].reward_flag;
	if (0 != (reward_flag & (1 << level_phase)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}

	const ZeroGiftGodCostumeCfg *cfg_item = LOGIC_CONFIG->GetZeroGiftCfg().GetGodCostumeCfg(seq);
	if (nullptr == cfg_item)
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	if (opengame_day < cfg_item->open_day)
	{
		m_role->NoticeNum(errornum::EN_ZERO_GIFT_BUY_DAY_LIMIT);
		return;
	}

	if (m_role->GetLevel() < cfg_item->level_limit[level_phase])
	{
		m_role->NoticeNum(errornum::EN_ZERO_GIFT_GOD_COSTUME_FETCH_LEVEL_LIMIT);
		return;
	}

	reward_flag |= (1 << level_phase);

	m_role->GetKnapsack()->PutListOrMail(cfg_item->reward_item_list[level_phase], PUT_REASON_ZERO_GIFT_GOD_COSTUME);

	this->SendZeroGiftGodCostumeInfo();

	gamelog::g_log_other.printf(LL_INFO, "FetchGodCostume user[%d, %s], level[%d], seq[%d], level_phase[%d] reward_item_0[%s], reward_item_1[%s], reward_item_2[%s]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), seq, level_phase, ItemConfigDataLog(&cfg_item->reward_item_list[level_phase][0], nullptr),
		ItemConfigDataLog(&cfg_item->reward_item_list[level_phase][1], nullptr), ItemConfigDataLog(&cfg_item->reward_item_list[level_phase][2], nullptr));
}
