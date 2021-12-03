#ifndef __MOJIE_MANAGER_HPP__
#define __MOJIE_MANAGER_HPP__

#include "servercommon/struct/mojieparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;

class MojieManager
{
public:
	MojieManager();
	~MojieManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const MojieParam &zhibao_param);
	void GetInitParam(MojieParam *zhibao_param);

	void Update(time_t now_second);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void SendMojieInfo();

	bool OnUpMojielevel(Role *role, int mojie_type, bool is_auto_buy);

	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag);
	bool IsMianyiDeadlyAttack();									   // 是否免疫致命的一击

private:

	Role *m_role;

	MojieParam m_mojie_param;
	CharIntAttrs m_attrs_add;												// 各属性加成数值
};

#endif // __MOJIE_MANAGER_HPP__
