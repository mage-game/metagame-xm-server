#ifndef __MSG_SHENZHOU_WEAPON_H__
#define __MSG_SHENZHOU_WEAPON_H__

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/shenzhouweapondef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum SHENZHOU_REQ_TYPE
	{
		SHENZHOU_REQ_TYPE_ALL_INFO_REQ = 0,									// 请求所有信息
		SHENZHOU_REQ_TYPE_BUY_GATHER_TIME,									// 购买采集次数
		SHENZHOU_REQ_TYPE_EXCHANGE_IDENTIFY_EXP,							// 兑换鉴定经验
		SHENZHOU_REQ_TYPE_INDENTIFY,										// 鉴定物品 param1 背包物品下标, param2 鉴定数量
		SHENZHOU_REQ_TYPE_UPLEVEL_WEAPON_SLOT ,								// 提升魂器部件等级， param1 魂器类型，param2 魂器部位
		SHENZHOU_REQ_TYPE_GATHER_INFO_REQ,									// 请求采集信息
		SHENZHOU_REQ_TYPE_HELP_OTHER_BOX,									// 协助别人的宝箱
		SHENZHOU_REQ_TYPE_OPEN_BOX,											// 打开宝箱
		SHENZHOU_REQ_TYPE_BOX_INFO,											// 请求宝箱信息
		SHENZHOU_REQ_TYPE_PUT_BOX,											// 放入宝箱
		SHENZHOU_REQ_TYPE_UPLEVEL_ELEMENT ,									// 提升元素等级， param1 魂器类型，param2 元素类型
		SHENZHOU_REQ_TYPE_UPLEVEL_LINGSHU,									// 提升灵枢等级， param1 魂器类型， param2 魂印槽
		SHENZHOU_REQ_TYPE_HUNYIN_INLAY,										// 镶嵌魂印， param1 魂器类型， param2 魂印槽， param3背包索引
		SHENZHOU_REQ_TYPE_INVITE_HELP_OTHER_BOX,							// 邀请协助宝箱
		SHENZHOU_REQ_TYPE_REMOVE_HELP_BOX,									// 清除协助
		SHENZHOU_REQ_TYPE_XILIAN_OPEN_SLOT,									// 开启洗练槽，param1 魂器类型， param2 属性槽
		SHENZHOU_REQ_TYPE_XILIAN_REQ,										// 请求洗练，param1 魂器类型， param2锁定槽0-7位表示1-8位属性, param3洗练材料类型, param4 是否自动购买, param5 是否免费洗炼
		SHENZHOU_REQ_TYPE_OPEN_HUNYIN_SLOT,									// 开启魂印槽，param1 魂器类型， param2 魂印槽
		SHENZHOU_REQ_TYPE_AUTO_UPLEVEL_LINGSHU,								// 一键提升灵枢等级， param1 魂器类型
	};

	class CSSHenzhouWeaponOperaReq											// 神州六器操作请求  5684
	{
	public:
		CSSHenzhouWeaponOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
		int param_4;
		int param_5;
	};

	class SCShenzhouWeapondAllInfo											// 神州六器所有信息 2407
	{
	public:
		SCShenzhouWeapondAllInfo();
		MessageHeader header;

		char today_gather_times;											// 今日采集总数
		char today_buy_gather_times;										// 今日购买采集总次数
		short today_exchange_identify_exp_times;							// 今日兑换鉴定经验次数
		short identify_level;												// 鉴定等级
		short identify_star_level;											// 鉴定星级
		int identify_exp;													// 鉴定经验
		int hunqi_jinghua;													// 魂器精华
		int lingshu_exp;													// 灵枢经验
		int day_free_xilian_times;											// 今日已免费洗练次数

		int hunqi_count;													// 魂器数量
		ShenzhouWeaponParam::WeaponData weapon_data[SHENZHOU_WEAPON_COUNT]; // 魂器等级
		ShenzhouWeaponParam::XilianData xilian_data[SHENZHOU_WEAPON_COUNT]; // 洗练数据
	};

	class SCShenzhouWeapondGatherInfo										// 神州六器采集信息 2408
	{
	public:
		SCShenzhouWeapondGatherInfo();
		MessageHeader header;

		char today_gather_times;											// 今日采集总数
		char today_buy_gather_times;										// 今日购买采集总次数
		short scene_leave_num;												// 场景剩余数量		
		int normal_item_num;												// 普通物品数量
		int rare_item_num;													// 稀有物品数量
		int unique_item_num;												// 绝世物品数量
		unsigned int next_refresh_time;										// 下次刷新时间
	};

	class SCShenzhouBoxInfo													// 神州六器宝箱信息 5692
	{
	public:
		SCShenzhouBoxInfo();
		MessageHeader header;

		int box_id;															// 宝箱id
		int today_open_box_free_num;										// 今天免费开宝箱次数
		int last_free_ggl_time;												// 最后免费开宝箱的时间
		int today_help_box_num;												// 每天协助别人宝箱的次数
		int box_help_uid_list[SHENZHOU_WEAPON_BOX_HELP_MAX_CONUT];			// 协助角色的uid
	};

	class CSSHenzhouWeaponOneKeyIdentifyReq									// 5693一键鉴定
	{
	public:
		CSSHenzhouWeaponOneKeyIdentifyReq();
		MessageHeader header;
	};

	class CSShenzhouHunyinResolveReq										// 5694魂印分解
	{
	public:
		CSShenzhouHunyinResolveReq();
		MessageHeader header;

		static const int HUNYIN_RESOLVE_MAX_INDEX_NUM = 200;

		int index_count;
		short index_in_bag_list[HUNYIN_RESOLVE_MAX_INDEX_NUM];
	};
}

#pragma pack(pop)

#endif

