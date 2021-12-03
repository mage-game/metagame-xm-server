#include "monthcard.h"
#include "obj/character/role.h"
#include "engineadapter.h"
#include "other/route/mailroute.hpp"
#include "servercommon/string/gameworldstr.h"
#include "gameworld/world.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "item/knapsack.h"

MonthCard::MonthCard() : m_role(NULL), m_active_timestamp(0), m_last_days(0), m_next_reward_day_idx(0), m_buy_card_times(0)
{

}

MonthCard::~MonthCard()
{

}

void MonthCard::Init(Role *role, unsigned int active_timestamp, int last_day, int next_reward_day_idx, int buy_card_times)
{
	m_role = role;
	m_active_timestamp = active_timestamp;
	m_last_days = last_day;
	m_next_reward_day_idx = next_reward_day_idx;
	m_buy_card_times = buy_card_times;
}

void MonthCard::GetInitParam(unsigned int *active_timestamp, int *last_day, int *next_reward_day_idx, int *buy_card_times)
{
	*active_timestamp = m_active_timestamp;
	*last_day = m_last_days;
	*next_reward_day_idx = m_next_reward_day_idx;
	*buy_card_times = m_buy_card_times;
}

void MonthCard::OnBuyCard()
{
	++ m_buy_card_times;

	gamelog::g_log_month_card.printf(LL_INFO, "OnBuyMonthCard user[%d, %s] role_level[%d], buy_times[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), m_buy_card_times);

	ROLE_LOG_QUICK6(LOG_TYPE_MONTH_CARD, m_role, m_buy_card_times, 0, NULL, NULL);

	if (m_active_timestamp > 0)			// 月卡已经改为一次购买，终身拥有
	{
		return;
	}

	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_month_card_buy_notice,
		UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp());

	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
	}

	if (m_active_timestamp > 0)			// 当前已经有激活的月卡，就延期
	{
		m_last_days += MONTH_CARD_LAST_DAYS;
	}
	else								// 当前没有激活的月卡
	{
		m_active_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_last_days = MONTH_CARD_LAST_DAYS;
	}

	this->SendInfo();
}

void MonthCard::OnLogin()
{
	this->SendInfo();
}

void MonthCard::OnDayChange(int old_dayid, int now_dayid)
{
	this->SendInfo();
}

void MonthCard::SendInfo()
{
	Protocol::SCMonthCardInfo mci;
	mci.active_timestamp = m_active_timestamp;
	mci.reserve_int = 0;
	mci.next_reward_day_idx = m_next_reward_day_idx;
	mci.buy_times = m_buy_card_times;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&mci, sizeof(mci));
}

void MonthCard::FetchDayReward()
{
	if (m_last_days <= 0 || m_active_timestamp <= 0 || m_buy_card_times <= 0)
	{
		m_role->NoticeNum(errornum::EN_MONTH_CARD_HAS_NOT_ACTIVITY_FETCH);
		return;
	}

	// 有奖励可发
	if (m_next_reward_day_idx < m_last_days)
	{
		time_t now = EngineAdapter::Instance().Time();
		int day_index = GetDayIndex(static_cast<time_t>(m_active_timestamp), now); // 算出当前是月卡激活后的第几天

		if (day_index < m_next_reward_day_idx)
		{
			m_role->NoticeNum(errornum::EN_MONTH_CARD_REWARD_HAS_FETCH);
			return;
		}

		m_next_reward_day_idx = day_index + 1; // 下次要到明天才能发奖励

		m_role->GetKnapsack()->GetMoney()->AddGoldBind(MONTH_CARD_REWARD_BIND_GOLD_PER_DAY, "MonthCardReward");
		m_role->NoticeNum(noticenum::NT_MONTH_CARD_REWARD_FETCH_SUCCESS);
	}

	// 奖励天数完了，就重置数据
	if (m_next_reward_day_idx >= m_last_days)
	{
		m_active_timestamp = 0;
		m_last_days = 0;
		m_next_reward_day_idx = 0;
	}

	this->SendInfo();
}


