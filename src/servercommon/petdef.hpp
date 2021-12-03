#ifndef __PETDEF_H__
#define __PETDEF_H__

#include "servercommon.h"
#include "gameworld/gameworld/item/itembase.h"

static const int PET_MAX_COUNT_LIMIT = 12;										// ���������������
static const int PET_MAX_STORE_COUNT = 48;										// ����齱�����������
static const int PET_MAX_LEVEL_LIMIT = 100;										// �������ȼ�����
static const int PET_MAX_GRADE_LIMIT = 15;										// ��������������
static const int PET_EGG_MAX_COUNT_LIMIT = 60;									// ���ﵰ���������
static const int PET_REWARD_CFG_COUNT_LIMIT = 100;								// ���ｱƷ���������������
static const int PET_MAX_EXTRA_ATTR_CFG_COUNT_LIMIT = 10;						// �����������������������
static const int PET_MAX_EXTRA_ATTR_PET_COUNT_LIMIT = 3;						// �������������װ��������
static const int PET_SKILL_CFG_MAX_COUNT_LIMIT = 12;							// ���＼������������
static const int PET_SPECIAL_IMG_MAX_ID_LIMIT = 10;                          	// ����û�����������
static const int PET_QINMI_LEVEL_MAX_LEVEL = 20;								// �����������ȼ�
static const int PET_QINMI_LEVEL_MAX_STAR = 5;									// �������ܵȼ��������
static const int PET_FOOD_MARKET_MAX_REWARD_COUNT = 100;						// ����Ի��г��������
static const int PET_FOOD_MARKET_MAX_FOOD_COUNT = 3;							// ����Ի��г����ʳ�ｱ��

static const short INVALID_PET_ID = -1;											// ��Ч�ĳ���ID
static const short INVALID_PET_SPECIAL_IMG = 0;									// ��Ч�û�����
static const short MAX_SPECIAL_IMG_COUNT = 32;									// �û�������
static const short MAX_SPECIAL_IMG_LEVEL = 50;									// �û����ȼ�

enum PET_SKILL_SLOT_TYPE
{
	PET_SKILL_SLOT_TYPE_ACTIVE = 0,							// �������ܲ�
	PET_SKILL_SLOT_TYPE_PASSIVE_1,							// �������ܲ�1
	PET_SKILL_SLOT_TYPE_PASSIVE_2,							// �������ܲ�2

	PET_SKILL_SLOT_TYPE_COUNT								// ���ܲ�������
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

	short level;															// �ȼ�
	short grade;															// ����
	short bless;															// ף��ֵ
	short reserve_sh1;
	GameName name;															// ��������
	PetSkillItem skill_list[PET_SKILL_SLOT_TYPE_COUNT];						// Я���ļ���
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

	unsigned int last_free_chou_timestamp;								// ��һ����ѳ�����ʱ���

	PetStoreItem backpack_item_list[PET_MAX_STORE_COUNT];

	int cur_on_use_pet_id;
	PetParamItem own_pet_list[PET_MAX_COUNT_LIMIT];

	int special_img_active_flag;

	short use_img_id;
	char food_market_free_times;
	char has_played_food_market;	// �Ƿ�����Ի��г�

	PetQinmiParamItem pet_qinmi_list[PET_MAX_COUNT_LIMIT];

	UNSTD_STATIC_CHECK(32 == MAX_SPECIAL_IMG_COUNT);
	char special_img_level_list[MAX_SPECIAL_IMG_COUNT];
};

typedef char PetParamHex[sizeof(PetParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(PetParamHex) < 4096);

#pragma pack(pop)

#endif

