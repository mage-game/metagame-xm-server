#ifndef __FISH_POOL_MANAGER_HPP__
#define __FISH_POOL_MANAGER_HPP__

enum FISH_POOL_MANAGER_DATA_STATE
{	
	FISH_POOL_MANAGER_DATA_STATE_INVALID = 0,						// 初始状态
	FISH_POOL_MANAGER_DATA_STATE_LOADING,							// 数据加载中 
	FISH_POOL_MANAGER_DATA_STATE_LOAD_FINISH,						// 数据加载完成
	FISH_POOL_MANAGER_DATA_STATE_MAX,
};

class Role;
class FishPool;
class RMIInitFishPoolObjectImpl;

#include "servercommon/struct/global/fishpoolparam.hpp"
#include <map>

class FishPoolManager
{
public:
	friend class RMIInitFishPoolObjectImpl;
	friend class FishPool;

	static FishPoolManager & Instance();

	bool OnServerStart();
	void OnServerStop();

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool IsLoadFinish() { return FISH_POOL_MANAGER_DATA_STATE_LOAD_FINISH == m_data_state; }

	void OnUserLevelChange(Role *role);
	void OnUserLogin(Role *role);
	void OnUserLogout(Role *role);
	void OnSyncUserInfo(Role *role);

	void RaiseFish(Role *role);
	void HarvestFish(Role *role);
	void StealFish(Role *steal_role, int be_steal_uid, char is_fake_pool, char type, char quality);
	void BuyBulletReq(Role *role);
	void OnQueryReq(Role *role, int query_type, int param);

	void UpFishQuality(Role *role);
	void BuyFangFishTimes(Role *role);
	FishPool * GetFishPool(int uid);

	void SendWorldFishPoolGeneralInfo(Role *role);
	void SendStealFishPoolGeneralInfo(Role *role);

private:
	FishPoolManager();
	~FishPoolManager();

	// 从数据库Load
	bool LoadFishPool(long long id_from);
	void LoadFishPoolSucc();
	bool InitFishPool(const FishPoolListParam::FishPoolAttr &fish_pool_attr);

	// 保存到数据库
	bool OnFishPoolChange(FishPool *fishpool, int change_state);
	bool CheckSaveFishPool(time_t now_second);

	void SendFishPoolDetailInfo(Role *role, int target_uid);
	void SendFishPoolRaiseInfo(Role *role, int target_uid);

	typedef std::map<int, FishPool*> UserFishPoolMap;
	typedef std::map<int, FishPool*>::iterator UserFishPoolMapIt;
	UserFishPoolMap m_user_fishpool_map;
	FishPoolListParam m_change_fishpool_param;

	int m_data_state;
	time_t m_last_save_timestamp;
	time_t m_last_update_timestamp;
};

#endif

