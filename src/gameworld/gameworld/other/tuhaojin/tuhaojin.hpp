#ifndef __TUHAOJIN_H__
#define __TUHAOJIN_H__

class Role;

#include "obj/character/attribute.hpp"
#include "servercommon/tuhaojindef.hpp"


class TuHaoJin
{
public:
	TuHaoJin();
	~TuHaoJin();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const TuHaoJinParam &param);
	void GetInitParam(TuHaoJinParam *param);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void SendJinguaInfo();
	char GetTuhaojinColor();

	void OnUseTuhaoJin(char tuhaojin_color_id);
	bool OnTuhaojinUpLevel();
	

protected:
	

private:
	Role *m_role;
	TuHaoJinParam m_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值
};

#endif

