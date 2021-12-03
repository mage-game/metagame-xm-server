#include "teaminfo.hpp"

#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "obj/character/role.h"
#include "scene/scene.h"
#include "globalconfig/globalconfig.h"
#include "protocal/msgscene.h"
#include "protocal/msgrole.h"
#include "engineadapter.h"

#include "effect/skilleffect/effectbuff.hpp"

#include "global/team/teammanager.hpp"
#include "global/team/team.hpp"

#include "world.h"

#include "other/event/eventhandler.hpp"
#include "other/capability/capability.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/cross/rolecross.h"

TeamInfo::TeamInfo()
	: m_role(NULL), m_team_index(0), m_team_seq(0), m_last_call_together_time(0), m_last_call_in_time(0)
{

}

TeamInfo::~TeamInfo()
{

}

void TeamInfo::Init(Role *role, int team_index, int team_seq, char is_auto_join_team)
{
	m_role = role;

	m_team_index = team_index;
	m_team_seq = team_seq;
	m_is_auto_join_team = is_auto_join_team;
}

void TeamInfo::GetInitParam(int *team_index_p, int *team_seq_p, char *is_auto_join_team)
{
	*team_index_p = m_team_index;
	*team_seq_p = m_team_seq;
	*is_auto_join_team = m_is_auto_join_team;
}

void TeamInfo::OnSelfLeaveTeam() // 我离开队伍
{
	EventHandler::Instance().OnLeaveTeam(m_role);

	m_team_index = -1; m_team_seq = -1;

	this->ReCalcFriendBuff();

	m_role->GetRoleCross()->OnRoleLeaveTeam();
}

void TeamInfo::OnSelfJoinTeam(int index, int seq) // 我加入队伍		
{
	m_team_index = index; m_team_seq = seq;

	EventHandler::Instance().OnJoinTeam(m_role);

	this->SendTeamMemberPosToMe(); 
	this->SendMyPosToMember();

	this->ReCalcFriendBuff();
}

bool TeamInfo::IsMember()
{
	if (!this->InTeam())
	{
		return false;
	}

	Team *team = this->GetMyTeam();
	if (NULL == team)
	{
		return false;
	}

	Team::TeamMember *member = team->GetTeamMember(m_role->GetUserId());
	if (NULL == member)
	{
		return false;
	}

	return true;
}

bool TeamInfo::IsMember(const UserID &user_id)
{
	if (!this->InTeam())
	{
		return false;
	}

	Team *team = this->GetMyTeam();
	if (NULL == team)
	{
		return false;
	}

	Team::TeamMember *member = team->GetTeamMember(user_id);
	if (NULL == member)
	{
		return false;
	}

	return true;
}

bool TeamInfo::IsLeader()
{
	if (!this->InTeam())
	{
		return false;
	}

	Team *team = this->GetMyTeam();
	if (NULL == team)
	{
		return false;
	}
	
	return team->IsLeader(m_role->GetUserId());
}

int TeamInfo::GetOtherMemberInTeam(Role **member_list, int max_list_size)  // 取出在周围的队友
{
	if (!this->InTeam()) 
	{
		return 0;
	}

	Team *team = this->GetMyTeam();
	if (NULL == team)
	{
		return 0;
	}

	Scene *scene = m_role->GetScene();
	if (NULL == scene)
	{
		return 0;
	}

	int scene_id = scene->GetSceneID();
	int scene_key = scene->GetSceneKey();
	Posi my_pos = m_role->GetPos();
	UserID my_user_id = m_role->GetUserId();

	const Axis MAX_DELTA_X = GLOBALCONFIG->GetRoleInTeamDistance().x;
	const Axis MAX_DELTA_Y = GLOBALCONFIG->GetRoleInTeamDistance().y;

	int count = 0;
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM && count < max_list_size; ++i)
	{
		Team::TeamMember *team_member = team->GetTeamMember(i);
		if (NULL == team_member)
		{
			continue;
		}

		Obj *member_obj = scene->GetObj(team_member->obj_id);
		if (NULL != member_obj && Obj::OBJ_TYPE_ROLE == member_obj->GetObjType())
		{
			Role *teammember_role = (Role*)member_obj;
			if (teammember_role->GetUserId() == team_member->user_id)
			{
				if (team_member->scene_id == scene_id && team_member->scene_key == scene_key && my_user_id != team_member->user_id)
				{
					Posi pos = teammember_role->GetPos();
					Posi delta_pos = my_pos - pos;
					if (delta_pos.x < MAX_DELTA_X && delta_pos.y < MAX_DELTA_Y && delta_pos.x > -MAX_DELTA_X && delta_pos.y > -MAX_DELTA_Y)
					{
						member_list[count ++] = teammember_role;
					}
				}
			}
		}
	}

	return count;
}

int TeamInfo::GetOtherMemberInTheSameScene(Role **member_list, int max_list_size)  // 取出在同一个场景的队友
{
	if (!this->InTeam()) 
	{
		return 0;
	}

	Team *team = this->GetMyTeam();
	if (NULL == team)
	{
		return 0;
	}

	Scene *scene = m_role->GetScene();
	if (NULL == scene)
	{
		return 0;
	}

	int scene_id = scene->GetSceneID();
	int scene_key = scene->GetSceneKey();
	UserID my_user_id = m_role->GetUserId();

	int count = 0;
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM && count < max_list_size; ++i)
	{
		Team::TeamMember *team_member = team->GetTeamMember(i);
		if (NULL == team_member)
		{
			continue;
		}

		Obj *member_obj = scene->GetObj(team_member->obj_id);
		if (NULL != member_obj && Obj::OBJ_TYPE_ROLE == member_obj->GetObjType())
		{
			Role *teammember_role = (Role*)member_obj;
			if (teammember_role->GetUserId() == team_member->user_id)
			{
				if (team_member->scene_id == scene_id && team_member->scene_key == scene_key && my_user_id != team_member->user_id)
				{
					member_list[count ++] = teammember_role;
				}
			}
		}
	}

	return count;
}

int TeamInfo::GetOtherMemberList(Role **member_list, int max_list_size)
{
	if (!this->InTeam())
	{
		return 0;
	}

	Team *team = this->GetMyTeam();
	if (NULL == team)
	{
		return 0;
	}

	Scene *scene = m_role->GetScene();
	if (NULL == scene)
	{
		return 0;
	}

	UserID my_user_id = m_role->GetUserId();

	int count = 0;
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM && count < max_list_size; ++i)
	{
		Team::TeamMember *team_member = team->GetTeamMember(i);
		if (NULL == team_member) continue;

		Role *teammember_role = World::GetInstWorld()->GetSceneManager()->GetRole(team_member->user_id);
		if (teammember_role != nullptr && my_user_id != team_member->user_id)
		{
			member_list[count++] = teammember_role;
		}
	}

	return count;
}

void TeamInfo::SendMyPosToMember(bool is_leave_scene) // 把我的位置发给别人 
{
// 	if (!this->InTeam()) 
// 	{
// 		return;
// 	}
// 
// 	Role *member_list[16] = {NULL};
// 	int count = this->GetOtherMemberInTheSameScene(member_list, 16);
// 	if (count > 0) 
// 	{
// 		Posi pos = m_role->GetPos();
// 
// 		Protocol::SCTeamMemberPos tmp;
// 		tmp.member_pos.role_id = UidToInt(m_role->GetUserId());
// 		tmp.member_pos.obj_id = m_role->GetId();
// 		tmp.member_pos.reserved = 0;
// 		tmp.member_pos.is_leave_scene = (is_leave_scene ? 1 : 0);
// 		tmp.member_pos.pos_x = pos.x;
// 		tmp.member_pos.pos_y = pos.y;
// 		tmp.member_pos.dir = m_role->GetDir();
// 		tmp.member_pos.distance = m_role->GetDirDistance();
// 		tmp.member_pos.move_speed = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
// 
// 		GSNetID team_member_netid_list[16];
// 		for (int i = 0; i < count; ++i)
// 		{
// 			team_member_netid_list[i] = member_list[i]->GetNetId();
// 		}
// 		EngineAdapter::Instance().NetMulSend(team_member_netid_list, count, (const char *)&tmp, sizeof(tmp));
// 	}
}

void TeamInfo::SendTeamMemberPosToMe()
{
//  	if (!this->InTeam()) 
//  	{
//  		return;
//  	}
//  
//  	Role *member_list[16] = {NULL};
//  	int count = this->GetOtherMemberInTheSameScene(member_list, 16);
//  	if (count >= 0 && count < MAX_TEAM_MEMBER_NUM) 
//  	{
// 		Protocol::SCTeamMemberPosList sctmpl;
// 		sctmpl.member_count = count;
//  		GSNetID netid = m_role->GetNetId();
//  		for (int i = 0; i < count; ++i)
//  		{
// 			Protocol::SCTeamMemberPosItem &member_item = sctmpl.member_list[i];
//  			Role *role = member_list[i];
//  			if (NULL == role) continue;
//  
//  			Posi pos = role->GetPos();
//  
//  			member_item.role_id = UidToInt(role->GetUserId());
//  			member_item.obj_id = role->GetId();
//  			member_item.reserved = 0;
//  			member_item.is_leave_scene = 0;
//  			member_item.pos_x = pos.x;
//  			member_item.pos_y = pos.y;
//  			member_item.dir = role->GetDir();
//  			member_item.distance = role->GetDirDistance();
//  			member_item.move_speed = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
//  		}
// 		int len = sizeof(sctmpl) - sizeof(sctmpl.member_list[0]) * (MAX_TEAM_MEMBER_NUM - count);
// 		EngineAdapter::Instance().NetSend(netid, (const char *)&sctmpl, len);
//  	}
}

void TeamInfo::OnTeamCallTogether()
{
	time_t now_second = EngineAdapter::Instance().Time();
	if (now_second < m_last_call_together_time + TEAM_CALL_TOGETHER_TIME_LIMIT_INTERVAL_S)
	{
		return;
	}

	if (!this->InTeam()) return;

	Team *team = this->GetMyTeam();
	if (NULL == team)
	{
		return;
	}

	if (team->IsLeader(m_role->GetUserId()))
	{
		m_role->NoticeNum(errornum::EN_YOU_HAVE_NO_POWER);
		return;
	}

	if (m_role->GetRoleStatusManager()->IsInFB())
	{
		m_role->NoticeNum(errornum::EN_TEAM_CALL_TOGETHER_LIMIT_IN_FB);
		return;
	}

	Protocol::SCTeamCallTogether tct;
	tct.scene_id = m_role->GetScene()->GetSceneID();
	tct.pos_x = static_cast<short>(m_role->GetPos().x);
	tct.pos_y = static_cast<short>(m_role->GetPos().y);

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		Team::TeamMember *tmb = team->GetTeamMember(i);
		if (NULL == tmb)
		{
			continue;
		}

		if (INVALID_USER_ID != tmb->user_id && m_role->GetUserId() != tmb->user_id)
		{
			Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(tmb->user_id);
			if (NULL != tmp_role)
			{
				EngineAdapter::Instance().NetSend(tmp_role->GetNetId(), (const char *)&tct, sizeof(tct));
			}
		}
	}

	m_role->NoticeNum(noticenum::NT_TEAM_CALL_TOGETHER_SUCC);

	m_last_call_together_time = now_second;
}

void TeamInfo::OnTeamCallIn(int fb_id)
{
	time_t now_second = EngineAdapter::Instance().Time();
	if (now_second < m_last_call_in_time + TEAM_CALL_IN_TIME_LIMIT_INTERVAL_S)
	{
		return;
	}

	if (!this->InTeam()) return;

	Team *team = this->GetMyTeam();
	if (NULL == team)
	{
		return;
	}

	if (team->IsLeader(m_role->GetUserId()))
	{
		m_role->NoticeNum(errornum::EN_YOU_HAVE_NO_POWER);
		return;
	}

	if (m_role->GetRoleStatusManager()->IsInFB())
	{
		m_role->NoticeNum(errornum::EN_TEAM_CALL_IN_LIMIT_IN_FB);
		return;
	}

	static Protocol::SCTeamCallIn tci;
	tci.team_index = m_team_index;
	tci.role_id = UidToInt(m_role->GetUserId());
	m_role->GetName(tci.role_name);
	tci.camp = static_cast<short>(m_role->GetCamp());
	tci.level = static_cast<short>(m_role->GetLevel());
	tci.capability = m_role->GetCapability()->GetTotalCapability();
	tci.limit_capability = team->GetLimitCapability();
	tci.limit_level = team->GetLimitLevel();
	tci.fb_id = fb_id;
	World::GetInstWorld()->SendToAllGateway((const char *)&tci, sizeof(tci));
	
	m_last_call_in_time = now_second;
}

void TeamInfo::ReCalcFriendBuff()
{
	//Team *team = this->GetMyTeam();
	//if (NULL == team)
	//{
	//	return;
	//}

	//Scene *scene = m_role->GetScene();
	//if (NULL == scene)
	//{
	//	return;
	//}
	//static const int FRIEND_BUFF_SKILL_ID = 1;

	//m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_FRIEND, FRIEND_BUFF_SKILL_ID);

	//int my_sceneid = m_role->GetScene()->GetSceneID();

	//int max_intimacy = 0;
	//for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	//{
	//	Team::TeamMember *tmm = team->GetTeamMember(i);
	//	if (NULL == tmm)
	//	{
	//		continue;
	//	}

	//	if (tmm->user_id == m_role->GetUserId())
	//	{
	//		continue;
	//	}

	//	Obj *member_obj = scene->GetObj(tmm->obj_id);
	//	if (NULL != member_obj && Obj::OBJ_TYPE_ROLE == member_obj->GetObjType())
	//	{
	//		Role *tmp_role = (Role*)member_obj;
	//		if (tmp_role->GetUserId() == tmm->user_id)
	//		{
	//			if (tmm->scene_id == my_sceneid && tmm->scene_index == m_role->GetScene()->GetIndex())
	//			{
	//				int intimacy = m_role->GetFriendManager()->GetFriendIntimacy(tmm->user_id);
	//				if (intimacy > max_intimacy)
	//				{
	//					max_intimacy = intimacy;
	//				}
	//			}
	//		}
	//	}
	//}

	//if (max_intimacy > 0)
	//{
	//	CharIntAttrs attrs_add;
	//	if (GLOBALCONFIG->GetFriendAttrList(max_intimacy, attrs_add))
	//	{
	//		EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, FRIEND_BUFF_SKILL_ID, 3600 * 24 * 1000, 1, EffectBase::PRODUCT_METHOD_FRIEND);
	//		for (int i = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BEGIN; i < CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAX; i++)
	//		{
	//			if (attrs_add.m_attrs[i] > 0)
	//			{
	//				effect_buff->AddAttrEffect(i, attrs_add.m_attrs[i]);
	//			}
	//		}

	//		effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	//		effect_buff->SetSave(false);

	//		m_role->AddEffect(effect_buff);
	//	}
	//}
}

bool TeamInfo::InTeam()
{
	return m_team_index > 0 && m_team_seq > 0;
}

Team * TeamInfo::GetMyTeam()
{
	Team *team = TeamManager::Instance().GetTeam(m_team_index);
	if (NULL == team)
	{
		return NULL;
	}

	if (team->GetTeamSeq() != m_team_seq)
	{
		return NULL;
	}

	return team;
}

int TeamInfo::GetMyTeamAssignMode()
{
	Team *team = this->GetMyTeam();
	if (NULL == team)
	{
		return TEAM_ASSIGN_MODE_ATTACK;
	}

	return team->GetAssignMode();
}

void TeamInfo::SyncRoleTeamInfo()
{
	static Protocol::SCRoleTeamInfo cmd;
	
	cmd.is_auto_apply_join_team = m_is_auto_join_team;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}



