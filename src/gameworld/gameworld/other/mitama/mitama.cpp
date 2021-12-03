#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "engineadapter.h"

#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"

#include "other/event/eventhandler.hpp"
#include "protocal/msgmitama.h"
#include "mitama.hpp"
#include "mitamaconfig.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"

Mitama::Mitama() : m_role(NULL)
{
}

Mitama::~Mitama()
{
}

void Mitama::InitParam(Role *role, const MitamaParam &param) 
{ 
	m_role = role; m_param = param; 
}

void Mitama::GetInitParam(MitamaParam *param)	
{ 
	*param = m_param; 
}

void Mitama::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		for (int mitama_seq = 0; mitama_seq < MITAMA_MAX_MITAMA_COUNT; ++ mitama_seq)
		{
			// 御魂等级战力
			for (int mitama_level = 0; mitama_level <= m_param.info_list[mitama_seq].level; ++ mitama_level)
			{
				const MitamaLevelConfig::LevelConfig *mitama_level_cfg = LOGIC_CONFIG->GetMitamaCfg().GetLevelCfg(mitama_seq, mitama_level);
				if (NULL != mitama_level_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += mitama_level_cfg->maxhp * MITAMA_MAX_SPIRIT_COUNT;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += mitama_level_cfg->gongji * MITAMA_MAX_SPIRIT_COUNT;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += mitama_level_cfg->fangyu * MITAMA_MAX_SPIRIT_COUNT;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += mitama_level_cfg->mingzhong * MITAMA_MAX_SPIRIT_COUNT;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += mitama_level_cfg->shanbi * MITAMA_MAX_SPIRIT_COUNT;
				}
			}

			// 每个小魂的战力
			for (int spirit_index = 0; spirit_index < MITAMA_MAX_SPIRIT_COUNT; ++ spirit_index)
			{
				int spirit_level = m_param.info_list[mitama_seq].spirit_level_list[spirit_index];

				const MitamaLevelConfig::LevelConfig *spirit_level_cfg = LOGIC_CONFIG->GetMitamaCfg().GetLevelCfg(mitama_seq, spirit_level);
				if (NULL != spirit_level_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += spirit_level_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += spirit_level_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += spirit_level_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += spirit_level_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += spirit_level_cfg->shanbi;
				}
			}
		}
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_MITAMA, m_attrs_add);
	base_add.Add(m_attrs_add.m_attrs, true);
}

void Mitama::OnUpgrade(int mitama_seq, int spirit_index, bool auto_buy)
{
	if (mitama_seq < 0 || mitama_seq >= MITAMA_MAX_MITAMA_COUNT || spirit_index < 0 || spirit_index >= MITAMA_MAX_SPIRIT_COUNT)
	{
		return;
	}

	char &mitama_level = m_param.info_list[mitama_seq].level;
	char &spirit_level = m_param.info_list[mitama_seq].spirit_level_list[spirit_index];

	// 检查御魂是否满级
	if (mitama_level >= MITAMA_MAX_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_MITAMA_LEVEL_FULL);
		return;
	}

	// 检查小魂等级（不能超过御魂等级+1）
	if (spirit_level >= mitama_level + 1)
	{
		m_role->NoticeNum(errornum::EN_MITAMA_JINGHUN_LEVEL_FULL);
		return;
	}

	// 检查消耗物是否足够
	const MitamaLevelConfig::LevelConfig *level_cfg = LOGIC_CONFIG->GetMitamaCfg().GetLevelCfg(mitama_seq, spirit_level);
	if (NULL == level_cfg)
	{
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	int stuff_count = 0;

	for (int i = 0; i < level_cfg->consume_item_count && i < MitamaLevelConfig::MAX_CONSUME_ITEM_COUNT; ++ i)
	{
		stuff_list[i].item_id = level_cfg->consume_item_list[i].item_id;
		stuff_list[i].num= level_cfg->consume_item_list[i].num;
		stuff_list[i].buyable = auto_buy;
		++ stuff_count;
	}

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
	{
		return;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "Mitama::OnUpgrade"))
	{
		return;
	}

	// 给予

	// 加小魂等级
	++ spirit_level;

	// 加御魂等级（如果可以）
	{
		bool can_upgrade = true;

		for (int index = 0; index < MITAMA_MAX_SPIRIT_COUNT; ++ index)
		{
			if (m_param.info_list[mitama_seq].spirit_level_list[index] <= mitama_level)
			{
				can_upgrade = false;
				break;
			}
		}

		if (can_upgrade)
		{
			++ mitama_level;

			// 清零所有小魂
			for (int index = 0; index < MITAMA_MAX_SPIRIT_COUNT; ++ index)
			{
				m_param.info_list[mitama_seq].spirit_level_list[index] = 0;
			}

			m_role->NoticeNum(noticenum::NT_MITAMA_UPGRADE_SUCC);
		}
	}
	
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MITAMA);

	// 发送
	this->SendSingleInfo(mitama_seq);

	// 日志
	gamelog::g_log_mitama.printf(LL_INFO, "Mitama::OnUpgrade user[%d %s], mitama_seq[%d], spirit_index[%d], mitama_level[%d], spirit_level[%d]", 
		m_role->GetUID(), m_role->GetName(), mitama_seq, spirit_index, mitama_level, spirit_level);
}

void Mitama::OnTaskFighting(int mitama_seq, int task_seq)
{
	if (mitama_seq < 0 || mitama_seq >= MITAMA_MAX_MITAMA_COUNT)
	{
		return;
	}

	// 检查任务状态是否为待命
	if (MITAMA_TASK_STATUS_IDLE != m_param.info_list[mitama_seq].task_status)
	{
		m_role->NoticeNum(errornum::EN_MITAMA_TASK_CANT_FIGHTING);
		return;
	}

	// 检查御魂等级是否足够
	const MitamaTaskInfoConfig *task_info_cfg = LOGIC_CONFIG->GetMitamaCfg().GetTaskInfoCfg(task_seq);
	if (NULL == task_info_cfg)
	{
		return;
	}

	if (m_param.info_list[mitama_seq].level < task_info_cfg->level)
	{
		m_role->NoticeNum(errornum::EN_MITAMA_TASK_LEVEL_LIMIT);
		return;
	}

	// 执行任务
	m_param.info_list[mitama_seq].task_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_param.info_list[mitama_seq].task_status = MITAMA_TASK_STATUS_PROCEEDING;
	m_param.info_list[mitama_seq].task_seq = task_seq;

	m_role->NoticeNum(noticenum::NT_MITAMA_TASK_FIGHTING_SUCC);

	this->SendSingleInfo(mitama_seq);
}

void Mitama::OnTaskFetchReward(int mitama_seq)
{
	if (mitama_seq < 0 || mitama_seq >= MITAMA_MAX_MITAMA_COUNT)
	{
		return;
	}

	// 检查任务状态是否可领奖
	if (MITAMA_TASK_STATUS_AWARD != m_param.info_list[mitama_seq].task_status)
	{
		return;
	}

	// 检查背包是否足够
	const MitamaTaskInfoConfig *task_info_cfg = LOGIC_CONFIG->GetMitamaCfg().GetTaskInfoCfg(m_param.info_list[mitama_seq].task_seq);
	if (NULL == task_info_cfg)
	{
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(task_info_cfg->reward_item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 获取奖励
	gamelog::g_log_mitama.buff_printf("Mitama::OnTaskAward user[%d %s], mitama_seq[%d], task_seq[%d], reward_count[%d], reward[",
		m_role->GetUID(), m_role->GetName(), mitama_seq, task_info_cfg->seq, task_info_cfg->reward_item_count);

	for (int i = 0; i < task_info_cfg->reward_item_count; ++ i)
	{
		const MitamaTaskRewardConfig::RewardInfo *reward_cfg = LOGIC_CONFIG->GetMitamaCfg().GetRandomTaskRewardCfg(m_param.info_list[mitama_seq].task_seq);
		if (NULL == reward_cfg)
		{
			continue;
		}

		m_role->GetKnapsack()->Put(reward_cfg->reward_item, PUT_REASON_MITAMA_TASK_REWARD);

		gamelog::g_log_mitama.buff_printf("(%s)", ItemConfigDataLog(&reward_cfg->reward_item, NULL));
	}

	// 重置任务状态
	m_param.info_list[mitama_seq].task_begin_timestamp = 0;
	m_param.info_list[mitama_seq].task_status = MITAMA_TASK_STATUS_IDLE;
	m_param.info_list[mitama_seq].task_seq = -1;

	m_role->NoticeNum(noticenum::NT_MITAMA_TASK_FETCH_REWARD_SUCC);

	// 发送
	this->SendSingleInfo(mitama_seq);

	// 日志
	gamelog::g_log_mitama.buff_printf("]");
	gamelog::g_log_mitama.commit_buff(LL_INFO);
}

void Mitama::OnExchangeItem(int item_seq, int num)
{
	if (item_seq < 0 || item_seq >= MITAMA_MAX_EXCHANGE_ITEM_COUNT || num <= 0)
	{
		return;
	}

	// 检查积分是否足够
	const MitamaExchangeItemCfg *item_cfg = LOGIC_CONFIG->GetMitamaCfg().GetExchangeItemCfg(item_seq);
	if (NULL == item_cfg)
	{
		return;
	}

	if (this->GetHotSpringScore() < item_cfg->need_score * num)
	{
		m_role->NoticeNum(errornum::EN_MITAMA_EXCHANGE_SCORE_NOT_ENOUGH);
		return;
	}

	// 检查背包空间是否足够
	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 消耗
	this->AddHotSpringScore(-item_cfg->need_score * num, "Mitama::OnExchangeItem");

	// 给予
	ItemConfigData item;
	item.item_id = item_cfg->exchange_item.item_id;
	item.num = item_cfg->exchange_item.num * num;
	item.is_bind = item_cfg->exchange_item.is_bind;
	m_role->GetKnapsack()->Put(item, PUT_REASON_MITAMA_EXCHANGE_ITEM);

	// 发送
	this->SendHotSpringScore();

	// 日志
	gamelog::g_log_mitama.printf(LL_INFO, "Mitama::OnExchangeItem user[%d %s], item_seq[%d], num[%d], need_score[%d], cur_score[%d]",
		m_role->GetUID(), m_role->GetName(), item_seq, num, item_cfg->need_score * num, this->GetHotSpringScore());
}

void Mitama::SendAllInfo()
{
	static Protocol::SCMitamaAllInfo mali;

	memcpy(mali.info_list, m_param.info_list, sizeof(mali.info_list));
	mali.hotspring_score = m_param.hotspring_score;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&mali, sizeof(mali));
}

void Mitama::SendSingleInfo(int mitama_seq)
{
	if (mitama_seq < 0 || mitama_seq >= MITAMA_MAX_MITAMA_COUNT)
	{
		return;
	}

	static Protocol::SCMitamaSingleInfo msli;

	msli.seq = mitama_seq;
	msli.info = m_param.info_list[mitama_seq];

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&msli, sizeof(msli));
}

void Mitama::SendHotSpringScore()
{
	static Protocol::SCMitamaHotSpringScore mhss;

	mhss.hotspring_score = m_param.hotspring_score;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&mhss, sizeof(mhss));
}

void Mitama::CheckTaskStatus()
{
	for (int mitama_seq = 0; mitama_seq < MITAMA_MAX_MITAMA_COUNT; ++ mitama_seq)
	{
		if (MITAMA_TASK_STATUS_PROCEEDING == m_param.info_list[mitama_seq].task_status)
		{
			const MitamaTaskInfoConfig * task_info_cfg = LOGIC_CONFIG->GetMitamaCfg().GetTaskInfoCfg(m_param.info_list[mitama_seq].task_seq);
			if (NULL != task_info_cfg)
			{
				unsigned int task_end_timestamp = m_param.info_list[mitama_seq].task_begin_timestamp + task_info_cfg->need_time * SECOND_PER_MIN;

				if (task_end_timestamp < static_cast<unsigned int>(EngineAdapter::Instance().Time()))
				{
					m_param.info_list[mitama_seq].task_status = MITAMA_TASK_STATUS_AWARD;
				}
			}
		}
	}
}

void Mitama::AddHotSpringScore(int add_score, const char* reason)
{
	if (NULL == reason)
	{
		return;
	}

	long long score_tmp = m_param.hotspring_score + (long long)add_score;
	if (score_tmp > INT_MAX)
	{
		score_tmp = INT_MAX;
	}
	else if(score_tmp < 0)
	{
		score_tmp = 0;
	}

	m_param.hotspring_score = (int)score_tmp;

	this->SendHotSpringScore();

	gamelog::g_log_mitama.printf(LL_INFO, "Mitama::AddHotSpringScore user[%d %s], add_score[%d], cur_score[%d], reason[%s]",
		m_role->GetUID(), m_role->GetName(), add_score, m_param.hotspring_score, reason);
}