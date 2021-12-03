#include "team.hpp"
#include "teammanager.hpp"

#include "obj/character/role.h"

#include "engineadapter.h"
#include "internalcomm.h"
#include "servercommon/noticenum.h"

#include "servercommon/internalprotocal/gcprotocal.h"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/commondata.hpp"

#include "protocal/msgteam.hpp"
#include "world.h"
#include "scene/scene.h"
#include "global/usercache/usercache.hpp"
#include "other/event/eventhandler.hpp"
#include "friend/teaminfo.hpp"
#include "other/capability/capability.hpp"
#include "other/cross/rolecross.h"


Team::Team()
	: m_team_index(-1), m_team_seq(0), m_must_check(false), m_member_can_invite(false), m_limit_capability(0), m_limit_level(0), m_cur_member_num(0), 
	m_leader_index(0), m_assign_mode(0), m_team_type(0), m_teamfb_mode(0), m_teamfb_layer(-1), m_teamfb_autostart(0)
{
}

Team::Team(Role *leader_user, bool must_check, bool member_can_invite, int assign_mode, int team_type)
	: m_team_index(-1), m_team_seq(0), m_must_check(must_check), m_member_can_invite(member_can_invite),m_limit_capability(0), 
	m_limit_level(0), m_cur_member_num(1), m_leader_index(0), m_assign_mode(assign_mode), m_team_type(team_type), m_teamfb_mode(0), m_teamfb_layer(-1), m_teamfb_autostart(0)
{
	m_member_list[0].user_id = leader_user->GetUserId();
	m_member_list[0].user_uuid = leader_user->GetRoleCross()->GetCrossOriginalUniqueUserID();
	m_member_list[0].join_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_member_list[0].scene_id = (NULL != leader_user->GetScene()) ? leader_user->GetScene()->GetSceneID() : 0;
	m_member_list[0].scene_key = (NULL != leader_user->GetScene()) ? leader_user->GetScene()->GetSceneKey() : 0;
	m_member_list[0].obj_id = leader_user->GetId();
}

Team::~Team()
{
	
}

void Team::SetTeamIndex(int team_index)
{
	m_team_index = team_index;
}

void Team::SetMustCheck(bool must_check)
{
	m_must_check = must_check;

	m_inviter_set.clear();

	this->SendTeamInfo();
}

void Team::SetMemberCanInvite(bool member_can_invite)
{
	m_member_can_invite = member_can_invite;

	this->SendTeamInfo();
}

void Team::SetTeamLimit(int limit_capability, int limit_level)
{
	m_limit_capability = limit_capability;
	m_limit_level = limit_level;

	this->SendTeamInfo();
}

Role * Team::GetLeader()
{
	if (m_leader_index < 0 || m_leader_index >= MAX_TEAM_MEMBER_NUM || m_member_list[m_leader_index].Invalid())
	{
		return NULL;
	}

	Role *leader_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_member_list[m_leader_index].user_id);

	return leader_role;
}

int Team::GetTeamAverageLevel()
{
	int average_level = 0;
	int team_count = 0;
	for(int i = 0; i < MAX_TEAM_MEMBER_NUM; i++)
	{
		if (!m_member_list[i].Invalid())
		{
			Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_member_list[i].user_id);
			if (NULL != temp_role)
			{
				average_level += temp_role->GetLevel();
				team_count++;
			}

		}
	}

	if (team_count > 0)
	{
		average_level = average_level / team_count;
	}

	return average_level;
}

int Team::GetMemberUidList(int *member_id_list, int max_count)
{
	if (NULL == member_id_list || max_count <= 0) return 0;

	int count = 0;
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM && count < max_count; ++i)
	{
		if (!m_member_list[i].Invalid())
		{
			member_id_list[count ++] = UidToInt(m_member_list[i].user_id);
		}
	}

	for (int i = count; i < max_count; ++i)
	{
		member_id_list[i] = 0;
	}

	return count;
}

int Team::GetMemberUUidList(long long *member_id_list, int max_count)
{
	if (NULL == member_id_list || max_count <= 0) return 0;

	int count = 0;
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM && count < max_count; ++i)
	{
		if (!m_member_list[i].Invalid())
		{
			member_id_list[count ++] = UniqueUserIDToLongLong(m_member_list[i].user_uuid);
		}
	}

	for (int i = count; i < max_count; ++i)
	{
		member_id_list[i] = 0;
	}

	return count;
}

bool Team::GetTeamInfo(GameName leader_name, int *leader_level, char *leader_prof, char *leader_sex, 
					   char *leader_vip_level, char *leader_camp, char *team_type, long long *avatar_timestamp)
{
	if (m_leader_index < 0 || m_leader_index >= MAX_TEAM_MEMBER_NUM || m_member_list[m_leader_index].Invalid())
	{
		return false;
	}

	const UserCacheNode *cache_node = UserCacheManager::Instance().GetUserNode(m_member_list[m_leader_index].user_id);
	if (NULL == cache_node)
	{
		return false;
	}

	if (NULL != leader_name) cache_node->GetName(leader_name);
	if (NULL != leader_level) *leader_level = cache_node->level;
	if (NULL != leader_prof) *leader_prof = cache_node->prof;
	if (NULL != leader_sex) *leader_sex = cache_node->sex;
	if (NULL != leader_vip_level) *leader_vip_level = cache_node->vip_level;
	if (NULL != leader_camp) *leader_camp = cache_node->camp;
	if (NULL != team_type) *team_type = m_team_type;
	if (NULL != avatar_timestamp) *avatar_timestamp = cache_node->avatar_timestamp;

	return true;
}

bool Team::IsLeader(const UserID &user_id)
{
	if (m_leader_index < 0 || m_leader_index >= MAX_TEAM_MEMBER_NUM || m_member_list[m_leader_index].Invalid())
	{
		return false;
	}

	return m_member_list[m_leader_index].user_id == user_id;
}

bool Team::IsNobodyOnline(const UserID &except_userid)
{
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (!m_member_list[i].Invalid()) 
		{
			Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_member_list[i].user_id);
			if (NULL != tmp_role && tmp_role->GetUserId() != except_userid)
			{
				return false;
			}
		}
	}

	return true;
}

bool Team::IsHasCouple()
{
	for(int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (m_member_list[i].Invalid())
		{
			continue;
		}

		Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_member_list[i].user_id);
		if (NULL == tmp_role)
		{
			continue;
		}

		if (!tmp_role->IsMarry())
		{
			continue;
		}
		
		CommonDataParam *commondata = tmp_role->GetCommonDataParam();
		if (NULL == commondata)
		{
			continue;
		}

		TeamMember * lover = this->GetTeamMember(IntToUid(commondata->lover_uid));
		if (NULL != lover)
		{
			return true;
		}
	}

	return false;
}

void Team::AutoSelectTeamLeader()
{
	unsigned int min_join_time = -1;
	int min_index = -1;

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (i != m_leader_index && !m_member_list[i].Invalid())
		{
			Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_member_list[i].user_id);
			if (NULL != tmp_role)
			{
				if (m_member_list[i].join_time < min_join_time)
				{
					min_join_time = m_member_list[i].join_time;
					min_index = i;
				}
			}
		}
	}

	if (0 <= min_index && min_index < MAX_TEAM_MEMBER_NUM)
	{
		m_leader_index = min_index;
		Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_member_list[m_leader_index].user_id);
		if (NULL != tmp_role)
		{
			tmp_role->NoticeNum(noticenum::NT_TEAM_CHANGE_CAPTAIN_SUCC);

			Protocol::SCTeamLeaderChange team_leader;
			team_leader.user_id = tmp_role->GetUID();
			tmp_role->GetName(team_leader.user_name);
			
			for(int i = 0; i < MAX_TEAM_MEMBER_NUM; i++)
			{
				if (i != min_index)
				{
					Role *team_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_member_list[i].user_id);
					if (NULL != team_role)
					{
						EngineAdapter::Instance().NetSend(team_role->GetNetId(), (const char*)&team_leader, sizeof(team_leader));
					}
				}
			}
		}
	}
}

bool Team::ChangeTeamLeader(const UserID &user_id)
{
	int member_index = 0;
	TeamMember *team_member = this->GetTeamMember(user_id, &member_index);
	if (NULL == team_member) return false;

	m_leader_index = member_index;

	this->SendTeamInfo();

	Role *leader_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_member_list[m_leader_index].user_id);
	if (NULL != leader_role)
	{
		Protocol::SCTeamLeaderChange team_leader;
		team_leader.user_id = leader_role->GetUID();
		leader_role->GetName(team_leader.user_name);

		for(int i = 0; i < MAX_TEAM_MEMBER_NUM; i++)
		{
			if (i != m_leader_index)
			{
				Role *team_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_member_list[i].user_id);
				if (NULL != team_role)
				{
					EngineAdapter::Instance().NetSend(team_role->GetNetId(), (const char*)&team_leader, sizeof(team_leader));
				}
			}
		}
	}

	return true;
}

bool Team::CheckTeamLimit(Role *user)
{
	if (NULL == user)
	{
		return false;
	}

	if (0 != m_limit_capability && user->GetCapability()->GetTotalCapability() < m_limit_capability)
	{
		return false;
	}

	if (0 != m_limit_level && user->GetLevel() < m_limit_level)
	{
		return false;
	}

	return true;
}

bool Team::AddTeamMember(Role *user)
{
	if (m_cur_member_num >= MAX_TEAM_MEMBER_NUM || NULL == user) return false;

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (m_member_list[i].Invalid())
		{
			m_member_list[i].user_id = user->GetUserId();
			m_member_list[i].user_uuid = user->GetRoleCross()->GetCrossOriginalUniqueUserID();
			m_member_list[i].join_time = (unsigned int)(EngineAdapter::Instance().Time());
			m_member_list[i].scene_id = (NULL != user->GetScene() ? user->GetScene()->GetSceneID() : 0);
			m_member_list[i].scene_key = (NULL != user->GetScene() ? user->GetScene()->GetSceneKey() : 0);
			m_member_list[i].obj_id = user->GetId();
			
			++ m_cur_member_num;
			user->GetTeamInfo()->OnSelfJoinTeam(m_team_index, m_team_seq);

			this->SendTeamInfo();

			Protocol::SCJoinTeam jt;
			jt.user_id = user->GetUID();
			user->GetName(jt.user_name);
			this->BroadcastToAllMember((const char*)&jt, sizeof(jt));

			return true;
		}
	}

	return false;
}

bool Team::RemoveMember(const UserID &user_id, bool is_kick_off)
{
	int member_index = 0;
	TeamMember *team_member = this->GetTeamMember(user_id, &member_index);
	if (NULL == team_member || member_index < 0 || member_index >= MAX_TEAM_MEMBER_NUM) return false;

	/*ggprotocal::RoleLeaveTeam lt;
	GlobalServer::GetInstGlobalServer()->SendToUserGameWorld(m_member_list[member_index].scene_id, user_id, (const char *)&lt, sizeof(lt));*/

	//this->DeleteMemberFromChannel(member_index);

	m_member_list[member_index].Reset();
	-- m_cur_member_num;
	
	Role *user = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
	if (NULL != user)
	{
		user->GetTeamInfo()->OnSelfLeaveTeam();

		Protocol::SCOutOfTeam msg_oft;
		msg_oft.reason = (is_kick_off ? Protocol::OFT_KICK_OFF : Protocol::OFT_SECEDE);
		msg_oft.user_id = user->GetUID();
		user->GetName(msg_oft.user_name);
		EngineAdapter::Instance().NetSend(user->GetNetId(), (const char *)&msg_oft, sizeof(msg_oft));

		this->BroadcastToAllMember((const char *)&msg_oft, sizeof(msg_oft));
	}

	if (member_index == m_leader_index) 
	{
		this->AutoSelectTeamLeader();
	}
	
	this->SendTeamInfo();

	return true;
}

bool Team::OnUserChangeScene(const UserID &user_id, int scene_id, int scene_key, ObjID obj_id)
{
	TeamMember *team_member = this->GetTeamMember(user_id);
	if (NULL == team_member) return false;

	if (team_member->scene_id == scene_id && team_member->obj_id == obj_id && team_member->scene_key == scene_key)
	{
		return true;
	}

	team_member->scene_id = scene_id;
	team_member->scene_key = scene_key;
	team_member->obj_id = obj_id;

	this->SendTeamInfo();

	return true;
}

bool Team::ChangeAssignMode(int assign_mode)
{
	if (assign_mode <= TEAM_ASSIGN_MODE_INVALID || assign_mode >= TEAM_ASSIGN_MODE_MAX) return false;

	m_assign_mode = assign_mode;

	this->SendTeamInfo();

	return true;
}

bool Team::ChangeTeamFbTeamType(int team_type)
{
	if (team_type < TeamManager::TEAM_TYPE_DEFAULT || team_type >= TeamManager::TEAM_TYPE_MAX) return false;

	m_team_type = team_type;
	this->SendTeamInfo();
	return true;
}

bool Team::ChangeTeamFbMode(int mode)
{
	if (mode < 0 || mode >= TEAM_FB_MAX_MODE) return false;

	m_teamfb_mode = mode;

	this->SendTeamInfo();
	return true;
}

bool Team::ChangeTeamFbLayer(int layer)
{
	if (layer < 0) return false;

	m_teamfb_layer = layer;

	this->SendTeamInfo();
	return true;
}

bool Team::ChangeTeamFbAutoStart(int auto_start)
{
	if (auto_start != 0 && auto_start != 1)
	{
		return false;
	}
	m_teamfb_autostart = auto_start;
	this->SendTeamInfo();
	return true;
}

bool Team::ChangeMenberFbRoomIsRead(const UserID &user_id, int is_ready)
{
	int member_index = 0;
	TeamMember *team_member = this->GetTeamMember(user_id, &member_index);
	if (NULL == team_member) return false;

	team_member->fbroom_ready = is_ready;
	this->SendTeamInfo();
	return true;
}

void Team::BroadcastToAllMember(const char *data, int len)
{
	if (NULL == data || len <= 0)
	{
		return;
	}

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (!m_member_list[i].Invalid()) 
		{
			Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_member_list[i].user_id);
			if (NULL != tmp_role)
			{
				EngineAdapter::Instance().NetSend(tmp_role->GetNetId(), data, len);
			}
		}
	}
}

bool Team::OnUserBaseInfoChange(const UserID &user_id)
{
	this->SendTeamInfo();
	return true;
}

bool Team::OnUserLogin(const UserID &user_id, GSNetID net_id)
{
	int member_index = -1;
	TeamMember *team_member = this->GetTeamMember(user_id, &member_index);
	if (NULL == team_member) return false;

	this->SendTeamInfo();

	return true;
}

bool Team::OnUserLogout(const UserID &user_id)
{
	int member_index = -1;
	TeamMember *team_member = this->GetTeamMember(user_id, &member_index);
	if (NULL == team_member) return false;

	if (this->IsLeader(user_id))
	{
		this->AutoSelectTeamLeader();
	}

	this->SendTeamInfo();

	return true;
}

void Team::Dismiss()
{
	GSNetID netid_list[MAX_TEAM_MEMBER_NUM];

	int count = 0;
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM && count < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (m_member_list[i].Invalid()) continue;

		Role *user = World::GetInstWorld()->GetSceneManager()->GetRole(m_member_list[i].user_id);
		if (NULL != user)
		{
			user->GetTeamInfo()->OnSelfLeaveTeam();

			netid_list[count ++] = user->GetNetId();
		}
	}

	m_cur_member_num = 0;

	Protocol::SCOutOfTeam msg_oft;
	msg_oft.reason = Protocol::OFT_DISMISS;
	msg_oft.user_id = UidToInt(INVALID_USER_ID);
	msg_oft.user_name[0] = '\0';
	EngineAdapter::Instance().NetMulSend(netid_list, count, (const char *)&msg_oft, sizeof(Protocol::SCOutOfTeam));
}

void Team::Invite(const UserID &user_id)
{
	m_inviter_set.insert(user_id);
}

bool Team::RemoveIfInvite(const UserID &user_id)
{
	InviterSet::iterator it = m_inviter_set.find(user_id);
	if (it != m_inviter_set.end())
	{
		m_inviter_set.erase(it);
		return true;
	}

	return false;
}

int Team::GetTeamLocal(int *scene_key)
{
	if (NULL == scene_key) return -1;

	if (m_leader_index < 0 || m_leader_index >= MAX_TEAM_MEMBER_NUM || m_member_list[m_leader_index].Invalid())
	{
		*scene_key = -1;
		return -1;
	}

	*scene_key = m_member_list[m_leader_index].scene_key;

	return m_member_list[m_leader_index].scene_id;
}

int Team::GetMemberSceneId(const UserID &user_id)
{
	TeamMember *member = this->GetTeamMember(user_id);
	if (NULL == member)
	{
		return 0;
	}

	return member->scene_id;
}

void Team::SendTeamInfo()
{
	UNSTD_STATIC_CHECK(Protocol::MAX_TEAM_MEMBER_COUNT >= MAX_TEAM_MEMBER_NUM);

	Protocol::SCTeamInfo ti;
	ti.team_index = m_team_index;
	ti.team_leader_index = 0;
	ti.assign_mode = static_cast<char>(m_assign_mode);
	ti.must_check = (m_must_check ? 1 : 0);
	ti.member_can_invite = (m_member_can_invite ? 1 : 0);
	ti.limit_capability = m_limit_capability;
	ti.limit_level = m_limit_level;
	ti.team_type = m_team_type;
	ti.teamfb_mode = m_teamfb_mode;
	ti.teamfb_layer = m_teamfb_layer;
	ti.reserve_sh = 0;
	ti.member_count = m_cur_member_num;

	GSNetID	netid_list[MAX_TEAM_MEMBER_NUM];
	UserID user_id_list[MAX_TEAM_MEMBER_NUM];

	int member_count = 0;
	int net_count = 0;

	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (m_member_list[i].Invalid()) continue;

		Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_member_list[i].user_id);
		if (NULL != tmp_role)
		{
			netid_list[net_count ++] = tmp_role->GetNetId();
		}

		const UserCacheNode *cache_node = UserCacheManager::Instance().GetUserNode(m_member_list[i].user_id);
		if (NULL == cache_node) continue;

		ti.team_member[member_count].level = cache_node->level;
		ti.team_member[member_count].role_id = UidToInt(m_member_list[i].user_id);
		ti.team_member[member_count].role_uuid = cache_node->uuid;
		cache_node->GetName(ti.team_member[member_count].name);
		ti.team_member[member_count].scene_id = m_member_list[i].scene_id;
		ti.team_member[member_count].scene_key = m_member_list[i].scene_key;
		ti.team_member[member_count].fbroom_ready = char(m_member_list[i].fbroom_ready);
		ti.team_member[member_count].is_online = (UserCacheManager::Instance().IsUserOnline(m_member_list[i].user_id) ? 1 : 0);
		ti.team_member[member_count].prof = cache_node->prof;
		ti.team_member[member_count].camp = cache_node->camp;
		ti.team_member[member_count].sex = cache_node->sex;
		ti.team_member[member_count].capability = cache_node->capability;
		ti.team_member[member_count].avatar_timestamp = cache_node->avatar_timestamp;

		user_id_list[member_count] = m_member_list[i].user_id;

		if (i == m_leader_index)
		{
			ti.team_leader_index = (char)member_count;
		}

		++ member_count;
	}

	ti.member_count = member_count;

	int sendlen = sizeof(Protocol::SCTeamInfo) - (Protocol::MAX_TEAM_MEMBER_COUNT - member_count) * sizeof(Protocol::TeamMemberInfo);
	EngineAdapter::Instance().NetMulSend(netid_list, net_count, (const char *)&ti, sendlen);
}

Team::TeamMember * Team::GetTeamMember(const UserID &user_id, int *member_index)
{
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		if (!m_member_list[i].Invalid() && m_member_list[i].user_id == user_id)
		{
			if (NULL != member_index) *member_index = i;
			
			return &m_member_list[i];
		}
	}

	return NULL;
}

Team::TeamMember * Team::GetTeamMember(int index)
{
	if (index < 0 || index >= MAX_TEAM_MEMBER_NUM)
	{
		return NULL;
	}

	if (m_member_list[index].Invalid())
	{
		return NULL;
	}

	return &m_member_list[index];
}

