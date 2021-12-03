#ifndef __EFFECTOBJ_H__
#define __EFFECTOBJ_H__

#include "obj/obj.h"

class Scene;

class EffectObj : public Obj
{
public:
	EffectObj();
	virtual ~EffectObj();

	void Init(UInt16 product_id, char product_method, time_t disappear_time, int param1 = 0, int param2 = 0, const Posi &src_pos = Posi(0, 0));

	UInt16 GetProductId() const { return m_product_id; }
	char GetProductMethod() const { return m_product_method; }
	time_t GetBirthTime() const { return m_birth_time; }
	time_t GetDisapperTime() const { return m_disappear_time; }
	int GetParam1() const { return m_param1; }
	int GetParam2() const { return m_param2; }
	const Posi &GetSrcPos() const { return m_src_pos; }

	virtual void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	static ObjID CreateEffectObject(Scene *scene, const Posi &pos, UInt16 product_id, char product_method, time_t disappear_time = 0, int param1 = 0, int param2 = 0, const Posi &src_pos = Posi(0, 0));
	static void RemoveEffectObject(Scene *scene, ObjID obj_id);
	
	void * operator new(size_t c);
	void operator delete(void *m);

private:
	UInt16 m_product_id;			// 产生此效果物的id
	char m_product_method;			// 产生此效果物的原因 EffectBase::PRODUCT_METHOD
	time_t m_birth_time;			// 出生时间
	time_t m_disappear_time;		// 消失时间

	int m_param1;
	int m_param2;
	Posi m_src_pos;
};

#endif	// __EFFECTOBJ_H__

