#ifndef _SPECIAL_EQUIP_TEAM_HPP_
#define _SPECIAL_EQUIP_TEAM_HPP_

#include "scene/speciallogic/speciallogic.hpp"

#include <set>

/*
	精英组队装备本
*/


class SpecialEquipTeam : public SpecialLogic
{
public:
	SpecialEquipTeam(Scene *scene);
	virtual ~SpecialEquipTeam();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual bool CanDestroy() { return false; }

	virtual void OnAddObj(Obj *obj);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialCanTransport(Role *role);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	virtual bool OnTimeout();

	void OnJumpLayerReq(Role *role);
	void GMJumpLayerReq(Role *role, int to_layer);

	void SendSceneLogicInfo(Role *role);

protected:
	virtual void OnAllMonsterDead();
	void SyncSceneLogicInfo(Role *role = nullptr);

	void OnOver();
	void OnPassNoticeWord();          //通关传闻

	bool m_is_passed;											// 是否通关
	bool m_is_over;												// 是否结束

	bool m_is_first_enter;										// 第一次有人进入
	bool m_can_jump;											// 是否能跳层
	int m_total_monster_num;									// 怪物总数
	int m_totall_boss_num;										// boss总数
	int m_kill_boss_num;										// 已击杀boss数量
	int m_kill_monster_num;										// 已击杀怪物数量
	unsigned int m_jumplayer_limit_time;						// 跳层超时时间
	int m_monster_hp_percent;									// 怪物血量百分比

	int m_team_index;											// 组队下标
	int m_add_monster_maxhp_per;								// 增加怪物血量百分比
	int m_add_monster_gongji_per;								// 增加怪物攻击百分比
	int m_add_monster_fangyu_per;								// 增加怪物防御百分比

	std::set<UserID> m_rewardexp_user_id_set;					// 领过过关经验的玩家
};

#endif
