#ifndef __FISH_POOL_HPP__
#define __FISH_POOL_HPP__

class Role;

#include "servercommon/struct/global/fishpoolparam.hpp"
#include "servercommon/fishpooldef.hpp"
#include "protocal/msgfishpool.h"

class FishPoolManager;

class FishPool
{
public:
	FishPool(FishPoolManager *fish_pool_manager);
	~FishPool();

	void Init(const FishPoolListParam::FishPoolAttr &fish_pool_attr);
	void GetAttr(FishPoolListParam::FishPoolAttr *fish_pool_attr);

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	int GetOwnerUid() { return m_role_id; }

	void OnSyncUserInfo(Role *role);
	void RaiseFish(Role *role);
	void HarvestFish(Role *role, bool is_skip = false);
	void StealFish(Role *steal_role, int be_steal_uid, char type, char quality);
	void StealFakeFish(Role *steal_role, int be_steal_uid, char type);
	
	void BuyBulletReq(Role *role);
	bool HasBullet();
	void IncreaseBullet(int add_value);
	void ConsumeBullet();

	void SendAllRaiseInfo(Role *role);
	void SendCommonInfo(Role *role);
	
	bool IsCanSteal();

	void UpFishQuality(Role *role);
	void BuyFangFishTimes(Role *role);
	void FishPoolChange(Role * role, int uid, int fish_num, int fish_quality, int is_steal_succ);
	void AddTealUid(int uid);
	bool IsStealUid(int uid);
	void ChangeTealUid(int uid);
	bool IsInOtherPoolList(FishPool * fish_pool);
	int GetOtherPoolList(Protocol::FishPoolGeneralInfoItem other_pool_list[]);
	void SetOtherPoolList(Protocol::FishPoolGeneralInfoItem * other_pool_list, int array_size);

	void OnSkipFishPool(Role *role, int quality);

	Protocol::FishPoolGeneralInfoItem * GetFakePool(int fake_uid);

	FishPoolCommonDataParam & GetFishPoolCommonDataParam() { return m_common_data; }

	void * operator new(size_t c);
	void operator delete(void *m);

private:

	FishPoolManager *m_fish_pool_manger;
	int m_role_id;

	Protocol::FishPoolGeneralInfoItem m_other_pool_list[FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT];

	FishPoolCommonDataParam m_common_data;
};

#endif


