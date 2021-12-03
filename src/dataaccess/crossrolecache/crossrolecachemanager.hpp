#ifndef __CROSS_ROLE_CACHE_MANAGER_HPP__
#define __CROSS_ROLE_CACHE_MANAGER_HPP__

#include <map>

#include "engineadapter.h"

#include "servercommon/struct/roleallparam.hpp"
#include "servercommon/struct/roleinfolist.h"

#include "servercommon/crossdef.hpp"

class CrossRoleCacheManager
{
public:
	static CrossRoleCacheManager & Instance();

	bool CreateCrossRole(PlatName pname, int cross_activity_type, long long original_uuid, int *role_id);

	bool RoleSave(int role_id, 
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
		const DailyFindListParam &daily_find_param);

	bool SaveTableRoleStatus(int role_id, bool is_logout);

	bool RoleInit(int uid, 
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
		long long *last_save_time);

	int GetCrossRoleID(PlatName pname);

	bool GetRoleInfo(int *role_id, int num, RoleInfoList *info);
	
private:
	CrossRoleCacheManager();
	~CrossRoleCacheManager();

	int GetAutoIncId(int cross_activity_type);

	bool SaveCrossRoleCache(int uid, RoleAllParam &all_param);
	bool GetCrossRoleCache(int uid, RoleAllParam *all_param, long long *last_save_time);
	void RemoveCrossRoleCache(PlatName pname);
	void RemoveCrossRoleCache(int uid);

	int m_auto_inc_id[CROSS_ACTIVITY_TYPE_MAX];	// ���������id

	struct CrossRoleCacheInfo
	{
		CrossRoleCacheInfo() : role_id(0), original_uuid(0), create_time(0), last_save_time(0), is_online(false)
		{
			memset(pname, 0, sizeof(pname));
			all_param.Reset();
		}

		PlatName		pname;					// ƽ̨��
		int				role_id;				// ��ɫid
		long long		original_uuid;			// ԭ����ɫȫ��ID
		time_t			create_time;			// ����ʱ��
		RoleAllParam	all_param;				// ���������в���
		time_t			last_save_time;			// ��󱣴�ʱ��
		bool			is_online;				// �Ƿ�����
	};

	typedef std::map<std::string, int> CrossRoleIDMap;
	typedef std::map<std::string, int>::iterator CrossRoleIDMapIt;
	CrossRoleIDMap m_cross_role_id_map;			// �����ɫpname��ID��Ӧ��

	typedef std::map<int, CrossRoleCacheInfo> CrossRoleCacheMap;
	typedef std::map<int, CrossRoleCacheInfo>::iterator CrossRoleCacheMapIt;
	CrossRoleCacheMap m_cross_role_cache_map;	// �����ɫ��Ϣ��
};

#endif

