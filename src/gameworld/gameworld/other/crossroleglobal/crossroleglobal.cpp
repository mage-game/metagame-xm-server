#include "crossroleglobal.hpp"
#include "other/capability/capability.hpp"
#include "obj/character/role.h"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/globalrole.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/bitmapfunction.hpp"
#include "global/guild/roleguild.hpp"
#include "config/logicconfigmanager.hpp"

CrossRoleGlobal::CrossRoleGlobal() : m_role(NULL)
{
}

CrossRoleGlobal::~CrossRoleGlobal()
{
}

void CrossRoleGlobal::InitParam(Role *role, const RoleCrossGlobalData &cross_role_global_data)
{
	m_role = role;
	m_cross_role_global_data = cross_role_global_data;
}

void CrossRoleGlobal::GetInitParam(RoleCrossGlobalData *cross_role_global_data)
{
	// 隐藏服不改变全局属性
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		GlobalRole *g_role = GLOBAL_ROLE_MGR.GetGlobalRole(m_role->GetUID());
		if (NULL == g_role)
		{
			return;
		}
		m_cross_role_global_data.Reset();

		g_role->GetCrossData(m_cross_role_global_data, m_role);
		m_role->GetRoleGuild()->GetRoleCrossData(m_cross_role_global_data);
	}

	*cross_role_global_data = m_cross_role_global_data;
}

void CrossRoleGlobal::ReCalcAttr(CharIntAttrs &base_add)
{
	for (int i = 0; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX - CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i++)
	{
		//base_add.m_attrs[i + CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN] += m_cross_role_global_data.attrs[i];
		base_add.AddValue(i + CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN, m_cross_role_global_data.attrs[i]);
	}
}

int CrossRoleGlobal::GetUsingPetId()
{
	return m_cross_role_global_data.little_pet_cur_id;
}

void CrossRoleGlobal::SetSaveInGlobalModule(int type, int param_0, int param_1)
{
	// 跨服不改变
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (type >= ADJUST_CAP_TYPE_TITLE && type < ADJUST_CAP_TYPE_COUNT)
	{
		const int index = LOGIC_CONFIG->GetDynamicCapAdjustCfg().GetSubIndex({ type, param_0, param_1 });
		if (index >= 0)
		{
			unsigned char *adjust_flag = m_cross_role_global_data.adjust_cap_flag_list[type];
			BitmapFunction::SetBit(adjust_flag, ADJUST_CAP_MAX_BYTE, index);
		}
	}
}

void CrossRoleGlobal::ClearSaveInGlobalModule(int type, int param_0, int param_1)
{
	// 跨服不清除
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (type >= ADJUST_CAP_TYPE_TITLE && type < ADJUST_CAP_TYPE_COUNT)
	{
		const int index = LOGIC_CONFIG->GetDynamicCapAdjustCfg().GetSubIndex({ type, param_0, param_1 });
		if (index >= 0)
		{
			unsigned char *adjust_flag = m_cross_role_global_data.adjust_cap_flag_list[type];
			BitmapFunction::ClearBit(adjust_flag, ADJUST_CAP_MAX_BYTE, index);
		}
	}
}

bool CrossRoleGlobal::CheckHaveSaveInGlobalModule(int type, int param_0, int param_1)
{
	bool is_have = false;
	if (type >= ADJUST_CAP_TYPE_TITLE && type < ADJUST_CAP_TYPE_COUNT)
	{
		const int index = LOGIC_CONFIG->GetDynamicCapAdjustCfg().GetSubIndex({ type, param_0, param_1 });
		if (index >= 0)
		{
			unsigned char *adjust_flag = m_cross_role_global_data.adjust_cap_flag_list[type];
			is_have = BitmapFunction::IsBitSet(adjust_flag, ADJUST_CAP_MAX_COUNT, index);
		}
	}

	return is_have;
}

void CrossRoleGlobal::ClearSaveInGlobalModule(int type)
{
	if (type >= ADJUST_CAP_TYPE_TITLE && type < ADJUST_CAP_TYPE_COUNT)
	{
		for (int i = 0; i < ADJUST_CAP_MAX_BYTE; ++i)
		{
			m_cross_role_global_data.adjust_cap_flag_list[type][i] = 0;
		}
	}
}

const std::vector<CapAdjustKeyObj> & CrossRoleGlobal::GetKeyObj(int type)
{
	m_role_have_cap_vec_cache.clear();

	const std::map<CapAdjustKeyObj, int> &cfg_map = LOGIC_CONFIG->GetDynamicCapAdjustCfg().GetAdjustMap();
	for (auto iter = cfg_map.begin(); iter != cfg_map.end(); ++iter)
	{
		if (iter->first.cap_type == type)
		{
			const int index = iter->second;
			unsigned char *adjust_flag = m_cross_role_global_data.adjust_cap_flag_list[type];

			if (BitmapFunction::IsBitSet(adjust_flag, ADJUST_CAP_MAX_BYTE, index))
			{
				m_role_have_cap_vec_cache.push_back(iter->first);
			}
		}
	}

	return m_role_have_cap_vec_cache;
}