#ifndef __CROSS_ROLE_GLOBAL__
#define __CROSS_ROLE_GLOBAL__

#include "servercommon/crossroleglobaldef.hpp"
#include "obj/character/attribute.hpp"
#include "config/dynamiccapadjustconfig.hpp"

#include <vector>

class Role;

class CrossRoleGlobal
{
public:
	CrossRoleGlobal();
	~CrossRoleGlobal();

	void InitParam(Role *role, const RoleCrossGlobalData &cross_role_global_data);
	void GetInitParam(RoleCrossGlobalData *cross_role_global_data);

	void ReCalcAttr(CharIntAttrs &base_add);

	int GetUsingPetId();

	void SetSaveInGlobalModule(int type, int param_0, int param_1 = 0);
	void ClearSaveInGlobalModule(int type, int param_0, int param_1 = 0);
	void ClearSaveInGlobalModule(int type);
	bool CheckHaveSaveInGlobalModule(int type, int param_0, int param_1 = 0);

	const std::vector<CapAdjustKeyObj> & GetKeyObj(int type);
private:
	Role *m_role;

	RoleCrossGlobalData m_cross_role_global_data;
	std::vector<CapAdjustKeyObj> m_role_have_cap_vec_cache;
};

#endif
