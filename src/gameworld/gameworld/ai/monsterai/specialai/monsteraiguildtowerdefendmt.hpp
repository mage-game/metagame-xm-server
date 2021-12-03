#ifndef _MONSTERAIGUILDTOWERDEFENDMT_HPP_
#define _MONSTERAIGUILDTOWERDEFENDMT_HPP_

#include "ai/monsterai/monsteraibase.hpp"

class MonsterAIGuildTowerDefendMT : public MonsterAIBase
{
public:
	MonsterAIGuildTowerDefendMT(Character *me);
	virtual ~MonsterAIGuildTowerDefendMT();

	virtual void Init(const MonsterAIParam &mp);

	virtual void OnEnterScene();
	virtual void OnLeaveScene();
	virtual void OnAOIEnter(ObjID obj_id);
	virtual void OnAOILeave(ObjID obj_id);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	bool FindEnemy();

	void MoveAOI();
	void GotoBirthPoint();

	UInt32 m_aoi_handle;
	unsigned int m_aoi_range;
	Posi m_aoi_setting_point;
	Posi m_birth_point;

	const static unsigned int MAX_ENEMY_NUM	= 8;
	ObjID m_enemy_list[MAX_ENEMY_NUM];
	unsigned int m_enemy_size;
};


#endif
