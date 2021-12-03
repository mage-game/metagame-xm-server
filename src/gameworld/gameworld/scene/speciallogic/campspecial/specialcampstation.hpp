#ifndef __SPECIAL_CAMP_STATION_HPP__
#define __SPECIAL_CAMP_STATION_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"


class SpecialCampStation : public SpecialLogic
{
public:
	SpecialCampStation(Scene *scene, int camp_type);
	virtual ~SpecialCampStation();

	virtual bool CanDestroy() { return false; }
	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);
	virtual bool SpecialStopGahterOnAttack(Role *role) { return false; }

protected:
	int m_camp_type;
};

#endif // __SPECIAL_CAMP_STATION_HPP__
