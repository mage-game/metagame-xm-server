#ifndef __MONSTER_AI_FOOL_MIANSHANG_HPP__
#define __MONSTER_AI_FOOL_MIANSHANG_HPP__

#include "ai/monsterai/monsteraibase.hpp"

class MonsterAIAddMianshanBuff : public MonsterAIBase
{
public:
	MonsterAIAddMianshanBuff(Character *me);
	virtual ~MonsterAIAddMianshanBuff();

	virtual void Init(const MonsterAIParam &mp);
	virtual void AI(unsigned long interval);
	virtual void Update(unsigned long interval, unsigned int now_second);
	virtual void OnDie(ObjID killer);

	void SetMianshangPer(int mianshan_per){ m_mianshang_per = mianshan_per;}
	void SetEffectType(bool only_boss) { m_only_effect_boss = only_boss;}

	void *operator new(size_t c);
	void operator delete(void *m);

protected:

	int m_mianshang_per;			// 免伤百分比
	bool m_only_effect_boss;		// 只作用boss
	unsigned int next_add_buff_time;	// 下一次加buff时间
};

#endif // __MONSTER_AI_FOOL_MIANSHANG_HPP__
