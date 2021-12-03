#ifndef _MYSTERIOUSSHOP_HPP_
#define _MYSTERIOUSSHOP_HPP_

#include "serverdef.h"
#include "servercommon/servercommon.h"

static const int MAX_MYSTERIOUS_GRID_NUM = 12;

#pragma pack(push, 4)

struct MysteriousShopGrid
{
	MysteriousShopGrid()  
	{
		this->Reset();
	}

	bool Invalid() { return 0 == item_id; }
	
	void Reset() 
	{
		item_id = 0;
		is_bind = 0;
		reserve1 = 0;
		reserve2 = 0;
		num = 0;
	}

	ItemID item_id;							// ��ƷID
	char is_bind;							// �Ƿ��
	char reserve1;			
	short num;								// ��Ʒ����
	short reserve2;
};

struct MysteriousShopGridList
{
	void Reset() 
	{
		bless_value = 0;
		lucky_value = 0;
		memset(mysterious_grid_list, 0, sizeof(mysterious_grid_list));
	}

	int bless_value;													// ף��ֵ
	int lucky_value;													// ����ֵ
	MysteriousShopGrid mysterious_grid_list[MAX_MYSTERIOUS_GRID_NUM];	// �����б�
};

typedef char MysteriousGridListHex[sizeof(MysteriousShopGridList) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MysteriousGridListHex) < 256)

#pragma pack(pop)

#endif
