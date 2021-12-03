#ifndef __MAGIC_CARD_H__
#define __MAGIC_CARD_H__

class Role;

#include "obj/character/attribute.hpp"
#include "servercommon/magiccarddef.hpp"
#include <set>

class MagicCard
{
public:
	MagicCard();
	~MagicCard();

	void SetRole(Role *role) { m_role = role; }

	void InitParam(Role *role, const MagicCardParam &param);
	void GetInitParam(MagicCardParam *param);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool ActiveCard(ItemID item_id, int num);
	void UseCard(int card_id);
	void UpgradeCard(int color, int slot_index, int consume_card_id, int consume_card_num);
	void ChouCard(int type, int times);
	void ExchangeCard(int card_id);
	void SendAllInfo();
	void ActiveSkill(int color);

	void OnGMRequest(int type, int param1, int param2, int param3);

protected:
	

private:
	Role *m_role;
	MagicCardParam m_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值
};

#endif

