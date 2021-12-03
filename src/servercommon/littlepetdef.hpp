#ifndef __LETTLE_PET_DEF_HPP__
#define __LETTLE_PET_DEF_HPP__

#include "servercommon.h"

static const int LITTLEPET_QIANGHUAGRID_MAX_NUM = 5;										// 强化格子最大数量
static const int LITTLEPET_QIANGHUAPOINT_MAX_NUM = 10;										// 强化点最大数量
static const int LITTLEPET_QIANGHUAPOINT_CURRENT_NUM = 8;									// 当前强化点数量
UNSTD_STATIC_CHECK(LITTLEPET_QIANGHUAPOINT_CURRENT_NUM <= LITTLEPET_QIANGHUAPOINT_MAX_NUM);

static const int LITTLE_PET_PERSON_MAX_NUM = 5;												// 个人小宠物最大数量
static const int LITTLE_PET_COUPLE_MAX_SHARE_NUM = LITTLE_PET_PERSON_MAX_NUM * 2;			// 夫妻共享宠物最大数量
static const int LITTLE_PET_PERSON_MAX_LOG_NUM = 10;										// 个人互动记录最大数
static const int LITTLE_PET_SHARE_MAX_LOG_NUM = LITTLE_PET_PERSON_MAX_LOG_NUM * 2;			// 双方互动记录最大数
static const int LITTLE_PET_MAX_CHOU_COUNT = 10;											// 抽奖次数最大值
static const int LITTLEPET_VALUE_TYPE_MAX_NUM = 7;											// 基础属性类型最大数量
static const int LITTLEPET_EQUIP_INDEX_MAX_NUM = 4;											// 小宠物装备下标数

#pragma pack(push, 4)

struct LittleStarItem
{
	LittleStarItem() { this->Reset(); }

	struct AttributeItem
	{
		AttributeItem() { this->Reset(); }

		void Reset()
		{
			qianghua_times = 0;
			arrt_type = -1;
			attr_value = 0;
		}

		short qianghua_times;
		short arrt_type;
		int attr_value;
	};

	void Reset()
	{
		for (int i = 0; i < LITTLEPET_QIANGHUAGRID_MAX_NUM; i++)
		{
			attr_list[i].Reset();
		}
		attr_reserve[0].Reset();
		attr_reserve[1].Reset();
	}

	AttributeItem attr_list[LITTLEPET_QIANGHUAGRID_MAX_NUM];
	AttributeItem attr_reserve[2];
};

struct LittlePetEquipParam
{
	void Reset()
	{
		equipment_id = 0;
		level = 0;
		is_bind = 0;
		reserve_ch = 0;
		reserve_sh = 0;
		memset(reserve_list, 0, sizeof(reserve_list));
	}

	LittlePetEquipParam()
	{
		this->Reset();
	}

	unsigned short equipment_id;
	short level;
	char is_bind;
	char reserve_ch;
	short reserve_sh;
	int reserve_list[3];
};

struct LittlePetParam
{
	LittlePetParam() { this->Reset(); }
	void Reset()
	{
		pet_id = 0;
		feed_degree = 0;
		interact_times = 0;
		baoshi_active_time = 0;
		feed_level = 0;
		reserve_sh = 0;
		reserve_ll = 0;

		memset(pet_name, 0, sizeof(pet_name));

		for (int i = 0; i<  LITTLEPET_VALUE_TYPE_MAX_NUM; ++i)
		{
			base_attr[i] = 0;
		}

		for (int i = 0; i < LITTLEPET_QIANGHUAPOINT_MAX_NUM; ++i)
		{
			star_list[i].Reset();
		}

		for (int i = 0; i < LITTLEPET_EQUIP_INDEX_MAX_NUM; ++i)
		{
			equipment_list[i].Reset();
		}
	}

	short pet_id;
	char feed_degree;
	char interact_times;
	unsigned int baoshi_active_time;
	short feed_level;
	short reserve_sh;
	long long reserve_ll;

	GameName pet_name;
	int base_attr[LITTLEPET_VALUE_TYPE_MAX_NUM];
	LittleStarItem star_list[LITTLEPET_QIANGHUAPOINT_MAX_NUM];
	LittlePetEquipParam equipment_list[LITTLEPET_EQUIP_INDEX_MAX_NUM];
};

struct LittlePetInteractLog
{
	LittlePetInteractLog() { this->Reset(); }
	void Reset()
	{
		reserve_sh = 0;
		pet_id = 0;
		uid = 0;
		timestamp = 0;
		memset(pet_name, 0, sizeof(pet_name));
	}

	short reserve_sh;
	short pet_id;
	int uid;
	unsigned int timestamp;
	GameName pet_name;
};

struct LittlePetData
{
	LittlePetData() { this->Reset(); }

	void Reset()
	{
		lover_id = -1;
		score = 0;
		last_free_chou_timestamp = 0;
		interact_times = 0;
		using_index = -1;
		is_first_choujiang = 0;
		
		reserve_sh = 0;

		for (int index = 0; index < LITTLE_PET_PERSON_MAX_LOG_NUM; ++index)
		{
			log_list[index].Reset();
		}

		for (int index = 0; index < LITTLE_PET_PERSON_MAX_NUM; ++index)
		{
			pet_list[index].Reset();
		}
	}

	int lover_id;
	int score;																				//积分
	unsigned int last_free_chou_timestamp;													//免费抽奖时间戳
	short interact_times;																	//玩家互动次数	
	short using_index;																		//当前使用的形象
	short is_first_choujiang;																//是否第一次抽奖
	short reserve_sh;

	LittlePetInteractLog log_list[LITTLE_PET_PERSON_MAX_LOG_NUM];							// 记录
	LittlePetParam pet_list[LITTLE_PET_PERSON_MAX_NUM];
};

typedef char LittlePetDataParamHex[sizeof(LittlePetData) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(LittlePetDataParamHex) < 10240);

#pragma pack(pop)

#endif // __LITTLEPET_DEF_HPP__




