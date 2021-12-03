#ifndef __SPECIAL_NATIONBOSS_HPP__
#define __SPECIAL_NATIONBOSS_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"

#include <vector>
#include <map>

class Role;

class SpecialNationalBoss : public SpecialLogic
{
public:
	SpecialNationalBoss(Scene *scene);
	virtual ~SpecialNationalBoss();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual bool CanDestroy() { return false; }

	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual bool OnTimeout() { return true; }

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);
	virtual bool SpecialCanMountOn(Role *role){return false;}

	void OnActivityStatusChange(int status);

	void OnBuyBuff(Role *role);

	bool IsFinish() { return m_is_finish; }

protected:
	struct NBUser
	{
		NBUser() : obj_id(INVALID_OBJ_ID), uid(0), camp(0), level(0), hurt(0), add_gongji_percent(0), rank_pos(0), buy_times(0)
		{
			memset(user_name, 0, sizeof(user_name)); 
		}

		ObjID obj_id;
		int uid;
		char camp;
		int level;
		long long hurt;
		int add_gongji_percent;
		short rank_pos;
		short buy_times;
		GameName user_name;
	};

	NBUser * GetUser(int uid);
	Role * GetRole(NBUser *user);

	void RecalcBuff(Role *role, int add_percent);

	void SendUserInfo(Role *role);
	void SendRankInfo(Role *role = NULL); 

	void NotifySystemMsg(char *str_buff, int str_len, int msg_type = SYS_MSG_CENTER_NOTICE);

	virtual void OnAllMonsterDead();
	void RefreshAll();
	void RefreshDPSRank();

	void OnActivityStart();
	void OnActivityClose();

	void OnFinish();

	const static int MONSTER_FLUSH_MAX_TIMES = 6;

	bool m_is_finish;
	time_t m_kickout_user_timestamp;

	time_t m_next_flush_time;
	int m_flush_times;
	long long m_total_bosshp;
	int m_world_level;

	time_t m_next_refresh_dps_rank_time;						
	std::vector<NationalBossRankItem> m_dps_rank_vec;				

	std::map<int, NBUser> m_user_map;

	Protocol::SCNationalBossRankInfo m_nbri;
};

#endif // 


