#include <memory.h>
#include "obj.h"
#include "scene/scene.h"
#include "scene/map.h"

#include "servercommon/servercommon.h"

Obj::Obj(ObjType type):m_obj_type(type),
			m_obj_id(INVALID_OBJ_ID), 
			m_scene(0),
			m_single_mgr_index(-1),
			m_posf(0,0), 
			m_posi(0,0),
			m_dir(Dir_S),
			m_dir_distance(0),
			m_height(0), 
			m_obser_handle(-1),
			m_valid(true),
			m_fly_height(0)
{
	memset(m_name, 0, sizeof(m_name));
}

Obj::Obj(ObjType type, ObjID obj_id, const GameName _name):m_obj_type(type),
			m_obj_id(obj_id),
			m_scene(0),
			m_single_mgr_index(-1),
			m_posf(0,0), 
			m_posi(0,0),
			m_dir(Dir_S),
			m_dir_distance(0),
			m_height(0), 
			m_obser_handle(-1),
			m_valid(true)
{
	F_STRNCPY(m_name, _name, sizeof(GameName));
}

Obj::~Obj()
{

}

void Obj::GetName(GameName name)
{
	F_STRNCPY(name, m_name, sizeof(GameName));
}

void Obj::SetName(GameName name)
{
	F_STRNCPY(m_name, name, sizeof(GameName));
}

void Obj::OnEnterScene()
{
	if (NULL != m_scene) m_obser_handle = m_scene->GetZoneMatrix()->CreateObser(m_obj_id);
}

void Obj::OnLeaveScene()
{
	if (NULL != m_scene) m_scene->GetZoneMatrix()->DestroyObser(m_obser_handle);
}

bool Obj::IsCharactor(ObjType type)
{
	if (type < 0 || type >= OBJ_TYPE_COUNT) return false;

	static bool is_character[] = {false, true, true, false, false, false, false, false, false, true, true, false, true, false};
	
	UNSTD_STATIC_CHECK(sizeof(is_character) / sizeof(is_character[0]) == OBJ_TYPE_COUNT);

	return type < OBJ_TYPE_COUNT && is_character[(int)type];
}


bool Obj::IsFightCharactor(ObjType type)
{
	if (Obj::OBJ_TYPE_ROLE == type || Obj::OBJ_TYPE_MONSTER == type)
	{
		return true;
	}

	return false;
}
