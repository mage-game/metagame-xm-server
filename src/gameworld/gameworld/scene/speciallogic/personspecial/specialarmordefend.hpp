#ifndef _SPECIAL_ARMORDEFEND_HPP_
#define _SPECIAL_ARMORDEFEND_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "other/fb/armordefendconfig.hpp"
#include "protocal/msgfb.h"

class SpecialArmorDefend : public SpecialLogic 
{
public:
	SpecialArmorDefend(Scene *scene);
	virtual ~SpecialArmorDefend();

	struct MovingMonsterInfo
	{
		MovingMonsterInfo() : obj_id(0), cur_pos_index(0) {}

		ObjID obj_id;
		int cur_pos_index;
	};

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnLeaveFB(Role *role);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);

	virtual bool SpecialCanRest(Role *role) { return false; }
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	void OnNextWaveReq(Role *role);

	virtual bool PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id);

	void SetRefreshWhenClean(short parm1);
	void SetIsPlayerGuider(bool is_guider) { m_is_new_player = is_guider; }
	void GmAddEnergy(int value);
	
private:
	Role * GetOwnerRole();
	void RefreshOne(time_t now_second);							// 从各出生点刷一只
	void SyncToClient(int reason = Protocol::SCTowerDefendInfo::NOTIFY_REASON_DEFAULT);
	void OnFinish(bool is_pass);
	void NotifyWarning();
	void SyncPerformSkill(int skill_index);
	void AddToMoveList(ObjID obj_id);
	void RemoveFromMoveList(ObjID obj_id);
	void CheckMonsterEscape();
	void CheckFbFinish();
	void SendArmorDefendDropInfo();
	bool IsCurrWaveRefreshAll();

	unsigned int m_init_time;									// 初始化时间
	bool m_is_finish;											// 是否结束
	bool m_is_pass;												// 是否成功
	bool m_is_last_wave;										// 是否最后一波
	bool m_msg_need_sync;										// 信息是否需要同步到客户端
	bool m_is_new_player;										// 是否新手引导

	int m_curr_wave_index;										// 当前波
	unsigned int m_next_wave_start_time;						// 下波刷怪时间
	unsigned int m_next_refresh_one_time;						// 下只刷新时间
	int m_refresh_left_count[ARMORDEFEND_PATH_MAX];				// 各刷怪点本波剩余数量

	UserID m_owner_user_id;
	ObjID m_owner_objid;
	int m_energy;

	int m_clear_wave_count;										// 杀光波数
	int m_kill_count[PERSONAL_TOWERDEFEND_MAX_WAVE_PER_LEVEL];	// 每波杀怪数量

	static const int ENERGY_MAX_NUM = 100;

	unsigned int m_next_perform_time_list[PERSONAL_ARMORDEFEND_SKILL_MAX_COUNT];// 技能上次释放时间

	int m_get_coin;
	int m_get_item_count;
	ItemConfigData m_reward_item_list[Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM];

	int m_escape_monster_count;
	int m_escape_boss_count;
	typedef std::map<ObjID, MovingMonsterInfo> MovingMonsterInfoMap;
	typedef std::map<ObjID, MovingMonsterInfo>::iterator MovingMonsterInfoMapIt;
	MovingMonsterInfoMap m_moving_monster_step_map;
	short m_refresh_when_clean;				//清完怪立即刷新
	unsigned int m_pass_kick_out_time;		//踢出玩家
};

#endif



