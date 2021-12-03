#ifndef __SHENGQI_PARAM_HPP__
#define __SHENGQI_PARAM_HPP__

#include "servercommon/servercommon.h"

static const int SHENGQI_MAX_NUM = 16;			// 圣器最大数量
static const int SHENGQI_SPIRIT_MAX = 500;		// 圣器附灵上限
static const int SHENGQI_STRENGT_MAX = 120;		// 圣器强化等级上限
static const int SHENGQI_SPIRIT_MAX_NUM = 4;	// 圣器附灵数量

enum SHENGQI_ATTR_TYPE
{
	SHENGQI_ATTR_TYPE_MAXHP = 0,									// 生命
	SHENGQI_ATTR_TYPE_GONGJI = 1,									// 攻击
	SHENGQI_ATTR_TYPE_FANGYU = 2,									// 物防
	SHENGQI_ATTR_TYPE_FA_FANGYU = 3,								// 法防（屏蔽）
	SHENGQI_ATTR_TYPE_BAOJI = 4,									// 暴击力
	SHENGQI_ATTR_TYPE_JIANREN = 5,									// 坚韧（抗暴）
	SHENGQI_ATTR_TYPE_MINGZHONG = 6,								// 命中
	SHENGQI_ATTR_TYPE_SHANBI = 7,									// 闪避
	SHENGQI_ATTR_TYPE_FUJIA_SHANGHAI = 8,							// 附加伤害
	SHENGQI_ATTR_TYPE_DIKANG_SHANGHAI = 9,							// 抵抗伤害
	SHENGQI_ATTR_TYPE_WUXING_GONGJI = 10,							// 五行攻击（屏蔽）
	SHENGQI_ATTR_TYPE_WUXING_FANGYU = 11,							// 五行防御（屏蔽）
	SHENGQI_ATTR_TYPE_PER_BAOJI = 12,								// 暴击率万分比
	SHENGQI_ATTR_TYPE_PER_KANGBAO = 13,								// 抗暴率万分比
	SHENGQI_ATTR_TYPE_PER_POFANG = 14,								// 破防率万分比
	SHENGQI_ATTR_TYPE_PER_MIANSHANG = 15,							// 免伤率万分比
	SHENGQI_ATTR_PVP_JIANSHANG_PER = 16,							// pvp减伤万分比
	SHENGQI_ATTR_PVP_ZENGSHANG_PER = 17,                            // pvp增伤万分比
	SHENGQI_ATTR_PVE_JIANSHANG_PER = 18,                            // pve减伤万分比
	SHENGQI_ATTR_PVE_ZENGSHANG_PER = 19,                            // pve增伤万分比
	SHENGQI_ATTR_SHANGHAI_JIACHENG_PER = 20,                        // 伤害加成万分比（屏蔽）
	SHENGQI_ATTR_SHANGHAI_JIANSHAO_PER = 21,                        // 伤害减少万分比（屏蔽）
	SHENGQI_ATTR_BASE_JIACHENG_PER = 22,							// 基础属性加成万分比

	SHENGQI_ATTR_TYPE_COUNT,										// 圣器属性最大类型
};

#pragma pack(push, 4)

struct ShengqiItem
{
	ShengqiItem()
	{
		this->Reset();
	}

	void Reset()
	{
		level = 0;

		memset(spirit_value, 0, sizeof(spirit_value));
		memset(per_spirit_value, 0, sizeof(per_spirit_value));

		spirit_flag = 0;
		spirit_max_flag = 0;
	}

	bool IsSpiritOpen(int spirit_index)
	{
		return 0 != (spirit_flag & (1 << spirit_index));
	}

	void OpenSpirit(int spirit_index)
	{
		spirit_flag |= (1 << spirit_index);
	}

	bool IsSpiritMax(int spirit_index)
	{
		return 0 != (spirit_max_flag & (1 << spirit_index));
	}

	void MaxSpirit(int spirit_index)
	{
		spirit_max_flag |= (1 << spirit_index);
	}

	short level;										// 等级
	int spirit_value[SHENGQI_SPIRIT_MAX_NUM];			// 附灵属性
	int per_spirit_value[SHENGQI_SPIRIT_MAX_NUM];		// 附灵属性万分比
	short spirit_flag;									// 附灵开启标志
	short spirit_max_flag;								// 附灵满级标志
};

struct ShengqiParam
{
	ShengqiParam() { this->Reset(); }

	void Reset()
	{
		activate_flag = 0;
		for (int i = 0; i < SHENGQI_MAX_NUM; ++i)
		{
			item_list[i].Reset();
		}
	}

	bool IsActivateShengqi()
	{
		return activate_flag != 0;
	}

	bool IsActivateShengqi(int shengqi_index)
	{
		return 0 != (activate_flag & (1 << shengqi_index));
	}

	void ActivateShengqi(int shengqi_index)
	{
		activate_flag |= (1 << shengqi_index);
	}

	int activate_flag;								// 激活开启标志
	ShengqiItem item_list[SHENGQI_MAX_NUM];
};

#pragma pack(pop)

typedef char ShengqiParamHex[sizeof(ShengqiParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShengqiParamHex) < 2048);

#endif
