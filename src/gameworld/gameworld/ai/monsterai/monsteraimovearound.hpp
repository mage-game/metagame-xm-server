#ifndef __MONSTERAIMOVEAROUND_HPP__
#define __MONSTERAIMOVEAROUND_HPP__

#include <vector>

#include "monsteraibase.hpp"

class MonsterAIMoveAround : public MonsterAIBase
{
public:
	MonsterAIMoveAround(Character *me, int aitype);
	virtual ~MonsterAIMoveAround();

	virtual void Init(const MonsterAIParam &mp);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	void MoveAround(unsigned long interval);

	std::vector<Posi> m_walk_pos;
	unsigned int m_cur_walk_pos;
	bool m_walk_state;
	
	int m_rest_time;
	int m_init_rest_time;
	int m_rest_deviation_time;
};

#endif
