#ifndef _EFFECT_BIANSHEN_HPP_
#define _EFFECT_BIANSHEN_HPP_


#include <stdlib.h>

#include "effect/effectbase.hpp"

class Character;
class Role;

// 变身
class EffectBianshen : public EffectBase
{
public:
	EffectBianshen(): m_effect_time(0), m_bianshen_image(0), m_buff_type(EBT_BIANXING_FOOL){}
	EffectBianshen(ObjID deliverer, UInt16 skill, int effect_time, char product_level, char priduct_method = EffectBase::PRODUCT_METHOD_SKILL, unsigned char buff_type = EBT_BIANXING_FOOL)
	: EffectBase(deliverer, skill, priduct_method, 1), m_effect_time(effect_time), m_bianshen_image(0), m_buff_type(buff_type) {}
	virtual ~EffectBianshen(){}

	void SetImage(short bianshen_img) { m_bianshen_image = bianshen_img; }
	virtual int GetImage() { return m_bianshen_image;}

	void SetObjIdDes(unsigned short objid_des) { m_obj_id_des = objid_des; }
	virtual int GetObjIdDes() {		return m_obj_id_des;	}

	void SetModelSize(int model_size) { m_model_size = model_size; }
	int GetModelSize() { return m_model_size; }

	void SetExtra(int extra_info) { m_extra_info = extra_info; }

	virtual char GetEffectType() { return EFFECT_TYPE_BIANSHEN; }
	virtual void Effect(Character *cha, bool *die){}
	virtual void ReEffect(Character *cha, bool *die) {}
	virtual bool Update(unsigned long interval, Character *cha, bool *die);

	virtual bool IsPositiveEffect() const { return false; }

	virtual unsigned char GetBuffType() const { return m_buff_type; }

	virtual void SetLeftTimeMS(int effect_time) { if (effect_time >= 0) m_effect_time = effect_time; }
	virtual int GetLeftTimeMS() { return m_effect_time; }

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]);

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual bool Serialize(TLVSerializer &s) const;
	virtual bool Unserialize(TLVUnserializer &s);

protected:
	int	m_effect_time;
	int m_bianshen_image;
	unsigned char m_buff_type;
	int m_obj_id_des;	//变成他
	int m_model_size;	//模型大小
	int m_extra_info;
};


#endif


