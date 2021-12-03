/******************************************************************************************
FileName: campdef.hpp
Author: dreamwang
Description: 阵营相关定义 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/08/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __CAMP_DEF_HPP__
#define __CAMP_DEF_HPP__

#include "servercommon/struct/itemlistparam.h"

const int CAMP_SAVE_INTERVAL = 60;					// 阵营回档时间 

// 阵营定义 
enum CAMP_TYPE
{
	CAMP_TYPE_INVALID = 0,
	CAMP_TYPE_FEIXING,									// 轩辕
	CAMP_TYPE_ZHURI,									// 神农
	CAMP_TYPE_ZHUIYUE,									// 蚩尤
	CAMP_TYPE_MAX,
};

struct CampSyncInfo
{
	CampSyncInfo() : role_count(0), rich_level(0), earnings_end_time1(0), earnings1(0), earnings_end_time2(0), earnings2(0)
	{
	}

	int role_count;
	int rich_level;
	unsigned int earnings_end_time1;
	int earnings1;
	unsigned int earnings_end_time2;
	int earnings2;
};

//------------------------------------------------------
// 师门装备
//------------------------------------------------------

enum CampEquipIndex
{
	E_INDEX_CAMP_TOUKUI = 0,									// 师门头盔
	E_INDEX_CAMP_YIFU,											// 师门衣服
	E_INDEX_CAMP_HUTUI,											// 师门护腿
	E_INDEX_CAMP_XIEZI,											// 师门鞋子
	E_INDEX_CAMP_HUSHOU,										// 师门护手
	E_INDEX_CAMP_XIANGLIAN,										// 师门项链
	E_INDEX_CAMP_WUQI,											// 师门武器
	E_INDEX_CAMP_JIEZHI,										// 师门戒指

	E_INDEX_CAMP_MAX,
};

static const int MAX_CAMP_EQUIP_INDEX = 8;	// 师门装备最大索引
UNSTD_STATIC_CHECK(MAX_CAMP_EQUIP_INDEX == E_INDEX_CAMP_MAX);

struct CampEquipParam
{
	CampEquipParam()
	{
		this->Reset();
	}

	void Reset()
	{
		for (int i = 0; i < MAX_CAMP_EQUIP_INDEX; ++ i)
		{
			equip_list[i].Reset();
		}

		memset(hunlian_level_list, 0, sizeof(hunlian_level_list));
		beast_level = 0;
		reserver_sh = 0;
		beast_exp = 0;
	}

	ItemDataWrapper equip_list[MAX_CAMP_EQUIP_INDEX];
	short hunlian_level_list[MAX_CAMP_EQUIP_INDEX];

	short beast_level;
	short reserver_sh;
	int beast_exp;
};

typedef char CampEquipParamHex[sizeof(CampEquipParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CampEquipParamHex) < 2048);


//------------------------------------------------------
// 师门功勋
//------------------------------------------------------
static const int HONOR_REWARD_ITEM_NUM = 3;						// 奖励物品个数

static const int GONG_XUN_RANDK_NUM = 10;							// 排行榜奖励配置个数

static const int GONG_XUN_VALUE_NUM = 15;							// 功勋值奖励配置个数


//------------------------------------------------------
// 师门排行
//------------------------------------------------------
static const int MAX_CAMP_NUM = 3;					// 师门数量
UNSTD_STATIC_CHECK(MAX_CAMP_NUM == CAMP_TYPE_MAX - 1);

static const int MAX_CAMP_TOPRANK3 = 3;				// 各个师门排行榜前三
static const int MAX_CAMP_RECORD_RANK3_NUM = 9;		// 前3名，3个师门
UNSTD_STATIC_CHECK(MAX_CAMP_RECORD_RANK3_NUM == MAX_CAMP_NUM * MAX_CAMP_TOPRANK3);

enum DIAO_XIANG_STATUS
{
	DIAO_XIANG_STATUS_DESTORY = 0,
	DIAO_XIANG_STATUS_EXIST,
};

//------------------------------------------------------
// 魂炼相关
//------------------------------------------------------
UNSTD_STATIC_CHECK((MAX_CAMP_EQUIP_INDEX % 2) == 0);
static const int MAX_HUNLIAN_LEVEL = 50;								// 魂炼最大等级

#endif // __CAMP_DEF_HPP__
