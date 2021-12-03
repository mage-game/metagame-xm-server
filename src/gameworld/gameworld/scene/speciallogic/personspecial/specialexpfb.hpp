#ifndef __SPECIAL_EXPFB_HPP__
#define __SPECIAL_EXPFB_HPP__

#include "scene/speciallogic/speciallogic.hpp"

class SpecialExpFB : public SpecialLogic
{
public:

	// 队员
	struct MemberItem
	{
		MemberItem () 
		{
			this->Reset();
		}

		void Reset()
		{
			uid = 0;
			exp = 0;
			record_max_exp = 0;
			buff_times = 0;
		}
		int uid;
		long long exp;
		long long record_max_exp;
		char buff_times;
	};

	SpecialExpFB(Scene *scene);
	virtual ~SpecialExpFB();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool OnTimeout();
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual void OnAddExp(Role *role, long long add_exp, int reason) override;
	
	void CreateWaveMonsters();
	void OnPayBuff(Role *role);
	void OnAutoPayBuff(Role *role);
	void OnPayBuffReq(Role *role, bool is_auto);
	void AddBuff(Role *role);
	SpecialExpFB::MemberItem *GetMemberItem(int uid);

protected:

	void OnExpFBFinish();					// 副本结束
	void SyncFBSceneLogicInfo(int uid, bool is_leave = false);		// 副本信息
	void SyncFBSceneLogicInfoAll();

	bool m_is_finish;						// 是否结束
	bool m_is_pass;							// 是否通关

	time_t m_timeout_time;
	time_t m_start_time;
	int m_total_allmonster_num;
	int m_kill_allmonster_num;
	int m_wave;
	int m_wave_refresh_count;				// 每波刷新次数
	int m_average_level;
	int m_member_num;
	unsigned long m_next_refresh_monster_time;
	int m_is_first_enter_enter;
	int m_add_monster_maxhp_per;								// 增加怪物血量百分比
	int m_add_monster_gongji_per;								// 增加怪物攻击百分比
	int m_add_monster_fangyu_per;								// 增加怪物防御百分比

	std::map<int, MemberItem *> m_member_map;
	
	unsigned int m_pass_kick_out_time;
};

#endif // __SPECIAL_EXPFB_HPP__


