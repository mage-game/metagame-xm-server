#ifndef __MSG_SHENGE_SYSTEM_HPP__
#define __MSG_SHENGE_SYSTEM_HPP__

#include "servercommon/shengesystemdef.hpp"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push, 4)

namespace Protocol
{

	enum SHENGE_SYSTEM_REQ_TYPE
	{
		SHENGE_SYSTEM_REQ_TYPE_ALL_INFO,					// 请求所有信息
		SHENGE_SYSTEM_REQ_TYPE_DECOMPOSE,					// 分解			p1 虚拟背包索引
		SHENGE_SYSTEM_REQ_TYPE_COMPOSE,						// 合成			p1 物品1的虚拟背包索引	p2 物品2的虚拟背包索引	p3 物品1的虚拟背包索引；
		SHENGE_SYSTEM_REQ_TYPE_SET_RUAN,					// 装备神格		p1 虚拟背包索引			p2 神格页				p3 格子索引；
		SHENGE_SYSTEM_REQ_TYPE_CHANGE_RUNA_PAGE,			// 切换神格页	p1 神格页
		SHENGE_SYSTEM_REQ_TYPE_CHOUJIANG,					// 抽奖			p1 次数；
		SHENGE_SYSTEM_REQ_TYPE_UPLEVEL,						// 升级 神格	p1 0 背包 1 神格页		p2 背包索引/ 神格页		p3 格子索引
		SHENGE_SYSTEM_REQ_TYPE_SORT_BAG,					// 整理背包
		SHENGE_SYSTEM_REQ_TYPE_UNLOAD_SHENGE,				// 拆除神格		p1 神格页				p2 格子索引
		SHENGE_STYTEM_REQ_TYPE_CLEAR_PAGE,					// 清除神格页	p1 神格页

		SHENGE_STYTEM_REQ_TYPE_UPLEVEL_ZHANGKONG,			// 升级掌控		p1 0：升级1次，1：升级10次
		SHENGE_STYTEM_REQ_TYPE_RECLAC_ATTR,					// 升级掌控后重算战斗力

		SHENGE_SYSTEM_REQ_TYPE_XILIAN,						// 神格神躯洗炼 p1 神躯id				p2 洗炼点			p3 是否自动购买
	};

	class CSShengeSystemReq  // 8421
	{
	public:
		CSShengeSystemReq();
		MessageHeader header;

		short info_type;
		short param1;
		short param2;
		short param3;
		unsigned int param_4;

		int index_count;
		int virtual_index_list[SHENGE_SYSTEM_BAG_MAX_GRIDS];
	};

	enum SHENGE_SYSTEM_INFO_TYPE
	{
		SHENGE_SYSTEM_INFO_TYPE_SIGLE_CHANGE = 0,			// 背包单个神格信息											p2 count数量
		SHENGE_SYSTEM_INFO_TYPE_ALL_BAG_INFO,				// 背包全部信息				p1当前使用神格页				p2 count数量	
		SHENGE_SYSTEM_INFO_TYPE_SHENGE_INFO,				// 神格页单个的神格信息		p1神格页 						p2 count数量			
		SHENGE_SYSTEM_INFO_TYPE_ALL_SHENGE_INFO,			// 神格页的全部神格信息		p1神格页						p2 count数量			
		SHENGE_SYSTEM_INFO_TYPE_ALL_CHOUJIANG_INFO,			// 奖品列表					p1 剩余免费次数					p2 count数量		p3 免费抽奖剩余时间
		SHENGE_SYSTEM_INFO_TYPE_ALL_MARROW_SCORE_INFO,		// 精华信息																		p3 精华积分
		SHENGE_SYSTEM_INFO_TYPE_USING_PAGE_INDEX,			// 神格页					p1 当前使用神格页
		SHENGE_SYSTEM_INFO_TYPE_COMPOSE_SHENGE_INFO,		// 合成信息
		SHENGE_SYSTEM_INFO_TYPE_ACTIVE_COMBINE_INFO,		// 激活的组合索引
		SHENGE_SYSTEM_INFO_TYPE_CHOUJIANG_INFO,				// 抽奖信息					p1 已用免费抽奖次数									p3 下次抽奖cd
	};

	struct ShengeSystemBag
	{
		char quanlity;
		char type;
		unsigned char level;
		unsigned char index;
	};

	class SCShengeSystemBagInfo  // 8422
	{
	public:
		SCShengeSystemBagInfo();
		MessageHeader header;

		char info_type;
		char param1;
		short count;
		unsigned int param3;
		ShengeSystemBag bag_list[SHENGE_SYSTEM_BAG_MAX_GRIDS];
	};
	
	// 掌控总信息
	class SCShengeZhangkongInfo  // 8423
	{
	public:
		SCShengeZhangkongInfo();
		MessageHeader header;

		ZhangkongParam zhangkong_list[SHENGE_SYSTEM_QUALITY_MAX_VALUE];
	};

	// 单个掌控修改
	class SCZhangkongSingleChange	// 8424
	{
	public:
		SCZhangkongSingleChange();
		MessageHeader header;
		
		int grid;
		int level;
		int exp;
		int add_exp;
	};

	// 神格神躯信息
	class SCShengeShenquAllInfo		// 8440
	{
	public:
		SCShengeShenquAllInfo();
		MessageHeader header;

		SingleShengeShenqu shenqu_list[SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM];
		int shenqu_history_max_cap[SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM];
	};

	// 单个神格神躯信息
	class SCShengeShenquInfo		// 8441
	{
	public:
		SCShengeShenquInfo();
		MessageHeader header;

		int shenqu_id;
		ShengeShenquAttrInfo shenqu_attr_list[SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM];
		int shenqu_history_max_cap;
	};

	// 星辉套装信息
	class SCShengeSuitInfo		// 8849
	{
	public:
		SCShengeSuitInfo();
		MessageHeader header;

		int shenge_quality_info[SHENGE_QUALITY_COUNT];		//每种品质神格数量统计
	};
}

#pragma pack(pop)
#endif
