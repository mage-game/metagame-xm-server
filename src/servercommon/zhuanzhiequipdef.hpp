#ifndef _ZHUANZHI_EQUIP_DEF_HPP_
#define _ZHUANZHI_EQUIP_DEF_HPP_

#include "servercommon.h"
#include "servercommon/struct/itemlistparam.h"

#pragma pack(push, ALIGN_VALUE)

static const int MAX_ZHUANZHI_FULING_ATTR_COUNT = 4;			// ���������������

static const int MAX_ZHUANZHI_STONE_SLOT = 6;					// ÿ����λ����λ
static const int MAX_ZHUANZHI_STONE_LEVEL = 11;					// ��ʯ���ȼ�
static const int MAX_REFINE_STUFF_COUNT = 3;					// ��ʯ���������������

static const int SUIT_ROLE_HAS_MAX_SUIT_NUM = 4;				// ��ɫ�����е�������װ

static const int EQUIP_ZHIZUN_COMPOSE_CONSUME_NUM = 2;			//����װ���ϳ����ĵ�װ����
static const int MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT = 3;		//ÿ��תְװ���ľ�����

static const int MAX_ZHUANZHI_EQUIP_COMPOSE_COUNT = 5;			//תְװ���ϳ�������װ���������

typedef unsigned short wake_type_t;
typedef unsigned short wake_level_t;
typedef unsigned int wake_weight_t;
typedef int wake_key_t;

//װ��-------------------------------------------------------------------------------------------

enum E_INDEX_ZHUANZHI
{
	E_INDEX_WUQI = 0,											// ����
	E_INDEX_YIFU,												// ����
	E_INDEX_HUSHOU,												// ����
	E_INDEX_YAODAI,												// ����
	E_INDEX_TOUKUI,												// ͷ��
	E_INDEX_XIANGLIAN,											// ����
	E_INDEX_SHOUZHUO,											// ����
	E_INDEX_JIEZHI,												// ��ָ
	E_INDEX_XIEZI,												// Ь��
	E_INDEX_YUPEI,												// ����

	E_INDEX_MAX,
};

//���㴢���������Ҫ���ֽ�������4����
static const int WAKE_BYTE = BIT_COUNT_OF_BYTE * ALIGN_VALUE;
static const int MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT = MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT * E_INDEX_MAX;
static const int ALIGN_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT = (MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT % WAKE_BYTE == 0) ? MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT : MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT + (WAKE_BYTE - MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT % WAKE_BYTE);
static const int MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE = ALIGN_ZHUANZHI_EQUIP_AWAKENING_LOCK_COUNT / BIT_COUNT_OF_BYTE;

enum EQUIP_SPECIAL_EFFECT_TYPE
{
	EQUIP_SPECIAL_EFFECT_TYPE_INVALID = 0,
	EQUIP_SPECIAL_EFFECT_TYPE_1,						// ���ӹ�������p1��ֱ�
	EQUIP_SPECIAL_EFFECT_TYPE_2,						// �����ܵ��˺���p1��ֱ�

	EQUIP_SPECIAL_EFFECT_TYPE_MAX,
};

struct ZhuanzhiEquipItem
{
	ZhuanzhiEquipItem() { this->Reset(); }

	void Reset()
	{
		star_level = 0;
		reserve_sh = 0;
		star_exp = 0;
		zhuanzhi_equip_data.Reset();
		memset(fuling_count_list, 0, sizeof(fuling_count_list));
	}

	UInt16 star_level;											// ���ǵȼ�
	Int16 reserve_sh;
	UInt32 star_exp;											// ���Ǿ���
	ItemDataWrapper zhuanzhi_equip_data;						// װ������
	UInt16 fuling_count_list[MAX_ZHUANZHI_FULING_ATTR_COUNT];	// �����������
};

//��ʯ-------------------------------------------------------------------------------------------

enum ZHUANZHI_STONE_TYPE
{
	ZHUANZHI_STONE_TYPE_RED = 0,								// ����ʯ  
	ZHUANZHI_STONE_TYPE_BLUE,									// ����ʯ

	ZHUANZHI_STONE_TYPE_MAX,
};

enum ZHUANZHI_STONE_SLOT_OPEN_TYPE
{
	ZHUANZHI_STONE_SLOT_OPEN_TYPE_ORDER = 0,							// װ������ 
	ZHUANZHI_STONE_SLOT_OPEN_TYPE_VIP,									// vip�ȼ�

	ZHUANZHI_STONE_SLOT_OPEN_TYPE_MAX,
};

struct ZhuanzhiStoneItem
{
	ZhuanzhiStoneItem() : stone_id(0), is_bind(0), reserve_ch(0), reserve1(0), reserve2(0) {}

	ItemID stone_id;
	UInt8 is_bind;
	UInt8 reserve_ch;
	Int16 reserve1;
	Int16 reserve2;

	void Reset()
	{
		stone_id = 0;
		is_bind = 0;
		reserve_ch = 0;
		reserve1 = 0;
		reserve2 = 0;
	}

	bool Invalid()
	{
		return stone_id <= 0;
	}
};

struct ZhuanzhiStonesPartData
{
	ZhuanzhiStonesPartData() { this->Reset(); }

	void Reset()
	{
		slot_open_flag = 0;
		reserve_ch = 0;
		refine_level = 0;
		refine_val = 0;

		for (int i = 0; i < MAX_ZHUANZHI_STONE_SLOT; i ++)
		{
			slot_list[i].Reset();
		}
	}

	UInt8 slot_open_flag;									// ���ױ��
	UInt8 reserve_ch;
	UInt16 refine_level;									// �����ȼ�
	UInt32 refine_val;										// ����ֵ

	ZhuanzhiStoneItem slot_list[MAX_ZHUANZHI_STONE_SLOT];	// ÿ�����ֱ�ʯ��Ϣ
};

//��װ-------------------------------------------------------------------------------------------

enum ZHUANZHI_SUIT_TYPE
{
	ZHUANZHI_SUIT_TYPE_INVALID = -1,
	ZHUANZHI_SUIT_TYPE_ZHUXIAN = 0,	// ������װ
	ZHUANZHI_SUIT_TYPE_ZHUSHEN,		// ������װ

	ZHUANZHI_SUIT_TYPE_MAX,
};

enum ZHUANZHI_SUIT_GROUP_TYPE
{
	ZHUANZHI_SUIT_GROUP_TYPE_FANG = 0,	// �������
	ZHUANZHI_SUIT_GROUP_TYPE_GONG,		// �������
	ZHUANZHI_SUIT_GROUP_TYPE_ZHIZUN,	// �������

	ZHUANZHI_SUIT_GROUP_TYPE_MAX,
};

struct OneSuitInfo
{
	OneSuitInfo() { this->Reset(); }

	struct stuff_info {
		ItemID item_id;
		short count;
	};
	void Reset()
	{
		memset(order_list, 0, sizeof(order_list));
		memset(stuff_bind_list, 0, sizeof(stuff_bind_list));
		memset(stuff_unbind_list, 0, sizeof(stuff_unbind_list));
		reserve_sh = 0;
	}

	UInt8 order_list[E_INDEX_MAX];	// װ����λ�Ľ���
	UInt16 reserve_sh;
	stuff_info stuff_bind_list[E_INDEX_MAX][ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_info stuff_unbind_list[E_INDEX_MAX][ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
};

//תְװ������
struct ZhuanzhiEquipAwakening
{
	ZhuanzhiEquipAwakening()
	{
		//this->Reset();	//���ǽṹ��,���첻��ҪReset
	}	

	void Reset()
	{
		memset(awakening_in_equip, 0, sizeof(awakening_in_equip));
		memset(awakening_in_displacement, 0, sizeof(awakening_in_displacement));
	}
	struct Awakening
	{
		Awakening() { this->Reset(); }

		void Reset()
		{
			type = 0;
			level = 0;
		}
		wake_type_t type;
		wake_level_t level;
	};
	Awakening awakening_in_equip[MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT];				//װ��λ
	Awakening awakening_in_displacement[MAX_ZHUANZHI_EQUIP_AWAKENING_COUNT];		//�滻λ
};

//����-------------------------------------------------------------------------------------------
enum ZHIZUNEQUIP_PART_TYPE
{
	ZHIZUNEQUIP_PART_TYPE_0,
	ZHIZUNEQUIP_PART_TYPE_1,

	ZHIZUNEQUIP_PART_TYPE_MAX,
};

struct ZhuanzhiEquipParam
{
	ZhuanzhiEquipParam() { this->Reset(); }

	void Reset()
	{

		capability = 0;
		stone_score = 0;

		for (int i = 0; i < E_INDEX_MAX; i++)
		{
			zhuanzhi_equip_list[i].Reset();
			stone_list[i].Reset();
			zhuanzhi_equip_awakening_list[i].Reset();
		}

		for (int i = 0; i < SUIT_ROLE_HAS_MAX_SUIT_NUM; i ++)
		{
			zhuanzhi_suit_order_list[i].Reset();
		}

		memset(zhuanzhi_equip_awakening_lock_flag, 0, sizeof(zhuanzhi_equip_awakening_lock_flag));
	}

	UInt32 capability;															// ս��ֵ
	UInt32 stone_score;															// ��ʯ����

	ZhuanzhiEquipItem zhuanzhi_equip_list[E_INDEX_MAX];							// װ������
	ZhuanzhiStonesPartData stone_list[E_INDEX_MAX]; 							// ��ʯ����
	OneSuitInfo zhuanzhi_suit_order_list[SUIT_ROLE_HAS_MAX_SUIT_NUM];			// ��װ
	ZhuanzhiEquipAwakening zhuanzhi_equip_awakening_list[E_INDEX_MAX];			// ����
	unsigned char zhuanzhi_equip_awakening_lock_flag[MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE];	//������־λ
};

typedef char ZhuanzhiEquipParamHex[sizeof(ZhuanzhiEquipParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ZhuanzhiEquipParamHex) < 8196);

#pragma pack(pop)

#endif

