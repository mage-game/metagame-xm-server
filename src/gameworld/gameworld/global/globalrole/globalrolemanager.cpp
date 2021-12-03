#include "globalrolemanager.hpp"
#include "globalrole.hpp"

#include "servercommon/serverclmempool.h"
#include "servercommon/servercommon.h"
#include "global/rmibackobjdef.h"
#include "serverlogic.h"
#include "internalcomm.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/struct/global/globalroledataparam.hpp"
#include "servercommon/struct/structcommon.h"
#include <servercommon/serverconfig/crossconfig.hpp>

GlobalRoleManager::GlobalRoleManager() 
	: m_data_state(GLOBAL_ROLE_MANAGER_DATA_STATE_INVALID), m_last_save_timestamp(0), m_is_role_map_change(true), m_last_update_timestamp(0)
{
	memset(&m_change_global_role_param, 0, sizeof(m_change_global_role_param));
}

GlobalRoleManager::~GlobalRoleManager()
{
	for (GlobalRoleMapIt it = m_global_role_map.begin(); it != m_global_role_map.end(); ++ it)
	{
		GlobalRole *g_role = it->second;
		if (NULL != g_role)
		{
			delete g_role;
		}
	}
	
	m_global_role_map.clear();
	m_global_role_rand_pool.clear();
}

GlobalRoleManager & GlobalRoleManager::Instance()
{
	static GlobalRoleManager _instance;
	return _instance;
}

bool GlobalRoleManager::OnServerStart()
{
	if (CrossConfig::Instance().IsHiddenServer()) return true;

	this->LoadData(0);
	return true; 
}

void GlobalRoleManager::OnServerStop()
{
	if (CrossConfig::Instance().IsHiddenServer()) return;

	this->CheckSaveGlobalRole(EngineAdapter::Instance().Time() + 2 * GLOBAL_ROLE_CHECK_SAVE_INTERVAL_S);
}

void GlobalRoleManager::Update(unsigned long interval, time_t now_second)
{
	if (CrossConfig::Instance().IsHiddenServer()) return;
	if (!this->IsLoadFinish()) return;

	if (now_second >= m_last_update_timestamp + 3) // 暂定3秒更新一次，如果以后有功能需要更精确，可以适当调低一点
	{
		m_last_update_timestamp = now_second;

		for (GlobalRoleMapIt it = m_global_role_map.begin(); it != m_global_role_map.end(); ++ it)
		{
			GlobalRole *global_role = it->second;
			if (NULL != global_role)
			{
				global_role->Update(interval, now_second);
			}
		}
	}

	this->CheckSaveGlobalRole(now_second);
}

void GlobalRoleManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	for (GlobalRoleMapIt it = m_global_role_map.begin(); it != m_global_role_map.end(); ++it)
	{
		GlobalRole *g_role = it->second;
		if (NULL != g_role)
		{
			g_role->OnDayChange(old_dayid, now_dayid);
		}
	}
}

void GlobalRoleManager::OnRoleLogin(Role *role)
{
	if (CrossConfig::Instance().IsHiddenServer()) return;

	if (NULL == role) return;
	if (!this->IsLoadFinish()) return;

	GlobalRole *global_role = this->GetGlobalRole(role->GetUID());
	if (NULL == global_role)
	{
		global_role = new GlobalRole(this, role->GetUID());

		GlobalRoleDatalListParam::RoleDataAttr global_role_attr;
		global_role_attr.role_id = role->GetUID();

		global_role->Init(global_role_attr);
		m_global_role_map[role->GetUID()] = global_role;
		m_is_role_map_change = true;

		this->OnGlobalRoleDataChange(global_role, structcommon::CS_INSERT);
	}

	global_role->OnRoleLogin(role);
}

void GlobalRoleManager::OnRoleLogout(Role *role)
{
	if (CrossConfig::Instance().IsHiddenServer()) return;

	if (NULL == role) return;
	if (!this->IsLoadFinish()) return;

	GlobalRole *global_role = this->GetGlobalRole(role->GetUID());
	if (NULL == global_role) return;
	
	global_role->OnRoleLogout(role);
}

bool GlobalRoleManager::OnGlobalRoleDataChange(GlobalRole *global_role, int change_state)
{
	if (CrossConfig::Instance().IsHiddenServer()) return false;
	if (NULL == global_role || !this->CheckSaveGlobalRole(EngineAdapter::Instance().Time())) return false;

	for (int i = 0; i < m_change_global_role_param.count; ++i) // 合并写DB 减少数据库并发
	{
		GlobalRoleDatalListParam::RoleDataAttr &save_global_role_attr = m_change_global_role_param.role_data_list[i];
		if (save_global_role_attr.role_id == global_role->GetOwnerUid() &&
			structcommon::CS_UPDATE == save_global_role_attr.change_state && structcommon::CS_UPDATE == change_state)
		{
			global_role->GetAttr(save_global_role_attr);
			return true;
		}
	}

	if (m_change_global_role_param.count < GLOBAL_ROLE_DATA_ONCE_LOAD_MAX_COUNT)
	{
		GlobalRoleDatalListParam::RoleDataAttr &global_role_attr = m_change_global_role_param.role_data_list[m_change_global_role_param.count];

		global_role_attr.change_state = change_state;
		global_role->GetAttr(global_role_attr);

		++m_change_global_role_param.count;

		return true;
	}

	return false;
}

void RMIInitGlobalRoleObjectImpl::InitGlobalRet(int ret, const GlobalRoleDatalListParam &global_role_list_param)
{
	if (0 != ret)
	{
		printf("RMIInitGlobalRoleObjectImpl::InitGlobalRet Failed %d\n", ret);
		ServerLogic::GetInstServerLogic()->StopServer();
		return;
	}

	long long next_id_from = 0;

	for (int i = 0; i < global_role_list_param.count; ++ i)
	{
		if (global_role_list_param.role_data_list[i].id_global_data > next_id_from)
		{
			next_id_from = global_role_list_param.role_data_list[i].id_global_data;
		}

		if (global_role_list_param.role_data_list[i].role_id <= 0) continue;

		if (!global_role_manager->InitGlobalRole(global_role_list_param.role_data_list[i]))
		{
			printf("RMIInitGlobalRoleObjectImpl::InitGlobalRet InitGlobalRole Failed \n");
			EngineAdapter::Instance().StopGame();
			return;
		}
	}

	if (global_role_list_param.count <= 0)
	{
		global_role_manager->LoadDataSucc();
	}
	else
	{
		global_role_manager->LoadData(next_id_from);
	}
}

bool GlobalRoleManager::LoadData(long long id_from)
{
	if (this->IsLoadFinish()) return false;

	m_data_state = GLOBAL_ROLE_MANAGER_DATA_STATE_LOADING;

	RMIInitGlobalRoleObjectImpl *impl = new RMIInitGlobalRoleObjectImpl();
	impl->global_role_manager = this;

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());  
	return gc.InitGlobalRoleAsyn(id_from, impl);
}

void GlobalRoleManager::LoadDataSucc()
{
	m_data_state = GLOBAL_ROLE_MANAGER_DATA_STATE_LOAD_FINISH;
}

bool GlobalRoleManager::InitGlobalRole(const GlobalRoleDatalListParam::RoleDataAttr &global_role_attr)
{
	if (m_global_role_map.end() != m_global_role_map.find(global_role_attr.role_id))
	{
		printf("GlobalRoleManager::InitGlobalRole Aleady has this GlobalRole");
		return false;
	}

	GlobalRole *global_role = new GlobalRole(this, global_role_attr.role_id);
	global_role->Init(global_role_attr);

	m_global_role_map[global_role_attr.role_id] = global_role;

	return true;
}

bool GlobalRoleManager::CheckSaveGlobalRole(time_t now_second)
{
	if (now_second >= m_last_save_timestamp + GLOBAL_ROLE_CHECK_SAVE_INTERVAL_S || m_change_global_role_param.count >= GLOBAL_ROLE_DATA_ONCE_LOAD_MAX_COUNT)
	{
		m_last_save_timestamp = now_second;

		if (m_change_global_role_param.count > 0)
		{
			RMISaveGlobalRoleObjectImpl *impl = new RMISaveGlobalRoleObjectImpl();

			RMIGlobalClient gc;
			gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB()); 

			if (!gc.SaveGlobalRoleAsyn(m_change_global_role_param, impl)) return false;

			m_change_global_role_param.count = 0;
		}
	}

	return true; 
}

int GlobalRoleManager::RandGetRole(GlobalRole *role_list[], int role_max, CheckGlobalRole check_func, void *param)
{
	if (NULL == role_list)
	{
		return 0;
	}

	if (m_is_role_map_change)
	{
		m_is_role_map_change = false;
		m_global_role_rand_pool.clear();
		m_global_role_rand_pool.reserve(m_global_role_map.size());

		for (GlobalRoleMapIt it = m_global_role_map.begin(); it != m_global_role_map.end(); it++)
		{
			m_global_role_rand_pool.push_back(it->second);
		}
	}

	int pool_size = (int)m_global_role_rand_pool.size();
	int rand_max = pool_size;
	int get_count = 0;

	for (int i = 0; i < pool_size && get_count < role_max && rand_max > 0; i++)
	{
		int rand_num = RandomNum(rand_max);
		GlobalRole *global_role = m_global_role_rand_pool[rand_num];
		if (NULL == check_func || check_func(global_role, param))
		{
			role_list[get_count++] = global_role;
		}

		rand_max--;
		m_global_role_rand_pool[rand_num] = m_global_role_rand_pool[rand_max];
		m_global_role_rand_pool[rand_max] = global_role;
	}

	return get_count;
}

GlobalRole * GlobalRoleManager::GetGlobalRole(int uid)
{
	if (uid <= 0) return NULL;

	GlobalRoleMapIt it = m_global_role_map.find(uid);
	if (it != m_global_role_map.end())
	{
		return it->second;
	}

	return NULL;
}

RoleAppearance * GlobalRoleManager::GetRoleAppearance(int uid)
{
	GlobalRole *g_role = this->GetGlobalRole(uid);
	if (nullptr == g_role)
	{
		return nullptr;
	}

	return &g_role->GetRoleAppearance();
}

CommonDataGlobal * GlobalRoleManager::GetCommonDataGlobal(int uid)
{
	GlobalRole *g_role = this->GetGlobalRole(uid);
	if (NULL == g_role)
	{
		return NULL;
	}

	return g_role->GetCommonDataGlobal();
}

LittlePet * GlobalRoleManager::GetLittlePet(int uid)
{
	GlobalRole *g_role = this->GetGlobalRole(uid);
	if (NULL == g_role)
	{
		return NULL;
	}

	return g_role->GetLittlePet();
}

LoveTree * GlobalRoleManager::GetLoveTree(int uid)
{
	GlobalRole *g_role = this->GetGlobalRole(uid);
	if (NULL == g_role)
	{
		return NULL;
	}

	return g_role->GetLoveTree();
}

LoveContract * GlobalRoleManager::GetLoveContract(int uid)
{
	GlobalRole *g_role = this->GetGlobalRole(uid);
	if (NULL == g_role)
	{
		return NULL;
	}

	return g_role->GetLoveContract();
}

JingLingHome * GlobalRoleManager::GeJingLingHome(int uid)
{
	GlobalRole *g_role = this->GetGlobalRole(uid);
	if (NULL == g_role)
	{
		return NULL;
	}

	return g_role->GetJingLingHome();
}

RoleFighting * GlobalRoleManager::GetRoleFighting(int uid)
{
	GlobalRole *g_role = this->GetGlobalRole(uid);
	if (NULL == g_role)
	{
		return NULL;
	}

	return g_role->GetRoleFighting();
}

QingyuanGlobal * GlobalRoleManager::GetQingyuanGlobal(int uid)
{
	GlobalRole *g_role = this->GetGlobalRole(uid);
	if (NULL == g_role)
	{
		return NULL;
	}

	return g_role->GetQingyuanGlobal();
}

SpouseHome * GlobalRoleManager::GetSpouseHomeGlobal(int uid)
{
	GlobalRole *g_role = this->GetGlobalRole(uid);
	if (nullptr == g_role)
	{
		return nullptr;
	}

	return g_role->GetSpouseHomeGlobal();
}