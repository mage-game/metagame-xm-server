#ifndef __ROLE_BOSS_MANAGER_H__
#define __ROLE_BOSS_MANAGER_H__

class Role;
class Monster;
class RoleShadow;

#include "gamedef.h"
#include "rolebossconfig.h"

class RoleBossManager
{
public:
	static RoleBossManager & Instance();
	void OnRoleLogin(Role *role);
	void Update(unsigned long interval, time_t now_second);
	void OnKillRoleShadow(Role *role, RoleShadow *role_shadow);

private:
	RoleBossManager();
	~RoleBossManager();
	void SendInfo(Role *role, int notify_reason = 0);

	time_t m_next_refresh_timestamp;

	struct RoleShadowInfo
	{
		RoleShadowInfo() { this->Reset(); }

		void Reset() 
		{
			scene_id = 0;
			pos_x = 0;
			pos_y = 0;
			obj_id = INVALID_OBJ_ID;
			role_id = 0;
		}
		
		int scene_id;
		int pos_x;
		int pos_y;
		ObjID obj_id;
		int role_id;
	};

	RoleShadowInfo m_role_shadow_info_list[ROLE_BOSS_MAX_COUNT];
};

#endif

