#ifndef MSG_JINGLING_H
#define MSG_JINGLING_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/jinglingdef.hpp"
#include "gameworld/other/jinglingadvantage/jinglingadvantage.hpp"

#pragma pack(push, 4) 

namespace Protocol
{
	enum JINGLING_OPER_TYPE
	{
		JINGLING_OPER_TAKEOFF = 0,			// 取下
		JINGLING_OPER_CALLBACK,				// 召回
		JINGLING_OPER_FIGHTOUT,				// 出战
		JINGLING_OPER_UPLEVEL,				// 升级
		JINGLING_OPER_UPLEVELCARD,			// 升级卡牌
		JINGLING_OPER_RENAME,				// 更改精灵名字
		JINGLING_OPER_UPGRADE,				// 升阶
		JINGLING_OPER_STRENGTH,				// 强化装备
		JINGLING_OPER_USEIMAGE,				// 使用形象
		JINGLING_OPER_USEPHANTOM_IMG,		// 使用幻化形象
		JINGLING_OPER_PHANTOMUPLEVEL,		// 幻化形象升级
		JINGLING_OPER_UPGRADESOUL,			// 灵魄升级
		JINGLING_OPER_UPGRADE_HALO,			// 精灵光环
		JINGLING_OPER_USE_HALO_IMG,			// 使用精灵光环
		JINGLING_OPER_ONEKEY_RECYCL_BAG,    // 一键回收精灵背包的精灵
		JINGLING_OPER_UPLEVEL_WUXING,		// 升级悟性
		JINGLING_OPER_UPLEVEL_XIANZHEN,		// 升级仙阵
		JINGLING_OPER_UPLEVEL_HUNYU,		// 升级魂玉
		JINGLING_OPER_REMOVE_SKILL,			// 技能 遗忘
		JINGLING_OPER_CHANGE_MOVE,			// 技能 变成可移动
		JINGLING_OPER_PUT_ON_SKILL,			// 技能 穿戴
		JINGLING_OPER_TAKE_OFF_SKILL,		// 技能 脱下
		JINGLING_OPER_LEARN_SKILL,			// 技能 学习
		JINGLING_OPER_REFRESH,				// 技能 刷新
		JINGLING_OPER_GET,					// 技能 获取
		JINGLING_OPER_REFRESH_ACTIVE,		// 技能 刷新激活
		JINGLING_OPER_OPEN_SKILL_SLOT,      // 技能槽开启
	};

	class SCJingLingInfo					// 精灵信息5671
	{
	public:
		SCJingLingInfo();
		MessageHeader header;

		JingLingCardParamItem card_list[JINGLING_CARD_MAX_TYPE];
 		int shuxingdan_list[JINGLING_SHUXINGDAN_MAX_TYPE];
 		short equip_strength_list[JINGLING_EQUIP_MAX_PART];

		struct InfoType 
		{
			int	index;
			ItemDataWrapper	item_wrapper;
		};

		GameName jingling_name;
		ItemID use_jingling_id;
		short use_imageid;
		int m_active_image_flag;
 		int grade;
 		int grade_bless;

		short phantom_level_list[JINGLING_PTHANTOM_MAX_TYPE];
		unsigned char phantom_image_flag[JINGLING_PTHANTOM_MAX_BYTE];
		int phantom_imageid;
		
 		short soul_level_list[JINGLING_CARD_MAX_TYPE];
 		int taozhuang_level;

 		int halo_active_image_flag;
 		short halo_level;
 		short use_halo_img;
 		int halo_bless_val;

		int xianzhen_level;
		int xianzhen_exp;
		int xianzhen_up_count;
		int hunyu_level_list[XIAN_ZHEN_HUN_YU_TYPE_MAX];
		JingLingSkillInfo skill_storage_list[JING_LING_SKILL_STORAGE_MAX];
		JingLingRefreshItem skill_refresh_item_list[JING_LING_SKILL_REFRESH_ITEM_MAX];

		int count;
		InfoType jingling_list[JINGLING_MAX_TAKEON_NUM];
	};
	
	class CSJingLingOper					// 5670精灵操作请求
	{
	public:
		CSJingLingOper();
		MessageHeader header;

		short oper_type;
		short param1;
		short param2;
		short param3;
		int param4;
		
		GameName jingling_name;
	};

	class SCJingLingViewChange				// 5672精灵形象改变广播
	{
	public:
		SCJingLingViewChange();
		MessageHeader header;

		ObjID obj_id;
		ItemID jingling_id;
		short level;
		short use_imageid;
		GameName jingling_name;
		int jingling_phantom_img;
		int jingling_cur_use_halo_img;
	};

	enum JING_LING_HOME_OPER_TYPE
	{
		JING_LING_HOME_OPER_TYPE_GET_INFO = 0,		// 查询信息
		JING_LING_HOME_OPER_TYPE_PUT_HOME,			// 放入家园
		JING_LING_HOME_OPER_TYPE_QUICK,				// 加快速度
		JING_LING_HOME_OPER_TYPE_GET_REWARD,		// 领取奖励
		JING_LING_HOME_OPER_TYPE_ROB,				// 掠夺
		JING_LING_HOME_OPER_TYPE_OUT,				// 取出
		JING_LING_HOME_OPER_TYPE_REFRESH_LIST,		// 刷新
		JING_LING_HOME_OPER_TYPE_READ_ROB_RECORD,	// 阅读被掠夺记录
	};

	class CSJingLingHomeOperReq
	{
	public:
		CSJingLingHomeOperReq();
		MessageHeader header;

		int oper_type;
		int role_id;
		short param1;
		short param2;
	};

	enum JING_LING_HOME_REASON
	{
		JING_LING_HOME_REASON_DEF = 0,
		JING_LING_HOME_REASON_PUT,
		JING_LING_HOME_REASON_QUICK,
		JING_LING_HOME_REASON_GET_REWARD,
		JING_LING_HOME_REASON_ROB_WIN,
		JING_LING_HOME_REASON_ROB_LOST,
		JING_LING_HOME_REASON_TAKE_OUT,
	};
	class SCJingLingHomeInfo
	{
	public:
		SCJingLingHomeInfo();
		MessageHeader header;

		short reason;
		short rob_times_of_me;
		int role_id;
		GameName name;
		JingLingHomeItem item_list[JINGLING_MAX_TAKEON_NUM];
	};

	class SCJingLingHomeListInfo
	{
	public:
		SCJingLingHomeListInfo();
		MessageHeader header;

		static const int LIST_MAX = 10;
		struct HomeInfo
		{
			int role_id;
			char prof;
			char sex;
			char camp;
			char vip_level;
			long long avatar_timestamp;
			GameName name;
		};

		int info_count;
		HomeInfo info_list[LIST_MAX];
	};

	class SCJingLingHomeRobRecord
	{
	public:
		SCJingLingHomeRobRecord();
		MessageHeader header;

		struct RobRecord
		{
			int role_id;
			GameName name;
			unsigned int rob_time;
		};

		unsigned int read_rob_record_time;
		int record_count;
		RobRecord rob_record_list[JING_LING_HOME_ROB_RECORD_MAX];
	};

	class SCJingLingHomeRobAck
	{
	public:
		SCJingLingHomeRobAck();
		MessageHeader header;

		struct RobItem
		{
			ItemID item_id;
			short item_num;
		};

		int role_id;
		int rob_lingjing;
		int rob_hunli;
		short is_win;
		short item_count;
		RobItem item_list[JING_LING_HOME_REWARD_ITEM_MAX];
	};

	enum JL_EXPLORE_OPER_TYPE
	{
		JL_EXPLORE_OPER_TYPE_SELECT_MODE = 0,
		JL_EXPLORE_OPER_TYPE_EXPLORE,
		JL_EXPLORE_OPER_TYPE_FETCH,
		JL_EXPLORE_OPER_TYPE_RESET,
		JL_EXPLORE_OPER_TYPE_BUY_BUFF,
	};
	class CSJinglIngExploreOperReq
	{
	public:
		CSJinglIngExploreOperReq();
		MessageHeader header;

		int oper_type;
		short param1;
		short param2;
	};

	enum JL_EXPLORE_INFO_REASON
	{
		JL_EXPLORE_INFO_REASON_DEF = 0,
		JL_EXPLORE_INFO_REASON_SELECT,
		JL_EXPLORE_INFO_REASON_CHALLENGE_SUCC,
		JL_EXPLORE_INFO_REASON_CHALLENGE_FAIL,
		JL_EXPLORE_INFO_REASON_FETCH,
		JL_EXPLORE_INFO_REASON_RESET,
		JL_EXPLORE_INFO_REASON_BUY_BUFF,
	};
	class SCJinglIngExploreInfo
	{
	public:
		SCJinglIngExploreInfo();
		MessageHeader header;

		short reason;
		short explore_mode;
		int explore_maxhp;
		int explore_hp;
		int buy_buff_count;
		JingLingExploreInfo explore_info_list[JING_LING_EXPLORE_LEVEL_COUNT];
	};

	class SCJingLingAdvantageInfo					//5606 精灵奇遇所有信息
	{
	public:
		SCJingLingAdvantageInfo();
		MessageHeader header;

		struct GatherPosInfo
		{
			int scene_id;
			short purple_count;
			short blue_count;
		};

		static const int SCENE_COUNT = 9;
		GatherPosInfo pos_list[SCENE_COUNT];
	};

	class SCJingLingAdvantageCount					//5607 精灵奇遇蓝色精灵数量
	{
	public:
		SCJingLingAdvantageCount();
		MessageHeader header;

		int today_gather_blue_jingling_count;		//今天获得的蓝色精灵数量
	};

	class SCJingLingAdvantageBossInfo				//5608 精灵奇遇boss刷新信息
	{
	public:
		SCJingLingAdvantageBossInfo();
		MessageHeader header;

		unsigned short monster_id;					//boss_id
		short reserve_sh;
		GameName role_name;
		short scene_id;
		short reserve_1;
	};

	enum JINGLING_ADCANTAGE_OPER_TYPE
	{
		JINGLING_ADCANTAGE_OPER_TYPE_BOSS = 0,        // 奇遇boss   monster_id 为bossid
		JINGLING_ADCANTAGE_OPER_TYPE_EGG,             // 仙宠蛋位置 monster_id 为场景id
	};

	class CSJingLingAdvantageBossEnter				//5609 飞至精灵奇遇boss
	{
	public:
		CSJingLingAdvantageBossEnter();
		MessageHeader header;

		int oper_type;
		int monster_id;								//怪物id
	};

	class SCJingLingEggPosInfo          // 5610   精灵蛋位置信息
	{
	public:
		SCJingLingEggPosInfo();
		MessageHeader header;

		int scene_id;
		int pos_x;
		int pos_y;
	};
}

#pragma pack(pop)

#endif

