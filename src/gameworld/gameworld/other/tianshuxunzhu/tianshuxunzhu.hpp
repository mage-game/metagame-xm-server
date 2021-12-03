#ifndef __TIANSHU_XUNZHU_HPP__
#define __TIANSHU_XUNZHU_HPP__

#include "item/itemgriddata.h"
#include "servercommon/struct/itemlistparam.h"

#define UINT_LAST_BIT ((int)sizeof(unsigned int) * 8 - 1)

const static int TIANSHUXUNZHU_EQUIP_LEVEL_1 = 50;
const static int TIANSHUXUNZHU_EQUIP_LEVEL_2 = 100;

class Role;

class TianShuXunZhu
{
public:

	// �̱�����
	static void CheckGuBenTianShu(Role *role, long long param);

	// ��ħ����
	static void CheckTuMoTianShu(Role *role);

	// ��Ѫ����
	static void CheckShiXueTianShu(Role *role, int param, int param2 = 0);

	// �ػ�����
	static void CheckShouHuTianShu(Role *role, int equip_part,int grade);

	// ��ս���� 
	static void CheckBaiZhanTianshu(Role * role);
	//��������
	static void CheckShenShouTianshu(Role * role);

	static void OnRoleLogin(Role *role);
	static void OnTianshuXunzhuFetchReward(Role *role, int type, int idx);		// ����Ѱ������ȡ����
	static void SendTianshuXunzhuInfo(Role *role);								// ����Ѱ����������Ϣ

	static void ReCalcAttr(Role * role,CharIntAttrs &base_attr, bool is_recalc);

	static int GetExtraExp(Role * role);
	static int OnHurtBoss(Role * role);
	static int GetSkillZengShang(Role * role);
	static int GetPvpJianSHang(Role * role);
	static int GetGeDangDikang(Role* role);
	static int GetHuiXinYiJi(Role * role);

public:
	static int GetSkillValue(Role * role,int tianshu_type);
};

#endif	// __TIANSHU_XUNZHU_HPP__