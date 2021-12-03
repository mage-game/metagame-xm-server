#ifndef __MSG_ACTIVITY_HPP__
#define __MSG_ACTIVITY_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/roleactivitydef.hpp"
#include "servercommon/vipdef.hpp"
#include "servercommon/crossdef.hpp"
#include "servercommon/challengefielddef.hpp"
#include "servercommon/struct/global/worldstatusparam.hpp"
#include "global/roleboss/rolebossconfig.h"
#include "config/activityconfig/opengameactivityconfig.hpp"
#include "config/activityconfig/xingzuoyijiconfig.hpp"
#include "servercommon/magicalpreciousdef.hpp"
#include "servercommon/qingyuandef.hpp"
#include "gameworld/gameworld/config/activityconfig/randactivityconfig.hpp"
#include "other/fb/holidayguardconfig.hpp"

#pragma pack(push)
#pragma pack(4)

namespace Protocol
{
	class SCActivityStatus
	{
	public:
		SCActivityStatus();
		MessageHeader header;

		short activity_type;
		char status;
		char is_broadcast;
		unsigned int next_status_switch_time;
		unsigned int param_1;
		unsigned int param_2;
		unsigned int open_type;
	};

	class SCActivityRoomStatusAck
	{
	public:
		SCActivityRoomStatusAck();
		MessageHeader		header;

		int activity_type;

		int room_user_max;
		ActivityRoomStatus	room_status_list[ACTIVITY_ROOM_MAX];			// 活动对应的房间信息 
	}; 

	class CSActivityRoomStatusReq											// 活动房间状态请求
	{
	public:
		CSActivityRoomStatusReq();
		MessageHeader	header;

		int	activity_type;													// 活动类型
	};

	class CSActivityEnterReq
	{
	public:
		CSActivityEnterReq();
		MessageHeader header;

		int activity_type;
		int room_index;
	};

	class CSSendGuildSosReq
	{
	public:
		CSSendGuildSosReq();
		MessageHeader header;

		int sos_type;
	};

	class CSReplyGuildSosReq
	{
	public:
		CSReplyGuildSosReq();
		MessageHeader header;

		int sos_type;
		int pos_x;
		int pos_y;

		int scene_id;
	};

	class SCActivitySkillUseInfo
	{
	public:
		SCActivitySkillUseInfo();
		MessageHeader		header;

		int skill_id;
	};

	 /////////////////////////////////////////// 诛邪战场相关 ///////////////////////////////////////////

	 enum ZHUXIE_NOTIFY_TYPE																// 诛邪战场通知原因
	 {	
		ZHUXIE_NOTIFY_TYPE_DEFAULT = 0,														// 默认类型
		ZHUXIE_NOTIFY_TYPE_FETCH_TASKKREWARD = 1,											// 获取任务奖励
		ZHUXIE_NOTIFY_TYPE_ENTER_SCENE = 2,													// 进入诛邪战场
	 };

	 class CSZhuXieFetchTaskReward															// 令却诛邪任务奖励
	 {
	 public:
		 CSZhuXieFetchTaskReward();
		 MessageHeader header;

		 int task_id;																		// 任务id
	 };

	 class SCZhuXieUserInfo
	 {
	 public:
		 SCZhuXieUserInfo();
		 MessageHeader header;

		 struct ClientTaskInfo
		 {
			 ClientTaskInfo() : task_id(0), param_value(0), max_value(0), is_fetch_reward(0) {}

			 short task_id;																	// 任务ID
			 short param_value;																// 任务当前值
			 short max_value;																// 完成任务所需最大值
			 short is_fetch_reward;															// 是否领取奖励																	// 经验奖励
		 };

		unsigned int boss_next_flush_time[ZHUXIE_BOSS_NUM];									// boss下次刷新时间
		ClientTaskInfo taskinfo_list[ZHUXIE_TASK_MAX];										// 用户任务信息
	 };

	 /////////////////////////////////////////// 天降财宝相关 ///////////////////////////////////////////

	 enum TIANJIANGCAIBAO_NOTIFY_TYPE																// 天降财宝通知原因
	 {	
		TIANJIANGCAIBAO_NOTIFY_TYPE_DEFAULT = 0,													// 默认类型
		TIANJIANGCAIBAO_NOTIFY_TYPE_FETCH_TASKKREWARD = 1,											// 获取任务奖励
		TIANJIANGCAIBAO_NOTIFY_TYPE_ENTER_SCENE = 2,												// 进入天降财宝
	 };

	 class CSTianJiangCaiBaoFetchTaskReward															// 领取天降财宝奖励
	 {
	 public:
		 CSTianJiangCaiBaoFetchTaskReward();
		 MessageHeader header;

		 int task_id;																		// 任务id
	 };

	 class SCTianJiangCaiBaoUserInfo
	 {
	 public:
		 SCTianJiangCaiBaoUserInfo();
		 MessageHeader header;

		 unsigned int bigqianduoduo_reflush_time;											// 大型钱多多刷新时间
		 unsigned int qianduoduo_reflush_time;												// 钱多多下次刷新时间
		 int cur_qianduoduo_num;
		 int cur_bigqianduoduo_num;
		 int get_total_gold;
		 int is_finish;																		// 活动是否结束，1为是，0为不是
		 int curr_task_id;
		 int curr_task_param;
		 int task_finish_num;

		 int reward_count;
		 RewardItemInfo reward_list[TIANJIANGCAIBAO_MAX_REWARD_ITEM_COUNT];					// 获得的奖励信息
	 };

	 /////////////////////////////////////////// 王陵探险相关 ///////////////////////////////////////////

	 class SCWangLingExploreUserInfo
	 {
	 public: 
		 SCWangLingExploreUserInfo();
		 MessageHeader header;

		 struct SClientTaskInfo
		 {
			 SClientTaskInfo() : task_id(0), is_finish(0), param_count(0), cur_param_value(0), is_double_reward(0){}

			 short task_id;																	// 任务ID
			 short is_finish;																// 是否领取奖励

			 int param_count;																// 任务关联的采集物或怪物的数量
			 int cur_param_value;															// 当前值
			 int is_double_reward;
		 };

		 struct RewardItemInfo																// 获得的物品信息
		 {
			 RewardItemInfo() : item_id(0), num(0) {}

			 int item_id;
			 int num;
		 };

		 int boss_reflush_time;
		 int limit_task_time;
		 int boss_num;
		 int boss_owner_uid;
		 int gather_buff_time;

		 SClientTaskInfo taskinfo_list[WANGLINGEXPLORE_TASK_MAX];							// 用户任务信息
		 RewardItemInfo reward_list[WANGLINGEXPLORE_MAX_REWARD_ITEM_COUNT];					// 获得的奖励信息
	 };

	 class WangLingExploreBossInfo //6637
	 {
	 public:
		 WangLingExploreBossInfo();
		 MessageHeader header;

		 int monster_id;
		 long long max_hp;
		 long long cur_hp;
	 };

	 class CSWangLingExploreBuyBuff //6638
	 {
	 public:
		 CSWangLingExploreBuyBuff();
		 MessageHeader header;
	 };

	/////////////////////////////////////////////////  群仙乱斗  /////////////////////////////////////////////////////////
	enum QUNXIANLUANDOU_NOTIFY_REASON
	{
		QUNXIANLUANDOU_NOTIFY_REASON_DEFAULT = 0,										// 默认
		QUNXIANLUANDOU_NOTIFY_REASON_WIN,												// 获胜
		QUNXIANLUANDOU_NOTIFY_REASON_LOSE,												// 失败
		QUNXIANLUANDOU_NOTIFY_REASON_DRAW,												// 平局
	};

	class SCQunXianLuanDouUserInfo														//4903 群仙乱斗 用户信息
	{
	public:
		SCQunXianLuanDouUserInfo();
		MessageHeader		header;

		int notify_reason;																// 通知原因

		short side;																		// 角色在战场中的阵营	
		short kills;																	// 杀人次数
		short lianzhan;																	// 连杀次数
		short assists;																	// 助攻次数
		short rob_shenshi;																// 夺石次数
		unsigned short realive_guard_monsterid;											// 复活点守卫怪物id;

		int kill_honor;																	// 击杀荣誉
		int assist_honor;																// 助攻荣誉
		int rob_shenshi_honor;															// 运石荣誉
		int free_reward_honor;															// 免费赠送荣誉
		unsigned int last_realive_here_timestamp;										// 最后一次原地复活时间
		int extra_honor;																// 额外奖励

		GameName first_rank_num[QUNXIANLUANDOU_SIDE_MAX];
		int first_rank_uid[QUNXIANLUANDOU_SIDE_MAX];
	};

	class SCQunXianLuanDouRankInfo		// 群仙乱斗 排行榜信息  4904
	{
	public:
		SCQunXianLuanDouRankInfo();
		MessageHeader		header;

		struct RankItem
		{
			RankItem() : uid(0), score(0), side(0)
			{
				memset(name, 0, sizeof(name));
			}

			int uid;
			GameName name;
			int score;
			int side;
		};

		int self_score;
		int self_rank_pos;
		int count;																		// 群仙乱斗 排行榜信息数量
		RankItem rank_list[QUNXIANLUANDOU_RANK_NUM];									// 群仙乱斗 排行榜信息列表
	};

	class SCQunXianLuanDouSideInfo														// 群仙乱斗 阵营信息
	{
	public:
		SCQunXianLuanDouSideInfo();
		MessageHeader		header;

		int score[QUNXIANLUANDOU_SIDE_MAX];												// 群仙乱斗 阵营积分
		unsigned int shenshi_next_refresh_time;											// 神石下次刷新时间，0：已经出来
	};

	class SCQunXianLuanDouShenShiStatus													// 神石状态
	{
	public:
		SCQunXianLuanDouShenShiStatus();												
		MessageHeader		header;

		int has_owner;																	// 是否有拥有者
		ObjID obj_id;																	// 神石objid
		ObjID owner_obj_id;																// 拥有者objid
		int owner_uid;																	// 拥有者user id
	};

	class CSBattleFieldHonorInfoReq														// 请求战场荣誉值												
	{
	public:
		CSBattleFieldHonorInfoReq();																										
		MessageHeader		header;
	};

	class SCBattleFieldHonorChange														// 战场荣誉值改变
	{
	public:
		SCBattleFieldHonorChange();																										
		MessageHeader		header;

		int honor;																		// 当前荣誉值
		int delta_honor;																// 荣誉改变数值
	};

	class CSQunXianLuanDouPickUpShenShi													// 捡起神石请求
	{
	public:
		CSQunXianLuanDouPickUpShenShi();												
		MessageHeader		header;

		int shenshi_objid;																// 神石objid
	};

	class SCQunxianluandouLianzhanChange												// 连斩数改变
	{
	public:
		SCQunxianluandouLianzhanChange();												
		MessageHeader		header;

		ObjID obj_id;																	// 对象id
		short lianzhan;																	// 连斩数
	};

	class SCQunxianluandouLuckyRewardInfo  // 4950
	{
	public:
		SCQunxianluandouLuckyRewardInfo();
		MessageHeader		header;

		unsigned int next_lucky_timestamp;

		int luck_user_count;
		GameName luck_user_namelist[ZHANCHANG_LUACY_ROLE_MAX];
	};

	class CSQunxianLuandouFirstRankReq				// 4945 请求群仙乱斗各边第一名字
	{
	public:
		CSQunxianLuandouFirstRankReq();
		MessageHeader header;
	};

	class SCQunxianLuandouFirstRankInfo				// 4946 下发群仙乱斗各边第一名字
	{
	public:
		SCQunxianLuandouFirstRankInfo();
		MessageHeader header;

		GameName first_rank_num[QUNXIANLUANDOU_SIDE_MAX];
		int first_rank_uid[QUNXIANLUANDOU_SIDE_MAX];
	};

	class CSFetchGuildBattleDailyReward				// 4975 公会争霸每日奖励操作请求
	{
	public:
		CSFetchGuildBattleDailyReward();
		MessageHeader header;

		enum OPERATE_TYPE
		{
			TYPE_INFO_REQ = 0,						// 请求信息
			TYPE_FETCH_REQ = 1,						// 领取奖励
		};

		int operate_type;
	};

	class SCSendGuildBattleDailyRewardFlag			// 4976 下发公会争霸奖励信息
	{
	public:
		SCSendGuildBattleDailyRewardFlag();
		MessageHeader header;

		short had_fetch;							//是否领取，0否1是
		char rank;									//排名
		char reserve_ch;
	};

	/////////////////////////////////////////////////   仙盟信息统计  //////////////////////////////////////////////////////
	class CSGuildInfoStatisticReq		//6690
	{
	public:
		CSGuildInfoStatisticReq();
		MessageHeader header;

		int activity_type;
	};

	class SCGuildInfoStatistic			//6691
	{
	public:
		SCGuildInfoStatistic();
		MessageHeader header;

		struct InfoItem
		{
			long long uid;
			GameName user_name;
			int guild_post;
			short kill_role;		//杀人数
			short kill_target;		//击杀目标次数
			long long hurt_roles;	//对玩家造成伤害
			long long hurt_targets;	//对目标的总伤害
		};

		enum 
		{
			NOTIFY_ACTIVITY_CLOSE = 0, // 活动结束返回
			NOTIFY_ACTIVITY_REQ = 1,   // 请求返回
		};

		int notify_type;  // 通知类型
		int activity_type;
		int guild_id;
		int count;
		InfoItem info_list[MAX_MEMBER_COUNT];
	};

	class SCGuildMvpInfo			//6692
	{
	public:
		SCGuildMvpInfo();
		MessageHeader header;
		
		int activity_type;
		long long uid;
		GameName user_name;
	};

	/////////////////////////////////////////////////   攻城战相关  /////////////////////////////////////////////////////////

	enum GCZ_GONG_SHOU_ZIYUAN_PLACE
	{
		GCZ_GOTO_GONG_PLACE = 0,					// 攻旗地点
		GCZ_GOTO_SHOU_PLACE = 1,					// 守旗地点
		GCZ_GOTO_ZIYUAN_PLACE = 2,					// 资源区地点
	};

	class CSGCZChangePlace							// 通过按钮改变角色的位置
	{
	public:
		CSGCZChangePlace();
		MessageHeader header;

		int place_type;													
	};

	class SCGCZRewardInfo							// 奖励结算
	{
	public:
		SCGCZRewardInfo();
		MessageHeader header;

		static const int MAX_ITEM_COUNT = 4;
		struct ItemInfo
		{
			ItemInfo() : reward_item_id(0), reward_item_num(0) {}
			ItemID reward_item_id;
			short reward_item_num;
		};

		int honor_reward;
		int gold_reward;
		int shengwang_reward;
		int chengzhu_uid; 
		int item_count;
		ItemInfo item_list[MAX_ITEM_COUNT];
	};

	class SCGCZGlobalInfo					// 攻城战全局信息   6701
	{
	public:
		SCGCZGlobalInfo();
		MessageHeader	header;

		char is_finish;
		char is_pochen;
		char is_poqiang;
		char cheng_zhu_prof;
		unsigned int current_shou_cheng_time;
		GuildID shou_guild_id;
		GuildName shou_guild_name;
		int totem_level;
		int po_cheng_times;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				guild_id = INVALID_GUILD_ID;
				shouchen_time = 0;
				memset(guild_name, 0, sizeof(guild_name));
			}

			GuildID guild_id;
			unsigned int shouchen_time;
			GuildName guild_name;
		};

		static const int RANK_NUM = 5;

		int self_guild_rank_pos;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class SCGCZRoleInfo								// 角色信息
	{
	public:
		SCGCZRoleInfo();
		MessageHeader		header;
		
		char is_shousite;
		char sos_times;
		short reserve_2;
		int zhangong;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				role_id = 0;
				zhangong = 0;
				memset(role_name, 0, sizeof(role_name));
			}

			int role_id;
			GameName role_name;
			int zhangong;
		};

		static const int RANK_NUM = 5;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class SCGongChengZhanOwnerInfo					// 城主信息
	{
	public:
		SCGongChengZhanOwnerInfo();
		MessageHeader		header;

		int owner_id;
		GameName owner_name;
		char vip_level;
		char sex;
		char prof;
		char reserve_ch;
		int guild_id;
	};

	class SCGongChengZhanFlagInfo //6706
	{
	public:
		SCGongChengZhanFlagInfo();
		MessageHeader header;

		long long max_hp;
		long long cur_hp;
	};

	class CSGCZFetchDailyRewardReq
	{
	public:
		CSGCZFetchDailyRewardReq();
		MessageHeader		header;
	};

	class SCGczLuckyRewardInfo  // 6753
	{
	public:
		SCGczLuckyRewardInfo();
		MessageHeader		header;

		unsigned int next_lucky_timestamp;

		int luck_user_count;
		GameName luck_user_namelist[ZHANCHANG_LUACY_ROLE_MAX];
	};

	class SCGCZWorshipInfo		//6754
	{
	public:
		SCGCZWorshipInfo();
		MessageHeader header;

		int worship_time;										//玩家膜拜次数
		unsigned int next_worship_timestamp;					//玩家下次膜拜时间戳
		unsigned int next_interval_addexp_timestamp;			//玩家下次加经验时间戳
	};

	class SCGCZWorshipActivityInfo				//6755
	{
	public:
		SCGCZWorshipActivityInfo();
		MessageHeader header;

		char worship_is_open;							// 膜拜活动是否开启
		char reserve_ch;								// 保留
		short reserve_sh;								// 保留
		unsigned int worship_end_timestamp;				// 膜拜活动结束时间戳
	};

	class CSGCZWorshipReq		//6756
	{
	public:
		CSGCZWorshipReq();
		MessageHeader header;
	};

	//////////////////////////////////////////////////////////////////////////
	class SCRoleWorshipInfo		// 6757 下发个人膜拜信息
	{
	public:
		SCRoleWorshipInfo();
		MessageHeader header;

		int activity_type;							// 活动类型
		int worship_times;							// 玩家膜拜次数
		unsigned int next_worship_timestamp;		// 玩家下次膜拜时间戳
	};

	class SCRoleWorshipStart		// 6758 广播角色开始膜拜动作
	{
	public:
		SCRoleWorshipStart();
		MessageHeader header;

		ObjID role_obj;
		short resever_sh;
	};

	class CSRoleWorshipReq			// 6759 膜拜点击
	{
	public:
		CSRoleWorshipReq();
		MessageHeader header;
	};

	/////////////////////////////////////////////////   公会争霸相关  /////////////////////////////////////////////////////////
	class CSGBAddHuDun								// 6200公会争霸  主动释放护盾
	{
	public:
		CSGBAddHuDun();
		MessageHeader		header;
	};

	class SCGBGlobalInfo							// 6251公会争霸 全局信息（广播）
	{
	public:
		SCGBGlobalInfo();
		MessageHeader		header;

		int self_guild_score;
		int self_guild_rank;
		char is_finish;
		char is_boss_alive;
		short reserve_2;
		unsigned husong_end_time;
		short glod_box_total_num;
		short sliver_box_total_num;
		int boss_curren_hp;
		int boss_max_hp;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				guild_id = INVALID_GUILD_ID;
				score = 0;
				memset(guild_name, 0, sizeof(guild_name));
			}

			GuildID guild_id;
			int score;
			GuildName guild_name;
		};

		static const int RANK_NUM = 100;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class SCGBRoleInfo								// 6252公会争霸 个人信息
	{
	public:
		SCGBRoleInfo();
		MessageHeader		header;


		short kill_role_num;
		short husong_appearance;
		int history_get_person_credit;
		int history_get_guild_credit;
		int husong_gooods_id;
		char is_add_hudun;
		char reserve_1;
		short reserve_2;
	};

	class CSGBRoleCalcSubmitReq						// 6201提交护送任务请求
	{
	public:
		CSGBRoleCalcSubmitReq();
		MessageHeader		header;
	};

	class SCGBBianShenView							// 6250变身形象广播
	{
	public:
		SCGBBianShenView();
		MessageHeader header;

		ObjID obj_id;
		short bianshen_image;

	};

	class CSGBWinnerInfoReq							// 6202请求上一届公会争霸霸主信息
	{
	public:
		CSGBWinnerInfoReq();
		MessageHeader header;
	};

	class SCGBSendWinnerInfo						// 6253返回上一届霸主信息
	{
	public:
		SCGBSendWinnerInfo();
		MessageHeader header;

		GuildID guild_id;
	};

	class SCGBLuckyRewardInfo  // 6254
	{
	public:
		SCGBLuckyRewardInfo();
		MessageHeader		header;

		unsigned int next_lucky_timestamp;

		int luck_user_count;
		GameName luck_user_namelist[ZHANCHANG_LUACY_ROLE_MAX];
	};

	/////////////////////////////////////////////////   新公会争霸   ///////////////////////////////////////////////////////
	class SCGBGlobalInfoNew							// 6255 新公会争霸 全局信息（广播）
	{
	public:
		SCGBGlobalInfoNew();
		MessageHeader		header;

		struct PointItem
		{
			PointItem()
			{
				this->Reset();
			}
			void Reset()
			{
				guild_id = 0;
				memset(guild_name, 0, sizeof(GameName));
				blood = 0;
				max_blood = 0;
			}

			int guild_id;
			GameName guild_name;
			long long blood;
			long long max_blood;
		};

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				guild_id = INVALID_GUILD_ID;
				score = 0;
				memset(guild_name, 0, sizeof(guild_name));
				tuangzhang_uid = 0;
			}

			GuildID guild_id;
			int score;
			GuildName guild_name;
			int tuangzhang_uid;
		};

		static const int RANK_NUM = 10;
		int self_guild_score;
		int self_guild_rank;
		char is_finish;
		char reserve1;
		short reserve2;
		int rank_count;
		RankItem rank_list[RANK_NUM];
		PointItem hold_point_guild_list[GUILD_BATTLE_NEW_POINT_NUM];
	};

	class SCGBRoleInfoNew								// 6256 新公会争霸 个人信息
	{
	public:
		SCGBRoleInfoNew();
		MessageHeader		header;

		int kill_role_num;
		int history_get_person_credit;
		char is_add_hudun;
		char sos_times;
		short reserve2;
	};

	/////////////////////////////////////////////////   领土战相关  /////////////////////////////////////////////////////////

	static const int TERRITORYWAR_SKILL_COUNT_MAX = 2;

	class SCTerritoryWarRoleInfo					// 5952 个人信息		
	{
	public:
		SCTerritoryWarRoleInfo();
		MessageHeader header;

		int current_credit;					// 当前拥有积分
		int history_credit;					// 历史获得积分
		int credit_reward_flag;				// 积分奖励标志位
		short kill_count;					// 击杀玩家数
		short assist_count;					// 助攻击杀玩家数
		short death_count;					// 玩家死亡数
		short side;							// 1(红)，0(蓝)
		char special_image_id;				// 购买攻城车形象
		char reserve_1;
		short reserve_2;
		short ice_landmine_count;			// 拥有的冰箱地雷数量
		short fire_landmine_count;			// 拥有的火焰地雷数量

		struct SkillInfo
		{
			SkillInfo():skill_index(0), last_perform_time(0){}
			
			int skill_index;
			unsigned int last_perform_time;
		};
		
		SkillInfo skill_list[TERRITORYWAR_SKILL_COUNT_MAX];
	};

	class SCTerritoryWarApperance					// 5950 变身形象广播
	{
	public:
		SCTerritoryWarApperance();
		MessageHeader header;

		ObjID obj_id;
		short special_image;

	};

	class SCTerritoryWarGlobeInfo					// 5951 全局信息（广播）
	{
	public:
		SCTerritoryWarGlobeInfo();
		MessageHeader		header;

		int red_guild_credit;
		int blue_guild_credit;
		short center_relive_side;
		short reserve;
		
		long long red_fortress_max_hp;
		long long red_fortress_curr_hp;
		long long blue_fortress_max_hp;
		long long blue_fortress_curr_hp;
		long long center_relive_max_hp;
		long long center_relive_curr_hp;

		int red_building_survive_flag;		// 红方建筑存活状态，1表示已死亡，0表示存活，1-12位表示
		int blue_building_survive_flag;		// 蓝方建筑存活状态，1表示已死亡，0表示存活，1-12位表示

		unsigned int read_next_can_buy_tower_wudi;	// 塔无敌冷去时间
		unsigned int blue_next_can_buy_tower_wudi;

		char winner_side;					// 胜利一方，-1表示没有胜利方，1红方胜利，0蓝方胜利
		char reserve_1;
		short reserve_2;
	};

	class CSTerritoryWarReliveShopBuy				// 5900 复活商点购买
	{
	public:
		CSTerritoryWarReliveShopBuy();
		MessageHeader header;

		short goods_id;
		short reserve;
	};

	class CSTerritoryWarReliveFightBuy				// 5901 战斗商店购买
	{
	public:
		CSTerritoryWarReliveFightBuy();
		MessageHeader header;

		short type;
		short goods_id;
	};

	class CSTerritoryWarQualification				// 5902
	{
	public:
		CSTerritoryWarQualification();
		MessageHeader header;
	};

	static const int CAN_JOIN_MAX_RANK = 10;
	class SCTerritoryWarQualification				// 5953
	{
	public:
		SCTerritoryWarQualification();
		MessageHeader header;

		GuildID m_guildbattel_rank_list[CAN_JOIN_MAX_RANK];
		GuildID m_territorywar_rank_list[CAN_JOIN_MAX_RANK];
	};

	class CSTerritorySetLandMine					// 5903
	{
	public:
		CSTerritorySetLandMine();
		MessageHeader header;

		int landmine_type; // 0火焰地雷 ，1冰箱地雷
		Axis pos_x;						
		Axis pos_y;						
	};

	class SCTwLuckyRewardInfo  // 5954
	{
	public:
		SCTwLuckyRewardInfo();
		MessageHeader		header;

		unsigned int next_lucky_timestamp;

		int luck_user_count;
		GameName luck_user_namelist[ZHANCHANG_LUACY_ROLE_MAX];
	};

	/////////////////////////////////////////////////  温泉  /////////////////////////////////////////////////////////

	class SCHotspringPlayerInfo					// 5352 温泉里玩家信息
	{
	public:
		SCHotspringPlayerInfo();
		MessageHeader		header;

		int popularity;								// 玩家人气值
		long long partner_uuid;
		short give_free_present_time;
		short swinsuit_image;
		long long self_uuid;

		ObjID partner_obj_id;							
		char question_right_count;					// 正确数量
		char question_wrong_count;					// 错误数量
		int curr_score;								// 获得积分
		//int total_exp;								// 获得经验
		long long total_exp;							// 获得经验

		short skill_use_times_1;					// 按摩技能使用次数
		short skill_use_times_2;					// 砸雪球技能使用次数
		unsigned int skill_1_can_perform_time;		// 按摩技能可使用时间戳
		unsigned int skill_2_can_perform_time;		// 砸雪球技能可使用时间戳
		short gather_times;							// 采集次数
		short reserve_sh;
		unsigned int start_fresh_gather_time;		// 采集物开始刷新时间

	};

	class SCHotspringRankInfo					// 5353温泉玩家排名信息
	{
	public:
		SCHotspringRankInfo();
		MessageHeader header;

		
		int self_popularity;
		int self_rank;
		char is_open;
		char reserve_1;
		short reserve_2;

		struct RankList
		{
			RankList()
			{

			}

			void Reset()
			{
				uuid = 0;
				popularity = 0;
				memset(name,0,sizeof(GuildName));
			}

			long long uuid;
			int uid;
			int popularity;
			GameName name;
		};

		static const int RANK_LIST = 100;
		int rank_count;
		RankList rank_list[RANK_LIST];
	};

	class CSHotspringGivePresent				// 5300请求送礼物
	{
	public:
		CSHotspringGivePresent();
		MessageHeader	header;

		long long target_uuid;
		int present_id;
		short is_use_gold;						// 0不购买，1购买
		short is_uid;							// 0不是，1是
	};

	class CSHSAddPartnerReq						// 5301添加伙伴请求
	{
	public:
		CSHSAddPartnerReq();
		MessageHeader		header;

		ObjID 		obj_id;
		short		is_yi_jian;
	};

	class SCHSAddPartnerReqRoute				// 5354询问被添加伙伴的对象								
	{
	public:
		SCHSAddPartnerReqRoute();
		MessageHeader		header;

		GameName			req_gamename;
		long long  			req_user_id;
		char				req_avatar;
		char				req_sex;
		char				req_prof;
		char				req_camp;
		int					req_level;
		long long			req_avatar_timestamp;
	};

	class CSHSAddPartnerRet						// 5302被添加伙伴对象处理邀请伙伴请求
	{
	public:
		CSHSAddPartnerRet();
		MessageHeader		header;

		long long	 		req_user_id;
		GameName			req_gamename;
		char				is_accept;
		char				reserved;
		char				req_sex;
		char				req_prof;
	};

	class CSHSDeleteParter						// 5303取消伙伴请求							
	{
	public:
		CSHSDeleteParter();
		MessageHeader		header;
	};

	class SCHSSendPartnerInfo					// 5350发送伙伴信息
	{
	public:
		SCHSSendPartnerInfo();
		MessageHeader		header;

		long long	 partner_uuid;    // 伙伴id
		ObjID partner_obj_id;
	};

	enum AddPartnerStatus
	{
		ADDPARTNER_REJECT = 0,					// 拒绝
		ADDPARTNER_AGREE = 1,					// 同意
	};

	class SCHSAddExpInfo						// 5351 发送历史经验信息								
	{
	public:
		SCHSAddExpInfo();
		MessageHeader		header;

		//int add_exp_total;
		long long add_exp_total; 
		int add_addition;  
	};

	static const int HOTSPRINT_MSG_MAX_SHUANGXIU_COUNT = 512;			// 温泉双修对象最多通知的对数（一般用不了那么多，超过了可以扩展）

	class SCHSShuangxiuInfo						// 5355广播发送温泉双休信息
	{
	public:
		SCHSShuangxiuInfo();
		MessageHeader		header;

		ObjID role_1_obj_id;	
		ObjID role_1_partner_obj_id;
		ObjID role_2_obj_id;
		ObjID role_2_partner_obj_id;
	};

	class SCHSQARankInfo						// 5356 温泉答题排名信息
	{
	public:
		SCHSQARankInfo();
		MessageHeader		header;

		int self_score;
		int self_rank;
		char is_finish;
		char reserve_1;
		short reserve_2;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				uuid = 0;
				uid = 0;
				score = 0;
				memset(name,0,sizeof(GameName));
			}

			long long uuid;
			int uid;
			int score;
			GameName name;
		};

		static const int RANK_NUM = 100;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class SCHSQAQuestionBroadcast						// 5357发送答题内容
	{
	public:
		SCHSQAQuestionBroadcast();
		MessageHeader header;

		unsigned int curr_question_begin_time;			// 本题开始时间
		unsigned int curr_question_end_time;			// 本题结束时间
		unsigned int next_question_begin_time;			// 下一题开始时间

		short broadcast_question_total;					// 已广播题目总数
		short curr_question_id;							// 当前问题id
		int side;										// 客户端用于随机哪一边正确
		QuestionStr curr_question_str;					// 当前题目字符串
		QuestionStr curr_answer0_desc_str;				// 当前答案0字符串
		QuestionStr curr_answer1_desc_str;				// 当前答案1字符串
	};

	class CSHSQAFirstPosReq								// 5304请求榜首位置
	{
	public:
		CSHSQAFirstPosReq();
		MessageHeader header;

	};

	class SCHSQASendFirstPos							// 5358返回榜首位置
	{
	public:
		SCHSQASendFirstPos();
		MessageHeader header;

		ObjID obj_id;
		short reserve;
		int pos_x;
		int pos_y;
	};

	class CSHSQAAnswerReq								// 5305玩家答题请求
	{
	public:
		CSHSQAAnswerReq();
		MessageHeader header;

		short is_use_item;
		short choose;
	};

	class SCHSQAnswerResult								// 5359发送玩家回答结果						
	{
	public:
		SCHSQAnswerResult();
		MessageHeader header;

		int result;										// 1 回答正确，0 回答错误
		int right_result;								// 正确答案
	};

	class CSHSQAUseCardReq								// 5306玩家请求使用变身卡
	{
	public:
		CSHSQAUseCardReq();
		MessageHeader header;

		int target_uid;
	};

	class CSHSUseSkillReq								// 5360玩家请求使用技能
	{
	public:
		CSHSUseSkillReq();
		MessageHeader header;

		ObjID obj_id;
		short skill_type;
	};

	class SCHSNoticeSkillInfo							// 5361玩家使用技能对象信息
	{
	public:
		SCHSNoticeSkillInfo();
		MessageHeader header;

		ObjID use_obj_id;
		ObjID be_use_obj_id;
		short skill_type;
		short reserve;
	};

	/////////////////////////////////////  跨服仙盟战  /////////////////////////////////////
	class SCCrossGuildBattleSceneInfo			// 5373
	{
	public:
		struct FlagItem
		{
			int monster_id;
			int plat_type;
			int server_id;
			GuildName guild_name;
			Attribute cur_hp;
			Attribute max_hp;
		};

		struct RankItem
		{
			int plat_type;
			int server_id;
			GuildName guild_name;
			int score;
			int own_num;
		};

		SCCrossGuildBattleSceneInfo();
		MessageHeader		header;

		int scene_id;
		FlagItem flag_list[CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE];
		int guild_join_num_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
		int rank_list_count;
		RankItem rank_list[CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM];
	};

	enum SC_CROSS_GUILDBATTLE_INFO_TYPE
	{
		SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_SCORE = 0,			// 个人积分
		SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_END,				// 结束
		SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_SOS,				// 召集 param2 次数
		SC_CROSS_GUILDBATTLE_NOTIFY_TYPE_ENTER,				// 进入场景
	};

	class SCCrossGuildBattleNotifyInfo					// 5374
	{
	public:
		SCCrossGuildBattleNotifyInfo();
		MessageHeader		header;

		int notify_type;
		int param_1;
		int param_2;
	};

	class SCCrossGuildBattleInfo						// 5375
	{
	public:
		SCCrossGuildBattleInfo();
		MessageHeader header;

		int server_reward_flag;
		int guild_reward_flag;
		int role_reward_flag;
		int daily_reward_flag;
		struct GuildItem
		{
			int plat_type;
			int server_id;
			int guild_id;
			int is_our_guild;		// 是否本帮派占领: 1->是; 0->否
			GuildName guild_name;
			GameName guild_tuanzhang_name;
			char prof;
			char sex;
			ItemID wuqi_id;              // 武器ID
			char shizhuang_wuqi;          // 时装武器
			char shizhuang_wuqi_is_special;      // 时装武器
			char shizhuang_body;          // 时装身体
			char shizhuang_body_is_special;      // 时装身体
			short halo_used_imageid;        // 光环形象
			short toushi_used_imageid;        // 头饰形象id
			short qilinbi_used_imageid;        // 麒麟臂形象id
			short mask_used_imageid;        // 面具形象id
		};

		GuildItem guild_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	enum CROSS_GUILDBATTLE_OPERATE
	{
		CROSS_GUILDBATTLE_OPERATE_REQ_INFO,
		CROSS_GUILDBATTLE_OPERATE_FETCH_REWARD,
		CROSS_GUILDBATTLE_OPERATE_REQ_TASK_INFO,
		CROSS_GUILDBATTLE_OPERATE_BOSS_INFO,
		CROSS_GUILDBATTLE_OPERATE_SCENE_RANK_INFO,	// param1 场景id
		CROSS_GUILDBATTLE_OPERATE_GOTO_SCENE,		// param1 场景索引
		CROSS_GUILDBATTLE_OPERATE_GET_DAILY_REWARD, // 获取占领的每天奖励
	};

	class CSCrossGuildBattleOperate				// 5370
	{
	public:
		CSCrossGuildBattleOperate();
		MessageHeader header;

		int operate_type;
		int param1;
		int param2;
	};

	class CSCrossGuildBattleGetRankInfoReq		// 5371, 请求获得跨服仙盟战排行榜结果
	{
	public:
		CSCrossGuildBattleGetRankInfoReq();
		MessageHeader header;
	};

	class SCCrossGuildBattleGetRankInfoResp		// 5372, 返回获得跨服仙盟战排行榜结果
	{
	public:
		SCCrossGuildBattleGetRankInfoResp();
		MessageHeader header;

		enum SendType
		{
			ST_INVALID = 0,
			ST_ACT_END = 1,
			ST_ROLE_REQ = 2
		};

		int	send_type;	// 0. 无效; 1. 活动结束时下发; 2. 玩家主动请求
		CrossGuildBattleSceneLogInfo scene_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	class SCCrossGuildBattleTaskInfo			// 5376
	{
	public:
		SCCrossGuildBattleTaskInfo();
		MessageHeader		header;

		int task_finish_flag[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
		int task_record[CROSS_GUILDBATTLE_MAX_SCENE_NUM][CROSS_GUILDBATTLE_MAX_TASK_NUM];
	};

	class SCCrossGuildBattleBossInfo		// 5377
	{
	public:
		SCCrossGuildBattleBossInfo();
		MessageHeader header;

		static const int MAX_BOSS_COUNT = 100;
		int scene_id;
		char is_refresh;
		char reserve_ch;
		short reserve_sh;

		struct BossInfo
		{
			int boss_id;
			int replace_boss_id;
			int status;
			unsigned int next_refresh_time;
		};

		int count;
		BossInfo boss_list[MAX_BOSS_COUNT];
	};

	class SCCrossGuildBattleGetMonsterInfoResp		// 5379, 返回获得跨服仙盟战精英怪信息
	{
	public:
		SCCrossGuildBattleGetMonsterInfoResp();
		MessageHeader header;

		int scene_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	class SCCrossGuildBattleFlagInfo // 5378 跨服帮派战，所有旗帜信息
	{
	public:
		SCCrossGuildBattleFlagInfo();
		MessageHeader header;

		struct FlagItem
		{
			int monster_id;
			int plat_type;
			int server_id;
			GuildName guild_name;
			Attribute cur_hp;
			Attribute max_hp;
		};

		struct SceneFlagInfo
		{
			int scene_id;
			FlagItem flag_list[CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE];
		};

		SceneFlagInfo flag_info[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};

	class SCCrossGuildBattleBossHurtInfo	//5369 跨服帮派战，BOSS伤害信息
	{
	public:
		SCCrossGuildBattleBossHurtInfo();
		MessageHeader header;
		static const int BOSS_HURT_INFO_RANK_MAX = 6;
		struct HurtInfo
		{
			long long guild_id;
			GuildName guild_name;
			long long hurt;
		};

		int boss_id;
		int own_guild_rank;	//自己仙盟排名
		long long own_guild_hurt;//自己仙盟伤害
		int count;
		HurtInfo info_list[BOSS_HURT_INFO_RANK_MAX];
	};

	class SCCrossGuildBattleSceneGuilderNum    // 5368 跨服帮派战，场景盟友数量
	{
	public:
		SCCrossGuildBattleSceneGuilderNum();
		MessageHeader header;
		struct SceneInfo
		{
			SceneInfo():scene_id(0),guilder_num(0)
			{}
			int scene_id;
			int guilder_num;
		};

		long long guild_id;
		SceneInfo scene_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	};
	/////////////////////////////////////////////////  世界&公会答题  /////////////////////////////////////////////////////////
	class SCQuestionBroadcast						// 5380发送答题内容
	{
	public:
		SCQuestionBroadcast();
		MessageHeader header;

		short question_type;							// 题目类型 2 世界 3公会
		short curr_question_id;							// 当前问题id
		QuestionStr curr_question_str;					// 当前题目字符串
		QuestionStr curr_answer0_desc_str;				// 当前答案0字符串
		QuestionStr curr_answer1_desc_str;				// 当前答案1字符串
		QuestionStr curr_answer2_desc_str;				// 当前答案2字符串
		QuestionStr curr_answer3_desc_str;				// 当前答案3字符串

		unsigned int curr_question_begin_time;			// 本题开始时间
		unsigned int curr_question_end_time;			// 本题结束时间
		unsigned int next_question_begin_time;			// 下一题开始时间
	};

	class CSQuestionAnswerReq							// 5381玩家答题请求
	{
	public:
		CSQuestionAnswerReq();
		MessageHeader header;

		short answer_type;								// 回答类型 2世界 3公会
		short choose;									// 选择答案
	};

	class SCQuestionAnswerResult						// 5382发送玩家回答结果						
	{
	public:
		SCQuestionAnswerResult();
		MessageHeader header;

		short answer_type;								// 结果类型 2世界， 3公会
		short reserve;
		short result;									// 1 回答正确，0 回答错误
		short right_result;								// 正确答案
	};

	class SCQuestionGuildRankInfo						// 5383公会答题排名信息
	{
	public:
		SCQuestionGuildRankInfo();
		MessageHeader		header;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				uid = 0;
				right_answer_num = 0;
				memset(name, 0, sizeof(GameName));
			}

			int uid;
			int right_answer_num;
			GameName name;
		};

		static const int RANK_NUM = 100;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class SCQuestionRightAnswerNum						// 5384发送玩家回答结果						
	{
	public:
		SCQuestionRightAnswerNum();
		MessageHeader header;

		int world_right_answer_num;								
		int guild_right_answer_num;
	};

	/////////////////////////////////////////////////  仙盟战  /////////////////////////////////////////////////////////

	enum XIANMMENGZHAN_USER_INFO_NOTIFY_REASON
	{
		XIANMENGZHAN_USER_INFO_NOTIFY_REASON_DEFAULT = 0,								// 默认
		XIANMENGZHAN_USER_INFO_NOTIFY_REASON_CALL_SUCCESS,								// 盟主召唤成功
	};

	class SCXianMengZhanUserInfo
	{
	public:
		SCXianMengZhanUserInfo();
		MessageHeader		header;

		char                notify_reason;												// 通知原因
		char                call_count;													// 已召唤次数
		char                call_allow;													// 是否可召唤
		char                reserve_ch;													// 保留
		short               score;														// 积分
		short               kill_count;													// 击杀数
		unsigned int		last_realive_here_timestamp;								// 上次原地复活时间
		short				lianzhan;													// 连斩
		short				reserve_sh;													// 保留
		int					assist_count;												// 助攻数
	};

	class SCXianMengZhanGuildInfo
	{
	public:
		SCXianMengZhanGuildInfo();
		MessageHeader		header;

		int					score;														// 积分
		int                 area_index;													// 据点索引
		unsigned int        last_call_time;												// 上一次召唤时间
	};

	class SCXianMengZhanDefendAreaInfo
	{
	public:
		SCXianMengZhanDefendAreaInfo();
		MessageHeader		header;

		struct DefendAreaItem
		{
			GuildName       guild_name;													// 仙盟名字
			short           guild_score;												// 仙盟积分
			char            area_index;													// 据点索引
			char            has_center_area;											// 保留
			int				guild_id;													// 占领仙盟ID
			char			camp;														// 所属阵营
			char			reserve_ch;													// 保留
			short			reserve_sh;													// 保留
		};

		int					center_area_guild_id;										// 中央据点仙盟ID
		GuildName			center_area_guild_name;										// 中央据点仙盟名
		int					center_area_guild_score;									// 中心据点仙盟积分
		char				center_area_guild_camp;										// 所属阵营
		char				reserve_ch;													// 保留
		short				reserve_sh;													// 保留

		int                 area_count;													// 列表大小
		DefendAreaItem      defend_area_list[XIANMENGZHAN_DEFEND_AREA_NUM];             // 据点列表
	};

	class SCXianMengZhanGuildRankInfo
	{
	public:
		SCXianMengZhanGuildRankInfo();
		MessageHeader		header;

		const static int GUILD_RANK_ITEM_MAX_COUNT = 10;

		struct GuildRankItem
		{
			GuildName       guild_name;													// 仙盟名字
			int				guild_id;													// 占领仙盟ID
			int				guild_score;												// 仙盟积分
			char			camp;														// 阵营
			char			reserve_ch;													// 保留
			short			reserve_sh;													// 保留
		};

		int rank_count;
		GuildRankItem rank_item_list[GUILD_RANK_ITEM_MAX_COUNT];
	}; 

	class SCXianMengZhanDanyaoBossInfo
	{
	public:
		SCXianMengZhanDanyaoBossInfo();
		MessageHeader	   header;

		static const int   MAX_DANYAO_BOSS_NUM = 50;

		struct DanyaoBoss
		{
			unsigned short danyao_boss_obj_id;
			short		   reserved;
			int			   guild_id;
			GuildName	   guild_name;
		};

		int				   boss_count;
		DanyaoBoss		   danyao_boss_info_list[MAX_DANYAO_BOSS_NUM];
	};

	class SCXianMengZhanGuildCallNotice
	{
	public:
		SCXianMengZhanGuildCallNotice();
		MessageHeader       header;

		GameName            caller_name;												//盟主名称
	};

	class SCXianMengZhanDefendAreaBeAttackNotice
	{
	public:
		SCXianMengZhanDefendAreaBeAttackNotice();
		MessageHeader		header;
	};

	class SCXianMengZhanResult
	{
	public:
		SCXianMengZhanResult();
		MessageHeader		header;

		int					guild_rank;
		int					guild_score;
		int					kill_score;
		int					total_score;
		int					assist_count;
	};

	class CSXianMengZhanGuildCall
	{
	public:
		CSXianMengZhanGuildCall();
		MessageHeader		header;

		int                use_gold;												//是否使用元宝
	};

	class CSXianMengZhanGuildFollow
	{
	public:
		CSXianMengZhanGuildFollow();
		MessageHeader		header;
	};

	class SCXianmengzhanLianzhanChange												// 连斩数改变
	{
	public:
		SCXianmengzhanLianzhanChange();												
		MessageHeader		header;

		ObjID obj_id;																	// 对象id
		short lianzhan;																	// 连斩数
	};

	/////////////////////////////////////////////////  星座遗迹  /////////////////////////////////////////////////////////

	class SCXingzuoYijiChangeBoxAndBoss													// 宝箱总数和boss总数改变 4940
	{
	public:
		SCXingzuoYijiChangeBoxAndBoss();												
		MessageHeader		header;

		short now_box_num;																// 剩余宝箱数量
		short now_boss_num;																// boss数量												
		short gather_box_num[XingzuoYiJiConfig::MAX_GRADE_BOX];							// 4种已采集宝箱数量
		unsigned int next_box_refresh_time;												// 下次箱子刷新时间
		unsigned int next_boss_refresh_time;											// 下次boss刷新时间
		int can_gather_num;																// 剩余采集次数
	};

	/////////////////////////////////////////////////  充值活动  /////////////////////////////////////////////////////////
	enum ZAI_CHONGZHI_STATE
	{
		ZAI_CHONGZHI_STATE_INVALID = 0,													// 再充值 未达到要求
		ZAI_CHONGZHI_STATE_CAN_FETCH,													// 再充值 可领取
		ZAI_CHONGZHI_STATE_HAS_FETCH,													// 再充值 已领取

		ZAI_CHONGZHI_STATE_MAX,
	};

	class SCChongZhiInfo
	{
	public:
		SCChongZhiInfo();
		MessageHeader		header;

		long long history_chongzhi;
		int history_chongzhi_count;
		int today_chongzhi;
		int chongzhi_reward_fetch_flag;

		unsigned int special_first_chongzhi_timestamp;									// 特殊首冲开始时间戳
		char is_daily_first_chongzhi_open;												// 每日首冲是否开启
		char is_daily_first_chongzhi_fetch_reward;										// 每日充值奖励是否已经领取
		short daily_total_chongzhi_fetch_reward_flag;									// 每日累计充值奖励领取标记
		char daily_total_chongzhi_stage;												// 累计充值当前阶段
		char daily_first_chongzhi_times;												// 每日首冲累计次数(满7次给予额外奖励后置0)
		char special_first_chongzhi_fetch_reward_flag;									// 特殊首冲奖励领取标记
		char zai_chongzhi_reward_flag;													// 再充值状态（0 未充值，1 可领取，2 已领取）
		int daily_total_chongzhi_stage_chongzhi;										// 当前阶段累计充值数
		char third_chongzhi_reward_flag;												// 第三次充值状态（0 未充值，1 可领取，2 已领取）
		char diff_weekday_chongzhi_is_open;												// 每日累充是否开启	（星期几相关）					
		short diff_weekday_chongzhi_stage_fetch_flag;									// 每日累充阶级奖励领取标 （星期几相关）
		int diff_wd_chongzhi_value;														// 每日累充额度（星期几相关）
		short daily_chongzhi_value;														// 每日充值充值金额
		unsigned short first_chongzhi_active_reward_flag;								// 首充奖励激活标记
		unsigned short first_chongzhi_fetch_reward_flag;								// 首充奖励领取标记
		short daily_chongzhi_fetch_reward_flag;											// 每日首充奖励领取标记
		short daily_chongzhi_complete_days;												// 每日首充完成天数
		short daily_chongzhi_times_fetch_reward_flag;									// 每日首充累计天数奖励标记
		unsigned int chongzhi_7day_reward_timestamp;									// 充值-七天返利达成时间
		short chongzhi_7day_reward_fetch_day;											// 充值-七天返利领取天数
		short chongzhi_7day_reward_is_fetch;											// 充值-七天返利今天是否领取
		short daily_chongzhi_fetch_reward2_flag;										// 每日累计充值奖励2领取标记
	};

	enum CHONGZHI_REWARD_TYPE
	{
		CHONGZHI_REWARD_TYPE_SPECIAL_FIRST = 0,											// 特殊首充
		CHONGZHI_REWARD_TYPE_DAILY_FIRST = 1,											// 日常首充
		CHONGZHI_REWARD_TYPE_DAILY_TOTAL = 2,											// 日常累充
		CHONGZHI_REWARD_TYPE_DIFF_WEEKDAY_TOTAL = 3,									// 每日累充
		CHONGZHI_REWARD_TYPE_FIRST = 4,													// 首充
		CHONGZHI_REWARD_TYPE_DAILY = 5,													// 每日充值
		CHONGZHI_REWARD_TYPE_DAILY_TIMES = 6,											// 每日充值累计天数奖励
		CHONGZHI_REWARD_TYPE_DAILY2 = 7,												// 每日充值2

		CHONGZHI_REWARD_TYPE_MAX, 
	};

	class CSChongzhiFetchReward
	{
	public:
		CSChongzhiFetchReward();
		MessageHeader		header;

		int type;
		int param;
		int param2;
	};

	class CSZaiChongzhiFetchReward														// 再充值，领取奖励
	{
	public:
		CSZaiChongzhiFetchReward();
		MessageHeader		header;

		int is_third;	//是否第三次充值奖励
	};

	/////////////////////////////////////////////////  集字活动  /////////////////////////////////////////////////////////
	class SCCollectExchangeInfo
	{
	public:
		SCCollectExchangeInfo();
		MessageHeader		header;

		int collection_exchange_times[RAND_ACTIVITY_ITEM_COLLECTION_REWARD_MAX_COUNT];
	};

	/////////////////////////////////////////////////  我要变元宝活动  /////////////////////////////////////////////////////////
	class CSTotalChongzhiWantMoneyFetchInfo // 5262
	{
	public:
		CSTotalChongzhiWantMoneyFetchInfo();
		MessageHeader		header;
	};

	class SCTotalChongzhiWantMoneyFetchInfo // 5210
	{
	public:
		SCTotalChongzhiWantMoneyFetchInfo();
		MessageHeader		header;

		int reward_state;										// 当前要领的阶段
		long long history_chongzhi;								// 当前已充值数
	};

	class CSTotalChongzhiWantMoneyFetchReward // 5263
	{
	public:
		CSTotalChongzhiWantMoneyFetchReward();
		MessageHeader		header;
	};

	class SCTotalChongzhiWantMoneyFetchReward // 5211
	{
	public:
		SCTotalChongzhiWantMoneyFetchReward();
		MessageHeader		header;

		int get_gold_bind;										// 随机获得的元宝数量
	};

	/////////////////////////////////////////////////  聚宝盆  /////////////////////////////////////////////////////////
	enum RA_CORNUCOPIA_OPERA_TYPE
	{
		RA_CORNUCOPIA_OPERA_TYPE_QUERY_INFO = 0,
		RA_CORNUCOPIA_OPERA_TYPE_FETCH_REWARD,
		RA_CORNUCOPIA_OPERA_TYPE_FETCH_REWARD_INFO,

		RA_CORNUCOPIA_OPERA_TYPE_MAX,
	};

	class SCRACornucopiaFetchInfo								// 5212聚宝盆，获取当前状态信息
	{
	public:
		SCRACornucopiaFetchInfo();
		MessageHeader		header;

		int reward_lun;											// 当前是第几轮
		long long history_chongzhi;								// 当前已充值数

		int record_count;										// 聚宝盆记录个数
		RACornucopiaRewardRecord record_list[RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT];
	};

	class SCRACornucopiaFetchReward		// 5213聚宝盆，获取玩家当前随机出来的奖励
	{
	public:
		SCRACornucopiaFetchReward();
		MessageHeader		header;

		int get_reward_gold;									// 获得的奖励
	};

	class SCRANewCornucopiaInfo			// 5226新聚宝盆信息
	{
	public:
		SCRANewCornucopiaInfo();
		MessageHeader		header;

		int cornucopia_value;
		int total_cornucopia_value;
		short cornucopia_day_index;
		short cornucopia_total_reward_flag;
		unsigned char new_cornucopia_task_value_list[RA_NEW_CORNUCOPIA_TASK_TYPE_MAX];
	};

	/////////////////////////////////////////////////  巡游  /////////////////////////////////////////////////////////
	class CSBuyXunyouOpera										// 5270 请求购买巡游操作数量
	{
	public:
		CSBuyXunyouOpera();
		MessageHeader header;

		int type;												// 购买类型
	};

	class SCXunyouOperaInfo										// 5229 巡游操作信息
	{
	public:
		SCXunyouOperaInfo();
		MessageHeader header;

		char yanhua_buy_times;									// 婚宴烟花购买次数					
		char yanhua_use_times;									// 婚宴烟花使用次数
		char hongbao_buy_times;									// 婚宴红包购买次数
		char hongbao_use_times;									// 婚宴红包使用次数
		char meigui_buy_times;									// 婚宴玫瑰购买次数
		char meigui_use_times;									// 婚宴玫瑰使用次数

		short reserve_sh;										// 保留
	};

	/////////////////////////////////////  全服boss /////////////////////////////////////
	class CSNationalBossBuyBuffReq								// 全服Boss 购买buff
	{
	public:
		CSNationalBossBuyBuffReq();
		MessageHeader header;
	};

	class SCNationalBossUserInfo								// 4915 全服Boss 用户信息
	{
	public:
		SCNationalBossUserInfo();
		MessageHeader header;

		long long total_hurt;									// 总伤害
		long long total_bosshp;									// boss总血量
		char boss_flush_times;									// boss刷新次数
		char is_finish;											// 是否已经结束
		short buy_times;										// 购买次数
		short rank_pos;											// 排名
		short add_gongji_percent;								// 增加攻击百分比
		unsigned int next_flush_timestamp;						// 下次刷新时间
		int world_level;										// 世界等级
	};

	class SCNationalBossRankInfo								// 4916 全服Boss 排行榜信息
	{
	public:
		SCNationalBossRankInfo();
		MessageHeader		header;

		const static int RANK_ITEM_MAX_COUNT = 10;

		struct RankItem
		{
			RankItem() : uid(0), total_hurt(0)
			{
				memset(name, 0, sizeof(name));
			}

			int uid;											// 用户uid
			GameName name;										// 用户名
			long long total_hurt;								// 造成的总伤害
			char camp;											// 阵营
			char reserve_ch;
			short reserve_sh;
		};

		long long total_bosshp;									// 全服Boss boss总血量
		int rank_count;											// 全服Boss 排行榜信息数量
		RankItem rank_list[RANK_ITEM_MAX_COUNT];				// 全服Boss 排行榜信息列表
	};

	/////////////////////////////////////////////////  开服活动  /////////////////////////////////////////////////////////
	class CSOpenGameActivityInfoReq								// 请求开服活动信息
	{
	public:
		CSOpenGameActivityInfoReq();
		MessageHeader		header;
	};

	class CSOpenGameActivityFetchReward							// 开服活动 请求领奖
	{
	public:
		CSOpenGameActivityFetchReward();
		MessageHeader		header;

		enum REWARD_TYPE
		{
			REWARD_TYPE_PUTON_EQUIPMENT,						// 装备收集
			REWARD_TYPE_CAPABILITY,								// 战斗力冲刺
			REWARD_TYPE_ROLELEVEL,								// 人物等级冲刺

			REWARD_TYPE_MAX,
		};

		short reward_type;										// 奖励类型
		short reward_seq;										// 奖励索引
	};

	class SCOpenGameActivityInfo								// 开服活动信息
	{
	public:
		SCOpenGameActivityInfo();
		MessageHeader		header;

		int oga_capability_reward_flag;							// 开服活动 战斗力奖励领取标记
		int oga_rolelevel_reward_flag;							// 开服活动 等级奖励领取标记
		int oga_puton_equipment_reward_flag;					// 开服活动 装备收集奖励标记
		int oga_buy_equipmentgift_flag;							// 开服活动 购买装备礼包标记

		int total_chongzhi;										// 累计充值数
		int puton_equipment_act_flag;							// 装备收集激活标记

		unsigned short oga_putonequipment_fetch_times[OGA_PUTONEQUIPMENT_REWARD_MAX_SEQ];	// 全服收集装备领取奖励数量
		unsigned short oga_capability_fetch_times[OGA_CAPABILITY_REWARD_MAX_SEQ];			// 全服战力冲刺领取奖励数量
		unsigned short oga_rolelevel_fetch_times[OGA_ROLELEVEL_REWARD_MAX_SEQ];		// 全服等级冲刺领取奖励数量

		short oga_guild_battle_reward_type;						//帮派争霸奖励类型
		short oga_guild_battle_reward_flag;						//帮派争霸奖励标志
	};

	class CSTitleOwnerInfoReq									// 请求称号拥有者信息
	{
	public:
		CSTitleOwnerInfoReq();
		MessageHeader		header;
	};

	class SCTitleOwnerInfo										// 称号拥有者信息
	{
	public:
		SCTitleOwnerInfo();
		MessageHeader		header;

		int xianjiezhizhun_owner_uid;
		GameName xianjiezhizhun_owner_name;
		int junlintianxia_owner_uid;
		GameName junlintianxia_owner_name;
		int qingshihongyan_owner_uid;
		GameName qingshihongyan_owner_name;
		int fengliutitang_owner_uid;
		GameName fengliutitang_owner_name;
		int guosetianxiang_owner_uid;
		GameName guosetianxiang_owner_name;
		int kunlunzhanshen_owner_uid;
		GameName kunlunzhanshen_owner_name;
		int penglaizhanshen_owner_uid;
		GameName penglaizhanshen_owner_name;
		int cangqiongzhanshen_owner_uid;
		GameName cangqiongzhanshen_owner_name;
		int wangchengchengzhu_owner_uid;
		GameName wangchengchengzhu_owner_name;
		int zuiqiangxianmeng_owner_uid;
		GameName zuiqiangxianmeng_owner_name;
		int weizhencangqiong_onwer_uid;
		GameName weizhencangqiong_owner_name;
		int bosshunter_owner_uid;
		GameName bosshunter_owner_name;
		int tianxiawushuang_owner_uid;
		GameName tianxiawushuang_owner_name;
		int xiongbatianxia_owner_uid;
		GameName xiongbatianxia_owner_name;
	};

	/////////////////////////////////////////////////  1V1竞技场  /////////////////////////////////////////////////////////
	class SCChallengeFieldStatus								// 4917 场景用户信息
	{
	public:
		SCChallengeFieldStatus();
		MessageHeader		header;

		struct SideInfo
		{
			long long		role_id;
			ObjID			obj_id;
			short			level;
			GameName		name;
			char			camp;
			char			prof;
			char			avatar;
			char			sex;
			Attribute		hp;
			Attribute		max_hp;
			Attribute		mp;
			Attribute		max_mp;
			Attribute		speed;
			short			pos_x;
			short			pos_y;
			Dir				dir;
			Scalar			distance;
			int				capability;
			GuildID			guild_id;
			GuildName		guild_name;
			char			guild_post;
			char			reserve_ch;
			short			reserve_sh;
		};

		int					status;				// CHALLENGE_FIELD_STATE
		unsigned int		next_status_timestamp;
		SideInfo			side_info_list[CHALLENGE_FIELD_SIDE_MAX];
	};

	class CSChallengeFieldGetOpponentInfo						// 请求对手外观
	{
	public:
		CSChallengeFieldGetOpponentInfo();
		MessageHeader		header;

		short				is_get_top;
		short				reserve_sh;
	};

	class SCChallengeFieldOpponentInfo							//4931 对手外观信息 
	{
	public:
		SCChallengeFieldOpponentInfo();
		MessageHeader		header;

		struct OpponentInfo
		{
			long long user_id;
			char camp;
			char prof;
			char sex;
			char reserve_ch;
			int capability;
			int best_rank_break_level;

			GameName name;

			RoleAppearance appearance;
		};

		int opponent_count;
		OpponentInfo opponent_info_list[CHALLENGE_FIELD_OPPONENT_MAX];
	};

	class SCRoleShadowBossInfo
	{
	public:
		SCRoleShadowBossInfo();
		MessageHeader header;

		struct BossItem
		{
			int scene_id;
			int pos_x;
			int pos_y;
			ObjID obj_id;
			short reserve_sh;
			int role_id;

			SCChallengeFieldOpponentInfo::OpponentInfo role_info;
		};

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT,
			NOTIFY_REASON_REFRESH,

			NOTIFY_REASON_MAX,
		};

		int notify_reason;
		unsigned int next_refresh_timestamp;
		BossItem item_list[ROLE_BOSS_MAX_COUNT];
	};

	class CSChallengeFieldGetLuckyInfo							// 请求幸运列表
	{
	public:
		CSChallengeFieldGetLuckyInfo();
		MessageHeader		header;
	};

	class CSChallengeFieldFetchGuangHui							// 领取光辉
	{
	public:
		CSChallengeFieldFetchGuangHui();
		MessageHeader		header;
	};

	class CSChallengeFieldGetRankInfo							// 请求英雄榜
	{
	public:
		CSChallengeFieldGetRankInfo();
		MessageHeader		header;
	};

	class CSChallengeFieldGetUserInfo							// 获得用户信息和战报
	{
	public:
		CSChallengeFieldGetUserInfo();
		MessageHeader		header;
	};

	class CSChallengeFieldResetOpponentList						// 刷新挑战列表
	{
	public:
		CSChallengeFieldResetOpponentList();
		MessageHeader		header;
	};

	class CSChallengeFieldFightReq								// 挑战请求
	{
	public:
		CSChallengeFieldFightReq();
		MessageHeader		header;

		short opponent_index;
		char ignore_rank_pos;
		char auto_buy_jointimes;
		int rank_pos;
	};

	class CSChallengeFieldBuyJoinTimes							// 购买参与次数
	{
	public:
		CSChallengeFieldBuyJoinTimes();
		MessageHeader		header;
	};

	class CSChallengeFieldFetchJifenRewardReq					// 领取积分奖励
	{
	public:
		CSChallengeFieldFetchJifenRewardReq();
		MessageHeader		header;

		short reward_seq;
		short reserve_sh;
	};

	class CSChallengeFieldBuyBuff								// 购买buff
	{
	public:
		CSChallengeFieldBuyBuff();
		MessageHeader		header;
	};

	class SCChallengeFieldUserInfo								//4918 挑战列表信息和个人信息
	{
	public:
		SCChallengeFieldUserInfo();
		MessageHeader		header;

		struct  OpponentInfo
		{
			long long user_id;
			int rank_pos;
		};

		int rank_pos;
		int curr_opponent_idx;
		int join_times;
		int buy_join_times;
		int jifen;
		int jifen_reward_flag;
		int reward_guanghui;
		int reward_bind_gold;
		int liansheng;
		int buy_buff_times;
		int best_rank_pos;
		int free_day_times;

		struct RewardItem
		{
			ItemID item_id;
			short num;
		};
		RewardItem reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
		OpponentInfo opponent_userid_list[CHALLENGE_FIELD_OPPONENT_MAX];
	};

	class SCChallengeFieldOpponentRankPosChange					// 4919 排位变化通知
	{
	public:
		SCChallengeFieldOpponentRankPosChange();
		MessageHeader		header;

		long long user_id;
		int rank_pos;
	};

	class SCChallengeFieldReportInfo							// 战报
	{
	public:
		SCChallengeFieldReportInfo();
		MessageHeader		header;

		struct ReportItem
		{
			unsigned int		challenge_time;					// 时间
			long long			target_uid;						// 对方id
			GameName			target_name;					// 对方名称
			char				is_sponsor;						// 自己是否为挑战者
			char				is_win;							// 是否胜利
			short				reserve_sh;						// 保留
			unsigned short		old_rankpos;					// 竞技前排名
			unsigned short		new_rankpos;					// 竞技后排名
		};

		int report_count;
		ReportItem report_list[CHALLENGE_FIELD_REPORT_MAX_COUNT];
	};

	class SCChallengeFieldRankInfo								// 英雄榜
	{
	public:
		SCChallengeFieldRankInfo();
		MessageHeader		header;

		struct RankItem
		{
			RankItem() :user_id(0), capability(0) 
			{
				memset(user_name, 0, sizeof(user_name));
			}

			long long user_id;
			int capability;
			GameName user_name;	
			char sex;
			char is_robot;
			char prof;
			char reserved;
			int role_level;

			RoleAppearance appearance;
		};

		RankItem rank_list[CHALLENGE_FIELD_TOP_RANKER_ITEM_NUM];
	};

	class SCChallengeFieldLuckyPosInfo							// 幸运排名
	{
	public:
		SCChallengeFieldLuckyPosInfo();
		MessageHeader		header;

		struct LuckyInfoItem
		{
			LuckyInfoItem() : rank_pos(0), user_id(0) 
			{
				memset(user_name, 0, sizeof(user_name));
			}

			int rank_pos;
			int user_id;
			GameName user_name;
		};

		int lucky_rankpos_list[CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT];
		LuckyInfoItem lucky_info_list[CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT];
	};

	class SCChallengeFieldBeDefeatNotice
	{
	public:
		SCChallengeFieldBeDefeatNotice();
		MessageHeader		header;
	};

	class SCChallengeFieldRewardGoldNotice
	{
	public:
		SCChallengeFieldRewardGoldNotice();
		MessageHeader		header;

		int reward_bind_gold;
	};

	class SCChallengeFieldWin			// 1V1竞技场直接胜利 4941
	{
	public:
		SCChallengeFieldWin();
		MessageHeader		header;

		short old_rank_pos;
		short new_rank_pos;
	};

	enum CFBreakType											// 竞技场历史最高突破请求
	{
		CF_BREAK_TYPE_INFO = 0,									// 0 请求信息
		CF_BREAK_TYPE_BREAK = 1,								// 1 请求突破
	};

	class CSChallengeFieldBestRankBreakReq						// 1V1竞技场历史最高突破请求 4942
	{
	public:
		CSChallengeFieldBestRankBreakReq();
		MessageHeader		header;

		int op_type;											// 0 请求信息 1 请求突破

	};

	class SCChallengeFieldBestRankBreakInfo						// 1V1竞技场历史最高突破信息 4943
	{
	public:
		SCChallengeFieldBestRankBreakInfo();
		MessageHeader		header;

		int best_rank_break_level;
		int best_rank_pos;
	};

	class CSChallengeFieldReadyStartFightReq					// 1V1竞技场请求准备正式开始战斗倒计时 4944
	{
	public:
		CSChallengeFieldReadyStartFightReq();
		MessageHeader		header;
	};

	/////////////////////////////////////  封测活动 /////////////////////////////////////

	enum CBA_OPERA_TYPE											// 封测活动请求操作类型
	{
		CBA_OPERA_TYPE_INVALID = 0,
		CBA_OPERA_TYPE_REQ_INFO,								// 请求活动信息
		CBA_OPERA_TYPE_FETCH_LOING_REWARD,						// 领取登录奖励
		CBA_OPERA_TYPE_FETCH_GUILD_REWARD,						// 领取仙盟奖励
		CBA_OPERA_TYPE_FETCH_MARRY_REWARD,						// 领取结婚奖励
		CBA_OPERA_TYPE_FETCH_ONLINE_REWARD,						// 领取在线奖励
		CBA_OPERA_TYPE_FETCH_UPLEVEL_REWARD,					// 领取升级奖励
		CBA_OPERA_TYPE_FETCH_JOIN_ACTIVITY_REWARD,				// 领取参与活动奖励

		CBA_OPERA_TYPE_MAX,
	};

	class CSCloseBetaActivityOperaReq							// 封测活动请求操作
	{
	public:
		CSCloseBetaActivityOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
		short param_2;
		short param_3;
	};

	class SCCloseBetaActivityInfo
	{
	public:
		SCCloseBetaActivityInfo();								// 封测活动信息
		MessageHeader header;

		char has_fetch_login_reward;							// 是否已经领取当日登录奖励
		char has_fetch_guild_reward;							// 是否已经领取仙盟奖励
		char has_fetch_marry_reward;							// 是否已经领取了结婚奖励
		char fetch_online_reward_flag;							// 领取在线时长奖励标记
		int fetch_uplevel_reward_flag;							// 升级奖励领取标记
		int join_activity_flag;									// 参与活动标记
		int fetch_activity_reward_flag;							// 领取活动奖励标记
		int expfb_satisfy_reward_cond_flag;						// 经验本满足奖励条件标记
		int expfb_fetch_reward_flag;							// 经验本领取奖励标记
		int equipfb_satisfy_reward_cond_flag;					// 装备本满足奖励条件标记
		int equipfb_fetch_reward_flag;							// 装备本领取奖励标记
		int tdfb_satisfy_reward_cond_flag;						// 塔防本满足奖励条件标记
		int tdfb_fetch_reward_flag;								// 塔防本领取奖励标记
		int challengefb_satisfy_reward_cond_flag;				// 挑战本满足奖励条件标记
		int challengefb_fetch_reeward_flag;						// 挑战本领取奖励标记
		int total_login_days;									// 总共登录天数
		int online_time;										// 在线时长
	};

	enum RA_LUCKY_ROLL_OPERA_TYPE
	{
		RA_LUCKY_ROLL_OPERA_TYPE_QUERY_INFO = 0,
		RA_LUCKY_ROLL_OPERA_TYPE_ROLL,
		RA_LUCKY_ROLL_OPERA_TYPE_FETCH_EXTRA_REWARD,
		RA_LUCKY_ROLL_OPERA_TYPE_10_COUNT_ROLL,					//10连抽

		RA_LUCKY_ROLL_OPERA_TYPE_MAX,
	};

	class CSLuckyRollActivityOperaReq							// 4969幸运转盘活动
	{
	public:
		CSLuckyRollActivityOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
	};

	class SCLuckyRollActivityInfo								// 4934幸运转盘活动信息
	{
	public:
		SCLuckyRollActivityInfo();
		MessageHeader header;

		int roll_times;
		int gold_poll;
		int record_count;
		LuckyRollRewardRecord record_list[LUCKY_ROLL_REWARD_RECORD_MAX_COUNT];
	};

	class SCLuckyRollActivityRollResult							// 4935幸运转盘抽奖结果
	{
	public:
		SCLuckyRollActivityRollResult();
		MessageHeader header;

		short reward_index;
		short reserve_sh;
	};

	class CSUpdateNoticeFetchReward
	{
	public:
		CSUpdateNoticeFetchReward();
		MessageHeader header;
	};

	class SCUpdateNoticeInfo
	{
	public:
		SCUpdateNoticeInfo();
		MessageHeader header;

		int server_version;
		int fetch_reward_version;
	};

	class SCRAChongzhiRankInfo
	{
	public:
		SCRAChongzhiRankInfo();
		MessageHeader header;

		int chongzhi_num;
	};

	class SCRAChongzhiRank2Info		//8982
	{
	public:
		SCRAChongzhiRank2Info();
		MessageHeader header;

		int chongzhi_num;
	};

	class SCRAConsumeGoldRankInfo
	{
	public:
		SCRAConsumeGoldRankInfo();
		MessageHeader header;

		int consume_gold_num;
	};

	class SCRAConsumeGoldFanliInfo
	{
	public:
		SCRAConsumeGoldFanliInfo();
		MessageHeader header;

		int consume_gold;
	};

	class SCRAProfessRankInfo		// 2464 表白排行活动信息
	{
	public:
		SCRAProfessRankInfo();
		MessageHeader header;

		int rank;															// 表白榜排名
		int ra_profess_score;												// 表白排行榜活动 表白积分
		int ra_profess_to_num;												// 表白排行榜活动 表白次数
		int ra_profess_from_num;											// 表白排行榜活动 被表白次数
	};

	/////////////////////////////////////  通用活动 /////////////////////////////////////

	class CSCommonActivityFetchReward
	{
	public:
		CSCommonActivityFetchReward();
		MessageHeader		header;

		int unit_type;
		int param;
		int	param2;
	};

	class SCCommonActivityOpenUnitInfo
	{
	public:
		SCCommonActivityOpenUnitInfo();
		MessageHeader		header;

		struct UnitInfo
		{
			int unit_type;
			int param1;
			int param2;
			long long reward_state;
		};

		int unit_num;
		UnitInfo unit_info_list[COMMON_ACTIVITY_UNIT_MAX_NUM];		
	};

	class CSRandActivityOperaReq  // 2257
	{
	public:
		CSRandActivityOperaReq();
		MessageHeader header;

		short rand_activity_type;
		short opera_type;
		int param_1;
		int param_2;
	};
	/////////////////////////////////////  随机活动 全服疯抢 /////////////////////////////////////
	enum RA_SERVER_PANIC_BUY_OPERA_TYPE
	{
		RA_SERVER_PANIC_BUY_OPERA_TYPE_QUERY_INFO = 0,
		RA_SERVER_PANIC_BUY_OPERA_TYPE_BUY_ITEM,

		RA_SERVER_PANIC_BUY_OPERA_TYPE_MAX,
	};

	class SCRAServerPanicBuyInfo								 // 全服疯抢活动信息
	{
	public:
		SCRAServerPanicBuyInfo();
		MessageHeader header;

		int user_buy_numlist[RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT];
		int server_buy_numlist[RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT];
	};

	/////////////////////////////////////  随机活动 个人疯抢 /////////////////////////////////////
	enum RA_PERSONAL_PANIC_BUY_OPERA_TYPE
	{
		RA_PERSONAL_PANIC_BUY_OPERA_TYPE_QUERY_INFO = 0,
		RA_PERSONAL_PANIC_BUY_OPERA_TYPE_BUY_ITEM,

		RA_PERSONAL_PANIC_BUY_OPERA_TYPE_MAX,
	};

	class SCRAPersonalPanicBuyInfo								// 个人疯抢活动信息
	{
	public:
		SCRAPersonalPanicBuyInfo();
		MessageHeader header;

		int buy_numlist[RAND_ACTIVITY_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT];
	};

	/////////////////////////////////////  随机活动 充值返利活动 /////////////////////////////////////
	enum RA_DAY_CHONGZHI_FANLI_OPERA_TYPE
	{
		RA_DAY_CHONGZHI_FANLI_OPERA_TYPE_QUERY_INFO = 0,
		RA_DAY_CHONGZHI_FANLI_OPERA_TYPE_FETCH_REWARD,

		RA_DAY_CHONGZHI_FANLI_OPERA_TYPE_MAX,
	};

	class SCRADayChongZhiFanLiInfo								// 充值返利领奖结果
	{
	public:
		SCRADayChongZhiFanLiInfo();
		MessageHeader header;

		int chongzhi_gold;										//充值金额
		int fetch_reward_flag;									//领取信息
	};

	/////////////////////////////////////  随机活动 每日消费活动 /////////////////////////////////////
	enum RA_DAY_CONSUME_GOLD_OPERA_TYPE
	{
		RA_DAY_CONSUME_GOLD_TYPE_QUERY_INFO = 0,
		RA_DAY_CONSUME_GOLD_TYPE_FETCH_REWARD,

		RA_DAY_CONSUME_GOLD_OPERA_TYPE_MAX,
	};

	class SCRADayConsumeGoldInfo								// 每日消费领奖结果
	{
	public:
		SCRADayConsumeGoldInfo();
		MessageHeader header;

		int consume_gold;									    //花费金额
		int fetch_reward_flag;									//领取信息
	};

	/////////////////////////////////////  随机活动 奇珍异宝 /////////////////////////////////////
	enum RA_CHESTSHOP_OPERA_TYPE
	{
		RA_CHESTSHOP_OPERA_TYPE_QUERY_INFO = 0,
		RA_CHESTSHOP_OPERA_TYPE_FETCH_REWARD,

		RA_CHESTSHOP_OPERA_TYPE_MAX,
	};

	class SCRAChestshopInfo									
	{
	public:
		SCRAChestshopInfo();
		MessageHeader header;

		short chestshop_times;
		short fetch_reward_flag;
	};

	/////////////////////////////////////  随机活动 宝石升级 /////////////////////////////////////
	enum RA_STONE_UPLEVEL_OPERA_TYPE
	{
		RA_STONE_UPLEVEL_OPERA_TYPE_QUERY_INFO = 0,
		RA_STONE_UPLEVEL_OPERA_TYPE_FETCH_REWARD,

		RA_STONE_UPLEVEL_OPERA_TYPE_MAX,
	};

	class SCRAStoneUplevelInfo									// 宝石升级信息
	{
	public:
		SCRAStoneUplevelInfo();
		MessageHeader header;

		int total_level;
		long long can_fetch_reward_flag;
		long long fetch_reward_flag;
	};

	/////////////////////////////////////  随机活动 仙女缠绵 /////////////////////////////////////
	enum RA_XIANNV_CHANMIAN_UPLEVEL_OPERA_TYPE
	{
		RA_XIANNV_CHANMIAN_UPLEVEL_OPERA_TYPE_QUERY_INFO = 0,
		RA_XIANNV_CHANMIAN_UPLEVEL_OPERA_TYPE_FETCH_REWARD,

		RA_XIANNV_CHANMIAN_UPLEVEL_OPERA_TYPE_MAX,
	};

	class SCRAXiannvChanmianUplevelInfo
	{
	public:
		SCRAXiannvChanmianUplevelInfo();
		MessageHeader header;

		int chanmian_level;
		long long can_fetch_reward_flag;
		long long fetch_reward_flag;
	};

	/////////////////////////////////////  随机活动 坐骑进阶 /////////////////////////////////////
	enum RA_MOUNT_UPGRADE_OPERA_TYPE
	{
		RA_MOUNT_UPGRADE_OPERA_TYPE_QUERY_INFO = 0,
		RA_MOUNT_UPGRADE_OPERA_TYPE_FETCH_REWARD,

		RA_MOUNT_UPGRADE_OPERA_TYPE_MAX,
	};

	class SCRAMountUpgradeInfo
	{
	public:
		SCRAMountUpgradeInfo();
		MessageHeader header;

		int mount_grade;
		short can_fetch_reward_flag;
		short fetch_reward_flag;
	};

	/////////////////////////////////////  随机活动 累计消费活动 /////////////////////////////////////
	enum RA_TOTAL_CONSUME_GOLD_OPERA_TYPE
	{
		RA_TOTAL_CONSUME_GOLD_TYPE_QUERY_INFO = 0,
		RA_TOTAL_CONSUME_GOLD_TYPE_FETCH_REWARD,

		RA_TOTAL_CONSUME_GOLD_OPERA_TYPE_MAX,
	};

	class SCRATotalConsumeGoldInfo								// 累计消费领奖结果
	{
	public:
		SCRATotalConsumeGoldInfo();
		MessageHeader header;

		int consume_gold;										//花费金额
		int fetch_reward_flag;									//领取信息
	};

	/////////////////////////////////////  随机活动 活跃奖励活动 /////////////////////////////////////
	enum RA_DAY_ACTIVE_DEGREE_OPERA_TYPE
	{
		RA_DAY_ACTIVE_DEGREE_OPERA_TYPE_QUERY_INFO = 0,
		RA_DAY_ACTIVE_DEGREE_OPERA_TYPE_FETCH_REWARD,

		RA_DAY_ACTIVE_DEGREE_OPERA_TYPE_TYPE_MAX,
	};

	class SCRADayActiveDegreeInfo								// 每日活跃度信息
	{
	public:
		SCRADayActiveDegreeInfo();
		MessageHeader header;

		int active_degree;										//活跃度
		int fetch_reward_flag;									//领取信息
	};

	/////////////////////////////////////  随机活动 骑兵进阶 /////////////////////////////////////
	enum RA_QIBING_UPGRADE_OPERA_TYPE
	{
		RA_QIBING_UPGRADE_OPERA_TYPE_QUERY_INFO = 0,
		RA_QIBING_UPGRADE_OPERA_TYPE_FETCH_REWARD,

		RA_QIBING_UPGRADE_OPERA_TYPE_MAX,
	};

	class SCRAQibingUpgradeInfo
	{
	public:
		SCRAQibingUpgradeInfo();
		MessageHeader header;
		
		int qibing_grade;
		short can_fetch_reward_flag;
		short fetch_reward_flag;
	};

	/////////////////////////////////////  随机活动 根骨全身等级 /////////////////////////////////////
	enum RA_MENTALITY_TOTAL_LEVEL_OPERA_TYPE
	{
		RA_MENTALITY_TOTAL_LEVEL_OPERA_TYPE_QUERY_INFO = 0,
		RA_MENTALITY_TOTAL_LEVEL_OPERA_TYPE_FETCH_REWARD,

		RA_MENTALITY_TOTAL_LEVEL_OPERA_TYPE_MAX,
	};

	class SCRAMentalityUplevelInfo
	{
	public:
		SCRAMentalityUplevelInfo();
		MessageHeader header;
		
		int total_mentality_level;
		short can_fetch_reward_flag;
		short fetch_reward_flag;
	};

	/////////////////////////////////////  随机活动 羽翼进化 /////////////////////////////////////
	enum RA_WING_UPGRADE_OPERA_TYPE
	{
		RA_WING_UPGRADE_OPERA_TYPE_QUERY_INFO = 0,
		RA_WING_UPGRADE_OPERA_TYPE_FETCH_REWARD,

		RA_WING_UPGRADE_OPERA_TYPE_MAX,
	};

	class SCRAWingUpgradeInfo
	{
	public:
		SCRAWingUpgradeInfo();
		MessageHeader header;
		
		int jinghua;
		long long can_fetch_reward_flag;
		long long fetch_reward_flag;
	};

	/////////////////////////////////////  随机活动 击杀boss活动 /////////////////////////////////////
	enum RA_KILL_BOSS_OPERA_TYPE
	{
		RA_KILL_BOSS_OPERA_TYPE_QUERY_INFO = 0,
		RA_KILL_BOSS_OPERA_TYPE_FETCH_REWARD,

		RA_KILL_BOSS_OPERA_TYPE_TYPE_MAX,
	};

	class SCRAKillBossInfo										// 击杀boss活动
	{
	public:
		SCRAKillBossInfo();
		MessageHeader header;

		int kill_count;											//击杀数量
	};

	/////////////////////////////////////  随机活动 全民祈福 /////////////////////////////////////
	enum RA_QUANMIN_QIFU_OPERA_TYPE
	{
		RA_QUANMIN_QIFU_OPERA_TYPE_QUERY_INFO = 0,
		RA_QUANMIN_QIFU_OPERA_TYPE_FETCH_REWARD,

		RA_QUANMIN_QIFU_OPERA_TYPE_MAX,
	};

	class SCRAQuanminQifuInfo
	{
	public:
		SCRAQuanminQifuInfo();
		MessageHeader header;

		short qifu_times;
		short qifu_fetch_flag;
	};

	/////////////////////////////////////  登录送礼 全民祈福 /////////////////////////////////////
	enum RA_LOGIN_GIFT_OPERA_TYPE
	{
		RA_LOGIN_GIFT_OPERA_TYPE_INFO = 0,
		RA_LOGIN_GIFT_OPERA_TYPE_FETCH_COMMON_REWARD,
		RA_LOGIN_GIFT_OPERA_TYPE_FETCH_VIP_REWARD,
		RA_LOGIN_GIFT_OPERA_TYPE_FETCH_ACCUMULATE_REWARD,

		RA_LOGIN_GIFT_OPERA_TYPE_MAX,
	};

	class SCRALoginGiftInfo
	{
	public:
		SCRALoginGiftInfo();
		MessageHeader header;

		short login_days;
		char has_login;
		char has_fetch_accumulate_reward;
		int fetch_common_reward_flag;
		int fetch_vip_reward_flag;
	};

	class SCRALoginGiftInfo_0    // 8819
	{
	public:
		SCRALoginGiftInfo_0();
		MessageHeader header;

		short login_days;
		char has_login;
		char has_fetch_accumulate_reward;
		int fetch_common_reward_flag;
		int fetch_vip_reward_flag;
	};

	enum EVERYDAY_NICE_GIFT_REQ
	{
		EVERYDAY_NICE_GIFT_SEND_INFO,				//请求消息
		EVERYDAY_NICE_GIFT_FETCH_REWARD,			//领取奖励
	};

	struct SCRAEverydayNiceGiftInfo	// 每日好礼 8820
	{
		SCRAEverydayNiceGiftInfo();
		MessageHeader header;

		short is_active;								// 是否已激活
		short reserver_sh;
		int everyday_recharge;							// 今日充值数量
		int can_fetch_reward_flag;						// 可以领取奖励标记
		int have_fetch_reward_flag;						// 已经领取奖励标记
	};

	/////////////////////////////////////  随机活动 手有余香 /////////////////////////////////////
	enum RA_SHOUYOU_YUXIANG_OPERA_TYPE
	{
		RA_SHOUYOU_YUXIANG_OPERA_TYPE_QUERY_INFO = 0,
		RA_SHOUYOU_YUXIANG_OPERA_TYPE_FETCH_REWARD,

		RA_SHOUYOU_YUXIANG_OPERA_TYPE_MAX,
	};

	class SCRAShouYouYuXiangInfo
	{
	public:
		SCRAShouYouYuXiangInfo();
		MessageHeader header;

		short shouyou_yuxiang_fetch_flag;
		short shouyou_yuxiang_give_flower_flag;
		int shouyou_yuxiang_flower_num;
	};

	/////////////////////////////////////  随机活动 战力比拼  ///////////////////////////////
	enum RA_BIPIN_CAPABILITY_OPERA_TYPE
	{
		RA_BIPIN_CAPABILITY_OPERA_TYPE_QUERY_INFO = 0,
		RA_BIPIN_CAPABILITY_OPERA_TYPE_FETCH_REWARD,

		RA_BIPIN_CAPABILITY_OPERA_TYPE_MAX,
	};

	struct TopUserInfo
	{
		TopUserInfo() { this->Reset(); }

		void Reset()
		{
			uid = 0; capability = 0;
			memset(name, 0, sizeof(name));
			prof = 0;
			reserve_ch = 0;
			reserve_sh = 0;
			avatar_timestamp = 0;
		}

		int uid;
		GameName name;
		long long capability;
		char prof;
		char reserve_ch;
		short reserve_sh;
		long long avatar_timestamp;
	};

	static const int TOP_COUNT = 3;

	class SCRABipinCapabilityInfo
	{
	public:
		SCRABipinCapabilityInfo();
		MessageHeader header;

		int bipin_activity_type;
		int capability;
		int fetch_reward_flag;

		TopUserInfo top_user_info_list[TOP_COUNT];	// 三个阵营里排名第一的信息
	};

	/////////////////////////////////////  随机活动 仙盟比拼 /////////////////////////////////////

	class SCRAXianMengBiPinInfo
	{
	public:
		SCRAXianMengBiPinInfo();
		MessageHeader header;

		int xianmeng_bipin_kill_boss_count;
	};

	/////////////////////////////////////  随机活动 仙盟崛起 /////////////////////////////////////

	class SCRAXianMengJueQiInfo
	{
	public:
		SCRAXianMengJueQiInfo();
		MessageHeader header;

		int xianmeng_jueqi_increase_capability;
	};

	/////////////////////////////////////  随机活动 单笔充值 /////////////////////////////////////
	enum RA_DANBI_CHONGZHI_OPERA_TYPE
	{
		RA_DANBI_CHONGZHI_OPERA_TYPE_QUERY_INFO = 0,
		RA_DANBI_CHONGZHI_OPERA_TYPE_FETCH_REWARD,

		RA_DANBI_CHONGZHI_OPERA_TYPE_MAX,
	};

	class SCRADanbiChongzhiInfo             // 2241
	{
	public:
		SCRADanbiChongzhiInfo();
		MessageHeader header;

		int reward_can_fetch_flag;
		int reward_fetch_flag;
	};

	/////////////////////////////////////  随机活动 每日充值排行 /////////////////////////////////////
	class SCRADayChongzhiRankInfo
	{
	public:
		SCRADayChongzhiRankInfo();
		MessageHeader header;

		int gold_num;
	};

	/////////////////////////////////////  随机活动 每日消费排行 /////////////////////////////////////
	class SCRADayConsumeRankInfo
	{
	public:
		SCRADayConsumeRankInfo();
		MessageHeader header;

		int gold_num;
	};
	
	/////////////////////////////////////////    一站到底     /////////////////////////////////////
	class SCYiZhanDaoDiRankInfo															// 一站到底 排行榜信息
	{
	public:
		SCYiZhanDaoDiRankInfo();														// 2231
		MessageHeader		header;

		struct RankItem
		{
			RankItem() : uid(0), jisha_count(0), dead_count(0)
			{
				memset(user_name, 0, sizeof(user_name));
			}

			int uid;
			GameName user_name;
			short jisha_count;
			short dead_count;
		};
	
		short self_jisha_count;
		short self_rand_pos;
		int count;																		// 一站到底 排行榜信息数量
		RankItem rank_list[YIZHANDAODI_RANK_NUM];										// 一站到底 排行榜信息列表
	};

	enum KICKOUT_BATTLE_FIELD_REASON
	{
		KICKOUT_BATTLE_FIELD_REASON_INVALID = 0,

		KICKOUT_BATTLE_FIELD_REASON_DEAD_ISOUT = 1,										// 复活次数没了
		KICKOUT_BATTLE_FIELD_REASON_TIMEOUT = 2,										// 战场时间到了

		KICKOUT_BATTLE_FIELD_REASON_MAX,
	};

	class SCYiZhanDaoDiKickout
	{
	public:
		SCYiZhanDaoDiKickout();															// 2227
		MessageHeader		header;	

		int reason;

		int jisha_count;
		int dead_count;
		int jisha_score;

		int lucky_count;
		int rank_pos;																	// 包括21名以后的名次统一显示为21名 告诉玩家 在20名以后 即可
	};

	class SCYiZhanDaoDiLuckyInfo
	{
	public:
		SCYiZhanDaoDiLuckyInfo();														// 2228
		MessageHeader		header;

		unsigned int next_lucky_timestamp;

		int luck_user_count;
		GameName luck_user_namelist[YIZHANDAODI_LUACY_ROLE_MAX];
	};

	class SCYiZhanDaoDiTitleChange
	{
	public:
		SCYiZhanDaoDiTitleChange();														// 2229
		MessageHeader		header;

		int					uid;
		ObjID				obj_id;
		short				title_id;
		int					jisha_count;
	};

	enum YIZHANDAODI_GUWU_TYPE
	{
		YIZHANDAODI_GUWU_TYPE_INVALID = 0,

		YIZHANDAODI_GUWU_TYPE_GONGJI,
		YIZHANDAODI_GUWU_TYPE_MAXHP,

		YIZHANDAODI_GUWU_TYPE_MAX,
	};

	class CSYiZhanDaoDiGuwu
	{
	public:
		CSYiZhanDaoDiGuwu();															// 2258
		MessageHeader		header;

		int guwu_type;
	};

	class SCYiZhanDaoDiUserInfo
	{
	public:
		SCYiZhanDaoDiUserInfo();	     // 2230
		MessageHeader		header;

		struct RewardItem
		{
			RewardItem() : item_id(0), num(0), is_bind(0) {}
			int item_id;
			short num;
			short is_bind;
		};

		int jisha_count;
		int jisha_title_id;
		int dead_count;
		int jisha_score;

		int gongji_guwu_per;
		int maxhp_guwu_per;
		int kill_num_reward_flag;

		short show_jiesuan;
		short reward_count;
		const static int YZDD_REWARD_MAX_COUNT = 8;
		
		RewardItem kill_num_reward_list[YZDD_REWARD_MAX_COUNT];
	};

	class SCYiZhanDaoDiLastFirstInfo
	{
	public:
		SCYiZhanDaoDiLastFirstInfo();
		MessageHeader		header;

		int uid;
		GameName game_name;
	};

	/////////////////////////////////////  怪物入侵活动 /////////////////////////////////////
	class SCMonsterInvadeTimeNotice
	{
	public:
		SCMonsterInvadeTimeNotice();		// 2226
		MessageHeader header;

		unsigned int next_monster_invade_time;
	};

	/////////////////////////////////////  大富豪活动 /////////////////////////////////////
	class SCMillionaireTimeNotice
	{
	public:
		SCMillionaireTimeNotice();		// 2249
		MessageHeader header;

		unsigned int next_millionaire_box_refresh_time;
		unsigned int next_millionaire_boss_refresh_time;
	};

	class CSMillionaireUseFrozenSkill
	{
	public:
		CSMillionaireUseFrozenSkill();		
		MessageHeader header;

		ObjID target_obj_id;
		short reserver;
	};
	/////////////////////////////////////  五行挂机活动 /////////////////////////////////////
	class CSWuXingGuaJiGetReward
	{
	public:
		CSWuXingGuaJiGetReward();		
		MessageHeader header;

		int target_idx;
	};

	class SCWuXingGuaJiLuckyInfo
	{
	public:
		SCWuXingGuaJiLuckyInfo();														// 2228
		MessageHeader		header;

		unsigned int next_lucky_timestamp;

		int luck_user_count;
		GameName luck_user_namelist[WUXINGGUAJI_LUACY_ROLE_MAX];
	};

	class SCWuXingGuaJiInfo
	{
	public:
		SCWuXingGuaJiInfo();		
		MessageHeader header;

		enum WUXINGGUAJI_REASON
		{
			WUXINGGUAJI_REASON_INVALID,

			WUXINGGUAJI_REASON_ENTER,
			WUXINGGUAJI_REASON_BOSS_DEAD,
			WUXINGGUAJI_REASON_GET_ITEM,
			WUXINGGUAJI_REASON_FETCH_REWARD,
			WUXINGGUAJI_REASON_BOSS_REFRESH,

			WUXINGGUAJI_REASON_MAX,
		};

		int reason;

		struct BossInfo
		{
			int monster_id;
			unsigned int next_boss_flush_time;
		};
		BossInfo boss_list[WUXINGGUAJI_BOSS_NUM];

		int my_lucky_count;
		WuXingGuaJiTargetInfo target_list[WUXINGGUAJI_TARGET_MAX];
	};

	class SCGuajiBossBornNotice
	{
	public:
		SCGuajiBossBornNotice();														
		MessageHeader		header;

		int boss_id;
		int scene_id;
		int pos_x;
		int pos_y;
	};

	/////////////////////////////////////  充值回馈 /////////////////////////////////////
	enum RA_CHARGE_REPAYMENT_OPERA_TYPE
	{
		RA_CHARGE_REPAYMENT_OPERA_TYPE_QUERY_INFO = 0,
		RA_CHARGE_REPAYMENT_OPERA_TYPE_FETCH_REWARD,

		RA_CHARGE_REPAYMENT_OPERA_TYPE_MAX,
	};
	class SCChargeRewardInfo // 2236
	{
	public:
		SCChargeRewardInfo();														
		MessageHeader		header;

		int reward_active_flag;							// 当前激活的充值奖励
		int reward_fetch_flag;							// 当前已获取的充值奖励
		int history_charge_during_act;					// 活动期间的累积充值额
	};

	/////////////////////////////////////  合服活动 /////////////////////////////////////
	class SCCSASubActivityState
	{
	public:
		SCCSASubActivityState();
		MessageHeader		header;

		char sub_activity_state_list[CSA_SUB_TYPE_MAX];
	};

	class SCCSAActivityInfo
	{
	public:
		struct RankUser
		{
			int role_id;
			long long rank_value;
			long long rank_reserved;
			GameName role_name;
			char camp;
			char sex;
			short prof;
		};

		struct RankItem
		{
			char is_finish;
			char reserve_ch;
			short reserve_sh;
			RankUser user_list[COMBINE_SERVER_ACTIVITY_RANK_REWARD_ROLE_NUM];
		};

		SCCSAActivityInfo();
		MessageHeader		header;

		int qianggou_buynum_list[COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE];
		int csa_xmz_winner_roleid;
		int csa_gcz_winner_roleid;

		RankItem rank_item_list[CSA_RANK_TYPE_MAX];	

		int server_panic_buy_num_list[COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT];
	};

	class CSCSAQueryActivityInfo
	{
	public:
		CSCSAQueryActivityInfo();
		MessageHeader		header;
	};
	
	enum CSA_LOGIN_GIFT_OPERA
	{
		CSA_LOGIN_GIFT_OPERA_FETCH_COMMON_REWARD = 0,
		CSA_LOGIN_GIFT_OPERA_FETCH_VIP_REWARD,
		CSA_LOGIN_GIFT_OPERA_FETCH_ACCUMULATE_REWARD,

		CSA_LOGIN_GIFT_OPERA_MAX,
	};

	enum CSA_ROLL_OPERA
	{
		CSA_ROLL_OPERA_ROLL = 0,
		CSA_ROLL_OPERA_BROADCAST = 1,
	};

	class CSCSARoleOperaReq
	{
	public:
		CSCSARoleOperaReq();
		MessageHeader		header;

		short sub_type;
		short param_1;
		short param_2;
		short reserve_sh;
	};

	class SCCSARoleInfo
	{
	public:
		SCCSARoleInfo();
		MessageHeader		header;

		int rank_qianggou_buynum_list[COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE];
		int roll_chongzhi_num;
		int chongzhi_rank_chongzhi_num;
		int consume_rank_consume_gold;
		int kill_boss_kill_count;

		int personal_panic_buy_numlist[COMBINE_SERVER_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT]; 
		int server_panic_buy_numlist[COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT];

		short login_days;
		char reserve_ch;
		char has_fetch_accumulate_reward;
		int fetch_common_reward_flag;
		int fetch_vip_reward_flag;
		int roll_total_chongzhi_num;
		short rank_combine_consume_num;                                // 合服消费排名
		short rank_combine_chongzhi_num;                               // 合服充值排名
	};

	class SCCSARollResult
	{
	public:
		SCCSARollResult();
		MessageHeader		header;

		int ret_seq;
	};

	enum CSA_BOSS_OPERA_TYPE
	{
		CSA_BOSS_OPERA_TYPE_ENTER = 0,						//进入场景
		CSA_BOSS_OPERA_TYPE_INFO_REQ = 1,					//请求boss信息
		CSA_BOSS_OPERA_TYPE_RANK_REQ = 2,					//请求排行榜信息
		CSA_BOSS_OPERA_TYPE_ROLE_INFO_REQ = 3,				//请求个人信息
	};

	class SCCSABossInfo
	{
	public:
		SCCSABossInfo();
		MessageHeader		header;

		struct BossInfo
		{
			BossInfo() : boss_id(0), boss_type(0), next_refresh_time(0)
			{}

			unsigned short boss_id;							//boss_id
			short boss_type;								//boss类型
			unsigned int next_refresh_time;					//下次刷新时间
		};

		BossInfo boss_info[COMBINE_SERVER_BOSS_MAX_COUNT];
		short refresh_status;								//boss刷新状态,0日常boss,1活动boss
		short gather_count;
	};

	class SCCSABossRankInfo
	{
	public:
		SCCSABossRankInfo();
		MessageHeader		header;

		struct BossRank
		{
			BossRank() : id(0), rank_value(0)
			{
				memset(name, 0, sizeof(name));
			}

			int id;										//个人(公会)id
			int rank_value;								//击杀数
			GameName name;								//名字
		};

		BossRank personal_rank[COMBINE_SERVER_BOSS_RANK_NUM];
		BossRank guild_rank[COMBINE_SERVER_BOSS_RANK_NUM];
	};

	class SCCSABossRoleInfo
	{
	public:
		SCCSABossRoleInfo();
		MessageHeader		header;

		short kill_boss_num;							//本人击杀数
		short guild_killl_boss_num;						//本人公会击杀数
	};

	/////////////////////////////////////  随机活动 累计充值（天数） /////////////////////////////////////
	enum RA_TOTAL_CHONGZHI_DAY_OPERA_TYPE
	{
		RA_TOTAL_CHONGZHI_DAY_OPERA_TYPE_QUERY_INFO = 0,
		RA_TOTAL_CHONGZHI_DAY_OPERA_TYPE_FETCH_REWARD,

		RA_TOTAL_CHONGZHI_DAY_OPERA_TYPE_MAX,
	};

	class SCRATotalChargeDayInfo	// 2242
	{
	public:
		SCRATotalChargeDayInfo();
		MessageHeader header;

		int charge_day_count;								// 累计充值天数
		int reward_has_fetch_flag;							// 已领取过的奖励标记
	};

	/////////////////////////////////////  随机活动 珍宝阁活动  /  ////////////////////////////////////
	enum RA_ZHENBAOGE_OPERA_TYPE
	{
		RA_ZHENBAOGE_OPERA_TYPE_QUERY_INFO = 0,					// 请求活动信息
		RA_ZHENBAOGE_OPERA_TYPE_BUY = 1,						// 单个购买请求
		RA_ZHENBAOGE_OPEAR_TYPE_BUY_ALL = 2,					// 全部购买请求
		RA_ZHENBAOGE_OPEAR_TYPE_FLUSH = 3,						// 刷新请求
		RA_ZHENBAOGE_OPEAR_TYPE_RARE_FLUSH = 4,					// 稀有刷新请求
		RA_ZHENBAOGE_OPERA_TYPE_FETCH_SERVER_GIFT = 5,			// 领取全服礼包

		RA_ZHENBAOGE_OPERA_TYPE_MAX
	};

	class SCRAZhenbaogeInfo
	{
	public:
		SCRAZhenbaogeInfo();
		MessageHeader header;

		UNSTD_STATIC_CHECK((RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT + 1) % 2 == 0);
		short zhenbaoge_item_list[RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT];
		short reserve_sh;
		short zhenbaoge_refresh_gift_fetch_flag;
		short cur_flush_times;
		unsigned int zhenbaoge_next_flush_timetamp;
	};

	/////////////////////////////////////跨服/////////////////////////////////
	class CSCrossStartReq                // 5750
	{
	public:
		CSCrossStartReq();
		MessageHeader	header;

		short			cross_activity_type;
		short			param;
		unsigned short	param_2;
		unsigned short	param_3;
		
		unsigned short	sos_pos_x;  // 传送到求救坐标(x,y) ， 如果是正常的进入活动就传(0,0)
		unsigned short  sos_pos_y;
	};

	class SCCrossEnterServer
	{
	public:
		SCCrossEnterServer();
		MessageHeader	header;

		int				cross_activity_type;
		char			login_server_ip[64];
		int				login_server_port;
		PlatName		pname;
		unsigned int	login_time;
		LoginStr		login_str;
		char			anti_wallow;
		char			reserved1;
		short			server;
	};

	/////////////////////////////////////////跨服修罗塔/////////////////////////////////

	class CSCrossXiuluoTowerJoinReq
	{
	public:
		CSCrossXiuluoTowerJoinReq();
		MessageHeader	header;
	};

	class SCCrossXiuluoTowerSelfActivityInfo		// 5701
	{
	public:
		SCCrossXiuluoTowerSelfActivityInfo();
		MessageHeader	header;

		short cur_layer;
		short max_layer;
		short immediate_realive_count;
		short boss_num;
		int total_kill_count;
		int kill_role_count;
		int cur_layer_kill_count;
		int reward_cross_honor;
		int score;
		int score_reward_flag;
		unsigned int refresh_boss_time;
		unsigned int gather_buff_end_timestamp;				// 采集buff结束时间戳
	};

	class SCCrossXiuluoTowerRankInfo				// 5702
	{
	public:
		SCCrossXiuluoTowerRankInfo();
		MessageHeader		header;

		struct RankInfo
		{
			GameName user_name;
			unsigned short finish_time;
			short max_layer;
			char prof;
			char camp;
			short reserve_sh;
		};

		int count;																
		RankInfo rank_list[CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM];	
	};

	class SCCrossXiuluoTowerChangeLayerNotice		// 5703
	{
	public:
		SCCrossXiuluoTowerChangeLayerNotice();
		MessageHeader		header;

		int					is_drop_layer;
	};

	class SCCrossXiuluoTowerUserResult				// 5704
	{
	public:
		SCCrossXiuluoTowerUserResult();
		MessageHeader		header;

		char				max_layer;
		char				rank_pos;
		short				kill_role_count;
		int					reward_cross_honor;
	};

	class CSCrossXiuluoTowerBuyBuff
	{
	public:
		CSCrossXiuluoTowerBuyBuff();
		MessageHeader		header;

		short				is_buy_realive_count;
		short				reserve_sh;
	};

	class SCCrossXiuluoTowerInfo
	{
	public:
		SCCrossXiuluoTowerInfo();
		MessageHeader		header;

		int					buy_realive_count;
		short				add_gongji_per;
		short				add_hp_per;
		long long			uuid;
	};

	class SCCrossHonorChange															// 跨服荣誉值改变
	{
	public:
		SCCrossHonorChange();																										
		MessageHeader		header;

		int honor;																		// 当前荣誉值
		int delta_honor;																// 荣誉改变数值
	};

	class SCCrossXiuluoTowerBuffInfo
	{
	public:
		SCCrossXiuluoTowerBuffInfo();																										
		MessageHeader		header;

		ObjID obj_id;
		short buff_num;
		unsigned int next_send_reward_time;
	};

	class SCCrossXiuluoTowerGatherInfo
	{
	public:
		SCCrossXiuluoTowerGatherInfo();
		MessageHeader		header;

		struct InfoItem
		{
			int gather_id;
			int gather_count;
		};
		static const int MAX_INFO_COUNT = 4;

		int count;
		InfoItem info_list[MAX_INFO_COUNT];
	};

	class CSCrossXiuluoTowerScoreRewardReq
	{
	public:
		CSCrossXiuluoTowerScoreRewardReq();
		MessageHeader		header;

		int index;
	};

	class CSCrossXiuluoTowerBuyBuffReq			// 5735 修罗塔购买buff
	{
	public:
		CSCrossXiuluoTowerBuyBuffReq();
		MessageHeader header;

	};

	class SCCossXiuluoTowerRankTitleInfo   // 5741 跨服修罗塔前三名进入10层的人
	{
	public:
		SCCossXiuluoTowerRankTitleInfo();
		MessageHeader header;
		struct RankInfo
		{
			long long uuid;
			GameName user_name;
			unsigned int  finish_time;
		};
		int rank_count;
		RankInfo rank_title_list[CROSS_ACTIVITY_XIULUO_TOWER_RANK_NUM];
	};

	class SCCossXiuluoTowerBossInfo //5742
	{
	public:
		SCCossXiuluoTowerBossInfo();
		MessageHeader header;
		int monster_id;
		long long max_hp;
		long long cur_hp;
	};

	/////////////////////////////////////  跨服1v1  /////////////////////////////////////

	class SCCrossActivity1V1SelfInfo		// 5707
	{
	public:
		SCCrossActivity1V1SelfInfo();
		MessageHeader	header;

		int cross_score_1v1;                                                // 跨服1v1积分
		int cross_day_join_1v1_count;                                       // 今日参与次数

		short cross_1v1_join_time_reward_flag;                              // 参与奖励
		short cross_1v1_fetch_score_reward_flag;                            // 功勋奖励

		short today_buy_times;
		short cur_season;                                                   // 当前第几赛季

		int cross_1v1_curr_activity_add_score;								// 跨服1v1本场活动增加的积分

		short cross_1v1_total_join_times;                                   // 赛季总的参与次数
		short cross_1v1_total_win_times;                                    // 赛季总的胜利次数

		int cross_1v1_gongxun;                                              // 赛季功勋

		short cross_1v1_dur_win_times;                                      // 赛季最高连赢次数
		short resver_sh;

		char cross_1v1_use_ring[CROSS_ACTIVITY_1V1_RING_USE_COUNT];         // 佩戴戒指

		char cross_1v1_have_ring[CROSS_1V1_SEASON_MAX];                      // 已有戒指
	};

	enum CROSS1V1_FIGHT_TIME_TYPE
	{
		CROSS1V1_FIGHT_TIME_TYPE_INVALID = 0,		// 
		CROSS1V1_FIGHT_TIME_TYPE_START,				// 开始
	};

	class SCCross1v1FightStart		// 5708 跨服1V1战斗开始
	{
	public:
		SCCross1v1FightStart();
		MessageHeader header;

		int timestamp_type;						// 时间类型
		unsigned int fight_start_timestmap;		// 开始时间
		unsigned int fight_end_timestmap;		// 结束时间
	};

	class CSCrossMatch1V1Req	// 5753		请求匹配
	{
	public:
		CSCrossMatch1V1Req();
		MessageHeader	header;
	};

	class CSCross1v1FightReady	// 5754  准备战斗
	{
	public:
		CSCross1v1FightReady();
		MessageHeader header;
	};	

	enum CROSS_1V1_FETCH_REWARD_TYPE
	{
		CROSS_1V1_FETCH_REWARD_TYPE_JOIN_TIMES = 1,		// 参加次数
		CROSS_1V1_FETCH_REWARD_TYPE_SCORE,				// 积分

		CROSS_1V1_FETCH_REWARD_TYPE_MAX,
	};

	class CSCross1v1FetchRewardReq	// 5755		领取奖励请求
	{
	public:
		CSCross1v1FetchRewardReq();
		MessageHeader	header;

		short fetch_type;			// 类型
		short seq;					// 序号
	};

	class CSCross1v1WearRingReq       // 5763    戒指操作
	{
	public:
		CSCross1v1WearRingReq();
		MessageHeader header;

		int oper_type;					// 操作类型
		int ring_seq;					// 戒指序号
	};

	class CSCross1v1MatchResultReq       // 5764 匹配结果请求
	{
	public:
		CSCross1v1MatchResultReq();
		MessageHeader header;

		int req_type;				// 0 : 请求 1：取消

	};

	class CSCross1v1BuyTimeReq		// 5762 购买次数
	{
	public:
		CSCross1v1BuyTimeReq();
		MessageHeader header;
	};

	/////////////////////////////////////////跨服3V3/////////////////////////////////

	class CSCrossMultiuserChallengeMatchgingReq				//5756 跨服3V3请求匹配
	{
	public:
		CSCrossMultiuserChallengeMatchgingReq();
		MessageHeader	header;
	};

	class CSCrossMultiuserChallengeGetBaseSelfSideInfo		//5757 跨服3V3请求初始队伍信息
	{
	public:
		CSCrossMultiuserChallengeGetBaseSelfSideInfo();
		MessageHeader	header;
	};

	class CSCrossMultiuserChallengeFetchDaycountReward		//5758  跨服3V3领取每日次数奖励
	{
	public:
		CSCrossMultiuserChallengeFetchDaycountReward();
		MessageHeader	header;

		short index;		// <0 全部领取   >0 领取一次奖励
		short reserve_sh;
	};

	class CSCrossMultiuerChallengeCancelMatching			//5759  跨服3V3取消匹配
	{
	public:
		CSCrossMultiuerChallengeCancelMatching();
		MessageHeader	header;
	};

	class SCCrossMultiuserChallengeBaseSelfSideInfo			//5712  跨服3V3初始队伍信息
	{
	public:
		SCCrossMultiuserChallengeBaseSelfSideInfo();
		MessageHeader	header;

		struct UserInfo
		{
			int				plat_type;		//平台类型
			int				server_id;		//服id
			int				uid;		
			GameName		user_name;		//用户名
			char			reserved;		//保留
			char			sex;			//性别
			char			prof;			//职业
			char			camp;			//阵营
			int				level;			//等级
			int				challenge_score;	// 跨服3V3分数
			int				win_percent;		//胜率
			int				mvp_count;		 // 跨服3V3MVP次数
			long long		capability;		 // 总战力
		};

		int user_count;
		UserInfo user_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT];
	};

	class SCCrossMultiuserChallengeSelfInfoRefresh			//5709	跨服3V3个人信息更新
	{
	public:
		SCCrossMultiuserChallengeSelfInfoRefresh();
		MessageHeader		header;

		int self_side;				//那一方	
		int kills;					//杀人数
		int assist;					//助攻数
		int dead;					//死亡数
	};

	class SCCrossMultiuserChallengeMatchInfoRefresh		//5710  跨服3V3比赛信息更新
	{
	public:
		SCCrossMultiuserChallengeMatchInfoRefresh();
		MessageHeader		header;

		int strong_hold_rate_info; //据点进度条0-100
		int side_score_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];
	};

	class SCCrossMultiuserChallengeMatchState		//5711  跨服3V3状态通知
	{
	public:
		SCCrossMultiuserChallengeMatchState();
		MessageHeader	header;

		struct MatchUserInfo
		{
			short			plat_type;		//平台类型
			ObjID			obj_id;
			int				role_id;		//角色id
			GameName		name;			//用户名
			short			prof;			//职业
			short			sex;			//性别
			short			kills;			//击杀数
			short			assist;			//助攻数
			short			dead;			//死亡数
			short			occupy;			//占领数
			int				origin_score;	//原本积分
			int				add_score;		//获得积分
			int				add_honor;		//获得荣耀
			int				add_gongxun;	//获得功勋
			int				is_mvp;			//是非是mvp
		};

		short				match_state;	//比赛状态 0 倒计时 1 对战 2 结束
		short				win_side;		//胜方
		unsigned int		next_state_time;//下一状态时间
		MatchUserInfo		user_info_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];
	};

	class SCCrossMultiuserChallengeSelfActicityInfo			//5713  跨服3V3个人活动信息
	{
	public:
		SCCrossMultiuserChallengeSelfActicityInfo();
		MessageHeader	header;

		int challenge_mvp_count;						//MVP次数
		int challenge_score;							//积分
		int challenge_total_match_count;				//总次数
		int challenge_win_match_count;					//胜利次数
		short win_percent;								//胜率
		short today_match_count;						//今天参加次数
		int matching_state;								//当前参赛状态 0 匹配队友中 1 匹配对手中  2 匹配完成  3 匹配取消

		char join_reward_fetch_flag;								// 跨服3v3参与奖领取标记
		char rev_ch;
		short rev_sh;

		char cross_3v3_season_reward_use[CROSS_ACTIVITY_3V3_RING_USE_COUNT]; // 跨服3v3 正在使用的令牌

		char cross_3v3_season_reward[CROSS_3V3_SEASON_MAX];         // 跨服3v3 已经激活的令牌

		int gongxun_reward_fetch_flag;								// 跨服3v3功勋奖领取标记
		int gongxun_value;											// 跨服3v3功勋值

		int season_count;                                           // 赛季
	};

	class CSMultiuserChallengeReqSideMemberPos  // 5760跨服3v3，获取队友位置信息
	{
	public:
		CSMultiuserChallengeReqSideMemberPos();
		MessageHeader		header;
	};

	struct SCMultiuserChallengeTeamMemberPosItem
	{
		int				role_id;		//用户id
		ObjID			obj_id;
		char			reserved;
		char			is_leave_scene;	//是非离开
		short			pos_x;			//x坐标
		short			pos_y;			//y坐标
		Dir				dir;			//方向
		Scalar			distance;		//方向距离
		int				move_speed;		//移动速度
	};

	class SCMultiuserChallengeTeamMemberPosList  // 5714跨服3v3，获取队友位置信息
	{
	public:
		SCMultiuserChallengeTeamMemberPosList();
		MessageHeader		header;

		int member_count;
		SCMultiuserChallengeTeamMemberPosItem member_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT];
	};

	class CSMultiuserChallengeWearCardReq           // 5736 跨服3v3令牌请求
	{
	public:
		CSMultiuserChallengeWearCardReq();
		MessageHeader header;

		int oper_type;  //1 佩戴 2脱下
		int card_seq;	//令牌索引
	};

	class CSCrossMultiuserChallengeFetchGongxunReward // 5737 跨服3v3领取功勋奖励
	{
	public:
		CSCrossMultiuserChallengeFetchGongxunReward();
		MessageHeader	header;

		short seq;    //<0 全部领取   >= 0 领取功勋配置中的一个
		short reserve_sh; //保留
	};

	/////////////////////////////////////  跨服团战  /////////////////////////////////////
	class CSCrossTuanzhanFetchReward											// 请求领取奖励
	{
	public:
		CSCrossTuanzhanFetchReward();
		MessageHeader		header;
	};

	class SCCrossTuanzhanStateNotify											// 比赛状态通知
	{
	public:
		SCCrossTuanzhanStateNotify();
		MessageHeader		header;

		unsigned int fight_start_time;											// 战斗开始时间
		unsigned int activity_end_time;											// 活动结束时间
		unsigned int rand_side_time;											// 随机阵营时间
	};

	class SCCrossTuanzhanPlayerInfo												// 玩家信息
	{
	public:
		SCCrossTuanzhanPlayerInfo();
		MessageHeader		header;

		ObjID obj_id;
		short is_broacast;
		short side;																// 所在阵营
		short score_reward_fetch_seq;
		unsigned int score;														// 积分
		unsigned int kill_num;													// 击杀次数
		unsigned int assist_kill_num;											// 助攻次数
		unsigned int dur_kill_num;												// 连杀次数
	};

	class SCCrossTuanzhanRankInfo												// 排名信息
	{
	public:
		SCCrossTuanzhanRankInfo();
		MessageHeader		header;

		struct RankItem
		{
			RankItem() : side(CROSS_TUANZHAN_SIDE_MAX), reserve_sh(0), score(0), kill(0), assist(0)
			{
				memset(name, 0, sizeof(name));
			}

			short side;
			short reserve_sh;
			unsigned int score;
			GameName name;
			short kill;
			short assist;
		};

		int rank_list_count;
		RankItem rank_list[CROSS_TUANZHAN_RANK_LIST_SIZE];
	};

	class SCCrossTuanzhanSideInfo											// 阵营积分信息
	{
	public:
		SCCrossTuanzhanSideInfo();
		MessageHeader	header;

		unsigned int side_score_list[CROSS_TUANZHAN_SIDE_MAX];
	};

	class SCCrossTuanzhanPillaInfo											// 通天柱子信息
	{
	public:
		SCCrossTuanzhanPillaInfo();
		MessageHeader	header;

		struct PillaInfo
		{
			unsigned short monster_id;												// 柱子怪物id
			ObjID obj_id;													// 柱子的对象id
			short owner_side;												// 占领柱子的阵营
			short index;
			GameName owner_name;											// 占领柱子玩家名
		};

		unsigned short pilla_list_count;
		short reserve_sh;
		PillaInfo pilla_list[CROSS_TUANZHAN_PILLA_MAX_COUNT]; 
	};

	class SCCrossTuanzhanPlayerDurKillInfo									// 连杀信息变更 
	{
	public:
		SCCrossTuanzhanPlayerDurKillInfo();
		MessageHeader	header;

		ObjID obj_id;														// 玩家对象id
		short reserve_sh;
		unsigned int dur_kill_num;											// 连杀次数
	};

	class SCCrossTuanzhanResultInfo											// 比赛结果通知
	{
	public:
		SCCrossTuanzhanResultInfo();
		MessageHeader	header;

		unsigned int personal_score;										// 个人积分
		unsigned int side_score;											// 阵营积分
		short result;														// 比赛结果，0 失败，1 胜利
		short reserve_sh;
	};

	/////////////////////////////////////  跨服牧场  /////////////////////////////////////

	class SCCrossPasturePlayerInfo											// 跨服牧场，玩家信息通知  5722
	{
	public:
		SCCrossPasturePlayerInfo();																										
		MessageHeader	header;

		int score;															// 当前积分
		short left_get_score_times;											// 剩余获取积分次数
		short reserve;
		int x;																// 特殊怪物出生点x
		int y;																// 特殊怪物出生点y
		unsigned int special_monster_refresh_time;							// 特殊怪物刷新时间戳
	};

	class SCCrossPastureRankInfo   //跨服牧场，玩家排行信息 5734
	{
	public:
		SCCrossPastureRankInfo();
		MessageHeader header;

		struct RankItem
		{
			int uid;
			GameName name;
			int score;
		};

		int rank_count;
		RankItem rank_list[CROSS_PASTURE_RANK_COUNT];
	};

	enum CROSS_PSATURE_ANIMAL_NOTIC_TYPE
	{
		CROSS_PSATURE_ANIMAL_NOTIC_TYPE_FOLLOW = 0,
		CROSS_PSATURE_ANIMAL_NOTIC_TYPE_BE_ROBBED,

		CROSS_PSATURE_ANIMAL_NOTIC_TYPE_MAX
	};

	class SCCPPlayerHasAttachAnimalNotic									// 跨服牧场怪物跟随状态通知
	{
	public:
		SCCPPlayerHasAttachAnimalNotic();
		MessageHeader header;

		int notic_reason;
		GameName robber_name;
	};

	struct SCCrossGuildBattleSpecialTimeNotice	// 5733
	{
		SCCrossGuildBattleSpecialTimeNotice();
		MessageHeader header;

		int activty_status = 0;					// 活动状态 0结束 2开启
		unsigned int end_timestamp = 0;			// 结束时间
	};

	/////////////////////////////////////  跨服BOSS  /////////////////////////////////////
	enum CROSS_BOSS_OPERA_TYPE
	{
		CROSS_BOSS_OPERA_TYPE_GET_FLUSH_INFO = 0,	// 刷新信息 param1 层数（为0则为所有层）
		CROSS_BOSS_OPERA_TYPE_BOSS_KILL_RECORD,		// 击杀记录 param1 层数 param2 boss_id
		CROSS_BOSS_OPERA_TYPE_DROP_RECORD,			// 掉落记录	
		CROSS_BOSS_OPERA_TYPE_CONCERN_BOSS,			// 关注boss	param1 层数 param2 boss_id
		CROSS_BOSS_OPERA_TYPE_UNCONCERN_BOSS,		// 关注boss param1 层数 param2 boss_id
		CROSS_BOSS_OPERA_TYPE_FORENOTICE,			// boss通知

		CROSS_BOSS_OPERA_TYPE_MAX,
	};

	class CSCrossBossBossInfoReq											// 请求跨服boss信息   5730
	{
	public:
		CSCrossBossBossInfoReq();
		MessageHeader	header;

		int req_type;
		int param_1;
		int param_2;
	};

	class SCCrossBossBossInfoAck											// 跨服boss信息   5731
	{
	public:
		SCCrossBossBossInfoAck();
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
			BossInfo boss_list[MAX_CROSS_BOSS_PER_SCENE];
		};

		int scene_count;
		SceneInfo scene_list[CROSS_BOSS_SCENE_MAX];
	};

	class SCCrossBossPlayerInfo												// 跨服BOSS玩家信息 5724
	{
	public:
		SCCrossBossPlayerInfo();
		MessageHeader	header;

		short left_can_kill_boss_num;										// 剩余Boss数
		short left_treasure_crystal_gather_times;							// 剩余珍惜水晶采集次数
		int left_ordinary_crystal_gather_times;								// 剩余普通水晶采集次数

		unsigned int concern_flag[CROSS_BOSS_SCENE_MAX];					// 关注flag
	};

	class SCCrossBossSceneInfo												// 跨服boss场景里的玩家信息 5725
	{
	public:
		SCCrossBossSceneInfo();																										
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
		CrossBossInfo boss_list[MAX_CROSS_BOSS_PER_SCENE];					// Boss信息
	};

	class SCCrossBossBossKillRecord											// 5736 跨服BOSS BOSS击杀历史
	{
	public:
		SCCrossBossBossKillRecord();
		MessageHeader header;

		struct KillerRecrod
		{
			KillerRecrod() : uuid(0), dead_timestamp(0)
			{
				memset(killer_name, 0, sizeof(GameName));
			}

			long long uuid;
			unsigned int dead_timestamp;
			GameName killer_name;
		};

		int record_count;
		KillerRecrod killer_record_list[CROSS_BOSS_MAX_HISTROY_RECROD];
	};

	class SCCrossBossDropRecord												// 5737 跨服BOSS 掉落历史
	{
	public:
		SCCrossBossDropRecord();
		MessageHeader header;

		struct DropItem
		{
			long long uuid;
			GameName role_name;
			unsigned int pickup_timestamp;
			int scene_id;
			unsigned short monster_id;
			ItemID item_id;
			int item_num;
			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];
		};

		int record_count;
		DropItem dorp_record_list[CROSS_BOSS_MAX_DROP_RECROD];
	};

	class CSCrossBossBuyReliveTimes											//购买复活次数 5723
	{
	public:
		CSCrossBossBuyReliveTimes();
		MessageHeader	header;

		short buy_times;
		short reserve_sh;
	};

	class SCServerShutdownNotify											// 服务器即将关闭通知 5726
	{
	public:
		SCServerShutdownNotify();																										
		MessageHeader	header;

		int remain_second;													// 离关闭服务器剩余秒数
	};

	/////////////////////////////////////  随机活动 次日活动  /////////////////////////////////////
	enum RA_TOMORROW_REWARD_OPERA_TYPE
	{
		RA_TOMORROW_REWARD_OPERA_TYPE_QUERY_INFO = 0,
		RA_TOMORROW_REWARD_OPERA_TYPE_FETCH_REWARD,

		RA_TOMORROW_REWARD_OPERA_TYPE_MAX,
	};

	class SCRATomorrowRewardInfo	// 2243
	{
	public:
		SCRATomorrowRewardInfo();
		MessageHeader header;

		int reward_count;									// 当前拥有抽奖次数
		int reward_index;									// 当前抽到的物品下标
	};

	/////////////////////////////////////  随机活动 累计充值活动  /////////////////////////////////////
	enum RA_TOTAL_CHARGE_OPERA_TYPE
	{
		RA_TOTAL_CHARGE_OPERA_TYPE_QUERY_INFO = 0,
		RA_TOTAL_CHARGE_OPERA_TYPE_FETCH_REWARD,

		RA_TOTAL_CHARGE_OPERA_TYPE_MAX,
	};

	class SCRATotalChargeInfo
	{
	public:
		SCRATotalChargeInfo();
		MessageHeader header;

		int total_charge_value;								// 累计充值数
		int reward_has_fetch_flag;							// 已领取过的奖励标记
	};

	//////////////////////////////////// 随机活动 限时积分兑换稀有装备 //////////////////////////////////////
	enum RA_TIME_LIMIT_EXCHANGE_OPERA_TYPE
	{
		RA_TIME_LIMIT_EXCHANGE_OPERA_TYPE_QUERY = 0,
		RA_TIME_LIMIT_EXCHANGE_OPERA_TYPE_EXCHANGE,

		RA_TIME_LIMIT_EXCHANGE_OPERA_TYPE_MAX,
	};

	class SCRATimeLimitExchangeEquiInfo
	{
	public:
		SCRATimeLimitExchangeEquiInfo();
		MessageHeader header;

		int ra_time_limit_exchange_exchange_times_list[TIME_LIMIT_EXCHANGE_ITEM_COUNT];		//兑换装备次数
	};

	class SCRATimeLimitExchangeJLInfo
	{
	public:
		SCRATimeLimitExchangeJLInfo();
		MessageHeader header;

		int ra_time_limit_exchange_exchange_times_list[TIME_LIMIT_EXCHANGE_ITEM_COUNT];		//兑换精灵次数
	};

	/////////////////////////////////////  随机活动 层抽奖活动  /////////////////////////////////////
	enum RA_LEVEL_LOTTERY_OPERA_TYPE
	{
		RA_LEVEL_LOTTERY_OPERA_TYPE_QUERY_INFO = 0,					// 请求历史记录信息
		RA_LEVEL_LOTTERY_OPERA_TYPE_DO_LOTTERY,						// 发起抽奖请求
		RA_LEVEL_LOTTERY_OPERA_TYPE_FETCHE_TOTAL_REWARD,			// 领取累计抽奖次数奖励
		RA_LEVEL_LOTTERY_OPERA_TYPE_ACTIVITY_INFO,					// 请求活动信息

		RA_LEVEL_LOTTERY_OPERA_TYPE_MAX,
	};

	class SCRALevelLotteryInfo			// 2400
	{
	public:
		SCRALevelLotteryInfo();
		MessageHeader header;

		int lottery_cur_level;										// 玩家当前所在层
		int history_count;											// 所有玩家抽奖历史纪录个数
		RALevelLotteryHistoryItem history_list[RA_LEVEL_LOTTERY_HISTORY_MAX_COUNT]; // 所有玩家抽奖历史
	};

	class SCRALevelLotteryRewardList	// 2401
	{
	public:
		SCRALevelLotteryRewardList();
		MessageHeader header;

		short reward_count;											 // 奖励总数量
		short reserve;

		char lottery_reward_list[RA_LEVEL_LOTTERY_REWARD_MAX_COUNT]; // 抽奖的奖励ID列表

		UNSTD_STATIC_CHECK(0 == (RA_LEVEL_LOTTERY_REWARD_MAX_COUNT % 4))
	};

	class SCRALevelLotteryActivityInfo	// 2453
	{
	public:
		SCRALevelLotteryActivityInfo();
		MessageHeader header;

		int ra_level_lottery_free_buy_times;							// 每日免费购买次数
		unsigned int ra_lottery_next_free_timestamp;					// 下次免费购买时间
		int ra_lottery_buy_total_times;									// 累计购买次数
		unsigned int ra_lottery_fetch_reward_flag;						// 领取奖励标志
	};

	/////////////////////////////////////  随机活动 充值扭蛋打折活动  /////////////////////////////////////
	static const int SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT = 10;

	enum RA_CHONGZHI_NIU_EGG_OPERA_TYPE
	{
		RA_CHONGZHI_NIU_EGG_OPERA_TYPE_QUERY_INFO = 0,				// 请求活动信息
		RA_CHONGZHI_NIU_EGG_OPERA_TYPE_CHOU,						// 抽奖
		RA_CHONGZHI_NIU_EGG_OPERA_TYPE_FETCH_REWARD,				// 领取全服奖励

		RA_CHONGZHI_NIU_EGG_OPERA_TYPE_MAX,
	};

	class SCRANiuEggInfo											// 2402
	{
	public:
		SCRANiuEggInfo();
		MessageHeader header;

		int server_total_niu_egg_times;								// 活动期间全服总扭蛋次数
		int cur_can_niu_egg_chongzhi_value;							// 剩余可抽奖的充值额度

		int server_reward_has_fetch_reward_flag;					// 当前已领取的奖励标记
		int history_count;											// 所有玩家抽奖历史纪录个数

		RAChongzhiNiuEggHistoryItem history_list[RA_NIU_EGG_CHOU_HISTORY_MAX_COUNT]; // 所有玩家抽奖历史
	};

	class SCRANiuEggChouResultInfo									// 2403
	{
	public:
		SCRANiuEggChouResultInfo();
		MessageHeader header;

		int reward_req_list_count;									// 抽取的奖励数量
		short reward_req_list[SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT];	// 抽取的奖励列表

		UNSTD_STATIC_CHECK(0 == SC_RA_NIUEGG_CHOU_MAX_COUNT_LIMIT % 2);
	};


	/////////////////////////////////////  随机活动 秘境寻宝活动  ////////////////////////////////////

	enum RA_MIJINGXUNBAO_OPERA_TYPE
	{
		RA_MIJINGXUNBAO_OPERA_TYPE_QUERY_INFO = 0,				// 请求活动信息
		RA_MIJINGXUNBAO_OPERA_TYPE_TAO,							// 淘宝
		RA_MIJINGXUNBAO_OPERA_TYPE_FETCH_REWARD,				// 领取个人累抽奖励 param_1 = 领取奖励的索引（0开始）

		RA_MIJINGXUNBAO_OPERA_TYPE_MAX,
	};

	class SCRAMiJingXunBaoInfo
	{
	public:
		SCRAMiJingXunBaoInfo();
		MessageHeader header;

		unsigned int ra_mijingxunbao_next_free_tao_timestamp;
		
		int chou_times;
		int reward_flag;
	};

	static const int SC_MiJingXunBaoTao_MAX_TIMES = 50;

	class SCRAMiJingXunBaoTaoResultInfo
	{
	public:
		SCRAMiJingXunBaoTaoResultInfo();
		MessageHeader header;

		int count;
		short mijingxunbao_tao_seq[SC_MiJingXunBaoTao_MAX_TIMES];
	};

	///////////////////////////////////// 随机活动 秘境寻宝2	////////////////////////////////////

	enum RA_MIJINGXUNBAO2_OPERA_TYPE
	{
		RA_MIJINGXUNBAO2_OPERA_TYPE_QUERY_INFO = 0,				// 请求活动信息
		RA_MIJINGXUNBAO2_OPERA_TYPE_TAO,						// 淘宝

		RA_MIJINGXUNBAO2_OPERA_TYPE_MAX,
	};

	class SCRAMiJingXunBao2Info			//2440
	{
	public:
		SCRAMiJingXunBao2Info();
		MessageHeader header;

		unsigned int ra_mijingxunbao_next_free_tao_timestamp;
	};

	class SCRAMiJingXunBao2TaoResultInfo		//2441
	{
	public:
		SCRAMiJingXunBao2TaoResultInfo();
		MessageHeader header;

		int count;
		short mijingxunbao_tao_seq[SC_MiJingXunBaoTao_MAX_TIMES];
	};

	/////////////////////////////////////  随机活动 摇钱树活动  /////////////////////////////////////
	static const int SC_RA_MONEYTREE_CHOU_MAX_COUNT_LIMIT = 30;

	enum RA_CHONGZHI_MONEY_TREE_OPERA_TYPE
	{
		RA_MONEY_TREE_OPERA_TYPE_QUERY_INFO = 0,			// 请求活动信息
		RA_MONEY_TREE_OPERA_TYPE_CHOU,						// 抽奖
		RA_MONEY_TREE_OPERA_TYPE_FETCH_REWARD,				// 领取全服奖励

		RA_MONEY_TREE_OPERA_TYPE_MAX,
	};

	class SCRAMoneyTreeInfo									// 2411摇钱树信息
	{
	public:
		SCRAMoneyTreeInfo();
		MessageHeader header;

		int money_tree_total_times;							// 活动期间个人总摇钱次数
		unsigned int money_tree_free_timestamp;				// 最后免费抽奖时间
		int server_total_pool_gold;							// 活动期间奖池金额
		int server_reward_has_fetch_reward_flag;			// 当前已领取的奖励标记
	};

	class SCRAMoneyTreeChouResultInfo						//2412 摇钱树奖励信息
	{
	public:
		SCRAMoneyTreeChouResultInfo();
		MessageHeader header;

		int reward_req_list_count;									// 抽取的奖励数量
		char reward_req_list[SC_RA_MONEYTREE_CHOU_MAX_COUNT_LIMIT];	// 抽取的奖励列表
	};

	/////////////////////////////////////  随机活动 摇钱树活动2  /////////////////////////////////////
	class SCRAMoneyTree2Info	// 2442
	{
	public:
		SCRAMoneyTree2Info();
		MessageHeader header;

		int server_total_money_tree_times;						// 活动期间全服总摇钱次数
		int server_total_pool_gold;								// 活动期间奖池金额
		int server_reward_has_fetch_reward_flag;				// 当前已领取的奖励标记

	};

	class SCRAMoneyTree2ChouResultInfo		//2443
	{
	public:
		SCRAMoneyTree2ChouResultInfo();
		MessageHeader header;

		int reward_req_list_count;									// 抽取的奖励数量
		char reward_req_list[SC_RA_MONEYTREE_CHOU_MAX_COUNT_LIMIT];	// 抽取的奖励列表
	};

	/////////////////////////////////////  随机活动 陛下请翻牌活动  ///////////////////////////////////
	enum RA_KING_DRAW_OPERA_TYPE
	{
		RA_KING_DRAW_OPERA_TYPE_QUERY_INFO = 0,					// 请求活动的信息
		RA_KING_DRAW_OPERA_TYPE_PLAY_ONCE,						// 只玩一次请求
		RA_KING_DRAW_OPERA_TYPE_PLAY_TIMES,						// 玩多次请求
		RA_KING_DRAW_OPERA_TYPE_REFRESH_CARD,					// 重置请求
		RA_KING_DRAW_OPERA_TYPE_FETCH_REWARD,					// 领取累抽奖励

		RA_KING_DRAW_OPERA_TYPE_MAX
	};

	class SCRAKingDrawInfo										// 翻牌信息协议 2420
	{
	public:
		SCRAKingDrawInfo();
		MessageHeader header;

		unsigned short draw_times[RA_KING_DRAW_LEVEL_COUNT];	// 抽奖总次数
		short reserve_sh;
		int return_reward_flag;									// 领取累抽标志
		RAKingDrawCardList card_list[RA_KING_DRAW_LEVEL_COUNT];	// 所有级别的翻牌信息
	};

	class SCRAKingDrawMultiReward								// 多次翻牌的协议 2421
	{
	public:
		SCRAKingDrawMultiReward();
		MessageHeader header;

		short reward_count;										// 奖品实际数量
		short reward_seq_list[RA_KING_DRAW_MAX_REWARD_COUNT];	// 奖品的seq
		short is_use_item;										// 是否使用了物品
	};

	/////////////////////////////////////  随机活动 累计充值3(三件套)  ///////////////////////////////////
	class SCRATotalCharge3Info  // 2422
	{
	public:
		SCRATotalCharge3Info();
		MessageHeader		header;

		int cur_total_charge;
		int cur_total_charge_has_fetch_flag;

	};

	class CSRATotalCharge3Info // 2423  累计充值3，请求当前玩家充值信息
	{
	public:
		CSRATotalCharge3Info();
		MessageHeader		header;
	};

	/////////////////////////////////////  随机活动 被动变身排行榜活动  /////////////////////////////////////

	class SCRASpecialAppearancePassiveInfo
	{
	public:
		SCRASpecialAppearancePassiveInfo();
		MessageHeader header;

		int role_change_times;												// 当前玩家的被变身次数
		int rank_count;														// 排行榜玩家个数
		RASpecialAppearacePassivePlayerRankInfo rank_list[RA_SPECIAL_APPEARANCE_PASSIVE_RANK_MAX_COUNT];// 排行榜列表
	};

	/////////////////////////////////////  随机活动 变身排行榜活动  /////////////////////////////////////

	class SCRASpecialAppearanceInfo
	{
	public:
		SCRASpecialAppearanceInfo();
		MessageHeader header;

		int role_change_times;												// 当前玩家的变身次数
		int rank_count;														// 排行榜玩家个数
		RASpecialAppearacePlayerRankInfo rank_list[RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT];// 排行榜列表
	};

	/////////////////////////////////////  随机活动 累计充值4(新三件套)  /////////////////////////////////////

	class CSRATotalCharge4Info // 2424  累计充值4，请求当前玩家充值信息
	{
	public:
		CSRATotalCharge4Info();
		MessageHeader		header;
	};

	class SCRATotalCharge4Info  // 2425
	{
	public:
		SCRATotalCharge4Info();
		MessageHeader		header;

		int cur_total_charge;
		int cur_consume_reward;                              //当前消耗的奖励
		int ra_total_charge_4_cur_has_fetch_flag;           //当前奖励领取标志

	};

	/////////////////////////////////////  随机活动 挖矿活动  ///////////////////////////////////
	enum RA_MINE_OPERA_TYPE
	{
		RA_MINE_OPERA_TYPE_QUERY_INFO = 0,					// 请求活动的信息
		RA_MINE_OPERA_REFRESH,								// 换矿请求
		RA_MINE_OPERA_GATHER,								// 挖矿请求
		RA_MINE_OPERA_FETCH_SERVER_REWARD,					// 领取全服奖励请求
		RA_MINE_OPERA_EXCHANGE_REWARD,						// 兑换锦囊

		RA_MINE_OPERA_TYPE_MAX,
	};

	class SCRAMineAllInfo	// 2426
	{
	public:
		SCRAMineAllInfo();
		MessageHeader header;

		int total_refresh_times;							// 全服刷矿次数
		int role_refresh_times;								// 玩家刷矿次数
		int free_gather_times;								// 剩余免费挖次数
		int next_refresh_time;								// 下一次系统自动刷矿时间
		int cur_reward_fetch_flag;							// 当前全服礼包领取标记
		int gather_count_list[RA_MINE_MAX_TYPE_COUNT];		// 当前挖到的矿石数
		char mine_cur_type_list[RA_MINE_MAX_REFRESH_COUNT];	// 当前矿场的矿石
		short reward_begin_index;							// 礼包领取起始下标
		short reward_end_index;								// 礼包领取结束下标
	};

	/////////////////////////////////////  随机活动 顶刮刮活动  ///////////////////////////////////
	enum RA_GUAGUA_OPERA_TYPE
	{
		RA_GUAGUA_OPERA_TYPE_QUERY_INFO = 0,					// 请求活动的信息
		RA_GUAGUA_OPERA_TYPE_PLAY_TIMES,						// 刮奖多次
		RA_GUAGUA_OPREA_TYPE_FETCH_REWARD,						// 领取累计奖励 p1 = 索引

		RA_GUAGUA_OPERA_TYPE_MAX
	};

	class SCRAGuaGuaInfo														// 2428
	{
	public:
		SCRAGuaGuaInfo();
		MessageHeader header;

		unsigned short guagua_acc_count;										// 累计抽奖次数
		unsigned short guagua_acc_reward_has_fetch_flag;						// 已经领取标记
	};

	class SCRAGuaGuaMultiReward													// 2429
	{
	public:
		SCRAGuaGuaMultiReward();
		MessageHeader header;

		int reward_count;														// 是奖励的seq的数目，不是具体的物品数
		int is_bind;
		short reward_seq_list[RA_GUAGUA_MAX_REWARD_COUNT];					// 奖品的seq，值-1代表安慰奖
	};
	/////////////////////////////////////  随机活动 天命卜卦活动  ///////////////////////////////////
	enum RA_TIANMING_DIVINATION_OPERA_TYPE
	{
		RA_TIANMING_DIVINATION_OPERA_TYPE_QUERY_INFO = 0,		// 请求活动信息
		RA_TIANMING_DIVINATION_OPERA_TYPE_ADD_LOT_TIMES,		// 竹签加注
		RA_TIANMING_DIVINATION_OPERA_TYPE_RESET_ADD_LOT_TIMES,	// 重置竹签加注倍数
		RA_TIANMING_DIVINATION_OPERA_TYPE_START_CHOU,			// 开始抽签

		RA_TIANMING_DIVINATION_OPERA_TYPE_MAX				
	};

	class SCRATianMingDivinationInfo							// 天命卜卦信息 2430
	{
		struct HistoryItem
		{
			char seq;											// 奖池下标
			char add_lot;										// 获奖加注倍数
		};

	public:
		SCRATianMingDivinationInfo();
		MessageHeader header;

		int	total_add_lot;																// 当前总加注数
		int free_chou_times;															// 今天已用免费卜卦次数
		char add_lots_list[RA_TIANMING_LOT_COUNT];										// 竹签加注
		HistoryItem reward_history_item_count_list[RA_TIANMING_REWARD_HISTORY_COUNT];	// 获奖历史记录
		unsigned short reward_history_list_cur_index;									// 历史列表中最旧下标
	};

	class SCTianMingDivinationActivityStartChouResult			// 天命卜卦获奖结果  2431
	{
	public:
		SCTianMingDivinationActivityStartChouResult();
		MessageHeader header;

		short reward_index;										// 获奖物品下标
		short reserve_sh;
	};

	/////////////////////////////////////  随机活动 翻翻转活动  ///////////////////////////////////
	enum RA_FANFAN_OPERA_TYPE
	{
		RA_FANFAN_OPERA_TYPE_QUERY_INFO = 0,		// 请求活动信息
		RA_FANFAN_OPERA_TYPE_FAN_ONCE,				// 翻一次牌
		RA_FANFAN_OPERA_TYPE_FAN_ALL,				// 翻全部牌
		RA_FANFAN_OPERA_TYPE_REFRESH,				// 重置
		RA_FANFAN_OPERA_TYPE_WORD_EXCHANGE,			// 字组兑换
		RA_FANFAN_OPERA_TYPE_LEICHOU_EXCHANGE,		// 累抽兑换

		RA_FANFAN_OPERA_TYPE_MAX,
	};

	class SCRAFanFanAllInfo							// 2432
	{
	public:
		SCRAFanFanAllInfo();
		MessageHeader header;

		unsigned int ra_fanfan_next_refresh_time;									// 下一次重置时间
		unsigned char ra_fanfan_card_type_list[RA_FANFAN_CARD_COUNT];				// 卡牌类型列表，物品为seq值，隐藏卡和字组卡是枚举值
		RAFanFanWordActiveInfo ra_fanfan_word_active_info_list[RA_FANFAN_MAX_WORD_COUNT];	// 字组激活信息列表
		RAFanFanHiddenWordInfo ra_fanfan_hidden_word_info;							// 隐藏字信息
		char ra_fanfan_cur_free_times;												// 当前免费次数
		char reserve_ch[3];
		int ra_fanfan_leichou_times;												// 翻翻转累抽次数
		int ra_fanfan_reward_flag;													// 翻翻转奖励领取标记
	};

	class SCRAFanFanWordExchangeResult				// 2433
	{
	public:
		SCRAFanFanWordExchangeResult();
		MessageHeader header;

		short index;					// 索引
		short active_count;				// 激活数
	};

	/////////////////////////////////////  随机活动 连续充值  ///////////////////////////////////
	enum RA_CONTINUE_CHONGZHI_OPERA_TYPE
	{
		RA_CONTINUE_CHONGZHI_OPERA_TYPE_QUERY_INFO = 0,		// 请求活动信息
		RA_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_REWARD,		// 获取奖励
		RA_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_EXTRA_REWARD,	// 获取额外奖励

		RA_CONTINUE_CHONGZHI_OPERA_TYPE_MAX				
	};

	class SCRAContinueChongzhiInfo			// 2434
	{
	public:
		SCRAContinueChongzhiInfo();
		MessageHeader header;

		unsigned int today_chongzhi;						// 今日充值数
		short can_fetch_reward_flag;						// 奖励激活标记 位0标记特殊奖励,其他位标记达标奖励，值0未激活，值1已激活
		short has_fetch_reward_falg;						// 奖励领取标记 位0标记特殊奖励,其他位标记达标奖励，值0未领取，值1已领取
		char continue_chongzhi_days;						// 连续充值天数
		char is_activity_over;								// 是否连续充值中断 0未中断	1中断
		short reserve_sh;
	};

	/////////////////////////////////////  随机活动 连充特惠初  ///////////////////////////////////
	class SCRAContinueChongzhiInfoChu			// 2454
	{
	public:
		SCRAContinueChongzhiInfoChu();
		MessageHeader header;

		unsigned int today_chongzhi;						// 今日充值数
		short can_fetch_reward_flag;						// 奖励激活标记
		short has_fetch_reward_falg;						// 奖励领取标记
		char continue_chongzhi_days;						// 连续充值天数
		char reserve1;
		short reserve2;
	};

	/////////////////////////////////////  随机活动 连充特惠高  ///////////////////////////////////
	class SCRAContinueChongzhiInfoGao			// 2455
	{
	public:
		SCRAContinueChongzhiInfoGao();
		MessageHeader header;

		unsigned int today_chongzhi;						// 今日充值数
		short can_fetch_reward_flag;						// 奖励激活标记
		short has_fetch_reward_falg;						// 奖励领取标记
		char continue_chongzhi_days;						// 累计充值天数
		char reserve1;
		short reserve2;
	};

	/////////////////////////////////////  随机活动 限时连充  ///////////////////////////////////
	class SCRAContinueChongzhiInfo2			// 2466
	{
	public:
		SCRAContinueChongzhiInfo2();
		MessageHeader header;

		unsigned int today_chongzhi;						// 今日充值数
		short can_fetch_reward_flag;						// 奖励激活标记
		short has_fetch_reward_falg;						// 奖励领取标记
		char continue_chongzhi_days;						// 连续充值天数
		char reserve1;
		short reserve2;
	};

	/////////////////////////////////////  随机活动 连续消费  ///////////////////////////////////
	enum RA_CONTINUE_CONSUME_OPERA_TYPE
	{
		RA_CONTINUME_CONSUME_OPERA_TYPE_QUERY_INFO = 0,		// 请求活动信息
		RA_CONTINUE_CONSUME_OPEAR_TYPE_FETCH_REWARD,		// 获取奖励
		RA_CONTINUE_CONSUME_OPEAR_TYPE_FETCH_EXTRA_REWARD,	// 获取额外奖励

		RA_CONTINUE_CONSUME_OPERA_TYPE_MAX				
	};

	class SCRAContinueConsumeInfo			// 2435
	{
	public:
		SCRAContinueConsumeInfo();
		MessageHeader header;

		unsigned int today_consume_gold_total;				// 当天累计消费
		unsigned int cur_consume_gold;						// 上次领奖到现在的消费
		char continue_consume_total_days;					// 总达标天数
		char continue_consume_days;							// 当前连续达标天数
		char current_day_index;								// 当前活动处于第几天
		char extra_reward_num;								// 可领取特殊奖励数量
	};

	/////////////////////////////////////  随机活动 军歌嘹亮  ///////////////////////////////////

	enum RA_ARMY_DAY_OPERA_TYPE
	{
		RA_ARMY_DAY_OPERA_TYPE_INFO = 0,					// 请求活动信息
		RA_ARMY_DAY_OPERA_TYPE_EXCHANGE_FLAG,				// 兑换军旗
		RA_ARMY_DAY_OPERA_TYPE_EXCHANGE_ITEM,				// 兑换物品

		RA_ARMY_DAY_OPERA_TYPE_MAX
	};

	class SCRAArmyDayInfo						// 2436
	{
	public:
		SCRAArmyDayInfo();
		MessageHeader header;
		
		char belong_army_side;								// 所属军团
		char exchange_flag;									// 活跃度兑换标记
		short reserver_sh;
		int army_day_own_flags_list[RA_ARMY_DAY_ARMY_SIDE_NUM];	// 拥有所有军团旗帜数
		int army_day_own_score_list[RA_ARMY_DAY_ARMY_SIDE_NUM];	// 个人所有军团积分数
		int army_day_all_score_list[RA_ARMY_DAY_ARMY_SIDE_NUM];	// 全服所有军团积分数
	};

	/////////////////////////////////////  随机活动 循环充值  ///////////////////////////////////
	enum RA_CIRCULATION_CHONGZHI_OPERA_TYPE
	{
		RA_CIRCULATION_CHONGZHI_OPERA_TYPE_QUERY_INFO = 0,		// 请求活动信息
		RA_CIRCULATION_CHONGZHI_OPEAR_TYPE_FETCH_REWARD,		// 获取奖励
		RA_CIRCULATION_CHONGZHI_OPEAR_TYPE_FETCH_EXTRA_REWARD,	// 获取额外奖励

		RA_CIRCULATION_CHONGZHI_OPERA_TYPE_MAX				
	};

	class SCRACirculationChongzhiInfo							// 2437
	{
	public:
		SCRACirculationChongzhiInfo();
		MessageHeader header;

		unsigned int total_chongzhi;				// 累计充值
		unsigned int cur_chongzhi;					// 上次领奖到现在的充值
	};

	/////////////////////////////////////  随机活动 至尊幸运  ///////////////////////////////////
	enum RA_EXTREME_LUCKY_OPERA_TYPE
	{
		RA_EXTREME_LUCKY_OPERA_TYPE_QUERY_INFO = 0,			// 请求活动信息
		RA_EXTREME_LUCKY_OPERA_TYPE_GLOD_FLUSH,					// 刷新	
		RA_EXTREME_LUCKY_OPERA_TYPE_DRAW,					// 抽奖
		RA_EXTREME_LUCKY_OPREA_TYPE_AUTO_FLUSH,				// 自动刷新(抽完9次)
		RA_EXTREME_LUCKY_OPREA_TYPE_FETCH_REWARD,			// 领取返利奖励

		RA_EXTREME_LUCKY_OPERA_TYPE_MAX
	};

	class SCRAExtremeLuckyAllInfo							// 2438
	{
	public:
		SCRAExtremeLuckyAllInfo();
		MessageHeader header;

		unsigned int next_flush_timestamp;					// 下次刷新时间
		short free_draw_times;								// 剩余免费次数
		short cur_total_draw_times;							// 当前轮总抽奖次数(包含免费)
		int gold_draw_times;								// 元宝抽奖次数
		short total_draw_times;								// 总抽奖次数
		short return_reward_flag;							// 返利标志

		RAExtremeLuckyRewardInfo cur_item_info_list[RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT];			// 当前物品信息
	};

	class SCRAExtremeLuckySingleInfo					// 2439
	{
	public:
		SCRAExtremeLuckySingleInfo();
		MessageHeader header;

		short cur_total_draw_times;
		short gold_draw_times;
		short total_draw_times;								// 总抽奖次数
		short return_reward_flag;							// 返利标志
		int free_draw_times;
		RAExtremeLuckyRewardInfo single_item_info;
	};

	/////////////////////////////////////  随机活动 欢度国庆  ///////////////////////////////////
	enum RA_NATIONAL_DAY_OPERA_TYPE
	{
		RA_NATIONAL_DAY_OPERA_TYPE_INFO = 0,
		RA_NATIONAL_DAY_OPERA_TYPE_EXCHANGE,			// 兑换奖品

		RA_NATIONAL_DAY_OPERA_TYPE_MAX,
	};

	/////////////////////////////////////  随机活动 珍宝阁2  ///////////////////////////////////
	class SCRAZhenbaoge2Info								// 2444
	{
	public:
		SCRAZhenbaoge2Info();
		MessageHeader header;

		UNSTD_STATIC_CHECK((RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT + 1) % 2 == 0);
		short zhenbaoge_item_list[RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT];
		short reserve_sh;
		short zhenbaoge_server_fetch_flag;
		short cur_server_flush_times;
		unsigned int zhenbaoge_next_flush_timetamp;
	};

	/////////////////////////////////////  随机活动 步步高升  ///////////////////////////////////
	enum RA_PROMOTING_POSITION_OPERA_TYPE
	{
		RA_PROMOTING_POSITION_OPERA_TYPE_ALL_INFO,
		RA_PROMOTING_POSITION_OPERA_TYPE_PLAY,
		RA_PROMOTING_POSITION_OPERA_FETCH_REWARD,

		RA_PROMOTING_POSITION_OPERA_TYPE_MAX,
	};

	class SCRAPromotingPositionAllInfo	// 2445
	{
	public:
		SCRAPromotingPositionAllInfo();
		MessageHeader header;

		unsigned int next_free_timestamp;
		unsigned short play_times;
		RAPromotingPositionInfo start_pos;

		int reward_flag;
		int records_count;
		RAPromotingPositionRewardRecord record_list[RA_PROMOTION_POSITION_REWARD_RECORDS_MAX_COUNT];
	};

	class SCRAPromotingPositionRewardInfo // 2446
	{
	public:
		SCRAPromotingPositionRewardInfo();
		MessageHeader header;

		int split_position;		// 双开位置
		int reward_count;
		RAPromotingPositionRewardInfo reward_info_list[RA_PROMOTION_POSITION_MULTIPLAY_TIMES * 2];
	};

	/////////////////////////////////////  随机活动 黑市拍卖  ///////////////////////////////////
	enum RA_BLACK_MARKET_OPERA_TYPE
	{
		RA_BLACK_MARKET_OPERA_TYPE_ALL_INFO,
		RA_BLACK_MARKET_OPERA_TYPE_OFFER,

		RA_BLACK_MARKET_OPERA_TYPE_MAX,
	};

	struct SCRABlackMarketAllInfo				// 2447
	{
		struct ItemInfo
		{
			ItemInfo()
			{
				seq = -1;
				cur_price = 0;
				buyer_uid = 0;
				memset(buyer_name, 0, sizeof(buyer_name));
			}

			int seq;						// 物品的配置seq
			int cur_price;					// 当前拍卖价格
			int buyer_uid;					// 竞价玩家UID
			GameName buyer_name;			// 竞价玩家名
		};

	public:
		SCRABlackMarketAllInfo();
		MessageHeader header;

		ItemInfo item_info_list[RA_BLACK_MARKET_MAX_ITEM_COUNT];
	};
	
	/////////////////////////////////////  随机活动 珍宝商城  ///////////////////////////////////
	enum RA_TREASURES_MALL_OPERA_TYPE
	{
		RA_TREASURES_MALL_OPERA_TYPE_REQ_INFO = 0,	// 珍宝商城所有信息
		RA_TREASURES_MALL_OPERA_TYPE_BUY,		// 珍宝商城购买
		RA_TREASURES_MALL_OPERA_TYPE_EXCHANGE,	// 珍宝商城兑换

		RA_TREASURES_MALL_OPERA_TYPE_MAX,
	};

	class SCRATreasuresMallAllInfo				// 珍宝商城所有信息 2448
	{
	public:
		
		SCRATreasuresMallAllInfo();
		MessageHeader header;

		int cur_score;
	};

	/////////////////////////////////////  跨服组队  ///////////////////////////////////
	class CSCrossTeamFBOption					// 跨服组队本操作
	{
	public:
		CSCrossTeamFBOption();
		MessageHeader		header;

		int option_type;
		int layer;
		int room;
		int param;
		int param2;
		int param3;
	};

	class SCCrossTeamFBSelfInfo					// 跨服组队本信息
	{
	public:
		SCCrossTeamFBSelfInfo();
		MessageHeader		header;

		int cross_team_fb_pass_flag;
		int cross_team_fb_day_count;
	};

	/////////////////////////////////////  随机活动 开服活动  ///////////////////////////////////
	enum RA_OPEN_SERVER_OPERA_TYPE
	{
		RA_OPEN_SERVER_OPERA_TYPE_REQ_INFO = 0,				// 请求所有信息
		RA_OPEN_SERVER_OPERA_TYPE_FETCH,					// 领取奖励

		RA_OPEN_SERVER_OPERA_TYPE_REQ_BOSS_INFO,			// 获取boss猎手信息
		RA_OPEN_SERVER_OPERA_TYPE_FETCH_BOSS,				// 领取boss猎手奖励

		RA_OPEN_SERVER_OPERA_TYPE_FETCH_BATTE_INFO,			// 获取开服争霸信息
	};	

	class SCRAOpenServerInfo						// 开服活动信息
	{
	public:
		SCRAOpenServerInfo();
		MessageHeader header;

		int rand_activity_type;
		int reward_flag;
		int complete_flag;
		int today_chongzhi_role_count;				// 首充团购用
	};

	class SCRAOpenServerUpgradeInfo
	{
	public:
		SCRAOpenServerUpgradeInfo();
		MessageHeader header;

		int rand_activity_type;
		int total_upgrade_record_list[MAX_UPGRADE_RECORD_COUNT];
	};

	class SCRAOpenServerRankInfo					// 开服活动排行信息
	{
	public:
		SCRAOpenServerRankInfo();
		MessageHeader header;

		int rand_activity_type;
		int self_rank;
		int top1_uid;
		GameName top1_name;
		char top1_sex;
		char top1_prof;
		short reserve_2;
		int top1_param;
		long long top1_avatar_timestamp;

		int top1_grade;

		int top2_uid;
		GameName top2_name;
		int top2_grade;

		int top3_uid;
		GameName top3_name;
		int top3_grade;

	};

	class SCRAOpenServerBossInfo					// 开服活动boss猎手信息
	{
	public:
		SCRAOpenServerBossInfo();
		MessageHeader header;

		int oga_kill_boss_reward_flag;				// 开服活动 杀boss奖励标记
		long long oga_kill_boss_flag;				// 开服活动 杀boss标记

	};

	class CSRAOpenGameGiftShopBuy					// 6627购买限购礼包
	{
	public:
		CSRAOpenGameGiftShopBuy();
		MessageHeader header;

		int seq;
	};

	class CSRAOpenGameGiftShopBuyInfoReq			// 6628获取限购礼包信息
	{
	public:
		CSRAOpenGameGiftShopBuyInfoReq();
		MessageHeader header;
	};
	
	class SCRAOpenGameGiftShopBuyInfo				// 6626礼包限购信息
	{
	public:
		SCRAOpenGameGiftShopBuyInfo();
		MessageHeader header;

		int oga_gift_shop_flag;						// 礼包购买标志
	};

	class SCRAOpenServerBattleInfo					// 开服活动 开服争霸信息
	{
	public:
		SCRAOpenServerBattleInfo();
		MessageHeader header;

		int qunxian_uid;							// 元素战场
		int guildbatte_uid;							// 公会争霸
		int gongchengzhan_uid;						// 攻城战
		int territorywar_uid;						// 领土战
	};

	class SCBaiBeiFanLiInfo							// 百倍返利信息
	{
	public:
		SCBaiBeiFanLiInfo();
		MessageHeader header;

		int is_buy;
		unsigned int close_time;					// 结束时间戳
	};

	class CSBaiBeiFanLiBuy							// 百倍返利购买物品
	{
	public:
		CSBaiBeiFanLiBuy();
		MessageHeader header;
	};

	class SCBaiBeiFanLi2Info							//8918 百倍返利2信息
	{
	public:
		SCBaiBeiFanLi2Info();
		MessageHeader header;

		int is_buy;
		unsigned int close_time;					// 结束时间戳
	};

	class CSBaiBeiFanLi2Buy							//8919 百倍返利2购买物品
	{
	public:
		CSBaiBeiFanLi2Buy();
		MessageHeader header;
	};

	/////////////////////////////////////  跨服水晶幻境  /////////////////////////////////////
	class SCCrossShuijingPlayerInfo											// 跨服水晶，玩家信息通知 6617
	{
	public:
		SCCrossShuijingPlayerInfo();
		MessageHeader		header;

		int total_bind_gold;												// 总共-绑定元宝
		int total_mojing;													// 总共-魔晶
		int total_cross_honor;												// 总共-跨服荣誉
		int total_relive_times;												// 总共-复活次数
		int cur_limit_gather_times;											// 当前次-剩余的可采集次数
		unsigned int gather_buff_time;										// 采集不被打断buff时间
		int big_shuijing_num;												// 至尊水晶数量
	};

	class CSCrossShuijingBuyBuff											// 跨服水晶幻境，购买buff 6618
	{
	public:
		CSCrossShuijingBuyBuff();
		MessageHeader		header;
	};

	/////////////////////////////////////  水晶幻境  /////////////////////////////////////

	class SCShuijingPlayerInfo												// 水晶，玩家信息通知 6621
	{
	public:
		SCShuijingPlayerInfo();
		MessageHeader		header;

		int total_bind_gold;												// 总共-绑定元宝
		int total_mojing;													// 总共-魔晶
		int total_shengwang;												// 总共-声望
		int free_relive_times;												// 已免费复活次数
		int cur_gather_times;												// 当前采集次数
		unsigned int gather_buff_time;										// 采集不被打断buff时间

		int big_shuijing_num;												// 至尊水晶数量
		unsigned int big_shuijing_next_flush_time;							// 至尊水晶下次刷新时间
	};

	class SCShuijingSceneInfo												// 水晶, 场景信息通知 6635
	{
	public:
		SCShuijingSceneInfo();
		MessageHeader		header;

		int big_shuijing_num;												// 至尊水晶数量
		unsigned int big_shuijing_next_flush_time;							// 至尊水晶下次刷新时间
	};

	class SCShuijingTaskInfo												// 水晶，任务信息通知 6613
	{
	public:
		SCShuijingTaskInfo();
		MessageHeader		header;

		int gather_shuijing_total_num;				// 采集任意水晶次数
		int gather_big_shuijing_total_num;			// 采集任意大水晶次数
		int gather_diamond_big_shuijing_num;		// 采集钻石大水晶次数
		int gather_best_shuijing_count;				// 采集至尊水晶次数
		unsigned int fetch_task_reward_flag;
	};

	class CSShuijingFetchTaskReward					// 水晶幻境，领取任务奖励 6632
	{
	public:
		CSShuijingFetchTaskReward();
		MessageHeader		header;

		int task_id;
	};

	class CSShuijingBuyBuff							// 水晶幻境，购买buff 6622
	{
	public:
		CSShuijingBuyBuff();
		MessageHeader		header;
	};


	class SCNoticeGatherBuffInfo					// 6633广播采集不中断buff信息
	{
	public:
		SCNoticeGatherBuffInfo();
		MessageHeader		header;

		ObjID obj_id;
		short is_gather_wudi;			// 1有buff，0 没有buff
	};

	/////////////////////////////////////  随机活动 集字活动  ///////////////////////////////////
	enum RA_ITEM_COLLECTION_OPERA_TYPE
	{
		RA_ITEM_COLLECTION_OPERA_TYPE_QUERY_INFO = 0,					// 请求活动的信息
		RA_ITEM_COLLECTION_OPERA_TYPE_EXCHANGE,							// 请求兑换
		RA_ITEM_COLLECTION_OPERA_TYPE_MAX,
	};

	/////////////////////////////////////  魔龙秘宝  /////////////////////////////////////
	class CSFetchMagicalPreciousRewardReq	// 6624 请求
	{
	public:
		CSFetchMagicalPreciousRewardReq();
		MessageHeader		header;

		enum OPERATE_TYPE
		{
			FETCH_REWARD = 0,				// 领取奖励 p1:章节内容索引 p2:章节
			FETCH_ALL_INFO,					// 获取全部信息
			CONVERT_CHAPTER_REWARD,			// 兑换章节奖励 p1: 章节 p2:档位序号
			FETCH_CUR_CHAPTER_INFO,			// 获取当前章节信息 p2:章节
		};

		int operater_type;					// 操作类型
		int param_1;						// 
		int param_2;						// 
	};

	class SCSendMagicalPreciousAllInfo			// 6625 全部信息
	{
	public:
		SCSendMagicalPreciousAllInfo();
		MessageHeader		header;

		int current_chaper;										// 当前章节		-2为章节全部完成
		unsigned int chapter_invalid_time;						// 章节失效时间
		unsigned int chapter_fetch_finish_reward_flag;			// 章节完成奖励标志
		unsigned short chapter_score_list[MAGICAL_PRECIOUS_CHAPTER_COUNT]; // 每个章节的积分列表
		char chapter_fetch_reward_flag[MAGICAL_PRECIOUS_CHAPTER_COUNT][MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT];				// 章节积分状态标志，0是未完成，1是已完成且可领取积分，2是已领取积分

		struct ChapterParam
		{
			ChapterParam() : charper(0), charper_index(0), param(0)
			{}

			ChapterParam(int _charper, int _charper_index, long long _param) : charper(_charper), charper_index(_charper_index), param(_param)
			{}

			int charper;
			int charper_index;
			long long param;
		};

		int param_list_len;
		ChapterParam param_list[MAGICAL_PRECIOUS_CHAPTER_COUNT * MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT];
	};

	class SCSendMagicalPreciousCurChapterInfo			// 6634 当前章节信息
	{
	public:
		SCSendMagicalPreciousCurChapterInfo();
		MessageHeader		header;

		int fetch_chapter;										// 领取奖励的章节
		char chapter_fetch_reward_flag[MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT];		// 章节奖励标志
	};

	class SCMagicalPreciousConditionParamChange			// 6636 任务进度改变
	{
	public:
		SCMagicalPreciousConditionParamChange();
		MessageHeader		header;

		int param_list_len;
		unsigned short chapter_score_list[MAGICAL_PRECIOUS_CHAPTER_COUNT]; // 每个章节的积分列表
		char chapter_fetch_reward_flag[MAGICAL_PRECIOUS_CHAPTER_COUNT][MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT];				// 章节积分状态标志，0是未完成，1是已完成且可领取积分，2是已领取积分
		SCSendMagicalPreciousAllInfo::ChapterParam param_list[MAGICAL_PRECIOUS_CHAPTER_COUNT * MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT];
	};

	/////////////////////////////////////  经验炼制  /////////////////////////////////////
	enum RA_EXP_REFINE_OPERA_TYPE
	{
		RA_EXP_REFINE_OPERA_TYPE_BUY_EXP = 0,					// 炼制
		RA_EXP_REFINE_OPERA_TYPE_FETCH_REWARD_GOLD,				// 领取炼制红包
		RA_EXP_REFINE_OPERA_TYPE_GET_INFO,						// 获取信息
		
		RA_EXP_REFINE_OPERA_TYPE_MAX
	};

	class CSRAExpRefineReq										// 6629 经验炼制请求
	{
	public:
		CSRAExpRefineReq();
		MessageHeader header;

		int opera_type;											// 请求类型
	};

	class SCRAExpRefineInfo										// 6630 经验炼制信息
	{
	public:
		SCRAExpRefineInfo();
		MessageHeader header;

		short refine_today_buy_time;							// 每日炼制次数
		short reserve_ch;
		int refine_reward_gold;									// 总奖励金额
	};	

/////////////////////////////////////  红包好礼  /////////////////////////////////////
	class SCRARedEnvelopeGiftInfo		// 8410
	{
	public:
		SCRARedEnvelopeGiftInfo();
		MessageHeader header;

		int consume_gold_num_list[RAND_ACTIVITY_HONGBAO_HAOLI_MAX_COUNT];	// 消费钻石数(0-6天)
		int reward_flag;													// 是否已经领取
	};

	///////////////////////////////////  我们结婚吧  /////////////////////////////////////
	enum RA_MARRYME_OPERA_TYPE
	{
		RA_MARRYME_REQ_INFO,
	};

	class SCRAMarryMeAllInfo			// 8415
	{
	public:
		SCRAMarryMeAllInfo();
		MessageHeader header;

		struct CopleDetail
		{
			CopleDetail()
			{
				propose_id = 0;
				memset(propose_name, 0, sizeof(propose_name));

				accept_proposal_id = 0;
				memset(accept_proposal_name, 0, sizeof(accept_proposal_name));

				proposer_sex = -1;
				accept_proposal_sex = -1;
			}

			int propose_id;									//求婚者id
			GameName propose_name;							//求婚者名字

			int accept_proposal_id;							//被求婚者id
			GameName accept_proposal_name;					//被求婚者名字

			char proposer_sex;								//求婚者性别
			char accept_proposal_sex;						//被求婚者性别
			short reserve_sh;
		};

		int cur_couple_count;
		CopleDetail counp_list[RA_MARRY_SHOW_COUPLE_COUNT_MAX];
	};

	/////////////////////////////////////  充值-七日返利  /////////////////////////////////////
	class CSChongZhi7DayFetchReward		// 8420
	{
	public:
		CSChongZhi7DayFetchReward();
		MessageHeader header;
	};

	/////////////////////////////////////  金猪召唤		////////////////////////////////////

	class CSGoldenPigOperateReq								//8428 金猪召唤操作请求
	{
	public:
		CSGoldenPigOperateReq();
		MessageHeader header;

		enum GOLDEN_PIG_OPERATE_TYPE
		{
			GOLDEN_PIG_OPERATE_TYPE_REQ_INFO = 0,			// 请求信息
			GOLDEN_PIG_OPERATE_TYPE_SUMMON,					// 召唤
		};

		enum GOLDEN_PIG_SUMMON_TYPE
		{
			GOLDEN_PIG_SUMMON_TYPE_JUNIOR = 0,				// 初级召唤
			GOLDEN_PIG_SUMMON_TYPE_MEDIUM,					// 中级召唤
			GOLDEN_PIG_SUMMON_TYPE_SENIOR,					// 高级召唤

			GGOLDEN_PIG_SUMMON_TYPE_MAX,
		};

		short operate_type;									// 操作类型
		short param;										// 召唤参数
	};

	class SCGoldenPigSendInfo								//8429 金猪召唤积分信息
	{
	public:
		SCGoldenPigSendInfo();
		MessageHeader header;

		int summon_credit;									// 召唤积分
		int current_chongzhi;								// 当前充值
		unsigned int activity_end_time;						// 活动结束时间
	};

	class SCGoldenPigBossState													// 8435 金猪召唤boss状态
	{
	public:
		SCGoldenPigBossState();
		MessageHeader header;

		char boss_state[CSGoldenPigOperateReq::GGOLDEN_PIG_SUMMON_TYPE_MAX];	// boss状态 0不存在,1存在
		char reserve_ch;
	};

	////////////////////////////////////  开服投资	////////////////////////////////////

	enum RA_OPEN_SERVER_INVEST_TYPE
	{
		RA_OPEN_SERVER_INVEST_TYPE_INFO_REQ = 0,			// 请求信息
		RA_OPEN_SERVER_INVEST_TYPE_INVEST,					// 投资
		RA_OPEN_SERVER_INVEST_TYPE_FECTH_REWARD,			// 领取奖励
	};

	static const int MAX_INVEST_TYPE = 3;

	class SCOpenServerInvestInfo							// 8417 开服投资信息
	{
	public:
		SCOpenServerInvestInfo();
		MessageHeader header;

		int reward_flag;									// 奖励信息

		unsigned int time_limit[MAX_INVEST_TYPE];			// 限制时间
		char finish_param[MAX_INVEST_TYPE];					// 投资完成条件参数
		char reserve_ch;
	};

	/////////////////////////////////// 单返豪礼 //////////////////////////
	enum RA_SINGLE_CHONGZHI_OPERA_TYPE
	{
		RA_SINGLE_CHONGZHI_OPERA_TYPE_INFO = 0,				// 请求信息
		RA_SINGLE_CHONGZHI_OPERA_TYPE_FETCH_REWARD,			// 领取奖励
		RA_SINGLE_CHONGZHI_OPERA_TYPE_MAX,
	};

	class SCRASingleChongZhiInfo		// 2456
	{
	public:
		SCRASingleChongZhiInfo();
		MessageHeader header;

		int fetch_reward_flag;
		int is_fetch_reward_flag;
	};

	/////////////////////////////////////  聚划算  /////////////////////////////////////
	static const int SC_RA_XIANYUAN_TREAS_MAX_COUNT_LIMIT = 10;

	enum RA_XIANYUAN_TREAS_OPERA_TYPE
	{
		RA_XIANYUAN_TREAS_OPERA_TYPE_QUERY_INFO = 0,			// 请求活动信息
		RA_XIANYUAN_TREAS_OPERA_TYPE_BUY,						// 单个购买请求
		RA_XIANYUAN_TREAS_OPERA_TYPE_BUY_ALL,					// 全部购买请求
		RA_XIANYUAN_TREAS_OPERA_TYPE_FETCH_REWARD,				// 领取奖励

		RA_XIANYUAN_TREAS_OPERA_TYPE_MAX,
	};

	class SCRAXianyuanTreasInfo				// 2457
	{
	public:
		SCRAXianyuanTreasInfo();
		MessageHeader header;

		int all_buy_gift_fetch_flag;
		XianyuanTreasInfo xianyuan_list[RAND_ACTIVITY_XIANYUAN_TREAS_COUNT];
	};

	/////////////////////////////////////  限时秒杀  ///////////////////////////////////
	enum RA_RUSH_BUYING_OPERA_TYPE
	{
		RA_RUSH_BUYING_OPERA_TYPE_QUERY_ALL_INFO,			// 请求所有信息
		RA_RUSH_BUYING_OPERA_TYPE_BUY_ITEM,					// 抢购物品

		RA_RUSH_BUYING_OPERA_TYPE_MAX,
	};

	class SCRARushBuyingAllInfo
	{
	public:
		struct ItemBuyTimes
		{
			ItemBuyTimes() : server_buy_times(0), role_buy_times(0) {}

			short server_buy_times;
			short role_buy_times;
		};

	public:
		SCRARushBuyingAllInfo();
		MessageHeader header;

		unsigned int buy_end_timestamp;

		short reserve_sh;
		char buy_phase;
		char item_count;

		ItemBuyTimes item_buy_times_list[RA_RUSH_BUYING_SALES_ITEMS_COUNT];
	};

	/////////////////////////////////////  随机活动 地图寻宝  ///////////////////////////////////
	enum RA_MAP_HUNT_OPERA_TYPE
	{
		RA_MAP_HUNT_OPERA_TYPE_ALL_INFO = 0,		//请求所有信息
		RA_MAP_HUNT_OPERA_TYPE_FLUSH,				//请求刷新
		RA_MAP_HUNT_OPERA_TYPE_AUTO_FLUSH,			//请求自动刷新
		RA_MAP_HUNT_OPERA_TYPE_HUNT,				//寻宝
		RA_MAP_HUNT_OPERA_TYPE_FETCH_RETURN_REWARD,	//拿取返利奖励

		RA_MAP_HUNT_OPERA_TYPE_MAX
	};

	class SCRAMapHuntAllInfo						//2459
	{
	public:
		SCRAMapHuntAllInfo();
		MessageHeader header;

		RAMapHuntRouteInfo route_info;				//路线信息
		int flush_times;							//刷新次数
		unsigned int next_flush_timestamp;			//下次刷新时间
		short return_reward_fetch_flag;				//奖励领取标志
		short cur_has_used_free_count;				//当前已使用免费次数
		int can_extern_reward;						//路线索引
	};

	/////////////////////////////////////  随机活动 灯塔寻宝  ///////////////////////////////////
	enum RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE
	{
		RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_INFO = 0,					//请求所有信息
		RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_DRAW,						//抽奖
		RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_FETCH_EXTERN_REWARD,		//拿取层额外奖励
		RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_FETCH_SERVER_REWARD,		//拿取全服奖励

		RA_LIGHT_TOWER_EXPLORE_OPERA_TYPE_MAX
	};

	class SCRALightTowerExploreInfo			// 2460 灯塔寻宝所有信息
	{
	public:
		SCRALightTowerExploreInfo();
		MessageHeader header;

		char light_tower_info[RA_LIGHT_TOWER_EXPLORE_MAX_LAYER];
		char extern_reward_fetch_flag;
		char server_reward_fetcht_flag;
		char cur_index;

		short cur_layer;
		short reserver_sh;
		int draw_times;
	};

	class SCRALightTowerExploreDrawInfo		// 2461 灯塔抽奖所有信息
	{
	public:
		SCRALightTowerExploreDrawInfo();
		MessageHeader header;

		int reward_count;
		LightTowerExploreGridInfo reward_list[MAX_LIGHT_TOWER_EXPLORE_REWARD_COUNT];
	};

	/////////////////////////////////////  随机活动 新累计充值活动  /////////////////////////////////////
	enum RA_NEW_TOTAL_CHARGE_OPERA_TYPE
	{
		RA_NEW_TOTAL_CHARGE_OPERA_TYPE_QUERY_INFO = 0,
		RA_NEW_TOTAL_CHARGE_OPERA_TYPE_FETCH_REWARD,

		RA_NEW_TOTAL_CHARGE_OPERA_TYPE_MAX,
	};

	class SCRANewTotalChargeInfo	//2262
	{
	public:
		SCRANewTotalChargeInfo();
		MessageHeader header;

		int total_charge_value;								// 累计充值数
		int reward_has_fetch_flag;							// 已领取过的奖励标记
	};

	class SCRATotalChargeMultiInfo	//2286
	{
	public:
		SCRATotalChargeMultiInfo();
		MessageHeader header;

		int activity_type;
		int total_charge_value;								// 累计充值数
		int reward_has_fetch_flag;							// 已领取过的奖励标记
	};

	/////////////////////////////////////  随机活动 双倍元宝  /////////////////////////////////////

	class SCRADoubleGetInfo	//2287
	{
	public:
		SCRADoubleGetInfo();
		MessageHeader header;

		int double_get_reward_fetch_flag;							// 已获得奖励标记
	};

	class SCSingleChargeInfoMulti // 2288
	{
	public:
		SCSingleChargeInfoMulti();
		MessageHeader hearder;

		int activtiy_type;
		int single_charge_reward_fetch_flag; // 已领取过奖励标记
	};
	/////////////////////////////////////  随机活动 幻装商店活动  /////////////////////////////////////
	enum RA_MAGIC_SHOP_OPERA_TYPE
	{
		RA_MAGIC_SHOP_OPERA_TYPE_ALL_INFO,				//返回信息
		RA_MAGIC_SHOP_OPERA_TYPE_BUY,			        //购买 param1 = 类型 (0 买一个， 1一键购买) param2 = 索引

		RA_MAGIC_SHOP_OPERA_TYPE_MAX,
	};

	class SCRAMagicShopAllInfo			// 2463
	{
	public:
		SCRAMagicShopAllInfo();
		MessageHeader header;

		char magic_shop_buy_flag;
		char res_ch;
		short activity_day;
	};

	/////////////////////////////////////  随机活动 趣味植树  ///////////////////////////////////
	enum RA_PLANTING_TREE_OPERA_TYPE
	{
		RA_PLANTING_TREE_OPERA_TYPE_RANK_INFO,				// 请求排行榜信息
		RA_PLANTING_TREE_OPERA_TYPE_TREE_INFO,				// 请求一颗树的信息
		RA_PLANTING_TREE_OPERA_MINI_TYPE_MAP_INFO,			// 请求小地图树的信息
	};

	class SCPlantingTreeRankInfo         // 8448
	{
	public:
		SCPlantingTreeRankInfo();
		MessageHeader header;

		int rank_type;
		int opera_times;

		int rank_list_count;
		RAPlantingTreeRankInfo rank_list[RA_TREE_PLANTING_RANK_MAX_COUNT];
	};

	class SCPlantingTreeTreeInfo         // 8449
	{
	public:
		SCPlantingTreeTreeInfo();
		MessageHeader header;

		GameName owner_name;
		unsigned int vanish_time;
		int watering_times;
	};

	static const int PLANTING_TREE_MINI_MAP_MAX_TREE_COUNT = 5;
	class SCPlantingTreeMiniMapInfo             // 8450
	{
		struct TreeInfo
		{
			int obj_id;
			short pos_x;
			short pos_y;
		};

	public:
		SCPlantingTreeMiniMapInfo();
		MessageHeader header;

		int tree_count;
		TreeInfo tree_info_list[PLANTING_TREE_MINI_MAP_MAX_TREE_COUNT];
	};

	/////////////////////////////////////////////// 钓鱼 //////////////////////////////////////////////
	enum RA_FISHING_OPERA_TYPE
	{
		RA_FISHING_REQ,							//请求钓鱼
		RA_FISHING_ACK,							//请求钓鱼返回
		RA_FISHING_STEAL,						//请求偷鱼
		RA_FISHING_STEAL_ACK,					//偷鱼返回
		RA_FISHING_EXCHANGE,					//请求兑换鱼奖励
		RA_FISHING_CANCEL_AUTO_FISH,			//取消自动钓鱼
		RA_FISHING_EXIT_FISHING,				//退出钓鱼
		RA_FISHING_ALL_INFO,					//钓鱼所有信息

		RA_FISHING_MAX_TYPE,
	};

	class SCRAFishAllinfo
	{
	public:
		SCRAFishAllinfo();
		MessageHeader header;

		short fish_times;							//钓鱼的次数
		short steal_fish_times;
		short be_steal_fish_times;
		short steal_fish_type;
		short fish_score;
		short steal_fish_score;
		short fish_type;							//每次调到的鱼类型
		short fish_info[RA_FISHING_FISH_TYPE_COUNT + 1];
	};

	class SCRAFishStatusInfo
	{
	public:
		SCRAFishStatusInfo();
		MessageHeader header;

		char fish_status;
		char reserve_ch;
		ObjID obj_id;
	};

	////////////////////////////////////////////////// 达拉然水晶 /////////////////////////////////////
	enum RA_STEAL_COIN_OPERA_TYPE
	{
		RA_STEAL_CRTSTAL_DALARAN_REQ_INFO,
		RA_STEAL_CRYSTAL_DALARAN_REQ,
	};

	class SCRAStealCrystalDalatanInfo
	{
	public:
		SCRAStealCrystalDalatanInfo();
		MessageHeader header;

		unsigned int steal_count;
		unsigned int be_steal_count;
	};

	/////////////////////////////////////////////////狂返元宝////////////////////////////////////////////////
	class SCRaCrazyRebateChongInfo		// 8463
	{
	public:
		SCRaCrazyRebateChongInfo();
		MessageHeader header;

		int chongzhi_count;
	};

	class SCLoveDailyInfo				// 8464
	{
	public:
		SCLoveDailyInfo();
		MessageHeader header;

		int flag;
	};

	////////////////////////////////////////////////每日一爱2(单笔返利)/////////////////////////////////////////
	class SCLoveDaily2Info				// 8923
	{
	public:
		SCLoveDaily2Info();
		MessageHeader header;

		int flag;
	};

	///////////////////////////////////////// 月黑风高 ///////////////////////////////////////////////////////
	class SCCrossDarkNightUserInfo      // 5770
	{
	public:
		SCCrossDarkNightUserInfo();
		MessageHeader header;

		struct RewardItem
		{
			RewardItem():item_id(0),num(0){}
			ItemID item_id;
			short num;
		};

		struct RewardInfo
		{
			RewardItem reward_item;
			RewardItem reward_item2;
		};

		int score;
		int box_count;
		int total_reward_box_count;

		short is_finish;
		short reward_count;
		RewardInfo info_list[CROSS_DARK_NIGHT_BOSS_MAX_COUNT];
	};

	class SCCrossDarkNightRankInfo  //5771
	{
	public:
		SCCrossDarkNightRankInfo();
		MessageHeader header;

		struct RankItem
		{
			GameName name;
			int rank_val;
		};

		int rank_count;
		RankItem rank_list[CROSS_DARK_NIGHT_RANK_COUNT];
	};

	class SCCrossDarkNightBossInfo   //5772
	{
	public:
		SCCrossDarkNightBossInfo();
		MessageHeader header;

		struct BossInfo
		{
			int monster_id;
			Posi posi;
			long long max_hp;
			long long cur_hp;
			short boss_status;
			short reserve_sh;
		};

		BossInfo boss_info[CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX];
	};

	class SCCrossDarkNightPlayerInfoBroadcast  //5773
	{
	public:
		SCCrossDarkNightPlayerInfoBroadcast();
		MessageHeader header;

		ObjID obj_id;
		short box_count;
	};

	class SCCrossDarkNightRewardTimestampInfo //5774
	{
	public:
		SCCrossDarkNightRewardTimestampInfo();
		MessageHeader header;

		unsigned int next_check_reward_timestamp;
	};

	class CSCrossDarkNightRankOpera  //5776
	{
	public:
		CSCrossDarkNightRankOpera();
		MessageHeader header;

		int opera_type;
	};

	class SCCrossDarkNightTopPlayerPosi  //5775
	{
	public:
		SCCrossDarkNightTopPlayerPosi();
		MessageHeader header;

		int obj_id;
		Posi posi;
	};

	//////////////////////////////////////////////仙尊卡//////////////////////////////////////////////////

	class CSFairyBuddhaCardActivateReq                                      // 仙尊卡接任务请求 6855
	{
	public:
		CSFairyBuddhaCardActivateReq();
		MessageHeader header;
		short card_type;
		short reserve_sh;
	};

	class SCFairyBuddhaCardActivateInfo                                      // 仙尊卡信息 6856
	{
	public:
		SCFairyBuddhaCardActivateInfo();
		MessageHeader header;

		unsigned int bronze_timestamp;       // 青铜卡时间戳
		unsigned int silver_timestamp;       // 白银卡时间戳
		unsigned int jewel_timestamp;        // 钻石卡时间戳
		short is_forever_open;               // 是否永久激活
		short fairy_buddha_card_is_activate; // 仙尊卡是否激活
		int   gold_bind_is_get_flage;        // 每日绑元是否领取
	};

	class CSFairyBuddhaCardGoldBindReq                                      // 仙尊卡领取每日绑元 6857
	{
	public:
		CSFairyBuddhaCardGoldBindReq();
		MessageHeader header;
		short card_type;
		short reserve_sh;
	};

	/////////////////////////////////////////////////   帮派答题  /////////////////////////////////////////////////////////
	class CSGuildQuestionEnterReq		// 6860 活动进入请求
	{
	public:
		CSGuildQuestionEnterReq();
		MessageHeader header;
	};

	class SCGuildQuestionPlayerInfo		// 6861 玩家信息
	{
	public:
		SCGuildQuestionPlayerInfo();
		MessageHeader	header;

		int uid;
		GameName name;
		long long exp;
		int guild_gongxian;
		int guild_score;
		int gather_count;			// 采集次数

		int true_uid;
		GameName true_name;
	};

	class SCGuildQuestionGuildRankInfo	// 6862 帮派排行信息
	{
	public:
		SCGuildQuestionGuildRankInfo();
		MessageHeader		header;

		struct RankInfo
		{
			RankInfo() : guild_id(0), guild_score(0)
			{
				memset(guild_name, 0, sizeof(guild_name));
			}

			GuildID guild_id;					// 帮派ID
			GuildName guild_name;				// 帮派名字
			int guild_score;					// 帮派积分
		};

		int rank_count;
		RankInfo guild_rank_list[GUILD_QUESTION_SCORE_RANK_MAX];
	};

	class SCGuildQuestionQuestionInfo  // 6863 题目信息
	{
	public:
		SCGuildQuestionQuestionInfo();
		MessageHeader		header;

		void Reset()
		{
			question_state = 0;
			question_state_change_timestamp = 0;
			question_index = 0;
			question_id = 0;
			question_end_timestamp = 0;
			memset(question_str, 0, sizeof(question_str));
		}

		int question_state;								// 0：准备中；1：开始了；2：将要结束
		unsigned int question_state_change_timestamp;	// 答题状态切换时间戳

		int question_index;						// 第几题
		int question_id;						// 题目ID
		unsigned int question_end_timestamp;	// 本道题目结束时间戳
		GuildQuestionStr question_str;			// 题目	
	};

	/////////////////////////////////////////////////  钓鱼活动  /////////////////////////////////////////////////////////
	enum FISHING_OPERA_REQ_TYPE
	{
		FISHING_OPERA_REQ_TYPE_START_FISHING,					// 开始钓鱼（进入钓鱼界面）
		FISHING_OPERA_REQ_TYPE_CASTING_RODS,					// 抛竿
		FISHING_OPERA_REQ_TYPE_PULL_RODS,						// 收竿
		FISHING_OPERA_REQ_TYPE_CONFIRM_EVENT,					// 确认本次钓鱼事件
		FISHING_OPERA_REQ_TYPE_USE_GEAR,						// 使用法宝
		FISHING_OPERA_REQ_TYPE_BIG_FISH_HELP,					// 帮忙拉大鱼
		FISHING_OPERA_REQ_TYPE_STOP_FISHING,					// 停止钓鱼（离开钓鱼界面）
		FISHING_OPERA_REQ_TYPE_AUTO_FISHING,					// 自动钓鱼
		FISHING_OPERA_REQ_TYPE_RAND_USER,						// 随机角色请求
		FISHING_OPERA_REQ_TYPE_BUY_STEAL_COUNT,					// 购买偷鱼次数
		FISHING_OPERA_REQ_TYPE_RANK_INFO,					    // 请求钓鱼排行榜信息
		FISHING_OPERA_REQ_TYPE_STEAL_FISH,						// 偷鱼请求
		FISHING_OPERA_REQ_TYPE_EXCHANGE,						// 兑换请求 param1：兑换组合下标
		FISHING_OPERA_REQ_TYPE_BUY_BAIT,						// 购买鱼饵 param1: 购买鱼饵类型
		FISHING_OPERA_REQ_TYPE_SCORE_REWARD,					// 积分奖励
	};

	class CSFishingOperaReq   // 8777 请求协议
	{
	public:
		CSFishingOperaReq();
		MessageHeader header;

		int opera_type;				//操作类型
		int param1;					//参数1
		int param2;					//参数2
	};

	class SCFishingUserInfo  // 8778  玩家信息
	{
	public:
		SCFishingUserInfo();
		MessageHeader header;

		Int64 uuid;									// 玩家唯一id
		char fishing_status;						// 钓鱼状态
		unsigned char special_status_flag;			// 特殊状态（自动钓鱼、使用法宝等）
		char least_count_cfg_index;					// 双倍积分最低钓鱼数量配置索引
		char is_fish_event;							// 是否有鱼儿上钩
		char is_consumed_auto_fishing;				// 是否消费过自动钓鱼
		char reserve_ch[3];							// 保留3个char的长度

		unsigned int auto_pull_timestamp;			// 自动拉杆时间戳，没有触发事件则为0
		unsigned int special_status_oil_end_timestamp;	// 特殊状态香油结束时间戳

		int fish_num_list[FISHING_FISH_TYPE_MAX_COUNT];	// 鱼儿数量列表
		int gear_num_list[FISHING_GEAR_MAX_COUNT];		// 法宝数量列表

		int steal_fish_count;						// 偷鱼次数
		int be_stealed_fish_count;					// 被偷鱼次数
		int buy_steal_count;						// 购买偷鱼次数

		int news_count;								// 事件数量
		FishingNews news_list[FISHING_NEWS_MAX];	// 事件列表
	};

	class SCFishingCheckEventResult	//8779  事件信息返回
	{
	public:
		SCFishingCheckEventResult();
		MessageHeader header;

		short event_type;							// 事件类型
		short param1;
		short param2;
		short param3;
	};

	class SCFishingGearUseResult  //8780 使用法宝结果信息返回
	{
	public:
		SCFishingGearUseResult();
		MessageHeader header;

		short gear_type;				// 法宝类型
		short param1;
		short param2;
		short param3;
	};

	class SCFishingEventBigFish //8781	传说中的大鱼
	{
	public:
		SCFishingEventBigFish();
		MessageHeader header;

		int owner_uid;					// 传说中的大鱼的玩家id
	};

	class SCFishingTeamMemberInfo	//8782		队伍成员信息
	{
	public:
		SCFishingTeamMemberInfo();
		MessageHeader header;

		int member_count;				// 队伍人数

		int member_uid_1;				// 队伍玩家1角色id
		int least_count_cfg_index_1;	// 玩家1的双倍积分配置下标
		int fish_num_list_1[FISHING_FISH_TYPE_MAX_COUNT];	//鱼儿数量列表

		int member_uid_2;
		int least_count_cfg_index_2;
		int fish_num_list_2[FISHING_FISH_TYPE_MAX_COUNT];

		int member_uid_3;
		int least_count_cfg_index_3;
		int fish_num_list_3[FISHING_FISH_TYPE_MAX_COUNT];
	};

	class SCFishingFishInfo		//8783						// 玩家信息（发给队伍）
	{
	public:
		SCFishingFishInfo();
		MessageHeader header;

		int uid;											// 玩家id
		int least_count_cfg_index;							// 双倍积分配置下标
		int fish_num_list[FISHING_FISH_TYPE_MAX_COUNT];		// 鱼儿数量列表
	};

	class SCFishingRandUserInfo		//8784		随机玩家信息
	{
	public:
		SCFishingRandUserInfo();
		MessageHeader header;

		int user_count;												// 玩家数量
		FishingRandUserInfo user_info_list[FISHING_RAND_ROLE_NUM];	// 玩家列表
	};

	class SCFishingScoreInfo	//8785
	{
	public:
		SCFishingScoreInfo();
		MessageHeader header;

		int fishing_score;					// 钓鱼积分
	};

	class SCFishingStealResult		//8786		偷鱼结果信息
	{
	public:
		SCFishingStealResult();
		MessageHeader header;

		short is_succ;						// 是否成功
		short fish_type;					// 鱼儿类型
		int fish_num;						// 鱼儿数量
	};

	class SCFishingGetFishBrocast	//8787	// 钓鱼广播信息
	{
	public:
		SCFishingGetFishBrocast();
		MessageHeader header;

		int uid;							// 玩家uid
		int get_fish_type;					// 获得鱼儿种类
	};

	class SCFishingScoreRankList		// 8788		// 钓鱼积分排行信息
	{
	public:
		struct FishRankItem
		{
			GameName name;
			long long uid;							// 玩家id
			int total_score;						// 总积分
		};

		SCFishingScoreRankList();
		MessageHeader		header;

		int fish_rank_count;
		FishRankItem fish_rank_list[FISHING_SCORE_MAX_RANK];
	};

	class SCFishingScoreStageInfo	// 8789 当前积分阶段信息
	{
	public:
		SCFishingScoreStageInfo();
		MessageHeader header;

		int cur_score_stage;			//当前阶段
		int fishing_score;				//当前钓鱼积分
	};

	class SCFishingNotifyStatusChange		// 8790
	{
	public:
		SCFishingNotifyStatusChange();
		MessageHeader header;

		long long uuid;
		int obj_id;											// 对象obj id
		int status;											// 状态
		int	be_stealed_fish_count;							// 被偷鱼数量
		int	fish_num_list[FISHING_FISH_TYPE_MAX_COUNT];		// 当前钓上的各类鱼的数量
	};

	struct SCFishingStealInfo		// 8791
	{
		SCFishingStealInfo();
		MessageHeader header;

		GameName thief_name;		// 盗贼名字
		GameName be_stolen_name;	// 被偷名字
		short fish_type;			// 被偷鱼类型
		short fish_num;				// 被偷鱼的数量
	};

	struct SCFishingConfirmResult	// 8792
	{
		SCFishingConfirmResult();
		MessageHeader header;

		short confirm_type;
		unsigned short param_1;
		short param_2;
		short param_3;
	};

	////////////////////////////////////// 夜战王城 /////////////////////////////////////////////

	class SCNightFightRoleInfo           // 8795 夜战王城人物信息
	{
	public:
		SCNightFightRoleInfo();
		MessageHeader header;

		int turn;
		int score;								// 积分
		int total_score;						// 总积分
		int is_red_side;						// 是否是红方
		int rank;								// 排行
		int total_rank;							// 总排行
		int kill_role_num;						// 击杀其他玩家数量
		unsigned int next_redistribute_time;	// 下次发奖励 并 重新分配时间戳
		unsigned int next_get_score_time;		// 下次获取积分时间戳
		unsigned int next_update_rank_time;		// 下次更新排行时间戳
		unsigned int kick_out_time;				// 延迟踢出时间
		unsigned int next_flush_boss_time;		// 下次刷新boss时间戳
		int is_finish;							// 结束标记 0，没结束 1，结束
	};

	enum NIGHT_FIGHT_OPERA_TYPE
	{
		NIGHT_FIGHT_OPERA_TYPE_ENTER,			// 进入请求
		NIGHT_FIGHT_OPERA_TYPE_POSI_INFO,		// 获取坐标信息  param_1:rank
	};

	class CSNightFightOperaReq			  // 8796 夜战王城请求
	{
	public:
		CSNightFightOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
	};

	class SCNightFightRankInfo           // 8797 夜战王城排行信息
	{
	public:
		SCNightFightRankInfo();
		MessageHeader header;

		const static int RANK_ITEM_MAX_COUNT = 10;

		struct Rank
		{
			int score;
			GameName user_name;
			ObjID obj_id;
			short is_red_side;
			long long user_key;
		};

		int rank_count;
		Rank rank_info_list[RANK_ITEM_MAX_COUNT];
	};

	class SCNightFightBossRankInfo           // 8802 夜战王城boss排行信息
	{
	public:
		SCNightFightBossRankInfo();
		MessageHeader header;

		const static int BOSS_RANK_ITEM_MAX_COUNT = 20;

		struct BossRank
		{
			long long user_key;
			char is_red_side;			// 阵营
			char reserve_ch;
			short reserve_sh;
			GameName user_name;
			int hurt_per;				// 玩家伤害万分比
			long long hurt_val;			// 玩家伤害值
		};

		int boss_left_hp_per;										// boss剩余血量万分比

		int boss_rank_count;
		BossRank boss_rank_info_list[BOSS_RANK_ITEM_MAX_COUNT];
	};

	class SCNightFightReward			  // 8798 夜战王城排名奖励信息
	{
	public:
		SCNightFightReward();
		MessageHeader header;

		const static int MAX_RANK_COUNT = 16;

		int reward_list[MAX_RANK_COUNT];
	};

	class SCNightFightRedSideListInfo	  // 8799 夜战王城红方人物obj_id列表
	{
	public:
		SCNightFightRedSideListInfo();
		MessageHeader header;

		const static int RED_SIDE_MEMBER_MAX_COUNT = 25;

		int red_side_count;
		ObjID red_side_list[RED_SIDE_MEMBER_MAX_COUNT];
	};

	class SCNightFightAllRoleScoreInfo	  // 8800 乱斗战场人员积分信息
	{
	public:
		SCNightFightAllRoleScoreInfo();
		MessageHeader header;

		const static int RANK_ITEM_MAX_COUNT = 64;

		struct RoleScoreInfo
		{
			ObjID obj_id;
			short score;
		};

		int role_count;
		RoleScoreInfo score_info_list[RANK_ITEM_MAX_COUNT];
	};
	
	class SCNightFightPlayerPosi  //8801 坐标信息
	{
	public:
		SCNightFightPlayerPosi();
		MessageHeader header;

		short rank;
		ObjID obj_id;
		Posi posi;
	};

	class SCNightFightTotalScoreRank  //8803 夜战王城总积分排行榜
	{
	public:
		SCNightFightTotalScoreRank();
		MessageHeader header;

		const static int TOTAL_SOCRE_RANK_MAX = 10;

		struct TotalRank
		{
			int total_score;
			GameName user_name;
			long long user_key;
		};

		int rank_count;
		TotalRank total_rank_list[TOTAL_SOCRE_RANK_MAX];

	};


	////////////////////////////////////// 乱斗战场 /////////////////////////////////////////////

	class SCMessBattleRoleInfo           // 8560 乱斗战场人物信息
	{
	public:
		SCMessBattleRoleInfo();
		MessageHeader header;

		int turn;								// 第几轮
		int score;								// 积分
		int rank;								// 排行
		int total_score;						// 总积分
		int total_rank;							// 总排行
		int boss_hp_per;						// boss血量百分比
		unsigned int next_redistribute_time;	// 下次发奖励 并 重新分配时间戳
		unsigned int next_get_score_time;		// 下次获取积分时间戳
		unsigned int next_update_rank_time;		// 下次更新排行时间戳
		unsigned int kick_out_time;				// 延迟踢出时间
		int is_finish;							// 活动结束标志 0 结束，1 未结束
		long long boss_max_hp;					// boss最大血量
		long long boss_cur_hp;					// boss当前血量
	};

	class CSMessBattleEnterReq			  // 8561 乱斗战场进入请求
	{
	public:
		CSMessBattleEnterReq();
		MessageHeader header;
	};

	class SCMessBattleRankInfo           // 8562 乱斗战场积分排行信息
	{
	public:
		SCMessBattleRankInfo();
		MessageHeader header;

		const static int RANK_ITEM_MAX_COUNT = 64;

		struct Rank
		{
			int score;
			GameName user_name;
			long long user_key;
		};

		int rank_count;
		Rank rank_info_list[RANK_ITEM_MAX_COUNT];
	};

	class SCMessBattleReward			  // 8563 乱斗战场奖励排行信息
	{
	public:
		SCMessBattleReward();
		MessageHeader header;

		const static int MAX_RANK_COUNT = 16;

		int reward_list[MAX_RANK_COUNT];
	};

	class SCMessBattleHurtRankInfo	  // 8564 乱斗战场伤害排行信息
	{
	public:
		SCMessBattleHurtRankInfo();
		MessageHeader header;

		const static int RANK_ITEM_MAX_COUNT = 64;

		struct Rank
		{
			int hurt_per;
			GameName user_name;
			long long user_key;
		};

		int rank_count;
		Rank rank_info_list[RANK_ITEM_MAX_COUNT];
	};

	class SCMessBattleAllRoleScoreInfo	  // 8565 乱斗战场人员积分信息
	{
	public:
		SCMessBattleAllRoleScoreInfo();
		MessageHeader header;

		const static int RANK_ITEM_MAX_COUNT = 64;

		struct RoleScoreInfo
		{
			ObjID obj_id;
			short score;
		};

		int role_count;
		RoleScoreInfo score_info_list[RANK_ITEM_MAX_COUNT];
	};

	class SCMessBattleToalScoreRank    // 8566 乱斗战场总积分排行榜
	{
	public:
		SCMessBattleToalScoreRank();
		MessageHeader header;
		const static int  TOTAL_SOCRE_RANK_MAX = 10;
		struct TotalScore
		{
			int total_score;
			GameName user_name;
			long long user_key;
		};
		int rank_count;
		TotalScore total_score_list[TOTAL_SOCRE_RANK_MAX];
	};

	////////////////////////////////////// 跨服猎鲲地带 /////////////////////////////////////////////
	class SCCrossLieKunFBSceneInfo			//  8575 跨服猎鲲地带场景信息
	{
	public:
		SCCrossLieKunFBSceneInfo();
		MessageHeader header;

		struct BossInfo
		{
			int boss_id;												// bossid
			ObjID boss_objid;											// boss的objid
			short reserve_sh;											// 保留
		};

		short zone;														// 区域
		short reserve_sh;												// 保留
		BossInfo boss_info_list[LIEKUN_BOSS_TYPE_COUNT];				// boss信息
		int guild_id[LIEKUN_BOSS_TYPE_COUNT];							// 归属帮派id
		unsigned int boss_next_flush_timestamp[LIEKUN_BOSS_TYPE_COUNT];	// boss下次刷新时间
	};

	class SCCrossLieKunFBLiveFlagInfo		//  8576 跨服猎鲲地带主boss存活信息
	{
	public:
		SCCrossLieKunFBLiveFlagInfo();
		MessageHeader header;

		short zone;														// 区域
		char is_main_live_flag;											// 主boss存活标记
		char reserve_ch;												// 保留
	};

	class SCCrossLieKunFBBossHurtInfo	//8577 跨服猎鲲地带，BOSS伤害信息
	{
	public:
		SCCrossLieKunFBBossHurtInfo();
		MessageHeader header;
		static const int BOSS_HURT_INFO_RANK_MAX = 6;
		struct HurtInfo
		{
			long long guild_id;
			GuildName guild_name;
			long long hurt;
		};

		int boss_id;
		int own_guild_rank;	//自己仙盟排名
		long long own_guild_hurt;//自己仙盟伤害
		int count;
		HurtInfo info_list[BOSS_HURT_INFO_RANK_MAX];
	};

	class SCRAPerfectLoverInfo   
	{
	public:
		SCRAPerfectLoverInfo();
		MessageHeader header;

		void Reset() 
		{
			my_rank = 0;
			memset(lover_name, 0, sizeof(lover_name));

			perfect_lover_type_record_flag = 0;
			ra_perfect_lover_count = 0;
			memset(ra_perfect_lover_name_list, 0, sizeof(ra_perfect_lover_name_list));
		}

		int my_rank;
		GameName lover_name;

		short perfect_lover_type_record_flag;
		short ra_perfect_lover_count;

		GameName ra_perfect_lover_name_list[RAND_PERFECT_LOVER_MAX_RECORD_COUPLE_COUNT * 2];
	};


	////////////////////////////////////// 全民进阶 /////////////////////////////////////////////
	enum UPGRADE_TYPE
	{
		UPGRADE_TYPE_MOUNT,
		UPGRADE_TYPE_WING,
		UPGRADE_TYPE_SHIZHUANG_TYPE_BODY,
		UPGRADE_TYPE_SHIZHUANG_TYPE_WUQI,
		UPGRADE_TYPE_FABAO,
		UPGRADE_TYPE_FOOTPRINT,
		UPGRADE_TYPE_HALO,
		UPGRADE_TYPE_QILINBI,				//麒麟臂
		UPGRADE_TYPE_SHENGONG,				//仙女光环
		UPGRADE_TYPE_TOUSHI,				//头饰
		UPGRADE_TYPE_YAOSHI,				//腰饰
		UPGRADE_TYPE_LINGTONG,				//灵童
		UPGRADE_TYPE_LINGGONG,				//灵弓
		UPGRADE_TYPE_LINGQI,				//灵骑
		UPGRADE_TYPE_FIGHT_MOUNT,			//战骑
	};

	class SCQuanMinJinJieInfo	//8816
	{
	public:
		SCQuanMinJinJieInfo();
		MessageHeader header;

		int reward_flag;					// 奖励标记
		int grade;							// 阶数
	};

	////////////////////////////////////// 全民总动员 /////////////////////////////////////////////
	class SCUpgradeGroupeInfo	//8817
	{
	public:
		SCUpgradeGroupeInfo();
		MessageHeader header;

		int ra_upgrade_group_reward_flag;					// 奖励标记
		int count[10];										// 进阶人数
	};

	////////////////////////////////////// 消费领奖 /////////////////////////////////////////////
	enum RA_CONSUM_GIFT_OPERA_TYPE 
	{
		RA_CONSUM_GIFT_OPERA_TYPE_INFO = 0,       // 请求信息
		RA_CONSUM_GIFT_OPERA_TYPE_FETCH,          // 领取奖励    参数2  seq
		RA_CONSUM_GIFT_OPERA_TYPE_ROLL,           // 开始摇奖
		RA_CONSUM_GIFT_OPERA_TYPE_ROLL_REWARD,    // 领取摇奖奖励
		RA_CONSUM_GIFT_OPERA_TYPE_ROLL_TEN,       // 开始摇奖十次
		RA_CONSUM_GIFT_OPERA_TYPE_ROLL_REWARD_TEN,    // 领取摇奖奖励10次
	};

	class SCRAConsumGift			      //  消费领奖信息   8467
	{
	public:
		SCRAConsumGift();
		MessageHeader header;

		int consum_gold;                  // 消费数目
		short act_theme;                  // 今日主题
		short left_roll_times;            // 剩余摇奖次数
		unsigned int reward_fetch_flag;   // 领奖标记
	};

	class SCRAConsumGiftRollReward        // 消费领奖摇奖奖励   8469
	{
	public:
		SCRAConsumGiftRollReward();
		MessageHeader header;

		char seq;                        // 奖励索引
		char decade;                     // 十位数
		char units_digit;                // 个位数
		char res_sh;                     // 保留
	};

	class SCRAConsumGiftRollRewardTen        // 消费领奖摇奖奖励10次   8834
	{
	public:
		SCRAConsumGiftRollRewardTen();
		MessageHeader header;
		char seq[RA_CONSUM_GIFT_MAX_ROLL_COUNT];                        // 奖励索引
		char decade[RA_CONSUM_GIFT_MAX_ROLL_COUNT];                     // 十位数
		char units_digit[RA_CONSUM_GIFT_MAX_ROLL_COUNT];                // 个位数
		char res_sh[RA_CONSUM_GIFT_MAX_ROLL_COUNT];                     // 保留
	};

	////////////////////////////////////// 每日限购 /////////////////////////////////////////////
	enum RA_LIMIT_BUY_OPERA_TYPE
	{
		RA_LIMIT_BUY_OPERA_TYPE_INFO = 0,  // 信息
		RA_LIMIT_BUY_OPERA_TYPE_BUY,       // 购买
	};

	class SCRALimitBuyInfo             // 8470
	{
	public:
		SCRALimitBuyInfo();
		MessageHeader header;

		int act_type;                      // 今日活动主题
		char had_buy_count[RAND_ACTIVITY_DAILY_LIMIT_BUY_MAX_SEQ];   // 4
	};

	////////////////////////////////////// 聚宝盆新 /////////////////////////////////////////////
	enum RA_COLLECT_TREASURE_OPERA_TYPE
	{
		RA_COLLECT_TREASURE_OPERA_TYPE_INFO = 0,  // 信息
		RA_COLLECT_TREASURE_OPERA_TYPE_ROLL,      // 开始摇奖
		RA_COLLECT_TREASURE_OPERA_TYPE_REWARD,    // 获取奖励
	};

	class SCRACollectTreasureInfo          // 8471
	{
	public:
		SCRACollectTreasureInfo();
		MessageHeader header;

		short left_roll_times;
		short record_num;
		short had_join_times;
		short res_sh;
		CollectTreasureRollRecord join_record_list[RAND_ACTIVITY_COLLECT_TREASURE_MAX_RECORD_COUNT];   // 10
	};

	class SCRACollectTreasureResult        // 8472  摇奖结果下发
	{
	public:
		SCRACollectTreasureResult();
		MessageHeader header;

		int seq;
	};

	////////////////////////////////////// 欢乐累充 /////////////////////////////////////////////
	enum RA_HAPPY_CUMUL_CHONGZHI_OPERA_TYPE
	{
		RA_HAPPY_CUMUL_CHONGZHI_OPERA_TYPE_INFO = 0,  // 信息
		RA_HAPPY_CUMUL_CHONGZHI_OPERA_TYPE_FETCH,     // 领奖   seq
	};

	class SCRAHappyCumulChongzhiInfo          // 8473
	{
	public:
		SCRAHappyCumulChongzhiInfo();
		MessageHeader header;

		int chongzhi_num;        // 充值数量
		short act_type;          //主题
		short res_sh;
		unsigned int fetch_reward_flag;  
	};

	////////////////////////////////////// 暴击日 /////////////////////////////////////////////
	class SCRACriticalStrikeInfo			// 暴击日   8468
	{
	public:
		SCRACriticalStrikeInfo();
		MessageHeader header;

		int act_type;                      // 今日活动主题
	};

	////////////////////////////////////// 进阶返还 /////////////////////////////////////////////
	enum RA_JINJIE_RETURN_OPERA_TYPE
	{
		RA_JINJIE_RETURN_OPERA_TYPE_INFO = 0,  // 信息
		RA_JINJIE_RETURN_OPERA_TYPE_FETCH,     // 领奖   seq
	};

	class SCRAJinJieReturnInfo          // 8474
	{
	public:
		SCRAJinJieReturnInfo();
		MessageHeader header;

		int act_type;          //主题
		unsigned int fetch_reward_flag;
	};

	/////////////////////////////////////  随机活动 积分换大礼  ///////////////////////////////////
	enum RA_CONSUME_FOR_GIFT_OPERA_TYPE
	{
		RA_CONSUME_FOR_GIFT_OPERA_TYPE_ALL_INFO,			// 请求所有信息
		RA_CONSUME_FOR_GIFT_OPERA_TYPE_EXCHANGE_ITEM,		// 兑换物品

		RA_CONSUME_FOR_GIFT_OPERA_TYPE_MAX,
	};

	class SCRAConsumeForGiftAllInfo		//5235
	{
	public:
		SCRAConsumeForGiftAllInfo();
		MessageHeader header;

		int total_consume_gold;			//总消费
		int cur_points;					//当前积分

		int item_count;					//物品数量
		unsigned char item_exchange_times[RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT];	//交换次数,RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT = 20
	};
	///////////////////////////////////// 随机活动 秘境寻宝3	////////////////////////////////////

	enum RA_MIJINGXUNBAO3_OPERA_TYPE
	{
		RA_MIJINGXUNBAO3_OPERA_TYPE_QUERY_INFO = 0,				// 请求活动信息
		RA_MIJINGXUNBAO3_OPERA_TYPE_TAO,						// 淘宝
		RA_MIJINGXUNBAO3_OPERA_TYPE_FETCH_REWARD,				// 领取个人累抽奖励 param_1 = 领取奖励的索引（0开始）

		RA_MIJINGXUNBAO3_OPERA_TYPE_MAX,
	};

	class SCRAMiJingXunBaoInfo3  //5236  秘境寻宝3信息
	{
	public:
		SCRAMiJingXunBaoInfo3();
		MessageHeader header;

		unsigned int ra_mijingxunbao3_next_free_tao_timestamp;

		int chou_times;
		int reward_flag;
	};

	static const int SC_MiJingXunBaoTao3_MAX_TIMES = 30;

	class SCRAMiJingXunBaoTaoResultInfo3  //5237 秘境寻宝3结果信息
	{
	public:
		SCRAMiJingXunBaoTaoResultInfo3();
		MessageHeader header;

		int count;
		short mijingxunbao3_tao_seq[SC_MiJingXunBaoTao3_MAX_TIMES];
	};

	///////////////////////////////////// 随机活动 欢乐砸蛋	////////////////////////////////////

	enum RA_HUANLEZADAN_OPERA_TYPE
	{
		RA_HUANLEZADAN_OPERA_TYPE_QUERY_INFO = 0,			// 请求活动信息
		RA_HUANLEZADAN_OPERA_TYPE_TAO,						// 淘宝
		RA_HUANLEZADAN_OPERA_TYPE_FETCH_REWARD,				// 领取个人累抽奖励 param_1 = 领取奖励的索引（0开始）

		RA_HUANLEZADAN_OPERA_TYPE_MAX,
	};

	class SCRAHuanLeZaDanInfo //5238
	{
	public:
		SCRAHuanLeZaDanInfo();
		MessageHeader header;

		unsigned int ra_huanlezadan_next_free_tao_timestamp;

		int chou_times;
		int reward_flag;
	};

	static const int SC_HUANLEZADAN_MAX_TIMES = 30;

	class SCRAHuanLeZaDanTaoResultInfo  //5239
	{
	public:
		SCRAHuanLeZaDanTaoResultInfo();
		MessageHeader header;

		int count;
		short huanlezadan_tao_seq[SC_HUANLEZADAN_MAX_TIMES];
	};

	///////////////////////////////////// 随机活动 欢乐摇奖	////////////////////////////////////

	enum RA_HUANLE_YAOJIANG_OPERA_TYPE
	{
		RA_HUANLEYAOJIANG_OPERA_TYPE_QUERY_INFO = 0,			// 请求活动信息
		RA_HUANLEYAOJIANG_OPERA_TYPE_TAO,						// 淘宝
		RA_HUANLEYAOJIANG_OPERA_TYPE_FETCH_REWARD,				// 领取个人累抽奖励 param_1 = 领取奖励的索引（0开始）

		RA_HUANLEYAOJIANG_OPERA_TYPE_MAX,
	};

	class SCRAHuanLeYaoJiangInfo  // 5240
	{
	public:
		SCRAHuanLeYaoJiangInfo();
		MessageHeader header;

		unsigned int ra_huanleyaojiang_next_free_tao_timestamp;

		int chou_times;
		int reward_flag;
	};

	static const int SC_HUANLE_YAOJIANG_MAX_TIMES = 30;

	class SCRAHuanLeYaoJiangTaoResultInfo  // 5241
	{
	public:
		SCRAHuanLeYaoJiangTaoResultInfo();
		MessageHeader header;

		int count;
		short huanleyaojiang_tao_seq[SC_HUANLE_YAOJIANG_MAX_TIMES];
	};

	////////////////////////////////////////////////// 限时回馈 /////////////////////////////////////////////////
	enum RA_LIMIT_TIME_REBATE_OPERA_TYPE
	{
		RA_LIMIT_TIME_REBATE_OPERA_TYPE_INFO,			// 信息
		RA_LIMIT_TIME_REBATE_OPERA_TYPE_FETCH_REWARD,	// 领取奖励
	};

	class SCRALimitTimeRebateInfo			     // 8475 限时回馈
	{
	public:
		SCRALimitTimeRebateInfo();
		MessageHeader header;

		int cur_day_chongzhi;					// 今天充值金额	
		int chongzhi_days;						// 充值天数
		int reward_flag;						// 奖励标记
		int chongzhi_day_list[RA_LIMIT_TIME_REBATE_MAX_DAY];			// 充值列表
	};

	/////////////////////////////////////  随机活动 限时礼包 /////////////////////////////////////
	enum RA_TIMELIMIT_GIFT_OPERA_TYPE
	{
		RA_TIMELIMIT_GIFT_OPERA_TYPE_QUERY_INFO = 0,
		RA_TIMELIMIT_GIFT_OPERA_TYPE_FETCH_REWARD,

		RA_TIMELIMIT_GIFT_OPERA_TYPE_MAX,
	};

	enum RA_TIMELIMIT_GIFT_FETCH_TYPE
	{
		RA_TIMELIMIT_GIFT_FETCH_FIRST = 0,		// 第一份奖励领取操作
		RA_TIMELIMIT_GIFT_FETCH_SECOND,			// 第二份奖励领取操作
		//RA_TIMELIMIT_GIFT_FETCH_THIRD,			// 第三份奖励领取操作

		RA_TIMELIMIT_GIFT_FETCH_END,
	};

	class SCRATimeLimitGiftInfo			// 8476   限时礼包
	{
	public:
		SCRATimeLimitGiftInfo();
		MessageHeader header;

		int reward_can_fetch_flag1;		// 能否领取第一次奖励标志
		int reward_fetch_flag1;			// 是否领取第一次奖励标志
		int reward_can_fetch_flag2;		// 能否领取第二次奖励标志
		int reward_fetch_flag2;			// 是否领取第二次奖励标志
		short join_vip_level;			// 参与时vip等级
		short open_flag;				// 是否面板显示标志
		int begin_timestamp;			// 角色进入活动的时间
		int reward_can_fetch_flag3;		// 能否领取第三次奖励标志
		int reward_fetch_flag3;			// 是否领取第三次奖励标志
	};

	/////////////////////////////////////  随机活动 循环充值 2 ///////////////////////////////////
	enum CIRCULATION_CHONGZHI_OPERA_TYPE
	{
		CIRCULATION_CHONGZHI_OPERA_TYPE_QUERY_INFO = 0,		// 请求活动信息
		CIRCULATION_CHONGZHI_OPEAR_TYPE_FETCH_REWARD,		// 获取奖励

		CIRCULATION_CHONGZHI_OPERA_TYPE_MAX
	};

	class SCCirculationChongzhiInfo					// 8477  循环充值2
	{
	public:
		SCCirculationChongzhiInfo();
		MessageHeader header;

		unsigned int total_chongzhi;				// 累计充值
		unsigned int cur_chongzhi;					// 上次领奖到现在的充值
	};

	/////////////////////////////////////  随机活动 疯狂摇钱树 /////////////////////////////////////
	enum RA_SHAKEMONEY_OPERA_TYPE
	{
		RA_SHAKEMONEY_OPERA_TYPE_QUERY_INFO = 0,			// 请求信息
		RA_SHAKEMONEY_OPERA_TYPE_FETCH_GOLD,				// 领取元宝

		RA_SHAKEMONEY_OPERA_TYPE_MAX,
	};

	class SCRAShakeMoneyInfo        // 8478   疯狂摇钱树
	{
	public:
		SCRAShakeMoneyInfo();
		MessageHeader header;

		int total_chongzhi_gold;
		int chongzhi_gold;
		int seq;                   // 下发配置索引
	};

	/////////////////////////////////////  随机活动 限时豪礼 /////////////////////////////////////
	enum RA_TIMELIMIT_LUXURY_GIFT_BAG_OPERA_TYPE
	{
		RA_TIMELIMIT_LUXURY_GIFT_BAG_OPERA_TYPE_QUERY_INFO = 0,
		RA_TIMELIMIT_LUXURY_GIFT_BAG_OPERA_TYPE_BUY,

		RA_TIMELIMIT_LUXURY_GIFT_BAG_OPERA_TYPE_MAX,
	};

	class SCRATimeLimitLuxuryGiftBagInfo			// 8479
	{
	public:
		SCRATimeLimitLuxuryGiftBagInfo();
		MessageHeader header;

		short is_already_buy;			      // 是否已经购买
		short join_vip_level;			      // 参与时vip等级
		int begin_timestamp;			      // 角色进入活动的时间
		int time_limit_luxury_gift_open_flag; //开始标记
	};

	enum RA_DOUBLE_CHONGZHI_OPERA_TYPE
	{
		RA_DOUBLE_CHONGZHI_OPERA_TYPE_INFO = 0,			// 请求信息

		RA_DOUBLE_CHONGZHI_OPERA_TYPE_MAX,
	};

	class SCRAResetDoubleChongzhi      // 8480 普天同庆
	{
	public:
		SCRAResetDoubleChongzhi();
		MessageHeader header;

		int chongzhi_reward_flag;						// 双倍充值奖励的标志
		char open_flag;
		char res_ch;
		short res_sh;
	};

	///////////////////////////////// 随机活动 - RMB商店 //////////////////////////
	struct SCRARmbBugChestShopInfo		// 8481
	{
		SCRARmbBugChestShopInfo();
		MessageHeader header;

		char buy_count_list[RA_RMB_BUY_CHEST_SHOP_MAX_BUY_COUNT]; //64
	};

	///////////////////////////////// 随机活动 - 消费返礼 //////////////////////////
	enum RA_CONSUME_GOLD_REWARD_OPERATE_TYPE
	{
		RA_CONSUME_GOLD_REWARD_OPERATE_TYPE_INFO = 0,		// 请求活动信息
		RA_CONSUME_GOLD_REWARD_OPERATE_TYPE_FETCH = 1,		// 请求领取奖励
	};

	class SCRAConsumeGoldRewardInfo					// 8482
	{
	public:
		SCRAConsumeGoldRewardInfo();
		MessageHeader header;

		int consume_gold;							// 消费金额
		char fetch_reward_flag;						// 是否已经领取奖励
		char vip_level;								// 对应vip等级
		short activity_day;							// 活动第几天
	};

	///////////////////////////	集月饼活动		////////////////////////////
	enum RA_ITEM_COLLECTION_SECOND_OPERA_TYPE
	{
		RA_ITEM_COLLECTION_SECOND_OPERA_TYPE_QUERY_INFO = 0,					// 请求活动的信息
		RA_ITEM_COLLECTION_SECOND_OPERA_TYPE_EXCHANGE,							// 请求兑换   param1  seq  
		RA_ITEM_COLLECTION_SECOND_OPERA_TYPE_MAX,
	};

	class SCCollectSecondExchangeInfo			//8490 集月饼活动
	{
	public:
		SCCollectSecondExchangeInfo();
		MessageHeader		header;

		int collection_exchange_times[RAND_ACTIVITY_ITEM_COLLECTION_SECOND_REWARD_MAX_COUNT]; // 5
	};

	/////////////////////////////////////  版本累计充值  /////////////////////////////////////
	enum RA_VERSION_TOTAL_CHARGE_OPERA_TYPE
	{
		RA_VERSION_TOTAL_CHARGE_OPERA_TYPE_QUERY_INFO = 0,
		RA_VERSION_TOTAL_CHARGE_OPERA_TYPE_FETCH_REWARD,

		RA_VERSION_TOTAL_CHARGE_OPERA_TYPE_MAX,
	};

	class SCRAVersionTotalChargeInfo	//8484
	{
	public:
		SCRAVersionTotalChargeInfo();
		MessageHeader header;

		int total_charge_value;								// 累计充值数
		int reward_has_fetch_flag;							// 已领取过的奖励标记
	};

	/////////////////////////////////////  版本连续充值  ///////////////////////////////////
	enum RA_VERSION_CONTINUE_CHONGZHI_OPERA_TYPE
	{
		RA_VERSION_CONTINUE_CHONGZHI_OPERA_TYPE_QUERY_INFO = 0,		// 请求活动信息
		RA_VERSION_CONTINUE_CHONGZHI_OPEAR_TYPE_FETCH_REWARD,		// 获取奖励

		RA_VERSION_CONTINUE_CHONGZHI_OPERA_TYPE_MAX
	};

	class SCRAVersionContinueChongzhiInfo			// 8485
	{
	public:
		SCRAVersionContinueChongzhiInfo();
		MessageHeader header;

		unsigned int today_chongzhi;						// 今日充值数
		short can_fetch_reward_flag;						// 奖励激活标记
		short has_fetch_reward_falg;						// 奖励领取标记
		char continue_chongzhi_days;						// 连续充值天数
		char reserve1;
		short reserve2;
	};

	///////////////////////////////////// 版本活动 欢乐摇奖2	////////////////////////////////////
	enum RA_HUANLE_YAOJIANG_2_OPERA_TYPE
	{
		RA_HUANLEYAOJIANG_OPERA_2_TYPE_QUERY_INFO = 0,			// 请求活动信息
		RA_HUANLEYAOJIANG_OPERA_2_TYPE_TAO,						// 淘宝
		RA_HUANLEYAOJIANG_OPERA_2_TYPE_FETCH_REWARD,			// 领取个人累抽奖励 param_1 = 领取奖励的索引（0开始）

		RA_HUANLEYAOJIANG_OPERA_2_TYPE_MAX,
	};

	class SCRAHuanLeYaoJiangTwoInfo        // 8486
	{
	public:
		SCRAHuanLeYaoJiangTwoInfo();
		MessageHeader header;

		unsigned int ra_huanleyaojiang_next_free_tao_timestamp;

		int chou_times;
		int reward_flag;
	};

	static const int SC_HUANLE_YAOJIANG_2_MAX_TIMES = 30;

	class SCRAHuanLeYaoJiangTwoTaoResultInfo     // 8487
	{
	public:
		SCRAHuanLeYaoJiangTwoTaoResultInfo();
		MessageHeader header;

		int count;
		short huanleyaojiang_tao_seq[SC_HUANLE_YAOJIANG_2_MAX_TIMES];
	};

	///////////////////////////////////// 版本活动 线下单笔充值	////////////////////////////////////
	class SCRAOfflineSingleChargeInfo0			// 8488
	{
	public:
		SCRAOfflineSingleChargeInfo0();
		MessageHeader header;

		int activity_type;
		int charge_max_value;
		int reward_times[RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT];					// 奖励次数
	};

	///////////////////////////////////////////////// 买一送一 ////////////////////////////////////////////////
	enum RA_BUY_ONE_GET_ONE_FREE_OPERA_TYPE
	{
		RA_BUY_ONE_GET_ONE_FREE_OPERA_TYPE_INFO,
		RA_BUY_ONE_GET_ONE_FREE_OPERA_TYPE_BUY,
		RA_BUY_ONE_GET_ONE_FREE_OPERA_TYPE_FETCH_REWARD,
	};

	class SCBuyOneGetOneFreeInfo		// 8483
	{
	public:
		SCBuyOneGetOneFreeInfo();
		MessageHeader header;

		long long buy_flag;				// 购买标记
		long long free_reward_flag;		// 领取奖励标记
	};

	///////////////////////////////////////////////// 节日守护 ////////////////////////////////////////////////
	struct KillRankNoticeInfo
	{
		KillRankNoticeInfo() : uid(0), kill_monster_count(0), total_capablity(0), sex(0), prof(0), reserve_sh(0), pass_time(0)
		{
			memset(user_name, 0, sizeof(user_name));
		}

		KillRankNoticeInfo & operator =(KillRankNoticeInfo *param)
		{
			this->uid = param->uid;
			memcpy(this->user_name, param->user_name, sizeof(this->user_name));
			this->kill_monster_count = param->kill_monster_count;
			this->total_capablity = param->total_capablity;
			this->sex = param->sex;
			this->prof = param->prof;
			this->pass_time = param->pass_time;

			return *this;
		}

		int uid;
		GameName user_name;
		int kill_monster_count;
		int total_capablity;
		char sex;
		char prof;
		short reserve_sh;
		unsigned int pass_time;
	};

	// 节日守护击杀排行信息
	struct SCRAHolidayGuardRanKInfo		// 8831
	{
		SCRAHolidayGuardRanKInfo();
		MessageHeader header;

		KillRankNoticeInfo kill_rank[RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX];
	};

	// 节日守护 请求排行下发
	struct CSRAHolidayGuardRankInfoReq		// 8832
	{
		CSRAHolidayGuardRankInfoReq();
		MessageHeader header;
	};

	struct NpcInfo
	{
		int npc_index;
		int scene_id;
		int npc_id;
		int npc_x;
		int npc_y;
	};

	struct SCRAExtremeChallengeNpcInfo	// 8833
	{
		SCRAExtremeChallengeNpcInfo();
		MessageHeader header;

		NpcInfo npc_info_list[HOLIDAYGUARD_NPC_CFG_MAX_COUNT];
	};

	/////////////////////////////////////  版本活动 累计充值5(版本吉祥三宝)  /////////////////////////////////////

	class CSRATotalChargeFiveInfo // 8490   累计充值5，请求当前玩家充值信息
	{
	public:
		CSRATotalChargeFiveInfo();
		MessageHeader		header;
	};

	class SCRATotalChargeFiveInfo  // 8491 
	{
	public:
		SCRATotalChargeFiveInfo();
		MessageHeader		header;

		int cur_total_charge;
		int cur_total_charge_has_fetch_flag;
	};

	///////////////////////////////////////////////// 极限挑战 ////////////////////////////////////////////////
	enum EXTREMECHALLENGE
	{
		EXTREMECHALLENGE_INFO,						// 信息请求
		EXTREMECHALLENGE_REFRESH_TASK,				// 刷新任务    参数1 task_id
		EXTREMECHALLENGE_FETCH_REWARD,				// 领取奖励    参数1 task_id
		EXTREMECHALLENGE_INIT_TASK,					// 初始任务
		EXTREMECHALLENGE_FETCH_ULTIMATE_REWARD,		// 领取终极奖励
	};

	struct SCRAExtremeChallengeTaskInfo		        // 8489
	{
		SCRAExtremeChallengeTaskInfo();
		MessageHeader header;

		RAExremeChallengeTaskInfo task_state[RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM];			// 任务
		char is_have_fetch_ultimate_reward;

		char reserve_ch;
		short reserve_sh;
	};

	///////////////////////////////////////////////// 你充我送 ////////////////////////////////////////////////
	enum CHONGZHI_GIFT_OPER_TYPE
	{
		CHONGZHI_GIFT_OPER_TYPE_INFO = 0,               // 信息
		CHONGZHI_GIFT_OPER_TYPE_FETCH,                  // 领取奖励    param_1 = 索引
	};

	struct SCRAChongZhiGiftInfo		             // 8492
	{
		SCRAChongZhiGiftInfo();
		MessageHeader header;

		char magic_shop_fetch_reward_flag;
		char res_ch;
		short activity_day;
		unsigned int magic_shop_chongzhi_value;
	};

	///////////////////////////////////////////////// 合服活动--投资计划 ////////////////////////////////////////////////
	enum CSA_TOUZIJIHUA_OPERA
	{
		CSA_TOUZIJIHUA_INFO_REQ = 0,	// 请求信息
		CSA_TOUZIJIHUA_FETCH_REQ = 1,	// 领取奖励 param_2 填奖励索引
	};

	class SCCSATouzijihuaInfo					//8493
	{
	public:
		SCCSATouzijihuaInfo();
		MessageHeader		header;

		char csa_touzijihua_buy_flag;										// 是否购买合服投资计划
		char csa_touzijihua_reserve_ch;
		unsigned short csa_touzjihua_login_day;								// 投资计划每日奖励领取标志
		unsigned int csa_touzijihua_total_fetch_flag;						// 投资计划领取奖励标志
	};

	///////////////////////////////////////////////// 合服活动--成长基金 ////////////////////////////////////////////////
	enum CSA_FOUNDATION_OPERA
	{
		CSA_FOUNDATION_INFO_REQ = 0,	// 请求信息
		CSA_FOUNDATION_FETCH_REQ = 1,	// 领取奖励，param_2填奖励索引
	};

	class SCCSAFoundationInfo					//8494
	{
	public:
		SCCSAFoundationInfo();
		MessageHeader		header;

		CSAFoundationStatus csa_foundation_status[COMBINE_SERVER_MAX_FOUNDATION_TYPE];	//购买状态 COMBINE_SERVER_MAX_FOUNDATION_TYPE = 10
	};

	///////////////////////////////////////////////// 合服活动--经验炼制 ////////////////////////////////////////////////
	enum CSA_EXP_REFINE_OPERA_TYPE
	{
		CSA_EXP_REFINE_OPERA_TYPE_BUY_EXP = 0,					// 炼制
		CSA_EXP_REFINE_OPERA_TYPE_GET_INFO,						// 获取信息

		CSA_EXP_REFINE_OPERA_TYPE_MAX
	};

	class SCCSAExpRefineInfo									// 8495
	{
	public:
		SCCSAExpRefineInfo();
		MessageHeader header;

		char had_buy;											// 是否购买过
		char reserve_ch;
		short reserve_sh;
	};

	///////////////////////////////////////////////// 合服活动--攻城战 ////////////////////////////////////////////////
	class SCCSAGongChengZhanInfo					//8496
	{
	public:
		SCCSAGongChengZhanInfo();
		MessageHeader		header;

		int win_times;							// 胜利次数
	};

	////////////////////////////////////// 进阶返还2 /////////////////////////////////////////////
	enum RA_JINJIE_RETURN2_OPERA_TYPE
	{
		RA_JINJIE_RETURN2_OPERA_TYPE_INFO = 0,  // 信息
		RA_JINJIE_RETURN2_OPERA_TYPE_FETCH,     // 领奖   seq
	};

	class SCRAJinJieReturnInfo2        // 8498
	{
	public:
		SCRAJinJieReturnInfo2();
		MessageHeader header;

		int act_type;          //主题
		unsigned int fetch_reward_flag;
	};

	////////////////////////////////////// 暴击日2 /////////////////////////////////////////////
	class SCRACriticalStrike2Info			// 暴击日2   8499
	{
	public:
		SCRACriticalStrike2Info();
		MessageHeader header;

		int act_type;                      // 今日活动主题
	};

	////////////////////////////	组合购买		//////////////////////////////
	enum RA_COMBINE_BUY_OPERA_TYPE
	{
		RA_COMBINE_BUY_OPERA_TYPE_INFO = 0,					// 请求信息
		RA_COMBINE_BUY_OPERA_TYPE_ADD_IN_BUCKET = 1,		// 加入购物车，param_1 = 物品索引
		RA_COMBINE_BUY_OPERA_TYPE_REMOVE_BUCKET = 2,		// 移出购物车，param_1 = 购物车索引
		RA_COMBINE_BUY_OPERA_TYPE_BUY = 3,					// 购买
	};

	class SCRACombineBuyBucketInfo			// 8835
	{
	public:
		SCRACombineBuyBucketInfo();
		MessageHeader header;

		int seq_list[RA_COMBINE_BUY_BUCKET_ITEM_COUNT];	// 购物车物品索引  = 6
	};

	class SCRACombineBuyItemInfo			//8836
	{
	public:
		SCRACombineBuyItemInfo();
		MessageHeader header;

		unsigned char buy_item_num[RA_COMBINE_BUY_MAX_ITEM_COUNT];	//购买物品数量 RA_COMBINE_BUY_MAX_ITEM_COUNT = 96
	};

	/////////////////////////////	天天返利			////////////////////////////////
	enum DAY_CHONGZHI_REWARD_OPERA_TYPE
	{
		DAY_CHONGZHI_REWARD_OPERA_TYPE_QUERY_INFO = 0,		// 请求信息
		DAY_CHONGZHI_REWARD_OPERA_TYPE_FETCH_REWARD,		// 请求领取奖励	param_0 = 索引
		DAY_CHONGZHI_REWARD_OPERA_TYPE_FETCH_RARE_REWARD,	// 请求领取珍稀奖励	param_0 = 索引

		DAY_CHONGZHI_REWARD_OPERA_TYPE_MAX,
	};

	class CSDayChongzhiRewardReq			// 8837
	{
	public:
		CSDayChongzhiRewardReq();
		MessageHeader header;

		short opera_type;
		short param_0;
	};

	class SCDayChongzhiRewardInfo			// 8838
	{
	public:
		SCDayChongzhiRewardInfo();
		MessageHeader header;

		int day_count;															// 累计充值天数
		unsigned int reward_flag_list[DAY_CHONGZHI_REWARD_FLAG_LIST_LEN];		// 获取奖励标志 DAY_CHONGZHI_REWARD_FLAG_LIST_LEN = 4
		unsigned int rare_reward_flag_list[DAY_CHONGZHI_REWARD_FLAG_LIST_LEN];	// 获取珍稀奖励标志
	};

	/////////////////////////////////////  跨服秘藏BOSS  /////////////////////////////////////
	enum CROSS_MIZANG_BOSS_OPERA_TYPE
	{
		CROSS_MIZANG_BOSS_OPERA_TYPE_GET_FLUSH_INFO = 0,	// 刷新信息 param1 层数（为0则为所有层）
		CROSS_MIZANG_BOSS_OPERA_TYPE_BOSS_KILL_RECORD,		// 击杀记录 param1 层数 param2 boss_id
		CROSS_MIZANG_BOSS_OPERA_TYPE_DROP_RECORD,			// 掉落记录	
		CROSS_MIZANG_BOSS_OPERA_TYPE_CONCERN_BOSS,			// 关注boss	param1 层数 param2 boss_id
		CROSS_MIZANG_BOSS_OPERA_TYPE_UNCONCERN_BOSS,		// 关注boss param1 层数 param2 boss_id
		CROSS_MIZANG_BOSS_OPERA_TYPE_FORENOTICE,			// boss通知

		CROSS_MIZANG_BOSS_OPERA_TYPE_MAX,
	};

	class CSCrossMiZangBossBossInfoReq				// 请求跨服秘藏boss信息 8850
	{
	public:
		CSCrossMiZangBossBossInfoReq();
		MessageHeader	header;

		int req_type;
		int param_1;
		int param_2;
	};

	class SCCrossMizangBossBossInfoAck		       // 跨服秘藏boss信息 8851
	{
	public:
		SCCrossMizangBossBossInfoAck();
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
			BossInfo boss_list[MAX_CROSS_MIZANG_BOSS_PER_SCENE];
		};

		int scene_count;
		SceneInfo scene_list[CROSS_MIZANG_BOSS_SCENE_MAX];
	};

	class SCCrossMizangBossSceneInfo			// 跨服秘藏boss场景里的玩家信息 8852
	{
	public:
		SCCrossMizangBossSceneInfo();
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
		CrossBossInfo boss_list[MAX_CROSS_MIZANG_BOSS_PER_SCENE];					// Boss信息
	};

	class SCCrossMiZangBossPlayerInfo		// 跨服秘藏BOSS玩家信息 8853
	{
	public:
		SCCrossMiZangBossPlayerInfo();
		MessageHeader	header;

		short left_can_kill_boss_num;										// 剩余Boss数
		short left_treasure_crystal_gather_times;							// 剩余珍惜水晶采集次数
		int left_ordinary_crystal_gather_times;								// 剩余普通水晶采集次数

		unsigned int concern_flag[CROSS_MIZANG_BOSS_SCENE_MAX];					// 关注flag
	};

	/////////////////////////////////////  跨服幽冥BOSS  /////////////////////////////////////
	enum CROSS_YOUMING_BOSS_OPERA_TYPE
	{
		CROSS_YOUMING_BOSS_OPERA_TYPE_GET_FLUSH_INFO = 0,	// 刷新信息 param1 层数（为0则为所有层）
		CROSS_YOUMING_BOSS_OPERA_TYPE_BOSS_KILL_RECORD,		// 击杀记录 param1 层数 param2 boss_id
		CROSS_YOUMING_BOSS_OPERA_TYPE_DROP_RECORD,			// 掉落记录	
		CROSS_YOUMING_BOSS_OPERA_TYPE_CONCERN_BOSS,			// 关注boss	param1 层数 param2 boss_id
		CROSS_YOUMING_BOSS_OPERA_TYPE_UNCONCERN_BOSS,		// 关注boss param1 层数 param2 boss_id
		CROSS_YOUMING_BOSS_OPERA_TYPE_FORENOTICE,			// boss通知

		CROSS_YOUMING_BOSS_OPERA_TYPE_MAX,
	};

	class CSCrossYouMingBossBossInfoReq				// 请求跨服秘藏boss信息 8856
	{
	public:
		CSCrossYouMingBossBossInfoReq();
		MessageHeader	header;

		int req_type;
		int param_1;
		int param_2;
	};

	class SCCrossYouMingBossBossInfoAck		       // 跨服秘藏boss信息 8857
	{
	public:
		SCCrossYouMingBossBossInfoAck();
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
			BossInfo boss_list[MAX_CROSS_YOUMING_BOSS_PER_SCENE];
		};

		int scene_count;
		SceneInfo scene_list[CROSS_YOUMING_BOSS_SCENE_MAX];
	};

	class SCCrossYouMingBossSceneInfo			// 跨服秘藏boss场景里的玩家信息 8858
	{
	public:
		SCCrossYouMingBossSceneInfo();
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
		CrossBossInfo boss_list[MAX_CROSS_YOUMING_BOSS_PER_SCENE];					// Boss信息
	};

	class SCCrossYouMingBossPlayerInfo		// 跨服秘藏BOSS玩家信息 8859
	{
	public:
		SCCrossYouMingBossPlayerInfo();
		MessageHeader	header;

		short left_can_kill_boss_num;										// 剩余Boss数
		short left_treasure_crystal_gather_times;							// 剩余珍惜水晶采集次数
		int left_ordinary_crystal_gather_times;								// 剩余普通水晶采集次数

		unsigned int concern_flag[CROSS_YOUMING_BOSS_SCENE_MAX];					// 关注flag
	};

	enum LUCKY_WISH_TYPE
	{
		LUCKY_WISH_TYPE_ONLY_LUCKY_VALUE = 0,			//告诉客户端更新幸运值，无视RewardItem
		LUCKY_WISH_TYPE_COMMON_ITEM = 1,				//抽中普通物品
		LUCKY_WISH_TYPE_LUCKY_ITEM = 2,					//抽中了幸运物品
	};

	class SCLuckyWishInfo			// 随机活动-幸运许愿所有信息 8861
	{
	public:
		SCLuckyWishInfo();
		MessageHeader header;

		struct RewardItem
		{
			RewardItem() : item_id(0), is_bind(0), num(0) {}

			unsigned short item_id;
			short is_bind;
			int num;
		};

		int lucky_value;															// 幸运值
		char type;																	// 参考 LUCKY_WISH_TYPE
		char reserve_ch;															// 保留
		short item_list_count;														// 物品列表长度
		RewardItem reward_item[LUCKY_WISH_MAX_ITEM_COUNT];							// 抽中的物品id
	};

	enum RA_LUCKY_WISH_OPERA_TYPE
	{
		RA_LUCKY_WISH_OPERA_TYPE_ALL_INFO = 0,						//请求所有信息
		RA_LUCKY_WISH_OPERA_TYPE_WISH = 1,							//许愿	param_1	许愿次数
	};
	class CSLuckyWishOpera			// 随机活动-幸运许愿请求 8862
	{
	public:
		CSLuckyWishOpera();
		MessageHeader header;

		char type;																	// 参考 RA_LUCKY_WISH_OPERA_TYPE
		char param_1;																// 许愿次数
	};


	class SCImageCompetitionInfo           // 形象比拼活动  8863
	{
	public:
		SCImageCompetitionInfo();
		MessageHeader	header;

		int opengame_day;       
	};

	/////////////////////////////////////  随机活动 周末狂欢活动  /////////////////////////////////////
	enum RA_LOTTERY_1_OPERA_TYPE
	{
		RA_LOTTERY_1_OPERA_TYPE_INFO = 0,					// 请求活动信息
		RA_LOTTERY_1_OPERA_TYPE_DO_LOTTERY,					// 发起抽奖请求 p1抽奖类型 (1:一抽 2：十抽)
		RA_LOTTERY_1_OPERA_TYPE_FETCH_PERSON_REWARD,		// 领取个人奖励请求 p1序号

		RA_LOTTERY_1_OPERA_TYPE_MAX,
	};

	class SCRALottery1Info  // 2279 
	{
	public:
		SCRALottery1Info();
		MessageHeader header;

		unsigned int next_free_time;												// 下次免费时间
		unsigned int person_total_chou_times;									// 个人总抽奖次数
		unsigned int server_total_chou_times;									// 全服总抽奖次数
		unsigned short fetch_person_reward_flag;								// 领取个人奖励标记
		short item_count;					
		RALottery1ChouItem chou_item_list[RA_LOTTERY_1_TEN_CHOU_COUNT];			// 抽中的物品列表
	};

	/////////////////////////////////////  限购礼包2  /////////////////////////////////////
	enum GiftShopBuy2_OPERA_TYPE
	{
		GiftShopBuy2_OPERA_TYPE_INFO = 0,			//请求信息，opera_type = 0 
		GiftShopBuy2_OPERA_TYPE_BUY,				//请求购买，opera_type = 1，seq
	};

	class CSRAOpenGameGiftShopBuy2					// 8921 限购礼包2
	{
	public:
		CSRAOpenGameGiftShopBuy2();
		MessageHeader header;

		int opera_type;
		int seq;
	};

	class SCRAOpenGameGiftShopBuy2Info				// 8920 礼包限购信息
	{
	public:
		SCRAOpenGameGiftShopBuy2Info();
		MessageHeader header;

		int buy_flag;								// 礼包购买标志
	};

	/////////////////////////////////////  随机活动 零元购  /////////////////////////////////////
	enum RA_ZERO_BUY_RETURN_OPERA_TYPE
	{
		RA_ZERO_BUY_RETURN_OPERA_TYPE_INFO = 0,			// 请求活动信息
		RA_ZERO_BUY_RETURN_OPERA_TYPE_BUY,				// 购买 p1购买类型
		RA_ZERO_BUY_RETURN_OPERA_TYPE_FETCH_YUANBAO,	// 领取元宝 p1购买类型

		RA_ZERO_BUY_RETURN_OPERA_TYPE_MAX,
	};

	class SCRAZeroBuyReturnInfo  // 2280  零元购信息 
	{
	public:
		SCRAZeroBuyReturnInfo();
		MessageHeader header;

		unsigned int zero_buy_return_buy_timestamp[RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE];	 // 零元购购买时间戳3
		unsigned int zero_buy_return_day_fetch_flag[RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE]; // 零元购每日领取元宝标记3
	};

	////////////////////////////	狂嗨庆典		//////////////////////////////
	enum RA_KUANG_HAI_OPERA_TYPE
	{
		RA_KUANG_HAI_OPERA_TYPE_INFO = 0,			// 请求信息
		RA_KUANG_HAI_OPERA_TYPE_FETCH_REWARD = 1,	// 兑换奖励，param_1 = 奖励索引
	};

	class SCRAKuangHaiInfo	// 狂嗨庆典信息 2281
	{
	public:
		SCRAKuangHaiInfo();
		MessageHeader header;

		struct TaskItem
		{
			short item_idx;		// 索引
			short get_score;	// 已得嗨点数
		};

		short reward_flag;			// 奖励已兑换的标志
		short task_item_num;		// 数量
		short current_score;		// 当前嗨点
		short reserve_sh;
		TaskItem task_item_list[RA_KUANG_HAI_TASK_ITEM_MAX_NUM];
	};

	////////////////////////////	礼物收割		//////////////////////////////
	enum RA_GIFT_HARVEST_OPERA_TYPE
	{
		RA_GIFT_HARVEST_OPERA_TYPE_INFO = 0,	// 请求个人信息
		RA_GIFT_HARVEST_OPERA_TYPE_RANK_INFO,	// 请求排行信息
		RA_GIFT_HARVEST_OPERA_TYPE_ENTER_SCENE, // 请求进入场景
		RA_GIFT_HARVEST_OPERA_TYPE_ROUND_INFO,	// 请求当前轮次状态

		RA_GIFT_HARVEST_OPERA_TYPE_MAX,
	};

	class SCRAGiftHarvestPlayerInfo		//  玩家信息 2282
	{
	public:
		SCRAGiftHarvestPlayerInfo();
		MessageHeader header;

		int round;		// 本轮轮次
		int get_score;	// 获得的积分
		int kill_num;	// 击杀数
	};

	struct GiftHarvestRankItem
	{
		GiftHarvestRankItem() {}

		int role_id = 0;				// 角色id
		int cur_get_score = 0;			// 本轮所得分数
		char sex = 0;					// 性别
		char prof = 0;					// 职业
		short reserve_sh = 0;
		long long avatar_timestamp = 0;	// 头像
		GameName name;					// 名字
	};

	class SCRAGiftHarvestRankInfo	// 2283 排行榜信息
	{
	public:
		SCRAGiftHarvestRankInfo();
		MessageHeader header;

		int rank_num;				// 排行item数量
		GiftHarvestRankItem rank_list[GIFT_HARVEST_PERSON_RANK_MAX];
	};

	class SCGiftHarvestSkill		// 2284 礼物收割技能信息
	{
	public:
		SCGiftHarvestSkill();
		MessageHeader header;

		int skill_index;				// 技能下标
		unsigned int next_can_perform_time;		// 下次可释放技能时间戳
	};

	class SCGiftHarvestRoundNotice	// 2285
	{
	public:
		SCGiftHarvestRoundNotice();
		MessageHeader header;

		short round;						// 本轮次
		short round_state;					// 本轮状态
		unsigned int next_state_timestamp;	// 下一状态时间戳
	};

	
	/////////////////////////////////////////// 跨服边境之地 ///////////////////////////////////////////

	class SCCrossBianJingZhiDiUserInfo   // 6640  跨服边境之地角色信息
	{
	public:
		SCCrossBianJingZhiDiUserInfo();
		MessageHeader header;

		struct SClientTaskInfo
		{
			SClientTaskInfo() : task_id(0), is_finish(0), param_count(0), cur_param_value(0), is_double_reward(0) {}

			short task_id;																	// 任务ID
			short is_finish;																// 是否领取奖励

			int param_count;																// 任务关联的采集物或怪物的数量
			int cur_param_value;															// 当前值
			int is_double_reward;
		};

		struct RewardItemInfo																// 获得的物品信息
		{
			RewardItemInfo() : item_id(0), num(0) {}

			int item_id;
			int num;
		};

		unsigned int boss_reflush_time;
		int limit_task_time;
		int gather_buff_time;
		int sos_times;						// 召集次数

		SClientTaskInfo taskinfo_list[CROSS_BIANJINGZHIDI_TASK_MAX];						// 用户任务信息
		RewardItemInfo reward_list[CROSS_BIANJINGZHIDI_MAX_REWARD_ITEM_COUNT];				// 获得的奖励信息
	};

	class SCCrossBianJingZhiDiBossInfo		// 6641 跨服边境之地BOSS信息
	{
	public:
		SCCrossBianJingZhiDiBossInfo();
		MessageHeader header;

		struct BossInfoList // 单个BOSS信息
		{
			ObjID boss_obj;			// boss objID
			short boss_live_flag;	// boss是否存活

			int boss_id;			// 
			short born_pos_x;			// 出生位置x
			short born_pos_y;			// 出生位置y

			long long guild_uuid;	// 归属帮派uuid  0表示无归属
			GuildName guild_name;	// 归属帮派名
		};

		int count;
		BossInfoList boss_list[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE];
	};

	class SCCrossBianJingZhiDiBossHurtInfo	// 6642 跨服边境之地，BOSS伤害信息
	{
	public:
		SCCrossBianJingZhiDiBossHurtInfo();
		MessageHeader header;
		static const int BOSS_HURT_INFO_RANK_MAX = 6;
		struct HurtInfo
		{
			long long guild_id;
			GuildName guild_name;
			long long hurt;
		};

		int boss_id;
		int own_guild_rank;	//自己仙盟排名
		long long own_guild_hurt;//自己仙盟伤害
		int count;
		HurtInfo info_list[BOSS_HURT_INFO_RANK_MAX];
	};

	class CSCrossBianJingZhiDiBuyBuff		 // 6643 跨服边境之地买buff
	{
	public:
		CSCrossBianJingZhiDiBuyBuff();
		MessageHeader header;
	};

	/////////////////////////////////////////////////  跨服竞技场  /////////////////////////////////////////////////////////
	enum CROSS_CHALLENGEFIELD_OPERA_REQ
	{
		CROSS_CHALLENGEFIELD_OPERA_REQ_OPPONENT_INFO = 0,		 // 请求对手外观  p1:是否获取前几名(1/0)
		CROSS_CHALLENGEFIELD_OPERA_REQ_RANK_INFO = 1,			 // 请求英雄榜(前100名)
		CROSS_CHALLENGEFIELD_OPERA_REQ_FETCH_REWARD = 2,		 // 领取奖励
		CROSS_CHALLENGEFIELD_OPERA_REQ_SELFT_INFO = 3,			 // 获得自己信息和战报
		CROSS_CHALLENGEFIELD_OPERA_REQ_FIGHT = 4,				 // 挑战请求 p1:对手index p2:是否无视排名变化 p3:rank_pos
		CROSS_CHALLENGEFIELD_OPERA_REQ_REFRESH = 5,				 // 刷新挑战列表 
		CROSS_CHALLENGEFIELD_OPERA_REQ_READY = 6,				 // 请求准备正式开始战斗倒计时 
	};

	class CSCrossChallengeFieldOpera						// 8940 跨服竞技场请求
	{
	public:
		CSCrossChallengeFieldOpera();
		MessageHeader		header;

		short				req_type;
		short				param1;
		short				param2;
		short				param3;
	};

	class SCCrossChallengeFieldStatus								// 8941 场景用户信息
	{
	public:
		SCCrossChallengeFieldStatus();
		MessageHeader		header;

		struct SideInfo
		{
			long long		role_id;
			ObjID			obj_id;
			short			level;
			GameName		name;
			char			camp;
			char			prof;
			char			avatar;
			char			sex;
			Attribute		hp;
			Attribute		max_hp;
			Attribute		mp;
			Attribute		max_mp;
			Attribute		speed;
			short			pos_x;
			short			pos_y;
			Dir				dir;
			Scalar			distance;
			int				capability;
			GuildID			guild_id;
			GuildName		guild_name;
			char			guild_post;
			char			reserve_ch;
			short			reserve_sh;
		};

		int					status;				// CHALLENGE_FIELD_STATE
		unsigned int		next_status_timestamp;
		SideInfo			side_info_list[CHALLENGE_FIELD_SIDE_MAX];
	};

	class SCCrossChallengeFieldOpponentInfo							// 8943 对手对手详细信息 
	{
	public:
		SCCrossChallengeFieldOpponentInfo();
		MessageHeader		header;

		struct OpponentInfo
		{
			int server_id;
			long long user_id;
			long long avatar_timestamp;				
			char camp;
			char prof;
			char sex;
			char avatar;
			int capability;
			int best_rank_break_level;

			GameName name;

			RoleAppearance appearance;
		};

		int opponent_count;
		OpponentInfo opponent_info_list[CHALLENGE_FIELD_OPPONENT_MAX];
	};

	class SCCrossChallengeFieldUserInfo								//8942 挑战列表信息和个人信息
	{
	public:
		SCCrossChallengeFieldUserInfo();
		MessageHeader		header;

		struct  OpponentInfo
		{
			long long user_id;
			int rank_pos;
		};

		int rank_pos;
		int curr_opponent_idx;
		int join_times;
		int buy_join_times;
		int jifen;
		int jifen_reward_flag;
		int reward_guanghui;
		int reward_bind_gold;
		int liansheng;
		int buy_buff_times;
		int best_rank_pos;
		int free_day_times;

		struct RewardItem
		{
			ItemID item_id;
			short num;
		};
		RewardItem reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
		OpponentInfo opponent_userid_list[CHALLENGE_FIELD_OPPONENT_MAX];
	};

	class SCCrossChallengeFieldOpponentRankPosChange					// 8948 排位变化通知
	{
	public:
		SCCrossChallengeFieldOpponentRankPosChange();
		MessageHeader		header;

		long long user_id;
		int rank_pos;
	};

	class SCCrossChallengeFieldReportInfo							// 8946 战报
	{
	public:
		SCCrossChallengeFieldReportInfo();
		MessageHeader		header;

		struct ReportItem
		{
			unsigned int		challenge_time;					// 时间
			long long			target_uid;						// 对方id
			GameName			target_name;					// 对方名称
			char				is_sponsor;						// 自己是否为挑战者
			char				is_win;							// 是否胜利
			short				reserve_sh;						// 保留
			unsigned short		old_rankpos;					// 竞技前排名
			unsigned short		new_rankpos;					// 竞技后排名
		};

		int report_count;
		ReportItem report_list[CHALLENGE_FIELD_REPORT_MAX_COUNT];
	};

	class SCCrossChallengeFieldRankInfo								// 8947 英雄榜排行信息
	{
	public:
		SCCrossChallengeFieldRankInfo();
		MessageHeader		header;

		struct RankItem
		{
			RankItem() :user_id(0), capability(0)
			{
				memset(user_name, 0, sizeof(user_name));
			}

			int server_id;
			long long user_id;
			int capability;
			GameName user_name;
			char sex;
			char is_robot;
			char prof;
			char reserved;
			int role_level;

			RoleAppearance appearance;
		};

		RankItem rank_list[CHALLENGE_FIELD_TOP_RANKER_ITEM_NUM];
	};

	class SCCrossChallengeFieldBeDefeatNotice // 8944 被打败通知
	{
	public:
		SCCrossChallengeFieldBeDefeatNotice();
		MessageHeader		header;
	};

	class SCCrossChallengeFieldWin			// 8945 竞技场直接胜利 
	{
	public:
		SCCrossChallengeFieldWin();
		MessageHeader		header;

		short old_rank_pos;
		short new_rank_pos;
	};


	//////////////////////////////////////////////////////////////////////////

	class SCFirstRechargeBuffFlag	//8933
	{
	public:
		SCFirstRechargeBuffFlag();
		MessageHeader header;

		short ra_is_has_first_recharge_attr_add;	//是否拥有首充属性加成
		short reserve_sh;							//保留
	};

	/////////////////////////////////////  随机活动 幸运云购  ///////////////////////////////////
	enum RA_LUCKY_CLOUD_BUY_OPERA_TYPE
	{
		RA_LUCKY_CLOUD_BUY_OPERA_TYPE_INFO = 0,			// 请求信息
		RA_LUCKY_CLOUD_BUY_OPERA_TYPE_BUY = 1,			// 购买
		RA_LUCKY_CLOUD_BUY_OPERA_TYPE_OOPEN = 2,		// 打开面板（参数1为上次打开面板的时间戳）
		RA_LUCKY_CLOUD_BUY_OPERA_TYPE_CLOSE = 3,		// 关闭面板
		RA_LUCKY_CLOUD_BUY_OPERA_TYPE_LUCKY = 4,		// 幸运儿信息

		RA_LUCKY_CLOUD_BUY_OPERA_TYPE_MAX,
	};

	struct SCRALuckyCloudBuyInfo		//	幸运云购基本信息 8954
	{
	public:
		SCRALuckyCloudBuyInfo();
		MessageHeader header;

		unsigned short seq;			//索引
		unsigned short buy_self;	//自己购买的数量
	};

	class SCRALuckyCloudBuyBuyList	// 幸运云购购买记录 8955
	{
	public:
		SCRALuckyCloudBuyBuyList();
		MessageHeader header;

		unsigned int ret_timestamp;						// 返回的时间戳
		unsigned short total_buy;						//库存购买的数量
		unsigned short num;								// list元素数量
		GameName name_list[LUCKY_BUY_RECORD_MAX_NUM];	// 最大12个
	};

	class SCRALuckyCloudBuyOpenInfo	// 幸运云购活动开启通知 8956
	{
	public:
		SCRALuckyCloudBuyOpenInfo();
		MessageHeader header;

		unsigned int is_open;	// 0 目前不能参与， 1 开启可参与
	};

	///////////////////////////////////////  狂欢大乐购   /////////////////////////////////////////
	enum RA_CRACYBUY_TYPE
	{
		RA_CRACYBUY_ALL_INFO,							 // 请求充值信息
		RA_CRACYBUY_LIMIT_INFO,							 // 请求限购信息
		RA_CRACYBUY_BUY,								 // 购买请求 param1:物索引
	};

	class SCRACrazyBuyAllInfo	// 8959 疯狂抢购面板信息
	{
	public:
		SCRACrazyBuyAllInfo();
		MessageHeader header;

		int chongzhi;
		int level;
	};

	class SCRACracyBuyLimitInfo	// 8960 限购信息
	{
	public:
		SCRACracyBuyLimitInfo();
		MessageHeader header;

		struct Limit_Info
		{
			int person_limit;
			int all_limit;
		};

		Limit_Info item_limit_info[RA_MAX_CRACY_BUY_NUM_LIMIT];
	};

	///////////////////////////////////// 跨服护送水晶 /////////////////////////////////////

	enum CS_CROSS_HUSONG_SHUIJING_OPERA_TYPE
	{
		CS_CROSS_HUSONG_SHUIJING_OPERA_INFO,
		CS_CROSS_HUSONG_SHUIJING_OPERA_COMMIT,		
		CS_CROSS_HUSONG_SHUIJING_OPERA_GATHER_INFO,
	};

	class CSCrossHusongShuijingOpera // 8979
	{
	public:
		CSCrossHusongShuijingOpera();
		MessageHeader header;

		int req_type;
		int param1;
		int param2;
	};

	class SCCrossHusongShuijingInfo	//8980
	{
	public:
		SCCrossHusongShuijingInfo();
		MessageHeader header;

		short vip_buy_times;				// 购买次数
		short rob_count;					// 截取成功次数
		short gather_times;					// 采集次数
		unsigned short commit_count;		// 护送提交次数
		short husong_type;					// 护送类型
		short husong_status;				// 护送状态
		UInt32 invalid_time;				// 护送的失效时间
	};

	enum CS_CROSS_HUSONG_SHUIJING_GATHER_INFO_TYPE
	{
		CS_CROSS_HUSONG_SHUIJING_GATHER_DEFAULT, //0 信息变化
		CS_CROSS_HUSONG_SHUIJING_GATHER_REFRESH, //1 水晶刷新
	};

	class SCCrossHusongShuijingGatherInfo // 8981
	{
	public:
		SCCrossHusongShuijingGatherInfo();
		MessageHeader header;

		short type;						//水晶信息类型：0 信息变化，1 水晶刷新
		short cur_remain_gather_time_big;//大水晶当前剩余可采集次数
		UInt32 next_refresh_time_big;     //大水晶下次刷新时间
	};

}

#pragma pack(pop)

#endif	// __MSG_ACTIVITY_HPP__
