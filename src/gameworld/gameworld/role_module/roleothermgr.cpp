#include "roleothermgr.h"

#include "servercommon/struct/roleotherinitparam.hpp"
#include "servercommon/struct/global/autoparam.hpp"
#include "role_module/i_role_module.hpp"
#include "serverlogic.h"
#include "gamelog.h"
#include "obj/character/role.h"

#include "other/tianshenhuti/tianshenhuti.hpp"
//-------------------------------------------------------------------------------------------

TianshenHuti *RoleOtherMgr::GetTianshenHuti() { return (TianshenHuti*)this->GetRoleModule(ROLE_MODULE_TYPE_TIANSHENHUTI); }

//-------------------------------------------------------------------------------------------

RoleOtherMgr::RoleOtherMgr() :
	m_role(nullptr)
{
	// Ä£¿é³õÊ¼»¯
	memset(m_all_modules, 0, sizeof(m_all_modules));
	m_all_modules[ROLE_MODULE_TYPE_TIANSHENHUTI] = new TianshenHuti();

}

RoleOtherMgr::~RoleOtherMgr()
{
	for (IRoleModule *module : m_all_modules)
	{
		delete module;
	}
}

void RoleOtherMgr::SetRole(Role *role)
{
	m_role = role;

	for (IRoleModule *module : m_all_modules)
	{
		module->SetRole(role);
	}
}

void RoleOtherMgr::InitParam(const RoleOtherInitParam &init_param)
{
	this->InitParamPage10(init_param.data_page_10);
}

void RoleOtherMgr::GetInitParam(RoleOtherInitParam *init_param)
{
	this->GetInitParamPage10(init_param->data_page_10);
}

void RoleOtherMgr::InitParamPage10(DataPageParam &data_page)
{
	TLVUnserializer unserializer;
	unserializer.Reset(data_page.data, data_page.len);

	for (int module_type = ROLE_MODULE_TYPE_MIN; module_type < ROLE_MODULE_TYPE_COUNT; ++ module_type)
	{
		auto module_tmp = m_all_modules[module_type];
		if (!module_tmp->GetParam().Unserialize(unserializer))
		{
			gamelog::g_log_debug.printf(LL_ERROR, "module:%d Unserialize fail! in %s", module_type, __FUNCTION__);
			ServerLogic::GetInstServerLogic()->Stop();
		}

		module_tmp->OnInit();
	}
}

void RoleOtherMgr::InitParamPage10ByCahce(DataPageParam &data_page)
{
	TLVUnserializer unserializer;
	unserializer.Reset(data_page.data, data_page.len);

	for (int module_type = ROLE_MODULE_TYPE_MIN; module_type < ROLE_MODULE_TYPE_COUNT; ++module_type)
	{
		auto module_tmp = m_all_modules[module_type];
		if (!module_tmp->GetParam().Unserialize(unserializer))
		{
			gamelog::g_log_debug.printf(LL_ERROR, "module:%d Unserialize fail! in %s", module_type, "RoleOtherMgr::InitParamPage10ByCahce");
			ServerLogic::GetInstServerLogic()->Stop();
		}
	}
}

void RoleOtherMgr::GetInitParamPage10(DataPageParam &data_page)
{
	TLVSerializer serializer;
	serializer.Reset(data_page.data, sizeof(data_page.data));

	for (int module_type = ROLE_MODULE_TYPE_MIN; module_type < ROLE_MODULE_TYPE_COUNT; ++module_type)
	{
		auto module_tmp = m_all_modules[module_type];
		if (!module_tmp)
		{
			continue;
		}

		module_tmp->OnSave();

		if (!module_tmp->GetParam().Serialize(serializer))
		{
			gamelog::g_log_debug.printf(LL_ERROR, "module:%d Serialize fail! in %s", module_type, "RoleOtherMgr::GetInitParamPage10");
			ServerLogic::GetInstServerLogic()->Stop();
		}
	}

	data_page.len = serializer.Size();
}

void RoleOtherMgr::ReCalcAttr(CharIntAttrs &base_add, int recalc_reason, bool recalc_all)
{
	for (IRoleModule *module : m_all_modules)
	{
		module->ReCalcAttr(base_add, recalc_reason, recalc_all);
	}
}

void RoleOtherMgr::Update(time_t now_second, unsigned long interval)
{
	for (IRoleModule *module : m_all_modules)
	{
		module->Update(now_second, interval);
	}
}

void RoleOtherMgr::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	ClockReturnDayItem *old_day = (ClockReturnDayItem*)&old_dayid;
	ClockReturnDayItem *now_day = (ClockReturnDayItem*)&now_dayid;
	
	for (IRoleModule *module : m_all_modules)
	{
		module->OnDayChange(*old_day, *now_day);
	}
}

void RoleOtherMgr::OnWeekChange()
{
	for (IRoleModule *module : m_all_modules)
	{
		module->OnWeekChange();
	}
}

void RoleOtherMgr::OnMonthChange()
{
	for (IRoleModule *module : m_all_modules)
	{
		module->OnMonthChange();
	}
}

void RoleOtherMgr::OnAttackCalcHurt(ObjID attacker, Attribute &delta_hp)
{

}

void RoleOtherMgr::OnHurtOther(Character *target, int skill_id, long long &injure, int target_index, int &passive_flag)
{
	for (IRoleModule *module : m_all_modules)
	{
		module->OnHurtOther(target, skill_id, injure, target_index, passive_flag);
	}
}

void RoleOtherMgr::OnKillOther(Character *dead)
{

}

void RoleOtherMgr::OnDie(ObjID killer_objid)
{

}

void RoleOtherMgr::OnEnterScene()
{

}

void RoleOtherMgr::OnLeaveScene()
{

}

void RoleOtherMgr::OnRoleLogin()
{
	for (IRoleModule *module : m_all_modules)
	{
		module->OnRoleLogin();
	}
}

void RoleOtherMgr::OnRoleLogout()
{
	for (IRoleModule *module : m_all_modules)
	{
		module->OnRoleLogout();
	}
}

void RoleOtherMgr::OnAllInfoReq()
{
	for (IRoleModule *module : m_all_modules)
	{
		module->OnAllInfoReq();
	}
}

IRoleModule * RoleOtherMgr::GetRoleModule(int type) const
{
	if (type < ROLE_MODULE_TYPE_MIN || type >= ROLE_MODULE_TYPE_COUNT) return nullptr;

	return m_all_modules[type];
}
