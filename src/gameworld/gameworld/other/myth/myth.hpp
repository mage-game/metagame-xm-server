#ifndef __MYTH_HPP__
#define __MYTH_HPP__

#include "obj/character/attribute.hpp"
#include "servercommon/mythdef.hpp"

class Role;
class CharIntAttrs;

class Myth
{
public:
	Myth();
	~Myth();

	void InitParam(Role *role, const MythParam &param);
	void GetInitParam(MythParam *param);
	void SetRole(Role* role) { m_role = role; }

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnChpaterUplevel(int chpater_id);
	void OnResonance(int chpater_id);

	bool IsChpaterResonanceFinsh(int chpater_id);
	void SendChpaterAllInfo();
	void SendChpaterSingleInfo(int chpater_id);

	/*****************************************************************************************************************/
	// 下列函数暂时屏蔽了。 -2018.12.18
	bool OnDigestion(int chpater_id);
	void OnDecompose(int knapsack_index);
	void Compose(int item_id, int index_1, int index_2, int index_3);
	bool AddSoulItem(MythSoulGodItem &tmp_item, const char *reason, bool is_send = true);
	bool AddItem(ItemID item_id, int num);
	bool ConsumeItem(ItemID item_id, int num);
	void TakeOffSoul(int chpater_id, int slot_index);
	void InlaySoul(int chpater_id, int backpack_index, int slot_index);
	void SendBackpackInfo();
	void SendBackpackAdditionInfo(int knapsack_index = -1);
	int GetItemCount(ItemID item_id);
	bool TransformFromKnapsackItem(ItemID item_base_id, int item_base_num, const char *reason);	
	bool BatchUseItemCheck(ItemID item_id, int check_num);
	bool EmptyBackpackNumMoreThan(int check_num);
	void GmAddEssence(int num);
	/*****************************************************************************************************************/

private:
	Role *m_role;
	MythParam m_param;
	CharIntAttrs m_attrs_add;
	
	void AddResonanceAttrPer(CharIntAttrs &m_attrs_add, int attr_per_type, Attribute attr_val);
	int TranslateToRoleCharattr(int resonace_type);
	void ResonanceUpLevel(int chpater_id);
};


#endif //__MYTH_HPP__
