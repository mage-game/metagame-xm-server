#ifndef __WABAO_HPP__
#define __WABAO_HPP__

class Role;

class Wabao
{
public:
	Wabao();
	~Wabao();

	void SetRole (Role *role) { m_role = role; }
	void Init(Role *role, int baozang_scene_id, int baozang_pos_x, int baozang_pos_y, int baotu_count, int wabao_reward_type, unsigned int fetch_flag, unsigned int shouhuzhe_time);
	void GetInitParam(int *baozang_scene_id, int *baozang_pos_x, int *baozang_pos_y, int *baotu_count, int *wabao_reward_type, unsigned int *fetch_flag, unsigned int *shouhuzhe_time);

	void OnDayChange(int old_dayid, int now_dayid);
	void OnOperaReq(int opera_type, short is_killed = 0);
	void OnFetchBaotuReward(int total_degree);
	void OnRoleLevelChange(int old_level, int level);
	
	void SendWabaoInfo();
	void Update(unsigned int now_second);
	void SetBaotuCount(int baotu_count) { m_baotu_count = baotu_count; }

private:

	void OnStart();
	void OnDig();
	void OnQuickComplete();
	void OnFetchReward(bool is_quick_completed = false);
	void OnShouhuzheOperaReq(short is_killed);

	Role *m_role;
	int m_baozang_scene_id;
	int m_baozang_pos_x;
	int m_baozang_pos_y;
	int m_baotu_count;
	int m_wabao_reward_type;
	unsigned int m_baotu_fetch_flag;		// 宝图领取标志
	unsigned int m_shouhuzhe_time;
};

#endif // __WABAO_HPP__

