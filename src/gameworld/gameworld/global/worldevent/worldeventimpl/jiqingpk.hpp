#ifndef __JI_QING_PK_HPP__
#define __JI_QING_PK_HPP__

class WorldEventManager;

#include "servercommon/worldeventdef.hpp"
#include "global/worldevent/worldevent.hpp"

class WorldEventJiqingPk : public WorldEvent
{
public:
	WorldEventJiqingPk(WorldEventManager *world_event_manager);
	virtual ~WorldEventJiqingPk();

	virtual void Init(const WorldEventParam &ev_param);
	virtual void GetInitParam(WorldEventParam *ev_param);

	virtual void Update(unsigned long interval, time_t now_second);
	
	virtual void OnHurtRoleShadow(Role *role, RoleShadow *role_shadow, int hurt);

	virtual void OnLogicStart();					
	virtual void OnLogicStop();

private:
	void ClearPKRole();
	void FlushPKRole();

	bool GetPkRoleInfo(int *cap, int *uid);

	struct PKRoleItem
	{
		PKRoleItem() { this->Reset(); }

		void Reset() { scene_id = 0; objid = INVALID_OBJ_ID; pkrole_id = 0; }
		bool IsVaild() { return INVALID_OBJ_ID != objid; }

		int scene_id;
		ObjID objid;
		int pkrole_id;
	};

	int m_pkrole_count;
	PKRoleItem m_pkrole_list[WORLD_EVENT_PKROLE_MAX];				// 存在的PK对象

	int m_flush_num;												// 当前刷的总数

	time_t m_last_check_alive_pkrole_timestamp;
};

#endif // __JI_QING_PK_HPP__



