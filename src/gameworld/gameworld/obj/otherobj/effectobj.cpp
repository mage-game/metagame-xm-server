#include "effectobj.h"
#include "scene/scene.h"

#include "engineadapter.h"

EffectObj::EffectObj() : Obj(OBJ_TYPE_EFFECT), m_product_id(0), m_product_method(0), m_birth_time(0), m_disappear_time(0), m_param1(0), m_param2(0), m_src_pos(0, 0)
{

}

EffectObj::~EffectObj()
{

}

void EffectObj::Init(UInt16 product_id, char product_method, time_t disappear_time, int param1, int param2, const Posi &src_pos)
{	
	m_product_id = product_id;
	m_product_method = product_method;
	m_birth_time = EngineAdapter::Instance().Time();
	m_disappear_time = disappear_time;
	m_param1 = param1;
	m_param2 = param2;
	m_src_pos = src_pos;
}

void EffectObj::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	if (0 != m_disappear_time && now_second >= m_disappear_time)
	{
		m_scene->DeleteObj(m_obj_id);
	}
}

ObjID EffectObj::CreateEffectObject(Scene *scene, const Posi &pos, UInt16 product_id, char product_method, time_t disappear_time, int param1, int param2, const Posi &src_pos)
{
	EffectObj *effect_obj = new EffectObj();
	effect_obj->SetPos(pos);
	effect_obj->Init(product_id, product_method, disappear_time, param1, param2, src_pos);

	scene->AddObj(effect_obj);

	return effect_obj->GetId();
}

void EffectObj::RemoveEffectObject(Scene *scene, ObjID obj_id)
{
	Obj *obj = scene->GetObj(obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_EFFECT == obj->GetObjType())
	{
		scene->DeleteObj(obj->GetId());
	}
}

