#ifndef __SHENZHOU_WEAPON_H__
#define __SHENZHOU_WEAPON_H__

class Role;

#include "obj/character/attribute.hpp"
#include "servercommon/shenzhouweapondef.hpp"
#include "obj/character/character.h"
#include "protocal/msgshenzhouweapon.h"
#include "other/shenzhouweapon/shenzhouweaponconfig.hpp"

static const int SHENZHOU_WEAPON_GATHER_BROADCAST_INTERVAL = 30;				// �ɼ���������С���
static const int MAX_IDENTIFY_BATCH_ITEM_COUNT = 999;							// ���һ��������Ʒ����

class ShenzhouWeapon
{
public:
	ShenzhouWeapon();
	~ShenzhouWeapon();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const ShenzhouWeaponParam &param);
	void GetInitParam(ShenzhouWeaponParam *param);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	//////////////////////////////////////////////////////////////////////////
	
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag);
	void OnKillOtherRole(Role * target_role);
	long long GetShenzhouSkillFixInjure(Character *target, long long base_injure, int skill_id);
	void CheckSkillAttr(CharIntAttrs &skill_attrs_add);
	
	//////////////////////////////////////////////////////////////////////////

	void SendBaseInfo();													// ���ͻ�����Ϣ
	void SendGatherInfo();													// ���Ͳɼ���Ϣ

	bool CanGather(int gather_id);											// �ܷ�ɼ���ֻ�ж����������Ĳɼ��
	void OnStartGather(int gather_id);										// ��ʼ�ɼ�
	void OnGather(int gather_id);											// ��Ҳɼ����
	void BuyGatherTimes();													// ����ɼ�����
	void SkipGather();														// һ���������вɼ�����

	void ExchangeIdentifyExp();												// �һ���������
	void AddIdentifyExp(int add_identify_exp, bool is_notice_client = true);// ���Ӽ�������

	void IdentifyBatchItem(int index, int num);								// ���������Ʒ
	void OneKeyIdentifyItem();												// һ������

	void SlotUpLevel(int hunqi_id, int slot);								// ����������������
	void RefreshWeaponLevel(int hunqi_id);									// ˢ�����������ȼ�

	void ElementUpLevel(int hunqi_id, int element_type);					// ����Ԫ������

	void HelpOtherBox(int uid);												// Э�����˵ı���
	void InviteHelpOtherBox();												// ����Э��
	void OpenBox(int itmes);												// ������
	void PutBox();															// ���뱦��
	void SendBoxInfo();														// ���ͱ�����Ϣ
	void RemoveBoxHelp();													// �������Э��

	void HunyinInlay(int hunqi_id, int hunyin_slot, int index_in_bag);		// ��Ƕ��ӡ
	void RefreshHunyinSuitColor(int hunqi_id);								// ˢ�»�ӡ��װ��ɫ
	void HunyinResolve(Protocol::CSShenzhouHunyinResolveReq *cmd);			// �ֽ��ӡ
	void LingshuUplevel(int hunqi_id, int hunyin_slot, bool is_notice_client = true);						// ��������
	void AutoLingshuUplevel(int hunqi_id);									// ��������,��ǰ���������л�ʯֱ��1��

	void GMAddIdentifyExp(int exp);
	void GMAddShenzhouExp(int exp);

	ShenzhouWeaponParam & GetShenZhouParam(){ return m_param; }

	void OpenXilianSlot(int hunqi, int slot);
	void XilianReq(int hunqi, int lock_flag, int xilian_comsume_color, bool is_auto_buy, bool is_free);
	int TransformXilianAttrType(int type);
	void OnRoleLevelChange(int level);
	void OnCompleteTask(TaskID task_id);

	int GetActiveCount();													// ��������
	int GetHunyinInlayCount(int hunyin_color);								// ��Ƕ��ӡ����						

	void OpenHunYinSlot(int hunqi_id, int hunyin_slot, bool is_notice = true, bool is_check_open = false);	//�������

	void ReCalcSuitCount();

	int GetHunyinQualityCount(int hunqi_id, int color);
	int GetHunyinSuitQualityCount(int hunqi_id, int color);					// ��ȡ����Ʒ�ʵ���װ����,ע:��װҲ����

	void OnRoleLogin();	

	void GMAddHunqiLevel(int hunqi_id);

private:

	//////////////////////////////////////////////////////////////////////////
	// ��������
	void OnAttackSkill01(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	void OnAttackSkill03(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	void OnAttackSkill07(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	void OnAttackSkill08(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	void OnAttackSkill10(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);

	void OnHurtOtherSkill02(Character *target, int skill_id, long long base_injure, long long &injure);
	void OnHurtOtherSkill04(Character *target, int skill_id, long long base_injure, long long &injure);
	void OnHurtOtherSkill05(Character *target, int skill_id, long long base_injure, long long &injure);
	void OnHurtOtherSkill06(Character *target, int skill_id, long long base_injure, long long &injure);
	void OnHurtOtherSkill09(Character *target, int skill_id, long long base_injure, long long &injure);
	void OnHurtOtherSkill11(Character *target, int skill_id, long long base_injure, long long &injure);

	//////////////////////////////////////////////////////////////////////////

	void XilianOneReq(int hunqi, int slot, int xilian_comsume_color, bool is_recal = true);
	void CheckFuncOpen();

	Role *m_role;
	ShenzhouWeaponParam m_param;

	CharIntAttrs m_attrs_add;												// �����Լӳ���ֵ
	time_t m_last_gather_time;

	int m_hunyin_quality_count[SHENZHOU_WEAPON_COUNT][HUNYIN_COLOR_MAX];

	int m_capablity;
};

#endif

