#ifndef __SPECIAL_CROSS_DARK_NIGHT_HPP__
#define __SPECIAL_CROSS_DARK_NIGHT_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "servercommon/crossdef.hpp"

class Role;

static const int CROSS_DARK_NIGHT_SYNC_RANK_INTERVAL = 5;				//同步排行榜到玩家间隔
static const int CROSS_DARK_NIGHT_BROADCAST_RANK_INTERVAL = 60;			//排行第一播报间隔
static const int CROSS_DARK_NGHT_SYNC_BOSS_INFO_INTERVAL = 1;

struct HurtItemInfo
{
	HurtItemInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		uuid = INVALID_UNIQUE_USER_ID;
	}

	UniqueUserID uuid;
};

struct CrossDarkNightPlayerInfo
{
	CrossDarkNightPlayerInfo() : uuid(INVALID_UNIQUE_USER_ID), obj_id(INVALID_OBJ_ID), box_count(0), score(0), score_reward_flag(0), total_reward_box_count(0)
	{
		memset(name, 0, sizeof(name));
	}

	UniqueUserID uuid;
	ObjID obj_id;
	GameName name;
	int box_count;
	int score;
	int score_reward_flag;
	int total_reward_box_count;

	std::map<UniqueUserID, HurtItemInfo> hurt_map;
	ItemConfigData reward_item[CROSS_DARK_NIGHT_BOSS_MAX_COUNT];
	ItemConfigData reward_item2[CROSS_DARK_NIGHT_BOSS_MAX_COUNT];
};

struct CrossDarkNightRankItem
{
	CrossDarkNightRankItem() : uuid(INVALID_UNIQUE_USER_ID), rank_val(0)
	{
		memset(game_name, 0, sizeof(game_name));
	}

	UniqueUserID uuid;
	GameName game_name;
	int rank_val;
};

enum DARKNIGHT_BOSSSTATUS
{
	DARKNIGHT_BOSSSTATUS_NONEXIST = 0,								// 不存在
	DARKNIGHT_BOSSSTATUS_EXIST,										// 存在
};

struct CrossDarkNightBossInfo
{
	CrossDarkNightBossInfo() :monster_id(0),boss_status(DARKNIGHT_BOSSSTATUS_NONEXIST),cur_boss_index(0),posi(0, 0),max_hp(0),cur_hp(0)
	{
	}

	void Reset()
	{
		monster_id = 0;
		boss_status = DARKNIGHT_BOSSSTATUS_NONEXIST;
		cur_boss_index = 0;
		posi = Posi(0, 0);
		max_hp = 0;
		cur_hp = 0;
		hurt_map.clear();
	}

	int monster_id;
	char boss_status;
	int cur_boss_index;
	Posi posi;
	long long max_hp;
	long long cur_hp;

	std::map<UniqueUserID, HurtItemInfo> hurt_map;
};

class SpecialCrossDarkNight : public SpecialLogic
{
public:
	SpecialCrossDarkNight(Scene *scene);
	virtual ~SpecialCrossDarkNight();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnCharacterDie(Character *dead_cha, ObjID killer_objid);

	virtual void OnAddObj(Obj *obj);

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool CanDestroy() { return is_finsh; }
	virtual bool SpecialStopGahterOnAttack(Role *role) { return false; } //采集不被打断
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void OnAOIRoleEnter(Role *role, Role *target_role);

	void OnActivityOpen();
	void OnActivityClose();

	void OnPlayerOpera(Role *role);

	CrossDarkNightPlayerInfo * GetPlayerInfo(UniqueUserID uuid);
	CrossDarkNightPlayerInfo * GetPlayerInfo(ObjID obj_id);
	Role *GetRole(ObjID obj_id);

private:

	void OnFlushBoss();
	void CheckGiveReward();
	void OnAddScore(UniqueUserID uuid, int score);
	void OnUpdateRank(UniqueUserID uuid, int box_count);

	void SendRoleInfo(Role *role);
	void SyncRankinfoToUser();
	void SyncBossInfoToUser(Role *role = NULL);
	void BroadcastPlayerInfo(ObjID obj_id, Role *target_role = nullptr);
	void SyncCheckRewardTimeStamp(Role *role = NULL);
	void GiveScoreToAllPlayer();

	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);
	bool DarkNightDropToHelper(const Posi &pos, int extra_dis, int * begin_idx, Posi & out_pos);
	void RobBoxOnRelive(Role * dead_role);
	CrossDarkNightBossInfo * GetBossInfo(int monster_id);

	unsigned int m_next_check_reward_timestamp;
	unsigned int m_next_sync_rankinfo_timestamp;
	unsigned int m_next_broadcast_timestamp;
	unsigned int m_next_give_score_time_stamp;

	typedef std::map<UniqueUserID, CrossDarkNightPlayerInfo> UserInfoMap;
	typedef std::map<UniqueUserID, CrossDarkNightPlayerInfo>::iterator UserInfoMapIt;
	UserInfoMap m_user_info_map;

	bool m_need_sync_rank;
	std::vector<CrossDarkNightRankItem> m_rank_vec;

	CrossDarkNightBossInfo m_boss_info[CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX];
	
	int m_cur_boss_index;
	bool is_finsh;

	short m_give_reward_index;
	std::map<UniqueUserID, UniqueUserID> m_deader_killer_map;

	unsigned int m_next_sync_bossinfo_time;
	bool m_need_sync_boss_info;
};

#endif	//__SPECIAL_CROSS_DARK_NIGHT_HPP__