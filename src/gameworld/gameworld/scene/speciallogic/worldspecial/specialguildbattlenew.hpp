#ifndef __SPECIAL_GUILD_BATTLE_NEW_HPP__
#define __SPECIAL_GUILD_BATTLE_NEW_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include "config/logicconfigmanager.hpp"
#include "servercommon/activitydef.hpp"
#include <map>
#include <vector>

class SpecialGuildBattleNew : public SpecialLogic
{
public:

	// 据点
	struct PointItem
	{
		PointItem() : hold_guild_id(0), is_live(false), boss_id(0), realive_pos(0, 0), blood(0), max_blood(0), boss_notice_lasttime(0), last_warning_attack_time(0),
			has_be_hit_broadcast_flag(0), low_hp_broadcast_flag(0)
		{}

		int hold_guild_id;
		bool is_live;
		int boss_id;
		Posi realive_pos;
		Attribute blood;
		Attribute max_blood;
		int boss_notice_lasttime;
		unsigned int last_warning_attack_time;
		int has_be_hit_broadcast_flag;
		int low_hp_broadcast_flag;
	};

	//参与者
	struct JoinerItem
	{
		JoinerItem () 
		{
			this->Reset();
		}

		void Reset()
		{
			uid = 0;
			guild_id = 0;
			kill_role_num = 0;
			assist = 0;
			history_get_person_credit = 0;
			personal_credit_reward_flag = 0;
			fetch_last_personal_reward_index = 0;
			lianzhan = 0;
			is_addhudun = false;
			sos_times = 0;
			memset(user_name, 0, sizeof(user_name));
			guild_post = 0;
			hurt_roles = 0;
			kill_flags = 0;
			hurt_flags = 0;
		}

		bool IsFetchPersonalCreditReward(int reward_index)
		{
			return 0 != (personal_credit_reward_flag & (1 << reward_index)); 
		}

		void SetFetchPersonalRewardFlag(int reward_index) 
		{
			personal_credit_reward_flag |= (1 << reward_index); 
		}

		int uid;
		GuildID guild_id;
		int kill_role_num;
		int assist;
		int history_get_person_credit;
		int personal_credit_reward_flag;
		int lianzhan;
		short fetch_last_personal_reward_index;
		bool is_addhudun;
		char sos_times;
		std::map<UserID, time_t> hurt_map;												// 伤害我的 用户列表
		GameName user_name;
		char guild_post;
		long long hurt_roles;							//对玩家伤害
		int kill_flags;									//击杀旗帜数
		long long hurt_flags;							//对旗帜伤害
	};

	// 参与仙盟
	struct GuildItem
	{
		GuildItem() : guild_id(0), guild_credit(0), credit_change_time(0), is_hold_big_point(0){}

		bool Invalid() { return 0 == guild_id; }

		int guild_id;
		int guild_credit;
		unsigned int credit_change_time;
		int is_hold_big_point;
		std::vector<JoinerItem*> member_joiner_vec;
	};

	// 膜拜信息
	struct WorshipItem
	{
		WorshipItem() :manual_worship_times(0), next_can_manual_worship_timestamp(0), manual_worship_reward_timestamp(0) {}

		Int32 manual_worship_times;					// 膜拜次数
		UInt32 next_can_manual_worship_timestamp;   // 下次能够膜拜的时间戳
		UInt32 manual_worship_reward_timestamp;     // 膜拜给予奖励的时间戳
	};

	SpecialGuildBattleNew(Scene *scene);
	virtual ~SpecialGuildBattleNew();

	void OnActivityStatusChange(int status);

	virtual bool CanDestroy();
	virtual void Update(unsigned long interval, time_t now_second);

	void CheckBornPointBoss(time_t now_second);

	virtual bool SpecialCanMountOn(Role *role) { return false; }

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);

	virtual void OnAttackCalcHurt(Character *cha, ObjID enemy_id, Attribute &delta_hp);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnHurtOther(Character *cha, Character *target);
	virtual void PointBossNotice();
	bool OnSosCost(Protocol::CSSendGuildSosReq *, Role *role);

	GuildItem * GetGuildItem(GuildID guild_id);
	JoinerItem * GetJoinerItem(int uid);
	PointItem * GetPointItemByPos(Posi pos);
	PointItem * GetPointItemByIndex(int index);

	void OnAddHuDun(Role *role);

	void SendGuildInfoStatistic(long long guild_id, int notify, Role *role = NULL);
	void SendResultInGuildChat(); // 发送胜利结果到仙盟聊天

	bool IsWorshipDuring();
	void OnWorship(Role* role);
	void SendWorshipInfo(Role *role);
	void SyncWorshipStart(Role *role);
	void CheckWorshipReward(time_t now_second);

protected:

	enum WARNING_TPYE
	{
		WARNING_TPYE_ATTACK = 0,
		WARNING_TPYE_HP = 1,
	};

	void OnActivityStandby();
	void OnActivityStart();
	void OnActivityClose();

	int FindGuildRank(GuildID guild_id);

	void CalcRewardOnActClose();

	void SyncGlobalInfoToAll();
	void SyncGlobalInfo(Role *role);
	void SyncRoleInfo(Role *role);
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	void ClearSpeedEffect(Role *role);

	void CheckPersonalScoreReward(Role *role);

	void SendLuckyInfoToUser(Role *role = NULL);
	void GaveLuckyItemToUser();

	int GetBestKiller();
	void CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist = false);
	void CalcGuildRank();
	void AddGuildScore();
	void AddPersonScore();

	void NotifyWarning(int warning_type, int guild_id, int monster_id, int warning_percent = 0);
	void OnFlagMonsterBeAttacked(PointItem *point_item, Monster *monster);

	void CheckGuildPersonalRank();
	void SendGuildInfoStatisticAll();
	void SyncGuildMvpInfo(GuildID guild_id,Role *role =NULL);

	std::map<int, GuildItem *> m_guild_map;
	std::map<int, JoinerItem *> m_joiner_map;
	std::map<int, PointItem *> m_point_map;
	std::vector<GuildItem *> m_guild_rank_vec;
	std::vector<JoinerItem *> m_person_rank_vec;

	bool m_is_finish;
	bool m_credit_is_change;
	unsigned int m_next_calc_time;
	unsigned int m_last_first_guild_broadcast_time;
	unsigned int m_last_add_guild_score_time;
	unsigned int m_last_add_person_score_time;
	unsigned int m_sync_global_info_time;

	unsigned int m_last_warning_attack_time[GUILD_BATTLE_NEW_POINT_NUM];

	Protocol::SCGBLuckyRewardInfo m_luck;
	
	unsigned int m_last_check_guild_person_rank;
	unsigned int m_kick_out_all_role_time;

	std::map<UniqueUserID, WorshipItem> m_worship_user_map;  // 膜拜角色列表<unique_uid, info>
	time_t m_worship_start_time;							// 膜拜开始时间
};

#endif // __SPECIAL_GUILD_BATTLE_NEWA_HPP__
