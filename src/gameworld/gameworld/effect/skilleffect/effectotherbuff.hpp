#ifndef __EFFECTMULTIFUNBUFF_HPP__
#define __EFFECTMULTIFUNBUFF_HPP__

#include <stdlib.h>

#include "effect/effectbase.hpp"
#include "obj/character/attribute.hpp"

// ����BUFF
class EffectOtherBuff : public EffectBase
{
public:
	EffectOtherBuff();
	EffectOtherBuff(ObjID deliverer, UInt16 product_id, int effect_time, char product_level, char product_method = EffectBase::PRODUCT_METHOD_SKILL);
	virtual ~EffectOtherBuff();

	virtual char GetEffectType() { return EFEECT_TYPE_OTHER; }
	virtual void Effect(Character *cha, bool *die);
	virtual void ReEffect(Character *cha, bool *die);
	virtual bool Update(unsigned long interval, Character *cha, bool *die);
	void IntervalEffect(Character *cha);
	virtual void ClearEffect(Character *cha);

	void ReCalcBaseAttr(CharIntAttrs &base_add);

	virtual void SetLeftTimeMS(int effect_time) { if (effect_time >= 0) m_effect_time = effect_time; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void Merge(EffectBase *new_effect, Character *cha);

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

	// �ɼ��ٶ�
	void IncGatherSpeedPer(short per) { m_param1 = per; }
	short GetIncGatherSpeedPer() { return m_param1; }

	// �Խ������˺�
	void IncToBuildingHurtPer(short per) { m_param1 = per; }
	short GetIncToBuildingHurtPer() { return m_param1; }

	// ������Ѫ����ı�ֵ
	void SetContinueHp(int interval, unsigned short hp) {m_interval = interval; m_param1 = hp; }

	// ����mp����ı�ֵ
	void SetContinueMp(int interval, unsigned short mp) {m_interval = interval; m_param1 = mp; }

	// ���Ӵ����ȼ�
	void IncPutOnLevel(short level) { m_param1 = level; }
	short GetIncPutOnLevel() { return m_param1; }

	// �������ӳɰٷֱ�
	void IncExtraExpPer(short per) { m_param1 = per; }
	short GetIncExtraExpPer() {return m_param1; }

	// ��������ʱ�����˺�
	void IncHurtOnAtk(unsigned short per) { m_param1 = per; }
	unsigned short GetIncHurtOnAtk() { return m_param1; }

	// ����ս ��һ�ι����������ɶԷ���Ѫ���ٷֱ�
	void SetGczNextHpPerHurt(short per) { m_param1 = per;}
	short GettGczNextHpPerHurt() { return m_param1; }

	// ����ս ��һ�ι����Է��ķ�������20%������10S
	void SetGczNextDecFYPer(short per, short time) { m_param1 = per; m_param2 = time; }
	void GetGczNextDecFYPer(short *per, short *time) { if(NULL != per && NULL != time) { *per = m_param1; *time = m_param2; }}

	// ����ս ��һ�ι��������ʹֱ�Ӽ��ٵ���3%����Ѫ
	void SetGczReduceBossHpPer(short per) { m_param1 = per;}
	short GetGczReduceBossHpPer() { return m_param1; }

	// ����ս ������һ�ι�����ʹ�ܵ���������Ҽ�����������ܵ�2S�ĳ�ĬЧ��
	void SetGczNextCMTime(short time) { m_param1 = time; }
	short GetGczNextCMTime() { return m_param1; }

	// ����ս ��һ�ι�����ʹ�ܵ���������Ҽ�����������ܵ�2S�Ķ���Ч��
	void SetGczNextDSTime(short time) { m_param1 = time; }
	short GetGczNextDSTime() { return m_param1; }

	//  ����ս ��һ�ι�����ʹ�ܵ�����������ܵ�2S��ѣ��
	void SetGczNextXYTime(short time) { m_param1 = time; }
	short GetGczNextXYTime() { return m_param1; }

	// ���������������
	void SetRoleBaseAttr(int attr_type, int value) { m_param1 = static_cast<int>(attr_type); m_param2 = static_cast<int>(value); }

protected:
	void Attach(Character *cha, bool is_attach);

	int m_effect_time;												// ����ʱ��

	int m_interval_is_do_now;										// �������Ƿ�����ִ�У�����x�����ִ��
	int m_interval;													// ������ʱ��
	int m_interval_elapse_time;

	bool m_has_time_limit;

	unsigned short m_param1;
	unsigned short m_param2;
	unsigned short m_param3;
	unsigned short m_param4;
};

#endif


