#ifndef __MENTALITY_CONFIG_HPP__
#define __MENTALITY_CONFIG_HPP__

#include <map>

#include "servercommon/mentalitydef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "obj/character/attribute.hpp"

// 精炼：经脉、根骨和元神
// 新版：星脉、灵宫和星象

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct MentalityBaseItemConfig																				// 一个等级的 所有元神配置
{
	MentalityBaseItemConfig() : rolelevel_limit(0), need_yuanli(0), cd_time(0) {}

	int rolelevel_limit;																					// 限制角色等级
	int need_yuanli;																						// 需要元力
	int cd_time;																							// 冷却CD

	CharIntAttrs::AttrPair all_attrlist[MENTALITY_TYPE_MAX];												// 所有元神的属性 
};

struct MentalityGenguItemConfig																				// 根骨配置
{
	MentalityGenguItemConfig() : protect_level(0), succ_rate(0), need_coin(0), upgrade_itemid(0), protect_item_id(0), protect_item_num(0) {}

	int protect_level;																						// 保护等级
	int succ_rate;																							// 升根骨成功概率
	int need_coin;																							// 升根骨所需铜币
	ItemID upgrade_itemid;																					// 根骨提升符

	ItemID protect_item_id;
	int protect_item_num;

	CharIntAttrs::AttrPair all_attrlist[MENTALITY_TYPE_MAX];												// 所有根骨的属性 根骨同元神类型
};

struct MentalityOtherConfig 
{
	MentalityOtherConfig() : clear_cd_cost(0) {}

	int clear_cd_cost;																						// 清楚CD花费
};

const static int MENTALITY_WUXING_MAX_LEVEL = 12;															// 五行最高等级

struct MentalityWuxingConfig
{
	MentalityWuxingConfig() : wuxing_index(0), wuxing_level(0), attr_val(0), need_total_mentality_level(0), uplevel_stuff_id(0),
		uplevel_stuff_num(0), succ_rate(0), reduce_rate(0) {}

	int wuxing_index;
	int wuxing_level;
	int attr_val;
	int need_total_mentality_level;
	int uplevel_stuff_id;
	int uplevel_stuff_num;
	int succ_rate;
	int reduce_rate;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MentalityConfig : public ConfigBase
{
public:
	MentalityConfig();
	~MentalityConfig();

	bool Init(const std::string &path, std::string *err);													// 读取配置文件

	const MentalityBaseItemConfig * GetTrainMentalityBaseItem(int mentality_level);							// 获取元神升级消耗
	const MentalityGenguItemConfig * GetUpgradeMentalityGenguItem(int gengu_level);							// 获取根骨升级消耗		

	void GetMentalityBaseAttr(int mentality_type, int mentality_level, CharIntAttrs &base_add, int prof) const;				// 增加特定元神等级的加成属性

	void GetGenguBaseAttr(int gengu_type,int gengu_level, CharIntAttrs &base_add, int prof) const;								//增加特定的根骨等级加成属性

	int GetMentalityCDIntervalPerGold() { return CLEAR_CD_INTERVAL_PER_GOLD; }								// 获取单元元宝缩短元神修炼冷却时间
	
	bool IsGenguLevelBoardcast(int gengu_level);															// 判断根骨等级是否广播
	
	void GetMentalityTitleAttr(CharIntAttrs &base_add, int least_mentality_level, int prof);							// 增加元神称号加成属性
	void GetGenguTitleAttr(CharIntAttrs &base_add, int least_gengu_level, int prof);									// 增加根骨称号加成属性

	const MentalityOtherConfig & GetOtherCfg() { return m_other_cfg; }										// 其他配置

	const MentalityWuxingConfig * GetWuxingCfg(int wx_index, int wx_level);									// 五行配置

private:
	struct MentalityTitleConfig																				// 元神称号奖励
	{
		MentalityTitleConfig() : least_mentality_level(0) {}

		int least_mentality_level;	
		CharIntAttrs::AttrPair mentality_attrlist[MENTALITY_TYPE_MAX];
	};

	struct GenguTitleConfig																					// 根骨称号
	{
		GenguTitleConfig() : least_gengu_level(0) {}

		int least_gengu_level;
		CharIntAttrs::AttrPair gengu_attrlist[MENTALITY_TYPE_MAX];
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	MentalityBaseItemConfig m_mentality_base_config_list[MAX_MENTALITY_BASE_LEVEL + 1];						// 0-160级元神配置
	MentalityGenguItemConfig m_mentality_gengu_config_list[MAX_MENTALITY_GENGU_LEVEL + 1];					// 0-30级根骨升级设定 

	int m_mentality_title_count;																			// 元神称号奖励列表长度
	MentalityTitleConfig m_mentality_title[MAX_MENTATITY_TITLE_COUNT_MAX];									// 元神称号奖励列表

	int m_gengu_title_count;																				// 根骨称号奖励列表长度
	GenguTitleConfig m_gengu_title[MAX_GENGU_TITLE_COUNT_MAX];												// 根骨称号奖励列表

	MentalityOtherConfig m_other_cfg;																		// 其他配置

	MentalityWuxingConfig m_wuxing_cfglist[MENTALITY_WUXING_MAX_COUNT][MENTALITY_WUXING_MAX_LEVEL + 1];		// 五行配置

};

#endif // __MENTALITY_CONFIG_HPP__

