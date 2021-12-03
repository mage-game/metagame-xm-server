#ifndef _WORLDBOSSMANAGER_HPP_
#define _WORLDBOSSMANAGER_HPP_

#include "servercommon/servercommon.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "worldbossbaseconfig.hpp"
#include "inetworkmodule.h"
#include <map>
#include "gameworld/obj/character/monster.h"
#include <vector>

class Role;
const static int WORLD_KILLER_LIST_MAX_COUNT = 5;

class WorldBossManager
{
public:
	struct PersonHurtRankItem
	{
		PersonHurtRankItem() : uid(0), hurt(0) {}

		PersonHurtRankItem(int _uid, long long _hurt):uid(_uid), hurt(_hurt){}

		int uid;
		long long hurt;
	};

	struct GuildHurtRankItem
	{
		GuildHurtRankItem() : guild_id(0), hurt(0) {}

		GuildHurtRankItem(GuildID _guild_id, long long _hurt):guild_id(_guild_id), hurt(_hurt){}

		GuildID guild_id;
		long long hurt;
	};

	struct PersonHurtInfo
	{
		PersonHurtInfo() : uid(0), guild_id(0), hurt(0) {}

		int uid;
		GuildID guild_id;
		long long hurt;
	};

	struct GuildHurtInfo
	{
		GuildHurtInfo():guild_id(0), guild_hurt(0){}

		GuildID guild_id;
		long long guild_hurt;
	};

	struct KillerInfo
	{
		KillerInfo():killer_uid(0), killier_time(0)
		{
			memset(killer_name, 0, sizeof(killer_name));
		}

		int killer_uid;
		unsigned int killier_time;
		GameName killer_name;
	};

	struct WorldBossInfo
	{
		WorldBossInfo() : boss_id(0), scence_id(0), boss_status(0), is_change(0), max_hp(0),
			first_trigger_hudun_per(0), second_trigger_hudun_per(0), first_activity_hudun_hp(0), second_activity_hudun_hp(0),
			shield_continue_time(0), shield_effective_times(0), shield_fix_hurt(0), is_activity_first_hudun(0), is_activity_second_hudun(0),is_protect_by_hudun_1(0),is_protect_by_hudun_2(0),
			trigger_hudun_reward(0), roll_max_num(0), roll_effective_time_1(0), get_hudun_reward_role_uid_1(0),current_roll_max_point_1(0),
			is_give_hudun_reward_1(1), roll_effective_time_2(0), get_hudun_reward_role_uid_2(0),current_roll_max_point_2(0), is_give_hudun_reward_2(1)
		{
		}

		int boss_id;
		int scence_id;
		short boss_status;
		short is_change;

		KillerInfo killer_info_list[WORLD_KILLER_LIST_MAX_COUNT];

		Attribute max_hp;
		int first_trigger_hudun_per;
		int second_trigger_hudun_per;
		long long first_activity_hudun_hp;
		long long second_activity_hudun_hp;
		unsigned int shield_continue_time;
		int shield_effective_times;
		long long shield_fix_hurt;
		short is_activity_first_hudun;
		short is_activity_second_hudun;
		short is_protect_by_hudun_1;
		short is_protect_by_hudun_2;
		short trigger_hudun_reward;
		int roll_max_num;
		unsigned int roll_effective_time_1;
		int get_hudun_reward_role_uid_1;
		int current_roll_max_point_1;
		short is_give_hudun_reward_1;
		unsigned int roll_effective_time_2;
		int get_hudun_reward_role_uid_2;
		int current_roll_max_point_2;
		short is_give_hudun_reward_2;

		std::map<int, PersonHurtInfo> hurt_map;
		std::map<GuildID, GuildHurtInfo> guild_hurt_map;
		std::vector<PersonHurtRankItem> person_rank_vec;
		std::vector<GuildHurtRankItem> guild_rank_vec;
	};

	struct GuildWeekRankItem
	{
		GuildWeekRankItem()
		{
			this->Reset();
		}
		
		void Reset()
		{
			guild_id = 0;
			memset(guild_name, 0, sizeof(guild_name));
			kill_num_last_change_time = 0;
		}

		int guild_id;
		GuildName guild_name;
		int kill_worldboss_num;
		unsigned int kill_num_last_change_time;
	};

	enum ENBOSSSTATUS
	{
		ENBOSSSTATUS_NONEXIST = 0,										// 不存在
		ENBOSSSTATUS_EXIST,												// 存在
	};

	static WorldBossManager &Instance();

	void OnServerStart();
	void OnStopServer();

	void GMRefreshNow();

	void Update(unsigned long interval, time_t now_second);
	void OnWeekChange();
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	// 被攻击，可以修正血量
 	void OnAttack(Role *attacker, Monster *monster, long long &delta_hp);
	// 被攻击后
	void OnBeenAttack(Role *attacker, Monster *monster, UInt16 skill_id, long long delta_hp, bool from_skill);

	void MonsterDie(gglobalprotocal::ReportWorldBossToGlobal *report_info);

	void RollPoint(Role *role, int boss_id, int hudun_index);

	void CheckAddActiveDegree(int boss_id);		//击杀世界boss,参与者增加活跃度
	void GivePersonHurtReward(int boss_id);
	void GiveGuildHurtReward(int boss_id);
	void GiveKillerGuildReward(Role *role, int boss_id);
	void GiveWeekRankReward();
	void GiveRandomRedPaper(Role *role, int boss_id);
	void GiveHundunReward(int uid, int boss_id, int roll_num,int hudun_index);
	void NotifySystemMsg(Scene* scene, char *str_buff, int str_len, int notice_type);

	void SendWorldBossListInfo(Role *role);
	void SendWorldBossPersonalHurtInfo(Role *role,int boss_id);
	void SendWorldBossGuildHurtInfo(Role *role, int boss_id);
	void SendWorldBossWeekRankInfo(Role *role);
	void SendHurtInfoToAll(int boss_id);
	void SendHudunDestroyInfo(WorldBossInfo &boss_info, int hudun_index);
	void SendTopRollInfo(int boss_id, int hudun_index, int top_roll_point, const char *top_name);
	void SendBossInfoToAllGateway(WorldBossInfo boss_info, int killer_uid = 0);
	void SendBossKillerList(Role *role, int boss_id);

private:
	WorldBossManager();
	~WorldBossManager();

	void CalcHurtRank(int boss_id);
	void CalcWeekRank();

	void RefreshAllWorldBoss();
	void RefreshOneWorldBoss(WorldBossInfo &world_boss);

	unsigned int m_next_refresh_worldboss_time;

	unsigned int m_next_refresh_hurtinfo_time;
	unsigned int m_refresh_hurtinfo_interval;

	unsigned int m_roll_effective_time_interval;

	typedef std::map<int, WorldBossInfo> WorldBossMap;
	typedef std::map<int, WorldBossInfo>::iterator WorldBossMapIt;
	WorldBossMap m_worldboss_info_map;

	std::vector<GuildWeekRankItem> m_guild_week_rank_vec;
};

#endif
