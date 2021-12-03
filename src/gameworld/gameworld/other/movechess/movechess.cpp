#include "movechess.hpp"

#include "servercommon/configcommon.h"

#include "item/itemgriddata.h"
#include "item/itempool.h"
#include "item/itemextern.hpp"

#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/commondata.hpp"

#include "protocal/msgrole.h"

#include "other/event/eventhandler.hpp"
#include "protocal/msgitem.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"

#include "item/knapsack.h"
#include "other/sublock/sublock.hpp"

MoveChess::MoveChess()
{
	m_max_step = 0;
	memset(m_shake_point_rate,0,sizeof(m_shake_point_rate));
}

MoveChess::~MoveChess()
{
	
}

bool MoveChess::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;

	PUGI_XML_LOAD_AND_CHECK(document, configname, "MoveChess", *err);


	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = InitMoveChessOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: Init Move Chess Other failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode reward_element = RootElement.child("reward");
		if (reward_element.empty())
		{
			*err = configname + ": no [reward].";
			return false;
		}

		iRet = InitMoveChessRewardCfg(reward_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: Init Move Chess Reward failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode shake_rate_element = RootElement.child("shake_rate");
		if (shake_rate_element.empty())
		{
			*err = configname + ": no [shake_rate].";
			return false;
		}

		iRet = InitMoveChessShakeCfg(shake_rate_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: Init Move Chess Shake Rate failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode shake_rate_element = RootElement.child("special_reward_grid");
		if (shake_rate_element.empty())
		{
			*err = configname + ": no [special_reward_grid].";
			return false;
		}

		iRet = InitMoveChessSpecialGirdCfg(shake_rate_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: Init Move Chess Special Gird failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int MoveChess::InitMoveChessOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "consume_gold_count", m_other_cfg.consume_gold_count) || m_other_cfg.consume_gold_count <= 0 )
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "free_times_per_day", m_other_cfg.free_times_per_day) || m_other_cfg.free_times_per_day <= 0 )
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "reset_time_per_day", m_other_cfg.reset_time_per_day) || m_other_cfg.reset_time_per_day < 0 )
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "reset_consume_gold", m_other_cfg.reset_consume_gold) || m_other_cfg.reset_consume_gold < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "free_cd", m_other_cfg.free_cd) || m_other_cfg.free_cd < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "replace_item_id", m_other_cfg.replace_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.replace_item_id))
	{
		return -6;
	}

	PugiXmlNode Itemelement = dataElement.child("item1");
	ItemID itemid = 0;
	if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
	{
		if (!m_other_cfg.consume_item.ReadConfig(Itemelement))
		{
			return -7;
		}
	}

	Itemelement = dataElement.child("item2");
	itemid = 0;
	if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
	{
		if (!m_other_cfg.reset_reward_item.ReadConfig(Itemelement))
		{
			return -8;
		}
	}

	{
		PugiXmlNode element = dataElement.child("end_step_reward_list");
		if (element.empty())
		{
			return -100;
		}

		int i = 0;
		PugiXmlNode item_element = element.child("end_step_reward");
		while (!item_element.empty())
		{
			if (i >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -101 - i;
			}

			if (!m_other_cfg.max_step_reward[i].ReadConfig(item_element))
			{
				return - 151 - i;
			}

			i++;
			item_element = item_element.next_sibling();
		}
	}

	if (!PugiGetSubNodeValue(dataElement, "max_step", m_max_step) || m_max_step <= 0)
	{
		return -12;
	}

	return 0;
}

int MoveChess::InitMoveChessRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	//short last_item_id = 0;
	while(!dataElement.empty())
	{
		PugiXmlNode Itemelement = dataElement.child("reward_item");
		ItemID item_id = 0;
		ItemConfigData item_data;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", item_id) && NULL != ITEMPOOL->GetItem(item_id))
		{
			if (!item_data.ReadConfig(Itemelement))
			{
				return -1;
			}
		}
		
		if (m_reward_item_map.end() != m_reward_item_map.find(item_id))
		{
			return -100;
		}

		MoveChessRewardCfg &cfg = m_reward_item_map[item_id];
		cfg.reward_item = item_data;
		//last_item_id = item_id;

		if (!PugiGetSubNodeValue(dataElement,"normal_rate",cfg.rate_list[MOVE_CHESS_MODE_NORMAL]) || cfg.rate_list[MOVE_CHESS_MODE_NORMAL] < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement,"special_rate",cfg.rate_list[MOVE_CHESS_MODE_SPECIAL]) || cfg.rate_list[MOVE_CHESS_MODE_SPECIAL] < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement,"is_broadcast",cfg.is_broadcast) || cfg.is_broadcast < 0 || cfg.is_broadcast > 1)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement,"can_repeat",cfg.can_repeat) || cfg.can_repeat < 0 || cfg.can_repeat > 0)
		{
			return -6;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int MoveChess::InitMoveChessShakeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}
	
	int total_rate = 0;
	while(!dataElement.empty())
	{
		int shake_point = 0;
		if (!PugiGetSubNodeValue(dataElement, "shake_point" ,shake_point) || shake_point <= MOVE_CHESS_POINT_INVALID || shake_point >= MOVE_CHESS_POINT_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement,"rate",m_shake_point_rate[shake_point]) || m_shake_point_rate[shake_point] < 0)
		{
			return -2;
		}
		total_rate += m_shake_point_rate[shake_point];

		dataElement = dataElement.next_sibling();
	}

	if (0 >= total_rate)
	{
		return -3;
	}

	return 0;
}

int MoveChess::InitMoveChessSpecialGirdCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		int special_gird = 0;
		if (!PugiGetSubNodeValue(dataElement, "special_reward_grid" ,special_gird) || special_gird <= 0 || special_gird >= m_max_step || m_special_step.end() != m_special_step.find(special_gird))
		{
			return -1;
		}

		if (m_special_step.find(special_gird) != m_special_step.end())
		{
			return -2;
		}

		MoveChessSpecialStepCfg &cfg = m_special_step[special_gird];
		cfg.special_reward_grid = special_gird;

		if (!PugiGetSubNodeValue(dataElement, "is_notice", cfg.is_notice))
		{
			return -3;
		}

		{
			PugiXmlNode element = dataElement.child("reward_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - i;
				}

				if (!cfg.reward[i].ReadConfig(item_element))
				{
					return -151 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}
		
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

void MoveChess::Shake(Role *role, short is_use_item)
{
	if (NULL == role) return;

	if (role->GetKnapsack()->IsLock() || role->GetSublock()->CheckHasLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(3))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	CommonDataParam *common_p = role->GetCommonDataParam();
	if(NULL == common_p) return;

	if (common_p->move_chess_cur_step >= m_max_step)
	{
		role->NoticeNum(errornum::EN_MOVE_CHESS_ARRIVE_END_POINT);
		return;
	}

	int total_rate = 0;
	for (int i = MOVE_CHESS_POINT_ONE; i < MOVE_CHESS_POINT_MAX; i++)
	{
		total_rate += m_shake_point_rate[i];
	}

	if (total_rate <= 0)
	{
		return;
	}

	int shake_point = 0;
	int shake_rand_value = RandomNum(total_rate);
	for(int shake_index = MOVE_CHESS_POINT_ONE ; shake_index < MOVE_CHESS_POINT_MAX; shake_index++)
	{
		if (shake_rand_value < m_shake_point_rate[shake_index])
		{
			shake_point = shake_index;
			break;
		}

		shake_rand_value -= m_shake_point_rate[shake_index];
	}

	if (shake_point == 0)
	{
		return;
	}

	unsigned int now_sec = (unsigned int)(EngineAdapter::Instance().Time());
	int gold_cost = m_other_cfg.consume_gold_count;

	if (common_p->move_chess_free_times < m_other_cfg.free_times_per_day)
	{
		if ((common_p->move_chess_next_free_time == 0 || now_sec > common_p->move_chess_next_free_time))
		{
			gold_cost = 0;
		}
	}

	bool is_result_bind = true; // 获得的奖励默认绑定，若消耗元宝则设置成不绑定
	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	if (gold_cost > 0)
	{
		if (is_use_item)
		{
			ItemID use_item_id = m_other_cfg.consume_item.item_id;
			if (NULL != ITEMPOOL->GetItem(use_item_id) && role->GetLevel() >= ITEMPOOL->GetItem(use_item_id)->GetLimitLevel())
			{
				ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
				stuff_list[0].item_id = use_item_id;
				stuff_list[0].num = m_other_cfg.consume_item.num;
				stuff_list[0].buyable = true;

				if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, false))
				{
					return;
				}

				gold_cost = consume_list.need_gold_num;
			}
		}
		else
		{
			is_result_bind = false;
		}
	}

	if (gold_cost > 0)
	{
		if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(gold_cost))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}
	
	if (!this->GetReward(role, shake_point, is_result_bind)) return;
	
	// 消耗
	if (gold_cost == 0 && consume_list.count == 0)
	{
		++common_p->move_chess_free_times;

		if (common_p->move_chess_free_times >= m_other_cfg.free_times_per_day)
		{
			int free_cd = EngineAdapter::Instance().NextDayInterval(0,0,0);
			common_p->move_chess_next_free_time = now_sec + (unsigned int)(free_cd);
		}
		else
		{
			common_p->move_chess_next_free_time = now_sec + (unsigned int)(m_other_cfg.free_cd);
		}	
	}
	else
	{
		role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "MoveChess");
		EventHandler::Instance().OnShopBuyItem(role, consume_list.buy_count, consume_list.buy_list, consume_list.need_gold_num, 0);

		if (gold_cost > 0)
		{
			role->GetKnapsack()->GetMoney()->UseGold(gold_cost, "MoveChess");

			gamelog::g_log_movechess.buff_printf("MoveChess::Buy [user[%d %s] gold_cost:%d [", 
				role->GetUID(), (const char*)role->GetName(), (int)gold_cost);
		}
	}

	this->SendShakePoint(role, shake_point);
	role->SendMoveChessInfo();

	gamelog::g_log_function_stats.printf(LL_INFO, "[Join][user[%s %d] type:MoveChess times:1]",
		role->GetName(), role->GetUID());
}

void MoveChess::BuyReset(Role *role)
{
	if (NULL == role) return;

	CommonDataParam *common_p = role->GetCommonDataParam();
	if (NULL == common_p) return;
	
	if (common_p->move_chess_reset_times >= m_other_cfg.reset_time_per_day)
	{
		role->NoticeNum(errornum::EN_MOVE_CHESS_RESET_TIME_LIMIT);
		return;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (m_other_cfg.reset_consume_gold > 0 && !role->GetKnapsack()->GetMoney()->UseGold(m_other_cfg.reset_consume_gold, "MoveChesetReset"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	common_p->move_chess_reset_times++;
	common_p->move_chess_cur_step = 0;
	role->SendMoveChessInfo();

	role->GetKnapsack()->PutOrMail(m_other_cfg.reset_reward_item, PUT_REASON_MOVE_CHESS);
}

void MoveChess::SendShakePoint(Role* role, int shake_point)
{
	if (NULL == role || shake_point <= MOVE_CHESS_POINT_INVALID || shake_point >= MOVE_CHESS_POINT_MAX) return;

	static Protocol::SCMoveChessShakePoint cmd;
	cmd.shake_point = shake_point;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}

bool MoveChess::GetReward(Role *role, int shake_point, bool is_bind)
{
	if (NULL == role) return false;

	CommonDataParam *common_p = role->GetCommonDataParam();
	if (NULL == common_p) return false;

	if (common_p->move_chess_cur_step >= m_max_step)
	{
		return false;
	}

	int cur_shake_count = shake_point;
	if (cur_shake_count <= MOVE_CHESS_POINT_INVALID || cur_shake_count >= MOVE_CHESS_POINT_MAX)
	{
		return false;
	}

	int have_move_step_old = common_p->move_chess_cur_step;

	int item_index = 0;
	int rate_count = 0;
	int normal_total_rate = 0;
	int special_total_rate = 0;
	std::vector<int>item_index_list;
	std::vector<int>normal_rate_list;
	std::vector<int>special_rate_list;

	for (MoveChessItemMapIt it = m_reward_item_map.begin(), end = m_reward_item_map.end(); it != end; ++it)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(it->second.reward_item.item_id);
		if (NULL != item_base)
		{
			item_index_list.push_back(item_index); // 记录下物品的索引
			normal_rate_list.push_back( it->second.rate_list[MOVE_CHESS_MODE_NORMAL]);
			normal_total_rate += normal_rate_list[rate_count];

			special_rate_list.push_back(it->second.rate_list[MOVE_CHESS_MODE_SPECIAL]);
			special_total_rate += special_rate_list[rate_count];

			++ rate_count;
		}
		item_index++;
	}

	if (normal_total_rate <= 0) return false;

	int list_num = 0;
	static ItemConfigData item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	memset(item_list, 0, sizeof(item_list));

	int curr_count = 1, round_count = 0;

	int notcie_count = 0;
	ItemID notice_item_list[10];
	memset(notice_item_list, 0, sizeof(notice_item_list));

	Protocol::SCMoveChessStepRewardInfo step_info;
	step_info.item_count = 0;

	while (round_count++ < rate_count && curr_count <= cur_shake_count)
	{
		int cur_step = have_move_step_old + curr_count;

		if (cur_step > m_max_step)
		{
			break;
		}

		bool is_special_step = false;
		if (m_special_step.end() != m_special_step.find(cur_step))
		{
			is_special_step = true;
		}

		// 走到终点，SUPREMACY模式
		if (cur_step == m_max_step)
		{
			for(int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
			{
				UInt16 item_id = m_other_cfg.max_step_reward[i].item_id;
				int item_num = m_other_cfg.max_step_reward[i].num;

				MoveChessItemMapIt reward_item = m_reward_item_map.find(item_id);
				if (reward_item != m_reward_item_map.end() && !reward_item->second.can_repeat)
				{
					for (int j = 0; j < list_num; j++) // 重复检查
					{
						if (item_list[j].item_id == item_id)
						{
							item_id = m_other_cfg.replace_item_id;
							break;
						}
					}
				}

				ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, item_list, &list_num, 
						item_id, item_num, is_bind);

				++ curr_count;

				if (item_id > 0 && item_num > 0 && step_info.item_count >= 0 && step_info.item_count < Protocol::SCMoveChessStepRewardInfo::MAX_REWARD_ITEM_COUNT)
				{
					step_info.reward_list[step_info.item_count].step = curr_count;
					step_info.reward_list[step_info.item_count].item_id = item_id;
					step_info.reward_list[step_info.item_count].item_num = item_num;
					step_info.item_count++;
				}
			}
		}
		else if (cur_step > 0 && cur_step < m_max_step)
		{
			int rand_index = 0;
			int rand_value = 0;

			if (is_special_step)
			{
				// 走到特殊格子，SPECIAL模式

				if (special_total_rate > 0)
				{
					rand_value = RandomNum(special_total_rate);

					for (rand_index = 0; rand_index < rate_count; rand_index++)
					{
						if (special_rate_list[rand_index] <= 0)
							continue;

						if (rand_value <= special_rate_list[rand_index]) break;

						rand_value -= special_rate_list[rand_index];
					}
				}

				std::map<int, MoveChessSpecialStepCfg>::iterator iter = m_special_step.find(cur_step);
				if (iter != m_special_step.end())
				{
					for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
					{
						if (0 <= iter->second.reward[i].item_id)
						{
							ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, item_list, &list_num,
								iter->second.reward[i].item_id, iter->second.reward[i].num, is_bind);

							if (iter->second.reward[i].item_id > 0 && iter->second.reward[i].num > 0 && step_info.item_count >= 0 && step_info.item_count < Protocol::SCMoveChessStepRewardInfo::MAX_REWARD_ITEM_COUNT)
							{
								step_info.reward_list[step_info.item_count].step = curr_count;
								step_info.reward_list[step_info.item_count].item_id = iter->second.reward[i].item_id;
								step_info.reward_list[step_info.item_count].item_num = iter->second.reward[i].num;
								step_info.item_count++;

								if (notcie_count >= 0 && notcie_count < 10 && iter->second.is_notice != 0)
								{
									notice_item_list[notcie_count] = iter->second.reward[i].item_id;
									notcie_count++;
								}
							}
						}
					}
				}
			}
			else
			{
				// 走到普通格子，NORMAL模式
				rand_value = RandomNum(normal_total_rate);

				for (rand_index = 0; rand_index < rate_count; rand_index++)
				{
					if (normal_rate_list[rand_index] <= 0)
						continue;

					if (rand_value <= normal_rate_list[rand_index]) break;

					rand_value -= normal_rate_list[rand_index];
				}
			}

			if (rand_index >= 0 && rand_index < rate_count)
			{
				int item_index = item_index_list[rand_index];

				if (item_index >= 0 && item_index < (int)m_reward_item_map.size())
				{
					MoveChessItemMapIt reward_item = m_reward_item_map.begin();
					std::advance(reward_item, item_index);

					UInt16 item_id = reward_item->second.reward_item.item_id;
					int item_num = reward_item->second.reward_item.num;

					if (!reward_item->second.can_repeat)
					{
						for (int j = 0; j < list_num; j++) // 重复检查
						{
							if (item_list[j].item_id == item_id)
							{
								item_id = m_other_cfg.replace_item_id;
								break;
							}
						}
					}

					if (notcie_count >= 0 && notcie_count < 10 && reward_item->second.is_broadcast > 0 && item_id == reward_item->second.reward_item.item_id)
					{
						notice_item_list[notcie_count] = item_id;
						notcie_count++;
					}

					ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, item_list, &list_num, 
						item_id, item_num, is_bind);

					++ curr_count;

					if (item_id > 0 && item_num > 0 && step_info.item_count >= 0 && step_info.item_count < Protocol::SCMoveChessStepRewardInfo::MAX_REWARD_ITEM_COUNT)
					{
						step_info.reward_list[step_info.item_count].step = curr_count;
						step_info.reward_list[step_info.item_count].item_id = item_id;
						step_info.reward_list[step_info.item_count].item_num = item_num;
						step_info.item_count++;
					}
				}
			}
		}
	}

	common_p->move_chess_cur_step += cur_shake_count;
	if (common_p->move_chess_cur_step > m_max_step)
	{
		common_p->move_chess_cur_step = m_max_step;
	}
	
	for (int item_count = 0; item_count < list_num && item_count < ItemNamespace::MAX_KNAPSACK_GRID_NUM; item_count++)
	{
		role->GetKnapsack()->PutOrMail(item_list[item_count], PUT_REASON_MOVE_CHESS);
	}

	if (list_num > 0)
	{
		ItemExtern::SendRewardListInfo(role, item_list, list_num, Protocol::SCRewardListInfo::NOTICE_TYPE_MOVE_CHESS);

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&step_info, 
			sizeof(Protocol::SCMoveChessStepRewardInfo) - sizeof(Protocol::SCMoveChessStepRewardInfo::StepRewardInfo) * (Protocol::SCMoveChessStepRewardInfo::MAX_REWARD_ITEM_COUNT - step_info.item_count));
	}

	for (int i = 0; i < notcie_count && i < 10; i++)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_movechess_good_item,
			role->GetUID(), role->GetName(), (int)role->GetCamp(), notice_item_list[i]);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	return true;
}
