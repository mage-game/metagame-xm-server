#include "servercommon/struct/jinjiesysrewardparam.hpp"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "jinjiesysrewardmgr.hpp"
#include "config/logicconfigmanager.hpp"
#include "jinjiesysrewardconfig.hpp"
#include "obj/character/role.h"
#include "item/knapsack.h"
#include "servercommon/commondata.hpp"
#include "servercommon/errornum.h"
#include "protocal/msgrole.h"
#include "gamecommon.h"
#include "config/sharedconfig/sharedconfig.h"
#include "engineadapter.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "other/route/mailroute.hpp"
#include "other/cloak/cloakmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/footprint/footprint.hpp"
#include "other/halo/halomanager.hpp"
#include "other/mask/maskmanager.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/upgrade/upgrademanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

void JinjieSysRewardMgr::InitParam(Role *role, const JinjieSysRewardParam &param)
{
	*(role->GetJinjieSysRewardParam()) = param;
}

void JinjieSysRewardMgr::OnActiveJinjieSys(Role *role, int sys_type)
{
	if (nullptr == role)
	{
		return;
	}
	if (sys_type < 0 || sys_type >= JINJIE_TYPE_MAX_COUNT)
	{
		return;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	for (int i = 0; i < JinjiesysRewardConfig::REWARD_TYPE_MAX; ++i)
	{
		const JinjieRewardItemCfg *item_cfg = LOGIC_CONFIG->GetJinjiesysRewardConfig().GetItemCfg(i, sys_type);
		if (nullptr != item_cfg && item_cfg->openserver_day == 0)
		{
			role->GetJinjieSysRewardParam()->complete_timestamp_list[i][sys_type] =
				static_cast<unsigned int>(EngineAdapter::Instance().Time()) + item_cfg->duration_time_s;
			gamelog::g_log_jinjie_sys_reward.printf(LL_INFO, "JinjieSysRewardMgr::OnActiveJinjieSys user[%d %s] reward_type:%d jinjie_type:%d complete_timestamp:%u opengame_day:%d rolelevel:%d",
				role->GetUID(), role->GetName(), i, sys_type, role->GetJinjieSysRewardParam()->complete_timestamp_list[i][sys_type], opengame_day, role->GetLevel());
		}
	}
}

int JinjieSysRewardMgr::GetJinjieSpecialImgExtraAddPer(Role *role, int sys_type)
{
	if (nullptr == role)
	{
		return 0;
	}

	if (!LOGIC_CONFIG->GetJinjiesysRewardConfig().CanUseCfg(JinjiesysRewardConfig::REWARD_TYPE_SPECIAL_IMG, sys_type))
	{
		return 0;
	}

	const JinjieRewardItemCfg *item_cfg = LOGIC_CONFIG->GetJinjiesysRewardConfig().GetItemCfg(JinjiesysRewardConfig::REWARD_TYPE_SPECIAL_IMG, sys_type);
	if (nullptr == item_cfg || item_cfg->param_0 < 0)
	{
		return 0;
	}

	const JinjiesysFuncStruct *func_struct = JinjiesysFuncHelper::Instance().GetFuncStruct(sys_type);
	if (nullptr != func_struct && nullptr != func_struct->check_special_img_func)
	{
		if (func_struct->check_special_img_func(role, item_cfg->param_0))
		{
			const JinjieRewardAttrCfg *attr_cfg = LOGIC_CONFIG->GetJinjiesysRewardConfig().GetAttrCfg(sys_type);
			if (nullptr != attr_cfg)
			{
				return attr_cfg->add_per;
			}
		}
	}

	return 0;
}

void JinjieSysRewardMgr::OnJinjieSysRewardUpgrade(Role *role, int sys_type, int to_grade)
{
	if (nullptr == role)
	{
		return;
	}

	const auto &cfg_vec = LOGIC_CONFIG->GetJinjiesysRewardConfig().GetActiveItemCfgVec(to_grade, sys_type);
	for (auto item_cfg : cfg_vec)
	{
		if (nullptr == item_cfg)
		{
			continue;
		}

		// 检查是否超过时间
		if (!LOGIC_CONFIG->GetJinjiesysRewardConfig().CanUseCfg(item_cfg->reward_type, item_cfg->system_type))
		{
			continue;
		}

		unsigned int end_time = role->GetJinjieSysRewardParam()->complete_timestamp_list[item_cfg->reward_type][item_cfg->system_type];
		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) >= end_time)
		{
			continue;
		}

		unsigned int *flag = nullptr;
		unsigned int *reward_flag = nullptr;

		switch (item_cfg->reward_type)
		{
		case JinjiesysRewardConfig::REWARD_TYPE_SPECIAL_IMG:
			{
				flag = &role->GetCommonDataParam()->jinjiesys_can_reward_flag;
				reward_flag = &role->GetCommonDataParam()->jinjiesys_reward_flag;
			}
			break;

		case JinjiesysRewardConfig::REWARD_TYPE_TITLE:
			{
				flag = &role->GetCommonDataParam()->jinjiesys_can_reward_flag_1;
				reward_flag = &role->GetCommonDataParam()->jinjiesys_reward_flag_1;
			}
			break;

		default:
			break;
		}

		if (nullptr == flag || nullptr == reward_flag)
		{
			continue;
		}

		if (0 != (*flag & (1 << sys_type)) || 0 != (*reward_flag & (1 << sys_type)))
		{
			continue;
		}

		if (to_grade >= item_cfg->grade)
		{
			*flag |= 1 << sys_type;

			this->SendJinjieSysRewardInfo(role);

			LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_JINJIESYS_REWARD, role->GetUID(), role->GetName(), "JinjieSysRewardMgr::OnJinjieSysRewardUpgrade",
				sys_type, to_grade, ItemConfigDataLog(&item_cfg->reward_item, nullptr), nullptr);
		}
	}
}

void JinjieSysRewardMgr::CheckJinjieSysReward(Role *role, int sys_type, int special_img_id)
{
	if (nullptr == role)
	{
		return;
	}

	this->CheckBroadcast(role, sys_type, special_img_id);
	this->CheckGiveReward(role, sys_type, special_img_id);
}

void JinjieSysRewardMgr::OnJinjiesysRewardOpera(Role *role, int opera_type, int param_0, int param_1)
{
	if (nullptr == role)
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::JINJIESYS_REWARD_OPEAR_TYPE_INFO:
		{
			this->SendJinjieSysRewardInfo(role);
		}
		break;

	case Protocol::JINJIESYS_REWARD_OPEAR_TYPE_BUY:
		{
			this->OnJinjiesysBuySpecialImg(role, param_0, param_1);
		}
		break;

	case Protocol::JINJIESYS_REWARD_OPEAR_TYPE_FETCH:
		{
			this->OnJinjiesysFetchUpgradeReward(role, param_0, param_1);
		}
		break;

	default:
		break;
	}
}

void JinjieSysRewardMgr::SetCompleteTimestamp(Role *role)
{
	if (nullptr == role)
	{
		return;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	UNSTD_STATIC_CHECK(JINJIE_TYPE_MAX <= JINJIE_TYPE_MAX_COUNT)
	//time_t zero_time = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime();
	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	bool is_send = false;
	for (int reward_type = 0; reward_type < JinjiesysRewardConfig::REWARD_TYPE_MAX && reward_type < JINJIE_REWARD_TYPE_MAX_COUNT; ++reward_type)
	{
		for (int i = 0; i < JINJIE_TYPE_MAX && i < JINJIE_TYPE_MAX_COUNT; ++i)
		{
			const JinjieRewardItemCfg *cfg = LOGIC_CONFIG->GetJinjiesysRewardConfig().GetItemCfg(reward_type, i);
			if (nullptr != cfg && 0 == role->GetJinjieSysRewardParam()->complete_timestamp_list[reward_type][i] && 
				cfg->openserver_day > 0 && opengame_day >= cfg->openserver_day && role->GetLevel() >= cfg->open_level)
			{
				//role->GetJinjieSysRewardParam()->complete_timestamp_list[reward_type][i] = static_cast<unsigned int>(zero_time + (cfg->openserver_day - 1) * SECOND_PER_DAY + cfg->duration_time_s);
				role->GetJinjieSysRewardParam()->complete_timestamp_list[reward_type][i] = now_time + cfg->duration_time_s;
				is_send = true;
				this->CheckJinjiesysGrade(role, i);		//解决在大小目标开启之前就已经完成目标的问题
				gamelog::g_log_jinjie_sys_reward.printf(LL_INFO, "JinjieSysRewardMgr::SetCompleteTimestamp user[%d %s] reward_type:%d jinjie_type:%d complete_timestamp:%u opengame_day:%d rolelevel:%d", 
					role->GetUID(), role->GetName(), reward_type, i, role->GetJinjieSysRewardParam()->complete_timestamp_list[reward_type][i], opengame_day, role->GetLevel());
			}
		}
	}
	if (is_send)
	{
		this->SendJinjieSysRewardTimestamp(role);
	}
}

void JinjieSysRewardMgr::CheckJinjiesysGrade(Role * role, int sys_type)
{
	if (nullptr == role || sys_type <= JINJIE_TYPE_INVALID || sys_type >= JINJIE_TYPE_MAX)
	{
		return;
	}
	int grade = this->GetJinjiesysGrade(role, sys_type);
	if (grade > 0)
	{
		this->OnJinjieSysRewardUpgrade(role, sys_type, grade);
	}
}

void JinjieSysRewardMgr::OnJinjiesysFetchUpgradeReward(Role *role, int sys_type, int reward_type)
{
	if (nullptr == role)
	{
		return;
	}

	if (!LOGIC_CONFIG->GetJinjiesysRewardConfig().CanUseCfg(reward_type, sys_type))
	{
		return;
	}

	if (!role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const JinjieRewardItemCfg *item_cfg = LOGIC_CONFIG->GetJinjiesysRewardConfig().GetItemCfg(reward_type, sys_type);
	if (nullptr == item_cfg)
	{
		return;
	}

	const unsigned int *can_reward_flag = nullptr;
	unsigned int *reward_flag = nullptr;

	switch (item_cfg->reward_type)
	{
	case JinjiesysRewardConfig::REWARD_TYPE_SPECIAL_IMG:
	{
		can_reward_flag = &role->GetCommonDataParam()->jinjiesys_can_reward_flag;
		reward_flag = &role->GetCommonDataParam()->jinjiesys_reward_flag;
	}
	break;

	case JinjiesysRewardConfig::REWARD_TYPE_TITLE:
	{
		can_reward_flag = &role->GetCommonDataParam()->jinjiesys_can_reward_flag_1;
		reward_flag = &role->GetCommonDataParam()->jinjiesys_reward_flag_1;
	}
	break;

	default:
		break;
	}

	if (nullptr == can_reward_flag || nullptr == reward_flag)
	{
		return;
	}

	if (0 == (*can_reward_flag & (1 << sys_type)) || 0 != (*reward_flag & (1 << sys_type)))
	{
		return;
	}

	//const JinjiesysFuncStruct *func_struct = JinjiesysFuncHelper::Instance().GetFuncStruct(sys_type);
	//if (nullptr != func_struct && nullptr != func_struct->get_grade_func)
	//{
	//	int grade = func_struct->get_grade_func(role);
	//	if (grade >= item_cfg->grade)
	//	{
			if (0 == (*reward_flag & (1 << sys_type)))
			{
				*reward_flag |= 1 << sys_type;

				role->GetKnapsack()->Put(item_cfg->reward_item, PUT_REASON_JINJIESYS_REWARD);

				this->SendJinjieSysRewardInfo(role);

				LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_JINJIESYS_REWARD, role->GetUID(), role->GetName(), "JinjieSysRewardMgr::OnJinjiesysFetchUpgradeReward",
					sys_type, item_cfg->reward_type, ItemConfigDataLog(&item_cfg->reward_item, nullptr), nullptr);
			}
	//	}
	//}
}

void JinjieSysRewardMgr::OnJinjiesysBuySpecialImg(Role *role, int sys_type, int reward_type)
{
	if (nullptr == role)
	{
		return;
	}

	if (!LOGIC_CONFIG->GetJinjiesysRewardConfig().CanUseCfg(reward_type, sys_type))
	{
		return;
	}

	if (!role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const JinjieRewardItemCfg *item_cfg = LOGIC_CONFIG->GetJinjiesysRewardConfig().GetItemCfg(reward_type, sys_type);
	if (nullptr == item_cfg || item_cfg->cost <= 0)
	{
		return;
	}

	const unsigned int *can_reward_flag = nullptr;
	unsigned int *reward_flag = nullptr;

	switch (item_cfg->reward_type)
	{
	case JinjiesysRewardConfig::REWARD_TYPE_SPECIAL_IMG:
	{
		can_reward_flag = &role->GetCommonDataParam()->jinjiesys_can_reward_flag;
		reward_flag = &role->GetCommonDataParam()->jinjiesys_reward_flag;
	}
	break;

	case JinjiesysRewardConfig::REWARD_TYPE_TITLE:
	{
		can_reward_flag = &role->GetCommonDataParam()->jinjiesys_can_reward_flag_1;
		reward_flag = &role->GetCommonDataParam()->jinjiesys_reward_flag_1;
	}
	break;

	default:
		break;
	}

	if (nullptr == can_reward_flag || nullptr == reward_flag)
	{
		return;
	}

	if (0 != (*reward_flag & (1 << sys_type)) || 0 != (*can_reward_flag & (1 << sys_type)))
	{
		role->NoticeNum(errornum::EN_JINJIESYS_REWARD_OWNED);
		return;
	}
	else
	{
		char reason[128] = { 0 };
		SNPRINTF(reason, sizeof(reason), "OnJinjiesysBuySpecialImg");
		if (!role->GetKnapsack()->GetMoney()->UseGold(item_cfg->cost, reason))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		role->GetKnapsack()->Put(item_cfg->reward_item, PUT_REASON_JINJIESYS_REWARD);
		*reward_flag |= 1 << sys_type;

		this->SendJinjieSysRewardInfo(role);

		LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_JINJIESYS_REWARD, role->GetUID(), role->GetName(), "OnJinjiesysBuySpecialImg",
			sys_type, item_cfg->reward_type, ItemConfigDataLog(&item_cfg->reward_item, nullptr), nullptr);
	}
}

void JinjieSysRewardMgr::SendJinjieSysRewardInfo(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	Protocol::SCJinjiesysRewardInfo info;
	info.reward_flag = role->GetCommonDataParam()->jinjiesys_reward_flag;
	info.can_reward_flag = role->GetCommonDataParam()->jinjiesys_can_reward_flag;
	info.reward_flag_1 = role->GetCommonDataParam()->jinjiesys_reward_flag_1;
	info.can_reward_flag_1 = role->GetCommonDataParam()->jinjiesys_can_reward_flag_1;

	SEND_TO_ROLE(role, info);
}

void JinjieSysRewardMgr::SendJinjieSysRewardTimestamp(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	static Protocol::SCJinjiesysRewardTimestamp info;

	info.use_sys_count = 0;
	for (int reward_type = 0; reward_type < JinjiesysRewardConfig::REWARD_TYPE_MAX; ++reward_type)
	{
		for (int i = 0; i < JINJIE_TYPE_MAX && info.use_sys_count < Protocol::SCJinjiesysRewardTimestamp::JINJIE_SYS_COUNT; ++i)
		{
			if (LOGIC_CONFIG->GetJinjiesysRewardConfig().CanUseCfg(reward_type, i))
			{
				auto &sys_timestamp = info.timestamp_list[info.use_sys_count++];

				sys_timestamp.reward_type = reward_type;
				sys_timestamp.sys_type = i;
				sys_timestamp.end_timestamp = role->GetJinjieSysRewardParam()->complete_timestamp_list[reward_type][i];
			}
		}
	}

	SEND_TO_ROLE(role, info);

	UNSTD_STATIC_CHECK(JINJIE_TYPE_MAX <= Protocol::SCJinjiesysRewardTimestamp::JINJIE_SYS_COUNT);
	UNSTD_STATIC_CHECK(JINJIE_TYPE_MAX <= JINJIE_TYPE_MAX_COUNT);
	UNSTD_STATIC_CHECK(JinjiesysRewardConfig::REWARD_TYPE_MAX <= JINJIE_REWARD_TYPE_MAX_COUNT);
}

int JinjieSysRewardMgr::GetJinjiesysGrade(Role * role, int sys_type)
{
	if (nullptr == role || sys_type <= JINJIE_TYPE_INVALID || sys_type >= JINJIE_TYPE_MAX)
	{
		return 0;
	}
	UNSTD_STATIC_CHECK(24 == JINJIE_TYPE_MAX);

	switch (sys_type)
	{
	case JINJIE_TYPE_CLOAK:
	{
		return role->GetCloakManager()->GetCloakLevel();
	}
	break;

	case JINJIE_TYPE_FIGHT_MOUNT:
	{
		return role->GetFightMountManager()->GetGrade();
	}
	break;

	case JINJIE_TYPE_FOOTPRINT:
	{
		return role->GetFootPrint()->GetGrade();
	}
	break;

	case JINJIE_TYPE_HALO:
	{
		return role->GetHalo()->GetGrade();
	}
	break;

	case JINJIE_TYPE_LINGZHU:
	{
		return role->GetUpgradeManager()->GetGrade(JinjiesysFuncHelper::Instance().JinJieSysTypeToUpgradeManagerType(sys_type));
	}
	break;

	case JINJIE_TYPE_MASK:
	{
		return role->GetMaskManager()->GetGrade();
	}
	break;

	case JINJIE_TYPE_MOUNT:
	{
		return role->GetMountManager()->GetGrade();
	}
	break;

	case JINJIE_TYPE_QILINBI:
	{
		return role->GetQiLinBiManager()->GetGrade();
	}
	break;

	case JINJIE_TYPE_SHENGONG:
	{
		return role->GetShengong()->GetGrade();
	}
	break;

	case JINJIE_TYPE_SHENYI:
	{
		return role->GetShenyi()->GetGrade();
	}
	break;

	case JINJIE_TYPE_TOUSHI:
	{
		return role->GetTouShiManager()->GetGrade();
	}
	break;

	case JINJIE_TYPE_WING:
	{
		return role->GetWing()->GetGrade();
	}
	break;

	case JINJIE_TYPE_XIANBAO:
	{
		return role->GetUpgradeManager()->GetGrade(JinjiesysFuncHelper::Instance().JinJieSysTypeToUpgradeManagerType(sys_type));
	}
	break;

	case JINJIE_TYPE_YAOSHI:
	{
		return role->GetYaoShiManager()->GetGrade();
	}
	break;

	case JINJIE_TYPE_LINGGONG:
	{
		return role->GetUpgradeManager()->GetGrade(JinjiesysFuncHelper::Instance().JinJieSysTypeToUpgradeManagerType(sys_type));
	}
	break;

	case JINJIE_TYPE_LINGQI:
	{
		return role->GetUpgradeManager()->GetGrade(JinjiesysFuncHelper::Instance().JinJieSysTypeToUpgradeManagerType(sys_type));
	}
	break;

	case JINJIE_TYPE_LINGCHONG:
	{
		return role->GetUpgradeManager()->GetGrade(JinjiesysFuncHelper::Instance().JinJieSysTypeToUpgradeManagerType(sys_type));
	}
	break;

	case JINJIE_TYPE_WEIYAN:
	{
		return role->GetUpgradeManager()->GetGrade(JinjiesysFuncHelper::Instance().JinJieSysTypeToUpgradeManagerType(sys_type));
	}
	break;

	case JINJIE_TYPE_SHOUHUAN:
	{
		return role->GetUpgradeManager()->GetGrade(JinjiesysFuncHelper::Instance().JinJieSysTypeToUpgradeManagerType(sys_type));
	}
	break;

	case JINJIE_TYPE_TAIL:
	{
		return role->GetUpgradeManager()->GetGrade(JinjiesysFuncHelper::Instance().JinJieSysTypeToUpgradeManagerType(sys_type));
	}
	break;

	case JINJIE_TYPE_FLYPET:
	{
		return role->GetUpgradeManager()->GetGrade(JinjiesysFuncHelper::Instance().JinJieSysTypeToUpgradeManagerType(sys_type));
	}
	break;

	case JINJIE_TYPE_FABAO:
	{
		return role->GetFabaoManager()->GetGrade();
	}
	break;

	case JINJIE_TYPE_SHIZHUANG:
	{
		return role->GetShizhuang()->GetShiZhuangGrade(SHIZHUANG_TYPE_BODY);
	}
	break;

	case JINJIE_TYPE_SHENBING:
	{
		return role->GetShizhuang()->GetShiZhuangGrade(SHIZHUANG_TYPE_WUQI);
	}
	break;

	default: return 0;
	}
}

void JinjieSysRewardMgr::CheckBroadcast(Role *role, int sys_type, int special_img_id)
{
	if (nullptr == role)
	{
		return;
	}

	if (!LOGIC_CONFIG->GetJinjiesysRewardConfig().CanUseCfg(JinjiesysRewardConfig::REWARD_TYPE_SPECIAL_IMG, sys_type))
	{
		return;
	}

	const JinjieRewardItemCfg *item_cfg = LOGIC_CONFIG->GetJinjiesysRewardConfig().GetItemCfg(JinjiesysRewardConfig::REWARD_TYPE_SPECIAL_IMG, sys_type);
	const JinjieRewardAttrCfg *attr_cfg = LOGIC_CONFIG->GetJinjiesysRewardConfig().GetAttrCfg(sys_type);
	if (nullptr != item_cfg && nullptr != attr_cfg && item_cfg->param_0 == special_img_id)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_jinjiesys_reward_active,
			UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), sys_type, sys_type, attr_cfg->add_per / 100);

		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}
}

void JinjieSysRewardMgr::CheckGiveReward(Role *role, int sys_type, int special_img_id)
{
	if (nullptr == role)
	{
		return;
	}

	if (!LOGIC_CONFIG->GetJinjiesysRewardConfig().CanUseCfg(JinjiesysRewardConfig::REWARD_TYPE_SPECIAL_IMG, sys_type))
	{
		return;
	}

	const JinjieRewardItemCfg *item_cfg = LOGIC_CONFIG->GetJinjiesysRewardConfig().GetItemCfg(JinjiesysRewardConfig::REWARD_TYPE_SPECIAL_IMG, sys_type);
	if (nullptr != item_cfg && item_cfg->param_0 == special_img_id)
	{
		unsigned int &flag = role->GetCommonDataParam()->jinjiesys_can_reward_flag;
		unsigned int &reward_flag = role->GetCommonDataParam()->jinjiesys_reward_flag;

		flag |= 1 << sys_type;
		reward_flag |= 1 << sys_type;

		unsigned int &reward_flag_1 = role->GetCommonDataParam()->jinjiesys_reward_flag_1;
		//激活了大目标奖励，给予小目标奖励
		if (0 == (reward_flag_1 & 1 << sys_type))
		{
			item_cfg = LOGIC_CONFIG->GetJinjiesysRewardConfig().GetItemCfg(JinjiesysRewardConfig::REWARD_TYPE_TITLE, sys_type);
			if (nullptr != item_cfg)
			{
				if (role->GetKnapsack()->HaveSpace())
				{
					role->GetKnapsack()->Put(item_cfg->reward_item, PUT_REASON_JINJIESYS_REWARD);
				}
				else
				{
					static MailContentParam reward_mail;
					reward_mail.Reset();

					reward_mail.AddItem(item_cfg->reward_item);
					int length = SNPRINTF(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), gamestring::g_jinjiesys_reward_mail_content, sys_type);
					if (length > 0)
					{
						MailRoute::MailToRole(role, SYSTEM_MAIL_REASON_INVALID, reward_mail);

						LOG_QUICK8(gamelog::g_log_quick, LOG_TYPE_JINJIESYS_REWARD, role->GetUID(), role->GetName(), "JinjieSysRewardMgr::CheckGiveReward",
							sys_type, special_img_id, ItemConfigDataLog(&item_cfg->reward_item, nullptr), nullptr);
					}
				}

				unsigned int &flag_1 = role->GetCommonDataParam()->jinjiesys_can_reward_flag_1;

				flag_1 |= 1 << sys_type;
				reward_flag_1 |= 1 << sys_type;
			}
		}
	}
}

void JinjieSysRewardMgr::OnRoleLogin(Role *role)
{
	if (nullptr == role)
	{
		return;
	}
	this->SetCompleteTimestamp(role);
}

void JinjieSysRewardMgr::OnRoleLevelChange(Role * role, int level)
{
	if (nullptr == role)
	{
		return;
	}
	this->SetCompleteTimestamp(role);
}

void JinjieSysRewardMgr::OnDayChange(Role * role, unsigned int old_dayid, unsigned int now_dayid)
{
	if (nullptr == role || old_dayid >= now_dayid)
	{
		return;
	}
	this->SetCompleteTimestamp(role);
}