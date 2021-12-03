#ifndef __MONSTERAIXIANMENGZHANAREABOSS_HPP__
#define __MONSTERAIXIANMENGZHANAREABOSS_HPP__

#include "ai/monsterai/monsteraibase.hpp"

class MonsterAIXianMengZhanAreaBoss : public MonsterAIBase
{
public:
	MonsterAIXianMengZhanAreaBoss(Character *me);
	virtual ~MonsterAIXianMengZhanAreaBoss();

	virtual void Init(const MonsterAIParam &mp);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

	void    SetGuildID(GuildID guild_id) { m_guild_id = guild_id; }
	GuildID GetGuildID() { return m_guild_id; }

	void SetDefendArea(int defend_area) { m_defend_area = defend_area; }
	int  GetDefendArea() { return m_defend_area; }

protected:
	virtual bool IsAttackWhenOnAttack(ObjID attacker) { return false; }

	GuildID   m_guild_id;               // œ…√À
	int       m_defend_area;            // æ›µ„
};

#endif // __MONSTERAIXIANMENGZHANAREABOSS_HPP__
