#ifndef __CARD_ZU_H__
#define __CARD_ZU_H__

class Role;

#include "obj/character/attribute.hpp"
#include "servercommon/cardzudef.hpp"
#include <set>

class Cardzu
{
	enum BUY_LINGLI_TYPE
	{
		BUY_LINGLI_TYPE_COIN = 0,											// ͭ�ҵ���
		BUY_LINGLI_TYPE_GOLD,												// Ԫ������
		BUY_LINGLI_TYPE_GOLD10,												// Ԫ��ʮ����
		BUY_LINGLI_TYPE_GOLD_BIND,											// ���굥��
	};

public:
	Cardzu();
	~Cardzu();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const CardzuParam &param);
	void GetInitParam(CardzuParam *param);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void SendCardzuAllInfo();												// ���Ϳ���������Ϣ
	void OnChouCardReq(int cardzu_id, int type);							// �鿨
	void OnHualingReq(int card_id, int count);								// ������
	void OnLingzhuReq(int card_id);											// ��������
	void OnActiveZuheReq(int zuhe_id);										// ����ĳ�����
	void OnUpgradeZuheReq(int zuhe_id, int target_level);					// ����ĳ�����

protected:
	typedef std::set<int> ChangeCardSet;
	typedef std::set<int>::const_iterator ChangeCardSetIt;
	void OnNotifyChange(int zuhe_id = -1, ChangeCardSet *change_card_list = NULL); // ���Ϳ���ı���Ϣ
	void CheckMaxActiveCardzu(bool is_init_check);
	void OnCoinChouCard(int cardzu_id);										// ͭ�ҵ���
	void OnGoldChouCard(int cardzu_id);										// Ԫ������
	void OnGoldBatchChouCard(int cardzu_id);								// Ԫ��ʮ����
	void OnGoldBindChouCard(int cardzu_id);									// ���굥��

private:
	Role *m_role;
	CardzuParam m_param;

	CharIntAttrs m_attrs_add;												// �����Լӳ���ֵ
	int m_active_max_cardzu_id;												// ��ǰ����������id
};

#endif

