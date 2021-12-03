#ifndef __SPECIAL_XIAN_MENG_ZHAN_HPP__
#define __SPECIAL_XIAN_MENG_ZHAN_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <set>
#include <map>

class SpecialXianMengZhan : public SpecialLogic
{
	friend struct XianMengZhanUser;
	friend struct XianMengZhanGuild;
	friend struct XianMengZhanDefendArea;

public:
	SpecialXianMengZhan(Scene *scene);
	virtual ~SpecialXianMengZhan();

	virtual bool CanDestroy() { return false; }
	virtual void Update(unsigned long interval, time_t now_second);

	virtual bool SpecialCanGather(Role *role,  GatherObj *gather);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnAttack(Character *cha, ObjID attacker_id, Attribute delta_hp);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual int GetSpecialParam(Role *role);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	virtual void OnAddObj(Obj *obj);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialCanMountOn(Role *role) { return false; }

	void OnActivityStatusChange(int status);
	void OnRoleReqSignUpInfo(int uid, GuildID guild_id);
	void OnRoleSignUp(int uid, GuildID guild_id);
	void OnRoleCallGuildMembers(Role *role, bool use_gold);
	void OnRoleFollowGuildLeader(Role *role);

	void GmAddXianli(Role *role, int xianli);

protected:

	void OnActivityStandby();
	void OnActivityStart();
	void OnActivityClose();

	void OnGuildGetDefendArea(XianMengZhanGuild *guild, int defend_area);
	void OnGuildDropDefendArea(XianMengZhanGuild *guild, int defend_area);
	void OnGuildLossDefendArea(XianMengZhanGuild *guild, int defend_area);

	void OnUserGetDefendArea(Role *role, int defend_area);
	void OnGuildGetCenterArea(GuildID guild_id);
	void OnUserKillDanYaoBoss(GuildID guild_id);

	void OnUserDead(XianMengZhanUser *user, XianMengZhanUser *killer, Role *killer_role, const Posi &dead_pos);
	void OnResetLianzhan(XianMengZhanUser *user);

	void GuildAddScore(XianMengZhanGuild *guild, int score);
	void GuildChangeDefendArea(XianMengZhanGuild *guild, int defend_area);
	void UserAddScore(XianMengZhanUser *user, int score, bool add_to_guild = true);
	void UserAddKillCount(XianMengZhanUser *user, int kill_count);

	void CheckGatherPoint(time_t now_second = 0);
	void CheckCenterArea(time_t now_second);
	void CheckDefendAreaReward(time_t now_second);
	void CheckLianZhanNotice(Role *kill_user, Role *dead_user);

	void SyncUserInfo(time_t now_second = 0);
	void SyncGuildInfo(time_t now_second = 0);
	void SyncDefendAreaList(time_t now_second = 0);
	void SynGuildRankInfo(time_t now_second = 0);

	void SendToGuild(GuildID guild_id, const char *data, unsigned int length, UserID exclude_user = INVALID_USER_ID);
	void SendUserInfo(XianMengZhanUser *user, int notify_reason);
	void SendGuildInfo(XianMengZhanGuild *guild, XianMengZhanUser *user = NULL);
	void SendDefendAreaList(XianMengZhanUser *user = NULL);
	void SendGuildCallNotice(XianMengZhanGuild *guild, Role *caller);
	void NotifySystemMsg(char *str_buff, int str_len, int msg_type = SYS_MSG_ACTIVE_NOTICE);

	Role * GetRoleHelper(XianMengZhanUser *user);
	XianMengZhanUser *  GetXianMengZhanUser(UserID user_id);
	XianMengZhanGuild * GetXianMengZhanGuild(GuildID guild_id);

	void CalcMonsterAddAttrs(int monster_id, int *add_maxhp, int *add_gongji);

	void BroadcastLianzhanInfo(XianMengZhanUser *user);

	void Reward();
	void Clear();

	void RealiveHereHelper(Role *role, int realive_type);

	bool m_isopen;
	GuildID m_center_guild;
	time_t m_hold_center_timestamp;
	ObjID m_center_gather_obj_id;

	XianMengZhanUserMap m_user_map;
	XianMengZhanGuildMap m_guild_map;

	XianMengZhanDefendArea m_defend_area_list[XIANMENGZHAN_DEFEND_AREA_NUM];

	time_t m_last_center_area_check_time;
	time_t m_last_center_area_reward_time;
	time_t m_last_defend_area_reward_time;
	time_t m_last_update_time;

	time_t m_last_sync_user_info_time;
	time_t m_last_sync_guild_info_time;
	time_t m_next_sync_defend_area_list_time;
	time_t m_next_sync_guild_rank_time;

	time_t m_next_free_reward_time;
};

#endif // __SPECIAL_XIAN_MENG_ZHAN_HPP__
