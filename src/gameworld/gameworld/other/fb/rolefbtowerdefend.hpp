#ifndef _ROLEFBTOWERDEFEND_HPP_
#define _ROLEFBTOWERDEFEND_HPP_

#include "other/fb/towerdefendconfig.hpp"
#include "servercommon/fbdef.hpp"
#include "monster/monsterinitparam.h"
#include "servercommon/struct/itemlistparam.h"
#include "protocal/msgfb.h"

class Role;

class RoleFBTowerDefend
{
public:
	RoleFBTowerDefend();
	~RoleFBTowerDefend();

	void SetRole(Role *role) { m_role = role; }

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnLogin();

	void Init(Role *role, const TowerDefendFBParam &param);
	void GetInitParam(TowerDefendFBParam *param);

	void SendInfo();

	bool PersonalFBCanEnter(int level);
	void PersonalFBAutoFB(int level);
	void OnPersonalFBEnter();
	void OnPersonalFBFinish(bool is_pass, int scene_id, int star_level);
	void PersonalBuyJoinTimes();
	bool PersonalItemAddJoinTimes(int times);

	bool PersonalHolidayGuardFBCanEnter();
	void OnPersonalHolidayGuardFBEnter();
	void SetPersonalHolidayGuardFBKillMonsterCount(short kill_count);
	short GetPersonalHolidayGuardFBKillMonsterCount() { return m_td_param.personal_holiday_guard_kill_monster; }
	void OnRefreshHolidayGuardFBKillMonsterCount() { m_td_param.personal_holiday_guard_kill_monster = 0; this->SendInfo(); }
	int GetPersonalHolidayGuardFBEnterCount() { return m_td_param.personal_holiday_guard_join_times; }
	int GetPersonalFBPassLevel() { return m_td_param.personal_max_pass_level; }
	int GetPersonalFbEnterTimes() { return m_td_param.personal_join_times; }
	int GetDailyFindRewardTeam(int list_num_max,ItemConfigData * reward_list, int * xiannv_shengwu);

	// 组队守护相关
	void LeaderSetAttrType(int uid, char type);
	void BroadcastAttrType();
	void OnJoinTeam();
	void OnLeaveTeam();
	void SetTeamAttrType(char type);
	void SetTeamMemberMinWave(short min_wave);
	short GetTeamMemberMinWave();
	void GiveTeamWaveReward(ItemConfigData *item_list,int max_num,int & angry_value);
	char GetTeamAttrType() { return m_td_param.team_attrtype; }
	const TowerDefendFBParam* GetTowerDefFBParam(int level) const;
	void SetTeamPassedWave(int wave);
	int GetTeamPassdeWave() { return m_td_param.team_max_wave_passed; }

	void FetchDoubleReward(int oper_type);
	bool CheckSceneFetchDoubleReward();

	int GetTodayTDMultipleRewardTimes() { return m_td_param.today_multiple_buy_times; }					// 组队守护本今天多倍奖励购买次数
	void AddTodayTDMultipleRewardTimes(char times) { m_td_param.today_multiple_buy_times += times; }	// 组队守护本今天多倍奖励购买次数

private:
	void AutoFbDrophelper(const TowerDefendWaveCfg &wave_cfg, ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT], 
		int *index_count, long long *reward_exp, int *reward_coin, int *reward_yuanli);
	
	Role *m_role;
	TowerDefendFBParam m_td_param;
};

#endif
