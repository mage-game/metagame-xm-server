#ifndef __CROSS_ACTIVITY_MANAGER_HPP__
#define __CROSS_ACTIVITY_MANAGER_HPP__

#include "engineadapter.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/crossdef.hpp"
#include "crossactivity/impl/crossactivity1v1.hpp"
#include "crossactivity/impl/crossactivitycrossmultiuserchallenge.hpp"
#include "crossactivity/impl/crossactivitytuanzhan.hpp"
#include "crossactivity/impl/crossactivitypasture.hpp"
#include "crossactivity/impl/crossactivitycrossboss.hpp"
#include "crossactivity/impl/crossactivityhotspring.hpp"
#include "crossactivity/impl/crossactivityshuijing.hpp"
#include "crossactivity/impl/crossactivitycrossguildbattle.hpp"
#include "crossactivity/impl/crossactivitycrossliekunfb.hpp"
#include "impl/crossactivitycrossmizangboss.hpp"
#include "impl/crossactivitycrossyoumingboss.hpp"

class CrossServer;
class CrossUser;
class CrossActivity;
class CrossActivityXiuluoTower;
class CrossActivityTuanzhan;
class CrossActivityPasture;
class CrossActivityCrossBianJingZhiDi;

class CrossActivityManager
{
public:
	CrossActivityManager();
	~CrossActivityManager();

	void SetCrossServer(CrossServer *cross_server) { m_cross_server = cross_server; }
	CrossServer * GetCrossServer() { return m_cross_server; }

	void OnServerStart() {}
	void OnServerStop() {}

	void InitCrossActivity(const CrossActivityParam &cross_activity_param);
	void GetCrossActivityParam(CrossActivityParam *cross_activity_param);
	void Update(unsigned long interval, time_t now_second);
	void OnDayChange();
	void OnWeekChange();

	bool IsLoadFinish();

	void OnRegisterGameServer(NetID global_netid);
	void OnInternalGSDisconnnect(NetID global_netid, const UniqueServerID &unique_server_id);
	void OnCrossUserLogin(CrossUser *cross_user);
	void OnCrossUserLogout(CrossUser *cross_user);

	bool IsCrossActivityOpen(int cross_activity_type);
	bool IsCrossActivityClose(int cross_activity_type);

	CrossActivity * GetCrossActivity(int cross_activity_type);
	CrossActivity1V1 * GetCrossActivity1V1();
	CrossActivityCrossMultiuserChallenge * GetCrossActivityCrossMultiuserChallenge();
	CrossActivityTuanzhan * GetCrossActivityTuanzhan();
	CrossActivityShuijing * GetCrossActivityShuijing();
	CrossActivityGuildBattle * GetCrossActivityGuildBattle();
	int GetWaitSceneID(const UniqueUserID &unique_user_id, int cross_activity_type, Posi *pos);

	void OnForceToNextState(int cross_activity_type);

	bool CheckCanStartCross(int cross_activity_type, const UniqueUserID &unique_uesr_id, int merge_server_id);

	CrossActivityXiuluoTower * GetCrossActivityXiuluoTower();
	CrossActivityPasture * GetCrossActivityPasture();
	CrossActivityCrossBoss * GetCrossActivityCrossBoss();
	CrossActivityHS * GetCrossActivityHotSpring();
	CrossActivityCrossLieKunFb *GetCrossActivityCrossLieKunFb();
	CrossActivityCrossMizangBoss * GetCrossActivityCrossMizangBoss();
	CrossActivityCrossYouMingBoss * GetCrossActivityCrossYouMingBoss();
	CrossActivityCrossBianJingZhiDi * GetCrossActivityCrossBianJingZhiDi();

private:
	CrossServer *m_cross_server;

	CrossActivity *m_cross_activity_list[CROSS_ACTIVITY_TYPE_MAX];		// 跨服活动列表
};

#endif // __CROSS_ACTIVITY_MANAGER_HPP__

