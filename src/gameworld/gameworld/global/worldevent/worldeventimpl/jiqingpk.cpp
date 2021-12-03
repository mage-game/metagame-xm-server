#include "jiqingpk.hpp"

#include "obj/character/role.h"
#include "world.h"
#include "scene/scene.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "servercommon/serverconfig/localconfig.hpp"

#include "config/logicconfigmanager.hpp"
#include "global/worldevent/worldeventconfig.hpp"
#include "global/rank/rankmanager.hpp"
#include "global/challengefield/challengeuser.hpp"
#include "global/challengefield/challengefield.hpp"
#include "obj/character/roleshadow.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/worldevent/roleworldevent.hpp"

WorldEventJiqingPk::WorldEventJiqingPk(WorldEventManager *world_event_manager)
	: WorldEvent(world_event_manager, WORLD_EVENT_TYPE_JIQINGPK), m_pkrole_count(0), m_flush_num(0), m_last_check_alive_pkrole_timestamp(0)
{

}

WorldEventJiqingPk::~WorldEventJiqingPk()
{

}

void WorldEventJiqingPk::Init(const WorldEventParam &ev_param)
{
	m_score = ev_param.score_list[m_event_type];
	m_flush_num = ev_param.jiqingpk_flush_num;
}

void WorldEventJiqingPk::GetInitParam(WorldEventParam *ev_param)
{
	ev_param->score_list[m_event_type] = m_score;
	ev_param->jiqingpk_flush_num = m_flush_num;
}

void WorldEventJiqingPk::Update(unsigned long interval, time_t now_second)
{
	if (now_second >= m_last_check_alive_pkrole_timestamp + 3)
	{
		m_last_check_alive_pkrole_timestamp = now_second;

		bool has_alive_pkrole = false;
		{
			for (int i = 0; i < m_pkrole_count; i++)
			{
				PKRoleItem &pkrole_item = m_pkrole_list[i];
				if (pkrole_item.IsVaild())
				{
					Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(pkrole_item.scene_id, 0);
					if (NULL != scene)
					{
						Obj *obj = scene->GetObj(pkrole_item.objid);
						if (NULL == obj || Obj::OBJ_TYPE_ROLE_SHADOW != obj->GetObjType())
						{
							pkrole_item.Reset();
							continue;
						}

						RoleShadow *role_shadow = (RoleShadow*)obj;
						if (ROLE_SHADOW_THPE_WORLD_EVENT != role_shadow->GetRoleShadowType()) 
						{
							pkrole_item.Reset();
							continue;
						}

						has_alive_pkrole = true;
						break;
					}
				}
			}
		}
		if (!has_alive_pkrole)
		{
			this->ForceEndLogicState();
		}
	}
}

void WorldEventJiqingPk::OnHurtRoleShadow(Role *role, RoleShadow *role_shadow, int hurt)
{
	if (NULL == role || NULL == role_shadow || hurt <= 0)
	{
		return;
	}

	if (ROLE_SHADOW_THPE_WORLD_EVENT == role_shadow->GetRoleShadowType()) 
	{
		role->GetRoleWorldEvent()->AddScore(m_event_type, hurt);
	}
}

void WorldEventJiqingPk::OnLogicStart()
{
	this->ClearPKRole();

	this->FlushPKRole();
}

void WorldEventJiqingPk::OnLogicStop()
{
	this->ClearPKRole();
}

void WorldEventJiqingPk::ClearPKRole()
{
	for (int i = 0; i < m_pkrole_count; i++)
	{
		PKRoleItem &pkrole_item = m_pkrole_list[i];
		if (pkrole_item.IsVaild())
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(pkrole_item.scene_id, 0);
			if (NULL != scene)
			{
				Obj *obj = scene->GetObj(pkrole_item.objid);
				if (NULL != obj && Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
				{
					RoleShadow *role_shadow = (RoleShadow*)obj;
					if (ROLE_SHADOW_THPE_WORLD_EVENT == role_shadow->GetRoleShadowType()) 
					{
						scene->DeleteObj(role_shadow->GetId());
					}
				}
			}

			pkrole_item.Reset();
		}
	}

	m_pkrole_count = 0;
}

void WorldEventJiqingPk::FlushPKRole()
{
	m_pkrole_count = 0;

	JiqingPKConfig jiqingpk_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetJiqingPKConfig();
	jiqingpk_cfg.RandPos();

	for (int i = 0; i < WORLD_EVENT_PKROLE_PER_FLUSH_NUM && m_pkrole_count < WORLD_EVENT_PKROLE_MAX; i++)
	{
		JiqingPKConfig::FlushPosCfgItem &poscfg_item = jiqingpk_cfg.pos_cfg_list[i];
		if (poscfg_item.IsVaild())
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(poscfg_item.scene_id, 0);
			if (NULL != scene)
			{
				int pkrole_cap = 0, pkrole_uid = 0; 
				if (this->GetPkRoleInfo(&pkrole_cap, &pkrole_uid))
				{
					UniqueUserID unique_user_id(LocalConfig::Instance().GetPlatType(), IntToUid(pkrole_uid));
					ChallengeUser *challenge_user = ChallengeField::Instance().GetChallengeUser(unique_user_id);
					if (NULL == challenge_user) // 如果排行榜上的人还没有竞技场数据，就取竞技场的第一名来代替
					{
						long long challengefield_king = WorldStatus::Instance().GetChallengeFieldKingUserId();

						challenge_user = ChallengeField::Instance().GetChallengeUser(LongLongToUniqueUserID(challengefield_king));
					}

					if (NULL != challenge_user)
					{
						RoleShadow *role_shadow = new RoleShadow(ROLE_SHADOW_THPE_WORLD_EVENT);
						ChallengeParam param = challenge_user->GetChallengeParam();
						RoleAppearance appearance = challenge_user->GetChallengeAppearance();

						param.gongji = static_cast<Attribute>(pkrole_cap / 8.0f) ;
						param.max_hp = param.gongji * 500;
						param.fangyu = static_cast<Attribute>(param.gongji * 0.8f);
						param.mingzhong = 1000000;
						param.shanbi = 1;
						param.baoji = 1;
						param.jianren = 1000000;

						role_shadow->Init(param, appearance, 0, 0);
						role_shadow->SetPos(poscfg_item.pos);
						role_shadow->SetBirthPos(poscfg_item.pos);
						scene->AddObj(role_shadow);

						m_pkrole_list[m_pkrole_count].scene_id = role_shadow->GetScene()->GetSceneID();
						m_pkrole_list[m_pkrole_count].objid = role_shadow->GetId();
						m_pkrole_list[m_pkrole_count].pkrole_id= UidToInt(role_shadow->GetUserId());
						++ m_pkrole_count;

						m_flush_num = (m_flush_num + 1) % WORLD_EVENT_PKROLE_MAX_RANKPOS;
					}
				}
			}
		}
	}
}

bool WorldEventJiqingPk::GetPkRoleInfo(int *cap, int *uid)
{															
	UserID user_id_list[WORLD_EVENT_PKROLE_MAX_RANKPOS];	// 取前12出来 	

	int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopUser(PERSON_RANK_TYPE_CAPABILITY_ALL, 
		sizeof(user_id_list) / sizeof(user_id_list[0]), user_id_list);

	int get_rankpos = WORLD_EVENT_PKROLE_MAX_RANKPOS - m_flush_num;  // 当前位置 = 最后那个位置 - 已经刷了几个 (12 - 0; 12-11)
	if (get_rankpos >= rank_count) // 排行榜没有那么多人 只能取最后一个了 
	{
		get_rankpos = rank_count;
	}

	if (get_rankpos > 0 && get_rankpos <= WORLD_EVENT_PKROLE_MAX_RANKPOS)
	{
		UserID pkrole_user_id = user_id_list[get_rankpos - 1];

		const PersonRankItem *rankitem = RankManager::Instance().GetPersonRank()->GetPersonRankItem(PERSON_RANK_TYPE_CAPABILITY_ALL, 
			pkrole_user_id);
		if (NULL != rankitem)
		{
			if (NULL != cap) *cap = static_cast<int>(rankitem->rank_value);
			if (NULL != uid) *uid = UidToInt(pkrole_user_id);
			return true;
		}
	}	

	return false;
}




