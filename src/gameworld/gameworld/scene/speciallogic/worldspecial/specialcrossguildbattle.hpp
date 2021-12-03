#ifndef _SPECIAL_CROSS_GUILDBATTLE_HPP_
#define _SPECIAL_CROSS_GUILDBATTLE_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include "servercommon/crossdef.hpp"
#include "servercommon/serverdef.h"
#include <map>

class Role;

class SpecialCrossGuildBattle : public SpecialLogic
{
public:
	struct FlagItem
	{
		FlagItem() { this->Reset(); }
		
		void Reset()
		{
			flag_id = 0;
			obj_id = INVALID_OBJ_ID;
			next_add_score_time = 0;
			last_warning_attack_time = 0;
			has_be_hit_broadcast_flag = 0;
			low_hp_broadcast_flag = 0;
			guild_id = INVALID_UNIQUE_USER_ID;
		}

		int flag_id;
		int obj_id;
		int next_add_score_time;
		int last_warning_attack_time;
		int has_be_hit_broadcast_flag;
		int low_hp_broadcast_flag;

		UniqueUserID guild_id;
	};

	struct GuildItem
	{
		GuildItem() : guild_id(INVALID_UNIQUE_USER_ID), score(0), own_num(0), role_num(0),guild_tuanzhang_prof(0), guild_tuanzhang_sex(0), guild_tuangzhang_uuid(0),
			wuqi_id(0),shizhuang_wuqi(0),shizhuang_wuqi_is_special(0),shizhuang_body(0),shizhuang_body_is_special(0),halo_used_imageid(0),toushi_used_imageid(0),
			qilinbi_used_imageid(0),mask_used_imageid(0)
		{
			memset(guild_name, 0, sizeof(guild_name));
			memset(guild_tuanzhang_name, 0, sizeof(guild_tuanzhang_name));
		}

		UniqueUserID guild_id;
		GuildName guild_name;
		GameName guild_tuanzhang_name;
		int score;
		int own_num;
		int role_num;
		char guild_tuanzhang_prof;
		char guild_tuanzhang_sex;
		long long guild_tuangzhang_uuid;
		ItemID wuqi_id;              // ����ID
		char shizhuang_wuqi;          // ʱװ����
		char shizhuang_wuqi_is_special;      // ʱװ����
		char shizhuang_body;          // ʱװ����
		char shizhuang_body_is_special;      // ʱװ����
		short halo_used_imageid;        // �⻷����
		short toushi_used_imageid;        // ͷ������id
		short qilinbi_used_imageid;        // ���������id
		short mask_used_imageid;        // �������id
	};

	struct HurtInfo
	{
		HurtInfo() : obj_id(INVALID_OBJ_ID), last_hurt_time(0) {}
		HurtInfo(ObjID _obj_id, unsigned int _last_hurt_time) : obj_id(_obj_id), last_hurt_time(_last_hurt_time) {}

		ObjID obj_id;
		unsigned int last_hurt_time;
	};

	struct UserItem
	{
		UserItem() : role_id(0), next_add_score_time(0) {}

		int role_id;
		int next_add_score_time;

		typedef std::map<int, HurtInfo> HurtMap;
		HurtMap be_hurt_user_map;
	};

	enum SHOUHU_BOSSSTATUS
	{
		SHOUHU_BOSSSTATUS_NONEXIST = 0,										// ������
		SHOUHU_BOSSSTATUS_EXIST,											// ����
	};

	struct GuildHurt
	{
		GuildHurt() : guild_id(INVALID_UNIQUE_USER_ID), hurt(0), logout_timestamp(0),rank(-1)
		{
			uuid_map.clear();
		}

		UniqueUserID guild_id;							// ����id
		long long hurt;									// dps
		time_t logout_timestamp;						// ���ɳ�Աȫ�����ߵ�ʱ����±��
		int rank;
		std::map<long long, long long> uuid_map;		
	};

	static const int CORSS_GUILD_BATTLE_OWNER_TIME_S = 60;  // ���︱�����߹�������ʱ�� ��
	struct ShouHuBossInfo
	{
		ShouHuBossInfo() : boss_id(0), replace_boss_id(0),born_pos(0, 0), next_refresh_time(0), boss_status(0), 
		refresh_interval(0), begin_refresh_time(0), end_refresh_time(0),
		refresh_interval1(0), begin_refresh_time1(0), end_refresh_time1(0),
		boss_obj(0),guild_uuid(INVALID_UNIQUE_USER_ID),gather_obj(INVALID_OBJ_ID),
		gather_guild_uuid(INVALID_UNIQUE_USER_ID)
		{
		}

		int boss_id;
		int replace_boss_id;
		Posi born_pos;
		unsigned int next_refresh_time;
		short boss_status;
		unsigned int refresh_interval;
		int begin_refresh_time;
		int end_refresh_time;
		unsigned int refresh_interval1;
		int begin_refresh_time1;
		int end_refresh_time1;

		ObjID boss_obj;										// boss��objid
		UniqueUserID guild_uuid;							// boss���չ�������uuid
		std::map<UniqueUserID, GuildHurt> guild_hurt_map;	// ����dps��
		ObjID gather_obj;
		UniqueUserID gather_guild_uuid;						// �����������id
		std::set<UniqueUserID>	gather_list;				// ����ɼ��б�
	};

	// Ĥ����Ϣ
	struct WorshipItem
	{
		WorshipItem() :manual_worship_times(0), next_can_manual_worship_timestamp(0), manual_worship_reward_timestamp(0) {}

		Int32 manual_worship_times;					// Ĥ�ݴ���
		UInt32 next_can_manual_worship_timestamp;   // �´��ܹ�Ĥ�ݵ�ʱ���
		UInt32 manual_worship_reward_timestamp;     // Ĥ�ݸ��轱����ʱ���
	};

public:
	SpecialCrossGuildBattle(Scene *scene);
	virtual ~SpecialCrossGuildBattle();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	virtual void OnAddObj(Obj *obj);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool CanDestroy() { return false; }
	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual bool SpecialStopGahterOnAttack(Role *role) { return false; } //�ɼ��������
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	void OnActivityStatusChange(int status);
	void OnActivitystandy();
	void OnActivityOpen();
	void OnActivityClose();

	void SendRankLogToRole(const char *log_msg, const int msg_length);

	void GetGuildRankList(
		CrossGuildBattleRankItem rank_list[CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM],
		CrossGuildBattleRankLogItem log_rank_list[CROSS_GUILDBATTLE_MAX_LOG_RANK_NUM]);
	int GetGuildJoinNum();

	void OnServerStart();
	void SynBossInfoToOrigin(long long uuid, bool is_refresh = false);
	void SyncSceneInfo(Role *role);

	void SyncSceneMonsterCountInfoToOrigin(Role *role);

	void OnSyncDropLog(Role *role, int log_type, unsigned short item_id, short num);

	int GetSpecialMonsterCount();

	void GetFlagInfo(Protocol::SCCrossGuildBattleFlagInfo::FlagItem flag_list[CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE]);

	ShouHuBossInfo* GetShouHuBossInfo(int boss_id);

	int GetSceneGuildRoleNum(UniqueUserID guild_id);

	void SendMsgToSceneGuild(const char * msg,int len, UniqueUserID guild_id);

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

	void SetMonsterGenerater();
	void StopGenerater();
	void StartGenerater();

	void CheckUpdataBoss(time_t now_second);
	void RefreshCreateFlag();
	void RefreshShouhuBoss(ShouHuBossInfo &boss_info);
	void ChangeAllBossStatus(int status);
	void CheckAddGuildScore(int now_time);
	void CheckAddRoleScore(int now_time);
	void CheckTask(Role *role, int task_param);
	void KickAllToOrginalToServer();
	UniqueUserID GetGuildUniqueUserId(Role *role);
	FlagItem *GetFlagItemById(int flag_id);
	int GetOwnNum(UniqueUserID guild_id);
	void OnFlagMonsterBeAttacked(FlagItem *flag_item, Monster *monster);

	UserItem *GetUserItem(Role *role);
	GuildItem *GetGuildItem(UniqueUserID guild_id);

	void SyncChangeNotifyInfo(Role *role, int notify_type);
	void SynBossInfo(Role *role, bool is_refresh = false);
	void SyncFlagInfo(Role * role);
	void NotifyWarning(int warning_type, UniqueUserID guild_id, int monster_id, int percent = 0);

	void OnShouHuBossDie(ShouHuBossInfo * boss_info);
	void CheckAllBossBound();															// ���ȫ��boss���������ɳ�ԱԶ��boss�������ɳ�Ա��boss��dps����ȥ����
	void CheckSingleBossBound(int monster_id);	// ��鵥��boss���������ɳ�ԱԶ��boss�������ɳ�Ա��boss��dps����ȥ����
	
	bool m_first;
	bool m_activity_open;
	bool m_had_update;

	int m_today_drop_limit_num;

	int m_next_sync_scene_info_time;
	
	typedef std::map<UniqueUserID, GuildItem> GuildMap;
	GuildMap m_guild_map;

	typedef std::map<int, UserItem> UserMap;
	UserMap m_role_map;

	FlagItem m_flag_list[CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE];

	int m_total_level;
	int m_total_num;
	unsigned int m_broadcast_time;

	std::map<int, ShouHuBossInfo> m_shouhu_boss_map;

	typedef std::vector<GuildItem*> RankVec;
	RankVec m_score_rank_vec;
	
	unsigned int m_boss_hurt_info_time;

	std::map<UniqueUserID, WorshipItem> m_worship_user_map;  // Ĥ�ݽ�ɫ�б�<unique_uid, info>
	time_t m_worship_start_time;							// Ĥ�ݿ�ʼʱ��
};


#endif // _SPECIAL_CROSS_1V1_HPP_

