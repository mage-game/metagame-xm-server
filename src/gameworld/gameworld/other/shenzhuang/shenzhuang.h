#ifndef __SHENZHUANG_H__
#define __SHENZHUANG_H__

#include "obj/character/attribute.hpp"
#include "equipment/equipment.h"
#include "servercommon/shenzhuangdef.hpp"
#include "protocal/msgrole.h"

class Role;

class ShenZhuang
{
public:
	ShenZhuang();
	~ShenZhuang();

	void SetRole(Role *role);

	void Init(Role *role, const ShenZhuangParam &szparam);
	void GetInitParam(ShenZhuangParam *szparam);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnUplevel(int part_index);
	void OnDie(Role *killer);

	int GetRedRatio(int part_index, int red_star);
	int GetPinkRatio(int part_index);
	int GetDayGainTimes() { return m_szparam.day_gain_times; }
	void AddDayGainTiems(int times);

	void SyncShenZhuangInfo();

protected:
	Role *m_role;

	CharIntAttrs	m_attrs_add;
	ShenZhuangParam m_szparam;
};

#endif

