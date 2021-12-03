#ifndef __CROSS_ACTIVITY_TEAM_FB_HPP__
#define __CROSS_ACTIVITY_TEAM_FB_HPP__

#include "crossactivity/crossactivity.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"

static const int TEAM_FB_LAYER_LIMIT_MAX = 30;							// 限制关卡类最大关卡配置数

class CrossActivityTeamFB : public CrossActivity
{
public:
	CrossActivityTeamFB(CrossActivityManager *cross_activity_manager);
	~CrossActivityTeamFB();

	virtual void Init(const CrossActivityData &data);
	virtual void OnCrossUserLogin(CrossUser *cross_user);
	virtual bool CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id = 0);

	void OnCrossTeamFBRoomOption(int option2_type, int layer, int room);
	void OnCrossTeamFBOption(int option_type, CrossTeamFBUserInfo info, int param = 0, int param2 = 0, int param3 = 0);

	void NoticeUserCanEnterCross(long long uuid);
	void NoticeCrossTeamFBRoomInfoChange(int layer, int room, long long uuid, int opera_type, bool is_send_all_user = true);
	void GetCrossTeamFBRoomListInfo(NetID global_netid, const UniqueUserID &unique_user_id, int layer);
	void GetCrossTeamFBRoomInfo(NetID global_netid, const UniqueUserID &unique_user_id, int layer, int room);

private:
	virtual void OnChangeToNextStatus();

	void AddCrossTeamFBUserInfo(CrossTeamFBUserInfo info);
	void DelCrossTeamFBUserInfo(long long uuid);
	bool IsNewServer(const UniqueServerID &unique_server_id);
	bool IsNewServerByIndex(int day_index);

	typedef std::map<long long, CrossTeamFBUserInfo> CrossTeamFBUserInfoMap;
	typedef std::map<long long, CrossTeamFBUserInfo>::iterator CrossTeamFBUserInfoMapIt;
	CrossTeamFBUserInfoMap m_user_info;

	typedef std::map<int, CrossTeamFBRoomInfo> CrossTeamFBRoomInfoMap;
	typedef std::map<int, CrossTeamFBRoomInfo>::iterator CrossTeamFBRoomInfoMapIt;
	int m_room_no_max[TEAM_FB_LAYER_LIMIT_MAX];
	CrossTeamFBRoomInfoMap m_room_info[TEAM_FB_LAYER_LIMIT_MAX];
};

#endif // __CROSS_ACTIVITY_TEAM_FB_HPP__


