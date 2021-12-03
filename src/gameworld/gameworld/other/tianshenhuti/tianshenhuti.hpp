#ifndef _TIANSHENHUTI_HPP_
#define _TIANSHENHUTI_HPP_

#include "servercommon/tianshenhutidef.hpp"
#include "obj/character/attribute.hpp"
#include "role_module/i_role_module.hpp"

class Role;

/*
 * ��ĩװ��
 */
class TianshenHuti : public IRoleModule
{
public:
	TianshenHuti();
	~TianshenHuti() override;

	virtual AutoParamBase & GetParam() override { return m_param; }
	virtual void OnInit() override;

	virtual void ReCalcAttr(CharIntAttrs &base_add, int recalc_reason, bool recalc_all) override;
	virtual void OnRoleLogin() override;
	virtual void OnAllInfoReq() override { this->SendAllInfoToRole(); }
	virtual void OnDayChange(const ClockReturnDayItem &old_day, const ClockReturnDayItem &new_day) override;
	virtual void OnWeekChange() override;

	virtual void OnHurtOther(Character *target, int skill_id, long long &injure, int target_index, int &passive_flag) override;

	void PutOnEquip(int backpack_slot_index);								// ��ָ���������ӵ�װ����������
	void TakeOffEquip(int slot_index);										// ��ָ����λװ������
	void TransformEquip(int slot_index1, int slot_index2, int new_equip_slot);// ת�������������װ��
	void CombineEquip(int slot_index1, int slot_index2, int slot_index3, int client_param);
	bool CombioneOneEquip(int slot_index1, int slot_index2, int slot_index3, int *new_equip_id);
	// �ϳ��������������װ��
	void CombineEquipOneKey(int level);										// һ���ϳ�

	int GetBackpackEmptyGridNum();											// ��ȡʣ�౳������
	int PutEuipToBackpack(TianshenEquipID equip_id);						// ��ָ��װ���Ž�����
	TianshenEquipID GetBackpackEquip(int slot_index);						// ��ȡָ�������������װ��
	TianshenhutiParam* GetTianshenhutiParam() { return &m_param.Param(); }

	void RollReq(int roll_type);
	void SendAllInfoToRole();												// ����������Ϣ�����

	void AddRollScore(int add_score, const char *reason);					// ���ӳ齱����
	bool OnAddEquipByItem(int equip_id, const char *reason);
	bool OnAutoAddEquipByItem(int equip_id, const char *reason);			// �Զ�����װ������ĩװ���ı���
	void GetTaozhuangInjureAdd(int skill_index, double *rate_injure, int *static_injure);

private:
	bool RemoveEquipFromBackpack(int slot_index);							// ɾ��ָ��λ��װ��������װ��id�������һ��װ������ȱ�����ã�

	void UpdateTaozhuangNum(bool is_init = false);

	void AddAccumulateTime(int add_times);

	AutoTianshenhutiParam m_param;
	CharIntAttrs m_attrs_add;												// �����Լӳ���ֵ

	int m_backpack_num;														// ��ǰ������Ʒ����

	std::map<int, int> m_taozhuang_num_map;
};

#endif