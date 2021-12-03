#ifndef _SPECIAL_MESS_BATTLE_HPP_
#define _SPECIAL_MESS_BATTLE_HPP_

#include <map>
#include <vector>
#include "scene/speciallogic/speciallogic.hpp"

const static int MESS_BATTLE_MAX_RANK_COUNT = 16;

struct MessBattleRankInfo
{
	MessBattleRankInfo() : score(0), user_key(0) {}

	int score;
	GameName user_name;
	long long user_key;
};

struct PlayerHurtBossInfo
{
	PlayerHurtBossInfo() : hurt_val(0), user_key(0) {}

	GameName user_name;
	long long hurt_val;
	long long user_key;
};

struct MessBattleTotalScoreRank
{
	MessBattleTotalScoreRank() :total_score(0), user_key(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}
	int total_score;	//前n-1轮积分加当期轮积分
	GameName user_name;
	long long user_key;
};

class SpecialMessBattle : public SpecialLogic
{
	struct RoleFbInfo
	{
		RoleFbInfo() : consecutive_kill_num(0), score(0), total_score(0), kill_role_num(0), user_key(0), wudi_times(0)
		{
			memset(rank_list, -1, sizeof(rank_list));
		}

		int consecutive_kill_num;
		int score;
		int total_score; //前n-1轮积分
		int kill_role_num;
		long long user_key;
		int rank_list[MESS_BATTLE_MAX_RANK_COUNT];
		int wudi_times;
	};

	struct RoleRewardList
	{
		long long user_key;
		int rank_list[MESS_BATTLE_MAX_RANK_COUNT];
	};

public:
	SpecialMessBattle(Scene *scene);
	virtual ~SpecialMessBattle();

	virtual bool CanDestroy();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);

	void OnActivityStatusChange(int status);
	void OnActivityStart();
	void OnActivityClose();
	void SetKickOutTime();

private:
	void Init();
	void SortRank();
	void SortHurtRank();
	void UpdateRank();
	void GiveReward();
	bool IsInCrossServer();
	void InitBoss();
	long long GetRoleKey(Role *role);
	Role * GetRole(long long user_key);
	ObjID GetObjId(long long user_key);
	int GetRoleRankByRoleKey(long long role_key);
	int GetRoleTotalRankByRoleKey(long long role_key);

	void SendInfoToRole(Role *role);
	void KickAllToOrginalToServer();
	void SendAllRoleScoreInfo();
	void SendRoleRewardInfo(Role *role);
	void UpdateUser(RoleFbInfo &user_info);
	void AddSpecialBuff(Role *role, int ebt_type, bool is_add);
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);
	void CheckRewardOnClose();

	int m_turn;
	bool m_is_init_finish;
	std::vector<MessBattleRankInfo> m_rank_vec;					// 积分排行
	unsigned int m_next_redistribute_time;
	unsigned int m_next_update_rank_time;
	unsigned int m_kick_out_time;
	unsigned int m_next_update_role_time;
	unsigned int m_last_check_time;														// 上次检查时间戳

	typedef std::map<long long, RoleFbInfo> PlayerInfoMap;
	typedef std::map<long long, RoleFbInfo>::iterator PlayerInfoMapIt;
	PlayerInfoMap m_role_info_map;

	ObjID m_boss_obj_id;
	long long m_boss_max_hp;									// boss总血量
	long long m_boss_hp;										// boss当前血量
	std::vector<PlayerHurtBossInfo> m_hurt_boss_info_vec;		// 对boss伤害信息

	int m_last_relive_pos;

	typedef std::map<long long, RoleRewardList> PlayerRewardRecordMap;
	typedef std::map<long long, RoleRewardList>::const_iterator PlayerRewardRecordMapIt;
	PlayerRewardRecordMap m_player_reward_record_map;

	std::vector<MessBattleTotalScoreRank> m_total_score_rank_vec; //总积分排行榜
};

#endif // _SPECIAL_MESS_BATTLE_HPP_
