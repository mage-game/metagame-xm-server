#pragma once
#ifndef __XIANZUNKA_HPP__
#define __XIANZUNKA_HPP__
#include "servercommon/xianzunkadef.hpp"

class Role;

class XianZunKa
{
public:
	XianZunKa();
	~XianZunKa();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const XianZunKaParam &param);
	void GetInitParam(XianZunKaParam *param);
	void Update(unsigned int now_second);
	void OnAddChongzhi(long long chongzhi_gold);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalcAttrBeLink();								//�������ϵͳ���Լӳ�

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	bool OnUseXianZunKa(int card_type);
	void OnBuyXianZunKa(int card_type); 
	void OnGiveTemporaryXianZunKa(int card_type);			//��ʱ����
	void OnFetchDailyReward(int card_type);
	void OnActiveXianZunKa(int card_type);
	void OnActiveFlagChange();
	short SetAndGetHasXianzunkaFlag();

	const int GetAddExpPer();
	const int GetAddEquipStrengthSuccRate();
	const int GetAddMountGradeAttrsPer();
	const int GetAddWingGradeAttrsPer();
	const int GetAddShenbingGradeAttrsPer();
	const int GetAddShizhuangGradeAttrsPer();

	int GetActiviteCount();

	void SendAllInfo();
	void OnXianZunKaActivityChongZhi(long long chongzhi_gold);

private:

	Role *m_role;
	XianZunKaParam m_param;

	CharIntAttrs m_attrs_add;												// �����Լӳ���ֵ
	int m_other_capability;
};

#endif
