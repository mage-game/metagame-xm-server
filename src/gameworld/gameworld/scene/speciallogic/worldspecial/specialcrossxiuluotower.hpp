#ifndef __SPECIAL_CROSS_XIULUO_TOWER_HPP__
#define __SPECIAL_CROSS_XIULUO_TOWER_HPP__

#include <map>
#include <stdio.h>

#include "iclockmodule.h"

#include "scene/speciallogic/speciallogic.hpp"

#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/serverconfig/crossxiuluotowercommonconfig.hpp"

class SpecialCrossXiuluoTower : public SpecialLogic
{
	struct UserInfo
	{
		UserInfo() : gather_buff_end_timestamp(0)
		{
			
		}

		unsigned int gather_buff_end_timestamp;
	};

public:
	SpecialCrossXiuluoTower(Scene *scene);
	virtual ~SpecialCrossXiuluoTower();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual bool CanDestroy();

	virtual void OnAddObj(Obj *obj);
	virtual void OnRemoveObj(Obj *obj);

	virtual bool SpecialCanReAlive(Role *role, int realive_type);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialStopGahterOnAttack(Role *role);
	virtual bool SpecialCanMountOn(Role *role){return false;}

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnLeaveFB(Role *role);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);

	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual int GetSpecialParam(Role *role);

	void OnActivityStatusChange(int status);

	void RefreshXiuluoTowerRankInfo(Role *role = NULL);
	virtual bool SpecialIsCostMp(Role *role) { return false; }

	void OnScoreRewardReq(Role *role, int index);
	void OnBuyGatherBuff(Role *role);
	void SendAllRoleInfo();

	void GMAddScore(Role *role, int add_score);
	void GMFlushBoss(Role *role);
	void SendRankTitleList(Role * role = nullptr);

protected:
	void AddScore(Role *role, int add_score);
	void SendXiuluoTowerSelfInfo(Role *role);

	void RecalcBuff(Role *role);
	void FlushMonster();
	void MonsterNotice();

	void RemoveAllGather();
	void RefreshGather();
	void SendGatherInfo(Role *role = NULL);

	void OnActivityOpen();
	void OnActivityClose();

	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	void OnSyncDropLog(Role *role, int log_type, unsigned short item_id, short num);
	void OnSyncRewardLog(Role *role);

	void CheckNotifyGatherBuffInfo();
	void NotifyGatherBuffInfo(Role *role, short is_has_buff);
	void SendBossInfo(Role * role =nullptr);
	void CheckSendBossInfo();

private:
	static const int DUR_DEAD_SKILL_ID = 2;

	bool m_is_init;
	bool m_need_send_gather_info;
	bool m_is_notice;

	unsigned int m_send_rank_title;
	unsigned int m_next_notice_gather_buff_info_time;
	unsigned int m_next_check_boss_info_time;

	CrossXiuLuoTowerBossInfo m_boss_info;
};

#endif // __SPECIAL_CROSS_XIULUO_TOWER_HPP__

