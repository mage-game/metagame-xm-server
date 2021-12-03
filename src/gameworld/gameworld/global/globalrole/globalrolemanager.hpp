#ifndef __GLOBAL_ROLE_MANAGER_HPP__
#define __GLOBAL_ROLE_MANAGER_HPP__

#include <map>
#include <vector>
#include "servercommon/struct/global/globalroledataparam.hpp"

enum GLOBAL_ROLE_MANAGER_DATA_STATE
{	
	GLOBAL_ROLE_MANAGER_DATA_STATE_INVALID = 0,						// 初始状态
	GLOBAL_ROLE_MANAGER_DATA_STATE_LOADING,							// 数据加载中 
	GLOBAL_ROLE_MANAGER_DATA_STATE_LOAD_FINISH,						// 数据加载完成
	GLOBAL_ROLE_MANAGER_DATA_STATE_MAX,
};

class Role;
class GlobalRole;
class RMIInitFishPoolObjectImpl;
class LittlePet;
class LoveTree;
class LoveContract;
class JingLingHome;
class RoleFighting;
class QingyuanGlobal;
class CommonDataGlobal;
class SpouseHome;

typedef bool CheckGlobalRole(GlobalRole *global_role, void *param);

#define GLOBAL_ROLE_MGR GlobalRoleManager::Instance()

class GlobalRoleManager
{
public:
	typedef std::map<int, GlobalRole*> GlobalRoleMap;
	typedef std::map<int, GlobalRole*>::iterator GlobalRoleMapIt;

	friend class RMIInitGlobalRoleObjectImpl;
	friend class GlobalRole;

	static GlobalRoleManager & Instance();

	bool OnServerStart();
	void OnServerStop();

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool IsLoadFinish() { return GLOBAL_ROLE_MANAGER_DATA_STATE_LOAD_FINISH == m_data_state; }

	void OnRoleLogin(Role *role);
	void OnRoleLogout(Role *role);
	bool OnGlobalRoleDataChange(GlobalRole *global_role, int change_state);

	int RandGetRole(GlobalRole *role_list[], int role_max, CheckGlobalRole check_func = NULL, void *param = NULL);

	const GlobalRoleMap & GetGlobalRoleMap() const { return m_global_role_map; }

	GlobalRole * GetGlobalRole(int uid);
	RoleAppearance * GetRoleAppearance(int uid);
	CommonDataGlobal * GetCommonDataGlobal(int uid);

	LittlePet * GetLittlePet(int uid);
	LoveTree * GetLoveTree(int uid);
	LoveContract * GetLoveContract(int uid);
	JingLingHome * GeJingLingHome(int uid);
	RoleFighting * GetRoleFighting(int uid);
	QingyuanGlobal * GetQingyuanGlobal(int uid);
	SpouseHome * GetSpouseHomeGlobal(int uid);

private:
	GlobalRoleManager();
	~GlobalRoleManager();

	// 从数据库Load
	bool LoadData(long long id_from);
	void LoadDataSucc();
	bool InitGlobalRole(const GlobalRoleDatalListParam::RoleDataAttr &global_role_attr);

	// 保存到数据库
	bool CheckSaveGlobalRole(time_t now_second);

	int m_data_state;
	time_t m_last_save_timestamp;

	GlobalRoleMap m_global_role_map;

	bool m_is_role_map_change;
	std::vector<GlobalRole*> m_global_role_rand_pool;

	GlobalRoleDatalListParam m_change_global_role_param;

	time_t m_last_update_timestamp;
};

#endif

