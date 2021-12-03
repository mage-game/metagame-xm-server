#ifndef __SHENQI_DEF_HPP__
#define __SHENQI_DEF_HPP__

#include "servercommon.h"

static const int SHENQI_SUIT_NUM_MAX = 64;		//神器套装最大数
UNSTD_STATIC_CHECK(SHENQI_SUIT_NUM_MAX <= 64);

enum SHENQI_PART_TYPE
{
	SHENQI_PART_TYPE_0 = 0,				//剑穗/头盔
	SHENQI_PART_TYPE_1,					//剑柄/护颈
	SHENQI_PART_TYPE_2,					//剑身/心镜
	SHENQI_PART_TYPE_3,					//剑尖/战袍

	SHENQI_PART_TYPE_MAX
};
UNSTD_STATIC_CHECK(4 == SHENQI_PART_TYPE_MAX);

enum SHENQI_QUALITY_TYPE
{
	SHENQI_QUALITY_TYPE_INVALID = 0,
	SHENQI_QUALITY_TYPE_BLUE,			//蓝品质
	SHENQI_QUALITY_TYPE_PURPLE,			//紫品质
	SHENQI_QUALITY_TYPE_ORANGE,			//橙品质
	SHENQI_QUALTTY_TYPE_RED,			//红品质

	SHENQI_QUALITY_TYPE_MAX
};

enum SHENQI_SPECIAL_TEXIAO_TYPE
{
	SHENQI_SPECIAL_TEXIAO_TYPE_FOOL_DEAD,			// 使被击杀者变形
	SHENQI_SPECIAL_TEXIAO_TYPE_DANCING,				// 跳舞

	SHENQI_SPECIAL_TEXIAO_TYPE_MAX
};

#pragma pack(push, 4)

struct ShenqiItemParam
{
	void Reset()
	{
		level = 0;
		reserve_sh = 0;
		exp = 0;
		memset(quality_list, 0, sizeof(quality_list));
	}

	ShenqiItemParam() { this->Reset(); }

	short level;
	short reserve_sh;
	int exp;
	char quality_list[SHENQI_PART_TYPE_MAX];
};

struct ShenqiParam
{
	void Reset() 
	{
		for (int i = 0; i < SHENQI_SUIT_NUM_MAX; ++i)
		{
			shenbing_list[i].Reset();
			baojia_list[i].Reset();
		}

		shenbing_image_flag = 0;
		shenbing_texiao_flag = 0;
		baojia_image_flag = 0;
		baojia_texiao_flag = 0;

		shenbing_cur_image_id = 0;
		shenbing_cur_texiao_id = 0;
		baojia_cur_image_id = 0;
		baojia_cur_texiao_id = 0;

		killer_shenbing_texiao_id = 0;
		baojia_texiao_open_flag = 0;
		reserve_ch = 0;
	}

	ShenqiParam() { this->Reset(); }

	ShenqiItemParam shenbing_list[SHENQI_SUIT_NUM_MAX];
	ShenqiItemParam baojia_list[SHENQI_SUIT_NUM_MAX];

	unsigned long long shenbing_image_flag;
	unsigned long long shenbing_texiao_flag;
	unsigned long long baojia_image_flag;
	unsigned long long baojia_texiao_flag;
	
	char shenbing_cur_image_id;
	char shenbing_cur_texiao_id;
	char baojia_cur_image_id;
	char baojia_cur_texiao_id;

	short killer_shenbing_texiao_id;
	char baojia_texiao_open_flag;				// 宝甲特殊效果开启标记
	char reserve_ch;
};

typedef char ShenqiParamHex[sizeof(ShenqiParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShenqiParamHex) < 3264);

#pragma pack(pop)

#endif  // __XIANJIE_DEF_HPP__

