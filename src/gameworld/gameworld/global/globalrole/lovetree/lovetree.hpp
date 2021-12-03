#ifndef __LOVETREE_HPP__
#define __LOVETREE_HPP__

#include "lovetreeconfig.hpp"
#include "obj/character/attribute.hpp"
#include "global/globalrole/globalrole.hpp"
#include "protocal/msglovetree.h"

class Role;

class LoveTree : public IGlobalRoleItem
{
public:
	LoveTree(GlobalRole *g_role);
	~LoveTree();

	virtual void Init(const LoveTreeData &love_tree_attr);
	virtual void GetAttr(LoveTreeData &love_tree_attr);

	void Update(unsigned int now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc, Role *role);
	void OnRoleLogin(Role *role);

	void ReName(GameName new_name);

	int GetLoverId();
	void GetTotalAttr(CharIntAttrs &tmp_attr, bool is_self = true);

	void SetLoverId(int lover_id, bool is_calc = true);

	bool AddLoveTreeExp(Role *role, bool is_auto_buy = false, bool is_water_other = false);
	void SendLoveTreeInfo(Role *role, bool is_self);

private:

	CharIntAttrs m_attrs_add;
	LoveTreeData m_love_tree_param;
};


#endif // __LOVETREE_HPP__
