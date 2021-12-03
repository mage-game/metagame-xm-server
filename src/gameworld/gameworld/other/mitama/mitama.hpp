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
	void OnTaskFighting(int mitama_seq, int task_seq);			// 出征御魂
	void OnTaskFetchReward(int mitama_seq);						// 领取出征奖励
	void OnExchangeItem(int item_seq, int num);					// 兑换物品

	void SendAllInfo();

	void CheckTaskStatus();										// 检查所有任务的状态

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
