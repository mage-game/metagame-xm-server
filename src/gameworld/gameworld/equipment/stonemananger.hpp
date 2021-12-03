#ifndef _STONEMANAGER_HPP_
#define _STONEMANAGER_HPP_


#include "item/itemgriddata.h"
#include "equipment.h"
#include "servercommon/struct/itemlistparam.h"
#include "obj/character/attribute.hpp"
#include "gamedef.h"
#include "servercommon/stonedef.hpp"
#include "globalconfig/equipconfig.h"


class Role;

class StoneManager
{
public:
	StoneManager();
	~StoneManager();

	void SetRole(Role *role);
	void Init(Role *role, const StoneParam &stone_param);
	void GetInitParam(StoneParam* stone_param);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	
	bool IsRepeatInlay(int equip_part, int stone_type);
	bool InlayStone(int equip_part, int stone_slot, const int index_in_bag);
	bool UnInlayStone(int equip_part, int stone_slot);
	bool Uplevelstone(int equip_part,int stone_slot, int uplevel_type);

	void SendToClient(short reason = 0);

	int GetTotalStoneLevel() { return m_total_stone_level; }
	int GetMaxStoneLevel() { return m_max_stone_level; }

	StonesOnPart GetStonePartByEquipType(int equip_type);
	StonesOnPart GetStonePartByEquipIndex(int equip_index);

	bool IsSatisfyMagicalPrecious(int stone_level, int stone_type);
	bool IsInlayStoneByLevel(int stone_level);
	void CheckFuncOpen();

private:

	short GetStoneLimitFlag(int equip_index);

	Role *m_role;

	CharIntAttrs m_attrs_add;

	StoneParam m_stone_param;

	int m_total_stone_level;
	int m_max_stone_level;
	int m_system_msg_time;
};

#endif
