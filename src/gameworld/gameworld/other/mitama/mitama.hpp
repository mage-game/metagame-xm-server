#ifndef __MITAMA_HPP__
#define __MITAMA_HPP__

#include "servercommon/mitamadef.hpp"
#include "obj/character/attribute.hpp"

class Role;

class Mitama
{
public:
	Mitama();
	~Mitama();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const MitamaParam &param);
	void GetInitParam(MitamaParam *param);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnUpgrade(int mitama_seq, int spirit_index, bool auto_buy = false);
	void OnTaskFighting(int mitama_seq, int task_seq);			// ��������
	void OnTaskFetchReward(int mitama_seq);						// ��ȡ��������
	void OnExchangeItem(int item_seq, int num);					// �һ���Ʒ

	void SendAllInfo();

	void CheckTaskStatus();										// ������������״̬

	void AddHotSpringScore(int add_score, const char* reason);
	int GetHotSpringScore() { return m_param.hotspring_score; }

protected:
	void SendSingleInfo(int mitama_seq);
	void SendHotSpringScore();

private:
	Role *m_role;

	MitamaParam m_param;
	CharIntAttrs m_attrs_add;
};

#endif
