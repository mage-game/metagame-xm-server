#ifndef _MONSTER_HALO_HPP_
#define _MONSTER_HALO_HPP_

#include "character.h"

class Monster;

class MonsterHalo : public Character
{
public:
	MonsterHalo(ObjID owner_obj_id, int owner_key, UInt16 product_id, char product_method);
	virtual ~MonsterHalo();

	void Init(Attribute hp);

	virtual	void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	void SetHaloInfo(int range, int interval, int count);
	void SetEffectParam(int effect_target, const SkillAttachEffectParam &effect_param);

	ObjID GetOwnerObjID() { return m_owner_obj_id; }
	int GetEffectType() { return m_effect_param.effect_type;}

	virtual bool CanBeCaptureBySkill() { return false; }

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	Monster * GetOwnerMonster();

	void Attach(const Posi &pos);

	UInt16 m_product_id;									// ������Ч���ļ���id ����buff����
	char m_product_method;									// EffectBase::PRODUCT_METHOD

	int m_range;											// ���÷�Χ
	int m_interval;											// ���ü��
	int m_left_count;										// ����ʣ�����
	int m_past_count;										// �Ѿ����õĴ���

	int m_effect_target;									// Ч��Ŀ��
	SkillAttachEffectParam m_effect_param;					// Ч�� 

	bool m_first;											// ��ǵ�һ��effect
	int m_effect_time;										// ������ʱ��

	ObjID m_owner_obj_id;									// ������
	int m_owner_key;										
};

#endif
