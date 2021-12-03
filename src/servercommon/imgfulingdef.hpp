#ifndef __IMG_FULING_DEF_HPP__
#define __IMG_FULING_DEF_HPP__

#include "servercommon.h"

enum JINGJIE_TYPE
{
	JINGJIE_TYPE_MOUNT = 0,							//坐骑
	JINGJIE_TYPE_WING = 1,							//羽翼
	JINGJIE_TYPE_HALO = 2,							//光环
	JINGJIE_TYPE_FIGHT_MOUNT = 3,					//魔骑
	JINGJIE_TYPE_SHIZHUANG = 4,						//时装
	JINGJIE_TYPE_SHIZHUANG_WEAPON = 5,				//神兵(时装武器)
	JINGJIE_TYPE_FOOT_PRINT = 6,					//足迹
	JINGJIE_TYPE_FABAO = 7,					        //法宝
	JINGJIE_TYPE_SHENYI = 8,                        //神翼
	JINGJIE_TYPE_SHENGONG = 9,                      //神弓
	JINGJIE_TYPE_JINGJIE = 10,                       //境界

	JINGJIE_TYPE_COUNT,
};

enum IMG_FULING_SKILL
{
	IMG_FULING_SKILL_0 = 0,							
	IMG_FULING_SKILL_1 = 1,
	IMG_FULING_SKILL_2 = 2,							
	IMG_FULING_SKILL_3 = 3,					
	IMG_FULING_SKILL_4 = 4,						
	IMG_FULING_SKILL_5 = 5,						
	IMG_FULING_SKILL_6 = 6,
	IMG_FULING_SKILL_7 = 7,

	IMG_FULING_SKILL_MAX,
};

static const unsigned int IMG_FULING_SLOT_COUNT = 7;											// 每个类型的赋灵槽数

#pragma pack(push, 4)

struct ImgTypeFulingParam
{
	ImgTypeFulingParam() : img_count(0), level(0), skill_level(0), cur_exp(0)
	{
		memset(img_id_list, 0, sizeof(img_id_list));
	}

	unsigned char img_id_list[IMG_FULING_SLOT_COUNT];								//每个形象槽的形象id
	unsigned char img_count;														//目前激活的形象槽总数
	short level;																	//等级
	short skill_level;																//技能等级
	unsigned int cur_exp;															//当前等级经验
};

struct ImgFulingParam
{
	ImgFulingParam()
	{
		this->Reset();
	}

	void Reset()
	{
		memset(fuling_list, 0, sizeof(fuling_list));
		first_check = 0;
		reserve_ch = 0;
		reserve_sh = 0;
	}

	ImgTypeFulingParam fuling_list[JINGJIE_TYPE_COUNT];
	
	char first_check;
	char reserve_ch;
	short reserve_sh;
};

typedef char ImgFulingParamHex[sizeof(ImgFulingParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ImgFulingParamHex) < 512);

#pragma pack(pop)

#endif  // __IMG_FULING_DEF_HPP__

