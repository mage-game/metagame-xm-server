#ifndef __GREATE_SOLDIER_SKILL_H__
#define __GREATE_SOLDIER_SKILL_H__

#include "servercommon/basedef.h"

class Role;
class GreateSoldier;

class GreateSoldierSkill
{
public:
	void Init(Role *role, GreateSoldier *greate_soldier);

	void CheckActiveSkill();					// ��鼤���

	void Update(time_t now_second, unsigned long interval);

	void OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);

	void SetBaoji(bool is_baoji) { is_greatesoldier_baoji_skill = is_baoji; }
	bool IsBaoji() { return is_greatesoldier_baoji_skill; }

private:
	void OnChangeSpecialSkill();				// ���ı����⼼�ܣ�����or��������ʱ������

private:
	Role *m_role = nullptr;
	GreateSoldier *m_greate_soldier = nullptr;

	UInt32 m_special_skill_flag = 0;			// ���⼼�ܼ�����
	UInt32 m_old_special_skill_flag = 0;		// �ɵ����⼼�ܼ�����

	time_t m_skill_perform_time_11 = 0;			// �ϴμ���11�ͷ�ʱ��
	time_t m_skill_perform_time_13 = 0;			// �ϴμ���13�ͷ�ʱ��

	bool is_greatesoldier_baoji_skill = false;	// �Ƿ񱩻����ܣ��ٷְٲ�������
};

#endif // __GREATE_SOLDIER_SKILL_H__