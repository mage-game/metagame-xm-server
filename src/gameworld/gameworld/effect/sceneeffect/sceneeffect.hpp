#ifndef __SCENE_EFFECT_HPP__
#define __SCENE_EFFECT_HPP__

#include "gameworld/gamedef.h"
#include "obj/obj.h"

#include "effect/sceneeffect/sceneeffectbase.hpp"
#include "skill/skilldef.hpp"

////////////////////////////////////////////////////// ���� ///////////////////////////////////////////////////////////////
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

	Posi m_pos;												// ����λ��
	int m_range;											// ���÷�Χ
	int m_interval;											// ���ü��
	int m_left_count;										// ����ʣ�����
	int m_past_count;										// �Ѿ����õĴ���
	bool m_liveon_continue_skill;							// �Ƿ���������ʩ��
	
	static const int MAX_FAZHEN_EFFECT_COUNT = 2;

	short m_effect_target;									// Ч��Ŀ��
	short m_effect_count;
	SkillAttachEffectParam m_effect_param_list[MAX_FAZHEN_EFFECT_COUNT];	// Ч�� 

	bool m_first;											// ��ǵ�һ��effect
	int m_effect_time;										// ������ʱ��

	bool m_effect_now;										// �Ƿ�������Ч
	ObjID m_effect_obj_id;
	int m_side;												// ��Ӫ
};

////////////////////////////////////////////////////// ���� ///////////////////////////////////////////////////////////////
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

	short m_target_count;											// Ŀ������
	HurtTarget m_target_list[MAX_TARGET_COUNT];						// Ŀ���б�
	
	long long m_injure;												// �˺���
	int m_fight_type;												// �˺�����
};

////////////////////////////////////////////////////// �������� ///////////////////////////////////////////////////////////////
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

	Posi m_pos;												// ����λ��
	int m_range;											// ���÷�Χ
	int m_interval;											// ���ü��
	int m_left_count;										// ����ʣ�����
	int m_hurt;												// �����˺�

	bool m_first;											// ��ǵ�һ��effect
	int m_effect_time;										// ������ʱ��

	ObjID m_effect_obj_id;
};


////////////////////////////////////////////////////// ����121���� ///////////////////////////////////////////////////////////////
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

	Posi m_pos;												// ����λ��
	int m_range;											// ���÷�Χ
	int m_hurt;												// �����˺�

	int m_dec_movespeed_percent;							// ���ٰٷֱ�
	int m_dec_movespeed_dur_ms;								// ����ʱ��
	int m_xuanyun_dur_ms;									// ѣ�γ���ʱ��
	int m_chenmo_dur_ms;									// ��Ĭʱ��

	bool m_first;											// ��ǵ�һ��effect
	int m_effect_time;										// ������ʱ�� ms
	int m_left_time;										// ʣ��ʱ�� ms

	ObjID m_effect_obj_id;
};

////////////////////////////////////////////////////// ����421���� ///////////////////////////////////////////////////////////////
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

	Posi m_pos;												// ����λ��
	int m_range;											// ���÷�Χ
	int m_hurt;												// �����˺�

	int m_dec_movespeed_percent;							// ���ٰٷֱ�
	int m_dec_movespeed_dur_ms;								// ����ʱ��

	bool m_first;											// ��ǵ�һ��effect
	int m_effect_time;										// ������ʱ�� ms
	int m_left_time;										// ʣ��ʱ�� ms

	int m_rand_pos_count;									// ���������
	int m_rand_range;										// ������˺���Χ

	ObjID m_effect_obj_id;
};


////////////////////////////////////////////////////// �ͻ�����Ч���ڷ������û��ʵ������Ч���� ///////////////////////////////////////////////////////////////
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

	bool m_first;											// ��ǵ�һ��effect
	Posi m_pos;												// ����λ��
	Posi m_src_pos;
	ObjID m_effect_obj_id;
	unsigned int m_end_time;

	int m_param1;
	int m_param2;
};

#endif // __SCENE_EFFECT_HPP__

