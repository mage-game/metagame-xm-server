#ifndef __EQUIP_SUIT_H__
#define __EQUIP_SUIT_H__

#include "servercommon/equipsuitdef.hpp"
#include "servercommon/serverdef.h"
#include "obj/character/attribute.hpp"
#include "servercommon/maildef.hpp"

class Role;

class EquipSuit
{
public:

	struct SuitTypeCount 
	{
		SuitTypeCount() : suit_id(0), suit_count(0) {}

		short suit_id;
		short suit_count;
	};

	enum EQUIP_SUIT_LEVEL_TYPE
	{		
		EQUIP_SUIT_LEVEL_TYPE_SHISHI = 1,									// 史诗套装等级
		EQUIP_SUIT_LEVEL_TYPE_CHUANSHUO = 2,								// 传奇套装等级
	};

	UNSTD_STATIC_CHECK(EQUIP_SUIT_LEVEL_TYPE_CHUANSHUO == MAX_EQUIP_SUIT_LEVEL);

	EquipSuit();
	~EquipSuit();

	void SetRole(Role *role);

	void Init(Role *role, const EquipSuitParam &equipsuit_param);
	void GetInitParam(EquipSuitParam *equipsuit_param);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnSuitEquipOperate(short operate_typ, short equip_index);
	void OnUpSuitEquip(int equip_part);
	
	void OnEquipPutOn(int equip_part, ItemID old_equip_id, ItemID new_equip_id);
	void OnEquipTakeOff(int equip_part, ItemID equip_id);

	void SendToClient();

	bool IsSatisfyMagicalPrecious(int order, int equip_part);
	int GetSuitPartCount(int suit_id);

private:
	void ReCalcSuitCount();
	void PutInKnapsack(ItemConfigData item_list[MAX_ATTACHMENT_ITEM_NUM]);

	Role			*m_role;
	CharIntAttrs	m_attrs_add;			
	EquipSuitParam	m_equipsuit_param;

	std::map<short, SuitTypeCount> m_suit_ss_map;					// 史诗套装列表
	std::map<short, SuitTypeCount> m_suit_cq_map;					// 传奇套装列表
};

#endif
