#include "rolepreciousboss.hpp"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "item/knapsack.h"
#include "servercommon/errornum.h"
#include "other/activedegree/activedegree.hpp"

RolePreciousBoss::RolePreciousBoss() : m_role(NULL)
{

}

RolePreciousBoss::~RolePreciousBoss()
{

}

void RolePreciousBoss::Init(Role *role, const PreciousBossParam &param)
{
	m_role = role;
	m_param = param;
}

void RolePreciousBoss::GetInitParam(PreciousBossParam *param)
{
	*param = m_param;
}

void RolePreciousBoss::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	m_param.ResetTaskList();
	BossFamilyManager::Instance().SendPreciousBossTaskInfo(m_role);
}

bool RolePreciousBoss::AddPreciousBossTaskCondition(int task_type, int max_count, int add_count)
{
	if (task_type < PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_BOSS || task_type >= PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_MAX)
	{
		return false;
	}

	m_param.task_list[task_type].task_condition += add_count;
	m_param.task_list[task_type].task_type = task_type;
	if (m_param.task_list[task_type].task_condition >= max_count && 0 == m_param.task_list[task_type].is_finish)
	{
		m_param.task_list[task_type].task_condition = max_count;
		m_param.task_list[task_type].is_finish = 1;
		return true;
	}

	return false;
}

bool RolePreciousBoss::SkipTask()
{
	if (m_role->GetLevel() < LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossOtherCfg().skip_task_limit_level)
	{
		return false;
	}

	//检查是否全部完成
	int skip_count = 0;
	for (int i = 0; i < PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_MAX; ++i)
	{
		if (0 == m_param.task_list[i].is_finish)
		{
			++skip_count;
		}
	}

	//消耗
	int consume = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossOtherCfg().skip_task_consume * skip_count;
	if (consume <= 0)
	{
		return false;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(consume, "Skip preciousboss task"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return true;
	}

	for (int i = 0; i < PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_MAX; ++i)
	{
		const PreciousBossTaskCfg *cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossTaskCfg(i);
		if (NULL != cfg && 0 == m_param.task_list[i].is_finish)
		{
			m_param.task_list[i].task_condition = cfg->task_condition;
			m_param.task_list[i].is_finish = 1;
			m_param.task_list[i].task_type = i;
			BossFamilyManager::Instance().PreciousBossRewardHelper(m_role, i, true);
		}
	}

	BossFamilyManager::Instance().SendPreciousBossTaskInfo(m_role);

	return true;
}