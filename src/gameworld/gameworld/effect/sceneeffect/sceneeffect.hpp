#ifndef __SCENE_EFFECT_HPP__
#define __SCENE_EFFECT_HPP__

#include "gameworld/gamedef.h"
#include "obj/obj.h"

#include "effect/sceneeffect/sceneeffectbase.hpp"
#include "skill/skilldef.hpp"

////////////////////////////////////////////////////// 法阵 ///////////////////////////////////////////////////////////////
class SceneEffectSpecialFaZhen : public SceneEffectBase
{
public:
	SceneEffectSpecialFaZhen(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method);
	virtual ~SceneEffectSpecialFaZhen();

	void SetFaZhenInfo(const Posi &pos, int range, int interval, int count, bool liveon_continue_skill, bool effect_now, int side = -1);
	void SetEffectParam(int effect_target, short effect_count, const SkillAttachEffectParam *effect_param_list);

	virtual char GetSceneEffectType() const { return SceneEffectBase::SCENE_EFFECT_TYPE_SPECIAL_FAZHEN; }
	virtual bool Effect(unsigned long interval, time_t now_second);

	void *operator new(size_t c);
	void operator delete(void *m);

private:
	void Attach();

	Posi m_pos;												// 作用位置
	int m_range;											// 作用范围
	int m_interval;											// 作用间隔
	int m_left_count;										// 作用剩余次数
	int m_past_count;										// 已经作用的次数
	bool m_liveon_continue_skill;							// 是否依赖持续施法
	
	static const int MAX_FAZHEN_EFFECT_COUNT = 2;

	short m_effect_target;									// 效果目标
	short m_effect_count;
	SkillAttachEffectParam m_effect_param_list[MAX_FAZHEN_EFFECT_COUNT];	// 效果 

	bool m_first;											// 标记第一次effect
	int m_effect_time;										// 已作用时间

	bool m_effect_now;										// 是否马上生效
	ObjID m_effect_obj_id;
	int m_side;												// 阵营
};

////////////////////////////////////////////////////// 纯伤 ///////////////////////////////////////////////////////////////
class SceneEffectHurt : public SceneEffectBase
{
public:
	static const int MAX_TARGET_COUNT = 64;

	SceneEffectHurt(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method);
	virtual ~SceneEffectHurt();

	void SetHurtInfo(long long injure, int fight_type);
	void AddTarget(ObjID obj_id);

	virtual char GetSceneEffectType() const { return SceneEffectBase::SCENE_EFFECT_TYPE_HURT; }
	virtual bool Effect(unsigned long interval, time_t now_second);

	void *operator new(size_t c);
	void operator delete(void *m);

private:
	struct HurtTarget
	{
		HurtTarget() : obj_id(INVALID_OBJ_ID), user_id(INVALID_USER_ID) {}

		ObjID obj_id;
		UserID user_id;
	};

	short m_target_count;											// 目标数量
	HurtTarget m_target_list[MAX_TARGET_COUNT];						// 目标列表
	
	long long m_injure;												// 伤害量
	int m_fight_type;												// 伤害类型
};

////////////////////////////////////////////////////// 塔防法阵 ///////////////////////////////////////////////////////////////
class SceneEffectTowerDefendFaZhen : public SceneEffectBase
{
public:
	SceneEffectTowerDefendFaZhen(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method);
	virtual ~SceneEffectTowerDefendFaZhen();

	void SetFaZhenInfo(const Posi &pos, int range, int interval, int count, int hurt);

	virtual char GetSceneEffectType() const { return SceneEffectBase::SCENE_EFFECT_TYPE_TOWERDEFEND_FAZHEN; }
	virtual bool Effect(unsigned long interval, time_t now_second);

	void *operator new(size_t c);
	void operator delete(void *m);

private:
	void Attach();

	Posi m_pos;												// 作用位置
	int m_range;											// 作用范围
	int m_interval;											// 作用间隔
	int m_left_count;										// 作用剩余次数
	int m_hurt;												// 单次伤害

	bool m_first;											// 标记第一次effect
	int m_effect_time;										// 已作用时间

	ObjID m_effect_obj_id;
};


////////////////////////////////////////////////////// 技能121法阵 ///////////////////////////////////////////////////////////////
class SceneEffectFaZhen121 : public SceneEffectBase
{
public:
	SceneEffectFaZhen121(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method);
	virtual ~SceneEffectFaZhen121();

	void SetFaZhenInfo(const Posi &pos, int range, int left_time, int hurt, int dec_movespeed_percent, int dec_movespeed_dur_ms, int xuanyun_dur_ms, int chenmo_dur_ms);

	virtual char GetSceneEffectType() const { return SceneEffectBase::SCENE_EFFECT_TYPE_SKILL_121; }
	virtual bool Effect(unsigned long interval, time_t now_second);

	void *operator new(size_t c);
	void operator delete(void *m);

private:
	void Attach();

	Posi m_pos;												// 作用位置
	int m_range;											// 作用范围
	int m_hurt;												// 单次伤害

	int m_dec_movespeed_percent;							// 减速百分比
	int m_dec_movespeed_dur_ms;								// 减速时间
	int m_xuanyun_dur_ms;									// 眩晕持续时间
	int m_chenmo_dur_ms;									// 沉默时间

	bool m_first;											// 标记第一次effect
	int m_effect_time;										// 已作用时间 ms
	int m_left_time;										// 剩余时间 ms

	ObjID m_effect_obj_id;
};

////////////////////////////////////////////////////// 技能421法阵 ///////////////////////////////////////////////////////////////
class SceneEffectFaZhen421 : public SceneEffectBase
{
public:
	SceneEffectFaZhen421(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method);
	virtual ~SceneEffectFaZhen421();

	void SetFaZhenInfo(const Posi &pos, int range, int left_time, int hurt, int dec_movespeed_percent, int dec_movespeed_dur_ms, int rand_pos_count, int rand_range);

	virtual char GetSceneEffectType() const { return SceneEffectBase::SCENE_EFFECT_TYPE_SKILL_421; }
	virtual bool Effect(unsigned long interval, time_t now_second);

	void *operator new(size_t c);
	void operator delete(void *m);

private:
	void Attach(bool is_first);

	Posi m_pos;												// 作用位置
	int m_range;											// 作用范围
	int m_hurt;												// 单次伤害

	int m_dec_movespeed_percent;							// 减速百分比
	int m_dec_movespeed_dur_ms;								// 减速时间

	bool m_first;											// 标记第一次effect
	int m_effect_time;										// 已作用时间 ms
	int m_left_time;										// 剩余时间 ms

	int m_rand_pos_count;									// 随机点数量
	int m_rand_range;										// 随机点伤害范围

	ObjID m_effect_obj_id;
};


////////////////////////////////////////////////////// 客户端特效，在服务端是没有实际作用效果的 ///////////////////////////////////////////////////////////////
class SceneEffectClient : public SceneEffectBase
{
public:
	SceneEffectClient(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method);
	virtual ~SceneEffectClient();

	void SetEffectParam(const Posi &pos, const Posi &src_pos, unsigned int end_time, int param1 = 0, int param2 = 0);

	virtual char GetSceneEffectType() const { return SceneEffectBase::SCENE_EFFECT_TYPE_CLIENT; }
	virtual bool Effect(unsigned long interval, time_t now_second);
	
	virtual void DelEffectObj();

	void *operator new(size_t c);
	void operator delete(void *m);

private:
	void Attach();

	bool m_first;											// 标记第一次effect
	Posi m_pos;												// 作用位置
	Posi m_src_pos;
	ObjID m_effect_obj_id;
	unsigned int m_end_time;

	int m_param1;
	int m_param2;
};

#endif // __SCENE_EFFECT_HPP__

