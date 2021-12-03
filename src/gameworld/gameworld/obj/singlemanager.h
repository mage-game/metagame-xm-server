
#ifndef SINGLEMANAGER_H
#define SINGLEMANAGER_H

#include "gamedef.h"

class Obj;
class Scene;

class SingleManager
{
public:
	SingleManager();
	virtual ~SingleManager();

	void Init();
	void Release();

	bool Add( Obj *obj);
	bool Remove(Obj *obj);

	UInt32 Size()const { return m_size; }

	void SetScene(Scene* scene) { m_scene = scene; }

	Scene *GetScene() { return m_scene; }

	Obj *GetObjByIndex(UInt32 index)
	{
		if (index < m_size)
		{
			return m_obj[index];
		}
		else
		{
			return 0;
		}
	}

private:
	void Resize(UInt32 size);

protected:
	Obj		**m_obj;
	UInt32	m_size;
	UInt32	m_max_size;

	Scene	*m_scene;
};

#endif


