#ifndef __CHINESE_ZODIAC_DEF_H__
#define __CHINESE_ZODIAC_DEF_H__

#include "servercommon.h"

static const int CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT = 12;				// ��������������������
static const int CHINESE_ZODIAC_LEVEL_MAX_LIMIT = 100;					// �������ǵȼ�����

static const int CHINESE_ZODIAC_EQUIP_SLOT_MAX_LIMIT = 8;				// ����װ������������  ���ܸģ����ݿ�
static const int CHINESE_ZODIAC_CUR_EQUIP_SLOT_MAX_LIMIT = 5;			// ��ǰ����װ��������
static const int CHINESE_ZODIAC_MAX_EQUIP_LEVEL = 100;					// װ����ߵȼ�
static const int CHINESE_ZODIAC_MAX_EQUIP_SUIT = 6;						// װ����װ�����

UNSTD_STATIC_CHECK(CHINESE_ZODIAC_CUR_EQUIP_SLOT_MAX_LIMIT <= CHINESE_ZODIAC_EQUIP_SLOT_MAX_LIMIT);

static const int CHINESE_ZODIAC_XINGHUN_LEVEL_MAX = 50;					// �����ǻ�ȼ����ֵ
static const int CHINESE_ZODIAC_XINGHUN_TITLE_COUNT_MAX = 50;			// �ǻ�ƺ��������

static const int TIAN_XIANG_TABEL_MIDDLE_ROW = 3;							// �м����һ�е�����
static const int TIAN_XIANG_TABEL_ROW_COUNT = 7;							// �� y �������
static const int TIAN_XIANG_TABEL_MIDDLE_GRIDS = 7;							// �� x �������
static const int TIAN_XIANG_ALL_CHAPTER_COMBINE = 3;						// ÿ���½ڵ������
static const int TIAN_XIANG_COMBINE_MEX_BEAD_NUM = 15;						// ÿ���������������
static const int TIAN_XIANG_CHAPTER_NUM = 10;								// �½������
static const int TIAN_XIANG_CURR_CHAPTER_NUM = 5;							// ��ǰ�½��������������

static const int GGL_COMBINE_TYPE_MAX_NUM = 10;								// ������һ�γ齱������
static const int GGL_COMBINE_TYPE_NUM = 7;									// �������������͸���
static const int GGL_COMBINE_ITEM_NUM = 3;									// ���������Ӹ���

static const int MIJI_KONG_NUM = 8;											// �����ؼ��Ŀ���
static const int MIJI_KONG_KILL_MAX_COUNT = 200;							// �����ؼ��������
static const int MIJI_KONG_MAX_LEVEL = 3;									// �������ȼ�

static const int XING_LING_MAX_LEVEL = 40;									// �������ȼ�

static const int MAX_CHINESEZODIAC_RECYCLE_NUM = 100;						// ���ηֽ���������

enum TIAN_XIANG_BEAD_TYPE
{
	TIAN_XIANG_BEAD_TYPE_INVALID = 0,	//��Ч
	TIAN_XIANG_BEAD_TYPE_FIRE,			//�����
	TIAN_XIANG_BEAD_TYPE_METALS,		//�����
	TIAN_XIANG_BEAD_TYPE_WATER,			//ˮ���
	TIAN_XIANG_BEAD_TYPE_WOOD,			//ľ���
	TIAN_XIANG_BEAD_TYPE_CHIJINLINGZHU,	//�������
	TIAN_XIANG_BEAD_TYPE_SIX,			//����6����չ��

	TIAN_XIANG_BEAD_TYPE_MAX,			//����������
	TIAN_XIANG_BEAD_TYPE_SAME = TIAN_XIANG_BEAD_TYPE_MAX,  //ͬ���������
	TIAN_XIANG_BEAD_TYPE_ALL,			// �����������
};

enum MIJI_ARRT_TYPE
{
	MIJI_ARRT_MAXHP = 0,				// ��Ѫ
	MIJI_ARRT_GONGJI,					// ����
	MIJI_ARRT_FANGYU,					// ����
	MIJI_ARRT_BAOJI,					// ����
	MIJI_ARRT_KANGBAO,					// ����
	MIJI_ARRT_MINGZHONG,				// ����
	MIJI_ARRT_SHANBI,					// ����
	MIJI_ARRT_NVSHENGONGJI,				// Ů�񹥻�
	MIJI_ARRT_GD_ZENGSHANG,				// �̶�����
	MIJI_ARRT_GD_MIANFANG,				// �̶�����
	
	MIJI_SPECIAL_TYPE_MIN,
	MIJI_ARRT_HUIXUE_PER = MIJI_SPECIAL_TYPE_MIN,	// ��Ѫ����X%
	MIJI_ARRT_JINENG_COOLCOWN_SUODUAN_PER,			// ������ȴ����X%
	MIJI_ARRT_JINENG_XISHU_PER,						// ����ϵ�����X%
	MIJI_SPECIAL_TYPE_MAX,
};

#pragma pack(push, 4)

struct EquipLevelList
{
	EquipLevelList() { this->Reset(); }

	void Reset()
	{
		memset(equip_Item_list, 0, sizeof(equip_Item_list));
	}

	ItemID equip_Item_list[CHINESE_ZODIAC_EQUIP_SLOT_MAX_LIMIT];		// ÿ����λ��Ӧװ���ȼ�
};

struct TianXiangTableGridParam
{
	TianXiangTableGridParam() { this->Reset(); }
	void Reset()
	{
		type = 0;
		reserve = 0;
	}

	bool IsEmpty() const { return 0 == type; }	//�Ƿ�Ϊ��

	short type;
	short reserve;
};

struct ComPosi
{
	ComPosi() { this->Reset(); }
	ComPosi(int _x, int _y): x(_x), y(_y) {}

	void Reset()
	{
		x = -1;
		y = -1;
	}

	short x;
	short y;
};

struct MijiParam
{
	MijiParam() {}

	char single_miji_list[MIJI_KONG_NUM];
};

struct TianXiangTotalTableGridParam
{   
	TianXiangTotalTableGridParam() { this->Reset(); }

	void Reset()
	{
		memset(table_lsit, 0, sizeof(table_lsit));
		memset(bead_by_combine_lsit, -1, sizeof(bead_by_combine_lsit));
		
	}
	// ���и���
	TianXiangTableGridParam table_lsit[TIAN_XIANG_TABEL_MIDDLE_GRIDS][TIAN_XIANG_TABEL_ROW_COUNT];

	// �������Ӷ�Ӧ�����
	ComPosi bead_by_combine_lsit[TIAN_XIANG_ALL_CHAPTER_COMBINE][TIAN_XIANG_COMBINE_MEX_BEAD_NUM];
};

struct XingLingParam
{   
	XingLingParam() { this->Reset(); }

	void Reset()
	{
		level = -1;
		bless = 0;
	}

	int level;
	int bless;
};

struct ChineseZodiacParam
{
	ChineseZodiacParam() { this->Reset(); }

	void Reset()
	{
		memset(zodiac_level_list, 0, sizeof(zodiac_level_list));
		memset(xinghun_level_list, 0, sizeof(xinghun_level_list));
		memset(xinghun_level_max_list, 0, sizeof(xinghun_level_max_list));
		memset(xinghun_baoji_value_list, 0, sizeof(xinghun_baoji_value_list));
 		memset(miji_list, -1, sizeof(miji_list));

		curr_chapter = 0;
		today_ggl_free_times = 0;
		zodiac_progress = 1;
		is_finish_all_chapter = 0;
		reserver_ch = 0;
		combine_system_msg_flag = 0;
		last_free_ggl_time = 1;
		xinghun_progress = 1;

		for (int i = 0; i < CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT; ++ i)
		{
			chinesezodiac_equip_list[i].Reset();
		}

		for (int i = 0; i < TIAN_XIANG_CHAPTER_NUM; ++ i)
		{
			chapter_lsit[i].Reset();
		}

		for (int i = 0; i < TIAN_XIANG_CHAPTER_NUM; ++ i)
		{
			xingling_lsit[i].Reset();
		}
	}

	short zodiac_level_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
	short xinghun_level_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
	short xinghun_level_max_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
	EquipLevelList chinesezodiac_equip_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
	
	TianXiangTotalTableGridParam chapter_lsit[TIAN_XIANG_CHAPTER_NUM];
	MijiParam miji_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];

	int curr_chapter;
	int today_ggl_free_times;		// ����ʹ��ҡ�����ֵ���Ѵ���
	int zodiac_progress;			// ������������
	char is_finish_all_chapter;		// �Ƿ�����������½�
	char reserver_ch;				
	short combine_system_msg_flag;	// ��Ͽ������ű��
	unsigned int last_free_ggl_time;// �����ѹ����ֵ�ʱ��

	XingLingParam xingling_lsit[TIAN_XIANG_CHAPTER_NUM];
	short xinghun_baoji_value_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
	int xinghun_progress;			// �ǻ꿪������
};

typedef char ChineseZodiacParamHex[sizeof(ChineseZodiacParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChineseZodiacParamHex) < 9216);

#pragma pack(pop)

#endif

