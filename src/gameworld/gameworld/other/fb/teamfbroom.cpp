#include "obj/character/role.h"

#include "scene/scene.h"
#include "scene/scenemanager.h"

#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "global/team/teammanager.hpp"
#include "teamfbroom.hpp"
#include "protocal/msgfb.h"
#include "servercommon/servercommon.h"
#include "npc/transmit/fbmanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "other/fb/migongxianfuconfig.hpp"
#include "other/fb/yaoshoujitanteamfbconfig.hpp"
#include "other/teamequipfb/teamequipfbconfig.hpp"
#include "servercommon/errornum.h"
#include "gameworld/world.h"
#include "friend/teaminfo.hpp"
#include "other/capability/capability.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/fb/roledailyfb.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "fbequipconfig.hpp"

TeamFbRoom::TeamFbRoom()
{

}

TeamFbRoom::~TeamFbRoom()
{

}

TeamFbRoom & TeamFbRoom::Instance()
{
	static TeamFbRoom key; 
	return key; 
}

void TeamFbRoom::ReqOperate(Role *role, Protocol::CSTeamFbRoomOperate *cmd)
{
	if (NULL == role || NULL == cmd) return;

	if (REQ_ROOM_LIST == cmd->operate_type)
	{
		this->SyncRoomList(role, cmd->param1, cmd->param2, cmd->param3);
	}
	else if (CREATE_ROOM == cmd->operate_type)
	{
		this->CreateRoom(role, cmd->param1, cmd->param2, cmd->param3, cmd->param4);
	}
	else if (JOIN_ROOM == cmd->operate_type)
	{
		this->JoinRoom(role, cmd->param1, cmd->param2, cmd->param3);
	}
	else if (START_ROOM == cmd->operate_type)
	{
		this->Start(role);
	}
	else if (EXIT_ROOM == cmd->operate_type)
	{
		this->Exit(role);
	}
	else if (CHANGE_MODE == cmd->operate_type)
	{
		this->ChangeMode(role, cmd->param1, cmd->param2, cmd->param3);
	}
	else if (KICK_OUT == cmd->operate_type)
	{
		this->KickOut(role, cmd->param1);
	}
	else if (ENTER_AFFIRM == cmd->operate_type)
	{
		this->EnterAffirm(role, cmd->param1);
	}
}

void TeamFbRoom::SyncRoomList(Role *role, int team_type, int mode, int layer)
{
	if (NULL == role) return;

	static Protocol::SCTeamFbRoomList cmd;
	int count = 0;

	RAObjList<Team*> &team_list = TeamManager::Instance().GetTeamList();

	for (RAObjList<Team*>::Iterator iter = team_list.Beg(); iter != team_list.End(); ++ iter)
	{
		Team *team = team_list[iter.m_index];
 		if (NULL == team)
 		{
 			continue;
 		}
 
 		Role *leader = team->GetLeader();
 		if (team_type == team->GetTeamType() && mode == team->GetTeamFbMode() && 
			layer == team->GetTeamFbLayer() && NULL != leader && 
 			count < FB_TEAM_ROOM_MAX_NUM && leader->GetScene()->InStaticScene())
 		{
 			Protocol::SCTeamFbRoomList::TeamFbRoomItem *room_item = &cmd.room_list[count];
 			room_item->Reset();
 
 			room_item->team_index = team->GetTeamIndex();
 			leader->GetName(room_item->leader_name);
 			room_item->leader_capability = leader->GetCapability()->GetTotalCapability();
 			room_item->limit_capability = team->GetLimitCapability();
 			room_item->avatar_key_big = 0;
 			room_item->avatar_key_small = 0;
 			room_item->mode = team->GetTeamFbMode();
 			room_item->menber_num = team->GetMemberNum();
 			room_item->leader_sex = leader->GetSex();
 			room_item->leader_prof = leader->GetProf();
 			room_item->leader_uid = leader->GetUID();
			room_item->layer = team->GetTeamFbLayer();
			room_item->assign_mode = team->GetAssignMode();
			room_item->is_must_check = team->IsMustCheck() ? 1 : 0; 
			room_item->reserve_2 = 0;
 
 			++ count;
 		}
	}
	
	cmd.team_type = team_type;
	cmd.count = count;
	unsigned int sendlen = sizeof(cmd) - sizeof(cmd.room_list[0]) * (FB_TEAM_ROOM_MAX_NUM - cmd.count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&cmd, sendlen);
}

void TeamFbRoom::CreateRoom(Role *role, int team_type, int mode, int limit_capability,  int layer, int is_auto_start)
{
	if (NULL == role) return;

	if (team_type <= TeamManager::TEAM_TYPE_DEFAULT || team_type >= TeamManager::TEAM_TYPE_MAX)
	{
		return;
	}

	if (mode < TEAM_FB_MODE_EASY || mode >= TEAM_FB_MAX_MODE)
	{
		return;
	}

	if (is_auto_start != 0 && is_auto_start != 1)
	{
		return;
	}

	if (limit_capability < 0)
	{
		return;
	}

	if (!this->CheckIsCanEnterFbOnJoin(role, this->GetFbTypeByTeamType(team_type), layer, OperateType::CREATE_ROOM))
	{
		return;
	}

	int team_index = TeamManager::Instance().CreateTeam(role, false, false, TEAM_ASSIGN_MODE_RANDOM, team_type);
	
	Team *team = TeamManager::Instance().GetTeam(team_index);
	if (NULL != team)
	{
		team->SetTeamLimit(limit_capability, 1);
		team->ChangeTeamFbMode(mode);
		team->ChangeTeamFbLayer(layer);
		team->ChangeTeamFbAutoStart(is_auto_start);
		team->ChangeMenberFbRoomIsRead(role->GetUserId(), DEFAULT_STAE);
	}
}

void TeamFbRoom::JoinRoom(Role *role, int team_type, int team_index, int layer)
{
	if (NULL == role) return;

	// role有队伍的话，则尝试合并
	if (role->GetTeamInfo()->InTeam())
	{
		Team *role_team = TeamManager::Instance().GetTeam(role->GetTeamInfo()->GetTeamIndex());
		Team *join_team = TeamManager::Instance().GetTeam(team_index);
		if (role_team && join_team)
		{
			int check_ret = TeamManager::Instance().CheckCanMergeTeam(role, join_team, true);
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
				Role *leader_role = role_team->GetLeader();
				if (nullptr != leader_role)
				{
					if (TeamManager::Instance().DismissTeam(role->GetTeamInfo()->GetTeamIndex(), leader_role))
					{
						// 将role的队伍成员全部加入接受申请的队伍
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
				TeamManager::Instance().SendCheckMergeResultToRole(role, check_ret, true);
			}
		}
		else
		{
			role->NoticeNum(errornum::EN_TEAM_IS_NOT_EXIST);
		}

		return;
	}

	// role 没有队伍，则默认逻辑
	if (!this->CheckIsCanEnterFbOnJoin(role, this->GetFbTypeByTeamType(team_type), layer, OperateType::JOIN_ROOM))
	{
		return;
	}

	Team *team = TeamManager::Instance().GetTeam(team_index);
	if (NULL == team)
	{
		return;
	}

	TeamManager::Instance().AddTeamMember(team->GetLeader(), team_index, role);
}

void TeamFbRoom::Start(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	Team *team = role->GetTeamInfo()->GetMyTeam();
	if (NULL == team)
	{
		return;
	}

	// 只有队长才有资格进入
	if (team->GetLeader() != role)
	{
		role->NoticeNum(errornum::EN_TEAM_ROOM_LEADER_LIMIT);
		return;
	}

	for (int j = 0;j < MAX_TEAM_MEMBER_NUM; ++ j)
	{
		Team::TeamMember *member = team->GetTeamMember(j);
		if (NULL != member)
		{
			member->fbroom_ready = DEFAULT_STAE;
		}
	}

	team->ChangeMenberFbRoomIsRead(role->GetUserId(), AGREE_STATE);

	// 已准备人数等于队伍人数就可以进入
	if (team->GetMemberNum() == 1)
	{
		int fb_type = this->GetFbTypeByTeamType(team->GetTeamType());
		if (FBManager::FBCT_YAOSHOUJITANG_TEAM == fb_type || FBManager::FBCT_MIGONGXIANFU_TEAM == fb_type)
		{
			FBManager::Instance().EnterFB(team->GetLeader(), fb_type, team->GetTeamFbMode(), 0, 0);
		}
		else if (FBManager::FBCT_TEAM_EQUIP_FB == fb_type)
		{
			FBManager::Instance().EnterFB(team->GetLeader(), fb_type, team->GetTeamFbLayer(), 0, 0);
		}
		else if (FBManager::FBCT_DAILY_FB == fb_type)
		{
			FBManager::Instance().EnterFB(team->GetLeader(), fb_type, DAILYFB_TYPE_EXP, 0, 0);
		}
		else if (FBManager::FBCT_TOWERDEFEND_TEAM == fb_type)
		{
			FBManager::Instance().EnterFB(team->GetLeader(), fb_type, 0, 0, 0);
		}
		else if (FBManager::FBCT_EQUIP_TEAM_FB == fb_type)
		{
			FBManager::Instance().EnterFB(team->GetLeader(), fb_type, 0, 0, 0);
		}
	}
	else
	{
		int fb_type = this->GetFbTypeByTeamType(team->GetTeamType());
		if (FBManager::FBCT_YAOSHOUJITANG_TEAM == fb_type || FBManager::FBCT_MIGONGXIANFU_TEAM == fb_type||
			FBManager::FBCT_TOWERDEFEND_TEAM == fb_type || FBManager::FBCT_EQUIP_TEAM_FB == fb_type)
		{
			
		}
		else if (FBManager::FBCT_TEAM_EQUIP_FB == fb_type)
		{
			if (!FBManager::Instance().CheckTeamEnterEquipFb(role, team->GetTeamFbLayer()))
			{
				return;
			}
		}
		else if (FBManager::FBCT_DAILY_FB == fb_type)
		{
			if (!FBManager::Instance().CheckTeamEnterDailyFB(role, DAILYFB_TYPE_EXP))
			{
				return;
			}
		}
		else if (FBManager::FBCT_QINGYUAN == fb_type)
		{
			if (!role->GetQingyuan()->CanJoinFB(true))
			{
				return;
			}
		}
		else
		{
			return;
		}

		static Protocol::SCTeamFbRoomEnterAffirm cmd;
		cmd.team_type = team->GetTeamType();
		cmd.mode = team->GetTeamFbMode();
		cmd.layer = team->GetTeamFbLayer();
		
		// 判断队员能否开始
		for (int j = 0;j < MAX_TEAM_MEMBER_NUM; ++ j)
		{
			Team::TeamMember *temp_member = team->GetTeamMember(j);
			if (NULL == temp_member)
			{
				continue;
			}

			Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(temp_member->user_id);
			if (NULL == temp_role)
			{
				continue;
			}

			if (FBManager::FBCT_QINGYUAN == fb_type && !temp_role->GetQingyuan()->CanJoinFB(true))
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_marry_fb_can_not_enter,
					UidToInt(temp_role->GetUserId()), temp_role->GetName(), (int)temp_role->GetCamp());

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

		team->BroadcastToAllMember((const char*)&cmd, sizeof(cmd));
	}
}

void TeamFbRoom::EnterAffirm(Role *role, int is_ready)
{
	if (NULL == role || (is_ready != AGREE_STATE && is_ready != REJECT_STATE))
	{
		return;
	}

	Team *team = role->GetTeamInfo()->GetMyTeam();
	if (NULL == team)
	{
		return;
	}

	team->ChangeMenberFbRoomIsRead(role->GetUserId(), is_ready);

	int ready_num = 0;

	for (int i = 0;i < MAX_TEAM_MEMBER_NUM; ++ i)
	{
		Team::TeamMember *member = team->GetTeamMember(i);
		if (NULL != member && member->fbroom_ready == AGREE_STATE)
		{
			++ ready_num;
		}
	}

	if (ready_num >= team->GetMemberNum())
	{
		int fb_type = this->GetFbTypeByTeamType(team->GetTeamType());

		if (FBManager::FBCT_YAOSHOUJITANG_TEAM == fb_type || FBManager::FBCT_MIGONGXIANFU_TEAM == fb_type)
		{
			FBManager::Instance().EnterFB(team->GetLeader(), fb_type, team->GetTeamFbMode(), 0, 0);
		}
		else if (FBManager::FBCT_TEAM_EQUIP_FB == fb_type)
		{
			FBManager::Instance().EnterFB(team->GetLeader(), fb_type, team->GetTeamFbLayer(), 0, 0);
		}
		else if (FBManager::FBCT_DAILY_FB == fb_type)
		{
			FBManager::Instance().EnterFB(team->GetLeader(), fb_type, DAILYFB_TYPE_EXP, 1, 0);
		}
		else if (FBManager::FBCT_TOWERDEFEND_TEAM == fb_type)
		{
			FBManager::Instance().EnterFB(team->GetLeader(), fb_type, 0, 0, 0);
		}
		else if (FBManager::FBCT_EQUIP_TEAM_FB == fb_type)
		{
			FBManager::Instance().EnterFB(team->GetLeader(), fb_type, 0, 0, 0);
		}
		else if (FBManager::FBCT_QINGYUAN == fb_type)
		{
			FBManager::Instance().EnterFB(team->GetLeader(), fb_type, 0, 0, 0);
		}
	}
}

void TeamFbRoom::Exit(Role *role)
{
	TeamManager::Instance().OnExitTeam(role);
}

void TeamFbRoom::ChangeMode(Role *role, int team_type, int mode, int layer)
{
	if (NULL == role) return;

	if (mode < TEAM_FB_MODE_EASY || mode >= TEAM_FB_MAX_MODE)
	{
		return;
	}

	Team *team = role->GetTeamInfo()->GetMyTeam();
	if (NULL == team || team->GetLeader() != role)
	{
		return;
	}

	team->ChangeTeamFbTeamType(team_type);
	team->ChangeTeamFbMode(mode);
	team->ChangeTeamFbLayer(layer);
}

void TeamFbRoom::KickOut(Role *role, int role_id)
{
	static Protocol::CSKickOutOfTeam cmd;
	cmd.role_id = role_id;

	TeamManager::Instance().OnKickOutOfTeam(role, &cmd);
}

int TeamFbRoom::GetFbTypeByTeamType(int team_type)
{
	if (TeamManager::TEAM_TYPE_YAOSHOUJITANG == team_type)
	{
		return FBManager::FBCT_YAOSHOUJITANG_TEAM;
	}
	else if (TeamManager::TEAM_TYPE_TEAM_MIGONGXIANFU == team_type)
	{
		return FBManager::FBCT_MIGONGXIANFU_TEAM;
	}
	else if (TeamManager::TEAM_TYPE_TEAM_EQUIP_FB == team_type)
	{
		return FBManager::FBCT_TEAM_EQUIP_FB;
	}
	else if (TeamManager::TEAM_TYPE_TEAM_DAILY_FB == team_type)
	{
		return FBManager::FBCT_DAILY_FB;
	}
	else if (TeamManager::TEAM_TYPE_TEAM_TOWERDEFEND == team_type)
	{
		return FBManager::FBCT_TOWERDEFEND_TEAM;
	}
	else if (TeamManager::TEAM_TYPE_EQUIP_TEAM_FB == team_type)
	{
		return FBManager::FBCT_EQUIP_TEAM_FB;
	}
	else if (TeamManager::TEAM_TYPE_QINGYUAN_FB == team_type)
	{
		return FBManager::FBCT_QINGYUAN;
	}

	return 0;
}

bool TeamFbRoom::CheckIsCanEnterFbOnJoin(Role *role, int fb_type, int param_1, int check_reason)
{
	if (NULL == role) return false;

	// 状态检查
	if (!role->GetRoleStatusManager()->CanEnterFB(true, true))
	{
		return false;
	}

	// 等级检查
	if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinFB(fb_type, role))
	{
		role->NoticeNum(errornum::EN_FB_ROLE_LEVEL_LIMIT);
		return false;
	}

	int day_count_id = 0;
	int free_join_times = 0;

	if (FBManager::FBCT_YAOSHOUJITANG_TEAM == fb_type)  // 妖兽祭坛组队本
	{
		day_count_id = DayCounter::DAYCOUNT_ID_YAOSHOUJITAN_JOIN_TIMES;
		const YaoShouJiTanTeamFBModeConfig *mode_cfg = LOGIC_CONFIG->GetYaoShouJiTanTeamFbCfg().GetFbModeCfg(TEAM_FB_MODE_EASY);
		if (NULL != mode_cfg)
		{
			free_join_times = mode_cfg->free_join_times;
		}
	}
	else if (FBManager::FBCT_MIGONGXIANFU_TEAM == fb_type)
	{
		day_count_id = DayCounter::DAYCOUNT_ID_MIGOGNXIANFU_JOIN_TIMES;
		const MiGongXianFuModeItemCfg *mode_cfg = LOGIC_CONFIG->GetMiGongXianFuTeamFbCfg().GetModeItemCfg(TEAM_FB_MODE_EASY);
		if (NULL != mode_cfg)
		{
			free_join_times = mode_cfg->free_join_times;
		}
	}
	else if (FBManager::FBCT_TEAM_EQUIP_FB == fb_type)
	{
		const TeamEquipFBLayerCfg *fb_cfg = LOGIC_CONFIG->GetTeamEquipFBConfig().GetTeamEquipFBLayerCfg(param_1);
		if (NULL == fb_cfg)
		{
			return false;
		}

		if (role->GetLevel() < fb_cfg->level_limit)
		{
			return false;
		}
	}
	else if (FBManager::FBCT_DAILY_FB == fb_type)
	{
		if (!role->GetRoleDailyFb()->CanEnter(DAILYFB_TYPE_EXP))
		{
			return false;
		}
	}
	else if (FBManager::FBCT_TOWERDEFEND_TEAM == fb_type)
	{
		return true;
	}
	else if (FBManager::FBCT_EQUIP_TEAM_FB == fb_type)
	{
		day_count_id = DayCounter::DAYCOUNT_ID_TEAM_EQUIP_FB_JOIN_TIMES;
		free_join_times = LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().join_times;
	}
	else
	{
		return false;
	}
	
	if (FBManager::FBCT_YAOSHOUJITANG_TEAM == fb_type || FBManager::FBCT_MIGONGXIANFU_TEAM == fb_type ||
		FBManager::FBCT_MIGONGXIANFU_TEAM == fb_type || FBManager::FBCT_EQUIP_TEAM_FB == fb_type)
	{
		if (day_count_id <= 0 || free_join_times <= 0)
		{
			return false;
		}

		if ((check_reason == OperateType::JOIN_ROOM) && (FBManager::FBCT_EQUIP_TEAM_FB == fb_type))
		{
			return true;               // 加入房间不检查次数
		}

		//次数检查
		if (role->GetDayCounter()->GetDayCount(day_count_id) >= free_join_times)
		{
			role->NoticeNum(errornum::EN_FB_DAYTIMES_NOTENOUGH);
			return false;
		}
	}

	return true;
}
