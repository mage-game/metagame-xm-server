#ifndef __ZHIBAO_MANAGER_HPP__
#define __ZHIBAO_MANAGER_HPP__

#include "servercommon/struct/zhibaoparam.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;

const static int ZHIBAO_SPECIAL_IMG_START_ID = 1000;							// 特殊形象开始ID
class ZhiBaoManager
{
public:
	ZhiBaoManager();
	~ZhiBaoManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const ZhiBaoParam &zhibao_param);
	void GetInitParam(ZhiBaoParam *zhibao_param);
	void GetRoleAppearance(RoleAppearance *appearance);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	int GetMountAddPer();
	int GetWingAddPer();
	int GetZhiBaoLevel() { return m_zhibao_param.level; }

	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag);

	void SendAllZhiBaoInfo();

	void AddZhiBaoExp(int add_exp);
	bool UplevelZhiBao();
    void UseImage(int image);

	void ActiveOrUpgradeHuanhua(int type);
	void UseHuanhuaType(int type);
	bool ActiveHuanHua(int huanhua_id);

private:
	void RefreshSkillLevel();

	Role *m_role;

	ZhiBaoParam m_zhibao_param;
	CharIntAttrs m_attrs_add;												// 各属性加成数值

	int m_skill_level_list[MAX_ZHIBAO_SKILL_COUNT];
	int m_tianlei_hurt;
};

#endif // __ZHIBAO_MANAGER_HPP__
