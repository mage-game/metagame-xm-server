#include "mergeloginmanager.h"

#include "servercommon/serverconfig/localconfig.hpp"

#include "loginserver.h"
#include "engineadapter.h"
#include "internalcomm.h"
#include "rmibackobjdef.h"
#include "loginlog.h"
#include "loginusertrace.hpp"

class MergeLoginTimer : public ITimerCallback
{
public:
	MergeLoginTimer(MergeLoginManager *merge_login_manager) : m_merge_login_manager(merge_login_manager) {}

	virtual void OnTimer() { m_merge_login_manager->TimerClear(); }
	virtual void Free() { delete this; }

	MergeLoginManager *m_merge_login_manager;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
MergeLoginManager::MergeLoginManager() : m_auto_increase_key(0)
{
	this->TimerClear();
}

MergeLoginManager::~MergeLoginManager()
{
	for (MergeLoginItemMap::iterator it = m_merge_login_item_map.begin(); it != m_merge_login_item_map.end(); ++it)
	{
		delete it->second;
	}

	m_merge_login_item_map.clear();
}

MergeLoginManager & MergeLoginManager::Instance()
{
	static MergeLoginManager mlm;
	return mlm;
}

void MergeLoginManager::OnLoginReq(NetID netid, PlatName original_panme, int db_index)
{
	if (NULL == original_panme) return;

	original_panme[sizeof(PlatName) - 1] = 0;

	int key = this->GetAutoIncreaseKey();
	if (0 == key)
	{
		this->PlatLoginReq(netid, original_panme, db_index, 0, 0);  // 查询的时候（can_insert = 0）
	}
	else
	{
		MergeLoginItem *merge_login_item = new MergeLoginItem();
		merge_login_item->invalid_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 30;
		merge_login_item->merge_role.count = 0;

		std::set<int> &db_index_set = this->GetAllowServerIdSet(original_panme);

		for (std::set<int>::iterator it_set = db_index_set.begin(); it_set != db_index_set.end(); ++it_set)
		{
			if (*it_set >= 0)
			{
				this->PlatLoginReq(netid, original_panme, *it_set, key, 0);
				++merge_login_item->account_count;
			}
		}

		m_merge_login_item_map[key] = merge_login_item;
	}
}

void MergeLoginManager::AddRoleInfoList(NetID netid, int merge_key, int result, const RoleInfoList *role_info, PlatName original_panme, int db_index)
{
	if (0 == merge_key)						// 不合服的情况下 原协议返回
	{
		Protocol::SCRoleList rl;
		rl.reserved = 0;
		rl.result = result;
		rl.count = 0;

		if (NULL != role_info)
		{
			for (; rl.count < role_info->count && rl.count < MAX_ACCOUNT_ROLE_NUM && rl.count < RoleInfoList::MAX_ROLE_NUM; ++rl.count)
			{
				rl.role_item[rl.count].role_id = role_info->role_info_list[rl.count].role_id;
				F_STRNCPY(rl.role_item[rl.count].role_name, role_info->role_info_list[rl.count].role_name, sizeof(rl.role_item[0].role_name));
				rl.role_item[rl.count].avatar = role_info->role_info_list[rl.count].avatar;
				rl.role_item[rl.count].sex = role_info->role_info_list[rl.count].sex;
				rl.role_item[rl.count].country = role_info->role_info_list[rl.count].country;
				rl.role_item[rl.count].level = role_info->role_info_list[rl.count].level;
				rl.role_item[rl.count].professional = role_info->role_info_list[rl.count].professional;
				rl.role_item[rl.count].create_time = role_info->role_info_list[rl.count].create_time;
				rl.role_item[rl.count].last_login_time = role_info->role_info_list[rl.count].last_login_time;
				rl.role_item[rl.count].wuqi_id = role_info->role_info_list[rl.count].wuqi_id;
				rl.role_item[rl.count].shizhuang_wuqi = role_info->role_info_list[rl.count].shizhuang_wuqi;
				rl.role_item[rl.count].shizhuang_wuqi_is_special = role_info->role_info_list[rl.count].shizhuang_wuqi_is_special;
				rl.role_item[rl.count].shizhuang_body = role_info->role_info_list[rl.count].shizhuang_body;
				rl.role_item[rl.count].shizhuang_body_is_special = role_info->role_info_list[rl.count].shizhuang_body_is_special;
				rl.role_item[rl.count].wing_used_imageid = role_info->role_info_list[rl.count].wing_used_imageid;
				rl.role_item[rl.count].halo_used_imageid = role_info->role_info_list[rl.count].halo_used_imageid;
				rl.role_item[rl.count].yaoshi_used_imageid = role_info->role_info_list[rl.count].yaoshi_used_imageid;
				rl.role_item[rl.count].toushi_used_imageid = role_info->role_info_list[rl.count].toushi_used_imageid;
				rl.role_item[rl.count].qilinbi_used_imageid = role_info->role_info_list[rl.count].qilinbi_used_imageid;
				rl.role_item[rl.count].mask_used_imageid = role_info->role_info_list[rl.count].mask_used_imageid;
				rl.role_item[rl.count].upgrade_used_imageid_0 = role_info->role_info_list[rl.count].upgrade_used_imageid_0;
				rl.role_item[rl.count].upgrade_used_imageid_1 = role_info->role_info_list[rl.count].upgrade_used_imageid_1;
				rl.role_item[rl.count].upgrade_used_imageid_2 = role_info->role_info_list[rl.count].upgrade_used_imageid_2;
				rl.role_item[rl.count].upgrade_used_imageid_3 = role_info->role_info_list[rl.count].upgrade_used_imageid_3;
				rl.role_item[rl.count].upgrade_used_imageid_4 = role_info->role_info_list[rl.count].upgrade_used_imageid_4;
				rl.role_item[rl.count].upgrade_used_imageid_5 = role_info->role_info_list[rl.count].upgrade_used_imageid_5;
				rl.role_item[rl.count].upgrade_used_imageid_6 = role_info->role_info_list[rl.count].upgrade_used_imageid_6;
				rl.role_item[rl.count].upgrade_used_imageid_7 = role_info->role_info_list[rl.count].upgrade_used_imageid_7;
				rl.role_item[rl.count].upgrade_used_imageid_8 = role_info->role_info_list[rl.count].upgrade_used_imageid_8;
			}
		}

		int sendlen = sizeof(Protocol::SCRoleList) - (MAX_ACCOUNT_ROLE_NUM - rl.count) * sizeof(Protocol::RoleItem);
		EngineAdapter::Instance().NetSend(netid, (const char*)&rl, sendlen);
	}
	else									// 合服后 
	{
		MergeLoginItemMap::iterator it = m_merge_login_item_map.find(merge_key);
		if (it != m_merge_login_item_map.end())
		{
			MergeLoginItem *merge_login_item = it->second;
			--merge_login_item->account_count;

			Protocol::SCMergeRoleList &merge_role = merge_login_item->merge_role;

			if (NULL != role_info)
			{
				for (int i = 0; i < role_info->count && i < RoleInfoList::MAX_ROLE_NUM; ++i)
				{
					if (merge_role.count >= 0 && merge_role.count < MAX_MERGE_ACCOUNT_ROLE_NUM)
					{
						merge_role.role_item_list[merge_role.count].role_id = role_info->role_info_list[i].role_id;
						F_STRNCPY(merge_role.role_item_list[merge_role.count].role_name, role_info->role_info_list[i].role_name, sizeof(merge_role.role_item_list[0].role_name));
						merge_role.role_item_list[merge_role.count].avatar = role_info->role_info_list[i].avatar;
						merge_role.role_item_list[merge_role.count].sex = role_info->role_info_list[i].sex;
						merge_role.role_item_list[merge_role.count].country = role_info->role_info_list[i].country;
						merge_role.role_item_list[merge_role.count].level = role_info->role_info_list[i].level;
						merge_role.role_item_list[merge_role.count].professional = role_info->role_info_list[i].professional;
						merge_role.role_item_list[merge_role.count].result = result;
						merge_role.role_item_list[merge_role.count].create_time = role_info->role_info_list[i].create_time;
						merge_role.role_item_list[merge_role.count].last_login_time = role_info->role_info_list[i].last_login_time;
						merge_role.role_item_list[merge_role.count].wuqi_id = role_info->role_info_list[i].wuqi_id;
						merge_role.role_item_list[merge_role.count].shizhuang_wuqi = role_info->role_info_list[i].shizhuang_wuqi;
						merge_role.role_item_list[merge_role.count].shizhuang_wuqi_is_special = role_info->role_info_list[i].shizhuang_wuqi_is_special;
						merge_role.role_item_list[merge_role.count].shizhuang_body = role_info->role_info_list[i].shizhuang_body;
						merge_role.role_item_list[merge_role.count].shizhuang_body_is_special = role_info->role_info_list[i].shizhuang_body_is_special;
						merge_role.role_item_list[merge_role.count].wing_used_imageid = role_info->role_info_list[i].wing_used_imageid;
						merge_role.role_item_list[merge_role.count].halo_used_imageid = role_info->role_info_list[i].halo_used_imageid;
						merge_role.role_item_list[merge_role.count].yaoshi_used_imageid = role_info->role_info_list[i].yaoshi_used_imageid;
						merge_role.role_item_list[merge_role.count].toushi_used_imageid = role_info->role_info_list[i].toushi_used_imageid;
						merge_role.role_item_list[merge_role.count].qilinbi_used_imageid = role_info->role_info_list[i].qilinbi_used_imageid;
						merge_role.role_item_list[merge_role.count].mask_used_imageid = role_info->role_info_list[i].mask_used_imageid;
						merge_role.role_item_list[merge_role.count].upgrade_used_imageid_0 = role_info->role_info_list[i].upgrade_used_imageid_0;
						merge_role.role_item_list[merge_role.count].upgrade_used_imageid_1 = role_info->role_info_list[i].upgrade_used_imageid_1;
						merge_role.role_item_list[merge_role.count].upgrade_used_imageid_2 = role_info->role_info_list[i].upgrade_used_imageid_2;
						merge_role.role_item_list[merge_role.count].upgrade_used_imageid_3 = role_info->role_info_list[i].upgrade_used_imageid_3;
						merge_role.role_item_list[merge_role.count].upgrade_used_imageid_4 = role_info->role_info_list[i].upgrade_used_imageid_4;
						merge_role.role_item_list[merge_role.count].upgrade_used_imageid_5 = role_info->role_info_list[i].upgrade_used_imageid_5;
						merge_role.role_item_list[merge_role.count].upgrade_used_imageid_6 = role_info->role_info_list[i].upgrade_used_imageid_6;
						merge_role.role_item_list[merge_role.count].upgrade_used_imageid_7 = role_info->role_info_list[i].upgrade_used_imageid_7;
						merge_role.role_item_list[merge_role.count].upgrade_used_imageid_8 = role_info->role_info_list[i].upgrade_used_imageid_8;

						++merge_role.count;
					}
				}

				this->AddCachePlatNameDbIndex(original_panme, db_index);
			}

			if (merge_login_item->account_count <= 0)		// DB全部返回后 发回客户端
			{
				int sendlen = sizeof(Protocol::SCMergeRoleList) - (MAX_MERGE_ACCOUNT_ROLE_NUM - merge_role.count) * sizeof(Protocol::MergeRoleItem);
				EngineAdapter::Instance().NetSend(netid, (const char*)&merge_role, sendlen);

				delete it->second;
				m_merge_login_item_map.erase(it);
			}
		}
	}
}

void MergeLoginManager::AddCachePlatNameDbIndex(PlatName original_panme, int db_index)
{
	//original_panme[sizeof(PlatName) - 1] = 0;

	//m_cache_pname_db_index_map[original_panme].insert(db_index);
}

void MergeLoginManager::TimerClear()
{
	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (MergeLoginItemMap::iterator it = m_merge_login_item_map.begin(); it != m_merge_login_item_map.end();)
	{
		if (now_time >= it->second->invalid_time)
		{
			delete it->second;
			m_merge_login_item_map.erase(it++);
		}
		else
		{
			++it;
		}
	}

	MergeLoginTimer *timer = new MergeLoginTimer(this);
	EngineAdapter::Instance().CreateTimerSecond(10, timer);
}

void MergeLoginManager::ClearDbIndex(PlatName original_panme)
{
	//original_panme[sizeof(PlatName) - 1] = 0;

	//m_cache_pname_db_index_map.erase(original_panme);
}

void MergeLoginManager::ClearAllDbIndex()
{
	//m_cache_pname_db_index_map.clear();
}

int MergeLoginManager::GetAutoIncreaseKey()
{
	static const bool is_merged = LocalConfig::Instance().IsMergedServer();
	if (!is_merged) return 0;

	return ++m_auto_increase_key;
}

void MergeLoginManager::PlatLoginReq(NetID netid, PlatName original_panme, int db_index, int key, char can_insert)
{
	static PlatName temp_panme = {0};
	F_STRNCPY(temp_panme, original_panme, sizeof(temp_panme));
	PlatNameCatPostfix(temp_panme, db_index);

	if (LoginUserTrace::Instance().IsTrace(temp_panme))
	{
		loginlog::g_log_login_trace.printf(LL_INFO, "[OnLoginReq] [plat_name:%s]", temp_panme);
	}

	RMIUserLoginBackObjectImpl *ulboi = new RMIUserLoginBackObjectImpl();
	ulboi->netid = netid;
	ulboi->merge_key = key;
	F_STRNCPY(ulboi->pname, temp_panme, sizeof(ulboi->pname));
	F_STRNCPY(ulboi->original_panme, original_panme, sizeof(ulboi->original_panme));

	RMILoginClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
	lc.UserLoginAsyn(ulboi->pname, can_insert, ulboi);
}

std::set<int> & MergeLoginManager::GetAllowServerIdSet(PlatName original_panme)
{
	/*
	PlatNameDbIndexMap::iterator it = m_cache_pname_db_index_map.find(original_panme);
	if (it != m_cache_pname_db_index_map.end())
	{
		return it->second;
	}
	*/

	return LocalConfig::Instance().GetAllowServerIdSet();
}



