#ifndef _SPECIAL_NIGHT_FIGHT_HPP_
#define _SPECIAL_NIGHT_FIGHT_HPP_

#include <map>
#include <vector>
#include "scene/speciallogic/speciallogic.hpp"

const static int NIGHT_FIGHT_MAX_RANK_COUNT = 16;

struct ScoreRankInfo
{
	ScoreRankInfo() : score(0), obj_id(INVALID_OBJ_ID), is_red_side(0), user_key(0) {}

	int score;
	ObjID obj_id;
	short is_red_side;
	GameName user_name;
	long long user_key;
};

struct TotalScoreRank
{
	int total_score;
	GameName user_name;
	long long user_key;
};

class SpecialNightFight : public SpecialLogic
{
	struct RoleFbInfo
	{
		RoleFbInfo() :is_login_out(false), is_red_side(false), score(0),total_score(0),kill_role_num(0), user_key(0), next_redistribute_time(0)
		{
			memset(rank_list, -1, sizeof(rank_list));
		}

		bool is_login_out;
		bool is_red_side;
		int score;
		int total_score;
		int kill_role_num;
		long long user_key;
		unsigned int next_redistribute_time;	// 下次重新分配的时间
		int rank_list[NIGHT_FIGHT_MAX_RANK_COUNT];
	};

	struct RoleRewardList
	{
		long long user_key;
		int rank_list[NIGHT_FIGHT_MAX_RANK_COUNT];
	};

	struct HurtBossInfo
	{
		HurtBossInfo() : user_key(0), is_red_side(false), hurt_per(0), hurt_val(0)
		{
			memset(user_name, 0, sizeof(user_name));
		}

		bool operator <(const HurtBossInfo &other) const
		{
			return hurt_val > other.hurt_val;
		}

		long long user_key;
		bool is_red_side;
		GameName user_name;
		int hurt_per;
		Attribute hurt_val;
	};

public:
	SpecialNightFight(Scene *scene);
	virtual ~SpecialNightFight();

	virtual bool CanDestroy();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);

	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	
	void OnActivityStatusChange(int status);
	void OnActivityStart();
	void OnActivityClose();
	void OnGetPlayerPosi(Role *role, int rank);

private:
	void Init();

	void SortScoreRank();			// 积分排序
	void UpdateScoreRank();			// 刷新积分排行
	void GiveScoreRankReward();		// 发放积分排行

	bool IsInCrossServer();
	long long GetRoleKey(Role *role);
	Role * GetRole(long long user_key);
	int GetRoleRankByRoleKey(long long role_key);
	int GetRoleTotalRankByRoleKey(long long role_key);
	ScoreRankInfo * GetRoleRankInfoByRoleKey(long long role_key);

	void CheckFlushBoss();  // 刷BOSS(如果BOSS尚未死亡则强制死亡，无击杀者)
	void CheckGiveScoreOnBossDie(bool is_red_side);	// 击杀boss奖励积分
	void BroadcastBossDie(Role *killer_role, int boss_id, bool is_red_side);
	void CheckClearBossRank();		// 清除伤害boss排行
	void CalcRoleHurtBossVal(Role *role, Attribute delta_hp);		// 计算boss伤害
	void SortBossHurtRank();		// boss伤害排序

	void CheckRedistribute(int next_time);  // 重新分配阵营身份

	void SendInfoToRole(Role *role);
	void SendRedSideListInfo();
	void SendAllRoleScoreInfo();
	void SendHurtBossRankInfo(Role *role = nullptr);
	void CheckRewardOnClose();

	void SetKickOutTime();
	void KickAllToOrginalToServer();

	int m_is_finish;								// 是否结束副本
	int m_turn;										// 轮数
	bool m_is_init_finish;							// 是否初始化完成
	int m_red_side_num;								// 红方阵营数量
	int m_blue_side_num;							// 蓝方阵营数量

	ObjID m_boss_objid;								// boss的objid
	int m_boss_left_hp_per;							// boss剩余血量万分比
	Attribute m_boss_max_hp;						// boss最大血量

	unsigned int m_next_redistribute_time;			// 下次重新分配阵营时间戳
	unsigned int m_next_get_score_time;				// 下次获取积分时间戳
	unsigned int m_next_update_rank_time;			// 下次更新排行时间戳
	unsigned int m_kick_out_time;					// 踢出时间戳
	unsigned int m_next_update_role_time;			// 刷新人物信息时间戳

	std::vector<ScoreRankInfo> m_score_rank_vec;	// 每轮积分排行	
	std::vector<HurtBossInfo> m_hurt_boss_info_vec;	// boss伤害排行
	std::vector<TotalScoreRank> m_total_score_vec;	// 总积分排行

	typedef std::map<long long, RoleFbInfo> PlayerInfoMap;
	typedef std::map<long long, RoleFbInfo>::iterator PlayerInfoMapIt;
	PlayerInfoMap m_role_info_map;

	typedef std::map<long long, RoleRewardList> PlayerRewardRecordMap;
	typedef std::map<long long, RoleRewardList>::const_iterator PlayerRewardRecordMapIt;
	PlayerRewardRecordMap m_player_reward_record_map;
};

#endif // _SPECIAL_NIGHT_FIGHT_HPP_
