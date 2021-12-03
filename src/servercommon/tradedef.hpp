/******************************************************************************************
FileName: tradedef.hpp
Author: dreamwang
Description: �����г���ض���   
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/09/ 09:04:05     1.0           ����
*****************************************************************************************/

#ifndef __TRADE_DEF_HPP__
#define __TRADE_DEF_HPP__

#include <stdio.h>
#include "servercommon/struct/itemlistparam.h"

/////////////////////////////////////////////////     ����     ////////////////////////////////////////////////

static const int PUBLICSALE_MAX_ITEM_COUNT = 20;			// ÿ�����������������Ʒ����
static const int PUBLICSALE_WRITE_DB_INTERVAL = 60;			// ��ʱ��дDB��ʱ��
static const int PUBLICSALE_CHECK_DUE_INTERVAL = 60;		// ��������Ʒ�ļ��
static const int PUBLICSALE_LEVEL_LIMIT = 40;				// �����ȼ�����
static const int PUBLICSALE_TO_WORLD_COST = 2000;			// ����֪ͨ������Ƶ������

static const int P2P_TRADE_MAX_ITEM_COUNT = 8;				// ���˽��������Ʒ��

static const int PUBLICSALE_COIN_SALE_TYPE = 1300;			// ͭ�ҵ���������

// ����ϵͳ����״̬ 
enum PUBLICSALE_DATA_STATE
{
	PUBLICSALE_DATA_STATE_INVALID = 0,						// ��ʼ״̬
	PUBLICSALE_DATA_STATE_LOADING,							// ���ݼ����� 
	PUBLICSALE_DATA_STATE_LOAD_FINISH,						// ���ݼ������ 
	PUBLICSALE_DATA_STATE_MAX,
};

// �����۸�����
enum PUBLIC_SALE_PRICE_TYPE
{
	PUBLIC_SALE_PRICE_TYPE_INVALID = 0,
	PUBLIC_SALE_PRICE_TYPE_COIN,
	PUBLIC_SALE_PRICE_TYPE_GOLD,

	PUBLIC_SALE_PRICE_TYPE_MAX,
};

// ������Ʒ����
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

//////////////////////////    ���˽���    ////////////////////////////////////////////////

enum TRADE_STATE
{
	TS_NONE = 0,											// ����״̬
	TS_TRADING,												// ������
	TS_LOCK,												// ����
	TS_AFFIRM,												// ȷ��
	TS_AFFIRM_SUCC,											// ȷ�ϳɹ� �������
	TS_CANCEL,												// ����ȡ��
};

#endif  // __TRADE_DEF_HPP__
