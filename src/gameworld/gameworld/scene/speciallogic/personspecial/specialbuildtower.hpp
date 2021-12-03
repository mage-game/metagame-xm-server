#ifndef __SPECIAL_BUILD_TOWER_HPP__
#define __SPECIAL_BUILD_TOWER_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "other/fb/buildtowerfbconfig.hpp"
#include <map>

class SpecialBuildTower : public SpecialLogic
{
public:
	SpecialBuildTower(Scene *scene);
	virtual ~SpecialBuildTower();

	struct MovingMonsterInfo
	{
		MovingMonsterInfo() : obj_id(0), cur_pos_index(0) {}

		ObjID obj_id;
		int cur_pos_index;
	};

	struct TowerObjInfo
	{
		TowerObjInfo() { this->Reset(); }

		void Reset()
		{
			obj_id = INVALID_OBJ_ID;
			tower_type = -1;
			level = 0 ;
		}

		ObjID obj_id;
		short tower_type;
		short level;
	};

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool SpecialCanGuWu() { return false; }

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnLeaveFB(Role *role);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void OnAddObj(Obj *obj);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	void OnFBFinish(bool is_passed);

	void OnTowerOperate(int operate_type, int pos_index, int tower_type);
	void OnBuildTowerReq(int pos_index, int tower_type);
	void OnUpgradeTowerReq(int pos_index);
	void OnRemoveTowerReq(int pos_index);
	void OnFlushFirstWaveReq();
	void OnCallExtraMonsterReq();

	void OnBuyExtraMonsterReq(int monster_num);
	void OnCreateSpMonster();

protected:
	Role * GetOwnerRole();
	void SyncFBSceneLogicInfo(int notify_reason = 0, bool is_active_leave_fb = false);
	
	void InitBackstage();
	bool FlushMonsterWave();
	bool FlushOneMonster();
	void AddToMoveList(ObjID obj_id);
	void RemoveFromMoveList(ObjID obj_id);
	void CheckMonsterEscape();
	bool IsTowerMonster(int monster_id);
	void AddSceneSpeed(Role *role);

	ObjID m_owner_objid; 
	UserID m_owner_user_id;

	long long m_role_gongji;

	bool m_is_init;

	bool m_is_finish;
	bool m_is_pass;

	int m_total_douhun;

	int m_cur_wave;
	int m_wave_flush_count_list[BUILD_TOWER_MAX_MONSTER_WAVE + 1];
	time_t m_boss_flush_timestamp_list[BUILD_TOWER_MAX_MONSTER_WAVE + 1];

	time_t m_flush_next_wave_timestamp;
	time_t m_flush_next_monster_timestamp;
	time_t m_delay_kickout_timestamp;

	bool m_can_call_extra_monster;		//限制额外怪刷新
	int m_escape_monster_count;
	long long m_total_exp;

	int m_xian_hun_stone_num;
	int m_hun_suipian_num;

	typedef std::map<ItemID, int> BuildTowerDropItemMap;
	typedef std::map<ItemID, int>::iterator BuildTowerDropItemMapIt;
	BuildTowerDropItemMap m_drop_item_map;

	typedef std::map<ObjID, MovingMonsterInfo> MovingMonsterInfoMap;
	typedef std::map<ObjID, MovingMonsterInfo>::iterator MovingMonsterInfoMapIt;
	MovingMonsterInfoMap m_moving_monster_step_map;

	TowerObjInfo m_tower_info_list[BUILD_TOWER_MAX_TOWER_POS_INDEX + 1];
	short m_notify_next_wave_timestamp; //客户端是否需要提示下一波时间

	int m_special_monster_num;			//额外怪数量

	bool m_can_buy_extra_monster;		//限制额外怪购买

	int m_remain_buyable_monster_num;	//剩余可购买额外怪数量
};

#endif // __SPECIAL_GUILD_TOWER_HPP__