#ifndef _MONSTERAIXIANMENGZHANGUARDBOSS_HPP_
#define _MONSTERAIXIANMENGZHANGUARDBOSS_HPP_


#include "ai/monsterai/monsteraistaticinitiative.hpp"

class MonsterAIXianMengZhanGuardBoss : public MonsterAIStaticInitiative
{
public:
	MonsterAIXianMengZhanGuardBoss(Character *me);
	virtual ~MonsterAIXianMengZhanGuardBoss();

	void *operator new(size_t c);
	void operator delete(void *m);

	void    SetGuildID(GuildID guild_id) { m_guild_id = guild_id; }
	GuildID GetGuildID() { return m_guild_id; }

	void SetDefendArea(int defend_area) { m_defend_area = defend_area; }
	int  GetDefendArea() { return m_defend_area; }

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	GuildID   m_guild_id;
	int       m_defend_area;
};

#endif // _MONSTERAIXIANMENGZHANGUARDBOSS_HPP_
