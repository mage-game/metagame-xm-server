#ifndef MSG_XIANNV_H
#define MSG_XIANNV_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/xiannvparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCAllXiannvInfo								// 仙女所有信息2201
	{
	public:
		SCAllXiannvInfo();
		MessageHeader header;

		int active_xiannv_flag;
		unsigned char active_huanhua_flag[MAX_XIANNV_ACTIVE_SPECIAL_IMAGE_BYTE];
		short huanhua_id;
		short reserved;
		GameName xiannv_name[MAX_XIANNV_COUNT];
		int xiannv_huanhua_level[MAX_XIANNV_HUANHUA_COUNT];

		XiannvItem xn_item_list[MAX_XIANNV_COUNT];
		char pos_list[MAX_XIANNV_POS];

		int shengwu_lingye;									// 圣物灵液
		short cur_gold_miling_times;						// 当前次元宝觅灵次数（领取奖励后清零）
		short shengwu_chou_id;								// 未领取的抽奖exp所属圣物id
		short shengwu_chou_exp_list[XIANNV_SHENGWU_CHOU_EXP_COUNT];		// 未领取的抽奖exp奖励
		NvshenShengwuItem all_shengwu_list[XIANNV_SHENGWU_MAX_ID + 1];

		short grid_level_list[XIANNV_SHENGWU_GONGMING_MAX_GRID_ID + 1];
		char day_free_miling_times;							// 当天免费觅灵次数
		char day_fetch_ling_time;							// 当天获取灵液次数

		char miling_list[XIANNV_SHENGWU_MILING_FONT_COUNT];	// 当前抽到的灵类型列表
		short reserve_sh;
		int shengwu_essence;						// 仙器精华数量
	};

	class SCXiannvInfo									// 指定仙女信息2200
	{
	public:
		SCXiannvInfo();
		MessageHeader header;

		short notify_reaon;
		short xiannv_id;
		XiannvItem xn_item;
	};

	class SCXiannvViewChange							// 仙女形象改变广播2203
	{
	public:
		SCXiannvViewChange();
		MessageHeader header;

		ObjID obj_id;
		short use_xiannv_id;
		short huanhua_id;
		short reserved;
		GameName xiannv_name;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSXiannvCall									// 请求仙女出战2250
	{
	public:
		CSXiannvCall();
		MessageHeader header;

		char pos_list[MAX_XIANNV_POS];
	};

	class CSXiannvRename								// 请求重命名2251
	{
	public:
		CSXiannvRename();
		MessageHeader header;

		short xiannv_id;
		GameName new_name;
	};

	class CSXiannvActiveReq								// 请求激活2252
	{
	public:
		CSXiannvActiveReq();
		MessageHeader header;

		short xiannv_id;
		short item_index;
	};

	class CSXiannvAddZizhiReq							// 请求加资质2253
	{
	public:
		CSXiannvAddZizhiReq();
		MessageHeader header;

		short xiannv_id;
		short auto_buy;
	};

	class CSXiannvActiveHuanhua							// 请求激活幻化2265
	{
	public:
		CSXiannvActiveHuanhua();
		MessageHeader header;

		short xiannv_id;
		short item_index;
	};

	class CSXiannvImageReq								//请求形象2266
	{
	public:
		CSXiannvImageReq();
		MessageHeader header;

		short huanhua_id;
	};

	class CSXiannvHuanHuaUpLevelReq						// 请求幻化升级2267
	{
	public:
		CSXiannvHuanHuaUpLevelReq();
		MessageHeader header;

		short huanhua_id;
		short auto_buy;
	};


	class CSXiannvChangeSkillReq						// 2268 请求改变仙女技能
	{
	public:
		CSXiannvChangeSkillReq();
		MessageHeader header;

		unsigned short xiannv_skill_id;
		unsigned short reserve;
		int xiannv_skill_level;
	};

	class CSXiannvShengwuReq							// 2270 女神圣器请求协议
	{
	public:
		enum ReqType
		{
			NORMAL_CHOU_EXP = 0,						// 抽取经验-普通抽取，param1 是否自动购买，param2 是否自动选择碎片，param3 是否10连抽
			PERFECT_CHOU_EXP,							// 抽取经验-完美抽取，param1 是否自动购买，param2 是否自动选择碎片，param3 是否10连抽
			FETCH_EXP,									// 领取经验
			UPGRADE_GRID,								// 提升共鸣格子，param1 格子ID
			CHOU_LING,									// 灵液抽取
			FETCH_LING,									// 灵液领取，param1 是否双倍领取（0 否，1 是）
			UPGRADE_EXP,								// 升阶，param1 是圣物ID
		};

		CSXiannvShengwuReq();
		MessageHeader header;

		short req_type;
		short param1;
		short param2;
		short param3;
	};

	class SCXiannvShengwuChangeInfo						// 2271 女神圣物变化增量信息
	{
	public:
		enum NotifyType
		{
			LINGYE = 0,									// 基本信息，param1 今日已使用免费引灵次数, param2 今日已使用元宝引灵次数，param3 今日已领取灵液次数，param4 剩余灵液
			SHENGWU_INFO,								// 圣物信息，param1 圣物ID，param2 圣物等级，param3 NULL，param4 圣物经验值
			GRID_INFO,									// 格子信息，param1 格子ID，param2 格子等级
			ESSENCE,									// 圣物精华信息, param3为变化数量 param4 圣物精华数量
		};

		SCXiannvShengwuChangeInfo();
		MessageHeader header;

		short notify_type;
		short param1;
		short param2;
		short param3;
		int param4;
	};

	class SCXiannvShengwuMilingList						// 2272 女神圣物觅灵列表
	{
	public:
		SCXiannvShengwuMilingList();
		MessageHeader header;

		char miling_list[XIANNV_SHENGWU_MILING_FONT_COUNT];
		short reserve_sh;
	};

	class SCXiannvShengwuChouExpList					// 2273 女神圣物回忆获取的经验列表
	{
	public:
		SCXiannvShengwuChouExpList();
		MessageHeader header;

		char chou_type;									// 回忆（抽经验）类型
		char reserve_ch;
		short shengwu_chou_id;							// 未领取的抽奖exp所属圣物id
		short shengwu_chou_exp_list[XIANNV_SHENGWU_CHOU_EXP_COUNT];		// 未领取的抽奖exp奖励
	};


	class SCXiannvShengwuChouExpResult					// 2274 女神圣物抽经验结果（用于客户端播放特效）
	{
	public:
		SCXiannvShengwuChouExpResult();
		MessageHeader header;

		struct FetchItem
		{
			short shengwu_id;
			short add_exp;
		};

		short is_auto_fetch;								// 是否自动获取

		short add_exp_count;								// 获取经验的圣物数量
		FetchItem add_exp_list[XIANNV_SHENGWU_MAX_ID + 1];	// 获取经验的圣物列表
	};
}

#pragma pack(pop)

#endif

