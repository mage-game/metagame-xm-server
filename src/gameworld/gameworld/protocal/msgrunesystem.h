#ifndef __MSG_RUNE_SYSTEM_HPP__
#define __MSG_RUNE_SYSTEM_HPP__

#include "servercommon/runesystemdef.hpp"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push, 4)

namespace Protocol
{
	enum RUNE_SYSTEM_REQ_TYPE
	{
		RUNE_SYSTEM_REQ_TYPE_ALL_INFO = 0,				// 请求所有信息
		RUNE_SYSTEM_REQ_TYPE_BAG_ALL_INFO,				// 请求背包所有信息
		RUNE_SYSTEM_REQ_TYPE_RUNE_GRID_ALL_INFO,		// 请求符文槽所有信息
		RUNE_SYSTEM_REQ_TYPE_DISPOSE_ONE,				// 分解一个				p1 虚拟背包索引
		RUNE_SYSTEM_REQ_TYPE_COMPOSE,					// 合成					p1 索引1		p2 非零（索引1是背包索引);零（索引1是格子索引） p3 索引2  p4 非零（索引2是背包索引);零（索引2是格子索引）
		RUNE_SYSTEM_REQ_TYPE_SET_RUAN,					// 装备符文				p1 虚拟背包索引	p2 符文槽格子索引
		RUNE_SYSTEM_REQ_TYPE_XUNBAO_ONE,				// 寻宝一次
		RUNE_SYSTEM_REQ_TYPE_XUNBAO_TEN,				// 寻宝十次
		RUNE_SYSTEM_REQ_TYPE_UPLEVEL,					// 升级符文				p1 符文槽格子索引
		RUNE_SYSTEM_REQ_TYPE_CONVERT,					// 符文兑换
		RUNE_SYSTEM_REQ_TYPE_OTHER_INFO,				// 其他信息
		RUNE_SYSTEM_REQ_TYPE_AWAKEN,					// 符文格觉醒			p1 格子， p2觉醒类型
		RUNE_SYSTEM_REQ_TYPE_AWAKEN_CALC_REQ,			// 符文格觉醒重算战力
		RUNE_SYSTEM_REQ_TYPE_RAND_ZHILING_SLOT,			// 随机注灵槽
		RUNE_SYSTEM_REQ_TYPE_ZHULING,					// 注灵，参数1 符文格子index
	};

	class CSRuneSystemReq
	{
	public:
		CSRuneSystemReq();
		MessageHeader header;

		short req_type;
		short param1;
		short param2;
		short param3;
		short param4;
	};

	class CSRuneSystemDisposeOneKey
	{
	public:
		CSRuneSystemDisposeOneKey();
		MessageHeader header;

		int list_count;
		short dispose_list[RUNE_SYSTEM_BAG_MAX_GRIDS];	
	};

	enum RUNE_SYSTEM_INFO_TYPE
	{
		RUNE_SYSTEM_INFO_TYPE_INVAILD = 0,
		RUNE_SYSTEM_INFO_TYPE_ALL_BAG_INFO,				// 背包全部信息
		RUNE_SYSTEM_INFO_TYPE_RUNE_XUNBAO_INFO,			// 符文寻宝信息
		RUNE_SYSTEM_INFO_TYPE_OPEN_BOX_INFO,			// 打开符文宝箱
		RUNE_SYSTEM_INFO_TYPE_CONVERT_INFO,				// 符文兑换信息
	};

	struct RuneSystemUnit
	{
		RuneSystemUnit() : quanlity(-1), type(-1), level(0), index(-1) {}

		char quanlity;
		char type;
		short level;
		int index;
	};

	class SCRuneSystemBagInfo
	{
	public:
		SCRuneSystemBagInfo();
		MessageHeader header;

		int info_type;
		int jinghua_box_magic_crystal;
		int count;
		RuneSystemUnit bag_list[RUNE_SYSTEM_BAG_MAX_GRIDS];
	};

	class SCRuneSystemRuneGridInfo
	{
	public:
		SCRuneSystemRuneGridInfo();
		MessageHeader header;

		RuneParam rune_grid[RUNE_SYSTEM_SLOT_MAX_NUM];
		RuneAwakenParm rune_grid_awaken[RUNE_SYSTEM_SLOT_MAX_NUM];
	};

	class SCRuneSystemRuneGridAwakenInfo		// 6691
	{
	public:
		SCRuneSystemRuneGridAwakenInfo();
		MessageHeader header;

		short awaken_seq;			// 觉醒索引
		short is_need_recalc;		// 是否需要发起重算属性
	};

	class SCRuneSystemOtherInfo
	{
	public:
		SCRuneSystemOtherInfo();
		MessageHeader header;

		int pass_layer;										// 通关层数
		int rune_jinghua;									// 符文精华
		int rune_suipian;									// 符文碎片
		int magic_crystal;									// 符文水晶
		char suipian[RUNE_SYSTEM_XUNBAO_RUNE_MAX_COUNT];
		short reserve_sh;
		
		unsigned int next_free_xunbao_timestamp;			// 下次免费时间戳
		unsigned int total_xunbao_times;					// 累计寻宝次数
		unsigned short rune_slot_open_flag;					// 符文槽开启标记s
		char free_xunbao_times;								// 是否有免费寻宝
		char reserve_ch;
		int rune_awaken_times;								// 符文元宝觉醒次数
	};

	class CSRuneTowerFetchTime
	{
	public:
		CSRuneTowerFetchTime();
		MessageHeader header;
	};

	class SCRuneTowerInfo
	{
	public:
		SCRuneTowerInfo();
		MessageHeader header;

		int pass_layer;
		int fb_today_layer;
		int offline_time;
		long long add_exp;
		int add_jinghua;
		int add_equip_blue;
		int add_equip_purple;
		int add_equip_orange;
		int fetch_time_count;
	};

	class SCRuneTowerFbInfo
	{
	public:
		SCRuneTowerFbInfo();
		MessageHeader header;

		char is_finish;
		char is_pass;
		char is_boss_scene;
		char reserved;
		int layer;
		int kill_monster_num;
		int buff_time;
		long long add_exp;
		int add_jinghua;
		int add_equip_blue;
		int add_equip_purple;
		int add_equip_orange;
		int monster_pos_index;
		unsigned int boss_end_time;

		struct ItemInfo
		{
			ItemID item_id;
			short num;
		};
		static const int ITEM_MAX = 5;

		int boss_drop_item_count;
		ItemInfo boss_drop_item_list[ITEM_MAX];
	};

	class SCRuneTowerOfflineInfo
	{
	public:
		SCRuneTowerOfflineInfo();
		MessageHeader header;

		int fb_offline_time;
		int guaji_time;
		int kill_monster_num;
		int old_level;
		int new_level;
		long long add_exp;
		int add_jinghua;
		int add_equip_blue;
		int add_equip_purple;
		int add_equip_orange;
		int add_mojing;
		int recycl_equip_blue;
		int recycl_equip_purple;
		int recycl_equip_orange;
	};

	class SCRuneSystemComposeInfo
	{
	public:
		SCRuneSystemComposeInfo();
		MessageHeader header;
	};

	enum RUNE_TOWER_FB_OPER_TYPE
	{
		RUNE_TOWER_FB_OPER_AUTOFB = 0,
		RUNE_TOWER_FB_OPER_REFRESH_MONSTER = 1,
	};
	class CSRuneTowerAutoFb   // 6992
	{
	public:
		CSRuneTowerAutoFb();
		MessageHeader header;

		int req_type;
	};

	class SCRuneTowerAutoReward
	{
	public:
		SCRuneTowerAutoReward();
		MessageHeader header;

		static const int MAX_REWARD_COUNT = 60;
		struct RewardItem
		{
			RewardItem() : item_id(0), num(0) {}
			ItemID item_id;
			short num;
		};

		int reward_jinghua;
		int item_count;
		RewardItem item_list[MAX_REWARD_COUNT];
	};

	// 符文注灵
	class SCRuneSystemZhulingRandResultInfo			// 8442
	{
	public:
		SCRuneSystemZhulingRandResultInfo();
		MessageHeader header;

		int index;
		int zhuling_slot_bless;
	};

	class SCRuneSystemZhulingAllInfo			// 8443
	{
	public:
		SCRuneSystemZhulingAllInfo();
		MessageHeader header;

		int zhuling_slot_bless;
		RuneZhuLingParam run_zhuling_list[RUNE_SYSTEM_SLOT_MAX_NUM];
	};
}

#pragma pack(pop)
#endif
