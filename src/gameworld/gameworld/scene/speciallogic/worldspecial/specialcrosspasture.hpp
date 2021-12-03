#ifndef _SPECIAL_CROSS_PASTURE_HPP_
#define _SPECIAL_CROSS_PASTURE_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <set>
#include "servercommon/crossdef.hpp"
#include "servercommon/serverdef.h"

class Role;

struct CrossPasturePlayerInfo
{
	struct PastureSkillItem 
	{
		PastureSkillItem () : skill_id(0), last_perform_time(0) {}

		bool Invalid() const { return 0 == skill_id; }
		void Reset() { skill_id = 0; last_perform_time = 0; }

		UInt16 skill_id;
		unsigned int last_perform_time;
	};

	CrossPasturePlayerInfo() : uuid(INVALID_UNIQUE_USER_ID), score(0), cur_cross_get_score(0), get_score_times(0), cur_cross_get_score_times(0), last_relive_time(0)
	{
		memset(name, 0, sizeof(name));
	}

	UniqueUserID uuid;
	GameName name;															// 角色名
	int score;																// 当前总积分
	int cur_cross_get_score;												// 当前次跨服获得积分
	int get_score_times;													// 获取积分的总次数
	int cur_cross_get_score_times;											// 当前次跨服获取积分的次数
	unsigned int last_relive_time;											// 上一次复活的时间戳

	PastureSkillItem skill_list[CROSS_PASTURE_SKILL_CFG_MAX_COUNT];			// 角色在当前场景里的额外技能信息
};

struct CrossPastureMonster
{
	CrossPastureMonster() : monster_id(0), score(0), max_num(0), cur_num(0), next_refresh_time(0) {}

	int monster_id;															// 怪物id
	int score;																// 拉进农场获得积分
	int max_num;															// 最大数量
	int cur_num;															// 当前数量
	unsigned int next_refresh_time;											// 该ID的怪物下次刷新时间
};
struct CrossPastureRankItem													// 群仙乱斗 排行榜信息
{
	CrossPastureRankItem() : uuid(INVALID_UNIQUE_USER_ID), score(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UniqueUserID uuid;
	GameName user_name;																// 角色名
	int score;																		// 总积分
};

class SpecialCrossPasture : public SpecialLogic
{
	static const int PLAYER_FIRST_ENTER_ADD_SCORE = 0;					// 玩家第一次进入赠送积分

public:
	SpecialCrossPasture(Scene *scene);
	virtual ~SpecialCrossPasture();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnAddObj(Obj *obj);
	virtual void OnRemoveObj(Obj *obj);

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialCanPerformSkill(Role *role) { return m_activity_open; }
	virtual bool SpecialCanSupplyEffect(Role *role, int supply_type) { return m_activity_open; }
	virtual long long SpecialRecalcInjure(Character *attacker, Character *injurer, long long injure);
	virtual bool PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id);

	virtual int GetSpecialParam(Role *role);

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool CanDestroy() { return false; }
	virtual bool SpecialIsCostMp(Role *role) { return false; }

	void OnActivityOpen();
	void OnActivityClose();

	void ClearRankTitle();

	void RankItemReward();

protected:
	void SendPlayerInfoToPlayer(Role *role);
	void SendPlayerInfoToPlayer(ObjID obj_id);

	void NotifySystemMsgToScene(char *str_buff, int str_len, int notice_type);
	void SyncPlayerInfoToCross(Role *role);

	Role * GetRole(ObjID obj_id);
	CrossPasturePlayerInfo * GetPlayerInfo(UniqueUserID uuid);
	CrossPasturePlayerInfo * GetPlayerInfo(ObjID obj_id);
	void FixMonsterNum(int monster_id);
	void FixSPMonsterNum(int monster_id, time_t now_second);

	void CheckPastureUserRank();
	void SendPastureRankToUser(Role*role=NULL);
	bool m_activity_open;

	time_t m_activity_end_time;												// 活动结束时间（防止cross没通知过来）

	typedef std::map<UniqueUserID, CrossPasturePlayerInfo> UserInfoMap;
	typedef std::map<UniqueUserID, CrossPasturePlayerInfo>::iterator UserInfoMapIt;
	UserInfoMap m_user_info_cache;											// 玩家信息缓存，包括离线玩家

	std::map<int, CrossPastureMonster> m_monster_info_map;

	unsigned int m_last_check_animal_status_time;

	std::map<UniqueUserID, unsigned int> m_role_last_die_time_map;			// 记录玩家上次死亡时间，N秒后才能满血复活

	time_t m_pasture_last_rank_timestamp;													// 最后一次检查排行榜时间戳
	std::vector<CrossPastureRankItem> m_pasture_rank_vec;
	time_t m_pasture_next_refresh_specialmonster_timestamp;									// 下一次刷新speaial monster的时间戳

	int special_monster = 0;
	Posi special_monster_pos;
};


#endif // _SPECIAL_CROSS_1V1_HPP_

