#ifndef __MONSTERAICOIN_HPP__
#define __MONSTERAICOIN_HPP__

#include <vector>

#include "ai/monsterai/monsteraibase.hpp"

class MonsterAICoin : public MonsterAIBase
{
public:
	MonsterAICoin(Character *me);
	virtual ~MonsterAICoin();

	virtual void Init(const MonsterAIParam &mp);
	void SetMovePath(std::vector<Posi> &path);
	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	void GotoNextPosi();

	std::vector<Posi> m_walk_pos;
	int m_cur_walk_pos;
	int m_max_walk_pos;
	bool m_recalc_target_pos;
};

#endif
