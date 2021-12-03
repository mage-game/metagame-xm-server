
#ifndef __HESHENLUOSHU_DEF_HPP__
#define __HESHENLUOSHU_DEF_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"

static const int HESHENLUOSHU_MAX_TYPE = 5;											// 河神洛图最大类型
static const int HESHENLUOSHU_MAX_SEQ = 10;											// 河神洛图最大索引
static const int HESHENLUOSHU_MAX_INDEX = 16;										// 河神洛图最大数量
static const int HESHENLUOSHU_SUIT_MAX_NUM = 8;										// 河神洛图最大套装数
static const int HESHENLUOSHU_ITEM_MAX_LEVEL = 200;									// 河神洛图卡片最大等级
static const int HESHENLUOSHU_HUASHEN_MAX_LEVEL = 100;								// 河神洛图卡片最大化神等级
#pragma pack(push, 4)

struct HeShenLuoShuHuaShenParam
{
	HeShenLuoShuHuaShenParam() : level(0), exp(0) {}
	short level;
	short exp;
};

struct HeShenLuoShuParam
{
	HeShenLuoShuParam() { this->Reset(); }

	void Reset()
	{
		memset(heshenluotudata, -1, sizeof(heshenluotudata));
		memset(heshenluotushushengdata, 0, sizeof(heshenluotushushengdata));
	}

	short heshenluotudata[HESHENLUOSHU_MAX_TYPE][HESHENLUOSHU_MAX_SEQ][HESHENLUOSHU_MAX_INDEX];
	HeShenLuoShuHuaShenParam heshenluotushushengdata[HESHENLUOSHU_MAX_TYPE][HESHENLUOSHU_MAX_SEQ];
};

typedef char HeShenLuoShuParamHex[sizeof(HeShenLuoShuParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(HeShenLuoShuParamHex) < 4096);

#pragma pack(pop)
#endif