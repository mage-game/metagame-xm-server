#ifndef _SPECIAL_TEAM_ZHUAGUI_HPP_
#define _SPECIAL_TEAM_ZHUAGUI_HPP_

#include "scene/speciallogic/speciallogic.hpp"

#include "protocal/msgfb.h"

class SpecialTeamZhuaGui : public SpecialLogic
{
public:
	SpecialTeamZhuaGui(Scene *scene);
	virtual ~SpecialTeamZhuaGui();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);
	virtual void GetPickFallingItem(Role *role, FBDropItemInfo *info_list, int &list_index, int list_max);

	virtual bool SpecialCanMountOn(Role *role) { return false; }

	void AddMember(Role *role);

private:
	virtual void OnAllMonsterDead();

	void NotifyZhuaGuiInfo(int reason, Role *role);
	void NotiryZhuaGuiAddPerInfo(Role *role = NULL);

	struct ZhuaGuiFBPersonalInfo
	{
		ZhuaGuiFBPersonalInfo() : uid(0), drop_item_num(0) {}

		int uid;
		int drop_item_num;		
		FBDropItemInfo drop_item_list[Protocol::SCZhuaGuiFbInfo::NEW_FB_REWARD_ITEM_SC_MAX];			// 掉落物品统计
	};

	bool m_first;
	bool m_boss_isflush;
	int m_boss_monster_id;

	bool m_is_has_couple;
	int m_team_member_count;
	int m_team_average_level;

	unsigned int  m_kick_time;

	Protocol::SCZhuaGuiFbInfo m_zgfi;
	ZhuaGuiFBPersonalInfo m_personal_info_list[MAX_TEAM_MEMBER_NUM];
};

#endif  // _SPECIAL_TEAM_ZHUAGUI_HPP_

