/*************************************************************************************************
FileName: aibase.hpp
Author: dreamwang
Description: AI行为 接口规范
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/11/10/ 13:37:05     1.0           创建
************************************************************************************************/

#ifndef __AIBASE_HPP__
#define __AIBASE_HPP__

#include "gamedef.h"

class Obj;
class Monster;
class Character;
class RoleShadow;

class AIBase
{
	friend class Monster;
	friend class RoleShadow;

public:
	AIBase(Character *me) : m_me(me) {}
	virtual ~AIBase() {}

	virtual void OnEnterScene() = 0;
	virtual void OnLeaveScene() = 0;
	virtual void OnAOIEnter(ObjID obj_id) = 0;
	virtual void OnAOILeave(ObjID obj_id) = 0;

	virtual void OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp) = 0;

	virtual void AI(unsigned long interval) = 0;
	virtual void Update(unsigned long interval, unsigned int now_second) = 0;

protected:
	virtual bool IsEnemy(Obj *obj) = 0;
	virtual bool IsFriend(Obj *obj, bool check_alive = true) = 0;

	Character *m_me;
};

#endif  // __AIBASE_HPP__

