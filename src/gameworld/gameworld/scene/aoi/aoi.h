#ifndef __AOI_H__
#define __AOI_H__

#include "gamedef.h"

class AOI
{
public:
	AOI() : obj_id(INVALID_OBJ_ID), is_circle(false), centre(0, 0), enter_radius(0), leave_radius(0) {}
	AOI(ObjID _obj_id, bool circle, const Posi &_centre, Posi _enter_radius, Posi _leave_radius)
			: obj_id(_obj_id), is_circle(circle), centre(_centre), enter_radius(_enter_radius), leave_radius(_leave_radius) {}
	~AOI() {}


	ObjID obj_id;
	bool is_circle;
	Posi centre;
	Posi enter_radius;
	Posi leave_radius;

	bool IsIn(const Posi &pos)
	{
		if (is_circle)
		{
			Posi delta_pos = pos - centre;
			return (delta_pos.x * delta_pos.x + delta_pos.y * delta_pos.y) < (enter_radius.x * enter_radius.x);
		}
		else
		{
			if (pos.x >= centre.x - enter_radius.x && pos.x < centre.x + enter_radius.x
				&& pos.y >= centre.y - enter_radius.y && pos.y < centre.y + enter_radius.y)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
	}

	bool IsOut(const Posi &pos)
	{
		if (is_circle)
		{
			Posi delta_pos = pos - centre;
			return (delta_pos.x * delta_pos.x + delta_pos.y * delta_pos.y) >= (enter_radius.x * enter_radius.x);
		}
		else
		{
			if (pos.x < centre.x - leave_radius.x || pos.x >= centre.x + leave_radius.x
				|| pos.y < centre.y - leave_radius.y || pos.y >= centre.y + leave_radius.y)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
	}

	bool IsNotifyMsg(const Posi &pos)
	{
		if (is_circle)
		{
			Posi delta_pos = pos - centre;
			return (delta_pos.x * delta_pos.x + delta_pos.y * delta_pos.y) < (leave_radius.x * leave_radius.x);
		}
		else
		{
			if (pos.x >= centre.x - leave_radius.x && pos.x < centre.x + leave_radius.x
				&& pos.y >= centre.y - leave_radius.y && pos.y < centre.y + leave_radius.y)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
};

#endif



