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
static const int SCENE_NO_ONE_UPDATE_INTERVAL = 1000;	// ���˳�����ˢ�¼��

class Scene
{
public:

	/*
		�����ั������
		1�����˸��� �����ӳ�ɾ�� ��֤�������� (update��ɾ��)
		2��ս������ ��Զ������
		3����Ӹ��� �������˳������� ��onremoveobj��ɾ����
		4���ܶ��˵ĸ��������硢���˸������лʱ��ģ�����û�˾����٣����ϼ���Ƿ����� (update��ɾ��)
	*/

	enum FB_TYPE
	{
		SCENE_TYPE_COMMON_SCENE = 0,									// ��ͨ�����������٣�
		SCENE_TYPE_GUILD_STATION = 1,									// ����פ�أ������٣�
		SCENE_TYPE_ZHUXIE = 2,											// ��аս���������٣�
		SCENE_TYPE_COINFB = 3,											// ��ұ� (���˸����ӳ�ɾ��)(��ͼ��
		SCENE_TYPE_EXPFB = 4,											// ���鸱�� (�ɸ��ˣ�����Ӹ���)
		SCENE_TYPE_QUNXIANLUANDOU = 5,									// Ⱥ���Ҷ��������٣�
		SCENE_TYPE_PERSONAL_TOWERDEFEND = 6,							// �������� (���˸����ӳ�ɾ��) 
		SCENE_TYPE_PHASE_FB = 7,										// �׶θ��� (���˸����ӳ�ɾ��)
		SCENE_TYPE_GONGCHENGZHAN = 8,									// ����ս   �������٣� 
		SCENE_TYPE_XIANMENGZHAN = 9,									// ����ս   �������٣�
		SCENE_TYPE_CAMP_STATION = 10,									// ��Ӫפ�� �������٣�
		SCENE_TYPE_HUN_YAN_FB = 11,										// ���縱��  (���ϼ��ɾ������Ϊ�лʱ��)
		SCENE_TYPE_NATIONAL_BOSS = 12,									// ȫ��boss �������٣�
		SCENE_TYPE_CHALLENGE_FB = 13,									// ��ս����  (���˸����ӳ�ɾ��)��������
		SCENE_TYPE_GUILD_MONSTER = 14,									// ��������  (���˸����ӳ�ɾ��)
		SCENE_TYPE_CHALLENGE_FIELD = 15,								// 1v1��������û�˾����� ��RemoveObj����
		SCENE_TYPE_STORY_FB = 16,										// ���鸱��  (���˸����ӳ�ɾ��)
		SCENE_TYPE_TAOSHOUJITAN_TEAM_FB = 17,							// ���޼�̳  ��û�˾����� ��RemoveObj����
		SCENE_TYPE_LOVERS_FB = 18,										// ��Ե����  ��û�˾����� ��RemoveObj����
		SCENE_TYPE_ZHANSHENDIAN_FB = 19,								// ս���� (���˸����ӳ�ɾ��)
		SCENE_TYPE_SHENMOZHIXI_FB = 20,									// ��ħ֮϶����(���˸����ӳ�ɾ��)
		SCENE_TYPE_YIZHANDAODI = 21,									// һվ���� �������٣�
		SCENE_TYPE_CROSS_GUILDBATTLE = 22,								// �������ս
		SCENE_TYPE_GUILD_FB = 23,										// ���˸���  (���ϼ��ɾ������Ϊ�лʱ��)
		SCENE_TYPE_WUXINGGUAJI = 24,									// ���йһ�  �������٣�
		SCENE_TYPE_ZHUANZHI_PERSONAL = 25,								// ����תְ���� (���˸����ӳ�ɾ��)
		SCENE_TYPE_MIGONGXIANFU_TEAM = 26,								// �Թ��ɸ���û�˾����� ��RemoveObj����
		SCENE_TYPE_WUSHUANG_FB = 27,									// ��˫���� (���˸����ӳ�ɾ��)��������	
		SCENE_TYPE_CROSS_XIULUO_TOWER = 28,								// ���������
		SCENE_TYPE_CROSS_1V1 = 29,										// ���1V1
		SCENE_TYPE_CROSS_MULTIUSER_CHALLENGE = 30,						// ���3V3
		SCENE_TYPE_PATA_FB = 31,										// �������� (���˸����ӳ�ɾ��)��������
		SCENE_TYPE_GUILD_BOSS = 32,										// ����BOSS���� ����һ�˽�ȥ���������ô��ڣ�ͣ���ؽ���
		SCENE_TYPE_YAOSHOU_GUANGCHANG = 33,								// ���޹㳡 ���� �����ϼ��ɾ������Ϊ�лʱ�䣩
		SCENE_TYPE_SUOYAOTA = 34,										// ���������� �����ϼ��ɾ������Ϊ�лʱ�䣩
		SCENE_TYPE_SHUIJING = 35,										// ˮ���������٣�
		SCENE_TYPE_TEAM_ZHUAGUI = 36,									// ���ץ��  ��û�˾����� ��RemoveObj����
		SCENE_TYPE_CAMPGAOJI_DUOBAO = 37,								// ʦ�Ÿ߼��ᱦ(���˸����ӳ�ɾ��)
		SCENE_TYPE_CROSS_TUANZHAN = 38,									// �����ս
		SCENE_TYPE_CROSS_PASTURE = 39,									// �������
		SCENE_TYPE_VIP_FB = 40,											// VIP����(������Ϊbossfamily�����ⳡ�����ͽ��е���) �������٣�
		SCENE_TYPE_GUILDBATTLE = 41,									// �������� (�����ɾ��) 
		SCENE_TYPE_NO_USE = 42,											// ���ã��µ�ͼ�����滻�� 
		SCENE_TYPE_TIANJIANGCAIBAO = 43,								// �콵�Ʊ�	(�����ɾ��) 
		SCENE_TYPE_CROSS_BOSS = 44,										// ���boss	
		SCENE_TYPE_HOTSPRING = 45,										// ��Ȫ	(�����ɾ��) 
		SCENE_TYPE_WANGLINGEXPLORE = 46,								// ����̽��	(�����ɾ��) 
		SCENE_TYPE_CROSS_TEAM_FB = 47,									// ������ (û�˾�����)	
		SCENE_TYPE_TERRITORY_WAR = 48,									// ����ս	(�����ɾ��)
		SCENE_TYPE_MO_JIE = 49,											// ħ��
		SCENE_TYPE_FUN_TYPE_MOUNT = 50,									// ���ܿ�������-���� (û�˾�����)
		SCENE_TYPE_FUN_TYPE_WING = 51,									// ���ܿ�������-���� (û�˾�����)
		SCENE_TYPE_FUN_TYPE_XIANNV = 52,								// ���ܿ�������-Ů�� (û�˾�����)
		SCENE_TYPE_CROSS_SHUIJING = 53,									// ���ˮ���þ�
		SCENE_TYPE_FB_GUIDE = 54,										// ��������
		SCENE_TYPE_RUNE_TOWER = 55,										// ������
		SCENE_TYPE_TEAM_EQUIP_FB = 56,									// ���װ������ (û�˾�����)
		SCENE_TYPE_MILLIONAIRE = 57,									// �󸻺� �������ɾ����
		SCENE_TYPE_DAILY_TASK_FB = 58,									// �ճ����񸱱� �����˸����ӳ�ɾ����
		SCENE_TYPE_XINGZUOYIJI = 59,									// �����ż� (��ɾ��)
		SCENE_TYPE_TUITU_FB = 60,										// ��ͼ���� (���˸����ӳ�ɾ��)
		SCENE_TYPE_FIGHTING = 61,										// ���������ڿ󡢺�����(û�˾�����)
		SCENE_TYPE_QINYUAN_SHENGDI = 62,								// ��Եʥ�أ����˱������٣�
		SCENE_TYPE_CSA_BOSS = 63,										// �Ϸ�boss(�������ɾ��)
		SCENE_TYPE_PERSONAL_EQUIPFB = 64,								// ��װ���� (���˸����ӳ�ɾ��)(��ͼ��
		SCENE_TYPE_CROSS_DARK_NIGHT = 65,								// �ºڷ�� (�䱦�ؾ�)
		SCENE_TYPE_BABY_BOSS = 66,										// ����boss(��ɾ��)
		SCENE_TYPE_SHANGGU_BOSS = 67,								    // �Ϲ�BOSS
		SCENE_TYPE_ARMOR_DEFEND = 68,									// ������ͼ�������ߴ���ʯ��
		SCENE_TYPE_BUILD_TOWER = 69,									// ���˽���/���� (���˸����ӳ�ɾ��)
		SCENE_TYPE_FISHING = 70,										// ����(�����ɾ��)
		SCENE_TYPE_NIGHT_FIGHT_FB = 71,									// ҹս����
		SCENE_TYPE_MESS_BATTLE_FB = 72,									// �Ҷ�ս��
		SCENE_TYPE_PERSON_BOSS = 73,								    // ����BOSS����(���˸����ӳ�ɾ��)
		SCENE_TYPE_GUILD_QUESTION = 74,									// ���ɴ���
		SCENE_TYPE_XINMO_FB = 75,                                       // ��ħ����(����תְ�����ӳ�ɾ��)
		SCENE_TYPE_TEAM_TOWERDEFEND = 76,								// ����ػ�  ��û�˾����� ��RemoveObj����
		SCENE_TYPE_CROSS_LIEKUN_FB = 77,								// ����ش�
		SCENE_TYPE_TEAM_EQUIPFB = 78,									// �������-��Ӣ���װ����(û�˾�����)
		SCENE_TYPE_HOLIDAY_GUARD_FB = 79,								// �����ػ�(��������)
		SCENE_TYPE_CROSS_MIZANG_BOSS = 80,                              // ����ز�boss
		SCENE_TYPE_CROSS_YOUMING_BOSS = 81,                             // �����ڤboss
		SCENE_TYPE_GODMAGIC_BOSS = 82,									// ��ħboss
		SCENE_TYPE_GIFT_HARVEST = 83,									// �����ո��ͼ
		SCENE_TYPE_CROSS_BIANJINGZHIDI = 84,							// ����߾�֮��
		SCENE_TYPE_CROSS_CHALLENGE_FIELD = 85,							// �����������û�˾����� ��RemoveObj����
		SCENE_TYPE_CROSS_HUSONG_SHUIJING = 86,							// �������ˮ��
		SCENE_TYPE_TOWERDEFEND_TEST= 87,								// �����������

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
	static bool IsGuajiScene(int scene_id);									// �Ƿ��ǹһ�����
	static bool IsWorldBossScene(int scene_id);								// �Ƿ�������boss����
	static bool IsWorldBossReliveScene(int scene_id);						// �Ƿ�������boss��Ѹ����
	static bool IsBossDongKuScene(int scene_id);							// �Ƿ���boss���߳���
	static bool IsBossFamilyScene(int scene_id);							// �Ƿ���boss֮�ҳ���
	static bool IsYaoShouGuangChangScene(int scene_type, int scene_id);		// �Ƿ������޹㳡����
	static bool IsSuoYaoTaScene(int scene_type, int scene_id);				// �Ƿ�������������
	static bool IsDabaoBossScene(int scene_id);								// �Ƿ��Ǵ򱦳���
	static bool IsActiveBossScene(int scene_id);							// �Ƿ��ǻ�Ծboss����
	static bool IsActiveBossSceneOneLayer(int scene_id);					// �Ƿ��ǻ�Ծboss��һ�㳡��
	static bool IsCampNormalDuoBaoScene(int scene_id);						// �Ƿ���ʦ�Ŷᱦ����
	static bool IsJuLingScene(int scene_id);								// �Ƿ��Ǿ��鳡��
	static bool IsBabyTrialScene(int scene_id);								// �Ƿ��Ǳ�����������
	static bool IsQinYuanScene(int scene_id);								// �Ƿ�����Ե�澰(���޹⻷����)����			
	static bool IsCangBaoMiJing(int scene_id);								// �Ƿ����ڲر��ؾ�����
	static bool IsShiFangShiJie(int scene_id);								// �Ƿ�����ʮ������
	static bool IsShangGuYiJi(int scene_id);								// �Ƿ������Ϲ��ż�
	static bool IsBossScene(int scene_id);									// �Ƿ���boss����
	static bool IsShuijingScene(int scene_id);								// �Ƿ���ˮ���þ�����
	static bool IsPerciousBossScene(int scene_id);							// �Ƿ����ز�boss����

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

	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);  // �����ڴ���
	
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

	SceneIndex m_index;										// scenemanager�е��±���Ϊ��ʱid
	int	m_scene_id;											// ������Ϸ��ʶ�𳡾���id
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

	typedef std::vector<ObjID>	RemoveObjList;				// �ӳ�ɾ����������Obj���Ե���DeleteObj���Լ��Ƴ�
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

	bool			m_forbid_pk;							// �ó����Ƿ�����pk

	time_t			m_scene_create_time;
	time_t			m_scene_timeout;
	bool			m_is_timeout_kick_role;

	bool			m_is_delay_delete_scene;				// �Ƿ��ӳ�ɾ��
	time_t			m_delay_delete_scene_time;				// �ӳ�ɾ������ʱ��

	SceneFBDynInfo	m_fbsene_dyninfo;						// ������̬��Ϣ
	int				m_monster_unique_key;					// ����KEY

	SpecialLogic	*m_special_logic;

	unsigned int	m_world_boss_last_brocast_time;			// ����bossѪ����Ϣ���㲥ʱ��
	
	bool			m_guiji_scene_forbid_pk;				// �һ���ͼ�Ƿ��ֹPk

	unsigned long	m_scene_no_one_update_interval;			// ���˳���ˢ�¼��
};

#endif

