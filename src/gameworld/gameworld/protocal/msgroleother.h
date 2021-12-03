#ifndef MSGROLEOTHER_H
#define MSGROLEOTHER_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/mazedef.hpp"

#pragma pack(push) 
#pragma pack(4)


namespace Protocol
{

	/*
		角色其他功能协议
	*/

	class CSRoleChooseCamp
	{
	public:
		CSRoleChooseCamp();
		MessageHeader	header;

		char camp_type;
		char is_random;
		short reserved_other;
	};

	class CSRoleChooseProf
	{
	public:
		CSRoleChooseProf();
		MessageHeader	header;

		char prof_type;
		char reserved;
		short reserved_other;
	};

	class CSSaveRoleHangupSet
	{
	public:
		CSSaveRoleHangupSet();
		MessageHeader	header;

		HangupSetData	hangup_set;
	};

	class CSSaveRoleCustomSet
	{
	public:
		CSSaveRoleCustomSet();
		MessageHeader	header;
		
		CustomSetData	custom_set;
	};

	class SCHangupSetInfo
	{
	public:
		SCHangupSetInfo();
		MessageHeader	header;
		
		HangupSetData	hangup_set;
		CustomSetData	custom_set;
	};

	class CSDailyWelfareTipsReq
	{
	public:
		CSDailyWelfareTipsReq();
		MessageHeader	header;
	};

	class SCDailyWelfareTips
	{
	public:
		SCDailyWelfareTips();
		MessageHeader	header;

		static const int DAILY_WELFARE_TIPS_MAX = 32;
		char welfare_tips_curr[DAILY_WELFARE_TIPS_MAX];
		char welfare_tips_max[DAILY_WELFARE_TIPS_MAX];
	};

	class CSLightenWeddingRing			// 2827 点亮婚戒
	{
	public:
		CSLightenWeddingRing();
		MessageHeader header;
		short use_gold;
		short reserve;
	};

	class CSUplevelWeddingRing			// 2828 升级婚戒
	{
	public:
		CSUplevelWeddingRing();
		MessageHeader header;
	};

	class CSPerformWeddingRingSkill		// 2829 释放婚戒技能
	{
	public:
		CSPerformWeddingRingSkill();
		MessageHeader header;

		short ring_level;				// 婚戒等级
		short target_obj_id;			// 目标即伴侣的对象ID
	};

	class SCWeddingRingSkillInfo		// 6007 婚戒技能信息
	{
	public:
		SCWeddingRingSkillInfo();
		MessageHeader header;

		unsigned int	last_wr_skill_4_perform_time;			// 上次婚戒4级技能使用时间
		unsigned int	last_wr_skill_5_perform_time;			// 上次婚戒5级技能使用时间
		unsigned int	last_wr_skill_6_perform_time;			// 上次婚戒6级技能使用时间
	};

	////////////////////////////////////// 迷宫 /////////////////////////////////
	class CSQueryMaze
	{
	public:
		CSQueryMaze();
		MessageHeader header;
	};

	class CSMoveMaze
	{
	public:
		CSMoveMaze();
		MessageHeader header;

		short target_x;
		short target_y;
	};

	class CSBuyMaze
	{
	public:
		CSBuyMaze();
		MessageHeader header;
	};

	class CSFetchMazeJifenReward
	{
	public:
		CSFetchMazeJifenReward();
		MessageHeader header;

		int seq;
	};

	class SCMazeMoveResult
	{
	public:
		SCMazeMoveResult();
		MessageHeader header;

		enum RET
		{
			RET_DEFAULT = 0,		// 默认
			RET_BOMB = 1,			// 炸弹
			RET_ITEM = 2,			// 宝箱之道具
			RET_COIN = 3,			// 宝箱之铜钱
		};

		short result;
		short reserve_sh;
		int param;					// RET_BOMB：0， RET_ITEM：道具数量， RET_COIN：铜钱数量
	};

	class SCMazeInfo
	{
	public:
		SCMazeInfo();
		MessageHeader header;

		enum REASON
		{
			REASON_DEFAULT = 0,
			REASON_DAYCHANGE = 1,			// 跨天重置
			REASON_BUY = 2,					// 购买次数
			REASON_MOVE = 3,				// 移动
		};

		short reason;
		short jifen_reward_flag;								// 积分奖励标记

		char shown_box_x;										// 显示的宝箱位置
		char shown_box_y;
		short today_item_count;									// 今天获取道具数量

		int today_coin;											// 今天获取铜钱数量

		char buy_times;											// 已购买次数
		char curr_x;											// 角色当前坐标
		char curr_y;
		char jifen;												// 积分

		char move_times;										// 移动次数
		char reserve_ch;										// 保留1
		short reserve_sh;										// 保留2

		// 格子信息 5*5, maze_grid[i][j] 按位操作，1:宝箱，2:炸弹，4:踩过
		char maze_grid[MazeParam::ONE_SIDE_COUNT][MazeParam::ONE_SIDE_COUNT];	
		char reserve_list[3];
	};
}

#pragma pack(pop)

#endif

