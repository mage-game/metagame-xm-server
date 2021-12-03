#ifndef _SPECIAL_GODMAGIC_BOSS_HPP_
#define _SPECIAL_GODMAGIC_BOSS_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgfb.h"
#include <set>
#include "servercommon/serverdef.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"

class Role;

class SpecialGodmagicBoss : public SpecialLogic
{
public:
	enum ENBOSSSTATUS
	{
		ENBOSSSTATUS_NONEXIST = 0,										// 不存在
		ENBOSSSTATUS_EXIST,												// 存在
	};

	struct GodmagicRoleInfo
	{
		GodmagicRoleInfo() : obj_id(0), is_gather_stauts(0), last_notice_gather_obj_id(0), next_notice_gather_timestamp(0), today_gather_ordinary_crystal_times(0), today_gather_treasure_crystal_times(0), 
			tire_value(0), next_reduce_hp_timestamp(0) {}

		ObjID obj_id;
		bool is_gather_stauts;												// 是否在采集中
		ObjID last_notice_gather_obj_id;									// 上次提醒了的采集物的objid
		unsigned int next_notice_gather_timestamp;							// 下次提醒采集物是否能被采集的时间戳

		int today_gather_ordinary_crystal_times;							// 今日已采集普通水晶次数
		int today_gather_treasure_crystal_times;							// 今日已采集珍惜水晶次数
		int tire_value;														// 疲劳值

		unsigned int next_reduce_hp_timestamp;								// 下次扣除血量时间
	};

	struct GodmagicBossBossKillerRecrod
	{
		GodmagicBossBossKillerRecrod() : uid(0), dead_timestamp(0)
		{
			memset(killer_name, 0, sizeof(GameName));
		}

		int uid;
		unsigned int dead_timestamp;
		GameName killer_name;
	};

	struct GodmagicBossInfo
	{
		GodmagicBossInfo() : boss_id(0), is_exist(0), next_flush_time(0), record_count(0), record_cur_index(0), has_notice_role(false){}

		int boss_id;
		int is_exist;
		unsigned int next_flush_time;
		int record_count;
		int record_cur_index;
		bool has_notice_role;
		GodmagicBossBossKillerRecrod killer_recrod_list[GODMAGIC_BOSS_MAX_HISTROY_RECROD];
	};

	struct GodmagicBossMonsterInfo
	{
		GodmagicBossMonsterInfo() : monster_id(0), is_exist(0), next_flush_time(0)
		{
		}

		int monster_id;
		int is_exist;
		unsigned int next_flush_time;
	};

	struct GodmagicBossOrdinaryCrystalInfo
	{
		GodmagicBossOrdinaryCrystalInfo() : index(0), is_exist(0), next_flush_time(0)
		{
		}

		int index;
		int is_exist;
		unsigned int next_flush_time;
	};

public:
	SpecialGodmagicBoss(Scene *scene);
	virtual ~SpecialGodmagicBoss();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	virtual void OnCharacterDie(Character *dead_cha, ObjID killer_objid);
	virtual void OnAddObj(Obj *obj);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnStartGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual bool SpecialRelive(Role *role, int realive_type);

	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual bool SpecialCanMountOn(Role *role) { return true; }
	virtual bool SpecialCanReAlive(Role *role, int realive_type);
	virtual bool CanDestroy() { return false; }
	void OnAttackCalHurt(Role *attacker, Monster *monster, Attribute &delta_hp);


	void OnStopGather(Role *role);
	void OnActivityOpen();
	void OnActivityClose();
	void SendInfoToRole(Role *role);
	void SendBossKillerRecordInfo(Role *role, int boss_id);
	void SyncSceneInfo(Role *role = NULL);
	void OnPickFallingItem(Role *role, int scene_id, unsigned short monster_id, ItemID item_id, const ItemDataWrapper &item_wrapper);
	bool AddGodmagicBossTireToDropOwn(Role *role);

	bool IsGodmagicBossTireFull(Role *role);
	bool DecGodmagicBossTire(Role *role, int tire_value);
	int GetBossCount() { return m_boss_count; }
	void GetGodmagicBossInfo(int index, Protocol::SCGodmagicBossInfoAck::BossInfo &boss_info, short &left_treasure_crystal_count, short &left_monster);
	bool ConcernBoss(int uid, int boss_id);
	bool UnConcernBoss(int uid, int boss_id);

	void CreateBossBroadcast(int notify_reason, int scene_id, int boss_id, unsigned int next_refresh_time, Role *drop_owner_role);  // 跨服boss刷新通知
protected:
	void RefreshTreasureCrystal();
	void CleanAllTreasureCrystal();
	void RefreshOrdinaryCrystal();
	void RefreshMonster();
	void RefreshBoss();
	void AddExp();
	void ReduceGatherRoleHp();
	void SetGatherStautsFalse(Role *role);

	bool m_first;
	int m_boss_count;
	int m_monster_count;
	int m_left_monster_count;
	int m_ordinary_crystal_count;
	int m_left_treasure_crystal_num;
	unsigned int m_boss_next_flush_timestamp;
	unsigned int m_monster_next_flush_timestamp;
	unsigned int m_next_add_exp_timestamp;
	unsigned int m_treasure_crystal_next_flush_timestamp;
	unsigned int m_ordinary_crystal_next_flush_timestamp;

	GodmagicBossInfo m_boss_list[MAX_GODMAGIC_BOSS_PER_SCENE];
	GodmagicBossMonsterInfo m_monster_list[GODMAGIC_BOSS_MAX_MONSTER_NUM];
	GodmagicBossOrdinaryCrystalInfo m_ordinary_crystal_list[GODMAGIC_BOSS_MAX_ORDINARY_CRYSTAL];

	std::set<ObjID> m_treasure_crystal_obj_id_set;

	typedef std::map<int, GodmagicRoleInfo> GodmagicRoleInfoMap;
	typedef std::map<int, GodmagicRoleInfo>::iterator GodmagicRoleInfoMapIt;
	GodmagicRoleInfoMap m_user_map;

	std::map<int, std::set<int> > m_boss_register_role;  // 关注BOSS的角色列表 map<bossid, 角色uuid容器>
};


#endif // _SPECIAL_GODMAGIC_BOSS_HPP_

