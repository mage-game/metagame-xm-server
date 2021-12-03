#ifndef __SHENGE_SYSTEM_DEF_HPP__
#define __SHENGE_SYSTEM_DEF_HPP__

#include "servercommon.h"

static const int SHENGE_SYSTEM_BAG_MAX_GRIDS = 250;									// �����������������ɱ� ���ݿ�
static const int SHENGE_SYSTEM_MAX_SHENGE_PAGE = 5;									// ������ҳ ���ɱ� ���ݿ�
static const int SHENGE_SYSTEM_CUR_SHENGE_PAGE = 3;									// �������ҳ��
UNSTD_STATIC_CHECK(SHENGE_SYSTEM_CUR_SHENGE_PAGE <= SHENGE_SYSTEM_MAX_SHENGE_PAGE);
static const int SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID = 16;							// ��ǰ��ͨ�������� 
static const int SHENGE_SYSTEM_MAX_SHENGE_GRID = 20;								// ��ͨ����������� ���ɱ� ���ݿ�
UNSTD_STATIC_CHECK(SHENGE_SYSTEM_CUR_MAX_SHENGE_GRID <= SHENGE_SYSTEM_MAX_SHENGE_GRID);

static const int SHENGE_SYSTEM_MAX_SHENGE_LEVEL = 400;								// ������ȼ�
static const int SHENGE_SYSTEM_PER_SHENGE_PAGE_MAX_ZHONGJI_SHENGE_COUNT = 4;		// ���ҳ�ռ����������
static const int SHENGE_SYSTEM_RECOVER_TIME_INTERVAL = 5;							// ��Ѫ���

static const int SHENGE_SYSTEM_QUALITY_MAX_VALUE = 4;								// ������������ֵ(�ƿصĸ�����)
static const int SHENGE_SYSTEM_ZHANGKONG_RAND_EXP_COUNT = 3;						// �ƿؿ�������ľ����С����
static const int SHENGE_SYSTEM_ZHANGKONG_MAX_LEVEL = 200;							// �ƿ���ߵȼ�
static const int SHENGE_SYSTEM_ZHANGKONG_MAX_GRADE = 40;							// �ƿ���߽���
static const int SHENGE_SYSTEM_ZHANGKONG_MAX_STAR = 5;								// �ƿ����������

static const int SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM = 3;
static const int SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM = 7;
static const int SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM = 10;
static const int SHENGE_SYSTEM_XILIAN_BAODI_MAX_COUNT = 9;

enum SHENGE_QUALITY
{
	SHENGE_QUALITY_BLUE,															// ��ɫ
	SHENGE_QUALITY_PURPLE,															// ��ɫ
	SHENGE_QUALITY_ORANGE,															// ��ɫ
	SHENGE_QUALITY_RED,																// ��ɫ
	SHENGE_QUALITY_GOLD,															// ��ɫ
	SHENGE_QUALITY_PING,															// ��ɫ

	SHENGE_QUALITY_COUNT															// �����ɫ�׼�
};

enum SHENGE_TYPE
{
	SHENGE_TYPE_HP = 0,																//��Ѫ
	SHENGE_TYPE_GONGJI,																//����
	SHENGE_TYPE_FANGYU,																//����
	SHENGE_TYPE_MINGZHONG,															//����
	SHENGE_TYPE_SHANBI,																//����
	SHENGE_TYPE_ADD_SHANGHAI,														//����
	SHENGE_TYPE_DIKANG_SHANGHAI,													//����
	SHENGE_TYPE_SPEED,																//����
	SHENGE_TYPE_LUCKY,																//����
	SHENGE_TYPE_GREEDY,																//̰��
	SHENGE_TYPE_NORMAL_COUNT,														//��ͨ�������

	SHENGE_TYPE_GONGFA = SHENGE_TYPE_NORMAL_COUNT,									//����
	SHENGE_TYPE_JIANJIA,															//���
	SHENGE_TYPE_JIXING,																//����
	SHENGE_TYPE_DONGCHA,															//����
	SHENGE_TYPE_HARVEST,															//����

	SHENGE_TYPE_COUNT																// ����������
};

enum SHENGE_SYSTEM_ATTR_TYPE
{
	SHENGE_SYSTEM_ATTR_TYPE_MAXHP,													// ���Ѫ��
	SHENGE_SYSTEM_ATTR_TYPE_GONGJI,													// ����
	SHENGE_SYSTEM_ATTR_TYPE_FANGYU,													// ����
	SHENGE_SYSTEM_ATTR_TYPE_ADD_SHANGHAI,											// ����
	SHENGE_SYSTEM_ATTR_TYPE_DIKANG_SHANGHAI,										// ����
	SHENGE_SYSTEM_ATTR_TYPE_SPEED,													// ����
	SHENGE_SYSTEM_ATTR_TYPE_BAOJI_FUJIA_SHANGHAI,									// ����
	SHENGE_SYSTEM_ATTR_TYPE_SHANBI,													// ����
	SHENGE_SYSTEM_ATTR_TYPE_ADD_ITEM_DROP,											// ����Ұ�����
	SHENGE_SYSTEM_ATTR_TYPE_ADD_EXP,												// ���Ӿ���

	SHENGE_SYSTEM_ATTR_TYPE_COUNT,													// ��������������
};

// Ч��Ϊ�ٷֱȼӳ�
enum SHENGE_SPECIAL_EFFECT_TYPE_PERCEN
{
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_EXP = 0,							// ���Ӿ���
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_ITEM_DROP,						// ����Ұ�����
	SHENGE_SPECIAL_EFFECT_TYPE_DIKANG_SHANGHAI,						// ����ҹ���ʱ����
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_SHANBI,							// �����������
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_DEC_FENGYU,						// ���ӷ�������(ʵ���������˺�)
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_ROLE_DAMAGE,						// ���Ӷ�����˺�
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_BOSS_DAMAGE,						// ���Ӷ�boss�˺�
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_SHENGE_ATTR,						// �����������
	SHENGE_SPECIAL_EFFECT_TYPE_ADD_RECOVER,							// ���ӻ�Ѫ����

	SHENGE_SPECIAL_EFFECT_TYPE_COUNT								// ����Ч���������
};

enum SHENGE_SYSTEM_XILIAN_ATTR_TYPE
{
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_MAXHP,													// ���Ѫ��
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_GONGJI,													// ����
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_FANGYU,													// ����
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_BAOJI_FUJIA_SHANGHAI,									// ����
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_SHANBI,													// ����
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_ADD_SHANGHAI,											// ����
	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_DIKANG_SHANGHAI,											// ����

	SHENGE_SYSTEM_XILIAN_ATTR_TYPE_COUNT,													// ������������������
};

#pragma pack(push, 4)

struct ShengeParamData
{
	ShengeParamData() { this->Reset(); }

	void Reset()
	{
		quality = -1;
		type = -1;
		level = 0;
	}

	char quality;				// �׼�
	char type;					// ����
	short level;				// �ȼ�

	bool IsEmpty() const { return level <= 0 || quality < 0 || type < 0; }
};

struct ShengeSinglePage
{
	ShengeSinglePage() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < SHENGE_SYSTEM_MAX_SHENGE_GRID; ++i)
		{
			shenge_grid[i].Reset();
		}
	}

	UNSTD_STATIC_CHECK(20 == SHENGE_SYSTEM_MAX_SHENGE_GRID)
		ShengeParamData shenge_grid[SHENGE_SYSTEM_MAX_SHENGE_GRID];							// ������
};

struct ZhangkongParam
{   
	ZhangkongParam() { this->Reset(); }

	void Reset()
	{
		level = 0;
		exp = 0;
	}

	int level;
	int exp;
};

// �������ϴ��
//struct AttrInfo
//{
//	AttrInfo() { this->Reset(); }
//	void Reset()
//	{
//		qianghua_times = 0;
//		attr_point = -1;
//		attr_value = 0;
//		reserve_sh = 0;
//	}
//
//	short qianghua_times;
//	short attr_point;
//	short attr_value;
//	short reserve_sh;
//};

struct AttrInfo
{
	AttrInfo() { this->Reset(); }
	void Reset()
	{
		qianghua_times = 0;
		attr_point = -1;
		attr_value = 0;
	}

	short qianghua_times;
	short attr_point;
	int attr_value;
};

struct ShengeShenquAttrInfo
{
	ShengeShenquAttrInfo() { this->Reset(); }
	void Reset()
	{
		for (int i = 0; i < SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM; ++ i)
		{
			attr_info[i].Reset();
		}
	}

	AttrInfo attr_info[SHENGE_SYSTEM_SHENGESHENQU_XILIAN_SLOT_MAX_NUM];
};

struct SingleShengeShenqu
{
	SingleShengeShenqu() { this->Reset(); }
	void Reset()
	{
		for (int i = 0; i < SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM; ++ i)
		{
			shenqu_attr_list[i].Reset();
		}
	}

	ShengeShenquAttrInfo shenqu_attr_list[SHENGE_SYSTEM_SHENGESHENQU_ATTR_MAX_NUM];
};

struct ShengeSystemParam
{
	ShengeSystemParam() { this->Reset(); }

	void Reset()
	{
		using_shenge_page_index = 0;
		free_choujiang_times = 0;
		is_can_get_fisrt_ten_chou = 0;
		reserve_ch = 0;
		next_free_chou_timestamp = 0;
		marrow_score = 0;

		for (int i = 0; i < SHENGE_SYSTEM_MAX_SHENGE_PAGE; ++i)
		{
			shenge_page_list[i].Reset();
		}

		for (int i = 0; i < SHENGE_SYSTEM_BAG_MAX_GRIDS; ++i)
		{
			virtual_bag_list[i].Reset();
		}

		for (int i = 0; i < SHENGE_SYSTEM_QUALITY_MAX_VALUE; ++i)
		{
			zhangkong_level_list[i].Reset();
		}

		memset(history_set_total_level, 0, sizeof(history_set_total_level));

		for (int i = 0; i < SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM; ++i)
		{
			shenqu_list[i].Reset();
		}

		memset(shenqu_history_cap_list, 0, sizeof(shenqu_history_cap_list));
	}

	char using_shenge_page_index;																	// ʹ�õ����ҳ
	char free_choujiang_times;																		// ��ѳ齱����
	char is_can_get_fisrt_ten_chou;																	// ��һ��ʮ�����־
	char reserve_ch;
	unsigned int next_free_chou_timestamp;															// ���´���ѳ齱��ʣ��cd
	int marrow_score;																				// �����Ƭ
	UNSTD_STATIC_CHECK(5 == SHENGE_SYSTEM_MAX_SHENGE_PAGE)
		ShengeSinglePage shenge_page_list[SHENGE_SYSTEM_MAX_SHENGE_PAGE];							// ���ҳ����
	UNSTD_STATIC_CHECK(250 == SHENGE_SYSTEM_BAG_MAX_GRIDS)
		ShengeParamData virtual_bag_list[SHENGE_SYSTEM_BAG_MAX_GRIDS];								// ���ⱳ��
	int history_set_total_level[SHENGE_SYSTEM_MAX_SHENGE_PAGE];										// ��ʷװ����ߵȼ�
	ZhangkongParam zhangkong_level_list[SHENGE_SYSTEM_QUALITY_MAX_VALUE];							// �ƿ�

	SingleShengeShenqu shenqu_list[SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM];
	int shenqu_history_cap_list[SHENGE_SYSTEM_SHENGESHENQU_MAX_NUM];
};

typedef char ShengeSystemParamHex[sizeof(ShengeSystemParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShengeSystemParamHex) < 8192);

#pragma pack(pop)

#endif
