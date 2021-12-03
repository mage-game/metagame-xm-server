#ifndef __GLOBAL_ROLE_DATA_PARAM_HPP__
#define __GLOBAL_ROLE_DATA_PARAM_HPP__

#include "servercommon/littlepetdef.hpp"
#include "servercommon/lovetreedef.hpp"
#include "servercommon/lovecontractdef.hpp"
#include "servercommon/jinglingdef.hpp"
#include "servercommon/fightingdef.hpp"
#include "servercommon/qingyuandef.hpp"
#include "servercommon/commondataglobaldef.hpp"
#include "servercommon/spousehomedef.hpp"

#include "common/tlvprotocol.h"

static const int GLOBAL_ROLE_DATA_ONCE_LOAD_MAX_COUNT = 80;
const static int GLOBAL_ROLE_CHECK_SAVE_INTERVAL_S = 60;						// 检查保存的时间间隔（秒）

class GlobalRoleDatalListParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct RoleDataAttr
	{
		RoleDataAttr() : id_global_data(0), change_state(0), role_id(0) {}

		long long id_global_data;			// 仅仅初始化的时候有用 
		char change_state;

		int role_id;

		LittlePetData little_pet_param;
		LoveTreeData love_tree_param;
		LoveContractData love_contract_param;
		JingLingHomeParam jing_ling_home_param;
		FightingParam fighting_data;
		QingyuanGlobalParam qingyuan_global_param;
		RoleAppearance role_appearance;
		CommonDataGlobalParam common_data_global_param;
		SpouseHomeParam spouse_home_param;
	};

	GlobalRoleDatalListParam() : count(0) {}

	int count;
	RoleDataAttr role_data_list[GLOBAL_ROLE_DATA_ONCE_LOAD_MAX_COUNT];
};

#endif


