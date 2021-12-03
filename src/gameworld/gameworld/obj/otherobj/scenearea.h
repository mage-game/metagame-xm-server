
#ifndef SCENEAREA_H
#define SCENEAREA_H

#include "obj/obj.h"

class SceneArea : public Obj
{
public:
	SceneArea();
	virtual ~SceneArea();

	void Init(const Posi &pos, Axis aoi_range);

	virtual void OnEnterScene();
	virtual void OnLeaveScene();
	virtual void OnAOIEnter(ObjID obj_id);
	virtual void OnAOILeave(ObjID obj_id);

	void *			operator new(size_t c);
	void			operator delete(void *m);
protected:
	Axis			m_range;
	UInt32			m_aoi_handle;
};


#endif



