#ifndef __FIGHTBACK_MANAGER_HPP__
#define __FIGHTBACK_MANAGER_HPP__

#include <map>
#include <set>

class Role;

// 角色反击对象管理
class FightBackManager
{
public:
	static FightBackManager & Instance();

	void Update(unsigned long interval, time_t now_second);

	void OnRoleDie(Role *role);
	
	bool IsFightBackObj(Role * role, Role *target_role);	// target是否是role的反击对象
	void AddFightBackRole(Role *role, Role *attacker_role); // 将攻击者加入反击列表
	void SendRoleList(Role *role);
	void SendAddOrDelRoleList(Role *role, std::set<int> &role_list, int notify);

private:
	FightBackManager();
	~FightBackManager();

	typedef std::map<int, unsigned int> AttackRoleMap;			// <attack_uid, attack_timestamp>
	typedef std::map<int, unsigned int>::iterator AttackRoleMapIt;
	std::map<int, AttackRoleMap >		m_fight_back_role_map;	// 角色反击对象列表<uid, <attack_uid, attack_timestamp> >

	std::map<int, std::set<int> >		m_role_attack_list;		// 角色攻击列表<attack_uid, <uid> > ， 缓存攻击者是哪些角色的反击对象

	time_t	m_next_check_timestamp;								// 下一次检查删除攻击对象的时间
};

#endif // __FIGHTBACK_MANAGER_HPP__

