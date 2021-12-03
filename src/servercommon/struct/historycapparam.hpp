
#ifndef __HISTORY_CAP_PARAM_HPP__
#define __HISTORY_CAP_PARAM_HPP__

#pragma pack(push) 
#pragma pack(4)

enum HISTORY_CAPABILITY_TYPE
{
	HISTORY_CAPABILITY_TYPE_TAOTAL = 0,			// 总战力
	HISTORY_CAPABILITY_TYPE_EQUIPMENT,			// 装备属性战斗力
	HISTORY_CAPABILITY_TYPE_XIANNV,				// 仙女属性战斗力
	HISTORY_CAPABILITY_TYPE_MENTALITY,			// 元神属性战斗力
	HISTORY_CAPABILITY_TYPE_MOUNT,				// 坐骑属性战斗力
	HISTORY_CAPABILITY_TYPE_WING,				// 羽翼属性战斗力
	HISTORY_CAPABILITY_TYPE_SHENZHUANG,			// 神装属性战斗力
	HISTORY_CAPABILITY_TYPE_JINGLING,			// 精灵属性战斗力
	HISTORY_CAPABILITY_TYPE_WASH,				// 洗练属性战斗力
	HISTORY_CAPABILITY_TYPE_FEIXIAN,			// 飞仙属性战斗力

	HISTORY_CAPABILITY_TYPE_MAX,		
};

struct HistoryCapItem
{
	HistoryCapItem() { this->Reset(); }

	void Reset() { max_cap = 0; reach_timestamp = 0; }

	int max_cap;
	unsigned int reach_timestamp;
};

struct HistoryCapParam
{
	HistoryCapParam() { this->Reset(); }

	void Reset() 
	{
		for (int i = 0; i < HISTORY_CAPABILITY_TYPE_MAX; ++ i)
		{
			history_cap_list[i].Reset();
		}
	}

	HistoryCapItem history_cap_list[HISTORY_CAPABILITY_TYPE_MAX];
};

#pragma pack(pop)

typedef char HistoryCapParamHex[sizeof(HistoryCapParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(HistoryCapParamHex) < 256);

#endif  // __HISTORY_CAP_PARAM_HPP__


