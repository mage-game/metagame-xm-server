#ifndef _MONSTERAIXIANMENGZHANDANYAOBOSS_HPP_
#define _MONSTERAIXIANMENGZHANDANYAOBOSS_HPP_

#include "ai/monsterai/monsteraibase.hpp"

class MonsterAIXianMengZhanDanYaoBoss : public MonsterAIBase
{
public:
	MonsterAIXianMengZhanDanYaoBoss(Character *me);
	virtual ~MonsterAIXianMengZhanDanYaoBoss();

	virtual void Init(const MonsterAIParam &mp);

	virtual void OnEnterScene();
	virtual void OnLeaveScene();
	virtual void OnAOIEnter(ObjID obj_id);
	virtual void OnAOILeave(ObjID obj_id);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

	void InitAIParam(GuildID guild_id, const Posi &center_pos, int skill_interval, int skill_id, int atk_range, int add_gongji_percent, int add_fangyu_percent, int buff_dur_ms);

	GuildID GetGuildID() { return m_guild_id; }

protected:
	virtual bool IsEnemy(Obj *obj);
	virtual bool IsFriend(Obj *obj, bool check_alive = true);

	virtual bool IsAttackWhenOnAttack(ObjID attacker) { return false; }

	bool FindEnemy(unsigned long interval);
	void MoveAOI();
	void PerformBuffSkill();

	UInt32 m_aoi_handle;
	unsigned int m_aoi_range;
	Posi m_aoi_cur_pos;

	bool m_stop;

	GuildID m_guild_id;
	Posi m_center_pos;
	int m_skill_id;
	int m_skill_interval;
	unsigned int m_next_perform_time;
	
	int m_atk_range;
	int m_add_gongji_percent;
	int m_add_fangyu_percent;
	int m_buff_dur_ms;

	const static int MAX_ENEMY_NUM	= 5;	// 可记录最大敌人数
	ObjID	m_enemy_list[MAX_ENEMY_NUM];	// 敌人列表
	int		m_enemy_size;					// 当前敌人数目
};

#endif // _MONSTERAIXIANMENGZHANDANYAOBOSS_HPP_
