#ifndef __BOSSCARD_HPP__
#define __BOSSCARD_HPP__

#include "servercommon/bosscarddef.hpp"
#include "obj/character/attribute.hpp"

class Role;

class BossCard
{
public:
	BossCard(): m_role(NULL) {}
	~BossCard() {}

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const BossCardParam &param);
	void GetInitParam(BossCardParam *param) const;

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnOperaReq(int opera_type, int param1, int param2);						// �������

	void CheckActiveCard(unsigned short monster_id);                  // ����boss:dps��һ/���һ��   ����boss:�������
private:
	void OnActiveCard(int monster_seq);
	void OnFetchCardGroupReward(int card_type);
	void SendAllInfo();

private:
	Role *m_role;
	BossCardParam m_param;
	CharIntAttrs m_attrs_add;												// �����Լӳ���ֵ

	bool IsActivityThisCardType(int card_type);
};

#endif
