
#include "objmanager.h"
#include "obj.h"

ObjManager::ObjManager()
{

}

ObjManager::~ObjManager()
{

}

bool ObjManager::Add(Obj *obj)
{
	ObjID obj_id = (ObjID)m_objlist.Insert(obj);
	obj->SetId(obj_id);
	return true;
}

bool ObjManager::Remove(ObjID obj_id)
{
	m_objlist.Erase(obj_id);
	return true;
}


