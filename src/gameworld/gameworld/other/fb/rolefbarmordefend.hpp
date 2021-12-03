#ifndef _ROLEFBARMORDEFEND_HPP_
#define _ROLEFBARMORDEFEND_HPP_

#include "other/fb/armordefendconfig.hpp"
#include "servercommon/fbdef.hpp"
#include "monster/monsterinitparam.h"
#include "servercommon/struct/itemlistparam.h"
#include "protocal/msgfb.h"
class Role;

class RoleFBArmorDefend
{
public:
	RoleFBArmorDefend();
	~RoleFBArmorDefend();

	void SetRole(Role *role) { m_role = role; }

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void Init(Role *role, const ArmorDefendFBParam &param);
	void GetInitParam(ArmorDefendFBParam *param);

	void SendInfo();

	bool IsNewPlayer();
	bool PersonalFBCanEnter(int level);
	void PersonalFBAutoFB(int level);
	void OnPersonalFBEnter();
	void OnPersonalFBFinish(bool is_pass, int scene_id, int clear_wave_count);
	void PersonalBuyJoinTimes();
	bool PersonalItemAddJoinTimes(int times);
	void OnLogin();
	int GetPersonalJoinTimes() { return m_am_param.personal_join_times; }
	const ArmorDefendFBParam* GetTowerDefFBParam(int level) const;

	int GetArmoedefendPassLayer() { return m_am_param.personal_max_pass_level; }

	void AutoFbDrophelper(const ArmorDefendWaveCfg &wave_cfg, ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT], 
		int *index_count, long long *reward_exp, int *reward_coin, int *reward_yuanli);
	
private:
	Role *m_role;
	ArmorDefendFBParam m_am_param;
};

#endif
