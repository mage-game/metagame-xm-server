#include "campmanager.hpp"
#include "obj/character/role.h"
#include "protocal/msgsccamp.hpp"
#include "servercommon/internalprotocal/ggprotocal.h"

#include "global/rmibackobjdef.h"
#include "internalcomm.h"
#include "serverlogic.h"
#include "world.h"
#include "global/rank/rankmanager.hpp"
#include "global/usercache/usercache.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "scene/speciallogic/worldspecial/campdefendmanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"

CampManager::CampManager()
	: m_load_flag(false), m_next_save_time(0)
{
	for (int i = CAMP_TYPE_INVALID; i < CAMP_TYPE_MAX; i++)
	{
		m_camp_list[i].SetCampType(i);
		m_camp_list[i].SetCampManager(this);
	}
}

CampManager::~CampManager()
{
}

CampManager & CampManager::Instance()
{
	static CampManager umm;
	return umm;
}

bool CampManager::OnServerStart()
{
	return this->Load();
}

bool CampManager::Load()
{
	RMIInitCampBackObjectImpl *impl = new RMIInitCampBackObjectImpl();

	RMIGlobalClient ac;
	ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return ac.InitCampAsyn(impl);
}

void RMIInitCampBackObjectImpl::InitCampRet(int ret, const CampParam &camp_param)
{
	CampManager::Instance().LoadRet(ret, camp_param);
}

void CampManager::LoadRet(int ret, const CampParam &camp_param)
{
	if (0 != ret)
	{
		printf("CampManager::Init Failed \n");
		ServerLogic::GetInstServerLogic()->StopServer();
		return;
	}

	for (int i = CAMP_TYPE_INVALID; i < camp_param.count && i < CAMP_TYPE_MAX; ++i)
	{
		Camp *camp = this->GetCamp(camp_param.camp_list[i].base_info.camp_type);
		if (NULL != camp) 
		{
			camp->Init(camp_param.camp_list[i]);
		}
	}

	m_next_save_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_load_flag = true;
}

void CampManager::Update(unsigned long interval, time_t now_second)
{
	if (!m_load_flag) return;

	if (now_second > m_next_save_time)
	{
		this->CalCampCabality();
		this->Save();
		m_next_save_time = now_second + CAMP_SAVE_INTERVAL;
	}
}

void CampManager::OnUserLogin(Role *user)
{
 	this->OnGetCampInfo(user);
}

void CampManager::OnRoleChangeCamp(int from_camp, int to_camp)
{
	if (from_camp <= CAMP_TYPE_INVALID || from_camp >= CAMP_TYPE_MAX) return;
	if (to_camp <= CAMP_TYPE_INVALID || to_camp >= CAMP_TYPE_MAX) return;

	{
		Camp *camp = this->GetCamp(from_camp);
		if (NULL != camp)
		{
			camp->AddRoleCount(-1);
		}
	}

	{
		Camp *camp = this->GetCamp(to_camp);
		if (NULL != camp)
		{
			camp->AddRoleCount(1);
		}
	}
}

void CampManager::OnRoleJoinCamp(int camp_type)
{
	if (camp_type <= CAMP_TYPE_INVALID || camp_type >= CAMP_TYPE_MAX) return;
	
	Camp *camp = this->GetCamp(camp_type);
	if (NULL != camp)
	{
		camp->AddRoleCount(1);
	}
}

Camp * CampManager::GetCamp(int camp)
{
	if (camp <= CAMP_TYPE_INVALID || camp >= CAMP_TYPE_MAX) return NULL;

	return &m_camp_list[camp];
}

void CampManager::OnServerStop()
{
 	this->Save();
}

void CampManager::OnWorldStatusFirstStart()
{

}

void CampManager::Save()
{
	static CampParam camp_param;
	camp_param.count = 0;

	bool is_change = false;

	int count = 0;
	for (int i = CAMP_TYPE_INVALID; i < CAMP_TYPE_MAX; ++i)
	{
		bool change_flag = false;

		m_camp_list[i].GetChangeAttr(&camp_param.camp_list[count], &change_flag);

		if (change_flag) { ++ count; is_change = true; }
	}
	camp_param.count = count;

	if (is_change)
	{
		RMISaveCampBackObjectImpl *impl = new RMISaveCampBackObjectImpl();

		RMIGlobalClient ac;
		ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());

		if (ac.SaveCampAsyn(camp_param, impl)) 
		{
			for (int i = CAMP_TYPE_INVALID; i < CAMP_TYPE_MAX; i++)
			{
				m_camp_list[i].ClearDirtyMark();
			}
		}
	}
}

void CampManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

	this->CalCampCabality();
}

void CampManager::OnGetCampInfo(Role *role)
{
	if (NULL == role) return;

	Protocol::SCCampInfo ci;
	
	ci.camp_item_list[CAMP_TYPE_INVALID].camp_type = CAMP_TYPE_INVALID;
	ci.camp_item_list[CAMP_TYPE_INVALID].camp_role_count = 0;

	for (int camp_type = CAMP_TYPE_INVALID + 1; camp_type < CAMP_TYPE_MAX; ++ camp_type)
	{
		Camp *camp = this->GetCamp(camp_type);
		if (NULL == camp) continue;
		
		ci.camp_item_list[camp_type].camp_type = camp_type;
		ci.camp_item_list[camp_type].camp_role_count = camp->GetRoleCount();
		ci.camp_item_list[camp_type].total_capability = camp->GetTotalCapability();
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ci, sizeof(ci));
}

void CampManager::GetCampTotalCapability(long long capability_list[CAMP_TYPE_MAX])
{
	for (int camp_type = CAMP_TYPE_INVALID + 1; camp_type < CAMP_TYPE_MAX; ++ camp_type)
	{
		Camp *camp = this->GetCamp(camp_type);
		if (NULL == camp) continue;

		capability_list[camp_type] = camp->GetTotalCapability();
	}
}

void CampManager::CalCampCabality()
{
	long long camp_total_capability[CAMP_TYPE_MAX]; memset(camp_total_capability, 0, sizeof(camp_total_capability));

	PersonRank *person_rank = RankManager::Instance().GetPersonRank();
	if (NULL != person_rank)
	{
		time_t now_second = EngineAdapter::Instance().Time();
		const int GET_MAX_ROLE_NUM = 100;
		UserID user_id_list[GET_MAX_ROLE_NUM];
		int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopUser(PERSON_RANK_TYPE_CAPABILITY_ALL, GET_MAX_ROLE_NUM, user_id_list);

		for (int i = 0; i < rank_count; ++ i)
		{
			const PersonRankItem *rank_item = person_rank->GetPersonRankItem(PERSON_RANK_TYPE_CAPABILITY_ALL, user_id_list[i]);
			if (NULL != rank_item)
			{
				if (rank_item->camp > CAMP_TYPE_INVALID && rank_item->camp < CAMP_TYPE_MAX)
				{
					UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(user_id_list[i]);
					if (NULL != user_node )
					{
						if ((now_second <= user_node->last_logout_timestamp + 3 * SECOND_PER_DAY) || UserCacheManager::Instance().IsUserOnline(user_id_list[i]))
						{
							camp_total_capability[(int)rank_item->camp] += rank_item->rank_value;
						}
					}
				}
			}
		}
	}

	for (int i = CAMP_TYPE_INVALID + 1; i < CAMP_TYPE_MAX; ++ i)
	{
		long long capability = camp_total_capability[i] / 100;
		m_camp_list[i].SetTotalCapability(capability);
	}
}

void CampManager::CalcCampRank3(int camp)
{
	if (camp <= CAMP_TYPE_INVALID || camp >= CAMP_TYPE_MAX)
	{
		return;
	}

	WorldStatusCommonDataParam &world_common_p = WorldStatus::Instance().GetCommonData();

	int old_camp_toprank3_uid[MAX_CAMP_RECORD_RANK3_NUM] = {0};
	UNSTD_STATIC_CHECK(sizeof(old_camp_toprank3_uid) == sizeof(world_common_p.camp_toprank3_uid));
	memcpy(old_camp_toprank3_uid, world_common_p.camp_toprank3_uid, sizeof(old_camp_toprank3_uid));
	
	// 清理数据
	{
		for (int i = 0; i < MAX_CAMP_RECORD_RANK3_NUM; ++ i)
		{
			if (MAX_CAMP_TOPRANK3 > 0 && (i / MAX_CAMP_TOPRANK3) + 1 == camp)
			{
				world_common_p.camp_toprank3_uid[i] = 0;
			}
		}
	}
	
	// 移除旧的
	{
		for (int i = 0; i < MAX_CAMP_RECORD_RANK3_NUM; ++ i)
		{
			if (MAX_CAMP_TOPRANK3 > 0 && (i / MAX_CAMP_TOPRANK3) + 1 == camp &&
				old_camp_toprank3_uid[i] > 0)
			{
				Role *old_topranker3 = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_camp_toprank3_uid[i]));
				this->CheckRoleRank3Reward(old_topranker3);
			}
		}
	}

	// 计算排名
	{
		UserID user_id_list[MAX_CAMP_TOPRANK3];
		int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopUser(PERSON_RANK_TYPE_CAPABILITY_ALL, MAX_CAMP_TOPRANK3, user_id_list, camp);

		for (int k = 0; k < MAX_CAMP_TOPRANK3 && k < rank_count; ++ k)
		{
			int index = (camp - 1) * MAX_CAMP_TOPRANK3 + k;
			if (index >= 0 && index < MAX_CAMP_RECORD_RANK3_NUM)
			{
				world_common_p.camp_toprank3_uid[index] = UidToInt(user_id_list[k]);
			}
		}
	}
	
	// 重算新的奖励
	{
		for (int i = 0; i < MAX_CAMP_RECORD_RANK3_NUM; ++ i)
		{
			if (MAX_CAMP_TOPRANK3 > 0 && (i / MAX_CAMP_TOPRANK3) + 1 == camp &&
				world_common_p.camp_toprank3_uid[i] > 0)
			{
				Role *new_topranker3 = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(world_common_p.camp_toprank3_uid[i]));
				this->CheckRoleRank3Reward(new_topranker3);
			}
		}
	}
}

void CampManager::CheckRoleRank3Reward(Role *role)
{
	if (NULL != role)
	{
		WorldStatus::Instance().SynOtherTitleInfo(role, SYNC_REASON_RANK);
	}
}
