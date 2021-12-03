#ifndef __COMMONSKILLMONSTER_HPP__
#define __COMMONSKILLMONSTER_HPP__

#include "skill/skilldef.hpp"
#include "skill/skillbase.hpp"

#include <string>

// 怪物的普通攻击技能 单体对敌人 : 单体对敌人 ID从 10001 开始
class MonsterCommonSkillToEnemy : public SkillBase
{
public:
	MonsterCommonSkillToEnemy(std::string &filename) : SkillBase(SKILL_TYPE_ACTIVE, true, true),
		m_filename(filename), m_effect_target(SKILL_ATTACH_EFFECT_TARGET_INVALID), m_effect_count(0) {}
	virtual ~MonsterCommonSkillToEnemy() {}

	virtual bool LoadConfig(const char *dir, std::string *err);
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return m_filename.c_str(); }

protected:
	std::string m_filename;

	short m_effect_target;
	short m_effect_count;
	SkillAttachEffectParam m_effect_list[SKILL_ATTACH_MAX_EFFECT];
};

// 怪物的普通群攻攻击技能 群体对敌人坐标 : 群体对敌人坐标 ID从 11001 开始
class MonsterRangeCommonSkillToEnemyPos : public SkillBase
{
public:
	MonsterRangeCommonSkillToEnemyPos(std::string &filename) : SkillBase(SKILL_TYPE_ACTIVE, true, true), 
		m_filename(filename), m_effect_target(SKILL_ATTACH_EFFECT_TARGET_INVALID), m_effect_count(0)
	{
		for (int i = 0; i <= MAX_SKILL_LEVEL; i++)
		{
			m_attack_range_enemy_num_list[i] = SKILL_ATTACH_EFFECT_MAX_TARGET;
		}
	}
	virtual ~MonsterRangeCommonSkillToEnemyPos() {}

	virtual bool LoadConfig(const char *dir, std::string *err);
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return m_filename.c_str(); }

protected:
	std::string m_filename;
	
	/*
	1 Boss
	2 小怪
	3 全部怪物
	4 玩家及其宠物
	5 全部对象
	6 敌人
	*/
	short m_effect_target;
	short m_effect_count;
	SkillAttachEffectParam m_effect_list[SKILL_ATTACH_MAX_EFFECT];
};

// 怪物的单体技能 单体对自己 : 单体对自己 ID从 12001 开始   如：BOSS免疫
class MonsterCommonSkillToSelf : public SkillBase
{
public:
	MonsterCommonSkillToSelf(std::string &filename) : SkillBase(SKILL_TYPE_ACTIVE, false, false), 
		m_filename(filename), m_effect_target(SKILL_ATTACH_EFFECT_TARGET_INVALID), m_effect_count(0) {}
	virtual ~MonsterCommonSkillToSelf() {}

	virtual bool LoadConfig(const char *dir, std::string *err);
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return m_filename.c_str(); }

protected:
	std::string m_filename;

	short m_effect_target;
	short m_effect_count;
	SkillAttachEffectParam m_effect_list[SKILL_ATTACH_MAX_EFFECT];
};

// 怪物的群体技能 群体对自己坐标 : 群体对自己坐标 ID从 13001 开始
class MonsterRangeCommonSkillToSelfPos : public SkillBase
{
public:
	MonsterRangeCommonSkillToSelfPos(std::string &filename) : SkillBase(SKILL_TYPE_ACTIVE, true, true),
		m_filename(filename), m_effect_target(SKILL_ATTACH_EFFECT_TARGET_INVALID), m_effect_count(0)
	{
		for (int i = 0; i <= MAX_SKILL_LEVEL; i++)
		{
			m_attack_range_enemy_num_list[i] = SKILL_ATTACH_EFFECT_MAX_TARGET;
		}
	}
	virtual ~MonsterRangeCommonSkillToSelfPos() {}

	virtual bool LoadConfig(const char *dir, std::string *err);
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return m_filename.c_str(); }

protected:
	std::string m_filename;

	short m_effect_target;
	short m_effect_count;
	SkillAttachEffectParam m_effect_list[SKILL_ATTACH_MAX_EFFECT];
};

// 法阵技 对敌人坐标 : 法阵技 对敌人坐标 ID从15001开始
class MonsterFaZhenSkillToEnemyPos : public SkillBase
{
public:
	MonsterFaZhenSkillToEnemyPos(std::string &filename) : SkillBase(SKILL_TYPE_ACTIVE, false, true), 
		m_filename(filename), m_fazhen_type(SKILL_FAZHEN_EFFECT_TYPE_INVALID), m_fazhen_interval(0), m_fazhen_lifetime(0), m_effect_now(false),
		m_effect_target(SKILL_ATTACH_EFFECT_TARGET_INVALID), m_effect_count(0)
	{
		for (int i = 0; i <= MAX_SKILL_LEVEL; i++)
		{
			m_attack_range_enemy_num_list[i] = SKILL_ATTACH_EFFECT_MAX_TARGET;
		}
	}
	virtual ~MonsterFaZhenSkillToEnemyPos() {}

	virtual bool LoadConfig(const char *dir, std::string *err);
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return m_filename.c_str(); }

protected:
	std::string m_filename;

	short m_fazhen_type;
	short m_fazhen_interval;
	int m_fazhen_lifetime;
	bool m_effect_now;

	short m_effect_target;
	short m_effect_count;
	SkillAttachEffectParam m_effect_list[SKILL_ATTACH_MAX_EFFECT];
};

// 法阵技 对自己坐标 : 法阵技 对自己坐标 ID从14001开始
class MonsterFaZhenSkillToSelfPos : public SkillBase
{
public:
	MonsterFaZhenSkillToSelfPos(std::string &filename) : SkillBase(SKILL_TYPE_ACTIVE, false, false), 
		m_filename(filename), m_fazhen_type(SKILL_FAZHEN_EFFECT_TYPE_INVALID), m_fazhen_interval(0), m_fazhen_lifetime(0), m_effect_now(false),
		m_effect_target(SKILL_ATTACH_EFFECT_TARGET_INVALID), m_effect_count(0)
	{
		for (int i = 0; i <= MAX_SKILL_LEVEL; i++)
		{
			m_attack_range_enemy_num_list[i] = SKILL_ATTACH_EFFECT_MAX_TARGET;
		}
	}
	virtual ~MonsterFaZhenSkillToSelfPos() {}

	virtual bool LoadConfig(const char *dir, std::string *err);
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return m_filename.c_str(); }

protected:
	std::string m_filename;

	short m_fazhen_type;
	short m_fazhen_interval;
	int m_fazhen_lifetime;
	bool m_effect_now;

	short m_effect_target;
	short m_effect_count;
	SkillAttachEffectParam m_effect_list[SKILL_ATTACH_MAX_EFFECT];
};

// 技能对敌人，效果对其他人
class MonsterSkillToEnemyEffectToOther : public SkillBase
{
public:
	MonsterSkillToEnemyEffectToOther(std::string &filename) : SkillBase(SKILL_TYPE_ACTIVE, true, true),
		m_filename(filename), m_effect_target(SKILL_ATTACH_EFFECT_TARGET_INVALID), m_effect_count(0) {}
	virtual ~MonsterSkillToEnemyEffectToOther() {}

	virtual bool LoadConfig(const char *dir, std::string *err);
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return m_filename.c_str(); }

protected:
	std::string m_filename;

	short m_effect_target;
	short m_effect_count;
	SkillAttachEffectParam m_effect_list[SKILL_ATTACH_MAX_EFFECT];
};

// 群攻技 对多个目标矩形范围 : 多矩形群攻 ID从18001开始
class MonsterRectRangeCommonSkillToEnemyPos : public SkillBase
{
public:
	enum DIR_TYPE
	{
		DIR_TYPE_SINGLE = 1,			// 单向
		DIR_TYPE_DOUBLE,				// 双向
		DIR_TYPE_CROSS,					// 十字
	};

	MonsterRectRangeCommonSkillToEnemyPos(std::string &filename) : SkillBase(SKILL_TYPE_ACTIVE, false, true), 
		m_filename(filename), m_rect_length(0), m_rect_width(0), m_dir_type(0), 
		m_effect_target(SKILL_ATTACH_EFFECT_TARGET_INVALID), m_effect_count(0)
	{
		for (int i = 0; i <= MAX_SKILL_LEVEL; i++)
		{
			m_attack_range_enemy_num_list[i] = SKILL_ATTACH_EFFECT_MAX_TARGET;
		}
	}
	virtual ~MonsterRectRangeCommonSkillToEnemyPos() {}

	virtual bool LoadConfig(const char *dir, std::string *err);
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return m_filename.c_str(); }

protected:
	bool IsInRange(Posi origin_pos, float dir, float distance, float width, Posi target_pos) const;

	std::string m_filename;

	short m_rect_length;
	short m_rect_width;
	short m_dir_type;

	short m_effect_target;
	short m_effect_count;
	SkillAttachEffectParam m_effect_list[SKILL_ATTACH_MAX_EFFECT];
};

// 群攻技 多区域 : 多区域 ID从17001开始
class MonsterMultiRangeCommonSkillToEnemyPos : public SkillBase
{
public:
	MonsterMultiRangeCommonSkillToEnemyPos(std::string &filename) : SkillBase(SKILL_TYPE_ACTIVE, false, false), 
		m_filename(filename), m_rand_zone_count(0), m_sub_zone_range(0), m_effect_target(SKILL_ATTACH_EFFECT_TARGET_INVALID), m_effect_count(0)
	{
		for (int i = 0; i <= MAX_SKILL_LEVEL; i++)
		{
			m_attack_range_enemy_num_list[i] = SKILL_ATTACH_EFFECT_MAX_TARGET;
		}
	}
	virtual ~MonsterMultiRangeCommonSkillToEnemyPos() {}

	virtual bool LoadConfig(const char *dir, std::string *err);
	virtual void Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param = 0) const;
	virtual const char * GetSkillName() { return m_filename.c_str(); }

protected:
	std::string m_filename;

	short m_rand_zone_count;						// 随机区域数量
	short m_sub_zone_range;							// 小区域范围

	short m_effect_target;
	short m_effect_count;
	SkillAttachEffectParam m_effect_list[SKILL_ATTACH_MAX_EFFECT];
};


#endif

