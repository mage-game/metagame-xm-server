// #include "stdafx.h"

#include "shengxingzhulimanager.hpp"
#include "shengxingzhuliconfig.hpp"
#include "protocal/msgshengxingzhuli.h"
#include "gamecommon.h"
#include "world.h"
#include "shengxingzhuliconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "other/mount/mountfunction.hpp"
#include "other/wing/wingfunction.hpp"
#include "other/halo/halofunction.hpp"
#include "other/shenyi/shenyifunction.hpp"
#include "item/itempool.h"

#include "other/mount/mountmanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "other/mount/mountopconfig.hpp"
#include "other/halo/haloconfig.hpp"
#include "other/wing/wingconfig.hpp"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "obj/character/role.h"
#include "other/shenyi/shenyimanager.hpp"
#include "other/footprint/footprint.hpp"
#include "other/fightmount/fightmountconfig.hpp"
#include "other/shengong/shengongfunction.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/fightmount/fightmountfunction.hpp"

ShengxingzhuliManager::ShengxingzhuliManager() : m_role(NULL)
{
	m_param.Reset();
}

ShengxingzhuliManager::~ShengxingzhuliManager()
{

}

void ShengxingzhuliManager::InitParam(const ShengxingzhuliParam &shengxingzhuli_param)
{
	m_param = shengxingzhuli_param;
}

void ShengxingzhuliManager::GetInitParam(ShengxingzhuliParam * shengxingzhuli_param)
{
	*shengxingzhuli_param = m_param;
}

void ShengxingzhuliManager::OnRoleChongzhi(int chongzhi)
{
	m_param.chongzhi_today += chongzhi;
	m_param.today_reward_stage = LOGIC_CONFIG->GetShengxingzhuliCfg().GetStageByChongzhi(m_param.chongzhi_today);

	this->SendAllInfo();
}

void ShengxingzhuliManager::SetIsGetReward()
{
	++m_param.today_reward_fetch_stage;
}

//手动领取，自动补发
void ShengxingzhuliManager::OnGetReward(bool is_manual)
{//屏蔽此功能
	return;
	
	if (m_param.today_reward_fetch_stage >= m_param.today_reward_stage && is_manual)
	{
		m_role->NoticeNum(errornum::EN_SHENGXINGZHULI_FETCH_TOO_MUCH);
		return;
	}

	int weekday_system_type = -1;	// 今天对应的系统
	if (is_manual)
	{
		weekday_system_type = this->GetDaySystemIndex();
	}
	else
	{
		weekday_system_type = this->GetDaySystemIndex(-1);	//如果不需要传闻，表示是零点自动领取，此时要获取前一天的系统来进行操作
	}

	if (weekday_system_type < SHENGXINGZHULI_SYSTEM_TYPE_BEGIN || weekday_system_type >= SHENGXINGZHULI_SYSTEM_TYPE_COUNT)
	{
		return;
	}

	bool can_upgrade = false;

	switch (weekday_system_type)
	{
		case SHENGXINGZHULI_SYSTEM_TYPE_MOUNT:
		{
			can_upgrade = MountFunction::CheckCanUpGrade(m_role);
		}
		break;

		case SHENGXINGZHULI_SYSTEM_TYPE_WING:
		{
			can_upgrade = WingFunction::CheckCanUpGrade(m_role);
		}
		break;

		case SHENGXINGZHULI_SYSTEM_TYPE_FOOT_PRINT:
		{
			can_upgrade = FootPrint::CheckCanUpGrade(m_role);
		}
		break;

		case SHENGXINGZHULI_SYSTEM_TYPE_HALO:
		{
			can_upgrade = HaloFunction::CheckCanUpGrade(m_role);
		}
		break;

		case SHENGXINGZHULI_SYSTEM_TYPE_FIGHT_MOUNT:
		{
			can_upgrade = FightMountFunction::CheckCanUpGrade(m_role);
		}
		break;

		case SHENGXINGZHULI_SYSTEM_TYPE_SHENGONG:
		{
			can_upgrade = ShengongFunction::CheckCanUpGrade(m_role);
		}
		break;

		case SHENGXINGZHULI_SYSTEM_TYPE_SHENYI:
		{
			can_upgrade = ShenyiFunction::CheckCanUpGrade(m_role);
		}
		break;

		default:
			return;
	}

	if (can_upgrade)
	{
		int need_chongzhi = LOGIC_CONFIG->GetShengxingzhuliCfg().GetNeedChongZhiByStage(m_param.today_reward_fetch_stage + 1);
		if (m_param.chongzhi_today < need_chongzhi)
		{
			if (is_manual)
			{
				m_role->NoticeNum(errornum::EN_SHENGXINGZHULI_CHONGZHI_NOT_ENOUGH);
			}

			return;
		}

		int target_star_level = 0;
		int add_star = 1;
		if (!is_manual && m_param.today_reward_stage > m_param.today_reward_fetch_stage)
		{
			add_star = m_param.today_reward_stage - m_param.today_reward_fetch_stage;
		}

		const char * chuanwen_format = NULL;				// 传闻指针

		switch (weekday_system_type)
		{
			case SHENGXINGZHULI_SYSTEM_TYPE_MOUNT:
			{
				target_star_level = MountFunction::UplevelAndSaveBlessValue(m_role, add_star);
				chuanwen_format = gamestring::g_shengxingzhuli_mount;
			}
			break;

			case SHENGXINGZHULI_SYSTEM_TYPE_WING:
			{
				target_star_level = WingFunction::UplevelAndSaveBless(m_role, add_star);
				chuanwen_format = gamestring::g_shengxingzhuli_wing;
			}
			break;

			case SHENGXINGZHULI_SYSTEM_TYPE_FOOT_PRINT:
			{
				target_star_level = FootPrint::UplevelAndSaveBless(m_role, add_star);
				chuanwen_format = gamestring::g_shengxingzhuli_fazhen;
			}
			break;

			case SHENGXINGZHULI_SYSTEM_TYPE_HALO:
			{
				target_star_level = HaloFunction::UplevelAndSaveBless(m_role, add_star);
				chuanwen_format = gamestring::g_shengxingzhuli_halo;
			}
			break;

			case SHENGXINGZHULI_SYSTEM_TYPE_FIGHT_MOUNT:
			{
				target_star_level = FightMountFunction::UplevelAndSaveBless(m_role, add_star);
				chuanwen_format = gamestring::g_shengxingzhuli_jingling_fazhen;
			}
			break;

			case SHENGXINGZHULI_SYSTEM_TYPE_SHENGONG:
			{
				target_star_level = ShengongFunction::UplevelAndSaveBless(m_role, add_star);
				chuanwen_format = gamestring::g_shengxingzhuli_jingling_halo;
			}
			break;

			case SHENGXINGZHULI_SYSTEM_TYPE_SHENYI:
			{
				target_star_level = ShenyiFunction::UplevelAndSaveBless(m_role, add_star);
				chuanwen_format = gamestring::g_shengxingzhuli_pifeng;
			}
			break;

			default:
				return;
		}

		if (is_manual && chuanwen_format != NULL)
		{
			// 10级是1阶10星，11级是2阶1星
			int system_jiepin = (target_star_level - 1) / 10;
			int system_xing = (target_star_level - 1) % 10 + 1;

			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), chuanwen_format,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), system_jiepin, system_xing);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}

		//自动补发，邮件提醒
		if (!is_manual)
		{
			static MailContentParam contentparam; contentparam.Reset();

			int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_shengxingzhuli_mail_subject);
			int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_shengxingzhuli_mail_content);
			if (length1 > 0 && length2 > 0)
			{
				MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}
	//else if (!can_upgrade)		//满级奖励屏蔽
	//{
	//	const ShengxingzhuliOtherCfg& other_cfg = LOGIC_CONFIG->GetShengxingzhuliCfg().GetShengxingzhuliOtherCfg();

	//	const ItemConfigData& reward_item = other_cfg.reward_item_list[weekday_system_type];

	//	if (ITEMPOOL->GetItem(reward_item.item_id) != NULL)
	//	{
	//		ItemDataWrapper reward_item(reward_item.item_id, reward_item.num, reward_item.is_bind);
	//		m_role->GetKnapsack()->Put(reward_item, PUT_REASON_SHENGXINGZHULI_REWARD);
	//	}
	//}
	else
	{
		return;
	}

	++m_param.today_reward_fetch_stage;
	this->SendAllInfo();
}

void ShengxingzhuliManager::SendAllInfo()
{
	Protocol::SCGetShengxingzhuliInfoAck msg;

	msg.today_reward_fetch_stage = m_param.today_reward_fetch_stage;
	msg.chognzhi_today = m_param.chongzhi_today;
	msg.func_level = 0;
	msg.today_reward_stage = m_param.today_reward_stage;

	msg.func_type = this->GetDaySystemIndex();

	if (this->IsSystemMaxGrade(msg.func_type))
	{
		msg.is_max_level = 1;
	}
	else
	{
		msg.is_max_level = 0;
	}
	
	switch (msg.func_type)
	{
		case SHENGXINGZHULI_SYSTEM_TYPE_MOUNT:
		{
			msg.func_level = m_role->GetMountManager()->GetMountParam()->star_level;
			break;
		}

		case SHENGXINGZHULI_SYSTEM_TYPE_WING:
		{
			msg.func_level = m_role->GetWing()->GetWingParam()->star_level;
			break;
		}

		case SHENGXINGZHULI_SYSTEM_TYPE_FOOT_PRINT:
		{
			msg.func_level = m_role->GetFootPrint()->GetFootprintParam()->star_level;
			break;
		}

		case SHENGXINGZHULI_SYSTEM_TYPE_HALO:
		{
			msg.func_level = m_role->GetHalo()->GetHaloParam()->star_level;
			break;
		}

		case SHENGXINGZHULI_SYSTEM_TYPE_FIGHT_MOUNT:
		{
			msg.func_level = m_role->GetFightMountManager()->GetFightMountParam()->star_level;
			break;
		}

		case SHENGXINGZHULI_SYSTEM_TYPE_SHENGONG:
		{
			msg.func_level = m_role->GetShengong()->GetShengongParam()->star_level;
			break;
		}

		case SHENGXINGZHULI_SYSTEM_TYPE_SHENYI:
		{
			msg.func_level = m_role->GetShenyi()->GetShenyiParam()->star_level;
			break;
		}

		default:
			msg.func_level = 0;
	}
	
	EngineAdapter::Instance().NetSend((m_role)->GetNetId(), (const char*)&(msg), sizeof(msg));
}

void ShengxingzhuliManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	// 如果玩家激活了当天对应的系统，并且没领取奖励，并且充值额度够了，那么直接发放然后邮件通知,注意要发放前一天的奖励
	int today_func = this->GetDaySystemIndex(-1);

	bool is_active = this->IsSystemActive(today_func);

	if (is_active && m_param.today_reward_stage > m_param.today_reward_fetch_stage)
	{
		this->OnGetReward(false);
	}

	m_param.Reset();

	this->SendAllInfo();
}

bool ShengxingzhuliManager::IsSystemActive(int sys_type)
{
	// 如果对应的系统已经激活返回true，还没有激活返回false,默认没激活
	if (sys_type < SHENGXINGZHULI_SYSTEM_TYPE_BEGIN || sys_type >= SHENGXINGZHULI_SYSTEM_TYPE_COUNT)
		return false;

	switch (sys_type)
	{
	case  SHENGXINGZHULI_SYSTEM_TYPE_MOUNT:
	{
		return m_role->GetMountManager()->GetMountParam()->IsMountActived();
	}

	case SHENGXINGZHULI_SYSTEM_TYPE_WING:
	{
		return m_role->GetWing()->GetWingParam()->IsWingActived();
	}

	case SHENGXINGZHULI_SYSTEM_TYPE_HALO:
	{
		return m_role->GetHalo()->GetHaloParam()->IsHaloActived();
	}

	case SHENGXINGZHULI_SYSTEM_TYPE_FOOT_PRINT:
	{
		return m_role->GetFootPrint()->GetFootprintParam()->IsFootprintActived();
	}

	case SHENGXINGZHULI_SYSTEM_TYPE_FIGHT_MOUNT:
	{
		return m_role->GetFightMountManager()->GetFightMountParam()->IsFightMountActived();
	}

	case SHENGXINGZHULI_SYSTEM_TYPE_SHENGONG:
	{
		return m_role->GetShengong()->GetShengongParam()->IsShengongActived();
	}

	case SHENGXINGZHULI_SYSTEM_TYPE_SHENYI:
	{
		return m_role->GetShenyi()->GetShenyiParam()->IsShenyiActived();
	}

	default:
		return false;
	}
	return false;
}

bool ShengxingzhuliManager::IsSystemMaxGrade(int sys_type)
{
	// 如果对应的系统最高级返回true，不是最高级返回false,默认是最高级
	if (sys_type < SHENGXINGZHULI_SYSTEM_TYPE_BEGIN || sys_type >= SHENGXINGZHULI_SYSTEM_TYPE_COUNT)
		return true;

	int sys_start_level = 0;
	switch (sys_type)
	{
	case  SHENGXINGZHULI_SYSTEM_TYPE_MOUNT:
	{
		sys_start_level = m_role->GetMountManager()->GetMountParam()->star_level;

		if (NULL == LOGIC_CONFIG->GetMountOpConfig().GetUpStarExpCfg(sys_start_level + 1))
		{
			return  true;
		}
		else
		{
			return false;
		}
		break;
	}

	case SHENGXINGZHULI_SYSTEM_TYPE_WING:
	{
		sys_start_level = m_role->GetWing()->GetWingParam()->star_level;

		if (NULL == LOGIC_CONFIG->GetWingConfig().GetUpStarExpCfg(sys_start_level + 1))
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	}

	case SHENGXINGZHULI_SYSTEM_TYPE_HALO:
	{
		sys_start_level = m_role->GetHalo()->GetHaloParam()->star_level;

		if (NULL == LOGIC_CONFIG->GetHaloConfig().GetUpStarExpCfg(sys_start_level + 1))
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	}

	case SHENGXINGZHULI_SYSTEM_TYPE_FOOT_PRINT:
	{
		sys_start_level = m_role->GetFootPrint()->GetFootprintParam()->star_level;

		if (NULL == LOGIC_CONFIG->GetFootPrintConfig().GetUpStarExpCfg(sys_start_level + 1))
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	}

	case SHENGXINGZHULI_SYSTEM_TYPE_FIGHT_MOUNT:
	{
		sys_start_level = m_role->GetFightMountManager()->GetFightMountParam()->star_level;
		if (NULL == LOGIC_CONFIG->GetFightMountOpConfig().GetUpStarExpCfg(sys_start_level + 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	break;

	case SHENGXINGZHULI_SYSTEM_TYPE_SHENGONG:
	{
		sys_start_level = m_role->GetShengong()->GetShengongParam()->star_level;
		if (NULL == LOGIC_CONFIG->GetShengongConfig().GetUpStarExpCfg(sys_start_level + 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	break;

	case SHENGXINGZHULI_SYSTEM_TYPE_SHENYI:
	{
		sys_start_level = m_role->GetShenyi()->GetShenyiParam()->star_level;
		if (NULL == LOGIC_CONFIG->GetShenyiConfig().GetUpStarExpCfg(sys_start_level + 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	break;

	default:
		return true;
	}
	return true;
}

int ShengxingzhuliManager::GetDaySystemIndex(int day)
{
	// 首先判断是否开服一周内，开服前7天,根据开服天数拿对应系统，否则根据周几拿对应系统
	// 传入的参数默认为0，如果是0则表示获取今天的系统对应序号，如果 是-1表示拿前一天的，是1表示拿后一天的
	// 要根据今天是周几来决定给哪个系统升星，然后还要判断是否能升星，未满星就升星，满星就给物品奖励
	int weekday = -1;				// 今天是周几
	int weekday_system_type = -1;	// 今天对应的系统

	{
		const time_t server_start_time = m_role->GetServerStartTime();
		if (MAX_SHENGXINGZHULI_SPECIAL_DAYS_COUNT > GetDayIndex(server_start_time, EngineAdapter::Instance().Time()) + day)
		{
			weekday = (GetDayIndex(server_start_time, EngineAdapter::Instance().Time()) + 1) % MAX_SHENGXINGZHULI_SPECIAL_DAYS_COUNT + day;
		}
		else
		{
			time_t now_time_t = EngineAdapter::Instance().Time();
			const tm* now_time_tm = ::localtime(&now_time_t);

			weekday = now_time_tm->tm_wday + day;

			// weekday只能拿0到6
			while (weekday < 0)
			{
				weekday += MAX_SHENGXINGZHULI_SPECIAL_DAYS_COUNT;
			}

			while (weekday >= MAX_SHENGXINGZHULI_SPECIAL_DAYS_COUNT)
			{
				weekday -= MAX_SHENGXINGZHULI_SPECIAL_DAYS_COUNT;
			}
		}
	}

	const ShengxingzhuliWeekdaySystem * weekday_system_cfg = LOGIC_CONFIG->GetShengxingzhuliCfg().GetShengxingzhuliWeekdaySystemCfg();
	if (NULL == weekday_system_cfg)
	{
		return -1;
	}


	if (SATURDAY < weekday || SUNDAY > weekday)
	{
		return -1;
	}

	switch (weekday)
	{
	case MONDAY:
	{
		weekday_system_type = weekday_system_cfg->monday_sys;
		break;
	}

	case TUESDAY:
	{
		weekday_system_type = weekday_system_cfg->tuesday_sys;
		break;
	}

	case WEDNESDAY:
	{
		weekday_system_type = weekday_system_cfg->wednesday_sys;
		break;
	}

	case THURSDAY:
	{
		weekday_system_type = weekday_system_cfg->thursday_sys;
		break;
	}

	case FRIDAY:
	{
		weekday_system_type = weekday_system_cfg->friday_sys;
		break;
	}

	case SATURDAY:
	{
		weekday_system_type = weekday_system_cfg->saturday_sys;
		break;
	}

	case SUNDAY:
	{
		weekday_system_type = weekday_system_cfg->sunday_sys;
		break;
	}

	default:
		return -1;
	}

	return weekday_system_type;
}