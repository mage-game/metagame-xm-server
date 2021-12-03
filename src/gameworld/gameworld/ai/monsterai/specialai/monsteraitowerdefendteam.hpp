#ifndef _MONSTERAITOWERDEFENDTEAM_HPP_
#define _MONSTERAITOWERDEFENDTEAM_HPP_

#include "ai/monsterai/monsteraibase.hpp"

class MonsterAITowerDefendTeam : public MonsterAIBase
{
public:
	MonsterAITowerDefendTeam(Character *me);
	virtual ~MonsterAITowerDefendTeam();

	virtual void Init(const MonsterAIParam &mp);

	virtual void OnEnterScene();
	virtual void OnLeaveScene();
	virtual void OnAOIEnter(ObjID obj_id);
	virtual void OnAOILeave(ObjID obj_id);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

	void SetBirthPath(int birth_path); 

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	bool FindEnemy();
	virtual bool IsAttackWhenOnAttack(ObjID attacker);

	void MoveAOI();
	void GotoNextPosi();

	bool MoveAround();

	int m_birth_path;											// ³öÉúÂ·¾¶

	UInt32 m_aoi_handle;
	unsigned int m_aoi_range;
	Posi m_aoi_setting_point;

	const static unsigned int MAX_ENEMY_NUM	= 8;
	ObjID m_enemy_list[MAX_ENEMY_NUM];						
	unsigned int m_enemy_size;
	
	bool m_walk_path_way;
	std::vector<Posi> m_walk_pos;
	unsigned int m_cur_walk_pos;
};

#endif

