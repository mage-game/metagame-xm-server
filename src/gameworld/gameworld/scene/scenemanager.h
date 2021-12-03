#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include <vector>
#include <map>
#include <set>
#include <functional>

#include "inetworkmodule.h"
#include "common/raobjlist.h"

#include "messagehandler.h"
#include "config/scenemanagerconfig.h"
#include "protocal/msgother.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"

#define SCENE_MGR World::GetInstWorld()->GetSceneManager()

namespace Protocol
{
	class SCSystemMsg;
}

class SceneManager;

class World;
class ILogModule;
class RMIRoleSaveBackObject;
class Scene;
class Map;
class Obj;

class SpecialGuildStation;
class SpecialZhuXie;
class SpecialQunXianLuanDou;
class SpecialGongChengZhan;
class SpecialGuildBattleNew;
class SpecialXianMengZhan;
class SpecialCampStation;
class SpecialHunyan;
class SpecialNationalBoss;
class SpecialYiZhanDaoDi;
class SpecialGuildFb;
class SpecialWuXingGuaJi;
class SpecialGuildBoss;
class SpecialYaoShouGuangChang;
class SpecialSuoYaoTa;
class SpecialCrossHotspring;
class SpecialQuestion;
class SpecialTianJiangCaiBao;
class SpecialWangLingExplore;
class SpecialTerritoryWar;
class SpecialCrossXiuluoTower;
class SpecialMillionaire;
class SpecialXingzuoYiji;
class SpecialCrossMultiuserChallenge;
class SpecialShuiJing;
class SpecialCombineServerBoss;
class SpecialCrossDarkNight;
class SpecialCrossFishing;
class SpecialNightFight;
class SpecialGuildQuestion;
class SpecialMessBattle;
class SpecialCrossBoss;
class SpecialCross1v1;
class SpecialChallengeField;
class SpecialArmorDefend;
class SpecialCrossMiZangBoss;
class SpecialCrossYouMingBoss;
class SpecialGodmagicBoss;
class SpecialGiftHarvest;
class SpecialCrossLocalChallengeField;
class SpecialCrossHusongShuijing;

//typedef void (SceneManager::*OnCrossGameMsgHandler)(const char *data, int length);
typedef void (SceneManager::*OnCrossGameMsgHandler)(NetID net_id, const char *data, int length);

const static int SCENE_MANAGER_RANDOM_GET_ROLE_MAX_COUNT = 16;
const static int COMMON_SCENE_KEY = 1; // 通用场景key，用于所有玩家公用一个场景的情况

struct CrossUserInfo
{
	CrossUserInfo() : uuid(0), camp(0), cross_type(-1) {}

	long long uuid;
	char camp;
	char cross_type;  // 跨服类型
};

typedef std::map<long long, CrossUserInfo> CrossUserMap;
typedef std::map<long long, CrossUserInfo>::iterator CrossUserMapIt;

class SceneManager
{
	struct FBConfig
	{
		ConfigScene config;
		int scene_type;
	};
public:
	SceneManager();
	~SceneManager();

	void SetWorld(World *world) { m_world = world; }
	int GetSceneTypeWithID(int scene_id);

	bool Init(const SceneManagerConfig &scm, int total_server, int server_index);
	bool ReInit(const SceneManagerConfig &scm, int total_server, int server_index);
	void Release();
	
	void OnRecv(const GSNetID &netid, IP userip, const char *data, int length);
	bool OnDisconnect(const GSNetID &netid, SceneIndex *p_scene_index = NULL, ObjID *p_obj_id = NULL);
	void OnGateWayDisconnect(NetID gateway_netid);

	void LogoutAllRole(NetID gateway_netid = (NetID)-1);

	void RoleTimeOut(Scene *scene, Role *role, const char *reason, int param1 = 0);
	void Update(unsigned long intervals);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void RegisterRoleLocal(const GSNetID &netid, SceneIndex sceneid, ObjID obj_id);
	void UnregisterRoleLocal(const GSNetID &netid, const UserID &user_id);

	void Logout(Scene *scene, ObjID obj_id);
	void Logout(const UserID &user_id);

	void OnRoleLogin(Role *role);

	bool GoTo(Role *role, int target_scene_id, int target_scene_key, const Posi &target_pos);
	bool FlyByShoe(Role *role, int target_scene_id, int target_scene_key, int x, int y, int is_auto_buy, int is_force = 0);						// 使用飞鞋飞到某处
	
	bool CreateFB(int fb_scene_id, int fb_scene_key, Scene **scene, bool check_key = true, unsigned int scene_timeout = 0);
	bool DestroyFB(SceneIndex scene_index);

	void ForceAllOnlineRoleSyncData();

	int GetSceneList(int *scene_list, int max_num);
	Scene * GetSceneByIndex(SceneIndex i);
	Scene * GetSceneById(int id, int key);
	bool CheckIsDelayDeleteScene(int scene_id);
	bool CheckIsTeamScene(int scene_id);
	bool CheckIsGuildScene(int scene_id);
	bool CheckIsYaoShouGuangChangScene(int scene_id);
	bool CheckIsSuoYaoTaScene(int scene_id);
	bool CheckIsBossScene(int scene_id);          // 需要门票的boss场景
	bool CheckIsTeamTowerFB(int scene_id);        // 组队的爬塔本   重连到队友所在层

	SpecialCampStation * GetCampStationSpecialLogic(int camp_type);
	SpecialGuildStation * GetGuildStationSpecialLogic(GuildID guild_id, bool is_create = false);
	SpecialZhuXie * GetSpecialZhuXie(int fb_key, bool is_create = false);
	SpecialTerritoryWar * GetSpecialTerritoryWar(int fb_key, bool is_create = false);
	SpecialQunXianLuanDou * GetSpecialQunXianLuanDou(int fb_key, bool is_create = false);
	SpecialGongChengZhan * GetSpecialGongChengZhan(int fb_key, bool is_create = false);
	SpecialGuildBattleNew * GetSpecialGuildBattle(int fb_key, bool is_create = false);
	SpecialXianMengZhan * GetSpecialXianMengZhan(int fb_key, bool is_create = false);
	SpecialHunyan * GetSpecialHunyan(int fb_key, bool is_create = false);
	SpecialNationalBoss * GetSpecialNationBoss(int fb_key, bool is_create = false);
	SpecialYiZhanDaoDi * GetSpecialYiZhanDaoDi(int fb_key, bool is_create = false);
	SpecialGuildFb * GetSpecialGuildFb(int scene_id, int fb_key, bool is_create = false);
	SpecialWuXingGuaJi * GetSpecialWuXingGuaJi(int fb_key, bool is_create = false);
	SpecialCrossMultiuserChallenge * GetSpecialCrossMultiuserChallenge(int fb_key, bool is_create = false);
	SpecialCross1v1 * GetSpecialCross1v1Challenge(int fb_key, bool is_create = false, unsigned int time_out = 0);
	SpecialGuildBoss * GetSpecialGuildBoss(int scene_id, int fb_key, bool is_create = false);
	SpecialYaoShouGuangChang * GetSpecialYaoShouGuangChang(int fb_key, bool is_create = false);
	SpecialSuoYaoTa * GetSpecialSuoYaoTa(int fb_key, bool is_create = false);
	SpecialShuiJing * GetSpecialShuiJing(int fb_key, bool is_create = false);
	SpecialCrossHotspring * GetSpecialCrossHotspring(int fb_key, bool is_create);
	SpecialTianJiangCaiBao * GetSpecialTianJiangCaiBao(int fb_key, bool is_create = false);
	SpecialWangLingExplore * GetSpecialWangLingExplore(int fb_key, bool is_create = false);
	SpecialCrossXiuluoTower * GetSpecialCrossXiuluoTower(int scene_id, int fb_key, bool is_create = false);
	SpecialMillionaire * GetSpecialMillionaire(int fb_key, bool is_create = false);
	SpecialXingzuoYiji * GetSpecialXingzuoYiji(int fb_key, bool is_create = false);
	SpecialCombineServerBoss * GetSpecialCSABoss(int fb_key, bool is_create = false);
	SpecialCrossDarkNight * GetSpecialCrossDarkNight(int fb_key, bool is_create);
	SpecialCrossFishing * GetSpecialCrossFishing(int fb_key, bool is_create);
	SpecialNightFight * GetSpecialNightFight(int scene_id, int fb_key, bool is_create = false);
	SpecialGuildQuestion * GetSpecialGuildQuestion(int scene_id, int fb_key, bool is_create = false);
	SpecialMessBattle * GetSpecialMessBattle(int scene_id, int fb_key, bool is_create = false);
	SpecialCrossBoss *GetSpecialCrossBoss(int scene_id, int fb_key, bool is_create = false);
	SpecialChallengeField *GetSpecialChallengeField(int scene_id, int fb_key);
	SpecialCrossLocalChallengeField *GetSpecialCrossLocalChallengeField(int scene_id, int fb_key);
	SpecialArmorDefend *GetSpecialArmorDefend(int scene_id, int fb_key, bool is_create = false);
	SpecialCrossMiZangBoss *GetSpecialCrossMizangBoss(int scene_id, int fb_key, bool is_create = false);
	SpecialCrossYouMingBoss *GetSpecialCrossYoumingBoss(int scene_id, int fb_key, bool is_create = false);
	SpecialGodmagicBoss * GetSpeciaGodmagicBoss(int scene_id, int fb_key, bool is_create = false);
	SpecialGiftHarvest * GetSpecialGiftHarvest(int fb_key, bool is_create);
	SpecialCrossHusongShuijing * GetSpcialCrossHusongShuijing(int scene_id,int fb_key,bool is_create = false);
 
	bool IsForbidSublock() { return m_is_forbid_sublock; }
	bool IsMicroPcOpen() { return m_is_micro_pc_open; }
	bool IsMonitorAutoForbid() { return m_is_monitor_auto_forbid; }

	int GetNpcScenes(UInt16 npc_id, Scene **scene_array, int max_num);

	// not_find_sendtoL 表示如果找不到该user_id 是否发送给globalserver转发
	bool SendMessageToGameWorld(const UserID &user_id, const char *data, int len, bool not_find_sendtoG = true);
	bool SendToUser(const UserID &user_id, const char *data, int length);
	bool NoticeToUser(const UserID &user_id, int notice_num);
	bool SendToUserByGlobal(const UserID &user_id, const char *data, int length);
	void SendToAllUser(const char *data, unsigned int length, std::function<bool(Role*)> filter = nullptr); // 根据lambda表达式筛选玩家再发送协议
	void SendToVipPhaseUser(const char *msg, int length, int vip_level_min, int vip_level_max);

	void GetAllRole(std::set<Role*> &role_list, std::function<bool(Role*)> filter = nullptr);				// 获取符合条件的在线玩家
	
	Obj * GetObj(SceneIndex scene_index, ObjID obj_id);
	Role * GetRole(SceneIndex scene_index, ObjID obj_id);
	Role * GetRole(const UserID &user_id);
	Role * GetRoleByUUID(long long uuid);
	Role * GetRole(GameName role_name);
	bool UserExist(const UserID &user_id);						// user 是否存在
	bool UserCrosserExist(const UserID &user_id);				// user 是否处于跨服中
	bool UserCrosserExist(const long long uuid);				// user 是否处于跨服中
	bool IsOnLine(long long uuid);								// 是否在线（已包括跨服）
	int GetOnLineType(long long uuid);							// 在线状态（已包括跨服）

	static bool CreateSystemMsg(Protocol::SCSystemMsg *sm, int *sendlen, const char *msg, int length, int msg_type, int color = 0, int display_pos = 0);
	
	bool SystemMsgToGuild(int guild_id, const char *msg, int length, int msg_type, int color = 0, int display_pos = 0);
	bool SystemMsgToCamp(int camp_type, const char *msg, int length, int msg_type, int color = 0, int display_pos = 0);
	bool SystemMsgThread(const char *msg, int length, int msg_type, int color = 0, int display_pos = 0);
	//战区传闻
	bool SystemMsgToAllGameWorld(const char *msg, int length, int msg_type, int color = 0, int display_pos = 0);

	void AddGS(int scene, int index);								// 其他gameserver信息
	void DeleteGS(int scene);
	bool GetGS(int scene, int *index);

	void OnCrossServerRecv(NetID net_id, const char *msg, int length);
	void OnChatServerRecv(NetID net_id, const char *msg, int length);

	void OnGlobalUserMsg(NetID net_id, const char *msg, int length);
	void OnReloadConfig(const char *msg, int length);
	void OnCmdToWorldCount(const char *msg, int length);
	void OnCmdToWorldFbCount(const char *msg, int length);

	void OnSyncGuildInfo(NetID net_id, const char *msg, int length);

	void OnSyncWorldLevel(NetID net_id, const char *msg, int length);

	void OnForbidSublock(const char *msg, int length);
	void OnForceSetMicroPcFlag(NetID net_id, const char *msg, int length);
	void OnForceSetAutoForbid(NetID net_id, const char *msg, int length);

	void OnSyncMarryInfo(NetID net_id, const char *msg, int length);

	void StartSceneStatusLog();

	void OnSyncActivityStatus(ActivityStatus *as);
	void OnRADayChongzhiRankGiveReward();
	void OnRADayConsumeRankGiveReward();

	void OnRAGoldenPigSendBossState();

	bool RandomGetRoleList(Role *role, Protocol::RoleInfo role_list[Protocol::MAX_RANDOM_ROLE_LIST_NUM], int *get_num);
	int RandomGetRoleList(int role_list[SCENE_MANAGER_RANDOM_GET_ROLE_MAX_COUNT]);
	
	int GetOnlineRoleNum();
	int GetMaxRoleNum();

	void OnCrossUserAdd(int uid, int cross_type);
	void OnDisconnectFromCrossServer(const int cross_type);
	CrossUserMap & GetCrossUserMap() { return m_cross_user_map; }

	void OnCrossGuildBattleInfoSync(NetID net_id, const char *data, int length);
	void OnCrossGuildBattleGetRankInfoResp(NetID net_id, const char *data, int length);

	Scene *GetRandomStaticScene(); // 获取一个随机的普通场景对象
	bool GotoRandomStaticScene(Role *role);
	
	void DestroyTeamEquipFB(int fb_key, bool real_destroy = false);

	void OnUseGoldInHiddenGs(NetID net_id, const char *msg, int length);	// 用于记录隐藏服消耗元宝

	void OnSyncCrossSeasonChange(int cross_activity);  // 同步赛季变更

	void OnCrossBossInfoReq(crossgameprotocal::GameGameBossInfoReq *ggbir);
	void OnCrossMizangBossInfoReq(crossgameprotocal::GameGameBossInfoReq *ggbir);
	void OnCrossYoumingBossInfoReq(crossgameprotocal::GameGameBossInfoReq *ggbir);
	void SendCrossBossDropRecord(long long uuid);

	int GetAllServerRoleLevelNum(int min_level, int max_level);  // 获取等级区间的角色个数

protected:
	bool ChangeScene(Scene *scene, ObjID obj_id, int target_scene_id, int target_scene_key, const Posi &target_pos);
	bool ChangeLocalSceneHelper(Scene *old_scene, Role *role, Scene *target_scene, const Posi &target_pos);
	bool CreateScene(const ConfigScene &scene_config, int scene_key, Scene **scene_create, int scene_type, unsigned int scene_timeout, 
		bool timeout_kick);

	void EraseTimeOut();
	void EraseSpeedup();

	bool SaveRole(Role *role, int target_scene_id, const Posi &target_pos, RMIRoleSaveBackObject *rsbo);
	
	int GetSceneIDBySceneType(int scene_type);

	World *m_world;
	unsigned long m_game_time;

	typedef RAObjList<Scene*> SceneList;
	typedef std::map<int, Map*> MapList;

	SceneList m_scene_list;
	MapList m_map_list;

	struct SceneIDKey 
	{
		SceneIDKey() : scene_id(0), scene_key(0) {}
		SceneIDKey(int sid, int skey) : scene_id(sid), scene_key(skey) {}

		int scene_id;
		int scene_key;
		bool operator < (const SceneIDKey &v) const 
		{
			return (scene_id < v.scene_id) || (scene_id == v.scene_id && scene_key < v.scene_key);
		}
	};

	typedef std::map<SceneIDKey, SceneIndex> SceneIDMap;
	SceneIDMap m_sceneidmap;												// 用于映射游戏配置中的sceneid到实际运行时在m_scene_list中的下标

	typedef std::vector<SceneIndex> DestroySceneList;						// 场景的延迟删除
	DestroySceneList m_destroy_scene_list;

	void DestroyScene();

	struct RoleLocal 
	{
		RoleLocal() : scene_index(INVALID_OBJ_ID), obj_id(INVALID_OBJ_ID), last_active_time(0) {}
		RoleLocal(SceneIndex _s, ObjID _u, unsigned long lat) : scene_index(_s), obj_id(_u), last_active_time(lat) {}

		SceneIndex	scene_index;
		ObjID	obj_id;
		unsigned long last_active_time;
	};

	typedef std::map<GSNetID, RoleLocal> NetSceneMap;
	NetSceneMap m_net_to_rolelocal;

protected:
	typedef std::map<UserID, RoleLocal> UIDSceneMap;
	UIDSceneMap m_uid_to_rolelocal;
	
	MessageHandler	m_message_handler;

	typedef std::map<int, FBConfig> FBConfigList;
	FBConfigList m_fb_config_list;

	std::map<int, int> m_scene_id_to_type_map;				// 场景ID对应场景类型（方便查询）
	
	unsigned long m_last_check_timeout;						// 上次检查超时的时间
	unsigned long m_check_timeout_interval;					// 检查timeout的时间间隔
	bool m_check_timeout;									// 是否检查超时
	unsigned long m_heartbeat_dead_time;					// 多长时间后没有心跳则认为该连接死亡 果断地断开
	time_t m_last_chk_speedup_time;							// 上次检查加速时间

	bool m_is_forbid_sublock;								// 是否禁止二级锁
	bool m_is_micro_pc_open;								// 是否开放登陆器任务
	bool m_is_monitor_auto_forbid;							// 是否聊天邮件等出现异常自动封号

	struct ServerRoleNum
	{
		ServerRoleNum() { this->Reset(); }
		void Reset() { memset(role_num_list, 0, sizeof(role_num_list)); };

		short role_num_list[MAX_ROLE_LEVEL + 1];
	};
	std::map<long long, ServerRoleNum> m_unique_server_role_num; // 普通服的角色等级人数表 map<原服唯一ID，角色等级人数>

protected:
	struct GS
	{
		int index;
	};
	typedef std::map<int, GS> RemoteGSList;
	RemoteGSList m_remote_gs_list;

	CrossUserMap m_cross_user_map;							// 处于跨服状态的角色标记信息

	/////////////////////////////////////////// 跨服相关 ////////////////////////////////////////////////////////////////////////////
protected:
	OnCrossGameMsgHandler m_on_cross_handler[1024];

	void InitCrossHandler();
	void OnSyncCrossUser(NetID net_id, const char *data, int length);
	void OnSaveCrossRoleInfo(NetID net_id, const char *data, int length);
	void OnStartCrossAck(NetID net_id, const char *msg, int length);
	void OnCrossUserMsg(NetID net_id, const char *data, int length);
	void OnCrossAllUserMsg(NetID net_id, const char *data, int length);
	void OnCrossSyncActivityStatus(NetID net_id, const char *data, int length);
	void OnCrossXiuluoTowerGotoScene(NetID net_id, const char *data, int length);
	void OnSyncXiuluotaUserResult(NetID net_id, const char *data, int length);
	void OnSyncXiuluotaRankTitle(NetID net_id, const char* data, int length);
	void OnKickCrossUser(NetID net_id, const char *data, int length);
	void OnCrossGetRoleInfoRequest(NetID net_id, const char *data, int length);
	void OnCrossHello(NetID net_id, const char *data, int length);
	void OnCrossSyncTitleInfo(NetID net_id, const char *data, int length);
	void OnCrossForbidRoleTalk(NetID net_id, const char *data, int length);
	void OnCrossGame1V1GotoScene(NetID net_id, const char *data, int length);
	void OnCrossGame1V1Info(NetID net_id, const char *data, int length);
	void OnSyncCross1v1MatchInfo(NetID net_id, const char *msg, int length);

	void OnCrossMultiuserChallengeStartMatch(NetID net_id, const char *msg, int length);
	void OnSyncCrossMultiuserChallengeMatchingState(NetID net_id, const char *data, int length);
	void OnCrossGameMultiuserChallengeGotoScene(NetID net_id, const char *data, int length);
	void OnCrossGlobalMultiuserChallengeScoreRankReward(NetID net_id, const char *data, int length);

	void OnCrossGameUpdateCrossRecord(NetID net_id, const char *msg, int length);
	void OncrossGameRankWeekRewardCommand(NetID net_id, const char *msg, int length);

	void OnCrossTuanzhanGotoScene(NetID net_id, const char *data, int length);

	void OnCrossBossCreateBossBroadcast(NetID net_id, const char *data, int length);
	void OnCrossBossBossInfoReq(NetID net_id, const char *data, int length);

	void OnSyncCrossTeamFBUserInfo(NetID net_id, const char *data, int length);

	void OnCrossGameSyncRoleData(NetID net_id, const char *msg, int length);
	void OnCrossGameChannelChat(NetID net_id, const char *msg, int length);
	void OnCrossGameRankInfoReq(NetID net_id, const char *msg, int length);
	void OnCrossActivityGotoScene(NetID net_id, const char *data, int length);
	void OnCrossGameSendMailToUser(NetID net_id, const char *msg, int length);
	void OnCrossGameSingleChat(NetID net_id, const char *msg, int length);
	void OnCrossGameLocalSpeaker(NetID net_id, const char *msg, int length);

	void OnCrossGuildBattleGetBossInfoResp(NetID net_id, const char *data, int length);
	void OnCrossGuildBattleGetMonsterInfoResp(NetID net_id, const char *data, int length);
	void OnCrossGuildBattleSpecialTimeNotice(NetID net_id, const char *data, int length);
	void OnSyncCrossGuildBattleUserScoreRankTop(NetID net_id, const char *data, int length);
	void OnSyncCrossGuildBattleRegisterReq(NetID net_id, const char *data, int length);
	void OnSyncCrossGuildBattleRegister(NetID net_id, const char *data, int length);

	void OnCrossFishingInfoSync(NetID net_id, const char *data, int length);

	void OnCross1v1ScoreRankReward(NetID net_id, const char *msg, int length);

	void OnSyncWorldCommonDataChange(NetID net_id, const char *data, int length);

	void OnCrossWorldLevel(NetID net_id, const char *data, int length);
	void OnCrossGameLieKunFBKillZoneBossSync(NetID net_id, const char *data, int length);
	void OnCrossGameLieKunFBTopRewardSync(NetID net_id, const char *data, int length);

	void OnCrossGameMessBattleRankTopSync(NetID net_id, const char *data, int length);
	void OnCrossGameNightFightRankTopSync(NetID net_id, const char *data, int length);

	void OnSyncCrossRankFirst(NetID net_id, const char *data, int length);

	void OnSyncCrossCoupleRankFirst(NetID net_id, const char *data, int length);

	void OnCrossCommonBossInfoChange(NetID net_id, const char *msg, int length);
	void OnSyncCrossCommonBossInfoReq(NetID net_id, const char *msg, int length);
	void OnSyncCrossCommonBossInfo(NetID net_id, const char *msg, int length);

	void OnCrossCommonBossDropInfo(NetID net_id, const char *msg, int length);

	// 接收原服同步过来的角色人数
	void OnSyncRoleNumToHidden(NetID net_id, const char *msg, int length);

	void OnSyncGameServerTimeInfo(NetID net_id, const char *msg, int length);

	void OnCrossGameSyncCrossRandActivityData(NetID net_id, const char *msg, int length);

	void OnGameCrossGuildBattleSos(NetID net_id, const char *msg, int length);

	void OnGameCrossKillCrossBoss(NetID net_id, const char *msg, int length);

	void OnPickGuildRareItemInHiddenGs(NetID net_id, const char *msg, int length);

	void OnSyncRareItemFromOriginalGs(NetID net_id, const char *msg, int length);

	void OnBroadcastCrossRankToGuildChat(NetID net_id, const char *msg, int length);

	// 接收并下发Protocol::SCSystemMsg
	void OnRecvAndTransferCrossSystemMsg(NetID net_id, const char *msg, int length);

	// 隐藏服请求发送仙盟聊天
	void OnCrossRequestGuildChat(NetID net_id, const char *msg, int length);

	void OnGameCrossGuildMemberBeKill(NetID net_id, const char *msg, int length);
	void OnCrossGameGetGuildEnemyList(NetID net_id, const char *msg, int length);

	// 跨服竞技场
	void OnSyncCrossChallengefiledInfoReq(NetID net_id, const char *msg, int length);
	void OnSyncCrossChallengefiledInfo(NetID net_id, const char *msg, int length);

	// 跨服守护水晶
	void OnSyncCrossHusongShuijingGatherInfo(NetID net_id, const char *msg, int length);
};

#endif

