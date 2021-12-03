#ifndef __DOUBLE_SIDE_FB_MANAGER_HPP__
#define __DOUBLE_SIDE_FB_MANAGER_HPP__

#include<map>

class CrossUser;

class DoubleSideFBManager
{
public:
	static DoubleSideFBManager & Instance()
	{
		static DoubleSideFBManager dsm;
		return dsm;
	}

	void RequireEnterFb(Role * role, int activity_type, bool is_cross, int &scene_key);
	void RequireLeaveFb(Role * role, int activity_type, bool is_cross);

	int GetRoleFbKey(Role * role);

private:
	DoubleSideFBManager();
	~DoubleSideFBManager();

	long long GetRoleKey(Role * role);
	void SendRoleToScene(Role *role, int activity_type, bool is_cross, int secen_key);

	typedef std::map<long long, int> RoleSeatMap;
	typedef std::map<long long, int>::iterator RoleSeatMapIt;
	RoleSeatMap m_role_seat_map;								// 角色所在房间

	typedef std::map<int, std::vector<int> > ActivityRoomMemberNumMap;
	typedef std::map<int, std::vector<int> >::iterator ActivityRoomMemberNumIt;
	ActivityRoomMemberNumMap m_activity_room_member_num_map;	// 活动 - 该活动中的房间-房间中的人数
};

#endif // __DOUBLE_SIDE_FB_MANAGER_HPP__
