
#ifndef RMIROLE_H
#define RMIROLE_H

#include "servercommon/basedef.h"
#include "servercommon/struct/roleinfolist.h"
#include "rmi/rmibase.h"
#include "servercommon/servercommon.h"

class RoleInitParam;
class RoleOtherInitParam;
class SkillListParam;
class ItemListParam;
class HotkeyListParam;
class FriendListParam;
class BlacklistsParam;
class EnemyListParam;
class BusinessListParam;
class DailyFindListParam;
class RoleQueryParam;
class IStatement;

class RMIRoleObject : public rmi::RMIObject
{
public:
	RMIRoleObject();
	virtual ~RMIRoleObject();

	virtual void __free();
	virtual const char * __get_obj_id();

	static int __GetRoleInfo(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int GetRoleInfo(int *role_id, int num, RoleInfoList *info);

	// ע�� ÿ����һ��������ݱ�������RoleInit RoleSave LoadRoleToCache DestroyRole RemoveRoleFromCache  ����ط���ʵ������߼�

	static int __RoleInit(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int RoleInit(int uid, 
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
						long long *last_save_time);	// �ӱ�ʱ�ǵ���DestroyRole��ҲҪɾ��

	static int __RoleSave(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int RoleSave(int uid, 
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

	static int __CreateRole(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int CreateRole(const RoleInitParam &role_p, int db_index, int *role_id);

	static int __DestroyRole(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int DestroyRole(int role_id);

	static int __RoleSaveStatus(TLVUnserializer &in_param, TLVSerializer *out_param);

	static int __FriendSave(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int FriendSave(int role_id, const FriendListParam &friends, const BlacklistsParam &blacks);

	static int RemoveRoleFromCache(int role_id);	

	static int __RoleQuery(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int RoleQuery(int uid, RoleQueryParam *other_p, ItemListParam *item_list_param);	

	static int __CreateCrossRole(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int CreateCrossRole(PlatName pname, int cross_activity_type, long long original_uuid, int *role_id);

private:
	static int SaveTableRole(IStatement *stmt, int role_id, bool is_logout, const RoleInitParam &role_p, const RoleOtherInitParam &other_p, int *error);
	static int SaveTableItemList(IStatement *stmt, int role_id, const ItemListParam &item_list_param);
	static int SaveTableHotkey(IStatement *stmt, int role_id, const HotkeyListParam &hotkey);
	static int SaveTableFrinend(IStatement *stmt, int role_id, const FriendListParam &friends);
	static int SaveBlacklists(IStatement *stmt, int role_id, const BlacklistsParam &blacks);
	static int SaveTableEnemy(IStatement *stmt, int role_id, const EnemyListParam &enemy);
	static int SaveRoleSkill(IStatement *stmt, int role_id, const SkillListParam &skill_param);
	static int SaveDailyFind(IStatement *stmt, int role_id, const DailyFindListParam &daily_find_param);

	static int LoadRoleToCache(IStatement *stmt, int role_id);			// �����������
	static int CreateRoleId(int *role_id);
	static int SaveTableRoleStatus(int role_id, bool is_logout, bool is_micro_pc);
};

#endif
