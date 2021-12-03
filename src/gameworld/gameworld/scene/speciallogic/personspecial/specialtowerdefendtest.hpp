#ifndef _SPECIAL_TOWERDEFEND_TEST_HPP_
#define _SPECIAL_TOWERDEFEND_TEST_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "other/fb/towerdefendconfig.hpp"
#include "protocal/msgfb.h"

const static int TOWERDEFEND_TEST_MONSTER_MINE = 20;
const static int TOWERDEFEND_TEST_MONSTER_MAX = 30;

class SpecialTowerDefendTest : public SpecialLogic 
{
public:
	SpecialTowerDefendTest(Scene *scene);
	virtual ~SpecialTowerDefendTest();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnLeaveFB(Role *role);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual bool OnTimeout();
	virtual bool CanDestroy();
	virtual void OnRemoveObj(Obj *obj);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	virtual bool SpecialCanReAlive(Role *role, int realive_type);
	virtual bool SpecialCanRest(Role *role) { return false; }
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	void OnNextWaveReq(Role *role);

private:
	Role * GetOwnerRole();
	void RefreshOne(time_t now_second);							// 从各出生点刷一只
	void SyncToClient(int reason = Protocol::SCTowerDefendInfo::NOTIFY_REASON_DEFAULT);
	void OnFinish(bool is_pass);
	bool OnNewPlayerLevelFinish();
	void CheckAttackMonsterStatus(unsigned long interval, time_t now_second);
	void OnAllAttackMonsterDead();
	void NotifyWarning(short warning_type, short percent);

	bool m_is_inited;											// 初始化
	bool m_is_finish;											// 是否结束
	bool m_is_pass;												// 是否成功
	bool m_is_last_wave;										// 是否最后一波
	bool m_msg_need_sync;										// 信息是否需要同步到客户端

	int m_curr_wave_index;										// 当前波

	unsigned int m_next_refresh_one_time;						// 下只刷新时间
	int m_refresh_left_count[TOWERDEFEND_PATH_MAX];				// 各刷怪点本波剩余数量


	long long m_life_tower_left_hp;								// 生命塔剩余HP
	long long m_life_tower_left_maxhp;							// 生命塔剩余最大HP

	UserID m_owner_user_id;
	ObjID m_owner_objid;

	int m_get_item_count;
	ItemConfigData m_reward_item_list[Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM];

	time_t m_send_drop_info_timestamp;

	bool m_need_refresh_monster;		//是否需要刷怪
};

#endif



