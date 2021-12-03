#ifndef __PETDEF_H__
#define __PETDEF_H__

#include "servercommon.h"
#include "gameworld/gameworld/item/itembase.h"

static const int PET_MAX_COUNT_LIMIT = 12;										// 宠物最大数量限制
static const int PET_MAX_STORE_COUNT = 48;										// 宠物抽奖背包最大数量
static const int PET_MAX_LEVEL_LIMIT = 100;										// 宠物最大等级限制
static const int PET_MAX_GRADE_LIMIT = 15;										// 宠物最大阶数限制
static const int PET_EGG_MAX_COUNT_LIMIT = 60;									// 宠物蛋最大数限制
static const int PET_REWARD_CFG_COUNT_LIMIT = 100;								// 宠物奖品配置最大数量限制
static const int PET_MAX_EXTRA_ATTR_CFG_COUNT_LIMIT = 10;						// 宠物额外属性配置数量限制
static const int PET_MAX_EXTRA_ATTR_PET_COUNT_LIMIT = 3;						// 宠物额外属性套装数量限制
static const int PET_SKILL_CFG_MAX_COUNT_LIMIT = 12;							// 宠物技能配置最大个数
static const int PET_SPECIAL_IMG_MAX_ID_LIMIT = 10;                          	// 宠物幻化配置最大个数
static const int PET_QINMI_LEVEL_MAX_LEVEL = 20;								// 宠物亲密最大等级
static const int PET_QINMI_LEVEL_MAX_STAR = 5;									// 宠物亲密等级最大星星
static const int PET_FOOD_MARKET_MAX_REWARD_COUNT = 100;						// 宠物吃货市场最大奖励数
static const int PET_FOOD_MARKET_MAX_FOOD_COUNT = 3;							// 宠物吃货市场最多食物奖励

static const short INVALID_PET_ID = -1;											// 无效的宠物ID
static const short INVALID_PET_SPECIAL_IMG = 0;									// 无效幻化形象
static const short MAX_SPECIAL_IMG_COUNT = 32;									// 幻化最大个数
static const short MAX_SPECIAL_IMG_LEVEL = 50;									// 幻化最大等级

enum PET_SKILL_SLOT_TYPE
{
	PET_SKILL_SLOT_TYPE_ACTIVE = 0,							// 主动技能槽
	PET_SKILL_SLOT_TYPE_PASSIVE_1,							// 被动技能槽1
	PET_SKILL_SLOT_TYPE_PASSIVE_2,							// 被动技能槽2

	PET_SKILL_SLOT_TYPE_COUNT								// 技能槽总数量
};

UNSTD_STATIC_CHECK((PET_SKILL_SLOT_TYPE_COUNT + 1) % 2 == 0);

#pragma pack(push, 4)


struct PetSkillItem
{
	PetSkillItem() { this->Reset(); }

	void Reset()
	{
		skill_index = 0;
		skill_level = 0;
	}

	char skill_index;
	char skill_level;
};

struct PetParamItem
{
	PetParamItem() { this->Reset(); }

	void Reset()
	{
		level = 0;
		grade = 0;
		bless = 0;
		reserve_sh1 = 0;
		memset(name, 0, sizeof(name));

		for (int slot = PET_SKILL_SLOT_TYPE_ACTIVE; slot < PET_SKILL_SLOT_TYPE_COUNT; ++ slot)
		{
			skill_list[slot].Reset();
		}

		reserve_sh2 = 0;
	}

	short level;															// 等级
	short grade;															// 阶数
	short bless;															// 祝福值
	short reserve_sh1;
	GameName name;															// 宠物名字
	PetSkillItem skill_list[PET_SKILL_SLOT_TYPE_COUNT];						// 携带的技能
	short reserve_sh2;
};

struct PetStoreItem
{
	PetStoreItem() { this->Reset(); }

	void Reset()
	{
		id = ItemBase::INVALID_ITEM_ID;
		is_bind = 1;
		num = 0;
	}

	ItemID id;
	short is_bind;
	int num;
};

struct PetQinmiParamItem
{
	PetQinmiParamItem() { this->Reset(); }

	void Reset()
	{
		qinmi_val = 0;
		level = 0;
		star = 1;

		reserve_sh = 0;
		reserve_ll = 0;
	}

	int qinmi_val;
	char level;
	char star;
	short reserve_sh;
	long long reserve_ll;
};

struct PetParam
{
	PetParam() { this->Reset(); }

	void Reset()
	{
		last_free_chou_timestamp = 0;

		for (int index = 0; index < PET_MAX_STORE_COUNT; ++ index)
		{
			backpack_item_list[index].Reset();
		}
		cur_on_use_pet_id = INVALID_PET_ID;

		for (int index = 0; index < PET_MAX_COUNT_LIMIT; ++ index)
		{
			own_pet_list[index].Reset();
		}

		special_img_active_flag = 0;
		use_img_id = 0;
		food_market_free_times = 0;
		has_played_food_market = 0;

		for (int i = 0; i < PET_MAX_COUNT_LIMIT; ++ i)
		{
			pet_qinmi_list[i].Reset();
		}
		
		memset(special_img_level_list, 0, sizeof(special_img_level_list));
	}

	unsigned int last_free_chou_timestamp;								// 上一次免费抽宠物的时间戳

	PetStoreItem backpack_item_list[PET_MAX_STORE_COUNT];

	int cur_on_use_pet_id;
	PetParamItem own_pet_list[PET_MAX_COUNT_LIMIT];

	int special_img_active_flag;

	short use_img_id;
	char food_market_free_times;
	char has_played_food_market;	// 是否玩过吃货市场

	PetQinmiParamItem pet_qinmi_list[PET_MAX_COUNT_LIMIT];

	UNSTD_STATIC_CHECK(32 == MAX_SPECIAL_IMG_COUNT);
	char special_img_level_list[MAX_SPECIAL_IMG_COUNT];
};

typedef char PetParamHex[sizeof(PetParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(PetParamHex) < 4096);

#pragma pack(pop)

#endif

