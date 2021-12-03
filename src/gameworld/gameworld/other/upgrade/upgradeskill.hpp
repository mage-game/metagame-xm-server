#ifndef __UPGRADE_SKILL_HPP__
#define __UPGRADE_SKILL_HPP__

#include "obj/character/attribute.hpp"
#include "obj/character/character.h"

#include <map>

class Role;
class UpgradeManager;

class UpgradeSkill
{

	struct VirSkillItem
	{
		short upgrade_type = 0;										// ��������
		short vir_skill_index = 0;									// ���⼼������
		short level = 0;											// ����
		short param_a = 0;
		short param_b = 0;
		short param_c = 0;
		short param_d = 0;
		unsigned int last_perform_skill_timestamp = 0;				// �����ϴδ���ʱ���
	};

public:
	UpgradeSkill();
	~UpgradeSkill();

	void Init(Role * role, UpgradeManager *upgrade_manager);

	void OnInitFinish();												// ��Ҫ��ʼ������Ĳ���������

	void RecalcActivedSkill();											// ����һ���Ѽ�����б�

	void OnHurtOther(Character *target, int skill_id, long long injure, int fight_type);					// ����
	void OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);			// ������
	void OnAfterHurtOther(Character *target, int skill_id, long long injure, bool from_skill, int fight_type);

	// ��������---------------------------------------------------------------------
	void OnHurtOtherUpgrade2(Character * target, long long injure, bool from_skill);

	// �ܻ�����---------------------------------------------------------------------


	// �Զ�����---------------------------------------------------------------------

	// ��������---------------------------------------------------------------------


	// ------------------------------------------------------------------------------
	

private:
	Role *m_role;
	UpgradeManager *m_upgrade_manager;

	std::map<int, VirSkillItem> m_cur_active_skill_map;				// ���е�ǰ����ļ���
};

#endif