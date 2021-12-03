#ifndef TALENTDEF_DEF_HPP
#define TALENTDEF_DEF_HPP

#include "servercommon.h"

#pragma pack(push, 4)

static const int JINJIE_TALENT_CHOUJIANG_GRID_MAX_NUM = 9;	//�츳�齱����������
static const int JINJIE_TALENT_SKILL_GRID_MAX_NUM = 13;		//�츳��������������
//static const int TALENT_TYPE_MAX = GREATE_SOLDIER_MAX_COUNT;	//�����츳����
static const int TALENT_ATTENTION_SKILL_MAX_SAVE_NUM = 32;		//��ע�ļ����б�

enum JINJIE_TALENT_TYPE
{
	JINJIE_TALENT_MOUNT = 0,		//����
	JINJIE_TALENT_WING,				//����
	JINJIE_TALENT_HALO,				//�⻷
	JINJIE_TALENT_FIGHTMOUNT,		//ħ��
	//JINJIE_TALENT_SHENGGONG,		//��
	JINJIE_TALENT_SHIZHUANG,		//ʱװ
	//JINJIE_TALENT_SHENYI,			//����
	JINJIE_TALENT_SHENBING,			//���
	JINJIE_TALENT_FOOTPRINT,		//���
	JINJIE_TALENT_FABAO,			//����
	JINJIE_TALENT_TYPE_MAX,
};

enum JINJIE_TALENT_SKILL_TYPE
{
	JINJIE_TALENT_SKILL_TYPE_0 = 0,		//��Ѫ
	JINJIE_TALENT_SKILL_TYPE_1,			//����
	JINJIE_TALENT_SKILL_TYPE_2,			//����
	JINJIE_TALENT_SKILL_TYPE_3,			//����
	JINJIE_TALENT_SKILL_TYPE_4,			//����
	JINJIE_TALENT_SKILL_TYPE_5,			//����
	JINJIE_TALENT_SKILL_TYPE_6,			//����
	JINJIE_TALENT_SKILL_TYPE_7,			//�̶�����
	JINJIE_TALENT_SKILL_TYPE_8,			//�̶�����
	JINJIE_TALENT_SKILL_TYPE_9,			//��Ӧϵͳ�������԰ٷֱ�
	JINJIE_TALENT_SKILL_TYPE_10,			//���츳ҳ��Ѫ�ٷֱ�+�̶�ֵ
	JINJIE_TALENT_SKILL_TYPE_11,			//���츳ҳ�����ٷֱ�+�̶�ֵ
	JINJIE_TALENT_SKILL_TYPE_12,			//���츳ҳ�����ٷֱ�+�̶�ֵ
	JINJIE_TALENT_SKILL_TYPE_13,			//���츳ҳ���аٷֱ�+�̶�ֵ
	JINJIE_TALENT_SKILL_TYPE_14,			//���츳ҳ���ܰٷֱ�+�̶�ֵ
	JINJIE_TALENT_SKILL_TYPE_15,			//���츳ҳ�����ٷֱ�+�̶�ֵ
	JINJIE_TALENT_SKILL_TYPE_16,			//���츳ҳ�����ٷֱ�+�̶�ֵ
	JINJIE_TALENT_SKILL_TYPE_17,			//���츳ҳ�̶����˰ٷֱ�+�̶�ֵ
	JINJIE_TALENT_SKILL_TYPE_18,			//���츳ҳ�̶����˰ٷֱ�+�̶�ֵ
	JINJIE_TALENT_SKILL_TYPE_19,			//�����ռ�����
	JINJIE_TALENT_SKILL_TYPE_20,			//�����ռ�����
	JINJIE_TALENT_SKILL_TYPE_21,			//�⻷�ռ�����
	JINJIE_TALENT_SKILL_TYPE_22,			//ħ���ռ�����
	JINJIE_TALENT_SKILL_TYPE_23,			//���ռ�����
	JINJIE_TALENT_SKILL_TYPE_24,			//�����ռ�����
	JINJIE_TALENT_SKILL_TYPE_25,			//�㼣�ռ�����
	JINJIE_TALENT_SKILL_TYPE_26,			//�����ռ�����
	JINJIE_TALENT_SKILL_TYPE_COUNT,
};

/*
�츳���ܸ�
*/
struct SCJinJieTalentGrid
{
public:
	SCJinJieTalentGrid()
	{
		Reset();
	}
	void Reset()
	{
		is_open = 0;
		skill_star = 0;
		skill_id = 0;
	}

	char is_open;				//�����Ƿ���
	char skill_star;			//�����Ǽ�
	short skill_id;				//����ID
};

/*
�����츳
*/
struct SCJinJieTalentSingle
{
public:
	SCJinJieTalentSingle()
	{
		Reset();
	}

	void Reset()
	{
		memset(grid_array, 0, sizeof(grid_array));
	}

	SCJinJieTalentGrid grid_array[JINJIE_TALENT_SKILL_GRID_MAX_NUM];
};

struct JinJieTalentParam
{
	JinJieTalentParam()
	{
		this->Reset();
	}

	void Reset()
	{
		flush_count = 0;
		memset(choujiang_grid_skill, 0, sizeof(choujiang_grid_skill));
		for (int i = 0; i < JINJIE_TALENT_TYPE_MAX; ++i)
		{
			skill_info_list[i].Reset();
		}
		free_chou_count = 0;
		reserve_sh = 0;

		memset(attention_skill_id, 0, sizeof(attention_skill_id));
	}

	void ResetChoujiang()
	{
		flush_count = 0;
		memset(choujiang_grid_skill, 0, sizeof(choujiang_grid_skill));
	}

	short flush_count;
	short choujiang_grid_skill[JINJIE_TALENT_CHOUJIANG_GRID_MAX_NUM];						//�齱����
	SCJinJieTalentSingle skill_info_list[JINJIE_TALENT_TYPE_MAX];
	short free_chou_count;
	short reserve_sh;
	short attention_skill_id[TALENT_ATTENTION_SKILL_MAX_SAVE_NUM];
};

typedef char JinJieTalentParamHex[sizeof(JinJieTalentParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(JinJieTalentParamHex) < 1024);

#pragma pack(pop)
#endif