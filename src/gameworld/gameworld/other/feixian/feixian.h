#ifndef __FEIXIANMANAGER_H__
#define __FEIXIANMANAGER_H__

#include "item/itemgriddata.h"
#include "equipment/equipment.h"
#include "servercommon/struct/itemlistparam.h"
#include "obj/character/attribute.hpp"
#include "gamedef.h"
#include "equipment/suitbase.hpp"
#include "protocal/msgequipment.h"
#include "servercommon/feixianparam.hpp"
#include "reallyequipconfig.hpp"

class Role;

class FeixianManager
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
	};
	const static unsigned short STAR_COUNT_LIMIT = 3;

	FeixianManager();
	~FeixianManager();

	void SetRole(Role *role);

	void Init(Role *role, const FeiXianParam &param);
	void GetInitParam(FeiXianParam *param);

	void Update(time_t now_second);

	int GetEquipIndexByType(int equip_type);

	int PutOn(const ItemDataWrapper &item_wrapper);
	int TakeOff(short equip_index, bool is_from_puton = false);
	int ComposeEquipInKnapsack(short index1, short index2);                     // 合成,装备索引index1,index2都在背包
	int ComposeEquipInBody(short index1, short index2);                         // 合成,装备索引index1在身上
	int LevelUpInKnapsack(short index1, int& itemindex);                        // 在背包升级装备
	int LevelUpInBody(short index1);                                            // 在身上升级装备
	bool CheckKnapsack();                                                       // 检查背包是否锁
	int CheckEquipInKnapsack(short index, ItemGridData* item_datas, bool levelup = true);             // 检查装备在背包的情况
	int CheckEquipInBody(short index,bool levelequip = true);                                         // 检查装备在身上的情况
	int LevelUpConsumeItem(ItemID id, const LevelupListConfig* pLevelUpListCfg);                      // 消耗升级物品

	void SyncEquipFeixianInfo();

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnRoleLevelChange();

	bool static GetRandomSpeicalAttr(ItemGridData *item_grid_data);
	bool static GetFixSpecialAttr(ItemGridData *item_grid_data, int getway, int getway_param1 = 0);

	void EquipXianPinFixValue(int attr_type, int &fix_value);

	void ReCalcXianpinTotalStar(int &total_star);

	bool FindFeixianEquipByIndex(short index);
private:
	void ReCalcXianpinAttr(CharIntAttrs &base_add, int puton_level);
	bool AttachAttr(const ItemGridData &item_data, CharIntAttrs &base_add, int puton_level, int &flush_purple_maxhp_count, int &flush_purple_gongji_count);
	void ReCalcXianpinParam(EquipmentParam* param1, EquipmentParam* param2);                // 合成成功后调用，背包里装备1和装备2
	void ReCalcXianpinParamInBody(ItemDataWrapper& param1, EquipmentParam* param2);         // 合成成功后调用，身上装备1和背包装备2
	static bool Compare(const EquipXianPinConfig* a, const EquipXianPinConfig* b);          // set排序比较规则
	const static int XIANPIN_SHUXING_NUM_MAX = 3;                                           // 最大属性条目数量,用于橙装                

	Role			*m_role;

	FeiXianParam m_feixianparam;

	CharIntAttrs	m_attrs_add;							// 各属性加成数值

	Attribute		m_xianpin_value_list[Equipment::E_XIANPIN_MAX];	// 总仙品数值列表
};

#endif

