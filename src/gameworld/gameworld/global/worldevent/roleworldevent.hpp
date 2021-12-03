#ifndef __ROLE_WORLD_EVENT_HPP__
#define __ROLE_WORLD_EVENT_HPP__

class Role;

#include "servercommon/worldeventdef.hpp"

class RoleWorldEvent
{
public:
	RoleWorldEvent();
	~RoleWorldEvent();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, int fetch_reward_flag, int find_npc_score, int kill_rand_boss_score, int shenmozhixi_score, int xukongzhimen_score, 
		int quanmintafang_score, int jiqingpk_score, int tiannvsanhua_score, short find_npc_flag, char light_word_flag, char curr_find_npc_id);
	void GetInitParam(int *fetch_reward_flag, int *find_npc_score, int *kill_rand_boss_score, int *shenmozhixi_score, int *xukongzhimen_score, 
		int *quanmintafang_score, int *jiqingpk_score, int *tiannvsanhua_score, short *find_npc_flag, char *light_word_flag, char *curr_find_npc_id);

	void OnDayChange(int old_dayid, int now_dayid);
	void AddScore(int event_type, int score);
	void SetFindNpc(int npc_index); 
	int GetCurrFindNpcId() { return m_curr_find_npc_id; }
	void FetchReward();
	void LightWord();

	void SendInfo();

private:
	Role *m_role;

	void CheckLightWorld();

	char m_light_word_flag;
	short m_find_npc_flag;
	char m_curr_find_npc_id;
	int m_fetch_reward_flag;
	int m_score_list[WORLD_EVENT_TYPE_MAX];
};

#endif // __ROLE_WORLD_EVENT_HPP__

