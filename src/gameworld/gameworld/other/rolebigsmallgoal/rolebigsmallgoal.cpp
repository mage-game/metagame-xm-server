#include "config/logicconfigmanager.hpp"
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
#include "rolebigsmallgoal.hpp"
#include "config/rolebigsmallgoalconfig.hpp"
#include "protocal/msgother.h"
#include "item/itempool.h"
#include "item/expense/norexitem.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

void RoleBigSmallGoal::InitParam(Role *role, const RoleBigSmallGoalRewardParam &param)
{
	m_role = role;
	m_param = param;
}

void RoleBigSmallGoal::GetInitParam(RoleBigSmallGoalRewardParam *param) const
{
	*param = m_param;
}

void RoleBigSmallGoal::OnOpenFunc(int system_type)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX)
	{
		return;
	}
	//时间只跟系统类型有关，默认用小目标来判断，策划保持大小目标的开启配置一致即可
	if (!this->CanOpen(system_type, ROLE_BIG_SMALL_GOAL_REWARD_TYPE_SMALL))
	{
		return;
	}

	if (m_param.goal_info_list[system_type].open_func_time_stamp != 0)
	{
		return;
	}

	m_param.goal_info_list[system_type].open_func_time_stamp = (unsigned int)EngineAdapter::Instance().Time();
	this->CheckGoalActive(system_type, ROLE_BIG_SMALL_GOAL_REWARD_TYPE_SMALL);
	this->CheckGoalActive(system_type, ROLE_BIG_SMALL_GOAL_REWARD_TYPE_BIG);

	gamelog::g_log_role_big_small_goal.printf(LL_INFO, "BigSmallGoal::OnOpenFunc user[%d, %s] system_type:%d", m_role->GetUID(), m_role->GetName(), system_type);
}

void RoleBigSmallGoal::OnRoleLevelChange(int level)
{
	for (int i = ROLE_BIG_SMALL_GOAL_SYSTEM_INVALID + 1; i < ROLE_BIG_SMALL_GOAL_SYSTEM_MAX; ++i)
	{
		this->OnOpenFunc(i);
	}
}

void RoleBigSmallGoal::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid >= now_dayid)
	{
		return;
	}
	for (int i = ROLE_BIG_SMALL_GOAL_SYSTEM_INVALID + 1; i < ROLE_BIG_SMALL_GOAL_SYSTEM_MAX; ++i)
	{
		this->OnOpenFunc(i);
	}
}

bool RoleBigSmallGoal::CanOpen(int system_type, int goal_type)
{
	if (system_type <= ROLE_BIG_SMALL_GOAL_SYSTEM_INVALID || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX ||
		goal_type < 0 || goal_type >= ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX)
	{
		return false;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return false;
	}
	RoleBSGoalItemConfig *goal_item_cfg = LOGIC_CONFIG->GetRoleBigSmallGoalConfig().GetBSGoalItemConfig(system_type, goal_type);
	if (nullptr == goal_item_cfg)
	{
		return false;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	if (opengame_day < goal_item_cfg->openserver_day || m_role->GetLevel() < goal_item_cfg->open_level)
	{
		return false;
	}
	return true;
}

void RoleBigSmallGoal::OnSystemOperaCheck(int system_type, int oper_type, int param_1, int param_2)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX)
	{
		return;
	}

	this->OnOpenFunc(system_type);

	for (int i = 0; i < ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX; ++i)
	{
		this->CheckGoalProgress(system_type, i, oper_type, param_1, param_2);
	}
}

void RoleBigSmallGoal::CheckGoalProgress(int system_type, int goal_type, int oper_type, int param_1, int param_2)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX ||
		goal_type < 0 || goal_type >= ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX)
	{
		return;
	}

	if (0 != m_param.goal_info_list[system_type].goal_item_list[goal_type].active_flag)  //已激活
	{
		return;
	}

	RoleBSGoalItemConfig *goal_item_cfg = LOGIC_CONFIG->GetRoleBigSmallGoalConfig().GetBSGoalItemConfig(system_type, goal_type);
	if (goal_item_cfg == nullptr)
	{
		return;
	}

	if (oper_type != goal_item_cfg->condition_type)
	{
		return;
	}

	switch (goal_item_cfg->condition_type)
	{
		case ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_1:
		{
			if (param_1 >= goal_item_cfg->param_1 && m_param.goal_info_list[system_type].goal_item_list[goal_type].goal_progress < param_2)	//防止脱下N件装备后，再穿小于N件的情况，数量变少，完成不了目标
			{
				m_param.goal_info_list[system_type].goal_item_list[goal_type].goal_progress = param_2;
			}
		}
		break;

		case ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_2:
		{
			if (param_1 >= goal_item_cfg->param_1)
			{
				m_param.goal_info_list[system_type].goal_item_list[goal_type].goal_progress = 1;
			}
		}
		break;

		case ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_3:
		{
			if (param_1 == goal_item_cfg->param_1)
			{
				m_param.goal_info_list[system_type].goal_item_list[goal_type].goal_progress = 1;
			}
		}
		break;

		case ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_4:
		{
			if (param_1 >= goal_item_cfg->param_1)
			{
				m_param.goal_info_list[system_type].goal_item_list[goal_type].goal_progress += 1;
			}
		}
		break;

		case ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_5:
		{
			if (param_1 >= goal_item_cfg->param_1)
			{
				m_param.goal_info_list[system_type].goal_item_list[goal_type].goal_progress += 1;
			}
		}
		break;
	}

	this->CheckGoalActive(system_type, goal_type);
}

void RoleBigSmallGoal::CheckGoalActive(int system_type, int goal_type)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX ||
		goal_type < 0 || goal_type >= ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX)
	{
		return;
	}

	//时间只跟系统类型有关，默认用小目标来判断，策划保持大小目标的开启配置一致即可
	if (!this->CanOpen(system_type, ROLE_BIG_SMALL_GOAL_REWARD_TYPE_SMALL))
	{
		return;
	}

	if (0 != m_param.goal_info_list[system_type].goal_item_list[goal_type].active_flag)  //已激活
	{
		return;
	}

	RoleBSGoalItemConfig *goal_item_cfg = LOGIC_CONFIG->GetRoleBigSmallGoalConfig().GetBSGoalItemConfig(system_type, goal_type);
	if (goal_item_cfg == nullptr)
	{
		return;
	}

	unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();

	bool need_send = false;
	if (m_param.goal_info_list[system_type].goal_item_list[goal_type].goal_progress >= goal_item_cfg->param_2)
	{
		m_param.goal_info_list[system_type].goal_item_list[goal_type].active_flag = 1;
		need_send = true;
		if (now_time <= m_param.goal_info_list[system_type].open_func_time_stamp + goal_item_cfg->free_time_since_open_s)   // 免费时间内激活
		{
			m_param.goal_info_list[system_type].goal_item_list[goal_type].active_in_limit = 1;
		}
	}

	if (need_send)
	{
		this->SendGoalInfo(system_type);

		gamelog::g_log_role_big_small_goal.printf(LL_INFO, "BigSmallGoal::CheckGoalActive user[%d, %s] system_type:%d goal_type:%d active_flag:%d in_free_time:%d",
			m_role->GetUID(), m_role->GetName(), system_type, goal_type, m_param.goal_info_list[system_type].goal_item_list[goal_type].active_flag,
			m_param.goal_info_list[system_type].goal_item_list[goal_type].active_in_limit);
	}
}

void RoleBigSmallGoal::CheckBroadcast(int system_type, int goal_type)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX ||
		goal_type < 0 || goal_type >= ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX)
	{
		return;
	}

	if (0 == m_param.goal_info_list[system_type].active_special_attr)
	{
		return;
	}

	RoleBSGoalItemConfig *goal_item_cfg = LOGIC_CONFIG->GetRoleBigSmallGoalConfig().GetBSGoalItemConfig(system_type, goal_type);
	if (goal_item_cfg == nullptr)
	{
		return;
	}

	const RoleBSGoalAttrConfig * attr_cfg = LOGIC_CONFIG->GetRoleBigSmallGoalConfig().GetBSGoalAttrCfg(system_type);
	if (goal_item_cfg->is_broadcast && attr_cfg != nullptr)    // 传闻
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_role_big_small_goal_active_reward,
			UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), system_type, system_type, attr_cfg->add_attr_value / 100);

		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}
}

void RoleBigSmallGoal::SendGoalInfo(int system_type)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX) return;

	static Protocol::SCRoleBigSmallGoalInfo rbsgi;
	rbsgi.system_type = system_type;
	rbsgi.active_special_attr_flag = m_param.goal_info_list[system_type].active_special_attr;
	rbsgi.open_system_timestamp = m_param.goal_info_list[system_type].open_func_time_stamp;
	for (int i = 0; i < ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX; ++i)
	{
		rbsgi.goal_item_info[i].active_flag = m_param.goal_info_list[system_type].goal_item_list[i].active_flag && m_param.goal_info_list[system_type].goal_item_list[i].active_in_limit;
		rbsgi.goal_item_info[i].fetch_flag = m_param.goal_info_list[system_type].goal_item_list[i].fetch_flag;
	}
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&rbsgi, sizeof(rbsgi));
}

void RoleBigSmallGoal::FetchGoalRewardReq(int system_type, bool use_gold)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX) return;

	if (m_param.goal_info_list[system_type].goal_item_list[0].fetch_flag != 0) // 小目标已经领了  那领大目标
	{
		this->FetchGoalReward(system_type, 1, use_gold);
		return;
	}

	this->FetchGoalReward(system_type, 0, use_gold);
}

void RoleBigSmallGoal::FetchGoalReward(int system_type, int goal_type, bool use_gold)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX ||
		goal_type < 0 || goal_type >= ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX) return;

	if (m_param.goal_info_list[system_type].goal_item_list[goal_type].fetch_flag != 0)   // 已领
	{
		m_role->NoticeNum(errornum::EN_CHENGJIU_ALREADY_REWARD);
		return;
	}

	RoleBSGoalItemConfig *goal_item_cfg = LOGIC_CONFIG->GetRoleBigSmallGoalConfig().GetBSGoalItemConfig(system_type, goal_type);
	if (goal_item_cfg == nullptr)
	{
		return;
	}

	bool can_fetch = false;
	int need_gold = 0;

	if (m_param.goal_info_list[system_type].goal_item_list[goal_type].active_flag != 0
		&& m_param.goal_info_list[system_type].goal_item_list[goal_type].active_in_limit != 0) // 激活并且是时限内免费
	{
		can_fetch = true;
	}
	else
	{
		if (use_gold)                                                                          // 只能购买
		{
			need_gold = goal_item_cfg->cost_gold;
			can_fetch = true;
		}
	}

	if (!can_fetch)
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(goal_item_cfg->reward_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(goal_item_cfg->cost_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(goal_item_cfg->cost_gold, "RoleBigSmallGoal::FetchGoalReward"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_param.goal_info_list[system_type].goal_item_list[goal_type].fetch_flag = 1;
	m_role->GetKnapsack()->PutList(goal_item_cfg->reward_count, goal_item_cfg->reward_item_list, PUT_ROLE_BIG_SMALL_GOAL_REWARD);
	this->SendGoalInfo(system_type);

	gamelog::g_log_role_big_small_goal.printf(LL_INFO, "BigSmallGoal::FetchGoalReward user[%d, %s] system_type:%d goal_type:%d active_flag:%d in_free_time:%d fetch_falg:%d",
		m_role->GetUID(), m_role->GetName(), system_type, goal_type, m_param.goal_info_list[system_type].goal_item_list[goal_type].active_flag,
		m_param.goal_info_list[system_type].goal_item_list[goal_type].active_in_limit,
		m_param.goal_info_list[system_type].goal_item_list[goal_type].fetch_flag);
}

bool RoleBigSmallGoal::ActiveRoleBigSmallGoalSpecialAttr(int system_type, int param_1)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX)
	{
		return false;
	}

	if (m_param.goal_info_list[system_type].goal_item_list[1].fetch_flag == 0)
	{
		return false;
	}

	if (m_param.goal_info_list[system_type].active_special_attr != 0)   // 已激活特殊属性
	{
		return false;
	}

	const RoleBSGoalAttrConfig * attr_cfg = LOGIC_CONFIG->GetRoleBigSmallGoalConfig().GetBSGoalAttrCfg(system_type);
	if (attr_cfg == nullptr || param_1 != attr_cfg->active_attr_param)
	{
		return false;
	}

	m_param.goal_info_list[system_type].active_special_attr = 1;        // 激活
	this->SendGoalInfo(system_type);
	this->CheckBroadcast(system_type, 1);

	switch (system_type)
	{
	case ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE:
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_RESON_RUNE_SYSTEM);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
	}
	break;

	case ROLE_BIG_SMALL_GOAL_SYSTEM_SHENZHOU_WEAPON:
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENZHOU_WEAPON);
	}
	break;

	case ROLE_BIG_SMALL_GOAL_SYSTEM_SHENSHOU:
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENSHOU);
	}
	break;

	case ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGE:
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGE_SYSTEM);
	}
	break;

	case ROLE_BIG_SMALL_GOAL_SYSTEM_SHENYIN:
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHEN_YIN);
	}
	break;

	case ROLE_BIG_SMALL_GOAL_SYSTEM_EQUIP_STRENGTHEN:
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);
	}
	break;

	case ROLE_BIG_SMALL_GOAL_SYSTEM_STONE:
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_STONE);
	}
	break;

	case ROLE_BIG_SMALL_GOAL_SYSTEM_CHINESE_ZODIAC:
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC);
	}
	break;

	case ROLE_BIG_SMALL_GOAL_SYSTEM_SHENGQI:
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGQI);
	}
	break;

	case ROLE_BIG_SMALL_GOAL_SYSTEM_GREATE_SOLDIER:
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);
	}
	break;

	default:
		break;
	}

	return true;
}

void RoleBigSmallGoal::RoleBigSmallGoalAddSpecialAttr(int system_type, CharIntAttrs &attrs_add)
{
	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX)
	{
		return;
	}

	if (m_param.goal_info_list[system_type].active_special_attr == 0)   // 未激活
	{
		return;
	}

	const RoleBSGoalAttrConfig * attr_cfg = LOGIC_CONFIG->GetRoleBigSmallGoalConfig().GetBSGoalAttrCfg(system_type);
	if (attr_cfg != nullptr)
	{
		attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += attr_cfg->max_hp;
		attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += attr_cfg->gongji;
		attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += attr_cfg->fangyu;
	}
}

int RoleBigSmallGoal::GetRoleBigSmallGoalAddAttr(int system_type)
{
	int add_value = 0;

	if (system_type < ROLE_BIG_SMALL_GOAL_SYSTEM_RUNE || system_type >= ROLE_BIG_SMALL_GOAL_SYSTEM_MAX)
	{
		return add_value;
	}

	if (m_param.goal_info_list[system_type].active_special_attr != 0)
	{
		add_value = LOGIC_CONFIG->GetRoleBigSmallGoalConfig().GetBSGoalAttrAddValue(system_type);
	}

	return add_value;
}

void RoleBigSmallGoal::OnGetItem(int item_id, int num)
{
	// 目标由获取改为装备了,先屏蔽掉
	/*const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	if (NULL == itembase) return;	if (ItemBase::I_TYPE_EQUIPMENT == itembase->GetItemType())
	{
		const Equipment *equip = (const Equipment *)itembase;
		if (Equipment::E_TYPE_SPECIAL_JINGLING == equip->GetEquipType())
		{
			this->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_XIANCHONG, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_4, equip->GetColor(), num);
		}
	}
	else if (ItemBase::I_TYPE_EXPENSE == itembase->GetItemType())
	{
		const NorexItem *norex_item = dynamic_cast<const NorexItem *>(itembase);
		if (NorexItem::I_NOREX_ITEM_SHENYIN == norex_item->GetUseType())
		{
			this->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENYIN, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_4, norex_item->GetColor(), num);
		}
	}*/
}

void RoleBigSmallGoal::OnRoleBigSmallGoalOpera(int opera_type, int param_1, int param_2)
{
	switch (opera_type)
	{
		case Protocol::ROLE_BIG_SMALL_GOAL_OPERA_INFO:
		{
			this->SendGoalInfo(param_1);
		}
		break;

		case Protocol::ROLE_BIG_SMALL_GOAL_OPERA_FETCH:
		{
			this->FetchGoalRewardReq(param_1, param_2 == 1);
		}
		break;

		default:
			break;
	}
}

void RoleBigSmallGoal::OnRoleLogin()
{
	for (int i = ROLE_BIG_SMALL_GOAL_SYSTEM_INVALID + 1; i < ROLE_BIG_SMALL_GOAL_SYSTEM_MAX; ++i)
	{
		this->OnOpenFunc(i);
	}
	for (int i = 0; i < ROLE_BIG_SMALL_GOAL_SYSTEM_MAX; i++)
	{
		this->SendGoalInfo(i);
	}
}