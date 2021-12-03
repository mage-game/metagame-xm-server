#ifndef __MENTALITY_DEF_HPP__
#define __MENTALITY_DEF_HPP__

#include <string.h>
#include "servercommon/servercommon.h"

enum MENTALITY_TYPE
{
	MENTALITY_TYPE_MAXHP = 0,										// 气血
	MENTALITY_TYPE_GONGJI,											// 攻击
	MENTALITY_TYPE_FANGYU,											// 防御
	MENTALITY_TYPE_MINGZHONG,										// 命中
	MENTALITY_TYPE_SHANBI,											// 闪避
	MENTALITY_TYPE_FUJIA_SHANGHAI,									// 附加伤害
	MENTALITY_TYPE_DIKANG_SHANGHAI,									// 抵抗伤害

	MENTALITY_TYPE_MAX,												
};

static const int MAX_MENTALITY_BASE_LEVEL = 100;					// 元神最大等级 
static const int MAX_MENTALITY_GENGU_LEVEL = 50;					// 根骨最大等级

static const int MAX_MENTATITY_TITLE_COUNT_MAX = 10;				// 元神称号最大数量
static const int MAX_GENGU_TITLE_COUNT_MAX = 6;						// 根骨称号最大数量

static const int MAX_MENTALITY_UPGRADE_ONCE = MENTALITY_TYPE_MAX;	// 元神一键提升最大提升次数
static const int CLEAR_CD_INTERVAL_PER_GOLD = 600;					// 单位元宝缩短 元神修炼冷却时间(s)


#pragma pack(push, 4)

const static int MENTALITY_WUXING_MAX_COUNT = 35;
static const int MENTALITY_SHUXINGDAN_MAX_TYPE = 3;

struct MentalityItem
{
	MentalityItem() : mentality_level(0), gengu_level(0), gengu_max_level(0), reserve_1(0) {}

	short mentality_level;											// 元神等级
	short gengu_level;												// 根骨等级
	short gengu_max_level;											// 历史最大 根骨等级 
	short reserve_1;											

	bool Invalid() { return 0 == mentality_level; }
	bool GenguInvalid() { return 0 == gengu_level; }
};

struct MentalityParam
{
	MentalityParam() { this->Reset(); }

	void Reset()
	{
		train_cd_next_time = 0;
		curr_trained_mentality_type = 0;
		flag = 0;
		reserved1 = 0;
		reserved2 = 0;

		memset(mentality_list, 0, sizeof(mentality_list));
		memset(reserve_sh_list, 0, sizeof(reserve_sh_list));
		memset(wuxing_level_list, 0, sizeof(wuxing_level_list));
		memset(shuxingdan_list, 0, sizeof(shuxingdan_list));
	}

	enum
	{
		FLAG_CLEAR_CD = 0X1,
	};

	unsigned int train_cd_next_time;								// 元神训练完成时间戳	
	short curr_trained_mentality_type;								// 当前训练元神类型
	short flag;												
	long long reserved1;
	long long reserved2;

	MentalityItem mentality_list[MENTALITY_TYPE_MAX];				// 元神根骨等级

	UNSTD_STATIC_CHECK(35 == MENTALITY_WUXING_MAX_COUNT);

	short reserve_sh_list[5];
	short wuxing_level_list[MENTALITY_WUXING_MAX_COUNT];			// 五行等级

	int shuxingdan_list[MENTALITY_SHUXINGDAN_MAX_TYPE];				// 属性丹列表
};

typedef char MentalityParamHex[sizeof(MentalityParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MentalityParamHex) < 512);

#pragma pack(pop)

#endif  // __MENTALITY_DEF_HPP__

