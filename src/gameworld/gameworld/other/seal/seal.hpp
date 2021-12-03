#ifndef __SEAL_H__
#define __SEAL_H__

#include "protocal/msgseal.hpp"
#include "obj/character/attribute.hpp"
#include "obj/character/role.h"
#include "globalconfig/composeconfig.h"

//圣印副本的特殊属性
struct ShengYinFBAttr
{
	ShengYinFBAttr() : yaolichuantou(0), yaolidikang(0), molichuantou(0), molidikang(0), shenlichuantou(0), shenlidikang(0) {}

	void Reset()
	{
		yaolichuantou = 0;
		yaolidikang = 0;
		molichuantou = 0;
		molidikang = 0;
		shenlichuantou = 0;
		shenlidikang = 0;
	}

	int	yaolichuantou;
	int	yaolidikang;
	int	molichuantou;
	int	molidikang;
	int	shenlichuantou;
	int	shenlidikang;
};

class Role;

class Seal
{
public:
	Seal();
	~Seal();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const SealParam &param);
	void GetInitParam(SealParam *param);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void SealOpera(int opera_type, int param1, int param2);
	void SendAllInfo();
	void Recycle(int recycle_num, int *recycle_backpack_index_list);

	bool AddSealByItemId(ItemID item_id, int num, bool is_bind, int level);				// 镶嵌圣印
	bool DecSealByItemId(int item_id, const char *reason, int num = 1, bool is_send = true);
	bool AddScore(int score, const char *reason, bool notify_change = true);		// 增加灵魂

	void GetShengYinFBAttr(ShengYinFBAttr &attr);

	void CheckActivateSealSystem();

	int GetInlaySealCount(int order);			// 获取装备个数

	bool ComposeSeal(int product_num, const ProductConfig *product_cfg);

private:
	void SendBaseInfo() const;
	void SendSlotInfo() const;
	//void SendBackpackInfo();
	//void PutOn(short backpack_index);

	void UpLevel(short slot_index);
	void OnUseSoul(short soul_type, int use_count);

	//bool CheckHasSpace(int count = 1) const;					// 虚拟背包是否有足够空格子
	//void SortBackpack();
	//int GetSealItemNumInVirtualBagById(int item_id);

	Role *m_role;

	SealParam m_param;
	CharIntAttrs m_attrs_add;									// 各属性加成数值

	int m_empty_grids;											// 背包空格子数量
};

#endif