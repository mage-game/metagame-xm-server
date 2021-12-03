#include "crossrolecachemanager.hpp"

CrossRoleCacheManager::CrossRoleCacheManager()
{
	memset(m_auto_inc_id, 0, sizeof(m_auto_inc_id));
}

CrossRoleCacheManager::~CrossRoleCacheManager()
{
}

CrossRoleCacheManager & CrossRoleCacheManager::Instance()
{
	static CrossRoleCacheManager crcm;
	return crcm;
}

bool CrossRoleCacheManager::CreateCrossRole(PlatName pname, int cross_activity_type, long long original_uuid, int *role_id)
{
	if (0 == pname[0] || cross_activity_type <= CROSS_ACTIVITY_TYPE_INVALID || cross_activity_type >= CROSS_ACTIVITY_TYPE_RESERVED || NULL == role_id)
	{
		return false;
	}

	this->RemoveCrossRoleCache(pname);
	
	bool is_create_role_id_succ = false;
	int new_role_id = 0;

	int loop_count = 0;
	while (++ loop_count < 1024)
	{
		int inc_id = this->GetAutoIncId(cross_activity_type);

		int real_id = (((cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID) << CROSS_USER_CROSS_ACTIVITY_MARK_BIT) | inc_id);

		new_role_id = UidToInt(UserID(CROSS_USER_DB_INDEX, real_id));

		if (m_cross_role_cache_map.find(new_role_id) == m_cross_role_cache_map.end())
		{
			is_create_role_id_succ = true;

			break;
		}
	}

	if (!is_create_role_id_succ) return false;

	static CrossRoleCacheInfo cross_role_cache;
	F_STRNCPY(cross_role_cache.pname, pname, sizeof(PlatName));
	cross_role_cache.role_id = new_role_id;
	cross_role_cache.original_uuid = original_uuid;
	cross_role_cache.all_param.Reset();
	cross_role_cache.create_time = EngineAdapter::Instance().Time();

	m_cross_role_id_map[pname] = new_role_id;
	m_cross_role_cache_map[new_role_id] = cross_role_cache;

	*role_id = new_role_id;

	return true;
}

int CrossRoleCacheManager::GetCrossRoleID(PlatName pname)
{
	pname[sizeof(PlatName) - 1] = 0;

	CrossRoleIDMapIt it = m_cross_role_id_map.find(pname);
	if (it != m_cross_role_id_map.end())
	{
		return it->second;
	}

	return 0;
}

bool CrossRoleCacheManager::SaveCrossRoleCache(int uid, RoleAllParam &all_param)
{
	if (uid <= 0) return false;

	CrossRoleCacheMapIt it = m_cross_role_cache_map.find(uid);
	if (it == m_cross_role_cache_map.end()) return false;

//	all_param.other_param.role_cross_param.cross_original_uuid = it->second.original_uuid;

	it->second.all_param = all_param;
	it->second.last_save_time = EngineAdapter::Instance().Time();

	return true;
}

bool CrossRoleCacheManager::GetCrossRoleCache(int uid, RoleAllParam *all_param, long long *last_save_time)
{
	if (uid <= 0) return false;

	CrossRoleCacheMapIt it = m_cross_role_cache_map.find(uid);
	if (it == m_cross_role_cache_map.end()) return false;

	*all_param = it->second.all_param;
	*last_save_time = it->second.last_save_time;

//	all_param->other_param.role_cross_param.cross_original_uuid = it->second.original_uuid;

	return true;
}

void CrossRoleCacheManager::RemoveCrossRoleCache(PlatName pname)
{
	pname[sizeof(PlatName) - 1] = 0;

	if (0 == pname[0]) return;

	CrossRoleIDMapIt it = m_cross_role_id_map.find(pname);
	if (it == m_cross_role_id_map.end()) return;
	
	m_cross_role_cache_map.erase(it->second);
	m_cross_role_id_map.erase(it);
}

void CrossRoleCacheManager::RemoveCrossRoleCache(int uid)
{
	if (uid <= 0) return;

	CrossRoleCacheMapIt it = m_cross_role_cache_map.find(uid);
	if (it == m_cross_role_cache_map.end()) return;

	m_cross_role_id_map.erase(it->second.pname);
	m_cross_role_cache_map.erase(it);
}

int CrossRoleCacheManager::GetAutoIncId(int cross_activity_type)
{
	int cross_activity_type_index = cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID;
	if (cross_activity_type_index <= 0 || cross_activity_type_index >= CROSS_ACTIVITY_TYPE_MAX) return -1;

	if (m_auto_inc_id[cross_activity_type_index] >= CROSS_USER_INC_ID_MAX)
	{
		m_auto_inc_id[cross_activity_type_index] = 0;
	}

	++ m_auto_inc_id[cross_activity_type_index];

	return m_auto_inc_id[cross_activity_type_index];
}

bool CrossRoleCacheManager::RoleSave(int role_id, 
									 bool is_logout,
									 const RoleInitParam &role_p, 
									 const RoleOtherInitParam &other_p,
									 const ItemListParam &item_list_param,
									 const HotkeyListParam &hotkey,
									 const FriendListParam &friends,
									 const BlacklistsParam &blacklists_param,
									 const EnemyListParam &enemy,
									 const BusinessListParam &business, 
									 const SkillListParam &skill_param,
									 const DailyFindListParam &daily_find_param)
{
	if (is_logout)
	{
		this->RemoveCrossRoleCache(role_id);

		return true;
	}

	static RoleAllParam all_param;
	all_param.Reset();

	all_param.role_param = role_p;
	all_param.other_param = other_p;
	all_param.item_param = item_list_param;
	all_param.hotkey_param = hotkey;
	all_param.friend_param = friends;
	all_param.black_param = blacklists_param;
	all_param.enemy_param = enemy;
	all_param.business_param = business;
	all_param.skill_param = skill_param;
	all_param.daily_find_param = daily_find_param;

	return this->SaveCrossRoleCache(role_id, all_param);
}

bool CrossRoleCacheManager::SaveTableRoleStatus(int role_id, bool is_logout)
{
	if (is_logout)
	{
		this->RemoveCrossRoleCache(role_id);

		return true;
	}

	CrossRoleCacheMapIt it = m_cross_role_cache_map.find(role_id);
	if (it == m_cross_role_cache_map.end()) return false;

	it->second.is_online = true;

	return true;
}

bool CrossRoleCacheManager::RoleInit(int uid, 
									 RoleInitParam *p, 
									 RoleOtherInitParam *other_p,
									 ItemListParam *item_list_param, 
									 HotkeyListParam *hotkey,
									 FriendListParam *friends,
									 BlacklistsParam *blacklists_param,
									 EnemyListParam *enemy,
									 BusinessListParam *business_good,
									 SkillListParam *skill_param,
									 DailyFindListParam *daily_find_param,
									 long long *last_save_time)
{
	static RoleAllParam all_param;
	all_param.Reset();

	if (this->GetCrossRoleCache(uid, &all_param, last_save_time))
	{
		*p = all_param.role_param;
		*other_p = all_param.other_param;
		*item_list_param = all_param.item_param;
		*hotkey = all_param.hotkey_param;
		*friends = all_param.friend_param;
		*blacklists_param = all_param.black_param;
		*enemy = all_param.enemy_param;
		*business_good = all_param.business_param;
		*skill_param = all_param.skill_param;
		*daily_find_param = all_param.daily_find_param;

		return true;
	}

	return false;
}

bool CrossRoleCacheManager::GetRoleInfo(int *role_id, int num, RoleInfoList *info)
{
	info->count = 0;

	for (int i = 0; i < num && i < 32; ++ i)
	{
		CrossRoleCacheMapIt it = m_cross_role_cache_map.find(role_id[i]);
		if (it != m_cross_role_cache_map.end())
		{
			info->role_info_list[info->count].role_id = role_id[i];
			info->role_info_list[info->count].scene_id = it->second.all_param.role_param.scene_id;
			info->role_info_list[info->count].last_scene_id = it->second.all_param.role_param.last_scene_id;
			info->role_info_list[info->count].level = it->second.all_param.role_param.level;
			info->role_info_list[info->count].professional = it->second.all_param.role_param.prof;
			info->role_info_list[info->count].create_time = static_cast<unsigned int>(it->second.create_time);
			F_STRNCPY(info->role_info_list[info->count].role_name, it->second.all_param.role_param.role_name, sizeof(GameName));
			info->role_info_list[info->count].avatar = it->second.all_param.role_param.avatar;
			info->role_info_list[info->count].sex = it->second.all_param.role_param.sex;
			info->role_info_list[info->count].country = it->second.all_param.role_param.camp;
			info->role_info_list[info->count].wuqi_id = 0;
			auto& shizhuang_param = it->second.all_param.other_param.shizhuang_param.item_list;
			if (shizhuang_param[SHIZHUANG_TYPE_WUQI].is_use_special_img)
			{
				info->role_info_list[info->count].shizhuang_wuqi = shizhuang_param[SHIZHUANG_TYPE_WUQI].use_special_img;
			}
			else
			{
				info->role_info_list[info->count].shizhuang_wuqi = shizhuang_param[SHIZHUANG_TYPE_WUQI].use_img;
			}
			info->role_info_list[info->count].shizhuang_wuqi_is_special = shizhuang_param[SHIZHUANG_TYPE_WUQI].is_use_special_img;

			if (shizhuang_param[SHIZHUANG_TYPE_BODY].is_use_special_img)
			{
				info->role_info_list[info->count].shizhuang_body = shizhuang_param[SHIZHUANG_TYPE_BODY].use_special_img;
			}
			else
			{
				info->role_info_list[info->count].shizhuang_body = shizhuang_param[SHIZHUANG_TYPE_BODY].use_img;
			}
			info->role_info_list[info->count].shizhuang_body_is_special = shizhuang_param[SHIZHUANG_TYPE_BODY].is_use_special_img;

			info->role_info_list[info->count].shizhuang_wuqi = it->second.all_param.other_param.shizhuang_param.item_list[SHIZHUANG_TYPE_WUQI].use_img;
			info->role_info_list[info->count].shizhuang_body = it->second.all_param.other_param.shizhuang_param.item_list[SHIZHUANG_TYPE_BODY].use_img;
			info->role_info_list[info->count].wing_used_imageid = it->second.all_param.other_param.wing_param.used_imageid;
			info->role_info_list[info->count].halo_used_imageid = it->second.all_param.other_param.halo_param.used_imageid;
			info->role_info_list[info->count].yaoshi_used_imageid = it->second.all_param.other_param.yaoshi_param.used_imageid;
			info->role_info_list[info->count].toushi_used_imageid = it->second.all_param.other_param.toushi_param.used_imageid;
			info->role_info_list[info->count].qilinbi_used_imageid = it->second.all_param.other_param.qilinbi_param.used_imageid;
			info->role_info_list[info->count].mask_used_imageid = it->second.all_param.other_param.mask_param.used_imageid;
			info->role_info_list[info->count].upgrade_used_imageid_0 = it->second.all_param.other_param.upgrade_manager_param.data_list[UPGRADE_TYPE_0].used_imageid;
			info->role_info_list[info->count].upgrade_used_imageid_1 = it->second.all_param.other_param.upgrade_manager_param.data_list[UPGRADE_TYPE_1].used_imageid;
			info->role_info_list[info->count].upgrade_used_imageid_2 = it->second.all_param.other_param.upgrade_manager_param.data_list[UPGRADE_TYPE_2].used_imageid;
			info->role_info_list[info->count].upgrade_used_imageid_3 = it->second.all_param.other_param.upgrade_manager_param.data_list[UPGRADE_TYPE_3].used_imageid;
			info->role_info_list[info->count].upgrade_used_imageid_4 = it->second.all_param.other_param.upgrade_manager_param.data_list[UPGRADE_TYPE_4].used_imageid;
			info->role_info_list[info->count].upgrade_used_imageid_5 = it->second.all_param.other_param.upgrade_manager_param.data_list[UPGRADE_TYPE_5].used_imageid;
			info->role_info_list[info->count].upgrade_used_imageid_6 = it->second.all_param.other_param.upgrade_manager_param.data_list[UPGRADE_TYPE_6].used_imageid;
			info->role_info_list[info->count].upgrade_used_imageid_7 = it->second.all_param.other_param.upgrade_manager_param.data_list[UPGRADE_TYPE_7].used_imageid;
			info->role_info_list[info->count].upgrade_used_imageid_8 = it->second.all_param.other_param.upgrade_manager_param.data_list[UPGRADE_TYPE_8].used_imageid;

			++ info->count;
			if (info->count >= RoleInfoList::MAX_ROLE_NUM) break;
		}
	}

	return (info->count > 0);
}

