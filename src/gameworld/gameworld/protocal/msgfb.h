#ifndef MSG_FB_H
#define MSG_FB_H

#include "servercommon/fbdef.hpp"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/crossdef.hpp"
#include "other/fb/towerdefendconfig.hpp"
#include "other/fb/buildtowerfbconfig.hpp"
#include "other/runesystem/runetowerconfig.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCAutoFBRewardDetail
	{
	public:
		SCAutoFBRewardDetail();
		MessageHeader header;

		const static int REWARD_ITEM_MAX_COUNT = 32;

		struct RewardItem
		{
			ItemID item_id;
			short num;
			char is_bind;
			char reserve_ch;
			short reserve_sh;
		};

		short fb_type;
		short reserve_sh;

		int reward_coin;
		long long reward_exp;
		int reward_xianhun;
		int reward_yuanli;		
		int item_count;
		RewardItem item_list[REWARD_ITEM_MAX_COUNT];
	};

	///////////////////////////////////////////////// 装备副本 /////////////////////////////////////////////////
	class CSEquipFBGetInfo                // 5457 请求装备副本信息
	{
	public:
		CSEquipFBGetInfo();
		MessageHeader header;
	};

	class SCEquipFBInfo		              // 5414 副本信息
	{
	public:
		SCEquipFBInfo();
		MessageHeader header;

		short max_layer_today_entered;											// 今天进入的最大层数
		short flag;																// 按位，EFB_FLAG
		char mysterylayer_list[FB_EQUIP_MAX_GOODS_SEQ];							// 神秘层商品购买记录
	};

	class SCEquipFBResult                 // 5409 通关结果
	{
	public:
		SCEquipFBResult();
		MessageHeader header;

		struct FallItemInfo
		{
			ItemID item_id;
			short item_num;
			short item_color;
			short res_sh;
		};

		char is_finish;                  // 当前层结束
		char is_passed;					 // 1：通关，0：未通关 （当前层通关）
		char can_jump;					 // 1：能跳层，0：不能跳层
		char is_all_over;				 // 1：完结 0：未完结 (全部通关)
		int  use_time;					 //  副本开始到当前总用时
		short have_pass_reward;			 // 0 无，帮人打， 1 有，有门票
		char is_leave;                   // 是否离开场景
		char item_count;
		//FallItemInfo fall_item_list[NEQFB_PICK_FALL_ITEM_RECORD_COUNT];
		FallItemInfo fall_item_list[NEQFB_MAX_DROP_COUNT];
	};

	class SCEquipFBTotalPassExp           // 5412 经验
	{
	public:
		SCEquipFBTotalPassExp();
		MessageHeader header;

		long long total_pass_exp;				 // 累积通关经验
	};

	/////////////////////////////////////////////////// 塔防副本 /////////////////////////////////////////////////////
	class SCTowerDefendRoleInfo						// 5404
	{
	public:
		SCTowerDefendRoleInfo();
		MessageHeader header;

		char personal_join_times;
		char personal_buy_join_times;
		char personal_max_pass_level;
		char personal_auto_fb_free_times;
		short personal_item_buy_join_times;
		short personal_last_level_star;
	};

	class CSTowerDefendRoleInfo
	{
	public:
		CSTowerDefendRoleInfo();
		MessageHeader header;
	};

	class CSTowerDefendBuyJoinTimes			// 5454 购买次数
	{
	public:
		CSTowerDefendBuyJoinTimes();
		MessageHeader header;
	};

	class SCTowerDefendInfo					// 5413
	{
	public:
		SCTowerDefendInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_INIT = 1,
			NOTIFY_REASON_NEW_WAVE_START = 2,
			NOTIFY_REASON_FINISHED = 3,
		};

		short reason;
		short reserve;

		unsigned int time_out_stamp;
		short is_finish;
		short is_pass;
		int pass_time_s;

		long long life_tower_left_hp;					// 生命塔HP
		long long life_tower_left_maxhp;				// 生命塔最大HP
		short curr_wave;								// 当前波
		short curr_level;								// 当前是第几关
		int next_wave_refresh_time;						// 下一波到来时间
		short clear_wave_count;							// 清理怪物波数
		short death_count;								// 死亡次数

		struct DropItem
		{
			DropItem() : num(0), item_id(0) {}

			short num;
			ItemID item_id;
		};

		int get_coin;
		int get_item_count;
		SCTowerDefendInfo::DropItem drop_items[FB_RECORD_DROP_ITEM_MAX_NUM];
	};

	class SCFBDropInfo							// 5420副本怪物掉落统计
	{
	public:
		SCFBDropInfo();
		MessageHeader header;

		const static int FB_DROP_ITEM_MAX_NUM = 32;

		struct DropItem
		{
			DropItem() : num(0), item_id(0) {}

			short num;
			ItemID item_id;
		};

		int get_coin;
		int get_item_count;
		DropItem drop_items[FB_DROP_ITEM_MAX_NUM];
	};

	class SCTowerDefendResult	// 5433
	{
	public:
		SCTowerDefendResult();
		MessageHeader header;

		char is_passed;
		char clear_wave_count;
		short use_time;
		short have_pass_reward; // 0 无，帮人打，1 有，有门票
		short reserve_sh;
	};

	class SCTowerDefendWarning						// 5431
	{
	public:
		SCTowerDefendWarning();
		MessageHeader header;

		enum WARNING_TYPE
		{
			WARNING_TYPE_NORMAL = 1,
			WARNING_TYPE_LOW_PERCENT,
		};

		short warning_type;								// 1：普通挨打（忽略百分比），2：HP低于百分比
		short percent;
	};

	class CSTowerDefendNextWave
	{
	public:
		CSTowerDefendNextWave();
		MessageHeader header;
	};

	//////////////////////////////////////////////////// 通用副本通关协议 /////////////////////////////////////////////////////
	class SCFBFinish					// 5410副本通用通关通知
	{
	public:
		SCFBFinish();
		MessageHeader header;
	};

	class SCFBBossHPPercentInfo
	{
	public:
		SCFBBossHPPercentInfo();
		MessageHeader header;

		unsigned short monster_id;
		short hp_percent;
	};

	/////////////////////////////////////////////////// 新装备本 /////////////////////////////////////////////////////
	class SCNeqFBInfo									// 8501新装备本信息
	{
	public:
		SCNeqFBInfo();
		MessageHeader header;
		enum INFO_TYPE
		{
			NEQ_FB_INFO_DEFAULT= 0,  //全部刷新
			NEQ_FB_INFO_VIP_BUY_TIME = 1, //vip购买次数
			NEQ_FB_INFO_ITEM_BUY_TIME =2, //普通购买次数
			NEQ_FB_INFO_REWARD = 3,// 领取奖励

		};
		struct OneLevel
		{
			char join_times;
			char max_star;								// 本关卡最大星数
			short reserve_sh;
		};

		struct OneChapter
		{
			char reward_flag;							// 奖励标记  flag & reward_seq != 0 表示领过此奖励
			char reserve_ch;
			short reserve_sh;
			OneLevel level_list[NEQFB_MAX_LEVEL_PER_CHAPTER];
		};

		short info_type;
		short reverse_sh;
		int neqfb_score;								// 积分
		short toal_buy_times;							// 购买总次数
		short today_fight_all_times;					// 今天战斗次数
		short today_vip_buy_times;						// vip购买次数
		short today_item_buy_times;						// 物品购买次数
		OneChapter chapter_list[NEQFB_MAX_CHAPTER];		// 章列表
	};

	class SCNeqPass										// 8502新装备本通关信息
	{
	public:
		SCNeqPass();
		MessageHeader header;

		short pass_result;								// 1：通关，0：失败
		short reserve;
		int pass_sec;									// 通关时间秒
		short pass_star;								// 通关星星
		short reward_score;								// 奖励积分
	};

	class SCNeqRollPool		// 8503
	{
	public:
		SCNeqRollPool();
		MessageHeader header;

		struct RollItem
		{
			unsigned short item_id;
			char is_bind;
			char num;
		};
		RollItem roll_item_list[NEQFB_ROLLPOOL_TOTAL_COUNT];
	};

	class SCNeqRollInfo		//8504
	{
	public:
		SCNeqRollInfo();
		MessageHeader header;

		enum RIREASON
		{
			RIREASON_START = 0,							// 开始翻牌
			RIREASON_ROLL = 1,							// 翻牌结果
			RIREASON_END = 2,							// 翻牌结束
		};
		short reason;
		short reserve;

		char max_free_roll_times;
		char free_roll_times;
		char gold_roll_times;
		char hit_seq;									// 命中物品序号，即数组下标
	};

	class CSNeqFBStarRewardReq							// 8511新装备本请求领奖
	{
	public:
		CSNeqFBStarRewardReq();
		MessageHeader header;

		short chapter;									// 章
		short seq;										// 奖励序号
	};

	class CSNeqFBExchangeReq							// 8512 新装备本请求兑换
	{
	public:
		CSNeqFBExchangeReq();
		MessageHeader header;

		int item_id;
	};

	class CSNeqFBBuyTimesReq							// 8513 新装备本请求购买次数
	{
	public:
		CSNeqFBBuyTimesReq();
		MessageHeader header;
	};

	class CSNeqFBAutoReq								// 8514 新装备本请求扫荡
	{
	public:
		CSNeqFBAutoReq();
		MessageHeader header;

		short chapter;									// 章
		short level;									// 关卡
	};

	class CSNeqInfoReq									// 8515 查询新装备本关卡信息
	{
	public:
		CSNeqInfoReq();
		MessageHeader header;
	};

	class CSNeqRollReq		// 8516
	{
	public:
		CSNeqRollReq();
		MessageHeader header;

		short end_roll;
		short reserve_sh;
	};

	/////////////////////////////////////////////////// 防具材料副本 /////////////////////////////////////////////////////
	enum CS_ARMOR_DEFEND_REQTYPE
	{
		CS_ARMOR_DEFEND_ROLE_INFO_REQ = 0,
		CS_ARMOR_DEFEND_BUY_JOIN_TIMES = 1,
		CS_ARMOE_DEFEND_NEXT_WAVE_REQ = 2,
		CS_ARMOR_DEFEND_AUTO_REFRESH = 3,//0 不自动刷，1 清完自动刷新
	};

	class CSArmorDefendRoleReq     //8535
	{
	public:
		CSArmorDefendRoleReq();
		MessageHeader header;

		short req_type;
		short parm1;
	};

	class SCArmorDefendRoleInfo		// 8536
	{
	public:
		SCArmorDefendRoleInfo();
		MessageHeader header;

		char personal_join_times;
		char personal_buy_join_times;
		char personal_max_pass_level;
		char personal_auto_fb_free_times;
		short personal_item_buy_join_times;
		short reserve_sh;
	};

	class SCArmorDefendResult	// 8537
	{
	public:
		SCArmorDefendResult();
		MessageHeader header;

		const static int FB_DROP_ITEM_MAX_NUM = 32;
		struct DropItem
		{
			DropItem() : num(0), item_id(0) {}

			short num;
			ItemID item_id;
		};
		char is_passed;
		char clear_wave_count;
		short use_time;  //副本总用时

		int get_coin;
		int get_item_count;
		DropItem drop_items[FB_DROP_ITEM_MAX_NUM];
	};

	class SCArmorDefendInfo  // 8538
	{
	public:
		SCArmorDefendInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_INIT = 1,
			NOTIFY_REASON_NEW_WAVE_START = 2,
			NOTIFY_REASON_FINISHED,
		};

		short reason;
		short reserve_ch;
		int escape_monster_count;						// 逃跑数
		short curr_wave;								// 当前波
		short energy;									// 能量
		int next_wave_refresh_time;						// 下一波到来时间
		short clear_wave_count;							// 清理怪物波数
		short refresh_when_clear;						// 清完怪立即刷新
	};

	class SCArmorDefendWarning  //8539
	{
	public:
		SCArmorDefendWarning();
		MessageHeader header;
							
		short escape_num;
		short reserve_sh;
	};

	class SCArmorDefendPerformSkill//8540
	{
	public:
		SCArmorDefendPerformSkill();
		MessageHeader header;
		int skill_index;
		unsigned int next_perform_time[ARMORDEFEND_SKILL_MAX_COUNT];
	};

	//////////////////////////////////////////////////// 建塔本协议 /////////////////////////////////////////////////////

	enum BUILD_TOWER_OPERA_TYPE     
	{
		BUILD_TOWER_OPERA_TYPE_BUILD = 0,       // 建造塔
		BUILD_TOWER_OPERA_TYPE_UPGRADE = 1,		// 升级塔
		BUILD_TOWER_OPERA_TYPE_REMOVE = 2,		// 移除塔
		BUILD_TOWER_OPERA_TYPE_FLUSH = 3,		// 立即刷怪
		BUILD_TOWER_OPERA_TYPE_CALL = 4,		// 召唤鬼蟾王
	};

	class CSBuildTowerReq		 //8570
	{
	public:
		CSBuildTowerReq();
		MessageHeader header;

		int operate_type;
		short param1;
		short param2;
	};

	class SCBuildTowerFBSceneLogicInfo  //8571
	{
	public:
		SCBuildTowerFBSceneLogicInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_MONSTER_WAVE = 1,
			NOTIFY_BOSS_FLUSH = 2,
			NOTIFY_EXTRA_BOSS = 3,
			NOTIFY_FB_END = 4,
			NOTIFY_WAVE_FLUSH_END = 5, //怪刷完
			NOTIFY_PREPARE_TIME = 10,//准备提醒
		};

		void reset()
		{
			notify_reason = NOTIFY_REASON_DEFAULT;
			is_finish = 0;
			is_pass = 0;
			can_call_extra_monster = 0;

			time_out_timestamp = 0;
			exp = 0;
			douhun = 0;
			cur_wave = 0;
			escape_monster_count = 0;
			next_wave_timestamp = 0;
			item_count = 0;
			special_monster_num = 0;
			remain_buyable_monster_num = 0;
			memset(tower_info_list, 0, sizeof(tower_info_list));
			memset(item_list, 0, sizeof(item_list));
		}

		struct DropItem
		{
			ItemID item_id;
			short num;
		};

		struct TowerInfo
		{
			short tower_type;
			short tower_level;
		};

		char notify_reason;
		char is_finish;
		char is_pass;
		char can_call_extra_monster;

		unsigned int time_out_timestamp;
		long long exp;
		int douhun;
		short cur_wave;
		short escape_monster_count;
		unsigned int next_wave_timestamp;
		short notify_next_wave_timestamp;
		short item_count;

		int special_monster_num;				//额外怪物数量
		int remain_buyable_monster_num;

		TowerInfo tower_info_list[BUILD_TOWER_MAX_TOWER_POS_INDEX + 1];
		DropItem  item_list[BUILD_TOWER_MAX_DROP_ID_NUM];
	};

	class CSBuildTowerBuyTimes		 //8572
	{
	public:
		CSBuildTowerBuyTimes();
		MessageHeader header;
	};

	/////////////////////////////////////////////////// 组队守护副本 /////////////////////////////////////////////////////
	class SCTeamTowerDefendInfo		// 8580
	{
	public:
		SCTeamTowerDefendInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_SKILL = 1,
			NOTIFY_REASON_INIT = 2,
			NOTIFY_REASON_NEXT_WAVE = 3,
		};

		struct SkillInfo
		{
			SkillInfo() : skill_id(0), skill_level(0), next_can_perform_time(0) {}

			UInt16 skill_id;
			short skill_level;
			unsigned int next_can_perform_time;
		};

		static const int MAX_SKILL_COUNT = 4;

		int reason;
		long long life_tower_left_hp;					// 生命塔HP
		long long life_tower_left_maxhp;				// 生命塔最大HP
		int gongji_uid;									// 攻击加成者
		int fangyu_uid;									// 防御加成者
		int assist_uid;									// 辅助加成者
		int curr_wave;									// 当前波
		int next_wave_refresh_time;						// 下一波到来时间
		int score;										// 获得积分
		int exp;										// 获得经验
		int clear_wave;									// 消灭波数
		SkillInfo skill_list[MAX_SKILL_COUNT];			// 技能列表
	};

	class SCTeamTowerDefendAttrType		//8581
	{
	public:
		SCTeamTowerDefendAttrType();
		MessageHeader header;

		struct AttrType
		{
			AttrType():uid(0),attr_type(0){}
			int uid;					// 玩家id
			int attr_type;				// 组队塔防加成属性类型
		};

		AttrType attr_list[MAX_TEAM_MEMBER_NUM];
	};

	class SCTeamTowerDefendSkill		// 8582
	{
	public:
		SCTeamTowerDefendSkill();
		MessageHeader header;

		UInt16 skill_index;				// 技能下标
		short skill_level;				// 技能等级
		unsigned int next_can_perform_time;		// 下次可释放技能时间戳
	};

	enum TEAM_TOWER_DEFEND_OPREAT_REQ_TYPE
	{
		TEAM_TOWER_DEFEND_SET_ATTR_TYPE = 0,
		TEAM_TOWER_DEFEND_NEXT_WAVE_REQ = 1,
	};

	class CSTeamTowerDefendOpreatReq		//8583 请求设置组队塔防加成属性类型
	{
	public:
		CSTeamTowerDefendOpreatReq();
		MessageHeader header;

		int req_type;
		int param1;						// 玩家id
		int param2;						// 属性加成类型
	};

	class SCTeamTowerDefendAllRole  // 8584 所有玩家信息下发
	{
	public:
		SCTeamTowerDefendAllRole();
		MessageHeader header;
		static const int BUFF_COUNT_MAX = 6;
		struct roleInfo
		{
			int uid;
			GameName user_name;
			long long hp;
			long long max_hp;
			char attr_type;
			char buff_count;
			char buff[BUFF_COUNT_MAX];
		};
		int role_count;
		roleInfo role_list[MAX_TEAM_MEMBER_NUM];
	};

	class SCTeamTowerDefendResult	//8585 组队守护 结束信息
	{
	public:
		SCTeamTowerDefendResult();
		MessageHeader header;

		const static int FB_DROP_ITEM_MAX_NUM = 32;
		struct DropItem
		{
			DropItem() : item_id(0), num(0) {}

			ItemID item_id;
			short num;
		};

		char is_passed;
		char clear_wave_count;
		short use_time;  //副本总用时

		int xiannv_shengwu;//仙气值
		int item_count;
		DropItem drop_items[FB_DROP_ITEM_MAX_NUM];
	};


	///////////////////////////////////////////////////剧情副本 /////////////////////////////////////////////////////
	class CSStoryFBGetInfo
	{
	public:
		CSStoryFBGetInfo();
		MessageHeader header;
	};

	class SCStoryFBInfo
	{
	public:
		SCStoryFBInfo();
		MessageHeader header;

		struct InfoItem
		{
			short is_pass;
			short today_times;
		};
		
		InfoItem info_list[FB_STORY_MAX_COUNT];
	};

	class SCFbPickItemInfo
	{
	public:
		SCFbPickItemInfo();
		MessageHeader header;

		static const int PICK_ITEM_COUNT_MAX = 20;

		int item_count;
		ItemDataWrapper item_list[PICK_ITEM_COUNT_MAX];
	};

	class CSFbGuwu
	{
	public:
		CSFbGuwu();
		MessageHeader header;

		int is_gold;
	};

	class SCFBSceneLogicInfo				// 5499
	{
	public:
		SCFBSceneLogicInfo();
		MessageHeader header;

		void reset()
		{
			time_out_stamp = 0;
			flush_timestamp = 0;
			kick_timestamp = 0;
			scene_type = 0;
			is_finish = 0;
			is_pass = 0;
			is_active_leave_fb = 0;

			total_boss_num = 0;
			total_allmonster_num = 0;
			kill_boss_num = 0;
			kill_allmonster_num = 0;

			pass_time_s = 0;
			coin = 0;
			exp = 0;

			param1 = 0;
			param2 = 0;
			param3 = 0;
		}
		unsigned int time_out_stamp;									// 副本超时结束时间戳（可用于倒计时）
		unsigned int flush_timestamp;                                   // 怪物刷新世界戳（倒计时怪物的刷新）
		unsigned int kick_timestamp;                                    // 角色踢出副本时间戳（可用于倒计时）

		char scene_type;												// 场景类型
		char is_finish;													// 是否结束
		char is_pass;													// 是否通关
		char is_active_leave_fb;										// 是否主动退出
		
		short total_boss_num;											// boss总数量
		short total_allmonster_num;										// 怪物总数量（包括普通怪和boss)
		short kill_boss_num;											// 已击杀boss数量
		short kill_allmonster_num;										// 已击杀怪物总数量（包括普通怪和boss)

		int pass_time_s;												// 进入副本到目前经过的时间（少）
		int coin;														// 铜币
		long long exp;														// 经验

		long long param1;
		int param2;
		int param3;
	};
	
	// 经验副本信息 5555
	class SCExpFbInfo
	{
	public:
		SCExpFbInfo();
		MessageHeader header;

		void reset()
		{
			time_out_stamp = 0;
			scene_type = 0;
			is_finish = 0;
			guwu_times = 0;
			team_member_num = 0;
			exp = 0;
			record_max_exp = 0;
			exp_percent = 0;
			wave = 0;
			kill_allmonster_num = 0;
			start_time = 0;
		}
		time_t time_out_stamp;											// 副本超时结束时间戳
		char scene_type;												// 场景类型
		char is_finish;													// 是否结束
		char guwu_times;												// 鼓舞次数
		char team_member_num;											// 组队人数
		long long exp;													// 经验
		long long record_max_exp;									    // 历史最高经验
		int exp_percent;
		short wave;														// 波数
		unsigned short kill_allmonster_num;								// 杀怪数
		time_t start_time;												// 开始时间
	};

	// 经验副本购买鼓舞  5556
	class CSExpFbPayBuff
	{
	public:
		CSExpFbPayBuff();
		MessageHeader header;

		short is_auto;													// 是否自动鼓舞
		short reverse_sh;
	};

	/////////////////////////////////////////////////// 副本引导 ////////////////////////////////////////////////////////
	class CSFbGuideFinish												// 8400 完成
	{
	public:
		CSFbGuideFinish();
		MessageHeader header;

		int monster_x;
		int monster_y;								
		int monster_id;	
	};

	class CSFbGuideCreateGather											// 8401
	{
	public:
		CSFbGuideCreateGather();
		MessageHeader header;

		int gather_x;
		int gather_y;
		int gather_id;
		int gather_time;	// 单位秒
	};

	///////////////////////////////////////////////////功能开启副本 /////////////////////////////////////////////////////
	class SCFunOpenMountInfo									// 8300 坐骑副本信息
	{
	public:
		SCFunOpenMountInfo();
		MessageHeader header;

		char is_finish;											// 是否结束
		char reserve_1;								
		short reserve_2;									

		int cur_step_monster_total_num;							// 当前步骤怪物刷新总数量
		int cur_step_monster_kill_num;							// 当前步骤怪物击杀数量

		int cur_step_gather_total_num;							// 当前步采集物刷新总数量
		int cur_step_gather_num;								// 当前步骤采集物采集数量
	};

	class SCFunOpenWingInfo							// 8301 羽翼副本信息
	{
	public:
		SCFunOpenWingInfo();
		MessageHeader header;

		char is_finish;											// 是否结束
		char reserve_1;								
		short reserve_2;											
		
		int cur_step_monster_total_num;							// 当前步骤怪物刷新总数量
		int cur_step_monster_kill_num;;							// 当前步骤怪物击杀数量
	};

	class SCFunOpenXiannvInfo						// 8302女神副本信息
	{
	public:
		SCFunOpenXiannvInfo();
		MessageHeader header;
		
		char is_finish;											// 是否结束
		char reserve_1;								
		short reserve_2;											

		int cur_step_monster_total_num;							// 当前步骤怪物刷新总数量
		int cur_step_monster_kill_num;;							// 当前步骤怪物击杀数量
	};

	class CSFunOpenStoryStep						// 8325
	{
	public:
		CSFunOpenStoryStep();
		MessageHeader header;

		int create_monster_step;
	};

	class CSFunOpenSetObjToPos						// 8326
	{
	public:
		CSFunOpenSetObjToPos();
		MessageHeader header;
		
		ObjID obj_id;
		short reserve;
		
		int pos_x;
		int pos_y;
	};

	class SCFunOpenStoryStepEnd						// 8327
	{
	public:
		SCFunOpenStoryStepEnd();
		MessageHeader header;

		int cur_step;								// 当前步骤
	};

	///////////////////////////////////////////////////魔戒副本 /////////////////////////////////////////////////////
	class SCMoJieFBAllInfo							// 所有信息 
	{
	public:
		SCMoJieFBAllInfo();
		MessageHeader header;

		int mojie_human_pian_task_mark;						// 魔戒人类篇完成标记
		int mojie_airen_pian_task_mark;						// 魔戒矮人篇完成标记
		int mojie_jingling_pian_task_mark;					// 魔戒精灵篇完成标记
	};

	///////////////////////////////////////////////////日常副本 /////////////////////////////////////////////////////
	class CSDailyFBGetRoleInfo						// 请求日常副本所有信息 5410
	{
	public:
		CSDailyFBGetRoleInfo();
		MessageHeader header;
	};

	class CSExpFBRetchFirstRewardReq				// 经验本领取首通奖励 5554
	{
	public:
		CSExpFBRetchFirstRewardReq();
		MessageHeader header;

		int fetch_reward_wave;
	};

	class SCDailyFBRoleInfo							// 所有信息 5411
	{
	public:
		SCDailyFBRoleInfo();
		MessageHeader header;

		short expfb_today_pay_times;	// 今天购买次数
		short expfb_today_enter_times;	// 今天进入次数
		unsigned int last_quit_fb_time; // 最后一次退出时间
		long long max_exp;				// 最大经验
		short max_wave;					// 最大波数
		unsigned short expfb_history_enter_times; // 历史进入次数
	};

	///////////////////////////////////////////////////爬塔副本 /////////////////////////////////////////////////////
	class CSPataFbAllInfo
	{
	public:
		CSPataFbAllInfo();
		MessageHeader header;
	};

	class SCPataFbAllInfo
	{
	public:
		SCPataFbAllInfo();
		MessageHeader header;

		short pass_level;
		short today_level;
	};

	///////////////////////////////////////////////////VIP副本////////////////////////////////////////////////
	class CSVipFbAllInfoReq
	{
	public:
		CSVipFbAllInfoReq();
		MessageHeader header;
	};

	class SCVipFbAllInfo
	{
	public:
		SCVipFbAllInfo();
		MessageHeader header;
		
		int is_pass_flag;
		char today_times_list[FB_VIP_NUM_MAX];
	};

	///////////////////////////////////////////////////组队副本房间 ////////////////////////////////////////////////
	class CSTeamFbRoomOperate
	{
	public:
		CSTeamFbRoomOperate();
		MessageHeader header;

		int operate_type;
		int param1;
		int param2;
		int param3;
		int param4;
		int param5;
	};

	class SCTeamFbRoomList
	{
	public:
		SCTeamFbRoomList();
		MessageHeader header;

		int team_type;
		int count;

		struct TeamFbRoomItem
		{
			TeamFbRoomItem() { this->Reset(); }

			void Reset()
			{
				team_index = 0;
				memset(leader_name, 0, sizeof(GameName));
				leader_capability = 0;
				limit_capability = 0;
				avatar_key_big = 0;
				avatar_key_small = 0;
				menber_num = 0;
				mode = 0;
				leader_sex = 0;
				leader_prof = 0;
				layer = 0;
				assign_mode = 0;
				is_must_check = 0;
				reserve_2 = 0;
			}

			int team_index;
			GameName leader_name;
			int leader_capability;
			int limit_capability;
			unsigned int avatar_key_big;
			unsigned int avatar_key_small;
			char menber_num;
			char mode;
			char leader_sex;
			char leader_prof;
			int leader_uid;
			char layer;
			char assign_mode;
			char is_must_check;
			char reserve_2;
		};

		TeamFbRoomItem room_list[FB_TEAM_ROOM_MAX_NUM];
	};

	class SCTeamFbRoomEnterAffirm
	{
	public:
		SCTeamFbRoomEnterAffirm();
		MessageHeader header;

		int team_type;
		int mode;
		int layer;
	};

	//////////////////////////////////多人妖兽祭坛副本///////////////////////////////////////////////////////////////////
	static const int TEAMFB_MAX_ATTR_NUM = 4;
	class SCYsjtTeamFbSceneLogicInfo
	{
	public:
		SCYsjtTeamFbSceneLogicInfo();
		MessageHeader header;
		
		unsigned int time_out_stamp;									// 副本超时结束时间戳（可用于倒计时）
		char scene_type;												// 场景类型
		char is_finish;													// 是否结束
		char is_pass;													// 是否通关
		char is_active_leave_fb;										// 是否主动退出
		
		int kill_boss_num;
		int pass_time_s;
		int mode;
		int boss_attr_type;
		YaoShouJiTanTeamFbRoleAttrInfo role_attrs[TEAMFB_MAX_ATTR_NUM];
	};

	//////////////////////////////////迷宫仙府副本///////////////////////////////////////////////////////////////////
	class CSMgxfTeamFbTouchDoor
	{
	public:
		CSMgxfTeamFbTouchDoor();
		MessageHeader header;

		short layer;
		short door_index;
	};

	class SCMgxfTeamFbSceneLogicInfo
	{
	public:
		SCMgxfTeamFbSceneLogicInfo();
		MessageHeader header;

		unsigned int time_out_stamp;									// 副本超时结束时间戳（可用于倒计时）
		short is_finish;													// 是否结束
		short is_pass;													// 是否通关

		int pass_time_s;
		int mode;
		short layer;													// 玩家现在所在层数
		char kill_hide_boos_num;										// 杀死的隐藏boss数量
		char kill_end_boss_num;											// 杀死的最终boss数量
		
		struct LayerItem
		{
			int door_status_list[MIGONGXIANFU_DOOR_NUM];
		};
		
		LayerItem layer_list[MIGONGXIANFU_LAYER_NUM];
	};

	/////////////////////////////////////////////////// 挑战副本 /////////////////////////////////////////////////////
	enum CHALLENGE_FB_OPERATE_TYPE
	{
		CHALLENGE_FB_OPERATE_TYPE_AUTO_FB = 0,							//扫荡
		CHALLENGE_FB_OPERATE_TYPE_RESET_FB,								//重置
		CHALLENGE_FB_OPERATE_TYPE_SEND_INFO_REQ,						//请求发送协议
		CHALLENGE_FB_OPERATE_TYPE_BUY_TIMES,							//购买次数
	};

	class CSChallengeFBOP								//5467
	{
	public:
		CSChallengeFBOP();
		MessageHeader header;

		short type;										//0扫荡，1重置关卡，2请求发送协议
		short param;									//0,1关卡索引  2, 3发0
	};

	class SCChallengeFBInfo								//5407
	{
	public:
		SCChallengeFBInfo();
		MessageHeader header;

		struct Level
		{
			char is_pass;
			char fight_layer;
			short state;
			short history_max_reward;
			short is_continue;
			char use_count;
			char res_ch;
			short res_sh;
		};

		int enter_count;
		int buy_count;
		short is_auto;
		short reserve_sh;
		Level level_list[ChallengeFBParam::LEVEL_MAX_COUNT];
	};

	class SCChallengePassLevel							//5408
	{
	public:
		SCChallengePassLevel();
		MessageHeader header;

		short level;
		char is_pass;
		char fight_layer;
		char is_active_leave_fb;
		char reward_flag;
		short reserve_sh;
		unsigned int pass_time;
	};

	class SCChallengeLayerInfo						//5419
	{
	public:
		SCChallengeLayerInfo();
		MessageHeader header;

		char is_pass;
		char is_finish;
		short reserve_sh;
		unsigned int time_out_stamp;
	};

	/////////////////////////////////////////////////// 仙盟神兽 /////////////////////////////////////////////////////
	class CSQueryGuildMonster
	{
	public:
		CSQueryGuildMonster();
		MessageHeader header;
	};

	class SCGuildMonsterInfo
	{
	public:
		SCGuildMonsterInfo();
		MessageHeader header;

		short my_finish_seq;
		short guild_finish_seq;
		GameName guild_finish_username;
		char prof;
		char reserve_ch;
		short reserve_sh;
	};

	class SCGuildMonsterFBResult
	{
	public:
		SCGuildMonsterFBResult();
		MessageHeader header;

		short is_pass;
		short reserve;
	};

	//////////////////////////////////阶段副本//////////////////////////////
	class SCPhaseFBInfo			// 5418 阶段副本基本信息
	{
	public:
		SCPhaseFBInfo();
		MessageHeader header;

		struct InfoItem
		{
			char is_pass;			// 已经通过了第几层
			char today_buy_times;	// 今日购买的副本次数
			short today_times;		// 今日副本次数
		};

		InfoItem info_list[FB_PHASE_MAX_COUNT];
	};

	enum PHASE_FB_OPERATE_TYPE
	{
		PHASE_FB_OPERATE_TYPE_INFO = 0,			// 获取信息
		PHASE_FB_OPERATE_TYPE_BUY_TIMES = 1,	// 购买次数
	};

	class CSPhaseFBInfoReq		// 5465 阶段副本个人信息请求
	{
	public:
		CSPhaseFBInfoReq();
		MessageHeader header;

		short operate_type;		// 操作类型enum PHASE_FB_OPERATE_TYPE
		short fb_index;			// 副本索引
	};

	//////////////////////////////////仙盟副本//////////////////////////////
	class SCGuildFBInfo					// 5478
	{
	public:
		SCGuildFBInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_ENTER,				// 进入场景
			NOTIFY_REASON_WAIT,					// 等待
			NOTIFY_REASON_UPDATE,				// 更新数据
			NOTIFY_REASON_FINISH,				// 副本完成

			NOTIFY_REASON_MAX,
		};

		const static int RANK_ITEM_MAX_COUNT = 20;

		struct Rank
		{
			int user_id;
			GameName user_name;
			long long hurt_val;
		};

		short notify_reason;
		short curr_wave;
		unsigned int next_wave_time;
		short wave_enemy_count;
		short wave_enemy_max;
		short is_pass;
		short is_finish;
		long long hp;
		long long maxhp;
		unsigned int kick_role_time;

		int self_rank_pos;
		long long self_hurt_val;
		int rank_count;
		Rank rank_info_list[RANK_ITEM_MAX_COUNT]; // boss伤害排名信息
	};

	enum REWARD_RECORDE_INFO_TYPE
	{
		REWARD_RECORDE_INFO_TYPE_CUR_WAVE = 0,			// 当前波奖励记录
		REWARD_RECORDE_INFO_TYPE_HISTORY,				// 历史奖励记录
	};

	class SCGuildFBRewardRecordInfo				//	2701 仙盟副本奖励信息
	{
	public:
		SCGuildFBRewardRecordInfo();
		MessageHeader header;

		static const int MAX_ITEM_COUNT = 20;
		struct ItemInfo
		{
			ItemInfo() : reward_item_id(0), reward_item_num(0), item_color(0), reserve_sh(0){}
			ItemID reward_item_id;
			short reward_item_num;
			short item_color;
			short reserve_sh;
		};

		short info_type;
		short resserve_sh;

		int item_count;
		ItemInfo item_list[MAX_ITEM_COUNT];
	};


	//////////////////////////////////boss之家//////////////////////////////

	enum BOSS_ENTER_TYPE
	{
		BOSS_ENTER_TYPE_IVNALID = 0,

		BOSS_ENTER_TYPE_BOSS_FAMILY,	   // vip boss	
		BOSS_ENTER_TYPE_BOSS_MIKU,         // 精英boss
		BOSS_ENTER_TYPE_BOSS_DABAO,        // 套装boss
		BOSS_ENTER_LEAVE_BOSS_SCENE,       // 离开
		BOSS_ENTER_TYPE_BOSS_ACTIVE,       // 活跃boss
		BOSS_ENTER_TYPE_BOSS_PRECIOUS,
		BOSS_ENTER_TYPE_BOSS_SHANGGU,      // 上古boss
		BOSS_ENTER_TYPE_BOSS_BABY,         // 宝宝boss
		BOSS_ENTER_TYPE_MAX,               // 以上是原服的Boss，而且有其它操作用到 

		BOSS_TYPE_CROSS_BOSS = 10,         // 跨服远古boss
		BOSS_TYPE_CROSS_MIZANG_BOSS,       // 神域(密藏)BOSS
		BOSS_TYPE_CROSS_YOUMING_BOSS,
		
		BOSS_ENTER_TYPE_GODMAGIC_BOSS ,		  // 神魔boss

		BOSS_ENTER_TYPE_ALL_MAX,           // boss类型最大值(包括跨服和原服)
	};

	class CSBossFamilyOperate				//  5506 boss之家操作
	{
	public:
		CSBossFamilyOperate();
		MessageHeader header;

		enum BOSS_FAMILY_OPERATE_TYPE
		{
			BOSS_FAMILY_BUY_MIKU_WEARY = 0, // 购买秘窟BOSS疲劳值
			BOSS_FAMILY_BUY_ACTIVE_BOSS_WEARY = 1, // 购买活跃BOSS疲劳值
			BOSS_DABAO_BUY_ENTER_COUNT = 2,        // 购买打宝boss进入次数
		};

		int operate_type;
	};

	class CSEnterBossFamily			// 5477 进入boss之家请求
	{
	public:
		CSEnterBossFamily();
		MessageHeader header;

		int enter_type;
		int scene_id;

		char is_buy_dabao_times;
		char reserve1;
		unsigned short boss_id;
	};

	class CSGetBossInfoReq    // 5463 请求boss信息
	{
	public:
		CSGetBossInfoReq();
		MessageHeader header;

		int enter_type;
	};

	class CSBossKillerInfoReq		// 5466 Boss击杀列表信息请求
	{
	public:
		CSBossKillerInfoReq();
		MessageHeader header;

		int boss_type;
		int boss_id;
		int scene_id;
	};

	class SCDabaoBossInfo		// 6551 打宝boss信息
	{
	public:
		SCDabaoBossInfo();
		MessageHeader header;

		short dabao_angry_value;
		short enter_count;

		unsigned int kick_time;

	};

	class SCFamilyBossInfo		// 6552 boss之家boss信息
	{
	public:
		SCFamilyBossInfo();
		MessageHeader header;

		static const int MAX_BOSS_COUNT = 100;
		int scene_id;
		int count;

		struct FamilyBossInfo
		{
			int boss_id;
			int status;
			unsigned int next_refresh_time;
		};

		FamilyBossInfo boss_list[MAX_BOSS_COUNT];
	};

	class SCMikuBossInfo		// 6553 秘窟boss信息
	{
	public:
		SCMikuBossInfo();
		MessageHeader header;

		short miku_boss_weary;
		short reserve;

		static const int MAX_BOSS_COUNT = 100;
		int count;
		int scene_id;
		int miku_monster_num;

		struct MikuBossInfo
		{
			int boss_id;
			int status;
			unsigned int next_refresh_time;
		};

		MikuBossInfo boss_list[MAX_BOSS_COUNT];
	};

	class SCMikuMonsterInfo		//  6561秘窟精英怪物信息
	{
	public:
		SCMikuMonsterInfo();
		MessageHeader header;

		int miku_monster_num;
	};

	class SCBossRoleInfo		// 6554 boss相关个人信息
	{
	public:
		SCBossRoleInfo();
		MessageHeader header;

		short miku_boss_weary;
		short reserve;
	};

	class SCDabaoBossNextFlushInfo		// 6555 打宝boss刷新时间信息
	{
	public:
		SCDabaoBossNextFlushInfo();
		MessageHeader header;

		static const int MAX_BOSS_COUNT = 100;
		int scene_id;
		int count;

		struct DabaoBossInfo
		{
			int boss_id;
			unsigned int next_refresh_time;
		};

		DabaoBossInfo boss_list[MAX_BOSS_COUNT];
	};

	class SCActiveBossNextFlushInfo		// 6559 活跃boss刷新时间信息
	{
	public:
		SCActiveBossNextFlushInfo();
		MessageHeader header;

		static const int MAX_BOSS_COUNT = 100;
		int scene_id;
		int count;

		struct ActiveBossInfo
		{
			int boss_id;
			unsigned int next_refresh_time;
		};

		ActiveBossInfo boss_list[MAX_BOSS_COUNT];
	};

	class SCActiveBossInfo			// 6560 活跃boss信息
	{
	public:
		SCActiveBossInfo();
		MessageHeader header;

		short enter_count;
		short active_boss_weary;
	};

	class SCActiveBossHurtRankInfo			// 6566 活跃boss伤害排行信息
	{
	public:
		SCActiveBossHurtRankInfo();
		MessageHeader header;

		static const int MAX_RANK_COUNT = 5;

		struct RankInfo
		{
			GameName name;			// 玩家名字
			Attribute hurt;			// 伤害
		};

		int monster_id;
		long long my_hurt;
		short my_rank;
		short rank_count;			// 排行数量
		RankInfo rank_info_list[MAX_RANK_COUNT];
	};

	class SCActiveBossLeaveInfo			  // 6567 离开活跃boss伤害信息区域
	{
	public:
		SCActiveBossLeaveInfo();
		MessageHeader header;
	};

	class SCMikuBossHurtRankInfo			// 6568 精英(困难)boss伤害排行信息
	{
	public:
		SCMikuBossHurtRankInfo();
		MessageHeader header;

		static const int MAX_RANK_COUNT = 5;

		struct RankInfo
		{
			GameName name;			// 玩家名字
			Attribute hurt;			// 伤害
		};

		int monster_id;
		long long my_hurt;
		short my_rank;
		short rank_count;			// 排行数量
		RankInfo rank_info_list[MAX_RANK_COUNT];
	};
	class SCMikuBossLeaveInfo			  // 6569 离开活跃boss伤害信息区域
	{
	public:
		SCMikuBossLeaveInfo();
		MessageHeader header;
	};
	//////////////////////////////////////////////////////////////////////////
	enum NOTIFY_REASON_BOSS
	{
		NOTIFY_REASON_BOSS_ADVANCE = 0,					// 刷新前提醒（60s）
		NOTIFY_REASON_BOSS_REFRESH = 1,					// boss刷新
		NOTIFY_REASON_BOSS_DIE = 2,						// BOSS死亡
	};

	class SCBossInfoToAll			// 6556 boss广播信息
	{
	public:
		SCBossInfoToAll();
		MessageHeader header;

		int notify_reason;					// 通知类型  enum NOTIFY_REASON_BOSS
		int boss_type;						// BOSS枚举类型 enum BOSS_ENTER_TYPE
		int boss_id;
		int scene_id;
		int status;
		unsigned int next_refresh_time;
		int killer_uid;
	};

	class SCBossKillerList			// 6557 boss击杀列表信息
	{
	public:
		SCBossKillerList();
		MessageHeader header;

		struct KillerInfo
		{
			KillerInfo():killer_uid(0), killier_time(0)
			{
				memset(killer_name, 0, sizeof(killer_name));
			}

			int killer_uid;
			unsigned int killier_time;
			GameName killer_name;
		};
		static const int KILLER_LIST_MAX_COUNT = 5;

		KillerInfo killer_info_list[KILLER_LIST_MAX_COUNT];
	};

	class SCFollowBossInfo		// 6558 boss关注列表信息
	{
	public:
		SCFollowBossInfo();
		MessageHeader header;

		static const int MAX_BOSS_COUNT = 100;
		int count;

		struct BossBaseInfo
		{
			short boss_id;
			char boss_type;
			char reserve_ch;
			int scene_id;
		};

		BossBaseInfo follow_boss_list[MAX_BOSS_COUNT];
	};

	class CSFollowBossReq		//  6570 boss关注请求
	{
	public:
		CSFollowBossReq();
		MessageHeader header;

		enum FOLLOW_BOSS_OPE_TYPE
		{
			FOLLOW_BOSS = 0,		// 关注boss
			UNFOLLOW_BOSS,			// 取消关注
			GET_FOLLOW_LIST,		// 获取关注列表
		};

		int opera_type;
		int boss_type;
		int boss_id;
		int scene_id;
	};

	class SCPreciousBossTaskInfo					// 6562 秘藏boss任务信息
	{
	public:
		SCPreciousBossTaskInfo();
		MessageHeader header;

		static const int MAX_TASK_COUNT = 10;

		struct PreciousBossTaskInfo
		{
			PreciousBossTaskInfo() : task_condition(0), task_type(0), is_finish(0)
			{}

			short task_condition;					// 任务条件数量
			short task_type;						// 任务类型
			int is_finish;							// 是否完成
		};

		int count;									// 任务数量
		PreciousBossTaskInfo task_list[MAX_TASK_COUNT];
	};

	class SCPreciousBossInfo						// 6563 秘藏boss信息
	{
	public:
		SCPreciousBossInfo();
		MessageHeader header;

		static const int MAX_BOSS_COUNT = 10;

		struct PreciousBossInfo
		{
			PreciousBossInfo() : boss_id(0), next_refresh_time(0)
			{}

			int boss_id;							// boss的id
			unsigned int next_refresh_time;			// 下次刷新时间,为0表示boss存活，否则不存活
		};

		int count;									// boss数量
		PreciousBossInfo boss_list[MAX_BOSS_COUNT];
	};

	class CSPreciousOperaReq							// 6564 秘藏请求位置信息
	{
	public:
		CSPreciousOperaReq();
		MessageHeader header;

		enum PRECIOUS_OPERA_TYPE
		{
			PRECIOUS_OPERA_TYPE_POS = 0,
			PRECIOUS_OPERA_TYPE_TASK,
		};

		int type;									// 0请求位置，1请求任务协议
		int param;									// 位置类型
		int param_2;								// boss_id
	};

	class SCPreciousPosInfo							// 6565 秘藏位置信息
	{
	public:
		SCPreciousPosInfo();
		MessageHeader header;

		int pos_x;									// 横坐标
		int pos_y;									// 纵坐标
		int param;									// 目标id
	};

	//////////////////////////////////请求场景怪物刷新信息//////////////////////////////

	static const int SCENE_MONSTER_GENERATER_MAX_NUM = 64;
	static const int SCENE_GATHER_GENERATER_MAX_NUM = 64;

	class CSReqMonsterGeneraterList // 请求怪物生成点列表信息  5479
	{
	public:
		CSReqMonsterGeneraterList();
		MessageHeader		header;

		int get_scene_id;
	};

	struct SCMonsterGeneraterInfo
	{
		int boss_id;
		unsigned int next_refresh_time;
	};

	class SCMonsterGeneraterList			// 下发当前场景怪物生成点列表信息5480
	{
	public:
		SCMonsterGeneraterList();
		MessageHeader		header;

		int boss_count;
		SCMonsterGeneraterInfo boss_list[SCENE_MONSTER_GENERATER_MAX_NUM];
	};

	class CSReqGatherGeneraterList			// 请求采集物生成点列表信息 5525
	{
	public:
		CSReqGatherGeneraterList();
		MessageHeader		header;

		int get_scene_id;
		int scene_key;
	};

	struct SCGatherGeneraterInfo
	{
		int gather_id;
		int pos_x;
		int pos_y;
		unsigned int next_refresh_time;
	};

	class SCGatherGeneraterList				// 下发当前场景采集物生成点列表信息  5526
	{
	public:
		SCGatherGeneraterList();
		MessageHeader		header;

		int gather_count;
		SCGatherGeneraterInfo gather_list[SCENE_GATHER_GENERATER_MAX_NUM];
	};

	//////////////////////////////////妖兽广场//////////////////////////////
	
	class CSEnterYaoShouGuangChang
	{
	public:
		CSEnterYaoShouGuangChang();
		MessageHeader		header;

		int is_buy_times;
	};

	static const int YAOSHOUGUANGCHANG_ROOM_USER_MAX = 10;

	class SCYaoShouGuangChangFBInfo	
	{
	public:
		SCYaoShouGuangChangFBInfo();
		MessageHeader		header;

		enum  
		{
			YAOSHOUGUANGCHANG_NOTIFY_INVALID = 0,
			YAOSHOUGUANGCHANG_NOTIFY_SCORE_CHANGE,
			YAOSHOUGUANGCHANG_NOTIFY_ENTER_SCENE,
			YAOSHOUGUANGCHANG_NOTIFY_WAVE_FINISH,
			YAOSHOUGUANGCHANG_NOTIFY_MAX,
		};

		void Reset()
		{
			reason = 0;

			wave_index = 0;
			fb_lv = 0;
			role_num = 0;
			monster_num = 0;

			user_count = 0;
			memset(user_list, 0, sizeof(user_list));
		}

		int reason;

		int wave_index;
		int fb_lv;
		int role_num;
		int monster_num;

		struct YSGCUserInfo
		{
			int uid;
			int score;
		};
		
		int user_count;
		YSGCUserInfo user_list[YAOSHOUGUANGCHANG_ROOM_USER_MAX];
	};

	class SCYaoShouGuangChangState
	{
	public:
		SCYaoShouGuangChangState();
		MessageHeader		header;

		int status;
		unsigned int next_status_time;
		unsigned int next_standy_time;
		unsigned int next_stop_time;

		int datais_valid;
		int ysgc_max_score;
		int quanfu_topscore;
		int	quanfu_topscore_uid;
		GameName quanfu_topscore_name;
	};

	class CSGetYaoShouGuangChangState
	{
	public:
		CSGetYaoShouGuangChangState();
		MessageHeader		header;
	};

	class CSGetYaoShouGuangChangReward
	{
	public:
		CSGetYaoShouGuangChangReward();
		MessageHeader		header;

		int times;
	};

	class SCNotifyYaoShouGuangChangReward
	{
	public:
		SCNotifyYaoShouGuangChangReward();
		MessageHeader		header;

		int score;
		int exp;
		int bind_coin;
	};

	//////////////////////////////////锁妖塔//////////////////////////////

	class CSEnterSuoYaoTa
	{
	public:
		CSEnterSuoYaoTa();
		MessageHeader		header;

		int is_buy_times;
	};

	static const int SUOYAOTA_ROOM_USER_MAX = 10;
	static const int SUOYAOTA_TASK_MAX = 4;

	class SCSuoYaoTaFBInfo	
	{
	public:
		SCSuoYaoTaFBInfo();
		MessageHeader		header;

		void Reset()
		{
			reason = 0;

			fb_lv = 0;

			user_count = 0;
			memset(user_list, 0, sizeof(user_list));

			memset(task_list, 0, sizeof(task_list));
		}

		enum  
		{
			SUOYAOTA_NOTIFY_INVALID = 0,
			SUOYAOTA_NOTIFY_SCORE_CHANGE,
			SUOYAOTA_NOTIFY_ENTER_SCENE,
			SUOYAOTA_NOTIFY_WAVE_FINISH,
			SUOYAOTA_NOTIFY_MAX,
		};

		int reason;

		int fb_lv;

		struct SYTTask
		{
			int task_type;
			int param_id;
			int param_num;
			int param_max;
		};
		SYTTask task_list[SUOYAOTA_TASK_MAX];

		struct SYTUserInfo
		{
			int uid;
			int score;
		};

		int user_count;
		SYTUserInfo user_list[SUOYAOTA_ROOM_USER_MAX];
	};

	class SCSuoYaoTaState
	{
	public:
		SCSuoYaoTaState();
		MessageHeader		header;

		int status;
		unsigned int next_status_time;
		unsigned int next_standby_time;
		unsigned int next_stop_time;
		
		int datais_valid;
		int syt_max_score;
		int quanfu_topscore;
		int	quanfu_topscore_uid;
		GameName quanfu_topscore_name;
	};

	class CSGetSuoYaoTaState
	{
	public:
		CSGetSuoYaoTaState();
		MessageHeader		header;
	};

	class CSGetSuoYaoTaReward
	{
	public:
		CSGetSuoYaoTaReward();
		MessageHeader		header;

		int times;
	};

	class SCNotifySuoYaoTaReward
	{
	public:
		SCNotifySuoYaoTaReward();
		MessageHeader		header;

		int score;
		int exp;
		int bind_coin;
	};

	////////////////////////////////////////////////////////////////////////////////////
	class CSGetGuildFBGuardPos				// 5496
	{
	public:
		CSGetGuildFBGuardPos();
		MessageHeader		header;
	};

	class SCGuildFBGuardPos					// 5497
	{
	public:
		SCGuildFBGuardPos();
		MessageHeader		header;

		int scene_id;
		int pos_x;
		int pos_y;
	};

	////////////////////////////////////////////////////////////////////////////////////
	class SCWorldBossDead
	{
	public:
		SCWorldBossDead();
		MessageHeader		header;

		int boss_id;
	};

	enum ZHUAGUI_NOTIFY_REASON
	{
		ZHUAGUI_NOTIFY_REASON_ENTER,
		ZHUAGUI_NOTIFY_REASON_MONSTER_DEAD,
		ZHUAGUI_NOTIFY_REASON_ENTER_FINISH,
	};

	class SCZhuaGuiFbInfo
	{
	public:
		SCZhuaGuiFbInfo();
		MessageHeader		header;

		static const int NEW_FB_REWARD_ITEM_SC_MAX = 30;						// 通知客户端最大物品数
		void Reset()
		{
			monster_count = 0;
			ishave_boss = 0;
			boss_isdead = 0;
			enter_role_num = 0;
			item_count = 0;
			kick_time = 0;

			memset(zhuagui_info_list, 0, sizeof(zhuagui_info_list));
			memset(item_list, 0, sizeof(item_list));
		}

		int reason;

		int monster_count;
		short ishave_boss;
		short boss_isdead;
		unsigned int kick_time;

		struct TeamZhuaGuiInfo
		{
			TeamZhuaGuiInfo():uid(0), get_hunli(0), get_mojing(0), kill_boss_count(0){}

			int uid;
			int get_hunli;
			int get_mojing;
			int kill_boss_count;
		};

		void AddHunli(int uid, int huoli, int mojing, int boss_monster_count)
		{
			for (int i = 0; i < MAX_TEAM_MEMBER_NUM; i++)
			{
				if (zhuagui_info_list[i].uid == uid || 0 == zhuagui_info_list[i].uid)
				{
					zhuagui_info_list[i].uid = uid;
					zhuagui_info_list[i].get_hunli += huoli;
					zhuagui_info_list[i].get_mojing += mojing;
					zhuagui_info_list[i].kill_boss_count += boss_monster_count;
					break;
				}
			}
		}

		TeamZhuaGuiInfo zhuagui_info_list[MAX_TEAM_MEMBER_NUM];
		short enter_role_num;
		short item_count;
		FBDropItemInfo item_list[NEW_FB_REWARD_ITEM_SC_MAX];
	};

	class SCZhuaguiAddPerInfo
	{
	public:
		SCZhuaguiAddPerInfo();
		MessageHeader	header;

		short couple_hunli_add_per;  
		short couple_boss_add_per;
		short team_hunli_add_per;
		short team_boss_add_per;
	};

	class SCCrossTeamFbInfo					// 5498
	{
	public:
		SCCrossTeamFbInfo();
		MessageHeader		header;

		struct UserInfo
		{
			GameName user_name;
			long long dps;
		};

		int user_count;
		UserInfo user_info[CROSS_TEAM_FB_ROLE_MAX];
	};

	class SCTeamEquipFbInfo						// 5570
	{
	public:
		SCTeamEquipFbInfo();
		MessageHeader		header;

		struct UserInfo
		{
			GameName user_name;
			long long dps;
		};

		int user_count;
		UserInfo user_info[TEAM_EQUIP_FB_ROLE_MAX];
	};

	class CSTeamEquipFbBuyDropCount							// 5572组队装备副本购买掉落次数
	{
	public:
		CSTeamEquipFbBuyDropCount();
		MessageHeader		header;
	};


	class SCTeamEquipFbDropCountInfo						// 5571组队装备副本掉落次数信息
	{
	public:
		SCTeamEquipFbDropCountInfo();
		MessageHeader		header;

		int team_equip_fb_pass_flag;						// 组队装备本通关标记
		int team_equip_fb_day_count;						// 组队装备本每日次数
		int team_equip_fb_day_buy_count;					// 组队装备本每日购买次数
	};

	/////////////////////////////////////////// 推图副本 /////////////////////////////////////////////

	enum TUITU_FB_OPERA_REQ_TYPE
	{
		TUITU_FB_OPERA_REQ_TYPE_ALL_INFO = 0,				//请求信息	
		TUITU_FB_OPERA_REQ_TYPE_BUY_TIMES,					//购买进入副本次数 param_1 购买副本类型 param_2, 购买次数
		TUITU_FB_OPERA_REQ_TYPE_FETCH_STAR_REWARD,			//拿取星级奖励 param_1:副本类型  param_2:配置表seq
		TUITU_FB_OPERA_REQ_TYPE_SAODANG,					//扫荡 param_1:副本类型 param_2:章节 param_3:关卡

		TUITU_FB_OPERA_REQ_TYPE_MAX
	};

	class CSTuituFbOperaReq			// 8430
	{
	public:
		CSTuituFbOperaReq();
		MessageHeader header;

		int opera_type;					
		int param_1;
		int param_2;
		int param_3;
	};

	class SCTuituFbInfo				//8431
	{
	public:
		SCTuituFbInfo();
		MessageHeader header;

		TuituFbParam::FbInfo fb_info_list[TUTUI_FB_TYPE_NUM_MAX];
	};

	class SCTuituFbResultInfo		//8432
	{
		struct ItemInfo
		{
			ItemID item_id;
			short num;
			char is_bind;
			char reserve_ch;
			short reserve_sh;
		};

	public:
		static const int MAX_REWARD_ITEM_NUM = 64;

		SCTuituFbResultInfo();
		MessageHeader header;

		char star;			//通关星级 star > 0则成功 否则失败
		char reserve_ch;	//保留字段
		short item_count;	//物品个数

		ItemInfo reward_item_list[MAX_REWARD_ITEM_NUM];
	};

	class SCTuituFbSingleInfo		//8433
	{
	public:
		SCTuituFbSingleInfo();
		MessageHeader header;
	
		short fb_type;						//副本类型
		char chapter;						//副本章节
		char level;							//副本关卡等级
		short cur_chapter;					//当前进行章节
		short cur_level;					//当前进行关卡等级
		short today_join_times;				//今日进入副本次数
		short buy_join_times;				//购买次数
		short chapter_total_star;			//章节总星数
		short chapter_star_reward_flag;		//章节星数奖励标记
		TuituFbParam::LayerInfo layer_info;	//关卡信息
	};

	class SCTuituFbFetchResultInfo
	{
	public:
		SCTuituFbFetchResultInfo();
		MessageHeader header;

		short is_success;			//是否成功拿取 1成功 0失败
		short fb_type;				//fb类型（目前只有普通本有奖励）
		short chapter;				//章节
		short seq;					//拿取的seq（客户端根据这个seq取奖励显示）
	};
	/////////////////////////////////////////// 宝宝boss /////////////////////////////////////////////
	class CSBabyBossOperate						//8614
	{
	public:
		CSBabyBossOperate();
		MessageHeader header;

		enum OPERATE_TYPE
		{
			TYPE_BOSS_INFO_REQ = 0,				// 请求boss信息
			TYPE_ROLE_INFO_REQ = 1,				// 请求人物相关信息
			TYPE_ENTER_REQ = 2,					// 请求进入宝宝boss
			TYPE_LEAVE_REQ = 3,					// 请求离开宝宝boss
		};

		int operate_type;
		int param_0;
		unsigned short param_1;
		short reserve_sh;
	};

	class SCBabyBossRoleInfo				//8615
	{
	public:
		SCBabyBossRoleInfo();
		MessageHeader header;

		short enter_times;					//进入次数
		short angry_value;					//愤怒值
		unsigned int kick_time;				//提出时间
	};

	struct BabyBossInfo
	{
		BabyBossInfo() : scene_id(0), boss_id(0), next_refresh_time(0)
		{}

		short scene_id;						//场景id
		unsigned short boss_id;				//boss_id
		unsigned int next_refresh_time;		//下次刷新时间

		struct KillerInfo
		{
			KillerInfo() :killer_uid(0), killier_time(0)
			{
				memset(killer_name, 0, sizeof(killer_name));
			}

			int killer_uid;
			unsigned int killier_time;		//击杀时间
			GameName killer_name;			//击杀名字
		};

		static const int KILLER_LIST_MAX_COUNT = 5;

		KillerInfo killer_info_list[KILLER_LIST_MAX_COUNT];
	};

	class SCAllBabyBossInfo					//8616
	{
	public:
		SCAllBabyBossInfo();
		MessageHeader header;

		static const int MAX_BABY_BOSS_COUNT = 100;

		int boss_count;
		BabyBossInfo boss_info_list[MAX_BABY_BOSS_COUNT];
	};

	class SCSingleBabyBossInfo				//8617
	{
	public:
		SCSingleBabyBossInfo();
		MessageHeader header;

		BabyBossInfo boss_info;
	};

	class SCRuneTowerPassRewardInfo         // 8818 符文塔奖励信息    客户端显示奖励面板
	{
	public:
		SCRuneTowerPassRewardInfo();
		MessageHeader header;

		struct RewardItem
		{
			ItemID item_id;
			char is_bind;
			char num;
		};
		
		int reward_count;
		RewardItem reward_list[RUNE_TOWER_RAND_REWARD_MAX_COUNT];     // 7
	};

	/////////////////////////////////////////////////// 节日守护副本 /////////////////////////////////////////////////////
	enum CS_HOLIDAI_GUARD_REQ
	{
		HOLIDAY_GUARD_NEXT_WAVE,
	};

	class CSHolidayGuardRoleReq		// 8825 请求操作
	{
	public:
		CSHolidayGuardRoleReq();
		MessageHeader header;

		int req_type;
	};

	class SCHolidayGuardRoleInfo		// 8826 个人信息
	{
	public:
		SCHolidayGuardRoleInfo();
		MessageHeader header;

		int personal_join_times;
		short personal_kill_monster_count;
		short reserve_ch;
	};

	class SCHolidayGuardInfo			// 8827 塔防信息
	{
	public:
		SCHolidayGuardInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_INIT = 1,
			NOTIFY_REASON_NEW_WAVE_START = 2,
			NOTIFY_REASON_KILL_MONSTER = 3,
		};

		short reason;
		short reserve;

		unsigned int time_out_stamp;
		short is_finish;
		short is_pass;
		int pass_time_s;

		long long life_tower_left_hp;							// 生命塔HP
		long long life_tower_left_maxhp;						// 生命塔最大HP
		short curr_wave;								// 当前波
		short reserve_1;
		int next_wave_refresh_time;						// 下一波到来时间
		short clear_wave_count;							// 清理怪物波数
		short total_kill_monster_count;					// 击杀怪物数量

		struct DropItem
		{
			DropItem() : num(0), item_id(0) {}

			short num;
			ItemID item_id;
		};

		int get_coin;
		int get_item_count;
		SCHolidayGuardInfo::DropItem drop_items[FB_RECORD_DROP_ITEM_MAX_NUM];
	};

	class SCHolidayGuardFBDropInfo		// 8828 副本怪物掉落统计
	{
	public:
		SCHolidayGuardFBDropInfo();
		MessageHeader header;

		const static int FB_DROP_ITEM_MAX_NUM = 32;

		struct DropItem
		{
			DropItem() : num(0), item_id(0) {}

			short num;
			ItemID item_id;
		};

		int get_coin;
		int get_item_count;
		DropItem drop_items[FB_DROP_ITEM_MAX_NUM];
	};

	class SCHolidayGuardResult		// 8829 通关结果	
	{
	public:
		SCHolidayGuardResult();
		MessageHeader header;

		char is_passed;
		char clear_wave_count;
		short resertotal_kill_monster_countve_sh;
	};

	class SCHolidayGuardWarning		// 8830 生命塔被攻击预警
	{
	public:
		SCHolidayGuardWarning();
		MessageHeader header;

		enum WARNING_TYPE
		{
			WARNING_TYPE_NORMAL = 1,
			WARNING_TYPE_LOW_PERCENT,
		};

		short warning_type;			// 1：普通挨打（忽略百分比），2：HP低于百分比
		short percent;
	};

	class SCNoticeBossDead                                  // boss死亡通知同场景有输出的玩家 8823
	{
	public:
		SCNoticeBossDead();
		MessageHeader header;

		int boss_id;
		int killer_uid;
		long long killer_avatar_timestamp;					// 击杀者头像
	};

	class CSFetchDoubleRewardReq							// 8873 请求双倍领取
	{
	public:
		CSFetchDoubleRewardReq();
		MessageHeader header;

		enum OPERA_TYPE
		{
			OPERA_TYPE_INFO_REQ = 0,
			OPERA_TYPE_BUY_DOUBLE_REWARD,
			OPERA_TYPE_BUY_TRIPLE_REWARD,
		};

		int fb_type;										//副本类型
		int opera_type;											
	};

	class SCFetchDoubleRewardResult							// 8874 双倍领取结果
	{
	public:
		SCFetchDoubleRewardResult();
		MessageHeader header;

		struct DropItem
		{
			DropItem() : num(0), item_id(0) {}

			short num;
			ItemID item_id;
		};

		int fb_type;										//副本类型
		char today_buy_times;								//今天已购买次数
		char reserve_sh;
		short item_count;
		int xiannv_shengwu;									//仙气值组队守护用
		DropItem item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	};

	////////////////////////////个人boss//////////////////////////////////
	class CSPersonBossInfoReq					// 个人boss信息请求   8876
	{
	public:
		CSPersonBossInfoReq();
		MessageHeader header;
	};

	class SCPersonBossInfo						// 个人boss信息    8877
	{
	public:
		SCPersonBossInfo();
		MessageHeader header;

		struct Info
		{
			Info() : layer(0), times(0) {}
			short layer;
			short times;
		};

		Info info_list[MAX_PERSON_BOSS_LAYER];     // 40
	};

	/////////////////////////////////////  神魔BOSS  /////////////////////////////////////
	enum GODMAGIC_BOSS_OPERA_TYPE
	{
		GODMAGIC_BOSS_OPERA_TYPE_ENTER =0,				// 进入 param1 scene_id pararm2 boss_id
		GODMAGIC_BOSS_OPERA_TYPE_PLAYER_INFO,			// 玩家信息 
		GODMAGIC_BOSS_OPERA_TYPE_GET_FLUSH_INFO,		// 刷新信息 param1 层数（为0则为所有层）

		GODMAGIC_BOSS_OPERA_TYPE_MAX,
	};

	class CSGodmagicBossInfoReq											// 请求神魔boss信息   8910
	{
	public:
		CSGodmagicBossInfoReq();
		MessageHeader	header;

		int req_type;
		int param_1;
		int param_2;
	};

	class SCGodmagicBossInfoAck											//  神魔boss信息   8911
	{
	public:
		SCGodmagicBossInfoAck();
		MessageHeader	header;

		struct BossInfo
		{
			int boss_id;
			unsigned int next_flush_time;
		};

		struct SceneInfo
		{
			short layer;
			short left_treasure_crystal_count;
			short left_monster_count;
			short boss_count;
			BossInfo boss_list[MAX_GODMAGIC_BOSS_PER_SCENE];
		};

		int scene_count;
		SceneInfo scene_list[GODMAGIC_BOSS_SCENE_MAX];
	};

	class SCGodmagicBossPlayerInfo												// 神魔BOSS玩家信息 8912
	{
	public:
		SCGodmagicBossPlayerInfo();
		MessageHeader	header;

		short left_can_kill_boss_num;										// 剩余Boss数
		short left_treasure_crystal_gather_times;							// 剩余珍惜水晶采集次数
		int left_ordinary_crystal_gather_times;								// 剩余普通水晶采集次数

	};

	class SCGodmagicBossSceneInfo												// 神魔boss场景里的玩家信息 8913
	{
	public:
		SCGodmagicBossSceneInfo();
		MessageHeader	header;

		struct CrossBossInfo
		{
			CrossBossInfo() : boss_id(0), is_exist(0), next_flush_time(0) {}

			int boss_id;
			int is_exist;
			unsigned int next_flush_time;
		};

		short left_monster_count;											// 剩余小怪数量
		short left_treasure_crystal_num;									// 剩余珍惜水晶数量
		short layer;
		short treasure_crystal_gather_id;									// 珍惜水晶采集物id
		unsigned int monster_next_flush_timestamp;							// 小怪下次刷新时间
		unsigned int treasure_crystal_next_flush_timestamp;					// 珍惜水晶下次刷新时间
		CrossBossInfo boss_list[MAX_GODMAGIC_BOSS_PER_SCENE];					// Boss信息
	};
	
	/////////////////////////////////////////// 云游boss ////////////////////////////////////
	class CSYunyouBossOperate						//8906
	{
	public:
		CSYunyouBossOperate();
		MessageHeader header;

		enum OPERATE_TYPE
		{
			TYPE_BOSS_INFO_REQ = 0,				// 请求boss信息 p0场景id
			TYPE_BOSS_COUNT_ALL_SCENE = 1,		// 请求boss所有场景个数信息 
		};

		int operate_type;
		int param_0;
	};

	class SCYouyouBossInfo				//8907 单个场景具体信息
	{
	public:
		SCYouyouBossInfo();
		MessageHeader header;

		struct BossInfo
		{
			int scene_id = 0;
			int boss_id = 0;
			int born_pos_x = 0;
			int born_pos_y = 0;
		};

		unsigned int nex_refresh_time;			// 下一次刷新时间
		int scene_id;

		int boss_count;
		BossInfo boss_info_list[YUNYOU_BOSS_MAX_NUM_PER_SCENE]; // boss信息
	};

	class SCYouyouSceneInfo				// 8908 云游boss全部场景个数信息
	{
	public:
		SCYouyouSceneInfo();
		MessageHeader header;

		struct SceneInfo
		{
			int scene_id = 0;
			int boss_count = 0;
		};

		int scene_count;
		SceneInfo scene_info_list[YUNYOU_BOSS_MAX_SCENE_NUM]; // boss信息
	};
}

#pragma pack(pop)

#endif


