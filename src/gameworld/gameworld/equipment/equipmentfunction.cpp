#include "equipment/equipmentfunction.h"

#include "equipment/equipment.h"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "obj/character/role.h"

#include "item/knapsack.h"
#include "item/itempool.h"

#include "protocal/msgequipment.h"

#include "engineadapter.h"

#include "other/capability/capability.hpp"

const char * EquipFunction::EquipParamDataClientString(int role_id, ItemID equip_id, bool is_bind, UInt32 invalid_time, const EquipmentParam *equip_param, const EquipBaptizePartInfo *baptize_info)
{
	static char equip_param_string[1024] = {0};
	memset(equip_param_string, 0, sizeof(equip_param_string));

	if (NULL == equip_param)
	{
		SNPRINTF(equip_param_string, sizeof(equip_param_string), "");
	}
	else
	{
		UNSTD_STATIC_CHECK(ItemNamespace::MAX_XIANPIN_NUM >= 6);
		UNSTD_STATIC_CHECK(EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM == 6);
		EquipBaptizePartInfo tmp_baptize_info; tmp_baptize_info.Reset();
		if (NULL != baptize_info)
		{
			memcpy(tmp_baptize_info.attr_seq_list, baptize_info->attr_seq_list, sizeof(tmp_baptize_info.attr_seq_list));
			memcpy(tmp_baptize_info.baptize_list, baptize_info->baptize_list, sizeof(tmp_baptize_info.baptize_list));
		}

		SNPRINTF(equip_param_string, sizeof(equip_param_string), 
			"%d:%d:%d:%d:"
			"%d:%d:%d:"
			"%d:%d:%d:%d:%d:%d:"
			"%d:%d:%d:%d:%d:%d:"
			"%d:%d:%d:%d:%d:%d:%d",
			(int)equip_id, (int)equip_param->strengthen_level, (int)equip_param->quality, (int)equip_param->shen_level,
			(int)equip_param->fuling_level, (int)equip_param->has_lucky, (int)equip_param->star_level,
			(int)equip_param->xianpin_type_list[0], (int)equip_param->xianpin_type_list[1], (int)equip_param->xianpin_type_list[2], (int)equip_param->xianpin_type_list[3], (int)equip_param->xianpin_type_list[4], (int)equip_param->xianpin_type_list[5],
			(int)tmp_baptize_info.attr_seq_list[0], (int)tmp_baptize_info.attr_seq_list[1], (int)tmp_baptize_info.attr_seq_list[2], (int)tmp_baptize_info.attr_seq_list[3], (int)tmp_baptize_info.attr_seq_list[4], (int)tmp_baptize_info.attr_seq_list[5], 
			tmp_baptize_info.baptize_list[0], tmp_baptize_info.baptize_list[1], tmp_baptize_info.baptize_list[2], tmp_baptize_info.baptize_list[3], tmp_baptize_info.baptize_list[4], tmp_baptize_info.baptize_list[5], equip_param->param1);
	}

	return equip_param_string;
}

bool EquipFunction::CanStrength(int equiptype)
{
	return true;
}

bool EquipFunction::CanUpQuality(int equiptype)
{
	return true;
}

bool EquipFunction::CanRefine(int equiptype)
{
	return true;
}
