#ifndef __MONSTER_AI_GONGCHENGZHAN_HPP__
#define __MONSTER_AI_GONGCHENGZHAN_HPP__

#include "ai/monsterai/monsteraistaticinitiative.hpp"

class MonsterAIGongchengzhan : public MonsterAIStaticInitiative
{
public:
	MonsterAIGongchengzhan(Character *me);
	virtual ~MonsterAIGongchengzhan();

	virtual void Init(const MonsterAIParam &mp);
	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

	void SetSite(int site) { m_site = site; }
	int GetSite() { return m_site; }

	void AddGongji();

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	int m_site;
};

#endif // __MONSTER_AI_GONGCHENGZHAN_HPP__


