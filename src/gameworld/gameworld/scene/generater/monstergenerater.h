#ifndef __MONSTERGENERATER_H__
#define __MONSTERGENERATER_H__

#include <stdlib.h>

#include "gamedef.h"
#include "config/dynamicadjustconfig.hpp"

class Scene;
class Monster;
class MonsterInitParam;

class MonsterGenerater
{
public:
	MonsterGenerater(Scene *scene);
	~MonsterGenerater();

	bool Init(int max_total_monster, int max, unsigned long interval, const Posi &pos, UInt16 monster_id);
	void Update(unsigned long interval);

	void MonsterDie(Monster *monster);

	void Reset();
	void CalcMonsterAddAttrs(int *add_maxhp, int *add_gongji, int *add_fangyu);

	void CrossGuildBattleSetAddAttrs(int level);

	void ClearMonster() { m_monster_id = 0; }
	void ChangeMonster(UInt16 monster_id) { m_is_create = true; m_monster_id = monster_id; }

	void RefreshNow();

	int GetMonsterID() { return m_monster_id; }
	unsigned int GetMonsterNextFlushTime();

	void SetPause(bool is_pause);
	const Posi & GetPos() { return m_pos; }
	bool IsBabyBoss() { return m_is_baby_boss; }

	void *	operator new(size_t c);
	void	operator delete(void *m);

public:
	bool				m_is_create;			// 是否继续出怪
	int					m_cur_total_monster;	// 该出怪点已经出过多少只怪
	int					m_max_total_monster;	// 该出怪点总共允许出多少只怪，可用于副本
	int					m_max_monster_count;	// 允许同时出怪数量
	int					m_cur_monster_count;	// 当前同时出怪数量
	unsigned long		m_create_interval;
	unsigned long		m_cur_time;

	Scene				*m_scene;
	Posi				m_pos;

	UInt16				m_monster_id;

	bool				m_is_pause;

	int					m_add_maxhp;
	int					m_add_gongji;
	int					m_add_fangyu;

	bool				m_had_update;
	bool				m_need_adjust;
	RefreshAdjustKeyObj m_refresh_adjust_obj;
	bool				m_is_baby_boss;
};

#endif

