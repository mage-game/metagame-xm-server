#include "sceneeffectbase.hpp"

#include "obj/otherobj/effectobj.h"

#include "scene/scene.h"

#include "protocal/msgfight.h"

SceneEffectBase::SceneEffectBase(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method) 
	: m_scene(scene), m_deliver_objid(deliver_objid), m_product_id(product_id), m_product_method(product_method)
{
	
}
	
SceneEffectBase::~SceneEffectBase()
{
	
}

int SceneEffectBase::GetFriendByArea(const Posi &pos, int range, ObjID *target_list, int max_num, bool check_alive)
{
	static ObjID obj_list[256] = {0};
	int count = m_scene->GetZoneMatrix()->GetObjByArea(pos, range, range, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

	int num = 0;
	for (int i = 0; num < max_num && i < count; ++i)
	{
		Obj *obj = m_scene->GetObj(obj_list[i]);
		if (NULL != obj && Obj::IsCharactor(obj->GetObjType()) && this->IsFriend(obj, check_alive))
		{
			target_list[num++] = obj_list[i];
		}
	}

	return num;
}

int SceneEffectBase::GetTargetByArea(const Posi &pos, int range, ObjID *target_list, int max_num)
{
	static ObjID obj_list[256] = {0};
	int count = m_scene->GetZoneMatrix()->GetObjByArea(pos, range, range, obj_list, sizeof(obj_list)/sizeof(obj_list[0]));

	int num = 0;
	for (int i = 0; num < max_num && i < count; ++i)
	{
		Obj *obj = m_scene->GetObj(obj_list[i]);
		if (NULL != obj && Obj::IsCharactor(obj->GetObjType()) && this->IsEnemy(obj))
		{
			target_list[num++] = obj_list[i];
		}
	}

	return num;
}

