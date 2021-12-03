#include "system.hpp"
#include "systemdef.hpp"

#include "internalcomm.h"
#include "engineadapter.h"
#include "global/rmibackobjdef.h"

#include "servercommon/darmi/rmisystemclient.h"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/internalprotocal/gcprotocal.h"
#include "global/offlineregister/offlineregistermanager.hpp"

#include "protocal/msgother.h"
#include "world.h"
#include "utility/configpath.h"

#include "global/mail/mailmanager.hpp"
#include "obj/character/role.h"
#include "other/cmdhandler/cmdhandler.hpp"
#include "gamelog.h"
#include "global/activity/activitymanager.hpp"
#include "global/specialactivity/specialactivitymanager.hpp"
#include "config/reloadconfigmanager.hpp"
#include "global/usercache/usercache.hpp"
#include "other/event/eventhandler.hpp"
#include "other/cmdhandler/cmdhandler.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/cross/crossmanager.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "item/knapsack.h"
#include "other/roleactivity/roleactivity.hpp"
#include "other/monthcard/monthcard.h"
#include "global/worldstatus2/worldstatus2.hpp"
#include "global/worldstatus3/worldstatus3.hpp"
#include "gameworld/config/logicconfigmanager.hpp"
#include "gameworld/config/agentadapterconfig.hpp"
#include "gameworld/chat/chatmanager.h"
#include "global/crossrandactivity/crossramanagerlocal.hpp"
#include "servercommon/serverconfig/openserverconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "gameworld/systemcmddef.h"
#include "servercommon/serverconfig/localconfig.hpp"

System::System()
	: m_next_get_public_notice(0), m_next_get_cmd(0), 
	m_curr_normal_notice_index(0), m_next_normal_notice_time(0), m_normal_notice_count(0)
{
	// memset(m_normal_noticelist, 0, sizeof(m_normal_noticelist)); 该结构有string 不允许memset 
}

System::~System()
{

}

void System::Init()
{
	m_next_get_public_notice = m_next_get_cmd = EngineAdapter::Instance().Time();
}

void RMIGetPublicNoticeBackObjectImpl::GetPublicNoticeRet(int ret, const PublicNoticeList &notice_list)
{
	if (ret != 0) return;
	if (notice_list.count <= 0 || notice_list.count > PublicNoticeList::MAX_PUBLICNOTICE_NUM) return;

	bool normal_notice_is_empty = System::Instance().NormalNoticeIsEmpty();

	for (int i = 0; i < notice_list.count; ++i)
	{
		if (notice_list.notice_list[i].urgent)
		{
			const char *notice = notice_list.notice_list[i].notice.c_str();
			int length = static_cast<int>(notice_list.notice_list[i].notice.size());
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(notice, length, SYS_MSG_CENTER_ROLL, notice_list.notice_list[i].type, notice_list.notice_list[i].color);
		}
		else if (normal_notice_is_empty)
		{
			System::Instance().AddNormalNotice(notice_list.notice_list[i]);
		}
	}
}

void System::Update(unsigned long interval, time_t now_second)
{
	if (!WorldStatus::Instance().IsLoadFinish()||
		!WorldStatus2::Instance().IsLoadFinish()||
		!WorldStatus3::Instance().IsLoadFinish()) return;

	if (now_second > m_next_get_public_notice)
	{
		this->StartPublicNoticeCheck();
		m_next_get_public_notice = now_second + 30;
	}

	if (now_second > m_next_get_cmd)
	{
		this->StartCmdCheck();
		m_next_get_cmd = now_second + 10;
	}

	this->UpdateNormalNotice(now_second);
}

void System::AddNormalNotice(const PublicNoticeList::NoticeAttr &notice_attr)
{
	if (m_normal_notice_count >= 0 && m_normal_notice_count < PublicNoticeList::MAX_PUBLICNOTICE_NUM)
	{	
		m_normal_noticelist[m_normal_notice_count++] = notice_attr;
	}
}

void System::UpdateNormalNotice(time_t now_second)
{
	if (now_second > m_next_normal_notice_time && m_normal_notice_count > 0 && m_normal_notice_count <= PublicNoticeList::MAX_PUBLICNOTICE_NUM)
	{
		for (; m_curr_normal_notice_index < m_normal_notice_count; m_curr_normal_notice_index++)
		{
			const PublicNoticeList::NoticeAttr &notice_attr = m_normal_noticelist[m_curr_normal_notice_index];
			if (now_second > notice_attr.begin_time && now_second < notice_attr.end_time)
			{
				++ m_curr_normal_notice_index;
				m_next_normal_notice_time = now_second + notice_attr.send_interval;

				{
					const char *notice = notice_attr.notice.c_str();
					int length = static_cast<int>(notice_attr.notice.size());
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(notice, length, SYS_MSG_CENTER_ROLL, notice_attr.type, notice_attr.color);
				}

				break;
			}
		}

		if (m_curr_normal_notice_index >= m_normal_notice_count)
		{
			m_normal_notice_count = 0; m_curr_normal_notice_index = 0;
		}
	}
}

void System::ResetNotice()
{
	m_normal_notice_count = 0; m_curr_normal_notice_index = 0;
	m_next_get_public_notice = EngineAdapter::Instance().Time() - 30;
}

void RMIGetCmdBackObjectImpl::GetCmdRet(int ret, const CmdList &cmd_list)
{
	if (0 != ret) return;

	for (int i = 0; i < cmd_list.count && i < CmdList::MAX_CMD_NUM; ++i)
	{
		System::Instance().OnCmd(cmd_list.cmd_list[i].cmd_id, cmd_list.cmd_list[i].creator, cmd_list.cmd_list[i].cmd);
	}
}

#define GET_ROLE_CMD_PARRM() \
	int role_id = 0;\
	int param1 = 0, param2 = 0;\
	bool succ = false;\
	if (!(iss >> role_id>> param1 >> param2).fail())\
	{\
		if (NULL != World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id)))\
		{\
			succ = true;\
			gamelog::g_log_cmd.printf(LL_INFO, "Succ [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());\
		}\
		else\
		{\
			gamelog::g_log_cmd.printf(LL_INFO, "Fail [%d][%s] %s. Cant Find User.", cmd_id, creator.c_str(), cmd.c_str());\
		}\
	}\
	else\
	{\
		gamelog::g_log_cmd.printf(LL_INFO, "Fail [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());\
	}

void System::OnCmd(int cmd_id, const std::string &creator, const std::string &cmd)
{
	// 貌似前两个参数都没什么作用，只有最后参数cmd才是包含了要处理的命令
	// 由于cmd包含了命令字符串，个别可能还有参数，因此定义格式为空格分隔，
	// 比如 forbidtalk 1234 ，其中forbidtalk为命令，1234为参数（role_id),中间用空格隔开

	std::istringstream iss;
	iss.str(cmd);

	std::string c;
	if ((iss >> c).fail()) return;

	else if (c == "CmdToReloadCrossConfig")
	{
		CrossManager::Instance().ReloadCrossConfig();

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Suc", cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "ChongzhiCmd")
	{
		bool succ = false;
		int role_id = 0, chongzhi_num = 0;

		if (!(iss >> role_id >> chongzhi_num).fail())
		{
			if (chongzhi_num > 0)
			{
				if (NULL != UserCacheManager::Instance().GetUserNode(role_id))
				{
					Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id));
					if (NULL != role)
					{
						role->GetRoleActivity()->CheckChongzhiExtraRewardByNum(chongzhi_num);
					}
					else
					{
						OfflineRegisterManager::Instance().OnChongzhiCmd(IntToUid(role_id), chongzhi_num);
					}

					succ = true;
				}
			}
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "Mute") // 禁言
	{
		int role_id = 0;
		unsigned int forbid_time = 0;

		bool succ = false;

		if (!(iss >> role_id >> forbid_time).fail())
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id));
			if (NULL != role)
			{
				role->ForbidTalk((time_t)forbid_time, "Command", "Command");
				succ = true;
			}
			else
			{
				int plat_type = LocalConfig::Instance().GetPlatType();
				UserID userid = IntToUid(role_id);
				UniqueUserID unique_userid(plat_type, userid);
				long long uuid = UniqueUserIDToLongLong(unique_userid);

				CrossUserMap &cross_user_map = World::GetInstWorld()->GetSceneManager()->GetCrossUserMap();

				if (cross_user_map.find(uuid) != cross_user_map.end())
				{
					crossgameprotocal::GameCrossForbidTalk gcft;

					gcft.uuid = uuid;
					gcft.forbid_time = forbid_time;

					CrossManager::Instance().SendToAllCross((const char *)&gcft, sizeof(gcft));
				}
				else
				{
					unsigned int end_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + forbid_time;
					OfflineRegisterManager::Instance().OnSetForbidTalk(role_id, end_timestamp);
				}
			}

			WorldStatus2::Instance().OnRoleMuteClearProfession(role_id);
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "No online"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "Cmd") // 重读配置表 配置的类型
	{
		iss >> c;
		if (c == "Reload")
		{
			int config_type = 0;
			// 这个是重读配置的参数，不是所有命令都需要的
			// 比如重读任务配置表，param1可以指定某个任务的id，不指定（为0）则表示重读所有
			int param1 = 0, param2 = 0;	
			if (!(iss >> config_type >> param1 >> param2).fail())
			{
				// 这个命令原样发给gameworld处理，至于config_type等代表什么意思，由gameworld解析
				gglobalprotocal::GSystemCmdReload gs;
				gs.config_type = config_type;
				gs.param1 = param1;
				gs.param2 = param2;
				World::GetInstWorld()->GetSceneManager()->OnReloadConfig((const char*)&gs, sizeof(gs));
			}
		}
	}
	else if (c == "CmdToRole") // 改变角色的一些属性，（新手指导员标志、威望值等）
	{
		int role_id = 0;
		int effect_type = 0, param = 0, param1;

		if (!(iss >> role_id>> effect_type >> param >> param1).fail())
		{
			bool ret = false;

			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id));
			if (NULL != role)
			{
				//gamelog::g_log_cmd.printf(LL_INFO, "Succ [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());

				ret = CmdHandler::Instance().OnCmdToRole(role, effect_type, param, param1);

				gamelog::g_log_scenestatus.printf(LL_INFO, "[CmdToRole %s! User[%d, %s], effect_type:%d, effect_param:%d, effect_param1:%d]", 
					(ret ? "Succ" : "Fail"),
					role_id, role->GetName(), effect_type, param, param1);
			}
			else
			{
				UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(role_id);
				if (user_node)
				{// 必须是本服的玩家，防止后台操作错服引起offlineregister 重复问题
					switch (effect_type)
					{
					case systemcmd::EFFECT_AUTHORITY:
						ret = OfflineRegisterManager::Instance().OnSetAuthorityType(role_id, param);
						break;
					case systemcmd::EFFECT_CONSUME_ITEM:
						ret = OfflineRegisterManager::Instance().OnSetConsumeItem(role_id, param, param1);
						break;
					case systemcmd::EFFECT_USE_GOLD:
						ret = OfflineRegisterManager::Instance().OnSetConsumeGold(role_id, param);
						break;
					case systemcmd::EFFECT_USE_GOLD_BIND:
						ret = OfflineRegisterManager::Instance().OnSetConsumeBindGold(role_id, param);
						break;
					default:
						break;
					}
				}
			}

			gamelog::g_log_cmd.printf(LL_INFO, "CmdToRole %s online:%s [%d][%s] %s ", (ret ? "Succ" : " Failed"), (role ? "Y" : "N"), cmd_id, creator.c_str(), cmd.c_str());
		}
		else
		{
			gamelog::g_log_cmd.printf(LL_INFO, "Fail [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());
		}
	}
	else if (c == "CmdToRoleCount") // 设置角色功能次数
	{
		GET_ROLE_CMD_PARRM();

		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id));
		if (succ && NULL != role)
		{
			ggprotocal::SystemCmdToRoleCount cmd_to_role_count;
			cmd_to_role_count.count_type = param1;
			cmd_to_role_count.count = param2;
			int ret = CmdHandler::Instance().OnCmdToRoleCount(role, cmd_to_role_count.count_type, cmd_to_role_count.count, false);

			gamelog::g_log_cmd.printf(LL_INFO, "[OnCmdToRoleCount %s! User[%d, %s], count_type:%d, count:%d]", (ret ? "Succ" : "Fail"),
				role_id, role->GetName(), cmd_to_role_count.count_type, cmd_to_role_count.count);
		}
		else
		{
			gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Fail", cmd_id, creator.c_str(), cmd.c_str());
		}
	}
	else if (c == "CmdToRoleFbCount") // 设置角色副本次数
	{
		GET_ROLE_CMD_PARRM();

		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id));
		if (succ && NULL != role)
		{
			ggprotocal::SystemCmdToRoleFbCount cmd_to_role_fb_count;
			cmd_to_role_fb_count.day_count_id = param1;
			cmd_to_role_fb_count.count = param2;
			bool ret = CmdHandler::Instance().OnCmdToRoleFbCount(role, cmd_to_role_fb_count.day_count_id, cmd_to_role_fb_count.count, false);

			gamelog::g_log_cmd.printf(LL_INFO, "[CmdToRoleFbCount %s! User[%d, %s], day_count_id:%d, count:%d]", (ret ? "Succ" : "Fail"),
				role_id, role->GetName(), cmd_to_role_fb_count.day_count_id, cmd_to_role_fb_count.count);
		}
		else
		{
			gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Fail", cmd_id, creator.c_str(), cmd.c_str());
		}
	}
	else if (c == "CmdToRoleRename")	// 角色改名
	{
		int role_id = 0;
		std::string name;

		if (!(iss >> role_id >> name).fail())
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id));
			if (NULL != role)
			{
				if (name.length() >= sizeof(GameName))
				{
					gamelog::g_log_cmd.printf(LL_INFO, "Fail [%d][%s] %s. Name too long", cmd_id, creator.c_str(), cmd.c_str());
				}
				else
				{
					GameName new_name; memset(new_name, 0, sizeof(new_name));

					STRNCPY(new_name, name.c_str(), sizeof(new_name));
					new_name[sizeof(new_name) - 1] = 0;
					role->OnResetNameReq(new_name);

					gamelog::g_log_cmd.printf(LL_INFO, "[CmdToRoleRename Succ! User[%d, %s], name:%s]",role_id, role->GetName(), new_name);
				}
			}
			else
			{
				gamelog::g_log_cmd.printf(LL_INFO, "Fail [%d][%s] %s. Cant Find User.", cmd_id, creator.c_str(), cmd.c_str());
			}
		}
		else
		{
			gamelog::g_log_cmd.printf(LL_INFO, "Fail [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());
		}
	}
	else if (c == "CmdToRoleKickOut")
	{
		int role_id = 0;
		if (!(iss >> role_id).fail())
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id));
			if (NULL != role)
			{
				EngineAdapter::Instance().NetDisconnect(role->GetNetId(), role->GetUserId(), role->GetName(), 0, "KickOut");
			}
			else
			{
				gamelog::g_log_cmd.printf(LL_INFO, "Fail [%d][%s] %s. Cant Find User.", cmd_id, creator.c_str(), cmd.c_str());
			}
		}
		else
		{
			gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Fail", cmd_id, creator.c_str(), cmd.c_str());
		}
	}
	else if (c == "newnotice")	// db变更时的通知,如新的系统邮件
	{
		int notice_type = 0, role_id = 0;
		if (!(iss >> notice_type >> role_id).fail()) 
		{
			if (notice_type > NEWNOTICE_TYPE_INVALID && notice_type < NEWNOTICE_TYPE_MAX)
			{
				this->NewNoticeCmdHelper(notice_type, role_id);
			}
		}
	}
	else if (c == "resetnotice")
	{
		this->ResetNotice();
	}
	else if (c == "CmdToActivity")	// 场景玩法活动切换至下一个状态
	{
		bool succ = false;

		int activity_type = 0;
		if (!(iss >> activity_type).fail())
		{
			if (activity_type > ACTIVITY_TYPE_INVALID && activity_type < ACTIVITY_TYPE_MAX)
			{
				succ = ActivityManager::Instance().OnForceToNextState(activity_type);
			}
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "CmdToSpeicalActivity")	// 设置特殊活动(如开服活动，跨服活动，封测活动等的）的开始时间和结束时间
	{
		bool succ = false;

		int activity_type = 0;
		unsigned int begin_time = 0, end_time = 0;

		if (!(iss >> activity_type >> begin_time >> end_time).fail())
		{
			if (activity_type > SPECIAL_ACTIVITY_TYPE_INVALID && activity_type < SPECIAL_ACTIVITY_TYPE_RESVERED )
			{
				if (!CrossConfig::Instance().IsHiddenServer())
				{
					succ = SpecialActivityManager::Instance().OnForceSetSpecialActivityTime(activity_type, begin_time, end_time);
				}
			}
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "CmdToRandActivity")	// 设置随机活动的开始时间和结束时间
	{
		bool succ = false;

		int activity_type = 0, limit_day;
		unsigned int begin_time = 0, end_time = 0;

		if (!(iss >> activity_type >> begin_time >> end_time >> limit_day).fail())
		{
			int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
			if (opengame_day < 5 && activity_type == RAND_ACTIVITY_TYPE_LOTTERY_1)
			{
				succ = false;
			}
			else if (limit_day >= 0 && activity_type >= RAND_ACTIVITY_TYPE_BEGIN && activity_type < RAND_ACTIVITY_TYPE_END)
			{
				if (!CrossConfig::Instance().IsHiddenServer())
				{
					succ = RandActivityManager::Instance().OnSetActivityTime(activity_type, begin_time, end_time, limit_day);
				}
			}
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "CrossActivityForceNextState")	// 跨服活动切换至下一个状态
	{
		bool succ = false;

		int activity_type = 0;

		if (!(iss >> activity_type).fail())
		{
			if (activity_type > CROSS_ACTIVITY_TYPE_INVALID && activity_type < CROSS_ACTIVITY_TYPE_RESERVED)
			{
				if (CrossConfig::Instance().IsHiddenServer())
				{
					succ = ActivityShadow::Instance().CrossActivityForceNextState(activity_type);
				}
			}
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "ForceSetRandActivityTime")	// 强制设置随机活动，不受限制（不对运营开放，我们内部用）
	{
		bool succ = false;

		int activity_type = 0, open_type = RAND_ACTIVITY_OPEN_TYPE_BEGIN;
		unsigned int begin_time = 0, end_time = 0;

		if (!(iss >> activity_type >> begin_time >> end_time >> open_type).fail())
		{
			if (activity_type >= RAND_ACTIVITY_TYPE_BEGIN && activity_type < RAND_ACTIVITY_TYPE_END)
			{
				if (!CrossConfig::Instance().IsHiddenServer())
				{
					succ = RandActivityManager::Instance().OnForceSetActivityTime(activity_type, begin_time, end_time);
				}
			}
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "CmdToWorldCount")
	{
		int count_type = 0, count = 0;
		if (!(iss >> count_type >> count).fail())
		{
			gglobalprotocal::SystemCmdToWorldCount cmd_to_world_count;
			cmd_to_world_count.count_type = count_type;
			cmd_to_world_count.count = count;
			World::GetInstWorld()->GetSceneManager()->OnCmdToWorldCount((const char*)&cmd_to_world_count, sizeof(cmd_to_world_count));

			gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Suc", cmd_id, creator.c_str(), cmd.c_str());
		}
		else
		{
			gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Fail", cmd_id, creator.c_str(), cmd.c_str());
		}
	}
	else if (c == "CmdToWorldFbCount")
	{
		int day_count_id = 0, count = 0;
		if (!(iss >> day_count_id >> count).fail())
		{
			gglobalprotocal::SystemCmdToWorldFbCount cmd_to_world_fb_count;
			cmd_to_world_fb_count.day_count_id = day_count_id;
			cmd_to_world_fb_count.count = count;
			World::GetInstWorld()->GetSceneManager()->OnCmdToWorldFbCount((const char*)&cmd_to_world_fb_count, sizeof(cmd_to_world_fb_count));

			gamelog::g_log_cmd.printf(LL_INFO, "[OnCmdToWorldCount, day_count_id:%d, count:%d]", cmd_to_world_fb_count.day_count_id, cmd_to_world_fb_count.count);
		}
		else
		{
			gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Fail", cmd_id, creator.c_str(), cmd.c_str());
		}
	}
	else if (c == "CmdToForbidSublock")
	{
		static gglobalprotocal::ForbidSublock utc;
		World::GetInstWorld()->GetSceneManager()->OnForbidSublock((const char*)&utc,sizeof(utc));

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Suc", cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "CmdToSetAutoForbid")
	{
	/*	int is_auto_forbid = 0;
		if (!(iss >> is_auto_forbid).fail())
		{
			static gglobalprotocal::ForceSetAutoForbid fsaf;
			fsaf.is_auto_forbid = is_auto_forbid;

			m_global_server->SendToAllGameWorld((const char*)&fsaf, sizeof(fsaf));

			gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Suc", cmd_id, creator.c_str(), cmd.c_str());
		}
		else
		{
			gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Fail", cmd_id, creator.c_str(), cmd.c_str());
		}*/
	}
	else if (c == "CmdToGuild")
	{
		GuildID guild_id = 0;
		int oper_type = 0;
		int ret = 0;

		if (!(iss >> guild_id >> oper_type).fail())
		{
			ret = GuildManager::Instance().OnGmCommand(guild_id, oper_type);
			if (0 != ret)
			{
				gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Fail", cmd_id, creator.c_str(), cmd.c_str());
			}

			gamelog::g_log_cmd.printf(LL_INFO, "Cmd [%d][%s] %s. Result:[%d].", cmd_id, creator.c_str(), cmd.c_str(), ret);
		}
		else
		{
			gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Fail", cmd_id, creator.c_str(), cmd.c_str());
		}
	}
	else if (c == "CmdToGuildSetValue")
	{
		/*int guild_id = 0, type = 0, value = 0;

		if (!(iss >> guild_id >> type >> value).fail())
		{
			if (m_global_server->GetGuildManager()->OnGmSetValue(guild_id, type, value))
			{
				gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Suc", cmd_id, creator.c_str(), cmd.c_str());
			}
			else
			{
				gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "return false", cmd_id, creator.c_str(), cmd.c_str());
			}
		}
		else
		{
			gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", "Fail", cmd_id, creator.c_str(), cmd.c_str());
		}*/
	}
	else if(c == "CmdBuyMonthCard")
	{
		int role_id = 0;

		if (!(iss >> role_id).fail())
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id));
			if (NULL != role)
			{
				role->GetMonthCard()->OnBuyCard();
			}
			else
			{
				gamelog::g_log_cmd.printf(LL_INFO, "Fail [%d][%s] %s. Cant Find User.", cmd_id, creator.c_str(), cmd.c_str());
			}
		}
	}
	else if (c == "CmdChatOpenLevel")		// 设置聊天等级
	{
		bool succ = false;
		int channel_type = 0, level = 0, vip_level = 0, chat_limit_condition_type = 0;

		if (!(iss >> channel_type >> level >> vip_level).fail() &&
			level >= 0 && level <= MAX_ROLE_LEVEL && vip_level >= 0 && vip_level < MAX_VIP_LEVEL)
		{
			iss >> chat_limit_condition_type;

			// 等级为0时表示只设置vip不设置等级
			if (0 != level)
			{
//				LOGIC_CONFIG->GetAgentAdapterConfig().CmdSetChannelChatLevel(channel_type, level);
				WorldStatus::Instance().CmdSetChannelLevelLimit(channel_type, level);
			}

			WorldStatus::Instance().CmdSetChannelVipLevelLimit(channel_type, vip_level);

			WorldStatus::Instance().CmdSetChatLimitConditionType(channel_type, chat_limit_condition_type);

			succ = true;
			ChatManager::Instance().SyncChatOpenLevelToClient();
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "CmdForbitChat")
	{
		bool succ = false;
		int channel_type = 0;
		unsigned int forbid_start_timestamp = 0;
		unsigned int forbid_end_timestamp = 0;

		if (!(iss >> channel_type >> forbid_start_timestamp >> forbid_end_timestamp).fail()
			&& channel_type >= 0 && channel_type < OPENLEVEL_LIMIT_TYPE_MAX)
		{
			ChatManager::Instance().SetChannelChatForbid(channel_type, forbid_start_timestamp, forbid_end_timestamp);
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "ForbidAudioChat")
	{
		bool succ = false;
		int audio_chat_forbid_flag = 0;

		if (!(iss >> audio_chat_forbid_flag).fail())
		{
			WorldStatus::Instance().CmdSetAudioChatForbidFlag(audio_chat_forbid_flag);
			ChatManager::Instance().SyncChatOpenLevelToClient();

			succ = true;
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "SetAvatar")
	{
		bool succ = false;
		int avatar_type = 0;
		int id = 0;

		if (!(iss >> avatar_type >> id).fail())
		{
			if (1 == avatar_type)
			{
				Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(id));
				if (nullptr != role)
				{
					role->SetAvatarTimeStamp(0);
				}
			}
			else if (2 == avatar_type)
			{
				Guild *guild = GuildManager::Instance().GetGuild(id);
				if (nullptr != guild)
				{
					GuildCommonDataParam *common_param = guild->GetGuildCommonDataParam();
					common_param->avatar_timestamp = 0;
				}
			}

			succ = true;
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "AdjustServerRoleNum")
	{
		bool succ = false;
		int server_role_num = 0;

		if (!(iss >> server_role_num).fail())
		{
			WorldStatus::Instance().CmdSetServerRoleNum(server_role_num);

			succ = true;
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else if (c == "SetCrossRandActivity")	// 设置跨服随机活动
	{
		bool succ = false;

		int activity_type = 0;
		unsigned int begin_time = 0, end_time = 0;

		if (!(iss >> activity_type >> begin_time >> end_time).fail())
		{
			if (activity_type >= CROSS_RAND_ACTIVITY_TYPE_BEGIN && activity_type < CROSS_RAND_ACTIVITY_TYPE_END)
			{
				if (!CrossConfig::Instance().IsHiddenServer())
				{
					succ = CrossRAManagerLocal::Instance().OnForceSetActivityTime(activity_type, begin_time, end_time);
				}
			}
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	} else if (c == "swaprole")
	{
		bool succ = false;

		int role_id1 = 0, role_id2 = 0;
		if (!(iss >> role_id1 >> role_id2).fail())
		{
			RMISwapRoleBackObjectImpl* obj = new RMISwapRoleBackObjectImpl();
			obj->role_id1 = role_id1;
			obj->role_id2 = role_id2;
			RMISystemClient sc;
			sc.__bind_session(InternalComm::Instance().GetDBHandler().GetSystemDB());
			succ = sc.SwapRole(role_id1, role_id2, obj);
		}

		gamelog::g_log_cmd.printf(LL_INFO, "%s [%d][%s] %s", (succ ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	}
	else
	{
		gamelog::g_log_cmd.printf(LL_WARNING, "Parse Error : [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());
	}
}

void System::NewNoticeCmdHelper(int notice_type, int role_id)
{
	if (NEWNOTICE_TYPE_PERSON_MAIL == notice_type)
	{
		if (role_id > 0) 
		{
			UserMailManager::Instance().LoadUserSystemMailListByCmd(IntToUid(role_id));
		}
	}
	else if (NEWNOTICE_TYPE_QUANFU_MAIL == notice_type)
	{
		UserMailManager::Instance().LoadAllUserSystemMailListByCmd();
	}
	else if (NEWNOTICE_TYPE_CHONGZHI == notice_type)
	{
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id));
		if (NULL != role)
		{
			role->GetKnapsack()->GetMoney()->ForceGetGold();
		}
	}
}

void System::StartPublicNoticeCheck()
{
	RMIGetPublicNoticeBackObjectImpl *gpnbo = new RMIGetPublicNoticeBackObjectImpl();

	RMISystemClient sc;
	sc.__bind_session(InternalComm::Instance().GetDBHandler().GetSystemDB());
	sc.GetPublicNoticeAsyn(gpnbo);
}

void System::StartCmdCheck()
{
	RMIGetCmdBackObjectImpl *callback_obj = new RMIGetCmdBackObjectImpl();

	RMISystemClient sc;
	sc.__bind_session(InternalComm::Instance().GetDBHandler().GetSystemDB());
	sc.GetCmd(RMISystemClient::CMD_TYPE_GLOBAL, callback_obj);
}

System & System::Instance()
{
	static System sys;
	return sys;
}


