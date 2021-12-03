#ifndef __SPECIAL_WUXINGGUAJI_HPP__
#define __SPECIAL_WUXINGGUAJI_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"

#include <vector>
#include <map>

class Role;

class SpecialWuXingGuaJi : public SpecialLogic
{
public:
	SpecialWuXingGuaJi(Scene *scene);
	virtual ~SpecialWuXingGuaJi();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual bool CanDestroy() { return false; }

	virtual void OnCharacterDie(Character *dead_cha, ObjID killer_objid);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);
	virtual bool SpecialCanMountOn(Role *role){return false;}

	void OnActivityStatusChange(int status);

	void WuXingGuaJiGetReward(Role *role, Protocol::CSWuXingGuaJiGetReward *wxgjgr);

protected:
	void OnActivityStart();
	void OnActivityClose();

	void SendUserInfo(Role *role, int reason);
	void SendLuckyInfoToUser(Role *role = NULL);

	void GaveLuckyItemToUser();
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);
	WuXingGuaJiUser * GetWuXingGuaJiUser(const UserID &user_id);
	
	bool m_isopen;

	WuXingGuaJiUserMap m_wuxingguaji_user_map;

	struct BossInfo
	{
		BossInfo() : monster_id(0), boss_objid(INVALID_OBJ_ID), next_boss_flush_time(0) {}

		int monster_id;
		ObjID boss_objid;
		unsigned int next_boss_flush_time;
	};
	BossInfo m_boss_info_list[WUXINGGUAJI_BOSS_NUM];

	Protocol::SCWuXingGuaJiLuckyInfo m_wxgjli;	
};

#endif // __SPECIAL_WUXINGGUAJI_HPP__


