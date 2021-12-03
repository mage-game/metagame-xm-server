#ifndef __HUASHEN_HPP__
#define __HUASHEN_HPP__

#include "servercommon/huashendef.hpp"
#include "obj/character/attribute.hpp"

class Role;

class HuaShen
{
public:
	HuaShen();
	~HuaShen();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const HuashenParam &param);
	void GetInitParam(HuashenParam *param);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);		
	void SendHuaShenAllInfo();
	void SendHuashenSpiritAllInfo();
	void SendHuaShenSpiritInfo(int huashen_id);

	void ChangeImage(int id);
	bool UpLevel(int id, bool is_auto_buy);
	bool UpGrade(int id, bool is_auto_buy);
	bool OnUseHuaShenCard(int id);
	bool OnUpgradeSpirit(int huashen_id, int spirit_id, bool is_auto_upgrade);

	int GetCurHuaShenID() { return m_param.cur_huashen_id; }
	int GetLevel(int id);
	int GetSpiritLevel(int huashen_id, int spirit_id);
	
protected:
	bool IsHuaShenIDActive(int id);

private:
	Role *m_role;
	HuashenParam m_param;
	CharIntAttrs m_attrs_add;
};

#endif
