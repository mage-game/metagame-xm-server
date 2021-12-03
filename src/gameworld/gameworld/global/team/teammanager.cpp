#include "teammanager.hpp"
#include "team.hpp"

#include "engineadapter.h"

#include "world.h"
#include "scene/scene.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/string/gameworldstr.h"
#include "protocal/msgchatmsg.h"
#include "chat/chatmanager.h"
#include "other/gm/gm.hpp"
#include "friend/teaminfo.hpp"
#include "other/capability/capability.hpp"
#include "other/cross/rolecross.h"
#include "other/fb/dailyfbconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/fb/teamfbroom.hpp"

TeamManager::TeamManager()
	: m_team_seq_aotu_increment(0)
{
	m_team_list.Insert(new Team()); // 仅仅是为了保证队伍Index从1开始 保证队伍副本的key不为0 
}

TeamManager::~TeamManager()
{
	for (TeamList::Iterator i = m_team_list.Beg(); i != m_team_list.End(); ++i)
	{
		delete *i;
	}

	m_team_list.Clear();
}

TeamManager & TeamManager::Instance()
{
	static TeamManager ws;
	return ws;
}

void TeamManager::SyncPersonBaseInfo(Role *user)
{
	if (NULL == user) return;

	if (user->GetTeamInfo()->InTeam())
	{
		this->OnUserBaseInfoChange(user->GetTeamInfo()->GetTeamIndex(), user->GetUserId());
	}
}

void TeamManager::OnUserLogin(Role *user)
{
	if (NULL == user)
	{
		return;
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		//隐藏服登陆.清除本服的队伍信息
		user->GetTeamInfo()->OnSelfLeaveTeam();
		return;
	}

	int team_index = this->GetTeamIndex(UidToInt(user->GetUserId()), true);
	
	Team *team = this->GetTeam(team_index);
	if (NULL != team)
	{
		if (team->OnUserLogin(user->GetUserId(), user->GetNetId()))
		{
			user->GetTeamInfo()->LoginFixTeamIndexSeq(team_index, team->GetTeamSeq());
		}
	}
}

void TeamManager::OnUserLogout(Role *user)
{
	if (NULL == user)
	{
		return;
	}

	if (user->GetTeamInfo()->InTeam())
	{
		int team_index = user->GetTeamInfo()->GetTeamIndex();

		Team *team = this->GetTeam(team_index);
		if (NULL != team)
		{
			bool is_fb_team_type = false;
			if (TEAM_TYPE_YAOSHOUJITANG == team->GetTeamType() ||			// 无尽祭坛，下线离开队伍，如果是队长，会自动转让队伍
				TEAM_TYPE_TEAM_MIGONGXIANFU == team->GetTeamType()||		// 转职组队副本，同上
				TEAM_TYPE_TEAM_EQUIP_FB == team->GetTeamType() ||			// 组队装备本，同上
				TEAM_TYPE_TEAM_DAILY_FB == team->GetTeamType() ||           // 日常副本，同上
				CrossConfig::Instance().IsHiddenServer())
			{
				is_fb_team_type	= true;
			}

			{  // 经验副本的组队掉线 不离开队伍
				const DailyFbItem *fb_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetDailyFbItemCfg(DAILYFB_TYPE_EXP);
				if (is_fb_team_type && NULL != fb_cfg && nullptr != user->GetScene() && fb_cfg->scene_id == user->GetScene()->GetSceneID())
				{
					is_fb_team_type = false;
				}
			}

			int tl_key = -1;
			int tl_id = team->GetTeamLocal(&tl_key);

			bool is_leader_logout = team->IsLeader(user->GetUserId());

			team->OnUserLogout(user->GetUserId());

			if (team->IsNobodyOnline(user->GetUserId()))
			{
				this->DestroyTeamHelper(team);
				this->RemoveTeamLocalMap(tl_id, tl_key, team->GetTeamIndex());
			}
			else
			{
				if (is_leader_logout)
				{
					this->RemoveTeamLocalMap(tl_id, tl_key, team_index);
					tl_id = team->GetTeamLocal(&tl_key);
					this->AddTeamLocalMap(tl_id, tl_key, team_index);
				}
				
				bool is_activity_special_state = user->GetRoleCross()->IsInCross3v3Matching();
				if (is_fb_team_type || is_activity_special_state) //组队副本里掉线直接离开队伍. 特殊活动状态下掉线离开队伍
				{
					team->RemoveMember(user->GetUserId(), false);
				}
				else
				{
					this->SaveTeamIndex(UidToInt(user->GetUserId()), team_index);
				}
			}
		}
	}
}

int TeamManager::CreateTeam(Role *leader, bool must_check, bool member_can_invite, int assign_mode, int team_type)
{
	if (NULL == leader)
	{
		return TOR_NO_POWER;
	}

	if (assign_mode <= TEAM_ASSIGN_MODE_INVALID || assign_mode >= TEAM_ASSIGN_MODE_MAX) 
	{
		return TOR_INVALID_ASSIGN_MODE;
	}

	if (leader->GetTeamInfo()->InTeam()) 
	{
		return TOR_ALREADY_IN_TEAM;
	}

	Team *team = new Team(leader, must_check, member_can_invite, assign_mode, team_type);

	int index = m_team_list.Insert(team);
	team->SetTeamIndex(index);

	m_team_seq_aotu_increment++;
	team->SetTeamSeq(m_team_seq_aotu_increment);

	leader->GetTeamInfo()->OnSelfJoinTeam(index, m_team_seq_aotu_increment);

	this->AddTeamLocalMap(leader, index);

	team->SendTeamInfo();

	return index;
}

bool TeamManager::DismissTeam(int team_index, Role *executor)
{
	if (NULL == executor)
	{
		return false;
	}

	Team *team = this->GetTeam(team_index);
	if (NULL == team)
	{
		return false;
	}

	if (!team->IsLeader(executor->GetUserId())) 
	{
		return false;
	}

	// 本来这个设计是不错的，防止在副本中踢掉队友，独享奖励
	// 但是现在组队副本队长下线是要解散组队的，这里又不让解散
	// 考虑到第一种情况，不太可能出现，先注释代码
	//if (Scene::IsTeamScene(executor->GetScene()->GetSceneType(), executor->GetScene()->GetSceneID()))
	//{
	//	executor->NoticeNum(errornum::EN_TEAM_TEAM_FB_OPERA_LIMIT);
	//	return false;
	//}

	team->Dismiss();
	
	this->RemoveTeamLocalMap(executor, team_index);
	this->DestroyTeamHelper(team);
	
	return true;
}

int TeamManager::AddTeamMember(Role *executor, int team_index, Role *user, bool is_call_in_ack)
{
	if (NULL == executor || NULL == user)
	{
		return TOR_NO_POWER;
	}

	if (user->GetTeamInfo()->InTeam()) 
	{
		return TOR_ALREADY_IN_TEAM;
	}

	// 检测加入者活动状态
	if (user->GetRoleCross()->IsInCross3v3Matching())
	{
		return TOR_IN_ROLE_MATCHING;
	}

	Team *team = this->GetTeam(team_index);
	if (NULL == team) return TOR_TEAM_NOT_EXIST;

	if (team->IsFull()) return TOR_TEAM_IS_FULL;
	
	if (!team->RemoveIfInvite(user->GetUserId())) 
	{
		if (!team->IsLeader(executor->GetUserId()))
		{
			if (!team->CheckTeamLimit(user))
			{
				return TOR_TEAM_LIMIT;
			}

			if (!is_call_in_ack && team->IsMustCheck())
			{
				return TOR_MUST_CHECK;
			}
		}
	}

	Role *leader_role = team->GetLeader();
	// 检查是否在组队副本中
	if (this->CheckTeamInTeamTypeFB(team))
	{
		return TOR_IN_TEAM_FB;
	}

	// 检测队伍活动状态
	if (NULL != leader_role)
	{
		if (leader_role->GetRoleCross()->IsInCross3v3Matching())
		{
			return TOR_IN_TEAM_MATCHING;
		}
	}

	if (!team->AddTeamMember(user))
	{
		return TOR_TEAM_IS_FULL;
	}

	return TOR_SUC;
}

int TeamManager::RemoveTeamMember(Role *executor, int team_index, const UserID &user_id)
{
	if (NULL == executor)
	{
		return TOR_NO_POWER;
	}

	Team *team = this->GetTeam(team_index);
	if (NULL == team)
	{
		return TOR_TEAM_NOT_EXIST;
	}

	if (executor->GetUserId() != user_id)
	{
		if (!team->IsLeader(executor->GetUserId()))
		{
			return TOR_NO_POWER;
		}
	}

	bool is_remove_leader = false;
	int tl_id = -1; int tl_key = -1;
	if (team->IsLeader(user_id))
	{
		is_remove_leader = true;
		tl_id = team->GetTeamLocal(&tl_key);
	}

	if (team->RemoveMember(user_id, (executor->GetUserId() != user_id)))
	{
		if (team->GetMemberNum() <= 0 || team->IsNobodyOnline(user_id)) // 删除队伍
		{
			this->DestroyTeamHelper(team);
			this->RemoveTeamLocalMap(tl_id, tl_key, team_index);
		}
		else   // 还有人 
		{
			if (is_remove_leader) // 如果队长被干掉
			{
				this->RemoveTeamLocalMap(tl_id, tl_key, team_index);
				tl_id = team->GetTeamLocal(&tl_key);
				this->AddTeamLocalMap(tl_id, tl_key, team_index);
			}
		}

		return TOR_SUC;
	}

	return TOR_NOT_IN_THIS_TEAM;
}

int TeamManager::ChangeTeamLeader(Role *executor, Role *user)
{
	if (NULL == executor || NULL == user)
	{
		return TOR_NO_POWER;
	}

	if (executor == user) return TOR_SUC;

	Team *team = this->GetTeam(executor->GetTeamInfo()->GetTeamIndex());
	if (NULL == team) return TOR_TEAM_NOT_EXIST;

	if (!team->IsLeader(executor->GetUserId())) return TOR_NO_POWER;

	if (!team->ChangeTeamLeader(user->GetUserId()))
	{
		return TOR_NOT_IN_THIS_TEAM;
	}

	this->RemoveTeamLocalMap(executor, team->GetTeamIndex());
	this->AddTeamLocalMap(user, team->GetTeamIndex());

	return TOR_SUC;
}

bool TeamManager::ChangeMustCheck(Role *executor, bool must_check)
{
	if (NULL == executor)
	{
		return false;
	}

	Team *team = this->GetTeam(executor->GetTeamInfo()->GetTeamIndex());
	if (NULL == team) return false;

	if (!team->IsLeader(executor->GetUserId())) return false;

	team->SetMustCheck(must_check);

	return true;
}

bool TeamManager::ChangeAssignMode(Role *executor, int assign_mode)
{
	if (NULL == executor)
	{
		return false;
	}

	Team *team = this->GetTeam(executor->GetTeamInfo()->GetTeamIndex());
	if (NULL == team) return false;

	if (!team->IsLeader(executor->GetUserId())) return false;

	if (!team->ChangeAssignMode(assign_mode)) return false;

	return true;
}

bool TeamManager::ChangeMemberCanInvite(Role *executor, bool member_can_invite)
{
	if (NULL == executor)
	{
		return false;
	}

	Team *team = this->GetTeam(executor->GetTeamInfo()->GetTeamIndex());
	if (NULL == team) return false;

	if (!team->IsLeader(executor->GetUserId())) return false;

	team->SetMemberCanInvite(member_can_invite);

	return true;
}

bool TeamManager::ChangeTeamLimit(Role *executor, int limit_capability, int limit_level)
{
	if (NULL == executor)
	{
		return false;
	}

	Team *team = this->GetTeam(executor->GetTeamInfo()->GetTeamIndex());
	if (NULL == team) return false;

	if (!team->IsLeader(executor->GetUserId())) return false;

	team->SetTeamLimit(limit_capability, limit_level);

	return true;
}

void TeamManager::DestroyTeamHelper(Team *team)
{
	if (NULL == team)
	{
		return ;
	}

	unsigned int team_index = team->GetTeamIndex();
	m_team_list.Erase(team_index);

	EventHandler::Instance().OnDestroyTeam(team_index);

	delete team;
}

Team * TeamManager::GetTeam(int index)
{
	if (!TeamManager::IsValidTeamIndex(index)) return NULL;			// 不允许ID为0的队伍

	if (m_team_list.Exist(index))
	{
		return m_team_list[index];
	}

	return NULL;
}

bool TeamManager::OnUserChangeScene(int team_index, const UserID &user_id, int scene_id, int scene_key, ObjID obj_id)
{
	Team *team = this->GetTeam(team_index);
	if (NULL == team) return false;

	bool is_team_leader = false;
	int sid = -1; int skey = -1;
	if (team->IsLeader(user_id))
	{
		is_team_leader = true;
		sid = team->GetTeamLocal(&skey);
	}

	if (!team->OnUserChangeScene(user_id, scene_id, scene_key, obj_id))
	{
		return false;
	}

	if (is_team_leader)
	{
		this->RemoveTeamLocalMap(sid, skey, team_index);
		this->AddTeamLocalMap(scene_id, scene_key, team_index);
	}

	return true;
}

bool TeamManager::OnUserBaseInfoChange(int team_index, const UserID &user_id)
{
	Team *team = this->GetTeam(team_index);
	if (NULL == team) return NULL;

	return team->OnUserBaseInfoChange(user_id);
}

Role * TeamManager::GetLeader(int team_index)
{
	Team *team = this->GetTeam(team_index);
	if (NULL == team) return NULL;

	return team->GetLeader();
}

bool TeamManager::IsTeamFull(int team_index)
{
	Team *team = this->GetTeam(team_index);
	if (NULL == team) return false;

	return team->IsFull();
}

int TeamManager::Invite(Role *executor, Role *be_invited_user)
{
	if (NULL == executor || NULL == be_invited_user)
	{
		return TOR_NO_POWER;
	}

	if (executor->GetTeamInfo()->InTeam())
	{
		int team_index = executor->GetTeamInfo()->GetTeamIndex();

		Team *team = this->GetTeam(team_index);
		if (NULL == team) return TOR_TEAM_NOT_EXIST;
		
		if (!team->CheckTeamLimit(be_invited_user))
		{	
			return TOR_TEAM_LIMIT;
		}

		if (team->IsMemberCanInvite() || team->IsLeader(executor->GetUserId()))
		{
			team->Invite(be_invited_user->GetUserId());
			return TOR_SUC;
		}
	}

	return TOR_NO_POWER;
}


void TeamManager::AutoHaveTeam(Role *user)
{
	if (NULL == user)
	{
		return;
	}

	if (user->GetTeamInfo()->InTeam())
	{
		return;
	}

	if (NULL == user->GetScene())
	{
		return;
	}

	SceneLocal sl(user->GetScene()->GetSceneID(), user->GetScene()->GetSceneKey());

	TeamLocalMap::iterator teamlocal_it = m_team_local_map.find(sl);
	if (teamlocal_it != m_team_local_map.end())
	{
		TeamIndexSet *tis = &teamlocal_it->second.team_index_set;

		for (TeamIndexSet::iterator it = tis->begin(); it != tis->end(); ++it)
		{
			Team *team = this->GetTeam(*it);
			if (NULL != team)
			{
				if (!team->IsFull() && !team->IsMustCheck() && team->CheckTeamLimit(user)) // 1：队伍未满 2：允许直接加入 3：等级战力限制满足
				{
					Role *teamleader = team->GetLeader();
					if (NULL != teamleader)
					{
						this->AddTeamMember(teamleader, team->GetTeamIndex(), user, true); // 2：模拟队长邀请了该玩家 

						return;
					}
				}
			}
		}
	}

	if (!user->GetTeamInfo()->InTeam()) // 确保还是没队伍 
	{
		int index = this->CreateTeam(user, false, true, TEAM_ASSIGN_MODE_ATTACK);

		static char buff[1024]; memset(buff, 0, sizeof(buff));

		Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)buff;
		cc->channel_type = chatdef::CHANNEL_TYPE_WORLD;
		cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_team_create_notice, index);

		ChatManager::Instance().OnChannelChat(user, cc, cc->msg_length + sizeof(Protocol::CSChannelChat), chatdef::ORIGIN_TYPE_NORMAL_CHAT);
	}
}

void TeamManager::SendTeamListToUser(Role *user)
{
	if (NULL == user || NULL == user->GetScene())
	{
		return;
	}

	SceneLocal sl(user->GetScene()->GetSceneID(), user->GetScene()->GetSceneKey());

	TeamLocalMap::iterator iter = m_team_local_map.find(sl);
	if (iter != m_team_local_map.end())
	{
		Protocol::SCTeamListAck *msg = &iter->second.msg;
		TeamIndexSet *tis = &iter->second.team_index_set;

		int count = 0;
		for (TeamIndexSet::iterator i = tis->begin(); count < Protocol::MAX_TEAM_LIST_NUM && i != tis->end(); ++i)
		{
			Team *team = this->GetTeam(*i);
			if (NULL == team) continue;
			
			if (team->GetTeamInfo(msg->team_list[count].leader_name, &msg->team_list[count].leader_level, &msg->team_list[count].leader_prof,
				&msg->team_list[count].leader_sex, &msg->team_list[count].leader_vip_level, &msg->team_list[count].leader_camp, 
				&msg->team_list[count].team_type, &msg->team_list[count].avatar_timestamp))
			{
				msg->team_list[count].team_index = team->GetTeamIndex();
				msg->team_list[count].cur_member_num = team->GetMemberNum();
				team->GetMemberUidList(msg->team_list[count].member_uid_list, sizeof(msg->team_list[0].member_uid_list) / sizeof(msg->team_list[0].member_uid_list[0]));
				team->GetMemberUUidList(msg->team_list[count].member_uuid_list, sizeof(msg->team_list[0].member_uuid_list) / sizeof(msg->team_list[0].member_uuid_list[0]));
				++ count;
			}
		}
		msg->count = count;

		int sendlen = sizeof(Protocol::SCTeamListAck) - (Protocol::MAX_TEAM_LIST_NUM - msg->count) * sizeof(Protocol::TeamItem);
		EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)msg, sendlen);
	}
	else
	{
		Protocol::SCTeamListAck tla;
		tla.count = 0;
		int sendlen = sizeof(Protocol::SCTeamListAck) - (Protocol::MAX_TEAM_LIST_NUM - tla.count) * sizeof(Protocol::TeamItem);
		EngineAdapter::Instance().NetSend(user->GetNetId(), (const char*)&tla, sendlen);
	}
}

void TeamManager::AddTeamLocalMap(Role *user, int team_index)
{
	if (NULL == user || NULL == user->GetScene())
	{
		return;
	}

	this->AddTeamLocalMap(user->GetScene()->GetSceneID(), user->GetScene()->GetSceneKey(), team_index);
}

void TeamManager::RemoveTeamLocalMap(Role *user, int team_index)
{
	if (NULL == user || NULL == user->GetScene())
	{
		return;
	}

	this->RemoveTeamLocalMap(user->GetScene()->GetSceneID(), user->GetScene()->GetSceneKey(), team_index);
}

void TeamManager::AddTeamLocalMap(int scene_id, int scene_key, int team_index)
{
	SceneLocal sl(scene_id, scene_key);
	TeamLocalMap::iterator iter = m_team_local_map.find(sl);
	if (iter != m_team_local_map.end())
	{
		iter->second.team_index_set.insert(team_index);
	}
	else
	{
		SceneTeamInfo stf;
		stf.team_index_set.insert(team_index);
		m_team_local_map[sl] = stf;
	}
}

void TeamManager::RemoveTeamLocalMap(int scene_id, int scene_key, int team_index)
{
	TeamLocalMap::iterator iter = m_team_local_map.find(SceneLocal(scene_id, scene_key));
	if (iter != m_team_local_map.end())
	{
		iter->second.team_index_set.erase(team_index);
		if (iter->second.team_index_set.size() == 0)
		{
			m_team_local_map.erase(iter);
		}
	}
}

void TeamManager::OnCreateTeam(Role *role, Protocol::CSCreateTeam *ct)
{
	int team_index = this->CreateTeam(role, 0 != ct->must_check, 0 != ct->member_can_invite, ct->assign_mode, ct->team_type);
	if (TeamManager::TOR_ALREADY_IN_TEAM == team_index)
	{
		role->NoticeNum(errornum::EN_ALREADY_IN_TEAM);
	}
}

void TeamManager::OnInviteUser(Role *role, Protocol::CSInviteUser *iu)
{
	if (this->IsTeamFull(role->GetTeamInfo()->GetTeamIndex()))
	{
		role->NoticeNum(errornum::EN_TEAM_IS_FULL);
		return;
	}

	UserID be_invited_user_id = IntToUid(iu->role_id);
	if (be_invited_user_id == role->GetUserId()) return;

	Role *be_invited_user = World::GetInstWorld()->GetSceneManager()->GetRole(be_invited_user_id);
	if (NULL == be_invited_user)
	{
		role->NoticeNum(errornum::EN_USER_IS_NOT_ONLINE);
		return;
	}

	if (be_invited_user->GetTeamInfo()->InTeam())
	{
		if (!role->GetTeamInfo()->InTeam())
		{
			this->ReqJoinTeamHelper(role, be_invited_user->GetTeamInfo()->GetTeamIndex(), false);
		}
		else
		{
			//role->NoticeNum(errornum::EN_HE_IS_IN_TEAM);
			// 被邀请方如果有队伍，则邀请该队伍的队长合并队伍
			Team *be_invited_team = this->GetTeam(be_invited_user->GetTeamInfo()->GetTeamIndex());
			Team *role_team = this->GetTeam(role->GetTeamInfo()->GetTeamIndex());
			if (!be_invited_team || !role_team)
			{
				role->NoticeNum(errornum::EN_TEAM_IS_NOT_EXIST);
				return;
			}

			int check_ret = this->CheckCanMergeTeam(be_invited_user, role_team, false);
			if (TeamManager::TOR_SUC == check_ret)
			{
				Role * be_invited_leader = be_invited_team->GetLeader();
				if (be_invited_leader)
				{
					role->NoticeNum(noticenum::NT_TEAM_INVITE_REQ_SUCC);

					Protocol::SCInviteUserTransmit iut;
					iut.inviter = UidToInt(role->GetUserId());
					role->GetName(iut.inviter_name);
					iut.inviter_camp = role->GetCamp();
					iut.inviter_prof = role->GetProf();
					iut.inviter_sex = role->GetSex();
					iut.inviter_level = role->GetLevel();
					iut.inviter_team_num = (char)role_team->GetMemberNum();
					iut.inviter_avatar_timestamp = role->GetAvatarTimeStamp();
					EngineAdapter::Instance().NetSend(be_invited_leader->GetNetId(), (const char *)&iut, sizeof(iut));
				}
				else
				{
					role->NoticeNum(errornum::EN_TEAM_LEADER_NOT_FOUND);
				}
			}
			else
			{
				this->SendCheckMergeResultToRole(role, check_ret, false);
			}

			return;
		}
	}
	else 
	{
		if (role->GetTeamInfo()->InTeam())
		{
			int invite_ret = this->Invite(role, be_invited_user);
			if (TeamManager::TOR_SUC != invite_ret)
			{
				role->NoticeNum(TeamManager::TOR_NO_POWER == invite_ret ? errornum::EN_YOU_HAVE_NO_POWER : errornum::EN_TEAM_JOIN_LIMIT);
				return;
			}
		}

		if (be_invited_user->GetTeamInfo()->IsAutoJoinTeam())
		{
			Protocol::CSInviteUserTransmitRet cmd;
			cmd.inviter = UidToInt(role->GetUserId());
			cmd.result = 0;
			this->OnInviteRet(be_invited_user, &cmd);
		}
		else
		{
			role->NoticeNum(noticenum::NT_TEAM_INVITE_REQ_SUCC);

			Protocol::SCInviteUserTransmit iut;
			iut.inviter = UidToInt(role->GetUserId());
			role->GetName(iut.inviter_name);
			iut.inviter_camp = role->GetCamp();
			iut.inviter_prof = role->GetProf();
			iut.inviter_sex = role->GetSex();
			iut.inviter_level = role->GetLevel();
			Team *team = role->GetTeamInfo()->GetMyTeam();
			if (NULL != team)
			{
				iut.inviter_team_num = (char)team->GetMemberNum();
			}
			else
			{
				iut.inviter_team_num = 0;
			}
			iut.inviter_avatar_timestamp = role->GetAvatarTimeStamp();
			EngineAdapter::Instance().NetSend(be_invited_user->GetNetId(), (const char *)&iut, sizeof(iut));
		}
	}
}

void TeamManager::OnInviteUniqueUser(Role *role, Protocol::CSInviteUniqueUser *iu)
{
	if (nullptr == role)
	{
		return;
	}

	if (nullptr == role->GetScene())
	{
		return;
	}

	if (this->IsTeamFull(role->GetTeamInfo()->GetTeamIndex()))
	{
		role->NoticeNum(errornum::EN_TEAM_IS_FULL);
		return;
	}

	UniqueUserID be_invited_user_id = LongLongToUniqueUserID(iu->role_uuid);
	if (be_invited_user_id == role->GetUniqueUserID())
	{
		return;
	}

	Role *be_invited_user = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(iu->role_uuid);

	if (!role->IsInCross())
	{
		if (NULL == be_invited_user)
		{
			if (World::GetInstWorld()->GetSceneManager()->UserCrosserExist(iu->role_uuid))  // 组队只可以组本服的人，所以要么在线，要么在跨服
			{
				role->NoticeNum(errornum::EN_USER_IS_NOT_IN_SAME_SERVER);
			}
			else
			{
				role->NoticeNum(errornum::EN_USER_IS_NOT_ONLINE);
			}

			return;
		}
	}
	else
	{
		if (NULL == be_invited_user)
		{
			role->NoticeNum(errornum::EN_USER_IS_NOT_IN_SAME_SERVER_SCENE);
			return;
		}
		else
		{
			if (nullptr == be_invited_user->GetScene())
			{
				return;
			}

			// 同场景并且同个服务器的玩家才可以组队
			if (be_invited_user->GetScene() != role->GetScene() || 
				be_invited_user->GetRoleCross()->GetOriginPlatType() != role->GetRoleCross()->GetOriginPlatType() || 
				be_invited_user->GetRoleCross()->GetOriginMergeServerID() != role->GetRoleCross()->GetOriginMergeServerID())
			{
				role->NoticeNum(errornum::EN_USER_IS_NOT_IN_SAME_SERVER_SCENE);
				return;
			}
		}
	}

	if (be_invited_user->GetTeamInfo()->InTeam())
	{
		if (!role->GetTeamInfo()->InTeam())
		{
			this->ReqJoinTeamHelper(role, be_invited_user->GetTeamInfo()->GetTeamIndex(), false);
		}
		else
		{
			//role->NoticeNum(errornum::EN_HE_IS_IN_TEAM);
			// 被邀请方如果有队伍，则邀请该队伍的队长合并队伍
			Team *be_invited_team = this->GetTeam(be_invited_user->GetTeamInfo()->GetTeamIndex());
			Team *role_team = this->GetTeam(role->GetTeamInfo()->GetTeamIndex());
			if (!be_invited_team || !role_team)
			{
				role->NoticeNum(errornum::EN_TEAM_IS_NOT_EXIST);
				return;
			}

			int check_ret = this->CheckCanMergeTeam(be_invited_user, role_team, false);
			if (TeamManager::TOR_SUC == check_ret)
			{
				Role * be_invited_leader = be_invited_team->GetLeader();
				if (be_invited_leader)
				{
					role->NoticeNum(noticenum::NT_TEAM_INVITE_REQ_SUCC);

					Protocol::SCInviteUserTransmit iut;
					iut.inviter = UidToInt(role->GetUserId());
					role->GetName(iut.inviter_name);
					iut.inviter_camp = role->GetCamp();
					iut.inviter_prof = role->GetProf();
					iut.inviter_sex = role->GetSex();
					iut.inviter_level = role->GetLevel();
					iut.inviter_team_num = (char)role_team->GetMemberNum();
					iut.inviter_avatar_timestamp = role->GetAvatarTimeStamp();
					EngineAdapter::Instance().NetSend(be_invited_leader->GetNetId(), (const char *)&iut, sizeof(iut));
				}
				else
				{
					role->NoticeNum(errornum::EN_TEAM_LEADER_NOT_FOUND);
				}
			}
			else
			{
				this->SendCheckMergeResultToRole(role, check_ret, false);
			}

			return;
		}
	}
	else
	{
		if (role->GetTeamInfo()->InTeam())
		{
			int invite_ret = this->Invite(role, be_invited_user);
			if (TeamManager::TOR_SUC != invite_ret)
			{
				role->NoticeNum(TeamManager::TOR_NO_POWER == invite_ret ? errornum::EN_YOU_HAVE_NO_POWER : errornum::EN_TEAM_JOIN_LIMIT);
				return;
			}
		}

		if (be_invited_user->GetTeamInfo()->IsAutoJoinTeam())
		{
			Protocol::CSInviteUserTransmitRet cmd;
			cmd.inviter = UidToInt(role->GetUserId());
			cmd.result = 0;
			this->OnInviteRet(be_invited_user, &cmd);
		}
		else
		{
			role->NoticeNum(noticenum::NT_TEAM_INVITE_REQ_SUCC);

			Protocol::SCInviteUserTransmit iut;
			iut.inviter = UidToInt(role->GetUserId());
			role->GetName(iut.inviter_name);
			iut.inviter_camp = role->GetCamp();
			iut.inviter_prof = role->GetProf();
			iut.inviter_sex = role->GetSex();
			iut.inviter_level = role->GetLevel();
			Team *team = role->GetTeamInfo()->GetMyTeam();
			if (NULL != team)
			{
				iut.inviter_team_num = (char)team->GetMemberNum();
			}
			else
			{
				iut.inviter_team_num = 0;
			}
			iut.inviter_avatar_timestamp = role->GetAvatarTimeStamp();
			EngineAdapter::Instance().NetSend(be_invited_user->GetNetId(), (const char *)&iut, sizeof(iut));
		}
	}
}

void TeamManager::OnInviteRet(Role *role, Protocol::CSInviteUserTransmitRet *iutr)
{
	if (NULL == role)
	{
		return;
	}

	//if (role->GetTeamInfo()->InTeam())
	//{
	//	role->NoticeNum(errornum::EN_ALREADY_IN_TEAM);
	//	return;
	//}

	int inviter_team_index = -1;

	UserID inviter_user_id = IntToUid(iutr->inviter);
	Role *inviter_user = World::GetInstWorld()->GetSceneManager()->GetRole(inviter_user_id);
	if (NULL == inviter_user)	// 邀请者下线了，看下他留下的队伍有没有
	{
		inviter_team_index = this->GetTeamIndex(iutr->inviter);
	}
	else
	{
		inviter_team_index = inviter_user->GetTeamInfo()->GetTeamIndex();
	}

	if (iutr->result == 0)
	{
		int ret = 0;
		
		if (TeamManager::IsValidTeamIndex(inviter_team_index))
		{
			// 如果自己有队伍，则尝试合并到邀请方队伍里
			if (role->GetTeamInfo()->InTeam())
			{
				Team *role_team = this->GetTeam(role->GetTeamInfo()->GetTeamIndex());
				Team *inviter_team = this->GetTeam(inviter_team_index);
				if (role_team && inviter_team)
				{
					int check_ret = this->CheckCanMergeTeam(role, inviter_team, true);
					if (TeamManager::TOR_SUC == check_ret)
					{
						// 开始合并

						// 保存role的队伍成员
						Role *role_list[MAX_TEAM_MEMBER_NUM] = { NULL };
						int member_uid_list[MAX_TEAM_MEMBER_NUM];
						int member_count = role_team->GetMemberUidList(member_uid_list, MAX_TEAM_MEMBER_NUM);
						for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
						{
							Role *member_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
							role_list[i] = member_role;
						}

						// 解散role的队伍
						if (this->DismissTeam(role->GetTeamInfo()->GetTeamIndex(), role))
						{
							// 将role的队伍成员全部加入邀请者的队伍
							for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
							{
								Role *member_role = role_list[i];
								if (NULL != member_role)
								{
									inviter_team->AddTeamMember(member_role);
								}
							}
						}
					}
					else
					{
						this->SendCheckMergeResultToRole(role, check_ret, true);
					}
				}
				else
				{
					role->NoticeNum(errornum::EN_TEAM_IS_NOT_EXIST);
				}

				return;  // 合并后就返回，接下来不用执行了
			}
			// 没有队伍，则是默认的单人加入队伍
			else
			{
				ret = this->AddTeamMember(role, inviter_team_index, role);
			}
		}
		else
		{
			ret = TeamManager::TOR_TEAM_NOT_EXIST;
		}

		if (ret != TeamManager::TOR_SUC)
		{
			switch (ret)
			{
			case TeamManager::TOR_IN_TEAM_FB:
				{
					role->NoticeNum(errornum::EN_TEAM_SPECIAL_TEAM_JOIN_LIMIT);
				}
				break;

			case TeamManager::TOR_ALREADY_IN_TEAM:
				{
					role->NoticeNum(errornum::EN_ALREADY_IN_TEAM);
				}
				break;

			case TeamManager::TOR_TEAM_NOT_EXIST:
				if (NULL != inviter_user)
				{
					int team_index = this->CreateTeam(inviter_user, false, true, TEAM_ASSIGN_MODE_ATTACK);
					if (TeamManager::IsValidTeamIndex(team_index))
					{
						this->AddTeamMember(inviter_user, team_index, role);
					}
				}
				else
				{
					role->NoticeNum(errornum::EN_USER_IS_NOT_ONLINE);
				}
				break;

			case TeamManager::TOR_MUST_CHECK:
				{
					Role *leader = this->GetLeader(inviter_team_index);
					if (NULL != leader)
					{
						Protocol::SCReqJoinTeamTransmit rit;
						rit.req_role_id = UidToInt(role->GetUserId());
						rit.req_total_cabality = role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_TOTAL);
						role->GetName(rit.req_role_name);
						rit.req_role_camp = role->GetCamp();
						rit.req_role_prof = role->GetProf();
						rit.req_role_sex = role->GetSex();
						rit.reserved = 0;
						rit.req_role_level = role->GetLevel();
						rit.req_avatar_timestamp = role->GetAvatarTimeStamp();
						EngineAdapter::Instance().NetSend(leader->GetNetId(), (const char *)&rit, sizeof(Protocol::SCReqJoinTeamTransmit));
					}
				}
				break;

			case TeamManager::TOR_TEAM_IS_FULL:
				{
					role->NoticeNum(errornum::EN_TEAM_IS_FULL);
				}
				break;

			case TeamManager::TOR_TEAM_LIMIT:
				{
					role->NoticeNum(errornum::EN_TEAM_JOIN_LIMIT);
				}
				break;

			case TeamManager::TOR_IN_TEAM_MATCHING:
				{
					role->NoticeNum(errornum::EN_TEAM_JOIN_OPERA_LIMIT_IN_TEAM_MATCHING);
				}
				break;

			case TeamManager::TOR_IN_ROLE_MATCHING:
				{
					role->NoticeNum(errornum::EN_TEAM_JOIN_OPERA_LIMIT_IN_ROLE_MATCHING);
				}
				break;
			}
		}
	}
	else
	{
		if (NULL != inviter_user) 
		{
			inviter_user->NoticeNum(errornum::EN_HE_IS_REJUCT);
		}
	}
}

void TeamManager::OnJoinTeamRet(Role *role, Protocol::CSReqJoinTeamRet *jtr)
{
	UserID reqer_user_id = IntToUid(jtr->req_role_id);

	Role *reqer = World::GetInstWorld()->GetSceneManager()->GetRole(reqer_user_id);
	if (NULL == reqer)
	{
		role->NoticeNum(errornum::EN_USER_IS_NOT_ONLINE);
		return;
	}

	if (jtr->result == 0)
	{
		// 如果申请者有队伍，那么尝试合并他的队伍
		if (reqer->GetTeamInfo()->InTeam())
		{
			Team *join_team = this->GetTeam(role->GetTeamInfo()->GetTeamIndex());
			Team *reqer_team = this->GetTeam(reqer->GetTeamInfo()->GetTeamIndex());
			if (join_team && reqer_team)
			{
				int check_ret = this->CheckCanMergeTeam(reqer, join_team, true);
				if (TeamManager::TOR_SUC == check_ret)
				{
					// 开始合并

					// 保存reqer的队伍成员
					Role *role_list[MAX_TEAM_MEMBER_NUM] = { NULL };
					int member_uid_list[MAX_TEAM_MEMBER_NUM];
					int member_count = reqer_team->GetMemberUidList(member_uid_list, MAX_TEAM_MEMBER_NUM);
					for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
					{
						Role *member_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
						role_list[i] = member_role;
					}

					// 解散reqer的队伍
					Role *leader_role = this->GetLeader(reqer->GetTeamInfo()->GetTeamIndex());
					if (nullptr != leader_role)
					{
						if (this->DismissTeam(reqer->GetTeamInfo()->GetTeamIndex(), leader_role))
						{
							// 将reqer的队伍成员全部加入接受申请的队伍
							for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
							{
								Role *member_role = role_list[i];
								if (NULL != member_role)
								{
									join_team->AddTeamMember(member_role);
								}
							}
						}
					}
					else
					{
						role->NoticeNum(errornum::EN_TEAM_LEADER_NOT_FOUND);
					}
				}
				else
				{
					this->SendCheckMergeResultToRole(role, check_ret, true);
				}
			}
			else
			{
				role->NoticeNum(errornum::EN_TEAM_IS_NOT_EXIST);
			}

			return;
		}

		// 申请者没有队伍，则是默认的单人申请入队
		int ret = this->AddTeamMember(role, role->GetTeamInfo()->GetTeamIndex(), reqer);
		if (ret != TeamManager::TOR_SUC)
		{
			if (TeamManager::TOR_TEAM_IS_FULL == ret)
			{
				role->NoticeNum(errornum::EN_TEAM_IS_FULL);
				reqer->NoticeNum(errornum::EN_TEAM_IS_FULL);
			}
			else if (TeamManager::TOR_TEAM_LIMIT == ret)
			{
				role->NoticeNum(errornum::EN_TEAM_JOIN_LIMIT);
				reqer->NoticeNum(errornum::EN_TEAM_JOIN_LIMIT);
			}
			else if (TeamManager::TOR_ALREADY_IN_TEAM == ret)
			{
				role->NoticeNum(errornum::EN_HE_IS_IN_TEAM);
			}
			else if (TeamManager::TOR_IN_TEAM_FB == ret)
			{
				role->NoticeNum(errornum::EN_TEAM_SPECIAL_TEAM_JOIN_LIMIT);
			}
			else if (TeamManager::TOR_IN_TEAM_MATCHING == ret)
			{
				role->NoticeNum(errornum::EN_TEAM_JOIN_OPERA_LIMIT_IN_TEAM_MATCHING);
				reqer->NoticeNum(errornum::EN_TEAM_JOIN_OPERA_LIMIT_IN_TEAM_MATCHING);
			}
			else if (TeamManager::TOR_IN_ROLE_MATCHING == ret)
			{
				role->NoticeNum(errornum::EN_TEAM_JOIN_OPERA_LIMIT_IN_ROLE_MATCHING);
				reqer->NoticeNum(errornum::EN_TEAM_JOIN_OPERA_LIMIT_IN_ROLE_MATCHING);
			}

			return;
		}
		else
		{
			// reqer->NoticeNum(noticenum::NT_TEAM_JOIN_SUCC);
		}
	}
	else
	{
		reqer->NoticeNum(errornum::EN_TEAM_LEADER_IS_REJUCT);
		return;
	}
}

void TeamManager::ReqJoinTeamHelper(Role *user, int team_index, bool is_call_in_ack)
{
	if (nullptr == user)
	{
		return;
	}

	// user如果已经有队伍，那么尝试申请合并
	if (user->GetTeamInfo()->InTeam())
	{
		Team *user_team = this->GetTeam(user->GetTeamInfo()->GetTeamIndex());
		Team *join_team = this->GetTeam(team_index);
		if (user_team && join_team)
		{
			int check_ret = this->CheckCanMergeTeam(user, join_team, true);
			if (TeamManager::TOR_SUC == check_ret)
			{
				// 要加入的队伍如果是房间型的话就不申请，直接入队
				if (TEAM_TYPE_DEFAULT != join_team->GetTeamType())
				{
					// 开始合并

					// 保存user的队伍成员
					Role *role_list[MAX_TEAM_MEMBER_NUM] = { NULL };
					int member_uid_list[MAX_TEAM_MEMBER_NUM];
					int member_count = user_team->GetMemberUidList(member_uid_list, MAX_TEAM_MEMBER_NUM);
					for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
					{
						Role *member_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
						role_list[i] = member_role;
					}

					// 解散user的队伍
					Role *leader_role = this->GetLeader(user->GetTeamInfo()->GetTeamIndex());
					if (nullptr != leader_role)
					{
						if (this->DismissTeam(user->GetTeamInfo()->GetTeamIndex(), leader_role))
						{
							// 将user的队伍成员全部加入接受申请的队伍
							for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
							{
								Role *member_role = role_list[i];
								if (NULL != member_role)
								{
									join_team->AddTeamMember(member_role);
								}
							}
						}
					}
					else
					{
						user->NoticeNum(errornum::EN_TEAM_LEADER_NOT_FOUND);
					}
				}
				else
				{
					Role * leader_role = join_team->GetLeader();
					if (leader_role)
					{
						// 队伍加入队伍，必须提交申请
						Protocol::SCReqJoinTeamTransmit rit;
						rit.req_role_id = UidToInt(user->GetUserId());
						rit.req_total_cabality = user->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_TOTAL);
						user->GetName(rit.req_role_name);
						rit.req_role_camp = user->GetCamp();
						rit.req_role_prof = user->GetProf();
						rit.req_role_sex = user->GetSex();
						rit.reserved = 0;
						rit.req_role_level = user->GetLevel();
						rit.req_avatar_timestamp = user->GetAvatarTimeStamp();
						EngineAdapter::Instance().NetSend(leader_role->GetNetId(), (const char *)&rit, sizeof(Protocol::SCReqJoinTeamTransmit));

						user->NoticeNum(noticenum::NT_TEAM_JOIN_REQ_SUCC);
					}
					else
					{
						user->NoticeNum(errornum::EN_TEAM_LEADER_NOT_FOUND);
					}
				}
			}
			else
			{
				this->SendCheckMergeResultToRole(user, check_ret, true);
			}
		}
		else
		{
			user->NoticeNum(errornum::EN_TEAM_IS_NOT_EXIST);
		}

		return;
	}

	// 申请方没有队伍，默认单人申请入队
	int ret = this->AddTeamMember(user, team_index, user, is_call_in_ack);
	if (ret != TeamManager::TOR_SUC)
	{
		if (ret == TeamManager::TOR_TEAM_NOT_EXIST)
		{
			user->NoticeNum(errornum::EN_TEAM_IS_NOT_EXIST);
		}
		else if (ret == TeamManager::TOR_MUST_CHECK)
		{
			Role *leader = this->GetLeader(team_index);
			if (NULL != leader)
			{
				Protocol::SCReqJoinTeamTransmit rit;
				rit.req_role_id = UidToInt(user->GetUserId());
				rit.req_total_cabality = user->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_TOTAL);
				user->GetName(rit.req_role_name);
				rit.req_role_camp = user->GetCamp();
				rit.req_role_prof = user->GetProf();
				rit.req_role_sex = user->GetSex();
				rit.reserved = 0;
				rit.req_role_level = user->GetLevel();
				rit.req_avatar_timestamp = user->GetAvatarTimeStamp();
				EngineAdapter::Instance().NetSend(leader->GetNetId(), (const char *)&rit, sizeof(Protocol::SCReqJoinTeamTransmit));

				user->NoticeNum(noticenum::NT_TEAM_JOIN_REQ_SUCC);
			}
		}
		else if (ret == TeamManager::TOR_TEAM_IS_FULL)
		{
			user->NoticeNum(errornum::EN_TEAM_IS_FULL);
		}
		else if (ret == TeamManager::TOR_TEAM_LIMIT)
		{
			user->NoticeNum(errornum::EN_TEAM_JOIN_LIMIT);
		}
		else if (ret == TeamManager::TOR_ALREADY_IN_TEAM)
		{
			user->NoticeNum(errornum::EN_ALREADY_IN_TEAM);
		}
		else if (ret == TeamManager::TOR_IN_TEAM_FB)
		{
			user->NoticeNum(errornum::EN_TEAM_SPECIAL_TEAM_JOIN_LIMIT);
		}
		else if (ret == TeamManager::TOR_IN_TEAM_MATCHING)
		{
			user->NoticeNum(errornum::EN_TEAM_JOIN_OPERA_LIMIT_IN_TEAM_MATCHING);
		}
		else if (ret == TeamManager::TOR_IN_ROLE_MATCHING)
		{
			user->NoticeNum(errornum::EN_TEAM_JOIN_OPERA_LIMIT_IN_ROLE_MATCHING);
		}
	}
	else
	{
		if (is_call_in_ack) 
		{
			// user->NoticeNum(noticenum::NT_TEAM_JOIN_SUCC);
		}
	}
}

void TeamManager::OnReqJoinTeam(Role *role, Protocol::CSReqJoinTeam *rjt)
{
	this->ReqJoinTeamHelper(role, rjt->team_index, 0 != rjt->is_call_in_ack);
}

void TeamManager::OnDismissTeam(Role *user)
{
	if (!user->GetTeamInfo()->InTeam())
	{
		user->NoticeNum(errornum::EN_YOU_ARE_NOT_IN_TEAM);
		return;
	}

	//if (GLOBAL_CONFIG->GetOtherConfig()->IsTeamFb(user->GetSceneId()))
	//{
	//	user->NoticeNum(errornum::EN_TEAM_IN_TEAM_FB);
	//	return;
	//}

	if (!this->DismissTeam(user->GetTeamInfo()->GetTeamIndex(), user))
	{
		user->NoticeNum(errornum::EN_YOU_HAVE_NO_POWER);
		return;
	}
}

void TeamManager::OnKickOutOfTeam(Role *user, Protocol::CSKickOutOfTeam *koof)
{
	if (user == nullptr || koof == nullptr) return;

	if (!user->GetTeamInfo()->InTeam())
	{
		user->NoticeNum(errornum::EN_YOU_ARE_NOT_IN_TEAM);
		return;
	}

	UserID bekick_uid = IntToUid(koof->role_id);
	if (user->GetUserId() == bekick_uid)
	{
		user->NoticeNum(errornum::EN_YOU_HAVE_NO_POWER);
		return;
	}

	if (Scene::IsTeamScene(user->GetScene()->GetSceneType(), user->GetScene()->GetSceneID()))
	{
		user->NoticeNum(errornum::EN_TEAM_TEAM_FB_OPERA_LIMIT);
		return;
	}

	int ret = this->RemoveTeamMember(user, user->GetTeamInfo()->GetTeamIndex(), bekick_uid);
	if (ret != TeamManager::TOR_SUC)
	{
		switch (ret)
		{
		case TeamManager::TOR_TEAM_NOT_EXIST:
			{
				user->NoticeNum(errornum::EN_TEAM_IS_NOT_EXIST);
			}
			break;

		case TeamManager::TOR_NO_POWER:
			{
				user->NoticeNum(errornum::EN_YOU_HAVE_NO_POWER);
			}
			break;

		case TeamManager::TOR_NOT_IN_THIS_TEAM:
			{
				user->NoticeNum(errornum::EN_HE_IS_NOT_IN_YOUR_TEAM);
			}
			break;
		}
	}

}

void TeamManager::OnChangeTeamLeader(Role *user, Protocol::CSChangeTeamLeader *ctl)
{
	if (!user->GetTeamInfo()->InTeam())
	{
		user->NoticeNum(errornum::EN_YOU_ARE_NOT_IN_TEAM);
		return;
	}

	Role *be_changed = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(ctl->role_id));
	if (NULL == be_changed)
	{
		user->NoticeNum(errornum::EN_USER_IS_NOT_ONLINE);
		return;
	}

	int ret = this->ChangeTeamLeader(user, be_changed);

	switch (ret)
	{
	case TeamManager::TOR_TEAM_NOT_EXIST:
		user->NoticeNum(errornum::EN_TEAM_IS_NOT_EXIST);
		break;

	case TeamManager::TOR_NO_POWER:
		user->NoticeNum(errornum::EN_YOU_HAVE_NO_POWER);
		break;

	case TeamManager::TOR_NOT_IN_THIS_TEAM:
		user->NoticeNum(errornum::EN_HE_IS_NOT_IN_YOUR_TEAM);
		break;

	case TeamManager::TOR_SUC:
		be_changed->NoticeNum(noticenum::NT_TEAM_CHANGE_CAPTAIN_SUCC);
		break;
	}
}

void TeamManager::OnExitTeam(Role *user)
{
	if (NULL == user)
	{
		return;
	}

	if (!user->GetTeamInfo()->InTeam())
	{
		user->NoticeNum(errornum::EN_YOU_ARE_NOT_IN_TEAM);
		return;
	}

	if (Scene::IsTeamScene(user->GetScene()->GetSceneType(), user->GetScene()->GetSceneID()))
	{
		user->NoticeNum(errornum::EN_TEAM_TEAM_FB_OPERA_LIMIT);
		return;
	}

	this->RemoveTeamMember(user, user->GetTeamInfo()->GetTeamIndex(), user->GetUserId());
}

void TeamManager::OnTeamListReq(Role *role)
{
	this->SendTeamListToUser(role);
}

void TeamManager::SaveTeamIndex(int uid, int team_index)
{
	m_user_teamindex_map[uid] = team_index;
}

int TeamManager::GetTeamIndex(int uid, bool is_remove)
{
	int team_index = 0;

	UserTeamIndexMapIt it = m_user_teamindex_map.find(uid);
	if (it != m_user_teamindex_map.end())
	{
		team_index = it->second;

		if (is_remove) 
		{
			m_user_teamindex_map.erase(it);
		}
	}

	return team_index;
}

bool TeamManager::CheckTeamInTeamTypeFB(Team *team)
{
	if (nullptr == team)
	{
		return false;
	}

	Role *leader_role = team->GetLeader();
	if (TEAM_TYPE_YAOSHOUJITANG == team->GetTeamType() || TEAM_TYPE_TEAM_TOWERDEFEND == team->GetTeamType()
		|| TEAM_TYPE_TEAM_MIGONGXIANFU == team->GetTeamType() || TEAM_TYPE_TEAM_EQUIP_FB == team->GetTeamType() ||
		TEAM_TYPE_TEAM_DAILY_FB == team->GetTeamType() || TEAM_TYPE_EQUIP_TEAM_FB == team->GetTeamType() ||
		TEAM_TYPE_QINGYUAN_FB == team->GetTeamType())
	{
		if (nullptr != leader_role && nullptr != leader_role->GetScene())
		{
			if (Scene::SCENE_TYPE_TAOSHOUJITAN_TEAM_FB == leader_role->GetScene()->GetSceneType() ||
				Scene::SCENE_TYPE_MIGONGXIANFU_TEAM == leader_role->GetScene()->GetSceneType() ||
				Scene::SCENE_TYPE_TEAM_EQUIPFB == leader_role->GetScene()->GetSceneType() ||
				Scene::SCENE_TYPE_TEAM_TOWERDEFEND == leader_role->GetScene()->GetSceneType() ||
				Scene::SCENE_TYPE_LOVERS_FB == leader_role->GetScene()->GetSceneType())
			{
				return true;
			}
		}
	}

	return false;
}

// role队伍尝试合并到target_team 
// is_ret 标记是否队长权限
int TeamManager::CheckCanMergeTeam(Role *role, Team *target_team, bool is_ret)
{
	if (nullptr == role)
	{
		return -999;
	}

	if (!role->GetTeamInfo()->InTeam())
	{
		return TeamManager::TOR_TEAM_NOT_EXIST;
	}

	Team *role_team = this->GetTeam(role->GetTeamInfo()->GetTeamIndex());

	if (nullptr == role_team || nullptr == target_team)
	{
		return TeamManager::TOR_TEAM_NOT_EXIST;
	}

	if (target_team->GetTeamIndex() == role->GetTeamInfo()->GetTeamIndex())
	{
		return TeamManager::TOR_IN_THIS_TEAM;
	}

	// 如果是确定或者回复操作，则判断有没有权限
	if (is_ret)
	{
		// 只有队长才能合并
		if (!role_team->IsLeader(role->GetUserId()))
		{
			return TeamManager::TOR_NO_POWER;
		}
	}

	if (target_team->GetMemberNum() + role_team->GetMemberNum() > MAX_TEAM_MEMBER_NUM)
	{
		return TeamManager::TOR_TEAM_IS_FULL;
	}

	if (role->GetRoleCross()->IsInCross3v3Matching())  // 自己正在匹配，不可合并
	{
		return TeamManager::TOR_IN_ROLE_MATCHING;
	}

	if (this->CheckTeamInTeamTypeFB(role_team) || this->CheckTeamInTeamTypeFB(target_team))
	{
		return TeamManager::TOR_IN_TEAM_FB;
	}

	Role *leader_role = target_team->GetLeader();
	if (!leader_role)
	{
		return TeamManager::TOR_LEADER_NOT_EXIST;
	}

	if (leader_role->GetRoleCross()->IsInCross3v3Matching())  // 目标队伍正在匹配，不可合并
	{
		return TeamManager::TOR_IN_TEAM_MATCHING;
	}

	// 如果目标方是组队副本房间，则判定房间限制，必须全部队友都要满足才能合并
	if (TEAM_TYPE_DEFAULT != target_team->GetTeamType())
	{
		int member_uid_list[MAX_TEAM_MEMBER_NUM];
		int member_count = role_team->GetMemberUidList(member_uid_list, MAX_TEAM_MEMBER_NUM);
		for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
		{
			Role *member_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
			if (member_role)
			{
				int fb_type = TeamFbRoom::Instance().GetFbTypeByTeamType(target_team->GetTeamType());
				if (!TeamFbRoom::Instance().CheckIsCanEnterFbOnJoin(member_role, fb_type, target_team->GetTeamFbLayer(), TeamFbRoom::OperateType::JOIN_ROOM))
				{
					return TeamManager::TOR_TEAM_LIMIT;
				}
			}
		}
	}

	return TeamManager::TOR_SUC;
}

// check_ret检查结果
// is_ret 标记是否队长权限
void TeamManager::SendCheckMergeResultToRole(Role *role, int check_ret, int is_ret)
{
	if (nullptr == role)
	{
		return;
	}

	switch (check_ret)
	{
	case TeamManager::TOR_TEAM_NOT_EXIST:
	{
		role->NoticeNum(errornum::EN_TEAM_IS_NOT_EXIST);
		return;
	}
	break;

	case TeamManager::TOR_IN_THIS_TEAM:
	{
		role->NoticeNum(errornum::EN_ROLE_IS_IN_THIS_TEAM);
		return;
	}
	break;

	case TeamManager::TOR_TEAM_IS_FULL:
	{
		role->NoticeNum(errornum::EN_TEAM_TOO_MANY_MEMBERS);
		return;
	}
	break;

	case TeamManager::TOR_IN_ROLE_MATCHING:
	{
		role->NoticeNum(errornum::EN_TEAM_JOIN_OPERA_LIMIT_IN_ROLE_MATCHING);
		return;
	}
	break;

	case TeamManager::TOR_IN_TEAM_MATCHING:
	{
		role->NoticeNum(errornum::EN_TEAM_JOIN_OPERA_LIMIT_IN_TEAM_MATCHING);
		return;
	}
	break;

	case TeamManager::TOR_IN_TEAM_FB:
	{
		role->NoticeNum(errornum::EN_TEAM_SPECIAL_TEAM_JOIN_LIMIT);
		return;
	}
	break;

	case TeamManager::TOR_TEAM_LIMIT:
	{
		role->NoticeNum(errornum::EN_TEAM_JOIN_LIMIT_OF_MERGE);
		return;
	}
	break;

	case TeamManager::TOR_NO_POWER:
	{
		if (is_ret)
		{
			role->NoticeNum(errornum::EN_YOU_HAVE_NO_POWER_MERGE);
		}
		return;
	}
	break;

	default:
		break;
	}
}
