#ifndef __ZODIAC_H__
#define __ZODIAC_H__

#include "protocal/msgzodiac.hpp"
#include "obj/character/attribute.hpp"

class Role;

class Zodiac
{
public:
	Zodiac();
	~Zodiac();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const ZodiacParam &shengqi_param);
	void GetInitParam(ZodiacParam *shengqi_param) const;

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ZodiacOpera(int opera_type, int param1, int param2);
	void SendAllInfo();
	void ZodiacActivate(int backpack_index);
	void ZodiacLevelup(int zodiac_index);
	void ZodiacDecompose(int decompose_num, int *decompose_backpack_index_list);

	bool AddItemsToBackpack(ItemID item_id, int num = 1);		// 增加物品到背包
	bool AddJinghua(int jinghua_num, const char *reason);		// 增加精华

	int CheckActivateZodiacNum(int zodiac_index);

	void GMClearAll();

private:
	void SendBaseInfo();
	void SendZodiacInfo();
	void SendBackpackInfo();

	bool CheckHasSpace(int count = 1) const;					// 虚拟背包是否有足够空格子
	void SortBackpack();

	Role *m_role;

	ZodiacParam m_param;
	CharIntAttrs m_attrs_add;									// 各属性加成数值

	int m_empty_grids;											// 背包空格子数量
};

#endif