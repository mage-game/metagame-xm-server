#ifndef __LIEMING_MANAGER_H__
#define __LIEMING_MANAGER_H__

#include "servercommon/liemingdef.hpp"
#include "obj/character/attribute.hpp"
#include "protocal/msglieming.h"

class Role;

class LieMingManager
{
public:
	LieMingManager();
	~LieMingManager();

	void SetRole(Role *role) { m_role = role; }
	//void Init(Role *role, const LieMingParam &lieming_param, const LieMingHunshouBagParam &hunshou_bag);
	//void GetInitParam(LieMingParam *lieming_param, LieMingHunshouBagParam *bag_param);
	//void GeiInitLieMingParam(LieMingParam *lieming_param);
	void Init(Role *role, const LieMingParam2 &lieming_param2); //new
	void GetInitParam(LieMingParam2 *lieming_param2);
	void GeiInitLieMingParam(LieMingParam2 *lieming_param2);

// 	void AddHunli(int hunli);
// 	int GetHunli() { return m_bag_param.hunli; };

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnFuhun(int slot_idx, int grid_idx);
	void OnTakeoffFuhun(int slot_idx);
	void OnFuhunAddExp(int slot_idx, int target_level);

	bool AddHunshou(int hunshou_id);
	int Chouhun(bool notify_client, int chouhun_type, int *result_color);
	bool SuperChouhun();
	void BatchChouhun();
	void SpecialBatchChouhun();
	void PutHunshouBag(int pool_idx, bool is_notice = true);
	void OnOnekeyPutHunshouBag();
	void OnChouhunPoolConverToExp();
	void OnChouhunPoolSingleItemConverToExp(int pool_idx);
	void OnChouhunBagSingleItemConverToExp(int bag_idx);
	void OnHunshouBagMerge();
	void OneHunshouExchange(short source_index, short dest_index);
	void OnBatchHunshouExchange(Protocol::CSLieMingExchangeList *lmel);

	void SendEquipInfo(int notify_reason = 0);
	void SendBagInfo(int notify_reason = 0);
	void ActiveMinggeSolt(int slot_index);

	void GmAddHunshouExp(int exp);

	//策划需求(命魂改成物品)(new)
	int PutOn(const ItemDataWrapper &item_wrapper);	//装备 命魂
	int TakeOff(short equip_index,bool is_puton = false);					//脱下 命魂
	void UpLevel(short index, bool is_auto = false);						//升级 命魂
	void AddExp(int exp);							//增加 命魂经验
	void AddDecomposeExp(int color, int level);		//分解返还命魂升级经验
	int GetDecomposeExp(int color, int level);		// 获取分解返还命魂升级经验
	short GetFirstEmptyGridIndex();					//获取空的格子索引

	int GetSatisfyMagicalPreciousCount(int order);	// 获取对应品质命魂个数

	void AutoUpLevel();								//自动升级 命魂

protected:

	int GetBagFreeSlot();

	Role *m_role;
	LieMingParam2 m_param;
	LieMingHunshouBagParam m_bag_param;

	int m_liehun_pool_hunshou_count;

	CharIntAttrs m_attrs_add;	

	ItemGridData m_minghun_list[LIEMING_FUHUN_SLOT_COUNT];
};

#endif

