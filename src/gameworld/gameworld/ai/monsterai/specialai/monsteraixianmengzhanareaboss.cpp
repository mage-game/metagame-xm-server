#include "monsteraixianmengzhanareaboss.hpp"
#include "servercommon/crossdef.hpp"

MonsterAIXianMengZhanAreaBoss::MonsterAIXianMengZhanAreaBoss(Character *me)
	: MonsterAIBase(me, MonsterInitParam::AITYPE_SPECIALAI_XIANMENGZHAN_AREABOSS), 
	m_guild_id(INVALID_GUILD_ID), m_defend_area(-1)
{

}

MonsterAIXianMengZhanAreaBoss::~MonsterAIXianMengZhanAreaBoss()
{

}

void MonsterAIXianMengZhanAreaBoss::Init(const MonsterAIParam &mp)
{

}

void MonsterAIXianMengZhanAreaBoss::AI(unsigned long interval)
{

}
