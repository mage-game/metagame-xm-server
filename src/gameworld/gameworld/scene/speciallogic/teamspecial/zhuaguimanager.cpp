#include "zhuaguimanager.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/zhuaguiconfig.hpp"

#include "world.h"
#include "scene/scene.h"

#include "obj/character/role.h"
#include "obj/otherobj/worldeventobj.h"

#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/commondata.hpp"

#include "global/team/team.hpp"
#include "friend/teaminfo.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "protocal/msgrole.h"

ZhuaGuiMgr & ZhuaGuiMgr::Instance()
{
	static ZhuaGuiMgr mgr;
	return mgr;
}

ZhuaGuiMgr::ZhuaGuiMgr()
	: m_first(true), m_zhuagui_scene_key_inc(0)
{

}

ZhuaGuiMgr::~ZhuaGuiMgr()
{

}

void ZhuaGuiMgr::Update(unsigned long interval, time_t now_second)
{
	if (m_first)
	{
		m_first = false;

		for (int i = 0; i < ZHUAGUI_SCENE_COUNT; i++)
		{
			ZhuaGuiNpcSceneCfg *cfg = LOGIC_CONFIG->GetZhuaGuiConfig().GetZhuaGuiNpcSceneCfgByIdx(i);
			if (NULL != cfg)
			{
				std::vector<NpcObjReg> npcobj_set;
				m_scene_npc_map[cfg->scene_id] = npcobj_set;
			}
		}
	}

	for (SceneNpcMapIt it = m_scene_npc_map.begin(); it != m_scene_npc_map.end(); ++ it)
	{
		if (LOGIC_CONFIG->GetZhuaGuiConfig().IsNeedFlush(it->first, (int)it->second.size()))
		{
			this->FlushNpcHelper(it->first);
		}
	}
}

void ZhuaGuiMgr::OnRoleLogin(Role *role)
{
	this->SyncZhuaGuiInfo(role);
}

void ZhuaGuiMgr::OnTouchNpc(Role *role, int scene_id, ObjID objid)
{
	if (role == nullptr)
	{
		return;
	}

	TeamInfo *team_info = role->GetTeamInfo();
	if (NULL == team_info)
	{
		role->NoticeNum(errornum::EN_FB_NOT_IN_TEAM);
		return;
	}

	if (!team_info->InTeam())
	{
		role->NoticeNum(errornum::EN_FB_NOT_IN_TEAM);
		return;
	}

	Team *team = team_info->GetMyTeam();
	if (NULL == team)
	{
		return;
	}

	if (!team->IsLeader(role->GetUserId()))
	{
		role->NoticeNum(errornum::EN_MARRY_RESERVE_MUST_BE_TEAM_LEADER);
		return;
	}

	int join_user_list[MAX_TEAM_MEMBER_NUM]; memset(join_user_list, 0, sizeof(join_user_list));

	int join_user_count = team->GetMemberUidList(join_user_list, MAX_TEAM_MEMBER_NUM);
	if (join_user_count <= 1)
	{
		role->NoticeNum(errornum::EN_TEAM_MUST_HAVE_MEMEBER);
		return;
	}

	// 检查角色进入条件是否满足
	for (int i = 0; i < join_user_count; ++ i)
	{
		Role *join_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(join_user_list[i]));
		if (NULL == join_role)
		{
			role->NoticeNum(errornum::EN_TEAM_TOWERDEFEND_PARTNER_NOT_ONLINE);
			return;
		}

		// 等级检查
		if (!LOGIC_CONFIG->GetZhuaGuiConfig().CanEnterZhuaguFb(join_role->GetLevel()))
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_team_towerdefend_teammate_level_limit,
				UidToInt(join_role->GetUserId()), join_role->GetName(), (int)join_role->GetCamp());
			if (length > 0)
			{
				static Protocol::SCSystemMsg sm;
				int sendlen = 0;
				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3))
				{
					team->BroadcastToAllMember((const char*)&sm, sendlen);
				}
			}

			return;
		}

		// 状态检查
		if (!join_role->GetRoleStatusManager()->CanEnterFB(true, false))
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_team_towerdefend_teammate_can_not_enter,
				UidToInt(join_role->GetUserId()), join_role->GetName(), (int)join_role->GetCamp());
			if (length > 0)
			{
				static Protocol::SCSystemMsg sm;
				int sendlen = 0;
				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3))
				{
					team->BroadcastToAllMember((const char*)&sm, sendlen);
				}
			}

			return;
		}
	}

	// 队员进入
	int zhuagui_scene_id = 0;
	Posi enter_pos(0, 0);
	if (LOGIC_CONFIG->GetZhuaGuiConfig().GetEnterFbInfo(&zhuagui_scene_id, &enter_pos))
	{
		int zhuagui_scenekey = this->GetZhuaGuiSceneKey();

		for (int i = 0; i < join_user_count; ++ i)
		{
			Role *join_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(join_user_list[i]));
			if (NULL != join_role)
			{
				bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(join_role, zhuagui_scene_id, zhuagui_scenekey, enter_pos);
				if (!ret)
				{
					join_role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
				}

				join_role = NULL;   // 至此 所有的角色已经失效 不要再进行数据操作！
			}	
		}

		this->OnRoleEnterZhuaguiFB(scene_id, objid);
	}
}

void ZhuaGuiMgr::OnRoleEnterZhuaguiFB(int scene_id, ObjID objid)
{
	SceneNpcMapIt it = m_scene_npc_map.find(scene_id);
	if (it != m_scene_npc_map.end())
	{
		for (std::vector<NpcObjReg>::iterator s_it = it->second.begin(); s_it != it->second.end(); ++ s_it)
		{
			NpcObjReg &npcobj = *s_it;
			if (npcobj.objid == objid)
			{
				Obj *obj = World::GetInstWorld()->GetSceneManager()->GetObj(npcobj.scene_idx, npcobj.objid);
				if (NULL != obj && Obj::OBJ_TYPE_WORLDEVENT_OBJ == obj->GetObjType())
				{
					Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneByIndex(npcobj.scene_idx);
					if (NULL != scene)
					{
						scene->DeleteObj(obj->GetId());
					}
				}

				it->second.erase(s_it);

				break;
			}
		}

		if (LOGIC_CONFIG->GetZhuaGuiConfig().IsNeedFlush(it->first, (int)it->second.size()))
		{
			this->FlushNpcHelper(it->first);
		}
	}
}

void ZhuaGuiMgr::FlushNpcHelper(int scene_id)
{
	SceneNpcMapIt it = m_scene_npc_map.find(scene_id);
	if (it != m_scene_npc_map.end())
	{
		for (std::vector<NpcObjReg>::iterator s_it = it->second.begin(); s_it != it->second.end(); ++ s_it)
		{
			NpcObjReg &npcobj = *s_it;

			Obj *obj = World::GetInstWorld()->GetSceneManager()->GetObj(npcobj.scene_idx, npcobj.objid);
			if (NULL != obj && Obj::OBJ_TYPE_WORLDEVENT_OBJ == obj->GetObjType())
			{
				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneByIndex(npcobj.scene_idx);
				if (NULL != scene)
				{
					scene->DeleteObj(obj->GetId());
				}
			}
		}

		it->second.clear();

		{
			int flushnpc_count = 0;
			Posi randpos_list[ZHUAGUI_SCENE_POS_COUNT]; memset(randpos_list, 0, sizeof(randpos_list));

			if (LOGIC_CONFIG->GetZhuaGuiConfig().GetRandomFlushInfo(scene_id, &flushnpc_count, randpos_list))
			{
				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
				if (NULL != scene)
				{
					for (int i = 0; i < flushnpc_count; i++)
					{
						WorldEventObj *npcev_obj = new WorldEventObj();

						npcev_obj->Init(0, WORLD_EVENT_OBJ_ZHUAGUI_NPC, 0, 1, 10, 0);
						npcev_obj->SetPos(randpos_list[i]);
						scene->AddObj(npcev_obj);

						{
							NpcObjReg npcobj_reg;
							npcobj_reg.scene_id = scene_id;
							npcobj_reg.flush_pos = randpos_list[i];
							npcobj_reg.scene_idx = scene->GetIndex();
							npcobj_reg.objid = npcev_obj->GetId();
							it->second.push_back(npcobj_reg);
						}
					}
				}
			}
		}
	}
}

void ZhuaGuiMgr::SyncZhuaGuiInfo(Role *role)
{
	CommonDataParam *common_p = role->GetCommonDataParam();
	
	static Protocol::SCZhuaGuiRoleInfo zgri;
	zgri.zhuagui_day_gethunli = common_p->zhuagui_day_gethunli;
	zgri.zhuagui_day_catch_count = common_p->zhuagui_day_catch_count;
	zgri.zhuagui_hunli = role->GetHunli();
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&zgri, sizeof(zgri));
}

