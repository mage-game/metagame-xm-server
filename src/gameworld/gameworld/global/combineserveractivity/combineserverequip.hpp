#ifndef __COMBINE_SERVER_EQUIP_HPP__
#define __COMBINE_SERVER_EQUIP_HPP__

#include "servercommon/combineserverequipdef.hpp"
#include "obj/character/attribute.hpp"

class Role;
class CombineServerActivityEquip
{
public:
	CombineServerActivityEquip();
	~CombineServerActivityEquip();

	void SetRole(Role *role);

	void Init(Role *role, const CSAEquipParam &csa_equip_param);
	void GetInitParam(CSAEquipParam *csa_equip_param);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnUpCommonlevel(int part_index);
	void OnUpGreatlevel(int part_index);
	void SyncCSAEquipInfo();
	bool CheckActiveSpecial();
	void SyncCSAEquipActiveSpecial();
	short GetActiveSpecial() { return m_param.active_special; }

private:
	Role *m_role;

	CSAEquipParam m_param;
	CharIntAttrs m_attrs_add;
};

#endif // __COMBINE_SERVER_EQUIP_HPP__

