#ifndef __FIGHTBACK_MANAGER_HPP__
#define __FIGHTBACK_MANAGER_HPP__

#include <map>
#include <set>

class Role;

// ��ɫ�����������
class FightBackManager
{
public:
	static FightBackManager & Instance();

	void Update(unsigned long interval, time_t now_second);

	void OnRoleDie(Role *role);
	
	bool IsFightBackObj(Role * role, Role *target_role);	// target�Ƿ���role�ķ�������
	void AddFightBackRole(Role *role, Role *attacker_role); // �������߼��뷴���б�
	void SendRoleList(Role *role);
	void SendAddOrDelRoleList(Role *role, std::set<int> &role_list, int notify);

private:
	FightBackManager();
	~FightBackManager();

	typedef std::map<int, unsigned int> AttackRoleMap;			// <attack_uid, attack_timestamp>
	typedef std::map<int, unsigned int>::iterator AttackRoleMapIt;
	std::map<int, AttackRoleMap >		m_fight_back_role_map;	// ��ɫ���������б�<uid, <attack_uid, attack_timestamp> >

	std::map<int, std::set<int> >		m_role_attack_list;		// ��ɫ�����б�<attack_uid, <uid> > �� ���湥��������Щ��ɫ�ķ�������

	time_t	m_next_check_timestamp;								// ��һ�μ��ɾ�����������ʱ��
};

#endif // __FIGHTBACK_MANAGER_HPP__

