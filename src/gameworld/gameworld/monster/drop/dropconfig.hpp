#ifndef __DROP_CONFIG_HPP__
#define __DROP_CONFIG_HPP__

#include <string>

#include "servercommon/basedef.h"
#include "item/itemextern.hpp"

struct DropItemCfg
{
	DropItemCfg() : prob(0), broadcast(0) {}

	ItemConfigData item_config_data;										// 物品信息
	int prob;																// 物品概率
	short broadcast;														// 传闻类型
};

class DropConfig
{
public:
	static const int DROP_ITEM_PROB_MAX = 64;								// 掉落物品配置个数
	static const int DROP_CERTAIN_PROB = -1;								// 必掉物品的概率 填-1代表必掉落

	DropConfig();
	~DropConfig();

	bool LoadConfig(const char *path, std::string *err);
	
	UInt16 GetDropID() { return m_drop_id; }

	int RandDropItem(char prof, DropItemCfg item_list[DROP_ITEM_PROB_MAX]);
	int GetFixDropItem(int rand_count, DropItemCfg item_list[DROP_ITEM_PROB_MAX]);


	//本掉落ID连续执行N次掉落 优化算法  直接概率相加 超过 概率1直接给一件,不超过执行相加后的概率
	int RandDropItemN(int n, DropItemCfg item_list[DROP_ITEM_PROB_MAX]);



private:
	UInt16 m_drop_id;														// 掉落ID

	int m_drop_item_total_count;											// 全部物品配置个数
	int m_drop_item_prob_count;												// 有概率的物品配置个数
	DropItemCfg m_drop_item_prob_list[DROP_ITEM_PROB_MAX];					// 掉落列表
};

#endif // __DROP_CONFIG_HPP__

