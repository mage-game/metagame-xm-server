#include "mazemanager.hpp"

#include "gamelog.h"
#include "engineadapter.h"

#include "obj/character/role.h"

#include "servercommon/errornum.h"
#include "protocal/msgroleother.h"

#include "scene/scene.h"
#include "mazeconfig.hpp"

#include "config/logicconfigmanager.hpp"
#include "servercommon/noticenum.h"
#include "other/event/eventhandler.hpp"
#include <algorithm>

MazeManager::MazeManager()
	: m_role(NULL)
{
}

MazeManager::~MazeManager()
{

}

void MazeManager::Init(Role *role, const MazeParam &maze_param)
{
	m_role = role;
	m_maze_param = maze_param;
}

void MazeManager::GetOtherInitParam(MazeParam *param)
{
	*param = m_maze_param;
}

void MazeManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
// 	if (old_dayid == now_dayid)
// 	{
// 		return;
// 	}
// 
// 	this->StartMaze();
// 
// 	this->OnQueryMaze(Protocol::SCMazeInfo::REASON_DAYCHANGE);
}

void MazeManager::StartMaze()
{
// 	m_maze_param.Reset();
// 
// 	bool born_in_1_1 = false;
// 	int rand_val = RandomNum(100);
// 	if (rand_val < 50)
// 	{
// 		born_in_1_1 = true;
// 	}
// 
// 	std::vector<Posi> tmp_vec;
// 
// 	if (born_in_1_1)
// 	{
// 		// 在附近(10, 01, 11)随1个，其他地方随9个宝箱
// 		int rand_val = RandomNum(3);
// 		if (0 == rand_val)
// 		{
// 			this->SetBox(1, 0);
// 		}
// 		else if (1 == rand_val)
// 		{
// 			this->SetBox(0, 1);
// 		}
// 		else
// 		{
// 			this->SetBox(1, 1);
// 		}
// 
// 		for (int i = 0; i < MazeParam::ONE_SIDE_COUNT; ++i)
// 		{
// 			for (int j = 0; j < MazeParam::ONE_SIDE_COUNT; ++j)
// 			{
// 				if (i <= 1 && j <= 1)
// 				{
// 					continue;
// 				}
// 
// 				tmp_vec.push_back(Posi(i, j));
// 			}
// 		}
// 	}
// 	else
// 	{
// 		// 在其他地方随10个宝箱
// 		for (int i = 0; i < MazeParam::ONE_SIDE_COUNT; ++i)
// 		{
// 			for (int j = 0; j < MazeParam::ONE_SIDE_COUNT; ++j)
// 			{
// 				if (i <= 0 && j <= 0)
// 				{
// 					continue;
// 				}
// 
// 				tmp_vec.push_back(Posi(i, j));
// 			}
// 		}
// 	}
// 
// 	int plant_box_count = MazeParam::MAX_BOX_COUNT;
// 	if (born_in_1_1)
// 	{
// 		plant_box_count -= 1;
// 	}
// 
// 	std::random_shuffle(tmp_vec.begin(), tmp_vec.end());
// 
// 	for (int i = 0; i < plant_box_count + MazeParam::MAX_BOMB_COUNT && i < (int)tmp_vec.size(); ++i)
// 	{
// 		if (i < plant_box_count)
// 		{
// 			Posi pos = tmp_vec[i];
// 			this->SetBox(pos.x, pos.y);
// 		}
// 		else
// 		{
// 			Posi pos = tmp_vec[i];
// 			this->SetBomb(pos.x, pos.y);
// 		}
// 	}
// 
// 	m_maze_param.curr_x = 0;
// 	m_maze_param.curr_y = 0;
// 
// 	this->SetOpen(m_maze_param.curr_x, m_maze_param.curr_y);
// 	this->RandShowBox();
// 	m_maze_param.buy_times = 0;
// 
// 	m_maze_param.maze_flag |= MazeParam::MZ_INIT;
// 
// 	this->PrintMaze();
}

void MazeManager::BuyMove()
{
// 	int move_times = m_maze_param.move_times;
// 	int left_times = m_maze_param.buy_times + MazeParam::MAX_FREEMOVE_TIMES - move_times;
// 
// 	if (left_times >= MazeParam::MAX_FREEMOVE_TIMES)
// 	{
// 		m_role->NoticeNum(errornum::EN_MAZE_MOVE_TIMES_FULL);
// 		return;
// 	}
// 
// 	bool is_all_grid_open = true;
// 	{
// 		for (int i = 0; i < MazeParam::ONE_SIDE_COUNT; ++ i)
// 		{
// 			for (int j = 0; j < MazeParam::ONE_SIDE_COUNT; ++ j)
// 			{
// 				if (!this->IsOpen(i, j))
// 				{
// 					is_all_grid_open = false;
// 					break;
// 				}
// 			}
// 		}
// 	}
// 	if (is_all_grid_open)
// 	{
// 		m_role->NoticeNum(errornum::EN_MAZE_GAME_FINISH);
// 		return;
// 	}
// 
// 	int need_gold = LOGIC_CONFIG->GetMazeConfig().GetBuyCostGold(m_maze_param.buy_times + 1);		
// 	if (need_gold <= 0)
// 	{
// 		m_role->NoticeNum(errornum::EN_MAZE_CANNOT_BUY_MOVE_TIMES);
// 		return;
// 	}
// 	
// 	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "MazeBuyMove"))
// 	{
// 		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 		return;
// 	}
// 
// 	m_maze_param.buy_times += 1;
// 
// 	this->OnQueryMaze(Protocol::SCMazeInfo::REASON_BUY);
// 
// 	m_role->NoticeNum(noticenum::NT_MAZE_BUY_MOVE_TIMES_SUCC);
// 
// 	{
// 		// 日志
// 		gamelog::g_log_maze.printf(LL_INFO, "Maze::BuyMove [%d, %s] use_all_gold[%d], curr_buy_times[%d]", 
// 			m_role->GetUID(), m_role->GetName(), need_gold, m_maze_param.buy_times);
// 
// 		ROLE_LOG_QUICK6(LOG_TYPE_MAZE_BUY_MOVE, m_role, need_gold, m_maze_param.buy_times, NULL, NULL);
// 	}
}

void MazeManager::OnQueryMaze(short reason)
{
// 	if (0 == (m_maze_param.maze_flag & MazeParam::MZ_INIT))
// 	{
// 		this->StartMaze();
// 	}
// 
// 	// send
// 	Protocol::SCMazeInfo scinfo;
// 
// 	scinfo.reason = reason;
// 	scinfo.jifen_reward_flag = m_maze_param.jifen_reward_flag;
// 
// 	scinfo.shown_box_x = m_maze_param.shown_box_x;
// 	scinfo.shown_box_y = m_maze_param.shown_box_y;
// 	scinfo.today_item_count = m_maze_param.today_item_count;
// 
// 	scinfo.today_coin = m_maze_param.today_coin;
// 	scinfo.buy_times = m_maze_param.buy_times;
// 	scinfo.curr_x = m_maze_param.curr_x;
// 	scinfo.curr_y = m_maze_param.curr_y;
// 	scinfo.jifen = m_maze_param.jifen;
// 
// 	scinfo.move_times = m_maze_param.move_times;
// 	scinfo.reserve_ch = 0;
// 	scinfo.reserve_sh = 0;
// 
// 	memset(scinfo.maze_grid, 0, sizeof(scinfo.maze_grid));
// 
// 	for (int i = 0; i < MazeParam::ONE_SIDE_COUNT; ++i)
// 	{
// 		for (int j = 0; j < MazeParam::ONE_SIDE_COUNT; ++j)
// 		{
// 			if (this->IsOpen(i, j) || (i == m_maze_param.shown_box_x && j == m_maze_param.shown_box_y))
// 			{
// 				scinfo.maze_grid[i][j] = m_maze_param.maze_grid[i][j];
// 			}
// 		}
// 	}
// 
// 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scinfo, sizeof(scinfo));
}

void MazeManager::OnMoveReq(int target_x, int target_y)
{
// 	JoinLimitUnit limit_unit;
// 	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_MAZE, &limit_unit);
// 
// 	if (is_limit && m_role->GetLevel() < limit_unit.level)
// 	{
// 		m_role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
// 		return;
// 	}
// 
// 	if (0 == (m_maze_param.maze_flag & MazeParam::MZ_INIT))
// 	{
// 		return;
// 	}
// 
// 	if (!this->IsValidPos(target_x, target_y))
// 	{
// 		return;
// 	}
// 
// 	if (this->IsOpen(target_x, target_y))
// 	{
// 		m_maze_param.curr_x = target_x;
// 		m_maze_param.curr_y = target_y;
// 
// 		this->OnQueryMaze(Protocol::SCMazeInfo::REASON_MOVE);
// 		return;
// 	}
// 
// 	if (!this->Connectable(m_maze_param.curr_x, m_maze_param.curr_y, target_x, target_y))
// 	{
// 		return;
// 	}
// 
// 	if (!m_role->GetKnapsack()->HaveSpace())
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 		return;
// 	}
// 
// 	int move_times = m_maze_param.move_times;
// 	if (move_times >= m_maze_param.buy_times + MazeParam::MAX_FREEMOVE_TIMES)
// 	{
// 		return;
// 	}
// 
// 	++ m_maze_param.move_times;
// 	
// 	{
// 		int join_flag = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_MAZE_JOIN_FLAG);
// 		
// 		if (join_flag <= 0)
// 		{
// 			m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_MAZE_JOIN_FLAG);
// 		}
// 	}
// 
// 	m_maze_param.curr_x = target_x;
// 	m_maze_param.curr_y = target_y;
// 	this->SetOpen(target_x, target_y);
// 
// 	if (this->IsBox(target_x, target_y))
// 	{
// 		int item_count = 0;
// 		int coin = 0;
// 
// 		LOGIC_CONFIG->GetMazeConfig().RandReward(item_count, coin);
// 
// 		if (item_count > 0)
// 		{
// 			ItemID item_id = LOGIC_CONFIG->GetMazeConfig().GetRewardItemID();
// 			m_role->GetKnapsack()->Put(ItemConfigData(item_id, true, item_count), PUT_REASON_MAZE);
// 
// 			m_maze_param.today_item_count += item_count;
// 
// 			this->SendMoveResult(Protocol::SCMazeMoveResult::RET_ITEM, item_count);
// 		}
// 		else if (coin > 0)
// 		{
// 			m_role->GetKnapsack()->GetMoney()->AddCoinBind(coin, "MazeBox");
// 
// 			m_maze_param.today_coin += coin;
// 
// 			this->SendMoveResult(Protocol::SCMazeMoveResult::RET_COIN, coin);
// 		}
// 
// 		++ m_maze_param.jifen;
// 
// 		if (m_maze_param.shown_box_x == target_x && m_maze_param.shown_box_y == target_y)
// 		{
// 			this->RandShowBox();
// 		}
// 
// 		{
// 			// 日志
// 			gamelog::g_log_maze.printf(LL_INFO, "Maze::HitBox user[%d, %s], move_times[%d],  get_coin[%d], get_item[%d]",
// 				m_role->GetUID(), m_role->GetName(), m_maze_param.move_times, coin, item_count);
// 		}
// 	}
// 	else if (this->IsBomb(target_x, target_y))
// 	{
// 		move_times = m_maze_param.move_times;
// 		if (move_times < m_maze_param.buy_times + MazeParam::MAX_FREEMOVE_TIMES)
// 		{
// 			++ m_maze_param.move_times;
// 		}
// 
// 		m_role->NoticeNum(errornum::EN_MAZE_HIT_BOMB);
// 		this->SendMoveResult(Protocol::SCMazeMoveResult::RET_BOMB, 0);
// 
// 		{
// 			// 日志
// 			gamelog::g_log_maze.printf(LL_INFO, "Maze::HitBox user[%d, %s], move_times[%d]",
// 				m_role->GetUID(), m_role->GetName(), m_maze_param.move_times);
// 		}
// 	}
// 	else
// 	{
// 		this->SendMoveResult(Protocol::SCMazeMoveResult::RET_DEFAULT, 0);
// 	}
// 
// 	this->OnQueryMaze(Protocol::SCMazeInfo::REASON_MOVE);
}

void MazeManager::OnFetchJifenReward(int seq)
{
// 	if (seq < 0 || seq >= (int)sizeof(m_maze_param.jifen_reward_flag) * 8)
// 	{
// 		return;
// 	}
// 
// 	if (0 != (m_maze_param.jifen_reward_flag & (1 << seq)))
// 	{
// 		m_role->NoticeNum(errornum::EN_HAS_FETCH_THIS_REWARD);
// 		return;
// 	}
// 
// 	const MazeJifenRewardConfig::ConfigItem * cfg_item = LOGIC_CONFIG->GetMazeConfig().GetJifenReward(seq);
// 	if (NULL == cfg_item)
// 	{
// 		return;
// 	}
// 
// 	if (m_maze_param.jifen < cfg_item->need_jifen)
// 	{
// 		m_role->NoticeNum(errornum::EN_REWARD_NOT_SATISFY);
// 		return;
// 	}
// 
// 	if (!m_role->GetKnapsack()->HaveSpace())
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 		return;
// 	}
// 
// 	m_maze_param.jifen_reward_flag |= (1 << seq);
// 
// 	m_role->GetKnapsack()->Put(cfg_item->reward_item, PUT_REASON_MAZE);
// 
// 	m_maze_param.today_item_count += cfg_item->reward_item.num;
// 
// 	this->OnQueryMaze();
// 
// 	m_role->NoticeNum(noticenum::NT_MAZE_FETCH_REWARD_SUCC);
}

void MazeManager::SetBox(int x, int y)
{
// 	if (this->IsValidPos(x, y))
// 	{
// 		m_maze_param.maze_grid[x][y] |= MazeParam::GF_BOX;
// 	}
}

void MazeManager::SetBomb(int x, int y)
{
// 	if (this->IsValidPos(x, y))
// 	{
// 		m_maze_param.maze_grid[x][y] |= MazeParam::GF_BOMB;
// 	}
}

void MazeManager::SetOpen(int x, int y)
{
// 	if (this->IsValidPos(x, y))
// 	{
// 		m_maze_param.maze_grid[x][y] |= MazeParam::GF_OPEN;
// 	}
}

bool MazeManager::IsBox(int x, int y)
{
// 	if (this->IsValidPos(x, y))
// 	{
// 		return 0 != (m_maze_param.maze_grid[x][y] & MazeParam::GF_BOX);
// 	}

	return false;
}

bool MazeManager::IsBomb(int x, int y)
{
// 	if (this->IsValidPos(x, y))
// 	{
// 		return 0 != (m_maze_param.maze_grid[x][y] & MazeParam::GF_BOMB);
// 	}

	return false;
}

bool MazeManager::IsOpen(int x, int y)
{
// 	if (this->IsValidPos(x, y))
// 	{
// 		return 0 != (m_maze_param.maze_grid[x][y] & MazeParam::GF_OPEN);
// 	}

	return false;
}

bool MazeManager::IsValidPos(int x, int y)
{
// 	if (x < 0 || x >= MazeParam::ONE_SIDE_COUNT || y < 0 || y >= MazeParam::ONE_SIDE_COUNT)
// 	{
// 		return false;
// 	}

	return true;
}

void MazeManager::RandShowBox()
{
// 	std::vector<Posi> tmp_vec;
// 	
// 	for (int i = 0; i < MazeParam::ONE_SIDE_COUNT; ++i)
// 	{
// 		for (int j = 0; j < MazeParam::ONE_SIDE_COUNT; ++j)
// 		{
// 			if (this->IsBox(i, j) && !this->IsOpen(i, j))
// 			{
// 				tmp_vec.push_back(Posi(i, j));
// 			}
// 		}
// 	}
// 
// 	if (tmp_vec.empty())
// 	{
// 		return;
// 	}
// 
// 	int rand_val = RandomNum((int)tmp_vec.size());
// 	if (rand_val >= 0 && rand_val < (int)tmp_vec.size())
// 	{
// 		Posi pos = tmp_vec[rand_val];
// 
// 		m_maze_param.shown_box_x = pos.x;
// 		m_maze_param.shown_box_y = pos.y;
// 	}
}

void MazeManager::PrintMaze()
{
	
}

bool MazeManager::Connectable(int src_x, int src_y, int next_x, int next_y)
{
// 	if (src_x - 1 == next_x && src_y == next_y)
// 	{
// 		return true;
// 	}
// 
// 	if (src_x + 1 == next_x && src_y == next_y)
// 	{
// 		return true;
// 	}
// 
// 	if (src_x == next_x && src_y + 1 == next_y)
// 	{
// 		return true;
// 	}
// 
// 	if (src_x == next_x && src_y - 1 == next_y)
// 	{
// 		return true;
// 	}

	return false;
}

void MazeManager::SendMoveResult(int result, int param)
{
// 	Protocol::SCMazeMoveResult scret;
// 	scret.result = result;
// 	scret.reserve_sh = 0;
// 	scret.param = param;
// 
// 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scret, sizeof(scret));
}
