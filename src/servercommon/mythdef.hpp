#ifndef __MYTH_DEF_HPP__
#define __MYTH_DEF_HPP__

#include "servercommon.h"

static const int MAX_MYTH_CHAPTER_ID = 20;
static const int MAX_MYTH_SOUL_SLOT = 3;
static const int MAX_MYTH_SOUL_TYPE = 6;
static const int MAX_MYTH_DIGESTION_LEVEL = 300;
static const int MAX_MYTH_CHPATER_LEVEL = 150;
static const int MAX_MYTH_QUALITY_SOUL = 5;
static const int MAX_MYTH_RESONANCE_LEVEL = 20;
static const int MAX_MYTH_RESONANCE_ATTR_NUM = 4;
static const int MAX_MYTH_BAOJI_MULTIPLE = 3;
static const int MAX_MYTH_SOUL_QUALITY = 5;
static const int MAX_MYTH_STAR_SOUL = 3;
static const int MAX_MYTH_RAND_ATTR_NORMAL_COUNT = 9;
static const int MAX_MYTH_RAND_ATTR_RARE_COUNT = 9;

static const int MAX_MYTH_KNAPSACK_GIRD_COUNT = 100;							// ������
static const int MAX_MYTH_SOUL_RAND_ATTR_COUNT = 3;								// ���������

static const int MAX_MYTH_KNAPSACK_GIRD_STACK_COUNT = 99;						// �ѵ�����

enum MYTH_ATTR_TYPE																// װ��������������
{
	MYTH_ATTR_TYPE_INVALID_MIN = 0,

	MYTH_ATTR_TYPE_PER_HP,														// ������ֱ�
	MYTH_ATTR_TYPE_PER_GONGJI,													// ������ֱ�
	MYTH_ATTR_TYPE_PER_FANGYU,													// �����ֱ�
	MYTH_ATTR_TYPE_PER_SHANBI,													// ������ֱ�
	MYTH_ATTR_TYPE_PER_MINGZHONG,												// ������ֱ�
	MYTH_ATTR_TYPE_PER_BAOJI,													// ������ֱ�
	MYTH_ATTR_TYPE_PER_JIANREN,													// ������ֱ�
	MYTH_ATTR_TYPE_PER_GUSHANG,													// ������ֱ�
	MYTH_ATTR_TYPE_PER_JIANSHANG,												// ������ֱ�
	MYTH_ATTR_TYPE_MAX_HP,														// �����̶�ֵ
	MYTH_ATTR_TYPE_GONGJI,														// �����̶�ֵ
	MYTH_ATTR_TYPE_FANGYU,														// ����̶�ֵ
	MYTH_ATTR_TYPE_SHANBI,														// ���̶ܹ�ֵ
	MYTH_ATTR_TYPE_MINGZHONG,													// ���й̶�ֵ
	MYTH_ATTR_TYPE_BAOJI,														// �����̶�ֵ
	MYTH_ATTR_TYPE_JIANREN,														// �����̶�ֵ
	MYTH_ATTR_TYPE_GUSHANG,														// ���˹̶�ֵ											
	MYTH_ATTR_TYPE_JIANSHANG,													// ���˹̶�ֵ

	MYTH_ATTR_TYPE_INVALID_MAX,
};

enum MYTH_RESONANCE_ATTR_TYPE
{
	MYTH_RESONANCE_ATTR_TYPE_MAXHP = 1,
	MYTH_RESONANCE_ATTR_TYPE_GONGJI,
	MYTH_RESONANCE_ATTR_TYPE_FANGYU,
	MYTH_RESONANCE_ATTR_TYPE_MINGZHONG,
	MYTH_RESONANCE_ATTR_TYPE_SHANBI,
	MYTH_RESONANCE_ATTR_TYPE_BAOJI,
	MYTH_RESONANCE_ATTR_TYPE_JIANREN,
	MYTH_RESONANCE_ATTR_TYPE_CONSTANT_ZENGSHANG,
	MYTH_RESONANCE_ATTR_TYPE_CONSTANT_MIANSHANG,
	MYTH_RESONANCE_ATTR_TYPE_PER_GONGJI,										// ������ֱ�
	MYTH_RESONANCE_ATTR_TYPE_PER_FANGYU,										// �����ֱ�
	MYTH_RESONANCE_ATTR_TYPE_PER_HP,											// ������ֱ�
	MYTH_RESONANCE_ATTR_TYPE_PER_BAOJI,											// ������ֱ�
	MYTH_RESONANCE_ATTR_TYPE_PER_GUSHANG,										// ������ֱ�
	MYTH_RESONANCE_ATTR_TYPE_PER_JIANSHANG,										// ������ֱ�

	MYTH_RESONANCE_ATTR_TYPE_SKILL_ZENGSHANG_PER = 21,
	MYTH_RESONANCE_ATTR_TYPE_SKILL_JIANSHANG_PER,
	MYTH_RESONANCE_ATTR_TYPE_PER_KANGBAO,
	MYTH_RESONANCE_ATTR_TYPE_MINGZHONG_PER,
	MYTH_RESONANCE_ATTR_TYPE_SHANBI_PER,
	MYTH_RESONANCE_ATTR_TYPE_GEDANG_PER,
};


#pragma pack(push, 4)

struct MythSoulGodItem
{
	bool Isvalid() const
	{
		return item_id > 0;
	}

	struct AttrItem
	{
		AttrItem()
		{
			this->Reset();
		}

		void Reset()
		{
			attr_type = 0;
			reserve_sh = 0;
			attr_value = 0;
		}

		short attr_type;
		short reserve_sh;
		int attr_value;
	};

	MythSoulGodItem() { this->Reset(); }

	void Reset()
	{
		item_id = 0;
		num = 0;
		quality = 0;

		for (int i = 0; i < MAX_MYTH_SOUL_RAND_ATTR_COUNT; i++)
		{
			attr_list[i].Reset();
		}
	}

	ItemID item_id;
	unsigned char num;
	unsigned char quality;
	AttrItem attr_list[MAX_MYTH_SOUL_RAND_ATTR_COUNT];
};

struct MythResonanceItem
{
	MythResonanceItem() { this->Reset();}

	bool IsActivatedResonance()
	{
		return (0 != resonance_level);
	}

	void ActivatedResonance()
	{
		resonance_level = 1;
		memset(cur_level_resonance, 0, sizeof(cur_level_resonance));
	}

	void ResonanceUpLevel()
	{
		++resonance_level;
		memset(cur_level_resonance, 0, sizeof(cur_level_resonance));
	}

	void Reset()
	{
		memset(cur_level_resonance, 0, sizeof(cur_level_resonance));
		resonance_level = 0;
	}

	char cur_level_resonance[MAX_MYTH_SOUL_SLOT];
	char dirty_ch;			// �����ã�û�г�ʼ����������

	short resonance_level;
	short dirty_sh;			// �����ã�û�г�ʼ����������
};

struct MythChpaterItem
{
	MythChpaterItem() { this->Reset(); }

	void Reset()
	{
		level = 0;
		digestion_level = 0;
		digestion_level_val = 0;
	
		resonance_item.Reset();

		for (int i = 0; i < MAX_MYTH_SOUL_SLOT; i++)
		{
			soul_god_list[i].Reset();
		}
	}

	bool IsActivatingMythChpater()
	{
		return 1 == level;
	}

	void UpLevelMythChpater()
	{
		++level;
	}

	short level;
	short digestion_level;
	int digestion_level_val;

	MythResonanceItem resonance_item;
	MythSoulGodItem soul_god_list[MAX_MYTH_SOUL_SLOT];
};

struct MythParam
{
	MythParam() { this->Reset(); }

	void Reset()
	{
		soul_essence = 0;

		for (int i = 0; i < MAX_MYTH_CHAPTER_ID; i ++)
		{
			chpater_list[i].Reset();
		}

		for (int i = 0; i < MAX_MYTH_KNAPSACK_GIRD_COUNT; i++)
		{
			knapasck_list[i].Reset();
		}
	}

	int soul_essence;														//��꾫�����ֽ��ã�

	MythSoulGodItem knapasck_list[MAX_MYTH_KNAPSACK_GIRD_COUNT];			//�����б�
	MythChpaterItem chpater_list[MAX_MYTH_CHAPTER_ID];						//���ں��淽����չ		
};

typedef char MythParamHex[sizeof(MythParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MythParamHex) < 10240);

#pragma pack(pop)

#endif //__MYTH_DEF_HPP__