#ifndef __MERGE_LOGIN_MANAGER_H__
#define __MERGE_LOGIN_MANAGER_H__

#include <map>
#include <set>

#include "servercommon/servercommon.h"
#include "servercommon/logindef.h"
#include "inetworkmodule.h"

#include "protocal/msglogin.h"

class RoleInfoList;

struct MergeLoginItem
{
	MergeLoginItem() : account_count(0), invalid_time(0) {}

	void *			operator new(size_t c);
	void			operator delete(void *m);

	int account_count;
	unsigned int invalid_time;

	Protocol::SCMergeRoleList merge_role;
};

class MergeLoginManager
{
public:
	static MergeLoginManager & Instance();

	void OnLoginReq(NetID netid, PlatName original_panme, int db_index);

	void AddRoleInfoList(NetID netid, int merge_key, int result, const RoleInfoList *role_info, PlatName original_panme, int db_index);
	void AddCachePlatNameDbIndex(PlatName original_panme, int db_index);

	void TimerClear();

	void ClearDbIndex(PlatName original_panme);
	void ClearAllDbIndex();

private:
	MergeLoginManager();
	~MergeLoginManager();

	int GetAutoIncreaseKey();

	void PlatLoginReq(NetID netid, PlatName original_panme, int db_index, int merge_key, char can_insert);

	std::set<int> & GetAllowServerIdSet(PlatName original_panme);

	int m_auto_increase_key;

	typedef std::map<int, MergeLoginItem*> MergeLoginItemMap;
	MergeLoginItemMap m_merge_login_item_map;

	// typedef std::map<std::string, std::set<int> > PlatNameDbIndexMap;
	// PlatNameDbIndexMap m_cache_pname_db_index_map;
};

#endif

