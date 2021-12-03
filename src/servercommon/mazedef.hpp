#ifndef __MASE_DEF_HPP__
#define __MASE_DEF_HPP__

#include <memory>
#include "servercommon/basedef.h"
#include "servercommon/servercommon.h"

#pragma pack(push, 4)

struct MazeParam
{
	static const int ONE_SIDE_COUNT = 5;
	static const int MAX_BOX_COUNT = 8;
	static const int MAX_BOMB_COUNT = 5;
	static const int MAX_FREEMOVE_TIMES = 10;

	enum GRID_FLAG
	{
		GF_BOX = (0x1 << 0),					// ±¦Ïä
		GF_BOMB = (0x1 << 1),					// Õ¨µ¯
		GF_OPEN = (0x1 << 2),					// ²È¹ý
	};

	enum MZ_FLAG
	{
		MZ_INIT = (0x1 << 0),
	};

	MazeParam() { this->Reset(); }

	void Reset()
	{
		maze_flag = 0;
		jifen = 0;
		shown_box_x = shown_box_y = 0;
		today_item_count = 0;
		jifen_reward_flag = 0;
		today_coin = 0;
		buy_times = 0;
		curr_x = 0; curr_y = 0;
		move_times = 0;
		reserve_ch = 0;
		reserve_sh = 0;
		memset(maze_grid, 0, sizeof(maze_grid));
	}

	char maze_flag;
	char jifen;
	char shown_box_x;
	char shown_box_y;

	short today_item_count;
	short jifen_reward_flag;
	int today_coin;

	char buy_times;
	char curr_x;
	char curr_y;

	char move_times;
	char reserve_ch;
	short reserve_sh;

	char maze_grid[ONE_SIDE_COUNT][ONE_SIDE_COUNT];
};

typedef char MazeParamHex[sizeof(MazeParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MazeParamHex) < 128);

#pragma pack(pop)

#endif // __TASK_DEF_HPP__



