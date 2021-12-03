#ifndef _SPECIAL_BABY_BOSS_HPP_
#define _SPECIAL_BABY_BOSS_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include "protocal/msgfb.h"

class Role;
class FallingItem;

struct KillerInfoList
{
	KillerInfoList()
	{}

public:
	void Push(const Protocol::BabyBossInfo::KillerInfo &killer_info)
	{
		kill_info_list.push_back(killer_info);
		if (static_cast<int>(kill_info_list.size()) > Protocol::BabyBossInfo::KILLER_LIST_MAX_COUNT)
		{
			kill_info_list.pop_front();
		}
	}

	void GetKillerList(Protocol::BabyBossInfo::KillerInfo temp_kill_info_list[], int list_len)
	{
		if (list_len != Protocol::BabyBossInfo::KILLER_LIST_MAX_COUNT)
		{
			return;
		}

		int info_count = 0;
		for (auto it = kill_info_list.rbegin(); it != kill_info_list.rend(); ++it)
		{
			temp_kill_info_list[info_count++] = *it;
			if (info_count >= list_len)
			{
				break;
			}
		}
	}

private:

	std::deque<Protocol::BabyBossInfo::KillerInfo> kill_info_list;
};

class SpecialBabyBoss : public SpecialLogic
{
public:

	SpecialBabyBoss(Scene *scene);
	virtual ~SpecialBabyBoss();

	virtual void OnRoleEnterScene(Role* role) override;
	virtual void OnRoleLeaveScene(Role* role, bool is_logout) override;
	virtual bool CanDestroy() override { return false; }
	virtual void OnCharacterDie(Character *dead_cha, ObjID killer_objid) override;
	virtual void Update(unsigned long interval, time_t now_second) override;
	void AddAngryValueToDropOwner(Role *role, unsigned short monster_id);
	void AddAngryValueToKiller(Role *role, unsigned short monster_id);
	bool GetKillerInfo(unsigned short boss_id, Protocol::BabyBossInfo::KillerInfo temp_kill_info_list[], int list_len);

private:
	void CheckKickOut(unsigned int now_sec);
	void CheckAutoAddAngerValue(unsigned int interval, unsigned int now_sec);
	void AddAngryValue(Role *role, int value);

	std::map<int, unsigned int> m_user_auto_add_angry_value_map;
	std::map<int, unsigned int> m_user_logout_auto_kick_time_map;
	std::map<unsigned short, KillerInfoList> m_boss_kill_info_map;
};

#endif // _SPECIAL_BABY_BOSS_HPP_

