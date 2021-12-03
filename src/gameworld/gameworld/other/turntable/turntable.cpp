#include "turntable.h"
#include "other/turntable/turntableconfig.h"
#include "gameworld/config/logicconfigmanager.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/commondata.hpp"
#include "obj/character/role.h"
#include "gameworld/gameworld/world.h"
#include "item/itempool.h"
#include "item/itemextern.hpp"
#include "other/route/mailroute.hpp"
#include "item/knapsack.h"
#include "scene/scene.h"

TurnTable::TurnTable() : m_role(NULL)
{
	memset(m_lucky_value_list, 0, sizeof(m_lucky_value_list));
}

TurnTable::~TurnTable()
{
}

void TurnTable::Init(Role *role, const short lucky_value_list[TURNTABLE_TYPE_MAX_COUNT])
{
	m_role = role;
	memcpy(m_lucky_value_list, lucky_value_list, sizeof(m_lucky_value_list));
}

void TurnTable::GetInitParam(short lucky_value_list[TURNTABLE_TYPE_MAX_COUNT])
{
	memcpy(lucky_value_list, m_lucky_value_list, sizeof(m_lucky_value_list));
}

void TurnTable::OnRoll(int req_type)
{
	if (req_type < 0 || req_type >= TURNTABLE_TYPE_MAX_COUNT)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_HAS_NO_SPACING);
		return;
	}

	const TurnTableRollCfg *roll_cfg = LOGIC_CONFIG->GetTurnTableConfig().GetRollCfgList(req_type);
	if (NULL == roll_cfg)
	{
		return;
	}

	CommonDataParam *common_p = m_role->GetCommonDataParam();
	if (req_type == TURNTABLE_TYPE_MILLIIONAIRE && common_p->is_turn)
	{
		m_role->NoticeNum(errornum::EN_MILLIONAIRE_HAD_TURN);
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	short stuff_count = 0;
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

	//只需要一个物品
	stuff_list[stuff_count].item_id = roll_cfg->need_item;
	stuff_list[stuff_count].num = roll_cfg->need_num;
	stuff_list[stuff_count].buyable = false;
	++ stuff_count;

	if (req_type != TURNTABLE_TYPE_MILLIIONAIRE && !ItemExtern::GetConsumeListByOrder(m_role, true, stuff_count, stuff_list, &consume_list, true))
	{
		return;
	}

	if (req_type != TURNTABLE_TYPE_MILLIIONAIRE && consume_list.count <= 0)
	{
		return;
	}

	int rewards_index = -1;

	if (m_lucky_value_list[req_type] >= roll_cfg->lucky_max)
	{
		rewards_index = roll_cfg->great_index;
	}
	else
	{
		int total_weight = 0;

		for(int i = 0; i < TURNTABLE_REWARDS_MAX_COUNT ; ++ i)
		{
			total_weight += roll_cfg->rewards[i].weight;
		}

		if (total_weight <= 0)
		{
			return;
		}

		int rand_weight = RandomNum(total_weight);

		for(int i = 0; i < TURNTABLE_REWARDS_MAX_COUNT ; ++ i)
		{
			if (rand_weight < roll_cfg->rewards[i].weight)
			{
				rewards_index = i;

				break;
			}

			rand_weight -= roll_cfg->rewards[i].weight;
		}
	}

	if (rewards_index < 0)
	{
		return;
	}

	if (req_type != TURNTABLE_TYPE_MILLIIONAIRE && !m_role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "TurnTableRoll"))
	{
		return;
	}	
	
	if (roll_cfg->great_index == roll_cfg->rewards[rewards_index].item_index)
	{
		m_lucky_value_list[req_type] = 0;
	}
	else
	{
		++ m_lucky_value_list[req_type];
	}
	
	// 抽到大富豪奖励邮件发送，其他直接扔背包
	if (req_type == TURNTABLE_TYPE_MILLIIONAIRE && roll_cfg->great_index == roll_cfg->rewards[rewards_index].item_index)
	{
		// 打包奖励物品
		ItemConfigData reward_item_1[3] = {roll_cfg->rewards[1].item,roll_cfg->rewards[2].item,roll_cfg->rewards[3].item};
		ItemConfigData reward_item_2[3] = {roll_cfg->rewards[4].item,roll_cfg->rewards[5].item,roll_cfg->rewards[6].item};
		ItemConfigData reward_item_3[3] = {roll_cfg->rewards[7].item};

		// 抽到大富豪，改变状态
		common_p->is_turn_millionaire = 1;

		this->NotifyImage(m_role);

		// 奖励优先放背包，背包满了发邮件
		m_role->GetKnapsack()->PutListOrMail(reward_item_1,PUT_REASON_TURNTABLE);
		m_role->GetKnapsack()->PutListOrMail(reward_item_2,PUT_REASON_TURNTABLE);
		m_role->GetKnapsack()->PutListOrMail(reward_item_3,PUT_REASON_TURNTABLE);
	}
	else	
	{
		m_role->GetKnapsack()->Put(roll_cfg->rewards[rewards_index].item, PUT_REASON_TURNTABLE);
	}

	if (req_type == TURNTABLE_TYPE_MILLIIONAIRE)
	{
		common_p->is_turn = 1;
		common_p->millionaire_reward_index = rewards_index;
		m_role->SendMillionaireInfo();
	}

	if (roll_cfg->rewards[rewards_index].is_notice)
	{
		if (m_role->GetCommonDataParam()->is_turn_millionaire && req_type == TURNTABLE_TYPE_MILLIIONAIRE)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_turntable_millionaire,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}
		else
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_turntable_great_gift,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), (int)roll_cfg->need_item, (int)roll_cfg->rewards[rewards_index].item.item_id);

			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	Protocol::SCTurnTableReward ttr;
	ttr.type = static_cast<char>(req_type);
	ttr.rewards_index = static_cast<char>(rewards_index);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ttr, sizeof(ttr));

	this->SendInfo();
}

void TurnTable::SendInfo()
{
	Protocol::SCTurnTableInfo tti;
	memcpy(tti.lucky_point, m_lucky_value_list, sizeof(tti.lucky_point));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&tti, sizeof(tti));
}

void TurnTable::NotifyImage(Role *role)
{
	static Protocol::SCTurnTableMillionaireView cmd;

	cmd.obj_id = role->GetId();
	cmd.is_millionaire = (short)role->GetCommonDataParam()->is_turn_millionaire;

	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}


