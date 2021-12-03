#include "fishpoolmanager.hpp"
#include "fishpool.hpp"

#include "scene/scenemanager.h"
#include "world.h"
#include "servercommon/serverclmempool.h"
#include "servercommon/fishpooldef.hpp"
#include "global/rmibackobjdef.h"
#include "serverlogic.h"
#include "internalcomm.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "global/fishpool/fishpoolconfig.hpp"
#include "globalconfig/namecreateconfig.hpp"
#include "servercommon/struct/structcommon.h"
#include "global/usercache/usercache.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

FishPoolManager::FishPoolManager() 
	: m_data_state(FISH_POOL_MANAGER_DATA_STATE_INVALID), m_last_save_timestamp(0), m_last_update_timestamp(0)
{
	memset(&m_change_fishpool_param, 0, sizeof(m_change_fishpool_param));
}

FishPoolManager::~FishPoolManager()
{
	for (UserFishPoolMapIt it = m_user_fishpool_map.begin(); it != m_user_fishpool_map.end(); ++ it)
	{
		FishPool *fishpool = it->second;
		if (NULL != fishpool)
		{
			delete fishpool;
		}
	}
	
	m_user_fishpool_map.clear();
}

FishPoolManager & FishPoolManager::Instance()
{
	static FishPoolManager _instance;
	return _instance;
}

bool FishPoolManager::OnServerStart()
{
	this->LoadFishPool(0);
	return true; 
}

void FishPoolManager::OnServerStop()
{
	this->CheckSaveFishPool(EngineAdapter::Instance().Time() + 2 * FISH_POOL_CHECK_SAVE_INTERVAL);
}

void FishPoolManager::Update(unsigned long interval, time_t now_second)
{
	if (!this->IsLoadFinish()) return;

	if (now_second >= m_last_update_timestamp + 3)
	{
		m_last_update_timestamp = now_second;

		for (UserFishPoolMapIt it = m_user_fishpool_map.begin(); it != m_user_fishpool_map.end(); ++ it)
		{
			FishPool *fishpool = it->second;
			if (NULL != fishpool)
			{
				fishpool->Update(interval, now_second);
			}
		}
	}

	this->CheckSaveFishPool(now_second);
}

void FishPoolManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	for (UserFishPoolMapIt it = m_user_fishpool_map.begin(); it != m_user_fishpool_map.end(); ++ it)
	{
		FishPool *fishpool = it->second;
		if (NULL != fishpool)
		{
			fishpool->OnDayChange(old_dayid, now_dayid);
		}
	}
}

void FishPoolManager::OnUserLevelChange(Role *role)
{
 	if (NULL == role) return;
 
 	this->OnSyncUserInfo(role);
}

void FishPoolManager::OnUserLogin(Role *role)
{
 	if (NULL == role) return;
 
 	this->OnSyncUserInfo(role);

	FishPool *fish_pool = this->GetFishPool(role->GetUID());
	if (NULL != fish_pool)
	{
		fish_pool->SendAllRaiseInfo(role);
		fish_pool->SendCommonInfo(role);
		this->SendStealFishPoolGeneralInfo(role);
	}
}

void FishPoolManager::OnUserLogout(Role *role)
{
 	if (NULL == role) return;
 
 	this->OnSyncUserInfo(role);
}

void FishPoolManager::OnSyncUserInfo(Role *role)
{
 	if (NULL == role) return;
 
 	if (!this->IsLoadFinish()) return;
 
 	if (CrossConfig::Instance().IsHiddenServer())
 	{
 		return;
 	}
 
 	JoinLimitUnit limit_unit;
 	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_FISH_POOL, &limit_unit);			 //需要添加等级限制
 	if (is_limit && role ->GetLevel() < limit_unit.level)
 	{
 		return;
 	}
 
 	UserFishPoolMapIt it = m_user_fishpool_map.find(role->GetUID());
 	if (m_user_fishpool_map.end() == it)
 	{
 		FishPool *fish_pool = new FishPool(this);
 
 		FishPoolListParam::FishPoolAttr fish_pool_attr;
 		fish_pool_attr.role_id = role->GetUID();
 
 		fish_pool->Init(fish_pool_attr);
 		m_user_fishpool_map[role->GetUID()] = fish_pool;
 
 		this->OnFishPoolChange(fish_pool, structcommon::CS_INSERT);
 	}
 
 	 it = m_user_fishpool_map.find(role->GetUID());
 	 if (it != m_user_fishpool_map.end())
 	 {
 		 FishPool *fish_pool = it->second;
 		 if (NULL != fish_pool)
 		 {
 			fish_pool->OnSyncUserInfo(role);
 		 }
 	 }
}

void FishPoolManager::RaiseFish(Role *role)
{
 	if (NULL == role)
 	{
 		return;
 	}
 
 	FishPool *fish_pool = this->GetFishPool(role->GetUID());
 	if (NULL != fish_pool)
 	{
 		fish_pool->RaiseFish(role);
 	}
}

void FishPoolManager::HarvestFish(Role *role)
{
 	if (NULL == role)
 	{
 		return;
 	}
 
 	FishPool *fish_pool = this->GetFishPool(role->GetUID());
 	if (NULL != fish_pool)
 	{
 		fish_pool->HarvestFish(role);
 	}
}

void FishPoolManager::StealFish(Role *steal_role, int be_steal_uid, char is_fake_pool, char type, char quality)
{
 	if (NULL == steal_role)
 	{
 		return;
 	}
	
	if (1 == is_fake_pool)
	{
		FishPool *fish_pool = this->GetFishPool(steal_role->GetUID());
		if (NULL != fish_pool)
		{
			fish_pool->StealFakeFish(steal_role, be_steal_uid, type);
		}
	}
	else
	{
		FishPool *fish_pool = this->GetFishPool(be_steal_uid);
		if (NULL != fish_pool)
		{
			fish_pool->StealFish(steal_role, be_steal_uid, type, quality);
		}
	}
}

void FishPoolManager::BuyBulletReq(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	FishPool *fish_pool = this->GetFishPool(role->GetUID());
	if (NULL != fish_pool)
	{
		fish_pool->BuyBulletReq(role);
	}
}

void FishPoolManager::OnQueryReq(Role *role, int query_type, int param)
{
 	if (NULL == role) 
 	{
 		return;
 	}
 
 	switch (query_type)
 	{
 		case Protocol::FISH_POOL_QUERY_TYPE_ALL_INFO:
 			{
 				this->SendFishPoolDetailInfo(role, param);
 			}
 			break;
 
 		case Protocol::FISH_POOL_QUERY_TYPE_RAISE_INFO:
 			{
 				this->SendFishPoolRaiseInfo(role, param);
 			}
 			break;
 
 		case Protocol::FISH_POOL_QUERY_TYPE_WORLD_GENERAL_INFO:
 			{
 				this->SendWorldFishPoolGeneralInfo(role);
 			}
 			break;
 
 		case Protocol::FISH_POOL_QUERY_TYPE_STEAL_GENERAL_INFO:
 			{
				this->SendStealFishPoolGeneralInfo(role);
 			}
			break;

		case Protocol::FISH_POOL_UP_FISH_QUALITY:
			{
				this->UpFishQuality(role);
			}
			break;

		case Protocol::FISH_POOL_BUY_FANG_FISH_TIMES:
			{
				this->BuyFangFishTimes(role);
			}
 			break;
 	}
}

void FishPoolManager::UpFishQuality(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	FishPool *fish_pool = this->GetFishPool(role->GetUID());

	if (NULL != fish_pool)
	{
		fish_pool->UpFishQuality(role);
	}
}

void FishPoolManager::BuyFangFishTimes(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	FishPool *fish_pool = this->GetFishPool(role->GetUID());

	if (NULL != fish_pool)
	{
		fish_pool->BuyFangFishTimes(role);
	}
}

void RMIInitFishPoolObjectImpl::InitFishPoolRet(int ret, const FishPoolListParam &fish_pool_list_param)
{
 	if (0 != ret)
 	{
 		printf(" RMIInitFishPoolObjectImpl::InitFishPoolRet Failed \n");
 		ServerLogic::GetInstServerLogic()->StopServer();
 		return;
 	}
 
 	long long next_id_from = 0;
 
 	for (int i = 0; i < fish_pool_list_param.count; ++ i)
 	{
 		if (fish_pool_list_param.fish_pool_list[i].id_fishpool > next_id_from)
 		{
 			next_id_from = fish_pool_list_param.fish_pool_list[i].id_fishpool;
 		}
 
 		if (fish_pool_list_param.fish_pool_list[i].role_id <= 0) continue;
 
 		if (!fish_pool_manager->InitFishPool(fish_pool_list_param.fish_pool_list[i]))
 		{
 			printf(" RMIInitFishPoolObjectImpl::InitFishPoolRet InitFishPool Failed \n");
 			EngineAdapter::Instance().StopGame();
 			return;
 		}
 	}
 
 	if (fish_pool_list_param.count <= 0)
 	{
 		fish_pool_manager->LoadFishPoolSucc();
 	}
 	else
 	{
 		fish_pool_manager->LoadFishPool(next_id_from);
 	}
}

bool FishPoolManager::LoadFishPool(long long id_from)
{
 	if (this->IsLoadFinish()) return false;
 
 	m_data_state = FISH_POOL_MANAGER_DATA_STATE_LOADING;
 
 	RMIInitFishPoolObjectImpl *impl = new RMIInitFishPoolObjectImpl();
 	impl->fish_pool_manager = this;
 
 	RMIGlobalClient gc;
 	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());  
 	return gc.InitFishPoolAsyn(id_from, impl);
}

void FishPoolManager::LoadFishPoolSucc()
{
	m_data_state = FISH_POOL_MANAGER_DATA_STATE_LOAD_FINISH;
}

bool FishPoolManager::InitFishPool(const FishPoolListParam::FishPoolAttr &fish_pool_attr)
{
 	if (m_user_fishpool_map.end() != m_user_fishpool_map.find(fish_pool_attr.role_id))
 	{
 		printf("FishPoolManager::InitFishPool m_user_fishpool_map.end() != m_user_fishpool_map.find");
 		return false;
 	}
 
 	FishPool *fish_pool = new FishPool(this);
 	fish_pool->Init(fish_pool_attr);
 
 	m_user_fishpool_map[fish_pool_attr.role_id] = fish_pool;

	return true;
}

bool FishPoolManager::OnFishPoolChange(FishPool *fishpool, int change_state)
{
 	if (NULL == fishpool || !this->CheckSaveFishPool(EngineAdapter::Instance().Time())) return false;
 
 	if (m_change_fishpool_param.count < FISH_POOL_ONCE_LOAD_MAX_COUNT)
 	{
 		for (int i = 0; i < m_change_fishpool_param.count; ++ i) // 合并写DB 减少数据库并发
 		{
 			FishPoolListParam::FishPoolAttr *save_fishpoolattr = &m_change_fishpool_param.fish_pool_list[i];
 			if (save_fishpoolattr->role_id == fishpool->GetOwnerUid() && 
 				structcommon::CS_UPDATE == save_fishpoolattr->change_state && structcommon::CS_UPDATE == change_state)
 			{
 				fishpool->GetAttr(save_fishpoolattr);
 				return true;
 			}
 		}
 
 		FishPoolListParam::FishPoolAttr *fishpool_attr = &m_change_fishpool_param.fish_pool_list[m_change_fishpool_param.count];
 
 		fishpool_attr->change_state = change_state;
 		fishpool->GetAttr(fishpool_attr);
 
 		++ m_change_fishpool_param.count;
 
 		return true; 
 	}

	return false;
}

bool FishPoolManager::CheckSaveFishPool(time_t now_second)
{
 	if (now_second >= m_last_save_timestamp + FISH_POOL_CHECK_SAVE_INTERVAL || m_change_fishpool_param.count >= FISH_POOL_ONCE_LOAD_MAX_COUNT) 
 	{
 		m_last_save_timestamp = now_second;
 
 		if (m_change_fishpool_param.count > 0)
 		{
 			RMISaveFishPoolObjectImpl *impl = new RMISaveFishPoolObjectImpl();
 
 			RMIGlobalClient gc;
 			gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB()); 
 
 			if (!gc.SaveFishPoolAsyn(m_change_fishpool_param, impl)) return false;
 
 			m_change_fishpool_param.count = 0;
 		}
 	}

	return true; 
}

FishPool * FishPoolManager::GetFishPool(int uid)
{
 	if (uid <= 0) return NULL;
 
 	UserFishPoolMapIt it = m_user_fishpool_map.find(uid);
 	if (it != m_user_fishpool_map.end())
 	{
 		return it->second;
 	}

	return NULL;
}

void FishPoolManager::SendWorldFishPoolGeneralInfo(Role *role)
{
	if (NULL == role) return;

	FishPool *self_fish_pool = this->GetFishPool(role->GetUID());
	if (NULL == self_fish_pool) return;

	static Protocol::SCFishPoolWorldGeneralInfo fpfgi;
	for (int i = 0; i < FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT; i++)
	{
		fpfgi.info_list[i].Reset();
	}
	fpfgi.info_count = self_fish_pool->GetOtherPoolList(fpfgi.info_list);
	
	// 随机获取世界的鱼池
	static int rand_role_list[FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT];
	memset(rand_role_list, 0, sizeof(rand_role_list));
	int rand_role_count = UserCacheManager::Instance().RandomGetUidList(rand_role_list, FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT);

 	for (int i = 0; i < rand_role_count && i < FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT && fpfgi.info_count < FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT; ++ i)
 	{
		if (0 == rand_role_list[i] || role->GetUID() == rand_role_list[i]) continue;

 		FishPool *fish_pool = this->GetFishPool(rand_role_list[i]);
		if (NULL == fish_pool) continue;

 		if (fish_pool->GetOwnerUid() == rand_role_list[i] && fish_pool->IsCanSteal())
 		{
			if (self_fish_pool->IsInOtherPoolList(fish_pool)) continue;

 			const UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(fish_pool->GetOwnerUid());
 			if (NULL != user_node)
 			{
				FishPoolCommonDataParam & common_data = fish_pool->GetFishPoolCommonDataParam();

 				Protocol::FishPoolGeneralInfoItem &info_item = fpfgi.info_list[fpfgi.info_count];

 				info_item.owner_uid = fish_pool->GetOwnerUid();
 				user_node->GetName(info_item.owner_name);
 				info_item.fish_quality = common_data.fish_quality;
 				info_item.fish_num = common_data.fish_num;
 				info_item.fang_fish_time = common_data.fang_fish_time;
				info_item.is_fake_pool = 0;
				info_item.is_fuchou = -1;
				info_item.be_steal_quality = -1;
				info_item.steal_fish_time = -1;

 				++ fpfgi.info_count;
 			}
 		}
 	}

	// 不够12个，创建假的玩家鱼池
	int curr_count = fpfgi.info_count;
	if (fpfgi.info_count < 12)
	{
		for (int i = 0; i < 12 - curr_count && fpfgi.info_count < FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT; i++)
		{
			Protocol::FishPoolGeneralInfoItem &info_item = fpfgi.info_list[fpfgi.info_count];
			memset(info_item.owner_name, 0, sizeof(info_item.owner_name));

			if (LOGIC_CONFIG->GetNameCreateCfg().GetRandName(info_item.owner_name))
			{
				if (FISH_QUALITY_COUNT <= 0) return;
				int rand_quality = LOGIC_CONFIG->GetFishPoolCfg().GetFakeFishQuality();

				FishQualityTypeCfg * fish_quality_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetFishQualityCfg(rand_quality);
				if (NULL != fish_quality_cfg && fish_quality_cfg->need_time > 0)
				{
					info_item.owner_uid = RandomNum(99999) + RandomNum(99999);
					info_item.fish_quality = rand_quality;
					info_item.fish_num = fish_quality_cfg->count;
					if (fish_quality_cfg->need_time <= 0) return;
					info_item.fang_fish_time = static_cast<int>(EngineAdapter::Instance().Time()) - RandomNum(fish_quality_cfg->need_time);
					info_item.is_fake_pool = 1;
					info_item.is_fuchou = -1;
					info_item.be_steal_quality = -1;
					info_item.steal_fish_time = -1;

					++fpfgi.info_count;
				}
			}
		}
	}

	self_fish_pool->SetOtherPoolList(fpfgi.info_list, FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT);
 	int send_len = sizeof(fpfgi) - sizeof(fpfgi.info_list[0]) * (FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT - fpfgi.info_count);
 	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&fpfgi, send_len);
}

void FishPoolManager::SendStealFishPoolGeneralInfo(Role *role)
{
	if (NULL == role) return;

	FishPool * fish_pool = this->GetFishPool(role->GetUID());
	if (NULL == fish_pool) return;

	static Protocol::SCFishPoolStealGeneralInfo fpfgi;
	fpfgi.info_count = 0;

	for (int i = 0; i < FISH_POOL_BE_TEAL_FISH_UID_MAX && fpfgi.info_count < FISH_POOL_BE_TEAL_FISH_UID_MAX; ++i)
	{
		TealFishParam &teal_fish_param = fish_pool->GetFishPoolCommonDataParam().teal_fish_list[i];

		if (0 == teal_fish_param.uid) continue;

		FishPool *fish_pool = this->GetFishPool(teal_fish_param.uid);
		if (NULL != fish_pool && fish_pool->GetOwnerUid() == teal_fish_param.uid)
		{
			const UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(fish_pool->GetOwnerUid());
			if (NULL != user_node)
			{
				FishPoolCommonDataParam & common_data = fish_pool->GetFishPoolCommonDataParam();

				Protocol::FishPoolGeneralInfoItem &info_item = fpfgi.info_list[fpfgi.info_count];

				info_item.owner_uid = fish_pool->GetOwnerUid();
				user_node->GetName(info_item.owner_name);
				info_item.fish_quality = common_data.fish_quality;
				info_item.fish_num = common_data.fish_num;
				info_item.fang_fish_time = common_data.fang_fish_time;
				info_item.is_fake_pool = 0;
				info_item.is_fuchou = teal_fish_param.is_fuchou;
				info_item.be_steal_quality = teal_fish_param.be_steal_quality;
				info_item.steal_fish_time = teal_fish_param.steal_fish_time;

				++fpfgi.info_count;
			}
		}
	}

	int send_len = sizeof(fpfgi) - sizeof(fpfgi.info_list[0]) * (FISH_POOL_BE_TEAL_FISH_UID_MAX - fpfgi.info_count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&fpfgi, send_len);
}

void FishPoolManager::SendFishPoolDetailInfo(Role *role, int uid)
{
 	if (NULL == role)
 	{
 		return;
 	}
 
 	FishPool *fish_pool = this->GetFishPool(uid);
 	if (NULL != fish_pool)
 	{
 		fish_pool->SendCommonInfo(role);
		fish_pool->SendAllRaiseInfo(role);
 	}
}

void FishPoolManager::SendFishPoolRaiseInfo(Role *role, int uid)
{
 	if (NULL == role)
 	{
 		return;
 	}
 
 	FishPool *fish_pool = this->GetFishPool(uid);
 	if (NULL != fish_pool)
 	{
 		fish_pool->SendAllRaiseInfo(role);
 	}
}


