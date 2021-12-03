/******************************************************************************************
FileName: tradedef.hpp
Author: dreamwang
Description: 交易市场相关定义   
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/09/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __TRADE_DEF_HPP__
#define __TRADE_DEF_HPP__

#include <stdio.h>
#include "servercommon/struct/itemlistparam.h"

/////////////////////////////////////////////////     拍卖     ////////////////////////////////////////////////

static const int PUBLICSALE_MAX_ITEM_COUNT = 20;			// 每个人最多能拍卖的物品个数
static const int PUBLICSALE_WRITE_DB_INTERVAL = 60;			// 定时回写DB的时间
static const int PUBLICSALE_CHECK_DUE_INTERVAL = 60;		// 检查过期物品的间隔
static const int PUBLICSALE_LEVEL_LIMIT = 40;				// 拍卖等级限制
static const int PUBLICSALE_TO_WORLD_COST = 2000;			// 发送通知到世界频道费用

static const int P2P_TRADE_MAX_ITEM_COUNT = 8;				// 个人交易最多物品数

static const int PUBLICSALE_COIN_SALE_TYPE = 1300;			// 铜币的搜索类型

// 拍卖系统数据状态 
enum PUBLICSALE_DATA_STATE
{
	PUBLICSALE_DATA_STATE_INVALID = 0,						// 初始状态
	PUBLICSALE_DATA_STATE_LOADING,							// 数据加载中 
	PUBLICSALE_DATA_STATE_LOAD_FINISH,						// 数据加载完成 
	PUBLICSALE_DATA_STATE_MAX,
};

// 拍卖价格类型
enum PUBLIC_SALE_PRICE_TYPE
{
	PUBLIC_SALE_PRICE_TYPE_INVALID = 0,
	PUBLIC_SALE_PRICE_TYPE_COIN,
	PUBLIC_SALE_PRICE_TYPE_GOLD,

	PUBLIC_SALE_PRICE_TYPE_MAX,
};

// 出售物品类型
enum PUBLIC_SALE_ITEM_TYPE
{
	PUBLIC_SALE_ITEM_TYPE_INVALID = 0,

	PUBLIC_SALE_ITEM_TYPE_COIN,
	PUBLIC_SALE_ITEM_TYPE_ITEM,

	PUBLIC_SALE_ITEM_TYPE_MAX,
};


struct SaleItemParam
{
	SaleItemParam() : sale_index(0), sale_type(0), level(0), prof(0), color(0), order(0), 
					price(0), sale_value(0), price_type(0), sale_item_type(0), sale_time(0), due_time(0) {}

	ItemDataWrapper item_wrapper;
	int sale_index;
	int sale_type;
	short level;
	short prof;
	short color;
	short order;
	int price;
	int sale_value;
	short price_type;
	short sale_item_type;
	unsigned int sale_time;
	unsigned int due_time;
};

//////////////////////////    个人交易    ////////////////////////////////////////////////

enum TRADE_STATE
{
	TS_NONE = 0,											// 空闲状态
	TS_TRADING,												// 交易中
	TS_LOCK,												// 锁定
	TS_AFFIRM,												// 确认
	TS_AFFIRM_SUCC,											// 确认成功 交易完成
	TS_CANCEL,												// 交易取消
};

#endif  // __TRADE_DEF_HPP__
