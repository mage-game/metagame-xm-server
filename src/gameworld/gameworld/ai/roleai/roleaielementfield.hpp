#ifndef __ROLEAI_ELEMENT_FIELD_HPP__
#define __ROLEAI_ELEMENT_FIELD_HPP__

#include "roleaibase.hpp"

// ÔªËØÕ½³¡
class RoleAIElementField : public RoleAIBase
{
public:
	RoleAIElementField(Character *me);
	virtual ~RoleAIElementField();

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

	virtual void OnEnterScene();
	virtual void OnLeaveScene();
	virtual void OnAOILeave(ObjID obj_id);

	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp);

	void SetSide(int side) { m_side = side; }
	short GetSide() { return m_side; }

protected:

	virtual bool IsEnemy(Obj *obj);
	bool FindEnemy(unsigned long interval);
	void GoBack();

	short m_side;

	UInt32 m_aoi_handle;
};

#endif

