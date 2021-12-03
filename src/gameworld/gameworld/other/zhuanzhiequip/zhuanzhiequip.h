#ifndef __ZHUANZHI_EQUIP_H__
#define __ZHUANZHI_EQUIP_H__

#include "item/itemgriddata.h"
#include "equipment/equipment.h"
#include "servercommon/struct/itemlistparam.h"
#include "obj/character/attribute.hpp"
#include "gamedef.h"
#include "equipment/suitbase.hpp"
#include "protocal/msgequipment.h"
#include "servercommon/zhuanzhiequipdef.hpp"
#include "zhuanzhiequipconfig.hpp"
#include "protocal/msgzhuanzhiequip.hpp"

#include <set>

class Role;

class ZhuanzhiEquip
{
public:
	ZhuanzhiEquip();
	~ZhuanzhiEquip();

	void SetRole(Role *role);

	void Init(Role *role, const ZhuanzhiEquipParam &param);
	void GetInitParam(ZhuanzhiEquipParam *param);
	void OnInitFinish();

	void Update(time_t now_second);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalcAttrConditionAttr(CharIntAttrs &base_add);

	int GetEquipIndexByType(int equip_type);												// 获取装备索引

	void OnOperaReq(int opera_type, int param1, int param2, int param3, int param4, int param5);// 操作请求

	int PutOn(const ItemDataWrapper &item_wrapper);											// 穿上
	int TakeOff(short equip_index, bool is_from_puton = false);								// 脱下

	bool CheckKnapsack();																	// 检查背包是否锁
	bool IsCanPuton(int equip_part, int equip_order, ItemID item_id);

	void ReCalZhuanZhiEquipTotalLevel();
	void RefreshEquipInfo();																// 刷新装备信息

	void GetEquipSpecialAttr(CharIntAttrs &tmp_attr, const CharIntAttrs &base_equip_attr);	// 获取装备特殊属性
	const Equipment *GetZhuanzhiEquip(ItemID item_id);										// 获取装备

	void OnRoleLevelChange();

	const ItemDataWrapper *GetEquipmentGridData(short equip_index);

	//升星附灵相关---------------------------------------------------------------------------------------------
	bool EquipUpStar(short equip_index);													// 升级星星
	bool OnFuling(int equip_index);															// 附灵

	int GetEquipTotalStarLevel();															// 获取总星星等级
	int GetEquipStarLevel(int equip_index);													// 获取星星等级
	void AddEquipStarLevel(int equip_index);												// 增加星星等级
	int GetEquipStarExp(int equip_index);													// 获取星星经验
	void SetEquipStarExp(int equip_index, int exp);											// 增加星星经验

	void SyncZhuanzhiEquipInfo();

	//玉石相关---------------------------------------------------------------------------------------------
	void OnInlayStone(int part_index, int slot_index, int bag_index);						// 镶嵌玉石
	bool OnUnInlayStone(int part_index, int slot_index, bool is_from_inlay = false);		// 卸下玉石
	void OnStoneLevelUp(int part_index, int slot_index);									// 升级玉石
	bool UpRefineLevel(int part_index, int seq, int is_autobuy);							// 精炼玉石
	void OnStoneResolve(int bag_index);														// 玉石分解
	void OnStoneConvert(int seq);															// 玉石兑换
	bool OnComsumeStoneScore(int consume_score);											// 玉石精粹消耗

	void CheckAllPartChangeSlot(int open_type, int part_index = 0);							// 检查开启/关闭槽
	void CheckPutBackStone(int part_index, int slot_index);									// 检查返还玉石

	int GetTotalStoneLevel();																// 获取总等级
	int GetZhuanzhiEquipOrder(int part_index);												// 获取装备阶数
	int GetStoneScore() { return m_param.stone_score; }										// 获取玉石精粹

	bool IsCanInlayStone(int part_index, int slot_index, int stone_id);						// 判断能否镶嵌

	void SyncStoneInfo();

	//套装相关---------------------------------------------------------------------------------------------
	void OnForgeZhuanzhiSuit(int suit_index, int equip_index);								// 锻造转职套装

	void CheckPutBackStuff(int equip_index, int equip_order);								// 检查返还锻造材料
	int ReCalcSameOrderCount(int suit_index, int group_type, int order = 0);				// 计算相同阶数数量

	void GetSuitTotalAttr(CharIntAttrs &tmp_attr);

	void SyncZhuanzhiSuitInfo();

	//至尊装备相关---------------------------------------------------------------------------------------------
	void OnZhiZunEquipCompose(ItemID compose_id, int best_attr_num, int equip_index, int bag_index1, int bag_index2);	// 至尊装备合成

	bool IsCanCompose(int equip_type, int index);											// 检查背包装备能否合成
	int GetZhizunEquipIndexList(int *index_list);											// 获取至尊装备索引

	int GetZhiZunEquipOtherCap();															// 增加至尊装备额外战力

	//效果触发---------------------------------------------------------------------------------------------
	void CheckActiveEquipEffect();															// 检查效果激活

	void OnHurtOther(Character *target, long long base_injure, long long &injure);			// 攻击

	void OnHurtOtherZhiZunEffect(Character *target, long long &injure);						// 至尊装备效果
	void OnHurtOtherUpGongji(Character * target);											// 增加攻击力
	void OnHurtOtherPVPUpShanghai(Character *target, long long base_injure);				// 增加对玩家伤害
	void OnHurtOtherPVEUpShanghai(Character *target, long long base_injure);				// 增加对怪物伤害

	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);	// 被攻击

	void OnBeendAttackDownShanghai(ObjID attacker, Attribute &delta_hp);					// 减伤
	void OnBeendAttackPVPDownShanghai(ObjID attacker, Attribute &delta_hp);					// 减少玩家对自己伤害
	void OnBeendAttackPVEDownShanghai(ObjID attacker, Attribute &delta_hp);					// 减少怪物对自己伤害

	//仙品属性相关-------------------------------------------------------------------------------------------
	Attribute EquipXianPinFixValue(int attr_type);
	void ReCalcXianpinTotalStar(int &total_star);

	//----------------------------------------------------------------------------------------------------------
	int GetSatisfyPutonCount(int order, int color);											// 获取满足条件的穿戴个数
	int GetSuitTypeCount(int suit_index);													// 获取套装类型数量																		
	bool HasSuit(int part_index);															// 该部位是否已穿戴																		

	void PrintLog(const char *reason, int part_index, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0);

	int GetEquipNoticeColor(int equip_index);

	//觉醒相关-------------------------------------------------------------------------------------------
	void ReCalcAttrAwakening(CharIntAttrs &base_add);										// 计算觉醒属性加成
	void Wake(int part_index, int is_auto_buy);												// 觉醒
	void CoverAwakening(int part_index);													// 覆盖觉醒(替换)
	void SyncAwakeningAllInfo();															// 发送觉醒所有信息
	void SyncAwakeningInfo(int part_index);													// 发送觉醒单个信息
	bool LockAwakening(int part_index, int lock_index);										// 加锁
	bool UnLockAwakening(int part_index, int lock_index);									// 解锁
	bool IsLockAwakening(int part_index, int lock_index);									// 是否锁定
	unsigned int GetLockAwakeningNum(int part_index);										// 获取锁定数量
	wake_level_t GetAwakeningLevel(int part_index, int lock_index);							// 获取觉醒等级
	wake_level_t GetAwakeningEquipLevel(int part_index, int lock_index);					// 获取觉醒装备等级
	wake_type_t GetAwakeningType(int part_index, int lock_index);							// 获取觉醒类型
	wake_type_t GetAwakeningEquipType(int part_index, int lock_index);						// 获取觉醒装备类型
	bool IsAwakeningMaxLevel(int part_index, int lock_index);								// 是否觉醒满级
	void WakeRumor(int part_index);															// 觉醒传闻
	//---------------------------------------------------------------------------------------------------

	void ComposeZhuanzhiEquip(const Protocol::CSZhuanzhiEquipCompose *req);
	void SyncComposeSucceed(bool is_succeed);
	bool IsCanComposeZhuanzhi(const Equipment *compose_equip, int index);					// 检查背包装备能否合成
	bool IsZhuanzhiEquipType(int equip_type);												// 是否转职装备类型
	int GetXianpinCountByBag(short bag_index);												// 获取背包中装备的仙品条数，根据背包格子索引
	int GetXianpinCount(int part_index);													// 获取身上的仙品条数

	int GetForgeZhuanzhiNum() const;														//获取锻造装备的件数
private:
	int GetEquipColor(int equip_index);
	void ReCalcXianpinAttr(CharIntAttrs &base_add, int puton_level);
	bool AttachAttr(const ItemGridData &item_data, CharIntAttrs &base_add, int puton_level, int &flush_purple_maxhp_count, int &flush_purple_gongji_count);               

	//觉醒相关-------------------------------------------------------------------------------------------
	bool WakeConsume(int part_index, int is_auto_buy);										// 觉醒消耗
	bool WakeBase(int part_index, int lock_index);											// 觉醒的逻辑
	bool ChangeAwakeningLevel(int part_index, int lock_index);								// 改变觉醒等级的逻辑处理
	bool ChangeAwakeningType(int part_index, int lock_index);								// 改变觉醒类型的逻辑处理
	bool AddAwakeningLevelBase(int part_index, int lock_index, wake_level_t level);			// 实际增加等级
	bool DecAwakeningLevelBase(int part_index, int lock_index, wake_level_t level);			// 实际减少等级
	bool SetAwakeningTypeBase(int part_index, int lock_index, wake_type_t type);			// 设置觉醒类型
	//---------------------------------------------------------------------------------------------------


	Role			*m_role = nullptr;
	ZhuanzhiEquipParam m_param;
	CharIntAttrs	m_attrs_add;								// 各属性加成数值

	int m_total_up_star_level = 0;								// 全身星星总等级
	int m_max_up_star_level = 0;								// 最大升星等级
	int m_total_stone_level = 0;								// 全身玉石总等级

	int m_special_attr_pvp_jianshang = 0;						// 装备特殊属性pvp减伤 
	int m_special_attr_pvp_zengshang = 0;						// 装备特殊属性pvp增伤
	int m_special_attr_pve_jianshang = 0;						// 装备特殊属性pve减伤
	int m_special_attr_pve_zengshang = 0;						// 装备特殊属性pve增伤

	int m_other_capability = 0;									// 其他虚拟战力

	Attribute	m_xianpin_value_list[Equipment::E_XIANPIN_MAX];			// 总仙品数值列表

	std::set<int> m_active_effect_type_set;						// 已激活效果列表
};

//-------------------------------------------------------------------------------------------
inline int ZhuanzhiEquip::GetEquipStarLevel(int equip_index)
{
	if (equip_index < 0 || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX) return 0;

	return m_param.zhuanzhi_equip_list[equip_index].star_level;
}

inline void ZhuanzhiEquip::AddEquipStarLevel(int equip_index)
{
	if (equip_index < 0 || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX) return;

	m_param.zhuanzhi_equip_list[equip_index].star_level += 1;
}

inline int ZhuanzhiEquip::GetEquipStarExp(int equip_index)
{
	if (equip_index < 0 || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX) return 0;

	return m_param.zhuanzhi_equip_list[equip_index].star_exp;
}

inline void ZhuanzhiEquip::SetEquipStarExp(int equip_index, int exp)
{
	if (equip_index < 0 || equip_index >= E_INDEX_ZHUANZHI::E_INDEX_MAX || exp < 0) return;

	m_param.zhuanzhi_equip_list[equip_index].star_exp = exp;
}
//-------------------------------------------------------------------------------------------

#endif

