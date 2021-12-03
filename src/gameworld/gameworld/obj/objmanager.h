
#ifndef OBJMANAGER_H
#define OBJMANAGER_H

#include "common/raobjlist.h"
#include "gamedef.h"


class Obj;

class ObjManager
{
	typedef RAObjList<Obj *> ObjList;
public:
	typedef ObjList::Iterator ObjIterator;
	ObjManager();
	~ObjManager();

	bool Add(Obj *obj);
	bool Remove(ObjID obj_id);
	Obj *GetObj(ObjID obj_id)
	{
		return m_objlist[obj_id];
	}

	bool Exist(ObjID obj_id)
	{
		return m_objlist.Exist(obj_id);
	}

	ObjIterator FirstObj()
	{
		return m_objlist.Beg();
	}

	ObjIterator LastObj()
	{
		return m_objlist.End();
	}

	void Clear()
	{
		m_objlist.Clear();
	}

	unsigned int Size() const { return m_objlist.Size(); }

protected:
	
	ObjList m_objlist;
};


#endif



