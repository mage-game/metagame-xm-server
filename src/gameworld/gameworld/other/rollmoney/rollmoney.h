#ifndef __ROLL_MONEY_H__
#define __ROLL_MONEY_H__

class Role;

const static int ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT = 3;
const static int ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT = 7;
const static int ROLL_MONEY_FETCH_GOLD_REWARD_MAX_TIEMS = 8;
const static int ROLL_MONEY_FETCH_COIN_REWARD_MAX_TIMES = 1;
const static int ROLL_MONEY_DAILY_ROLL_GOLD_MAX_TIMES = 3;

class RollMoney
{
public:
	RollMoney();
	~RollMoney();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, short gold_fetch_reward_times, short coin_fetch_reward_tims, char gold_roll_times, char coin_roll_times,
		char gold_num_0, char gold_num_1, char gold_num_2, char coin_num_0, char coin_num_1, char coin_num_2, char coin_num_3,
		char coin_num_4, char coin_num_5, char coin_num_6, char already_roll_gold_num);

	void GetInitParam(short *gold_fetch_reward_times, short *coin_fetch_reward_tims, char *gold_roll_times, char *coin_roll_times,
		char *gold_num_0, char *gold_num_1, char *gold_num_2, char *coin_num_0, char *coin_num_1, char *coin_num_2, char *coin_num_3,
		char *coin_num_4, char *coin_num_5, char *coin_num_6, char *already_roll_gold_num);

	void OnDayChange(int old_dayid, int now_dayid);
	void OnOperaReq(int opera_type);

private:
	void SendInfo();
	bool OnRollGoldNum();
	bool OnRollCoinNum();
	bool OnFetchGoldReward();
	bool OnFetchCoinReward();

	Role *m_role;

	int m_gold_roll_times;
	int m_coin_roll_times;
	int m_gold_fetch_reward_times;
	int m_coin_fetch_reward_timies;
	int m_day_roll_one_gold_times;	// 每天点单个元宝牌的次数

	char m_gold_roll_num_list[ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT];
	char m_coin_roll_num_list[ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT];
};

#endif


