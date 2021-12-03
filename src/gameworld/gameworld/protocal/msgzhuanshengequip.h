#ifndef __MSG_ZHUANSHENGEQUIP_H__
#define __MSG_ZHUANSHENGEQUIP_H__

#include "servercommon/zhuanshengequipdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum ZHUANSHENG_REQ_TYPE
	{
		ZHUANSHENG_REQ_TYPE_ALL_INFO = 0,
		ZHUANSHENG_REQ_TYPE_BACKPACK_INFO = 1,
		ZHUANSHENG_REQ_TYPE_OTHER_INFO = 2,
		ZHUANSHENG_REQ_TYPE_UPLEVEL = 3,					// 升级请求
		ZHUANSHENG_REQ_TYPE_CHANGE_XIUWEI = 4,				// 兑换修为请求
		ZHUANSHENG_REQ_TYPE_PUT_ON_EQUIP = 5,				// 穿装备
		ZHUANSHENG_REQ_TYPE_TAKE_OFF_EQUIP = 6,				// 脱装备
		ZHUANSHENG_REQ_TYPE_COMBINE_EQUIP = 7,				// 合装备
		ZHUANSHENG_REQ_TYPE_CHOU_EQUIP = 8,					// 抽装备
		ZHUANSHENG_REQ_TYPE_TAKE_OUT_EQUIP = 9,				// 取出装备
		ZHUANSHENG_REQ_TYPE_AUTO_COMBINE_EQUIP = 10,		// 自动合装备
	};

	class CSZhuanShengOpearReq		// 6950 转生操作请求
	{
	public:
		CSZhuanShengOpearReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param1;
		int param2;
		int param3;
	};

	class SCZhuanShengAllInfo		// 6975 转生所有信息
	{
	public:
		SCZhuanShengAllInfo();
		MessageHeader header;

		ItemDataWrapper zhuansheng_equip_list[ZHUANSHENG_EQUIP_TYPE_MAX];

		unsigned int last_time_free_chou_timestamp;									// 上次免费抽时间戳
		int personal_xiuwei;														// 个人修为
		short zhuansheng_level;														// 当前转生等级
		short day_change_times;														// 当天兑换次数
	};

	class SCZhuanShengBackpackInfo // 6976 转生背包信息
	{
	public:
		SCZhuanShengBackpackInfo();
		MessageHeader header;

		ZhuanshenEuipGridItem backpack_equip_list[ZHUANSHENG_EQUIP_BACKPACK_MAX_COUNT];	// 背包里拥有的所有装备列表
	};

	enum ZHUANSHENG_NOTICE_REASON
	{
		ZHUANSHENG_NOTICE_NORMAL,
		ZHUANSHENG_NOTICE_XIUWEI_NOT_ENOUGH,
	};
	
	class SCZhuanShengOtherInfo  // 6977 转生其它信息
	{	
	public:
		SCZhuanShengOtherInfo();
		MessageHeader header;
		
		int notice_reason;
		unsigned int last_time_free_chou_timestamp;									// 上次免费抽时间戳
		int personal_xiuwei;														// 个人修为
		short zhuansheng_level;														// 当前转生等级
		short day_change_times;														// 当天兑换次数
	};

	class SCZhuanShengXiuweiNotice
	{
	public:
		SCZhuanShengXiuweiNotice();
		MessageHeader header;
		
		int notice_reason;
		int add_xiuwei;																// 修为
	};

	static const int SC_ZHUANSHENG_CHOU_RESULT_MAX = 10;

	class SCZhuanShengChouResult // 6978 转生抽奖结果
	{
	public:
		SCZhuanShengChouResult();
		MessageHeader header;

		int result_count;
		char result_list[SC_ZHUANSHENG_CHOU_RESULT_MAX];
	};

	class SCZhuanShengCombineResult  // 6979 转生合成结果
	{
	public:
		SCZhuanShengCombineResult();
		MessageHeader header;

		unsigned short new_equip_id;
		short is_auto_combine;
	};

}

#pragma pack(pop)

#endif //__MSG_ZHUANSHENGEQUIP_H__


