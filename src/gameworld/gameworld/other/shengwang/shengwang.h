#ifndef __SHENGWANG_H__
#define __SHENGWANG_H__

#include "obj/character/attribute.hpp"
#include "equipment/equipment.h"
#include "servercommon/shengwangdef.hpp"

class Role;

class ShengWang
{
public:
	ShengWang();
	~ShengWang();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const ShengWangParam &shengwang_param);
	void GetInitParam(ShengWangParam *shengwang_param);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void GetXianJieLevel(short *xianjie_level);

	void UpXianjieLevel();
	void UpXiandanLevel();

	void SyncShengWangInfo();
	void BroadcastXianJieLevel();

	bool OnUseShuxingdan(int slot_idx, short &use_count);

protected:
	Role *m_role;
	CharIntAttrs	m_attrs_add;							// 各属性加成数值

	ShengWangParam m_shengwang_param;
};

#endif

