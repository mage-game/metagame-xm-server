#ifndef __MONTH_CARD_HPP__
#define __MONTH_CARD_HPP__

const static int MONTH_CARD_LAST_DAYS = 999999999;
const static int MONTH_CARD_REWARD_BIND_GOLD_PER_DAY = 100;

class Role;

class MonthCard
{
public:
	MonthCard();
	~MonthCard();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, unsigned int active_timestamp, int last_day, int next_reward_day_idx, int buy_card_times);
	void GetInitParam(unsigned int *active_timestamp, int *last_day, int *next_reward_day_idx, int *buy_card_times);

	void OnBuyCard();
	void OnLogin();
	void OnDayChange(int old_dayid, int now_dayid);
	void SendInfo();
	void FetchDayReward();

private:
	Role *m_role;
	unsigned int m_active_timestamp;
	int m_last_days;
	int m_next_reward_day_idx;
	int m_buy_card_times;
};

#endif // __MONTH_CARD_HPP__

