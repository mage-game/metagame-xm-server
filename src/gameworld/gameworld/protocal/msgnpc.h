#ifndef MSGNPC_H
#define MSGNPC_H

#include "servercommon/userprotocal/msgheader.h"
#include "gamedef.h"
#include "servercommon/taskdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	class CSTaskListReq	 // 请求已接任务列表
	{
	public:
		CSTaskListReq();
		MessageHeader	header;
	};

	class CSTaskGiveup
	{
	public:
		CSTaskGiveup();
		MessageHeader	header;
		int				taskid;
	};

	class CSTaskAccept	// 1853
	{
	public:
		CSTaskAccept();
		MessageHeader	header;

		int				taskid;
	};

	class CSTaskCommit
	{
	public:
		CSTaskCommit();
		MessageHeader	header;

		int				taskid;
	};

	class CSFlyByShoe	 // 使用小飞鞋
	{
	public:
		CSFlyByShoe();
		MessageHeader	header;

		int				scene_id;
		int				scene_key;
		short			pos_x;
		short			pos_y;
		short			is_auto_buy;
		short			is_force;
	};

	class CSSyncJump	 
	{
	public:
		CSSyncJump();
		MessageHeader	header;

		int				scene_id;
		int				scene_key;
		short			pos_x;
		short			pos_y;
	};

	class CSTaskTalkToNpc	// 1855 和npc对话
	{
	public:
		CSTaskTalkToNpc();
		MessageHeader	header;

		UInt16 npc_id;
		short reserve_sh;
	};

	class SCTaskListAck				// 请求已接任务列表放回
	{
	public:
		SCTaskListAck();
		MessageHeader	header;

		struct TaskItem
		{
			unsigned short task_id;
			char task_ver;
			char task_condition;
			short progress_num;
			char is_complete;
			char reserve_ch;
			unsigned int accept_time;
		};

		int				count;
		TaskItem		taskinfo_list[TaskParam::MAX_ACCEPTED_TASK_NUM];
	};

	class SCTaskInfo				// 任务列表改变
	{
	public:
		SCTaskInfo();
		MessageHeader	header;

		enum REASON
		{
			REASON_INFO_CHG = 0,	// 信息改变
			REASON_ACCEPT,			// 接取
			REASON_REMOVE,			// 移除 
		};

		short			reason;
		TaskID			task_id;

		char			is_complete;
		char			is_silent;
		short			param;

		unsigned int	accept_time;
	};

	class SCTaskAccpetableTaskList				// 1808 可接任务列表
	{
	public:
		SCTaskAccpetableTaskList();
		MessageHeader	header;

		const static int ACCEPTABLE_TASK_MAX_COUNT = 2048;

		int				count;
		TaskID			accpetable_task_list[ACCEPTABLE_TASK_MAX_COUNT];
	};

	#define MAX_TASK_RECORDER_INFO_NUM 2048
	class SCTaskRecorderList		//发送已完成任务列表
	{
	public:
		SCTaskRecorderList();
		MessageHeader	header;
		unsigned int	count;
		TaskID			task_recorder_info_item[MAX_TASK_RECORDER_INFO_NUM];
	};

	class SCTaskRecorderInfo		// 任务记录列表数据改变
	{
	public:
		SCTaskRecorderInfo();
		MessageHeader	header;
		TaskID			completed_taskid;
	};

	/////////////////////////////////////////////////////    任务通用协议   /////////////////////////////////////////////////////

	/////////////////////////////////////////////////////   刺杀任务   /////////////////////////////////////////////////////
	class CSCishaTaskOpera
	{
	public:
		enum OPERA_TYPE
		{
			OPERA_TYPE_GET_INFO = 0,
			OPERA_TYPE_ACCEPT_TASK = 1,

			OPERA_TYPE_MAX,
		};

		CSCishaTaskOpera();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
	};

	class SCCishaTaskInfo
	{
	public:
		SCCishaTaskInfo();
		MessageHeader header;

		TaskID task_id;
		short commit_times;
	};

	/////////////////////////////////////////////////////  (日常)屠魔任务  /////////////////////////////////////////////////////
	enum TUMO_OPERA_TYPE
	{
		TUMO_OPERA_TYPE_GET_INFO = 0,						// 获取任务信息
		TUMO_OPERA_TYPE_FETCH_COMPLETE_ALL_REWARD = 1,		// 领取全部完成奖励
		TUMO_OPERA_TYPE_COMMIT_TASK = 2,					// 提交任务 p1:任务id p2: 1 提交全部 0 单个
		TUMO_OPERA_TYPE_FETCH_REWARD = 3,					// 领取奖励 p1:任务索引 p2: 1 普通 2 双倍 3 三倍
		TUMO_OPERA_TYPE_FETCH_EXP_BALL_REWARD = 4,			// 领取经验球奖励 

		TUMO_OPERA_TYPE_MAX,
	};

	class CSTuMoTaskOpera		// 1856 日常任务通用请求
	{
	public:
		CSTuMoTaskOpera();
		MessageHeader header;

		short opera_type;
		unsigned short param_1;
		short param_2;
		short param_3;
	};

	enum TUMO_NOTIFY_REASON_TYPE
	{
		TUMO_NOTIFY_REASON_DEFALUT = 0,										// 屠魔默认通知类型

		TUMO_NOTIFY_REASON_ADD_TASK = 1,									// 增加任务
		TUMO_NOTIFY_REASON_REMOVE_TASK = 2,									// 移除任务
		TUMO_NOTIFY_REASON_EXP_BALL_CHANGE = 3,								// 经验球数据改变
		TUMO_NOTIFY_REASON_ONE_KEY_FINISH = 4,								// 一键完成

	};
	class SCTuMoTaskInfo	// 1805 日常任务信息
	{
	public:
		SCTuMoTaskInfo();
		MessageHeader header;

		short notify_reason;												// 通知原因
		short commit_times;													// 提交任务次数
		char is_accept;														// 任务是否已经被接受
		char has_fetch_complete_all_reward;									// 是否领取了完成所有任务的奖励
		TaskID task_id;														// 任务id
		short daily_task_can_fetch_flag;									// 日常任务可领取标记
		short daily_task_fetch_flag;										// 日常任务领取标记
		unsigned int daily_task_exp_ball_valid_time;						// 经验球失效时间
		long long daily_task_exp_ball_exp;									// 经验球当前经验
	};

	/////////////////////////////////////////////////////  跑环任务  /////////////////////////////////////////////////////

	class CSPaoHuanSkipTask
	{
	public:
		CSPaoHuanSkipTask();
		MessageHeader header;

		short skip_all;
		TaskID task_id;
		int reserve;
	};

	class CSGetPaoHuanTaskInfo
	{
	public:
		CSGetPaoHuanTaskInfo();
		MessageHeader header;
	};

	enum PAOHUAN_NOTIFY_REASON_TYPE
	{
		PAOHUAN_NOTIFY_REASON_DEFALUT = 0,									// 跑环通知类型
		PAOHUAN_NOTIFY_REASON_ADD_TASK = 1,									// 增加任务
		PAOHUAN_NOTIFY_REASON_REMOVE_TASK = 2,								// 移除任务
		PAOHUAN_NOTIFY_REASON_FETCH_REWARD = 3,								// 奖励标志改变

	};
	class SCPaohuanTaskInfo
	{
	public:
		SCPaohuanTaskInfo();
		MessageHeader header;

		short notify_reason;												// 通知原因
		short complete_times;												// 提交任务次数
		short fetch_phase_reward_count;										// 领取阶段奖励
		short is_accept;													// 任务是否已经被接受
		TaskID task_id;														// 任务id
	};

	/////////////////////////////////////////////////////  护送任务  /////////////////////////////////////////////////////

	class CSRefreshHusongTask												// 请求护送信息
	{
	public:
		CSRefreshHusongTask();
		MessageHeader header;

		char is_autoflush;													// 是否自动刷新
		char is_auto_buy;													// 是否自动购买
		short reserve_sh;													// 保留
		int to_color;														// 刷新到的颜色
	};

	class CSHusongBuyTimes
	{
	public:
		CSHusongBuyTimes();
		MessageHeader header;
	};

	enum HUSONG_INFO_NOTFIY_REASON
	{
		HUSONG_INFO_NOTFIY_REASON_DEFAULT = 0,
		HUSONG_INFO_NOTFIY_REASON_ADD_TASK,									// 增加任务
		HUSONG_INFO_NOTFIY_REASON_GIVEUP_TASK,								// 任务失败
		HUSONG_INFO_NOTIFY_REASON_COMMIT_TASK,								// 任务完成
		HUSONG_INFO_NOTFIY_REASON_FLUSHCOLOR,								// 刷新颜色
		HUSONG_INFO_NOTFIY_REASON_LITTLE_HELPER_ADD_TASK,					// 小助手接受任务
		HUSONG_INFO_NOTFIY_REASON_LITTLE_HELPER_COMMIT_TASK,				// 小助手完成任务

		HUSONG_INFO_NOTFIY_REASON_MAX,
	};

	class SCHusongInfo
	{
	public:
		SCHusongInfo();
		MessageHeader header;
		
		short notfiy_reason;												// 通知原因
		ObjID obj_id;														// 对象ID

		TaskID task_id;														// 任务id
		char task_color;													// 任务颜色
		char accept_in_activityTime;										// 在活动时间内接
		char is_use_hudun;													// 是否使用过护盾
		char reserve_1;
		short reserve_2;
	};

	class CSHusongAddShield													// 1874释放护盾
	{
	public:
		CSHusongAddShield();
		MessageHeader		header;
	};

	class SCHusongConsumeInfo
	{
	public:
		SCHusongConsumeInfo();
		MessageHeader header;

		int token_num;
		int gold_num;
		int bind_gold_num;
	};

	/////////////////////////////////////////////////////    仙盟任务   /////////////////////////////////////////////////////
	class SCGuildTaskInfo
	{
	public:
		SCGuildTaskInfo();
		MessageHeader header;

		char double_reward_flag;
		char is_finish;
		short complete_task_count;
		int guild_task_max_count;
	};

	class CSGetGuildTaskDoubleReward
	{
	public:
		CSGetGuildTaskDoubleReward();
		MessageHeader header;
	};

	class CSFinishAllGuildTask
	{
	public:
		CSFinishAllGuildTask();
		MessageHeader header;
	};

	/////////////////////////////////////////////////////    特殊任务   /////////////////////////////////////////////////////
	class CSForceFinishSpecialTask		// 指定完成某个特殊任务  1878
	{
	public:
		CSForceFinishSpecialTask();
		MessageHeader header;
		int special_task_type;
	};
	//////////////////////////////////////////////////// /////////////////////////////////////////////////////
	static const int MAX_TASK_RANDOM_NUM = 20;

	class SCTaskRollReward
	{
	public:
		SCTaskRollReward();
		MessageHeader header;
		
		struct RewardItem
		{
			int task_type;
			int task_index;
		};

		short task_type;
		short is_one_key_finish;
		int param_1;
		long long param_2;

		int count;
		RewardItem reward_item_list[MAX_TASK_RANDOM_NUM];
	};
}

#pragma pack(pop)

#endif

