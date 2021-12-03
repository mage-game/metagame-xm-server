#ifndef __JINGJIE_HPP__
#define __JINGJIE_HPP__

#include "servercommon/servercommon.h"
#include "obj/character/attribute.hpp"
#include "obj/character/character.h"


class Role;

class JingJie
{
public: 
	JingJie();
	~JingJie();

	void SetRole(Role *role) { m_role = role; }

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int fight_type, int target_index);
	const int GetIncreaHurtPer(Role *target_role);

	void GetRoleAppearance(RoleAppearance* data);
	void OnJingJieOpera(int opera_type, int param);
	void PromoteJingJie(int is_auto_buy);
	void SendJingJieInfo();
	void OnLogin();

private:
	Role *m_role;
	CharIntAttrs m_attr_temp;
	unsigned int m_last_yazhi_skill_perform_timestamp;				// 压制技能博尚时间
};

#endif
