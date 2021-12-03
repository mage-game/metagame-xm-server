#ifndef __RUNE_SYSTEM_DEF_HPP__
#define __RUNE_SYSTEM_DEF_HPP__

#include "servercommon.h"

static const int RUNE_SYSTEM_BAG_MAX_GRIDS = 200;								// �����������������ɱ� ���ݿ�
static const int RUNE_SYSTEM_SLOT_MAX_NUM = 10;									// ���Ĳ������
static const int RUNE_SYSTEM_RUNE_MAX_LEVEL = 200;								// �������ȼ�
static const int RUNE_SYSTEM_XUNBAO_RUNE_MAX_COUNT = 10;						// Ѱ���÷����������
static const int RUNE_SYSTEM_BOX_REWAED_MAX_ITEM = 8;							// ���ı��������Ʒ

enum RUNE_QUALITY_KIND
{
	RUNE_QUALITY_KIND_WHITE = 0,				// ��ɫ����
	RUNE_QUALITY_KIND_BLUE,						// ��ɫ����
	RUNE_QUALITY_KIND_PURPLE,					// ��ɫ����
	RUNE_QUALITY_KIND_ORANGE,					// ��ɫ����
	RUNE_QUALITY_KIND_RED,						// ��ɫ����
	RUNE_QUALITY_KIND_PINK,                     // ��ɫ����

	RUNE_QUALITY_KIND_MAX,
};

enum RUNE_TYPE
{
	RUNE_TYPE_GONGJI = 0,														// ����
	RUNE_TYPE_MAXHP,															// ����
	RUNE_TYPE_BAOJI,															// ����
	RUNE_TYPE_SHANBI,															// ����
	RUNE_TYPE_EXP,																// ����
	RUNE_TYPE_MINGZHONG,														// ����
	RUNE_TYPE_FANGYU,															// ����
	RUNE_TYPE_KANGBAO,															// ����
	RUNE_TYPE_HUOLI,															// ����
	RUNE_TYPE_PROTECT,															// �ػ�
	RUNE_TYPE_FENGRUI,															// ����
	RUNE_TYPE_JIANREN,															// ����
	RUNE_TYPE_JINGZHUN,															// ��׼
	RUNE_TYPE_XIANYOU,															// ����
	RUNE_TYPE_CHANGSHENG,														// ����
	RUNE_TYPE_JIANGANG,															// ���
	RUNE_TYPE_KUANGBAO,															// ��
	RUNE_TYPE_FENSUI,															// ����
	RUNE_TYPE_SHALU,															// ɱ¾
	RUNE_TYPE_RUNEJINGHUA,														// ���ľ�����������棩
	RUNE_TYPE_WUSHIYIJI,														// ����һ��
	RUNE_TYPE_WUSHUANG,                                                         // ��˫
	RUNE_TYPE_QIANGGONG,                                                        // ǿ��
	RUNE_TYPE_QIANGREN,                                                         // ǿ��
	RUNE_TYPE_XIANGONG,                                                         // �ɹ�
	RUNE_TYPE_XIANBAO,                                                          // �ɱ�
	RUNE_TYPE_LINGQIAO,                                                         // ����

	RUNE_TYPE_MAX,																//����������
};

enum RUNE_ATTR_TYPE
{
	RUNE_ATTR_TYPE_INVALID = 0,
	RUNE_ATTR_TYPE_GONGJI,											// ����
	RUNE_ATTR_TYPE_MAXHP,											// ����
	RUNE_ATTR_TYPE_BAOJI,											// ����
	RUNE_ATTR_TYPE_SHANBI,											// ����
	RUNE_ATTR_TYPE_EXP_FETCH,										// ���ﾭ��(%)
	RUNE_ATTR_TYPE_MINGZHONG,										// ����
	RUNE_ATTR_TYPE_JIANREN,											// ����
	RUNE_ATTR_TYPE_FANGYU,											// ����
	RUNE_ATTR_TYPE_WUQI_GONGJI,										// ��������(%)
	RUNE_ATTR_TYPE_FANGJU_HP,										// ��������(%)
	RUNE_ATTR_TYPE_FANGJU_SHANBI,									// ��������(%)
	RUNE_ATTR_TYPE_FANGJU_FANGYU,									// ���߷���(%)
	RUNE_ATTR_TYPE_FANGJU_JIANREN,									// ���߼���(%)
	RUNE_ATTR_TYPE_PER_POFANG,										// �Ʒ��ٷֱȣ������ʣ�
	RUNE_ATTR_TYPE_WUQI_BAOJI,                                      // ��������(%)
	RUNE_ATTR_TYPE_PER_MIANSHANG,                                   // ������
	RUNE_ATTR_TYPE_SHOUSHI_GONGJI,                                  // ���ι���(��ָ����������������)
	RUNE_ATTR_TYPE_SHOUSHI_BAOJI,                                   // ���α���(��ָ����������������)
	RUNE_ATTR_TYPE_PER_POJIA,                                       // �Ƽ�
	RUNE_ATTR_TYPE_PER_EQUIP_GONGJI,                                // ȫ����ͨװ�������ӳ���ֱ�
	RUNE_ATTR_TYPE_FUJIA_SHANGHAI,									// �����˺�

	RUNE_ATTR_TYPE_COUNT,											// ���������������
};

// Ч��Ϊ�ٷֱȼӳ�
enum RUNE_SPECIAL_EFFECT_TYPE_PERCEN
{
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_INVALID = 0,
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_WUQI_GONGJI,					// ������������
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_HP,						// ���ӷ�������
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_SHANBI,					// ���ӷ�������
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_FANGYU,					// ���ӷ��߷���
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_FANGJU_JIANREN,				// ���ӷ��߼���
	RUNE_SPECIAL_EFFECT_TYPE_ADD_KILL_MONSTER_EXP,					// ����ɱ�־���
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_WUQI_BAOJI,                    // ������������
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_SHOUSHI_GONGJI,                // �������ι���
	RUNE_SPECIAL_EFFECT_TYPE_ADD_PER_SHOUSHI_BAOJI,                 // �������α���
	RUNE_SPECIAL_EFFECT_TYPE_ADD_EQUIP_PER_GONGJI,					// ����ȫ����ͨװ������

	RUNE_SPECIAL_EFFECT_TYPE_COUNT									// ����Ч���������
};

enum RUNE_BOX_TYPE
{
	RUNE_BOX_TYPE_INVALID = 0,
	RUNE_BOX_TYPE_JINGHUA,				// �������䣬���Կ�������
	RUNE_BOX_TYPE_RUNE_AND_JINGHUA,		// ���ı��䣬���Կ��������ͷ���

	RUNE_BOX_TYPE_MAX,
};

enum RUNE_WEIGHT_TYPE
{
	RUNE_WEIGHT_TYPE_DEFAULT = 0,     // Ĭ��Ȩ��
	RUNE_WEIGHT_TYPE_X,			      // �ۼ�X��Ȩ��
	RUNE_WEIGHT_TYPE_X1,		      // �ۼ�X1��Ȩ��
};

#pragma pack(push, 4)

struct RuneParam
{
	RuneParam() { this->Reset(); }

	void Reset()
	{
		quality = -1;
		type = -1;
		level = 0;
	}

	char quality;				// Ʒ��
	char type;					// ����
	short level;				// �ȼ�

	bool IsEmpty() const { return level <= 0 || quality < 0 || type < 0; }
};

struct RuneAwakenParm
{
	RuneAwakenParm(){this->Reset();}

	void Reset()
	{
		maxhp = 0;
		gongji = 0;
		fangyu = 0;
		mingzhong = 0;
		shanbi = 0;
		baoji = 0;
		jianren = 0;
		add_per = 0;	
	}

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;		// δʹ��-�Ժ����չ
	Attribute shanbi;			// δʹ��-�Ժ����չ
	Attribute baoji;			// δʹ��-�Ժ����չ
	Attribute jianren;		// δʹ��-�Ժ����չ
	int add_per;		// Ч������
};

struct RuneZhuLingParam
{
	RuneZhuLingParam() { this->Reset(); }
	void Reset()
	{
		grade = 0;
		zhuling_bless = 0;
	}

	int grade;
	int zhuling_bless;
};

struct RuneSystemParam
{
	RuneSystemParam() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < RUNE_SYSTEM_SLOT_MAX_NUM; ++i)
		{
			rune_grid[i].Reset();
		}

		for (int i = 0; i < RUNE_SYSTEM_SLOT_MAX_NUM; ++i)
		{
			rune_grid_awaken[i].Reset();
		}

		for (int i = 0; i < RUNE_SYSTEM_BAG_MAX_GRIDS; ++i)
		{
			virtual_bag_list[i].Reset();
		}

		rune_suipian = 0;
		rune_jinghua = 0;
		magic_crystal = 0;

		next_free_xunbao_timestamp = 0;
		rune_slot_open_flag = 1;
		free_xunbao_times = 0;
		fb_fetch_time_count = 0;

		fb_pass_layer = 0;
		fb_today_layer = 0;
		fb_offline_time = 3600 * 12;
		fb_add_exp = 0;
		fb_add_jinghua = 0;
		fb_add_equip_blue = 0;
		fb_add_equip_purple = 0;
		fb_add_equip_orange = 0;

		is_need_recalc_awaken = 0;
		baodi = 0;

		rune_gold_awaken_times = 0;
		rune_common_awaken_times = 0;

		for (int i = 0; i < RUNE_SYSTEM_SLOT_MAX_NUM; ++ i)
		{
			run_zhuling_list[i].Reset();
		}

		zhuling_slot_bless = 0;
		total_xunbao_times = 0;
	}

	UNSTD_STATIC_CHECK(10 == RUNE_SYSTEM_SLOT_MAX_NUM);
	UNSTD_STATIC_CHECK(200 == RUNE_SYSTEM_BAG_MAX_GRIDS);

	RuneParam rune_grid[RUNE_SYSTEM_SLOT_MAX_NUM];							// ���ĸ�������
	RuneParam virtual_bag_list[RUNE_SYSTEM_BAG_MAX_GRIDS];					// ���ⱳ��

	int rune_suipian;														// ������Ƭ
	int rune_jinghua;														// ���ľ���
	int magic_crystal;														// ����ˮ��

	unsigned int next_free_xunbao_timestamp;								// ����Ѱ���´����ʱ���
	unsigned short rune_slot_open_flag;										// ���Ĳۿ������(���һλ������¼���ĺϳ��Ƿ���)
	char free_xunbao_times;													// ���Ѱ������

	char fb_fetch_time_count;												// ��������ȡ����ʱ�����
	int fb_pass_layer;														// ������ͨ�ز���
	int fb_today_layer;														// ���������ղ���
	int fb_offline_time;													// ����������ʱ��
	long long fb_add_exp;													// ���������ջ�þ���
	int fb_add_jinghua;														// ���������ջ�þ���
	int fb_add_equip_blue;													// ���������ջ����װ
	int fb_add_equip_purple;												// ���������ջ����װ
	int fb_add_equip_orange;												// ���������ջ�ó�װ

	short is_need_recalc_awaken;											// �Ƿ���Ҫ����������� 1��Ҫ�� 0����Ҫ
	short baodi;
	RuneAwakenParm rune_grid_awaken[RUNE_SYSTEM_SLOT_MAX_NUM];				// ���ĸ����

	int rune_gold_awaken_times;												// ����Ԫ�����Ѵ���
	int rune_common_awaken_times;											// ������ͨ���Ѵ���

	RuneZhuLingParam run_zhuling_list[RUNE_SYSTEM_SLOT_MAX_NUM];
	int zhuling_slot_bless;
	unsigned int total_xunbao_times;										// ��ʷѰ������
};

typedef char RuneSystemParamHex[sizeof(RuneSystemParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RuneSystemParamHex) < 8192);

#pragma pack(pop)
#endif
