#ifndef __COMMONSKILLMONSTER_HPP__
#define __COMMONSKILLMONSTER_HPP__

#include "skill/skilldef.hpp"
#include "skill/skillbase.hpp"

#include <string>

// �������ͨ�������� ����Ե��� : ����Ե��� ID�� 10001 ��ʼ
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

// �������ͨȺ���������� Ⱥ��Ե������� : Ⱥ��Ե������� ID�� 11001 ��ʼ
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
	2 С��
	3 ȫ������
	4 ��Ҽ������
	5 ȫ������
	6 ����
	*/
	short m_effect_target;
	short m_effect_count;
	SkillAttachEffectParam m_effect_list[SKILL_ATTACH_MAX_EFFECT];
};

// ����ĵ��弼�� ������Լ� : ������Լ� ID�� 12001 ��ʼ   �磺BOSS����
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

// �����Ⱥ�弼�� Ⱥ����Լ����� : Ⱥ����Լ����� ID�� 13001 ��ʼ
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

// ���� �Ե������� : ���� �Ե������� ID��15001��ʼ
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

// ���� ���Լ����� : ���� ���Լ����� ID��14001��ʼ
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

// ���ܶԵ��ˣ�Ч����������
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

// Ⱥ���� �Զ��Ŀ����η�Χ : �����Ⱥ�� ID��18001��ʼ
class MonsterRectRangeCommonSkillToEnemyPos : public SkillBase
{
public:
	enum DIR_TYPE
	{
		DIR_TYPE_SINGLE = 1,			// ����
		DIR_TYPE_DOUBLE,				// ˫��
		DIR_TYPE_CROSS,					// ʮ��
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

// Ⱥ���� ������ : ������ ID��17001��ʼ
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

	short m_rand_zone_count;						// �����������
	short m_sub_zone_range;							// С����Χ

	short m_effect_target;
	short m_effect_count;
	SkillAttachEffectParam m_effect_list[SKILL_ATTACH_MAX_EFFECT];
};


#endif

