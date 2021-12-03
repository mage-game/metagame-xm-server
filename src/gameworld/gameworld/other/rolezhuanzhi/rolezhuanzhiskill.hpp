#ifndef __ROLE_ZHUAN_ZHI_SKILL_HPP__
#define __ROLE_ZHUAN_ZHI_SKILL_HPP__

#include "obj/character/attribute.hpp"
#include "obj/character/character.h"
#include "config/zhuanzhiconfig.hpp"
#include <map>

class Role;
class RoleZhuanZhi;

// תְ������������id
//static const int ZHUANZHI_LEVLE_2_HURT_SKILL_ID = 180;
//static const int ZHUANZHI_LEVLE_2_RECOVER_SKILL_ID = 181;

class RoleZhuanZhiSkill
{

	struct VirSkillItem
	{
		short vir_skill_id = 0;										// ���⼼��id
		short vir_skill_type = 0;									// ���⼼������
		short follow_perform_skill_id = 0;							// ������������id
		short level = 0;											// ����
		unsigned int last_perform_skill_timestamp = 0;				// �����ϴδ���ʱ���
	};

public:
	RoleZhuanZhiSkill();
	~RoleZhuanZhiSkill();

	void Init(Role * role, RoleZhuanZhi *rolezhuanzhi);
	void OnInitFinish();

	void RecalcActivedSkill(bool is_init = false);											// ����һ���Ѽ�����б�
	bool IsActiveSkill(int skill_id);									// �Ƿ񼤻�

	VirSkillItem *GetVirSkillItem(int vir_skill_type);

	void OnHurtOther(Character *target, int skill_id, long long injure, int fight_type);					// ����

	void OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);			// ������
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);					// ������(�����˺�)

	// ��������---------------------------------------------------------------------
	void CheckKillOtherRole(bool is_kill);							   // ɱ��

	void OnHurtOtherPVPUpShanghaiAttr(Character *target, long long base_injure);				// ���������������Ӷ�����˺�
	void OnHurtOtherPVEUpShanghaiAttr(Character *target, long long base_injure);				// ���������������ӶԹ����˺�

	void OnHurtOtherPVEUpShanghai(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVPUpShanghai(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVPXuanyun(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVPUpShanghai2(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVPUpShanghai3(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVPUpShanghai4(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVMUpShanghai(Character * target, int vir_skill_id, int skill_id, long long injure);
	void OnHurtOtherHurtInterval(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherHurt1(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherHurt2(Character * target, int vir_skill_id, int skill_id, long long injure);
	void OnHurtOtherHurt4(Character * target, int vir_skill_id, int skill_id, long long injure);
	void OnHurtOtherHurt5(Character * target, int vir_skill_id, int skill_id, long long injure);
	void OnHurtOtherPVPChenmo(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVERecover(Character * target, int vir_skill_id, int skill_id, long long injure);

	// �ܻ�����---------------------------------------------------------------------
	void OnBeendAttackPVPDownShanghaiAttr(ObjID attacker, Attribute &delta_hp);					// �����������Լ�����Ҷ��Լ��˺�
	void OnBeendAttackPVEDownShanghaiAttr(ObjID attacker, Attribute &delta_hp);					// �����������Լ��ٹ�����Լ��˺�

	bool IsMianyiDeadlyAttack();									   // �Ƿ�����������һ��
	void OnBeenAttackNormalHpStore(int vir_skill_id, int skill_id);
	void OnBeenAttackWudiHpStore(int vir_skill_id, int skill_id);
	void OnBeenAttackPVPRebound(ObjID attacker, int vir_skill_id, int skill_id, long long delta_hp);
	void OnBeenAttackPVPDownShanghai(ObjID attacker, int vir_skill_id, int skill_id);

	// �Զ�����---------------------------------------------------------------------
	void CheckBeenAttack(unsigned int now);
	void CheckRecover(time_t now_second);								// �ظ�
	bool CheckMainyiXuanyun();											// ����ѣ��
	double GetAddExpPer();												// ���Ӿ���

	// ��������---------------------------------------------------------------------
	/*��ʱ����*/
	void OnPerformSkillReq(int vir_skill_id, ObjID target_id);
	void OnPerformRecover(int vir_skill_id);
	void OnPerformHurt3(int vir_skill_id, ObjID target_id);

	// -----------------------------------------------------------------------------
	int GetAddRangeEnemyNum() { return m_normal_aoe_add_target_num; }	

	void BroadcastSkillTrigger(int vir_skill_id, ObjID target_obj_id, long long injure = 0, bool send_to_role = false);

private:
	Role *m_role = nullptr;
	RoleZhuanZhi *m_rolezhuanzhi = nullptr;

	int m_max_cfg_skill_count = 0;									// ��ǰ���õ��������
	int m_normal_aoe_add_target_num = 0;							// ĿǰAOEĿ����������

	int m_special_attr_pvp_jianshang = 0;							// ������������pvp���� 
	int m_special_attr_pvp_zengshang = 0;							// ������������pvp����
	int m_special_attr_pve_jianshang = 0;							// ������������pve����
	int m_special_attr_pve_zengshang = 0;							// ������������pve����

	std::map<int, VirSkillItem> m_cur_active_skill_map;				// ���е�ǰ����ļ���
};

#endif