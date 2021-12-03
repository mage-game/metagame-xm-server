#include "scenearea.h"
#include "scene/scene.h"
#include "obj/character/role.h"


#include "other/event/eventhandler.hpp"

#include "config/logicconfigmanager.hpp"

#include "scene/activityshadow/activityshadow.hpp"
#include "protocal/msgactivity.hpp"

SceneArea::SceneArea():Obj(OBJ_TYPE_SCENEAREA), m_range(4), m_aoi_handle(-1)
{

}

SceneArea::~SceneArea()
{

}

void SceneArea::Init(const Posi &pos, Axis aoi_range)
{
	m_posi = pos;
	m_range = aoi_range;
}

void SceneArea::OnEnterScene()
{
	if (NULL == m_scene) return; 

	m_aoi_handle = m_scene->GetZoneMatrix()->CreateRectAOI(m_obj_id, m_posi, Posi(m_range, m_range), 
		Posi(m_range + 2, m_range + 2));
}

void SceneArea::OnLeaveScene()
{
	if (NULL == m_scene) return; 

	m_scene->GetZoneMatrix()->DestroyAOI(m_aoi_handle);
}

void SceneArea::OnAOIEnter(ObjID obj_id)
{
	if (NULL == m_scene) return; 

	/*Obj *obj = m_scene->GetObj(obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;

		
	}*/
}

void SceneArea::OnAOILeave(ObjID obj_id)
{
	/*if (NULL == m_scene) return; 

	Obj *obj = m_scene->GetObj(obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		
	}*/
}



