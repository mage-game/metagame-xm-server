#ifndef __EQUIPMENTMANAGER_H__
#define __EQUIPMENTMANAGER_H__

#include "item/itemgriddata.h"
#include "equipment.h"
#include "servercommon/struct/itemlistparam.h"
#include "obj/character/attribute.hpp"
#include "gamedef.h"
#include "equipment/suitbase.hpp"
#include "protocal/msgequipment.h"

class Role;

class EquipmentManager
{
public:
	enum APPEREFRESHREASON
	{
		APRR_INIT = 1,
		APRR_EXPIRED,
		APRR_PUTON,
		APRR_TAKEOFF,
		APRR_CHANGE_ID,
		APRR_STRENGTH,
		APRR_NAIJIU,
		APRR_GEMSTONE,
		APPR_UPLEVEL,
		APPR_QUALITY,
		APPR_SHENZHU,
		APPR_FULING,
		APPR_UPSTAR,
		APRR_ETERNITY,
	};

	EquipmentManager();
	~EquipmentManager();

	void SetRole(Role *role);

	void Init(Role *role, int param_list_num, const ItemListParam::ItemListData *param_data_list[], ItemID cross_equip_itemid, unsigned int cross_equip_gain_timestamp, EquipmentGridParam equipment_grid_param);
	void GetInitParam(ItemID *cross_equip_itemid, unsigned int *cross_equip_gain_timestamp, EquipmentGridParam *equipment_grid_param);

	void Update(time_t now_second);

	int PutOn(const ItemDataWrapper &item_wrapper, short equip_index);
	int TakeOff(short equip_index, bool is_from_puton = false);
	void UseEternityLevel(short eternity_level);

	void RefreshPutOnInfo(short equip_index, short refresh_reason);

	void SendEquipmentInfoToRole();
	void SendEquipmentInfoToRoleByIndex(short equip_index, short send_reason);
	
	bool TransformToNewEquip(short equip_index, UInt16 equip_id);
	void SetEquipGridDirty(short equip_index, bool to_notice_client = true, short send_reason = Protocol::SCEquipmentItemChange::SEND_REASON_DEFAULT);
	void GridSetBind(short equip_index, bool need_set_dirty);
	void GridClearInvalidTime(short equip_index, ItemID equip_id, bool need_set_dirty);
	
	void ChangeAllItemToOtherSex();

	bool GetEquipmentGridData(short equip_index, ItemGridData *equip_data);
	bool SetEquipmentGridData(short equip_index, ItemGridData *equip_data);

	void GetEquipmentList(int *count, ItemListParam::ItemListData *save_list);
	void GetChangeEquipmentList(int *count, ItemListParam::ItemListData *save_list);
	void ClearDirtyMark();

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalcXianpinAttr(CharIntAttrs &base_add, CharIntAttrs &total_equip_base_add);

	void GetRoleAppearance(RoleAppearance *appearance);

	bool GMPuton(int equip_index, int equip_id);

	void GetEquipMinInfo(int *min_equip_color, int *min_equip_level);

	int GetTotalStrengthen() { return m_total_strengthen_level; }
	int GetTotalStarLevel() { return m_total_up_star_level; }
	int GetMaxStarLevel() { return m_max_up_star_level; }
	int GetMaxStrengthen() { return m_max_strengthen_level; }
	int GetMaxQuality() { return m_max_quality; }
	int GetMaxEquipLevel() { return m_max_equip_level; }
	int GetPutonEquipmentActFlag() { return m_puton_equipment_act_flag; }
	int GetMinStrengthenLevel()	{ return m_min_strengthen_level; }
	int GetmTotalShenZhuLevel() { return m_total_shen_op_level; }
	int GetMinEternityLevel() { return m_min_eternity_level; }

	void OnChangeProf();

	void PutOnCrossEquip(int grid_idx);
	void TakeOffCrossEquip();

	void IsCanActShiZhuang(int *is_act_shengbin, int *is_act_fushi);

	bool OnFumo(short fumo_id);

	bool static GetRandomSpeicalAttr(ItemGridData *item_grid_data);
	bool static GetFixSpecialAttr(ItemGridData *item_grid_data, int getway, int getway_param1 = 0);

	bool AddEquiGridStrengthen(int equip_index, int value);
	bool AddEquiGridUplevel(int equip_index, int value);
	bool AddEquiGridShenzhu(int equip_index);
	bool AddEquiGridStar(int equip_index);
	bool SetEquiGridStarExp(int equip_index, int exp);
	bool AddEquiGridEternityLevel(int equip_index, int value);

	int GetEquipGridStrengthenLevel(int equip_index);
	int GetEquipGridUpLevel(int equip_index);
	int GetEquipGridShenzhuLevel(int equip_index);
	int GetEquipGridStarExp(int equip_index);
	int GetEquipGridStarLevel(int equip_index);
	int GetEquipGridEternityLevel(int equip_index);

	void SendEquipmentGridInfoToRole();
	Attribute EquipXianPinGetValue(int attr_type);
	void NotifyStrengLevel(Role *role);

	bool IsSatisfyMagicalPrecious(int order, int color, int part);
	int GetSatisfyMagicalPrecious2Count(int order, int eternity_level);

	short GetXianPinHightQualityCount(const unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM], bool is_calc = false);

	int GetStatisfyColorAndGradeEquipCount(int color, int order);

	const Equipment * GetEquipBaseInfoByIndex(int equip_index);

	int GetEquipColor(int equip_index);
	int GetEquipNoticeColor(int equip_index);
	int GetEquipOrder(int equip_index);

	void SetItemGridData(int equip_index, ItemGridData grid_data);

	int GetRedEquipCount(int grade);
	int GetEquipCount(int level);

	void CheckFuncOpen();

	void ReCalcEquipUplevelSuitCount();

	void SendEquipGradeSuitInfo();

	int GetEquipNumLevelBigger(int level);

	void ActiveEquipGradeSuit(int level);

private:
	bool AttachAttr(const ItemGridData &item_data, CharIntAttrs &base_add, int puton_level);
	void RefreshEquipInfo();
	void AddEquipmentSkill(const ItemID item_id);
	void DelEquipmentSkill(const ItemID item_id);

	Role			*m_role;

	ItemGridData	m_equipment_list[Equipment::E_INDEX_MAX];
	EquipmentGridParam m_equipment_grid_param;

	bool			m_dirty_mark[Equipment::E_INDEX_MAX];
	bool			m_old_state[Equipment::E_INDEX_MAX];
	bool			m_equipment_is_change;

	CharIntAttrs	m_attrs_add;							// 各属性加成数值

	int				m_total_strengthen_level;				// 全身总强化等级
	int				m_max_strengthen_level;					// 最大强化等级
	int				m_min_strengthen_level;					// 最小强化等级
	int				m_max_quality;							// 最大装备品质
	int				m_max_equip_level;						// 最大装备等级
	int				m_total_shen_op_level;					// 全身神铸总等级
	int				m_total_up_star_level;					// 全身星星总等级
	int				m_max_up_star_level;					// 最大升星等级
	int				m_puton_equipment_act_flag;				// 装备收集标记
	int				m_min_eternity_level;					// 最小永恒等级

	ItemID			m_cross_equip_itemid;					// 跨服装备ID
	unsigned int	m_cross_equip_gain_timestamp;			// 跨服装备获得时间
	unsigned int	m_cross_equip_invalid_timestamp;		// 跨服装备过期时间
	int				m_next_check_equip_fumo_timestamp;		// 下次检查附魔是否过期的时间
	Attribute		m_xianpin_value_list[Equipment::E_XIANPIN_MAX];	// 总仙品数值列表

	int m_equip_level_count[EquipConfig::UPLEVEL_SUIT_COUNT];
	int m_equip_grade_suit_cur_level;						//装备收集套装等级

	int m_equip_grade_suit_active_flag;						//装备等级套装激活标志
};

#endif

