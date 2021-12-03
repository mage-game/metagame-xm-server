#ifndef __LITTLE_HELPER_CONFIG_H__
#define __LITTLE_HELPER_CONFIG_H__

#include <map>
#include <vector>
#include <string>

#include "servercommon/rolelotterycountdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class Role;

namespace lhns						//little helper name space
{
	typedef short					complete_type_t;		//完成类型
	typedef short					seq_t;					//序列
	typedef short					opengame_day_t;			//开服天数
	typedef short					level_t;				//等级
	typedef int						money_type_t;			//货币类型
	typedef Int64					gold_t;					//元宝
	typedef unsigned short			monster_id_t;			//怪物id
	typedef UInt16					drop_id_t;				//掉落id
	typedef std::vector<drop_id_t>	drop_id_vec_t;
	typedef long long				exp_t;					//经验
	typedef int						prestige_t;				//声望
	typedef int						cross_honor_t;			//跨服荣誉

	enum CONSUME_TYPE
	{
		CONSUME_TYPE_BIND_GOLD = 0,							//绑元
		CONSUME_TYPE_GOLD = 1,								//元宝

		CONSUME_TYPE_MAX
	};

	const static int DROP_LIST_MAX_COUNT = 128;				//掉落列表
	const static int MAX_DROP_ITEM_COUNT = 400;				//最大掉落物品数量，协议里面用
}


struct LittleHelperCfg
{
	LittleHelperCfg() : opengame_day(0), level_left(0), level_right(0), money_type(0), gold(0), monster_id(0), exp(0), prestige(0), cross_honor(0)
	{
	}
	lhns::opengame_day_t		opengame_day;
	lhns::level_t				level_left;
	lhns::level_t				level_right;
	TaskID						task_id;
	lhns::money_type_t			money_type;
	lhns::gold_t				gold;
	lhns::monster_id_t			monster_id;
	lhns::drop_id_vec_t			drop_id_vec;
	lhns::exp_t					exp;
	lhns::prestige_t			prestige;
	lhns::cross_honor_t			cross_honor;
	ItemConfigData				reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

class LittleHelperConfig : public ConfigBase
{
public:
	LittleHelperConfig();
	~LittleHelperConfig();

	bool Init(const std::string &configname, std::string *err);

	const LittleHelperCfg * GetHelperCfg(lhns::complete_type_t type, lhns::seq_t seq);
	const LittleHelperCfg * GetHelperCfgByLevel(lhns::complete_type_t type, lhns::level_t level);
	size_t GetLittleHelperCfgVecSize(lhns::complete_type_t type);
private:
	int InitHelperCfg(PugiXmlNode RootElement);

	typedef std::vector<LittleHelperCfg> LittleHelperCfgVec;
	typedef std::map<int, LittleHelperCfgVec> LittleHelperCfgVecInMap;
	LittleHelperCfgVecInMap m_little_helper_cfg_vec_in_map;
};


#endif // __LITTLE_HELPER_CONFIG_H__

