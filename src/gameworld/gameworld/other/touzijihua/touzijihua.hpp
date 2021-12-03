#ifndef __TOUZIJIHUA_HPP__
#define __TOUZIJIHUA_HPP__

#include "config/touzijihuaconfig.hpp"

class Role;

class Touzijihua
{
public:
	Touzijihua();
	~Touzijihua();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, char active_plan_0, char active_plan_1, char active_plan_2, char active_plan_3, char active_highest_plan, short plan_0_reward_flag, short plan_1_reward_flag,
		short plan_2_reward_flag, short plan_3_reward_flag, unsigned int buy_time, int reward_flag, int vip_reward_flag, int reward_gold_bind_flag, char first_reward_flag, const char foundation_next_reward_index[TouzijihuaConfig::MAX_FOUNDATION_TYPE],
		short active_fb_plan,short fb_plan_reward_flag,short active_boss_plan,short boss_plan_reward_flag,short kill_boss_num, short kill_shenyu_boss_num, short active_shenyu_boss_plan, short shenyu_boss_plan_reward_flag);
	void GetInitParam(char *active_plan_0, char *active_plan_1, char *active_plan_2, char *active_plan_3, char *active_highest_plan, short *plan_0_reward_flag, short *plan_1_reward_flag,
		short *plan_2_reward_flag, short *plan_3_reward_flag, unsigned int *buy_time, int *reward_flag, int *vip_reward_flag, int *reward_gold_bind_flag, char *first_reward_flag, char foundation_next_reward_index[TouzijihuaConfig::MAX_FOUNDATION_TYPE],
		short *active_fb_plan, short *fb_plan_reward_flag, short *active_boss_plan, short *boss_plan_reward_flag, short *kill_boss_num, short *kill_shenyu_boss_num, short *active_shenyu_boss_plan, short *shenyu_boss_plan_reward_flag);

	void OnRoleLogin();
	void SendInfo();

	void OnFetchReward(int plan_type, int seq);
	bool CalcPlanReward(int plan_type, int seq, int *reward_gold, int *reward_bind_gold);
	void OnBuyPlan(int plan_type);

	void OnNewFetchReward(bool is_vip, int seq);
	void OnNewFetchFirstReward();
	void OnNewBuy();

	void GmReset();
	void GmAddDay(int day);

	bool OnBuyFoundation(int reward_index);
	void OnFetchFoundationReward(int reward_index);

	void OnBuyFbPlan();
	void OnFetchFbPlanReward(int index,bool is_buy =false);
	void OnBuyBossPlan();
	void OnFetchBossPlanReward(int index, bool is_buy = false);
	void SendFbBossInfo();
	void OnPassFb();
	void CheckActiveFbReward();
	void OnKillBoss();
	void CheckActiveBossReward();

	// 神域boss投资
	void OnBuyShenyuBossPlan();
	void OnFetchShenyuBossPlanReward(int index, bool is_buy = false);
	void OnKillShenyuBoss();
	void CheckActiveShenyuBossReward();

private:
	Role *m_role;

	char m_active_plan_0;
	char m_active_plan_1;
	char m_active_plan_2;
	char m_active_plan_3;
	char m_active_highest_plan;

	short m_plan_0_reward_flag;
	short m_plan_1_reward_flag;
	short m_plan_2_reward_flag;
	short m_plan_3_reward_flag;

	// 新投资计划
	unsigned int m_buy_time;
	int m_reward_flag;
	int m_vip_reward_flag;
	int m_reward_gold_bind_flag;
	char m_new_plan_first_reward_flag;

	char m_foundation_next_reward_index[TouzijihuaConfig::MAX_FOUNDATION_TYPE];

	short m_active_fb_plan;
	short m_fb_plan_reward_flag;

	short m_active_boss_plan;
	short m_boss_plan_reward_flag;
	short m_kill_boss_num;

	short m_kill_shenyu_boss_num = 0;
	short m_active_shenyu_boss_plan = 0;
	short m_shenyu_boss_plan_reward_flag = 0;

};

#endif


