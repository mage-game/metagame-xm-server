#ifndef _BOSSFAMILYMANAGER_HPP_
#define _BOSSFAMILYMANAGER_HPP_

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "bossfamilyconfig.hpp"
#include <map>
#include "protocal/msgfb.h"
#include <vector>
#include "obj/character/role.h"

/*
	活跃boss = active_boss
	vipBoss = boss_family
	套装boss = dabao_boss
	精英boss = miku_boss = dongku_boss
	
	场景类型都是普通，没有特殊场景逻辑
*/

class Role;
const static int KILLER_LIST_MAX_COUNT = 5;
const static int ACTIVE_BOSS_RANG_INFO = 25;
const static int MIKU_BOSS_RANG_INFO = 25;

namespace crossgameprotocal
{
	struct CrossGameCrossCommonBossInfoChange;
	struct CrossGameSyncCrossCommonBossInfo;
}

class BossFamilyManager
{
public:

	enum ENBOSSSTATUS
	{
		ENBOSSSTATUS_NONEXIST = 0,										// 不存在
		ENBOSSSTATUS_EXIST,												// 存在
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

	struct BossInfo
	{
		BossInfo() : boss_id(0), scene_id(0), born_pos(0, 0), has_notice(0), next_refresh_time(0), boss_status(0),refresh_interval(0),
			begin_refresh_time(0), end_refresh_time(0), seq(0)
		{
		}

		int boss_id;
		int scene_id;
		Posi born_pos;
		bool has_notice;
		unsigned int next_refresh_time;
		short boss_status;
		unsigned int refresh_interval;
		int begin_refresh_time;
		int end_refresh_time;
		int seq;

		KillerInfo killer_info_list[KILLER_LIST_MAX_COUNT];
	};

	struct BossHurtInfo
	{
		bool operator < (const BossHurtInfo & other) { return hurt > other.hurt; }

		BossHurtInfo() : uid(0), hurt(0), logouttime(0)
		{
			memset(name, 0, sizeof(name));
		}

		int uid;
		long long hurt;
		GameName name;
		unsigned int logouttime; // 从本场景下线时间
	};

	struct ActiveBossHurtRankInfo			// BOSS之家BOSS伤害信息，精英(困难/密窟)和活跃（简单）共用
	{
		ActiveBossHurtRankInfo() : rank_data_dirty_flag(0) {}

		int rank_data_dirty_flag;
		std::vector<BossHurtInfo> hurt_info_list;
		std::map<int, int> send_info_user;
	};

	struct MonsterInfo
	{
		MonsterInfo() : monster_id(0), scene_id(0), born_pos(0, 0), next_refresh_time(0), monster_status(0), refresh_interval(0),
			begin_refresh_time(0), end_refresh_time(0), seq(0), is_great_monster(0)
		{
		}

		int monster_id;
		int scene_id;
		Posi born_pos;
		unsigned int next_refresh_time;
		short monster_status;
		unsigned int refresh_interval;
		int begin_refresh_time;
		int end_refresh_time;
		int seq;
		bool is_great_monster;
	};

	struct PerciousMonster
	{
		PerciousMonster() : monster_type(0), refresh_interval(0)
		{}

		enum PERCIOUS_MONSTER_TYPE
		{
			TYPE_BOSS = 0,
			TYPE_COMMON_MONSTER,

			TYPE_MAX,
		};

		int monster_type;
		int refresh_interval;
	};

	struct FamilyBossInfo
	{
		FamilyBossInfo()
		{
			boss_info_map.clear();
		}

		std::map<int, BossInfo> boss_info_map;
	};

	struct MikuBossInfo
	{
		MikuBossInfo()
		{
			boss_info_map.clear();
			hurt_rank_info_map.clear();
		}

		std::map<int, BossInfo> boss_info_map;
		std::map<int, ActiveBossHurtRankInfo> hurt_rank_info_map;
	};

	struct DabaoBossInfo
	{
		DabaoBossInfo()
		{
			next_notice_time = 0;
			scene_id = 0;
			boss_info_map.clear();
		}

		unsigned int next_notice_time;
		int scene_id;
		std::map<int, BossInfo> boss_info_map;
	};

	struct MikuMonsterInfo
	{
		MikuMonsterInfo()
		{
			monster_info_map.clear();
		}

		std::map<int, MonsterInfo> monster_info_map;
	};

	struct ActiveBossInfo
	{
		ActiveBossInfo()
		{
			boss_info_map.clear();
			hurt_rank_info_map.clear();
		}

		std::map<int, BossInfo> boss_info_map;
		std::map<int, ActiveBossHurtRankInfo> hurt_rank_info_map;
	};

	struct PreciousGatherInfo
	{
		PreciousGatherInfo() : is_gather(0), next_refresh_time(0), pos(0, 0)
		{}

		char is_gather;
		unsigned int next_refresh_time;
		Posi pos;
	};

	struct BabyBossInfo
	{
		BabyBossInfo()
		{
			boss_info_map.clear();
		}
		std::map<int, BossInfo> boss_info_map;
	};

	// 云游boss ---------------------------------------------
	struct YunyouBossInfo
	{
		int scene_id = 0;
		int boss_id = 0;
		bool is_dead = false;	// 是否死亡
		int born_pos_x = 0;
		int born_pos_y = 0;
	};

	static BossFamilyManager &Instance();

	void Init(const BossSpecialRefreshParam &param);
	void GetInitParam(BossSpecialRefreshParam *param);

	void OnRoleLogin(Role *role);
	void OnRoleLogout(Role *role);
	void OnCheckComeBackWithOnLogin(Role *role, long long last_save_time);			// 检查离线超时踢回主城,会执行Goto,注意调用顺序

	void OnServerStart();
	void OnStopServer();

	void GMRefreshFamilyBossNow();
	void GMRefreshMikuBossNow();

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void BossDie(Monster* monster);											// BOSS死亡
	void OnDabaoBossDie(Monster *monster);
	void OnActiveBossDie(Monster *monster);
	void OnMikuBossDie(Monster *monster);
	void HiddenServerBossDie(Monster *monster);
	void OnPreciousBossDie(Monster *monster);
	void OnYunyouBossDie(Monster *monster, int scene_id);

	void OnRoleDie(int boss_type, Role *role);
	void MonsterBeKill(Role* killer, Monster *monster);
	void OnDabaoMonsterBeKill(Monster *monster, Role *role);
	void OnActiveMonsterBeKill(Monster *monster, Role *role);
	void OnMikuMonsterBeKill(Monster *monster, Role *role);
	void OnBabyMonsterDie(Monster *monster, Role *role);
	void HiddenServerMonsterBeKill(Role *killer, Monster *monster);
	void OnYunyouMonsterBeKill(Monster *monster, Role *role, int scene_id);

	void OnEventDropOwnerWithMonsterDie(Role *drop_owner, Monster *monster);  // BOSS死亡前触发的掉落归属逻辑，用来设置归属相关的数据

	void OnAttack(Role *attacker, Monster *monster, long long &delta_hp);

	bool CanFlyByShoe(Role *role, int target_scene_id);
	//void CheckDabaoBossTime(Role *role);

	void OnEnterBossFamily(Role *role, Protocol::CSEnterBossFamily *msg);
	void OnGetBossInfoReq(Role *role, int enter_type);
	void OnFamilyBossInfoReq(Role *role);
	void OnMikuBossInfoReq(Role *role);
	void OnDabaoBossInfoReq(Role *role);
	void OnActiveBossInfoReq(Role *role);
	void OnBossKillerInfoReq(Role *role, int boss_type, int boss_id, int scene_id);
	void OnFollowBossReq(Role *role, int boss_type, int boss_id, int scene_id);
	void OnUnFollowBossReq(Role *role, int boss_type, int boss_id, int scene_id);

	void SendDabaoBossInfo(Role *role);
	void SendActiveBossInfo(Role *role);
	void SendFamilyBossInfo(Role *role, int scene_id = 0);
	void SendMikuBossInfo(Role *role,int scene_id = 0);
	void SendMikuMonsterInfo(Role *role, int scene_id = 0);
	void SendFamilyBossInfoToAll(int scene_id);
	void SendMikuBossInfoToAll(int scene_id);
	void SendMikuMonsterInfoToAll(int scene_id);
	void SendBossRoleInfo(Role *role);
	void SendDabaoBossNextFlushInfo(Role *role, int scene_id = 0);
	void SendDabaoBossNextFlushInfoToAll(int scene_id);
	void SendActiveBossNextFlushInfo(Role *role, int scene_id = 0);
	void SendActiveBossNextFlushInfoToAll(int scene_id);
	void SendBossInfoToAllGateway(BossInfo boss_info, int boss_type, int notify_reason, int killer_uid = 0);
	void SendBossKillerList(Role *role, BossInfo boss_info);

	void OnBossFamilyOperate(Role *role, int operate_type);

	void RecalcMikuFlushTime();
	void RecalcMikuOnBossFlushTime(int boss_id);
	int GetBossSpecialFlushInterval(int boss_id);  // 获取刷新间隔 秒

	bool IsDabaoBossTireFull(Role *role);

	void OnPreciousMonsterBeKill(Monster *monster, Role *role);
	void SendPreciousBossInfoToAll();
	void SendPreciousBossInfo(Role *role);
	void SendPreciousBossPosInfo(Role *role, int param, int param_2);
	void SendPreciousBossTaskInfo(Role *role);
	void OnPreciousBossGather(Role *role, int gather_id, const Posi &gather_pos);
	bool IsInForbidPkArea(Role *role);
	void PreciousBossRewardHelper(Role *role, int reward_param, bool is_skip = false);

	void OnEnterBabyBoss(Role *role, int scene_id, unsigned short boss_id);
	void OnLeaveBabyBossReq(Role *role);
	bool IsBabyBossEnterTimesEnough(Role *role);
	bool AddBabyBossEnterTimes(Role *role);
	void SendBabyBossRoleInfo(Role *role);
	void SendAllBabyBossInfo(Role *role);

	void OnGodmagicBossReq(Role * role, Protocol::CSGodmagicBossInfoReq* req);
	void OnEnterGodmagicBoss(Role *role, int scene_id, int boss_id);
	void SendGodmagicBossRoleInfo(Role * role);
	int GetGodmagicBossTire(Role *role);			//神魔boss疲劳
	bool AddGodmagicBossKillNum(Role *role);		//增加神魔boss击杀数

	// cross boss
	void OnCrossCommonBossInfoChange(crossgameprotocal::CrossGameCrossCommonBossInfoChange *sccbi);
	void UpdateBossKillerInfo(BossInfo *boss_info, KillerInfo *killer_info);
	void UpdateBossKillerInfo(BossInfo *boss_info, Role *killer);
	void OnCrossCommonBossInfoSyncReq(NetID net_id);
	void OnSyncCrossCommonBossInfo(crossgameprotocal::CrossGameSyncCrossCommonBossInfo *sccbi);

	// 云游boss------------------------------------------------
	void OnYunyouBossOperate(Role *role, int operate_type, int param_0);
	void RefreshAllYunyouBoss();
	void UpdateYunyouBossInfo(const YunyouBossInfo &info);
	void SendYunyouBossInfo(Role *role , int scene_id);
	void SendYunyouAllSceneBossInfo(Role *role);

	bool IsYunyouBossDead(int scene_id , int boss_id);

	//------------------------------------------------
	void OnConnectCrossServerSucc();  // 连接跨服成功

	// BOSS刷新卡 ----------------------------------------------------------------------------
	bool OnUseBossRefreshCardActiveBoss(Role *role, const int boss_id = -1);	// 使用BOSS刷新卡刷新一个简单BOSS
	bool OnUseBossRefreshCardMikuBoss(Role *role, const int boss_id = -1);		// 使用BOSS刷新卡刷新一个困难BOSS

private:
	BossFamilyManager();
	~BossFamilyManager();

	void RefreshAllFamilyBoss();
	void RefreshAllMikuBoss();
	
	void RefreshOneFamilyBoss(BossInfo & family_boss);
	void RefreshOneMikuBoss(BossInfo &miku_boss);
	void RefreshOneDabaoBoss(BossInfo &dabao_boss);
	void RefreshMikuMonster(MonsterInfo &miku_monster);
	void RefreshOneActiveBossInfo(BossInfo &dabao_boss);
	void RefreshActiveBossInfo(int scene_id);
	void RefreshOneBabyBossInfo(BossInfo &baby_boss);

	void CheckKickOutDabao(int scene_id, time_t now_second);

	void UpdateMikuBossRefreshInterval();

	BossSpecialRefreshParam m_special_refresh_param;

	void RefreshOnePreciousBoss(BossInfo &precious_boss);
	void RefreshPreciousMonster(MonsterInfo& precious_monster);
	void PreciousBossRefreshGather(PreciousGatherInfo &gather_info);
	void PreciousBossTeamRewardHelper(Role *role, int reward_param);
	void PreciousBossTaskHelper(Role *role, int task_param, bool is_team_task = false);
	Posi PreciousBossPosHelper(int type);
	void AddPreciousBossPos(int pos_type, const Posi &pos);
	void SendBabyBossKillerInfo(Role *role, int boss_id);

	void OnRoleLoginActiveBoss(Role *role);   // 简单boss中上线
	void OnRoleLogoutActiveBoss(Role *role);  // 简单boss中下线
	void OnAttackActiveBoss(Role *attacker, Monster *monster, long long &delta_hp);
	void SendActiveBossHurtRankInfo(Monster *monster = nullptr);

	void SendMikuBossHurtRankInfo(Monster *monster = nullptr);

	void SyncRoleNumToHidden();  // 同步本服角色等级区间role个数到隐藏服

	unsigned int m_next_refresh_time;
	unsigned int m_refresh_interval;
	int m_kill_mikuboss_add_weary;
	unsigned int m_dabao_kick_delay_time;
	unsigned int m_active_kick_delay_time;
	bool m_is_init_precious_boss;
	
	typedef std::map<int, FamilyBossInfo> BossFamilyBossMap;
	typedef std::map<int, FamilyBossInfo>::iterator BossFamilyBossMapIt;
	
	typedef std::map<int, MikuBossInfo> MikuBossMap;
	typedef std::map<int, MikuBossInfo>::iterator MikuBossMapIt;

	typedef std::map<int, DabaoBossInfo> DabaoBossMap;
	typedef std::map<int, DabaoBossInfo>::iterator DabaoBossMapIt;

	typedef std::map<int, ActiveBossInfo> ActiveBossMap;
	typedef std::map<int, ActiveBossInfo>::iterator ActiveBossMapIt;

	typedef std::map<int, MikuMonsterInfo> MikuMonsterMap;
	typedef std::map<int, MikuMonsterInfo>::iterator MikuMonsterMapIt;

	typedef std::map<int, BabyBossInfo> BabyBossMap;
	typedef std::map<int, BabyBossInfo>::iterator BabyBossMapIt;

	typedef std::vector<MonsterInfo> PreciousMonsterInfoVector;
	typedef std::vector<MonsterInfo>::iterator PreciousMonsterInfoVectorIt;

	BossFamilyBossMap m_familyboss_info_map;
	MikuBossMap m_mikuboss_info_map;
	DabaoBossMap m_dabaoboss_info_map;
	ActiveBossMap m_activeboss_info_map;
	MikuMonsterMap m_miku_monster_info_map;
	BabyBossMap m_babyboss_info_map;

	std::map<int, int> m_miku_monster_num;

	std::set<long long> m_familyboss_set;
	std::set<long long> m_mikuboss_set;

	//秘藏boss
	std::map<int, BossInfo> m_precious_boss_info_map;

	std::map<ObjID, MonsterInfo> m_precious_monster_map;
	std::vector<MonsterInfo> m_refresh_precious_monster_vec;

	std::vector<PreciousGatherInfo> m_precious_gather_vec;
	std::map<int, std::vector<Posi> > m_precious_pos_pool;
	
	unsigned int m_send_active_boss_info_time;

	// cross boss
	BossFamilyBossMap m_cross_familyboss_info_map;

	time_t m_next_sync_role_num_time;		// 下一次同步角色数量的时间

	// 云游boss------------------------------------------------
	unsigned int m_next_refresh_yunyou_boss_time = 0;					 // 下次刷新云游boss时间
	std::map<int, std::vector<YunyouBossInfo> > m_yunyou_boss_info_map;  // 云游boss信息
};

#endif
