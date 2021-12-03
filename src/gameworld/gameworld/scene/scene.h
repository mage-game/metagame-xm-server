#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include <functional>
#include "common/raobjlist.h"

#include "obj/objmanager.h"
#include "obj/singlemanager.h"

#include "aoi/zonematrix.h"
#include "transportlist.h"
#include "npc/npcmanager.h"
#include "servercommon/logindef.h"

#include "config/sceneconfig.h"
#include "scene/generater/scenemonsterlogic.h"
#include "servercommon/crossdef.hpp"
#include "inetworkmodule.h"

class Monster;
class Role;
class GatherObj;
class Map;
class SceneManager;
class SpecialLogic;
class FallingItem;
class RoleShadow;

class RoleInitParam;
class RoleOtherInitParam;
class ItemListParam;
class HotkeyListParam;
class FriendListParam;
class BlacklistsParam;
class EnemyListParam;
class BusinessListParam;
class SkillListParam;
class DailyFindListParam;
class RoleAllParam;

static const int MAX_SCENE_NAME_LENGTH = 32;
static const int SCENE_NO_ONE_UPDATE_INTERVAL = 1000;	// 无人场景的刷新间隔

class Scene
{
public:

	/*
		有四类副本如下
		1、个人副本 都是延迟删除 保证掉线重入 (update中删除)
		2、战场副本 永远不销毁
		3、组队副本 所有人退出就销毁 （onremoveobj中删除）
		4、很多人的副本（婚宴、仙盟副本）有活动时间的，不能没人就销毁，不断检测是否销毁 (update中删除)
	*/

	enum FB_TYPE
	{
		SCENE_TYPE_COMMON_SCENE = 0,									// 普通场景（不销毁）
		SCENE_TYPE_GUILD_STATION = 1,									// 军团驻地（不销毁）
		SCENE_TYPE_ZHUXIE = 2,											// 诛邪战场（不销毁）
		SCENE_TYPE_COINFB = 3,											// 金币本 (个人副本延迟删除)(过图）
		SCENE_TYPE_EXPFB = 4,											// 经验副本 (可个人，可组队副本)
		SCENE_TYPE_QUNXIANLUANDOU = 5,									// 群仙乱斗（不销毁）
		SCENE_TYPE_PERSONAL_TOWERDEFEND = 6,							// 单人塔防 (个人副本延迟删除) 
		SCENE_TYPE_PHASE_FB = 7,										// 阶段副本 (个人副本延迟删除)
		SCENE_TYPE_GONGCHENGZHAN = 8,									// 攻城战   （不销毁） 
		SCENE_TYPE_XIANMENGZHAN = 9,									// 仙盟战   （不销毁）
		SCENE_TYPE_CAMP_STATION = 10,									// 阵营驻地 （不销毁）
		SCENE_TYPE_HUN_YAN_FB = 11,										// 婚宴副本  (不断检测删除，因为有活动时间)
		SCENE_TYPE_NATIONAL_BOSS = 12,									// 全民boss （不销毁）
		SCENE_TYPE_CHALLENGE_FB = 13,									// 挑战副本  (个人副本延迟删除)（爬塔）
		SCENE_TYPE_GUILD_MONSTER = 14,									// 仙盟神兽  (个人副本延迟删除)
		SCENE_TYPE_CHALLENGE_FIELD = 15,								// 1v1竞技场（没人就销毁 在RemoveObj处理）
		SCENE_TYPE_STORY_FB = 16,										// 剧情副本  (个人副本延迟删除)
		SCENE_TYPE_TAOSHOUJITAN_TEAM_FB = 17,							// 妖兽祭坛  （没人就销毁 在RemoveObj处理）
		SCENE_TYPE_LOVERS_FB = 18,										// 情缘副本  （没人就销毁 在RemoveObj处理）
		SCENE_TYPE_ZHANSHENDIAN_FB = 19,								// 战神殿副本 (个人副本延迟删除)
		SCENE_TYPE_SHENMOZHIXI_FB = 20,									// 神魔之隙副本(个人副本延迟删除)
		SCENE_TYPE_YIZHANDAODI = 21,									// 一站到底 （不销毁）
		SCENE_TYPE_CROSS_GUILDBATTLE = 22,								// 跨服帮派战
		SCENE_TYPE_GUILD_FB = 23,										// 仙盟副本  (不断检测删除，因为有活动时间)
		SCENE_TYPE_WUXINGGUAJI = 24,									// 五行挂机  （不销毁）
		SCENE_TYPE_ZHUANZHI_PERSONAL = 25,								// 个人转职副本 (个人副本延迟删除)
		SCENE_TYPE_MIGONGXIANFU_TEAM = 26,								// 迷宫仙府（没人就销毁 在RemoveObj处理）
		SCENE_TYPE_WUSHUANG_FB = 27,									// 无双副本 (个人副本延迟删除)（爬塔）	
		SCENE_TYPE_CROSS_XIULUO_TOWER = 28,								// 跨服修罗塔
		SCENE_TYPE_CROSS_1V1 = 29,										// 跨服1V1
		SCENE_TYPE_CROSS_MULTIUSER_CHALLENGE = 30,						// 跨服3V3
		SCENE_TYPE_PATA_FB = 31,										// 爬塔副本 (个人副本延迟删除)（爬塔）
		SCENE_TYPE_GUILD_BOSS = 32,										// 仙盟BOSS场景 （第一人进去创建，永久存在，停服重建）
		SCENE_TYPE_YAOSHOU_GUANGCHANG = 33,								// 妖兽广场 场景 （不断检测删除，因为有活动时间）
		SCENE_TYPE_SUOYAOTA = 34,										// 锁妖塔场景 （不断检测删除，因为有活动时间）
		SCENE_TYPE_SHUIJING = 35,										// 水晶（不销毁）
		SCENE_TYPE_TEAM_ZHUAGUI = 36,									// 组队抓鬼  （没人就销毁 在RemoveObj处理）
		SCENE_TYPE_CAMPGAOJI_DUOBAO = 37,								// 师门高级夺宝(个人副本延迟删除)
		SCENE_TYPE_CROSS_TUANZHAN = 38,									// 跨服团战
		SCENE_TYPE_CROSS_PASTURE = 39,									// 跨服牧场
		SCENE_TYPE_VIP_FB = 40,											// VIP副本(现在作为bossfamily的特殊场景类型进行调用) （不销毁）
		SCENE_TYPE_GUILDBATTLE = 41,									// 公会争霸 (活动结束删除) 
		SCENE_TYPE_NO_USE = 42,											// 无用，新地图类型替换掉 
		SCENE_TYPE_TIANJIANGCAIBAO = 43,								// 天降财宝	(活动结束删除) 
		SCENE_TYPE_CROSS_BOSS = 44,										// 跨服boss	
		SCENE_TYPE_HOTSPRING = 45,										// 温泉	(活动结束删除) 
		SCENE_TYPE_WANGLINGEXPLORE = 46,								// 王陵探险	(活动结束删除) 
		SCENE_TYPE_CROSS_TEAM_FB = 47,									// 跨服组队 (没人就销毁)	
		SCENE_TYPE_TERRITORY_WAR = 48,									// 领土战	(活动结束删除)
		SCENE_TYPE_MO_JIE = 49,											// 魔戒
		SCENE_TYPE_FUN_TYPE_MOUNT = 50,									// 功能开启副本-坐骑 (没人就销毁)
		SCENE_TYPE_FUN_TYPE_WING = 51,									// 功能开启副本-羽翼 (没人就销毁)
		SCENE_TYPE_FUN_TYPE_XIANNV = 52,								// 功能开启副本-女神 (没人就销毁)
		SCENE_TYPE_CROSS_SHUIJING = 53,									// 跨服水晶幻境
		SCENE_TYPE_FB_GUIDE = 54,										// 副本引导
		SCENE_TYPE_RUNE_TOWER = 55,										// 符文塔
		SCENE_TYPE_TEAM_EQUIP_FB = 56,									// 组队装备副本 (没人就销毁)
		SCENE_TYPE_MILLIONAIRE = 57,									// 大富豪 （活动结束删除）
		SCENE_TYPE_DAILY_TASK_FB = 58,									// 日常任务副本 （个人副本延迟删除）
		SCENE_TYPE_XINGZUOYIJI = 59,									// 星座遗迹 (不删除)
		SCENE_TYPE_TUITU_FB = 60,										// 推图副本 (个人副本延迟删除)
		SCENE_TYPE_FIGHTING = 61,										// 决斗场（挖矿、航海）(没人就销毁)
		SCENE_TYPE_QINYUAN_SHENGDI = 62,								// 情缘圣地（多人本不销毁）
		SCENE_TYPE_CSA_BOSS = 63,										// 合服boss(活动结束后删除)
		SCENE_TYPE_PERSONAL_EQUIPFB = 64,								// 新装备本 (个人副本延迟删除)(过图）
		SCENE_TYPE_CROSS_DARK_NIGHT = 65,								// 月黑风高 (珍宝秘境)
		SCENE_TYPE_BABY_BOSS = 66,										// 宝宝boss(不删除)
		SCENE_TYPE_SHANGGU_BOSS = 67,								    // 上古BOSS
		SCENE_TYPE_ARMOR_DEFEND = 68,									// 防具推图本（防具打造石）
		SCENE_TYPE_BUILD_TOWER = 69,									// 个人建塔/魂器 (个人副本延迟删除)
		SCENE_TYPE_FISHING = 70,										// 钓鱼(活动结束删除)
		SCENE_TYPE_NIGHT_FIGHT_FB = 71,									// 夜战王城
		SCENE_TYPE_MESS_BATTLE_FB = 72,									// 乱斗战场
		SCENE_TYPE_PERSON_BOSS = 73,								    // 个人BOSS副本(个人副本延迟删除)
		SCENE_TYPE_GUILD_QUESTION = 74,									// 帮派答题
		SCENE_TYPE_XINMO_FB = 75,                                       // 心魔副本(个人转职副本延迟删除)
		SCENE_TYPE_TEAM_TOWERDEFEND = 76,								// 组队守护  （没人就销毁 在RemoveObj处理）
		SCENE_TYPE_CROSS_LIEKUN_FB = 77,								// 猎鲲地带
		SCENE_TYPE_TEAM_EQUIPFB = 78,									// 组队爬塔-精英组队装备本(没人就销毁)
		SCENE_TYPE_HOLIDAY_GUARD_FB = 79,								// 节日守护(个人塔防)
		SCENE_TYPE_CROSS_MIZANG_BOSS = 80,                              // 跨服秘藏boss
		SCENE_TYPE_CROSS_YOUMING_BOSS = 81,                             // 跨服幽冥boss
		SCENE_TYPE_GODMAGIC_BOSS = 82,									// 神魔boss
		SCENE_TYPE_GIFT_HARVEST = 83,									// 礼物收割地图
		SCENE_TYPE_CROSS_BIANJINGZHIDI = 84,							// 跨服边境之地
		SCENE_TYPE_CROSS_CHALLENGE_FIELD = 85,							// 跨服竞技场（没人就销毁 在RemoveObj处理）
		SCENE_TYPE_CROSS_HUSONG_SHUIJING = 86,							// 跨服护送水晶
		SCENE_TYPE_TOWERDEFEND_TEST= 87,								// 塔防测试审核

		SCENE_TYPE_COUNT = 88,
	};

	struct SceneFBDynInfo
	{
		SceneFBDynInfo() { this->Reset(); }

		void Reset() { kill_boss = 0; kill_monster = 0; kill_monster_count_map.clear(); }

		short kill_boss;
		short kill_monster;

		typedef std::map<UInt16, short> KillMonsterCountMap;
		typedef std::map<UInt16, short>::iterator KillMonsterCountMapIt;
		KillMonsterCountMap kill_monster_count_map;
	};

	Scene(SceneManager *scene_manager);
	~Scene();

	bool Init(SceneIndex scene_index, const ConfigScene &configscene, Map *map, int scene_type, int scene_key, unsigned int scene_timeout, bool timeout_kick);
	bool ReInit(const ConfigScene &configscene, Map *map);
	void Release();

	SceneIndex GetIndex() { return m_index; }
	int GetSceneID() { return m_scene_id; }
	int GetSceneKey() { return m_scene_key; }
	int GetSceneType() { return m_scene_type; }
	unsigned int GetCreateTime() { return (unsigned int)m_scene_create_time; }
	unsigned int GetTimeOut() { return (unsigned int)m_scene_timeout; }
	const char * GetSceneName() { return m_scene_name; }
	Map * GetMap() { return m_map; }
	Transport * GetTransport(int tid) { return m_transport_list.GetTransport(tid); }
	Transport * GetOneTransport() { return m_transport_list.GetOneTransport(); }
	ZoneMatrix * GetZoneMatrix() { return &m_zone_matrix; }
	NPCManager * GetNPCList() { return &m_npc_list; }
	SceneFBDynInfo * GetSceneFBDynInfo() { return &m_fbsene_dyninfo; }												
	SpecialLogic *GetSpecialLogic() { return m_special_logic; }
	SceneMonsterLogic *GetMonsterLogic() { return &m_scene_monster_logic; }

	int GetMonsterUniqueKey() { return ++m_monster_unique_key; }

	void AddObj(Obj *obj);
	bool DeleteObj(ObjID uid);
	Obj * GetObj(ObjID id);
	void GetSceneStatus(int *obj_num, int *role_num, int *monster_num, int *pet_num, int *drop_item_num) const;
	Role * GetRoleByIndex(int index);
	Role * GetRoleByUID(int uid);
	Role * GetRoleByUUID(const UniqueUserID &uuid);
	Role * RandGetOneRole(int exclude_uid = 0);
	Monster * GetMonsterByIndex(int index);
	Monster *GetMonster(ObjID obj_id);
	Role * GetPrincipal(ObjID obj_id);
	GatherObj *GetGatherObj(ObjID obj_id);
	FallingItem * GetFallingItemByIndex(int index);
	GatherObj * GetGatherObjByIndex(int index);
	RoleShadow *GetRoleShadow(ObjID obj_id);
	void DeleteGatherObjByGatherId(int gather_id);

	UInt32 RoleNum() { return m_role_manager.Size(); }
	UInt32 MonsterNum() { return m_monster_manager.Size(); }
	int FallingItemNum() { return m_fallingitem_manager.Size(); }
	int GatherObjNum() { return m_gather_manager.Size(); }

	bool ResetScene(Role *role, bool reset_pos = true);

	int GetTownPoint(Role *role, Posi *pos = 0);
	bool GoToTownPoint(Role *role);
	bool ForbidPK();

	bool InCommonScene();
	bool InStaticScene();

	static bool IsStaticScene(int scene_type);
	static bool IsCrossGuildBattleScene(int scene_type);
	static bool	IsDelayDeleteScene(int scene_type, int scene_id);
	static bool IsPersonalScene(int scene_type, int scene_id);
	static bool IsTeamScene(int scene_type, int scene_id);
	static bool IsGuildScene(int scene_type, int scene_id);
	static bool IsGuajiScene(int scene_id);									// 是否是挂机场景
	static bool IsWorldBossScene(int scene_id);								// 是否是世界boss场景
	static bool IsWorldBossReliveScene(int scene_id);						// 是否是世界boss免费复活场景
	static bool IsBossDongKuScene(int scene_id);							// 是否是boss洞窟场景
	static bool IsBossFamilyScene(int scene_id);							// 是否是boss之家场景
	static bool IsYaoShouGuangChangScene(int scene_type, int scene_id);		// 是否是妖兽广场场景
	static bool IsSuoYaoTaScene(int scene_type, int scene_id);				// 是否是锁妖塔场景
	static bool IsDabaoBossScene(int scene_id);								// 是否是打宝场景
	static bool IsActiveBossScene(int scene_id);							// 是否是活跃boss场景
	static bool IsActiveBossSceneOneLayer(int scene_id);					// 是否是活跃boss第一层场景
	static bool IsCampNormalDuoBaoScene(int scene_id);						// 是否是师门夺宝场景
	static bool IsJuLingScene(int scene_id);								// 是否是聚灵场景
	static bool IsBabyTrialScene(int scene_id);								// 是否是宝宝试炼场景
	static bool IsQinYuanScene(int scene_id);								// 是否是仙缘奇景(夫妻光环产出)场景			
	static bool IsCangBaoMiJing(int scene_id);								// 是否是在藏宝秘境场景
	static bool IsShiFangShiJie(int scene_id);								// 是否是在十方世界
	static bool IsShangGuYiJi(int scene_id);								// 是否是在上古遗迹
	static bool IsBossScene(int scene_id);									// 是否在boss场景
	static bool IsShuijingScene(int scene_id);								// 是否在水晶幻境场景
	static bool IsPerciousBossScene(int scene_id);							// 是否在秘藏boss场景

	void CheckRoleTimeOut(unsigned long now);
	void CheckRoleSpeedup();

	void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool RoleEnterSceneAsyn(bool is_micro_pc, const GSNetID &netid, IP userip, const UserID & uid, PlatName pname, bool is_login, int plat_spid);
	void RoleEnterScene(bool is_micro_pc, 
						int plat_spid,
						PlatName pname, 
						const UserID & uid,
						const GSNetID &netid, 
						const RoleInitParam &param,
						const RoleOtherInitParam &other_p,
						const ItemListParam &item_list_param,
						const HotkeyListParam &hotkey,
						const FriendListParam &friends,
						const BlacklistsParam &blacklists_param,
						const EnemyListParam &enemy,
						const BusinessListParam &business_param,
						const SkillListParam &skill_param,
						const DailyFindListParam &dail_find_param,
						bool is_login = false,
						Role **p_role = 0);
	bool RoleLeaveScene(ObjID obj_id, bool is_logout = false);

	typedef bool (*RoleFilterFunc)(Role *role, void *func_param);
	bool SendToRole(const char *msg, int length, RoleFilterFunc role_filter = 0, void *func_param = 0);
	bool SendToRole(const char *msg, int length, std::function<bool(Role *)> filter_func);

	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);  // 场景内传闻
	
	void SendWorldBossHpInfo();

	void OnTrigger(int trigger_cond_type, Role *role = NULL, int trigger_cond_param0 = 0, int trigger_cond_param1 = 0);
	bool HasHiddenMonster();

	void SyncSceneTimeOut(Role *role);
	unsigned int GetSceneTimeOutStamp();

	void GetMonsterGeneraterInfo(Role *role);
	void GetGatherGeneraterInfo(Role *role);

	bool IsDropRecordBossScene();

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	void UpdateObj(unsigned long interval, time_t now_second, unsigned int now_dayid);
	void RemoveObj();
	int GetChannelID();

	SceneIndex m_index;										// scenemanager中的下标做为临时id
	int	m_scene_id;											// 整个游戏中识别场景的id
	int m_scene_key;
	SceneManager *m_scene_manager;
	
	char m_scene_name[MAX_SCENE_NAME_LENGTH];

	Map *m_map;
	ZoneMatrix m_zone_matrix;

	ObjManager m_obj_manager;
	SingleManager m_role_manager;
	SingleManager m_monster_manager;
	SingleManager m_fallingitem_manager;
	SingleManager m_trigger_manager;
	SingleManager m_gather_manager;

	typedef std::vector<ObjID>	RemoveObjList;				// 延迟删除操作，让Obj可以调用DeleteObj将自己移除
	RemoveObjList m_remove_obj_list;

	SceneMonsterLogic m_scene_monster_logic;

	TransportList m_transport_list;
	NPCManager m_npc_list;

protected:
	struct TownPoint 
	{
		TownPoint() : scene_id(0), pos(0, 0) {}
		int scene_id;
		Posi pos;
	};

	TownPoint		m_town_point;
	int				m_scene_type;

	bool			m_forbid_pk;							// 该场景是否允许pk

	time_t			m_scene_create_time;
	time_t			m_scene_timeout;
	bool			m_is_timeout_kick_role;

	bool			m_is_delay_delete_scene;				// 是否延迟删除
	time_t			m_delay_delete_scene_time;				// 延迟删除场景时间

	SceneFBDynInfo	m_fbsene_dyninfo;						// 副本动态信息
	int				m_monster_unique_key;					// 怪物KEY

	SpecialLogic	*m_special_logic;

	unsigned int	m_world_boss_last_brocast_time;			// 世界boss血量信息最后广播时间
	
	bool			m_guiji_scene_forbid_pk;				// 挂机地图是否禁止Pk

	unsigned long	m_scene_no_one_update_interval;			// 无人场景刷新间隔
};

#endif

