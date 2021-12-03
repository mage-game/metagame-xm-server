#ifndef __MONSTER_AI_FOOL_RECOVER_HPP__
#define __MONSTER_AI_FOOL_RECOVER_HPP__

#include "ai/monsterai/monsteraibase.hpp"

class MonsterAIAddRecoverBuff : public MonsterAIBase
{
public:
	MonsterAIAddRecoverBuff(Character *me);
	virtual ~MonsterAIAddRecoverBuff();

	virtual void Init(const MonsterAIParam &mp);
	virtual void AI(unsigned long interval);
	virtual void Update(unsigned long interval, unsigned int now_second);

	void SetEffectType(bool only_boss){ m_only_effect_boss = only_boss;}
	void SetRecoverType(bool recover_by_attack){ m_recover_by_attack = recover_by_attack;}
	void SetRecoverPer(int recover_per){ m_hp_recover_per = recover_per;}
	void SetRecoverInterval(int recover_interval){ m_hp_recover_interval = recover_interval;}

	void *operator new(size_t c);
	void operator delete(void *m);

protected:

	virtual bool IsEnemy(Obj *obj);
	bool FindEnemy();
	
	bool m_only_effect_boss;		// 是否只对boss作用
	bool m_recover_by_attack;		// 是否通过攻击回血
	int m_pass_interval;
	int m_hp_recover_per;			// 回血百分比
	int m_hp_recover_interval;		// 回血间隔
};

#endif // __MONSTER_AI_FOOL_RECOVER_HPP__


