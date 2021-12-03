#ifndef __RUNE_SYSTEM_HPP__
#define __RUNE_SYSTEM_HPP__

#include <map>
#include <set>

#include "protocal/msgrunesystem.h"
#include "obj/character/attribute.hpp"
#include "other/runesystem/runesystemconfig.hpp"

class Role;

enum RUNE_SYSTEM_AWAKEN_TYPE
{
	RUEN_AWAKEN_TYPE_COMMON = 0,
	RUEN_AWAKEN_TYPE_DIAMOND,
};

enum RUNE_SYSTEM_ERROR_TYPE
{
	RUNE_SYSTEM_ERROR_TYPE_GRID_INVALID = 0,					//格子无效
	RUNE_SYSTEM_ERROR_TYPE_TOWER_LAYER_NOT_ENOUGH = 1,			//符文塔未达到指定层数
	RUNE_SYSTEM_ERROR_TYPE_MAX_LEVEL = 2,						//符文已达到最大等级
	RUNE_SYSTEM_ERROR_TYPE_NOT_ENOUGH_SCORE = 3,				//战魂精华不足
};

class RuneSystem
{

public:
	RuneSystem();
	~RuneSystem();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const RuneSystemParam &param);
	void GetInitParam(RuneSystemParam *param) const;

	void OnRoleLogin(unsigned int last_save_time);
	void Update(unsigned int now_second);
	void OnDayChange(int old_dayid, int now_dayid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalRuneEffct();
	void ReCalcSpecialRoleBigGoalAttr();        // 激活人物大目标属性加成   只算装备的符文，不算符文等级

	void OperaationReq(const Protocol::CSRuneSystemReq *req);							// 操作请求

	//////////////////////////////////////////////////////////////////////////////////////
	
	void ReqSetRune(int virtual_index, int grid_index);		// 装备符文
	void ReqUpRuneLevel(int grid_index);					// 升级符文
	void ReqConvert(ItemID rune_id);						// 符文兑换
	void ReqComposeRune(int index1, bool index1_is_bag, int index2, bool index2_is_bag);// 符文合成
	void ReqAwaken(int grid_index, int awaken_type);		// 符文觉醒
	void ReqRecaleAwaken();									// 重算符文觉醒属性

	int GetRuneLevelByJingHua(int quality, int type, int jinghua);						// 精华重算符文等级

	void ReqXunBaoOneTimes(bool is_auto_buy);				// 寻宝一次
	void ReqXunBaoTenTimes(bool is_auto_buy);				// 寻宝十次

	int GetRuneWeightType(int total_times);																	// 获取寻宝权重类型
	int GetRuneRealWeight(const RuneSystemRuneFetchCfg &rune_weight_cfg, int total_times, bool is_ten);		// 获取权重值

	void DisposeOne(int virtual_index);						// 分解一个
	void DisposeOneKey(Protocol::CSRuneSystemDisposeOneKey *req);	// 一键分解

	bool IsCanUpLevelRune(RuneParam &rune);					// 是否可以升级符文
	void CheckOpenRuneSlotAndId();							// 检查符文槽及符文id开启
	bool IsOpenAwaken();									// 检查是否开启觉醒
	bool IsOpenLianhun();									// 检查是否开启炼魂

	double GetSpecialEffectValuPercentByType(int effect_type);

	Attribute RecalcFangJuAddMaxHp(Attribute fangju_maxhp);
	Attribute RecalcFangJuAddFangyu(Attribute fangju_fangyu);
	Attribute RecalcFangJuAddShanbi(Attribute wuqi_pojia);
	Attribute RecalcFangJuAddJianren(Attribute wuqi_pojia);
	Attribute RecalcWuQiAddGongJi(Attribute wuqi_gongji);
	Attribute RecalcEquipAddGongJi(Attribute equip_base_gongji);
	Attribute RecalcWuQiAddBaoji(Attribute wuqi_baoji);
	Attribute RecalcShouShiAddGongji(Attribute shoushi_gongji);
	Attribute RecalcShouShiAddBaoji(Attribute shoushi_baoji);
	
	void SendAllInfo();

	void GmOpera(int type, int p1, int p2, int p3, int p4);

	bool AddRuneByItemId(ItemID item_id, int num);			// 根据符文id加符文
	bool UseRuneBox(int box_type, int level_limit);			// 使用符文宝箱
	int GetBagSpaceNum() { return m_empty_grids; }
	int GetRuneQualityByItemId(ItemID item_id);

	void AddRuneJingHua(int jinghua);						// 加符文精华

	void PutRuneTowerRewardByItemId(ItemID item_id);		// 符文塔通关奖励符文

	void OnRuneTowerFetchTime();
	void OnRuneTowerAutoFb();
	int GetRuneTowerPassLayer() { return m_param.fb_pass_layer; }
	void OnRuneTowerPassLayer(int pass_layer);
	int GetRuneTowerTodayLayer() { return m_param.fb_today_layer; }
	short AddRuneTowerOfflineTime(int offline_time, short max_count);
	void OnRuneTowerOfflineTime(unsigned int last_save_time);
	void SendRuneTowerInfo();

	void GMSetRuneTower(int pass_layer, int today_layer);
	void SendComposeInfo();

	void RandZhulingSlot(int rune_index);
	void Zhuling(int rune_index);
	void SendZhulingResultInfo(int index);
	void SendZhulingAllInfo();
	void OnRoleLevelChange(int level);
	void OnCompleteTask(TaskID task_id);
	void SendVirtualBagAllInfo();
private:

	void PutRuneListToVirtualBag(const RuneParam rune[], int size, int magic_crystal, int info_type, const char *reason);	// 添加物品到虚拟背包
	void PutRuneToVirtualBag(const RuneParam &rune, int magic_crystal, int info_type, const char *reason);					// 添加物品到虚拟背包

	static bool IsInvalidVirtualIndex(int virtual_index) { return virtual_index < 0 || virtual_index >= RUNE_SYSTEM_BAG_MAX_GRIDS; }// 检查虚拟背包索引是否非法
	static bool IsInvalidGridIndex(int grid_index);// 检查虚拟格子索引是否非法

	bool CheckHasSpace(int count = 1) const { return m_empty_grids >= count; }				// 虚拟背包是否有足够空格子
	void SortVirtrulBag();																	// 排序

	static bool TransformAttrType(int type, int &real_type);								// 获取基础属性类型
	bool CheckIsActiveSpecialEffect(int effect_type);										// 检查是否激活某个特殊效果
	void ActiveSpecialEffectByRuneType(int type, double value, int add_per, int fuwen_index);
	void ActiveSpecialEffect(int effect_type, double value, int add_per, int fuwen_index);
	short GetSpecialEffectByRuneType(int type);												// 获取效果类型
	void AddSpecialEffect(int effect_type, double value);									// 增加特殊效果数值							
	void NotActiveSpecialEffect(int effect_type);

	void SendRuneGridAllInfo();
	void SendOtherInfo();
	void SendAwakenInfo(short seq, short is_need_recalc);
	void CheckFuncOpen();
	void CheckGoalProgress(int quality);           // 大小目标进度

	Role *m_role;
	RuneSystemParam m_param;
	CharIntAttrs m_attrs_add;
	int m_empty_grids;														// 背包空格子数量
	std::map<short, double>  m_special_buff_list;

	std::set<ItemID> m_rune_id_set;											// 记录已开启的符文id

	std::vector<RuneSystemRuneFetchCfg> m_rune_fetch_weight_vec;			// 符文获取权重配置

	int m_other_capability;
};

#endif