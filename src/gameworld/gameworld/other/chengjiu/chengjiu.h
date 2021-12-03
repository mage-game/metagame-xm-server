#ifndef __CHENGJIU_H__
#define __CHENGJIU_H__

#include "obj/character/attribute.hpp"
#include "equipment/equipment.h"
#include "servercommon/chengjiudef.hpp"

class Role;

class ChengJiu
{
public:
	ChengJiu();
	~ChengJiu();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const ChengJiuParam &chengjiu_param);
	void GetInitParam(ChengJiuParam *chengjiu_param);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnLogin();

	int GetChengJiuTitleLevel(short *title_level = NULL);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void UpTitleLevel();
	void FetchReward(short reward_id);
	void UpFuwenLevel();

	void SyncChengJiuInfo();
	void BroadcastChengjiuTitleLevel();

	void RecalcChengJiuConditionOnLogin();
	void OnChengJiuCondition(short condition, unsigned int param1 = 0, bool is_notify = true);

	bool OnUseShuxingdan(int slot_idx, int use_count);

protected:
	Role *m_role;
	CharIntAttrs	m_attrs_add;							// 各属性加成数值

	ChengJiuParam m_chengjiu_param;
};

#endif

