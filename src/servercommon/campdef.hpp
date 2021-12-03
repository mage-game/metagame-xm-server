/******************************************************************************************
FileName: campdef.hpp
Author: dreamwang
Description: ��Ӫ��ض��� 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/08/ 09:04:05     1.0           ����
*****************************************************************************************/

#ifndef __CAMP_DEF_HPP__
#define __CAMP_DEF_HPP__

#include "servercommon/struct/itemlistparam.h"

const int CAMP_SAVE_INTERVAL = 60;					// ��Ӫ�ص�ʱ�� 

// ��Ӫ���� 
enum CAMP_TYPE
{
	CAMP_TYPE_INVALID = 0,
	CAMP_TYPE_FEIXING,									// ��ԯ
	CAMP_TYPE_ZHURI,									// ��ũ
	CAMP_TYPE_ZHUIYUE,									// ���
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
// ʦ��װ��
//------------------------------------------------------

enum CampEquipIndex
{
	E_INDEX_CAMP_TOUKUI = 0,									// ʦ��ͷ��
	E_INDEX_CAMP_YIFU,											// ʦ���·�
	E_INDEX_CAMP_HUTUI,											// ʦ�Ż���
	E_INDEX_CAMP_XIEZI,											// ʦ��Ь��
	E_INDEX_CAMP_HUSHOU,										// ʦ�Ż���
	E_INDEX_CAMP_XIANGLIAN,										// ʦ������
	E_INDEX_CAMP_WUQI,											// ʦ������
	E_INDEX_CAMP_JIEZHI,										// ʦ�Ž�ָ

	E_INDEX_CAMP_MAX,
};

static const int MAX_CAMP_EQUIP_INDEX = 8;	// ʦ��װ���������
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
// ʦ�Ź�ѫ
//------------------------------------------------------
static const int HONOR_REWARD_ITEM_NUM = 3;						// ������Ʒ����

static const int GONG_XUN_RANDK_NUM = 10;							// ���а������ø���

static const int GONG_XUN_VALUE_NUM = 15;							// ��ѫֵ�������ø���


//------------------------------------------------------
// ʦ������
//------------------------------------------------------
static const int MAX_CAMP_NUM = 3;					// ʦ������
UNSTD_STATIC_CHECK(MAX_CAMP_NUM == CAMP_TYPE_MAX - 1);

static const int MAX_CAMP_TOPRANK3 = 3;				// ����ʦ�����а�ǰ��
static const int MAX_CAMP_RECORD_RANK3_NUM = 9;		// ǰ3����3��ʦ��
UNSTD_STATIC_CHECK(MAX_CAMP_RECORD_RANK3_NUM == MAX_CAMP_NUM * MAX_CAMP_TOPRANK3);

enum DIAO_XIANG_STATUS
{
	DIAO_XIANG_STATUS_DESTORY = 0,
	DIAO_XIANG_STATUS_EXIST,
};

//------------------------------------------------------
// �������
//------------------------------------------------------
UNSTD_STATIC_CHECK((MAX_CAMP_EQUIP_INDEX % 2) == 0);
static const int MAX_HUNLIAN_LEVEL = 50;								// �������ȼ�

#endif // __CAMP_DEF_HPP__
