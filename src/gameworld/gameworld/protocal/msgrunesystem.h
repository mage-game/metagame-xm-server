#ifndef __MSG_RUNE_SYSTEM_HPP__
#define __MSG_RUNE_SYSTEM_HPP__

#include "servercommon/runesystemdef.hpp"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push, 4)

namespace Protocol
{
	enum RUNE_SYSTEM_REQ_TYPE
	{
		RUNE_SYSTEM_REQ_TYPE_ALL_INFO = 0,				// ����������Ϣ
		RUNE_SYSTEM_REQ_TYPE_BAG_ALL_INFO,				// ���󱳰�������Ϣ
		RUNE_SYSTEM_REQ_TYPE_RUNE_GRID_ALL_INFO,		// ������Ĳ�������Ϣ
		RUNE_SYSTEM_REQ_TYPE_DISPOSE_ONE,				// �ֽ�һ��				p1 ���ⱳ������
		RUNE_SYSTEM_REQ_TYPE_COMPOSE,					// �ϳ�					p1 ����1		p2 ���㣨����1�Ǳ�������);�㣨����1�Ǹ��������� p3 ����2  p4 ���㣨����2�Ǳ�������);�㣨����2�Ǹ���������
		RUNE_SYSTEM_REQ_TYPE_SET_RUAN,					// װ������				p1 ���ⱳ������	p2 ���Ĳ۸�������
		RUNE_SYSTEM_REQ_TYPE_XUNBAO_ONE,				// Ѱ��һ��
		RUNE_SYSTEM_REQ_TYPE_XUNBAO_TEN,				// Ѱ��ʮ��
		RUNE_SYSTEM_REQ_TYPE_UPLEVEL,					// ��������				p1 ���Ĳ۸�������
		RUNE_SYSTEM_REQ_TYPE_CONVERT,					// ���Ķһ�
		RUNE_SYSTEM_REQ_TYPE_OTHER_INFO,				// ������Ϣ
		RUNE_SYSTEM_REQ_TYPE_AWAKEN,					// ���ĸ����			p1 ���ӣ� p2��������
		RUNE_SYSTEM_REQ_TYPE_AWAKEN_CALC_REQ,			// ���ĸ��������ս��
		RUNE_SYSTEM_REQ_TYPE_RAND_ZHILING_SLOT,			// ���ע���
		RUNE_SYSTEM_REQ_TYPE_ZHULING,					// ע�飬����1 ���ĸ���index
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
		RUNE_SYSTEM_INFO_TYPE_ALL_BAG_INFO,				// ����ȫ����Ϣ
		RUNE_SYSTEM_INFO_TYPE_RUNE_XUNBAO_INFO,			// ����Ѱ����Ϣ
		RUNE_SYSTEM_INFO_TYPE_OPEN_BOX_INFO,			// �򿪷��ı���
		RUNE_SYSTEM_INFO_TYPE_CONVERT_INFO,				// ���Ķһ���Ϣ
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

		short awaken_seq;			// ��������
		short is_need_recalc;		// �Ƿ���Ҫ������������
	};

	class SCRuneSystemOtherInfo
	{
	public:
		SCRuneSystemOtherInfo();
		MessageHeader header;

		int pass_layer;										// ͨ�ز���
		int rune_jinghua;									// ���ľ���
		int rune_suipian;									// ������Ƭ
		int magic_crystal;									// ����ˮ��
		char suipian[RUNE_SYSTEM_XUNBAO_RUNE_MAX_COUNT];
		short reserve_sh;
		
		unsigned int next_free_xunbao_timestamp;			// �´����ʱ���
		unsigned int total_xunbao_times;					// �ۼ�Ѱ������
		unsigned short rune_slot_open_flag;					// ���Ĳۿ������s
		char free_xunbao_times;								// �Ƿ������Ѱ��
		char reserve_ch;
		int rune_awaken_times;								// ����Ԫ�����Ѵ���
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

	// ����ע��
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
