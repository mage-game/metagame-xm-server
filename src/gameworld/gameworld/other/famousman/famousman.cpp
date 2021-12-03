#include "famousman.hpp"
#include "gamelog.h"
#include "engineadapter.h"
#include "world.h"
#include "obj/character/role.h"
#include "servercommon/string/gameworldstr.h"
#include "global/worldstatus/worldstatus.hpp"
#include "item/itembase.h"
#include "other/achieve/title/titlemanager.hpp"
#include "equipment/equipmentmanager.h"

#include "config/logicconfigmanager.hpp"
#include "other/achieve/title/titleconfig.hpp"
#include "other/famousman/famousmanconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/commondata.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "global/cross/crossmanager.h"
#include "other/cross/rolecross.h"

FamousMan::FamousMan()
	: m_role(NULL)
{
}

FamousMan::~FamousMan()
{

}

void FamousMan::FamousManOpera(Role *role, int opera_type)
{
	switch(opera_type)
	{
	case Protocol::FAMOUS_MAN_INFO_REQ:
		{
			this->SendFamousManInfo(role);
		}
		break;
	}
}

void FamousMan::OnCheckFamousMan(int famous_type, int param)
{
	// 合服之后不再重算名人堂称号，外服没合服之前合服时间会保持空，也就是0，不过为了兼容开发服经常会配合服时间测试，把条件放宽一点
	if (SHAREDCONFIG->GetOpenServerConfig().GetServerRealCombineTime() > 0 &&
		SHAREDCONFIG->GetOpenServerConfig().GetServerRealCombineTime() > SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime())
	{
		return;
	}

	int open_day = m_role->GetOpenServerDay();
	if (open_day > LOGIC_CONFIG->GetFamousManCfg().GetOpenTime())
	{
		return;
	}

	const FamousManCfg * famous_cfg = LOGIC_CONFIG->GetFamousManCfg().GetFamousManCfg(famous_type);
	if (NULL == famous_cfg) return;

	int famous_man_id = WorldStatus::Instance().GetFamousManID(famous_type);
	if (0 != famous_man_id)
	{
		return;
	}

	WorldStatusCommonDataParam &world_common_data = WorldStatus::Instance().GetCommonData();
	
	switch(famous_type)
	{
	case FAMOUS_MAN_ROLE_LEVEL:
		{
			if (param >= famous_cfg->param_1)
			{
				world_common_data.famous_man_type_1_uid = m_role->GetUID();

				unsigned short title_id = LOGIC_CONFIG->GetTitlePool().GetFamousManTitle(famous_type);
				m_role->GetTitleManager()->AddTitle(title_id, true, true, true, true);

				//江湖传闻
				GuildID guild_id = m_role->GetGuildID();
				if (INVALID_GUILD_ID != guild_id)
				{
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_BE_FAMOUS_MAN, title_id);
				}

				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_famous_man_type_1, 
					(int)UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), famous_cfg->param_1);
				if (len > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL_2);
				}
			}
			else
			{
				return;
			}
		}
		break;
	case FAMOUS_MAN_MOUNT_GRADE:
		{
			if (param >= famous_cfg->param_1)
			{
				world_common_data.famous_man_type_2_uid = m_role->GetUID();

				unsigned short title_id = LOGIC_CONFIG->GetTitlePool().GetFamousManTitle(famous_type);
				m_role->GetTitleManager()->AddTitle(title_id, true, true, true, true);

				//江湖传闻
				GuildID guild_id = m_role->GetGuildID();
				if (INVALID_GUILD_ID != guild_id)
				{
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_BE_FAMOUS_MAN, title_id);
				}

				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_famous_man_type_2, 
					(int)UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), (famous_cfg->param_1 - 1));
				if (len > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL_2);
				}
			}
			else
			{
				return;
			}
		}
		break;

	case FAMOUS_MAN_WING_GRADE:
		{
			if (param >= famous_cfg->param_1)
			{
				world_common_data.famous_man_type_3_uid = m_role->GetUID();

				unsigned short title_id = LOGIC_CONFIG->GetTitlePool().GetFamousManTitle(famous_type);
				m_role->GetTitleManager()->AddTitle(title_id, true, true, true, true);

				//江湖传闻
				GuildID guild_id = m_role->GetGuildID();
				if (INVALID_GUILD_ID != guild_id)
				{
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_BE_FAMOUS_MAN, title_id);
				}

				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_famous_man_type_3, 
					(int)UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), (famous_cfg->param_1 - 1));
				if (len > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL_2);
				}
			}
			else
			{
				return;
			}
		}
		break;

	case FAMOUS_MAN_EQUIP_GRADE:
		{
			if (param != ItemBase::I_COLOR_RED)
			{
				return;
			}

			//if (famous_cfg->param_1 <= m_role->GetEquipmentManager()->GetStatisfyColorAndGradeEquipCount(param, famous_cfg->param_2))
			if (famous_cfg->param_1 <= m_role->GetZhuanzhiEquip()->GetSatisfyPutonCount(famous_cfg->param_2, param))   //param：颜色	param_2：阶数
			{
				world_common_data.famous_man_type_4_uid = m_role->GetUID();

				unsigned short title_id = LOGIC_CONFIG->GetTitlePool().GetFamousManTitle(famous_type);
				m_role->GetTitleManager()->AddTitle(title_id, true, true, true, true);

				//江湖传闻
				GuildID guild_id = m_role->GetGuildID();
				if (INVALID_GUILD_ID != guild_id)
				{
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_BE_FAMOUS_MAN, title_id);
				}

				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_famous_man_type_4, 
					(int)UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), famous_cfg->param_1, famous_cfg->param_2);
				if (len > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL_2);
				}
			}
			else
			{
				return;
			}
		}
		break;

	case FAMOUS_MAN_TOWER_LEVEL:
		{
			if (param >= famous_cfg->param_1)
			{
				world_common_data.famous_man_type_5_uid = m_role->GetUID();

				unsigned short title_id = LOGIC_CONFIG->GetTitlePool().GetFamousManTitle(famous_type);
				m_role->GetTitleManager()->AddTitle(title_id, true, true, true, true);

				//江湖传闻
				GuildID guild_id = m_role->GetGuildID();
				if (INVALID_GUILD_ID != guild_id)
				{
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_BE_FAMOUS_MAN, title_id);
				}

				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_famous_man_type_5, 
					(int)UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), famous_cfg->param_1);
				if (len > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL_2);
				}
			}
			else
			{
				return;
			}
		}
		break;
	case FAMOUS_MAN_ACTIVE_XIANNV:
		{
			if (param == famous_cfg->param_1)
			{
				world_common_data.famous_man_type_6_uid = m_role->GetUID();

				unsigned short title_id = LOGIC_CONFIG->GetTitlePool().GetFamousManTitle(famous_type);
				m_role->GetTitleManager()->AddTitle(title_id, true, true, true, true);

				//江湖传闻
				GuildID guild_id = m_role->GetGuildID();
				if (INVALID_GUILD_ID != guild_id)
				{
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_BE_FAMOUS_MAN, title_id);
				}

				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_famous_man_type_6, 
					(int)UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), famous_cfg->param_2);
				if (len > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL_2);
				}
			}
			else
			{
				return;
			}
		}
		break;

	default: return;
	}

	// 能跑到这里，说明肯定激活了某个名人堂称号，标记一下
	CommonDataParam *role_common_param = m_role->GetCommonDataParam();
	role_common_param->famous_man_flag |= (1 << famous_type);

	this->SendFamousManInfo();
}

void FamousMan::SendFamousManInfo(Role *role)
{
	static Protocol::SCSendFamousManInfo gvi;

	WorldStatusCommonDataParam &world_conmmon_data = WorldStatus::Instance().GetCommonData();

	gvi.famous_man_type_1_uid = world_conmmon_data.famous_man_type_1_uid;
	gvi.famous_man_type_2_uid = world_conmmon_data.famous_man_type_2_uid;
	gvi.famous_man_type_3_uid = world_conmmon_data.famous_man_type_3_uid;
	gvi.famous_man_type_4_uid = world_conmmon_data.famous_man_type_4_uid;
	gvi.famous_man_type_5_uid = world_conmmon_data.famous_man_type_5_uid;
	gvi.famous_man_type_6_uid = world_conmmon_data.famous_man_type_6_uid;

	if (NULL == role)
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&gvi, sizeof(gvi));
	}
	else
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gvi, sizeof(gvi));
	}
}



