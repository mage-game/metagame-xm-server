#ifndef __SPECIAL_LOVERS_FB_HPP__
#define __SPECIAL_LOVERS_FB_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "servercommon/qingyuandef.hpp"


class SpecialLoversFB : public SpecialLogic
{
public:
	SpecialLoversFB(Scene *scene);
	~SpecialLoversFB();

	virtual void OnRoleEnterScene(Role *role);
	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool SpecialCanGuWu() { return true; } 

	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool OnTimeout();
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	bool IsOpenSceneRole(const UserID &user_id) { return user_id == m_open_scene_userid; }
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);

	void OnBuyBuff(Role *role);
	void OnBuyDoubleReward(Role *role);

private:
	virtual void OnAllMonsterDead();

	void SendInfo();
	void FBFinish(bool is_pass);
	Role * GetOwnerRole(ObjID role_obj);
	void AddRewardRecord(ItemConfigData *reward_item, char sex);
	void SendRewardRecordInfo();
	void SendFailRewardInfo(Role *role);  // 用于玩家中途退出

	UserID m_open_scene_userid;

	bool m_is_finish;
	bool m_is_pass;
	bool m_is_buff;
	int m_curr_wave;
	int m_wave_reward_mark;
	int m_max_wave_count;
	int m_buy_buff_times;
	int m_buff_failure_timestamp;
	bool m_male_is_buy_double_reward;
	bool m_female_is_buy_double_reward;
	time_t m_next_refresh_monster_time;
	time_t m_kick_out_timestamp;
	time_t m_per_wave_remain_time;

	int m_total_get_uplevel_stuffs;
	long long m_total_get_exps;
	
	std::vector<ItemConfigData> m_reward_item_record_vec_men;
	std::vector<ItemConfigData> m_reward_item_record_vec_woman;

	ObjID m_joiner_objid_list[QINGYUAN_FB_TEAM_MEMBER_COUNT];
	UserID m_joiner_userid_list[QINGYUAN_FB_TEAM_MEMBER_COUNT];
	int m_add_qingyuan_value_list[QINGYUAN_FB_TEAM_MEMBER_COUNT];
	int m_refresh_monster_role_level;
};
#endif

