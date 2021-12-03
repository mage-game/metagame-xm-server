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

	bool AddItemsToBackpack(ItemID item_id, int num = 1);		// ������Ʒ������
	bool AddJinghua(int jinghua_num, const char *reason);		// ���Ӿ���

	int CheckActivateZodiacNum(int zodiac_index);

	void GMClearAll();

private:
	void SendBaseInfo();
	void SendZodiacInfo();
	void SendBackpackInfo();

	bool CheckHasSpace(int count = 1) const;					// ���ⱳ���Ƿ����㹻�ո���
	void SortBackpack();

	Role *m_role;

	ZodiacParam m_param;
	CharIntAttrs m_attrs_add;									// �����Լӳ���ֵ

	int m_empty_grids;											// �����ո�������
};

#endif